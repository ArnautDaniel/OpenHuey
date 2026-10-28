\ castle-b1-2's place (scripts/story/places/castle-b1-2.fs): the washroom - the toilets, the water
\ (calming once a visit; the recovery document), the locked door and its knob, the Smudged Key, the
\ Chamomile, the Medallion Hewie sniffed out, the shower to hide behind and the stalker's search. Headless:
\   build/openhuey --test tests/places/test_castle_b1_2.fs
IN: test-castle-b1-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.castle-b1-2 ;
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
: room-afresh ( -- )  castle-b1-2 room!  60 run-frames ;
: hidden? ( -- flag )  fiona-hidden state-flag? ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
room-afresh

testing the toilets
T{ toilets-area 180 at  said -> toilets-msg }T
full-suite   \ (on: waits, scenes, walks - the full run's)
testing the water: calming, the recovery document the first time; again, refreshed
got-doc-recovery story-flag-clear
water-area 180 at  circle press  900 play-on
T{ got-doc-recovery story-flag?  recovery-doc items:items-of 0>  doing-busy? -> -1 -1 0 }T
T{ water-area 180 at  said -> refreshed-msg }T

testing the Chamomile: taken
cb1-2-camomile story-flag-clear  room-afresh  camomile items:items-of before !
0 78e 0e 15e put  circle press  300 read-on
T{ cb1-2-camomile story-flag?  camomile items:items-of -> -1 before @ 1+ }T

testing the Medallion Hewie sniffed out: picked up
cb1-2-medallion-found story-flag-set  cb1-2-medallion story-flag-clear  room-afresh  medallion items:items-of before !
0 -44.9e 0e -16e put  circle press  300 read-on
T{ cb1-2-medallion story-flag?  medallion items:items-of -> -1 before @ 1+ }T

testing the locked door: tried ("locked"), then its knob
smudged-door s" doors" actor-named send lock  room-afresh  locked-exit by-door
T{ said -> locked-msg }T
\ (the second try - the knob's remark - not checked: her walk from the door's spot to it doesn't
\ arrive in the engine yet; see docs/rooms-design.md)
testing ... the Smudged Key at it: unlocked, the key spent
smudged-key 1 items send give-item  10 run-frames  locked-exit by-door  4 use-on-place  300 play-on
T{ smudged-door doors:door-locked?  smudged-key items:items-of -> 0 0 }T

testing the shower: behind the curtain, hidden; out by the button
room-afresh  shower-area 180 at  circle press  [: hidden? ;] 600 soon drop
T{ hidden? -> -1 }T
circle press  [: doing-busy? 0= ;] 600 soon drop
T{ hidden?  free? -> 0 -1 }T

testing the stalker coming in while she hides: he searches - his comings counted
2 story.base:partner-load  5 run-frames  room-afresh
shower-area 180 at  circle press  [: hidden? ;] 600 soon drop
search-var var@ before !
castle-b1-2 $FA game:debilitas send stalker-in  10 run-frames
T{ [: came-while-hidden @ ;] in-place  search-var var@ -> -1 before @ 1+ }T
game:debilitas send stalker-out  600 play-on
test-summary
