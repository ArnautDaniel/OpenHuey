\ subscreen/galleries.fs - the extras room's galleries (docs/subsystems/items.md; src/game/subscreen.c),
\ each opened by the room's script with its mode: the movies (8: a scene seen chosen to see again -
\ the room's script replays it, from the story's variables $2A / $2B), the models ($B: a figure
\ turned round under the camera, its motions played), the music ($D: a track played), the art
\ ($E: a picture looked over, moved and zoomed) and the TYPE ranks ($F: what each one means). Each
\ is a list of eight a page, "???" until the system data unlocks it (`resident-flag?`: the
\ original's +0x24 bits on). Their texts are the extras room's own messages; their art the
\ gallery's .TEX in the background group (the panels and their pictures); the thumbnails and
\ the pictures are put in the picture group as they are shown. The model is drawn alone (`alone`)
\ on the original's backdrop, with the menu's world held - it alone moves.
IN: subscreen.galleries
USING: engine game-state actors messages keys facts ps2-screen subscreen.state subscreen.art subscreen.input subscreen.box subscreen.gallery-tables ;

19 constant gallery-layer   \ (the UI layers: ... 18 the mods'; 19 this - the fades over the page)
: var@ ( n -- v )  progress pr.vars + c@ ;
: var! ( v n -- )  progress pr.vars + c! ;
: gal-line ( msg -- addr len )  0 0 line-of ;   \ (the extras room's own message, its first line)
$16E constant unknown-msg   \ "???"

\ ---- drawn: the texts in the original's three colours (0 plain, 1 marked, 2 the cursor's) ----
: ink-of ( c -- rgba )
    case  2 of  true ink  endof  1 of  $FF784000 openness @ 2* 255 min or  endof  >r false ink r>  endcase ;
variable gal-text-layer   word-layer gal-text-layer !   \ (the words' layer: over the page; under the box; over the bars)
: text-in ( addr len x y c -- )  {: a n x y c :}  gal-text-layer @ a n  x sx  y sy  c ink-of  scale  ui-text ;
create number-buf 8 allot
: number$ ( n width -- addr len )   \ (right-aligned in width: "%2d", "%3d")
    {: n w :}  n n>s {: a len :}  w len - 0 max {: pad :}
    number-buf pad 32 fill  a number-buf pad + len move  number-buf pad len + ;
: ps2-width ( addr len -- n )  scale text-width 512 * fw @ 1 max / ;   \ (Task_MessageWidth)
: heads ( msg -- )  $F gal-line $30 $3B 0 text-in  gal-line $58 $3B 0 text-in ;
: row-y ( k -- y )  8 mod 35 * $5E + ;
: row ( k addr len colour width -- )   \ its number and its title
    {: k a n c w :}  k 1+ w number$ $30 k row-y c text-in  a n $58 k row-y c text-in ;
: help ( msg1 msg2 -- )   \ the controls' line (the second placed by the usual first's width)
    $12 gal-line ps2-width $56 + {: m1 m2 x :}   \ (one message at a time: line-of's text is its own buffer)
    m1 gal-line $46 $186 0 text-in  m2 gal-line x $186 0 text-in ;
: page-of ( g n -- )  of$ $186 $176 0 text-in ;
: arrows ( -- )  $168 $170 $1A blink part  $1B3 $170 $1B blink part ;
: page-black ( -- )  art-layer 0 0 screen-size  openness @ 2* 255 min  ui-rect ;   \ (the lists: nothing of the world, the frame's sides black)
: list-colour ( k -- c )  gal-at @ = if  2  else  0  then ;
\ the fade over it all (sub_fade_back: the overlay darkening as gal-fade goes to $80)
: overlay ( -- )  gal-fade @ {: a :}  a if  gallery-layer 0 0 screen-size  a 2* 255 min  ui-rect  then ;
: bars ( title-msg -- )   \ the name and the help, in bars at the top and the foot (the words over them)
    gallery-layer 0 0 screen-size drop  $38 sy  $80 ui-rect
    gallery-layer 0 $188 sy  screen-size drop  $38 sy  $80 ui-rect
    gallery-layer gal-text-layer !
    gal-line $20 $10 1 text-in  $9E gal-line $20 $198 0 text-in
    word-layer gal-text-layer ! ;

\ ---- the cursor through groups of 8 of n (SubScreen_StateExtras, Gallery_StateMusic ...): up and
\ down round its group, left / L1 and right / R1 to the group before / after's first ----
: group-start ( k -- k' )  8 / 8 * ;
: last-start ( n -- k )  1- group-start ;
: list-moved? ( n -- flag )
    {: n | k :}  gal-at @ to k
    up? if  k 8 mod if  k 1-  else  k 7 +  then  n 1- min  gal-at !  true exit  then
    down? if  k 1+ 8 mod 0= if  k group-start  else  k 1+  then  dup n < 0= if  drop k group-start  then  gal-at !  true exit  then
    left? prev-page? or if  k group-start ?dup if  8 -  else  n last-start  then  gal-at !  true exit  then
    right? next-page? or if  k group-start 8 +  dup n < 0= if  drop 0  then  gal-at !  true exit  then
    false ;

\ ---- the movies (mode 8, SubScreen_StateMovies): one seen picked - the story's $2A, the cursor
\ in $2B for next time - and the menu closes for the room to replay it ----
$2A constant movie-var   $2B constant cursor-var
: movie-seen? ( k -- flag )   \ (or all of them: +0x24 bit 21)
    21 resident-flag? if  drop true exit  then  movie-flags + c@ resident-flag? ;
: movie-title ( k -- addr len )  dup movie-seen? if  $100 +  else  drop unknown-msg  then  gal-line ;
: movie-row ( k -- )  dup #movies < if  dup >r  dup movie-title  r> list-colour  3 row  else  drop  then ;
: thumbnail-in ( -- )   \ (the cursor's picture, loaded as it comes to one)
    gal-at @ gal-shown @ = if  exit  then  gal-at @ gal-shown !
    gal-at @ movie-seen? if  gal-at @ picture-group thumbnail drop  then ;
: movies-view ( -- )
    page-black
    $B page-art  $10 heads
    gal-at @ group-start {: base :}
    8 0 do  gal-at @ group-start i + movie-row  loop
    $12 $13 help  base 8 / 1+ $E page-of  arrows
    thumbnail-in
    $158 $B0 $70 $60 picture
    gal-at @ movie-seen? if  picture-group 0 0 0 0 $70 $60 $80 from
    else  back-group 2 1 $10 0 $70 $60 $80 from  then
    $140 $80 $10 $C0 picture  back-group 2 0 0 0 $10 $C0 $80 from ;
: movies-keys ( -- )
    #movies list-moved? if  se-cursor exit  then
    back? if  gal-at @ cursor-var var!  $FF movie-var var!  close-menu exit  then
    confirm? if
        gal-at @ movie-seen? if
            gal-at @ cursor-var var!  gal-at @ movie-var var!  se-decide  -1 quiet !  close-menu
        else  se-buzzer  then
    then ;

\ ---- the music (mode $D, Gallery_StateMusic): a track picked - the music stopped, and once the
\ stream is free the track started; the triangle stops the one playing, or (none) leaves ----
: track ( k -- a )  3 * cells tracks + ;
: track-open? ( k -- flag )  track @ resident-flag? ;
: want ( track -- )  0 1e f>cell  s" music" actor-named send bgm-want ;
: track-title ( k -- addr len )  dup track-open? if  track cell+ @  else  drop unknown-msg  then  gal-line ;
: track-colour ( k -- c )  dup gal-at @ = if  drop 2  else  gal-playing @ = if  1  else  0  then  then ;
: track-row ( k -- )  dup #tracks < if  dup >r  dup track-title  r> track-colour  2 row  else  drop  then ;
: stream-free? ( -- flag )  music-playing? 0=  gal-wait @ 0= and ;
: music-view ( -- )
    page-black
    $D page-art  $10 heads
    gal-at @ group-start {: base :}
    8 0 do  gal-at @ group-start i + track-row  loop
    $1C8  gal-playing @ $FF = if  $13  else  $1C7  then  help
    base 8 / 1+ 4 page-of  arrows ;
: music-keys ( -- )
    gal-wait @ if  -1 gal-wait +!  then
    confirm? if
        gal-at @ track-open? if  $FF want  gal-at @ dup gal-playing !  gal-next !  8 gal-wait !
        else  se-buzzer  then
    else  #tracks list-moved? if  se-cursor
    else  back? if
        music-playing? if  $FF want  $FF gal-next !  8 gal-wait !  else  close-menu exit  then
    then  then  then
    stream-free? if
        gal-next @ $FF <> if  gal-next @ track 2 cells + @ want  $FF gal-next !  8 gal-wait !
        else  $FF gal-playing !  then
    then ;

\ ---- the art (mode $E, Gallery_StateArtList / StateArtView): a picture unlocked opened - the list
\ darkens, the picture is unpacked (art-picture: SUBSCR/ARTnn.LEN), it fades in; the arrows move
\ it a pixel, WASD (the stick) eight, F (R3) zooms it to full size, the circle centres it (L3: none
\ on the keyboard), the cross turns the moves round, select the bars, start the help ----
: picture-open? ( k -- flag )  cells picture-flags + @ resident-flag? ;
: picture-title ( k -- addr len )  dup picture-open? if  $190 +  else  drop unknown-msg  then  gal-line ;
: picture-row ( k -- )  dup #pictures < if  dup >r  dup picture-title  r> list-colour  2 row  else  drop  then ;
: pictures-view ( -- )
    page-black
    $E page-art  $10 heads
    gal-at @ group-start {: base :}
    8 0 do  gal-at @ group-start i + picture-row  loop
    $12 $13 help  base 8 / 1+ 6 page-of  arrows ;
: pictures-keys ( -- )
    gal-step @ 0= if   \ (the list)
        #pictures list-moved? if  se-cursor exit  then
        back? if  close-menu exit  then
        confirm? if  gal-at @ picture-open? if  1 gal-step !  se-decide  else  se-buzzer  then  then
        exit
    then
    gal-step @ 1 = if   \ (darkening; then unpacked, fading in)
        16 gal-fade +!  gal-fade @ $80 < if  exit  then  $80 gal-fade !
        gal-at @ picture-group art-picture if  art-h !  else  drop 0 art-h !  then
        $100 art-x !  $E0 art-y !  0 art-zoom !  0 art-turn !  0 art-help !  2 gal-step !  exit
    then
    gal-step @ 4 = if   \ (leaving: dark, then the list again)
        16 gal-fade +!  gal-fade @ $80 < if  exit  then  $80 gal-fade !  5 gal-step !  exit
    then
    gal-step @ 5 = if  -16 gal-fade +!  gal-fade @ 0> if  exit  then  0 gal-fade !  0 gal-step !  exit  then
    \ (shown: fading in, the help's shade, then the keys)
    art-help @ if  $60  else  0  then {: goal :}
    gal-fade @ goal > if  -16 gal-fade +!  gal-fade @ goal max gal-fade !  exit  then
    gal-fade @ goal < if  16 gal-fade +!  gal-fade @ goal min gal-fade !  exit  then
    select-button button-pressed? if  gal-bars @ 0= gal-bars !  se-decide exit  then
    start-button button-pressed? if  art-help @ 0= art-help !  se-decide exit  then
    art-help @ if  exit  then
    back? if  4 gal-step !  se-cancel exit  then
    confirm? if  art-turn @ 0= art-turn !  se-decide  then
    art-turn @ if  -1  else  1  then {: d :}
    key: Up held? if  d negate art-y +!  then   key: Down held? if  d art-y +!  then
    key: Left held? if  d negate art-x +!  then  key: Right held? if  d art-x +!  then
    key: A held? if  d -8 * art-x +!  then  key: D held? if  d 8 * art-x +!  then
    key: W held? if  d -8 * art-y +!  then  key: S held? if  d 8 * art-y +!  then
    circle button-pressed? if  $100 art-x !  $E0 art-y !  se-decide  then
    key: F pressed? if   \ (R3: about the middle)
        art-zoom @ 0= art-zoom !
        art-zoom @ if  art-x @ $100 - 2* $100 +  art-y @ $E0 - 2* $E0 +
        else  art-x @ $100 - 2/ $100 +  art-y @ $E0 - 2/ $E0 +  then  art-y !  art-x !  se-decide
    then
    art-zoom @ if  art-x @ 0 $200 clamp art-x !  art-y @ 0 $1C0 clamp art-y !
    else  art-x @ $80 $180 clamp art-x !  art-y @ $70 $150 clamp art-y !  then ;
: picture-view ( -- )   \ (the picture, its half or whole size about art-x / art-y; black round the frame)
    art-h @ 0= if  exit  then
    art-zoom @ if  $100 art-h @ 2/  else  $80 art-h @ 4 /  then {: hw hh :}
    art-x @ hw -  art-y @ hh -  hw 2*  hh 2*  picture  picture-group 0 0 0 0 $200 art-h @ $80 from
    screen-size {: w h :}  0 sx {: left :}   \ (on a wider window, black either side of the frame)
    gallery-layer 0 0 left h $FF ui-rect  gallery-layer left fw @ + 0 w left - fw @ - h $FF ui-rect
    art-help @ gal-fade @ $60 = and if  $1C5 gal-line $20 $20 0 text-in  then ;
: art-view ( -- )   \ (the list - under the dark - or the picture)
    gal-step @ 2 5 within if  art-layer 0 0 screen-size $FF ui-rect  picture-view  else  pictures-view  then
    overlay  gal-step @ 2 5 within gal-bars @ and if  gal-at @ $190 + bars  then ;

\ ---- the TYPE ranks (mode $F, SubScreen_StateEntries): two groups (0..7, 8..10); the cross shows
\ the entry's message in the box, then - unlocked - the one after it ----
: type-of ( k -- a )  4 * cells types + ;
: type-open? ( k -- flag )  type-of @ resident-flag? ;
: type-title ( k -- addr len )  dup type-open? if  type-of cell+ @  else  drop unknown-msg  then  gal-line ;
: types-view ( -- )
    page-black
    mode @ on-box = if  art-layer gal-text-layer !  then   \ (the box up: the list under its shade)
    $F page-art  $2B heads
    gal-at @ 8 < if  0  else  1  then {: g :}
    g 2* cells type-groups + 2@ ?do  i  i type-title  i list-colour  2 row  loop
    g 1+ 2 page-of
    mode @ on-box = if  word-layer gal-text-layer !  exit  then
    $12 $13 help  arrows ;
: back-to-types ( picked -- )  drop  on-gallery mode ! ;
: after-type ( picked -- )   \ (unlocked, and a message to follow: that one too)
    drop  gal-at @ type-of {: e :}
    e @ resident-flag?  e 3 cells + @ $FFFF <> and if  e 3 cells + @ ['] back-to-types ask  else  on-gallery mode !  then ;
: types-keys ( -- )
    gal-at @ {: k :}
    confirm? if  k type-of 2 cells + @ ['] after-type ask  se-decide exit  then
    up? if  k 0= if  7  else  k 8 = if  10  else  k 1-  then  then  gal-at !  se-cursor exit  then
    down? if  k 7 = if  0  else  k 10 = if  8  else  k 1+  then  then  gal-at !  se-cursor exit  then
    left? right? or prev-page? or next-page? or if  k 8 < if  8  else  0  then  gal-at !  se-cursor exit  then
    back? if  close-menu  then ;

\ ---- the models (mode $B, SubScreen_StateExtras / Gallery_StateModel): a figure unlocked chosen -
\ the list darkens, the figure is loaded (its model, textures, markers; its kind's motion table)
\ and shown alone on the backdrop in two lights, fading in; the cross plays its motion again, R1 /
\ L1 the next / previous of its motions, W / S take the camera nearer or further, A / D round,
\ I / K up and down (the sticks), select the bars, start the help; the triangle back ----
: one-of? ( k a n -- flag )   \ (k among the n cells at a)
    0 ?do  2dup i cells + @ = if  2drop true unloop exit  then  loop  2drop false ;
create hard-ones  10 , 12 , 14 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 ,   \ (+0x2C bit 22: flag 86)
create third-ones  16 , 18 , 20 , 22 ,                                     \ (bit 25: flag 89)
create second-ones  15 , 17 , 19 , 21 , 23 ,                               \ (bit 21: flag 85)
: model-open? ( k -- flag )   \ (SubScreen_ExtraUnlocked: the system data's +0x24 / +0x2C bits)
    dup 3 = if  drop 1 resident-flag? exit  then
    dup 5 = if  drop 3 resident-flag? exit  then
    dup 8 = if  drop 86 resident-flag?  8 resident-flag? 12 resident-flag? or  and exit  then
    dup hard-ones 11 one-of? if  drop 86 resident-flag? exit  then
    dup third-ones 4 one-of? if  drop 89 resident-flag? exit  then
    second-ones 5 one-of? if  85 resident-flag? exit  then
    0 resident-flag? ;
: model-title ( k -- addr len )  dup model-open? if  $70 +  else  drop unknown-msg  then  gal-line ;
: model-row ( k -- )  dup #models < if  dup >r  dup model-title  r> list-colour  2 row  else  drop  then ;
: models-view ( -- )
    page-black
    $B page-art  $10 heads
    gal-at @ group-start {: base :}
    8 0 do  gal-at @ group-start i + model-row  loop
    $12 $13 help  base 8 / 1+ 4 page-of  arrows ;
: motions ( -- a )  gal-at @ cells model-motion-at + @ cells model-motions + ;   \ (its count, then its motions)
: motion-of ( i -- id )  1+ cells motions + @ ;
: kind-table ( k -- addr len | 0 0 )   \ (Gallery_MakeModel's classes: their motion tables)
    {: k :}
    k 6 < if  s" motions-fiona" exit  then
    k 9 < if  s" motions-hewie" exit  then
    k 11 < if  s" motions-debilitas" exit  then
    k 13 < if  s" motions-daniella" exit  then
    k 15 < if  s" motions-riccardo" exit  then
    k 17 < if  s" motions-tintstalker" exit  then
    k 19 < if  s" motions-lorenzo" exit  then
    k 21 23 within if  s" motions-lorenzo2" exit  then
    k 23 = if  s" motions-kind12" exit  then
    k 25 = if  s" motions-homunculus" exit  then
    k 27 = if  s" motions-crow" exit  then
    0 0 ;
: play ( i -- )   \ (the entry's motion i: from its table, or - past $19 - blended in)
    gal-model @ swap motion-of
    gal-at @ $18 < gal-at @ $19 = or if  2dup motion-entry nip  else  5 1  then
    dup gal-loops !  motion-play ;
: model-step ( -- )   \ (Gallery_ModelMotions: one not looping, at its end, the first again)
    gal-at @ 5 = if   \ (the fifth's: $800, $801, $802 in turn)
        gal-model @ actor act.mflags l@ $20 and if
            gal-model @ motion@ case  $800 of  $801  endof  $801 of  $802  endof  >r -1 r>  endcase
            dup 0< 0= if  gal-model @ swap 0 0 motion-play exit  then  drop
        then
    then
    gal-model @ actor act.mflags l@ $20 and  gal-loops @ 1 and 0= and if  0 play  then ;
: model-files$ ( -- pck n tex n tex2 n mrk n )  gal-at @ model-files ;
: model-in ( -- )   \ (Gallery_StateModelChosen / StateModelLoad)
    model-files$ {: pa pn ta tn t2a t2n ma mn :}
    pa pn ta tn actor-load-textured dup gal-model !
    mn if  dup ma mn actor-markers  then
    gal-at @ kind-table ?dup if  motion-table  else  2drop  then
    1 gal-model @ actor act.visible l!
    0e gal-model @ actor act.x sf!  0e gal-model @ actor act.y sf!  0e gal-model @ actor act.z sf!  0e gal-model @ actor act.yaw sf!
    gal-model @ alone  cam-keep  look-keep look-plain   \ (the room's fog, tint and bloom: none - the original's own tint, plain)
    gal-at @ floats model-nearest + f@ 10e f+ gal-dist f!  0e gal-turn f!
    gal-at @ 3 * floats model-heights + float+ f@ gal-height f!
    0 gal-motion !  0 play
    1 stage-lights  55e 55e 50e stage-ambient   \ (Gallery_StateModelChosen: one everywhere, one from the camera's way - model-camera)
    0.19e 0.35e 0.48e clear-color ;   \ (the backdrop: $5C503C at the PS2's half - 120 160 184 - in linear light)
: model-out ( -- )   \ (gallery_leave: the figure gone, the camera, lights and background back)
    gal-model @ dup 0< if  drop exit  then  actor-free  -1 gal-model !
    -1 alone  cam-kept  look-kept  0 stage-lights  0.06e 0.06e 0.08e clear-color ;
: model-camera ( -- )   \ (Gallery_ModelCamera: the sticks; then the camera round the figure)
    gal-at @ floats model-nearest + f@ {: F: near :}
    key: W held? if  gal-dist f@ 1e f- near fmax gal-dist f!  then
    key: S held? if  gal-dist f@ 1e f+ 60e fmin gal-dist f!  then
    key: A held? if  gal-turn f@ 0.0873e f- gal-turn f!  then
    key: D held? if  gal-turn f@ 0.0873e f+ gal-turn f!  then
    gal-turn f@ pi f> if  gal-turn f@ 2e pi f* f- gal-turn f!  then
    gal-turn f@ pi fnegate f< if  gal-turn f@ 2e pi f* f+ gal-turn f!  then
    gal-at @ 3 * floats model-heights + {: hs :}
    key: I held? if  gal-height f@ 0.5e f+ hs 2 floats + f@ fmin gal-height f!  then
    key: K held? if  gal-height f@ 0.5e f- hs f@ fmax gal-height f!  then
    gal-turn f@ fsin gal-dist f@ f*  gal-height f@  gal-turn f@ fcos gal-dist f@ f*
    0e gal-height f@ 0e  cam-place
    \ the light: from the camera's way turned 15 degrees up and 30 round (the original's 0x4C), far off
    gal-turn f@ 30e deg>rad f+ {: F: a :}  15e deg>rad {: F: up :}
    a fsin up fcos f* 1e5 f*  up fsin 1e5 f*  a fcos up fcos f* 1e5 f*  65e 60e 60e 0e  0 stage-light ;
: model-keys ( -- )   \ (the figure shown: Gallery_StateModel)
    gal-model @ 0< if  exit  then
    model-step
    gal-step @ case
        3 of  -32 gal-fade +!  gal-fade @ 0> 0= if  0 gal-fade !  4 gal-step !  then  endof
        5 of  16 gal-fade +!  gal-fade @ $60 < 0= if  $60 gal-fade !  6 gal-step !  then  endof
        6 of  start-button button-pressed? if  7 gal-step !  se-decide  then  endof
        7 of  -16 gal-fade +!  gal-fade @ 0> 0= if  0 gal-fade !  4 gal-step !  then  endof
        8 of  32 gal-fade +!  gal-fade @ $80 < 0= if  $80 gal-fade !  model-out  9 gal-step !  then  endof
        4 of
            back? if  8 gal-step !  se-cancel
            else  start-button button-pressed? if  5 gal-step !  se-decide  then  then  endof
    endcase
    gal-model @ 0< if  exit  then
    confirm? if  gal-motion @ play
    else  next-page? if  gal-motion @ 1+ dup motions @ < 0= if  drop 0  then  dup gal-motion !  play
    else  prev-page? if  gal-motion @ 1- dup 0< if  drop motions @ 1-  then  dup gal-motion !  play
    else  select-button button-pressed? if  gal-bars @ 0= gal-bars !  se-decide
    else  model-camera  then  then  then  then ;
: models-keys ( -- )
    gal-step @ case
        0 of   \ (the list)
            #models list-moved? if  se-cursor exit  then
            back? if  close-menu exit  then
            confirm? if  gal-at @ model-open? if  1 gal-step !  se-decide  else  se-buzzer  then  then  endof
        1 of  16 gal-fade +!  gal-fade @ $80 < 0= if  $80 gal-fade !  model-in  3 gal-step !  then  endof
        9 of  -32 gal-fade +!  gal-fade @ 0> 0= if  0 gal-fade !  0 gal-step !  then  endof   \ (back: the list fading in)
        >r model-keys r>
    endcase ;
: models-shown? ( -- flag )  gal-model @ 0< 0= ;
: model-view ( -- )
    models-shown? 0= if  models-view overlay exit  then
    overlay
    gal-step @ 6 = if  $1C4 gal-line $20 $20 0 text-in  then
    gal-bars @ if  gal-at @ $70 + bars  gal-motion @ 1+ motions @ of$ $186 $198 0 text-in  then ;

\ ---- the galleries' modes: opened, their keys, drawn, closed ----
: gallery-file ( g -- addr len )
    case  gal-movies of  s" SUBSCR/GAL_MOVIE"  endof  gal-models of  s" SUBSCR/GAL_MODEL"  endof
          gal-music of  s" SUBSCR/GAL_MUSIC"  endof  gal-art of  s" SUBSCR/GAL_ART"  endof  >r s" SUBSCR/GAL_TYPE" r>  endcase ;
: start-gallery ( g -- )
    gal-kind !  on-gallery mode !
    0 gal-at !  0 gal-fade !  0 gal-step !  -1 gal-shown !  -1 gal-model !  0 gal-wait !  0 art-h !
    $FF gal-playing !  $FF gal-next !
    gal-kind @ gal-art = if  -1 gal-bars !  else  0 gal-bars !  then
    gal-kind @ gal-models = if  -1 gal-bars !  then
    gal-kind @ gal-movies = if  cursor-var var@ #movies 1- min gal-at !  then ;
: gallery-keys ( -- )
    gal-kind @ case
        gal-movies of  movies-keys  endof  gal-models of  models-keys  endof
        gal-music of  music-keys  endof  gal-art of  pictures-keys  endof  gal-types of  types-keys  endof
    endcase ;
: gallery-view ( -- )
    gallery-layer ui-clear
    gal-kind @ case
        gal-movies of  movies-view  endof  gal-models of  model-view  endof
        gal-music of  music-view  endof  gal-art of  art-view  endof  gal-types of  types-view  endof
    endcase ;
: gallery-shut ( -- )   \ (the menu closing: what a gallery left out put away)
    model-out  gallery-layer ui-clear  0 gal-kind ! ;
