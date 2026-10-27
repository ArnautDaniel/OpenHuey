\ story/places/castle-2f-3.fs - the bedroom in Belli Castle's 2F ($21), where Fiona wakes in a
\ sheet: Daniella brings her clothes, she changes, the stalker comes, she hides under the bed;
\ at night she hears the dog; later Hewie learns to attack here (the bedroom's own things: story/
\ places/bedroom.fs). A place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/
\ castle-2f-3.fs (converted from the game's, not loaded).
IN: story.places.castle-2f-3
USING: engine game-state actors messages common vectors facts flag-names story-names room-names stalker-names
    story.kit fiona.anims fiona.sounds hewie.anims stalker.anims scenes keys story.places.bedroom ;

own-reset  bedroom-owns owns !

\ ---- what is said here (the room's messages) ----
$C  constant sheets-msg          \ "I can't just walk around all day holding these sheets around me..."
$1F constant change-clothes-msg  \ "There is a change of clothes that maid has apparently laid out. Change clothes? YES NO"
$12 constant now-what-msg        \ "That worked... but now what should I do? If I could just find a map of this place..."
$C  constant dog-alright-msg     \ "I wonder if that dog will be alright?" (night)
$22 constant cant-sleep-msg      \ "I can't fall asleep. I am too worried about that dog."
$D  constant dog-night-msg       \ "If I just leave it like this, it may not make it through the night."
$26 constant sheets-pressed-msg  \ "Freshly starched and pressed sheets and bed covers now line the bed."
$27 constant boogieman-msg       \ "Oh come on, Fiona... It's not like you are going to find the Boogieman under there."
6   constant sunset-msg          \ "The sun is starting to set... And with it will come night..."
7   constant daylight-msg        \ "I should cover as much of this castle as I can while there is still some light left."
$11 constant hide-now-msg        \ "I have to find a place to hide!"
$F  constant what-now-msg        \ "Oh no! What should I do?! If I don't do something, that... thing will get me!"

\ ---- its own state ----
own: sunset-remarked
own: whine-frames          \ frames to Hewie's next whine (caught in the wire)
own: reaction-made         \ her start at seeing the maid, made this visit

\ ---- its places (the room's areas, by what is there) ----
$E constant maid-area   $10 constant clothes-area
$3C constant hall-door   $3D constant west-door   \ (its doors)

\ ---- the room's things ----
1 constant clothes-part   \ (its part: the clothes laid out)
15 constant whine-sound

: daylight-tint ( -- )  $84 $60 $3C $50  $24 $2A $2A $46  room-tint ;   \ (the afternoon's light: until she has the castle sketch)

\ ---- entering ----
: came-in ( exit -- )
    drop  room-sounds  tv-in  medallion-sparkle
    found-file-202 fact? 0= if  west-door lock  then
    c2f-3-flag1 fact?  hewie-found-in-wire fact? 0= and if  west-door lock  then
    found-file-201 fact?  found-file-202 fact? 0= and  clothes-part swap part
    0 ambient-next !  0 ambient-frames !  30 whine-frames !
    got-castle-sketch fact? 0= if  daylight-tint  then
    glass ;

create cameras  $17 , 0 , 0 ,   $18 , 1 , 1 ,   $19 , 0 , 0 ,   $1A , 2 , 2 ,   \ (area, set, path)
create entries  1 , 1 ,   0 , 0 ,                            \ (by exit 0, 1: the camera)
: ways ( -- )  2 exits-out  ahead 2 ways-out  cameras 4 cameras-at ;
\ ---- each frame ----
: doorway-light ( -- )  -2e 7.6e -47e  -2e 7.6e -67e  -2e 0e -47e  -2e 0e -67e  light-doorway ;

\ ---- her doings here (activities: she is held for each) ----
: remark-sheets ( -- )  fiona wait-move  sheets-msg say ;
: bed ( -- )   \ (a look under it, before the night)
    fiona wait-move  sheets-pressed-msg say
    fiona turn-round-anim play  fiona -1 idle  7 frames
    fiona crouch-down-anim play  fiona crouching-anim start-anim  16 frames
    fiona crouch-up-anim play  fiona -1 idle  boogieman-msg say ;
: window-day ( -- )
    fiona wait-move  fiona 45 turn
    sunset-remarked @ if  daylight-msg  else  -1 sunset-remarked !  sunset-msg  then  say ;
: remark-dog ( -- )  fiona wait-move  dog-alright-msg say ;
: remark-sleep ( -- )  fiona wait-move  cant-sleep-msg say ;
: window-dog ( -- )
    $80 45 -1.546e 59.728e at-spot  fiona reach-out-anim play  dog-night-msg say  fiona reach-back-anim play ;
: remark-hide ( -- )  fiona wait-move  hide-now-msg say ;

\ the medallion Hewie sniffed out, lying by the wall
$70 constant medallion-item
: medallion ( -- )  medallion-item c2f-3-medallion sparkle-fx -9e -11e picks-up ;

\ ---- what she may do here: offered by where she stands and which way she faces (the first
\ offered wins; in the sheet every thing only reminds her of it) ----
own: hiding-offered   \ the stalker coming: told to hide (her locked doors say so)
\ changing out of the sheet into the clothes the maid laid out (a scene: she remembers the car)
$D constant fiona-dressing   $12 constant dressing-figure-2   $18 constant dressing-figure-3   \ (the scene's figures, by kind)
3 constant dressing-movie   2 constant dressing-cutscene   8 constant car-memory-msg   \ "Wait a minute... I remember being in the car with..."
10 constant sheet-part   \ (her model's part: the sheet)
$202 constant clothes-file
$40A6 constant actions-doc-msg    \ "'Additional Actions' You are now able to perform the following new actions..."
$40AB constant costumes-doc-msg   \ "'Costumes' You are now able to change costumes..."
$A6 constant actions-doc   $AB constant costumes-doc
: dressing-shots ( shot -- )   \ (as the scene goes: the sheet off her, the clothes off the bed)
    case  2 of  0 cast-model sheet-part false actor-part-show  endof  5 of  clothes-part hide-part  endof  endcase ;
: change-clothes ( -- )
    fiona wait-move
    change-clothes-msg yes? 0= if  exit  then
    15 screen:to-black-2 with-music or fade
    fiona-dressing 3 figure-in  dressing-figure-2 4 figure-in  dressing-figure-3 5 figure-in  3 frames
    fiona-dressing true no-shadow
    dressing-movie scenes:unseen-movie dressing-cutscene load-scene  wait-fade
    ['] dressing-shots at-shots  panic-eased
    if  car-memory-msg run-scene  then
    world-held state!  fiona-dressing false no-shadow  3 figure-out  4 figure-out  5 figure-out
    0 dress  steps-in  clothes-part hide-part
    fiona $21 178 5.752e -41.411e place-at  1 1 her-camera  camera-restart
    hall-door unlock  west-door unlock  found-file-202 fact!  no-flee state-off
    15 fade-in  clothes-file give-file
    fiona rest-anim 10 play-blend  fiona 10 idle  now-what-msg say
    15 screen:to-dim fade wait-fade
    actions-doc-msg say  fg-3-flag3 fact? if  costumes-doc-msg say  then
    15 screen:from-dim fade wait-fade
    actions-doc filed
    fg-3-flag3 fact? if  got-doc-action fact!  costumes-doc filed  then ;
\ ---- hiding under the bed (the bedroom's): the tutorial's stalker sent in once she's under; out
\ by herself, its chase over ----
: tutorial-stalker ( -- )
    hiding-offered @ if  $10 448 stalker-to  0 stalker-order  hall-door unlock  west-door unlock  force-chased state!  then ;
: tutorial-over ( -- )  hiding-offered @ if  0 hiding-offered !  force-chased state-off  then ;
: hides ( -- )  ['] tutorial-stalker ['] tutorial-over hide-under-bed ;

\ ---- the stalker in the bedroom ----
$A constant stalker-search-motions
: search-odds ( -- pct )   \ (by the story's variable 2: how often he has come)
    2 var@ case  2 of 25 endof  3 of 50 endof  >r 0 r>  endcase ;
: searches-bed ( -- )  stalker-to-bed  $1C 180 -0.663e -24.791e finds-her-at ;   \ (he looks under the bed: found)
$602 constant tv-switch-anim   \ (his reach to the TV's switch)
: switches-tv-off ( -- )   \ (he turns the TV off - it breaks the more he does)
    stalker hold  stalker wait-move  0 door-for-him  1 door-for-him
    stalker $6A 135 -4e 13e walk-to
    stalker tv-switch-anim start-anim  18 frames
    tv-off-sound room-bank stalker sound-by  tv-static-sound room-bank stop-sound
    0 s" acoustics" actor-named send noise-setting  c2f-3-tv-examined fact-off  5 var@ 1+ 5 var!
    3 stalker-order  stalker stalker.anims:stand-anim play  16 frames ;
: search-or-not ( -- )   \ (as he comes in while she hides: whether he'll find her)
    {: | finds :}
    debilitas? if  stalker stalker-search-motions motions-for  then
    search-odds chance? to finds
    hiding-offered @ if  false to finds  0 hiding-offered !  force-chased state-off
    else
        2 creature-moving? if  true to finds  then
        4 stalker-alert? if  false to finds  then
    then
    finds if  ['] searches-bed start drop  else  120 summoner-sooner  then
    2 var@ 1+ 2 var! ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    tv-on?  2 creature-moving? 0= and  debilitas? and if
        120 summoner-sooner  ['] switches-tv-off start drop
    else  search-or-not  then ;

\ his first coming: she is told to hide (the evasion points, filed)
$40A5 constant evasion-doc-msg   \ "'Evasion Points' Press the button when near closets or beds to hide and escape..."
$A5 constant evasion-doc
: told-to-hide ( -- )
    fiona hold  stalkers-stay state!  fiona wait-move  fiona 0 shut-door
    fiona rest-anim 10 start-blend  what-now-msg show  fiona wait-anim  fiona 10 idle  wait-closed
    15 screen:to-dim fade wait-fade  evasion-doc-msg say  15 screen:from-dim fade wait-fade
    evasion-doc filed
    hall-door door-shut  hall-door lock  west-door lock  doors-redone
    force-chased state!  -1 hiding-offered ! ;
: clothed? ( -- flag )  found-file-202 fact? ;
: or-sheets ( xt -- xt' )  clothed? 0= if  drop ['] remark-sheets  then ;   \ (in the sheet: that, not this)
: night? ( -- flag )  c2f-3-flag1 fact?  hewie-found-in-wire fact? 0= and ;   \ (the dog heard, not yet found)
: by-the-bed ( -- )
    bed-area 180 60 facing? 0= if  exit  then
    c2f-2-flag1 fact? 0= if  ['] bed unhurried offer exit  then
    stalker-in? if  no-time-msg offer-remark-now  else  ['] hides calm-only offer  then ;
: spots ( -- )
    tv-area 180 90 facing? if  ['] tv or-sheets unhurried offer  then
    clothed? 0= if
        found-file-201 fact?  clothes-area 180 50 facing? and if  ['] change-clothes unhurried offer  then
        fiona at-locked-exit? if  ['] remark-sheets any-time offer  then
    else night? if
        fiona at-locked-exit? if  ['] remark-dog any-time offer  then
        bed-area 180 60 facing? if  ['] remark-sleep unhurried offer  then
        window-area 44 60 facing? if  ['] window-dog unhurried offer  then
    else  by-the-bed  then then
    fire-area -134 60 facing? if  ['] remark-fire unhurried offer  then
    portrait-1-area 90 60 facing? if  ['] portrait-1 or-sheets unhurried offer  then
    portrait-2-area 0 60 facing? if  ['] portrait-2 or-sheets unhurried offer  then
    portrait-3-area -90 60 facing? if  ['] portrait-3 or-sheets unhurried offer  then
    doll-area 0 50 facing? if  ['] doll or-sheets unhurried offer  then
    fiona birdcage-area 50 faces-area? if  ['] birdcage or-sheets unhurried offer  then
    scales-area -90 50 facing? if  ['] scales or-sheets unhurried offer  then
    chest-area 180 50 facing? if  fg-3-flag3 fact? if  ['] chest-costumes  else  ['] chest  then  or-sheets unhurried offer  then
    window-area 44 60 facing?  window-2-area 44 60 facing? or if
        c2f-3-flag1 fact? if  ['] window-night  else  ['] window-day  then  or-sheets unhurried offer
    then
    found-file-201 fact?  clock-area -90 50 facing? and if  ['] save-clock or-sheets calm-hewie-up offer  then
    hiding-offered @  fiona at-locked-exit? and if  ['] remark-hide any-time offer  then
    c2f-3-medallion-found fact?  c2f-3-medallion fact? 0= and  fiona -9e 1e -11e 5e 5e touches? and if
        ['] medallion calm-only offer
    then ;

\ ---- the maid: in the sheet, her first steps toward the door - Daniella comes with clothes ----
3 constant daniella   1 constant maid-movie   0 constant maid-cutscene   0 constant clothes-msg   \ "I've gathered some clothes for you..."
$201 constant first-file
: blown ( which -- )   \ (her hair as the scene goes: back, aside, straight)
    case  0 of  0e 0.1e -0.01e  endof  1 of  0.02e 0.1e 0e  endof  >r 0e 0.1e 0e r>  endcase  hair-force ;
: maid-shots ( shot -- )
    case  3 of  clothes-part show-part  endof  $15 of  0 blown  endof  $16 of  1 blown  endof  $17 of  2 blown  endof  endcase ;
: the-maid ( -- )
    fiona hold  fiona wait-move
    15 screen:to-black-2 with-music or fade
    daniella 3 figure-in  3 frames  0 true no-shadow
    maid-movie scenes:opaque-when-movie maid-cutscene load-scene  wait-fade
    glass-off  ['] maid-shots at-shots  panic-eased
    if  clothes-msg run-scene  then
    glass  wait-fade  world-held state!  0 false no-shadow  3 figure-out
    clothes-part show-part  2 blown  hall-door door-shut  doors-redone  hall-door lock
    fiona $10E 107 -27.455e -48.354e place-at  1 1 her-camera  camera-restart
    found-file-201 fact!  15 fade-in  first-file give-file ;
: maid-comes? ( -- )   \ (stepping into her way, before the clothes: once a visit)
    found-file-202 fact? found-file-201 fact? or  reaction-made @ or  if  exit  then
    fiona maid-area entered? 0= if  exit  then
    -1 reaction-made !  panic-eased  ['] the-maid start drop ;

\ ---- night: back with the castle's sketch - a dog crying out there (the forced way in, $80) ----
$204 constant night-file
: dog-whines ( -- )   \ (now and then, from the wire by the wall)
    begin  yield  whine-frames whine-due? if  whine-sound 228e 5e 78e room-sound  then  again ;
: night-falls ( -- )
    fiona hold  fiona wait-move  c2f-3-flag1 fact!  2 2 her-camera  1 prepare-exit
    fiona $A8 46  $A8 tri-center fswap fdrop  place-at
    hall-door door-shut  west-door door-shut  west-door lock  doors-redone  camera-restart
    15 fade-in  night-file give-file  30 whine-frames !  ['] dog-whines start-only ;

\ ---- Hewie, freed, learns to attack: the stalker in, Hewie told to go for him ----
5 constant trapped-movie   4 constant trapped-cutscene   7 constant saved-movie   6 constant saved-cutscene
$2A constant attack-tip-msg   \ "Push up on the right analog stick to command the dog to attack."
$14 constant saved-msg        \ "Thank you. You saved me. Come here... Come on, boy. What's your name? Hewie?..."
$40A7 constant dog-commands-msg   $A7 constant dog-commands-doc   \ "'Additional Dog Commands' Hewie has befriended you..."
$32 constant hard-warning-msg     \ "WARNING!! In HARD MODE, if your dog is ever 'critically wounded,' the game is ove..."
$207 constant hewie-file   $8274 constant hewie-file-2
$B constant tutorial-motions   $38 constant attack-call   \ (her call: go for him)
room-motion 1+ constant lying-low   room-motion 2 + constant rising   \ (the tutorial's motions: she lies low, rises to call)
$203 constant fleeing-anim     \ (the stalker's flight)
own: stalker-gone             \ he has run out (the tutorial's end)
0 constant attack-gesture
: trapped-shots ( shot -- )   \ (Fiona in front of the reflections, then not; a light up, then back)
    case  0 of  0 before-reflections-layer layer  endof  1 of  0 usual-layer layer  endof
        6 of  3 1.5e shadows-longer  endof  7 of  3 light-back  endof  endcase ;
: looks-about ( -- )   \ (the stalker, waiting: looking about now and then)
    stalker hold  stalker wait-move  stalker fiona look-at
    begin  stalker-p mine? while   \ (until another doing takes him: his flight)
        25 chance? if  stalker stalker.anims:look-about-anim 10 play-blend
        else  25 chance? if  stalker stalker.anims:look-about-3-anim 10 play-blend  then  then
        yield
    repeat ;
: runs-off ( -- )   \ (bitten, the stalker flees by the west door)
    stalker hold  stalker wait-move  stalker fiona look-at
    stalker walk-anim-move fleeing-anim -1  $C4 tri-center 0e  move  stalker wait-move
    0 exit-open? 0= if  stalker 0 open-door  then
    stalker walk-anim-move fleeing-anim -1  $76 -17e -83e floor-at  180 deg>yaw  move  stalker wait-move
    -1 stalker-gone ! ;
: hewie-saved ( -- )
    15 screen:to-black-2 with-music or fade
    saved-movie scenes:opaque-when-movie saved-cutscene load-scene  wait-fade
    hewie let-go  stalker let-go
    fiona 0 idle  fiona wait-anim  west-door door-shut  doors-redone  1 knock  0 true no-shadow  panic-eased
    if  saved-msg run-scene  then
    world-held state!  0 false no-shadow
    hewie activate  hewie well  hewie full-health  this-room @ 33 0 hewie-to  hewie $21 145 place-on-tri
    hewie-commandable state!  $29 0 hewie-do  hewie-hold-still  hewie-freed fact!
    fiona $13 -8 -16.325e -66.999e place-at  fiona 0 idle  fiona wait-anim
    0 prepare-exit  stalkers-blind state-off  stalker let-go  1 summon-take
    camera-restart  world-held state-off  traps-off state-off
    15 fade-in  hewie-file give-file
    15 screen:to-dim fade wait-fade  dog-commands-msg say  15 screen:from-dim fade wait-fade
    dog-commands-doc filed
    items-40-as-70 fact? if  15 screen:to-dim fade wait-fade  hard-warning-msg say  15 screen:from-dim fade wait-fade  then
    hewie-file-2 give-file ;
: told-to-attack ( frames -- )   \ (the tip shown until she gestures the attack, or the time is up)
    0 ?do  attack-tip-msg show  fiona-cmd-now @ attack-gesture = if  unloop exit  then  yield  loop ;
: attack-tutorial ( -- )
    fiona hold  traps-off state!  force-calm state!  fiona wait-move
    trapped-movie scenes:unseen-movie trapped-cutscene load-scene
    ['] trapped-shots at-shots
    this-room @ 176 0 hewie-to  this-room @ 20 stalker-to  panic-eased
    if  -1 run-scene  then
    world-held state!  scene-locked state-off  0 usual-layer layer  3 light-back  1 1 her-camera
    hewie hold  hewie activate  hewie well  hewie full-health  this-room @ 176 0 hewie-to  hewie $B0 180 -35.452e -8.852e place-at
    hewie-commandable state!  hewie-no-attack state!  stalkers-blind state!
    this-room @ 20 stalker-to  stalker activate  2 stalker-order  0 stalker-clock
    stalker $14 0 -15.648e -48.68e place-at  stalker full-health  ['] looks-about start drop
    fiona tutorial-motions motions-for  fiona use-motions  fiona $A 90 -39.575e -27.807e place-at  camera-restart
    fiona lying-low play  force-calm state-off  5 0 0 stage-music
    15 screen:from-black music-cut or fade  wait-fade
    240 told-to-attack
    fiona rising start-anim  16 frames  attack-call room-bank fiona sound-by
    hewie-no-attack state-off  $20 0 hewie-do  attack-tip-msg window send close-text  -1 shown-msg !
    fiona wait-anim  fiona stalker look-at
    fiona lying-low play
    begin  hewie-act-now @ $20 =  while  yield  repeat
    hewie-no-attack state!  west-door unlock  0 stalker-gone !  ['] runs-off start drop
    begin  stalker-gone @ 0= while  yield  repeat
    hewie-no-attack state-off  fiona -1 look-at
    hewie-saved ;

\ ---- the movies replayed (the extras' theatre: the room entered with its scripts held, the
\ movie by the story's variable $2A) - each played, nothing kept, back to the extras ----
: replay-done ( -- )
    costume-var var@ dress  3 figure-out  replay-over ;
: replay ( movie class cutscene subtitles -- )
    {: movie cls cut subs :}  fiona hold  fiona wait-move
    movie cls cut load-scene  wait-fade  panic-eased
    if  subs run-scene  then  replay-done ;
: replay-maid ( -- )
    1 wear  daniella 3 figure-in  3 frames  daylight-tint  glass  clothes-part show-part  0 true no-shadow
    ['] maid-shots at-shots  maid-movie scenes:opaque-when-movie maid-cutscene clothes-msg replay  0 false no-shadow ;
: replay-dressing ( -- )
    1 wear  fiona-dressing 3 figure-in  dressing-figure-2 4 figure-in  dressing-figure-3 5 figure-in  3 frames
    daylight-tint  clothes-part show-part  fiona-dressing true no-shadow  ['] dressing-shots at-shots
    dressing-movie scenes:unseen-movie dressing-cutscene car-memory-msg replay
    fiona-dressing false no-shadow  4 figure-out  5 figure-out ;
: replay-trapped ( -- )
    2 3 figure-in  3 frames  ['] trapped-shots at-shots
    trapped-movie scenes:unseen-movie trapped-cutscene -1 replay  0 usual-layer layer  3 light-back ;
: replay-saved ( -- )  0 true no-shadow  saved-movie scenes:opaque-when-movie saved-cutscene saved-msg replay  0 false no-shadow ;
: replaying ( -- )   \ (which movie: the extras' list - 0 the maid, 1 the dressing, $F Hewie trapped, $10 Hewie saved)
    replay-var var@ case
        0 of  ['] replay-maid  endof   1 of  ['] replay-dressing  endof
        $F of  ['] replay-trapped  endof   $10 of  ['] replay-saved  endof  >r 0 r>
    endcase  ?dup if  start drop  then ;


\ ---- she comes in: which way decides what happens ----
: came-forced ( -- )   \ (back with the sketch, the forced way in: the night)
    got-castle-sketch fact?  c2f-3-flag1 fact? 0= and 0= if  exit  then
    force-calm state-off  0e sounds-scaled  2 2 her-camera  fiona $A8 45 place-on-tri  world-held state!
    ['] night-falls start drop ;
forced-exit 1 or constant from-the-wire   \ (sent from the front garden, Hewie found in the wire)
: came-from-wire ( -- )   \ (from the front garden, Hewie found: the tutorial, once)
    c2f-3-flag2 fact? if  exit  then
    hewie-found-in-wire fact?  hewie-freed fact? 0= and 0= if  exit  then
    0e sounds-scaled  fiona $128 90 -38.951e -26.464e place-at  ['] attack-tutorial start drop  c2f-3-flag2 fact! ;
: stalker-hunting? ( -- )   \ (he is about in the castle, not down, Debilitas, not in here)
    stalker-told?  st-cond-now @ down-cond <> and  st-kind-now @ debilitas-kind = and
    st-room-now @ this-room @ <> and ;
: fiona-came ( -- )
    events-held state? if  hewie send part-from-fiona  replaying exit  then   \ (the extras: Hewie out of it, the movie played)
    entries 2 ways-in
    golem-done fact? 0=  c2f-2-flag1 fact? and  c2f-3-flag3 fact? 0= and if
        c2f-3-flag3 fact!
        came-by @ 0 =  in-panic? 0= and  stalker-hunting? and if  ['] told-to-hide start drop  then
    then
    came-by @ forced-exit = if  came-forced  then
    came-by @ from-the-wire = if  came-from-wire  then ;

: each-frame ( -- )
    events-held state? if  doorway-light exit  then   \ (a replay: only the doorway's light)
    ambience  fan  ways  show-tv  maid-comes?  hewie-sniffs  spots  hewie-looks  doorway-light ;
: person-in ( i -- )
    case  fiona-p of  fiona-came  endof  stalker-p of  stalker-came  endof  endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-2f-3 place!
