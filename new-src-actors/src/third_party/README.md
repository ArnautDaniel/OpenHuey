Vendored, public domain (or MIT, at the user's choice): Sean Barrett's stb
(https://github.com/nothings/stb at 2c980bb59875b0d32144a71867fbdebb2f77cd20).

- `stb_image.h` v2.30 - reading PNG (the library's textures)
- `stb_image_write.h` v1.16 - writing PNG (the import's conversion)

Built once, PNG only, in `stb.c`.

Vendored, public domain (or MIT-0): David Reid's dr_flac
(https://github.com/mackron/dr_libs at dfe8377631000664666519fdb83da193fd8037f4, 0.13.3 and the
bounds-checking and seeking fixes after it).

- `dr_flac.h` - reading FLAC (the library's music). Built in `dr_flac.c`, without stdio or Ogg.

Vendored, MIT: Johannes Kuhlmann's cgltf v1.15 (https://github.com/jkuhlmann/cgltf, the
release's cgltf.h) - reading glTF (the library's room meshes). Built in `cgltf.c`.
