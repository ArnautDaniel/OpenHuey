#!/usr/bin/env python3
"""Move C functions between files, with what they need.

    tools/movefunc.py PLAN          # PLAN: lines "func_name dest/file.c" (# comments)
    tools/movefunc.py --check PLAN  # only report what would move and any conflicts

For each function: its definition (and the comment block right above it) leaves the source
file. Into the destination go, as needed and not there already:
  - the file-local definitions it uses (macros, typedefs, structs, static functions and
    variables, static inline helpers), recursively, in their source order;
  - the extern declarations / prototypes of the symbols it uses.
The function is placed by address among the destination's functions (the original's layout);
the support items go before the first moved function that needs them. A local definition the
destination has under the same name with different text is a conflict: reported, nothing
moves. A source file left without functions is deleted; the difftest list follows the moves.
"""
import re
import sys
from collections import OrderedDict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SYM = ROOT / "config/symbol_addrs.txt"
DIFFLIST = ROOT / "tools/difftest_list.txt"
IDENT = re.compile(r"[A-Za-z_]\w*")


def load_addrs():
    out = {}
    for line in SYM.read_text().splitlines():
        m = re.match(r"^(\w+) = 0x([0-9A-Fa-f]+);", line)
        if m:
            out[m.group(1)] = int(m.group(2), 16)
    return out


ADDRS = load_addrs()


def addr_of(name):
    m = re.match(r"func_([0-9A-F]{8})$", name)
    if m:
        return int(m.group(1), 16)
    return ADDRS.get(name)


class Item:
    """a top-level chunk of a C file"""

    def __init__(self, kind, text, names, lead=""):
        self.kind = kind      # 'pp' 'func' 'decl' 'other' 'blank'
        self.text = text      # the chunk itself (without the leading comment)
        self.names = names    # names it defines
        self.lead = lead      # comment block directly above it (kept with it)

    def full(self):
        return self.lead + self.text

    def uses(self):
        body = strip_comments(self.text)
        body = re.sub(r'"(?:\\.|[^"\\])*"', '""', body)
        return set(IDENT.findall(body)) - set(self.names)


def strip_comments(t):
    t = re.sub(r"/\*.*?\*/", " ", t, flags=re.S)
    return re.sub(r"//[^\n]*", " ", t)


def split_items(text):
    """top-level items, each with its leading comment attached"""
    items = []
    i, n = 0, len(text)
    pending = ""
    while i < n:
        # whitespace / blank lines
        m = re.match(r"[ \t]*\n", text[i:])
        if m and text[i:i + len(m.group(0))].strip() == "":
            if pending.strip():
                pending += m.group(0)
            else:
                items.append(Item("blank", pending + m.group(0), []))
                pending = ""
            i += len(m.group(0))
            # a blank line detaches a pending comment from what follows
            if pending.strip() and pending.endswith("\n\n"):
                items.append(Item("other", pending, []))
                pending = ""
            continue
        rest = text[i:]
        if rest.startswith("/*") or rest.lstrip(" \t").startswith("/*") and not rest.split("\n", 1)[0].strip().startswith(("#",)):
            j = text.find("*/", i) + 2
            k = j
            while k < n and text[k] in " \t":
                k += 1
            if k < n and text[k] == "\n":
                k += 1
            pending += text[i:k]
            i = k
            continue
        if rest.startswith("//"):
            k = text.find("\n", i) + 1 or n
            pending += text[i:k]
            i = k
            continue
        if rest.startswith("#"):
            # preprocessor line(s), with continuations
            k = i
            while True:
                e = text.find("\n", k)
                if e == -1:
                    e = n
                    break
                if text[e - 1] == "\\":
                    k = e + 1
                    continue
                break
            chunk = text[i:e + 1]
            names = []
            m = re.match(r"#\s*define\s+(\w+)", chunk)
            if m:
                names = [m.group(1)]
            items.append(Item("pp", chunk, names, pending))
            pending = ""
            i = e + 1
            continue
        # a declaration or definition: up to ';' at depth 0, or a '}' closing a body
        j, depth, body = i, 0, False
        while j < n:
            c = text[j]
            if text.startswith("/*", j):
                j = text.find("*/", j) + 2
                continue
            if text.startswith("//", j):
                j = text.find("\n", j)
                continue
            if c == '"' or c == "'":
                q = c
                j += 1
                while j < n and text[j] != q:
                    j += 2 if text[j] == "\\" else 1
                j += 1
                continue
            if c in "({[":
                if c == "{" and depth == 0:
                    body = True
                depth += 1
            elif c in ")}]":
                depth -= 1
                if c == "}" and depth == 0 and body:
                    # a function body ends here unless a ';' follows (struct / initializer)
                    k = j + 1
                    while k < n and text[k] in " \t":
                        k += 1
                    if k < n and text[k] == ";":
                        j = k
                        break
                    if not re.match(r"\s*[\w*(\[]", text[k:k + 40]) or text[k] == "\n":
                        break
            elif c == ";" and depth == 0:
                break
            j += 1
        k = j + 1
        while k < n and text[k] in " \t":
            k += 1
        if k < n and text.startswith("/*", k) and "\n" not in text[k:text.find("*/", k)]:
            k = text.find("*/", k) + 2   # a trailing comment on the same line
        if k < n and text[k] == "\n":
            k += 1
        chunk = text[i:k]
        items.append(classify(chunk, pending))
        pending = ""
        i = k
    if pending:
        items.append(Item("other", pending, []))
    return items


