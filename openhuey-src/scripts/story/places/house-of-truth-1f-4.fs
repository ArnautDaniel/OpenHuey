\ story/places/house-of-truth-1f-4.fs - the House of Truth's 1F room of the demolisher ($8C): the
\ generator that won't run - jolted by her blows (the dynamo shaking) it runs: the first time the scene
\ (Lorenzo let in, the door locked behind - "No! It's locked!"), then the demolisher's belt for 240
\ frames; while it runs, Lorenzo stepping under it - crushed (the scene; the Sol Key there) - or she -
\ caught (the game over); Hewie moved off its way as she comes back; the generator's and the
\ demolisher's remarks; the locked door - its remark in turn with the symbol carved in it (its file) -
\ and the Sol Key turned in it; the Sol Key taken; Hewie's look; the replays. A place (story/kit.fs).
\ The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-4.fs (converted from the
\ game's, not loaded).
IN: story.places.house-of-truth-1f-4
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit story.places.house-of-truth fiona.anims gameover scenes ;

own-reset  truth-owns owns !

0 constant dead-gen-msg    \ "Looks like a power generator. The switch is turned on, but it's not getting any..."
1 constant jolt-msg        \ "Maybe something is wrong with the contacts? A good jolt might knock it back int..."
2 constant gone-msg        \ "It looks completely and utterly dead. I don't think physical abuse is going to make..."
3 constant no-msg          \ "No! It's locked!"
4 constant locked-msg      \ "The door is locked."
5 constant symbol-msg      \ "A strange symbol is carved into the door. I wonder what this symbol means?"
6 constant demolisher-msg  \ "This looks like a demolisher. These things are used to smash large rocks into bits..."
7 constant not-running-msg \ "It's not running right now... Maybe there is a power switch somewhere that needs to..."
own: gen-remarked   own: belt-running   own: reacted   own: hewie-way-open   own: locked-remarked   own: demolisher-remarked
own: door-pending          \ the things under it to be gone as she next steps in
own: jolts                 \ the generator's jolts to come (3, worn by her blows)
0 constant sol-fx   0 constant belt-part   $A constant generator-area   $C constant demolisher-area
6 constant back-area   $B constant under-area   0 constant start-sound   1 constant run-down-sound   2 constant run-sound
$E5 constant study-door   $E2 constant sun-door   $E6 constant other-door   $608 constant key-anim
1 constant let-in-movie   0 constant let-in-cutscene   3 constant caught-movie   2 constant caught-cutscene
5 constant crushed-movie   4 constant crushed-cutscene   $250 constant demolisher-file   $251 constant symbol-file
: debris ( on? -- )   \ (the room's things: the rubble $C .. $16 shown while the demolisher hasn't run)
    {: on :}  7 object-hide  8 object-show  9 object-show  $A object-hide  $B object-hide
    $17 $C do  i on if  object-show  else  object-hide  then  loop ;
: came-in ( exit -- )
    drop  room-sounds
    found-file-250 fact? 0=  ht1f-4-flag1 fact? 0= and if  3 jolts !  true debris
    else  false debris  got-sol-key fact? 0= if  sol-fx 0e -2.2e 65e flicker-at  then  then
    belt-part hide-part
    ht1f-4-flag1 fact? 0= if  force-followed state!  then
    1 $2300 reverb ;

\ ---- the dynamo shaking, the demolisher's belt ----
: dynamo ( -- addr len )  s" dynamo" ;
: shakes ( -- )   \ (in z: its phase back 60 degrees a frame, its size down a quarter - till it dies out)
    dynamo placed-here? 0= if  exit  then
    dynamo place-z object@ dynamo $38 object!  0e dynamo $34 object!  1e dynamo $3C object!
    begin
        dynamo $34 object@ 60e deg>rad f-  fdup pi fnegate f< if  2e pi f* f+  then  dynamo $34 object!
        -0.25e dynamo $3C object+!
        dynamo $38 object@  dynamo $3C object@ dynamo $34 object@ fsin f* f+  dynamo place-z object!
        dynamo $3C object@ f0> while  yield
    repeat ;
: roller ( -- addr len )  s" roller" ;   : belt ( -- addr len )  s" belt" ;
: belt-set ( -- )  0e roller turn-y object!  5.1e roller place-y object!  0e belt place-x object!  -3e belt place-y object!  0.4e belt $30 object! ;
: belt-step ( -- )   \ (the wheel turned by the belt at its speed, both shaking a little)
    belt $30 object@ 8e f/ roller turn-y object+!   rnd 0.5e f- 0.1e f* 5.1e f+ roller place-y object!
    belt $30 object@ belt place-x object+!  begin  belt place-x object@ 10e f< 0= while  -10e belt place-x object+!  repeat
    rnd 0.5e f- 0.1e f* 3e f- belt place-y object! ;
: belt-there? ( -- flag )  roller placed-here?  belt placed-here? and ;
: runs ( -- )   \ (an activity: the demolisher running 240 frames, then slowing to a stop)
    belt-part show-part  run-sound 24e 5e 65e room-sound
    -1 belt-running !  -1 hewie-way-open !  -1 door-pending !
    belt-there? if  belt-set  then  240 0 do  belt-there? if  belt-step  then  yield  loop
    run-down-sound 24e 5e 65e room-sound  0 belt-running !  belt-part hide-part
    belt-there? if  begin  belt-step  -0.01e belt $30 object+!  belt $30 object@ f0> while  yield  repeat  0e belt $30 object!  then
    0 keeps-hewie-off floor-off  3 jolts !  belt-part hide-part ;
: under-gone ( -- )  $B story.words:things-age-out-in ;   \ (what lies under the demolisher: gone)

\ ---- the first time it runs: Lorenzo let in ----
create belt-cues  $F , $12 , $13 , $14 , $17 , $28 , $29 , $2A , $2B , $2C , $31 , $37 , $38 , $39 , $3A , $3B , $3C ,
    $53 , $54 , $63 , $64 ,   \ (the frames the belt flickers at in the scene: shown, hidden, in turn)
: let-in-cues ( -- )
    belt-part hide-part
    21 0 do  belt-cues i cells + @  begin  cutscene-frame over < while  yield  repeat  drop
        i 1 and if  belt-part hide-part  else  belt-part show-part  then  loop ;
: let-in-scene ( -- )
    let-in-movie scenes:unseen-movie let-in-cutscene load-scene drop  ['] let-in-cues start drop
    panic-eased  -1 run-scene  ['] let-in-cues stop-doing ;
: lets-in ( -- )
    force-followed state-off  scene-locked state!  stalkers-stay state!  fiona hold  under-gone  yield
    15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    hewie-in? if  hewie let-go  then  stalker let-go  0 summon-take
    let-in-scene
    fiona $106 -44 46.98e -19.5e place-at  hewie activate  hewie $53 -94 23.54e 2.5e place-at
    partner activate  this-room @ 103 stalker-to  stalker $67 98 -69.97e 2.34e place-at  stalker 3 3 camera-set
    study-door false open-mark  1 door-tried  doors-redone  study-door lock  scene-5-pending state!
    stalker full-health  6 object-show  false debris  2 2 player-camera  camera-restart  found-file-250 fact!
    15 fade-in  $46 resident-flag-set  demolisher-file give-file  stalkers-stay state-off  scene-locked state-off
    ['] runs start drop ;

\ ---- her blows at the generator ----
: jolted ( -- )   \ (an activity: a jolt - worn by her blow, the dynamo shaken; at none it runs)
    start-sound room-bank fiona sound-by  1 $FF 4 rumble-pad
    jolts @  fiona-sub-now @ story.words:weak-blow-sub = if  1  else  5  then  -  0 max  jolts !
    shakes
    jolts @ 0= if
        found-file-250 fact? 0= if
            begin  fiona-free? 0= while  yield  repeat  panic-eased  ['] lets-in fiona start-holding drop
        else  ['] runs start drop  then
    then  30 frames ;

\ ---- while it runs: Lorenzo under it - crushed; she - caught ----
: crushed-scene ( -- )
    crushed-movie scenes:keyed-movie crushed-cutscene load-scene drop  $64 $FF movie-key  panic-eased  -1 run-scene ;
: crushed ( -- )
    scene-locked state!  fiona hold  under-gone  yield  15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    run-down-sound room-bank stop-sound  ['] runs stop-doing  hewie-in? if  hewie let-go  then  stalker let-go  0 summon-take
    crushed-scene  stalker let-go  3 summon-take  summoner-on state-off  study-door unlock
    fiona $7A 18 -10.04e 37.76e place-at  hewie activate  hewie $9D 18 6.01e 27.57e place-at  50 trust+
    ht1f-4-flag1 fact!  1 1 player-camera  camera-restart  6 object-show  false debris
    sol-fx 0e -2.2e 65e flicker-at  belt-part hide-part  1 prepare-exit  15 fade-in  $48 resident-flag-set
    0 keeps-hewie-off floor-off  scene-locked state-off  scene-5-pending state-off ;
: she-caught ( -- )
    capture-no-end state!  no-pause state!  scene-locked state!  fiona hold  under-gone  yield
    15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    ['] runs stop-doing  hewie-in? if  hewie let-go  then  stalker let-go  0 summon-take
    caught-movie scenes:unseen-movie caught-cutscene load-scene drop  panic-eased  -1 run-through
    $47 resident-flag-set  1e sounds-scaled  movie-death game-over ;

\ ---- Hewie moved off its way as she comes back ----
: hewie-moved ( -- )
    hewie-hp-now @  hewie let-go  hewie activate  ?dup 0= if  10  then  hewie send set-hp  under-gone
    33 chance? if  $146  else  50 chance? if  $130  else  $11C  then  then  hewie swap put-on-tri
    0 keeps-hewie-off floor-on  0 hewie-way-open ! ;

\ ---- the doors ----
: the-door ( -- )   \ (locked: tried - its remark in turn with the symbol's; with Lorenzo let in, "No! It's locked!")
    scene-5-pending state? if  no-msg show exit  then
    fiona 0 to-door
    locked-remarked @ 0=  chased? or  in-panic? or if
        in-panic? chased? or if  fiona rattle-front-slow-anim play  fiona -1 idle  7 frames
        else  fiona rattle-front-anim play  fiona -1 idle  7 frames  locked-msg say  then
        -1 locked-remarked !
    else
        4 frames  fiona $BF 90 90.5e 0e place-at  world-frozen state!
        1 20e 0e 0e 2e shot  4 frames  symbol-msg say  4 frames  shot-off  world-frozen state-off  0 locked-remarked !
    then  symbol-file give-file ;
: turns-key ( -- )  key-anim sun-door other-door sol-turned ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    8 = if  ['] turns-key start drop  true  else  false  then ;

\ ---- what she may do or say here ----
: the-generator ( -- )
    fiona wait-move  fiona 50e -32e turn-to-xz  fiona look-low-anim start-anim  16 frames
    ht1f-4-flag1 fact? if  gone-msg  else  gen-remarked @ if  jolt-msg  else  -1 gen-remarked !  dead-gen-msg  then  then
    say  fiona wait-anim ;
: the-demolisher ( -- )
    fiona wait-move
    demolisher-remarked @ 0= if  demolisher-msg say  -1 demolisher-remarked ! exit  then
    fiona hand-to-mouth-anim 10 play-blend  not-running-msg say ;
: takes-sol ( -- )
    fiona hold  fiona wait-move  fiona 0e 65e turn-to-xz  fiona pick-up-anim play  4 frames
    sol-fx effect-off  sol-key item-named  sol-key 1 give-item  sol-key show-item-tab  fanfare  obtained-2-msg say
    fiona up-from-pick-anim play  got-sol-key fact! ;
: spots ( -- )
    fiona generator-area in-area?  fiona 90 50 heading? and if  ['] the-generator unhurried offer  then
    8 locked-request? if  ['] the-door any-time offer  then
    ht1f-4-flag1 fact?  got-sol-key fact? 0= and  fiona 0e -3.2e 65e 5e 5e touches? and if  ['] takes-sol calm-only offer  then
    found-file-250 fact? 0=  fiona demolisher-area 50 faces-area? and if  ['] the-demolisher unhurried offer  then ;

\ ---- the movies replayed (the extras) ----
: replay-room ( -- )  7 object-hide  $A object-hide  $B object-hide ;
: replay-let-in ( -- )  fiona hold  replay-room  let-in-scene  replay-over ;
: replay-caught ( -- )
    fiona hold  replay-room  caught-movie scenes:unseen-movie caught-cutscene load-scene drop  panic-eased  -1 run-scene  replay-over ;
: replay-crushed ( -- )  fiona hold  replay-room  lorenzo-kind 3 figure-in  yield  3 figure-on  crushed-scene  3 figure-out  replay-over ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,  5 , 1 , 1 ,  6 , 0 , 0 ,  7 , 2 , 2 ,  8 , 1 , 1 ,  9 , 3 , 3 ,   \ (area, set, path)
create ahead  2 , 0 ,  3 , 1 ,   \ (area, exit loaded ahead)
create machine-look  44.93e 0e -26.49e  27e 10e 10e look,
: while-it-runs ( -- )   \ (the scene done, the house not yet freed of him)
    belt-running @  reacted @ 0= and if
        stalker under-area in-area? if  -1 reacted !  panic-eased  ['] crushed fiona start-holding drop
        else  fiona under-area entered? if  -1 reacted !  panic-eased  ['] she-caught fiona start-holding drop  then  then
    then
    hewie-way-open @ if
        fiona back-area entered? if  hewie-moved
        else  hewie 0 on-group? 0= if  0 keeps-hewie-off floor-on  0 hewie-way-open !  then  then
    then
    door-pending @  fiona back-area entered? and if  under-gone  0 door-pending !  then ;
: each-frame ( -- )
    2 exits-out  cameras 6 cameras-at  ahead 2 ways-out
    ht1f-4-flag1 fact? 0=  belt-running @ 0= and  50e 0e -28e 5e 11e her-blow-in? and  ['] jolted running? 0= and if
        ['] jolted start drop
    then
    found-file-250 fact?  ht1f-4-flag1 fact? 0= and if  while-it-runs  then
    machine-look 1 hewie-looks-at
    belt-running @ if  run-sound 24e 5e 65e keep-sound  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act case  $B of  ['] replay-let-in  endof  $C of  ['] replay-caught  endof  >r ['] replay-crushed r>  endcase
        start drop exit
    then
    0 0 0 way-in  1 3 3 way-in ;
: went ( -- )  force-followed state-off ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-4 place!
' items-used house-of-truth-1f-4 items-here
