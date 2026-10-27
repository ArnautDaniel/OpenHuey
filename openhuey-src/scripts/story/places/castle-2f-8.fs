\ story/places/castle-2f-8.fs - the kiln room in Belli Castle's 2F ($24): an earth doll - a golem -
\ blocks the door; the word plate brings it to life (EMETH: it walks to the door's side) or to
\ dust (METH), or sets it dancing (SALTATIO); the old parchment, the kiln, the mark, a breakable,
\ the obsidian choker. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/castle-2f-8.fs (converted from the game's, not loaded). The golem is the room's own model -
\ before the kiln (part 0), at the door (part 1), crumbled (part 2): there is one.
IN: story.places.castle-2f-8
USING: engine game-state actors messages common vectors facts flag-names story-names room-names floor-flags
    effects story.kit fiona.anims fiona.sounds scenes ;

own-reset

\ ---- what is said here ----
0  constant golem-msg         \ "A huge doll made of earth blocks the door. There is a slot-shaped hole in its chest..."
3  constant golem-again       \ "It looks like there is a recess that something thin and flat will fit into."
1  constant cant-push-msg     \ "Well, I guess you never know until you try but I figured as much. There is no way someo..."
2  constant here-to-stay-msg  \ "Whatever mechanism moved it seems to have run its course. I think it's here to stay..."
6  constant equipment-msg     \ "What kind of equipment is this? It looks old, but someone has taken painstakingly good..."
7  constant kiln-msg          \ "Such tremendous heat... With heat like this, I'm sure it's used for refining."
8  constant mark-msg          \ "What a strange mark... There is something about it that makes me feel uncomfortable."
9  constant nothing-msg       \ "Nothing happened."
$A constant file-here-msg     \ "There is some kind of file here."
$B constant calmer-msg        \ "...... That got my mind off things. I feel a little better now."
$C constant staring-msg       \ "...... Just staring at this all day isn't going to get me out of this place any so..."

\ ---- its own state ----
own: golem-remarked
own: calmed                \ the dance has calmed her once (this visit)
own: sounds-on
own: hewie-noticed         \ Hewie has looked at what is about (once a visit)
own: pulse-step            \ the dance's colours

\ ---- its places, its things ----
6 constant golem-area   7 constant equipment-area   8 constant kiln-area   9 constant mark-area   $A constant mark-2-area
$B constant moved-golem-area   $C constant parchment-area   2 constant to-exit-0-area   3 constant to-exit-1-area
0 constant golem-part   1 constant golem-moved-part   2 constant golem-dust-part   3 constant whole-part   4 constant broken-part
5 constant parchment-part
0 constant before-kiln-group   1 constant before-kiln-stalkers   5 constant before-kiln-push   \ (the floor the golem stands on there ...)
2 constant at-door-group   3 constant at-door-stalkers   6 constant at-door-push           \ (... and by the door)
4 constant breakable-group
c2f-8-breakable-broken whole-part broken-part  34e 0e 29e 5e 8e  breakable: the-vase
the-vase shard-grey 0 7 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv  the-vase breakable-group frees-floor  the-vase $FF noise-from
0 constant choker-fx
$13 constant golem-door
8 constant golem-kind   3 constant golem-figure   \ (the scenes' golem: a figure in slot 3)
0 constant try-sound   9 constant room-loop-sound   $84 constant calmed-sound
$A0 constant old-parchment   $8D constant obsidian-choker   $75 constant choker-other   \ (items)
$16 constant emeth-seen   $17 constant meth-seen   \ (the system data's flags: the scenes seen)
6 constant debilitas-6    \ (the stalker after it: Debilitas)
2 constant kiln-light

: daylight ( -- )  $84 $60 $3C $50 $24 $2A $2A $46 room-tint ;

\ ---- it comes in ----
: golem-before-kiln ( on -- )   \ (its floor: barred, its ring pushable, the stalkers kept off)
    {: on :}  before-kiln-group barred-floor on if floor-on else floor-off then
    before-kiln-push pushable on if floor-on else floor-off then
    before-kiln-stalkers keeps-stalkers-off on if floor-on else floor-off then ;
: golem-at-door ( -- )
    at-door-group barred-floor floor-on  at-door-push pushable floor-on  at-door-stalkers keeps-stalkers-off floor-on ;
: came-in ( exit -- )
    drop  room-sounds
    golem-done fact? 0= if  true golem-before-kiln  golem-part show-part
    else
        golem-part hide-part
        golem-moved fact? if  golem-at-door  golem-moved-part show-part
        else  golem-crumbled fact? if  golem-dust-part show-part  then  then
    then
    got-old-parchment fact? 0= if  parchment-part show-part  then
    effects:rising-smoke
    the-vase as-it-stands
    golem-crumbled fact?  got-obsidian-choker fact? 0= and if  choker-fx 10e 1e -25e flicker-at  then
    c2f-3-flag1 fact? 0= if  daylight  then
    kiln-light 1.5e shadows-longer ;

\ ---- the kiln: its glow pulsing, its light flickering ----
fvariable kiln-phase
: kiln-objects ( xt -- )  >r  s" jimen" r@ execute  s" kama" r@ execute  s" sumi" r> execute ;
: glow-from-start ( addr len -- )  {: a n :}  0 1 a n 0e morph ;
: glow-on ( addr len -- )  {: a n :}  0 1 a n  kiln-phase f@ fcos 0.5e f* 0.5e f+  morph ;
: kiln ( -- )   \ (the original's act04: the three blending, the light up and back)
    pi fnegate kiln-phase f!  ['] glow-from-start kiln-objects
    begin
        ['] glow-on kiln-objects  kiln-light 1.1e light-brighter  2 frames
        kiln-phase f@ 0.20944e f+  fdup pi f> if  2e pi f* f-  then  kiln-phase f!   \ (on 12 degrees)
        ['] glow-on kiln-objects  kiln-light light-back  kiln-light 1.5e shadows-longer  yield
    again ;

\ ---- the word plate's three: the golem moved, crumbled, dancing; any other word nothing ----
: golem-scene ( movie cutscene -- )   \ (made ready under black, played: the golem the scene's figure meanwhile)
    {: movie cut :}  15 screen:to-black-2 with-music or fade
    movie scenes:keyed-movie cut load-scene  wait-fade
    golem-part hide-part  fiona false shown ;
: after-golem ( -- )   \ (the scene over: the stalker anew, the door open, the golem done)
    world-held state!  fiona $A8 -160 place-on-tri  camera-restart  golem-door unlock  golem-done fact!
    1 0 0 stage-music  false golem-before-kiln ;
: comes-alive ( -- )   \ (EMETH: it walks to the door's side - the original's act0A)
    fiona hold  stalkers-stay state!  golem-kind golem-figure figure-in  fiona wait-move
    golem-kind true no-shadow  1 0 golem-scene  $F $3C movie-key  golem-figure figure-on
    stalker-unloaded  debilitas-6 stalker-ready  panic-eased  if  -1 run-scene  then
    yield  $A object-hide  golem-figure figure-out
    after-golem  golem-at-door  golem-moved-part show-part
    fiona true shown  15 fade-in  emeth-seen resident-flag-set  stalkers-stay state-off ;
: crumbles ( -- )   \ (METH: to dust, the choker in it - the original's act0B)
    fiona hold  stalkers-stay state!  fiona wait-move
    3 2 golem-scene  $F $96 movie-key  golem-dust-part show-part  choker-fx 10e 1e -25e flicker-at
    stalker-unloaded  debilitas-6 stalker-ready  panic-eased  if  -1 run-scene  then
    yield  4 object-hide  5 object-hide  6 object-hide  7 object-hide  8 object-hide  9 object-hide  $A object-hide
    after-golem  fiona true shown  15 fade-in  meth-seen resident-flag-set  stalkers-stay state-off ;
: dances ( -- )   \ (SALTATIO: the golem dances to its music - it calms her, the first time - act0E)
    fiona hold  stalkers-stay state!  golem-kind golem-figure figure-in  fiona wait-move
    15 screen:to-black-2 with-music or fade  yield  $37 1e background  wait-fade
    no-pause state!  panic-eased  panic-held state!
    fiona $EE 180 10e -20e place-at  1 23e -12e 0e 5e shot  fiona false shown  golem-part hide-part
    golem-figure figure-on
    false golem-before-kiln  golem-figure $EF 10e -30e floor-at 0e figure-at  true golem-before-kiln
    15 screen:from-black fade
    golem-figure $1300 0 0 figure-play  0 pulse-step !
    0 $50 $24 $2A $2A $46 pulse-tint  10 frames  music send bgm-resume
    begin  golem-figure figure-came-round? 0= while
        pulse-step @ $50 $24 $2A $2A $46 pulse-tint  pulse-step @ 1+ dup $60 > if  drop 0  then  pulse-step !  yield
    repeat
    15 screen:to-black fade wait-fade
    daylight  golem-part show-part  golem-figure figure-out  fiona true shown  shot-off
    panic-held state-off  no-pause state-off  15 fade-in
    calmed @ if  staring-msg say
    else
        fiona hand-to-mouth-anim start-anim  calmer-msg say  fiona wait-anim
        $E4 ease-panic  fiona send calm-reset  3 avoid-prompt  calmed-sound common-bank fiona sound-by
        -1 calmed !  30 frames
    then
    stalkers-stay state-off ;
: nothing ( -- )   \ (another word: nothing happens)
    fiona hold  try-sound room-bank fiona sound-by  fiona wait-move  30 frames  nothing-msg say ;
: plate-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    case
        $A of  ['] comes-alive start drop  true  endof   $B of  ['] crumbles start drop  true  endof
        $E of  ['] dances start drop  true  endof   5 of  ['] nothing start drop  true  endof
        >r false r>
    endcase ;

\ ---- the movies replayed (the extras: the golem's two scenes) ----
: replay-done ( -- )  world-held state!  replay-over ;
: replay-alive ( -- )
    fiona hold  daylight  golem-kind golem-figure figure-in  yield  golem-kind true no-shadow
    1 scenes:keyed-movie 0 load-scene  $F $3C movie-key  golem-part hide-part  fiona false shown  panic-eased
    if  -1 run-scene  then  golem-figure figure-out  fiona true shown  replay-done ;
: replay-crumbles ( -- )
    fiona hold  daylight
    3 scenes:keyed-movie 2 load-scene  $F $96 movie-key  golem-part hide-part  fiona false shown  golem-dust-part show-part  panic-eased
    if  -1 run-scene  then  fiona true shown  replay-done ;
: replaying ( -- )   \ (the extras' list: its rows for this room's two scenes)
    hewie send part-from-fiona
    replay-var var@ case  4 of  ['] replay-alive  endof  7 of  ['] replay-alive  endof  $17 of  ['] replay-alive  endof
        $25 of  ['] replay-alive  endof  >r ['] replay-crumbles r>  endcase  start drop ;

\ ---- what she may do or say here ----
: golem ( -- )
    fiona $F5 180 -1 walk-move 10e -28e go-to  world-frozen state!  1 10e -40e 0e 0e shot
    golem-remarked @ if  golem-again  else  -1 golem-remarked !  golem-msg  then  say
    16 frames  world-frozen state-off  shot-off ;
: here-to-stay ( -- )  fiona wait-move  fiona 45e -30e turn-to-xz  here-to-stay-msg say ;
: looked-at ( anim msg -- )  {: anim msg :}  fiona anim start-anim  40 frames  msg say  fiona wait-anim ;
: equipment ( -- )  fiona wait-move  fiona 6e 4e turn-to-xz  look-low-anim equipment-msg looked-at ;
: the-kiln ( -- )  fiona wait-move  fiona 56.58e -1.5e turn-to-xz  look-close-anim kiln-msg looked-at ;
: the-mark ( -- )  fiona wait-move  look-at-anim mark-msg looked-at ;
: parchment ( -- )
    fiona $D6 0 -1 walk-move 7.826e -7.168e go-to  file-here-msg say
    fiona reach-out-anim play  4 frames  parchment-part hide-part  got-old-parchment fact!
    old-parchment item-named  old-parchment 1 give-item  old-parchment show-item-tab  fanfare  obtained-2-msg say
    fiona reach-back-anim play ;
: choker ( -- )
    fiona wait-move  fiona 10e -25e turn-to-xz  fiona pick-up-anim play  4 frames
    obsidian-choker 1 has-items? 0= if
        choker-fx effect-off  got-obsidian-choker fact!  obsidian-choker item-named
        obsidian-choker 1 give-item  obsidian-choker show-item-tab  fanfare  obtained-2-msg say
    else
        choker-other got-obsidian-choker choker-fx takes-up
    then
    fiona up-from-pick-anim play ;
: strains ( -- )   \ (pushing the golem: it won't move)
    fiona hold  fiona strain-anim 0 start-blend  16 frames  fiona let-go-push-anim play
    cant-push-msg show  fiona -1 idle  7 frames  wait-closed ;
: spots ( -- )
    straining-now @  fiona held? 0= and if  ['] strains start drop  exit  then
    golem-done fact? 0= if  fiona golem-area 50 faces-area? if  ['] golem unhurried offer  then
    else  fiona moved-golem-area 50 faces-area?  golem-moved fact? and if  ['] here-to-stay unhurried offer  then  then
    fiona parchment-area in-area?  got-old-parchment fact? 0= and if
        fiona 0 50 heading? if  ['] parchment calm-only offer  then
    else  fiona equipment-area 60 faces-area? if  ['] equipment unhurried offer  then  then
    fiona kiln-area in-area?  fiona 45 45 heading? and if  ['] the-kiln unhurried offer  then
    fiona mark-area in-area?  fiona mark-2-area in-area? or  fiona 90 45 heading? and if  ['] the-mark unhurried offer  then
    choker-fx effect @  fiona 10e -25e 4e near-xz? and if  ['] choker calm-only offer  then
    the-vase breaks? drop ;

\ ---- Hewie: what he sits by and looks at ----
: by-the-golem ( -- )  43.743e 0e -29.309e hewie-sits-by ;
: by-the-dust ( -- )  10e 0e -25e hewie-sits-by ;
: calm-free? ( -- flag )  hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and ;
: hewie-looks ( -- )
    calm-free? hewie-noticed @ 0= and if
        golem-done fact? if
            golem-moved fact? if
                hewie 43.743e 0e -29.309e 15e 20e centred-in? if  -1 hewie-noticed !  30 chance? if  ['] by-the-golem start drop  then  then
            else golem-crumbled fact?  hewie 0 in-area? 0= and if
                hewie 10e 0e -25e 10e 20e centred-in? if  -1 hewie-noticed !  30 chance? if  ['] by-the-dust start drop  then  then
            then then
        then
    then
    hewie-in?  danger-now @ 0= and  hewie-cmdable-now @ and  hewie 49.99e 0e -0.54e 18e 18e centred-in? and if
        49.99e 18e -0.54e hewie-scent   \ (the kiln's top)
    then ;

\ ---- every frame: its sound, its ways out, its cameras ----
: ambience ( -- )
    room-sounds-in? 0= if  exit  then
    room-loop-sound 57e 0e -2e  sounds-on @ if  keep-sound  else  room-sound  -1 sounds-on !  then ;
create cameras  4 , 0 , -1 ,   5 , 1 , -1 ,                  \ (area, set, path)
create ahead  to-exit-0-area , 0 ,   to-exit-1-area , 1 ,    \ (area, exit loaded ahead)
create entries  1 , -1 ,   0 , -1 ,                          \ (by exit 0, 1: the camera)
: ways ( -- )  2 exits-out  cameras 2 cameras-at  ahead 2 ways-out ;
: each-frame ( -- )
    events-held state? if  exit  then
    ambience  ways  hewie-looks  spots ;
: player-came ( -- )  entries 2 ways-in ;
: person-in ( i -- )
    ['] kiln start-only
    fiona-p = if  events-held state? if  replaying exit  then  player-came  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-8 place!
' plate-used castle-2f-8 items-here
