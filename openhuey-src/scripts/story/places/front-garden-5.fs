\ story/places/front-garden-5.fs - the garden's far corner ($2D): Hewie's ways over its walls -
\ her "go there" (or "come back" from across) sends him over at four places, and at two he snaps
\ at the hung things (one drops the Viola Medallion); the chimes she brushes walking by; a crate
\ (beef jerky), a Medallion Hewie sniffs out, the ball, the rusted door; and a hiding place, where
\ the stalker may find her. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/front-garden-5.fs (converted from the game's, not loaded).
IN: story.places.front-garden-5
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names effects keys story.kit fiona.anims fiona.sounds hewie.sounds stalker.anims ;

own-reset

\ ---- what is said here ----
0 constant rusted-msg   \ "The hinge is rusted solid. Try as I may, I can't open this door."

\ ---- its own state (a visit) ----
own: chimes-rung           \ the first chime last rang "on" (they ring on and off in turn)
own: found-her             \ the stalker has found her hiding (she must flee)
own: came-while-hidden     \ the stalker came in while she hid
own: fall-at               \ the falling thing's frames (and its effect: fall-fx)
own: fall-fx
own: searching           \ his walk to the hiding place (its activity; 0 none)

\ ---- the room's things ----
0 constant whole-part   1 constant broken-part   2 constant lamp-part   2 constant crate-group
0 constant to-exit-0-area   3 constant left-side-area   4 constant right-side-area   9 constant hiding-area
$E constant door-area
0 constant viola-fx   1 constant medallion-fx   2 constant jerky-fx   3 constant ball-fx
2 constant hanger-a   3 constant hanger-b   4 constant first-chime   \ (the room's strings naming them: two hung things, two chimes)
0 constant door-push-sound   1 constant chime-on   2 constant chime-off   3 constant crate-sound
$A constant visits-var   $16 constant garden-track
$803 constant creeping-anim   $207 constant fleeing-anim   \ (her crawl into the hiding place; her run out of it)
fg-5-breakable-broken whole-part broken-part  -14e -10e 97e 5e 8e  breakable: the-crate
the-crate $80404040 0 5 shard-look  the-crate 0.562e 0.5e 0.25e 0.5e shard-uv
the-crate crate-group frees-floor  the-crate $10A noise-from  the-crate crate-sound room-bank shatters-with

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  garden-track 1e background
    0 object-hide  1 object-hide  lamp-part show-part
    c2f-3-flag1 fact? 0= if  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  then
    the-crate as-it-stands
    the-crate broken?  fg-5-beef-jerky fact? 0= and if  jerky-fx -14e -9e 97e flicker-at  then
    fg-5-flag1 fact?  fg-5-viola-medallion fact? 0= and if  viola-fx -265e -9e 160e flicker-at  then
    fg-5-medallion-found fact?  fg-5-medallion fact? 0= and if  medallion-fx -228.4e -9e 103.2e flicker-at  then
    first-chime 2 chimes-rest
    got-ball fact? 0= if  ball-fx -180e 5.61e 176e flicker-at  then ;

\ ---- the hung things: swaying when struck - Hewie snapping as he lands by one, her shove ----
: settles ( obj -- )  sways ;
: something-drops ( -- )   \ (from the first hung thing: falling, drifting, gone below the ground)
    0 fall-at !  -276.5e 3e 160e effects:loop-sprite fall-fx !
    begin  yield  1 fall-at +!
        3e  0.05e fall-at @ dup 1+ * 2/ s>f f*  f-  fdup -10e f> 0= if
            fdrop  fall-fx @ dup alive? if  kill  else  drop  then
            fg-5-flag1 fact!  fg-5-viola-medallion fact? 0= if  viola-fx -265e -9e 160e flicker-at  then  exit
        then
        -276.5e 0.5e fall-at @ s>f f* f+ f>cell  f>cell  160e f>cell  fall-fx @ dup alive? if  send effect-place  else  drop 2drop drop  then
    again ;
: hewie-by? ( F: x z -- flag )   \ (in the air - leaping - within 5 of the spot)
    {: F: x z :}  hewie-in? 0= if  false exit  then  hewie-act-now @ $7F <> if  false exit  then
    hewie x z 5e centre-across? ;
: snaps-at ( obj F: x z -- )   \ (waiting for him to land by it: his snap, it sways)
    {: obj F: x z :}  begin  x z hewie-by? 0= while  yield  repeat
    snap-sound common-bank hewie sound-by
    obj hanger-b = fg-5-flag1 fact? 0= and if  ['] something-drops start drop  then
    obj hewie sway-start  obj settles ;
: snaps-at-b ( -- )  hanger-b -276e 160e snaps-at ;
: snaps-at-a ( -- )  hanger-a -259.5e 190e snaps-at ;
: shoved ( obj -- )  shove-hit-sound common-bank fiona sound-by  dup fiona sway-start  settles ;
: shoved-a ( -- )  hanger-a shoved ;
: shoved-b ( -- )  hanger-b shoved ;
: shoves ( -- )   \ (her blow on one: it sways)
    -259.5e -10e 190e 5e 15e her-blow-in? if  ['] shoved-a start-only  then
    -276e -10e 160e 5e 15e her-blow-in? if  ['] shoved-b start-only  then ;

\ ---- Hewie's ways over: sent by her ("go there"; "come back" when she is across) ----
: path-floor ( group on -- )   \ (the floor he leaps over: kept from her and the stalkers while he does)
    {: g on | keep :}  keeps-fiona-off keeps-pursuers-off or to keep
    on if  g solid-floor floor-off  g keep floor-on  else  g keep floor-off  g solid-floor floor-on  then ;
: barks ( -- )   \ (he is taken from what he does: a bark first)
    hewie hold  hewie wait-move  hewie bark-move 0 0  0e 0e 0e 0e  move  hewie wait-move ;
: over-left ( tri face F: x z -- )   \ (to the wall's foot, then a leap over it)
    {: tri face F: x z :}  barks  hewie tri face -1 walk-path-move x z go-to
    0 true path-floor  $155 35 -130e 1.5e 115e hewie-leap  hewie wait-move  0 false path-floor ;
: left-from-near ( -- )  $14C -90 -90e 115e over-left ;
: left-from-far ( -- )  $4E 90 -170e 115e over-left ;
: round-right ( tri face dest F: x z -- )   \ (round by the right: the floor freed for good)
    {: tri face dest F: x z :}  barks  1 solid-floor floor-off
    hewie tri face -1 walk-path-move x z go-to  hewie dest -1 run-path-move path-to
    1 keeps-fiona-off keeps-pursuers-off or floor-off ;
: right-from-near ( -- )  $63 -90 $137 -130e 180e round-right ;
: right-from-far ( -- )  $39 90 $1B9 -230e 180e round-right ;
: up-to ( tri face dest snap F: x z lx ly lz -- )   \ (by a hung thing: a leap up to it - his snap at it meanwhile)
    {: tri face dest snap F: x z lx ly lz :}  barks  snap start drop
    hewie tri face -1 walk-path-move x z go-to  dest 35 lx ly lz hewie-leap  hewie wait-move ;
: to-hanger-b ( -- )  $1C -90 $193 ['] snaps-at-b  -236e 160e  -276e 1e 160e up-to ;
: to-hanger-a ( -- )  $161 -90 $8A ['] snaps-at-a  -220e 190e  -259.5e 1e 190.1e up-to ;
: go-there? ( -- flag )  started-now @ go-there-cmd = ;
: come-back? ( -- flag )  started-now @ come-back-cmd = ;
: unseen-from? ( F: x y z -- flag )  point-unseen? ;   \ (the spot off the screen: she faces it, the camera doesn't show it)
variable way-sent   \ (the way he is sent this frame: the last that fits, as the original's)
: sent ( xt -- )  hewie-breakable-now @ if  way-sent !  else  drop  then ;
: left-way ( -- )
    fiona 50 -130e 115e faces-point?  -130e -10e 115e unseen-from? and 0= if  exit  then
    hewie left-side-area in-area? if
        go-there? if  ['] left-from-near sent  else  come-back?  fiona left-side-area in-area? 0= and if  ['] left-from-near sent  then  then
    else
        go-there? if  ['] left-from-far sent  else  come-back?  fiona left-side-area in-area? and if  ['] left-from-far sent  then  then
    then ;
: right-way ( -- )
    fiona 50 -180e 180e faces-point?  -180e 5e 180e unseen-from? and 0= if  exit  then
    hewie right-side-area in-area? 0= if
        go-there? if  ['] right-from-near sent  else  come-back?  fiona right-side-area in-area? and if  ['] right-from-near sent  then  then
    else
        go-there? if  ['] right-from-far sent  else  come-back?  fiona right-side-area in-area? 0= and if  ['] right-from-far sent  then  then
    then ;
: hanger-ways ( -- )
    fiona 30 -276e 160e faces-point?  -276e -10e 160e unseen-from? and  go-there? and if  ['] to-hanger-b sent  then
    fiona 30 -260e 190e faces-point?  -260e -10e 190e unseen-from? and  go-there? and if  ['] to-hanger-a sent  then ;
: hewie-ways ( -- )   \ (calm, both of them in view, he up and about)
    danger-now @ calm = 0= if  exit  then
    hewie-in?  hewie-cond-now @ down-cond <> and  fiona unseen? 0= and  hewie unseen? 0= and 0= if  exit  then
    0 way-sent !  left-way  right-way  hanger-ways
    way-sent @ ?dup if  start drop  then ;

\ ---- Hewie: the Medallion he sniffs out; what he looks at by the hung things ----
: medallion-sniffed ( -- )  fg-5-medallion-found fg-5-medallion medallion-fx  -228.4e -9e 103.2e  hewie-finds ;
: hewie-sniffs ( -- )
    fg-5-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie -228.4e -10e 103.2e 10e 5e touches? 0= if  exit  then
    ['] medallion-sniffed start-only ;
create looks
    -275.96e -10e 159.38e  20e 13e 13e look,
    -259.2e  -10e 189.64e  20e 13e 13e look,

\ ---- the hiding place: in, crouched, calming; out by the button - or found, and out running ----
: caught-out ( -- )   \ (found, or panicking: up and away)
    found-her @ if  2 avoid-prompt  then
    fiona-hidden state-off  fiona follow  stalkers-stay state!
    fiona $C3 89 fleeing-anim walk-move -67.3e 79.93e go-to
    fiona 4 idle  fiona false noclip  stalkers-stay state-off ;
: hides ( -- )
    stalkers-stay state!  0 came-while-hidden !  0 found-her !
    fiona $E5 -90 -1 walk-move -74.756e 82.505e go-to
    fiona true noclip  fiona crouch-down-anim play
    fiona $D6 -90 creeping-anim walk-move -100.488e 79.931e go-to
    fiona $D6 90 creeping-anim walk-move -100.488e 79.931e go-to
    fiona crouching-anim start-anim  fiona-hidden state!  stalkers-stay state-off  0 avoid-prompt
    begin
        circle button-pressed? if
            fiona-hidden state-off  fiona follow  searching @ ?dup if  stop-act  0 searching !  then
            chased? if  caught-out exit  then
            stalkers-stay state!  fiona $E5 90 creeping-anim walk-move -74.756e 82.505e go-to
            fiona crouch-up-anim play  fiona false noclip  stalkers-stay state-off  exit
        then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound common-bank fiona sound-by  then  caught-out exit  then
        came-while-hidden hidden-calm  yield
    again ;
: search-odds ( -- pct )  visits-var var@ case  1 of 25 endof  2 of 50 endof  3 of 75 endof  >r 0 r>  endcase ;
: finds-her ( -- )   \ (he goes to the hiding place: there she is)
    stalker hold  stalker wait-move  stalker $1E6 -90 -1 walk-move -45.859e 82.656e go-to
    stalker there-anim play  -1 found-her !  stalker 10 idle
    begin  fiona held?  while  yield  repeat  0 stalker-order  0 searching ! ;
: stalker-came ( -- )
    fiona-hidden state? 0= if  0 came-while-hidden !  exit  then
    -1 came-while-hidden !  stalker follow
    search-odds will-find? if  ['] finds-her start searching !  else  120 summoner-sooner  then
    visits-var var@ 1+ visits-var var! ;

\ ---- what she may do or say here ----
: rusted-door ( -- )
    fiona $41 180 -1 walk-move -190.101e 72.001e go-to  4 frames
    fiona lean-anim play  door-push-sound room-bank fiona sound-by
    fiona strain-anim start-anim  16 frames  fiona let-go-push-anim play
    fiona -1 idle  7 frames  rusted-msg say ;
: pick-viola ( -- )  viola-medallion fg-5-viola-medallion viola-fx -265e 160e picks-up ;
: pick-medallion ( -- )  medallion fg-5-medallion medallion-fx -228.4e 103.2e picks-up ;
: pick-jerky ( -- )  beef-jerky fg-5-beef-jerky jerky-fx -14e 97e picks-up ;
: pick-ball ( -- )  ball got-ball ball-fx -180e 176e picks-up ;
: spots ( -- )
    found-file-202 fact?  fiona hiding-area in-area? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then
    fg-5-flag1 fact?  fg-5-viola-medallion fact? 0= and  fiona -265e -9e 160e 5e 5e touches? and if  ['] pick-viola calm-only offer  then
    fg-5-medallion-found fact?  fg-5-medallion fact? 0= and  fiona -228.4e -9e 103.2e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    fiona door-area in-area?  fiona 90 60 heading? and if  ['] rusted-door unhurried offer  then
    the-crate broken? if
        fg-5-beef-jerky fact? 0=  fiona -14e -9e 97e 5e 5e touches? and if  ['] pick-jerky calm-only offer  then
    else  the-crate breaks? if  jerky-fx -14e -9e 97e flicker-at  then  then
    got-ball fact? 0=  fiona -180e 5.61e 176e 5e 5e touches? and if  ['] pick-ball calm-only offer  then ;

\ ---- every frame ----
create cameras  1 , 0 , 0 ,   2 , 1 , 1 ,   $A , 0 , 0 ,   $B , 0 , 0 ,   $C , 1 , 1 ,   $D , 1 , 1 ,   \ (area, set, path)
: ways ( -- )  to-exit-0-area 0 leaves-by  cameras 6 cameras-at ;
: walking-by ( -- )   \ (among the chimes)
    fiona -97e 82e 14e centre-across? if  first-chime 2 false chimes-rung chime-on chime-off 5e chimes  then ;
: each-frame ( -- )
    ways  hewie-ways  shoves  hewie-sniffs  walking-by  looks 2 hewie-looks-at  spots ;
: person-in ( i -- )
    case  fiona-p of  0 0 0 way-in  endof  stalker-p of  stalker-came  endof  endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went front-garden-5 place!
