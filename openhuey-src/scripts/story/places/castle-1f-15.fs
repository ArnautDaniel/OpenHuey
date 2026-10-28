\ story/places/castle-1f-15.fs - the armour gallery in Belli Castle's 1F ($19): chased along the
\ balcony by Debilitas (elsewhere, or a way off), the encounter - Hewie away or down: a scene, and
\ she is left lying, frightened; Hewie with her: he saves her (Debilitas knocked down, a file) or,
\ likelier each time he has, fails (she is left lying, a file); the hole in the balcony floor (down
\ it by her choice; dropped in from it above, she lands); the diorama, the spear and the armour in
\ turn, the broken handrail in turn, the Camomile and the Medallion Hewie sniffs out, what he barks
\ at and looks up at, the steady sound, the far view's depth, the walls' shade, the other game's
\ creature. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-1f-15.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.castle-1f-15
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims fiona.sounds scenes ;

own-reset

\ ---- what is said here ----
0 constant diorama-msg     \ "'Agnus Dei' Looks like that's the title of this diorama."
1 constant spear-msg       \ "This spear decoration serves as an obvious conversation piece. Hard to believe peop..."
2 constant spear-2-msg     \ "I could try using this to protect myself... Damn! No good, it's just too heavy."
3 constant armour-msg      \ "A suit of armor forged from steel."
4 constant armour-2-msg    \ "I guess this room serves as a museum of sorts."
5 constant handrail-msg    \ "This section of handrail seems to be... gone. They always say 'never look down' in..."
6 constant handrail-2-msg  \ "If I fell here, there would be no coming back."
7 constant hole-msg        \ "Creaks and pops crackle out of the dubious-looking hole. Everything about the hole scre..."

\ ---- its own state (a visit) ----
own: spear-remarked  own: armour-remarked  own: handrail-remarked
own: encounter-started     \ the balcony's encounter begun (once a visit)
own: encounter-done        \ ... over, whichever way it went
own: hewie-noticed         \ Hewie has barked at the high window once this visit
own: sound-started         \ the steady sound begun (then kept up)

\ ---- the room's things ----
0 constant camomile-fx   1 constant medallion-fx
$C constant diorama-area   $D constant spear-area   $E constant armour-area   $F constant armour-2-area
$14 constant handrail-area   $17 constant hole-area   $18 constant balcony-area   6 constant stairs-area
8 constant steady-sound   $FE constant hole-exit   $27 constant below-room   7 constant saves-var
5 constant caught-movie   4 constant caught-cutscene   1 constant saved-movie   0 constant saved-cutscene
3 constant failed-movie   2 constant failed-cutscene   3 constant figure-slot
$1F constant caught-seen   $1D constant saved-seen   $1E constant failed-seen
$8278 constant saved-file   $8279 constant failed-file
: steady-at ( F: -- x y z )  -88e 65e -95e ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  0 sound-started !
    c1f-15-camomile-found fact?  c1f-15-camomile fact? 0= and if  camomile-fx 34.3e 1e -13.7e flicker-at  then
    c1f-15-medallion-found fact?  c1f-15-medallion fact? 0= and if  medallion-fx 56.3e 51e -71.5e flicker-at  then
    1 $2300 sound-reverb!
    items-40-as-70 fact?  c1f-15-flag1 fact? 0= and if
        c1f-15-flag1 fact!  castle-1f-15 0 389 $80 10 $FFDC creature-facing
    then ;

\ ---- the steady sound: begun once the room's sounds are in, then kept up ----
: sounds ( -- )
    sound-started @ 0= if  room-sounds-in? if  steady-sound steady-at room-sound  -1 sound-started !  then  exit  then
    steady-sound steady-at keep-sound ;

\ ---- the hole: down it if she will; dropped in from above, she lands ----
: the-hole ( -- )
    stalkers-stay state!  fiona wait-move  fiona look-close-anim 10 play-blend
    hole-msg ask 0= if
        15 screen:to-black fade  wait-fade  slide-sound common-bank fiona sound-by
        world-held state!  hole-exit take-exit
    else  stalkers-stay state-off  then ;
: lands ( -- )
    fiona hold  camera-restart  slide-sound common-bank fiona sound-by  3 map-page
    15 screen:from-black fade  wait-fade  stalkers-stay state-off ;

\ ---- chased along the balcony: the encounter ----
: he-hunts-her? ( -- flag )   \ (Debilitas in the game, not down, she chased - and he elsewhere or 30 off)
    debilitas?  st-cond-now @ down-cond <> and  chased? and 0= if  false exit  then
    st-room-now @ castle-1f-15 <>  stalker-in? 0= or if  true exit  then   \ (elsewhere, or gone from here)
    stalker body-at fiona body-at vec-dist 30e f< 0= ;
