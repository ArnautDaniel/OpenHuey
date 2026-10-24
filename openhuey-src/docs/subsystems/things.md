# Things lying about

## Purpose

What Fiona throws, drops or sets down - the ball, the powders, the magnesias, the doll, the
stone - and the story's own (src/game/placed.c: the original's placed things, gPlacedThings). The
engine moves and draws them (`src/game/things.c`): flying under gravity, bouncing off walls
(keeping 0.7 of their speed; the doll 0.2), rolling and slowing on the floor, dropping through
holes ($10000000) for 31 frames. What they do is the `things` actor's (`scripts/things.fs`).

## The kinds

| Kind | Item | Let go | Drawn | What it does |
|---|---|---|---|---|
| 0 | Ball ($90) | thrown | a yellow sprite | bounces, rolls; 3 minutes; then hers again if she has none (and none lies about) |
| 1 | Antimony Powder ($91) | aimed | a purple sprite | bursts on what it hits (a splash, $8D); a stalker treading on it: hurt 5 |
| 2 | Magnesia ($92) | set down | GFM model 1 | armed after 30 s: goes off ($8E, a burst) - a hard blow of 15 (20%: startled) to the stalker within 10; the homunculi within reach struck; the stalker or a homunculus treading on it sets it off at once (see below) |
| 3 | Bisque Doll ($93) | thrown | GFM model 2, lying | 5 minutes; Debilitas searching hears it ($1F) and takes it when on it |
| 4 | Refined Antimony ($94) | aimed | orange | as 1, hurt 10 |
| 5 | Refined Magnesia ($95) | set down | model 1, palette 1 | as 2: 1 min, 30, 50%, reach 15 |
| 6 | Antimony Tube ($97) | aimed | white | as 1, hurt 20 |
| 7 | Prima Magnesia ($98) | set down | model 1, palette 2 | as 2: 2 min, 45, 100%, reach 20 |
| 8 | Weeping Stone ($9B) | set down | GFM model 3 | after 15 s a noise the stalker hears ($4F at it), and gone |
| 9, 10 | the story's | | | 9 debris carrying a room object (`keri-drop`); 10 a trap (`trap-place`), smoke rising from it (SmokeTrail) - the House of Truth |

Fiona's kick or shove (her blow's point within 5) kicks one her way, 2 ahead and 2 up.
The kinds 0, 2, 3, 5, 7, 8 are kept in the save (a `things:lying` line each); ten of a kind lie
about at most - the oldest goes.

## Messages

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `thing-made` | `( kind tri x y z vx vy vz -- )` | in (Fiona's let go) | A thing at her hand on its way. |
| `blow` | `( kind damage how -- )` | out, to the stalker | A powder trodden on (1), a magnesia going off (2; how $8000 startled). |
| `thing-blow` | `( damage yaw -- )` | out, to a homunculus | A magnesia's blow: going off within its reach, or trodden on (yaw: the way it faces - to the magnesia, or 0). |
| `noise` | | out, to the acoustics | The stone's, the doll's. |
| `give-item` / `take-item` | | out, to the items | The ball back; one of her item used (from Fiona). |
| `fetch-this` | `( thing -- )` | out, to Hewie | Her ball thrown: his to fetch (hewie/fetch.fs). |
| `thing-taken` | `( thing -- )` | in (Hewie) | He picked it up: gone (the ball hers again if she has none). |

The doll (Thing03_StalkerCheck): Debilitas (kinds 2 6 7 $1B) in its room. Away from Fiona he
comes upon it once a visit - he hears it ($1F) and takes it unless he is after her. In her room,
while he searches (modes 1..3), it makes its noise once and draws him (`drawn-to`: if he sees it
and can get there it is his search's one stop - stalker/search.fs `go-for-doll`); within 10 of him
it is taken. The magnesias' bursts: the plain one the shove's puffs; the refined one's BurstA and
the prima's BurstB (`mag-burst-new`: a flash, a ring on the floor, pieces thrown and puffs rising
- three of each across the view for the prima).

The magnesias and the homunculi (ThingShared_ReachStalkers, _StompStalkers): in whatever room it
lies, the homunculi with a place in that room - off screen too (creatures.md) - are struck as
they stand there: armed, those within its reach across and 10 high (facing it); before that, one
standing on it (within its radius, 1 high) sets it off - its sound and burst wherever it lies,
as the original's - and is struck facing heading 0. The stalker sets it off by treading only in
her room. The homunculi take the blow (their action 4) once they are in play.

The stalker following a noise in the room being played is his leads' (`stalker/leads.fs`:
docs/subsystems/stalker.md); the doll draws him through its own message.

## Status

Rolling (Ball_StateRolling): slowed by 0.03 times the slope's flatness (most on the level),
pushed down it by the slope's sine (NavTri_Slope).


`tests/things/test_things.fs` (12): the ball thrown (her animation, the thing, her item used),
landing and rolling, back in her items when gone; a powder bursting; a magnesia set down and going
off when armed (its blows to the homunculi off screen: `tests/creatures/test_homunculi.fs`); a doll kept in a save and back as it lay.
