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
she is in one particular state (number 14 of her state machine; its meaning is still being worked out). Fear has a real number behind it; it isn't only
a screen effect.
