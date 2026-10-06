/* The Forth system: the inner interpreter, the outer interpreter (text -> actions), the
 * compiler and the core primitives. See forth.h for the model and docs/forth.md for the dialect. */
#include "forth.h"

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* cells a Float takes in threaded code (one on 64-bit, two on 32-bit) */
#define FCELLS ((sizeof(Float) + sizeof(Cell) - 1) / sizeof(Cell))
#define CELLS_FOR(bytes) (((bytes) + sizeof(Cell) - 1) / sizeof(Cell))

#define PRIM(name) static void name(Forth *f, Word *w __attribute__((unused)))
#define PUSH(x) forth_push(f, (Cell)(x))
#define POP() forth_pop(f)
#define FPUSH(x) forth_fpush(f, (Float)(x))
#define FPOP() forth_fpop(f)
#define IP (f->t->ip)

/* ---- stacks ---- */

void forth_push(Forth *f, Cell x) {
    Task *t = f->t;

    if (t->sp >= STACK_CELLS) {
        forth_error(f, "stack overflow");
    }
    t->ds[t->sp++] = x;
}

Cell forth_pop(Forth *f) {
    Task *t = f->t;

    if (t->sp <= 0) {
        forth_error(f, "stack underflow");
    }
    return t->ds[--t->sp];
}

static void rpush(Forth *f, Cell x) {
    Task *t = f->t;

    if (t->rp >= STACK_CELLS) {
        forth_error(f, "return stack overflow");
    }
    t->rs[t->rp++] = x;
}

static Cell rpop(Forth *f) {
    Task *t = f->t;

    if (t->rp <= 0) {
        forth_error(f, "return stack underflow");
    }
    return t->rs[--t->rp];
}

void forth_fpush(Forth *f, Float x) {
    Task *t = f->t;

    if (t->fp >= FSTACK_CELLS) {
        forth_error(f, "float stack overflow");
    }
    t->fs[t->fp++] = x;
}

Float forth_fpop(Forth *f) {
    Task *t = f->t;

    if (t->fp <= 0) {
        forth_error(f, "float stack underflow");
    }
    return t->fs[--t->fp];
}

/* the top items, checked (n = how many must be there) */
static Cell *top(Forth *f, int n) {
    if (f->t->sp < n) {
        forth_error(f, "stack underflow");
    }
    return &f->t->ds[f->t->sp - 1];
}

/* ---- output and errors ---- */

void forth_type(Forth *f, const char *s, size_t n) {
    f->out(f->out_ctx, s, n);
}

void forth_printf(Forth *f, const char *fmt, ...) {
    char buf[512];
    va_list ap;
    int n;

    va_start(ap, fmt);
    n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n > 0) {
        forth_type(f, buf, (size_t)n < sizeof(buf) ? (size_t)n : sizeof(buf) - 1);
    }
}

static void stdout_output(void *ctx, const char *s, size_t n) {
    (void)ctx;
    fwrite(s, 1, n, stdout);
    fflush(stdout);
}

void forth_set_output(Forth *f, OutputFn out, void *ctx) {
    f->out = out != NULL ? out : stdout_output;
    f->out_ctx = ctx;
}

_Noreturn void forth_error(Forth *f, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(f->error, sizeof(f->error), fmt, ap);
    va_end(ap);
    if (f->silent > 0) {
        /* inside `catch`: the caller decides what to say */
    } else if (f->nsrc > 0) {
        Source *s = &f->src[f->nsrc - 1];

        forth_printf(f, "\nerror: %s (%s:%d)\n", f->error, s->name, s->line);
    } else {
        forth_printf(f, "\nerror: %s\n", f->error);
    }
    if (f->catch == NULL) {
        abort();
    }
    longjmp(*f->catch, 1);
}

/* ---- the dictionary ---- */

static void align_here(Forth *f) {
    UCell a = (UCell)f->here;

    a = (a + sizeof(Cell) - 1) & ~(UCell)(sizeof(Cell) - 1);
    f->here = (uint8_t *)a;
}

static void *allot(Forth *f, size_t n) {
    void *p = f->here;

    if (f->here + n > f->end) {
        forth_error(f, "dictionary full");
    }
    f->here += n;
    return p;
}

static void comma(Forth *f, Cell x) {
    align_here(f);
    *(Cell *)allot(f, sizeof(Cell)) = x;
}

static void comma_float(Forth *f, Float x) {
    Cell c[FCELLS];
    size_t i;

    memset(c, 0, sizeof(c));
    memcpy(c, &x, sizeof(x));
    for (i = 0; i < FCELLS; i++) {
        comma(f, c[i]);
    }
}

static Word *new_word(Forth *f, const char *name, size_t len, Code code) {
    Word *w;

    if (len > WORD_NAME_MAX) {
        forth_error(f, "name too long: %.*s", (int)len, name);
    }
    align_here(f);
    w = allot(f, sizeof(Word));
    memset(w, 0, sizeof(Word));
    w->vocab = f->m.current;
    w->link = f->m.current->latest;
    w->code = code;
    w->len = (uint8_t)len;
    memcpy(w->name, name, len);
    f->m.current->latest = w;
    f->latest = w;
    return w;
}

static int same_name(const char *a, const char *b, size_t n) {
    size_t i;

    for (i = 0; i < n; i++) {
        if (tolower((unsigned char)a[i]) != tolower((unsigned char)b[i])) {
            return 0;
        }
    }
    return 1;
}

/* ---- vocabularies ---- */

static Vocab *vocab_find(Forth *f, const char *name, size_t len) {
    Vocab *v;

    for (v = f->vocabs; v != NULL; v = v->next) {
        if (strlen(v->name) == len && same_name(v->name, name, len)) {
            return v;
        }
    }
    return NULL;
}

Vocab *forth_vocab(Forth *f, const char *name) {
    Vocab *v = vocab_find(f, name, strlen(name));

    if (v == NULL) {
        v = calloc(1, sizeof(Vocab));
        snprintf(v->name, sizeof(v->name), "%s", name);
        v->next = f->vocabs;
        f->vocabs = v;
    }
    return v;
}

static void order_add(Manifest *m, Vocab *v) {
    int i;

    for (i = 0; i < m->norder; i++) {
        if (m->order[i] == v) {
            return;
        }
    }
    if (m->norder < ORDER_MAX) {
        m->order[m->norder++] = v;
    }
}

void forth_set_current(Forth *f, Vocab *v) {
    f->m.current = v;
    order_add(&f->m, v);
}

void forth_add_root(Forth *f, const char *dir) {
    if (f->nroots < 4) {
        snprintf(f->roots[f->nroots++], sizeof(f->roots[0]), "%s", dir);
    }
}

/* a source's starting view: only the core; definitions into `scratch` until IN: */
static Manifest fresh_manifest(Forth *f, int listener) {
    Manifest m;

    memset(&m, 0, sizeof(m));
    m.current = forth_vocab(f, "scratch");
    m.order[m.norder++] = m.current;
    m.listener = listener;
    return m;
}

static Word *in_vocab(Vocab *v, const char *name, size_t len) {
    Word *w;

    for (w = v->latest; w != NULL; w = w->link) {
        if (w->len == len && !(w->flags & WORD_HIDDEN) && same_name(w->name, name, len)) {
            return w;
        }
    }
    return NULL;
}

/* the one word of that name among vocabularies (an error if two have it) */
static Word *unique(Forth *f, Word *found, Word *w, const char *name, size_t len) {
    if (found != NULL && w != NULL && found != w) {
        forth_error(f, "%.*s is in both %s and %s: say %s:%.*s or %s:%.*s", (int)len, name, found->vocab->name,
                    w->vocab->name, found->vocab->name, (int)len, name, w->vocab->name, (int)len, name);
    }
    return found != NULL ? found : w;
}

/* Lookup: `vocab:name` exactly; else the source's own vocabulary, then the ones it uses
 * (exactly one may have the name), then - at the prompt - every loaded one, then the core */
Word *forth_find(Forth *f, const char *name, size_t len) {
    Word *found = NULL, *w;
    Vocab *v;
    size_t i;
    int k;

    for (i = 1; i + 1 < len; i++) {
        if (name[i] == ':') {
            v = vocab_find(f, name, i);
            if (v != NULL) {
                return in_vocab(v, name + i + 1, len - i - 1);
            }
            break;
        }
    }
    if ((w = in_vocab(f->m.current, name, len)) != NULL) {
        return w;
    }
    for (k = 0; k < f->m.norder; k++) {
        if (f->m.order[k] != f->core && f->m.order[k] != f->m.current) {
            found = unique(f, found, in_vocab(f->m.order[k], name, len), name, len);
        }
    }
    if (found == NULL && f->m.listener) {
        for (v = f->vocabs; v != NULL; v = v->next) {
            if (v != f->core) {
                found = unique(f, found, in_vocab(v, name, len), name, len);
            }
        }
    }
    return found != NULL ? found : in_vocab(f->core, name, len);
}

