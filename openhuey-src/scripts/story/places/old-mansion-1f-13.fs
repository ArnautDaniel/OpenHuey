\ story/places/old-mansion-1f-13.fs - the mandragora garden of the old mansion's 1F: the first time she
\ steps out of the entrance, the scene (the mandragoras' bed revealed); the five beds (a mandragora
\ pulled from each - her fright up; one at a time; the stalker sent toward her once the beds are
\ revealed); the dried-out plant; the info sheet; the flower bed below; Hebel's Perfume used by
\ Hewie: he leads her to a spot; the story's spirit about; Hewie's looks; dripping water, the
\ specks in the light. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-1f-13.fs (converted from the game's, not loaded).
IN: story.places.old-mansion-1f-13
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names effects story.kit fiona.anims scenes ;

own-reset

0 constant dried-msg      \ "It's totally dried-out. I guess it can't hurt me now... I'll never look at a plant the..."
1 constant sheet-msg      \ "Is this an info sheet on the plants here? There is something written here..."
2 constant blood-msg      \ "In order to procure the large quantity of blood required to complete the grand process,..."
3 constant pull-msg       \ "Full-bodied flowers sprout from the bed. Pull one out? YES NO"
4 constant one-only-msg   \ "You can't hold any more. You must first do something with the one you are carrying."
5 constant no-thanks-msg  \ "No thank you. I've just about had my fill of these freakish things."
6 constant flower-bed-msg \ "The flower bed below contains various flowers stretching in every direction. This must..."
own: frames-in  own: hewie-away  own: perfume-step
2 constant obtained-sound   $35 constant garden-track   1 constant garden-movie   0 constant garden-cutscene

\ ---- the beds: covered (the scene not yet seen) or revealed ----
: beds ( shown? -- )  {: on :}  10 2 do  i on if  object-show  else  object-hide  then  loop ;   \ (mandor_0..7)
: revealed ( -- )  false beds  0 show-part ;
: garden-looks ( -- )   \ (its dripping water, the specks in the light)
    effects:drips
    $10 $E $80 $80 $80 $40 rgba -54.6e 28.6e -20e specks   $10 $E $80 $80 $80 $40 rgba 54.6e 29e -20e specks
    $10 $E $80 $80 $80 $40 rgba -7.5e 5e 53.5e specks       $10 $E $80 $80 $80 $40 rgba 0e 64.8e -20.5e specks ;
: came-in ( exit -- )
    drop  room-sounds  garden-track 1e background  0 frames-in !  garden-looks
    om1f-13-flag2 fact? if  revealed  else  true beds  0 hide-part  then
    0 hole-floor floor-on ;
: spirit-about ( -- )   \ (the story's spirit, not about yet: let out here - before the 2F's way is found)
    om2f-8-flag1 fact?  story-spirit creature-up? or if  exit  then
    old-mansion-1f-13 1 323 2 5 story-spirit-in ;

\ ---- the first time out of the entrance: the scene ----
: garden-scene ( -- )
    garden-movie scenes:unseen-4-movie garden-cutscene load-scene
    0 true no-shadow  panic-eased  if  -1 run-scene  then  0 false no-shadow ;
: hewie-held ( -- )  hewie hold  hewie false shown  begin  yield  again ;   \ (down: hidden away till it's over)
: the-garden ( -- )
    om1f-13-flag2 fact!  stalker let-go  summoner-on state-off  stalkers-stay state!  fiona hold
    15 screen:to-black-2 with-music or fade  no-track 1e background  wait-fade
    hewie-in?  hewie-way-now @ 0= and dup 0= hewie-away !  if
        hewie-cond-now @ down-cond = if  ['] hewie-held start drop  else  hewie let-go  then
    then
    garden-scene  world-held state!  revealed
    hewie-away @ 0= if
        hewie-cond-now @ down-cond = if  ['] hewie-held stop-doing  hewie true shown  hewie let-go
        else  hewie activate  old-mansion-1f-13 182 0 hewie-to  hewie $B6 99 35.67e -1.81e place-at  then
    then
    fiona $1C -43 54.36e -8.26e place-at  0 0 player-camera  yield  camera-restart  om1f-13-flag1 fact!
    garden-track 1e background  15 fade-in  stalkers-stay state-off  scene-locked state-off
    $6C stalker-slot false door-for ;
: replayed ( -- )  fiona hold  garden-looks  garden-scene  replay-over ;

\ ---- the beds: a mandragora pulled (one at a time); the stalker sent toward her ----
: stalker-sent ( -- )
    om1f-13-flag2 fact? 0=  white-mandragora-used fact? or if  exit  then
    old-mansion-1f-8 stalker-slot 1 doors-into
    stalker-told? 0= if  old-mansion-b1-2 217 stalker-to  2 stalker-order  then  his-own-time ;
: pulls ( item fact -- )   \ (fact 0: none kept)
    {: it f :}  stalkers-stay state!  hewie-no-attack state!  fiona hold  fiona wait-move
    pull-msg yes? if
        fiona reach-out-anim play  4 frames  it item-named  it 1 give-item  it show-item-tab  fanfare  obtained-2-msg say
        f if  f fact!  then  60 fright+  obtained-sound room-bank fiona sound-by  fiona reach-back-anim play
        stalker-sent
    then
    stalkers-stay state-off  hewie-no-attack state-off ;
: white ( -- )  white-mandragora 0 pulls ;
: yellow ( -- )  yellow-mandragora got-yellow-mandragora pulls ;
: red ( -- )  red-mandragora got-red-mandragora pulls ;
: blue ( -- )  blue-mandragora got-blue-mandragora pulls ;
: purple ( -- )  purple-mandragora got-purple-mandragora pulls ;
: one-only ( -- )  fiona wait-move  one-only-msg say ;
: no-thanks ( -- )  fiona wait-move  no-thanks-msg say ;
: bed-offer ( xt -- )   \ (what a bed offers: a pull, else why not)
    white-mandragora-used fact? if  drop ['] no-thanks  else  mandragora? if  drop ['] one-only  then  then  unhurried offer ;
: the-beds ( -- )
    fiona $A 60 faces-area? if  ['] white bed-offer  then
    fiona 9 60 faces-area? if  ['] yellow bed-offer  then
    fiona 8 60 faces-area? if  ['] red bed-offer  then
    fiona $B 60 faces-area? if  ['] blue bed-offer  then
    fiona 7 in-area?  fiona 90 60 heading? and if  ['] purple bed-offer  then ;

\ ---- Hebel's Perfume used by Hewie: he leads her to a spot ----
: perfume-wait ( n -- )  begin  perfume-step @ over < while  yield  repeat  drop ;
: hewie-leads ( -- )
    hewie hold  hewie wait-move  hewie fiona body-pos fswap fdrop turn-to-xz  1 perfume-step !
    2 perfume-wait  hewie-bark  hewie wait-move  hewie 0 0 camera-set  hewie follow
    hewie $8D 0 -18e 29e walk-to  hewie-bark  hewie wait-move  30 frames
    fiona 0 0 camera-set  fiona follow  3 perfume-step ! ;
: sprays ( -- )
    stalkers-stay state!  fiona hold  0 perfume-step !  ['] hewie-leads start drop  fiona wait-move
    fiona hewie body-pos fswap fdrop turn-to-xz  1 perfume-wait
    fiona $C06 play  fiona 0 start-anim  1 frames  fiona hewie look-at  yield
    2 perfume-step !  3 perfume-wait  stalkers-stay state-off  5 trust+ ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    $B = if  ['] sprays start drop  true  else  false  then ;

\ ---- what she may say here ----
: the-plant ( -- )  fiona wait-move  dried-msg say ;
: the-sheet ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  sheet-msg say
    15 screen:to-dim fade wait-fade  blood-msg say  15 screen:from-dim fade wait-fade  stalkers-stay state-off ;
: the-flower-bed ( -- )  fiona wait-move  fiona -180 turn  fiona look-low-anim start-anim  40 frames  flower-bed-msg say  fiona wait-anim ;

create entries  0 , 0 ,   1 , 1 ,   1 , 1 ,   \ (by exit 0..2: the camera)
create ahead  3 , 1 ,   4 , 2 ,   \ (area, exit loaded ahead)
create looks  -48.85e 0e -48.91e  33e 11e 11e look,   48.42e -6e 28e  26e 11e 11e look,   2.46e -6e 33.29e  22e 11e 11e look,
    -22.19e -6e 36.99e  20e 11e 11e look,   26.75e 0e -36.99e  14e 11e 11e look,
: each-frame ( -- )
    1 frames-in +!  frames-in @ 2 = if  spirit-about  then
    3 exits-out  ahead 2 ways-out
    om1f-13-flag2 fact? 0=  fiona 5 left? and if  panic-eased  ['] the-garden start drop  then
    looks 5 hewie-looks-at
    fiona 6 60 faces-area? if  ['] the-plant unhurried offer  then
    fiona $C in-area?  fiona 90 60 heading? and if  ['] the-sheet unhurried offer  then
    the-beds
    fiona $D in-area?  fiona -90 60 heading? and  fiona $E in-area?  fiona -90 60 heading? and or if  ['] the-flower-bed unhurried offer  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop  exit  then
    entries 3 ways-in  came-by @ dup 1 =  swap 2 = or if  2 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-13 place!
' items-used old-mansion-1f-13 items-here
