\ story/places/castle-1f-10.fs - the doll's room in Belli Castle's 1F ($29): a doll on show, the
\ fire's flame stirring as someone passes, the beef jerky; once the file's found (castle-b1-6)
\ Debilitas sits here with his doll (a figure, loaded by castle-1f-2 on her way) - beside him, the
\ scene (the Smudged Key), then his doll's dance; chased by Debilitas out of his sight, she hides -
\ his scene; later the Bisque Doll on show to take.
\ A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-1f-10.fs
\ (converted from the game's, not loaded).
IN: story.places.castle-1f-10
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects scenes stalker-names story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
0 constant doll-on-show-msg   \ "A doll lies on display here. The owner must cherish it."
1 constant not-lying-msg      \ "There is no way I would even consider lying down here."
2 constant out-of-sight-msg   \ "It's sort of like a... Let's just forget it. 'Out of sight, out of mind.' Or so they say."

\ ---- its own state (a visit) ----
own: hewie-sent-off        \ Hewie was here as the stalker's scene began: out the other way
own: doll-danced           \ his doll's dance played
own: doll-to-take          \ the Bisque Doll on show, to take

\ ---- the room's things ----
0 constant bisque-part   0 constant his-floor-group   3 constant doll-thing
5 constant shelf-area   6 constant bed-area   7 constant chest-area   8 constant bisque-area
0 constant flame-fx   1 constant jerky-fx
$9000 constant his-sitting   $9001 constant his-doll-dance   $9002 constant his-doll-start
3 constant chase-movie   0 constant chase-cutscene   4 constant doll-movie   1 constant doll-cutscene
$22 constant chase-seen   $29 constant doll-seen   $20 constant chase-door   3 constant his-figure
: he-sits? ( -- flag )  found-file-20b fact?  c1f-10-flag1 fact? 0= and ;   \ (Debilitas and his doll here)
: him ( -- id )  his-figure figure ;
: seated ( -- )   \ (on his spot, his doll in his lap)
    him 0< if  exit  then  him $83 90 12.913e 14.864e place-at  his-figure his-sitting 1 0 figure-play ;
: sits ( -- )  his-figure figure-on  seated ;
: specks-about ( -- )
    8 4 $80 $80 $80 $20 rgba 25e 12e 19e specks   8 8 $60 $60 $60 $40 rgba 9.4e 7.3e -57.3e specks
    8 8 $70 $70 $70 $40 rgba -12e 21e -27e specks ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop
    c1f-10-flag3 fact? if  2 object-hide  then
    flame-fx 0 24.35e 9e 19.7e flame-at
    c1f-10-beef-jerky fact? 0= if  jerky-fx 11e 12e -58.5e flicker-at  then
    he-sits? if  his-floor-group keeps-fiona-off keeps-hewie-off or floor-on  sits  then
    specks-about
    bisque-part hide-part
    got-bisque-doll fact?  bisque-doll 1 has-items? 0= and  doll-thing thing-lying? 0= and if
        bisque-part show-part  -1 doll-to-take !
    then ;

\ ---- the stalker's scene (Debilitas after her, out of his sight: she hides here) ----
: hunted-unseen? ( -- flag )   \ (Debilitas in play and after her; in here, the camera not on him)
    stalker-told? 0=  st-cond-now @ down-cond = or  debilitas? 0= or  chased? 0= or if  false exit  then
    stalker-in? if  stalker body-at vec@ point-unseen?  else  true  then ;
: his-scene ( -- )
    $29 hewie-slot 1 doors-into  fiona hold
    $29 116 stalker-to  stalker $74 180 place-on-tri  stalker 0 -1 camera-set  stalker false shown  stalker let-go
    15 screen:to-black-2 with-music or fade
    chase-movie scenes:unseen-movie chase-cutscene load-scene  wait-fade
    hewie-in? if  hewie let-go  -1 hewie-sent-off !  then
    chase-door true doors-actor send door-opened  doors-redone  panic-eased
    if  -1 run-scene  then
    stalker let-go  0 summon-take  world-held state!  0 prepare-exit
    chase-door door-shut  forced-exit hewie-sent-off @ if  0  else  1  then  or take-exit
    $29 hewie-slot 0 doors-into  c1f-10-flag3 fact!  chase-seen resident-flag-set ;

\ ---- his doll: the first time beside him, the scene (the Smudged Key); then its dance ----
: doll-scene ( -- )
    stalkers-stay state!  15 screen:to-black-2 with-music or fade  fiona wait-move
    doll-movie scenes:unseen-movie doll-cutscene load-scene  wait-fade
    hewie-in?  hewie-cond-now @ down-cond = and if  hewie hold  hewie false shown  then  panic-eased
    if  -1 run-scene  then  wait-fade
    fiona $4B -6 -13.414e -1.767e place-at  fiona 0 idle  seated
    c1f-10-flag2 fact!  hewie true shown  hewie let-go  camera-restart  15 fade-in
    doll-seen resident-flag-set  smudged-key item-named  smudged-key 1 give-item  smudged-key show-item-tab  fanfare
    obtained-2-msg say  stalkers-stay state-off ;
: doll-dance ( -- )
    fiona wait-move  his-figure his-doll-start 0 7 figure-play  his-figure figure-wait
    his-figure his-doll-dance 1 7 figure-play  -1 doll-danced ! ;

\ ---- what she may do or say here ----
: doll-on-show ( -- )
    fiona $98 15 -1 walk-move 7.28e 20.316e go-to  1 20e 10e 20e 0e shot
    fiona look-low-anim 10 start-blend  doll-on-show-msg say  fiona wait-anim  16 frames  shot-off ;
: not-lying ( -- )  fiona wait-move  fiona look-low-anim 10 start-blend  not-lying-msg say  fiona wait-anim ;
: out-of-sight ( -- )
    fiona wait-move  fiona $9C 90 12e 15e place-at  1 10e 10e 0e 2e shot  world-frozen state!
    16 frames  out-of-sight-msg say  4 frames  shot-off  world-frozen state-off ;
: reach-jerky ( -- )
    fiona wait-move  fiona 11e -58.5e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  beef-jerky c1f-10-beef-jerky jerky-fx takes-up  fiona reach-back-anim play ;
: take-bisque ( -- )
    fiona wait-move  fiona -3.61e 2.23e turn-to-xz  fiona reach-out-anim play  4 frames
    bisque-doll item-named
    bisque-doll item-full? if  cant-carry-msg say
    else  bisque-doll 1 give-item  0 doll-to-take !  bisque-part hide-part  bisque-doll show-item-tab  fanfare  obtained-msg say  then
    fiona reach-back-anim play ;
: spots ( -- )
    he-sits? 0=  c1f-10-flag3 fact? 0= and  fiona shelf-area in-area? and  fiona 0 45 heading? and if
        chased?  in-panic? 0= and if
            hunted-unseen? if  ['] his-scene calm-only offer  else  no-time-message offer-remark-now  then
        else  ['] doll-on-show unhurried offer  then
    then
    c1f-10-beef-jerky fact? 0=  fiona 11e 12e -58.5e 7e 4e touches? and if  ['] reach-jerky calm-only offer  then
    he-sits?  fiona him 20 45 touches-facing? and if
        c1f-10-flag2 fact? 0= if  ['] doll-scene unhurried offer
        else  doll-danced @ 0= if  ['] doll-dance unhurried offer  then  then
    then
    fiona bed-area in-area?  fiona -45 50 heading? and if  ['] not-lying unhurried offer  then
    fiona chest-area in-area?  fiona 45 50 heading? and if  ['] out-of-sight unhurried offer  then
    doll-to-take @  fiona bisque-area 60 faces-area? and if  ['] take-bisque calm-only offer  then ;

\ ---- the movies replayed (the extras: the stalker's scene, his doll's) ----
: replayed ( movie cutscene -- )
    {: movie cut :}  fiona hold  flame-fx 0 24.35e 9e 19.7e flame-at  specks-about
    debilitas-kind 3 figure-in  yield
    movie scenes:unseen-movie cut load-scene  panic-eased
    if  -1 run-scene  then  3 figure-out  replay-over ;
: replay-chase ( -- )  chase-movie chase-cutscene replayed ;
: replay-doll ( -- )  doll-movie doll-cutscene replayed ;

\ ---- every frame ----
create cameras  1 , 0 , -1 ,   2 , 1 , -1 ,   3 , 1 , -1 ,   4 , 2 , -1 ,   \ (area, set, path)
create looks  7.42e 0e 22.15e 15e 9e 9e look,
: each-frame ( -- )
    events-held state? if  exit  then
    0 exit-out  cameras 4 cameras-at
    flame-fx 24.35e 9e 19.7e 8e -4e flame-stirred
    looks 1 hewie-looks-at  spots ;
: replaying ( -- )   \ (the extras' list: its rows for the two)
    hewie send part-from-fiona  replay-act 8 = if  ['] replay-chase  else  ['] replay-doll  then  start drop ;
: person-in ( i -- )
    fiona-p = if  events-held state? if  replaying exit  then  0 1 -1 way-in  then ;
: went ( -- ) ;   \ (Debilitas's figure: taken out by castle-1f-2 as she goes on)

' came-in ' person-in ' each-frame ' went castle-1f-10 place!
