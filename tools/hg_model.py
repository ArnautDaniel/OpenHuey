#!/usr/bin/env python3
"""Convert a character model (.PCK, + .TEX textures) to glTF binary (.glb).

    tools/hg_model.py HEW_000.PCK --tex HEW_000.TEX -o hewie.glb

Format, from the game's loader and renderer (see docs/model_format.md):
- PCK: u32 count (4), then resource offsets. Resource 0 = skeleton + meshes.
- Skeleton: u32 bones, u32 mesh table offset, ...; 0x70-byte bone records at +0x10
  (s32 parent, ..., inverse bind matrix at +0x30, row-vector convention).
- Mesh table: u32 parts; 0x30-byte part records at +0x10, offsets relative to the record:
  vertex count, 6 streams, texture, flags, palette size, palette offset, influences.
  Streams: positions (3 x s16 deltas from a 3 x s32 start, /4096), UVs (2 x u16 /32768),
  normals (3 x s16 /32768), weights (n x u16 /32768), bone slots (n x u8, /4 = palette
  slot), strip flags (u8: 1 = no triangle at this vertex).
- TEX: u32 count, 16-byte records (psm 0x13 = 8-bit indexed, w, h, ?, ?, offset from the
  record), 8-bit pixels followed by a 256-entry RGBA palette (PS2 CLUT order, alpha 0x80 = 1).
"""
import argparse
import json
import math
import struct
import zlib
from pathlib import Path

POS_SCALE = 1.0 / 4096
UNIT = 1.0 / 32768


# ---------------------------------------------------------------- matrices (row-vector, row-major)
def mat_inv(m):
    """Inverse of a 4x4 matrix given as 16 floats (row-major)."""
    a = [list(m[r * 4:r * 4 + 4]) + [1.0 if r == c else 0.0 for c in range(4)] for r in range(4)]
    for c in range(4):
        p = max(range(c, 4), key=lambda r: abs(a[r][c]))
        a[c], a[p] = a[p], a[c]
        pv = a[c][c]
        a[c] = [v / pv for v in a[c]]
        for r in range(4):
            if r != c and a[r][c] != 0.0:
                f = a[r][c]
                a[r] = [x - f * y for x, y in zip(a[r], a[c])]
    return [a[r][4 + c] for r in range(4) for c in range(4)]


def mat_mul(a, b):
    return [sum(a[r * 4 + k] * b[k * 4 + c] for k in range(4)) for r in range(4) for c in range(4)]


