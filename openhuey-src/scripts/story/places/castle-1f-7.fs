\ story/places/castle-1f-7.fs - the breaker room in Belli Castle's 1F ($15): the gate (a lid,
\ "kousi") over the way in, held up while someone stands on castle-1f-4's floor plate - it falls
\ once nobody does and nobody is under it; the breaker box humming - in among its wires before its
\ handle is lowered, she is killed (a game over); lowered, the door (9) unlocked; the stabbed doll;
\ Hewie barking at the breaker; brought back in by the forced way, the gate rattles shut on her and
\ she is sent back. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-1f-7.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.castle-1f-7
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects story.kit fiona.anims hewie.anims hewie-names scenes ;

own-reset

\ ---- what is said here ----
0 constant breaker-msg     \ "It's a breaker box. Lower the handle? YES NO"
1 constant lowered-msg     \ "The handle has been lowered."
2 constant doll-msg        \ "The doll has been stabbed with small iron sticks... This place is so freakish."
3 constant doll-2-msg      \ "Is someone just trying to scare me?"
4 constant doll-3-msg      \ "I just want to get out of this room as quickly as possible."
5 constant gate-msg        \ "It's closed. I wonder if there isn't a way for me to raise this?"
6 constant locked-msg      \ "It's locked and won't open. Now, how am I going to get you open?"

\ ---- its own state (a visit) ----
own: hum-started           \ the breaker's hum begun (then kept going)
own: wires-touched         \ she went in among the wires (once a visit)
own: gate-held             \ the gate held up as she came in (Hewie on the plate next door)

\ ---- the room's things ----
0 constant gate-floor   1 constant breaker-floor   0 constant handle-part   1 constant spark-part
2 constant lever-object   3 constant reach-motions   0 constant lower-anim   9 constant breaker-door
2 constant leaving-0-area   3 constant near-exit-1-area   8 constant breaker-area   9 constant gate-area
$11 constant wires-area   1 constant gate-exit   1 constant player-exit-area   0 constant locked-exit
0 constant breaker-sound   1 constant gate-sound   2 constant thud-sound   3 constant hum-sound
1 constant death-movie   0 constant death-cutscene   $1C constant death-seen   7 constant doll-looks-var
$F0 constant clear-tri   $C7 constant thrown-tri   $8277 constant breaker-file
: lowered? ( -- flag )  breaker-lowered fact? ;
: hum-at ( F: -- x y z )  -10e 15e 0e ;   : gate-at ( F: -- x y z )  0e 30e 30e ;

\ ---- the gate: shut, up; falling and bouncing shut (its first landing a thud, the gate-rising
\ fact cleared); rattling up and down a little more each time, till above 0 ----
: gate ( -- addr len )  s" kousi" ;
$30 constant not-landed   $34 constant speed   -25.3e fconstant shut-y
: gate-shut ( -- )  gate placed-here? 0= if  exit  then  shut-y gate place-y object!  0e gate speed object! ;
: gate-up ( -- )  gate placed-here? 0= if  exit  then  0e gate place-y object!  1e gate not-landed object!  0e gate speed object! ;
: lands ( -- )   \ (each time it reaches the floor: the first time the thud; a fifth of its speed back up)
    gate not-landed object@ f0> if
        -1e gate not-landed object!  gate-rising fact-off
        cam-scene? 0= if  thud-sound gate-at room-sound  then
    then
    shut-y gate place-y object!  gate speed object@ -0.2e f* gate speed object! ;
: falls ( -- settled? )
    -0.1e gate speed object+!  gate speed object@ gate place-y object+!
    gate place-y object@ shut-y f< if  lands  then
    gate speed object@ fabs 0.02e f< dup if  shut-y gate place-y object!  then ;
: rattles ( -- done? )
    gate speed object@ f0< if  1e 0.4e  else  -1e 0.1e  then  gate place-y object+!  gate speed object!
    rnd 0.1e f* gate place-y object+!
    gate place-y object@ f0> dup if  0e gate place-y object!  then ;
: gate-falls ( -- )  gate placed-here? 0= if  exit  then  begin  falls 0= while  yield  repeat ;
: gate-rattles ( -- )  gate placed-here? 0= if  exit  then  begin  rattles 0= while  yield  repeat ;

