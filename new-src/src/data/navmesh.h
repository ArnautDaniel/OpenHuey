/* The navigation mesh (PAC section 0): the triangles characters may stand on. Each record is
 * 0x50 bytes: three corners (x, y, z, w floats), the neighbour across each edge (-1: a wall),
 * and flags (src/game/navmesh.c NavMeshSet_Take; section 16 holds a byte of more flags per
 * triangle). */
#ifndef NAVMESH_H
#define NAVMESH_H

#include "../core/mathx.h"

#include <stddef.h>
#include <stdint.h>

typedef struct NavTri {
    Vec3 v[3];
    int32_t next[3];   /* the triangle across edge v[i] - v[(i + 1) % 3], or -1 */
    uint32_t flags;
    uint32_t lights;   /* the room lights reaching it: bit i = light i (+0x4C) */
} NavTri;

typedef struct NavMesh {
    NavTri *tris;
    int ntris;
    uint32_t block;    /* triangles with any of these flags are walls to whoever moves now (the
                        * original's per-character mask: Fiona 0x28020018, Hewie 0x29020008) */
} NavMesh;

int navmesh_build(NavMesh *n, const uint8_t *sec, size_t size);
/* the flags as the game sets them up (NavMeshSet_Take): some cleared, then section 16's byte a
 * triangle adds 0x4000 (bit 0) and 0x80000 (bit 1) */
void navmesh_take_flags(NavMesh *n, const uint8_t *sec16, size_t size);
void navmesh_free(NavMesh *n);
/* the triangle under (x, z) whose surface is nearest height y (and not more than `climb`
 * above it), not one of the `block` flags; -1 if none. *height: its surface there */
int navmesh_find(const NavMesh *n, Vec3 p, float climb, float *height);
/* move from p by (dx, dz), staying on the mesh with `radius` to spare ahead: sliding along
 * walls, following the floor */
Vec3 navmesh_move(const NavMesh *n, Vec3 p, float dx, float dz, float climb, float radius);
/* a way over the mesh from (tri `from`, point a) to (tri `to`, point b), keeping off the
 * `block` flags (the triangle it starts on excepted): A* over the triangles, then pulled
 * tight through the edges they share. Its turning points into out (b last; up to max): their
 * number, 0 if there is no way. (The original's planner, src/game/navmesh.c PathPlan_*, runs
 * several searches and curves its paths; this keeps to the shortest.) */
int navmesh_path(const NavMesh *n, int from, Vec3 a, int to, Vec3 b, Vec3 *out, int max);
/* walking straight from a (on triangle `from`) toward b over the mesh, kept off the `block`
 * flags (the start excepted): the triangle b is on, or -1 if a wall comes first (the original's
 * Actor_TriTo / NavMesh_Walk). *reach (if given): how far along it got */
int navmesh_walk(const NavMesh *n, int from, Vec3 a, Vec3 b, float *reach);
/* the middle of triangle i */
Vec3 navmesh_center(const NavMesh *n, int i);
/* the middle of the triangle nearest p */
Vec3 navmesh_nearest(const NavMesh *n, Vec3 p);

#endif