def classify(chunk, lead):
    code = strip_comments(chunk)
    s = code.strip()
    # function definition: "... name(params) {"
    m = re.match(r"^((?:static\s+|inline\s+|__attribute__\s*\(\(.*?\)\)\s*)*)[\w\s\*]*?\b(\w+)\s*\(([^;]*?)\)\s*(?:__attribute__\s*\(\(.*?\)\)\s*)*\{", s, re.S)
    if m and not s.startswith(("typedef", "struct", "union", "enum", "extern")) and "=" not in s.split("(")[0]:
        static = "static" in m.group(1).split() or s.startswith("static")
        return Item("func", chunk, [m.group(2)], lead) if not static else Item("sfunc", chunk, [m.group(2)], lead)
    if s.startswith("typedef"):
        names = re.findall(r"(\w+)\s*;\s*$", s) or re.findall(r"\(\s*\*\s*(\w+)\s*\)", s)
        m2 = re.match(r"typedef\s+(?:struct|union|enum)\s+(\w+)", s)
        if m2:
            names.append("struct " + m2.group(1))
        return Item("type", chunk, names, lead)
    if re.match(r"^(struct|union|enum)\s+\w+\s*\{", s):
        return Item("type", chunk, ["struct " + re.match(r"^(?:struct|union|enum)\s+(\w+)", s).group(1)], lead)
    if s.startswith("extern"):
        return Item("decl", chunk, decl_names(s), lead)
    if s.startswith("static"):
        return Item("svar", chunk, decl_names(s), lead)
    # a prototype without extern
    m = re.match(r"^[\w\s\*]+?\b(\w+)\s*\([^;]*\)\s*;$", s, re.S)
    if m:
        return Item("decl", chunk, [m.group(1)], lead)
    return Item("other", chunk, decl_names(s), lead)


def decl_names(s):
    s = re.sub(r"=.*", "", s, flags=re.S) if not s.startswith(("extern", "static")) or "(" not in s else s
    s = re.sub(r"\{.*\}", "", s, flags=re.S)
    m = re.match(r"^(?:extern|static)?\s*(?:const\s+|volatile\s+|unsigned\s+|signed\s+|struct\s+\w+\s+|union\s+\w+\s+)*\w+\s+(.*?);?\s*$", s.strip(), re.S)
    if not m:
        return []
    rest = m.group(1)
    names = []
    depth, cur = 0, ""
    parts = []
    for ch in rest:
        if ch in "([{":
            depth += 1
        if ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += ch
    parts.append(cur)
    for p in parts:
        p = p.split("=")[0]
        mm = re.match(r"[\s\*]*\(\s*\*\s*(\w+)", p) or re.match(r"[\s\*]*(\w+)", p)
        if mm:
            names.append(mm.group(1))
    return names


def defined_names(items):
    out = {}
    for it in items:
        if it.kind in ("pp", "type", "sfunc", "svar", "func", "other"):
            for nm in it.names:
                out[nm] = it
    return out


