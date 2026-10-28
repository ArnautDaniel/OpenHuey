\ story/places/castle-1f-4.fs - Belli Castle's 1F hall by the puppet room ($F): the puppet room's
\ door with no knob (the broken marionette opens it), the floor plate that holds castle-1f-7's gate
\ up (the mechanism's sound while it rises and falls), the weak wall that gives at the third blow
\ (bugs, the refined antimony), the plate-key's slot (REST), the cracked plate key, the chairs, a
\ save clock - and at night, back with the castle's sketch, Riccardo met here. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-1f-4.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-1f-4
USING: engine game-state actors messages common vectors facts flag-names story-names stalker-names room-names floor-flags
    effects story.kit fiona.anims fiona.sounds scenes ;

own-reset

\ ---- what is said here ----
$F  constant no-knob-msg       \ "What? There's no knob... Now what do I do? Wait... There is a hook here... Maybe i..."
$10 constant pupa-cella-msg    \ "The words 'Pupa Cella' are written here... Puppet room... I believe that is what..."
$11 constant wont-open-msg     \ "It won't open. Maybe I need a key or something?"
$12 constant no-luck-msg       \ "No luck. Could this be some strange puzzle or just another weird contraption?"
$16 constant chair-msg         \ "Why in the world is there a chair here?"
$18 constant normal-chair-msg  \ "This is just a normal chair, right?"
$17 constant chair-again       \ "Doesn't make much sense to me... Not that anything in this place does."
2   constant same-chair-msg    \ "The design and type are identical to the previous chair. They are probably from the..."
3   constant plain-chair-msg   \ "Your average, run-of-the-mill chair. No unique markings that differentiate this fro..."
$15 constant floor-msg         \ "There is something strange about this part of the floor."
$13 constant weak-wall-msg     \ "The wall looks weak here. Maybe if I hit it hard enough I can break through it?"
$14 constant bugs-msg          \ "Bugs? Yuck! I hate bugs! Why me?"
1   constant noise-msg         \ "What's that noise? I swear I just heard something..."
6   constant locked-msg        \ "Darn! The door is locked. Now how I am going to get this open?"
4   constant slot-msg          \ "What could this be used for? There appears to be a slot of sorts. Could this be som..."
5   constant key-in-msg        \ "The plate-key is locked into place."
7   constant reforge-msg       \ "Looks like this isn't enough... I will need to re-forge this plate-key to be able to us..."
8   constant nothing-msg       \ "Nothing happened..."
0   constant riccardo-msg      \ "Exire Debilitas! Please excuse me, Miss. I am Riccardo, keeper of the castle..." (the scene's subtitles)

\ ---- its own state ----
own: no-knob-remarked   own: locked-remarked   own: chair-remarked
own: sparkle-shown      \ the refined antimony shows (the wall broken)
own: shaking            \ the wall shaking
own: shakes             \ blows the wall has taken (it gives at the third)
own: someone-on-plate   \ someone stands on the floor plate
own: plate-frames       \ the mechanism's frames up (0..24): castle-1f-7's gate rising, falling
own: plate-act          \ the mechanism's doing (one at a time: a new one replaces it)
own: hewie-noticed      \ Hewie has barked at the wall (once a visit)
own: hewie-at-spot      \ Hewie at his spot by the clock (his action there made)
own: marionette-step    \ the marionette's scene: done (her part waits for it)

\ ---- its places, its things ----
$A constant puppet-door-area   $D constant plate-area   $E constant chair-area   $10 constant chair-2-area
$F constant clock-area   $11 constant slot-area   1 constant to-exit-3-area
4 constant to-exit-0-area   5 constant from-exit-3-area   6 constant to-exit-3-2-area   7 constant from-exit-1-area
8 constant to-exit-1-area   9 constant to-exit-2-area
0 constant puppet-door-part   1 constant wall-part   2 constant broken-wall-part   3 constant exit-1-door-part
4 constant plate-key-part   5 constant key-in-slot-part
0 constant puppet-door-group   \ (the floor before the puppet door: solid, pushable - till it opens)
7 constant puppet-door   8 constant slot-door
0 constant flame-fx   1 constant antimony-fx
3 constant figure-slot   4 constant other-figure   \ (slot 3: Daniella about at night, Riccardo in his scene's replay)
0 constant came-in-sound   1 constant broken-mechanism-sound   2 constant broken-mechanism-stop
3 constant mechanism-sound   4 constant mechanism-stop   5 constant first-ambient   9 constant shake-sound
10 constant rubble-sound   11 constant creak-sound   12 constant reach-sound   $AC constant slam
$94 constant refined-antimony   $12 constant cracked-plate-key   $228 constant puppet-file
$15 constant castle-1f-7-room   $31 constant after-riccardo-room

\ ---- the floor plate and the mechanism ----
: plate-height ( at-once? -- )   \ (the plate, "fumi_yuka", down while someone stands on it: at once, or by 0.2 a frame)
    {: now :}  s" fumi_yuka" placed-here? 0= if  exit  then
    someone-on-plate @ if  0e  else  1e  then  {: F: goal :}
    now if  goal s" fumi_yuka" place-y object!  exit  then
    goal f0= if  -0.2e  else  0.2e  then  s" fumi_yuka" place-y object@ f+  0e fmax 1e fmin  s" fumi_yuka" place-y object! ;
: mechanism-sound-now ( -- id )  broken-marionette-used fact? if  broken-mechanism-sound  else  mechanism-sound  then ;
: mechanism-at ( id -- )  -72e 10e 15e room-sound ;
: mechanism-stops ( -- )
    mechanism-sound-now room-bank stop-sound
    broken-marionette-used fact? if  broken-mechanism-stop  else  mechanism-stop  then  mechanism-at  gate-rising fact-off ;
: mechanism ( -- )   \ (the original's act0D: the gate up as the plate is held, down as it is let go)
    begin  room-sounds-in? 0= while  yield  repeat
    gate-rising fact!  mechanism-sound-now mechanism-at
    floor-plate-pressed fact? 0= if
        begin  plate-frames @ 0<>  floor-plate-pressed fact? 0= and  while  -1 plate-frames +!  yield  repeat
        floor-plate-pressed fact? 0= if  mechanism-stops  then
    else
        begin  plate-frames @ 24 <>  floor-plate-pressed fact? and  while  1 plate-frames +!  3 frames  repeat
        floor-plate-pressed fact? if  mechanism-stops  then
    then ;
: mechanism-starts ( -- )  plate-act @ ?dup if  1- stop-act  then  ['] mechanism start 1+ plate-act ! ;
: plate-down ( who -- )   \ (someone onto the plate)
    floor-plate-pressed fact!  -1 someone-on-plate !  came-in-sound room-bank rot sound-by ;
defer the-trap
: on-the-plate ( -- )   \ (the original's area $D logic: the first onto it, the last off)
    someone-on-plate @ 0= if
        hewie plate-area in-area? if  hewie plate-down  mechanism-starts exit  then
        fiona plate-area in-area? if
            fiona plate-down
            broken-marionette-used fact?  heard-the-noise fact? 0= and  breaker-lowered fact? 0= and
            if  ['] the-trap start drop  else  mechanism-starts  then  exit
        then
        stalker plate-area in-area? if  stalker plate-down  mechanism-starts  then  exit
    then
    hewie plate-area in-area?  fiona plate-area in-area? or  stalker plate-area in-area? or 0= if
        floor-plate-pressed fact-off  0 someone-on-plate !  mechanism-starts
    then ;
:noname ( -- )   \ (act0A: she stands on it alone, the marionette used - down to castle-1f-7, the gate on her)
    fiona hold  stalkers-stay state!  castle-1f-7-room s" rooms" actor-named send room-preload  fiona wait-move
    15 screen:to-black fade wait-fade
    world-held state!  scene-locked state!  world-frozen state!  subscreen-locked state!  panic-eased  panic-held state!
    forced-exit take-exit ; is the-trap

\ ---- it comes in ----
: came-in ( exit -- )
    room-sounds
    broken-marionette-used fact? if  puppet-door-part hide-part  else  puppet-door-part show-part  then
    2 object-show  3 object-show  4 object-show
    puppet-door doors:door-locked? if  puppet-door-group solid-floor pushable or floor-on  then
    dup forced-exit = if  floor-plate-pressed fact!  -1 someone-on-plate !  24 plate-frames !
    else
        dup 3 <> if  gate-rising fact-off  floor-plate-pressed fact-off  then
        gate-rising fact? if  floor-plate-pressed fact!  -1 someone-on-plate !  12 plate-frames !
        else  floor-plate-pressed fact? if  -1 someone-on-plate !  24 plate-frames !
        else  0 someone-on-plate !  0 plate-frames !  then  then
    then  drop
    true plate-height
    c2f-3-flag1 fact? 0= if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then
    flame-fx 0 22.8e 14.65e 68.75e flame-at
    weak-wall-broken fact? if
        broken-wall-part show-part  -1 sparkle-shown !
        c1f-4-refined-antimony fact? 0= if  antimony-fx 140e -9e 94e flicker-at  then
    else  wall-part show-part  0 sparkle-shown !  then
    got-cracked-plate-key fact? if  plate-key-part hide-part  else  plate-key-part show-part  then
    slot-door doors:door-locked? if  key-in-slot-part hide-part  else  key-in-slot-part show-part  then
    1 $2300 reverb ;

\ ---- Riccardo, at night (back with the castle's sketch): the scene, then out to $31 ----
: riccardo-shots ( shot -- )   \ (the original's act11: the stalker's shadow off for two shots)
    case  $F of  st-kind-now @ true no-shadow  endof  $10 of  st-kind-now @ false no-shadow  endof  endcase ;
: meets-riccardo ( -- )   \ (act00)
    fiona hold
    this-room @ 443 stalker-to  stalker $1BB 0 place-on-tri  0 summon-take
    yield  other-figure figure-out
    6 scenes:opaque-when-movie 0 load-scene  ['] riccardo-shots at-shots  panic-eased
    if  riccardo-msg run-scene  then
    scene-locked state-off  after-riccardo-room s" rooms" actor-named send room-preload
    fiona send calm-reset  world-held state!  0 summon-take  figure-slot figure-out
    forced-exit take-exit ;
: back-by-the-trap ( -- )   \ (act0B: back from it - "What's that noise?")
    fiona hold  fiona wait-move  world-held state-off  camera-restart
    fiona rest-anim 0 start-blend  panic-held state-off  15 screen:from-black fade wait-fade
    noise-msg show  fiona wait-anim  fiona -1 idle  7 frames  wait-closed
    scene-locked state-off  subscreen-locked state-off  stalkers-stay state-off ;

\ ---- the puppet door: the broken marionette on its hook ----
: marionette-scene ( -- )   \ (act05)
    15 screen:to-black-2 with-music or fade
    5 scenes:unseen-movie 1 load-scene  wait-fade
    puppet-door-part hide-part  2 object-show  3 object-show  4 object-show
    hewie-in?  hewie-cond-now @ down-cond = and if  hewie false shown  then
    panic-eased  if  -1 run-scene  then
    puppet-door-group solid-floor pushable or floor-off
    s" hook" turn-y 0e object!  s" a_Puppet" turn-y 0e object!  s" Puppetdoor" turn-y 0e object!
    hewie true shown  -1 marionette-step !  15 fade-in ;
: marionette-used ( -- )   \ (act01)
    fiona hold  0 marionette-step !  stalkers-stay state!  fiona wait-move
    items.names:broken-marionette items send take-item  puppet-door unlock  broken-marionette-used fact!
    ['] marionette-scene start drop
    begin  marionette-step @ 0= while  yield  repeat
    fiona $1DD -90 -4.507e 15.509e place-at  fiona 0 idle  camera-restart  stalkers-stay state-off ;

\ ---- the weak wall ----
: shakes-it ( -- )   \ (act08: a blow - it shakes, dust)
    1 $FF 4 rumble-pad  -1 shaking !  shake-sound room-bank fiona sound-by
    139e -9e 91.5e dust  139e -9e 94.5e dust
    with-fiona-free  1 shakes +!  0 shaking ! ;
: creaks ( -- )   \ (act0F: it settles - then the antimony shows)
    41 frames  creak-sound 120e -10e 92e room-sound  46 frames  creak-sound 120e -10e 92e room-sound
    37 frames  creak-sound 120e -10e 92e room-sound
    c1f-4-refined-antimony fact? 0= if  antimony-fx 140e -9e 94e flicker-at  then  -1 sparkle-shown ! ;
: startled ( -- )   \ (act07: bugs!)
    fiona hold  fiona wait-move  25 frames  $5A threat  1 $FF 8 rumble-pad
    seized-sound common-bank fiona sound-by  fiona startled-anim play  fiona -1 idle  7 frames ;
: wall-gives ( -- )   \ (act1B: the third blow - it breaks)
    1 $FF 4 rumble-pad  weak-wall-broken fact!  shake-sound room-bank fiona sound-by  rubble-sound 140e -10e 92e room-sound
    wall-part hide-part  broken-wall-part show-part  139e -9e 91.5e dust  139e -9e 94.5e dust
    $80202020 1 2  137e -10e 90e  0.7e 0.5e 0.3e 0.125e  0e  shards
    $80202020 1 2  137e -10e 95e  0.7e 0.5e 0.3e 0.125e  0e  shards
    140e -9.5e 90e  18e 110e  0e -90e 0e  effects:flecks
    ['] creaks start drop  with-fiona-free  ['] startled start drop ;
: blows ( -- )
    weak-wall-broken fact?  shaking @ or if  exit  then
    140e -10e 94e 6e 5e struck-in? 0= if  exit  then
    shakes @ 2 = if  ['] wall-gives  else  ['] shakes-it  then  start drop ;

\ ---- what she does and says ----
: no-knob ( -- )
    fiona $1FD -90 -1 walk-move -16e 14e go-to  world-frozen state!  1 20e -10e 0e 0e shot
    no-knob-remarked @ if  pupa-cella-msg  else  -1 no-knob-remarked !  no-knob-msg  then  say
    16 frames  world-frozen state-off  shot-off  puppet-file give-file ;
: wont-open ( -- msg )  locked-remarked @ if  no-luck-msg  else  -1 locked-remarked !  wont-open-msg  then ;
: strains ( -- )   \ (act06: pushing at the puppet door)
    fiona hold  fiona strain-anim 0 start-blend  16 frames  fiona let-go-push-anim play
    wont-open show  fiona -1 idle  7 frames  wait-closed ;
: locked-remark ( -- )  fiona wait-move  wont-open say ;   \ (act0C)
: hit-the-door ( -- )   \ (act1C: a blow on the puppet door)
    slam common-bank fiona sound-by  with-fiona-free  ['] locked-remark start-spot ;
: chair ( -- )
    fiona wait-move
    chair-remarked @ if  chair-again say exit  then
    chair-msg say  fiona $C1 0 -1 walk-move 0e -84e go-to  normal-chair-msg say  -1 chair-remarked ! ;
: chair-2 ( -- )
    fiona wait-move  fiona 65e 107e turn-to-xz  chair-remarked @ if  same-chair-msg  else  plain-chair-msg  then  say ;
: the-floor ( -- )
    fiona wait-move  fiona crouch-down-anim play  fiona crouching-anim start-anim  16 frames
    fiona crouch-up-anim play  fiona -1 idle  floor-msg say ;
: the-wall ( -- )  fiona wait-move  weak-wall-broken fact? if  bugs-msg  else  weak-wall-msg  then  say ;
: antimony ( -- )  refined-antimony c1f-4-refined-antimony antimony-fx 140e 94e picks-up ;
: plate-key ( -- )
    fiona wait-move  fiona 131.392e 147.223e turn-to-xz  fiona pick-up-anim play  4 frames
    got-cracked-plate-key fact!  plate-key-part hide-part
    cracked-plate-key item-named  cracked-plate-key 1 give-item  cracked-plate-key show-item-tab  fanfare  obtained-2-msg say
    fiona up-from-pick-anim play ;
: tries-the-door ( -- )   \ (act13: exit 2's door, locked)
    fiona 2 to-door
    in-panic? chased? or if  fiona rattle-behind-slow-anim play  fiona -1 idle  7 frames
    else  fiona rattle-behind-anim play  fiona -1 idle  7 frames  locked-msg say  then ;
: the-slot ( -- )
    fiona $A6 -90 -1 walk-move 120e 154e go-to  world-frozen state!  1 15e -10e 0e 0e shot
    slot-door doors:door-locked? if  slot-msg  else  key-in-msg  then  say
    16 frames  world-frozen state-off  shot-off ;
: at-the-slot ( -- )  fiona $10F -90 -1 walk-move 120.5e 153e go-to  4 frames  fiona reach-out-anim play ;
: rest ( -- )   \ (act14: REST on the slot - the plate in, the door unlocked)
    fiona hold  stalkers-stay state!  at-the-slot
    reach-sound room-bank fiona sound-by  key-in-slot-part show-part  30 frames
    unlock-sound common-bank fiona sound-by  slot-door unlock
    fiona reach-back-anim start-anim  fiona look-point-move 0 -1  130e -20e 170e 0e move  yield
    60 frames  fiona -1 look-at  yield  fiona wait-anim  stalkers-stay state-off ;
: reforge ( -- )  fiona hold  at-the-slot  16 frames  fiona reach-back-anim play  reforge-msg say ;   \ (act16)
: wrong-word ( -- )   \ (act17)
    fiona hold  at-the-slot  reach-sound room-bank fiona sound-by  16 frames  fiona reach-back-anim play  30 frames  nothing-msg say ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0= 0= if  drop false exit  then
    case
        1 of  ['] marionette-used start drop  true  endof   $14 of  ['] rest start drop  true  endof
        $16 of  ['] reforge start drop  true  endof   $17 of  ['] wrong-word start drop  true  endof
        >r false r>
    endcase ;
: spots ( -- )
    fiona puppet-door-area in-area? if
        straining-now @  fiona held? 0= and if  ['] strains start drop
        else  broken-marionette-used fact? 0=  fiona -90 45 heading? and if  ['] no-knob unhurried offer  then  then
    then
    fiona chair-area in-area?  fiona 180 45 heading? and if  ['] chair unhurried offer  then
    fiona chair-2-area in-area?  fiona 0 45 heading? and if  ['] chair-2 unhurried offer  then
    fiona plate-area in-area? if  ['] the-floor unhurried offer  then
    fiona clock-area in-area?  fiona -90 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    sparkle-shown @  c1f-4-refined-antimony fact? 0= and if
        fiona 140e -9e 94e 5e 5e touches? if  ['] antimony calm-only offer  then
    else  fiona 140e 94e 6e near-xz?  fiona 90 50 heading? and if  ['] the-wall unhurried offer  then  then
    2 before-locked? if  ['] tries-the-door any-time offer  then
    fiona slot-area in-area?  fiona -90 50 heading? and if  ['] the-slot unhurried offer  then
    got-cracked-plate-key fact? 0=  fiona 131.392e -30e 147.223e 5e 5e touches? and if  ['] plate-key calm-only offer  then ;

\ ---- every frame ----
: ambience ( -- )
    room-sounds-in? 0= if  exit  then
    mechanism-sound-now -72e 10e 15e keep-sound
    first-ambient -17e 22e -9e ambient-sounds ;
: flame-stirs ( -- )  flame-fx 22.8e 10.65e 68.75e 8e -4e flame-stirred ;   \ (someone stepping into its draught)
: barks-at-wall ( -- )  140e -10e 94e hewie-barks-at ;
: hewie-looks ( -- )
    hewie-in?  hewie-cond-now @ well-cond = and 0= if  exit  then
    weak-wall-broken fact? 0=  hewie-noticed @ 0= and  chased? 0= and  hewie-cmdable-now @ and
    hewie 140e -10e 94e 30e 20e centred-in? and if  -1 hewie-noticed !  ['] barks-at-wall start drop  then
    hewie-at-spot @ 0= if
        chased? 0=  hewie-cmdable-now @ and  hewie -38.5e 0e -65e 5e 5e centred-in? and if  -1 hewie-at-spot !  $80 0 hewie-do  then
    else  hewie -38.5e 0e -65e 7e 5e centred-in? 0= if  0 hewie-at-spot !  then  then ;
create cameras  $B , 1 , 1 ,   $C , 0 , 0 ,                \ (area, set, path)
create entries  1 , 1 ,   0 , 0 ,   0 , 0 ,   0 , 0 ,      \ (by exit 0 .. 3: the camera)
: ways ( -- )
    3 exits-out
    player held? 0=  player to-exit-3-area in-area? and if  3 take-exit  then   \ (the forced way, no door)
    cameras 2 cameras-at
    hewie-found-in-wire fact?  found-file-208 fact? 0= and if
        fiona to-exit-3-2-area entered?  fiona to-exit-2-area left? or if  figure-slot figure-out  then
    then
    fiona to-exit-0-area entered? if  0 prepare-exit  then
    fiona from-exit-3-area left?  fiona to-exit-3-2-area entered? or if  3 prepare-exit  then
    fiona from-exit-1-area left?  fiona to-exit-1-area entered? or if
        hewie-found-in-wire fact?  found-file-208 fact? 0= and if  daniella-kind figure-slot figure-in  then
        1 prepare-exit
    then
    fiona to-exit-2-area left? if  2 prepare-exit  then ;
: puppet-door-hit ( -- )   \ (a blow on the puppet door, before the marionette)
    broken-marionette-used fact? if  exit  then
    fiona held? if  exit  then
    -28e 0e 15e 10e 15e struck-in? if  ['] hit-the-door start drop  then ;
: each-frame ( -- )
    events-held state? if  exit  then
    ambience  ways  on-the-plate  false plate-height  puppet-door-hit  blows  flame-stirs  hewie-looks  spots ;

\ ---- someone comes in ----
: player-came ( -- )  entries 4 ways-in ;
: fiona-came ( -- )
    got-castle-sketch fact?  c2f-3-flag1 fact? 0= and if
        0e sounds-scaled  world-held state!  ['] meets-riccardo start drop
        exit-1-door-part hide-part  7 object-show  8 object-show
    else  exit-1-door-part show-part  7 object-hide  8 object-hide  then
    came-by @ forced-exit = if
        fiona $221 -90 place-on-tri  1 1 player-camera  world-frozen state-off  heard-the-noise fact!
        ['] back-by-the-trap start drop
    then
    saturnus-hint-on fact?  c1f-14-flag2 fact? 0= and if  c1f-14-flag2 fact!  0 summon-take  then ;
: replay-riccardo ( -- )   \ (act19)
    fiona hold  puppet-door-part show-part  2 object-show  3 object-show  4 object-show  wall-part show-part
    $84 $60 $3C $50 $24 $2A $2A $46 room-tint  flame-fx 0 22.8e 14.65e 68.75e flame-at
    riccardo-kind figure-slot figure-in  yield  2 stalker-ready  yield
    6 scenes:opaque-when-movie 0 load-scene  ['] riccardo-shots at-shots  panic-eased
    if  riccardo-msg run-scene  then
    stalker-unloaded  figure-slot figure-out  replay-over ;
: replay-marionette ( -- )   \ (act1A)
    fiona hold  5 scenes:unseen-movie 1 load-scene
    puppet-door-part hide-part  2 object-show  3 object-show  4 object-show  flame-fx 0 22.8e 14.65e 68.75e flame-at
    panic-eased  if  -1 run-scene  then  replay-over ;
: replaying ( -- )   \ (the extras' list: its rows for this room's two scenes)
    hewie send part-from-fiona  replay-var var@ $B = if  ['] replay-riccardo  else  ['] replay-marionette  then  start drop ;
: person-in ( i -- )
    events-held state? if  fiona-p = if  replaying  then  exit  then
    dup fiona-p = if  player-came  fiona-came  then  drop ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went castle-1f-4 place!
' items-used castle-1f-4 items-here
