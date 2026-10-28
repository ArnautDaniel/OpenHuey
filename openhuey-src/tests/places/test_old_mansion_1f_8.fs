\ old-mansion-1f-8's place: the graveyard - coming in (the sparkles, the hand at the back, the
\ creatures let out); the graves read in turn; the mausoleum; the thicket - Hewie into it and out,
\ the ambush; the glow at the back and the Antimony Tube Hewie fetches; the jerky, the medallion;
\ the back: the hand seen, its remarks, the mandragoras fed; the back door unlocked (the stalker
\ sent on); the hangers; the splashes; his barks; the replays. Headless:
\   build/openhuey --test tests/places/test_old_mansion_1f_8.fs
IN: test-old-mansion-1f-8
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names doors story.places.old-mansion-1f-8 ;
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
: room-afresh ( -- )  old-mansion-1f-8 room!  60 run-frames ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  10 run-frames ;
: picked ( -- n )  game:window enter  window:picked @  leave-actor ;
: yes-it ( -- )   \ (its choice may start on no; the question on its last page)
    [: key: Left tap  picked 0= dup 0= if  cross press  20 run-frames  then ;] 30 soon drop  cross press ;
: command ( cmd -- )  broadcast fiona-started  1 run-frames ;
: hewie-brought ( -- )  old-mansion-1f-8 $175 0 game:hewie send hewie-place  60 run-frames ;
: hewie-at ( F: x y z -- )  on-floor {: F: x y z :}  x f>cell y f>cell z f>cell 0e f>cell  game:hewie send place  1 run-frames ;
: stalker-at ( F: x y z -- )  on-floor {: F: x y z :}  x f>cell y f>cell z f>cell 0e f>cell  game:daniella send place ;
: fx-alive? ( k -- flag )  [: effect @ dup 0<> swap alive? and ;] in-place ;
: none-glowing ( -- )
    prima-materia sulfuric-ore silver-ore white-godstone red-godstone handful-of-ash
    6 0 do  dup items:items-of 0 ?do  dup [: spend ;] in-place  5 run-frames  loop  drop  loop ;
