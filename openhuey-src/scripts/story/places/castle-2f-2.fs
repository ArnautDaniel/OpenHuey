\ story/places/castle-2f-2.fs - the chandelier hall in Belli Castle's 2F ($20): her first steps by the
\ chandeliers (Debilitas's scene: he comes in, looks about), the box barring the way (pushed home: the
\ door beyond unlocked); the Bisque Doll knocked down by her blow, fallen and picked up; after the
\ telephone's warning, the door that won't open (inside, she is seized - the forced way; pulled free
\ she flees here: the file); the blood (the first time a fright), the floor, the angel painting, the
\ dark corner; the Album Medallion Hewie sniffs out, what he stands wary of, sits by and looks up at;
\ the chandeliers swinging, two flames, the wheel's sounds, the ticking beyond the door, Debilitas's
\ shove far off, the creatures, the walls' shade. A place (story/kit.fs). The original's scripts,
\ for checking: story/rooms-ref/castle-2f-2.fs (converted from the game's, not loaded).
IN: story.places.castle-2f-2
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects vectors story.kit fiona.anims fiona.sounds stalker.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant must-flee-msg   \ "I-I have to get out of here!"
1 constant presence-msg    \ "I still sense a presence inside the room. Best to trust my instincts and not go in."
2 constant blood-msg       \ "Is this... blood?"
3 constant nothing-msg     \ "Nothing unusual..."
4 constant angel-msg       \ "A woman with wings protruding from her back. An angel, perhaps?"
5 constant why-blood-msg   \ "Why is there blood here?"
6 constant box-msg         \ "This box is barring my path. I can't get in. I'll bet I could somehow push it out of th..."
7 constant dark-msg        \ "It's dark. I can't see a thing."

\ ---- its own state (a visit) ----
own: fled                  \ she fled out of the room beyond (pulled free): "I have to get out"
own: doll-down             \ the doll lying on open floor (to be knocked, or picked up)
own: doll-falling          \ ... falling
own: first-steps           \ her first steps by the chandeliers made (once a visit)
own: his-shove             \ Debilitas shoving the rubble far off (at the room's end taken)
own: tick-chosen  own: tick-count   \ the ticking beyond the door: how many, in threes
own: wary-noticed  own: sits-noticed   \ (Hewie: once a visit each)

\ ---- the room's things ----
0 constant flame-1-fx   1 constant flame-2-fx   2 constant album-fx
3 constant chandelier   4 constant chandelier-2   5 constant doll-object
2 constant her-scene-motions   6 constant his-shove-motions
$11 constant door-beyond   $12 constant box-door   2 constant beyond-exit
$11 constant blood-area   $12 constant floor-area   $13 constant angel-area   $16 constant box-area
$17 constant chandelier-area   $18 constant dark-area   $19 constant dark-2-area   $1B constant far-area
9 constant beyond-way-area   $A constant back-way-area
1 constant wheel-sound   2 constant wheel-start-sound   3 constant tick-sound   4 constant doll-sound
1 constant his-movie   0 constant his-cutscene   3 constant figure-slot
$209 constant fled-file   $224 constant his-file   $2302 constant look-round-anim
: warned? ( -- flag )  c1f-8-flag1 fact?  found-file-209 fact? 0= and ;   \ (the telephone's warning heeded not yet)
: first-time? ( -- flag )  c2f-2-flag1 fact? 0= ;
: box-home? ( -- flag )  c2f-2-flag2 fact? ;
: tinted ( -- )  c2f-3-flag1 fact? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then ;
: the-figures ( -- )  daniella-kind 3 figure-in  riccardo-kind 4 figure-in ;   \ (for the room beyond: its scene)
: no-figures ( -- )  3 figure-out  4 figure-out ;
: box-placed ( -- )   \ (in its place, or pushed home and fixed there)
    box-home? if  0 0 0 $377 $3BB obstacle-in  0 obstacle-fixed  else  0 0 0 $36B $3AF obstacle-in  then ;
: flames-lit ( -- )  flame-1-fx 0 23.3e 12.75e 87e flame-at  flame-2-fx 0 25.15e 12.75e 86e flame-at ;

\ ---- the chandeliers: swinging 15 degrees, their phase on 2 degrees a frame (the wheel's sound
\ each turn of the first) ----
: chandelier-still ( k -- )  this-room @ story.strings:room-string 0e swing-phase object! ;
: chandelier-swings ( k -- )
    this-room @ story.strings:room-string {: a n :}  a n placed-here? 0= if  exit  then
    2e a n swing-phase object+!
    a n swing-phase object@ 360e f< 0= if  -360e a n swing-phase object+!  wheel-sound room-bank 0 0  -85e 30e 90e sound-at  then
    a n swing-phase object@ deg>rad fsin 15e f* deg>rad  fdup pi f> if  2e pi f* f-  then  a n turn-y object! ;
: swinging ( -- )  begin  chandelier chandelier-swings  chandelier-2 chandelier-swings  yield  again ;

\ ---- the Bisque Doll ("dool"): knocked by her blow, it falls along the floor till it lands ----
: dool ( -- addr len )  s" dool" ;
$30 constant doll-vx   $34 constant doll-vy   $38 constant doll-vz   $3C constant doll-tri   \ (its push; its triangle: $80000000 landed)
$80000000 constant landed-bit   $20020008 constant doll-blocked
create doll-pt /vec allot
: doll-at ( -- v )  dool place-x object@  dool place-y object@  dool place-z object@  doll-pt vec!  doll-pt ;
: open-floor? ( tri -- flag )  dup 0< if  drop false exit  then  nav-flags doll-blocked and 0= ;
: doll-up ( -- )   \ (back up in its place, shown)
    dool placed-here? 0= if  exit  then  doll-object object-show
    9.66e dool place-x object!  0.51e dool place-y object!  93.4e dool place-z object!
    1.57e dool turn-x object!  0.79e dool turn-y object!  -3.14e dool turn-z object! ;
: doll-lands ( tri -- )   \ (still now; on open floor it may be had; a puff of dust unless on a soft floor)
    0 doll-falling !  0e dool doll-vx object!  0e dool doll-vy object!  0e dool doll-vz object!
    dup open-floor? 0= if  0 doll-down !  then
    dup 0< 0= if  dup nav-flags $2018000 and $10000 <> if  2 $50 $50 $50 doll-at vec@ dust-of  then  then
    landed-bit or dool doll-tri placed-l!  -1 doll-down ! ;
: doll-falls ( -- )   \ (gravity 0.2 a frame, along the floor as it slides, spinning a degree a frame)
    dool doll-tri placed-l@ dup -1 = if  drop exit  then
    dup landed-bit and if  $7FFFFFFF and open-floor? 0= if  0 doll-down !  then  exit  then  drop
    -0.2e dool doll-vy object+!  0 doll-down !
    dool place-y object@ dool doll-vy object@ f+
    dool doll-vx object@ dool doll-vz object@  doll-at doll-blocked v-nav-move
    doll-pt vec@ dool place-z object! dool place-y object! dool place-x object!
    dup dool doll-tri placed-l!
    dup -1 = if  drop fdrop exit  then
    fdup doll-pt v.y sf@ f> if
        drop dool place-y object!  1e deg>rad dool turn-y object+!
        dool turn-y object@ pi f> if  -2e pi f* dool turn-y object+!  then  exit
    then  fdrop  doll-lands ;
: falling ( -- )  c2f-2-flag5 fact!  begin  doll-falls  yield  again ;
: knocked? ( -- flag )   \ (her blow within 5 of it: a push (0, 1, 1) turned by her heading)
    dool placed-here? 0= if  false exit  then
    fiona body-point 0= if  false exit  then  doll-at vec-dist 5e f> if  false exit  then
    fiona body-yaw  fdup fsin dool doll-vx object!  1e dool doll-vy object!  fcos dool doll-vz object!
    doll-at v-tri dool doll-tri placed-l!  true ;
: doll-knocks ( -- )
    got-bisque-doll fact?  doll-down @ 0= or if  exit  then
    knocked? 0= if  exit  then
    doll-sound room-bank fiona sound-by  0 doll-down !  0 doll-falling !
    ['] falling running? 0= if  ['] falling start drop  then ;
: pick-doll ( -- )
    fiona wait-move  dool place-x object@ dool place-z object@ fiona turn-to-xz
    fiona pick-up-anim play  4 frames
    bisque-doll item-named  bisque-doll item-full? if  cant-carry-msg say
    else  bisque-doll 1 give-item  got-bisque-doll fact!  doll-object object-hide  bisque-doll show-item-tab  fanfare  obtained-msg say  then
    fiona up-from-pick-anim play ;

\ ---- as she comes in ----
: wheel-starts ( -- )   \ (till her first steps by the chandeliers: the wheel's start every 180 frames)
    180  begin  yield  room-sounds-in? if  dup 180 = if  drop 0  wheel-start-sound -82e 0e 92e room-sound  else  1+  then  then  again ;
: ticking ( -- )   \ (beyond the door, after the warning: a tick now and then - one, two or three)
    begin
        tick-chosen @ 0= if
            -1 tick-chosen !  30 chance? if  31  else  50 chance? if  61  else  1  then  then  tick-count !
        then
        yield  -1 tick-count +!
        tick-count @ dup 0= over 30 = or over 60 = or swap 90 = or if
            tick-sound -145e 10e 150e room-sound
            tick-count @ 0= if  0 tick-chosen !  then
            30 chance? if  30  else  50 chance? if  45  else  90  then  then  frames
        then
    again ;
: came-in ( exit -- )
    {: ex :}  room-sounds
    first-time? if  ['] wheel-starts start drop  then
    box-placed  flames-lit
    c2f-2-album-medallion-found fact?  c2f-2-album-medallion fact? 0= and if  album-fx -11.6e 1e 142.2e flicker-at  then
    got-lunar-refractor fact?  c2f-2-flag3 fact? 0= and if
        c2f-2-flag3 fact!  castle-2f-2 0 316 $D 15 creature-in  castle-2f-2 0 361 4 15 creature-in
    then
    tinted  chandelier chandelier-still  chandelier-2 chandelier-still  ['] swinging start drop
    doll-object object-hide
    got-bisque-doll fact? 0= if
        0 doll-down !
        c2f-2-flag1 fact?  c2f-2-flag5 fact? 0= and if  doll-up  -1 doll-down !  -1 doll-falling !  then
    then
    warned?  ex forced-exit <> and if  ['] ticking start drop  then ;

\ ---- her first steps by the chandeliers: Debilitas's scene - he comes in and looks about ----
: looks-about ( -- )   \ (his look round, then about now and then - till the camera leaves its fourth set, or 450 frames)
    stalker hold  stalker fiona look-at  yield  stalker look-round-anim 10 play-blend
    0  begin
        25 chance? if  stalker look-about-anim 10 play-blend  else  25 chance? if  stalker look-about-3-anim 10 play-blend  then  then
        cam-set@ 4 <>  over 450 >= or 0= while  1+  yield
    repeat  drop ;
: him-in ( tri -- )  partner dup 0< if  2drop exit  then  >r castle-2f-2 swap r> send stalker-in ;
: his-scene ( -- )
    fiona hold  ['] wheel-starts stop-doing
    15 screen:to-black-2 with-music or fade  wait-fade
    552 him-in  yield  stalker let-go  0 summon-take
    0 true no-shadow  partner-kind true no-shadow
    his-movie scenes:unseen-4-movie his-cutscene load-scene
    panic-eased  if  -1 run-scene  then
    world-held state!  0 false no-shadow  partner-kind false no-shadow
    fiona her-scene-motions motions-for  fiona use-motions
    got-bisque-doll fact? 0= if  doll-up  -1 doll-down !  then
    $30A him-in  yield  stalker $30A 179 -7.812e 78.949e place-at  stalker activate  0 stalker-order
    stalker 3 3 camera-set  ['] looks-about start drop
    fiona $2E9 -10 -8.354e 49.017e place-at  4 4 player-camera  camera-restart  force-calm state-off
    fiona room-motion 1 + 0 start-blend  c2f-2-flag1 fact!  world-held state-off  5 0 0 stage-music
    15 screen:from-black music-cut or fade  fiona room-motion 3 + play  his-own-time  wait-fade
    his-file give-file ;

\ ---- the door beyond (after the warning): it won't open - and inside she is seized; pulled free,
\ she flees back here ----
: door-beyond-tried ( -- )
    stalkers-stay state!  fiona beyond-exit to-door  fiona rattle-behind-anim play  fiona -1 idle  7 frames
    ['] ticking stop-doing
    fiona look-close-anim start-anim  22 frames
    15 screen:to-black-2 with-music or fade  wait-fade  world-held state!  yield
    fiona false shown  forced-exit take-exit ;
: pulled-free ( -- )   \ (brought back by the forced way: she flees, the file)
    fiona hold  fiona pull-free-anim 0 start-blend  found-file-209 fact!  world-held state-off
    0 0 player-camera  camera-restart  15 screen:from-black with-music or fade  wait-fade
    fled-file give-file  -1 fled !  stalkers-stay state-off  100 threat  fiona wait-anim ;
: shut-in-there ( -- )  fiona wait-move  fled @ if  must-flee-msg  else  presence-msg  then  say ;

\ ---- what she may do or say here ----
: blood ( -- )   \ (the first time: a fright)
    fiona wait-move
    c2f-2-flag4 fact? if  why-blood-msg say exit  then
    c2f-2-flag4 fact!  seized-sound common-bank fiona sound-by  fiona startled-anim start-anim
    $5A fright+  1 $FF 8 rumble-pad  16 frames  blood-msg show  fiona wait-anim  fiona -1 idle  7 frames  wait-closed ;
: the-floor ( -- )  fiona wait-move  fiona look-low-anim start-anim  40 frames  nothing-msg say  fiona wait-anim ;
: angel ( -- )
    fiona $292 -90 -1 walk-move -173e 90e go-to  1 20e -10e 0e 6e shot  16 frames
    fiona look-at-anim start-anim  55 frames  angel-msg say  fiona wait-anim  16 frames  shot-off ;
: the-box ( -- )  fiona wait-move  box-msg say ;
: dark-corner ( -- )
    fiona wait-move  fiona 0 turn  fiona look-at-anim start-anim  55 frames  dark-msg say  fiona wait-anim ;
: pick-album ( -- )  album-medallion c2f-2-album-medallion album-fx -11.6e 142.2e picks-up ;
: spots ( -- )
    fiona beyond-exit before-door? if
        warned?  stalker-in? 0= and if  ['] door-beyond-tried unhurried offer  then
        found-file-209 fact?  beyond-exit before-locked? and if  ['] shut-in-there any-time offer  then   \ (her try at its locked door)
    then
    fiona blood-area 45 faces-area? if  ['] blood unhurried offer  then
    fiona floor-area 45 faces-area? if  ['] the-floor unhurried offer  then
    fiona angel-area 45 faces-area? if  ['] angel unhurried offer  then
    box-home? 0=  fiona box-area in-area? and if  ['] the-box unhurried offer  then
    fiona dark-area in-area?  fiona dark-2-area in-area? or  fiona 0 50 heading? and if  ['] dark-corner unhurried offer  then
    c2f-2-album-medallion-found fact?  c2f-2-album-medallion fact? 0= and  fiona -11.6e 1e 142.2e 5e 5e touches? and if
        ['] pick-album calm-only offer  then
    got-bisque-doll fact? 0=  doll-down @ and  dool placed-here? and if
        fiona dool place-x object@ dool place-y object@ dool place-z object@ 5e 5e touches? if  ['] pick-doll unhurried offer  then
    then ;

\ ---- the box pushed home: fixed there, the door beyond it unlocked, castle-2f-8 open to the stalker ----
: box-pushed ( -- )
    box-home?  0 $377 obstacle-on-tri? 0= or if  exit  then
    c2f-2-flag2 fact!  0 obstacle-fixed  box-door unlock  castle-2f-8 stalker-slot false doors-into ;

\ ---- Debilitas's shove far off (on guard there): dust, her thrown if she is by him; then gone ----
: shoves-far ( -- )
    stalker hold  -1 his-shove !  stalker wait-move  stalker his-shove-motions motions-for
    stalker $1A2 0 -1 walk-path-move 77.27e -192.48e go-to
    stalker use-motions  stalker true noclip  stalker room-motion 5 start-blend8
    begin  stalker fiona 0 close-to? if  stalker throws-her  then  stalker went-round? 0= while  yield  repeat
    stalker false noclip  0 his-shove !  stalker let-go  0 summon-take ;
: shove-due ( -- )
    stalker far-area in-area?  4 stalker-alert? and  st-cond-now @ down-cond <> and  debilitas? and  stalker held? 0= and if
        ['] shoves-far start drop
    then ;

\ ---- the flames stirred as someone passes; the wheel's steady sounds ----
: draughts ( -- )  flame-1-fx 23.3e 12.75e 87e 8e -4e flame-stirred  flame-2-fx 25.15e 12.75e 86e 8e -4e flame-stirred ;
: sounds ( -- )
    room-sounds-in? 0= if  exit  then
    first-time? if  wheel-start-sound -82e 0e 92e keep-sound  then
    wheel-sound -85e 30e 90e keep-sound ;

\ ---- Hewie: wary by the far door (after the warning), sitting by the box pushed home, his looks
\ up, the Album Medallion he sniffs out ----
create looks
    -177.49e 0e 89.35e  18e 18e 18e look,
    55.23e 0e -184.17e  20e 14e 14e look,
    -84.16e 0e 90.89e  18e 28e 28e look,
: wary-by-door ( -- )  -144e 0e 109e hewie-stands-by ;
: sits-by-box ( -- )  33.656e 0e 111.061e hewie-sits-by ;
: at-his-command? ( -- flag )  hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and ;
: hewie-notices ( -- )
    at-his-command? 0= if  exit  then
    warned?  hewie 2 in-area? 0= and  wary-noticed @ 0= and  hewie -144e 0e 109e 30e 20e centred-in? and if
        -1 wary-noticed !  ['] wary-by-door start drop
    then
    box-home?  hewie 4 in-area? 0= and  sits-noticed @ 0= and  hewie 33.656e 0e 111.061e 10e 20e centred-in? and if
        -1 sits-noticed !  30 chance? if  ['] sits-by-box start drop  then
    then ;
: album-sniffed ( -- )  c2f-2-album-medallion-found c2f-2-album-medallion album-fx  -11.6e 1e 142.2e hewie-finds ;
: hewie-sniffs ( -- )
    c2f-2-album-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie -11.6e 0e 142.2e 10e 5e touches? 0= if  exit  then
    ['] album-sniffed start-only ;

\ ---- the walls' shade ----
: walls-shade ( -- )
    107e 8e -164.5e  85.8e 8.3e -178.2e  107.3e -25.5e -164.4e  85.8e -25.5e -178.2e  light-doorway
    87.9e 8.3e -176.8e  50.5e 8.3e -179.8e  87.9e -25.5e -176.8e  87.9e -25.5e -176.8e  light-doorway ;

\ ---- the movie replayed (the extras): Debilitas's scene ----
: replayed ( -- )
    fiona hold  debilitas-kind figure-slot figure-in  yield
    0 0 0 $36B $3AF obstacle-in  flames-lit  chandelier chandelier-still  chandelier-2 chandelier-still  tinted
    0 true no-shadow  debilitas-kind true no-shadow
    his-movie scenes:unseen-4-movie his-cutscene load-scene  panic-eased  if  -1 run-scene  then
    world-held state!  0 false no-shadow  debilitas-kind false no-shadow  figure-slot figure-out  replay-over ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    $D , 3 , 3 ,   $E , 1 , 1 ,   $F , 1 , 1 ,   $10 , 2 , 2 ,   $14 , 3 , 3 ,   $15 , 0 , 0 ,   $1A , 1 , 1 ,
create ahead  5 , 0 ,   $B , 0 ,   6 , 1 ,   7 , 1 ,   $C , 3 ,   8 , 4 ,   \ (area, exit)
create entries  2 , 2 ,   1 , 1 ,   0 , 0 ,   2 , 2 ,   3 , 3 ,   \ (by exit 0..4: the camera)
: ways ( -- )
    warned?  beyond-exit exit-open? and  fiona beyond-exit exit-area in-area? and  3 figure 0< and if  the-figures  then
    5 exits-out  cameras 7 cameras-at  ahead 6 ways-out
    fiona beyond-way-area entered? if  warned? if  the-figures  then  beyond-exit prepare-exit  then
    fiona back-way-area entered? if  warned? if  no-figures  then  4 prepare-exit  then
    first-time?  fiona chandelier-area entered? and  first-steps @ 0= and if
        -1 first-steps !  panic-eased  ['] his-scene start drop
    then ;
: each-frame ( -- )
    events-held state? if  exit  then
    ways  box-pushed  draughts  hewie-sniffs  doll-knocks  hewie-notices  shove-due  looks 3 hewie-looks-at
    sounds  spots  walls-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if  hewie send part-from-fiona  ['] replayed start drop exit  then
    entries 5 ways-in
    came-by @ forced-exit = if  fiona $142 0 place-on-tri  ['] pulled-free start drop  then ;
: went ( -- )
    stalker held?  his-shove @ and if  stalker let-go  0 summon-take  then
    found-file-209 fact?  door-beyond doors:door-locked? and if  door-beyond unlock  then ;

' came-in ' person-in ' each-frame ' went castle-2f-2 place!
