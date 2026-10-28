\ story/places/castle-1f-13.fs - the great hall in Belli Castle's 1F ($8): brought in by the forced
\ way - the balcony collapsing under her (a scene: she comes to below, Debilitas stalking along the
\ balcony above and shoving its rubble down on her; the crows' file), or seized and carried off (a
\ scene, out by the way loaded ahead); a place to crouch hidden behind (the stalker searching -
\ finding her more likely each time he comes); the fallen balcony and the false doors in turn, the
\ Chicken Jerky, the Antimony Powder Hewie sniffs out, the hanging thing she sets swinging, the
\ specks, the walls' shade, the creatures. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/castle-1f-13.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.castle-1f-13
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects keys story.kit fiona.anims fiona.sounds stalker.anims scenes ;

own-reset

\ ---- what is said here ----
0 constant balcony-msg     \ "I can't believe the balcony collapsed. It's like something out of a nightmare."
1 constant balcony-2-msg   \ "I'm not going to be able to get up there."
2 constant a-door-msg      \ "A door?"
3 constant false-door-msg  \ "What?! It's just a wall made to look like a door."
4 constant prank-msg       \ "Why would someone do this? Is it just some random prank?"

\ ---- its own state (a visit) ----
own: balcony-remarked  own: door-remarked  own: door-2-remarked
own: his-scene             \ Debilitas on the balcony (at the room's end he is put back up there)
own: hanger-rung           \ the hanging thing's creak last "on" (on and off in turn)

\ ---- the room's things ----
0 constant antimony-fx   1 constant jerky-fx   0 constant door-open-part   1 constant door-shut-part
4 constant first-balcony   6 constant #balcony   $A constant hanger   $B constant his-motions
$13 constant hiding-area   $10 constant balcony-area   $11 constant false-door-area   $12 constant false-door-2-area
3 constant crows-movie   2 constant crows-cutscene   1 constant seized-movie   0 constant seized-cutscene
$35 constant hall-door   $1A constant hall-track   $2F constant beyond-room   $B constant search-var
$21 constant crows-seen   $20 constant seized-seen   $227 constant crows-file   11 constant thud-sound
4 constant creak-on   5 constant creak-off   $81 constant seized-way
$803 constant creeping-anim   $207 constant fleeing-anim   \ (her creep behind the hiding place; her run out of it)
: fallen? ( -- flag )  found-file-227 fact? ;
: balcony-gone ( -- )  #balcony 0 do  first-balcony i + object-hide  loop ;
: balcony-up ( -- )  #balcony 0 do  first-balcony i + object-show  loop ;
: specks-about ( -- )
    $10 $A $80 $80 $80 $40 rgba -150e 23.7e 98.6e specks   $10 $A $80 $80 $80 $40 rgba -300e 29e 55e specks
    $A $A $80 $80 $80 $40 rgba -300e 29e -95e specks   $10 $C $80 $80 $80 $40 rgba -150e 23.7e -158.6e specks ;

\ ---- as she comes in ----
: back-on-patrol ( -- )   \ (from castle-b1-7: Debilitas loaded, not in the game - in here, searching)
    {: | p :}  partner to p  p 0< if  exit  then
    p activate  castle-1f-13 376 p send stalker-in  p 4 -1 camera-set  p full-health
    0 p send stalker-order  0 p send stalker-clock  p $178 120 place-on-tri  his-own-time ;
: came-in ( exit -- )
    drop  hall-door door-open? if  door-open-part  else  door-shut-part  then  show-part
    fallen? if
        found-file-227-2 fact? 0= if  found-file-227-2 fact!  castle-1f-13 0 248 4 15 0 creature-facing  then
        balcony-gone  room-sounds  hall-track 1e background
    else  balcony-up  then
    flowers-laid fact?  c1f-13-flag1 fact? 0= and if
        c1f-13-flag1 fact!  castle-1f-13 0 274 4 15 0 creature-facing  castle-1f-13 0 448 4 15 0 creature-facing
    then
    cb1-7-flag2 fact? if
        partner 0< 0=  partner body? 0= and  summoner-on state? and if  back-on-patrol  then
        cb1-7-flag2 fact-off
    then
    c1f-13-antimony-powder-found fact?  c1f-13-antimony-powder fact? 0= and if  antimony-fx -254.5e 1e -95e flicker-at  then
    specks-about  hanger 1 chimes-rest
    c1f-13-chicken-jerky fact? 0= if  jerky-fx -283.73e 1e 2.22e flicker-at  then ;

\ ---- Debilitas on the balcony after it fell: along it, his steps above, then his shove - the
\ rubble down (dust, a thud), her thrown if she is by him ----
: steps-above ( -- )  10 0 do  i 1 and stalker-bank 0 0  -147e i 8 * s>f f-  60e 14.5e  sound-at  i 9 < if  10 frames  then  loop ;
: on-the-balcony ( -- )
    stalker hold  -1 his-scene !  stalker his-motions motions-for  stalker use-motions  stalker true noclip  yield
    -219.01e 60.17e 14.42e -60 deg>yaw xyzw>cells stalker send place  stalker true noclip
    stalker true shown  stalker false hush
    steps-above
    stalker room-motion 5 start-blend8
    0  begin
        stalker fiona 0 close-to? if  stalker throws-her  then
        dup 60 = if  -283e 1e 37e dust  -280e 1e 42e dust  thud-sound room-bank stalker sound-by  then
        1+  stalker went-round? 0= while  yield
    repeat  drop
    stalker 0 0 camera-set  stalker full-health  2 stalker-order  0 stalker-clock  his-own-time  0 his-scene ! ;

\ ---- brought in by the forced way: the balcony falls (the first time; else she just comes to) ----
: hewie-kept ( -- )  hewie hold  begin  yield  again ;   \ (through it and after: taken off elsewhere)
: crows-scene ( -- )   \ (the crows' movie - the figures loaded before she came, the one wearing the other's)
    crows-movie scenes:keyed-movie crows-cutscene load-scene
    hewie false shown  2 cutscene-leave-alone  $1E $C8 movie-key  yield  4 3 figure-dressed-as ;
: balcony-falls ( -- )
    fiona hold
    fallen? if
        fiona wait-move  camera-restart  15 screen:from-black fade  wait-fade  scene-locked state-off
    else
        hewie-in? if  ['] hewie-kept start drop  then
        crows-scene  room-sounds  panic-eased  if  -1 run-scene  then
        balcony-gone  fiona $1D0 -80 place-on-tri  0 0 player-camera  camera-restart
        begin  room-sounds-in? 0= while  yield  repeat
        partner 0< 0= if
            castle-1f-13 383 partner send stalker-in  yield  stalker -1 -1 camera-set  stalker activate  his-own-time
            ['] on-the-balcony start drop
        then
        hewie-in? if  beyond-room -1 0 hewie-to  then
        fiona lying-anim 0 start-blend  3 figure-out  4 figure-out
        hall-track 1e background  15 screen:from-black with-music or fade
        fiona get-up-anim play
        found-file-227 fact!  found-file-227-2 fact!  castle-1f-13 0 248 4 15 0 creature-facing
        wait-fade  crows-seen resident-flag-set  crows-file give-file
    then
    stalkers-stay state-off  fiona let-go  1 prepare-exit ;
: seized ( -- )   \ (brought in seized: the movie, the stalker taken, out by the way loaded ahead)
    fiona hold  yield  camera-restart
    seized-movie scenes:keyed-movie seized-cutscene load-scene  $1E $C8 movie-key
    beyond-room preload  yield  4 3 figure-dressed-as
    panic-eased  if  -1 run-scene  then
    stalker let-go  0 summon-take  world-held state!  3 figure-out  4 figure-out
    seized-seen resident-flag-set  forced-exit take-exit ;

\ ---- the hiding place: crouched behind it, calming; out by the button (chased: running) - or
\ found, and away ----
: runs-out ( -- )   \ (found, in a panic, or chased: up and away, hugging herself)
    found-her @ if  2 avoid-prompt  then
    in-panic? 0= if  seized-sound common-bank fiona sound-by  then
    fiona-hidden state-off  fiona follow  stalkers-stay state!
    fiona $114 79 fleeing-anim walk-move -258.97e -140.95e go-to
    fiona hug-self-anim 20 play-blend  fiona false noclip  stalkers-stay state-off ;
: crouches ( -- )
    stalkers-stay state!  0 came-while-hidden !  0 found-her !
    fiona $1D4 -110 -1 walk-move -267e -140e go-to
    fiona true noclip  fiona crouch-down-anim play
    fiona $122 -110 creeping-anim walk-move -291.573e -146.706e go-to
    fiona $122 80 creeping-anim walk-move -291.573e -146.706e go-to
    fiona crouching-anim start-anim  fiona-hidden state!  stalkers-stay state-off  0 avoid-prompt
    begin
        circle button-pressed? if
            fiona-hidden state-off  fiona follow  stalker let-go
            chased? if  runs-out exit  then
            stalkers-stay state!  fiona $1D4 70 creeping-anim walk-move -267e -140e go-to
            fiona crouch-up-anim play  fiona false noclip  stalkers-stay state-off  exit
        then
        in-panic? found-her @ or if  in-panic? 0= if  seized-sound common-bank fiona sound-by  then  runs-out exit  then
        hidden-calm  yield
    again ;

\ ---- the stalker coming in while she hides: he searches ----
: finds-her ( -- )   \ (there she is)
    stalker hold  stalker wait-move  stalker $1A1 -90 -238.675e -139.193e walk-to
    stalker there-anim play  -1 found-her !  stalker 10 idle
    begin  fiona held? while  yield  repeat  0 stalker-order ;
: stalker-came ( -- )  came-on-her? if  stalker follow  search-var -1 ['] finds-her searches  then ;

\ ---- what she may do or say here ----
: fallen-balcony ( -- )
    fiona wait-move  fiona look-at-anim start-anim  40 frames
    balcony-remarked @ if  balcony-2-msg  else  -1 balcony-remarked !  balcony-msg  then  say  fiona wait-anim ;
: false-door ( remarked -- )   \ (her first try: a door? - no, a wall; then her wonder)
    dup @ if  drop prank-msg say exit  then
    a-door-msg say  fiona reach-out-anim play  fiona reach-back-anim play  false-door-msg say  -1 swap ! ;
: false-door-1 ( -- )  door-remarked false-door ;
: false-door-2 ( -- )  door-2-remarked false-door ;
: pick-jerky ( -- )  chicken-jerky c1f-13-chicken-jerky jerky-fx -283.73e 2.22e picks-up ;
: pick-antimony ( -- )  antimony-powder c1f-13-antimony-powder antimony-fx -254.5e -95e picks-up ;
: spots ( -- )
    c1f-13-chicken-jerky fact? 0=  fiona -283.73e 1e 2.22e 5e 5e touches? and if  ['] pick-jerky calm-only offer  then
    fiona hiding-area in-area? if
        stalker-in? if  no-time-message offer-remark-now  else  ['] crouches calm-only offer  then
    then
    fiona balcony-area 50 faces-area? if  ['] fallen-balcony unhurried offer  then
    fiona false-door-area in-area?  fiona 90 50 heading? and if  ['] false-door-1 unhurried offer  then
    fiona false-door-2-area in-area?  fiona 90 50 heading? and if  ['] false-door-2 unhurried offer  then
    c1f-13-antimony-powder-found fact?  c1f-13-antimony-powder fact? 0= and  fiona -254.5e 1e -95e 5e 5e touches? and if
        ['] pick-antimony calm-only offer  then ;

\ ---- the hanging thing: her steps set it creaking and swinging ----
: hanger-pushed ( -- )
    fiona -277e -142e 6e centre-across? if  hanger 1 false hanger-rung creak-on creak-off 5e chimes  then ;

\ ---- Hewie: the Antimony Powder he sniffs out ----
: antimony-sniffed ( -- )  c1f-13-antimony-powder-found c1f-13-antimony-powder antimony-fx  -254.5e 1e -95e hewie-finds ;
: hewie-sniffs ( -- )
    c1f-13-antimony-powder-found fact?  scrambling? 0= or if  exit  then
    hewie -254.5e 0e -95e 10e 5e touches? 0= if  exit  then
    ['] antimony-sniffed start-only ;

\ ---- the walls' shade ----
: walls-shade ( -- )
    -219.9e 59.3e 20.8e  -249.2e 59.3e -7.4e  -194.2e 59.3e -5.9e  -223.5e 59.3e -34.2e  light-doorway
    -180e 20e -119.9e  -200e 20e -119.9e  -180e 0e -119.9e  -200e 0e -119.9e  light-doorway ;

\ ---- the movies replayed (the extras): seized, the balcony falling ----
: replay ( movie cutscene balcony? -- )   \ (the crows and Debilitas as figures)
    {: movie cut fell :}  fiona hold  specks-about
    cast:crow 3 figure-in  cast:crow-2 4 figure-in  debilitas-kind 5 figure-in  yield  4 3 figure-dressed-as  yield
    movie scenes:keyed-movie cut load-scene  $1E $C8 movie-key  fell if  2 cutscene-leave-alone  then
    panic-eased  if  -1 run-scene  then
    3 figure-out  4 figure-out  5 figure-out  replay-over ;

\ ---- every frame ----
create cameras   \ (area, set, path)
    $C , 0 , 0 ,   $D , 1 , 1 ,   $E , 1 , 1 ,   $F , 0 , 0 ,   $14 , 0 , 0 ,   $15 , 3 , 3 ,   $16 , 0 , 0 ,
    $17 , 2 , 2 ,   $18 , 0 , 0 ,   $19 , 4 , -1 ,   $1A , 0 , 0 ,   $1B , 4 , -1 ,   $1C , 0 , 0 ,   $1D , 4 , -1 ,
create ahead  6 , 0 ,   $A , 0 ,   7 , 1 ,   8 , 1 ,   9 , 2 ,   $B , 3 ,   \ (area, exit)
create entries  4 , -1 ,   1 , 1 ,   2 , 2 ,   3 , 3 ,   \ (by exit 0..3: the camera)
: ways ( -- )  2 exits-out  2 2 leaves-by  3 3 leaves-by  cameras 14 cameras-at  ahead 6 ways-out ;
: each-frame ( -- )
    events-held state? if  exit  then
    ways  hanger-pushed  hewie-sniffs  spots  walls-shade ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if
                hewie send part-from-fiona
                replay-act case
                    $E of  [: seized-movie seized-cutscene false replay ;] start drop  endof
                    $F of  [: crows-movie crows-cutscene true replay ;] start drop  endof
                endcase  exit
            then
            entries 4 ways-in
            $82 forced-way? if
                fiona $10E 70 place-on-tri  0 0 player-camera  camera-restart
                came-by @ seized-way = if  ['] seized  else  ['] balcony-falls  then  start drop
            then  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )   \ (Debilitas on the balcony still: put back up there)
    stalker held?  his-scene @ and if
        stalker $1C0 -60 -275.75e 41.09e place-at  stalker 0 0 camera-set  0 his-scene !
    then ;

' came-in ' person-in ' each-frame ' went castle-1f-13 place!
