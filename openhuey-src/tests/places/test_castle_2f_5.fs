\ castle-2f-5's place (scripts/story/places/castle-2f-5.fs, story/places/bedroom.fs): the bedroom once
\ the chapter has turned - the mirror whole, then broken; the remarks, the TV, the windows, the chest,
\ the clock; she weak (flagging as she reaches to act, at a locked door, at the jerky), resting on the
\ bed (the mirror's scene, well again, Hewie back, Daniella left in the room); the bed to hide under,
\ Daniella searching it; the jerky Hewie sniffs out; the fragment's reflection; the movie replayed. Headless:
\   build/openhuey --test tests/places/test_castle_2f_5.fs
IN: test-castle-2f-5
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit story.places.bedroom story.places.castle-2f-5 items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: by ( deg F: x z -- )  {: F: x z :}  x 0e z put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: answer-no ( -- )  60 run-frames  cross press  30 run-frames  key: Down tap  10 run-frames  cross press  100 run-frames ;
: room-afresh ( -- )  castle-2f-5 room!  60 run-frames ;
: mirror ( -- whole broken broken2 fragment )  1 part?  5 part?  6 part?  s" a_fragment0" placed-shown? ;
: daniella-in ( tri -- )  castle-2f-5 swap partner send stalker-in  60 run-frames ;
: daniella-out ( -- )  partner send stalker-out  30 run-frames ;
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
over-music-b story-flag-clear  saturnus-hint-on story-flag-clear

testing coming in: the mirror whole (its fragments hidden) until she has rested; after, broken
room-afresh
T{ mirror -> -1 0 0 0 }T
saturnus-hint-on story-flag-set  room-afresh
T{ mirror -> 0 -1 -1 0 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
saturnus-hint-on story-flag-clear
testing ... the first fragment reflecting Daniella (the scene's): the door's glass made over
daniella-kind story.base:partner-load  5 run-frames  room-afresh
T{ [: fragment-glint  door-fx effect @ 0<> ;] in-place -> -1 }T
room-afresh

testing the portraits, the fire, the doll, the scales, the birdcage, the chest, the windows after dark
T{ 90 43.571e -39.929e by  said  90 43.571e -39.929e by  said -> portrait-1-msg portrait-1-again }T
T{ -90 -40.418e -39.428e by  said -> portrait-3-msg }T
T{ 0 -37e 63e by  said  0 -37e 63e by  said -> portrait-2-msg portrait-2-again }T   \ (another portrait seen)
T{ fire-area -134 at  said -> fire-msg }T
T{ doll-area 0 at  said  doll-area 0 at  said -> doll-msg doll-again }T
T{ scales-area -90 at  said -> scales-msg }T
T{ birdcage-area 0 at  said -> birdcage-msg }T
fg-3-flag3 story-flag-clear
T{ chest-area 180 at  said -> chest-msg }T
T{ window-area 44 at  said  window-2-area 44 at  said -> pitch-black-msg pitch-black-msg }T
testing ... the chest with her costumes: asked to change (no)
fg-3-flag3 story-flag-set
T{ chest-area 180 at  circle press  shown-soon -> chest-change-msg }T
answer-no
T{ doing-busy? -> 0 }T
fg-3-flag3 story-flag-clear
testing ... the clock: saving (or Hewie not by her) - no
T{ clock-area -90 at  circle press  shown-soon  dup $8014 =  swap $8024 = or -> -1 }T
answer-no
T{ doing-busy? -> 0 }T

testing the TV: on, then off
c2f-3-tv-examined story-flag-clear  room-afresh
180 -1e 13.5e by  circle press  300 play-on
T{ c2f-3-tv-examined story-flag?  0 part? -> -1 0 }T
180 -1e 13.5e by  circle press  300 play-on
T{ c2f-3-tv-examined story-flag?  0 part? -> 0 -1 }T

testing she weak: reaching to act she flags ("I don't feel so hot", then "rest"), her fear kept high
over-music-b story-flag-set  room-afresh  0 -10e 0e -40e put
T{ square press  shown-soon -> not-hot-msg }T
200 play-on
T{ square press  shown-soon -> get-rest-msg }T
200 play-on
T{ [: her-fear f@ ;] her 98e f> -> -1 }T
testing ... at a locked door
0 exit-door lock  doors-redone
T{ 0 exit-area 0 at  circle press  shown-soon  dup not-hot-msg =  swap get-rest-msg = or -> -1 }T
200 play-on  0 exit-door unlock  doors-redone

testing resting on the bed (weak): the mirror's scene - well again, the file, Hewie back, Daniella here, the doors open
hewie-freed story-flag-set  room-afresh  1 lock  3 lock  $C lock  no-flee state-flag-set  doors-redone
bed-area 180 at  circle press  1500 play-on
T{ saturnus-hint-on story-flag?  $231 filed?  mirror  free? -> -1 -1 0 -1 -1 0 -1 }T
T{ game:hewie body-room  partner body-room -> castle-2f-5 castle-2f-5 }T
T{ 1 doors:door-locked?  3 doors:door-locked?  $C doors:door-locked?  no-flee state-flag? -> 0 0 0 0 }T
testing ... Daniella here: the bed - no time
T{ bed-area 180 at  said -> no-time-msg }T
daniella-out
testing ... the window: its glass sharp now
T{ window-2-area 44 at  said -> sharp-glass-msg }T

testing the bed: under it, hidden, and out again
room-afresh  bed-area 180 at  circle press  240 run-frames
T{ fiona-hidden state-flag?  [: her-scripted @ ;] her -> -1 -1 }T
circle press  240 run-frames
T{ fiona-hidden state-flag?  free? -> 0 -1 }T

testing Daniella coming in while she hides, before she has searched: no look, she stays hidden
: hide ( -- )  room-afresh  bed-area 180 at  circle press  240 run-frames ;
0 2 var!  hide  $1C daniella-in
T{ fiona-hidden state-flag?  2 var@ -> -1 1 }T
daniella-out  circle press  240 run-frames
testing ... once she has searched before (half the time): she looks under it and finds her
: found? ( -- flag )  [: found-her @ ;] in-place ;
: tries ( n -- flag )   \ (until found: each a new visit)
    0 ?do  3 2 var!  hide  $1C daniella-in  400 run-frames
        found? if  true unloop exit  then  daniella-out  circle press  240 run-frames
    loop  false ;
T{ 10 tries -> -1 }T
daniella-out  600 play-on

testing Hewie by the wall, scrambling about: he finds the chicken jerky; she picks it up
c2f-3-medallion-found story-flag-clear  c2f-3-medallion story-flag-clear
game:hewie send join-fiona  room-afresh  0 -9e -14e by
game:hewie send activate
-9e 0e -16e on-floor  f>cell f>cell f>cell swap rot  0 game:hewie send place  2 run-frames  $13 0 game:hewie send do-action
T{ ' doing-busy? 300 soon -> -1 }T
600 run-frames
T{ c2f-3-medallion-found story-flag?  [: sparkle-fx effect @ 0<> ;] in-place -> -1 -1 }T
testing ... weak: she flags instead
saturnus-hint-on story-flag-clear
T{ 0 -9e -14e by  circle press  shown-soon  dup not-hot-msg =  swap get-rest-msg = or -> -1 }T
200 play-on  saturnus-hint-on story-flag-set
chicken-jerky items:items-of before !
0 -9e -14e by  circle press  400 play-on
T{ c2f-3-medallion story-flag?  chicken-jerky items:items-of -> -1 before @ 1+ }T

testing the movie replayed (the extras): played, back to the extras room
off-map-37 room!  30 run-frames   \ (from the extras room, as the theatre sends her)
game:hewie send part-from-fiona  $2D $2A var!  events-held state-flag-set  room-afresh  600 play-on
T{ room-id  events-held state-flag? -> off-map-37 0 }T
test-summary
