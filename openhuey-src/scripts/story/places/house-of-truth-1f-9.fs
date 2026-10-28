\ story/places/house-of-truth-1f-9.fs - the House of Truth's 1F hall of the pit ($92): the hole by
\ the wall where something was once set in - her remarks, the odd smell; Hewie sniffing it out (her
\ word, or her remark with him by: file 255); the Cane of Caduceus set in it - the scene: Lorenzo in
\ his last form, the door locked behind, the fans turning, the house coming down (debris falling -
\ the traps it springs, smoke rising - the drones and sparks at either side, their blows, the plates
\ rising under them); Lorenzo brought down - the scene: he falls, the house freed (file 256), the
\ doors as they are left; the pit's remark; "Oh no! It's locked!"; brought in by the forced way, the
\ scene there (out again by it); two flames; Hewie's bark at the hole, his look; the replays. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/house-of-truth-1f-9.fs
\ in the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.house-of-truth-1f-9
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit story.places.house-of-truth fiona.anims scenes ;

own-reset  truth-owns owns !

0 constant branch-msg      \ "A piece of a broken branch... Looks like something can be inserted here."
1 constant smell-msg       \ "There is an odd smell here. I can't put my finger on it."
2 constant inserted-msg    \ "I wonder if there used to be something inserted here before? There is this..."
3 constant scent-msg       \ "Hewie? Have you picked up the scent boy?"
4 constant sniff-msg       \ "So there was something inserted here before. Can you sniff it out for me?"
5 constant moaning-msg     \ "The whole building is moaning and groaning as if it were alive. This cane acted as..."
6 constant toast-msg       \ "If I fell in from here, I would be toast... Quite literally."
7 constant get-out-msg     \ "Something just doesn't seem right here. I'd better get out of here quick."
8 constant locked-now-msg  \ "Oh no! It's locked!"
9 constant naught-msg      \ "Your effort is for naught. How long do you plan to keep this up? Let's finish t..."
own: smell-remarked        \ the hole's first remark made (before the house came down: the branch; after: the cane)
own: pit-remarked          \ the pit's first remark made
own: hole-noticed          \ Hewie has barked at the hole (once a visit)
own: trap-set              \ the traps being set (the debris has fallen through)
own: left-on   own: right-on   \ someone in the left / right corner: its drone, its sparks (till they run down)
own: left-armed   own: right-armed   \ the left / right sparks' blows ready for whoever is in their way
own: drones-on   own: drone-on   own: fell   own: hazed
own: debris-out            \ the debris down (six at most: counted back as it falls through)
own: trap-spot             \ the next of five spots round the middle a trap is set on
own: left-pace   own: right-pace   \ how the left / right fan turns (fan-pace)
own: hewie-done            \ Hewie's walk to the hole over
0 constant cane-part   4 constant fan-0   5 constant fan-1
$11 constant player-off-floor   \ (group 0: her and the others kept off, the pit's floor)
3 constant hole-area   4 constant word-area   5 constant left-area   6 constant right-area
7 constant left-way   8 constant right-way
0 constant drone-high-sound   1 constant drone-sound   2 constant drone-left-sound   3 constant drone2-sound
5 constant corner-sound   6 constant drone-right-sound   $30 constant deep-drone   $47 constant deep-bank
$2E constant gesture-call
0 constant flame-0-fx   1 constant flame-1-fx   2 constant wisps-fx   3 constant haze-fx   4 constant embers-fx
5 constant left-sparks-fx   6 constant right-sparks-fx
3 constant lorenzo-movie   2 constant lorenzo-cutscene   1 constant fall-movie   0 constant fall-cutscene
7 constant arrival-movie   6 constant arrival-cutscene
$ED constant locked-door   $255 constant hole-file   $256 constant freed-file
: flames ( -- )  flame-0-fx 1 -36.4e 13.4e 54.4e flame-at  flame-1-fx 1 58.7e 10e -29.9e flame-at ;
: stones-fall ( -- )   \ (the ceiling coming down: stones of three kinds falling from 50)
    2 50e 0e 0e 0.5e story.base:stones-falling  7 50e 0e 0e 0.5e story.base:stones-falling
    5 50e 0.5e 0e 0.5e story.base:stones-falling ;
: debris-hidden ( -- )  $10 $A do  i object-hide  loop ;   \ ("keri0".."keri5": shown as they fall)
: haze-on ( -- )  effects:heat-haze haze-fx effect! ;   \ (the heat haze: its wave eased to a mark - +256 at once)
: embers ( flames? -- )  effects:pit-embers embers-fx effect! ;
: told-off ( k -- )  0 over tell-effect  0 swap effect ! ;   \ (an effect told 0 - off, fading - and forgotten)
: drone ( -- )  deep-drone story-volume or deep-bank bank-sound ;

\ ---- the fans "fan0", "fan1": turning at their speed (+0x30), eased by their pace ----
enum: fan-pace  stopping steadying racing at-base ;   \ (to a stop by 5% of the base, up to it by 2%, up to the top by 10%, at the base)
$30 constant fan-speed   $34 constant fan-base   $38 constant fan-top   $10 constant fan-turn
: fan$ ( k -- addr len )  if  s" fan1"  else  s" fan0"  then ;
: fan-set ( k -- )  {: k | a n :}  k fan$ to n to a  4e deg>rad fdup a n fan-speed object!  a n fan-base object!  20e deg>rad a n fan-top object! ;
: eased ( pace F: speed base -- speed' )
    case
        stopping of  0.05e f* f-  0e fmax  endof
        steadying of  fover fover 0.02e f* f+  fmin  fnip  endof
        at-base of  fnip  endof
        >r fdrop r>
    endcase ;
: fan-step ( k pace -- )
    {: k pace | a n :}  k fan$ to n to a  a n placed-here? 0= if  exit  then
    a n fan-speed object@
    pace racing = if  a n fan-base object@ 0.1e f* f+  a n fan-top object@ fmin  else  pace a n fan-base object@ eased  then
    fdup a n fan-speed object!
    a n fan-turn object@ f+  fdup pi f> if  2e pi f* f-  then  a n fan-turn object! ;
: fans ( -- )   \ (an activity: for good)
    0 fan-set  1 fan-set  begin  0 left-pace @ fan-step  1 right-pace @ fan-step  yield  again ;
: fans-on ( -- )  fan-0 object-show  fan-1 object-show  at-base left-pace !  at-base right-pace !  ['] fans start-only ;

\ ---- the plates "fumi0", "fumi1": up while their corner's drone runs, back down after ----
: plate$ ( k -- addr len )  if  s" fumi1"  else  s" fumi0"  then ;
: plate ( up? k -- )
    {: up k | a n :}  k plate$ to n to a  a n placed-here? 0= if  exit  then
    up if  a n place-y object@ f0> if  -0.2e a n place-y object+!  a n place-y object@ 0e fmax a n place-y object!  then
    else  a n place-y object@ 0.7e f< if  0.4e a n place-y object+!  a n place-y object@ 0.7e fmin a n place-y object!  then  then ;
: plates ( -- )  left-on @ 0 plate  right-on @ 1 plate ;

: came-in ( exit -- )
    room-sounds  flames
    ht1f-9-flag1 fact? if  cane-part show-part  fans-on  then
    tremor-warned fact? if  5 1e background  then
    forced-exit <> if
        tremors
        ht1f-9-flag2 fact? if  stones-fall  haze-on  true embers  then
    else  -1 drone-on !  then
    0 keeps-pursuers-off keeps-people-off or hole-floor or floor-on
    debris-hidden ;

\ ---- the house coming down: debris falling now and then; once it has fallen through, the traps ----
: keri$ ( k -- addr len )   \ (the debris' objects: "keri0".."keri5")
    case  0 of  s" keri0"  endof  1 of  s" keri1"  endof  2 of  s" keri2"  endof
          3 of  s" keri3"  endof  4 of  s" keri4"  endof  >r s" keri5" r>  endcase ;
: free-debris ( -- k | -1 )  6 0 do  i keri$ placed-shown? 0= if  i unloop exit  then  loop  -1 ;
fvariable dx  fvariable dy  fvariable dz
: drop-spot ( -- tri ) ( F: -- x y z )   \ (40..50 up, 25..40 out in any direction, over the floor)
    begin
        pi 2e pi f* rnd f* f-  40e 10e rnd f* f+ dy f!
        25e 15e rnd f* f+  fover fsin fover f* dx f!  fswap fcos f* dz f!
        dx f@ 0e dz f@ nav-tri  dup 0< while  drop
    repeat  dx f@ dy f@ dz f@ ;
: debris-falls ( -- )   \ (1..3 more, six down at most, each carrying the first "keri" free)
    debris-out @ 6 >= if  exit  then
    3 random 1+  6 debris-out @ - min  0 ?do
        free-debris dup 0< if  drop leave  then
        1 debris-out +!  drop-spot xyz>cells things-id send keri-drop
    loop ;
: falling ( -- )   \ (an activity: every 30 frames the stage on by 2 or 3 - at 2 or below a jolt, debris, the stage 150)
    {: | stage :}  20 to stage
    begin
        stage 3 < if  10 0.5e shaken  debris-falls  10 1e shaken  10 0.5e shaken  150 to stage  then
        30 frames  stage 2 -  50 chance? if  1-  then  to stage
    again ;
: fallen ( -- )   \ (each frame: the debris that fell through counted back)
    fell-now @ 0 ?do  -1 debris-out +!  -1 fell !  loop  0 fell-now ! ;
: trap-down ( -- )   \ (on the next of five spots round the middle, 72 degrees apart - a little random -
    1.0053e rnd f*  1.2566e trap-spot @ s>f f* f+  0.5027e f-  angle-wrap   \ 1.5..2 out, 2.1 up, smoke rising)
    1.5e 0.5e rnd f* f+  fover fsin fover f* dx f!  fswap fcos f* dz f!
    $3B  dx f@ f>cell  2.1e f>cell  dz f@ f>cell  things-id send trap-place
    trap-spot @ 2 + 5 mod trap-spot ! ;
: traps ( -- )   \ (an activity: the ground shaking, the wisps rising, three traps set)
    30 0.3e shaken  effects:wisp-column wisps-fx effect!  drone2-sound 0e 0e 0e room-sound
    30 1e shaken  3 0 do  trap-down  30 1e shaken  loop
    wisps-fx told-off  0 fell !  0 trap-set ! ;

\ ---- the drones at either side: the fan racing, sparks sprayed across - their blows - then running down ----
: sprays ( side k -- )  swap effects:spark-spray swap effect! ;
: corner ( on armed pace side sparks F: x -- )   \ (an activity)
    {: on armed pace side fx F: x :}
    side if  drone-right-sound  else  drone-left-sound  then  x 15e 0e room-sound
    racing pace !  60 frames  side fx sprays  15 frames  -1 armed !
    60 frames  stopping pace !  fx told-off  15 frames  0 armed !
    900 frames  steadying pace !  60 frames  0 on ! ;
: left-corner ( -- )  left-on left-armed left-pace 0 left-sparks-fx -70e corner ;
: right-corner ( -- )  right-on right-armed right-pace 1 right-sparks-fx 70e corner ;
: anyone-in? ( area -- flag )  {: a :}  fiona a in-area?  hewie a in-area? or  stalker a in-area? or ;
: struck ( who F: x -- )   \ (a blow from the side: her request, theirs a blow - kind 2, 5, threat 10)
    {: who F: x :}  x 14e 0e
    who fiona = if  2 xyz>cells 10e f>cell fiona send fiona-request exit  then
    2 5 0 xyz>cells who send blow-from ;
: blows ( way F: x -- )   \ (whoever is in the way, in their own move mode - not thrown, not reeling)
    {: way F: x :}
    fiona way in-area?  fiona-mode-now @ own-mode = and if  fiona x struck  then
    hewie way in-area?  hewie-mode-now @ 0= and if  hewie x struck  then
    stalker way in-area?  st-mode-now @ 0= and if  stalker x struck  then ;
: sides ( on armed area way F: x -- )
    {: on armed area way F: x :}
    on @ 0= if  area anyone-in? if  -1 on !  area left-area = if  ['] left-corner  else  ['] right-corner  then  start drop  then
    else  armed @ if  way x blows  then  then ;
: coming-down ( -- )   \ (the cane set in, Lorenzo not yet down)
    fallen
    trap-set @ 0=  fell @ and if  -1 trap-set !  ['] traps start drop  then
    left-on left-armed left-area left-way -70e sides
    right-on right-armed right-area right-way 70e sides ;
: corner-heard ( on area F: x z -- )   \ (before: someone stepping into a corner - its sound, once till all are out)
    {: on area F: x z :}
    on @ 0= if  area anyone-in? if  corner-sound room-bank x 0e z sound-there  -1 on !  then
    else  area anyone-in? 0= if  0 on !  then  then ;

\ ---- the cane set in: Lorenzo in his last form; the door locked behind ----
create tint-a  $7B c, $32 c, $2A c, $64 c, $24 c, $2C c, $36 c, $4E c,
create tint-b  $72 c, $96 c, $94 c, $6E c, $60 c, $56 c, $70 c, $3C c,
: tinted ( tint -- )  8 0 do  dup i + c@ swap  loop  drop  room-tint ;
: dark-look ( -- )  tint-a tinted  $8080805E 0 screen-blend ;
: till-frame ( n -- )  begin  cutscene-frame over < while  yield  repeat  drop ;
: till-shot ( shot -- )  begin  cutscene-frame cutscene-shot-at over <> while  yield  repeat  drop ;
: near-view ( -- )   \ (from the scene's frame 825: 1, 1..21, 80..120, 160)
    cutscene-frame 825 - s>f  1e  fover 0.3e f* 1e f+ 21e fmin  frot 0.7e f* 80e f+ 120e fmin  160e  depth-range ;
: lorenzo-cues ( -- )   \ (an activity: the scene's far view, the door, the light, the haze)
    depth-range-off  $21C till-frame
    begin  cutscene-frame $2ED < while  1e 1e 80e 160e depth-range  yield  repeat
    begin  cutscene-frame $348 < while  near-view  yield  repeat
    depth-range-off  4 till-shot  depth-range-off
    locked-door false open-mark  doors-redone  tint-b tinted  $9EEAEAFF 0 screen-blend
    5 till-shot  dark-look
    $47E till-frame  haze-on  -1 hazed !  6 256 + haze-fx tell-effect
    $492 till-frame  2 haze-fx tell-effect
    8 till-shot  0 haze-fx tell-effect ;
: lorenzo-scene ( -- )
    lorenzo-movie scenes:keyed-movie lorenzo-cutscene load-scene drop  ['] lorenzo-cues start drop
    $10 $C8 movie-key  panic-eased  naught-msg run-scene  ['] lorenzo-cues stop-doing ;
: lorenzo-comes ( -- )
    summoner-on state-off  stalker let-go  stalkers-stay state!  fiona hold
    fiona wait-move  fiona 0e 65e turn-to-xz
    15 screen:to-black-2 with-music or fade  wait-fade
    hewie-in? if  hewie let-go  then
    cane-part show-part  cane-of-caduceus spend  0 s" creatures" actor-named send creatures-clear  0 hazed !
    no-track 1e background  lorenzo-scene  dark-look
    fiona $7E 174 0.023e 24.438e place-at
    stalker let-go  stalker-unloaded  kind39-kind stalker-ready  partner activate
    this-room @ 133 stalker-to  stalker $85 30 -0.009e -44.982e place-at  stalker 0 0 camera-set
    this-room @ 151 0 hewie-to  hewie $97 180 place-on-tri
    fans-on
    locked-door false open-mark  doors-redone  locked-door lock
    ht1f-9-flag1 fact!  scene-5-pending state!  0 left-on !  0 right-on !
    ['] falling start drop
    hazed @ 0= if  haze-on  then  2 256 + haze-fx tell-effect  false embers
    7 1e background  15 screen:from-black fade  wait-fade
    $4D resident-flag-set  stalkers-stay state-off ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    4 = if  ['] lorenzo-comes fiona start-holding drop  true  else  false  then ;

\ ---- Lorenzo brought down: he falls; the house freed ----
: fall-scene ( -- )
    fall-movie scenes:keyed-movie fall-cutscene load-scene drop  $10 $C8 movie-key  panic-eased  -1 run-scene ;
: he-falls ( -- )
    no-track 1e background
    ['] traps stop-doing  ['] left-corner stop-doing  ['] right-corner stop-doing  ['] falling stop-doing
    0 left-on !  0 right-on !  0 left-armed !  0 right-armed !  0 trap-set !  0 fell !
    stalkers-stay state!  fiona hold
    15 screen:to-black-2 with-music or fade  fiona wait-move  wait-fade
    hewie-in? if  hewie let-go  then
    things-id send story-things-off  debris-hidden  at-base left-pace !  at-base right-pace !  embers-fx told-off
    fall-scene
    this-room @ 151 0 hewie-to  hewie $97 180 place-on-tri  50 trust+
    fiona $2B -90 54.575e -2.153e place-at  camera-restart  scene-5-pending state-off
    stalker let-go  stalker-unloaded  kind12-kind stalker-ready
    locked-door unlock  fan-0 object-show  fan-1 object-show
    $E6 lock  $E7 lock  $EB false open-mark  $EB lock
    stones-fall  drone  true embers
    15 fade-in  $4E resident-flag-set  freed-file give-file  $828D give-file  trust-file
    stalkers-stay state-off  tremors ;

\ ---- brought in by the forced way: the scene there; out again by it ----
: arrival-scene ( -- )
    arrival-movie scenes:keyed-movie arrival-cutscene load-scene drop  $10 $FF movie-key  panic-eased  -1 run-scene ;
: brought-in ( -- )
    fiona hold  hewie-in? if  house-of-truth-1f-8 -1 0 hewie-to  then  fiona wait-move
    fiona false shown  stones-fall  haze-on  arrival-scene
    world-held state!  stalker let-go  fiona true shown  wisps-fx told-off
    $4F resident-flag-set  forced-exit take-exit ;

\ ---- the hole: her remarks; Hewie sniffing it out (file 255) ----
: close-look ( msg -- )   \ (by the hole, the camera close on it, the world still)
    8 frames  fiona $31 0 0e 58e place-at  1 15e 10e 0e 5e shot  world-frozen state!
    8 frames  say  8 frames  world-frozen state-off  shot-off ;
: to-the-hole ( -- )   \ (Hewie's: over to it, sitting a while, his bark at her)
    hewie hold  hewie wait-move  hewie $31 -10 $204 walk-move 3e 55e go-to  hewie 0e 65e turn-to-xz
    hewie hewie.anims:sit-a-while-anim play  hewie fiona look-at  yield  hewie-bark  hewie wait-move
    -1 hewie-done !  hewie -1 look-at  yield ;
: sniffed-out ( -- )   \ (her eyes on him as he goes to it: file 255, his trust)
    fiona hewie look-at  yield  0 hewie-done !  ['] to-the-hole start drop
    begin  hewie-done @ 0= while  yield  repeat
    found-file-255 fact!  50 trust+  scent-msg say  fiona -1 look-at  yield
    hole-file give-file  $828C give-file  stalkers-stay state-off ;
: sends-him ( -- )   \ (her gesture: go there)
    stalkers-stay state!  fiona wait-move  fiona go-there-gesture-anim start-anim  fiona 1 wait-mark
    gesture-call common-bank fiona sound-by  fiona wait-anim  fiona -1 idle  7 frames  sniffed-out ;
: the-hole ( -- )
    fiona wait-move
    ht1f-9-flag2 fact? if
        smell-remarked @ 0= if  moaning-msg close-look  -1 smell-remarked !
        else  fiona 0e 65e turn-to-xz  fiona wait-move  get-out-msg say  then  exit
    then
    found-file-255 fact? if
        fiona 0e 65e turn-to-xz  fiona wait-move  sniff-msg close-look  exit
    then
    hewie-in?  hewie-cond-now @ down-cond <> and  stalker-in? 0= and if
        stalkers-stay state!  inserted-msg close-look  fiona $62 140 -10e 54e place-at  sniffed-out  exit
    then
    smell-remarked @ 0= if  branch-msg close-look  -1 smell-remarked !  else  smell-msg say  then ;
: go-there? ( -- flag )   \ (her word to Hewie by the hole, him well, no stalker)
    found-file-255 fact? 0=  fiona-cmd-now @ 0= and  hewie-in? and  hewie-cond-now @ down-cond <> and  stalker-in? 0= and
    fiona word-area in-area? and  fiona 50 0e 65e faces-point? and  hewie-breakable-now @ and ;

\ ---- what she may do or say here ----
: the-pit ( -- )
    fiona wait-move  fiona 0e 0e turn-to-xz  fiona wait-move
    pit-remarked @ 0=  over-music-f fact? 0= and if
        fiona look-low-anim start-anim  30 frames  toast-msg say  fiona wait-anim  -1 pit-remarked !
    else  get-out-msg say  then ;
: locked-now ( -- )  locked-now-msg show ;
: spots ( -- )
    fiona hole-area in-area?  fiona 0 50 heading? and if  ['] the-hole unhurried offer  then
    scene-5-pending state?  8 locked-request? and if  ['] locked-now any-time offer  then
    ht1f-9-flag2 fact?  fiona 0e 0e 0e 23e 4e touches? and  fiona 50 0e 0e faces-point? and if  ['] the-pit unhurried offer  then ;

\ ---- the movies replayed (the extras) ----
: replay-lorenzo ( -- )
    fiona hold  flames  lorenzo2-kind 3 figure-in  yield  3 figure-on  cane-part show-part
    lorenzo-scene  3 figure-out  replay-over ;
: replay-fall ( -- )
    fiona hold  lorenzo2-kind 3 figure-in  yield  3 figure-on  haze-on  debris-hidden
    fall-scene  3 figure-out  replay-over ;
: replay-arrival ( -- )
    fiona hold  flames  debris-hidden  fiona false shown  stones-fall  haze-on
    arrival-scene  fiona true shown  replay-over ;

\ ---- every frame ----
create cameras  1 , 0 , 0 ,  2 , 1 , 1 ,   \ (area, set, path)
create hewie-look  0.21e 0e 0.03e 33e 10e 0e look,
: drones ( -- )   \ (the cane set in: the drones overhead and about, kept up)
    ht1f-9-flag1 fact? 0= if  exit  then
    drones-on @ 0= if
        room-sounds-in? if  drone-high-sound 0e 50e 0e room-sound  drone-sound 0e 0e 0e room-sound  -1 drones-on !  then  exit
    then
    drone-high-sound 0e 50e 0e keep-sound  drone-sound 0e 0e 0e keep-sound  drone2-sound 0e 0e 0e keep-sound
    drone-left-sound -70e 15e 0e keep-sound  drone-right-sound 70e 15e 0e keep-sound ;
: barks-at-hole ( -- )  1.06e 0e 59.99e hewie-barks-at ;
: each-frame ( -- )
    drones
    drone-on @ if  deep-drone story-volume or keep-playing or deep-bank bank-sound  then
    1 exits-out  cameras 2 cameras-at
    go-there?  fiona held? 0= and if  ['] sends-him fiona start-holding drop  then
    ht1f-9-flag1 fact?  ht1f-9-flag2 fact? 0= and  st-cond-now @ down-cond = and  stalker-in? and if
        ht1f-9-flag2 fact!  panic-eased  ['] he-falls fiona start-holding drop
    then
    ht1f-9-flag1 fact?  ht1f-9-flag2 fact? 0= and if  coming-down
    else  left-on left-area -40e -40e corner-heard  right-on right-area 44e 37e corner-heard  then
    flame-0-fx -36.4e 13.4e 54.4e 8e -4e flame-stirred  flame-1-fx 58.7e 10e -29.9e 8e -4e flame-stirred
    found-file-255 fact? 0= if  hole-noticed 100 1.06e 0e 59.99e 26e 14e notices? if  ['] barks-at-hole start drop  then  then
    hewie-look 1 hewie-looks-at
    lorenzo2-kind slam-shake-of  kind39-kind slam-shake-of
    plates
    ht1f-9-flag2 fact?  drone-on @ 0= and if  drone  -1 drone-on !  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona
        replay-act case  $10 of  ['] replay-lorenzo  endof  $11 of  ['] replay-fall  endof  >r ['] replay-arrival r>  endcase
        start drop exit
    then
    0 0 0 way-in
    came-by @ forced-exit = if
        fiona $85 put-on-tri  0 0 player-camera  0e sounds-scaled  ['] brought-in fiona start-holding drop
    then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went house-of-truth-1f-9 place!
' items-used house-of-truth-1f-9 items-here