\ ---- as she comes in ----
: came-in ( exit -- )
    room-sounds  0 gate-held !
    dup forced-exit = if
        drop gate-shut  floor-plate-pressed fact-off
    else
        stalker-in?  stalker gate-floor on-group? and if  stalker clear-tri put-on-tri  then   \ (not left under the gate)
        gate-exit <> if  gate-rising fact-off  floor-plate-pressed fact-off  then
        gate-rising fact? 0=  floor-plate-pressed fact? 0= and if
            gate-floor barred-floor floor-on  gate-shut
        else
            floor-plate-pressed fact!  gate-floor barred-floor floor-off  -1 gate-held !  gate-up
        then
    then
    lowered? if
        lever-object object-hide  handle-part show-part  spark-part hide-part
    else
        lever-object object-show  handle-part hide-part  spark-part show-part  breaker-floor keeps-hewie-off floor-on
    then ;

\ ---- the gate: held while Hewie waits (or lies hurt) on the plate next door; let go, it falls -
\ unless someone is under it (Hewie kept from leaving meanwhile) ----
: hewie-on-plate? ( -- flag )
    hewie body? 0= if  false exit  then  hewie body-room castle-1f-4 <> if  false exit  then
    hewie-act-now @ dup wait-a-while-act = swap knocked-down-act = or ;
: someone-under? ( -- flag )
    fiona gate-floor on-group?  hewie gate-floor on-group? or  stalker gate-floor on-group? or ;
