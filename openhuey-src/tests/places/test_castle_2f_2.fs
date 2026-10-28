\ castle-2f-2's place (scripts/story/places/castle-2f-2.fs): the chandelier hall - coming in (the
\ doll up, the flames, the chandeliers swinging), the remarks (the blood - a fright the first time -
\ the floor, the angel, the box, the dark corner), the doll (put up; knocked by her blow, falling
\ and landing; picked up), the Album Medallion (sniffed out; picked up), the box pushed home (the
\ door unlocked), her first steps (Debilitas's scene), after the warning the ticking and the door
\ beyond (seized: the forced way; brought back pulled free; its locked door then), Debilitas's far
\ shove, Hewie wary and sitting by, the replay. Headless:
\   build/openhuey --test tests/places/test_castle_2f_2.fs
IN: test-castle-2f-2
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names floor-flags fiona.state story.kit story.places.castle-2f-2 doors items.names ;

: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: as-her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: loop-xt? ( xt -- flag )  dup ['] swinging =  over ['] wheel-starts = or  swap ['] ticking = or ;   \ (the room's own, always going)
: doing-busy? ( -- flag )  [: false  #activities 0 do  i act-slot @ 0<>  i cells act-xts + @ loop-xt? 0= and  or  loop ;] in-place ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )  on-floor s>f tp-facing  [: her-at $28020018 v-tri-in her-tri ! ;] as-her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: said ( -- msg )
    circle press  [: window-shown 0< 0= ;] 600 soon drop  window-shown
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: room-afresh ( -- )  castle-2f-2 room!  60 run-frames ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )  0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then  movie-skipped state-flag-set  game-tick  loop ;
: picked ( F: x y z -- )  {: F: x y z :}  0 x y z 4e f+ put  circle press  300 read-on ;   \ (just south of it, facing it)
: hewie-spot ( F: x y z -- )  on-floor f>cell f>cell f>cell swap rot  0 game:hewie send place  5 run-frames ;
: hewie-here ( F: x y z -- )  {: F: x y z :}  castle-2f-2 x y z nav-tri 0 game:hewie send hewie-place  30 run-frames  x y z hewie-spot ;
: filed? ( doc -- flag )
    128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
create blow-pt 3 floats allot
: blow-at ( F: x y z -- )  blow-pt 2 floats + f!  blow-pt float+ f!  blow-pt f!
    [: blow-pt f@  blow-pt float+ f@  blow-pt 2 floats + f@  body-point! ;] as-her ;
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: ~= ( F: a b -- flag )  f- fabs 0.001e f< ;
: chandelier-turn ( F: -- t )  s" chandelie" turn-y object@ ;
: by-doll ( -- )   \ (just south of it, facing it)
    [: dool place-x object@  dool place-z object@ ;] in-place {: F: dx F: dz :}  0 dx 0e dz 4e f+ put ;
create spot 12 allot
variable before

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
testing coming in (after her first steps): the doll up, the flames lit, the chandeliers swinging
c2f-2-flag1 story-flag-set  c2f-2-flag5 story-flag-clear  got-bisque-doll story-flag-clear  c1f-8-flag1 story-flag-clear  room-afresh
T{ [: doll-down @  dool place-y object@ 0.51e ~=  flame-1-fx effect @ 0<> ;] in-place -> -1 -1 -1 }T
T{ chandelier-turn  10 run-frames  chandelier-turn f- fabs 0.001e f> -> -1 }T

testing the remarks: the blood (the first time a fright, then its wonder), the floor, the angel, the box, the dark corner
c2f-2-flag4 story-flag-clear
T{ 68 50e 0e -180e put  said  68 50e 0e -180e put  said  c2f-2-flag4 story-flag? -> blood-msg why-blood-msg -1 }T
T{ 24 24e 0e 78e put  said -> nothing-msg }T
T{ -140 -174e 0e 94e put  said -> angel-msg }T   \ (facing it, off its blocked middle)
c2f-2-flag2 story-flag-clear  room-afresh
T{ box-area 0 at  said -> box-msg }T
T{ dark-area 0 at  said -> dark-msg }T

testing the doll knocked by her blow: falling, landing on the floor (still), then picked up
room-afresh  9.66e 1e 93.4e blow-at
T{ [: doll-knocks  ['] falling running? ;] in-place -> -1 }T
[: body-point-off ;] as-her
T{ [: [: dool doll-tri placed-l@ landed-bit and 0<> ;] in-place ;] 300 soon  [: dool doll-vy object@ f0=  doll-down @ ;] in-place -> -1 -1 -1 }T
bisque-doll items:items-of before !
by-doll  circle press  300 read-on
T{ got-bisque-doll story-flag?  bisque-doll items:items-of -> -1 before @ 1+ }T

testing the Album Medallion Hewie sniffs out: found, it shows; picked up
c2f-2-album-medallion-found story-flag-clear  c2f-2-album-medallion story-flag-clear
hewie-freed story-flag-set  hewie-commandable state-flag-set  room-afresh  -11.6e 0e 137e hewie-here
[: ['] album-sniffed start drop ;] in-place
T{ [: c2f-2-album-medallion-found story-flag? ;] 600 soon  [: album-fx effect @ 0<> ;] in-place -> -1 -1 }T
200 read-on
album-medallion items:items-of before !
90 -15.6e 0e 142.2e put  circle press  300 read-on   \ (west of it: the dark corner's area is south)
T{ c2f-2-album-medallion story-flag?  album-medallion items:items-of -> -1 before @ 1+ }T

testing Hewie: wary by the far door after the warning; sitting by the box pushed home (once a visit)
c1f-8-flag1 story-flag-set  found-file-209 story-flag-clear  room-afresh  -144e 0e 100e hewie-here
T{ [: [: wary-noticed @ ;] in-place ;] 60 soon -> -1 }T
300 run-frames  c1f-8-flag1 story-flag-clear  c2f-2-flag2 story-flag-set  room-afresh  33e 0e 111e hewie-here
T{ [: [: sits-noticed @ ;] in-place ;] 60 soon -> -1 }T
300 run-frames  game:hewie send part-from-fiona

testing after the warning: the ticking beyond the door
c1f-8-flag1 story-flag-set  found-file-209 story-flag-clear  room-afresh
T{ [: ['] ticking running? ;] in-place -> -1 }T

testing the box pushed home: fixed there, the door beyond it unlocked
c2f-2-flag2 story-flag-clear  c1f-8-flag1 story-flag-clear  box-door s" doors" actor-named send lock  room-afresh
[: 0 0 0 $377 $3BB obstacle-in ;] in-place  30 run-frames
T{ c2f-2-flag2 story-flag?  box-door door-locked? -> -1 0 }T

testing Debilitas's far shove: then gone
2 story.base:partner-load  5 run-frames  room-afresh  castle-2f-2 $1A2 game:debilitas send stalker-in  30 run-frames
[: ['] shoves-far start drop ;] in-place
T{ [: [: his-shove @ 0= ;] in-place ;] 1500 soon -> -1 }T
game:debilitas send stalker-out  60 run-frames

testing her first steps by the chandeliers: Debilitas's scene - he comes in, looks about; the file
c2f-2-flag1 story-flag-clear  room-afresh  chandelier-area 1 across  chandelier-area -1 across
T{ [: movie-skipped state-flag-set  $224 filed? ;] 1500 soon -> -1 }T
T{ c2f-2-flag1 story-flag? -> -1 }T
T{ game:debilitas body-room -> castle-2f-2 }T
game:debilitas send stalker-out  60 run-frames

testing after the warning, the door beyond tried: seized in there (the forced way)
c1f-8-flag1 story-flag-set  found-file-209 story-flag-clear  room-afresh
beyond-exit 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames
beyond-exit game:rooms send exit-prepare  5 run-frames   \ (the room beyond loaded ahead, as her way to it does: put there, she didn't come by it)
T{ circle press  [: room-id castle-2f-7 = ;] 900 soon -> -1 }T   \ (castle-2f-7: Daniella's scene)
testing ... brought back pulled free (2f-7's scene over, its forced way back to the room left): the file; then its locked door - "I have to get out"
T{ [: movie-skipped state-flag-set  room-id castle-2f-2 = ;] 1500 soon -> -1 }T
T{ [: doing-busy? 0= ;] 600 soon -> -1 }T
T{ found-file-209 story-flag? -> -1 }T
T{ fled-file filed? -> -1 }T
door-beyond s" doors" actor-named send lock  10 run-frames
spot vec@ tp  10 run-frames
T{ circle press  [: window-shown must-flee-msg = ;] 300 soon -> -1 }T
300 read-on  free-play  10 run-frames  0 game:fiona send costume

testing the movie replayed (the extras): Debilitas's scene
6 $2A var!  events-held state-flag-set  castle-2f-2 -1 rooms send go-to-room  5 run-frames
T{ [: cutscene-active? movie-status 1 = or ;] 200 soon -> -1 }T
T{ [: movie-skipped state-flag-set  events-held state-flag? 0= ;] 1500 soon -> -1 }T
test-summary
