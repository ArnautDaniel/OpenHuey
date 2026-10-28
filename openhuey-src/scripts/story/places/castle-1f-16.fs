\ story/places/castle-1f-16.fs - the TV room in Belli Castle's 1F ($1A): the fan turning, the TV
\ (her switching it on and off - the first time a fright - its noise heard about), a place to hide
\ (the kit's; the stalker coming in: Debilitas turns the TV off, else he searches - finding her more
\ likely each time he comes), the cracked mirror, the red painting, the Magnesia, the clock. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-1f-16.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-1f-16
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects keys story.kit fiona.anims fiona.sounds stalker.anims ;

own-reset

\ ---- what is said here ----
0 constant mirror-msg      \ "It would be nice to freshen up a bit... But there's a crack in this mirror and I can't..."
1 constant painting-msg    \ "A landscape painting composed entirely of the color red. It seems quite out of plac..."
2 constant painting-2-msg  \ "The color bears a remarkable resemblance to blood. But... surely this is just red p..."

\ ---- its own state (a visit) ----
own: sounds-started        \ the room's sounds begun (then kept up)
own: painting-remarked     \ the painting's first remark made (in turn with the second)

\ ---- the room's things ----
0 constant tv-fx   1 constant magnesia-fx   0 constant tv-off-part
8 constant clock-area   9 constant tv-area   $A constant hiding-area   $B constant mirror-area   $C constant painting-area
0 constant switch-on-sound   1 constant running-sound   2 constant switch-off-sound   3 constant first-on-sound
5 constant first-ambient   8 constant search-var   9 constant tv-offs-var
0 constant her-motions   1 constant his-motions   $602 constant tv-switch-anim
: tv-on? ( -- flag )  c1f-16-flag2 fact? ;
: running-at ( F: -- x y z )  23e 11e 9e ;

\ ---- the fan, round 1.15 degrees a frame ----
: fan ( -- )  begin  s" fan" 0.0200713e object-turn  yield  again ;

\ ---- the TV: its dark screen away while it's on, its noise heard about ----
: show-tv ( -- )  tv-on? if  tv-off-part hide-part  1 tv-fx tell-effect  else  tv-off-part show-part  0 tv-fx tell-effect  then ;
: heard-about ( v -- )  s" acoustics" actor-named send noise-setting ;   \ (the room's noise level: the TV on, louder)

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  ['] fan start drop
    0e 0e 0e 1 effects:tv-screen tv-fx effect!  show-tv  tv-on? if  1 heard-about  then
    c1f-16-magnesia fact? 0= if  magnesia-fx -25e 10e 13e flicker-at  then ;

\ ---- the room's sounds: four about it in turn, every 30 frames; the TV's while it runs ----
: sounds ( -- )
    sounds-started @ 0= if
        room-sounds-in? if  tv-on? if  running-sound running-at room-sound  then  -1 sounds-started !  then  exit
    then
    first-ambient 27e 15e -6e ambient-sounds
    c1f-16-flag1 fact? if  running-sound running-at keep-sound  then ;

\ ---- the TV switched: on (its noise heard; the first time ever, a fright) or off - not once
\ the stalker has turned it off twice ----
: first-fright ( -- )
    $5A fright+  1 $FF 8 rumble-pad  seized-sound common-bank fiona sound-by
    fiona uneasy-2-anim play  fiona -1 idle  7 frames
    fiona reach-out-anim play  running-sound running-at room-sound  fiona reach-back-anim play  c1f-16-flag1 fact! ;
: tv ( -- )
    fiona $E1 120 -1 walk-move 18.5e 13.6e go-to  1 20e 10e 0e 0e shot
    fiona reach-out-anim play
    tv-offs-var var@ 2 < if
        tv-on? if
            switch-off-sound room-bank fiona sound-by  running-sound room-bank stop-sound
            c1f-16-flag2 fact-off  0 heard-about
        else
            c1f-16-flag2 fact!  switch-on-sound room-bank fiona sound-by
            c1f-16-flag1 fact? if  running-sound running-at room-sound  else  first-on-sound $86 0e 0e 0e sound-there  then
            $40 7 room-noise  1 heard-about
        then
        c1f-16-flag1 fact? 0= if  first-fright  else  fiona reach-back-anim play  then
    else  fiona reach-back-anim play  then
    shot-off ;

\ ---- the hiding place (the kit's): in, lying low, calming; out by the button - or found, and
\ hauled out ----
: hides ( -- )  her-motions $CC 90 3 -4.7e -39.3e hides-in ;

\ ---- the stalker coming in while she hides ----
: finds-her ( -- )  stalker hold  stalker wait-move  0 door-for-him  $CE 90 -12e -37e finds-her-at ;
: switches-tv-off ( -- )   \ (Debilitas turns the TV off - after twice she can't turn it on)
    stalker hold  stalker wait-move  0 door-for-him  stalker 7 120 14e 18e walk-to
    stalker tv-switch-anim start-anim  18 frames
    switch-off-sound room-bank stalker sound-by  running-sound room-bank stop-sound
    c1f-16-flag2 fact-off  0 heard-about  tv-offs-var var@ 1+ tv-offs-var var!
    3 stalker-order  stalker stand-anim play  16 frames ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    tv-on?  2 creature-moving? 0= and  debilitas? and if  120 summoner-sooner  ['] switches-tv-off start drop
    else  search-var his-motions ['] finds-her searches  then ;

\ ---- what she may do or say here ----
: mirror ( -- )  fiona wait-move  fiona -145 turn  mirror-msg say ;
: painting ( -- )   \ (her remarks in turn)
    fiona wait-move  fiona 0 turn  fiona turn-round-anim play  fiona -1 idle  7 frames
    fiona look-at-anim start-anim  16 frames
    painting-remarked @ if  0 painting-remarked !  painting-2-msg  else  -1 painting-remarked !  painting-msg  then  say
    fiona wait-anim ;
: reach-magnesia ( -- )
    fiona wait-move  fiona -25e 13e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  magnesia c1f-16-magnesia magnesia-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    fiona clock-area 50 faces-area? if  ['] save-clock calm-hewie-up offer  then
    fiona tv-area in-area?  fiona 60 90 heading? and if  ['] tv unhurried offer  then
    fiona hiding-area in-area?  fiona 45 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    fiona mirror-area in-area?  fiona -72 50 heading? and if  ['] mirror unhurried offer  then
    fiona painting-area in-area?  fiona 0 50 heading? and if  ['] painting unhurried offer  then
    c1f-16-magnesia fact? 0=  fiona -25e 10e 13e 5e 5e touches? and if  ['] reach-magnesia calm-only offer  then ;

\ ---- the depth of field: on the room's far views (camera sets 1 and 2) ----
: depth-by-camera ( -- )
    cam-changed? 0= if  exit  then
    cam-set@ dup 1 = swap 2 = or if  20e 30e 2000e 2000e depth-range  else  depth-range-off  then ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    1 , 0 , -1 ,   2 , 1 , -1 ,   3 , 1 , -1 ,   4 , 2 , -1 ,   5 , 2 , -1 ,   6 , 2 , -1 ,   7 , 2 , -1 ,
: each-frame ( -- )  sounds  0 exit-out  cameras 7 cameras-at  show-tv  spots  depth-by-camera ;
: person-in ( i -- )
    case  fiona-p of  0 1 -1 way-in  endof  stalker-p of  stalker-came  endof  endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-16 place!
