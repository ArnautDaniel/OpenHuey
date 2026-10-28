\ story/places/old-mansion-1f-4.fs - the incubator room of the old mansion's 1F: the transmuter (its
\ plaque, its file; the White Godstone put in, the Red Godstone out - anything else, nothing; either
\ way, the first time, an incubator bursts - two homunculi loose); the incubators (something inside;
\ the burst ones empty); a hopping figure (the kit's hopper, its calls); the Chicken Jerky; Hewie
\ wary of the figure, his looks; the holes things fall through. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/old-mansion-1f-4.fs (converted from the game's,
\ not loaded). Its act 3 (two lines read in the dim) nothing starts.
IN: story.places.old-mansion-1f-4
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names story.kit fiona.anims fiona.sounds ;

own-reset

0 constant plaque-msg       \ "Transmuter Synthesizing two objects together is a task any fool could surely comple..."
1 constant transmuter-msg   \ "A transmuter? I've never heard of such a device before... It appears you can insert..."
2 constant nothing-msg      \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
5 constant transformed-msg  \ "Oh my! It actually transformed..."
6 constant incubator-msg    \ "Is this some kind of incubator? There is something inside..."
7 constant meat-msg         \ "It just looks like a clump of meat. It's all fatty and gross."
8 constant empty-msg        \ "It's empty now. I can't believe that thing inside was alive..."
own: plaque-read  own: incubator-remarked  own: figure-noticed  own: put-in
0 constant jerky-fx   0 constant lash-sound   1 constant hop-sound   2 constant call-sound
3 constant change-sound   4 constant seize-sound   $12 constant lab-track   $245 constant transmuter-file
0 constant changes-it   1 constant nothing-from-it   \ (the items' events here: items.fs)
$9000 $9001 lash-sound hop-sound  55e 0e -20e 90e  hopper: the-figure
create holes  $77 , $48 , $75 , $46 , $52 , $81 , 2 , $61 , $70 , $3E , $39 , 0 , $71 , $3F , $4B , $7A , $4A , $79 ,
    $45 , $74 , $5F , $85 , $51 , $80 , $4C , $7B , $3C , $6F , $4E , $7D , $86 , $60 , $54 , $83 , 6 , $63 ,
    $14 , $6D , $4D , $7C , $18 , $23 , $41 , $72 , $76 , $47 , $78 , $49 , $53 , $82 , $5A , $84 , $3B ,
53 constant #holes

\ ---- as she comes in ----
: hopping-about ( -- )  the-figure true hops-about ;
: came-in ( exit -- )
    drop  room-sounds  lab-track 1e background
    om1f-4-flag1 fact? if  1 show-part  else  0 show-part  then
    ['] hopping-about start drop  hunted state!
    om1f-4-chicken-jerky fact? 0= if  jerky-fx -51.07e 1e 19.59e flicker-at  then
    1 $2300 reverb
    #holes 0 do  holes i cells + @ hole-floor floor-tri-on  loop ;

\ ---- an incubator bursting (the first time the transmuter's used): two homunculi loose ----
: bursts ( -- )
    seize-sound room-bank -30e 10e 20e sound-there  16 frames
    seized-sound common-bank fiona sound-by  60 threat  1 $FF 8 rumble-pad  fiona uneasy-2-anim play
    0 hide-part  1 show-part
    old-mansion-1f-4 0 105 $80 5 $68 40e creature-dropped  old-mansion-1f-4 0 23 $80 10 $5A 0e creature-dropped
    om1f-4-flag1 fact! ;
: then-bursts ( -- )  om1f-4-flag1 fact? 0= if  bursts  then  stalkers-stay state-off ;

\ ---- the transmuter: its plaque; the White Godstone put in, the Red Godstone out; else nothing ----
: at-transmuter ( -- )  stalkers-stay state!  fiona hold  fiona wait-move  fiona $E 20 38e 18e walk-to ;
: the-transmuter ( -- )
    at-transmuter
    plaque-read @ if
        fiona hand-to-mouth-anim 10 start-blend  transmuter-msg say  fiona wait-anim  0 plaque-read !
    else
        15 screen:to-dim fade wait-fade  plaque-msg say  -1 plaque-read !  15 screen:from-dim fade wait-fade
    then
    transmuter-file give-file  stalkers-stay state-off ;
: working ( -- )  change-sound room-bank fiona sound-by  120 frames  fiona look-close-anim play ;
: changes ( -- )
    at-transmuter  working  transformed-msg say
    fiona reach-out-anim play  white-godstone spend  red-godstone item-named  red-godstone 1 give-item
    red-godstone show-item-tab  fanfare  obtained-2-msg say  got-red-godstone fact!  fiona reach-back-anim play  then-bursts ;
: nothing-comes ( -- )
    at-transmuter  working  fiona reach-out-anim play
    put-in @ dup item-named  show-item-tab  fanfare  obtained-2-msg say  fiona reach-back-anim play
    nothing-msg say  then-bursts ;
: items-used ( who ev var value -- handled? )
    {: who ev var it :}  who 0<> if  false exit  then
    ev changes-it = if  ['] changes start drop  true exit  then
    ev nothing-from-it = if  it put-in !  ['] nothing-comes start drop  true exit  then
    false ;

\ ---- the incubators ----
: inside ( -- )   \ (something inside - then the clump of meat)
    fiona look-close-anim start-anim  40 frames
    incubator-remarked @ if  meat-msg  else  -1 incubator-remarked !  incubator-msg  then  say  fiona wait-anim ;
: empty ( -- )  fiona look-close-anim start-anim  40 frames  empty-msg say  fiona wait-anim ;
: incubator-at ( F: x z -- )  fiona wait-move  fiona turn-to-xz  inside ;
: close-up ( tri deg cam F: x z -- )   \ (placed before it, its own camera; the player's after)
    {: tri deg cam F: x z :}  stalkers-stay state!  fiona hold  fiona wait-move  4 frames
    fiona tri deg x z place-at  fiona cam -1 camera-set  4 frames  inside  4 frames  stalkers-stay state-off ;
: by-the-door ( -- )  $62 173 3 6.25e -23.4e close-up  2 2 player-camera ;
: second ( -- )  -20e -40e incubator-at ;
: third ( -- )  -48e -40e incubator-at ;
: fourth ( -- )  -10e 0e incubator-at ;
: burst-one ( -- )
    fiona wait-move  fiona -40e 0e turn-to-xz  om1f-4-flag1 fact? if  empty  else  inside  then ;
: sixth ( -- )  8e 40e incubator-at ;
: far-one ( -- )
    om1f-4-flag1 fact? if  fiona wait-move  empty exit  then
    $50 -12 4 -17.5e 26.5e close-up  1 1 player-camera ;
: last-one ( -- )  -48e 40e incubator-at ;
: take-jerky ( -- )  chicken-jerky om1f-4-chicken-jerky jerky-fx -51.07e 19.59e picks-up ;
: wary-of-it ( -- )  54.4e 0e -20.46e hewie-stands-by ;

create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   3 , 0 , 0 ,   4 , 2 , 2 ,   \ (area, set, path)
create looks  45.1e 0e 31.88e  28e 18e 18e look,   54.4e 0e -20.46e  25e 16e 16e look,
: facing? ( area deg -- flag )  {: a d :}  fiona a in-area?  fiona d 60 heading? and ;
: incubators ( -- )
    7 90 facing? if  ['] by-the-door unhurried offer  then
    8 90 facing? if  ['] second unhurried offer  then
    9 90 facing? if  ['] third unhurried offer  then
    $A 0 facing?  $C 90 facing? or if  ['] fourth unhurried offer  then
    $B 0 facing?  $D 90 facing? or if  ['] burst-one unhurried offer  then
    $E 0 facing? if  ['] sixth unhurried offer  then
    $F 0 facing? if  ['] far-one unhurried offer  then
    $10 0 facing? if  ['] last-one unhurried offer  then ;
: each-frame ( -- )
    0 exit-out  cameras 4 cameras-at
    the-figure hopper-still dup 21 =  swap 66 = or if  call-sound hopper-sound  then
    figure-noticed 80 54.4e 0e -20.46e 25e 16e notices? if  ['] wary-of-it start drop  then
    looks 2 danger-now @ followed = hewie-looks-if
    fiona 5 in-area?  fiona 20 50 heading? and if  ['] the-transmuter unhurried offer  then
    om1f-4-chicken-jerky fact? 0=  fiona -51.07e 0e 19.59e 5e 5e touches? and if  ['] take-jerky calm-only offer  then
    incubators ;
: person-in ( i -- )  fiona-p = if  0 0 0 way-in  then ;
: went ( -- )  hopper-gone ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-4 place!
' items-used old-mansion-1f-4 items-here