: gate-down ( -- )  gate-rising fact!  gate-sound gate-at room-sound  gate-falls ;
: gate-watch ( -- )
    floor-plate-pressed fact? 0=  hewie-on-plate? 0= or if  0 gate-held !  then
    castle-1f-7 hewie-slot 0 doors-into
    floor-plate-pressed fact?  gate-held @ 0= and if
        someone-under? if  castle-1f-7 hewie-slot 1 doors-into
        else  floor-plate-pressed fact-off  gate-floor barred-floor floor-on  ['] gate-down start drop  then
    then ;

\ ---- the breaker's hum: begun once the room's sounds are in (not once lowered), then kept going;
\ the gate's motor kept going too ----
: hums ( -- )
    hum-started @ 0= if
        room-sounds-in? if  lowered? 0= if  hum-sound hum-at room-sound  then  -1 hum-started !  then  exit
    then
    hum-sound hum-at keep-sound  gate-sound gate-at keep-sound ;

\ ---- in among the wires, the breaker not lowered: she is killed (a game over) ----
: electrocuted ( -- )
    capture-no-end state!  no-pause state!  fiona hold  stalker let-go  16 frames  yield
    15 screen:to-black-2 with-music or fade
    death-movie scenes:keyed-movie death-cutscene load-scene  wait-fade  $12 $1E movie-key
    hewie let-go  panic-eased  if  -1 run-through  then
    hum-sound room-bank stop-sound  1e sounds-scaled
    death-seen resident-flag-set  gameover:movie-death game-over ;
: wires ( -- )
    lowered?  wires-touched @ or if  exit  then
    fiona wires-area entered? 0= if  exit  then
    -1 wires-touched !  panic-eased  ['] electrocuted start drop ;

\ ---- brought back in by the forced way: the gate rattles shut on her, and back she goes ----
: thrown-back ( -- )
    fiona hold  world-frozen state!  world-held state-off  15 screen:from-black fade  wait-fade
    begin  room-sounds-in? 0= while  yield  repeat
    castle-1f-4 preload  gate-sound gate-at room-sound  gate-rattles  thud-sound gate-at room-sound
    30 frames  15 screen:to-black fade  wait-fade  world-held state!  shot-off
    hum-sound room-bank stop-sound  forced-exit take-exit ;

\ ---- what she may do or say here ----
: lowers-it ( -- )   \ (the handle down: the hum stops, the door unlocked)
    lever-object 1 lower-anim object  fiona room-motion lower-anim + 10 start-blend
    50 frames  breaker-sound -44e 18e 36e room-sound  hum-sound room-bank stop-sound
    spark-part hide-part  breaker-lowered fact!  40 trust+
    castle-1f-7 stalker-slot 0 doors-into  breaker-floor keeps-hewie-off floor-off  breaker-door unlock
    fiona wait-anim  breaker-file give-file ;
: breaker ( -- )
    lowered? if  fiona wait-move  lowered-msg say  exit  then
    fiona reach-motions motions-for
    fiona $BF -90 -1 walk-move -39.071e 35.745e go-to  1 20e -10e 0e 0e shot
    breaker-msg ask  fiona use-motions  0= if  lowers-it  then
    16 frames  shot-off ;
: gate-shut-msg ( -- )  fiona wait-move  gate-msg say ;
: doll ( -- )   \ (her remarks in turn)
    fiona wait-move
    doll-looks-var var@ case
        0 of  doll-msg say  1  endof   1 of  doll-2-msg say  2  endof   >r doll-3-msg say  0 r>
    endcase  doll-looks-var var! ;
: rattles-locked ( -- )
    fiona locked-exit to-door
    in-panic?  chased? or if  fiona rattle-front-slow-anim play  fiona -1 idle  7 frames  exit  then
    fiona rattle-front-anim play  fiona -1 idle  7 frames  locked-msg say ;
: spots ( -- )
    fiona breaker-area in-area?  fiona -90 45 heading? and if  ['] breaker unhurried offer  then
    lowered? 0=  floor-plate-pressed fact? 0= and  fiona gate-area in-area? and  fiona -90 45 heading? and if
        ['] gate-shut-msg unhurried offer  then
    fiona $A in-area?  fiona -90 45 heading? and  fiona $B in-area?  fiona 180 45 heading? and or
    fiona $C in-area?  fiona $D in-area? or  fiona 90 45 heading? and or
    fiona $E in-area?  fiona 0 45 heading? and or if  ['] doll unhurried offer  then
    locked-exit before-locked? if  ['] rattles-locked any-time offer  then ;

\ ---- Hewie, the breaker not lowered: over by it, tense, barking at it; then a look at her ----
: barks-at-breaker ( -- )
    hewie hold  hewie $F5 -40 -1 walk-path-move -9.214e -23.873e go-to
    lowered? if  exit  then
    hewie stance-anim start-anim  60 frames  c1f-7-flag1 fact!
    6 0 do  lowered? if  unloop exit  then  hewie bark-tense-anim play  loop
    lowered? if  exit  then
    hewie fiona look-at  yield  hewie stance-anim start-anim  40 frames  hewie -1 look-at  yield ;

\ ---- the walls' shade by the door ----
: walls-shade ( -- )
    33e 23e -5.9e  45e 23e -5.9e  33e 11e -5.9e  45e 11e -5.9e  light-doorway
    33e 11e -5.9e  45e 11e -5.9e  33e 0e -5.9e  45e 0e -5.9e  light-doorway
    45e 23e -5.1e  33e 23e -5.1e  45e 0e -5.1e  33e 0e -5.1e  light-doorway ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  gate-shut  lever-object object-show  handle-part hide-part  spark-part show-part
    death-movie scenes:keyed-movie death-cutscene load-scene  $12 $1E movie-key  panic-eased
    if  -1 run-scene  then  replay-over ;

\ ---- every frame ----
create cameras  4 , 0 , 0 ,   5 , 1 , -1 ,   6 , 1 , -1 ,   7 , 0 , 0 ,   \ (area, set, path)
create entries  0 , 0 ,   0 , 0 ,             \ (by exit 0, 1: the camera)
: ways ( -- )
    hums  0 exit-out  player-exit-area gate-exit leaves-by  cameras 4 cameras-at
    fiona leaving-0-area left? if  0 prepare-exit  then
    fiona near-exit-1-area entered? if  gate-exit prepare-exit  then ;
: each-frame ( -- )  ways  gate-watch  wires  spots  walls-shade ;
: person-in ( i -- )
    dup fiona-p = if  drop
        events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
        entries 2 ways-in
        came-by @ forced-exit = if
            fiona thrown-tri -60 place-on-tri  1 30e 15e 0e 0e shot  ['] thrown-back start drop
        then  exit
    then
    hewie-p =  lowered? 0= and  c1f-7-flag1 fact? 0= and if  ['] barks-at-breaker start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-7 place!
