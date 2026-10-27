\ The rooms' own conditions (S5c; src/game/rooms/room_*.c Cond*): the facts the rooms keep for
\ them - a loud noise here last frame, the summoner's countdown, who struck Fiona, the things
\ lying about, the stalker (in the game or only loaded, his kind, out of view) - and a few rooms'
\ conditions on them. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_room_conds.fs
IN: test-room-conds
USING: tester engine game-state actors messages common room-names game debug fiona.state story.state story.words
  story.rooms.chaos-forest-13 ;

: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
-1 story:places-off !   \ (front-garden-2 as its converted room: the machinery's host)
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

testing nothing of kind 3 (the doll) lying about (castle-1f-10's Bisque Doll); one made, and it is
T{ [: 3 story.kit:thing-lying? 0= ;] in-room -> -1 }T
3 game:fiona body-tri  game:fiona body-pos f>cell >r f>cell >r f>cell r> r>  0 0 0  game:things send thing-made  deliver  2 run-frames
T{ [: 3 story.kit:thing-lying? 0= ;] in-room -> 0 }T
engine:things-clear

testing the count while Hewie is down (chaos-forest-13): below 100, then 100
T{ [: chaos-forest-13.cond00? ;] in-room -> 0 }T
100 progress pr.hewie-mistreated l!
T{ [: chaos-forest-13.cond00? ;] in-room -> -1 }T
0 progress pr.hewie-mistreated l!

test-summary