: encounter-begins ( movie cutscene -- )   \ (the stalkers held off, blind; to black, the scene loaded)
    {: movie cut :}  stalkers-stay state!  fiona hold  stalkers-blind state!
    15 screen:to-black-2 with-music or fade  fiona wait-move
    movie scenes:unseen-movie cut load-scene  wait-fade ;
: encounter-ends ( seen -- )   \ (back in, the stalkers let go; a movie seen)
    15 screen:from-black with-music or fade  wait-fade  stalkers-blind state-off
    -1 encounter-done !  resident-flag-set ;
: him-at ( tri face tri' F: x z -- )   \ (in the game here on tri', put there)
    {: tri face at F: x z | p :}  partner to p  p 0< if  exit  then
    castle-1f-15 at p send stalker-in  p tri face x z place-at ;
: caught-alone ( -- )   \ (no Hewie - or he's down, kept out of sight: she is left lying, frightened)
    caught-movie caught-cutscene encounter-begins
    hewie-in?  hewie-cond-now @ down-cond = and if  hewie hold  hewie false shown  then
    panic-eased  if  -1 run-scene  then
    world-held state!  fiona $169 180 60.358e 52e place-at  fiona lying-anim 0 start-blend  camera-restart
    $166 180 358 55.085e 70.506e him-at  partner 1 1 camera-set
    hewie true shown  hewie let-go  stalkers-stay state-off
    fiona hug-self-anim 30 play-blend
    caught-seen encounter-ends  100 threat ;
: saved ( -- )   \ (Hewie: Debilitas knocked down; he has saved her once more)
    saved-movie saved-cutscene encounter-begins
    panic-eased  if  -1 run-scene  then
    world-held state!  fiona $16C -57 63.654e 67e place-at  fiona 0 idle  camera-restart
    hewie $166 -57 56e 62e place-at
    $A1 -90 161 11.784e 75.875e him-at  partner send stalker-knock-out  partner 0 0 camera-set
    saves-var var@ 1+ saves-var var!  stalkers-stay state-off
    saved-seen encounter-ends  10 trust+  saved-file give-file ;
: failed ( -- )   \ (Hewie too: she is left lying)
    failed-movie failed-cutscene encounter-begins
    panic-eased  if  -1 run-scene  then
    world-held state!  fiona $16C -57 63.054e 67.67e place-at  fiona lying-anim 0 start-blend  camera-restart
    hewie $166 -57 62.709e 59.408e place-at
    $167 90 359 35.69e 79.463e him-at  partner 1 1 camera-set  stalkers-stay state-off
    failed-seen encounter-ends  failed-file give-file ;
: fail-odds ( -- pct )   \ (by how often he has saved her)
    saves-var var@ case  0 of 0 endof  1 of 50 endof  2 of 90 endof  3 of 90 endof  >r 0 r>  endcase ;
: encounter ( -- )
    encounter-done @  encounter-started @ or if  exit  then
    fiona balcony-area in-area?  in-panic? 0= and  he-hunts-her? and 0= if  exit  then
    -1 encounter-started !  panic-eased
    hewie-in? 0=  hewie-cond-now @ down-cond = or if  ['] caught-alone
    else  fail-odds chance? if  ['] failed  else  ['] saved  then  then  start drop ;

\ ---- what she may do or say here ----
: diorama ( -- )  fiona $112 175 -1 walk-move -0.195e -27.107e go-to  fiona look-close-anim play  diorama-msg say ;
: spear ( -- )
    fiona wait-move  spear-remarked @ if  spear-2-msg  else  -1 spear-remarked !  spear-msg  then  say ;
: armour ( -- )
    fiona wait-move  armour-remarked @ if  armour-2-msg  else  -1 armour-remarked !  armour-msg  then  say ;
: handrail ( -- )
    fiona $173 -90 -1 walk-move 33.471e 79.86e go-to  world-frozen state!  1 20e 50e 15e 0e shot
    handrail-remarked @ if  handrail-2-msg  else  -1 handrail-remarked !  handrail-msg  then  say
    16 frames  world-frozen state-off  shot-off ;
: pick-camomile ( -- )  camomile c1f-15-camomile camomile-fx 34.3e -13.7e picks-up ;
: pick-medallion ( -- )  medallion c1f-15-medallion medallion-fx 56.3e -71.5e picks-up ;
: spots ( -- )
    fiona diorama-area in-area?  fiona 180 45 heading? and if  ['] diorama unhurried offer  then
    fiona spear-area 60 faces-area? if  ['] spear unhurried offer  then
    fiona armour-area in-area?  fiona armour-2-area in-area? or  fiona 180 45 heading? and if  ['] armour unhurried offer  then
    fiona handrail-area in-area?  fiona -90 90 heading? and if  ['] handrail unhurried offer  then
    fiona hole-area in-area?  fiona -90 50 heading? and if  ['] the-hole unhurried offer  then
    c1f-15-camomile-found fact?  c1f-15-camomile fact? 0= and  fiona 34.3e 1e -13.7e 5e 5e touches? and if
        ['] pick-camomile calm-only offer  then
    c1f-15-medallion-found fact?  c1f-15-medallion fact? 0= and  fiona 56.3e 51e -71.5e 5e 5e touches? and if
        ['] pick-medallion calm-only offer  then ;

\ ---- Hewie: a bark at the high window once a visit (three times in ten), his looks up, the
\ Camomile and the Medallion he sniffs out ----
create looks
    -88e 50e -94.617e  30e 20e 16e look,
    -9.96e 0e 43.07e  83e 16e 16e look,
: barks-at-window ( -- )  -88e 50e -94.617e hewie-barks-at ;
: window-bark ( -- )
    hewie-noticed @  hewie-in? 0= or  hewie-cond-now @ well-cond <> or  chased? or  hewie-cmdable-now @ 0= or if  exit  then
    hewie -88e 50e -94.617e 30e 20e centred-in? 0= if  exit  then
    -1 hewie-noticed !  30 chance? if  ['] barks-at-window start drop  then ;
: camomile-sniffed ( -- )  c1f-15-camomile-found c1f-15-camomile camomile-fx  34.3e 1e -13.7e hewie-finds ;
: medallion-sniffed ( -- )  c1f-15-medallion-found c1f-15-medallion medallion-fx  56.3e 51e -71.5e hewie-finds ;
: hewie-sniffs ( -- )
    scrambling? 0= if  exit  then
    c1f-15-camomile-found fact? 0=  hewie 34.3e 0e -13.7e 10e 5e touches? and if  ['] camomile-sniffed start-only  then
    c1f-15-medallion-found fact? 0=  hewie 56.3e 50e -71.5e 10e 5e touches? and if  ['] medallion-sniffed start-only  then ;

\ ---- the far view's depth (camera set 0), the walls' shade ----
: depth-by-camera ( -- )
    cam-changed? 0= if  exit  then
    cam-set@ 0= if  20e 30e 2000e 2000e depth-range  else  depth-range-off  then ;
: walls-shade ( -- )
    28.7e 47.4e 22.4e  28.7e 47.4e -29e  74e 47.4e 22.4e  74e 47.4e -29e  light-doorway
    28.7e 47.4e -29e  28.7e 47.4e -80.4e  74e 47.4e -29e  74e 47.4e -80.4e  light-doorway
    26.3e 48e 30e  26.3e 48e -35.9e  72.1e 48e 30e  72.1e 48e -35.9e  light-doorway
    -55.1e 48e -14.9e  -55.1e 48e 39.6e  -100.9e 48e -14.9e  -100.9e 48e 39.6e  light-doorway
    -65.1e 48e 18.6e  -65.1e 48e 72.7e  -110.9e 48e 18.2e  -110.9e 48e 72.7e  light-doorway
    -65.1e 48e 61e  -65.1e 48e 91e  -110.9e 48e 61e  -110.9e 48e 91e  light-doorway ;

\ ---- the movies replayed (the extras): the encounter's three ----
: replay ( movie cutscene down? -- )   \ (Debilitas a figure; Hewie down kept out of sight)
    {: movie cut down :}  fiona hold  debilitas-kind figure-slot figure-in  yield
    movie scenes:unseen-movie cut load-scene
    down  hewie-in? and  hewie-cond-now @ down-cond = and if  hewie hold  hewie false shown  then
    panic-eased  if  -1 run-scene  then
    figure-slot figure-out  replay-over ;

\ ---- every frame ----
create cameras  $A , 0 , 0 ,   $B , 1 , 1 ,   $10 , 1 , 1 ,   $11 , 2 , -1 ,   $12 , 1 , 1 ,   $13 , 2 , -1 ,
create ahead  4 , 0 ,   5 , 2 ,   7 , 2 ,   6 , 3 ,   8 , 3 ,   9 , 4 ,   \ (area, exit)
: ways ( -- )
    0 exit-out  2 exit-out  3 exit-out  4 exit-out  cameras 6 cameras-at  ahead 6 ways-out
    fiona $15 entered?  fiona $16 entered? or if  below-room preload  then
    fiona stairs-area entered? if  3 map-page  then   fiona stairs-area left? if  2 map-page  then ;
: each-frame ( -- )
    sounds  events-held state? if  exit  then
    ways  window-bark  looks 2 hewie-looks-at  hewie-sniffs  encounter  spots  depth-by-camera  walls-shade ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act case
            $C of  [: saved-movie saved-cutscene false replay ;] start drop  endof
            $D of  [: failed-movie failed-cutscene false replay ;] start drop  endof
            $E of  [: caught-movie caught-cutscene true replay ;] start drop  endof
        endcase  exit
    then
    0 0 0 way-in  2 0 0 way-in  3 0 0 way-in  4 1 1 way-in
    came-by @ dup 3 = swap 4 = or if  3 map-page  then
    came-by @ hole-exit = if  fiona $23F 90 place-on-tri  2 -1 player-camera  ['] lands start drop  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-15 place!
