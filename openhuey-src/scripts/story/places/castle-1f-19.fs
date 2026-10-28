\ story/places/castle-1f-19.fs - the flooded passage in Belli Castle's 1F ($1E): a hole low in the
\ wall Hewie can hide in at her "go there" (out again at her "come back"); Debilitas passing his
\ hiding place sets him on the stalker - the ambush, a scene; across the water the Rubrum
\ Medallion, fetched by Hewie at her "go there"; splashes as someone steps into the water, Hewie's
\ bark at the hole, his look at the far side; the hole's remarks. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-1f-19.fs (converted from the game's, not
\ loaded).
IN: story.places.castle-1f-19
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims hewie.anims hewie-names scenes ;

own-reset

\ ---- what is said here ----
0 constant hole-msg        \ "There is a hole in the wall. I think I can see light on the other end. This must le..."
1 constant hole-2-msg      \ "I'm sure I can use this somehow... But I'm just too big and will never fit in there."
2 constant eyes-msg        \ "Hewie's eyes glimmer through the thick shadow that surrounds him."

\ ---- its own state (a visit) ----
own: hole-remarked         \ the hole's first remark made (in turn with the second)
own: hole-barked           \ Hewie has barked at the hole (once a visit)
own: splashed              \ each pool spot's splash made (a bit each)

