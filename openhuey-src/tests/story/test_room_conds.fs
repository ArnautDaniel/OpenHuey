\ The rooms' own conditions (S5c; src/game/rooms/room_*.c Cond*): the facts the rooms keep for
\ them - a loud noise here last frame, the summoner's countdown, who struck Fiona, the things
\ lying about, the stalker (in the game or only loaded, his kind, out of view) - and a few rooms'
\ conditions on them. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_room_conds.fs
IN: test-room-conds
USING: tester engine game-state actors messages common room-names game debug fiona.state story.state story.words
  story.rooms.castle-1f-10 story.rooms.castle-b1-5 story.rooms.old-mansion-b1-6 story.rooms.chaos-forest-13 story.rooms.castle-1f-9 story.rooms.castle-1f-13 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
free-play  front-garden-2 room!  20 run-frames

testing a noise of $20 or more here: kept for the next frame, then gone; a quieter one or elsewhere not
: noise-at-her ( loud room -- )  game:fiona body-tri -1 0 game:acoustics send noise  deliver ;
T{ $30 room-id noise-at-her  2 run-frames  [: loud-here? ;] in-room -> -1 }T
T{ 4 run-frames  [: loud-here? ;] in-room -> 0 }T
T{ $10 room-id noise-at-her  2 run-frames  [: loud-here? ;] in-room -> 0 }T
T{ $30 room-id 1+ noise-at-her  2 run-frames  [: loud-here? ;] in-room -> 0 }T

testing the summoner's countdown: running (5 s), then out
T{ 5 game:summoner send summoner-cooldown  3 run-frames  [: summoner-done? ;] in-room -> 0 }T
T{ 0 game:summoner send summoner-cooldown  3 run-frames  [: summoner-done? ;] in-room -> -1 }T

testing nothing of kind 3 (the doll) lying about; one made, and it is
T{ [: castle-1f-10.cond01? ;] in-room -> -1 }T
3 game:fiona body-tri  game:fiona body-pos f>cell >r f>cell >r f>cell r> r>  0 0 0  game:things send thing-made  deliver  2 run-frames
T{ [: castle-1f-10.cond01? ;] in-room -> 0 }T
engine:things-clear

testing Fiona's reaction with nobody behind it (castle-1f-9): not in one, so no
T{ [: castle-1f-9.cond00? fiona-struck? ;] in-room -> 0 0 }T

testing her 20 .. 120 from a point (castle-b1-5): 50 away yes, beside it no
: split16 ( n -- hi lo )  $FFFF and dup 8 rshift swap $FF and ;
: pt ( F: dx dz -- ) ( -- b0 b1 b2 b3 )   \ her place plus (dx, dz), as the command's bytes
    game:fiona body-at 8 + sf@ f+ f>s >r  game:fiona body-at sf@ f+ f>s split16  r> split16 ;
T{ [: 50e 0e pt castle-b1-5.cond00?  1e 0e pt castle-b1-5.cond00? ;] in-room -> -1 0 }T

testing a door's open state by its number (castle-1f-13): the room's exit 0 door, opened, shut
: door0 ( -- d )  room-id 0 room-exit-door ;
[: door0 door-open-set ;] in-room deliver
T{ [: door0 dup 8 rshift swap $FF and castle-1f-13.cond00? ;] in-room -> -1 }T
[: door0 door-open-clear ;] in-room deliver
T{ [: door0 dup 8 rshift swap $FF and castle-1f-13.cond00? ;] in-room  door0 door-flags 1 and 0<> = -> -1 }T

testing the letters (old-mansion-b1-6): "LRL" from place 0 - L (on), R wanted at 1 (on), R wanted at 2: no
: letters! ( -- )   \ (var 0's bytes: 'L' 'R' 'L' 0)
    [: 0  [char] L  [char] R 8 lshift or  [char] L 16 lshift or  var-set  1 0 var-set  2 0 var-set ;] in-room ;
letters!
T{ [: 0 old-mansion-b1-6.cond00?  1 old-mansion-b1-6.cond00?  1 old-mansion-b1-6.cond00?  2 var @ ;] in-room -> -1 -1 0 2 }T

testing her x past a line (old-mansion-b1-6 cond01): at least x - 1 yes, x + 1 no
: x-word ( F: x -- ) ( -- b1 b2 b3 b4 )  1000e f* f>s {: n :}  n 24 rshift $FF and  n 16 rshift $FF and  n 8 rshift $FF and  n $FF and ;
: past? ( F: dx -- flag )  game:fiona body-at sf@ f+  [: 0 ctx-who !  0 x-word old-mansion-b1-6.cond01? ;] in-room ;   \ (as Fiona's script)
T{ -1e past?  1e past? -> -1 0 }T

testing the count while Hewie is down (chaos-forest-13): below 100, then 100
T{ [: chaos-forest-13.cond00? ;] in-room -> 0 }T
100 progress pr.hewie-mistreated l!
T{ [: chaos-forest-13.cond00? ;] in-room -> -1 }T
0 progress pr.hewie-mistreated l!

test-summary
