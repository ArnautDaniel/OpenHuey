/* Where a room's exits are: the spots characters stand at to come in or go out. An exit with a
 * door of its own (PAC section 7: per exit an offset, 0 none; the door's nav triangle at +0,
 * where it stands at +0x10, its turn at rest at +0x20) has them in the door's frame, turned to
 * face away from the exit's event area: "out" 4 behind the door, "in" 12 ahead, "through" 12
 * behind, walked to over the nav mesh from the door's triangle. Otherwise the room table's
 * triangles say. src/game/room_map.c exit_tri, src/game/doors.c Doors_NavSpot. */
#ifndef EXITS_H
#define EXITS_H

#include "../core/mathx.h"
#include "room.h"
#include "world.h"

/* exit `exit`'s spot `which` (0 out, 1 in, 2 through) in room r: its nav triangle (-1: none)
 * and the point */
int exit_spot(const Room *r, const World *w, int exit, int which, Vec3 *out);

#endif
