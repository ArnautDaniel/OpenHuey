#!/usr/bin/env python3
"""A PCSX2 texture-replacement pack (SLUS-21075/replacements/...) into an OpenHuey mod, matched by
what the pictures show, not by their names (docs/modding.md: textures).

PCSX2 names a replacement <texture hash>-<palette hash>-<bits>.<ext>: hashes of the PS2's memory,
nothing OpenHuey can work out from its library, and the GS's TEX0 bits - the pixel format (low 6
bits), the width and height as powers of two (the next 4 bits each). And a picture is what was in
the PS2's memory at the texture's place when it was dumped: a room's texture with, in the parts
the room leaves empty, whatever else the game had loaded there. So the matching is by tiles (16
pixels of the original a side): each texture of the library, in each of its palettes' pictures,
takes each of its tiles from the picture of its format whose tile in the same place looks the
same (the picture that has most of them first), and keeps its own, enlarged, where none does.
A texture with too few tiles matched is left alone. Every bank that has the texture gets it (the
rooms share theirs), as <bank>.texbank/<i>.png (<i>-<k>.png: palette picture k).

Alpha: PCSX2's pictures keep the PS2's (0..128 opaque), but a pack's editors often saved theirs
0..255 or opaque throughout. Each picture's alpha is compared with the texture's, at both
scales, on the tiles it gives: the nearer is used, and when neither is near, the texture's own,
enlarged.

usage: hdpack.py <library> <pack folder> <mod folder> [--max N] [--threshold T] [--least N]
  library      OpenHuey's library (the folder holding data/)
  pack folder  the pack's (searched for .dds / .png under it; nothing in it is run)
  mod folder   written: <mod>/mod.fs, <mod>/library/data/... and <mod>/hdpack-report.txt (the
               folder's name is the mod's)
  --max N      the largest a texture is written, a side (default 2048: 8x a 256 texture)
  --threshold  the farthest a tile may be from the texture's (mean difference per channel,
               0..255; default 9)
  --least N    the fewest tiles (with detail: not one flat colour) a texture needs (default 8)

Needs Pillow and numpy (pip install pillow numpy). Run it with python3 -I."""
import os
import re
import sys
from concurrent.futures import ProcessPoolExecutor

import numpy as np
from PIL import Image

Image.MAX_IMAGE_PIXELS = None
TILE = 16           # a tile, in the original's pixels
SHRINK = 1          # compared at the original's size (smaller blurs one stone wall into another)
CHUNK = 128         # textures compared at once (memory)
FLAT = 6.0          # a tile whose pixels differ from their mean less than this is one colour
FLAT_NEAR = 4       # ... it is taken only this near (a texture's unused filler: no blotches)
FAR = 48            # a pixel this far off is wrong, not the upscaler's ...
MOST_FAR = 4        # ... and a tile with more of them than this isn't the same (a part of it differs)


