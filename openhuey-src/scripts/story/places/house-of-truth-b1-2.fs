\ story/places/house-of-truth-b1-2.fs - the House of Truth's B1 room she wakes in ($85): brought here
\ the first time, Lorenzo's words (the scene, the file, the doors shut on her); its door locked from
\ the far side (tried); the other door - the first time through it, to Lorenzo (B1-1); the clock (the
\ save); the Lavender; its sounds in turn; his slam's shake; the replay. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/house-of-truth-b1-2.fs (converted from the
\ game's, not loaded).
IN: story.places.house-of-truth-b1-2
USING: engine game-state actors messages common facts room-names flag-names story-names items.names
    story.kit story.places.house-of-truth fiona.anims scenes ;

own-reset  truth-owns owns !

0 constant belli-msg       \ "The great Aureolus Belli once said, 'The most valuable thing in this world is t..." (the scene's)
1 constant locked-msg      \ "It's locked. I can't open it. I guess they have no intention of letting me go home..."
0 constant lavender-fx   3 constant first-ambient   8 constant clock-area
1 constant belli-movie   0 constant belli-cutscene   $EF constant b1-1-door   $E9 constant hall-door
$252 constant belli-file   $4A constant belli-seen
create entries  0 , 0 ,  2 , 2 ,   \ (by exit 0, 1: the camera)
create cameras  4 , 0 , 0 ,  5 , 1 , 1 ,  6 , 2 , 2 ,  7 , 1 , 1 ,   \ (area, set, path)
create ahead  2 , 0 ,  3 , 1 ,   \ (area, exit loaded ahead)
: came-in ( exit -- )
    drop  room-sounds
    htb1-2-lavender fact? 0= if  lavender-fx -45.77e 5.75e 19.99e flicker-at  then ;

\ ---- brought here the first time: Lorenzo's words ----
: belli-scene ( -- )
    belli-movie scenes:unseen-movie belli-cutscene load-scene drop  panic-eased  belli-msg run-scene ;
: woken ( -- )
    stalkers-stay state!  fiona hold  belli-scene
    fiona $27 -79 -0.18e 14.47e place-at  1 1 player-camera  camera-restart
    b1-1-door false open-mark  hall-door lock  b1-1-door lock  doors-redone  found-file-252 fact!
    15 fade-in  belli-seen resident-flag-set  belli-file give-file
    scene-locked state-off  stalkers-stay state-off ;
: replayed ( -- )  fiona hold  belli-scene  replay-over ;

\ ---- the doors ----
: to-lorenzo ( -- )   \ (the first time through: to B1-1, by the forced way)
    stalkers-stay state!  fiona hold  fiona 1 to-door
    15 screen:to-black-2 with-music or fade  wait-fade  scene-locked state!
    1 prepare-exit  stalkers-stay state-off  forced-exit take-exit ;
: tries-it ( -- )  0 true locked-msg tries-locked ;

\ ---- what she may do here ----
: pick-lavender ( -- )
    fiona wait-move  fiona -45.77e 19.99e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  lavender htb1-2-lavender lavender-fx takes-up  fiona reach-back-anim play ;
: spots ( -- )
    fiona clock-area 50 faces-area? if  ['] save-clock calm-hewie-up offer  then
    over-music-e fact? 0=  fiona 1 before-door? and if  ['] to-lorenzo any-time offer  then
    8 locked-request?  fiona 0 before-door? and if  ['] tries-it any-time offer  then
    htb1-2-lavender fact? 0=  fiona -45.77e 5.75e 19.99e 5e 5e touches? and if  ['] pick-lavender calm-only offer  then ;
: each-frame ( -- )
    0 exit-out  over-music-e fact? if  1 exit-out  then
    cameras 4 cameras-at  ahead 2 ways-out  slam-shake
    room-sounds-in? if  first-ambient -41e 20e 25e ambient-sounds  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 2 ways-in
    found-file-252 fact? 0=  came-by @ forced-exit = and if
        world-held state!  panic-eased  ['] woken fiona start-holding drop
    then ;
: went ( -- )  scene-locked state-off ;

' came-in ' person-in ' each-frame ' went house-of-truth-b1-2 place!
