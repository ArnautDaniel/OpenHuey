\ story/places/water-tower-8f-1.fs - the top of the Water Tower ($C7): Riccardo's end. Coming up the
\ first time after the garden's scene - the scene (Lorenzo takes Riccardo's Azoth), on by the forced
\ way. The diorama (her remark; with the Folded Memo, the magisterium; after, the switch): a figure
\ set in it - with the Wooden Miniature, at the spot picked with a cursor - brings Lorenzo, Riccardo
\ loose again. The three models on the walls, worn by her blows till they fall (the third into the
\ water below); the walls' remark; the drop's two remarks in turn; the holes Hewie fits in - at her
\ sign he hides in one, lying in wait, eyes on whoever is near; at her "go for it" with Riccardo by
\ the hole, he grabs him from behind - by the first hole, with its model down, her sign ends it: the
\ final scenes (Riccardo falls; the doors opened, the files); at her "come back" he leaves it. The
\ locked door; the light on the water; the walls' shade; Hewie's bark at the drop; the replays. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/water-tower-8f-1.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.water-tower-8f-1
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names keys story.kit fiona.anims scenes ;

0 constant fiona-msg          \ "Fiona!"
1 constant azoth-msg          \ "Riccardo... You were always... incomplete. But now, I will have your Azoth. You..."
2 constant locked-msg         \ "It's locked!"
3 constant defy-msg           \ "Fiona! Why must you defy me? Why do you run? Let me into your womb!"
4 constant diorama-msg        \ "Wow. I've never seen anything like this before... Looks like some kind of diorama o..."
5 constant choose-msg         \ "Choose a location to set the model in. ?:USE ?:CANCEL"
6 constant wall-msg           \ "This wall is in really bad shape. It looks like it might break if a strong force impact..."
7 constant drop-msg           \ "If I fell from here... it would be all over. Come on, Fiona... Best to just keep it..."
8 constant path-msg           \ "A path has appeared on the surface of the lake. If I can just cross over that path..."
9 constant hole-msg           \ "There is a small hole... It looks just big enough for Hewie to fit in."
$A constant magisterium-msg   \ "So... the 'magisterium' listed in the memo... was actually referring to this minia..."
$B constant switch-msg        \ "So the miniature was the activation switch?"
$C constant again-msg         \ "If I try this out again and seal off my escape route I'd have no one to blame but myse..."
own: lying-down            \ Hewie lying in wait in a hole
own: grabbed               \ ... Riccardo came by and Hewie got him (her "go for it")
own: called-out            \ ... she called him back
own: hewie-clear           \ ... off the hole's floor (its way back as it was)
own: spot-chosen           \ the cursor's spot confirmed (else cancelled)
own: miniature?            \ the figure set is the Wooden Miniature (it fits)
own: model-set             \ the figure set in place: Lorenzo comes
own: drop-remarked         \ the drop's first remark made (they go in turn)
own: hewie-noticed         \ Hewie barked at the drop (once a visit)
own: final-on              \ the final scene under way (the doorway's shade off)
own: hole                  \ the hole Hewie went for (0 .. 2)
own: hewie-tint            \ his colour as he slips in ($RRGGBBAA as the GS has it)
own: stage-1  own: stage-2  own: stage-3   \ each model's wear (3 .. 0: down)
own: cursor-x  own: cursor-y               \ the aiming cursor (of the 512 x 448 screen)
own: hewie-in-hole         \ Hewie on the holes' floor last frame (for leaving: the next room's floor is in by then)
0 constant set-sound   1 constant cant-set-sound   7 constant crumble-sound   8 constant falls-sound
$2C constant cancel-sound   $86 constant plain-bank   $2F constant gesture-sound
$1B constant tower-track   9 constant lorenzo-track
$75 constant grab-act   $204 constant hewie-walk-anim   0 constant go-for-it-gesture   \ (her sign: gesture 0)
0 constant hole-floor-group   \ (nav group 0: the holes' floor - solid, but Hewie's way while he goes in)
solid-floor keeps-fiona-off or keeps-pursuers-off or constant in-hole-flags
$B constant diorama-area   $C constant wall-1-area   $D constant wall-2-area   $E constant wall-3-area
$103 constant top-door   $100 constant stairs-door   $E0 constant down-door   $81 constant azoth-way
: water-light ( -- id )  0 $40404040  0e 30.1e 0e  3750e 0.05e 0.01e  effects:caustic ;
9 constant water-fx
: light-on ( -- )  water-light water-fx effect! ;
: light-off ( -- )  water-fx effect-off ;
: came-in ( exit -- )
    drop  room-sounds  tower-track 1e background
    c7-second-event fact? 0= if  0 hide-part  1 hide-part  8 object-hide  9 object-hide  $A object-hide
    else  0 show-part  1 show-part  then
    wt8f-1-flag1 fact? 0= if  3 stage-1 !  2 show-part  3 hide-part  else  2 hide-part  3 show-part  then
    wt8f-1-flag3 fact? 0= if  3 stage-2 !  4 show-part  5 hide-part  else  4 hide-part  5 show-part  then
    wt8f-1-flag4 fact? 0= if  3 stage-3 !  6 show-part  7 hide-part  else  6 hide-part  7 show-part  then
    8 show-part  light-on ;