/* the word defined right after w (where w's body ends), or NULL if none */
static Word *word_after(Forth *f, Word *w) {
    Word *best = NULL, *x;
    Vocab *v;

    for (v = f->vocabs; v != NULL; v = v->next) {
        for (x = v->latest; x != NULL; x = x->link) {
            if (x > w && (best == NULL || x < best)) {
                best = x;
            }
        }
    }
    return best;
}

/* ---- the inner interpreter ---- */

/* run threaded code from IP until it stops (IP = NULL: `halt`) or the task yields */
static void run(Forth *f) {
    while (IP != NULL && !f->yielded) {
        Word *w = (Word *)*IP++;

        w->code(f, w);
    }
}

void forth_execute(Forth *f, Word *w) {
    Cell *saved = IP;
    Cell code[2];

    code[0] = (Cell)w;
    code[1] = (Cell)f->w_halt;
    IP = code;
    f->depth++;
    run(f);
    f->depth--;
    IP = saved;
}

/* code fields */

static void docol(Forth *f, Word *w) {   /* a colon definition: run its body */
    rpush(f, (Cell)IP);
    IP = w->body;
}

static void dovar(Forth *f, Word *w) {   /* CREATE: the body's address */
    PUSH(w->body);
}

static void docon(Forth *f, Word *w) {
    PUSH(w->body[0]);
}

static void dofcon(Forth *f, Word *w) {
    Float x;

    memcpy(&x, w->body, sizeof(x));
    FPUSH(x);
}

static void dodoes(Forth *f, Word *w) {   /* made by CREATE ... DOES>: body address, then the DOES> code */
    PUSH(w->body);
    rpush(f, (Cell)IP);
    IP = w->does;
}

static Word *prim_word(Forth *f, const char *name, Code code, int flags, int operands) {
    Word *w = new_word(f, name, strlen(name), code);

    w->flags = (uint8_t)flags;
    w->operands = (uint8_t)operands;
    return w;
}

Word *forth_prim(Forth *f, const char *name, Code code) {
    return prim_word(f, name, code, 0, 0);
}

Word *forth_constant(Forth *f, const char *name, Cell value) {
    Word *w = new_word(f, name, strlen(name), docon);

    comma(f, value);
    return w;
}

Word *forth_fconstant(Forth *f, const char *name, Float value) {
    Word *w = new_word(f, name, strlen(name), dofcon);

    comma_float(f, value);
    return w;
}

/* ---- threaded-code primitives (the compiler lays these down) ---- */

PRIM(p_lit) { PUSH(*IP++); }
PRIM(p_flit) {
    Float x;

    memcpy(&x, IP, sizeof(x));
    IP += FCELLS;
    FPUSH(x);
}
PRIM(p_litstring) {   /* ( -- addr len ): the length, then the bytes */
    Cell n = *IP++;

    PUSH(IP);
    PUSH(n);
    IP += CELLS_FOR((size_t)n);
}
PRIM(p_exit) { IP = (Cell *)rpop(f); }
PRIM(p_halt) { IP = NULL; }
PRIM(p_branch) { IP = (Cell *)*IP; }
PRIM(p_0branch) {
    if (POP() == 0) {
        IP = (Cell *)*IP;
    } else {
        IP++;
    }
}
PRIM(p_does) {   /* (does>): the latest word runs the rest of this definition; return now */
    f->latest->code = dodoes;
    f->latest->does = IP;
    IP = (Cell *)rpop(f);
}
PRIM(p_execute) {   /* not nested: jump into the word as the inner interpreter would */
    Word *x = (Word *)POP();

    if (x == NULL) {
        forth_error(f, "execute: null");
    }
    x->code(f, x);
}

/* counted loops: the return stack holds the exit address, the limit and the index */
PRIM(p_do) {
    Cell exit = *IP++, start = POP(), limit = POP();

    rpush(f, exit);
    rpush(f, limit);
    rpush(f, start);
}
PRIM(p_qdo) {
    Cell exit = *IP++, start = POP(), limit = POP();

    if (start == limit) {
        IP = (Cell *)exit;
        return;
    }
    rpush(f, exit);
    rpush(f, limit);
    rpush(f, start);
}
static void loop_step(Forth *f, Cell n) {
    Task *t = f->t;
    Cell d, nd;

    if (t->rp < 3) {
        forth_error(f, "loop: not in a do loop");
    }
    d = t->rs[t->rp - 1] - t->rs[t->rp - 2];
    nd = d + n;
    t->rs[t->rp - 1] += n;
    if ((d ^ nd) < 0) {   /* crossed the line between limit - 1 and limit */
        t->rp -= 3;
        IP++;
    } else {
        IP = (Cell *)*IP;
    }
}
PRIM(p_loop) { loop_step(f, 1); }
PRIM(p_ploop) { loop_step(f, POP()); }
PRIM(p_i) {
    if (f->t->rp < 3) {
        forth_error(f, "i: not in a do loop");
    }
    PUSH(f->t->rs[f->t->rp - 1]);
}
PRIM(p_j) {
    if (f->t->rp < 6) {
        forth_error(f, "j: not in a nested do loop");
    }
    PUSH(f->t->rs[f->t->rp - 4]);
}
PRIM(p_leave) {
    if (f->t->rp < 3) {
        forth_error(f, "leave: not in a do loop");
    }
    IP = (Cell *)f->t->rs[f->t->rp - 3];
    f->t->rp -= 3;
}
PRIM(p_unloop) {
    if (f->t->rp < 3) {
        forth_error(f, "unloop: not in a do loop");
    }
    f->t->rp -= 3;
}

/* ---- stack words ---- */

PRIM(p_dup) { Cell a = *top(f, 1); PUSH(a); }
PRIM(p_drop) { (void)POP(); }
PRIM(p_swap) { Cell *s = top(f, 2), a = s[0]; s[0] = s[-1]; s[-1] = a; }
PRIM(p_over) { Cell a = top(f, 2)[-1]; PUSH(a); }
PRIM(p_rot) { Cell *s = top(f, 3), a = s[-2]; s[-2] = s[-1]; s[-1] = s[0]; s[0] = a; }
PRIM(p_mrot) { Cell *s = top(f, 3), a = s[0]; s[0] = s[-1]; s[-1] = s[-2]; s[-2] = a; }
PRIM(p_nip) { Cell a = POP(); (void)POP(); PUSH(a); }
PRIM(p_tuck) { Cell b = POP(), a = POP(); PUSH(b); PUSH(a); PUSH(b); }
PRIM(p_2dup) { Cell *s = top(f, 2), a = s[-1], b = s[0]; PUSH(a); PUSH(b); }
PRIM(p_2drop) { (void)POP(); (void)POP(); }
PRIM(p_2swap) {
    Cell d = POP(), c = POP(), b = POP(), a = POP();

    PUSH(c); PUSH(d); PUSH(a); PUSH(b);
}
PRIM(p_2over) { Cell *s = top(f, 4), a = s[-3], b = s[-2]; PUSH(a); PUSH(b); }
PRIM(p_qdup) { Cell a = *top(f, 1); if (a != 0) PUSH(a); }
PRIM(p_pick) {
    Cell n = POP();

    if (n < 0 || n >= f->t->sp) {
        forth_error(f, "pick: out of range");
    }
    PUSH(f->t->ds[f->t->sp - 1 - n]);
}
PRIM(p_depth) { PUSH(f->t->sp); }
PRIM(p_tor) { rpush(f, POP()); }
PRIM(p_rfrom) { PUSH(rpop(f)); }
PRIM(p_rfetch) {
    if (f->t->rp < 1) {
        forth_error(f, "return stack underflow");
    }
    PUSH(f->t->rs[f->t->rp - 1]);
}
PRIM(p_rdrop) { (void)rpop(f); }

/* ---- arithmetic and logic ---- */