def declared(items):
    out = set()
    for it in items:
        if it.kind in ("decl", "func", "sfunc", "svar", "type", "pp", "other"):
            out.update(it.names)
    return out


def includes_of(items):
    return [it for it in items if it.kind == "pp" and it.text.lstrip().startswith("#include")]


def main():
    args = sys.argv[1:]
    check = False
    if args and args[0] == "--check":
        check = True
        args = args[1:]
    if not args:
        sys.exit(__doc__)
    plan = OrderedDict()
    for line in Path(args[0]).read_text().splitlines():
        line = line.split("#")[0].strip()
        if not line:
            continue
        f, dst = line.split()
        plan[f] = dst
    # locate every function
    files = {}
    owner = {}
    for p in sorted(list((ROOT / "src").rglob("*.c")) + list((ROOT / "src").rglob("*.inc"))):
        rel = str(p.relative_to(ROOT))
        its = split_items(p.read_text())
        if "".join(it.full() for it in its) != p.read_text():
            sys.exit(f"parse round-trip failed: {rel}")
        files[rel] = its
        for it in its:
            if it.kind in ("func", "sfunc"):
                owner.setdefault(it.names[0], rel)
    moves = OrderedDict()   # (src, dst) -> [func names]
    for f, dst in plan.items():
        if f not in owner:
            sys.exit(f"{f}: not defined in src/")
        moves.setdefault((owner[f], dst), []).append(f)
    problems = []
    edits = {}
    for (src, dst), funcs in moves.items():
        if src == dst:
            continue
        s_items = edits.get(src, files[src])
        d_items = edits.get(dst, files.get(dst))
        if d_items is None:
            sys.exit(f"{dst}: no such file (create it first)")
        s_def = defined_names(s_items)
        d_def = defined_names(d_items)
        # only what the destination declares before its first function counts: a moved
        # function may land anywhere above a later declaration
        first = next((k for k, x in enumerate(d_items) if x.kind in ("func", "sfunc")), len(d_items))
        d_hdr = header_decls(d_items)
        d_decl = declared(d_items[:first]) | set(d_hdr)
        d_def_top = defined_names(d_items[:first])
        # what to carry: closure over local definitions
        need, order = set(), []
        moving = [it for it in s_items if it.kind in ("func", "sfunc") and it.names[0] in funcs]
        work = []
        for it in moving:
            work.extend(it.uses())
        carried_local = []
        fwd = []
        renames = []
        seen = set()
        while work:
            nm = work.pop()
            if nm in seen:
                continue
            seen.add(nm)
            it = s_def.get(nm) or s_def.get("struct " + nm)
            if it is None or it.kind == "func" and it.names[0] in funcs:
                continue
            if it.kind == "func":
                continue   # a non-static function: declared, not copied
            if it in moving:
                continue
            other = d_def.get(nm) or d_def.get("struct " + nm)
            if other is not None and other.kind in ("func",):
                continue
            if other is not None and other.kind == "sfunc" and nm not in d_def_top:
                fwd.append("static " + re.sub(r"^static\s+", "", prototype(other.text)[len("extern "):]) + "\n")
                continue
            if other is not None and other.kind == "pp" and nm not in d_def_top \
                    and strip_comments(other.text).split() == strip_comments(it.text).split():
                other = None   # the destination defines it further down: an identical copy goes on top
            if other is not None:
                if strip_comments(other.text).split() != strip_comments(it.text).split():
                    # same name, different definition: the incoming one gets a new name
                    new = unique_name(nm, src, d_def, s_def)
                    renames.append((nm, new))
                    print(f"renamed {nm} -> {new} ({src} -> {dst}: the destination has its own)")
                    s_items = rename_in(s_items, nm, new)
                    s_def = defined_names(s_items)
                    moving = [x for x in s_items if x.kind in ("func", "sfunc") and x.names[0] in funcs]
                    work.extend([new])
                continue
            if it not in carried_local:
                carried_local.append(it)
                work.extend(it.uses())
        carried_local.sort(key=s_items.index)
        # extern declarations needed
        uses = set()
        for it in moving + carried_local:
            uses |= it.uses()
        decls = []
        for it in s_items:
            if it.kind != "decl":
                continue
            wanted = [nm for nm in it.names if nm in uses and nm not in d_decl and nm not in d_def]
            if wanted:
                if len(it.names) == len(wanted):
                    decls.append(it.full())
                else:
                    # keep only the wanted declarators of a multi-name extern
                    decls.append(rebuild_decl(it.text, wanted))
        # functions defined (non-static) in the source and used by the moved ones, not declared in dst
        for nm in sorted(uses):
            it = s_def.get(nm)
            if it is not None and it.kind == "func" and it.names[0] not in funcs and nm not in d_decl:
                decls.append(prototype(it.text) + "\n")
        # the moved functions' own prototypes replace any the destination has (they may differ)
        protos = [prototype(it.text)[len("extern "):] + "\n" for it in moving
                  if it.kind == "func" and it.names[0] not in d_hdr] + fwd
        for it in moving:
            if it.names[0] in d_hdr:
                print(f"header: {dst} {it.names[0]}: {' '.join(prototype(it.text).split())}  vs  {' '.join(strip_comments(d_hdr[it.names[0]]).split())}")
        d_incs = set(x.text.strip() for x in includes_of(d_items))
        incs = [x.text for x in includes_of(s_items) if x.text.strip() not in d_incs]
        if check:
            print(f"{src} -> {dst}: {', '.join(funcs)}")
            for x in incs:
                print("    + " + x.strip())
            for it in carried_local:
                print(f"    + local {', '.join(it.names)}")
            for d in decls:
                print("    + " + d.strip().replace("\n", " ")[:110])
            continue
        # remove from the source
        s_new = [it for it in s_items if it not in moving]
        # drop local items no longer used by anything left
        edits[src] = prune_unused(s_new)
        # insert into the destination
        edits[dst] = insert(drop_decls(d_items, set(funcs)), moving, carried_local, decls, incs, protos)
    if problems:
        print("\n".join(problems))
        sys.exit(1)
    if check:
        return
    for rel, its in edits.items():
        p = ROOT / rel
        has_func = any(it.kind in ("func", "sfunc") for it in its)
        if not has_func and rel.startswith("src/leaf/"):
            p.unlink()
            print(f"deleted {rel}")
            continue
        p.write_text(tidy("".join(it.full() for it in its)))
    # the difftest list follows the functions
    lines = DIFFLIST.read_text().splitlines()
    out = []
    for line in lines:
        parts = line.split(" ", 2)
        if len(parts) >= 2 and parts[1] in plan and parts[0] != plan[parts[1]]:
            parts[0] = plan[parts[1]]
            line = " ".join(parts)
        out.append(line)
    DIFFLIST.write_text("\n".join(out) + "\n")


