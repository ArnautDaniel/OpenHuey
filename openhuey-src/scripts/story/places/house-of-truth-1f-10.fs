\ story/places/house-of-truth-1f-10.fs - the House of Truth's 1F storeroom ($9B): the hiding place
\ under the hung things (in at her examine - not with the stalker here: "no time" - out at circle,
\ hauled out panicking or found); the stalker's search as he comes in on her hidden (by the odds);
\ the hung things swinging as she pushes by; the hole in the wall (in, by the forced way to
\ off-map-27 - and out of it here); the clock (the save); the Camomile; Hewie's bark at the hole,
\ his look; its steady sound and four in turn; the walls' shade; the map's pages. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-10.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.house-of-truth-1f-10
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit story.places.house-of-truth fiona.anims fiona.sounds stalker.anims keys ;

own-reset  truth-owns owns !

0 constant hole-msg        \ "A pungent odor and chapped, dry air coming from inside the hole wreak havoc on the sens..." (YES / NO)
own: rung                  \ the first hung thing last rang "on" (they ring on and off in turn)
own: steady-on             \ its steady sound started
own: hole-barked           \ Hewie has barked at the hole (once a visit)
0 constant camomile-fx   $A constant hiding-area   3 constant clock-area   6 constant hole-area
8 constant steady-sound   $A constant first-ambient   4 constant ring-on   5 constant ring-off
$24 constant searches-var   $803 constant crouch-step   $207 constant hauled-step
4 constant hauled-anim

\ ---- the hiding place ----
: out-of-hiding ( -- )   \ (found, or panicking: out, her eyes off him)
    found-her @ if  2 avoid-prompt  then
    fiona-hidden state-off  fiona follow  stalkers-stay state!
    fiona $11F 180 hauled-step walk-move 32.119e -37.94e go-to  fiona hauled-anim $14 play-blend
    fiona false noclip  stalkers-stay state-off ;
: hides ( -- )
    stalkers-stay state!  0 came-while-hidden !  0 found-her !
    fiona wait-move  fiona $11A 0 -1 walk-move 31.33e -24.45e go-to
    fiona true noclip  fiona crouch-down-anim play
    fiona $28 0 crouch-step walk-move 32.119e -7.5e go-to  fiona $28 180 crouch-step walk-move 32.119e -7.5e go-to
    fiona crouching-anim start-anim  fiona-hidden state!  stalkers-stay state-off  0 avoid-prompt
    begin
        circle button-pressed? if
            fiona-hidden state-off  fiona follow  stalker let-go
            chased? if  out-of-hiding exit  then
            stalkers-stay state!  fiona $11A 180 crouch-step walk-move 31.33e -24.45e go-to
            fiona crouch-up-anim play  fiona false noclip  stalkers-stay state-off exit
        then
        in-panic? found-her @ or if
            in-panic? 0= if  seized-sound common-bank fiona sound-by  then  out-of-hiding exit
        then
        hidden-calm  yield
    again ;
: found-there ( -- )  stalker hold  $11F -5 40.89e -25.76e finds-her-at ;
: searched ( -- )   \ (he comes in on her hidden: the camera on him; found by the odds - his searches counted)
    stalker follow
    searches-var mansion-odds chance?  2 creature-moving? or if  ['] found-there start drop  else  120 summoner-sooner  then
    searches-var var@ 1+ searches-var var! ;

\ ---- what she may do here ----
: pick-camomile ( -- )
    fiona wait-move  fiona 29.7e 32.54e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  camomile ht1f-10-camomile camomile-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    fiona hiding-area in-area?  fiona 0 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    fiona clock-area in-area?  fiona 180 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    fiona hole-area in-area?  fiona 0 50 heading? and if  hole-msg 60e 55e hole-offer  then
    ht1f-10-camomile fact? 0=  fiona 29.7e 5.25e 32.54e 5e 5e touches? and if  ['] pick-camomile calm-only offer  then ;

\ ---- Hewie: his bark at the hole (once a visit, now and then), his look up there ----
: hole-bark ( -- )  60.49e 0e 45.99e hewie-barks-at ;
create hole-look  60.49e 0e 45.99e  20e 18e 18e look,
: hewie-here ( -- )
    hole-barked 30 60.49e 0e 45.99e 20e 18e notices? if  ['] hole-bark start drop  then
    hole-look 1 hewie-looks-at ;

\ ---- every frame ----
create cameras  1 , 0 , 0 ,  2 , 1 , 1 ,  4 , 0 , 0 ,  5 , 2 , 2 ,   \ (area, set, path)
: shade ( -- )   \ (the walls' shade, by the camera's setup)
    cam-set@ 2 <> if  38e 78e -60.2e  70e 55e -60.2e  38e 60e -60.2e  70e 40e -60.2e  light-doorway  then
    cam-set@ dup 2 <> swap 0<> and if  90e 90e -40.5e  60e 90e -40.5e  90e 52e -40.5e  60e 52e -40.5e  light-doorway  then
    91e 51.8e -63e  91e 51.8e -40e  70e 51.8e -63e  70e 51.8e -40e  light-doorway ;
: each-frame ( -- )
    steady-on steady-sound 60e 20e 50e steady
    room-sounds-in? if  first-ambient 58e 21e -76e ambient-sounds  then
    0 0 leaves-by  cameras 4 cameras-at
    fiona 7 entered? if  0 prepare-exit  then
    fiona 8 entered? if  off-map-27 preload  then
    fiona 9 entered? if  1 map-page  then   fiona 9 left? if  2 map-page  then
    fiona 30.47e -9.97e 11e centre-across? if  0 3 true rung ring-on ring-off 1e chimes  then
    hewie-here  shade  spots ;
: came-in ( exit -- )
    drop  room-sounds  0 3 chimes-rest
    ht1f-10-camomile fact? 0= if  camomile-fx 29.7e 5.25e 32.54e flicker-at  then ;
: person-in ( i -- )
    dup stalker-p = if  drop  fiona-hidden state? dup came-while-hidden !  if  searched  then  exit  then
    fiona-p <> if  exit  then
    0 2 2 way-in
    came-by @ hole-way = if
        fiona $7E 180 60e 45e place-at  0 0 player-camera  ['] out-of-hole start drop
    then
    came-by @ 0= if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-10 place!
