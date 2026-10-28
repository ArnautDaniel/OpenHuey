\ old-mansion-1f-15's place: the books found (table, shelf); a book on a lectern not its own (shown;
\ taken back); each lectern's own - its shelves back at their stop; the shelves by author; the door
\ atop; the Refined Antimony. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_15.fs
IN: test-old-mansion-1f-15
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-15 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: use-on-place ( ev -- )  0 swap -1 0 broadcast item-event ;
create spot 12 allot
: by-door ( exit -- )  0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;
variable before
: room-afresh ( -- )  old-mansion-1f-15 room!  60 run-frames ;
: ~= ( F: a b -- flag )  f- fabs 0.01e f< ;
: facing-middle ( area -- )  area-middle drop {: F: x y z :}  90  x 3e f-  y z put ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
got-cathedral-secrets story-flag-clear  got-wisemans-crafts story-flag-clear  got-occult-philosophy story-flag-clear
books-placed story-flag-clear  lectern-0-book-1 story-flag-clear  lectern-1-book-1 story-flag-clear  lectern-2-book-2 story-flag-clear
room-afresh
testing the books found: on the table, standing out on a shelf
T{ 9 facing-middle  said -> table-book-msg }T
200 read-on
T{ got-cathedral-secrets story-flag?  cathedral-secrets items:items-of 0> -> -1 -1 }T
T{ $A 180 at  said -> stands-out-msg }T
200 read-on
T{ got-wisemans-crafts story-flag? -> -1 }T
testing a book on a lectern not its own: shown; taken back
occult-philosophy 1 items send give-item  10 run-frames
0 1 -1 0 broadcast item-event  400 play-on
T{ lectern-0-book-1 story-flag?  $B part?  occult-philosophy items:items-of -> -1 -1 0 }T
T{ $D -90 at  circle press  300 play-on  lectern-0-book-1 story-flag?  occult-philosophy items:items-of -> 0 1 }T
testing each lectern's own: its shelves back at their stop
0 0 -1 0 broadcast item-event  1200 play-on
T{ books-placed story-flag?  [: s" movebook_red" place-x object@ 0e ~= ;] in-place -> -1 -1 }T
testing the shelves by author; the door atop
T{ $1C 180 at  said -> author-4-msg }T
T{ $1D 0 at  said -> author-4-msg }T
testing the Refined Antimony: taken
om1f-15-refined-antimony story-flag-clear  room-afresh  refined-antimony items:items-of before !
0 36.86e 50e -31.91e put  circle press  300 read-on
T{ om1f-15-refined-antimony story-flag?  refined-antimony items:items-of -> -1 before @ 1+ }T
test-summary
