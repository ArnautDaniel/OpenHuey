\ story/places/front-garden-4.fs - the kennels in Belli Castle's front garden ($2B): the empty
\ cages, a boarded door, the big rusted door, a breakable, the camomile, the Album Medallion Hewie
\ sniffs out; now and then a rabbit grazing that runs off; the stalker who, finding her hidden here
\ by the open door, looks about and shuts it. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/front-garden-4.fs in the history (converted from the game's; gone with
\ the old machinery in 0.2.20).
IN: story.places.front-garden-4
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    items.names cast effects story.kit fiona.anims stalker.anims ;

own-reset

\ ---- what is said here ----
0 constant cages-msg         \ "Empty cages litter the area. Maybe this was some kind of kennel for keeping animals."
1 constant feathers-msg      \ "There are feathers and fur in the cage. For there to be this much, it must have bee..."
2 constant boarded-msg       \ "This door has been boarded up."
3 constant rusted-msg        \ "The hinge is rusted solid. Try as I may, I can't open this door."
4 constant large-door-msg    \ "Such a large door... What could be on the other side?"

\ ---- its own state (a visit) ----
own: cages-remarked
own: rabbit-heard           \ the rabbit's sound at the cages' end made
own: his-scene              \ the stalker's look about (its activity, while it lasts; else 0)

\ ---- the room's things ----
0 constant whole-part   1 constant broken-part   0 constant breakable-group
0 constant to-exit-1-area   3 constant by-the-door-area   4 constant cages-area   5 constant boarded-area
6 constant large-door-area
0 constant camomile-fx   1 constant medallion-fx
3 constant rabbit-slot   2 constant rabbit-steps   3 constant rabbit-call   $9000 constant grazing-anim
0 constant push-sound   $37 constant big-door   $16 constant garden-track
fg-4-breakable-broken whole-part broken-part  -120e 0e -13e 5e 8e  breakable: the-crate
the-crate $80404040 0 5 shard-look  the-crate 0.812e 0.687e 0.187e 0.312e shard-uv
the-crate breakable-group frees-floor  the-crate $124 noise-from

\ ---- the rabbit (one visit in three or so): grazing by the gate, off past the cages as one comes ----
: rabbit-runs ( -- )
    rabbit rabbit-slot figure-in  yield  rabbit-slot figure-on
    rabbit-slot -240e 0e 43e 90e figure-at  rabbit-slot grazing-anim 1 0 figure-play
    rabbit-slot rabbit-steps -290e 42e figure-goes  rabbit-slot figure-out ;
: rabbit-in? ( -- flag )  rabbit-slot figure dup 0< if  drop false  else  body?  then ;
: rabbit-calls ( -- )   \ (at the cages' end, once)
    rabbit-heard @  rabbit-in? 0= or if  exit  then
    rabbit-slot figure -304.86e 49.5e 20e centre-across? 0= if  exit  then
    -1 rabbit-heard !  rabbit-call room-bank rabbit-slot figure sound-by ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  garden-track 1e background
    c2f-3-flag1 fact? 0= if  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  then   \ (daylight, till the night)
    the-crate as-it-stands
    fg-4-camomile fact? 0= if  camomile-fx -202.89e 1e -6.76e flicker-at  then
    fg-4-album-medallion-found fact?  fg-4-album-medallion fact? 0= and if  medallion-fx -283.8e 1e 15.5e flicker-at  then
    30 chance? if  ['] rabbit-runs start drop  then ;

\ ---- what she may do or say here ----
: cages ( -- )
    fiona wait-move  fiona -140e 0e turn-to-xz
    cages-remarked @ if  feathers-msg  else  -1 cages-remarked !  cages-msg  then  say ;
: boarded ( -- )  fiona wait-move  fiona 180 turn  boarded-msg say ;
: the-large-door ( -- )   \ (before her clothes are found, only a look; after, she tries it)
    found-file-202 fact? 0= if  fiona wait-move  fiona 0 turn  large-door-msg say exit  then
    fiona $1C 0 -190.323e 61.999e walk-to  4 frames
    fiona lean-anim play  push-sound room-bank fiona sound-by
    fiona strain-anim start-anim  16 frames  fiona let-go-push-anim play
    fiona -1 idle  7 frames  rusted-msg say ;
: pick-camomile ( -- )  camomile fg-4-camomile camomile-fx -202.89e -6.76e picks-up ;
: pick-medallion ( -- )  album-medallion fg-4-album-medallion medallion-fx -283.8e 15.5e picks-up ;
: spots ( -- )
    fiona cages-area in-area?  fiona 60 -140e 0e faces-point? and if  ['] cages unhurried offer  then
    fiona boarded-area in-area?  fiona 90 60 heading? and if  ['] boarded unhurried offer  then
    fiona large-door-area in-area?  fiona 0 60 heading? and if  ['] the-large-door unhurried offer  then
    fg-4-camomile fact? 0=  fiona -202.89e 1e -6.76e 5e 5e touches? and if  ['] pick-camomile calm-only offer  then
    fg-4-album-medallion-found fact?  fg-4-album-medallion fact? 0= and
    fiona -283.8e 1e 15.5e 5e 5e touches? and if  ['] pick-medallion calm-only offer  then
    the-crate breaks? drop ;

\ ---- Hewie: the medallion he sniffs out, the rabbit he watches ----
: medallion-sniffed ( -- )  fg-4-album-medallion-found fg-4-album-medallion medallion-fx  -283.8e 1e 15.5e  hewie-finds ;
: hewie-sniffs ( -- )   \ (scrambling about by the cages' end, it not found yet)
    fg-4-album-medallion-found fact?  scrambling? 0= or if  exit  then
    hewie -283.8e 0e 15.5e 10e 5e touches? 0= if  exit  then
    ['] medallion-sniffed start-only ;
: watches-rabbit ( -- )   \ (calm, the rabbit within 70 of him: he looks at it)
    rabbit-in?  hewie-in? and  danger-now @ calm = and 0= if  exit  then
    hewie body-at rabbit-slot figure body-at vec-dist 70e f> if  exit  then
    rabbit-slot figure body-pos hewie-scent ;

\ ---- the stalker, finding her crouched by the open door: he looks about, backs off, shuts it ----
: shuts-her-in ( -- )
    stalker hold  stalker wait-move  stalker $A8 -90 -178e 43e walk-to
    0 exit-open? if
        stalker $1F 180 -212e 8e walk-to
        stalker stand-about-anims play  4 0 do  stalker step-back-anim play  loop
        stalker 0 shut-door  3 stalker-order  fg-4-flag1 fact!
    then  0 his-scene ! ;
: stalker-came ( -- )
    fiona-half-hidden state?  fiona by-the-door-area in-area? and  fg-4-flag1 fact? 0= and
    big-door door-open? and if  ['] shuts-her-in start his-scene !  then ;
: scene-broken ( -- )   \ (she is out of hiding: his scene over)
    his-scene @ 0=  fiona-half-hidden state? or if  exit  then
    his-scene @ stop-act  0 his-scene ! ;

\ ---- every frame ----
: ways ( -- )  to-exit-1-area 1 leaves-by ;
: each-frame ( -- )  ways  scene-broken  hewie-sniffs  watches-rabbit  rabbit-calls  spots ;
: person-in ( i -- )
    case  fiona-p of  1 0 0 way-in  endof  stalker-p of  stalker-came  endof  endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went front-garden-4 place!