def small(im, w, h):
    """RGB at (w / SHRINK, h / SHRINK)"""
    s = im.convert('RGB').resize((w // SHRINK, h // SHRINK), Image.BOX, reducing_gap=2.0)
    return np.asarray(s, dtype=np.uint8)


def tiles(a):
    """(..., H, W, 3) -> (..., H/t, W/t, t*t*3): each tile's pixels"""
    t = TILE // SHRINK
    *lead, H, W, C = a.shape
    a = a.reshape(*lead, H // t, t, W // t, t, C)
    a = np.moveaxis(a, -4, -3)
    return a.reshape(*lead, H // t, W // t, t * t * C)


def library_textures(lib):
    """(psm, w, h) -> ([(bank folder, picture name)], their tiles (N, ty, tx, t*t*3))"""
    groups = {}
    for top, dirs, files in os.walk(os.path.join(lib, 'data')):
        dirs.sort()
        if not top.endswith('.texbank') or 'bank.txt' not in files:
            continue
        for line in open(os.path.join(top, 'bank.txt')):
            f = line.split()
            if len(f) != 6 or f[0] != 'texture':
                continue
            i, w, h, psm, n = map(int, f[1:])
            if w % TILE or h % TILE:
                continue
            for k in range(n):
                name = '%d.png' % i if k == 0 else '%d-%d.png' % (i, k)
                path = os.path.join(top, name)
                if os.path.exists(path):
                    g = groups.setdefault((psm, w, h), ([], []))
                    g[0].append((os.path.relpath(top, lib), name))
                    g[1].append(tiles(small(Image.open(path), w, h)))
    return {k: (names, np.stack(arrs)) for k, (names, arrs) in groups.items()}


BITS = re.compile(r'-([0-9a-f]{8})(?:\.[a-z]+)*\.(dds|png)$', re.I)


def pack_pictures(pack):
    for top, dirs, files in os.walk(pack):
        dirs.sort()
        for f in sorted(files):
            m = BITS.search(f)
            if m:
                b = int(m.group(1), 16)
                yield os.path.join(top, f), b & 0x3F, 1 << ((b >> 6) & 0xF), 1 << ((b >> 10) & 0xF)


def tile_distance(t, q):
    """the mean difference of each tile of the textures t from q's - 255 when part of it is far off"""
    diff = np.abs(t.astype(np.int16) - q)
    d = diff.mean(axis=-1)
    px = diff.reshape(*diff.shape[:-1], -1, 3).mean(axis=-1)
    d[(px > FAR).sum(axis=-1) > MOST_FAR] = 255
    return d


def match(textures, pictures):
    """per group: [(picture, each texture's tiles' distance from the picture's (N, ty, tx))]"""
    found = {}
    for p, (path, psm, tw, th) in enumerate(pictures):
        try:
            im = Image.open(path)
            im.load()
        except Exception as e:
            print('  unreadable: %s (%s)' % (path, e), flush=True)
            continue
        for key, (names, t) in textures.items():
            kpsm, w, h = key
            if kpsm != psm or w > tw or h > th:
                continue
            # (a texture smaller than its TEX0's page is at its top left)
            part = im.crop((0, 0, round(im.width * w / tw), round(im.height * h / th)))
            q = tiles(small(part, w, h)).astype(np.int16)[None]
            d = np.concatenate([tile_distance(t[i:i + CHUNK], q) for i in range(0, len(t), CHUNK)])
            found.setdefault(key, []).append((p, np.minimum(d, 255).astype(np.uint8)))
        if p % 100 == 0:
            print('  %d / %d pictures' % (p, len(pictures)), flush=True)
    return found


def plan(textures, found, threshold, least):
    """the textures given HD tiles: (bank, name, w, h, [(picture, its tiles (ty, tx) mask)]); and
    each picture used, how many textures it gave to"""
    out, used = [], {}
    for key, entries in found.items():
        names, t = textures[key]
        _, w, h = key
        tf = t.astype(np.float32)
        detail = np.abs(tf - tf.mean(axis=-1, keepdims=True)).mean(axis=-1) > FLAT   # (N, ty, tx)
        pics = [p for p, _ in entries]
        d = np.stack([d for _, d in entries])                                      # (P, N, ty, tx)
        good = (d <= threshold) & (detail[None] | (d <= FLAT_NEAR))
        for n in range(len(names)):
            left = np.ones(good.shape[2:], bool)
            takes = []
            # the picture with most of its tiles first, then the next for what's left ...
            while True:
                score = (good[:, n] & left & detail[n]).sum(axis=(1, 2))
                best = int(score.argmax())
                if score[best] < (least if not takes else 1):
                    break
                mask = good[best, n] & left
                takes.append((pics[best], mask))
                left &= ~mask
            if takes:
                out.append((names[n][0], names[n][1], w, h, takes))
                for p, _ in takes:
                    used[p] = used.get(p, 0) + 1
    return out, used


def alpha_for(hd, original, mask):
    """the HD picture's alpha in OpenHuey's 0..255 (doubled or not), or None: use the original's"""
    o = np.asarray(original.getchannel('A'), dtype=np.float32)
    m = np.kron(mask, np.ones((TILE, TILE), bool))
    s = np.asarray(hd.getchannel('A').resize(original.size, Image.BOX), dtype=np.float32)
    as_is = np.abs(s - o)[m].mean()
    doubled = np.abs(np.minimum(s * 255 / 128, 255) - o)[m].mean()
    if min(as_is, doubled) > 16:
        return None
    a = hd.getchannel('A')
    return a if as_is <= doubled else a.point(lambda x: min(255, x * 255 // 128))


def write_one(job):
    lib, mod, pictures, biggest, (bank, name, w, h, takes) = job
    original = Image.open(os.path.join(lib, bank, name)).convert('RGBA')
    scale = 1
    for p, _ in takes:
        path, _psm, tw, th = pictures[p]
        with Image.open(path) as im:
            scale = max(scale, min(im.width / tw, im.height / th))
    scale = max(1, min(int(scale), biggest // max(w, h)))
    W, H, T = w * scale, h * scale, TILE * scale
    out = original.resize((W, H), Image.LANCZOS)
    for p, mask in takes:
        path, _psm, tw, th = pictures[p]
        im = Image.open(path).convert('RGBA')
        part = im.crop((0, 0, round(im.width * w / tw), round(im.height * h / th)))
        if part.size != (W, H):
            part = part.resize((W, H), Image.LANCZOS)
        alpha = alpha_for(part, original, mask)
        part.putalpha(alpha if alpha is not None else out.getchannel('A'))
        cover = Image.fromarray((np.kron(mask, np.ones((T, T), bool)) * 255).astype(np.uint8))
        out.paste(part, (0, 0), cover)
    folder = os.path.join(mod, 'library', bank)
    os.makedirs(folder, exist_ok=True)
    out.save(os.path.join(folder, name), compress_level=6)
    return scale


def main(argv):
    args, opts = [], {'--max': 2048, '--threshold': 9.0, '--least': 8}
    it = iter(argv)
    for a in it:
        if a in opts:
            opts[a] = type(opts[a])(next(it))
        else:
            args.append(a)
    if len(args) != 3:
        sys.exit(__doc__)
    lib, pack, mod = args

    print('reading the library ...', flush=True)
    textures = library_textures(lib)
    total = sum(len(v[0]) for v in textures.values())
    print('  %d textures' % total, flush=True)
    pictures = list(pack_pictures(pack))
    print('matching %d pictures ...' % len(pictures), flush=True)
    found = match(textures, pictures)
    textured, used = plan(textures, found, opts['--threshold'], opts['--least'])

    os.makedirs(mod, exist_ok=True)
    name = re.sub(r'[^a-z0-9-]+', '-', os.path.basename(os.path.abspath(mod)).lower()).strip('-') or 'hd-textures'
    with open(os.path.join(mod, 'mod.fs'), 'w') as f:
        f.write('\\ an HD texture pack, made by openhuey-src/tools/hdpack.py (hdpack-report.txt: what it matched)\n'
                'mod: %s\nversion: 1 0\nrequires: openhuey 1 0\n' % name)
    print('writing %d textures ...' % len(textured), flush=True)
    jobs = [(lib, mod, pictures, opts['--max'], t) for t in textured]
    with ProcessPoolExecutor() as pool:
        scales = list(pool.map(write_one, jobs, chunksize=4))

    with open(os.path.join(mod, 'hdpack-report.txt'), 'w') as r:
        r.write('%d pictures, %d used; %d of the library\'s %d textures given HD tiles\n\n' %
                (len(pictures), len(used), len(textured), total))
        r.write('textures (bank/picture, scale, share of its tiles from the pack, from):\n')
        for (bank, name, w, h, takes), s in sorted(zip(textured, scales), key=lambda x: x[0][:2]):
            share = sum(int(m.sum()) for _, m in takes) / ((w // TILE) * (h // TILE))
            r.write('  %s/%s  %dx  %3d%%  %s\n' % (bank, name, s, round(share * 100),
                    ' '.join(os.path.relpath(pictures[p][0], pack) for p, _ in takes)))
        r.write('\npictures not used:\n')
        for p, (path, *_rest) in enumerate(pictures):
            if p not in used:
                r.write('  %s\n' % os.path.relpath(path, pack))
    print('%d textures from %d of %d pictures; report in %s' %
          (len(textured), len(used), len(pictures), os.path.join(mod, 'hdpack-report.txt')))


if __name__ == '__main__':
    main(sys.argv[1:])
