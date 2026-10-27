\ story/places/castle-b1-1.fs - the kitchen in Belli Castle's B1 ($12): the Castle Sketch on the wall;
\ once Hewie is found in the wire (till his file), Daniella at the stove - her scene the first time she
\ comes near (the ladle, the smoke), "She looks busy..." after; with the sketch, before nightfall, the
\ scene as she steps in (Riccardo, Debilitas sent in; out by the near door); the cupboards to hide in
\ (the stalker always finds her, but on his alert 4), the plywood door she breaks through, the vase
\ and its Chicken Jerky, the Lavender Hewie sniffs out, the jerky on the shelf; the china, the box,
\ the wine rack, the sink, the pot, the meat; Hewie wary of Daniella; the replays. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-b1-1.fs (converted
\ from the game's, not loaded). (Its act $3, unlocking door 4, is never started by its phases: left out.)
IN: story.places.castle-b1-1
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims scenes ;

own-reset

\ ---- what is said here ----
3 constant busy-msg        \ "She looks busy preparing dinner... Perhaps this isn't the best time to strike up a conv..."
4 constant meat-msg        \ "Looks like meat is being served... It appears they still haven't finished preparing it."
5 constant empty-pot-msg   \ "There is nothing in the pot..."
6 constant tendrils-msg    \ "The pot is filled with what look like tendrils. Wait a second, this looks like... h..."
7 constant nauseous-msg    \ "Looking at it again is going to make me nauseous."
8 constant china-msg       \ "Varied ornate, high-class china lines the shelf."
9 constant polished-msg    \ "They have all been polished to sparkling perfection. The building itself may be old..."
$A constant sink-msg       \ "The sink looks rusted in several spots... A thick, reddish-black discoloration cakes t..."
$B constant wine-msg       \ "Numerous expensive spirits stretch along the wine rack."
$C constant father-msg     \ "Come to think of it, my father was quite the wine connoisseur."
$D constant box-msg        \ "The box emanates no specific odor. Looks like it is full of canned goods."
$E constant plywood-msg    \ "This plywood-paneled door is blocking my path. But it could be broken through easily e..."

\ ---- its own state (a visit) ----
own: china-remarked  own: wine-remarked  own: pot-remarked
own: other-side      \ the cupboard she hides in: the far one
own: searched        \ he has searched her hiding place (this time hidden)
own: area-reacted    \ the scene as she steps in made
own: sounds-on  own: hewie-wary

\ ---- the room's things ----
1 constant riccardo-movie    0 constant riccardo-cutscene
3 constant cooking-movie  2 constant cooking-cutscene
4 constant ladle   5 constant her-motions   6 constant his-motions   3 constant daniella-slot
0 constant flame-fx   1 constant jerky-fx   2 constant lavender-fx   3 constant jerky-2-fx   4 constant smoke-fx
0 constant sketch-part   3 constant plywood-part   4 constant broken-plywood-part   2 constant plywood-floor   1 constant stove-floor
0 constant plywood-sound   1 constant room-loop-sound   3 constant vase-sound
$E constant search-var   $226 constant cooking-file
$12 constant hide-area   $A constant sketch-area   5 constant step-in-area
cb1-1-breakable1-broken 1 2  -52.5e -60.05e -1.5e 5e 8e  breakable: the-vase
the-vase $80102830 0 8 shard-look  the-vase 0.562e 0.5e 0.25e 0.5e shard-uv
the-vase 0 frees-floor  the-vase $209 noise-from  the-vase vase-sound room-bank shatters-with

\ ---- Daniella at the stove (Hewie found in the wire, his file not yet) ----
: cooking? ( -- flag )  hewie-found-in-wire fact?  found-file-208 fact? 0= and ;
: her-hands ( on? -- )   \ (the figure's held things: shown or put away)
    daniella-kind cast-model dup 0< if  2drop exit  then  swap if  story.words:daniella-one  else  false false story.words:daniella-parts  then ;
: at-the-stove ( -- )
    daniella-slot figure-on  false her-hands  daniella-slot $9000 1 0 figure-play
    daniella-slot $2A 14.85e 17.65e floor-at 0e figure-at ;
: smoke-on ( -- )  effects:smoke-puffs smoke-fx effect! ;
: daylight ( -- )  $84 $60 $3C $50 $24 $2A $2A $46 room-tint ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds
    got-castle-sketch fact? 0= if  sketch-part show-part  then
    flame-fx 0 -35.8e -50.5e -27.25e flame-at
    the-vase as-it-stands  the-vase broken?  cb1-1-chicken-jerky fact? 0= and if  jerky-fx -52.5e -59.05e -1.5e flicker-at  then
    cb1-1-lavender-found fact?  cb1-1-lavender fact? 0= and if  lavender-fx -18.1e -59e -8.1e flicker-at  then
    cb1-1-chicken-jerky-2 fact? 0= if  jerky-2-fx -13e 9e 10e flicker-at  then
    cooking? if
        force-followed state!  stove-floor keeps-fiona-off keeps-hewie-off or floor-on
        at-the-stove  ladle 2 0 object  smoke-on
    then
    c2f-3-flag1 fact? 0= if  daylight  then
    1 -19.5e -26e -31e  4e 4e 100e 0e 10e wanderer-at  2 -53e -22e -11e  4e 5e 80e 0e 30e wanderer-at
    cb1-1-breakable2-broken fact? if
        plywood-part hide-part  broken-plywood-part show-part  plywood-floor barred-floor floor-off
    else
        plywood-part show-part  broken-plywood-part hide-part  plywood-floor barred-floor floor-on
    then
    1 $2300 reverb ;

\ ---- her cooking scene: the first time she comes near the stove ----
: cooking-scene ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move
    15 screen:to-black-2 with-music or fade  ladle 3 0 object
    cooking-movie scenes:keyed-movie cooking-cutscene load-scene  wait-fade  smoke-fx effect-off
    hewie-in?  hewie-cond-now @ down-cond = and if  $86 0 hewie-do  then
    $50 $FF movie-key  panic-eased  if  -1 run-scene  then
    daniella-slot $9000 1 0 figure-play  ladle 2 0 object  ladle object-show
    daniella-slot $2A 14.85e 17.65e floor-at 0e figure-at  false her-hands
    hewie true shown  hewie let-go  wait-fade
    fiona 0 idle  fiona $18B -75 43.7e 11.8e place-at  2 -1 player-camera  camera-restart  smoke-on
    15 fade-in  cooking-file give-file  $1B resident-flag-set  stalkers-stay state-off ;
: busy-cooking ( -- )   \ (near her after: her look, her remark)
    fiona hold  fiona wait-move  fiona daniella-slot figure look-at  yield  busy-msg say  fiona -1 look-at  yield ;
: hewie-wary-of-her ( -- )  16.5e 0e 16.5e hewie-stands-by ;
: at-the-stove-frame ( -- )   \ (each frame, while she cooks)
    cb1-1-flag1 fact? 0= if
        fiona 16.5e 0e 16.5e 30e 10e touches?  cam-set@ 2 = and if  cb1-1-flag1 fact!  panic-eased  ['] cooking-scene start drop  then
    else
        fiona 16.5e 0e 16.5e 10e 10e steps-into?  fiona-mode-now @ own-mode = and  in-panic? 0= and if
            panic-eased  ['] busy-cooking start drop
        then
    then
    sounds-on @ if  room-loop-sound 15e 12e 22e keep-sound
    else  room-sounds-in? if  room-loop-sound 15e 12e 22e room-sound  -1 sounds-on !  then  then
    hewie-wary @ 0=  hewie-in? and  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and
    hewie 16.5e 0e 16.5e 25e 10e centred-in? and if  -1 hewie-wary !  ['] hewie-wary-of-her start drop  then
    hewie-in?  danger-now @ followed = and  hewie-cmdable-now @ and  hewie 16.5e 0e 16.5e 50e 10e centred-in? and if
        16.5e 15e 16.5e hewie-scent
    then ;

\ ---- the sketch taken, before nightfall: stepping in, Riccardo's scene - Debilitas sent in, out by the near door ----
: the-scene ( -- )
    stalkers-stay state!  force-calm state!  fiona hold  fiona wait-move
    15 screen:to-black-2 with-music or fade
    riccardo-movie scenes:unseen-movie riccardo-cutscene load-scene  wait-fade
    castle-b1-1 25 stalker-to  stalker let-go  riccardo-kind daniella-slot figure-in
    panic-eased  if  -1 run-scene  then
    world-held state!  stalker let-go  scene-locked state!  camera-restart   \ (her sounds hushed too: Fiona has no `silent` yet)
    castle-b1-1 stalker-slot 0 doors-into  stalkers-stay state-off  0 take-exit ;

\ ---- the cupboards: she hides (the near one or the far); the stalker always finds her - but on his alert 4 ----
: hides ( -- )
    0 searched !
    other-side @ if  her-motions $1FA 90 -1 -49.507e -33.083e hides-in
    else  her-motions $14A -90 -1 -22.548e -33.225e hides-in  then ;
: finds-her ( -- )
    stalker hold  stalker wait-move
    0 door-for-him  1 door-for-him   \ (by a shut door he came by: opened)
    $1FE -180 -35.74e -5.079e finds-her-at ;
: searches-here ( -- )
    -1 searched !  debilitas? if  stalker his-motions motions-for  then  stalker follow
    4 stalker-alert? 0= if  ['] finds-her start drop  else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;
: stalker-came ( -- )  came-on-her? if  searches-here  then ;
: search-on ( -- )   \ (each frame: hidden, he here and up, not yet searched)
    st-cond-now @ down-cond =  stalker-told? and  searched @ or if  exit  then
    fiona-hidden state?  stalker-in? and if  searches-here  then ;

\ ---- the plywood door: her blow breaks through ----
: plywood-breaks ( -- )
    cb1-1-breakable2-broken fact? if  exit  then
    22e -40e -32e 6e 12e her-blow-in? 0= if  exit  then
    plywood-sound room-bank fiona sound-by  1 $FF 4 rumble-pad
    plywood-part hide-part  broken-plywood-part show-part  plywood-floor barred-floor floor-off  cb1-1-breakable2-broken fact!
    $80303030 2 8  19e -40e -32e  0.234e 0.5e 0.328e 0.5e  0e shards
    $80404040 2 8  22e -40e -32e  0.234e 0.5e 0.328e 0.5e  0e shards
    $80303030 2 8  25e -40e -32e  0.234e 0.5e 0.328e 0.5e  0e shards
    18e -36e -33e dust  23e -38e -33e dust  28e -36e -33e dust ;

\ ---- what she may do or say here ----
: the-sketch ( -- )
    fiona hold  fiona wait-move  fiona -35.35e -34.5e turn-to-xz  fiona reach-out-anim play
    sketch-part hide-part  castle-sketch item-named  castle-sketch 1 give-item  castle-sketch show-item-tab  fanfare  obtained-2-msg say
    fiona reach-back-anim play  got-castle-sketch fact!  summoner-on state-off  5 false open-mark  doors-redone ;
: say-it ( msg -- )  fiona wait-move  say ;
: the-china ( -- )  china-remarked @ if  polished-msg  else  -1 china-remarked !  china-msg  then  say-it ;
: the-box ( -- )  fiona wait-move  fiona look-at-anim start-anim  16 frames  box-msg say  fiona wait-anim ;
: the-wine ( -- )  wine-remarked @ if  father-msg  else  -1 wine-remarked !  wine-msg  then  say-it ;
: the-sink ( -- )  sink-msg say-it ;
: empty-pot ( -- )  empty-pot-msg say-it ;
: the-pot ( -- )  pot-remarked @ if  nauseous-msg  else  -1 pot-remarked !  tendrils-msg  then  say-it ;
: the-meat ( -- )  fiona wait-move  fiona look-low-anim start-anim  30 frames  meat-msg say  fiona wait-anim ;
: the-plywood ( -- )  plywood-msg say-it ;
: vase-jerky ( -- )  chicken-jerky cb1-1-chicken-jerky jerky-fx -52.5e -1.5e picks-up ;
: the-lavender ( -- )  lavender cb1-1-lavender lavender-fx -18.1e -8.1e picks-up ;
: shelf-jerky ( -- )   \ (reached for, not in a panic)
    fiona wait-move  fiona -13e 10e turn-to-xz  in-panic? if  exit  then
    fiona reach-out-anim play  4 frames  chicken-jerky cb1-1-chicken-jerky-2 jerky-2-fx takes-up  fiona reach-back-anim play ;
: facing? ( area deg within -- flag )  {: area deg within :}  fiona area in-area?  fiona deg within heading? and ;
: cupboard ( far? -- )   \ (before the sketch, the sketch; else hide there - no time while he can see her)
    got-castle-sketch fact? 0= if  drop  fiona sketch-area 45 faces-area? if  ['] the-sketch calm-only offer  then  exit  then
    stalker-in? 0=  stalker unseen? or if  other-side !  ['] hides calm-only offer  else  drop  no-time-message offer-remark-now  then ;
: spots ( -- )
    the-vase broken? if
        cb1-1-chicken-jerky fact? 0=  fiona -52.5e -59.05e -1.5e 5e 5e touches? and if  ['] vase-jerky calm-only offer  then
    else  the-vase breaks? if  jerky-fx -52.5e -59.05e -1.5e flicker-at  then  then
    cb1-1-lavender-found fact?  cb1-1-lavender fact? 0= and  fiona -18.1e -59e -8.1e 5e 5e touches? and if  ['] the-lavender calm-only offer  then
    cb1-1-chicken-jerky-2 fact? 0=  fiona -13e 9e 10e 5e 5e touches? and if  ['] shelf-jerky calm-only offer  then
    fiona hide-area 45 faces-area? if
        fiona -90 60 heading? if  false cupboard  then
        fiona 90 60 heading? if  true cupboard  then
    then
    9 90 50 facing? if  ['] the-china unhurried offer  then
    $B 90 50 facing? if  ['] the-box unhurried offer  then
    $C 180 50 facing? if  ['] the-wine unhurried offer  then
    $D 0 50 facing? if  ['] the-sink unhurried offer  then
    $E 0 50 facing? if
        hewie-found-in-wire fact? 0= if  ['] empty-pot unhurried offer
        else  found-file-208 fact? if  ['] the-pot unhurried offer  then  then
    then
    fiona 50 3e -2e faces-point?  fiona 50 -3e -2e faces-point? or  fiona $F in-area? and if  ['] the-meat unhurried offer  then
    fiona 22e -40e -32e 6e 12e touches?  fiona 0 50 heading? and if  ['] the-plywood unhurried offer  then ;

\ ---- Hewie scrambling about: the Lavender he sniffs out ----
: lavender-sniffed ( -- )  cb1-1-lavender-found cb1-1-lavender lavender-fx  -18.1e -59e -8.1e  hewie-finds ;
: hewie-sniffs ( -- )
    cb1-1-lavender-found fact?  scrambling? 0= or if  exit  then
    hewie -18.1e -60e -8.1e 10e 5e touches? if  ['] lavender-sniffed start-only  then ;

\ ---- the movies replayed (the extras) ----
: replay-scene ( -- )
    fiona hold  daylight  2 daniella-slot figure-in  yield
    riccardo-movie scenes:unseen-movie riccardo-cutscene load-scene  panic-eased  if  -1 run-scene  then
    daniella-slot figure-out  replay-over ;
: replay-cooking ( -- )
    fiona hold  daniella-kind daniella-slot figure-in  yield
    cooking-movie scenes:keyed-movie cooking-cutscene load-scene  $50 $FF movie-key  panic-eased  if  -1 run-scene  then
    daniella-slot figure-out  replay-over ;

\ ---- every frame ----
create cameras  2 , 1 , 0 ,   3 , 2 , -1 ,   5 , 1 , 0 ,   6 , 3 , 1 ,   7 , 0 , -1 ,   8 , 3 , 1 ,   $10 , 1 , 0 ,   $11 , 5 , -1 ,
create entries  5 , -1 ,   2 , -1 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit loaded ahead)
create looks  0.6e 0e -3.32e  15e 14e 14e look,   -46.47e -60e -29.37e  14e 19e 19e look,
: doorways-shade ( -- )
    -19e 8e 0e  13e 8e 0e  -19e 0e 0e  13e 0e 0e  light-doorway
    -18.1e 13e -2.7e  -16.2e 13e -2.7e  -18.1e 0e -2.7e  -16.2e 0e -2.7e  light-doorway
    -18.1e 13e -1e  -18.1e 13e -2.7e  -18.1e 0e -1e  -18.1e 0e -2.7e  light-doorway
    -19e 8e 15e  -19e 8e 0e  -19e 0e 15e  -19e 0e 0e  light-doorway ;
: each-frame ( -- )
    events-held state? if  exit  then
    2 exits-out  cameras 8 cameras-at  ahead 2 ways-out
    fiona $13 entered? if  2 map-page  then   fiona $13 left? if  1 map-page  then
    got-castle-sketch fact?  c2f-3-flag1 fact? 0= and  area-reacted @ 0= and  fiona step-in-area entered? and if
        -1 area-reacted !  panic-eased  ['] the-scene start drop
    then
    hewie-sniffs  flame-fx -35.8e -50.5e -27.25e 8e -4e flame-stirred
    cooking? if  at-the-stove-frame  then
    search-on  plywood-breaks  looks 2 hewie-looks-at  spots  doorways-shade ;
: person-in ( i -- )
    case
        fiona-p of
            events-held state? if
                hewie send part-from-fiona  replay-act $14 = if  ['] replay-scene  else  ['] replay-cooking  then  start drop  exit
            then
            entries 2 ways-in  came-by @ 0 2 within if  2 map-page  then  endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- )
    cooking? 0= if  exit  then
    force-followed state-off  true her-hands  daniella-slot figure dup 0< if  drop  else  let-go  then ;

' came-in ' person-in ' each-frame ' went castle-b1-1 place!