: give ( item -- )  [: 1 give-item ;] in-place  5 run-frames ;
: to-a-grave ( area deg -- msg )  at  said ;
: started? ( -- flag )  [: cutscene-active? movie-status 1 = or ;] 600 soon ;
free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
none-glowing  white-mandragora-used story-flag-clear  om1f-8-flag2 story-flag-clear
om1f-8-beef-jerky story-flag-clear  om1f-8-album-medallion story-flag-clear  om1f-5-lavender story-flag-clear
om1f-12-flag1 story-flag-set  om1f-8-flag1 story-flag-clear  om1f-8-antimony-tube story-flag-clear
room-afresh
testing coming in: its steady sound started once its sounds are in
T{ [: [: steady-on @ ;] in-place ;] 120 soon -> -1 }T
testing coming in: the sparkles, the glow at the back, the hand loaded there, the shadow, the creatures let out
T{ jerky-fx fx-alive?  lavender-fx fx-alive?  medallion-fx fx-alive?  glow-fx fx-alive?  shadow-fx fx-alive? -> -1 -1 -1 -1 -1 }T
T{ [: glow-shown @  hand 0< 0= ;] in-place  om1f-8-flag1 story-flag? -> -1 -1 -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

testing the Beef Jerky and the Album Medallion picked up
beef-jerky items:items-of before !
T{ 0 122.09e 8.28e 10.26e put  circle press  300 read-on  om1f-8-beef-jerky story-flag?  beef-jerky items:items-of  jerky-fx fx-alive? -> -1 before @ 1+ 0 }T
T{ 0 -140.37e 16e -25.97e put  circle press  300 read-on  om1f-8-album-medallion story-flag?  medallion-fx fx-alive? -> -1 0 }T

testing the graves: each its name; once two are read, one read again says they bear the same name
T{ $18 -80 to-a-grave  $10 0 to-a-grave  $18 -80 to-a-grave -> 1 2 same-name-msg }T
T{ $11 -80 to-a-grave  $12 -120 to-a-grave  $13 90 to-a-grave  $14 124 to-a-grave -> 3 4 5 6 }T

testing the mausoleum: its words, then the second remark, in turn
T{ mausoleum-area 180 at  said  mausoleum-area 180 at  said  mausoleum-area 180 at  said -> mausoleum-msg spirits-msg mausoleum-msg }T

testing the thicket's remark; the glow at the back
T{ thicket-area -60 at  said -> opening-msg }T
T{ glow-area -120 at  said -> glow-msg }T

testing the back the first time: too dark - looked closer, the hand grabs at her (her fright)
T{ back-area 90 at  circle press  shown-soon -> too-dark-msg }T
yes-it  [: om1f-8-flag2 story-flag? ;] 900 soon drop  [: doing-busy? 0= ;] 600 soon drop  0 panic-to  60 run-frames
T{ om1f-8-flag2 story-flag? -> -1 }T
testing then her remarks there in turn
T{ back-area 90 at  said  back-area 90 at  said  back-area 90 at  said  back-area 90 at  said -> not-dangerous-msg wants-msg traces-msg not-dangerous-msg }T

testing a mandragora other than the white fed to it: the scene, the hand roused
yellow-mandragora give  yellow-mandragora items:items-of before !
T{ $17 use-on-place  started? -> -1 }T
T{ [: movie-skipped state-flag-set  fed-seen resident-flag? ;] 2000 soon  yellow-mandragora items:items-of -> -1 before @ 1- }T
[: doing-busy? 0= ;] 600 soon drop  0 panic-to  60 run-frames

testing the White Mandragora: the hand laid to rest, the door on open, the shadow gone
white-mandragora give
T{ 0 use-on-place  started? -> -1 }T
T{ [: movie-skipped state-flag-set  doing-busy? 0= ;] 2000 soon  white-mandragora-used story-flag?  hand-door doors:door-locked?  shadow-fx fx-alive? -> -1 -1 0 0 }T
testing and the back after it: what the mandragora is for
T{ back-area 90 at  said -> substitute-msg }T
60 run-frames

testing the back door unlocked from this side: the stalker sent on (the way out the other door's)
back-door lock  leaving-door lock  room-afresh
back-door-area 0 at   \ (before it, in its area)
T{ [: 1 before-locked? ;] in-place -> -1 }T
circle press  [: back-door doors:door-locked? 0= ;] 600 soon drop  300 read-on
T{ back-door doors:door-locked?  leaving-door doors:door-locked?  [: stalker-leaving @ ;] in-place -> 0 0 0 }T

testing the splash as she steps into the pond (each spot once a visit)
T{ 0 70e 7e -20e put  0 80.25e 7e -6e put  [: splashed @ 1 and ;] in-place -> 1 }T

testing the hangers as Hewie brushes past: swinging, the first creaking
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  game:hewie send join-fiona  60 run-frames  hewie-brought
75e 7e -9.5e hewie-at  75e 7e -8e hewie-at  77e 7e -9.5e hewie-at
T{ [: first-hanger hanging swing-left object@ f0>  rung @ ;] in-place -> -1 -1 }T

testing Hewie into the thicket at her "go there" toward it, out at her "come back"; her remark with him there
room-afresh  hewie-brought  -90 90e 7e -9e put  60 run-frames
go-there-cmd command
T{ [: [: hiding? ;] in-place ;] 900 soon -> -1 }T
T{ thicket-area -60 at  said -> eyes-msg }T
come-back-cmd command
T{ [: [: hiding? 0= ;] in-place ;] 900 soon -> -1 }T

testing the ambush: Hewie in the thicket, Daniella by it - the scene, its file, his trust
room-afresh  hewie-brought  -90 90e 7e -9e put  60 run-frames  go-there-cmd command
T{ [: [: hiding? ;] in-place ;] 900 soon -> -1 }T
90 40e 7e 40e put  3 story.base:partner-load  5 run-frames
old-mansion-1f-8 $1DE game:daniella send stalker-in  5 run-frames
60e 7e -10e stalker-at
T{ [: movie-skipped state-flag-set  ambush-seen resident-flag? ;] 1500 soon  ambush-file filed? -> -1 -1 }T
300 play-on  game:daniella send stalker-out  5 run-frames

testing Hewie's bark at the glow (once a visit), his errand for it: the Prima Materia, the file
none-glowing  room-afresh  hewie-brought
-111.73e 15e -115.37e hewie-at  10 run-frames
T{ [: hewie-noticed @ 1 and ;] in-place -> 1 }T
[: doing-busy? 0= ;] 600 soon drop
[: ['] sends-for-glow start drop ;] in-place
T{ [: prima-materia items:items-of 0> ;] 2000 soon  [: glow-shown @  glow-fx effect @ ;] in-place -> -1 0 0 }T
300 read-on  [: doing-busy? 0= ;] 900 soon drop

testing his errand for the Antimony Tube across the wall
antimony-tube items:items-of before !
[: ['] fetches-tube start drop ;] in-place
T{ [: om1f-8-antimony-tube story-flag? ;] 2000 soon  antimony-tube items:items-of -> -1 before @ 1+ }T
300 read-on  [: doing-busy? 0= ;] 900 soon drop

testing the movies replayed (the extras): each played, then back by the forced way
: replays ( n -- flag )
    replay-var var!  events-held state-flag-set  old-mansion-1f-8 -1 rooms send go-to-room  5 run-frames
    started?  [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 2000 soon and ;
T{ 60 replays -> -1 }T
T{ 62 replays -> -1 }T
T{ 63 replays -> -1 }T
test-summary
