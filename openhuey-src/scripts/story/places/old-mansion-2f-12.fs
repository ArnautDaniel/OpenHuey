\ story/places/old-mansion-2f-12.fs - the synthesizer's hall of the old mansion's 2F, lit by its
\ lamps: the synthesizer (its plaque, its file; the Silver Ore put in, the White Godstone out -
\ anything else, nothing); the three-dial lock on the water pipe (the stick picks and turns them -
\ 1, 0, 2: the doors unlocked, the water drained; the file once the reservoir's been seen); the
\ locked door, water spilling under it; the books on the table; the save clock; two vases; the
\ Antimony Powder Hewie sniffs out; his looks; its ambient sounds; the doorway's shade. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-2f-12.fs
\ (converted from the game's, not loaded).
IN: story.places.old-mansion-2f-12
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names keys story.kit fiona.anims ;

own-reset

1 constant pipe-msg         \ "There's a dial connected to what looks like a water pipe... Operate the dial? YES NO"
2 constant stick-msg        \ "Push the left analog stick, up or down to change the dial setting. right or lef..."
3 constant plaque-msg       \ "Synthesizer By separating the platinum half-piece and then synthesizing it with the..."
4 constant synthesizer-msg  \ "A synthesizer? What a peculiar device. It appears you can insert an item here..."
5 constant nothing-msg      \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
6 constant changed-msg      \ "Its composition has changed!"
7 constant drain-msg        \ "There's a device for draining water here."
8 constant books-msg        \ "There are books spread out on the table. They seem to pertain to complex formulas, but..."
9 constant locked-msg       \ "It's locked. I can't open it... Now how am I going to get this open?"
$A constant water-msg       \ "Water is spilling out from the edge of the door frame. The room on the other side..."
own: plaque-read  own: locked-remarked  own: picked  own: turning  own: put-in
0 constant powder-fx   0 constant pick-sound   1 constant turn-sound   2 constant right-sound
3 constant unlocked-sound   4 constant change-sound   5 constant first-ambient
$23B constant pipe-file   $244 constant synthesizer-file   $74 constant far-door   $62 constant waterway-door
0 constant changes-it   4 constant nothing-from-it   \ (the items' events here: items.fs)
om2f-12-breakable1-broken 0 1  -114e 30e -48e 5e 8e  breakable: one-vase
one-vase $80303030 0 $C shard-look  one-vase 0.812e 0.687e 0.187e 0.312e shard-uv  one-vase 0 frees-floor  one-vase $DD noise-from
om2f-12-breakable2-broken 2 3  -114e 30e -42e 5e 8e  breakable: other-vase
other-vase $80303030 0 $C shard-look  other-vase 0.812e 0.687e 0.187e 0.312e shard-uv  other-vase 1 frees-floor  other-vase $DF noise-from

\ ---- the three-dial lock: dials 0..2 (their lit twins 3..5) on progress variables $18..$1A, a
\ quarter turn a notch; 1, 0, 2 opens it ----
: dial-var ( k -- var )  $18 + ;
: notch-angle ( k -- ) ( F: -- rad )  dial-var var@ 90 * s>f deg>rad angle-wrap ;
: dial ( k -- addr len )  this-room @ story.strings:room-string ;
: dials-set ( -- )  6 0 do  i 3 mod notch-angle  i dial turn-y object!  loop ;
: lit ( k on -- )  {: k on :}  k on if  object-hide  k 3 + object-show  else  object-show  k 3 + object-hide  then ;
: picks ( k -- )  picked @ false lit  dup true lit  picked !  pick-sound room-bank 293e 12e -102e sound-there ;
: turns-to ( v -- )  picked @ dial-var var!  -1 turning !  turn-sound room-bank 293e 12e -102e sound-there ;
: stick-asked ( -- done? )   \ (the stick: up / down pick a dial, left / right turn it; triangle leaves)
    triangle button-pressed? if  true exit  then
    0 arrow-pressed? if  picked @ dup 0= if  drop 2  else  1-  then  picks  false exit  then
    2 arrow-pressed? if  picked @ dup 2 < if  1+  else  drop 0  then  picks  false exit  then
    picked @ dial-var var@
    3 arrow-repeat? if  dup 0= if  drop 3  else  1-  then  turns-to  false exit  then
    1 arrow-repeat? if  dup 3 < if  1+  else  drop 0  then  turns-to  false exit  then
    drop false ;
: right? ( -- flag )  0 dial-var var@ 1 =  1 dial-var var@ 0= and  2 dial-var var@ 2 = and ;
: turned? ( -- there? )   \ (the picked dial and its twin, 4 degrees a frame to its setting)
    picked @ {: k :}
    k dial turn-y object@  k notch-angle f-  angle-wrap  rad>deg
    fdup 4e f> if  fdrop  k dial turn-y -4e deg>rad object+!  k 3 + dial turn-y -4e deg>rad object+!  false exit  then
    -4e f< if  k dial turn-y 4e deg>rad object+!  k 3 + dial turn-y 4e deg>rad object+!  false exit  then
    k notch-angle  k dial turn-y object!  k notch-angle  k 3 + dial turn-y object!  true ;
: at-the-dials ( -- right? )   \ (till she leaves, or turns them right)
    0 turning !  0 picked !  0 true lit
    begin
        count-arrows
        turning @ if  turned? if  0 turning !  right? if  true exit  then  then
        else  stick-asked if  false exit  then  then
        yield
    again ;
: drains ( -- )   \ (the dials right: the doors unlocked, the water drained)
    right-sound room-bank 318e 12e -80e sound-there  1 $FF $D2 rumble-pad
    fiona rest-anim play  fiona stand-anim start-anim  50 frames
    fiona look-point-move 0 0  318e 10e -80e 0e  move  yield
    unlock-sound common-bank 318e 12e -80e sound-there  far-door unlock  waterway-door unlock  om2f-12-flag1 fact!
    30 frames  unlocked-sound room-bank fiona sound-by  90 frames  fiona -1 look-at  yield ;
: the-pipe ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    om2f-12-flag1 fact? if  drain-msg say  stalkers-stay state-off exit  then
    pipe-msg yes? if
        fiona $4B 180 293.136e -95e place-at  world-frozen state!  1 5e 0e 0e 2e shot  stick-msg show
        at-the-dials  text-gone  world-frozen state-off  shot-off
        0 object-show  1 object-show  2 object-show  3 object-hide  4 object-hide  5 object-hide
        if  drains  stalkers-stay state-off exit  then
    then
    om1f-14-flag1 fact? if  pipe-file give-file  then
    stalkers-stay state-off ;

\ ---- the synthesizer: its plaque; the Silver Ore put in, the White Godstone out; else nothing ----
: at-synthesizer ( -- )  stalkers-stay state!  fiona hold  fiona wait-move  fiona $7F -90 -103e -75e walk-to ;
: the-synthesizer ( -- )
    at-synthesizer
    plaque-read @ if
        fiona hand-to-mouth-anim 10 start-blend  synthesizer-msg say  fiona wait-anim  0 plaque-read !
    else
        15 screen:to-dim fade wait-fade  plaque-msg say  -1 plaque-read !  15 screen:from-dim fade wait-fade
    then
    synthesizer-file give-file  stalkers-stay state-off ;
: working ( -- )  change-sound room-bank fiona sound-by  120 frames  fiona look-close-anim play ;
: changes ( -- )
    at-synthesizer  working  changed-msg say
    fiona reach-out-anim play  silver-ore spend  white-godstone item-named  white-godstone 1 give-item
    white-godstone show-item-tab  fanfare  obtained-2-msg say  fiona reach-back-anim play  stalkers-stay state-off ;
: nothing-comes ( -- )
    at-synthesizer  working  fiona reach-out-anim play
    put-in @ dup item-named  show-item-tab  fanfare  obtained-2-msg say  fiona reach-back-anim play
    nothing-msg say  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    {: who ev var it :}  who 0<> if  false exit  then
    ev changes-it = if  ['] changes start drop  true exit  then
    ev nothing-from-it = if  it put-in !  ['] nothing-comes start drop  true exit  then
    false ;

\ ---- what she may do or say here ----
: the-door ( -- )   \ (locked - or, said once, the water spilling under it)
    locked-remarked @ 0=  chased? or  in-panic? or if  0 true locked-msg tries-locked  -1 locked-remarked !  exit  then
    fiona 0 to-door  fiona crouch-down-anim play  fiona crouching-anim start-anim  water-msg say
    fiona crouch-up-anim play  fiona -1 idle  7 frames  0 locked-remarked ! ;
: the-books ( -- )  fiona wait-move  fiona -69.5e -46.5e turn-to-xz  fiona look-low-anim start-anim  40 frames  books-msg say  fiona wait-anim ;
: take-powder ( -- )  antimony-powder om2f-12-antimony-powder powder-fx 82.73e -85.2e picks-up ;
: powder-sniffed ( -- )  om2f-12-antimony-powder-found om2f-12-antimony-powder powder-fx  82.73e 31e -85.2e  hewie-finds ;

\ ---- as she comes in ----
create flames  -31e f, 51.9e f, -132.6e f,   0e f, 51.9e f, -142.7e f,   30.9e f, 51.8e f, -132.6e f,
    50e f, 51.9e f, -106.3e f,   -50e f, 51.9e f, -106.3e f,   82e f, 51.9e f, -62.7e f,
    160e f, 51.9e f, -62.7e f,   215.7e f, 21.9e f, -62.6e f,
: lamps-lit ( -- )  8 0 do  i 1+ 1  flames i 3 * floats +  dup f@  dup float+ f@  2 floats + f@  flame-at  loop ;
: came-in ( exit -- )
    drop  room-sounds  0 ambient-step !  0 ambient-frames !
    dials-set  3 object-hide  4 object-hide  5 object-hide
    om2f-12-antimony-powder-found fact?  om2f-12-antimony-powder fact? 0= and if  powder-fx 82.73e 31e -85.2e flicker-at  then
    lamps-lit  one-vase as-it-stands  other-vase as-it-stands
    1 $3FFF reverb ;

create cameras  4 , 2 , 1 ,   5 , 3 , -1 ,   6 , 0 , 0 ,   7 , 2 , 1 ,   8 , 0 , 0 ,   9 , 1 , -1 ,   \ (area, set, path)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit loaded ahead)
create looks  -103.36e 30e -72.17e  25e 18e 18e look,   288.99e 0e -98.66e  25e 16e 16e look,
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 6 cameras-at  ahead 2 ways-out
    om2f-12-antimony-powder-found fact? 0=  scrambling? and  hewie 82.73e 30e -85.2e 10e 5e touches? and if  ['] powder-sniffed start-only  then
    looks 2 hewie-looks-at
    one-vase breaks? drop  other-vase breaks? drop
    room-sounds-in? if  first-ambient 74e 30e -110e ambient-sounds  then
    fiona $A 50 faces-area? if  ['] the-pipe unhurried offer  then
    fiona $B in-area?  fiona -45 50 heading? and if  ['] the-synthesizer unhurried offer  then
    fiona $C in-area?  fiona 22 60 heading? and if  ['] the-books unhurried offer  then
    1 locked-request?  fiona 0 before-door? and if  ['] the-door any-time offer  then
    fiona $D in-area?  fiona 50 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    om2f-12-antimony-powder-found fact?  om2f-12-antimony-powder fact? 0= and  fiona 82.73e 30e -85.2e 5e 5e touches? and if
        ['] take-powder calm-only offer
    then
    251.4e 20e -115e  251.4e 20e -90e  251.4e 0e -115e  251.4e 0e -90e  light-doorway ;
: person-in ( i -- )  fiona-p = if  0 3 -1 way-in  1 1 -1 way-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-12 place!
' items-used old-mansion-2f-12 items-here