#define BINOP(name, expr) PRIM(name) { Cell b = POP(), a = POP(); PUSH(expr); }
BINOP(p_add, a + b)
BINOP(p_sub, a - b)
BINOP(p_mul, a * b)
BINOP(p_and, a & b)
BINOP(p_or, a | b)
BINOP(p_xor, a ^ b)
BINOP(p_lshift, (Cell)((UCell)a << b))
BINOP(p_rshift, (Cell)((UCell)a >> b))
BINOP(p_arshift, a >> b)
BINOP(p_min, a < b ? a : b)
BINOP(p_max, a > b ? a : b)
BINOP(p_eq, a == b ? -1 : 0)
BINOP(p_ne, a != b ? -1 : 0)
BINOP(p_lt, a < b ? -1 : 0)
BINOP(p_gt, a > b ? -1 : 0)
BINOP(p_le, a <= b ? -1 : 0)
BINOP(p_ge, a >= b ? -1 : 0)
BINOP(p_ult, (UCell)a < (UCell)b ? -1 : 0)
BINOP(p_ugt, (UCell)a > (UCell)b ? -1 : 0)

static Cell divisor(Forth *f, Cell b) {
    if (b == 0) {
        forth_error(f, "division by zero");
    }
    return b;
}
/* floored division, as Forth-2012 recommends: the remainder has the divisor's sign */
static void floored(Forth *f, Cell a, Cell b, Cell *q, Cell *r) {
    divisor(f, b);
    *q = a / b;
    *r = a % b;
    if (*r != 0 && (*r < 0) != (b < 0)) {
        *q -= 1;
        *r += b;
    }
}
PRIM(p_div) { Cell b = POP(), a = POP(), q, r; floored(f, a, b, &q, &r); PUSH(q); }
PRIM(p_mod) { Cell b = POP(), a = POP(), q, r; floored(f, a, b, &q, &r); PUSH(r); }
PRIM(p_divmod) { Cell b = POP(), a = POP(), q, r; floored(f, a, b, &q, &r); PUSH(r); PUSH(q); }
PRIM(p_muldiv) {   /* a * b / c with a wide intermediate */
    Cell c = POP(), b = POP(), a = POP();
    long long p = (long long)a * b;

    divisor(f, c);
    PUSH((Cell)(p / c));
}
PRIM(p_negate) { Cell *s = top(f, 1); *s = -*s; }
PRIM(p_abs) { Cell *s = top(f, 1); if (*s < 0) *s = -*s; }
PRIM(p_invert) { Cell *s = top(f, 1); *s = ~*s; }
PRIM(p_1add) { *top(f, 1) += 1; }
PRIM(p_1sub) { *top(f, 1) -= 1; }
PRIM(p_0eq) { Cell *s = top(f, 1); *s = *s == 0 ? -1 : 0; }
PRIM(p_0ne) { Cell *s = top(f, 1); *s = *s != 0 ? -1 : 0; }
PRIM(p_0lt) { Cell *s = top(f, 1); *s = *s < 0 ? -1 : 0; }
PRIM(p_0gt) { Cell *s = top(f, 1); *s = *s > 0 ? -1 : 0; }

/* ---- memory ---- */

static void *addr(Forth *f, Cell a) {
    if (a == 0) {
        forth_error(f, "null address");
    }
    return (void *)a;
}
PRIM(p_fetch) { Cell a = POP(); PUSH(*(Cell *)addr(f, a)); }
PRIM(p_store) { Cell a = POP(), x = POP(); *(Cell *)addr(f, a) = x; }
PRIM(p_pstore) { Cell a = POP(), x = POP(); *(Cell *)addr(f, a) += x; }
PRIM(p_cfetch) { Cell a = POP(); PUSH(*(uint8_t *)addr(f, a)); }
PRIM(p_cstore) { Cell a = POP(), x = POP(); *(uint8_t *)addr(f, a) = (uint8_t)x; }
PRIM(p_wfetch) { Cell a = POP(); uint16_t v; memcpy(&v, addr(f, a), 2); PUSH(v); }
PRIM(p_swfetch) { Cell a = POP(); int16_t v; memcpy(&v, addr(f, a), 2); PUSH(v); }
PRIM(p_wstore) { Cell a = POP(), x = POP(); uint16_t v = (uint16_t)x; memcpy(addr(f, a), &v, 2); }
PRIM(p_lfetch) { Cell a = POP(); uint32_t v; memcpy(&v, addr(f, a), 4); PUSH(v); }
PRIM(p_slfetch) { Cell a = POP(); int32_t v; memcpy(&v, addr(f, a), 4); PUSH(v); }
PRIM(p_lstore) { Cell a = POP(), x = POP(); uint32_t v = (uint32_t)x; memcpy(addr(f, a), &v, 4); }
PRIM(p_move) {   /* ( src dst n -- ) */
    Cell n = POP(), d = POP(), s = POP();

    if (n > 0) {
        memmove(addr(f, d), addr(f, s), (size_t)n);
    }
}
PRIM(p_fill) {   /* ( addr n byte -- ) */
    Cell c = POP(), n = POP(), a = POP();

    if (n > 0) {
        memset(addr(f, a), (int)c, (size_t)n);
    }
}
PRIM(p_cells) { *top(f, 1) *= (Cell)sizeof(Cell); }
PRIM(p_floats) { *top(f, 1) *= (Cell)sizeof(Float); }
PRIM(p_here) { align_here(f); PUSH(f->here); }
PRIM(p_allot) { Cell n = POP(); if (n < 0) f->here += n; else allot(f, (size_t)n); }
PRIM(p_comma) { comma(f, POP()); }
PRIM(p_ccomma) { *(uint8_t *)allot(f, 1) = (uint8_t)POP(); }
PRIM(p_fcomma) { comma_float(f, FPOP()); }
PRIM(p_align) { align_here(f); }
PRIM(p_compare) {   /* ( a1 n1 a2 n2 -- -1|0|1 ) */
    Cell n2 = POP(), a2 = POP(), n1 = POP(), a1 = POP();
    Cell n = n1 < n2 ? n1 : n2;
    int c = n > 0 ? memcmp((void *)a1, (void *)a2, (size_t)n) : 0;

    PUSH(c != 0 ? (c < 0 ? -1 : 1) : (n1 < n2 ? -1 : n1 > n2 ? 1 : 0));
}

/* ---- floats ---- */

#define FBINOP(name, expr) PRIM(name) { Float b = FPOP(), a = FPOP(); FPUSH(expr); }
#define FUNOP(name, expr) PRIM(name) { Float a = FPOP(); FPUSH(expr); }
#define FCMP(name, expr) PRIM(name) { Float b = FPOP(), a = FPOP(); PUSH((expr) ? -1 : 0); }
FBINOP(p_fadd, a + b)
FBINOP(p_fsub, a - b)
FBINOP(p_fmul, a * b)
FBINOP(p_fdiv, a / b)
FBINOP(p_fmin, a < b ? a : b)
FBINOP(p_fmax, a > b ? a : b)
FBINOP(p_fatan2, atan2(a, b))
FBINOP(p_fpow, pow(a, b))
FUNOP(p_fnegate, -a)
FUNOP(p_fabs, fabs(a))
FUNOP(p_fsqrt, sqrt(a))
FUNOP(p_fsin, sin(a))
FUNOP(p_fcos, cos(a))
FUNOP(p_ftan, tan(a))
FUNOP(p_fasin, asin(a))
FUNOP(p_facos, acos(a))
FUNOP(p_fexp, exp(a))
FUNOP(p_fln, log(a))
FUNOP(p_ffloor, floor(a))
FUNOP(p_fround, round(a))
FCMP(p_flt, a < b)
FCMP(p_fgt, a > b)
FCMP(p_fle, a <= b)
FCMP(p_fge, a >= b)
FCMP(p_feq, a == b)
PRIM(p_f0eq) { PUSH(FPOP() == 0.0 ? -1 : 0); }
PRIM(p_f0lt) { PUSH(FPOP() < 0.0 ? -1 : 0); }
PRIM(p_stof) { FPUSH((Float)POP()); }
PRIM(p_ftos) { PUSH((Cell)FPOP()); }   /* truncates toward zero */
PRIM(p_fdup) { Float a = FPOP(); FPUSH(a); FPUSH(a); }
PRIM(p_fdrop) { (void)FPOP(); }
PRIM(p_fswap) { Float b = FPOP(), a = FPOP(); FPUSH(b); FPUSH(a); }
PRIM(p_fover) { Float b = FPOP(), a = FPOP(); FPUSH(a); FPUSH(b); FPUSH(a); }
PRIM(p_frot) { Float c = FPOP(), b = FPOP(), a = FPOP(); FPUSH(b); FPUSH(c); FPUSH(a); }
PRIM(p_fdepth) { PUSH(f->t->fp); }
PRIM(p_ffetch) { Float x; memcpy(&x, addr(f, POP()), sizeof(x)); FPUSH(x); }
PRIM(p_fstore) { Float x = FPOP(); memcpy(addr(f, POP()), &x, sizeof(x)); }
PRIM(p_sffetch) { float x; memcpy(&x, addr(f, POP()), 4); FPUSH(x); }
PRIM(p_sfstore) { float x = (float)FPOP(); memcpy(addr(f, POP()), &x, 4); }