\ ---- the room's things ----
0 constant hole-group   6 constant leaving-0-area   5 constant leaving-1-area   4 constant leaving-2-area
7 constant leaving-2b-area   8 constant before-hole-area   9 constant hole-area
1 constant to-exit-1-area   2 constant to-exit-2-area   $1A constant passage-track
0 constant rouse-sound   $2E constant go-there-call   1 constant ambush-movie   0 constant ambush-cutscene
3 constant room-light   $22E constant ambush-file   $827A constant ambush-other-file   $23 constant ambush-seen
$12C constant hole-tri   $47 constant before-hole-tri   $2F constant after-ambush-tri   $118 constant shore-tri
3 constant ambush-figure
: hiding? ( -- flag )  yelp-anim-act hewie-doing? ;   \ (in the hole, lying low: his animation while the story's time counts)
: hole-floor-in ( -- )   \ (Hewie in the hole: she and the stalkers kept off its floor)
    hole-group solid-floor floor-off  hole-group keeps-fiona-off keeps-pursuers-off or floor-edge or floor-on ;
: hole-floor-out ( -- )
    hole-group keeps-fiona-off keeps-pursuers-off or floor-off  hole-group solid-floor floor-edge or floor-on ;
: specks-about ( -- )
    $10 $E $80 $80 $80 $40 rgba 41.5e 40.7e -137.7e specks  $10 $E $80 $80 $80 $40 rgba 5.9e 34.6e -158.4e specks ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  passage-track 1e background  hole-floor-out  specks-about ;
: hewie-came ( -- )   \ (hidden away in the hole: there still, her and the stalkers kept off it; else not left on its floor)
    hidden-act hewie-doing? if
        hole-floor-in
        hewie body-tri hole-tri <> if  hewie hole-tri 0 -12e -106e place-at  1800 2 hewie send hewie-yelp  then
    else  hewie hole-group on-group? if  hewie before-hole-tri put-on-tri  then  then ;

\ ---- Hewie into the hole (her "go there" at it) and out (her "come back") ----
: hewie-barks ( -- )  hewie bark-move 0 0  0e 0e 0e 0e  move  hewie wait-move ;
: into-hole ( -- )
    hewie hold  hewie-barks
    hewie before-hole-tri 0 -1 walk-path-move -12e -125e go-to
    hole-group solid-floor floor-off  hole-group keeps-fiona-off keeps-pursuers-off or floor-on
    hewie hole-tri 0 $204 walk-move -12e -106e go-to
    3600 2 hewie send hewie-yelp ;
: out-of-hole ( -- )
    hewie hold  hewie before-hole-tri 180 $204 walk-move -12e -125e go-to
    hewie -1 idle  hewie wait-anim  0 0 hewie send hewie-yelp  0 0 hewie-do
    hole-group keeps-fiona-off keeps-pursuers-off or floor-off  hole-group solid-floor floor-on ;

\ ---- the ambush: Debilitas by the hole, Hewie in it - he springs out at him (a scene) ----
: silhouette ( -- )   \ (the scene's own: Hewie a dark shape through its first shot; then the light lower)
    begin  0 cutscene-frame cutscene-shot-at = 0= while  yield  repeat
    begin  0 cutscene-frame cutscene-shot-at = while  hewie 0 0 0 $C8 rgba tint  yield  repeat
    hewie $80808080 tint
    begin  cutscene-frame $1E3 < while  yield  repeat  room-light 0.5e shadows-longer ;
: ambush ( -- )
    fiona hold  hewie hold  stalker hold  stalkers-stay state!
    15 screen:to-black-2 with-music or fade  no-track 1e background  fiona wait-move
    ambush-movie scenes:keyed-movie ambush-cutscene load-scene  wait-fade
    fiona false shown  force-calm state!  yield  force-calm state-off
    ['] silhouette start drop  $10 $28 movie-key  panic-eased  if  -1 run-scene  then
    stalker let-go  3 summon-take  fiona 0 idle  fiona true shown  hewie $80808080 tint  room-light light-back
    0 0 hewie send hewie-yelp  0 0 hewie-do  hewie after-ambush-tri -90 place-on-tri  hewie-timid state-off
    hole-group keeps-fiona-off keeps-pursuers-off or floor-off  hole-group solid-floor floor-on  hewie let-go
    passage-track 1e background  15 fade-in
    ambush-file give-file  ambush-seen resident-flag-set  5 trust+  ambush-other-file give-file  stalkers-stay state-off ;
: ambush? ( -- flag )   \ (Debilitas up, by the hole; she free, or half hidden; not panicking)
    in-panic? if  false exit  then
    fiona held? 0=  fiona-half-hidden state? or 0= if  false exit  then
    debilitas? 0= if  false exit  then
    st-cond-now @ down-cond <>  stalker before-hole-area in-area? and ;

\ ---- the Rubrum Medallion across the water: her "go there" toward it - Hewie fetches it ----
: fetches ( -- )
    hewie hold  hewie-barks
    hewie shore-tri 100 $204 walk-move 39.008e -266.892e go-to  hewie sit-a-while-anim 10 play-blend
    hewie true noclip  rouse-sound room-bank hewie sound-by
    hewie $13F 100 $204 walk-move 68.224e -274.752e go-to  hewie sit-a-while-anim 10 play-blend
    fiona-half-hidden state?  fiona held? 0= or if
        rubrum-medallion item-full? 0= if  10 trust+  then
        rubrum-medallion c1f-19-rubrum-medallion -1 takes-up
    then
    rouse-sound room-bank hewie sound-by
    hewie shore-tri -80 -1 walk-path-move 39.008e -266.892e go-to  hewie false noclip  stalkers-stay state-off ;
: sends-him ( -- )
    fiona hold  stalkers-stay state!  fiona wait-move
    hewie held? if  stalkers-stay state-off  else  ['] fetches start drop  then
    fiona go-there-gesture-anim start-anim  fiona 1 wait-mark  go-there-call common-bank fiona sound-by
    fiona wait-anim  fiona -1 idle  7 frames ;

\ ---- Hewie here ----
: hole-zone ( F: -- x y z )  -11.187e 5e -117.743e ;
: barks-at-hole ( -- )  hole-zone hewie-barks-at ;
create far-look  46.51e 0e -267.37e 28e 6e 6e look,
: hewie-here ( -- )
    hewie-in? 0= if  exit  then
    hiding? if
        started-now @ come-back-cmd = if  ['] out-of-hole start drop  then
        ambush? if  ['] ambush start drop  then
        exit
    then
    hole-barked @ 0=  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and
    hewie hole-zone 30e 20e centred-in? and if  -1 hole-barked !  30 chance? if  ['] barks-at-hole start drop  then  then
    started-now @ go-there-cmd =  hewie-cond-now @ down-cond <> and 0= if  exit  then
    fiona before-hole-area in-area?  stalker-in? 0= and  fiona 50 -13e -110e faces-point? and if
        hewie-breakable-now @ if  ['] into-hole start drop  then  exit
    then
    c1f-19-rubrum-medallion fact?  chased? or if  exit  then
    68e 0e -274e point-seen? 0=  fiona unseen? or  hewie unseen? or if  exit  then
    fiona 50 68e -274e faces-point?  hewie held? 0= and  hewie-breakable-now @ and if  ['] sends-him start drop  then ;

\ ---- the water: a splash as someone steps in (each spot once a visit) ----
: splash-by ( bit F: x z -- )
    {: bit F: x z :}  splashed @ bit and if  exit  then
    fiona x 0e z 16e 4e steps-into? if  fiona 5 x 0e z splash-from  bit splashed +!  exit  then
    hewie x 0e z 16e 4e steps-into? if  hewie $A x 0e z splash-from  bit splashed +!  exit  then
    stalker x 0e z 16e 4e steps-into? if  stalker 5 x 0e z splash-from  bit splashed +!  then ;
: splashes ( -- )
    1 46e -280e splash-by  2 52e -240e splash-by  4 50e -300e splash-by  8 70e -260e splash-by  16 73e -282e splash-by ;

\ ---- what she may say here ----
: the-hole ( -- )
    fiona $32 0 -1 walk-move -10.776e -128.378e go-to
    fiona crouch-down-anim play  fiona crouching-anim start-anim  4 frames
    hiding?  hewie hole-group on-group? and if  eyes-msg say
    else  hole-remarked @ if  0 hole-remarked !  hole-2-msg  else  -1 hole-remarked !  hole-msg  then  say  then
    4 frames  fiona crouch-up-anim play  fiona -1 idle ;
: spots ( -- )  fiona hole-area in-area?  fiona 0 50 heading? and if  ['] the-hole unhurried offer  then ;

\ ---- the movie replayed (the extras) ----
: replayed ( -- )
    fiona hold  specks-about  debilitas-kind ambush-figure figure-in  yield
    ambush-movie scenes:keyed-movie ambush-cutscene load-scene  fiona false shown
    ['] silhouette start drop  $10 $28 movie-key  panic-eased  if  -1 run-scene  then
    ambush-figure figure-out  fiona true shown  hewie $80808080 tint  room-light light-back  replay-over ;

\ ---- every frame ----
create cameras  $A , 0 , 0 ,   $B , 1 , 1 ,   \ (area, set, path)
create entries  1 , 1 ,   0 , 0 ,   0 , 0 ,   \ (by exit 0, 1, 2: the camera)
: ways ( -- )
    0 exit-out  to-exit-1-area 1 leaves-by  to-exit-2-area 2 leaves-by  cameras 2 cameras-at
    fiona leaving-0-area entered? if  0 prepare-exit  then
    fiona leaving-1-area entered? if  1 prepare-exit  then
    fiona leaving-2-area entered?  fiona leaving-2b-area entered? or if  2 prepare-exit  then ;
: each-frame ( -- )  ways  hewie-here  far-look 1 hewie-looks-at  splashes  spots ;
: person-in ( i -- )
    dup hewie-p = if  drop hewie-came exit  then
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 3 ways-in ;
: went ( -- )   \ (leaving: Hewie, held, let go by the shore)
    hewie held? if  hewie let-go  hewie shore-tri -80 39.008e -266.892e place-at  stalkers-stay state-off  then ;

' came-in ' person-in ' each-frame ' went castle-1f-19 place!
