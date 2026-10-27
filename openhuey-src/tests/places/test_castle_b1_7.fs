\ castle-b1-7's place (scripts/story/places/castle-b1-7.fs): the storeroom behind the furniture - the
\ flame, the key sparkling; Hewie through the hole from castle-1f-19, sent for the Study Key; the
\ covered furniture, the blocked door, the drain's Medallion, the vase and its Medallion. Headless:
\   build/openhuey --test tests/places/test_castle_b1_7.fs
IN: test-castle-b1-7
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors hewie-names story.places.castle-b1-7 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
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
: room-afresh ( -- )  castle-b1-7 room!  60 run-frames ;
: fx? ( k -- flag )  [: effect @ 0<> ;] in-place ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear

testing coming in: the flame, the key sparkling (not yet had), the box placed
got-study-key story-flag-clear  room-afresh
T{ flame-fx fx?  key-fx fx? -> -1 -1 }T

testing the furniture
T{ furniture-area 0 at  said -> sheets-msg }T
\ (the blocked door's remark not checked: no test spot found where her request at it is made - see
\ docs/rooms-design.md)

testing the drain: its Medallion the first time, then nothing
cb1-7-medallion-3 story-flag-clear  room-afresh  medallion items:items-of before !
drain-area 0 at  circle press  300 read-on
T{ cb1-7-medallion-3 story-flag?  medallion items:items-of -> -1 before @ 1+ }T
T{ drain-area 0 at  said -> nothing-msg }T

testing the vase broken: its Medallion picked up
cb1-7-breakable-broken story-flag-set  cb1-7-medallion story-flag-clear  room-afresh  medallion items:items-of before !
T{ medallion-fx fx? -> -1 }T
0 31e -40e 9e put  circle press  300 read-on
T{ cb1-7-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T

testing Hewie through the hole, waiting by it: sent for the key - he fetches it, hers
got-study-key story-flag-clear  hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh
castle-b1-7 hole-side-tri 1 game:hewie send hewie-place  10 run-frames  [: ['] waits-by-hole start drop ;] in-place  60 run-frames
study-key items:items-of before !
-99 7e -40e -22e put  go-there-cmd broadcast fiona-started  5 run-frames  1500 play-on
T{ got-study-key story-flag?  study-key items:items-of -> -1 before @ 1+ }T
test-summary
