\ story/places/front-garden-3.fs - Belli Castle's front garden by the cage ($2A), where a new game
\ starts: Fiona out of the cage in the opening, two figures in the trees; the box against the
\ grate that Hewie helps her push; the breakable with magnesia; at night Hewie caught in the wire,
\ freed; the refined magnesia he digs up. A place (story/kit.fs). The original's scripts, for
\ checking: story/rooms-ref/front-garden-3.fs (converted from the game's, not loaded).
IN: story.places.front-garden-3
USING: engine game-state actors messages common vectors facts flag-names story-names room-names floor-flags
    effects story.kit fiona.anims hewie.anims hewie-names scenes ;

own-reset

\ ---- what is said here (the room's messages) ----
2  constant not-going-back-msg   \ "I just want to put the terrible things I saw back there behind me. I am not going back."
3  constant tree-msg             \ "A large tree is sprawled out here."
4  constant tree-again           \ "The branches are well kempt. Looks like someone has put a lot of time and energ..."
5  constant tree-wire-msg        \ "There is a thick wire encircling the trunk of the tree."
6  constant where-am-i-msg       \ "Where am I?"
7  constant cold-msg             \ "It's kind of cold... The air smells... funny."
8  constant fountain-msg         \ "A fountain... Looks like it is quite the antique."
9  constant box-msg              \ "This door is being blocked by that wooden box. There's got to be something I can do..."
$A constant hole-msg             \ "There is a hole here, but it's far too small for me."
$B constant cruel-msg            \ "I can't believe there are people out there that would do this to a poor helpless anima..."
$C constant wire-bound-msg       \ "Unforgivable! A wire is bound to the poor animal. It is digging deeply into the dog's..."
$D constant wire-snagged-msg     \ "The wire looks like it got snagged on something. So many wounds... So much blood... Yo..."
0  constant go-now-msg           \ "Go, now. And don't get caught this time. Go..." (the freeing scene's subtitles)

\ ---- its own state ----
own: tree-remarked
own: stalkers-held      \ the stalkers kept away while Hewie is at his part (the grate)
own: hewie-at-box       \ Hewie sitting by the box, waiting for her
own: pushing            \ the push begun: Hewie goes through when she is by the box
own: box-step           \ how far the push has gone (the two of them keep step by it)
own: wire-noticed       \ Hewie has looked at what is about here (once a visit each)
own: grate-noticed
own: dig-noticed
own: opening-on         \ the opening begun ...
own: figure-cried       \ ... she has come near the figures (the second cries out and rises)
own: whine-frames       \ frames to Hewie's next whine (caught in the wire)
own: figure-1-act       \ the figures' activities (stopped as she goes on)
own: figure-2-act
own: digging          \ Hewie at the dig (put back if she leaves)

\ ---- its places (the room's areas, by what is there) ----
4 constant gate-area   5 constant to-castle-area   6 constant near-castle-area   9 constant near-gate-area
2 constant near-yard-area   3 constant near-cage-area  7 constant near-cage-2-area  8 constant near-cage-3-area
$A constant tree-area   $11 constant waking-area   $12 constant cold-area   $13 constant fountain-area
$16 constant back-to-cage-area   $17 constant box-area   $18 constant hole-area
$19 constant box-near-area   $1A constant hewie-box-area   $B constant figures-gone-area

\ ---- the room's things ----
0 constant whole-part   1 constant broken-part   2 constant grate-gone-part   \ (its parts: the breakable whole / in pieces, the grate pushed away)
3 constant wire-hewie-part   4 constant wire-part                            \ (the wire with Hewie in it, the wire alone)
0 constant breakable-group   1 constant solid-group   2 constant grate-group   3 constant dig-group   \ (nav groups)
fg-3-breakable-broken whole-part broken-part  185.7e -0.1e -35.4e 5e 8e  breakable: the-box   \ (the crate by the wall)
the-box shard-grey 0 $A shard-look  the-box 0.812e 0.687e 0.187e 0.312e shard-uv  the-box breakable-group frees-floor  the-box $2CF noise-from
0 constant magnesia-fx   \ (the place's effect: the magnesia's sparkle)
$2F constant grate-door
$E constant figure-1-kind   $F constant figure-2-kind   3 constant figure-1   4 constant figure-2   \ (the figures in the trees, their slots)
$1A constant castle-figure  \ (loaded for the way to the castle's door, exit 3)
0 constant hewie-effort-sound   1 constant whine-sound   2 constant her-effort-sound   10 constant figure-cry-sound
$2E constant go-for-it-call   \ (the common bank's)
$92 constant magnesia   $95 constant refined-magnesia
$200 constant opening-file   $205 constant freed-file   $8275 constant grate-file
2 constant debilitas-kind-n   \ (the opening's stalker: Debilitas)
0 constant cleared-once       \ (the system data's flag: the game finished once - the costumes there from the start)

: night? ( -- flag )  c2f-3-flag1 fact? ;
: in-wire? ( -- flag )  night?  hewie-found-in-wire fact? 0= and ;   \ (Hewie caught in it, not found yet)

\ ---- the opening: the figures in the trees, its music; she is the player's ----
: figure-idles ( slot -- )   \ (in its tree: one of its two idles, again and again)
    begin  dup  50 chance? if  $9000  else  $9001  then  1 0 figure-play  dup figure-wait  again ;
: figure-1-idles ( -- )  figure-1 figure-on  figure-1 266.9e 75.5e -138.7e 0e figure-at  figure-1 figure-idles ;
: figure-2-idles ( -- )
    figure-2 figure-1 figure-dressed-as  figure-2 figure-on  figure-2 229.0e 38.3e -55.3e 0e figure-at  figure-2 figure-idles ;
: figure-2-rises ( -- )   \ (she comes near: it cries out and rises out of sight, 90 frames)
    figure-cry-sound room-bank  figure-2 figure dup 0< if  2drop  else  sound-by  then
    figure-2 $9002 1 0 figure-play  8 frames
    90 0 do  figure-2 0e 0.5e 2e figure-shift  yield  loop ;
: the-opening ( -- )   \ (Fiona's act01)
    fiona hold
    debilitas-kind-n stalker-ready  figure-1-kind figure-1 figure-in  figure-2-kind figure-2 figure-in
    3 5 6 $17 $B $C $15 $24 $12  9 0 do  stalker-slot 1 doors-into  loop   \ (Debilitas kept out of these rooms)
    $17 hewie-slot 1 doors-into
    yield  0 music-part  2 0 0 stage-music  4 0 0 stage-music
    -1 opening-on !  ['] figure-1-idles start figure-1-act !  ['] figure-2-idles start figure-2-act !
    opening-done fact!  $16 1e background  in-play state-off
    15 screen:from-black fade wait-fade  opening-file give-file ;
: figures-near ( -- )   \ (the opening: reaching the figures' tree, the second cries out and rises; past them they go)
    opening-on @  figure-cried @ 0= and if
        fiona 225e 0e -44e 25e 10e touches? if
            -1 figure-cried !  figure-2-act @ stop-act  ['] figure-2-rises start figure-2-act !
        then
    then
    figure-cried @ if
        fiona figures-gone-area entered? if
            figure-1 figure-out  figure-2 figure-out  figure-1-act @ stop-act  figure-2-act @ stop-act
        then
    then ;

\ ---- it comes in ----
: opening-doors ( -- )   \ (the opening: every way locked but the garden's, these gone for good)
    7 lock  9 lock  $A lock  $B lock  $13 lock  $22 lock  $26 lock  $12 lock  $2F lock  2 lock  0 lock
    3 lock  6 lock  8 lock  $14 lock  $15 lock  $19 lock  $1E lock  $34 lock
    $35 closed-off  $18 closed-off  $1C closed-off  $1D closed-off  1 closed-off  $D closed-off
    exits-redone ;
: hewie-held-in-wire ( -- )   \ (caught: he can't get out - held in his animation till she frees him)
    hewie hold  hewie $168 0 228.1e 77.57e place-at  hewie $1004 0 start-blend
    begin  hewie-p mine? while  yield  repeat ;
: daylight ( -- )  $AC $60 $26 $50 $19 $1F $12 $80 room-tint  effects:leaves ;
: water-light ( -- )   \ (the light rippling off the fountain)
    $F $80808080  65.6387e 6.1e 70.9417e  39.8e 0.5e 0.1e  effects:caustic drop ;
: night-specks ( -- )
    $A 128.0e 56.7e 13.0e  $E $80 $80 $80 $40 rgba  specks
    $10 152.0e 56.7e 13.0e  $C $80 $80 $80 $40 rgba  specks ;
: came-in ( exit -- )
    drop  room-sounds  solid-group solid-floor floor-on
    opening-done fact? 0= if
        cleared-once resident-flag? if  fg-3-flag3 fact!  then
        fiona $275 -90 place-on-tri  2 1 player-camera
        subscreen-locked state-off  hewie-commandable state-off  hewie-no-attack state-off
        force-calm state!  hewie-no-dodge state!  hewie-timid state!  no-flee state!
        opening-doors
        0 var@ 3 + 0 var!  1 var@ 3 + 1 var!   \ (variables 0 and 1 counted on three: no script reads them)
        hewie send part-from-fiona  world-held state!  ['] the-opening start drop
    else  $16 1e background  then
    wire-hewie-part hide-part  wire-part hide-part
    night? if
        in-wire? if  30 whine-frames !  this-room @ 360 0 hewie-to  ['] hewie-held-in-wire start drop  wire-hewie-part show-part
        else  wire-part show-part  then
    then
    night? if  night-specks  else  daylight  then  water-light
    grate-open fact? if  2 object-hide  3 object-hide  grate-gone-part show-part
    else  grate-group barred-floor floor-on  then
    the-box as-it-stands
    the-box broken?  fg-3-magnesia fact? 0= and if  magnesia-fx 185.7e 0.9e -35.4e flicker-at  then ;

\ ---- the box against the grate: Hewie through the hole, the two of them push ----
: grate-pushed ( -- )   \ (the box on, the grate swung back and forth as it gives)
    begin  box-step @ 3 < while  0.4e s" kibako" place-z object+!  yield  repeat
    begin  box-step @ 4 < while  yield  repeat
    her-effort-sound room-bank fiona sound-by
    begin  box-step @ 5 < while  -1.5e deg>rad s" a_koushi" turn-y object+!  yield  repeat
    begin  box-step @ 6 < while   0.5e deg>rad s" a_koushi" turn-y object+!  yield  repeat
    begin  box-step @ 7 < while  -0.5e deg>rad s" a_koushi" turn-y object+!  yield  repeat ;
: she-pushes ( -- )   \ (Fiona's act0E)
    fiona hold  begin  room-sounds-in? 0= while  yield  repeat
    1 box-step !  fiona -90 turn
    begin  box-step @ 2 < while  yield  repeat
    8 frames  hewie-effort-sound room-bank hewie sound-by
    grate-pushed
    begin  box-step @ 8 < while  yield  repeat
    0 hewie-at-box !  grate-file give-file ;
: through-the-hole ( -- )   \ (Hewie's act0F: through the hole, the push from the other side)
    hewie hold  begin  box-step @ 1 < while  yield  repeat
    hewie false doorway-fades  hewie $80808080 tint  hewie $346 0 -35e 137e place-at
    7 -1 fixed-camera  hewie true noclip
    hewie $345 0 -1 walk-move -33.792e 160e go-to  2 box-step !
    hewie $194 0 $204 walk-move -33.905e 165e go-to  10 frames  3 box-step !
    hewie $34D -1 run-move path-to  4 box-step !
    64 frames  5 box-step !  8 frames  6 box-step !  16 frames  7 box-step !  8 frames
    grate-group barred-floor floor-off  grate-door unlock  5 4 player-camera
    stalkers-stay state-off  0 stalkers-held !  grate-open fact!  8 box-step !
    hewie true doorway-fades  30 trust+
    hewie $34E -1 run-move path-to  hewie false noclip ;
: push-starts ( -- )   \ (the original's act0D: once she is free, she pushes)
    with-fiona-free  panic-eased  ['] she-pushes start drop ;
: comes-back ( -- )   \ (Hewie's act0C: back from the box to her)
    hewie hold  hewie-bark  hewie wait-move
    hewie $346 90 -1 walk-move -35e 137e go-to
    hewie $DC 90 $204 walk-move -12.99e 136.933e go-to
    hewie false noclip  hewie -1 idle  stalkers-stay state-off  0 stalkers-held ! ;
: to-the-box ( -- )   \ (Hewie's act0B: told "go there" by the box - he waits by it, sitting)
    hewie hold  -1 stalkers-held !  0 pushing !  stalkers-stay state!
    hewie wait-move  hewie-bark  hewie wait-move
    hewie $DC -90 -1 walk-path-move -12.99e 136.933e go-to
    -1 hewie-at-box !  hewie true noclip
    hewie $359 -90 $204 walk-move -30e 137e go-to
    hewie sit-down-anim play  hewie sit-anim play
    begin
        fiona box-near-area in-area?  started-now @ come-back-cmd = and if
            0 hewie-at-box !  comes-back exit
        then
        fiona box-area in-area? if
            started-now @ dup come-back-cmd =  over go-there-cmd = or  swap go-for-it-cmd = or if
                0 box-step !  ['] push-starts start drop  through-the-hole exit
            then
            pushing @ if  through-the-hole exit  then
        then
        yield
    again ;
: box ( -- )   \ (Fiona's act09: blocked - or, Hewie waiting by it, the push)
    hewie-at-box @ if  0 box-step !  -1 pushing !  she-pushes exit  then
    4 frames  world-frozen state!
    fiona $2A7 -80 -25e 168e place-at  1 13e 20e 0e 0e shot  16 frames
    box-msg say  4 frames  shot-off
    fiona $21D -90 -21e 168e place-at  world-frozen state-off ;
: hole ( -- )
    fiona $DC -90 -1 walk-move -12.99e 136.933e go-to
    fiona crouch-down-anim play  fiona crouching-anim start-anim  hole-msg say
    fiona crouch-up-anim play  fiona -1 idle ;

\ ---- the refined magnesia: she sends him, he digs it up ----
: digs ( -- )   \ (Hewie's act11)
    hewie hold  -1 digging !  hewie wait-move  hewie-bark  hewie wait-move
    hewie $1B3 173 -1 walk-path-move 232.55e -97.32e go-to
    dig-group solid-floor floor-off  dig-group keeps-fiona-off keeps-pursuers-off or floor-on
    $1A1 35 238e 1e -135e hewie-leap  hewie wait-move
    fg-3-refined-magnesia fact? 0= if
        hewie $37 -1 run-move path-to  hewie sit-a-while-anim play
        fiona-half-hidden state?  fiona held? 0= or if
            refined-magnesia item-full? 0= if  10 trust+  then
            refined-magnesia fg-3-refined-magnesia -1 takes-up
        then
    then
    hewie 238e -135e turn-to-xz
    $1A1 35 238e 1e -135e hewie-leap  hewie wait-move
    dig-group solid-floor floor-on  stalkers-stay state-off  0 digging ! ;
: go-for-it ( -- )   \ (Fiona's act10: the gesture, him sent)
    fiona hold  stalkers-stay state!  fiona wait-move
    hewie held? if  stalkers-stay state-off  exit  then
    ['] digs start drop
    fiona go-for-it-gesture-anim start-anim  fiona 1 wait-mark  go-for-it-call common-bank fiona sound-by
    fiona wait-anim  fiona -1 idle  7 frames ;

\ ---- Hewie in the wire, freed (at night, back from the castle's 2F) ----
: frees-him ( -- )   \ (act00)
    hewie hold  stalkers-stay state!  hewie-found-in-wire fact!  fiona wait-move
    fiona 228.1e 77.57e turn-to-xz
    fiona tend-start-anim 10 play-blend  wire-bound-msg say  fiona tend-lead-in-anim 10 start-blend
    1 scenes:unseen-movie 0 load-scene
    15 screen:to-black-2 with-music or fade  no-track 1e background  wait-fade
    wire-hewie-part hide-part  wire-part show-part  panic-eased
    if  go-now-msg run-scene  then
    fiona $167 166 218.95e 74.59e place-at  hewie send part-from-fiona   \ (he runs off: out of the game - Hewie_Deactivate)
    camera-restart  $16 1e background  15 fade-in
    cruel-msg say  freed-file give-file  stalkers-stay state-off ;
: whines ( -- )   \ (now and then, from the wire)
    in-wire? 0= if  exit  then
    -1 whine-frames +!  whine-frames @ 0> if  exit  then
    30 chance? if  whine-sound room-bank hewie sound-by  180  else  60  then  whine-frames ! ;

\ ---- the movie replayed (the extras: its scripts held) ----
: replay-freed ( -- )
    hewie send part-from-fiona  night-specks  water-light  wire-part show-part  panic-eased
    1 scenes:unseen-movie 0 load-scene  if  go-now-msg run-scene  then
    forced-exit take-exit  events-held state-off  hewie send part-from-fiona ;

\ ---- what she may do or say here ----
: tree ( -- )
    fiona wait-move  fiona 250e 87e turn-to-xz
    night? if  tree-wire-msg say exit  then
    fiona look-at-anim start-anim  30 frames
    tree-remarked @ if  tree-again  else  -1 tree-remarked !  tree-msg  then  say  fiona wait-anim ;
: waking ( -- )   \ ("Where am I?" - her waking animation first)
    fiona wait-move  fiona rest-anim play  fiona -1 idle
    where-am-i-msg show  7 frames  wait-closed  fg-3-flag1 fact! ;
: cold ( -- )  fiona wait-move  cold-msg say  fg-3-flag2 fact! ;
: fountain ( -- )  fiona wait-move  fiona 65.64e 70.94e turn-to-xz  fountain-msg say ;
: not-back ( -- )   \ (turning back to the cage)
    fiona wait-move  fiona 120 turn  fiona turn-round-anim play  fiona -1 idle  not-going-back-msg say ;
: wire-snagged ( -- )
    fiona wait-move  fiona 228.1e 77.57e turn-to-xz
    fiona crouch-down-anim 10 play-blend  wire-snagged-msg say  fiona crouch-up-anim 10 play-blend ;
: pick-magnesia ( -- )  magnesia fg-3-magnesia magnesia-fx 185.7e -35.4e picks-up ;
: magnesia-shows ( -- )  magnesia-fx 185.7e 0.9e -35.4e flicker-at ;   \ (the box broken: the magnesia in it)
: spots ( -- )
    night? if
        in-wire? if  fiona hewie 10 90 touches-facing? if  ['] frees-him unhurried offer  then
        else  fiona 228.1e 77.57e 10e near-xz?  fiona 90 228e 77e faces-point? and if  ['] wire-snagged unhurried offer  then  then
    then
    fiona tree-area 60 faces-area? if  ['] tree unhurried offer  then
    grate-open fact? 0=  fiona box-area in-area? and  fiona -45 50 heading? and if  ['] box unhurried offer  then
    fiona hole-area in-area?  fiona -45 50 heading? and if  ['] hole unhurried offer  then
    fiona waking-area in-area?  fg-3-flag1 fact? 0= and if  ['] waking unhurried offer  then
    fiona cold-area in-area?  fg-3-flag2 fact? 0= and if  ['] cold unhurried offer  then
    fiona fountain-area 50 faces-area? if  ['] fountain unhurried offer  then
    the-box broken? if
        fg-3-magnesia fact? 0=  fiona 185.7e 0.9e -35.4e 5e 5e touches? and if  ['] pick-magnesia calm-only offer  then
    else
        the-box breaks? if  magnesia-shows  then
    then ;
: hewie-cues ( -- )   \ (her gestures to him: to the box, for the dig)
    hewie-in?  hewie-cond-now @ down-cond <> and  hewie held? 0= and 0= if  exit  then
    started-now @ go-there-cmd <> if  exit  then
    fiona box-near-area in-area?  hewie hewie-box-area in-area? and  fiona 50 -16e 137e faces-point? and
    grate-open fact? 0= and  stalker-in? 0= and if
        hewie-breakable-now @ if  ['] to-the-box start drop  then  exit
    then
    238e 0e -135e point-unseen?  fiona unseen? 0= and  hewie unseen? 0= and
    fiona 50 238e -135e faces-point? and  fg-3-refined-magnesia fact? 0= and if
        hewie-breakable-now @ if  ['] go-for-it start drop  then
    then ;
: back-to-cage ( -- )   \ (heading back to the cage: she won't)
    fiona held? 0=  in-panic? 0= and  fiona back-to-cage-area entered? and if  ['] not-back start drop  then ;

\ ---- Hewie about the garden: what he looks at as he passes ----
: zone-look ( F: x y z dy -- )  {: F: x y z dy :}  x  y dy f+  z  hewie-scent ;
: wire-stops ( -- )   \ (shared $8A: to the wire's tree, alert, a long look, staying put)
    hewie hold  hewie wait-move
    hewie spot-move 0 0  244.4e 11e 82.19e 0e move  hewie wait-move
    hewie stance-anim start-anim  yield
    hewie look-point-move 0 0  244.4e 11e 82.19e 0e move  120 frames
    hewie -1 look-at  yield  hewie 10 idle  10 frames  hewie send hewie-stay ;
: grate-stops ( -- )  -23.45e -10e 138e hewie-barks-at ;
: dig-stops ( -- )  252.44e 0e -156.9e hewie-barks-at ;
: calm-free? ( -- flag )  hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and ;
: hewie-looks ( -- )
    calm-free? 0= if  exit  then
    wire-noticed @ 0=  hewie 244.4e 11e 82.19e 44e 13e centred-in? and if
        -1 wire-noticed !  50 chance? if  ['] wire-stops start drop  then
    then
    hewie 244.4e 11e 82.19e 44e 13e centred-in? if  244.4e 11e 82.19e 16e zone-look  then
    grate-open fact? 0=  hewie hewie-box-area in-area? and  grate-noticed @ 0= and
    hewie -23.45e -10e 138e 40e 20e centred-in? and if  -1 grate-noticed !  ['] grate-stops start drop  then
    fg-3-refined-magnesia fact? 0= if
        dig-noticed @ 0=  hewie 252.44e 0e -156.9e 48e 20e centred-in? and if  -1 dig-noticed !  ['] dig-stops start drop  then
        danger-now @ 0=  hewie 252.44e 0e -156.9e 48e 20e centred-in? and if  252.44e 0e -156.9e 0e zone-look  then
    then ;

\ ---- its ways out, its cameras ----
: ways ( -- )
    0 0 leaves-by  1 1 leaves-by  gate-area 2 leaves-by
    fiona to-castle-area in-area?  fiona held? 0= and if  castle-figure figure-1 figure-in  3 take-exit  then
    fiona near-yard-area entered? if  1 prepare-exit  then
    fiona near-cage-area entered?  fiona near-cage-2-area entered? or  fiona near-cage-3-area entered? or if  0 prepare-exit  then
    fiona near-castle-area entered? if  castle-figure figure-1 figure-in  3 prepare-exit  then
    fiona near-gate-area entered? if  2 prepare-exit  then
    fiona near-castle-area left? if  figure-1 figure-out  then ;
: yard-camera ( -- set path )  c2f-2-flag1 fact? if  6 5  else  2 1  then ;   \ (the yard's view once the castle is known)
: cameras ( -- )
    $B 0 -1 area-camera  $C yard-camera area-camera  $D 0 -1 area-camera  $E 1 0 area-camera  $F 1 0 area-camera
    $10 3 2 area-camera  $14 4 3 area-camera  $15 0 -1 area-camera  $1B 1 0 area-camera  $1C 5 4 area-camera ;
: stalkers-back ( -- )   \ (Hewie no longer at his part: the stalkers may come)
    stalkers-held @  hewie held? 0= and if  0 stalkers-held !  stalkers-stay state-off  then ;

\ ---- she comes in by an exit: placed there, its camera ----
: player-came ( -- )  0 1 0 way-in  1 yard-camera way-in  2 5 4 way-in  3 3 2 way-in ;

: each-frame ( -- )
    events-held state? if  exit  then
    ways  cameras  back-to-cage  hewie-cues  stalkers-back  figures-near  whines  hewie-looks  spots ;
: person-in ( i -- )
    fiona-p = if  events-held state? if  ['] replay-freed start drop exit  then  player-came  then ;
: went ( -- )   \ (leaving: Hewie still in the wire out of the game, the first remarks made, the stalkers let go)
    in-wire? if  hewie send part-from-fiona  then
    fg-3-flag1 fact!  fg-3-flag2 fact!  stalkers-held @ if  stalkers-stay state-off  then
    digging @  hewie dig-group on-group? and if
        hewie $282 0 235e -126e place-at  dig-group solid-floor floor-on  stalkers-stay state-off
    then ;

' came-in ' person-in ' each-frame ' went front-garden-3 place!
