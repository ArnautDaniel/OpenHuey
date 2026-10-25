# Obstacles

## Purpose

The things in a room Fiona can push about (crates, a cabinet): up to five in the played room.
Each sits on the grid of the room's pushable floor and moves two squares at a push. The story
places them, asks where they are, stops them, and keeps their places across visits.

## API

Declared in `scripts/messages.fs`. All go to the `obstacles` actor.

| Message | Stack | Meaning |
|---|---|---|
| `place-obstacle` | `( i n kind a b -- )` | Obstacle i (model "oshi0n", of a kind) placed at square (a, b) (Obstacles_PlaceAt). |
| `place-obstacle-kept` | `( i n kind -- )` | The same, at the place kept for it (Obstacles_PlaceSaved). |
| `stop-obstacle` | `( i -- )` | It can't be pushed any more (Obstacles_Stop). |
| `keep-obstacle` / `keep-obstacle-at` | `( i -- )` / `( i a b -- )` | Its place kept: where it is, or that square. |
| `keep-obstacle-spot` | `( i -- )` | Where it stands kept, for a model. |
| `obstacle-model-to-spot` | `( i n -- )` | Model "oshi0n" its model, put at the spot kept (Obstacles_ModelBack). |
| `push-obstacle` | `( i dx dz -- )` | (Fiona) pushed along (dx, dz), floats `f>cell`: two squares over its move. |
| `obstacle-moved` | `( i -- )` | (Fiona) its move done. |

**Queries** (anyone's: facts in C, `game/obstacles.c`): `obstacle-on-square? ( i tri -- flag )` its
square (the one it is going to on its move's last frame); `obstacles-ring-mask ( tri -- bits )`
the obstacles with `tri` on the ring round them; `obstacle-at ( i -- ) ( F: -- x y z )` where it
stands. `can-push? ( i -- flag ) ( F: dx dz -- )` (`obstacles` vocabulary) gathers the other
characters in the room and asks `obstacle-can-push?`.

The story's words (`story/words.fs`): `obstacle-place`, `obstacle-place-saved`, `halt-obstacle`,
`obstacle-save`, `obstacle-save-at`, `obstacle-keep-spot`, `obstacle-model-back` send these;
`obstacle-on?` asks `obstacle-on-square?`.

## State

The table of five (shared: written only by the actor): each one's model, its reference square
(two nav triangles), the square it is moving to, its parts' squares, its middle, its move (frame
of 30, the push). The places and spots kept. Its kinds come from the executable
(`ObstacleKinds`); its move is OBSTACLE.MTN's one (30 frames' steps, 5 in all).

## Rules

1. **Squares.** The pushable floor is a grid of squares of 2.5, each a pair of nav triangles.
   A square one along a direction: from its middle 5 along over the mesh until two new
   triangles are crossed (`nav-square-step`, Obstacle_StepSquare).
2. **Parts.** A kind has up to 3 parts, each `w` x `d` squares from (dx, dz) squares off the
   reference square, and its model's offset. The one kind: one part of 4 x 4 from (-1, -1),
   offset (1.25, 1.25).
3. **The floor.** The triangles under each part are blocked ($20400000: walls to everyone);
   the ring of squares round each part is marked - its sides $20800000 (walls too, and the
   pushable floor $800000 Fiona pushes from), its corners $20000000.
4. **Can it go** (Obstacle_CanPush) along a direction: not stopped; no other character (Hewie,
   the stalkers, the story's figures - not those hidden, +0x29: `figure-shown`) on the ring
   round it moved two squares along, nor one; and every triangle two
   squares along, its own blocks lifted, walkable floor ($100) without $440080.
5. **Pushed** (Fiona's): the square two along is its target; its blocks and ring lifted, the
   others' rings marked again, then its blocks and ring two squares along. Over the 30 frames
   its middle moves by the move's steps along the push; at the sixth its scrape (sound 6, bank
   0) and a noise ($1F, hers) where it was before that frame's step, at its square. At the last frame its target is its reference
   square, its parts and middle follow.
6. **Its model** follows it each frame: its middle by its offset.
7. **A room left**: all gone; the next room's scripts place its own.

## Design notes

- The table is a fact like the doors' state: Fiona's push and the story ask it directly; only
  this actor changes it (and the floor's flags under it).
- The places kept live in the actor, not in the save (as in the original: the room manager's,
  not the progress').

## Status

**Built**: the geometry and the move in C (`src/game/obstacles.c`, from room_map.c's Obstacle_*); the actor (`scripts/obstacles.fs`) places, pushes, sounds, moves the models (`placed-at`). Checked by
`tests/obstacles/test_obstacles.fs`: placed in castle-b1-7 (its square, its place, the floor
blocked, its ring marked); which ways it can go; Fiona walking into it, leaning on it and
pushing it twice, straining at the end, letting go; its square after; stopped; its place kept
across a visit; gone with the room; the story's words; each push felt on the pad (Fiona's `fiona-jolt`); a model put back at the kept
spot (`obstacle-model-back`); a hidden figure on the ring not in its way; no push but in her own
moves (19 in all).

With it, two fixes to the engine's moving over the floor (`navmesh_move`): a step may go no
more than its climb up as well as down (the game's y runs down: blocked under her, she used to
step onto a balcony 41 above; the floor search itself, `navmesh_find`, is as it was - Hewie's
fight needs it); and a step with no way on nor along goes up to the wall ahead
(Actor_Move stops at it) instead of not at all.
