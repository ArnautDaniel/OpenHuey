/* The room's event areas (PAC section 2: a table of word offsets, one per area, then the areas).
 * An area of type 1 is four corners (x, z at +0x10 + k * 0x10) and a height range (+0x24 up to
 * +0x8); one of type 0 is a gate: a line from (+0x10..) to (+0x20..) that a step can cross.
 * src/game/event.c Events_InArea, EventCond_AreaCross. (The original also takes the floor's height
 * on stairs triangles; not here yet.) */
#ifndef AREAS_H
#define AREAS_H

#include "../core/mathx.h"
#include "room.h"

/* is p inside area `area` (only type 1 areas have an inside) */
int area_inside(const Room *r, int area, Vec3 p);
/* the middle of area `area`'s first and third corners, at the first one's height
 * (Events_AreaMiddle); 0 if there is no such area */
int area_middle(const Room *r, int area, Vec3 *out);
/* a step from `prev` to `cur`: 1 into the area (or across the gate one way), -1 out of it (the
 * other way), 0 neither */
int area_cross(const Room *r, int area, Vec3 prev, Vec3 cur);

/* how many areas the room's table has; area `area`'s corner k (0..3); its kind (+0: 1 a box
 * area_inside tests, else a gate line; -1 none) - for the debug labels */
int area_count(const Room *r);
int area_corner(const Room *r, int area, int k, Vec3 *out);
int area_kind(const Room *r, int area);

#endif
