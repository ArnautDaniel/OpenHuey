\ floor-flags.fs - a nav triangle's flags by name (NavMesh: each triangle's +0x3C, "blocking
\ attributes, tested against a caller's mask"). Most say who may not stand there - each
\ character's mask, below, is the flags it keeps off - others what the floor is (its footsteps),
\ a room's halves, where a thing may be pushed, a hole things fall through.
IN: floor-flags

1 constant slope-floor
2 constant step-floor
8 constant solid-floor             \ no one stands here (nor is anything put here at random)
$10 constant keeps-fiona-off
$20 constant keeps-pursuers-off    \ the stalkers and the creatures
$40 constant keeps-debilitas-off
$80 constant floor-edge
$200 constant floor-flag-200       \ ? (no character's mask)
$4000 constant wall-floor
$8000 constant wood-floor   $10000 constant stone-floor   $18000 constant metal-floor   \ (footsteps)
$20000 constant barred-floor       \ no one; a spirit gives up crossing it
$40000 constant floor-flag-40000   \ ? (the spirits' paths keep off it)
$80000 constant doorway-floor
$100000 constant one-half   $200000 constant other-half   \ a split room's two sides
$800000 constant pushable          \ (where a thing may be pushed: NAV_PUSHABLE)
$1000000 constant keeps-hewie-off
$2000000 constant grass-floor   $2008000 constant water-floor
$4000000 constant keeps-stalkers-off   \ the stalkers and the creatures
$8000000 constant keeps-people-off     \ Fiona, Hewie and the stalkers (not the creatures)
$10000000 constant hole-floor          \ things lying about fall through (TRI_HOLE: placed.c)
$20000000 constant keeps-people-off-2  \ (as keeps-people-off)
one-half other-half or constant both-halves
doorway-floor slope-floor or constant doorway-or-step   \ (where nothing may stand or be set down)

\ ---- who keeps off what: the characters' masks (FIONA_NAV_MASK, HEWIE_NAV_MASK, NPC_BlockFlags,
\ Debilitas_BlockFlags; the creatures'); the creatures' paths keep off the edges too ----
solid-floor barred-floor or keeps-people-off or keeps-people-off-2 or constant everyone-floor
everyone-floor keeps-fiona-off or constant fiona-floor                          \ $28020018
everyone-floor keeps-hewie-off or constant hewie-floor                          \ $29020008
everyone-floor keeps-stalkers-off or keeps-pursuers-off or constant pursuer-floor   \ $2C020028
pursuer-floor keeps-debilitas-off or constant debilitas-floor                  \ $2C020068
solid-floor barred-floor or keeps-stalkers-off or keeps-pursuers-off or constant creature-floor   \ $04020028
floor-flag-40000 floor-edge or constant creature-path-floor                    \ $40080