/* ---- output ---- */

/* x in the current base */
static const char *format_number(Forth *f, Cell x, int is_unsigned) {
    char *p = f->numbuf + sizeof(f->numbuf) - 1;
    UCell u = is_unsigned || x >= 0 ? (UCell)x : (UCell)0 - (UCell)x;
    UCell base = f->base >= 2 && f->base <= 36 ? (UCell)f->base : 10;

    *p = 0;
    do {
        UCell d = u % base;

        *--p = (char)(d < 10 ? '0' + d : 'a' + d - 10);
        u /= base;
    } while (u != 0);
    if (!is_unsigned && x < 0) {
        *--p = '-';
    }
    return p;
}
PRIM(p_dot) { forth_printf(f, "%s ", format_number(f, POP(), 0)); }
PRIM(p_udot) { forth_printf(f, "%s ", format_number(f, POP(), 1)); }
PRIM(p_fdot) { forth_printf(f, "%g ", FPOP()); }
PRIM(p_dots) {
    int i;

    forth_printf(f, "<%d> ", f->t->sp);
    for (i = 0; i < f->t->sp; i++) {
        forth_printf(f, "%s ", format_number(f, f->t->ds[i], 0));
    }
    if (f->t->fp > 0) {
        forth_printf(f, " F: ");
        for (i = 0; i < f->t->fp; i++) {
            forth_printf(f, "%g ", f->t->fs[i]);
        }
    }
}
PRIM(p_emit) { char c = (char)POP(); forth_type(f, &c, 1); }
PRIM(p_type) { Cell n = POP(), a = POP(); if (n > 0) forth_type(f, (const char *)a, (size_t)n); }
PRIM(p_cr) { forth_type(f, "\n", 1); }
PRIM(p_space) { forth_type(f, " ", 1); }
PRIM(p_spaces) { Cell n = POP(); while (n-- > 0) forth_type(f, " ", 1); }
PRIM(p_base) { PUSH(&f->base); }
PRIM(p_state) { PUSH(&f->compiling); }

/* ---- the outer interpreter: parsing ---- */

static Source *source(Forth *f) {
    if (f->nsrc == 0) {
        forth_error(f, "no input");
    }
    return &f->src[f->nsrc - 1];
}

/* the next space-delimited word of the input (length 0 at the end) */
static const char *parse_word(Forth *f, size_t *len) {
    Source *s = source(f);
    size_t start;

    while (s->pos < s->len && isspace((unsigned char)s->text[s->pos])) {
        if (s->text[s->pos] == '\n') {
            s->line++;
        }
        s->pos++;
    }
    start = s->pos;
    while (s->pos < s->len && !isspace((unsigned char)s->text[s->pos])) {
        s->pos++;
    }
    *len = s->pos - start;
    return s->text + start;
}

/* the input up to (not including) the delimiter, which is skipped; one leading space skipped */
static const char *parse_until(Forth *f, char delim, size_t *len) {
    Source *s = source(f);
    size_t start;

    if (s->pos < s->len && s->text[s->pos] == ' ') {
        s->pos++;
    }
    start = s->pos;
    while (s->pos < s->len && s->text[s->pos] != delim) {
        if (s->text[s->pos] == '\n') {
            s->line++;
        }
        s->pos++;
    }
    *len = s->pos - start;
    if (s->pos < s->len) {
        s->pos++;
    }
    return s->text + start;
}

/* a name for a definition: it must be there */
static const char *parse_name(Forth *f, size_t *len, const char *who) {
    const char *name = parse_word(f, len);

    if (*len == 0) {
        forth_error(f, "%s: name expected", who);
    }
    return name;
}

static Word *parse_find(Forth *f, const char *who) {
    size_t len;
    const char *name = parse_name(f, &len, who);
    Word *w = forth_find(f, name, len);

    if (w == NULL) {
        forth_error(f, "%s: unknown word %.*s", who, (int)len, name);
    }
    return w;
}

static int digit_value(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    c = (char)tolower((unsigned char)c);
    return c >= 'a' && c <= 'z' ? c - 'a' + 10 : 99;
}

/* an integer: optional '-', a base prefix ($ hex, # decimal, % binary), or 'c' for a character */
static int parse_int(Forth *f, const char *s, size_t n, Cell *out) {
    Cell base = f->base, v = 0;
    int neg = 0;
    size_t i = 0;

    if (n == 3 && s[0] == '\'' && s[2] == '\'') {
        *out = (unsigned char)s[1];
        return 1;
    }
    if (i < n && (s[i] == '$' || s[i] == '#' || s[i] == '%')) {
        base = s[i] == '$' ? 16 : s[i] == '#' ? 10 : 2;
        i++;
    }
    if (i < n && s[i] == '-') {
        neg = 1;
        i++;
    }
    if (i == n) {
        return 0;
    }
    for (; i < n; i++) {
        int d = digit_value(s[i]);

        if (d >= base) {
            return 0;
        }
        v = v * base + d;
    }
    *out = neg ? -v : v;
    return 1;
}

/* a float: digits with a '.' or an exponent (1.5 -2. 3e2 1.5e-3) */
static int parse_float(const char *s, size_t n, Float *out) {
    char buf[64], *end;
    size_t i;
    int digit = 0, mark = 0;

    if (n == 0 || n >= sizeof(buf)) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        if (isdigit((unsigned char)s[i])) {
            digit = 1;
        } else if (s[i] == '.' || s[i] == 'e' || s[i] == 'E') {
            mark = 1;
        } else if (s[i] != '-' && s[i] != '+') {
            return 0;
        }
    }
    if (!digit || !mark) {
        return 0;
    }
    memcpy(buf, s, n);
    buf[n] = 0;
    if (buf[n - 1] == 'e' || buf[n - 1] == 'E') {   /* "2e" is 2.0 (the classic Forth float syntax) */
        buf[--n] = 0;
    }
    *out = strtod(buf, &end);
    return n > 0 && end == buf + n;
}

const char *forth_parse_name(Forth *f, size_t *len) {
    return parse_word(f, len);
}

void forth_compile_literal(Forth *f, Cell x) {
    comma(f, (Cell)f->w_lit);
    comma(f, x);
}

/* ---- the outer interpreter ---- */

static void interpret(Forth *f) {
    for (;;) {
        size_t n;
        const char *s = parse_word(f, &n);
        Word *w;
        Cell x;
        Float fx;

        if (n == 0) {
            return;
        }
        w = forth_find(f, s, n);
        if (w != NULL) {
            if (f->compiling && !(w->flags & WORD_IMMEDIATE)) {
                comma(f, (Cell)w);
            } else {
                forth_execute(f, w);
            }
        } else if (parse_int(f, s, n, &x)) {
            if (f->compiling) {
                comma(f, (Cell)f->w_lit);
                comma(f, x);
            } else {
                PUSH(x);
            }
        } else if (parse_float(s, n, &fx)) {
            if (f->compiling) {
                comma(f, (Cell)f->w_flit);
                comma_float(f, fx);
            } else {
                FPUSH(fx);
            }
        } else {
            forth_error(f, "unknown word %.*s", (int)n, s);
        }
    }
}

static void push_source(Forth *f, const char *text, size_t len, const char *name, char *owned) {
    Source *s;

    if (f->nsrc >= SOURCE_DEPTH) {
        free(owned);
        forth_error(f, "includes nested too deep");
    }
    s = &f->src[f->nsrc++];
    s->text = text;
    s->len = len;
    s->pos = 0;
    s->name = name;
    s->line = 1;
    s->owned = owned;
}

static void pop_source(Forth *f) {
    Source *s = &f->src[--f->nsrc];

    free(s->owned);
    s->owned = NULL;
}