# ---------------------------------------------------------------- model
def read_model(d: bytes):
    count = struct.unpack_from("<I", d, 0)[0]
    res = struct.unpack_from(f"<{count}I", d, 4)
    r0 = res[0]
    nbones, mesh_off = struct.unpack_from("<iI", d, r0)
    bones = []
    for i in range(nbones):
        b = r0 + 0x10 + i * 0x70
        parent = struct.unpack_from("<i", d, b)[0]
        ibm = list(struct.unpack_from("<16f", d, b + 0x30))
        bones.append((parent, ibm))

    mt = r0 + mesh_off
    nparts = struct.unpack_from("<I", d, mt)[0]
    parts = []
    for i in range(nparts):
        b = mt + 0x10 + i * 0x30
        n, o_pos, o_uv, o_nrm, o_w, o_bi, o_fl, tex, flags, npal, o_pal, infl = struct.unpack_from("<12I", d, b)
        if n == 0:
            continue
        pal = list(d[b + o_pal:b + o_pal + npal])
        p = list(struct.unpack_from("<3i", d, b + o_pos))
        verts = []
        for k in range(n):
            dp = struct.unpack_from("<3h", d, b + o_pos + 0x10 + k * 6)
            p = [p[j] + dp[j] for j in range(3)]
            uv = struct.unpack_from("<2H", d, b + o_uv + k * 4)
            nrm = struct.unpack_from("<3h", d, b + o_nrm + k * 6)
            w = struct.unpack_from(f"<{infl}H", d, b + o_w + k * 2 * infl)
            bi = d[b + o_bi + k * infl:b + o_bi + (k + 1) * infl]
            fl = d[b + o_fl + k]
            joints = [pal[x // 4] for x in bi]
            weights = [x * UNIT for x in w]
            if sum(weights) == 0.0:
                weights[0] = 1.0
            verts.append(dict(
                pos=[v * POS_SCALE for v in p],
                uv=[uv[0] * UNIT, uv[1] * UNIT],
                nrm=[v * UNIT for v in nrm],
                joints=joints, weights=weights, flag=fl))
        parts.append(dict(tex=tex, flags=flags, verts=verts))

    # rigid parts, each on one bone (the eyeballs, ...): u32 count at resource 0 + header[2],
    # 0x20-byte records at +0x10: vertex count, positions, UVs, normals, strip flags (offsets
    # from the record), texture, ?, bone
    rt = r0 + struct.unpack_from("<I", d, r0 + 8)[0]
    for i in range(struct.unpack_from("<I", d, rt)[0]):
        b = rt + 0x10 + i * 0x20
        n, o_pos, o_uv, o_nrm, o_fl, tex, _, bone = struct.unpack_from("<8i", d, b)
        p = list(struct.unpack_from("<3i", d, b + o_pos))
        verts = []
        for k in range(n):
            dp = struct.unpack_from("<3h", d, b + o_pos + 0x10 + k * 6)
            p = [p[j] + dp[j] for j in range(3)]
            uv = struct.unpack_from("<2H", d, b + o_uv + k * 4)
            nrm = struct.unpack_from("<3h", d, b + o_nrm + k * 6)
            verts.append(dict(pos=[v * POS_SCALE for v in p], uv=[uv[0] * UNIT, uv[1] * UNIT],
                              nrm=[v * UNIT for v in nrm], joints=[bone], weights=[1.0],
                              flag=d[b + o_fl + k]))
        parts.append(dict(tex=tex, flags=0, verts=verts))
    return bones, parts


def strip_triangles(verts):
    """Triangle list from the strip flags (1 = vertex starts a strip / draws nothing).
    Winding: each triangle is turned to agree with its vertex normals."""
    tris = []
    for k in range(2, len(verts)):
        if verts[k]["flag"]:  # (strips run on across the 48-vertex VU1 batches: the GS keeps its queue)
            continue
        a, b, c = k - 2, k - 1, k
        pa, pb, pc = verts[a]["pos"], verts[b]["pos"], verts[c]["pos"]
        e1 = [pb[j] - pa[j] for j in range(3)]
        e2 = [pc[j] - pa[j] for j in range(3)]
        fn = [e1[1] * e2[2] - e1[2] * e2[1], e1[2] * e2[0] - e1[0] * e2[2], e1[0] * e2[1] - e1[1] * e2[0]]
        vn = [verts[a]["nrm"][j] + verts[b]["nrm"][j] + verts[c]["nrm"][j] for j in range(3)]
        if sum(fn[j] * vn[j] for j in range(3)) < 0:
            b, c = c, b
        tris.append((a, b, c))
    return tris


# ---------------------------------------------------------------- textures
def read_textures(d: bytes):
    count = struct.unpack_from("<I", d, 0)[0]
    out = []
    for i in range(count):
        r = 0x10 + i * 0x10
        psm, w, h, _, _, off = struct.unpack_from("<IHHHHI", d, r)
        if psm != 0x13:
            raise SystemExit(f"texture {i}: pixel format 0x{psm:X} not handled yet")
        px = d[r + off:r + off + w * h]
        clut = d[r + off + w * h:r + off + w * h + 1024]
        pal = []
        for j in range(256):
            # PS2 CLUT order: within each 32 entries, 8..15 and 16..23 are swapped
            s = (j & ~0x18) | ((j & 0x08) << 1) | ((j & 0x10) >> 1)
            cr, cg, cb, ca = clut[s * 4:s * 4 + 4]
            pal.append(bytes((cr, cg, cb, min(255, ca * 2))))
        rows = b"".join(b"\0" + b"".join(pal[x] for x in px[y * w:(y + 1) * w]) for y in range(h))
        out.append(png(w, h, rows))
    return out


def png(w, h, raw_rows):
    def chunk(t, data):
        c = struct.pack(">I", len(data)) + t + data
        return c + struct.pack(">I", zlib.crc32(t + data) & 0xFFFFFFFF)
    return (b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 6, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(raw_rows, 9)) + chunk(b"IEND", b""))


# ---------------------------------------------------------------- glTF
class Gltf:
    def __init__(self):
        self.j = {"asset": {"version": "2.0", "generator": "haunting-ground-decomp hg_model.py"},
                  "buffers": [], "bufferViews": [], "accessors": [], "nodes": [], "meshes": [],
                  "materials": [], "textures": [], "images": [], "samplers": [], "skins": [], "scenes": []}
        self.bin = bytearray()

    def view(self, data: bytes, target=None):
        while len(self.bin) % 4:
            self.bin.append(0)
        v = {"buffer": 0, "byteOffset": len(self.bin), "byteLength": len(data)}
        if target:
            v["target"] = target
        self.bin += data
        self.j["bufferViews"].append(v)
        return len(self.j["bufferViews"]) - 1

    def accessor(self, fmt, ctype, typ, rows, target=None, minmax=False):
        data = b"".join(struct.pack("<" + fmt, *r) for r in rows)
        a = {"bufferView": self.view(data, target), "componentType": ctype, "count": len(rows), "type": typ}
        if minmax:
            a["min"] = [min(r[i] for r in rows) for i in range(len(rows[0]))]
            a["max"] = [max(r[i] for r in rows) for i in range(len(rows[0]))]
        self.j["accessors"].append(a)
        return len(self.j["accessors"]) - 1

    def glb(self) -> bytes:
        self.j["buffers"] = [{"byteLength": len(self.bin)}]
        js = json.dumps({k: v for k, v in self.j.items() if v != []}).encode()
        js += b" " * (-len(js) % 4)
        self.bin += b"\0" * (-len(self.bin) % 4)
        body = (struct.pack("<II", len(js), 0x4E4F534A) + js + struct.pack("<II", len(self.bin), 0x004E4942) + self.bin)
        return struct.pack("<III", 0x46546C67, 2, 12 + len(body)) + body


FLOAT, USHORT, UINT = 5126, 5123, 5125


def build(bones, parts, images, name):
    g = Gltf()
    # skeleton: world bind = inverse of the stored inverse bind; local = world * parent world^-1
    worlds = [mat_inv(ibm) for _, ibm in bones]
    for i, (parent, _) in enumerate(bones):
        local = worlds[i] if parent < 0 else mat_mul(worlds[i], mat_inv(worlds[parent]))
        g.j["nodes"].append({"name": f"bone_{i:02d}", "matrix": [round(v, 7) for v in local]})
    for i, (parent, _) in enumerate(bones):
        if parent >= 0:
            g.j["nodes"][parent].setdefault("children", []).append(i)
    roots = [i for i, (p, _) in enumerate(bones) if p < 0]
    # glTF matrices are column-major column-vector = our row-major row-vector, as stored
    ibm = g.accessor("16f", FLOAT, "MAT4", [tuple(m) for _, m in bones])
    g.j["skins"].append({"inverseBindMatrices": ibm, "joints": list(range(len(bones))), "skeleton": roots[0]})

    g.j["samplers"].append({"magFilter": 9729, "minFilter": 9729, "wrapS": 10497, "wrapT": 10497})
    for i, img in enumerate(images):
        g.j["images"].append({"bufferView": g.view(img), "mimeType": "image/png", "name": f"tex_{i}"})
        g.j["textures"].append({"source": i, "sampler": 0})
    ntex = max([p["tex"] for p in parts] + [-1]) + 1
    for t in range(ntex):
        m = {"name": f"mat_{t}", "doubleSided": True,
             "pbrMetallicRoughness": {"metallicFactor": 0.0, "roughnessFactor": 1.0}}
        if t < len(images):
            m["pbrMetallicRoughness"]["baseColorTexture"] = {"index": t}
            m["alphaMode"] = "MASK"
        g.j["materials"].append(m)

    prims = []
    for p in parts:
        v = p["verts"]
        tris = strip_triangles(v)
        if not tris:
            continue
        j4 = [tuple((x["joints"] + [0, 0, 0, 0])[:4]) for x in v]
        w4 = [tuple((x["weights"] + [0.0, 0.0, 0.0, 0.0])[:4]) for x in v]
        attrs = {
            "POSITION": g.accessor("3f", FLOAT, "VEC3", [tuple(x["pos"]) for x in v], 34962, minmax=True),
            "NORMAL": g.accessor("3f", FLOAT, "VEC3", [tuple(n / (math.sqrt(sum(c * c for c in x["nrm"])) or 1)
                                                            for n in x["nrm"]) for x in v], 34962),
            "TEXCOORD_0": g.accessor("2f", FLOAT, "VEC2", [tuple(x["uv"]) for x in v], 34962),
            "JOINTS_0": g.accessor("4H", USHORT, "VEC4", j4, 34962),
            "WEIGHTS_0": g.accessor("4f", FLOAT, "VEC4", w4, 34962),
        }
        idx = g.accessor("I", UINT, "SCALAR", [(i,) for t in tris for i in t], 34963)
        prims.append({"attributes": attrs, "indices": idx, "material": p["tex"], "mode": 4})
    g.j["meshes"].append({"name": name, "primitives": prims})
    mesh_node = len(g.j["nodes"])
    g.j["nodes"].append({"name": name, "mesh": 0, "skin": 0})
    g.j["scenes"].append({"nodes": roots + [mesh_node]})
    g.j["scene"] = 0
    return g.glb()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("pck", type=Path)
    ap.add_argument("--tex", type=Path, help="texture file (.TEX) for the parts' texture ids")
    ap.add_argument("-o", "--out", type=Path)
    a = ap.parse_args()
    bones, parts = read_model(a.pck.read_bytes())
    images = read_textures(a.tex.read_bytes()) if a.tex else []
    out = a.out or a.pck.with_suffix(".glb")
    out.write_bytes(build(bones, parts, images, a.pck.stem))
    nv = sum(len(p["verts"]) for p in parts)
    print(f"{out}: {len(bones)} bones, {len(parts)} parts, {nv} vertices, {len(images)} textures")


if __name__ == "__main__":
    main()
