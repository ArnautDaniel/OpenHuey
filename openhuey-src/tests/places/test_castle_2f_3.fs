\ castle-2f-3's place (scripts/story/places/castle-2f-3.fs, docs/rooms-design.md): the bedroom as
\ the original plays it - in the sheet, the maid's scene, the clothes, the remarks, the TV, the bed
\ to hide under, the night, Hewie's attack lesson and the movies replayed. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/places/test_castle_2f_3.fs
IN: test-castle-2f-3
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    fiona.state story.kit story.places.castle-2f-3 ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: free? ( -- flag )  [: her-scripted @ ;] her 0=  world-held state-flag? 0= and ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: filed? ( id -- flag )  false  128 0 do  over progress pr.files i 2* + w@ = or  loop  nip ;
: var ( n -- v )  progress pr.vars + c@ ;
: gate-middle ( area -- ) ( F: -- x y z )   \ (a gate: halfway along its line)
    dup 0 area-corner drop  1 area-corner drop  {: F: a b c d e f :}  a d f+ 2e f/  b  c f f+ 2e f/ ;
: middle ( area -- ) ( F: -- x y z )  dup area-kind 1 = if  area-middle drop  else  gate-middle  then ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor she walks: as tests/story/test_story.fs)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] her  10 frames ;
: at ( area deg -- )  >r middle r> put ;   \ (the area's middle)
: by ( deg F: x z -- )  {: F: x z :}  x 0e z put ;   \ (a spot of the area she can stand on)
: said ( -- msg )   \ (the remark offered there: taken, its message, read on, done with)
    circle press  [: shown 0< 0= ;] 600 soon drop  shown
    100 0 do  shown 0< 0= if  cross press  then  10 frames  busy? 0= shown 0< and if  leave  then  loop ;   \ (all of it: a doing may say more)
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: room ( -- )  castle-2f-3 room!  60 frames ;   \ (the place afresh: a new visit)

free-play  10 frames

testing in the sheet: every thing only reminds her of it
1 game:fiona send costume  room
T{ portrait-1-area 90 at  said -> sheets-msg }T
T{ c2f-3-portrait-1-examined story-flag? -> 0 }T

testing the maid: crossing the gate toward the door, Daniella's scene, the clothes laid out, file $201
180 -35e -16e by  180 -35e -24e by
T{ busy? -> -1 }T
600 play-on
T{ found-file-201 story-flag?  $201 filed?  busy?  free? -> -1 -1 0 -1 }T
testing ... once: crossing again, nothing
180 -35e -16e by  180 -35e -24e by
T{ busy? -> 0 }T

testing the clothes on the bed: changed into, the instructions filed
clothes-area 180 at  circle press  600 play-on
T{ found-file-202 story-flag?  [: her-costume @ ;] her  free? -> -1 0 -1 }T

testing the portraits: the first time one remark, after it another
room
T{ 90 43.571e -39.929e by  said -> portrait-1-msg }T
T{ 90 43.571e -39.929e by  said -> portrait-1-again }T
T{ c2f-3-portrait-1-examined story-flag? -> -1 }T
T{ -90 -40.418e -39.428e by  said -> portrait-3-msg }T
T{ 0 -37e 63e by  said -> portrait-2-msg }T
T{ 0 -37e 63e by  said -> portrait-2-again }T   \ (another portrait seen)
T{ -90 -40.418e -39.428e by  said -> portrait-3-again }T   \ (both others seen)
testing ... the remarks remembered for this visit only: back in, the first again
room
T{ 90 43.571e -39.929e by  said -> portrait-1-msg }T

testing the room's things
T{ fire-area -134 at  said -> fire-msg }T
T{ doll-area 0 at  said  doll-area 0 at  said -> doll-msg doll-again }T
T{ scales-area -90 at  said -> scales-msg }T
T{ birdcage-area 0 at  said -> birdcage-msg }T
T{ chest-area 180 at  said -> chest-msg }T
T{ window-area 44 at  said  window-area 44 at  said -> sunset-msg daylight-msg }T
testing ... the bed before the stalker has come: a look under it
T{ bed-area 180 at  said -> sheets-pressed-msg }T

testing the TV: the first time hoped for, then on, then off
T{ 180 -1e 13.5e by  said  c2f-3-tv-examined story-flag? -> tv-hope-msg -1 }T
180 -1e 13.5e by  circle press  [: busy? 0= ;] 300 soon drop
T{ c2f-3-tv-examined story-flag? -> 0 }T

testing the bed once he has come: under it, hidden, and out again
c2f-2-flag1 story-flag-set  room
bed-area 180 at  circle press  240 frames
T{ fiona-hidden state-flag?  [: her-scripted @ ;] her -> -1 -1 }T
circle press  240 frames
T{ fiona-hidden state-flag?  free? -> 0 -1 }T

testing the stalker coming in while she hides, before he has searched: he doesn't look, she stays hidden
: hide ( -- )   \ (a fresh visit, under the bed)
    room  bed-area 180 at  circle press  240 frames ;
: debilitas-in ( -- )  $1C debilitas-tri  60 frames ;   \ (by the bed's foot)
: searched ( -- n )  2 var ;
0 2 progress pr.vars + c!  hide  debilitas-in
T{ fiona-hidden state-flag?  searched -> -1 1 }T
debilitas-out  circle press  240 frames
testing ... the TV left on: Debilitas turns it off instead - and it wears out
variable worn
c2f-3-tv-examined story-flag-set  hide  5 var worn !  debilitas-in  240 frames
T{ c2f-3-tv-examined story-flag?  5 var  fiona-hidden state-flag? -> 0 worn @ 1+ -1 }T
debilitas-out  circle press  240 frames
testing ... once he has searched before (half the time): he finds her under it
: found? ( -- flag )  [: found-her @ ;] in-place ;
: tries ( n -- flag )   \ (until found: each a new visit)
    0 ?do  3 2 progress pr.vars + c!  hide  debilitas-in  300 frames
        found? if  true unloop exit  then  debilitas-out  circle press  240 frames
    loop  false ;
T{ 10 tries -> -1 }T
debilitas-out  600 play-on

testing the evasion tip: Debilitas about the castle, she comes in by the hall door - told to hide, the doors locked
c2f-3-flag3 story-flag-clear  golem-done story-flag-clear
castle-1f-1 debilitas-hunt  castle-2f-3 0 rooms send go-to-room  5 frames
400 play-on
T{ c2f-3-flag3 story-flag?  $A5 items:items-of 0>  free? -> -1 -1 -1 }T   \ (the document among her items)
T{ [: hiding-offered @ ;] in-place -> -1 }T
testing ... she hides under the bed, he comes in: no search (she was told to), the chase over
bed-area 180 at  circle press  240 frames  debilitas-in  120 frames
T{ fiona-hidden state-flag?  force-chased state-flag?  [: hiding-offered @ found-her @ ;] in-place -> -1 0 0 0 }T
debilitas-out  circle press  240 frames

testing the night: back with the castle's sketch by the forced way in - file $204, the west door locked
got-castle-sketch story-flag-set
castle-2f-3 $80 rooms send go-to-room  5 frames
200 play-on
T{ c2f-3-flag1 story-flag?  $204 filed?  free? -> -1 -1 -1 }T
testing ... the dog on her mind: the bed, the window
T{ bed-area 180 at  said -> cant-sleep-msg }T
T{ window-area 44 at  said -> dog-night-msg }T

testing the attack lesson: from the front garden, Hewie found in the wire - the tip, his attack, the stalker gone, Hewie saved
hewie-found-in-wire story-flag-set
castle-2f-3 $81 rooms send go-to-room  5 frames
T{ c2f-3-flag2 story-flag?  busy? -> -1 -1 }T
1500 play-on
T{ hewie-freed story-flag?  $207 filed?  busy?  free? -> -1 -1 0 -1 }T
testing ... once
castle-2f-3 $81 rooms send go-to-room  60 frames
T{ busy? -> 0 }T

testing Hewie by the wall, scrambling about (a later visit, he with her): he finds the medallion - barks, sits, looks to her; it sparkles
game:hewie send join-fiona  room  0 -9e -14e by
game:hewie send activate   \ (afresh - Hewie_Activate: the lesson's stay, 0xBB, let go)
-9e 0e -16e on-floor  f>cell f>cell f>cell swap rot  0 game:hewie send place  2 frames  $13 0 game:hewie send do-action
T{ ' busy? 300 soon -> -1 }T
600 frames
T{ c2f-3-medallion-found story-flag?  busy? -> -1 0 }T
testing ... she picks it up there: the medallion hers, the sparkle gone
: medallions ( -- n )  $70 items:items-of ;
variable before  medallions before !
0 -9e -14e by  circle press  400 play-on
T{ c2f-3-medallion story-flag?  medallions -> -1 before @ 1+ }T

testing the movies replayed (the extras: its scripts held, the movie by variable $2A) - each played, her costume kept, out
: replayed ( movie -- room held costume )
    $2A progress pr.vars + c!  2 $26 progress pr.vars + c!  events-held state-flag-set
    castle-2f-3 room!  5 frames  300 play-on
    room-id  events-held state-flag?  $26 var ;
T{ 0 replayed -> castle-2f-3 0 room-exit-leads drop 0 2 }T     \ (the maid)
T{ 1 replayed -> castle-2f-3 0 room-exit-leads drop 0 2 }T     \ (the dressing)
T{ $F replayed -> castle-2f-3 0 room-exit-leads drop 0 2 }T    \ (Hewie trapped)
T{ $10 replayed -> castle-2f-3 0 room-exit-leads drop 0 2 }T   \ (Hewie saved)
test-summary