/* interpret text inside the current catch point (errors propagate) */
static void evaluate(Forth *f, const char *text, size_t len, const char *name, char *owned) {
    int level = f->nsrc;

    push_source(f, text, len, name, owned);
    interpret(f);
    while (f->nsrc > level) {
        pop_source(f);
    }
}

static char *read_file(const char *path, size_t *len) {
    FILE *fp = fopen(path, "rb");
    char *buf;
    long n;

    if (fp == NULL) {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    n = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buf = malloc((size_t)n + 1);
    if (buf == NULL || fread(buf, 1, (size_t)n, fp) != (size_t)n) {
        free(buf);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    buf[n] = 0;
    *len = (size_t)n;
    return buf;
}

/* include a file: names are kept for error messages for as long as the system lives */
static void include_file(Forth *f, const char *path, size_t plen) {
    char *name = malloc(plen + 1), *text;
    size_t len;

    Manifest saved = f->m;

    memcpy(name, path, plen);
    name[plen] = 0;
    text = read_file(name, &len);
    if (text == NULL) {
        forth_error(f, "include: can't read %s", name);
        /* (name leaks on this path: errors are rare and it is tiny) */
    }
    f->m = fresh_manifest(f, 0);
    evaluate(f, text, len, name, text);
    f->m = saved;   /* (on an error the catch point puts its own view back) */
}

/* run text with a catch point; on error: back to interpreting, stacks emptied (ABORT) */
int forth_eval(Forth *f, const char *text, size_t len, const char *name) {
    jmp_buf jb, *prev = f->catch;
    int level = f->nsrc, depth = f->depth;
    Cell *ip = IP;
    Task *t = f->t;
    Manifest saved = f->m;

    f->m = f->listener;   /* text from the prompt sees every loaded vocabulary */
    if (setjmp(jb) != 0) {
        f->listener = f->m;
        f->m = saved;
        f->catch = prev;
        while (f->nsrc > level) {
            pop_source(f);
        }
        f->depth = depth;
        f->t = t;
        IP = ip;
        f->yielded = 0;
        f->compiling = 0;
        t->sp = t->rp = t->fp = 0;
        return -1;
    }
    f->catch = &jb;
    evaluate(f, text, len, name, NULL);
    f->catch = prev;
    f->listener = f->m;   /* (USING: and IN: at the prompt last) */
    f->m = saved;
    return 0;
}

int forth_include(Forth *f, const char *path) {
    jmp_buf jb, *prev = f->catch;
    int level = f->nsrc, depth = f->depth;
    Manifest saved = f->m;

    if (setjmp(jb) != 0) {
        f->m = saved;
        f->catch = prev;
        while (f->nsrc > level) {
            pop_source(f);
        }
        f->depth = depth;
        f->compiling = 0;
        f->t->sp = f->t->rp = f->t->fp = 0;
        return -1;
    }
    f->catch = &jb;
    include_file(f, path, strlen(path));
    f->catch = prev;
    return 0;
}

int forth_call(Forth *f, Word *w) {
    jmp_buf jb, *prev = f->catch;
    int depth = f->depth;
    Cell *ip = IP;
    Task *t = f->t;
    Manifest saved = f->m;

    if (setjmp(jb) != 0) {
        f->m = saved;
        f->catch = prev;
        f->depth = depth;
        f->t = t;
        IP = ip;
        f->yielded = 0;
        return -1;
    }
    f->catch = &jb;
    forth_execute(f, w);
    f->catch = prev;
    return 0;
}

/* ---- the compiler ---- */

PRIM(p_colon) {
    size_t len;
    const char *name = parse_name(f, &len, ":");
    Word *def = new_word(f, name, len, docol);

    def->flags = WORD_HIDDEN;   /* found again once complete */
    f->compiling = -1;
}
PRIM(p_semicolon) {
    comma(f, (Cell)f->w_exit);
    f->latest->flags &= (uint8_t)~WORD_HIDDEN;
    f->compiling = 0;
}
PRIM(p_noname) {   /* :noname ( -- xt ) a definition without a name */
    Word *def = new_word(f, "", 0, docol);

    def->flags = WORD_HIDDEN;
    PUSH(def);
    f->compiling = -1;
}
PRIM(p_immediate) { f->latest->flags |= WORD_IMMEDIATE; }
PRIM(p_lbracket) { f->compiling = 0; }
PRIM(p_rbracket) { f->compiling = -1; }
PRIM(p_tick) { PUSH(parse_find(f, "'")); }
PRIM(p_brackettick) {
    comma(f, (Cell)f->w_lit);
    comma(f, (Cell)parse_find(f, "[']"));
}
PRIM(p_literal) {
    comma(f, (Cell)f->w_lit);
    comma(f, POP());
}
PRIM(p_fliteral) {
    comma(f, (Cell)f->w_flit);
    comma_float(f, FPOP());
}
PRIM(p_postpone) {   /* compile the next word's compile-time behaviour */
    Word *x = parse_find(f, "postpone");

    if (x->flags & WORD_IMMEDIATE) {
        comma(f, (Cell)x);
    } else {
        comma(f, (Cell)f->w_lit);
        comma(f, (Cell)x);
        comma(f, (Cell)f->w_comma);
    }
}
PRIM(p_recurse) { comma(f, (Cell)f->latest); }
PRIM(p_create) {
    size_t len;
    const char *name = parse_name(f, &len, "create");

    new_word(f, name, len, dovar);
}
PRIM(p_docompile) { comma(f, (Cell)f->w_does); }   /* does> */
PRIM(p_tobody) { Word *x = (Word *)POP(); PUSH(x->body); }
PRIM(p_latest) { PUSH(f->latest); }
PRIM(p_find) {   /* ( addr len -- xt | 0 ) */
    Cell n = POP(), a = POP();

    PUSH(forth_find(f, (const char *)a, (size_t)n));
}
PRIM(p_parse_line) {   /* ( "rest of the line" -- addr len ) */
    size_t len;
    const char *s = parse_until(f, '\n', &len);

    source(f)->line++;
    PUSH(s);
    PUSH(len);
}
PRIM(p_parse_name) {   /* ( "name" -- addr len ) */
    size_t len;
    const char *s = parse_word(f, &len);

    PUSH(s);
    PUSH(len);
}

/* strings and comments */
static void compile_string(Forth *f, const char *s, size_t len) {
    comma(f, (Cell)f->w_litstring);
    comma(f, (Cell)len);
    memcpy(allot(f, len), s, len);
    align_here(f);
}
PRIM(p_squote) {   /* s" text" ( -- addr len ) */
    size_t len;
    const char *s = parse_until(f, '"', &len);

    if (f->compiling) {
        compile_string(f, s, len);
    } else {
        char *buf = f->sbuf[f->nsbuf++ % 16];

        if (len >= sizeof(f->sbuf[0])) {
            len = sizeof(f->sbuf[0]) - 1;
        }
        memcpy(buf, s, len);
        buf[len] = 0;
        PUSH(buf);
        PUSH(len);
    }
}
PRIM(p_dotquote) {   /* ." text" */
    size_t len;
    const char *s = parse_until(f, '"', &len);

    if (f->compiling) {
        compile_string(f, s, len);
        comma(f, (Cell)f->w_type);
    } else {
        forth_type(f, s, len);
    }
}
PRIM(p_dotparen) {   /* .( text) prints now */
    size_t len;
    const char *s = parse_until(f, ')', &len);

    forth_type(f, s, len);
}
PRIM(p_paren) { size_t len; (void)parse_until(f, ')', &len); }
PRIM(p_backslash) { size_t len; (void)parse_until(f, '\n', &len); source(f)->line++; }
PRIM(p_char) {
    size_t len;
    const char *s = parse_name(f, &len, "char");

    PUSH((unsigned char)s[0]);
}
PRIM(p_bracketchar) {
    size_t len;
    const char *s = parse_name(f, &len, "[char]");

    comma(f, (Cell)f->w_lit);
    comma(f, (unsigned char)s[0]);
}
PRIM(p_include) {
    size_t len;
    const char *s = parse_name(f, &len, "include");

    include_file(f, s, len);
}
PRIM(p_included) { Cell n = POP(), a = POP(); include_file(f, (const char *)a, (size_t)n); }
PRIM(p_evaluate) {
    Cell n = POP(), a = POP();

    evaluate(f, (const char *)a, (size_t)n, "evaluate", NULL);
}

/* ---- inspecting the system ---- */

static void list_words(Forth *f, Vocab *v) {
    Word *x;
    int col = (int)strlen(v->name) + 2;

    forth_printf(f, "%s: ", v->name);
    for (x = v->latest; x != NULL; x = x->link) {
        if (x->flags & WORD_HIDDEN || x->len == 0) {
            continue;
        }
        if (col + x->len + 1 > 78) {
            forth_type(f, "\n  ", 3);
            col = 2;
        }
        forth_printf(f, "%s ", x->name);
        col += x->len + 1;
    }
    forth_type(f, "\n", 1);
}
PRIM(p_words) {   /* the words this source sees, by vocabulary */
    int k;

    for (k = 0; k < f->m.norder; k++) {
        if (f->m.order[k] != f->core) {
            list_words(f, f->m.order[k]);
        }
    }
    list_words(f, f->core);
}
PRIM(p_vocab_words) {   /* vocab-words name */
    size_t len;
    const char *name = parse_name(f, &len, "vocab-words");
    Vocab *v = vocab_find(f, name, len);

    if (v == NULL) {
        forth_error(f, "no vocabulary %.*s", (int)len, name);
    }
    list_words(f, v);
}
PRIM(p_vocabs) {   /* every vocabulary */
    Vocab *v;

    for (v = f->vocabs; v != NULL; v = v->next) {
        forth_printf(f, "%s%s ", v->name, v->state == VOCAB_LOADING ? "(loading)" : "");
    }
    forth_type(f, "\n", 1);
}

/* IN: name - definitions from here go into the vocabulary `name` */
PRIM(p_in) {
    size_t len;
    const char *name = parse_name(f, &len, "IN:");
    char buf[VOCAB_NAME_MAX + 1];
    Vocab *v;

    snprintf(buf, sizeof(buf), "%.*s", (int)len, name);
    v = forth_vocab(f, buf);
    if (v->state == VOCAB_NEW) {
        v->state = VOCAB_LOADED;   /* (a file included directly, not through USING:) */
    }
    forth_set_current(f, v);
}

/* the file for vocabulary a.b: <root>/a/b.fs */
static int vocab_file(Forth *f, const char *name, size_t len, char *path, size_t n) {
    int r;
    size_t i;

    for (r = 0; r < f->nroots; r++) {
        FILE *fp;
        int k = snprintf(path, n, "%s/", f->roots[r]);

        for (i = 0; i < len && (size_t)k + 4 < n; i++) {
            path[k++] = name[i] == '.' ? '/' : name[i];
        }
        snprintf(path + k, n - (size_t)k, ".fs");
        if ((fp = fopen(path, "rb")) != NULL) {
            fclose(fp);
            return 1;
        }
    }
    return 0;
}

/* a vocabulary, loaded from its file if it isn't yet */
static Vocab *require(Forth *f, const char *name, size_t len) {
    Vocab *v = vocab_find(f, name, len);
    char path[1024], buf[VOCAB_NAME_MAX + 1];
    jmp_buf jb, *prev = f->catch;

    if (v != NULL && v->state == VOCAB_LOADED) {
        return v;
    }
    if (v != NULL && v->state == VOCAB_LOADING) {
        forth_error(f, "USING: %.*s - it is still loading (vocabularies using each other)", (int)len, name);
    }
    if (len > VOCAB_NAME_MAX || !vocab_file(f, name, len, path, sizeof(path))) {
        forth_error(f, "USING: no vocabulary %.*s (no %.*s.fs in the scripts)", (int)len, name, (int)len, name);
    }
    snprintf(buf, sizeof(buf), "%.*s", (int)len, name);
    v = forth_vocab(f, buf);
    v->state = VOCAB_LOADING;
    if (setjmp(jb) != 0) {   /* the file failed: not loaded after all; pass the error on */
        f->catch = prev;
        v->state = VOCAB_NEW;
        longjmp(*prev, 1);
    }
    f->catch = &jb;
    include_file(f, path, strlen(path));
    f->catch = prev;
    v->state = VOCAB_LOADED;
    return v;
}

/* USING: a b c ; - this source uses these vocabularies (loading them if needed) */
PRIM(p_using) {
    for (;;) {
        size_t len;
        const char *name = parse_name(f, &len, "USING:");

        if (len == 1 && name[0] == ';') {
            return;
        }
        order_add(&f->m, require(f, name, len));
    }
}
PRIM(p_use) {   /* USE: name - one vocabulary */
    size_t len;
    const char *name = parse_name(f, &len, "USE:");

    order_add(&f->m, require(f, name, len));
}

/* <PRIVATE ... PRIVATE> - helpers in name.private: visible here, not to other users */
PRIM(p_private_begin) {
    char buf[VOCAB_NAME_MAX + 1];
    Vocab *v;

    snprintf(buf, sizeof(buf), "%.55s.private", f->m.current->name);
    v = forth_vocab(f, buf);
    v->parent = f->m.current;
    v->state = VOCAB_LOADED;
    forth_set_current(f, v);
}
PRIM(p_private_end) {
    if (f->m.current->parent == NULL) {
        forth_error(f, "PRIVATE> without <PRIVATE");
    }
    f->m.current = f->m.current->parent;
}

static const char *word_kind(Word *x) {
    if (x->code == docol) return "colon";
    if (x->code == dovar) return "variable";
    if (x->code == docon) return "constant";
    if (x->code == dofcon) return "fconstant";
    if (x->code == dodoes) return "does>";
    return "primitive";
}

/* the word at x, if x is one (a literal that is an execution token) */
static Word *as_word(Forth *f, Cell x) {
    Vocab *v;
    Word *w;

    for (v = f->vocabs; v != NULL; v = v->next) {
        for (w = v->latest; w != NULL; w = w->link) {
            if ((Cell)w == x) {
                return w;
            }
        }
    }
    return NULL;
}

/* show a word: a colon definition is decompiled */
static void see_code(Forth *f, Cell *p, Cell *end, int newline) {
    while (p < end) {
        Word *x = (Word *)*p++;

        if (x == f->w_lit) {
            Word *lw = as_word(f, *p);

            if (lw != NULL) {
                forth_printf(f, "['] %s ", lw->name);
            } else {
                forth_printf(f, "%s ", format_number(f, *p, 0));
            }
            p++;
        } else if (x == f->w_flit) {
            Float v;

            memcpy(&v, p, sizeof(v));
            p += FCELLS;
            forth_printf(f, "%g ", v);
        } else if (x == f->w_litstring) {
            Cell n = *p++;

            forth_printf(f, "s\" %.*s\" ", (int)n, (const char *)p);
            p += CELLS_FOR((size_t)n);
        } else if (x == f->w_exit && p == end) {
            if (newline) {
                forth_type(f, ";", 1);
            }
        } else if (x->operands == 1 && x->len == 6 && memcmp(x->name, "branch", 6) == 0 && (Cell *)*p > p &&
                   ((Word *)(p + 1))->code == docol && ((Word *)(p + 1))->len == 0) {
            /* a quotation: a branch over a headless word, then the word as a literal */
            Word *q = (Word *)(p + 1);
            Cell *after = (Cell *)*p;

            forth_type(f, "[: ", 3);
            see_code(f, q->body, after - 1, 0);   /* (its body, up to its exit) */
            forth_type(f, ";] ", 3);
            p = after + 2;   /* past the literal that leaves it */
        } else if (x->operands == 1) {
            forth_printf(f, "%s(%+ld) ", x->name, (long)(((Cell *)*p - p) * (Cell)sizeof(Cell)));
            p++;
        } else {
            forth_printf(f, "%s ", x->len > 0 ? x->name : "?");
        }
    }
    if (newline) {
        forth_type(f, "\n", 1);
    }
}
PRIM(p_see) {
    Word *x = parse_find(f, "see"), *next = word_after(f, x);
    Cell *end = next != NULL ? (Cell *)next : (Cell *)f->here;

    if (x->code == docol) {
        forth_printf(f, ": %s %s", x->name, x->flags & WORD_IMMEDIATE ? "( immediate ) " : "");
        see_code(f, x->body, end, 1);
    } else if (x->code == dodoes) {
        forth_printf(f, "%s (made by a defining word); does> ", x->name);
        see_code(f, x->does, (Cell *)x, 1);
    } else {
        forth_printf(f, "%s is a %s\n", x->name, word_kind(x));
    }
}
PRIM(p_bye) { f->bye = 1; }

/* ---- quotations: [: ... ;] leaves the code between as an execution token ---- */

/* the code goes inline: a branch over a headless word, then the word as a literal. At the
 * prompt it is compiled the same way, then the token left on the stack */
PRIM(p_quote_begin) {
    Cell *slot = NULL;
    Word *q;

    if (f->compiling) {
        comma(f, (Cell)forth_find(f, "branch", 6));
        comma(f, 0);
        slot = (Cell *)(f->here - sizeof(Cell));
    }
    align_here(f);
    q = allot(f, sizeof(Word));   /* not linked into a vocabulary: it has no name */
    memset(q, 0, sizeof(Word));
    q->code = docol;
    q->vocab = f->m.current;
    PUSH(slot);
    PUSH(q);
    f->compiling = -1;
}
PRIM(p_quote_end) {
    Word *q = (Word *)POP();
    Cell *slot = (Cell *)POP();

    comma(f, (Cell)f->w_exit);
    if (slot != NULL) {   /* inside a definition: jump here, then the token as a literal */
        *slot = (Cell)f->here;
        comma(f, (Cell)f->w_lit);
        comma(f, (Cell)q);
    } else {
        f->compiling = 0;
        PUSH(q);
    }
}

/* ---- lists ---- */

static List *list_arg(Forth *f, Cell x) {
    if (x == 0) {
        forth_error(f, "not a list (0)");
    }
    return (List *)x;
}
static void list_add(List *l, Cell x) {
    if (l->n == l->cap) {
        l->cap = l->cap * 2 + 8;
        l->items = realloc(l->items, (size_t)l->cap * sizeof(Cell));
    }
    l->items[l->n++] = x;
}
PRIM(p_list) { PUSH(calloc(1, sizeof(List))); }   /* ( -- l ) */
PRIM(p_list_free) {   /* ( l -- ) */
    List *l = list_arg(f, POP());

    free(l->items);
    free(l);
}
PRIM(p_push) { List *l = list_arg(f, POP()); list_add(l, POP()); }   /* ( x l -- ) */
PRIM(p_pop) {   /* ( l -- x ) the last */
    List *l = list_arg(f, POP());

    if (l->n == 0) {
        forth_error(f, "pop: the list is empty");
    }
    PUSH(l->items[--l->n]);
}
static Cell index_arg(Forth *f, List *l, Cell i) {
    if (i < 0) {
        i += l->n;   /* -1: the last */
    }
    if (i < 0 || i >= l->n) {
        forth_error(f, "list index %ld out of range (length %ld)", (long)i, (long)l->n);
    }
    return i;
}
PRIM(p_nth) { List *l = list_arg(f, POP()); Cell i = POP(); PUSH(l->items[index_arg(f, l, i)]); }   /* ( i l -- x ) */
PRIM(p_nth_store) {   /* ( x i l -- ) */
    List *l = list_arg(f, POP());
    Cell i = POP(), x = POP();

    l->items[index_arg(f, l, i)] = x;
}
PRIM(p_length) { PUSH(list_arg(f, POP())->n); }   /* ( l -- n ) */
PRIM(p_list_clear) { list_arg(f, POP())->n = 0; }
/* { a b c } - the values put on the stack in between, as a new list */
PRIM(p_brace_open) {
    Task *t = f->t;

    if (t->nmarks >= 16) {
        forth_error(f, "{ nested too deep");
    }
    t->marks[t->nmarks++] = t->sp;
}
PRIM(p_brace_close) {
    Task *t = f->t;
    List *l;
    int i, from;

    if (t->nmarks == 0) {
        forth_error(f, "} without {");
    }
    from = t->marks[--t->nmarks];
    if (from > t->sp) {
        forth_error(f, "}: the stack lost items since {");
    }
    l = calloc(1, sizeof(List));
    for (i = from; i < t->sp; i++) {
        list_add(l, t->ds[i]);
    }
    t->sp = from;
    PUSH(l);
}

/* catch ( xt -- 0 | -1 ): run xt; if it fails, put the stacks back as they were and give -1
 * (the message is in `error-message`) */
PRIM(p_catch) {
    Word *x = (Word *)POP();
    jmp_buf jb, *prev = f->catch;
    Task *t = f->t;
    int sp = t->sp, rp = t->rp, fp = t->fp, depth = f->depth, nsrc = f->nsrc, silent = f->silent;
    Cell *ip = IP, compiling = f->compiling;
    Manifest saved = f->m;

    if (setjmp(jb) != 0) {
        f->m = saved;
        f->catch = prev;
        f->t = t;
        t->sp = sp;
        t->rp = rp;
        t->fp = fp;
        f->depth = depth;
        f->silent = silent;
        while (f->nsrc > nsrc) {
            pop_source(f);
        }
        IP = ip;
        f->compiling = compiling;
        PUSH(-1);
        return;
    }
    f->catch = &jb;
    f->silent++;
    forth_execute(f, x);
    f->silent--;
    f->catch = prev;
    PUSH(0);
}
PRIM(p_error_message) { PUSH(f->error); PUSH(strlen(f->error)); }

/* ---- tasks ---- */

Task *forth_spawn(Forth *f, Word *w) {
    Task *t = calloc(1, sizeof(Task)), **end;

    t->boot[0] = (Cell)w;
    t->boot[1] = (Cell)f->w_halt;
    t->ip = t->boot;
    t->state = TASK_READY;
    t->id = ++f->next_task_id;
    snprintf(t->name, sizeof(t->name), "%s", w->len > 0 ? w->name : ":noname");
    for (end = &f->tasks; *end != NULL; end = &(*end)->next) {
    }
    *end = t;
    return t;
}

static void task_turn(Forth *f, Task *t) {
    jmp_buf jb, *prev = f->catch;
    Task *saved = f->t;
    int depth = f->depth, task_depth = f->task_depth;

    if (setjmp(jb) != 0) {
        forth_printf(f, "task %d (%s) stopped by the error\n", t->id, t->name);
        t->state = TASK_DONE;
    } else {
        f->catch = &jb;
        f->t = t;
        f->yielded = 0;
        f->depth++;
        f->task_depth = f->depth;
        run(f);
        if (!f->yielded) {
            t->state = TASK_DONE;
        }
    }
    f->catch = prev;
    f->t = saved;
    f->depth = depth;
    f->task_depth = task_depth;
    f->yielded = 0;
}

void forth_run_tasks(Forth *f) {
    Task **pp = &f->tasks;

    f->frame++;
    while (*pp != NULL) {
        Task *t = *pp;

        if (t->state == TASK_WAITING && f->frame >= t->wake) {
            t->state = TASK_READY;
        }
        if (t->state == TASK_READY) {
            task_turn(f, t);
        }
        if (t->state == TASK_DONE) {
            *pp = t->next;
            free(t);
        } else {
            pp = &t->next;
        }
    }
}

static void yield_check(Forth *f, const char *who) {
    if (f->t == f->main || f->depth != f->task_depth) {
        forth_error(f, "%s: only a task can wait (and not from inside a C callback)", who);
    }
}
PRIM(p_yield) {
    yield_check(f, "yield");
    f->yielded = 1;
}
PRIM(p_wait) {   /* ( frames -- ) */
    Cell n = POP();

    yield_check(f, "wait");
    f->t->state = TASK_WAITING;
    f->t->wake = f->frame + (n > 0 ? n : 1);
    f->yielded = 1;
}
PRIM(p_spawn) { PUSH(forth_spawn(f, (Word *)POP())->id); }   /* ( xt -- id ) */
PRIM(p_kill) {   /* ( id -- ) */
    Cell id = POP();
    Task *t;

    for (t = f->tasks; t != NULL; t = t->next) {
        if (t->id == id) {
            t->state = TASK_DONE;
            if (t == f->t) {   /* killing itself: stop now */
                f->yielded = 1;
            }
        }
    }
}
PRIM(p_tick_tasks) {   /* ( frames -- ) run the scheduler (from the interpreter: tests, the console) */
    Cell n = POP();

    if (f->t != f->main) {
        forth_error(f, "tick-tasks: only from the interpreter");
    }
    while (n-- > 0) {
        forth_run_tasks(f);
    }
}
PRIM(p_me) { PUSH(f->t == f->main ? 0 : f->t->id); }
PRIM(p_frame) { PUSH(f->frame); }
PRIM(p_tasks) {
    Task *t;

    for (t = f->tasks; t != NULL; t = t->next) {
        forth_printf(f, "%3d %-20s %s\n", t->id, t->name,
                     t->state == TASK_WAITING ? "waiting" : t->state == TASK_DONE ? "done" : "ready");
    }
}

/* ---- setting up ---- */

static void define_core(Forth *f) {
    static const struct {
        const char *name;
        Code code;
    } prims[] = {
        {"dup", p_dup}, {"drop", p_drop}, {"swap", p_swap}, {"over", p_over}, {"rot", p_rot},
        {"-rot", p_mrot}, {"nip", p_nip}, {"tuck", p_tuck}, {"2dup", p_2dup}, {"2drop", p_2drop},
        {"2swap", p_2swap}, {"2over", p_2over}, {"?dup", p_qdup}, {"pick", p_pick}, {"depth", p_depth},
        {">r", p_tor}, {"r>", p_rfrom}, {"r@", p_rfetch}, {"rdrop", p_rdrop},
        {"+", p_add}, {"-", p_sub}, {"*", p_mul}, {"/", p_div}, {"mod", p_mod}, {"/mod", p_divmod},
        {"*/", p_muldiv}, {"negate", p_negate}, {"abs", p_abs}, {"min", p_min}, {"max", p_max},
        {"1+", p_1add}, {"1-", p_1sub}, {"and", p_and}, {"or", p_or}, {"xor", p_xor},
        {"invert", p_invert}, {"lshift", p_lshift}, {"rshift", p_rshift}, {"arshift", p_arshift},
        {"=", p_eq}, {"<>", p_ne}, {"<", p_lt}, {">", p_gt}, {"<=", p_le}, {">=", p_ge},
        {"u<", p_ult}, {"u>", p_ugt}, {"0=", p_0eq}, {"0<>", p_0ne}, {"0<", p_0lt}, {"0>", p_0gt},
        {"@", p_fetch}, {"!", p_store}, {"+!", p_pstore}, {"c@", p_cfetch}, {"c!", p_cstore},
        {"w@", p_wfetch}, {"sw@", p_swfetch}, {"w!", p_wstore}, {"l@", p_lfetch}, {"sl@", p_slfetch},
        {"l!", p_lstore}, {"move", p_move}, {"fill", p_fill}, {"cells", p_cells}, {"floats", p_floats},
        {"here", p_here}, {"allot", p_allot}, {",", p_comma}, {"c,", p_ccomma}, {"f,", p_fcomma},
        {"align", p_align}, {"compare", p_compare},
        {"f+", p_fadd}, {"f-", p_fsub}, {"f*", p_fmul}, {"f/", p_fdiv}, {"fmin", p_fmin},
        {"fmax", p_fmax}, {"fatan2", p_fatan2}, {"f**", p_fpow}, {"fnegate", p_fnegate},
        {"fabs", p_fabs}, {"fsqrt", p_fsqrt}, {"fsin", p_fsin}, {"fcos", p_fcos}, {"ftan", p_ftan},
        {"fasin", p_fasin}, {"facos", p_facos}, {"fexp", p_fexp}, {"fln", p_fln},
        {"ffloor", p_ffloor}, {"fround", p_fround}, {"f<", p_flt}, {"f>", p_fgt}, {"f<=", p_fle},
        {"f>=", p_fge}, {"f=", p_feq}, {"f0=", p_f0eq}, {"f0<", p_f0lt}, {"s>f", p_stof},
        {"f>s", p_ftos}, {"fdup", p_fdup}, {"fdrop", p_fdrop}, {"fswap", p_fswap}, {"fover", p_fover},
        {"frot", p_frot}, {"fdepth", p_fdepth}, {"f@", p_ffetch}, {"f!", p_fstore},
        {"sf@", p_sffetch}, {"sf!", p_sfstore},
        {".", p_dot}, {"u.", p_udot}, {"f.", p_fdot}, {".s", p_dots}, {"emit", p_emit},
        {"type", p_type}, {"cr", p_cr}, {"space", p_space}, {"spaces", p_spaces}, {"base", p_base},
        {"state", p_state},
        {":", p_colon}, {":noname", p_noname}, {"immediate", p_immediate}, {"]", p_rbracket},
        {"'", p_tick}, {"create", p_create}, {">body", p_tobody}, {"latest", p_latest},
        {"find", p_find}, {"parse-name", p_parse_name}, {"parse-line", p_parse_line}, {"char", p_char}, {"include", p_include},
        {"included", p_included}, {"evaluate", p_evaluate}, {"execute", p_execute},
        {"list", p_list}, {"list-free", p_list_free}, {"push", p_push}, {"pop", p_pop}, {"nth", p_nth},
        {"nth!", p_nth_store}, {"length", p_length}, {"list-clear", p_list_clear}, {"{", p_brace_open},
        {"}", p_brace_close},
        {"words", p_words}, {"see", p_see}, {"vocabs", p_vocabs}, {"vocab-words", p_vocab_words},
        {"IN:", p_in}, {"USING:", p_using}, {"USE:", p_use}, {"<PRIVATE", p_private_begin},
        {"PRIVATE>", p_private_end}, {"bye", p_bye}, {"catch", p_catch}, {"error-message", p_error_message},
        {"i", p_i}, {"j", p_j}, {"leave", p_leave}, {"unloop", p_unloop},
        {"yield", p_yield}, {"wait", p_wait}, {"spawn", p_spawn}, {"kill", p_kill}, {"me", p_me},
        {"frame", p_frame}, {"tick-tasks", p_tick_tasks}, {".tasks", p_tasks},
    };
    static const struct {
        const char *name;
        Code code;
    } immediates[] = {
        {";", p_semicolon}, {"[", p_lbracket}, {"[']", p_brackettick}, {"literal", p_literal},
        {"fliteral", p_fliteral}, {"postpone", p_postpone}, {"recurse", p_recurse},
        {"does>", p_docompile}, {"s\"", p_squote}, {".\"", p_dotquote}, {".(", p_dotparen},
        {"(", p_paren}, {"\\", p_backslash}, {"[char]", p_bracketchar}, {"[:", p_quote_begin},
        {";]", p_quote_end},
    };
    size_t i;

    /* the words the compiler lays down (names in parentheses: not for direct use) */
    f->w_lit = prim_word(f, "(lit)", p_lit, 0, 0);
    f->w_flit = prim_word(f, "(flit)", p_flit, 0, 0);
    f->w_litstring = prim_word(f, "(litstring)", p_litstring, 0, 0xFF);
    f->w_exit = prim_word(f, "exit", p_exit, 0, 0);
    f->w_halt = prim_word(f, "(halt)", p_halt, 0, 0);
    f->w_does = prim_word(f, "(does>)", p_does, 0, 0);
    prim_word(f, "branch", p_branch, 0, 1);
    prim_word(f, "0branch", p_0branch, 0, 1);
    prim_word(f, "(do)", p_do, 0, 1);
    prim_word(f, "(?do)", p_qdo, 0, 1);
    prim_word(f, "(loop)", p_loop, 0, 1);
    prim_word(f, "(+loop)", p_ploop, 0, 1);
    for (i = 0; i < sizeof(prims) / sizeof(prims[0]); i++) {
        forth_prim(f, prims[i].name, prims[i].code);
    }
    for (i = 0; i < sizeof(immediates) / sizeof(immediates[0]); i++) {
        prim_word(f, immediates[i].name, immediates[i].code, WORD_IMMEDIATE, 0);
    }
    f->w_comma = forth_find(f, ",", 1);
    f->w_type = forth_find(f, "type", 4);
    forth_constant(f, "cell", (Cell)sizeof(Cell));
}

Forth *forth_new(size_t dict_bytes) {
    Forth *f = calloc(1, sizeof(Forth));

    f->mem = malloc(dict_bytes);
    f->here = f->mem;
    f->end = f->mem + dict_bytes;
    f->base = 10;
    f->core = forth_vocab(f, "forth");
    f->core->state = VOCAB_LOADED;
    f->m.current = f->core;
    f->m.order[f->m.norder++] = f->core;
    f->main = calloc(1, sizeof(Task));
    snprintf(f->main->name, sizeof(f->main->name), "interpreter");
    f->t = f->main;
    forth_set_output(f, NULL, NULL);
    define_core(f);
    f->listener = fresh_manifest(f, 1);
    return f;
}

void forth_free(Forth *f) {
    while (f->tasks != NULL) {
        Task *t = f->tasks;

        f->tasks = t->next;
        free(t);
    }
    while (f->nsrc > 0) {
        pop_source(f);
    }
    while (f->vocabs != NULL) {
        Vocab *v = f->vocabs;

        f->vocabs = v->next;
        free(v);
    }
    free(f->main);
    free(f->mem);
    free(f);
}