\ ---- tints: Hewie's as he slips in and out of sight (alpha $80 full, 0 gone); the stalker's in the
\ scenes, fading in from a frame of the scene ----
: hewie-shown ( gs -- )  hewie swap gs-tint ;
: hewie-fades ( from target step -- )   \ (a step a frame, then drawn as he is)
    {: from target step :}  from hewie-tint !
    begin  hewie-tint @ dup hewie-shown  target <> while  step hewie-tint +!  yield  repeat
    hewie $80808080 tint ;
: him-shown ( gs -- )  stalker swap gs-tint ;
: him-fading ( from rate -- )   \ (each frame: his alpha up rate a frame from the scene's frame `from` - full before it)
    {: from F: rate :}  cutscene-frame dup from < if  drop $80  else  from - s>f rate f* f>s $80 min  then
    24 lshift $808080 or  him-shown ;
: him-as-he-is ( -- )  stalker $80808080 tint ;
: him-fades-in ( start end from F: rate -- )   \ (an activity, with a scene: from its frame start to end)
    {: start end from F: rate :}  him-as-he-is
    begin  cutscene-frame start < while  yield  repeat
    begin  cutscene-frame end < while  from rate him-fading  yield  repeat
    $FF808080 him-shown ;
: lorenzo-fades ( -- )  $4F5 $523 1268 2.79e him-fades-in ;
: riccardo-fades ( -- )  $1A $38 25 4.27e him-fades-in ;
: shot? ( n -- flag )  cutscene-frame cutscene-shot-at = ;
: lights-flare ( -- )   \ (an activity: each frame over the last at two strengths in turn)
    begin  0 shot? 0= while  yield  repeat
    begin  cutscene-frame $E8 < while  $3E frame-blend  yield  repeat
    begin  cutscene-frame $E9 < while  yield  repeat
    begin  cutscene-frame $11C < while  $29 frame-blend  yield  repeat ;

\ ---- the models on the walls: her blows wear them (a weak one 1, else 5) - dust; down, its debris
\ falls and the wall's other part shows ----
: worn ( stage -- )   \ (a blow)
    dup @  fiona-sub-now @ story.base:weak-blow-sub = if  1  else  5  then  -  0 max  swap ! ;
: model-1 ( -- )
    crumble-sound room-bank fiona sound-by  stage-1 worn  -1e 623e 77e dust  4e 623e 77e dust
    stage-1 @ 0= if  falls-sound room-bank fiona sound-by  0 effects:debris  3 effects:debris  2 hide-part  3 show-part  wt8f-1-flag1 fact!  then
    30 frames ;
: model-2 ( -- )
    crumble-sound room-bank fiona sound-by  stage-2 worn  -59e 623e 50e dust  -55e 623e 54e dust
    stage-2 @ 0= if  falls-sound room-bank fiona sound-by  1 effects:debris  4 effects:debris  4 hide-part  5 show-part  wt8f-1-flag3 fact!  then
    30 frames ;
: model-3 ( -- )
    crumble-sound room-bank fiona sound-by  stage-3 worn  -8e 623e -77e dust  -2e 623e -77e dust
    stage-3 @ 0= if  falls-sound room-bank fiona sound-by  2 effects:debris  5 effects:debris  6 hide-part  7 show-part  wt8f-1-flag4 fact!  then
    30 frames ;
: struck-at ( xt area F: x z -- )   \ (her blow there, in the area: worn - one blow at a time)
    {: xt area F: x z :}  fiona area in-area?  x 620e z 10e 8e her-blow-in? and  xt running? 0= and if  xt start drop  then ;
: models ( -- )
    wt8f-1-flag1 fact? 0= if  ['] model-1 wall-1-area 3e 79e struck-at  then
    wt8f-1-flag3 fact? 0= if  ['] model-2 wall-2-area -58e 54e struck-at  then
    wt8f-1-flag4 fact? 0= if  ['] model-3 wall-3-area -5e -78e struck-at  then ;

\ ---- the holes: at her sign, facing one, Hewie hides in it - lying in wait, eyes on whoever is by it;
\ he grabs Riccardo at her "go for it", or comes out at her "come back" ----
create holes  -6 , -12 ,  -9 , 10 ,  12 , 7 ,     \ (where she points: each hole)
create hole-areas  5 , 6 , 7 ,   create by-areas  8 , 9 , $A ,   create her-areas  $F , $10 , $11 ,
: hole@ ( table -- n )  hole @ cells + @ ;
: hole-point ( k -- ) ( F: -- x z )  2* cells holes +  dup @ s>f  cell+ @ s>f ;
: hewie-to-hole ( -- )
    hole @ case
        0 of  hewie $91 20 -1 walk-path-move -15.35e -32.75e go-to  endof
        1 of  hewie $C7 140 -1 walk-path-move -22.68e 28.06e go-to  endof
        2 of  hewie $14 -120 -1 walk-path-move 31.63e 18.64e go-to  endof
    endcase ;
: hewie-into-hole ( -- )
    hole @ case
        0 of  hewie $C4 20 hewie-walk-anim walk-move -5.62e -11.6e go-to  endof
        1 of  hewie $C0 140 hewie-walk-anim walk-move -8.96e 10.23e go-to  endof
        2 of  hewie $BE -120 hewie-walk-anim walk-move 11.74e 6.8e go-to  endof
    endcase ;
: hewie-in-it ( -- )
    hole @ case
        0 of  hewie $D0 5 -0.12e 8.95e place-at  endof
        1 of  hewie $FF -125 -7.73e -4.22e place-at  endof
        2 of  hewie $F9 117 6.6e -4.24e place-at  endof
    endcase ;
: hewie-out-of-it ( -- )   \ (his leap out, back to her side)
    hole @ case
        0 of  $33 100 2.39e 1e 70.98e hewie-leap  endof
        1 of  $38 100 -61.02e 1e -37.59e hewie-leap  endof
        2 of  $4C 100 61.9e 1e -34.57e hewie-leap  endof
    endcase  hewie wait-move ;
: hole-way ( open? -- )   \ (the holes' floor: Hewie's way in, not hers nor the pursuers' - else solid)
    if  hole-floor-group solid-floor floor-off  hole-floor-group keeps-fiona-off keeps-pursuers-off or floor-on
    else  hole-floor-group keeps-fiona-off keeps-pursuers-off or floor-off  hole-floor-group solid-floor floor-on  then ;
: watches ( -- )   \ (his eyes on Riccardo by the hole, else her there, else ahead)
    stalker by-areas hole@ in-area? if  hewie stalker look-at exit  then
    fiona by-areas hole@ in-area? if  hewie fiona look-at exit  then
    hewie -1 look-at ;
: hides ( -- )   \ (an activity)
    hewie hold  hewie wait-move  hewie-bark  hewie wait-move  hewie-to-hole
    hewie true noclip  true hole-way  hewie-into-hole
    $80808080 $808080 $10000000 negate hewie-fades  hewie-in-it  hewie false noclip
    hewie still-anim-move hewie-walk-anim 0  0e 0e 0e 0e  move
    $808080 $80808080 $10000000 hewie-fades
    hewie hewie.anims:stand-anim 8 start-blend  8 frames
    hewie hewie.anims:lie-down-anim start-anim  hewie wait-anim  hewie hewie.anims:lie-anim 10 start-blend  10 frames
    -1 lying-down !
    begin  grabbed @ 0=  called-out @ 0= and while  watches  yield  yield  repeat
    hewie -1 look-at  yield
    grabbed @ if
        grab-act 0 hewie-do  0 hewie-clear !
        begin
            hewie-clear @ 0=  hewie in-hole-flags on-floor-of? 0= and if  -1 hewie-clear !  false hole-way  then
            grab-act hewie-doing?
        while  yield  repeat
        hewie in-hole-flags on-floor-of? if  hewie-out-of-it  false hole-way  then
    else
        hewie-bark  hewie wait-move  hewie-out-of-it  false hole-way
    then
    0 lying-down !  0 grabbed !  0 called-out ! ;

\ ---- the final scenes: Riccardo grabbed by the first hole, its model down - her sign; he falls ----
: final-scene ( -- )
    no-track 1e background  stalkers-stay state!
    1 scenes:unseen-movie 0 load-scene drop  15 screen:to-black-2 fade  wait-fade
    stalker let-go  0 summon-take  hewie let-go
    ['] riccardo-fades start drop  -1 final-on !  panic-eased  -1 run-scene  0 final-on !
    $FF808080 him-shown  scene-5-pending state-off  no-stalker-camera state-off
    stalker let-go  0 summon-take  hewie let-go
    3 scenes:unseen-movie 2 load-scene drop  ['] lights-flare start drop  panic-eased  fiona-msg run-scene
    $40 resident-flag-set  50 trust+  stalker let-go  0 summon-take  c7-second-event fact!
    fiona $33 0 -1.38e 70.2e place-at  hewie $33 0 5e 67.76e place-at
    fg-3-flag3 fact? 0= if  stalker-unloaded  then
    camera-restart  top-door unlock  stairs-door unlock  down-door lock
    water-tower-7f-1 stalker-slot 0 doors-into  water-tower-7f-2 stalker-slot 0 doors-into  water-tower-8f-1 stalker-slot 0 doors-into
    hole-floor-group solid-floor floor-on  0 lying-down !  0 grabbed !  0 called-out !
    tower-track 1e background  15 screen:from-black with-music or fade  wait-fade
    $41 resident-flag-set  $24D give-file  $828B give-file  trust-file  stalkers-stay state-off ;
: ends-it ( -- )   \ (her "go for it" - he is held: the end)
    fiona wait-move  fiona stalker look-at  yield
    fiona go-for-it-gesture-anim start-anim  fiona 1 wait-mark  gesture-sound common-bank fiona sound-by  fiona wait-anim
    fiona -1 idle  7 frames  final-scene ;
: sign-to-hole ( -- )   \ (her sign facing a hole, in its area: he hides there)
    fiona-cmd-now @ go-for-it-gesture <> if  exit  then
    3 0 do
        fiona i hole-areas swap cells + @ in-area? if
            i hole-point fiona 60 faces-point?  hewie-breakable-now @ and if  i hole !  ['] hides start drop  then
            unloop exit
        then
    loop ;
: by-the-hole? ( -- flag )  st-cond-now @ down-cond <>  stalker-free? and  stalker by-areas hole@ in-area? and ;
: grab-can? ( -- flag )  by-the-hole? if  grab-ready?  else  grab-forget  false  then ;   \ (Riccardo by the hole: can he be grabbed?)
: lying-in-wait ( -- )   \ (her "go for it" with Riccardo by the hole: he's grabbed; "come back": out)
    grab-can?  started-now @ go-for-it-cmd = if
        if  -1 grabbed !  hole @ 0=  wt8f-1-flag1 fact? and if  panic-eased  ['] ends-it fiona start-holding drop  then  then
        exit
    then  drop
    started-now @ come-back-cmd = if  -1 called-out !  then ;
: holes-here ( -- )
    grabbed @  called-out @ or if  exit  then
    lying-down @ if  lying-in-wait  else  sign-to-hole  then ;

\ ---- the diorama: a figure set in it, at a spot picked with the cursor - the miniature, in its
\ place, brings Lorenzo ----
: cursor-draw ( -- )
    ps2-screen:frame!  story.base:cursor-layer ui-clear
    story.base:cursor-layer  cursor-x @ ps2-screen:sx  cursor-y @ ps2-screen:sy  20 ps2-screen:sw  28 ps2-screen:sy
    0 $C -1  176 0 80 112  $FFFFFFFF  ui-image ;
: cursor-off ( -- )  story.base:cursor-layer ui-clear ;
: cursor-step ( -- )   \ (4 a frame by the arrows held: x 0..492, y 0..420)
    count-arrows
    1 cells arrows-held + @ 0>  3 cells arrows-held + @ 0> <> if
        cursor-x @  3 cells arrows-held + @ 0> if  4 - 0 max  else  4 + 492 min  then  cursor-x !
    then
    2 cells arrows-held + @ 0>  0 cells arrows-held + @ 0> <> if
        cursor-y @  0 cells arrows-held + @ 0> if  4 - 0 max  else  4 + 420 min  then  cursor-y !
    then ;
: in-its-place? ( -- flag )  cursor-x @ $97 $10E within  cursor-y @ $AB $DC within and ;
: over-diorama ( -- )   \ (frozen, over it, the camera on it)
    fiona true noclip  8 frames  world-frozen state!  fiona $36 180 0e 33e place-at  1 10e 60e 0e 5e shot  8 frames ;
: back-from-it ( -- )   \ (the camera hers again, back where she stood)
    shot-off  world-frozen state-off  fiona false noclip  fiona $9D 180 0e 41e place-at ;
: lorenzo-comes ( -- )
    15 screen:to-black-2 with-music or fade  no-track 1e background  wait-fade
    8 hide-part  light-off  shot-off  world-frozen state-off  stalkers-stay state!  hewie let-go
    begin  partner 0<  while  yield  repeat  summoner-on state-off
    5 scenes:keyed-movie 4 load-scene drop  ['] lorenzo-fades start drop  $10 $50 movie-key  panic-eased  defy-msg run-scene
    1 show-part  8 show-part  light-on
    stairs-door door-shut  stairs-door lock  doors-redone  camera-restart
    fiona $33 180 6.38e 70.12e place-at
    hewie activate  this-room @ 41 0 hewie-to  hewie $29 -160 -17.04e 65.97e place-at
    stalker activate  this-room @ 89 stalker-to  stalker $59 0 -58.86e 10.32e place-at  him-as-he-is
    hole-floor-group solid-floor floor-on  0 lying-down !  0 grabbed !  0 called-out !
    scene-5-pending state!  lorenzo-track 1e background  no-stalker-camera state!
    15 screen:from-black fade  wait-fade  $3F resident-flag-set  stalkers-stay state-off ;
: picked ( -- )   \ (cross: set there - the miniature in its place; else it won't go. Triangle: put away)
    begin
        cursor-step  cursor-draw
        cross button-pressed? if
            miniature? @ 0=  in-its-place? 0= or if  cant-set-sound plain-bank 0e 0e 0e sound-there
            else
                set-sound room-bank -1e 632e 31e sound-there  0 show-part  wooden-miniature spend  -1 model-set !
                stalker-unloaded  kind37-kind stalker-ready  text-gone  cursor-off  30 frames  exit
            then
        then
        triangle button-pressed? if  cancel-sound common-bank 0e 0e 0e sound-there  text-gone  cursor-off  30 frames  exit  then
        yield
    again ;
: sets-figure ( -- )   \ (an activity: the figure used at the diorama)
    fiona hold  fiona wait-move  over-diorama
    246 cursor-x !  242 cursor-y !  cursor-draw  choose-msg show
    picked  model-set @ if  lorenzo-comes exit  then  back-from-it ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    case
        3 of  0 miniature? !  ['] sets-figure start drop  true  endof
        4 of  -1 miniature? !  ['] sets-figure start drop  true  endof
        $10 of  [: fiona wait-move  again-msg say ;] start drop  true  endof
        >r false r>
    endcase ;

\ ---- coming up the first time after the garden's scene: Lorenzo takes Riccardo's Azoth ----
: azoth-scene ( -- )
    15 screen:to-black-2 with-music or fade  no-track 1e background  stalkers-stay state!
    fiona wait-move  wait-fade  fiona false shown
    $B 3 figure-in  $B before-reflections-layer layer  partner-kind usual-layer layer  3 figure-on  hewie let-go
    7 scenes:keyed-movie 6 load-scene drop  $2D $41 movie-key  panic-eased  azoth-msg run-scene
    stalker-unloaded  3 figure-out  world-held state!  0 prepare-exit
    wt8f-1-flag2 fact!  $42 resident-flag-set  azoth-way take-exit ;

\ ---- what she may do or say here ----
: the-diorama ( -- )
    fiona wait-move
    c7-second-event fact? 0= if  over-diorama  diorama-msg say  8 frames  back-from-it  exit  then
    folded-memo 1 has-items? if
        fiona true noclip  8 frames  world-frozen state!  fiona $D2 180 -0.95e 33e place-at  1 2e 50e 0e 5e shot  8 frames
        magisterium-msg say  8 frames  back-from-it  exit
    then
    switch-msg say ;
: the-wall ( -- )  fiona wait-move  wall-msg say ;
: the-drop ( -- )   \ (her two remarks in turn)
    fiona wait-move  fiona look-low-anim start-anim  16 frames
    drop-remarked @ if  0 drop-remarked !  path-msg say  else  -1 drop-remarked !  drop-msg say  then
    fiona wait-anim ;
: the-hole ( -- )
    fiona wait-move
    3 0 do  fiona i her-areas swap cells + @ in-area? if  fiona i hole-point turn-to-xz  leave  then  loop
    fiona look-low-anim start-anim  16 frames  hole-msg say  fiona wait-anim ;
: barks-at-drop ( -- )  0e 620e 41e hewie-barks-at ;
: spots ( -- )
    scene-5-pending state?  8 locked-request? and if  locked-msg any-time offer-remark  then
    fiona diorama-area in-area?  fiona 180 60 heading? and if  ['] the-diorama unhurried offer  then
    wt8f-1-flag3 fact? 0=  fiona wall-2-area in-area? and  -58e 54e fiona 50 faces-point? and if  ['] the-wall unhurried offer  then
    wt8f-1-flag4 fact? 0=  fiona wall-3-area in-area? and  -5e -78e fiona 50 faces-point? and if  ['] the-wall unhurried offer  then
    fiona wall-1-area in-area?  3e 79e fiona 50 faces-point? and if
        wt8f-1-flag1 fact? 0= if  ['] the-wall unhurried offer  else  c7-second-event fact? if  ['] the-drop unhurried offer  then  then
    then
    3 0 do  fiona i her-areas swap cells + @ in-area?  i hole-point fiona 60 faces-point? and if  ['] the-hole unhurried offer  then  loop ;
: hewie-at-drop ( -- )
    c7-second-event fact?  hewie-noticed @ or  hewie-in? 0= or  if  exit  then
    hewie-cond-now @ well-cond =  chased? 0= and  hewie-cmdable-now @ and  hewie 0e 620e 41e 17e 10e centred-in? and if
        -1 hewie-noticed !  ['] barks-at-drop start drop
    then ;

\ ---- the replays ----
: replay-lorenzo ( -- )
    0 show-part  2 show-part  4 show-part  6 show-part  8 show-part  20000e far-clip
    kind37-kind stalker-ready  begin  partner 0<  while  yield  repeat
    5 scenes:keyed-movie 4 load-scene drop  ['] lorenzo-fades start drop  $10 $50 movie-key  panic-eased  defy-msg run-scene
    stalker-unloaded  replay-over ;
: replay-final ( -- )
    0 show-part  1 show-part  3 show-part  5 show-part  7 show-part  8 show-part  20000e far-clip
    kind37-kind stalker-ready  begin  partner 0<  while  yield  repeat
    1 scenes:unseen-movie 0 load-scene drop  ['] riccardo-fades start drop  -1 final-on !  panic-eased  -1 run-scene  0 final-on !
    $FF808080 him-shown  stalker-unloaded  replay-over ;
: replay-flare ( -- )
    0 show-part  1 show-part  3 show-part  5 show-part  7 show-part  8 show-part  20000e far-clip
    tintstalker-kind 3 figure-in  yield  3 figure-on
    3 scenes:unseen-movie 2 load-scene drop  ['] lights-flare start drop  panic-eased  fiona-msg run-scene
    hewie let-go  3 figure-out  replay-over ;
: replay-azoth ( -- )
    0 show-part  1 show-part  3 show-part  5 show-part  7 show-part  8 show-part  20000e far-clip
    tintstalker-kind 3 figure-in  $B 4 figure-in  yield  3 figure-on  4 figure-on  fiona false shown
    $B before-reflections-layer layer  tintstalker-kind usual-layer layer
    7 scenes:keyed-movie 6 load-scene drop  $2D $41 movie-key  panic-eased  azoth-msg run-scene
    3 figure-out  4 figure-out  replay-over ;

\ ---- every frame ----
create cameras  1 , 0 , 0 ,  2 , 0 , 0 ,  3 , 1 , -1 ,  4 , 1 , -1 ,   \ (area, set, path)
: shade ( -- )
    final-on @ 0= if  17.4e 630e 89.8e  -59.9e 630e 69e  17.4e 580e 89.8e  -59.9e 580e 69e  light-doorway  then
    -51.2e 630e 75.7e  -91.2e 630e 6.5e  -51.2e 580e 75.7e  -91.2e 580e 6.5e  light-doorway
    -23.7e 617e -26.3e  -33.3e 617e 12.9e  -55.3e 617e -57.8e  -74.5e 617e 29e  light-doorway
    6e 617e -35e  -29.8e 617e -19.9e  15.6e 617e -78.5e  -66.5e 617e -44.5e  light-doorway
    27.8e 617e -23.1e  6e 617e -35e  66.5e 617e -44.4e  15.6e 617e -78.5e  light-doorway
    7e 617e 40.4e  35.1e 617e 6.9e  15.6e 617e 78.5e  78.5e 617e 15.6e  light-doorway
    35.1e 617e 6.9e  27.8e 617e -23e  78.5e 617e 15.6e  66.5e 617e -44.4e  light-doorway ;
: each-frame ( -- )
    fg-3-flag3 fact?  c7-second-event fact? and  wt8f-1-flag2 fact? 0= and if
        fiona 0 entered? if  wt8f-1-flag2 fact!  panic-eased  ['] azoth-scene fiona start-holding drop  then
    else  0 exit-out  then
    cameras 4 cameras-at  20000e far-clip
    holes-here  models  hewie-at-drop
    hewie-in?  hewie hole-floor-group on-group? and  hewie-in-hole !
    spots  shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act case  $13 of  ['] replay-lorenzo  endof  $14 of  ['] replay-final  endof  $15 of  ['] replay-flare  endof
            >r ['] replay-azoth r>  endcase
        fiona start-holding drop exit
    then
    0 0 0 way-in ;
: went ( -- )   \ (Hewie left on the holes' floor: out by the hole he went for - on its triangle, as he can be put once she's gone)
    hewie-in-hole @ 0= if  exit  then  hewie let-go
    this-room @  hole @ case  0 of  $33  endof  1 of  $38  endof  2 of  $4C  endof  endcase  hewie-way-now @  hewie-to ;

' came-in ' person-in ' each-frame ' went water-tower-8f-1 place!
' items-used water-tower-8f-1 items-here
