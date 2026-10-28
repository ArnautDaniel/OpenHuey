\ off-map-e0's place: brought in by the forced way, Fiona played - the run set out (the hiding
\ places' counts afresh, Fiona and Hewie at their spots, the stalker into $E1, the room ahead
\ loaded, the clock from 0 and Hewie played, the castle's doors on the way shut, the far ones
\ locked) and the world let go; then, the run going on: in by exit 0 (camera 1); its parts; the fan
\ turning; the glass of the window and the door; its sounds; its cameras; the wardrobe - Fiona
\ slips in at Hewie's command as he stands by it, out at his next; the stalker coming in while she
\ hides searches (counted); left behind in it, she's out; out by exit 0, its door open. Headless:
\   build/openhuey --test tests/places/test_off_map_e0.fs
IN: test-off-map-e0
USING: tester engine game-state actors messages keys room-names game debug facts flag-names story-names
    stalker-names fiona.state story.kit items.names story.clock doors story.places.off-map-e0 ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: soon ( xt n -- flag )  0 ?do  dup execute if  drop true unloop exit  then  game-tick  loop  drop false ;
: her ( xt -- x )  game:fiona enter  execute  leave-actor ;
: his ( xt -- x )  game:hewie enter  execute  leave-actor ;
: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: doing-busy? ( -- flag )  [: acts @ ;] in-place 0<> ;
: on-floor ( F: x y z -- x y' z )  {: F: x y z :}  x  x y z nav-floor drop  z ;
: put ( deg F: x y z -- )   \ (on the floor, on a triangle she walks - else the nearest the mesh has)
    on-floor s>f tp-facing  [: her-at $28020018 v-tri-in  dup 0< if  drop her-at vec@ nav-tri  then  her-tri ! ;] her  10 run-frames ;
: at ( area deg -- )  >r area-middle drop r> put ;
: hput ( deg F: x y z -- )  on-floor  s>f deg>rad xyzw>cells  game:hewie send place  10 run-frames ;   \ (Hewie, there on the floor)
: hat ( area deg -- )  >r area-middle drop r> hput ;
: his-camera@ ( -- set path )  game:hewie cam-setup-of ;
: his-tri ( -- tri )  [: self body-tri ;] his ;
: on-tri ( tri -- )  0 swap tri-center put ;
: shown-soon ( -- msg )  [: window-shown 0< 0= ;] 600 soon drop  window-shown ;
: said ( -- msg )
    circle press  shown-soon
    100 0 do  window-shown 0< 0= if  cross press  then  10 run-frames  doing-busy? 0= window-shown 0< and if  leave  then  loop ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: play-on ( n -- )   \ (its messages read on - the first answer yes - its movies skipped)
    0 ?do  window-shown 0< 0= if  i 20 mod 0= if  cross press  then  then
        movie-skipped state-flag-set  game-tick  loop ;
: spare@ ( -- room )  game:rooms enter  rooms:spare @  leave-actor ;   \ (the room loaded ahead; -1 none)
: her-camera@ ( -- set path )  game:fiona cam-setup-of ;
: across ( gate side -- )   \ (stepped over the gate's line: from the other side to this one, 4 from it)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  put
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  put  10 run-frames ;
: hacross ( gate side -- )   \ (Hewie stepped over the gate's line, as across)
    {: g side | F: ax F: ay F: az F: bx F: bz F: ox F: oz :}
    g 0 area-corner drop  to az to ay to ax   g 1 area-corner drop  to bz fdrop to bx
    bz az f- to ox  bx ax f- fnegate to oz   ox ox f* oz oz f* f+ fsqrt 4e fswap f/
    fdup ox f* side s>f f* to ox  oz f* side s>f f* to oz
    0  ax bx f+ 2e f/ ox f-  ay  az bz f+ 2e f/ oz f-  hput
    0  ax bx f+ 2e f/ ox f+  ay  az bz f+ 2e f/ oz f+  hput  10 run-frames ;
: hinto ( area -- )   \ (a gate: crossed one way - the one that gives him a camera, if either does - else back too)
    dup area-kind if  0 hat exit  then
    game:hewie 99 99 cam-char-setup  dup 1 hacross  his-camera@ 99 = swap 99 = and if  -1 hacross  else  drop  then ;
: into ( area -- )   \ (a gate crossed both ways; else its middle)
    dup area-kind 0= if  dup 1 across  -1 across  else  0 at  then ;
: leads ( room exit -- room' )  room-exit-leads drop ;
variable ex
: opened ( exit -- )  ex !  [: ex @ exit-door 1 doors-actor send door-opened ;] in-place  5 run-frames ;
: both-ways ( area -- )  dup 1 hacross  -1 hacross ;
: tap ( key -- )  dup true key-hold  game-tick  false key-hold  game-tick ;
: command ( -- )  key: I tap ;   \ (his gesture: up)
: hidden? ( -- flag )  fiona-hidden state-flag? ;
: stalker-here ( room tri -- )  debilitas-kind stalker-ready  30 run-frames  game:debilitas send stalker-in  10 run-frames ;

free-play  10 run-frames  0 game:fiona send costume  stalkers-blind state-flag-set  summoner-on state-flag-clear
clock-start  30 run-frames   \ (the run: Hewie the one played)
: by ( exit -- )  off-map-e0 swap room-by!  60 run-frames ;
: by-it ( -- )  0 11e 0e -45e hput ;   \ (Hewie in front of it)
: her-here ( -- )  0 0e 0e -30e put ;
: fan-turn ( F: -- a )  [: s" fan" turn-y placed-f@ ;] in-place ;
: shut? ( door -- flag )  doors:opened-bit doors:has? 0= ;
testing brought in by the forced way, Fiona played: the run set out
0 hewie-played !  5 $30 var!  5 $33 var!  0 $2E var!  debilitas-kind stalker-ready  30 run-frames  world-held state-flag-set
$11D 1 game:doors send door-opened  $127 1 game:doors send door-opened  10 run-frames
T{ off-map-e0 forced-exit room-by!  5 run-frames  hewie-played?  $30 var@  $33 var@  spare@ -> -1 0 0 off-map-e0 0 leads }T
T{ [: her-tri @ ;] her  his-tri  game:fiona cam-setup-of -> $55 $114 0 0 }T
T{ [: stalker body-room ;] in-place  $11D shut?  $127 shut?  $127 doors:door-locked?  $123 doors:door-locked?  -> off-map-e1 -1 -1 -1 0 }T
T{ [: world-held state-flag? 0= ;] 200 soon -> -1 }T
full-suite   \ (on: waits, scenes, walks - the full run's)
[: stalker-unloaded ;] in-place  10 run-frames
testing the run going on, in by exit 0: at it, camera 1; part 0 shown, 1 not; the glass; the fan turning
T{ 0 by  room-id  his-camera@  [: 0 part?  1 part? ;] in-place -> off-map-e0 1 1 -1 0 }T
T{ [: $1A effect @ 0<>  $1B effect @ 0<> ;] in-place -> -1 -1 }T
T{ fan-turn  10 run-frames  fan-turn f= -> 0 }T
testing its sounds: the steady one started once the room's are in
T{ [: [: sounds-on @ ;] in-place ;] 200 soon -> -1 }T
testing its cameras: area $17 / $19 set 0, $18 set 1 ($1A's, set 2: no corners in its data)
T{ $18 hinto  his-camera@  $17 hinto  his-camera@  $19 hinto  his-camera@  $1A 0 area-corner >r fdrop fdrop fdrop r> -> 1 1 0 0 0 0 0 }T
testing the wardrobe: he by it, she here and free - at his command she slips in; at his next, out
0 by  her-here  by-it
T{ [: fiona here? ;] in-place  hidden? -> -1 0 }T
T{ command  [: hidden? ;] 800 soon -> -1 }T
60 run-frames   \ (the fan turns in an activity of its own: her being let go is the sign she's out)
T{ command  [: hidden? 0= ;] 300 soon  [: [: fiona held? 0= ;] in-place ;] 300 soon -> -1 -1 }T
testing the stalker coming in while she hides: he searches (counted)
her-here  by-it  command  [: hidden? ;] 800 soon drop
0 $30 var!  off-map-e0 $1C stalker-here
T{ [: came-while-hidden @ ;] in-place  $30 var@ -> -1 1 }T
[: stalker-unloaded ;] in-place  10 run-frames
testing left behind in it: he leaves by exit 0 - she's out
T{ 0 opened  0 exit-area 0 hat  [: room-id off-map-e0 <> ;] 300 soon  hidden?  stalkers-stay state-flag? -> -1 0 0 }T
testing out by exit 0 (its door open)
T{ 0 by  0 opened  0 exit-area 0 hat  [: room-id off-map-e0 <> ;] 300 soon  room-id -> -1 off-map-e0 0 leads }T
0 hewie-played !
test-summary
