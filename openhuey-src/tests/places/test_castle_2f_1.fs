\ castle-2f-1's place (scripts/story/places/castle-2f-1.fs): the music room - coming in (the dial set
\ from its variable, the Refined Magnesia and the Lavender showing), the device (its first remark;
\ the dial turned a number by the stick - at the right one its lens catching the light; the moon
\ let through: its remark), the piano, the speakers, the door unlocked from this side, the Refined
\ Magnesia (sniffed out; picked up), the Lavender, the dials right here (the scene, down to
\ castle-1f-12), up from it with its dials right (the scene, back down), back up set right (the
\ moon door unlocked), the replay. Headless:
\   build/openhuey --test tests/places/test_castle_2f_1.fs
IN: test-castle-2f-1
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    floor-flags fiona.state story.kit story.places.castle-2f-1 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-2f-1 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
: hewie-here ( F: x y z -- )  {: F: x y z :}  castle-2f-1 x y z nav-tri 0 game:hewie send hewie-place  30 run-frames  x y z hewie-spot ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: dial-turn ( F: -- deg )  s" syuukouki" turn-y object@ rad>deg ;
create spot 12 allot
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in: the dial set from its variable (2: 60 degrees); the Refined Magnesia and the Lavender showing
c1f-12-flag1 story-flag-clear  2 3 var!  c2f-1-refined-magnesia-found story-flag-set  c2f-1-refined-magnesia story-flag-clear
c2f-1-lavender story-flag-clear  room-afresh
T{ dial-turn 60e ~=  [: magnesia-fx effect @ 0<>  lavender-fx effect @ 0<> ;] in-place -> -1 -1 -1 }T
T{ c1f-12-flag1 story-flag?  doing-busy? -> 0 0 }T   \ (not come by a forced way: nothing of the moon's)

testing the remarks: the piano, the speakers
T{ 90 10e 80e -16e put  said -> piano-msg }T   \ (facing its middle)
T{ speakers-area 180 at  said -> speakers-msg }T

testing the device: its first remark; the dial turned on a number by the stick (to 3, the right one: the lens's glint)
0 -36e 90e -2e put  circle press   \ (facing its middle)
T{ [: window-shown device-msg = ;] 300 soon -> -1 }T
: past-remark ( -- )  begin  window-shown device-msg = while  cross press  10 run-frames  repeat ;
past-remark  [: window-shown rotate-msg = ;] 300 soon drop  key: Right tap
T{ [: s" window-flash" actor-named 0< 0= ;] 200 soon  3 var@ -> -1 3 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
cross press  60 run-frames
T{ doing-busy?  window-shown 0< -> 0 -1 }T
testing ... the moon let through: its remark
c1f-12-flag1 story-flag-set
T{ 0 -36e 90e -2e put  said -> moonlight-msg }T
c1f-12-flag1 story-flag-clear

testing the door unlocked from this side: before it, the button
variable the-door  [: locked-exit exit-door ;] in-place the-door !
the-door @ s" doors" actor-named send lock  room-afresh
[: locked-exit ;] in-place 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
T{ circle press  [: window-shown unlocked-msg = ;] 300 soon -> -1 }T
300 read-on
T{ the-door @ door-locked? -> 0 }T

testing the Refined Magnesia and the Lavender: picked up
refined-magnesia items:items-of before !
39.5e 80e -38.8e picked
T{ c2f-1-refined-magnesia story-flag?  refined-magnesia items:items-of -> -1 before @ 1+ }T
lavender items:items-of before !
41.5e 68e 31e picked
T{ c2f-1-lavender story-flag?  lavender items:items-of -> -1 before @ 1+ }T

testing Hewie sniffing out the Refined Magnesia
c2f-1-refined-magnesia-found story-flag-clear  c2f-1-refined-magnesia story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  35e 80e -38.8e hewie-here
[: ['] magnesia-sniffed start drop ;] in-place
T{ [: c2f-1-refined-magnesia-found story-flag? ;] 600 soon  [: magnesia-fx effect @ 0<> ;] in-place -> -1 -1 }T
200 run-frames  game:hewie send part-from-fiona

testing the dials set right here: the scene, down to castle-1f-12 by the forced way
5 0 var!  2 1 var!  2 3 var!  room-afresh  0 -36e 90e -2e put  circle press  [: window-shown device-msg = ;] 300 soon drop
past-remark  [: window-shown rotate-msg = ;] 300 soon drop  key: Right tap  120 run-frames  cross press
T{ [: movie-skipped state-flag-set  room-id castle-1f-12 = ;] 1500 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing up from castle-1f-12 with its dials right: the scene, back down (the other way)
c1f-12-flag1 story-flag-clear  castle-2f-1 forced-exit rooms send go-to-room  5 run-frames
T{ [: movie-skipped state-flag-set  room-id castle-1f-12 = ;] 1500 soon -> -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing back up, set right: the moon door unlocked, the moon let through
moon-door s" doors" actor-named send lock  c1f-12-flag1 story-flag-clear  castle-2f-1 forced-exit 1 or rooms send go-to-room  5 run-frames
T{ [: doing-busy? 0= ;] 300 soon  moon-door door-locked?  c1f-12-flag1 story-flag? -> -1 0 -1 }T
free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras): played, then on to castle-1f-12's
events-held state-flag-set  castle-2f-1 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  room-id castle-1f-12 = ;] 1500 soon  $2C var@ -> -1 1 }T
test-summary
