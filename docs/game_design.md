# How Haunting Ground works inside

Notes for people who love the game and want to know how it really ticks, collected while
decompiling it. Function addresses (`0x...`) are in the US release (SLUS-210.75) for anyone who
wants to look for themselves. The game runs at 60 frames per second, so "600 frames" is 10 seconds.

## The cast is three slots

At any moment the game knows exactly three characters by role: slot 0 is Fiona, slot 1 is
Hewie, slot 2 is "the pursuer". Whoever is hunting Fiona in the current chapter is loaded
into slot 2. That's why only one stalker can chase you at a time: the game has room for one.

## One stalker, four faces

Debilitas, Daniella, Riccardo and Lorenzo are not four separate programs. They are one
shared "pursuer" brain with about 200 behaviours, and each stalker swaps out only a handful
of them: how they load their model, how they update each frame, a few special moves. Searching
rooms, following your noise, opening doors, giving up the chase: most of that is the same
code for all of them, tuned by different numbers.

The loader (`0x171160`) knows 40 kinds of slot-2 character. The four stalkers are only some
of them; the rest are story variants: Debilitas alone has about seven (different chapters
and scenes), Lorenzo has his young, old and final forms, and Riccardo and Daniella have
event versions.

## Fiona's panic, as the stalkers see it

The stalkers' code treats Fiona as panicking when her fear is above 90 (out of 100), or when
she is in one particular state (number 14 of her state machine; its meaning is still being
worked out). Fear has a real number behind it; it isn't only a screen effect.

## What every stalker starts with

Before each stalker applies its own tweaks, the shared setup (`0x29FB20`) gives every
pursuer the same baseline: 100 hit points, a body 5 units wide and 20 tall, eyesight that
reaches 150 units within a 60 degree angle, and an ear that ignores any noise of loudness 12
or less.

## Stalkers are part of your save

When you save, the game writes the stalker's situation into the save data alongside the
story flags (`0x29E9D0`): which room they're in, the floor triangle they stand on and which
way they face, their health, where they were heading and which exit they meant to take,
and their running timers. Loading a save puts the hunt back the way it was rather than
starting it fresh.

## Closeness is frightening

Fiona's panic is fed by a shared "threat" meter, and a stalker raises it simply by being
near her (`0x2982A0`). Within 10 units the stalker adds its full amount; every further 10
units cuts that to three quarters (100%, 75%, about 56%, about 42%), and beyond 40 units it
adds nothing at all. Keeping your distance is literally keeping your nerve.

## How close is "caught"

A stalker can grab Fiona if it can see her, or if she is within 20 units and the floor
between them is walkable in a straight line (`0x218A30`). One story flag halves that reach
to 10 units, another switches catching off entirely, and there's a per-Fiona switch that
makes her uncatchable while it is set.

## The search goes on while you can't see it

When a stalker loses Fiona it plans a search route of up to eight stops. For each stop it
rolls: 60% of the time it picks the room's next hand-placed "point of interest" (they're
visited in order, wrapping around), otherwise a random walkable spot (`0x27E5D0`).

The route doesn't pause when the stalker is off in another room. The game simply keeps time:
every 150 frames (2.5 seconds) away counts as one stop walked. When the stalker comes back
into the room you're in, it skips ahead along its route by that many stops, or tops the
route up with new ones (`0x27EEA0`). So hiding for longer really does let it wander further.