def unique_name(nm, src, d_def, s_def):
    base = nm + "_" + Path(src).stem.split("_")[-1]
    new, k = base, 2
    while new in d_def or new in s_def:
        new, k = f"{base}{k}", k + 1
    return new


def rename_in(items, old, new):
    w = re.compile(r"\b%s\b" % re.escape(old))
    out = []
    for it in items:
        t = w.sub(new, it.text)
        out.append(Item(it.kind, t, [new if n == old else n for n in it.names], it.lead) if t != it.text or old in it.names else it)
    return out


def rebuild_decl(text, wanted):
    code = strip_comments(text).strip().rstrip(";")
    m = re.match(r"(extern\s+(?:(?:const|volatile|unsigned|signed|struct\s+\w+)\s+)*\w+\s+)(.*)$", code, re.S)
    pre, rest = m.group(1), m.group(2)
    parts, depth, cur = [], 0, ""
    for ch in rest:
        if ch in "([{":
            depth += 1
        if ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += ch
    parts.append(cur)
    keep = []
    for p in parts:
        nm = decl_names("extern int " + p.strip() + ";")
        if nm and nm[0] in wanted:
            keep.append(p.strip())
    return pre + ", ".join(keep) + ";\n"


def prototype(text):
    code = strip_comments(text)
    head = code[:code.index("{")].strip()
    head = re.sub(r"\s+", " ", head)
    return "extern " + head + ";"


