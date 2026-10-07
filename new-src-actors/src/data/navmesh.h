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

/* a link between two levels - a ladder (the original's nav door regions, NavMesh +0x20): a pair
 * of triangles tagged with the same group (flags bits 9..13), [0] the one with the larger y (the
 * game's y runs down: the foot), each with the heading out across its edge toward the other and
 * that edge's middle */
typedef struct NavLink {
    int tri[2];
    float yaw[2];
    Vec3 spot[2];
} NavLink;

typedef struct NavMesh {
    NavTri *tris;
    int ntris;
    NavLink links[5];
    int nlinks;
    uint32_t block;    /* triangles with any of these flags are walls to whoever moves now (the
                        * original's per-character mask: Fiona 0x28020018, Hewie 0x29020008) */
} NavMesh;

int navmesh_build(NavMesh *n, const uint8_t *sec, size_t size);
/* the flags as the game sets them up (NavMeshSet_Take): some cleared, then section 16's byte a
 * triangle adds 0x4000 (bit 0) and 0x80000 (bit 1) */
void navmesh_take_flags(NavMesh *n, const uint8_t *sec16, size_t size);
void navmesh_free(NavMesh *n);
/* the links (NavMesh_FindDoorRegions; navmesh_take_flags calls it) */
void navmesh_find_links(NavMesh *n);
/* p is at link i (NavMesh_AtDoorRegion: within 5 in height and 20 across of either spot) */
int navmesh_link_at(const NavMesh *n, int i, Vec3 p);
/* which side of link i p (on triangle tri) is at (NavMesh_DoorSide: within 5 in height and 12
 * across of that side's spot, walking straight onto its triangle); -1 neither */
int navmesh_link_side(const NavMesh *n, int i, int tri, Vec3 p);
/* the point in front of link i's side (Actor_DoorFront: 5 out, shifted by (ox, oz) in its
 * frame), reached over the mesh from the spot: its triangle (the point in *out), -1 none */
int navmesh_link_front(const NavMesh *n, int i, int side, float ox, float oz, Vec3 *out);
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
/* the wall a straight walk from a (on `from`) toward b meets (an edge with nothing beyond, or a
 * blocked triangle): its direction as a heading; 0 if b is reached first */
int navmesh_wall(const NavMesh *n, int from, Vec3 a, Vec3 b, float *yaw);
int navmesh_walk(const NavMesh *n, int from, Vec3 a, Vec3 b, float *reach);
/* the middle of triangle i */
Vec3 navmesh_center(const NavMesh *n, int i);
/* the middle of the triangle nearest p */
Vec3 navmesh_nearest(const NavMesh *n, Vec3 p);

#endif
