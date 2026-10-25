\ story/kit.fs - what a place is made of (docs/rooms-design.md): a room written as a program, not
\ as the original's event bytecode. A place is one actor while its room is played. It owns its
\ state, directs the characters only while it holds them, and its long doings are activities -
\ tasks it runs each frame, which let go of whatever they hold when they end or are stopped.
\ The places themselves: story/places/*.fs. A room without one still runs its converted
\ scripts (story/room.fs).
IN: story.kit
USING: engine game-state actors messages common vectors facts flag-names story-names stalker-names room-names effects ;

\ ---- the characters a place directs: Fiona, Hewie, the stalker in the game ----
3 constant #people
enum: person  fiona-p hewie-p stalker-p ;
: fiona ( -- id )  s" fiona" actor-named ;
: hewie ( -- id )  s" hewie" actor-named ;
: stalker ( -- id )  stalker-in-game ;
: person-id ( i -- id )  case  fiona-p of fiona endof  hewie-p of hewie endof  stalker-p of stalker endof  >r -1 r>  endcase ;
: person-of ( id -- i | -1 )
    dup 0< if  drop -1 exit  then
    #people 0 do  dup i person-id = if  drop i unloop exit  then  loop  drop -1 ;

\ ---- the place's state ----
8 constant #activities
48 constant #own   \ the place's own cells (`own:`)
state: place-state
  cell field this-room
  cell field came-by            \ the exit she came in by (-1: none)
  cell field desc               \ the place's description (its words: below)
  cell field entered
  cell field leaving            \ an exit is being taken: no more doings start
  cell field going              \ the exit asked for this frame (-1 none)
  cell field shown-msg          \ the message on screen this place showed (-1 none)
  cell field answer             \ its answer (the option chosen)
  cell field fading
  #activities cells field acts       \ each activity's task (0: none)
  #activities cells field act-xts    \ and the word it runs
  #people cells field holder         \ which activity holds each character (-1: none)
  #people cells field move-done      \ each one's move done, as it last told (moving)
  #people cells field anim-ended
  #people cells field in-here        \ each one in this room, as last seen
  #people /vec * field was-at        \ where each was at the last frame's end (areas crossed)
  #people cells field seen
  \ the offer this frame (the action prompt): its doing (an xt, 0 none), whether a message
  cell field offer-xt  cell field offer-msg
  \ what the others tell
  cell field danger-now  cell field panic-stage
  cell field fiona-mode-now  cell field fiona-doing-now  cell field fiona-cmd-now
  cell field hewie-act-now  cell field hewie-cond-now  cell field hewie-reach-now  cell field hewie-cmdable-now  cell field hewie-near-now
  \ the scene asked for (scenes.fs): how far it is, whether its movie is there, what to do at each new shot
  cell field scene-step  cell field scene-movie  cell field cue-xt
  8 cells field effects        \ the effects the place made (actors; 0 none), gone with it
  cell field tab-moving         \ the item's tab sliding (the sub screen tells when it's done)
  #own cells field own
end-state

\ a place's own cell, by name: `own: came-while-hidden`
variable owns
: own: ( "name" -- )
    owns @ #own >= abort" own: too many cells for a place"
    create owns @ ,  1 owns +!  does> @ cells own + ;
: own-reset ( -- )  0 owns ! ;   \ (each place file starts its own)

\ ---- a place's description: its words ----
begin-structure /desc
  field: d.came-in     \ ( exit -- ) the room entered (she or the one played came by exit)
  field: d.person-in   \ ( i -- ) a person came into the room while it is played
  field: d.frame       \ ( -- ) each frame: spots, exits, cameras, what runs on its own
  field: d.went        \ ( -- ) the room left
end-structure
create places  256 cells allot   places 256 cells 0 fill
: place-of ( room -- desc | 0 )  dup 0 256 within if  cells places + @  else  drop 0  then ;
: place! ( came-in person-in frame went room -- )   \ a room is a place from now on
    {: c p fr w room | d :}  here to d  /desc allot
    c d d.came-in !  p d d.person-in !  fr d d.frame !  w d d.went !  d room cells places + ! ;
: hook ( field-xt -- xt | 0 )  desc @ dup if  swap execute @  else  nip  then ;   \ (one of the place's words)
: run-desc ( field-xt -- )  hook ?dup if  execute  then ;
: run-desc-with ( x field-xt -- )  hook ?dup if  execute  else  drop  then ;

\ ---- facts: the story's, by name (story-names.fs) ----
: fact? ( n -- flag )  story-flag? ;
: fact! ( n -- )  story-flag-set ;
: fact-off ( n -- )  story-flag-clear ;
: state? ( n -- flag )  state-flag? ;
: state! ( n -- )  state-flag-set ;
: state-off ( n -- )  state-flag-clear ;
: in-panic? ( -- flag )  panic-stage @ panicking >= ;
: chased? ( -- flag )  danger-now @ chased = ;

\ ---- activities: long doings, each a task the place runs each frame ----
variable acting   -1 acting !   \ the activity being run (-1: none)
: act-slot ( k -- addr )  cells acts + ;
: holder-of ( i -- addr )  cells holder + ;
: tell-held ( i on -- )  swap person-id dup 0< if  2drop exit  then  send scripted ;
: let-go-person ( i -- )   \ (let go: standing, its own again)
    dup holder-of @ 0< if  drop exit  then  -1 over holder-of !  0 tell-held ;
: let-go-all ( k -- )   \ (what activity k holds)
    #people 0 do  i holder-of @ over = if  i let-go-person  then  loop  drop ;
: stop-act ( k -- )
    dup act-slot @ ?dup if  forth:kill  then  0 over act-slot !  let-go-all ;
: stop-all ( -- )  #activities 0 do  i stop-act  loop ;
: start ( xt -- k | -1 )   \ an activity started (none free: not started)
    leaving @ if  drop -1 exit  then
    #activities 0 do  i act-slot @ 0= if  dup i cells act-xts + !  spawn-held i act-slot !  i unloop exit  then  loop
    drop -1 ;
: running? ( xt -- flag )
    #activities 0 do  i act-slot @  over i cells act-xts + @ = and if  drop true unloop exit  then  loop  drop false ;
: start-only ( xt -- )  dup running? if  drop  else  start drop  then ;   \ (once, if it isn't already running)
: run-acts ( -- )
    #activities 0 do
        i act-slot @ ?dup if
            i acting !  resume 0= if  0 i act-slot !  i let-go-all  then  -1 acting !
        then
    loop ;

\ ---- holding a person: a place moves only those its activity holds ----
: hold ( id -- )   \ (taken from any other activity holding it: the original's forced actions)
    person-of dup 0< if  drop exit  then  acting @ 0< abort" hold: not in an activity"
    dup holder-of @ acting @ = if  drop exit  then
    acting @ over holder-of !  -1 tell-held ;
: let-go ( id -- )  person-of dup 0< if  drop exit  then  let-go-person ;
: held? ( id -- flag )  person-of dup 0< if  drop false exit  then  holder-of @ 0< 0= ;
: mine? ( i -- flag )  holder-of @  acting @ =  acting @ 0< 0= and ;

\ ---- waiting (in an activity) ----
: frames ( n -- )  0 ?do  yield  loop ;
: here? ( id -- flag )  person-of dup 0< if  drop false exit  then  cells in-here + @ ;

\ ---- moving those held (the story's moves: scripted-move, answered by `moving`) ----
create mv-at /vec allot
: move ( id kind a b F: x y z yaw -- )
    {: id kind a b F: x y z yaw | i :}  id person-of to i
    i 0< if  exit  then
    i mine? 0= if  ." place: a move of " id . ." not held by this activity" cr exit  then
    0 i cells move-done + !  0 i cells anim-ended + !
    kind a b  x f>cell y f>cell z f>cell yaw f>cell  id send scripted-move ;
: wait-move ( id -- )   \ its move done
    person-of dup 0< if  drop exit  then  dup here? 0= if  drop exit  then
    begin  dup cells move-done + @ 0= while  yield  repeat  drop ;
: wait-anim ( id -- )   \ its animation come round
    person-of dup 0< if  drop exit  then
    begin  yield  dup cells anim-ended + @  over person-id here? 0= or  until
    -1 swap cells move-done + ! ;
: floor-at ( tri F: x z -- x y z )  {: tri F: x z :}  x  tri tri-center fdrop fswap fdrop  z ;
: deg>yaw ( deg -- ) ( F: -- yaw )  s>f deg>rad angle-wrap ;
: walk-to ( id tri face F: x z -- )   \ to (x, z) on tri, then facing face (degrees)
    {: id tri face F: x z :}  id walk-move tri -1  tri x z floor-at  face deg>yaw  move  id wait-move ;
: run-to ( id tri face F: x z -- )
    {: id tri face F: x z :}  id run-move tri -1  tri x z floor-at  face deg>yaw  move  id wait-move ;
: walk-to-tri ( id tri -- )  {: id tri :}  id walk-path-move tri -1  tri tri-center 0e  move  id wait-move ;
: play ( id anim -- )  {: id anim :}  id anim-move anim -1  0e 0e 0e 0e  move  id wait-anim ;   \ (played out)
: play-blend ( id anim frames -- )  {: id anim n :}  id blend-move anim n  0e 0e 0e 0e  move  id wait-anim ;
: start-anim ( id anim -- )  {: id anim :}  id anim-move anim -1  0e 0e 0e 0e  move ;   \ (not waited for)
: start-blend ( id anim frames -- )  {: id anim n :}  id blend-move anim n  0e 0e 0e 0e  move ;
: start-blend8 ( id anim frames -- )  {: id anim n :}  id blend8-move anim n  0e 0e 0e 0e  move ;
: idle ( id v -- )  {: id v :}  id idle-anim-move 0 v  0e 0e 0e 0e  move ;   \ (her idle, variant v; -1 the plain one)
: turn-yaw ( id F: yaw -- )  {: id F: yaw :}  id turn-to-heading-move 0 -1  0e 0e 0e yaw  move  id wait-move ;
: turn ( id deg -- )  deg>yaw turn-yaw ;
: turn-to-xz ( id F: x z -- )
    {: id F: x z :}  id body-pos fswap fdrop {: F: mx mz :}  id  x mx f-  z mz f-  fatan2 angle-wrap  turn-yaw ;
: look-at ( id who -- )   \ (her head on who; -1 nobody)
    {: id who :}  id look-move who who person-of  0e 0e 0e 0e  move ;
: place-at ( id tri face F: x z -- )   \ put there at once (the triangle's height)
    {: id tri face F: x z :}  tri x z floor-at  face deg>yaw  {: F: px py pz yaw :}
    px f>cell py f>cell pz f>cell yaw f>cell  id send place ;
: place-on-tri ( id tri face -- )  {: id tri face :}  id tri face  tri tri-center fswap fdrop  place-at ;
: noclip ( id on -- )  swap send noclip ;   \ (through blocked floor)

\ ---- the message window ----
: window ( -- id )  s" window" actor-named ;
: show ( msg -- )  dup shown-msg !  window send show-text ;   \ (not waited for)
: wait-closed ( -- )  begin  shown-msg @ 0< 0= while  yield  repeat ;
: say ( msg -- )  show wait-closed ;
: ask ( msg -- n )  say answer @ ;   \ (the option chosen: 0 the first)
: yes? ( msg -- flag )  ask 0= ;

\ ---- the screen's fade: a kind of screen.fs's, and the stage music going with it or not ----
: screen ( -- id )  s" screen" actor-named ;
$40 constant with-music   \ (the stage music eased out going to black, back coming in: 90 frames)
: fade ( frames kind -- )   \ (kind: screen.fs's fade-kind, or'd with with-music)
    {: frames kind :}
    kind with-music and if  0 90  kind 1 and if  255  else  0  then  s" music" actor-named send music-op  then
    kind $F and 1 and if  world-held state-off  then   \ (coming in: the world let go)
    frames 0> fading !  frames kind $F and screen send fade ;
: wait-fade ( -- )  begin  fading @ while  yield  repeat ;
: fade-out ( frames -- )  screen:to-black-2 with-music or fade wait-fade ;   \ (a scene's: to black, the music away)
: fade-in ( frames -- )  screen:from-black with-music or fade wait-fade ;

\ ---- areas (the room's, C's facts): in one, or crossing into one since the last frame ----
: in-area? ( id area -- flag )
    over here? 0= if  2drop false exit  then  swap body-at vec@ area-in? ;
: crossed ( id area -- n )   \ 1 into it, -1 out of it, 0 neither
    {: id area | i :}  id person-of to i  i 0< if  0 exit  then
    i cells seen + @ 0= if  0 exit  then
    i /vec * was-at + vec@  id body-at vec@  area area-cross ;
: entered? ( id area -- flag )  crossed 1 = ;
: heading? ( id deg within -- flag )   \ facing deg (degrees), within so many
    {: id deg within :}  id body-yaw angle-wrap rad>deg  deg s>f f-  fabs
    fdup 180e f> if  360e fswap f-  then  within s>f f<= ;
: faces-area? ( id area within -- flag )   \ inside it, facing its middle
    {: id area within :}  id area in-area? 0= if  false exit  then
    area area-middle fswap fdrop  id body-pos fswap fdrop  {: F: ax az mx mz :}
    ax mx f-  az mz f-  fatan2 angle-wrap rad>deg  {: F: toward :}
    id body-yaw angle-wrap rad>deg toward f-  fabs  fdup 180e f> if  360e fswap f-  then  within s>f f<= ;
: touches? ( id F: x y z r h -- flag )   \ its body overlapping the upright cylinder at (x y z), radius r, height h
    {: id F: x y zz r h :}  id here? 0= if  false exit  then
    id body-at vec@ id body-dims {: F: cx cy cz cr ch :}
    cy ch 2e f/ f+  y h 2e f/ f+ f- fabs  h fabs ch fabs f+ 2e f/ f<=
    zz cz f- fdup f*  x cx f- fdup f* f+  r cr f+ fdup f* f<=  and ;
: at-locked-exit? ( id -- flag )   \ in an exit's area, its door locked (trying it)
    {: id :}  8 0 do
        i exit-door dup 0< 0= if  doors:door-locked? id i exit-area in-area? and if  true unloop exit  then  else  drop  then
    loop  false ;
: remember-people ( -- )
    #people 0 do
        i person-id dup here? if  body-at i /vec * was-at + vec-copy  -1 i cells seen + !
        else  drop  0 i cells seen + !  then
    loop ;

\ ---- the camera ----
: camera-actor ( -- id )  s" camera" actor-named ;
: camera-set ( id set path -- )  cam-char-setup ;           \ (whose camera: a set of the room's, its path)
: area-camera ( area set path -- )   \ whoever crosses into the area takes that camera
    {: area set path :}  #people 0 do  i person-id dup here? if  dup area entered? if  set path camera-set  else  drop  then  else  drop  then  loop ;
: follow ( id -- )  cam-on ;                                \ the camera follows who
: fixed-camera ( set path -- )  cam-setup  -1 cam-followed! ;   \ the room's own camera, following nobody
: shot ( on F: dist pitch yaw rise -- )   \ the story's own shot of the one followed (0: back)
    {: on F: d p y r :}  on  d 1000e f* f>s  p 1000e f* f>s  y 1000e f* f>s  r 1000e f* f>s  camera-actor send event-shot ;
: shot-off ( -- )  0 0e 0e 0e 0e shot ;
: camera-restart ( -- )  cam-restart ;

\ ---- exits ----
: rooms-actor ( -- id )  s" rooms" actor-named ;
: exit-door ( exit -- door | -1 )  this-room @ swap room-exit-door ;
: take-exit ( exit -- )  going ! ;   \ (after this frame)
: prepare-exit ( exit -- )  rooms-actor send exit-prepare ;   \ (the room behind it loaded ahead)

\ ---- doors (the doors actor's) ----
: doors-actor ( -- id )  s" doors" actor-named ;
: lock ( door -- )  doors-actor send lock ;
: unlock ( door -- )  doors-actor send unlock ;
: door-shut ( door -- )  0 doors-actor send door-opened ;   \ (its opened mark cleared)
: doors-redone ( -- )  doors-actor send redo-doors ;

\ ---- the room's parts (its mesh's own groups, and its doors' models': story.words door-bits) ----
$14 constant parts-base
: part ( n on -- )  swap parts-base + 1+ swap  2dup door-group!  room-group! ;
: show-part ( n -- )  true part ;
: hide-part ( n -- )  false part ;

\ ---- sounds ----
$40000000 constant story-volume   \ (a sound's id bit: scaled by the story's volume setting)
$80000000 constant keep-playing   \ (a sound's id bit: one already playing kept going, moved, not restarted)
6 constant room-bank              \ (the room's own sounds)
: room-sound ( id F: x y z -- )  {: id F: x y z :}  id story-volume or room-bank 0 0  x y z  sound-at ;
: keep-sound ( id F: x y z -- )  {: id F: x y z :}  id story-volume or keep-playing or room-bank 0 0  x y z  sound-at ;
: sound-by ( id bank who -- )  {: id bank who :}  who body-pos {: F: x y z :}  id bank 0 0  x y z  sound-at ;   \ (at a character)

\ ---- scenes (scenes.fs): made ready while the screen is black, then played ----
enum: scene-progress  no-scene-yet scene-loading scene-is-ready scene-running scene-ended ;
: scenes ( -- id )  s" scenes" actor-named ;
: load-scene ( movie class cutscene -- movie? )   \ (the movie and cutscene: room strings; class: scenes.fs's movie-class)
    {: movie cls cut :}  scene-loading scene-step !  this-room @ movie cls cut  scenes send scene-load
    begin  scene-step @ scene-loading = while  yield  repeat  scene-movie @ ;
: run-scene ( subtitles -- )   \ played through (or skipped); subtitles: a message, or -1
    dup 0< if  drop $FFFF  then  scene-running scene-step !  scenes send scene-run
    begin  scene-step @ scene-running = while  yield  repeat  0 cue-xt ! ;
: at-shots ( xt -- )  cue-xt ! ;   \ ( shot -- ) run as each new shot of the scene comes
: panic-eased ( -- )  in-panic? if  terrified s" panic" actor-named send panic-stage!  then ;   \ (a scene, a message: the panic down to terror)

\ ---- the story's character slots (shared with the converted rooms): figures for a scene ----
3 constant first-figure-slot
: figure-in ( kind slot -- )   \ a stalker by his kind, else an event figure (cast.fs), where it is
    {: kind slot | a :}  kind story.words:stalker-actor to a
    a 0< 0= if  a slot story.words:slot-actor !  kind slot story.words:slot-id !  slot a send stalker-load exit  then
    slot story.words:slot-actor @ 0< 0= if  exit  then
    slot story.words:cast-actor to a  a 0< if  exit  then
    a slot story.words:slot-actor !  kind slot story.words:slot-id !  kind a send cast-load ;
: figure-out ( slot -- )  story.words:slot-clear ;
: no-shadow ( kind on -- )   \ a character's shadows off for a scene (or back) - by its kind (Fiona 0)
    {: kind on :}  kind cast-model dup 0< if  drop exit  then  actor act.shadow-off  on 0<> 1 and swap l! ;

\ ---- items (the items actor's) ----
: items ( -- id )  s" items" actor-named ;
: give-file ( item -- )  -1 items send add-file ;   \ a file, with its sound if new
: give-item ( item n -- )   \ counted (the play record) and given
    progress pr.items-counted dup l@ 1+ swap l!  items send give-item ;
: item-full? ( item -- flag )  items:items-of 99 < 0= ;
: item-named ( item -- )  0 swap message-param! ;   \ (the next system message names it)
: subscreen ( -- id )  s" subscreen" actor-named ;
$8C constant fanfare-scene
$83 constant fanfare-sound   $85 constant fanfare-bank
: fanfare ( -- )  fanfare-sound fanfare-bank bank-sound ;   \ (the item's jingle)
: show-item-tab ( item -- )   \ (its tab slid in to show her: waited out)
    -1 tab-moving !  0 swap subscreen send tab-op  begin  tab-moving @ while  yield  repeat ;
$801B constant filed-message    \ "has been saved to your file."
: filed ( doc -- )  dup item-named  1 give-item  fanfare  filed-message say ;   \ a document filed, said so

\ ---- the panic, Fiona ----
: calm-her ( n -- )  fiona send fear-less ;
: ease-panic ( n -- )  s" panic" actor-named send calm-in ;
: recover ( n -- )  fiona send recovery-less ;
: costume ( n -- )  dup fiona send costume  progress pr.vars $26 + c! ;   \ (and the story's own note of it)

\ ---- Hewie ----
: hewie-in? ( -- flag )  hewie here? ;
: hewie-doing? ( act -- flag )  hewie-in? if  hewie-act-now @ =  else  drop false  then ;
: hewie-do ( a b -- )  hewie send do-action ;
: hewie-to ( room at how -- )  swap hewie send hewie-place ;

\ ---- the sub screen ----
: open-subscreen ( mode -- )   \ (a mode of the menu: 1 saving, 7 the costumes; waited while it is up)
    subscreen send menu-mode  subscreen-wanted state!  begin  yield  subscreen-wanted state? 0= until ;

\ ---- a save clock (the shared script $84): "Save current game?" - not with Hewie out of reach;
\ he holds still while she saves ----
$8014 constant clock-msg          \ "The clock etches out moments in time... Save current game? YES NO"
$8024 constant clock-alone-msg    \ (shown when Hewie can't reach her: a system message)
1 constant saving-menu
: hewie-reachable? ( -- flag )  hewie-in? 0=  hewie-reach-now @ or ;
: save-clock ( -- )
    stalkers-stay state!  fiona wait-move
    hewie-reachable? if
        hewie-in? if  hewie hold  then
        clock-msg yes? if  saving-menu open-subscreen  then
    else  clock-alone-msg show  then
    stalkers-stay state-off ;

\ ---- the stalker ----
: stalker-in? ( -- flag )  stalker here? ;
: stalker-order ( mode -- )  stalker dup 0< if  2drop exit  then  send stalker-order ;
: summoner ( -- id )  s" summoner" actor-named ;
: summoner-waits ( secs -- )  summoner send summoner-cooldown ;

\ ---- the room's own: its sounds, effects, look, objects, motions ----
: room-sounds ( -- )  0 $1B0C00 sound-stop-voices  room-bank sound-load ;   \ (the last room's voices stopped, this one's loaded)
: room-sounds-in? ( -- flag )  room-bank sound-loaded? ;
: effect ( k -- addr )  cells effects + ;
: effect! ( id k -- )   \ the place's effect k (the one there before gone)
    dup effect @ ?dup if  dup alive? if  kill  else  drop  then  then  effect ! ;
: effect-off ( k -- )  0 swap effect! ;
: effects-gone ( -- )  8 0 do  i effect-off  loop ;
: tell-effect ( v k -- )  effect @ dup alive? if  send effect-command  else  2drop  then ;
\ the room's tint (Tint_SetParams: the look's slot): the glow colour and the contrast colour, RGBA
$1F constant tint-slot
create tint-bytes 9 allot
: room-tint ( gr gg gb ga cr cg cb ca -- )   \ (its mode 0)
    0 tint-bytes 8 + c!  8 0 do  tint-bytes 7 i - + c!  loop  tint-slot tint-bytes 9 look-set ;
\ a reflecting window (Reflection): reflection k (0..3) on the quad's corners, its side panels a window's
$10000000 constant window-panels
: window-in ( k strength F: x0 y0 z0 .. x3 y3 z3 -- )
    {: k str :}  k effect @ 0= if  k effects:reflection k effect!  then  k false str window-panels or reflect! ;
: window-over-all ( k strength F: x0 y0 z0 .. x3 y3 z3 -- )   \ (a scene's: over the whole picture)
    {: k str :}  k effect @ 0= if  k effects:reflection k effect!  then  k true str window-panels or reflect! ;
\ the room's placed objects, by name
$14 constant turn-y
: object-turn ( addr len F: by -- )   \ turned about its upright, wrapping
    {: a n F: by :}  a n placed-here? 0= if  exit  then
    a n turn-y placed-f@ by f+  fdup pi f> if  2e pi f* f-  then  a n turn-y placed-f! ;
\ the room's motions for a character: a file of them (a room string) loaded, then used (ids from room-motion)
$8000 constant room-motion
: model-of ( id -- model | -1 )   \ (a person's model, as the cutscenes find it: by its kind)
    person-of case  fiona-p of 0 cast-model endof  hewie-p of 1 cast-model endof
        stalker-p of story.words:slot2-kind @ cast-model endof  >r -1 r>  endcase ;
: motions-for ( id file -- )  this-room @ story.strings:room-string  rot model-of dup 0< if  drop 2drop exit  then  motions-load drop ;
: use-motions ( id -- )  model-of dup 0< if  drop exit  then  true swap motions-use ;

\ ---- exits: taken by the one played, free, in its area, its door open and not held ----
: exit-open? ( exit -- flag )   \ (a doorway, or a door not locked with its opened mark)
    exit-door dup 0< if  drop false exit  then
    dup door-flags 1 and if  drop true exit  then
    dup doors:door-locked? if  drop false exit  then  doors:opened-bit doors:has? ;
: fiona-free? ( -- flag )   \ (on her own or down, nobody holding her)
    fiona-mode-now @ dup own-mode = swap down-mode = or  fiona-p holder-of @ 0< and ;
: exit-out ( exit -- )   \ (each frame: she goes when she can)
    {: ex :}  fiona-free? 0= if  exit  then
    fiona ex exit-area in-area? 0= if  exit  then
    ex exit-open? 0= if  exit  then
    ex exit-door doors:door-held? if  exit  then
    ex take-exit ;
: light-doorway ( F: x0 y0 z0 .. x3 y3 z3 -- )  light-blocker ;   \ (this frame: the characters' shadows kept off where the wall shades)

\ ---- the action prompt: what she may do here this frame, taken with the action button ----
\ when it is offered: at any time; not while she is chased ("I have to get away!"); only calm
enum: offer-when  any-time unhurried calm-only calm-hewie-up ;   \ (the last: the save clocks - not with Hewie down in a hard game; "no time" while busy)
$80000005 constant read-scene   \ (Fiona's offers: a message to read)
5 constant act-scene            \ (and a doing of the place's)
$8016 constant no-time-message  \ "I have to get away! I don't have time for this!"
: offer-closed? ( -- flag )   \ (one taken this frame already, the scenes locked, or she is held)
    offer-xt @ 0<>  offer-msg @ 0<> or  scene-locked state? or  fiona-p holder-of @ 0< 0= or ;
: busy? ( -- flag )   \ (chased, the stalker here, or another than Hewie held)
    chased?  stalker here? or  fiona-p holder-of @ 0< 0= or  stalker-p holder-of @ 0< 0= or ;
: offer ( xt when -- )   \ her doing xt (an activity, Fiona held for it), the first offered wins
    {: xt when :}  offer-closed? if  exit  then
    in-panic? when any-time <> and if  exit  then
    when unhurried = chased? and if  no-time-message offer-msg !  exit  then
    when calm-hewie-up = if
        hard-mode? hewie-cond-now @ down-cond = and if  exit  then
        busy? if  no-time-message offer-msg !  exit  then
    then
    xt offer-xt ! ;
: offer-remark-now ( msg -- )   \ a message to read, whatever goes on
    offer-closed? if  drop exit  then  offer-msg ! ;
: offer-remark ( msg when -- )   \ a message to read
    {: msg when :}  offer-closed? if  exit  then
    in-panic? when any-time <> and if  exit  then
    when unhurried = chased? and if  no-time-message to msg  then
    msg offer-msg ! ;
: offer-out ( -- )   \ (each frame, with no message up: told to her and the prompt shown)
    shown-msg @ 0< 0= if  exit  then
    offer-xt @ if  act-scene offer-xt @  else  offer-msg @ if  read-scene offer-msg @  else  exit  then  then
    fiona send offer  1 window send prompt ;
\ her doing, run: she is held for it
create spot-xts #activities cells allot
: spot-doing ( -- )  fiona hold  acting @ cells spot-xts + @ execute ;
: start-spot ( xt -- )
    ['] spot-doing start dup 0< if  2drop exit  then  cells spot-xts + ! ;

\ ---- the place, as an actor ----
: people-in ( -- )   \ (each person newly in the room: its arrival)
    #people 0 do
        i person-id dup 0< 0= if  body-room this-room @ =  else  drop false  then
        dup i cells in-here + @ 0= and if  i ['] d.person-in run-desc-with  then
        i cells in-here + !
    loop ;
: frame-start ( -- )  0 offer-xt !  0 offer-msg ! ;
behaviour placing
  on room-enter ( room exit -- )   \ (queued as the room came in)
      over room-id <> if  2drop exit  then
      came-by !  dup this-room !  place-of desc !  -1 entered !
      -1 going !  -1 shown-msg !  #people 0 do  -1 i holder-of !  loop
      self subscribe tick  self subscribe frame-end  self subscribe moving  self subscribe danger
      self subscribe panic  self subscribe fiona-doing  self subscribe hewie-doing
      frame-start  came-by @ ['] d.came-in run-desc-with  people-in  remember-people ;
  on tick ( -- )
      entered @ 0= if  exit  then
      people-in  frame-start  ['] d.frame run-desc  offer-out  run-acts ;
  on frame-end ( -- )
      entered @ 0= if  exit  then  remember-people
      going @ 0< 0= if  going @  -1 going !  -1 leaving !  rooms-actor send go-through  then ;
  on take-offer ( scene arg -- )
      leaving @ shown-msg @ 0< 0= or if  2drop exit  then
      swap case
          act-scene of  start-spot  endof
          read-scene of  show  endof
          nip
      endcase ;
  on text-closed ( msg answer -- )  answer !  shown-msg @ = if  -1 shown-msg !  then ;
  on fade-done ( -- )  0 fading ! ;
  on moving ( done ended events -- )
      drop  sender person-of dup 0< if  drop 2drop exit  then
      tuck cells anim-ended + !  cells move-done + ! ;
  on scene-ready ( movie? -- )  scene-movie !  scene-is-ready scene-step ! ;
  on scene-shot ( shot -- )  cue-xt @ ?dup if  execute  else  drop  then ;
  on scene-over ( -- )  scene-ended scene-step ! ;
  on tab-done ( -- )  0 tab-moving ! ;
  on danger ( level -- )  danger-now ! ;
  on panic ( stage level -- )  drop panic-stage ! ;
  on fiona-doing ( mode sub cond cmd doing -- )  fiona-doing-now !  fiona-cmd-now !  drop drop  fiona-mode-now ! ;
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      {: here act mode sub cond mood group cmdable near reach breakable way trust :}
      act hewie-act-now !  cond hewie-cond-now !  reach hewie-reach-now !  cmdable hewie-cmdable-now !  near hewie-near-now ! ;
  on room-leave ( -- )  -1 leaving ! ;
  on room-left ( -- )  scene-step @ scene-running = if  scenes send scene-stop  then  stop-all  effects-gone  ['] d.went run-desc  s" story" actor-named send room-done ;
  on story-stop ( -- )  stop-all  effects-gone  s" story" actor-named send room-done ;
end-behaviour

\ a place's actor for its room, ready to be entered (story.fs)
: place-spawn ( room -- id )  placing place-state rot room-name spawn ;