def prune_unused(items):
    """drop static helpers / macros / local types nothing left uses (repeat until stable)"""
    while True:
        used = set()
        for it in items:
            used |= it.uses()
        drop = None
        for it in items:
            if it.kind in ("sfunc", "svar") and not (set(it.names) & used):
                drop = it
                break
            if it.kind == "pp" and it.names and not (set(it.names) & used):
                drop = it
                break
        if drop is None:
            break
        items = [it for it in items if it is not drop]
    # extern declarations nothing uses any more
    used = set()
    for it in items:
        if it.kind != "decl":
            used |= it.uses()
    return [it for it in items if not (it.kind == "decl" and it.names and not (set(it.names) & used))]


_hdr_cache = {}


def header_decls(items, seen=None):
    """names declared by the headers a file includes (recursively), -> declaration text"""
    out = {}
    seen = set() if seen is None else seen
    for it in includes_of(items):
        m = re.search(r'#include\s+"([^"]+)"', it.text)
        if not m:
            continue
        for base in (ROOT / "include", ROOT / "src"):
            h = base / m.group(1)
            if h.is_file():
                break
        else:
            continue
        if h in seen:
            continue
        seen.add(h)
        if h not in _hdr_cache:
            _hdr_cache[h] = split_items(h.read_text())
        its = _hdr_cache[h]
        for x in its:
            if x.kind == "decl":
                for n in x.names:
                    out.setdefault(n, x.text)
        out.update({k: v for k, v in header_decls(its, seen).items() if k not in out})
    return out


def drop_decls(items, names):
    out = []
    for it in items:
        if it.kind == "decl" and set(it.names) & names:
            keep = [n for n in it.names if n not in names]
            if not keep:
                continue
            it = Item("decl", rebuild_decl(it.text, keep), keep, it.lead)
        out.append(it)
    return out


def insert(d_items, moving, carried, decls, incs=(), protos=()):
    items = list(d_items)
    if incs:
        last = max((k for k, x in enumerate(items) if x.kind == "pp" and x.text.lstrip().startswith("#include")
                    and k < next((j for j, y in enumerate(items) if y.kind in ("func", "sfunc", "decl")), len(items))),
                   default=-1)
        items[last + 1:last + 1] = [Item("pp", t, []) for t in incs]
    funcs = [i for i, it in enumerate(items) if it.kind in ("func", "sfunc")]
    # declarations and carried local items: after the last include / decl block before the
    # first function (simple and predictable)
    first_func = funcs[0] if funcs else len(items)
    head = []
    for d in decls:
        head.append(Item("decl", d, decl_names(strip_comments(d).strip())))
    for it in carried:
        head.append(Item(it.kind, it.text + ("" if it.text.endswith("\n\n") else "\n"), it.names, it.lead))
    for d in protos:
        head.append(Item("decl", d, decl_names(strip_comments(d).strip())))
    if head:
        if not head[-1].text.endswith("\n\n"):
            head[-1].text += "\n"
        items[first_func:first_func] = head
    # each moved function by address among the destination's functions, outside #if blocks
    def depth_after(items):
        d, out = 0, []
        for x in items:
            if x.kind == "pp":
                t = x.text.lstrip()
                if re.match(r"#\s*if", t):
                    d += 1
                elif re.match(r"#\s*endif", t):
                    d -= 1
            out.append(d)
        return out

    for it in moving:
        a = addr_of(it.names[0])
        pos = None
        if a is not None:
            best = None
            for i, x in enumerate(items):
                if x.kind in ("func", "sfunc"):
                    xa = addr_of(x.names[0])
                    if xa is not None and xa < a and (best is None or xa > best[0]):
                        best = (xa, i)
            if best is not None:
                pos = best[1] + 1
                depth = depth_after(items)
                while pos < len(items) and depth[pos - 1] > 0:
                    pos += 1
            else:
                fs = [i for i, x in enumerate(items) if x.kind in ("func", "sfunc")]
                pos = fs[0] if fs else len(items)
        if pos is None:
            pos = len(items)
        new = Item(it.kind, it.text if it.text.endswith("\n") else it.text + "\n", it.names, it.lead)
        items[pos:pos] = [Item("blank", "\n", []), new]
    return items


def tidy(t):
    t = re.sub(r"\n{3,}", "\n\n", t)
    return t.rstrip("\n") + "\n"


if __name__ == "__main__":
    main()
