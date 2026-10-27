\ story/kit.fs - what a place is made of (docs/rooms-design.md): a room written as a program, not
\ as the original's event bytecode. A place is one actor while its room is played. It owns its
\ state, directs the characters only while it holds them, and its long doings are activities -
\ tasks it runs each frame, which let go of whatever they hold when they end or are stopped.
\ The places themselves: story/places/*.fs. A room without one still runs its converted
\ scripts (story/room.fs).
IN: story.kit
USING: engine game-state actors messages common vectors facts flag-names story-names stalker-names room-names effects hewie-names hewie.anims floor-flags keys ;

\ ---- the characters a place directs: Fiona, Hewie, the stalker in the game ----
3 constant #people
enum: person  fiona-p hewie-p stalker-p ;
: fiona ( -- id )  s" fiona" actor-named ;
: hewie ( -- id )  s" hewie" actor-named ;
: figure-of? ( id -- flag )   \ (in one of the story's slots 3..5: a scene's figure - a stalker's actor too)
    dup 0< if  drop false exit  then
    3 0 do  i 3 + story.words:slot-actor @ over = if  drop true unloop exit  then  loop  drop false ;
: stalker ( -- id )   \ (the one with a body, not a scene's figure)
    #stalkers 0 do  i stalker# dup 0< 0= if  dup body?  over figure-of? 0= and if  unloop exit  then  then  drop  loop  -1 ;
: person-id ( i -- id )  case  fiona-p of fiona endof  hewie-p of hewie endof  stalker-p of stalker endof  >r -1 r>  endcase ;
: person-of ( id -- i | -1 )
    dup 0< if  drop -1 exit  then
    #people 0 do  dup i person-id = if  drop i unloop exit  then  loop  drop -1 ;

\ ---- the place's state ----
8 constant #activities
48 constant #own   \ the place's own cells (`own:`)
32 constant #room-effects   \ (RoomEffects: 0x20 slots)
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
  #activities cells field act-lost   \ one of its characters taken by another: it ends (the original's action replaced)
  #people cells field holder         \ which activity holds each character (-1: none)
  #people cells field move-done      \ each one's move done, as it last told (moving)
  #people cells field asked          \ a move asked of each since it was held (wait-move waits on that only)
  #people cells field anim-ended
  3 cells field figure-ended     \ each figure's (slots 3..5) animation come round, as last told
  3 cells field figure-done      \ ... its move done (an animal off where it was sent)
  #people cells field marks          \ each one's animation marks reached (its event keys), as last told
  #people cells field in-here        \ each one in this room, as last seen
  #people /vec * field was-at        \ where each was at the last frame's end (areas crossed)
  #people cells field seen
  \ the offer this frame (the action prompt): its doing (an xt, 0 none), whether a message
  cell field offer-xt  cell field offer-msg
  \ what the others tell
  cell field danger-now  cell field panic-stage  cell field panic-level-now   \ (her panic's level, 0..100)
  cell field onset              \ her panic's first frames (the room's doings wait: SubPlay's frame flag bit 3)
  cell field request-now        \ a creature's request of her this frame (fiona-requested)
  cell field unseen-now         \ she can't be seen (item $89 worn, still)
  cell field shook-now          \ she shook this frame (the stick swung or a button)
  cell field started-now        \ the action she started for Hewie this frame (-1 none: fiona-started)
  cell field straining-now      \ she strains at something that won't move (fiona-straining)
  cell field fiona-mode-now  cell field fiona-doing-now  cell field fiona-cmd-now
  cell field struck-by-now      \ who struck or caught her (-1 nobody: her reaction her own)
  cell field hewie-act-now  cell field hewie-cond-now  cell field hewie-reach-now  cell field hewie-cmdable-now  cell field hewie-near-now
  cell field hewie-breakable-now   \ he may be broken off what he does (Hewie_MayBreakOff)
  cell field hewie-way-now         \ his side of a split room (one-half / other-half: 1, 2)
  cell field hewie-hp-now          \ his health, 0..100
  \ the scene asked for (scenes.fs): how far it is, whether its movie is there, what to do at each new shot
  cell field scene-step  cell field scene-movie  cell field cue-xt
  #room-effects cells field effects   \ the effects the place made (actors; 0 none), gone with it (RoomEffects: 32)
  cell field tab-moving         \ the item's tab sliding (the sub screen tells when it's done)
  \ the stalker in the game, as he tells (stalker-doing): his room, condition, kind, alert (his mode)
  cell field st-mode-now        \ his move mode (hewie-mode's names: free, off screen, struck, ...)
  cell field st-room-now  cell field st-cond-now  cell field st-kind-now  cell field st-alert-now  cell field st-came-now  cell field st-age
  cell field st-heard           \ he has told us at all (in the game: st-age frames ago)
  \ the creatures, as they tell (creature-doing): each one's room, how it moves, frames since
  #creatures cells field cr-room  #creatures cells field cr-mode  #creatures cells field cr-age
  cell field found-her          \ a hiding place's: the stalker has found her there (she must flee)
  cell field came-while-hidden  \ ... he came in while she hid (gone again: the prompt anew)
  #own cells field own
end-state

\ a place's own cell, by name: `own: came-while-hidden`
variable owns
variable breakables   \ (a place's breakables so far: each a target number, for a stalker's blow)
: own: ( "name" -- )
    owns @ #own >= abort" own: too many cells for a place"
    create owns @ ,  1 owns +!  does> @ cells own + ;
: own-reset ( -- )  0 owns !  0 breakables ! ;   \ (each place file starts its own)

\ ---- a place's description: its words ----
begin-structure /desc
  field: d.came-in     \ ( exit -- ) the room entered (she or the one played came by exit)
  field: d.person-in   \ ( i -- ) a person came into the room while it is played
  field: d.frame       \ ( -- ) each frame: spots, exits, cameras, what runs on its own
  field: d.went        \ ( -- ) the room left
  field: d.item        \ ( who ev var value -- handled? ) an item used here starting one of its doings (items-here)
end-structure
create places  256 cells allot   places 256 cells 0 fill
: place-of ( room -- desc | 0 )  dup 0 256 within if  cells places + @  else  drop 0  then ;
: place! ( came-in person-in frame went room -- )   \ a room is a place from now on
    {: c p fr w room | d :}  here to d  /desc allot
    c d d.came-in !  p d d.person-in !  fr d d.frame !  w d d.went !  0 d d.item !  d room cells places + ! ;
: hook ( field-xt -- xt | 0 )  desc @ dup if  swap execute @  else  nip  then ;   \ (one of the place's words)
: items-here ( xt room -- )  cells places + @ d.item ! ;   \ (its own item uses: the plates' words, ...)
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
: stop-doing ( xt -- )   \ (the activity running xt stopped, if one is)
    #activities 0 do  i act-slot @  over i cells act-xts + @ = and if  i stop-act  then  loop  drop ;
: start-only ( xt -- )  dup running? if  drop  else  start drop  then ;   \ (once, if it isn't already running)
: run-acts ( -- )
    #activities 0 do
        i cells act-lost + @ if  0 i cells act-lost + !  i stop-act  then
        i act-slot @ ?dup if
            i acting !  resume 0= if  0 i act-slot !  i let-go-all  then  -1 acting !
        then
    loop ;
: lost ( -- )  -1 acting @ cells act-lost + !  begin  yield  again ;   \ (this activity ends: run-acts stops it)

\ ---- holding a person: a place moves only those its activity holds ----
: hold ( id -- )   \ (taken from any other activity holding it: the original's forced actions)
    person-of dup 0< if  drop exit  then  acting @ 0< abort" hold: not in an activity"
    dup holder-of @ acting @ = if  drop exit  then
    acting @ over holder-of !  0 over cells asked + !  -1 tell-held ;   \ (nothing asked of it yet)
: let-go ( id -- )  person-of dup 0< if  drop exit  then  let-go-person ;
: held? ( id -- flag )  person-of dup 0< if  drop false exit  then  holder-of @ 0< 0= ;
: mine? ( i -- flag )  holder-of @  acting @ =  acting @ 0< 0= and ;

\ ---- waiting (in an activity) ----
: frames ( n -- )  0 ?do  yield  loop ;
: here? ( id -- flag )   \ (a person: in this room as last seen, and still in the game; a figure: in this room)
    dup figure-of? if  dup body? if  body-room room-id =  else  drop false  then  exit  then
    dup person-of dup 0< if  2drop false exit  then  cells in-here + @ if  body?  else  drop false  then ;

\ ---- moving those held (the story's moves: scripted-move, answered by `moving`) ----
create mv-at /vec allot
: move ( id kind a b F: x y z yaw -- )
    {: id kind a b F: x y z yaw | i :}  id person-of to i
    i 0< if  exit  then
    i mine? 0= if  lost  then   \ (taken by another doing: this one is over)
    0 i cells move-done + !  0 i cells anim-ended + !  0 i cells marks + !  -1 i cells asked + !
    kind a b  x f>cell y f>cell z f>cell yaw f>cell  id send scripted-move ;
: keep ( id -- )  person-of dup 0< if  drop exit  then  mine? 0= if  lost  then ;   \ (still ours, else this doing is over)
: wait-move ( id -- )   \ its move done
    person-of dup 0< if  drop exit  then  dup person-id here? 0= if  drop exit  then
    dup cells asked + @ 0= if  drop exit  then   \ (its own doings aren't ours to wait for)
    begin  dup cells move-done + @ 0= while  yield  repeat  drop ;
: wait-anim ( id -- )   \ its animation come round
    person-of dup 0< if  drop exit  then
    begin  yield  dup cells anim-ended + @  over person-id here? 0= or  until
    -1 swap cells move-done + ! ;
: went-round? ( id -- flag )   \ its animation come round this time (its motion's $20 key: the original's cond 0x48)
    person-of dup 0< if  drop false exit  then  cells anim-ended + @ 0<> ;
: wait-mark ( id bits -- )   \ its animation come to a mark (an event key of it)
    {: id bits | i :}  id person-of to i  i 0< if  exit  then
    begin  yield  i cells marks + @ bits and  id here? 0= or  until ;
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
: on-floor-of? ( id flags -- flag )   \ standing on a triangle with any of these flags
    over here? 0= if  2drop false exit  then  swap body-tri dup 0< if  2drop false exit  then  nav-flags and 0<> ;
: throws-her ( id -- )  false fiona send throw-down ;   \ she is thrown down by it
: put-on-tri ( id tri -- )   \ put on its middle at once, its heading kept
    {: id tri | F: x F: y F: z :}  tri tri-center to z to y to x
    x f>cell y f>cell z f>cell id body-yaw f>cell  id send place ;
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
$40 constant with-music   \ (the stage music eased with it, over 90 frames)
$80 constant music-cut    \ (the stage music cut with it, at once)
: fade-music ( kind -- )   \ (only to black and back: silent going out, full coming in)
    {: kind :}
    kind music-cut and if  1  else  kind with-music and if  90  else  exit  then  then
    kind $F and case  screen:to-black-2 of  0  endof  screen:from-black of  255  endof  2drop exit  endcase
    0 -rot  s" music" actor-named send music-op ;
: fade ( frames kind -- )   \ (kind: screen.fs's fade-kind, or'd with with-music or music-cut)
    {: frames kind :}  kind fade-music
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
    area area-middle drop fswap fdrop  id body-pos fswap fdrop  {: F: ax az mx mz :}
    ax mx f-  az mz f-  fatan2 angle-wrap rad>deg  {: F: toward :}
    id body-yaw angle-wrap rad>deg toward f-  fabs  fdup 180e f> if  360e fswap f-  then  within s>f f<= ;
: touches? ( id F: x y z r h -- flag )   \ its body overlapping the upright cylinder at (x y z), radius r, height h
    {: id F: x y zz r h :}  id here? 0= if  false exit  then
    id body-at vec@ id body-dims {: F: cx cy cz cr ch :}
    cy ch 2e f/ f+  y h 2e f/ f+ f- fabs  h fabs ch fabs f+ 2e f/ f<=
    zz cz f- fdup f*  x cx f- fdup f* f+  r cr f+ fdup f* f<=  and ;
: centred-in? ( id F: x y z r h -- flag )   \ its centre inside the upright cylinder across, overlapping it in height
    {: id F: x y zz r h :}  id here? 0= if  false exit  then
    id body-at vec@ id body-dims {: F: cx cy cz cr ch :}
    cy ch 2e f/ f+  y h 2e f/ f+ f- fabs  h fabs ch fabs f+ 2e f/ f<=
    zz cz f- fdup f*  x cx f- fdup f* f+  r fdup f* f<=  and ;
: centre-across? ( id F: x z r -- flag )   \ its centre within r of (x, z) across - anyone or anything with a body
    {: id F: x z r :}  id dup 0< swap body? 0= or if  false exit  then   \ (the rooms' zone bit 8)
    id body-pos fswap fdrop {: F: cx cz :}  cx x f- fdup f*  cz z f- fdup f* f+  r fdup f* f<= ;
: reaches-across? ( id F: x z r -- flag )   \ its body within r of (x, z) across, whatever its height (the rooms' zone bit 2)
    {: id F: x z r :}  id here? 0= if  false exit  then
    id body-at vec@ id body-dims {: F: cx cy cz cr ch :}  cx x f- fdup f*  cz z f- fdup f* f+  r cr f+ fdup f* f<= ;
: before-door? ( id exit -- flag )   \ before the exit's door (the original's cond 0x18)
    {: id ex :}  id here? 0= if  false exit  then  id body-pos ex 0 door-in-area? ;
: at-locked-exit? ( id -- flag )   \ in an exit's area, its door locked (trying it)
    {: id :}  8 0 do
        i exit-door dup 0< 0= if  doors:door-locked? id i exit-area in-area? and if  true unloop exit  then  else  drop  then
    loop  false ;
: remember-people ( -- )
    #people 0 do
        i person-id dup here? over body? and if  body-at  i /vec * was-at +  swap vec-copy  -1 i cells seen + !   \ (vec-copy is dst src: the body only read)
        else  drop  0 i cells seen + !  then
    loop ;

\ ---- the camera ----
: camera-actor ( -- id )  s" camera" actor-named ;
: camera-set ( id set path -- )  cam-char-setup ;           \ (whose camera: a set of the room's, its path)
: area-camera ( area set path -- )   \ whoever crosses into the area takes that camera
    {: area set path :}  #people 0 do  i person-id dup here? if  dup area entered? if  set path camera-set  else  drop  then  else  drop  then  loop ;
: camera-out ( area set path -- )   \ whoever crosses out of the area takes that camera
    {: area set path :}  #people 0 do  i person-id dup here? if  dup area crossed -1 = if  set path camera-set  else  drop  then  else  drop  then  loop ;
: follow ( id -- )  cam-on ;
: player ( -- id )  hewie-played? if  hewie  else  fiona  then ;   \ (the one played)
: player-camera ( set path -- )  player -rot camera-set  player follow ;
: exit-camera ( exit set path -- )   \ whoever is inside the exit's area takes that camera (as they come in)
    {: ex set path :}  #people 0 do  i person-id dup ex exit-area in-area? if  set path camera-set  else  drop  then  loop ;
: her-camera ( set path -- )  fiona -rot camera-set  fiona follow ;   \ (her setup, and the camera on her)                                \ the camera follows who
: near-clip ( F: v -- )  engine:camera cam.near sf! ;   \ (the camera's near clip: 1 the usual)
: fixed-camera ( set path -- )  cam-setup  -1 cam-followed! ;   \ the room's own camera, following nobody
: shot ( on F: dist pitch yaw rise -- )   \ the story's own shot of the one followed (0: back)
    {: on F: d p y r :}  on  d 1000e f* f>s  p 1000e f* f>s  y 1000e f* f>s  r 1000e f* f>s  camera-actor send event-shot ;
: shot-off ( -- )  0 0e 0e 0e 0e shot ;
: camera-restart ( -- )  cam-restart ;

\ ---- exits ----
: rooms-actor ( -- id )  s" rooms" actor-named ;
$80 constant forced-exit   \ (an exit's bit: not by a door - to the room loaded ahead, prepare-exit's)
: forced-way? ( last -- flag )  came-by @ forced-exit rot 1+ within ;   \ (she came by one of the forced ways $80 .. last)
: take-exit ( exit -- )  going ! ;   \ (after this frame)
: at-exit ( id exit -- )  over 0< if  2drop exit  then  swap send to-exit ;   \ (on its outside spot, facing in)
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
: part? ( n -- flag )  parts-base + 1+ room-group? ;   \ (shown)
: show-part ( n -- )  true part ;
: hide-part ( n -- )  false part ;

\ ---- sounds ----
$40000000 constant story-volume   \ (a sound's id bit: scaled by the story's volume setting)
$80000000 constant keep-playing   \ (a sound's id bit: one already playing kept going, moved, not restarted)
6 constant room-bank              \ (the room's own sounds)
: room-sound ( id F: x y z -- )  {: id F: x y z :}  id story-volume or room-bank 0 0  x y z  sound-at ;
: keep-sound ( id F: x y z -- )  {: id F: x y z :}  id story-volume or keep-playing or room-bank 0 0  x y z  sound-at ;
5 constant common-bank   \ (the sounds every room has: knocks, Hewie's find)
7 constant stalker-bank  \ (the stalker's own: each kind's set)
: sound-by ( id bank who -- )  {: id bank who :}  who dup 0< if  drop exit  then  body? 0= if  exit  then  who body-pos {: F: x y z :}  id bank 0 0  x y z  sound-at ;   \ (at a character)

\ ---- scenes (scenes.fs): made ready while the screen is black, then played ----
enum: scene-progress  no-scene-yet scene-loading scene-is-ready scene-running scene-ended ;
: scenes ( -- id )  s" scenes" actor-named ;
: load-scene ( movie class cutscene -- movie? )   \ (the movie and cutscene: room strings; class: scenes.fs's movie-class)
    {: movie cls cut :}  scene-loading scene-step !  this-room @ movie cls cut  scenes send scene-load
    begin  scene-step @ scene-loading = while  yield  repeat  scene-movie @ ;
: run-scene ( subtitles -- )   \ played through (or skipped); subtitles: a message, or -1
    dup 0< if  drop $FFFF  then  scene-running scene-step !  scenes send scene-run
    begin  scene-step @ scene-running = while  yield  repeat  0 cue-xt ! ;
: run-through ( subtitles -- )   \ played through, never skipped, its last frame left up (a game over's)
    dup 0< if  drop $FFFF  then  scene-running scene-step !  scenes send scene-run-through
    begin  scene-step @ scene-running = while  yield  repeat  0 cue-xt ! ;
: at-shots ( xt -- )  cue-xt ! ;
: signal-passed? ( k n -- flag )   \ the scene's signal k, counted, passing its nth this frame (the original's cond 0x3B)
    over cutscene-signals dup 0> 0= if  drop 2drop false exit  then     ( k n new )
    rot cutscene-signal-total                                          ( n new total )
    rot 2dup < if  2drop drop false exit  then  >r swap - r> < ;   \ ( shot -- ) run as each new shot of the scene comes
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
: other-game-file ( item -- flag )  $8000 and 0<> ;   \ (the other game's files: the story's items-40-as-70 plays it)
: give-file ( item -- )   \ a file, with its sound if new (in the other game only its own)
    items-40-as-70 fact? if
        dup other-game-file if  $8000 invert and  -1 items send add-file  else  drop  then
    else  dup other-game-file if  drop  else  -1 items send add-file  then  then ;
: give-item ( item n -- )   \ counted (the play record) and given
    progress pr.items-counted dup l@ 1+ swap l!  items send give-item ;
: item-full? ( item -- flag )  items:items-of 99 < 0= ;
$8010 constant cant-carry-msg   \ "It's a . Unfortunately you can't carry anymore items." (the item named)
$8011 constant obtained-msg     \ "You've obtained ."
$8012 constant obtained-2-msg   \ (the same, for a thing that isn't one of many)
$8019 constant used-msg         \ "Used ." (the item named)
: item-named ( item -- )  0 swap message-param! ;   \ (the next system message names it)
: subscreen ( -- id )  s" subscreen" actor-named ;
$8C constant fanfare-scene
$83 constant fanfare-sound   $85 constant fanfare-bank
: fanfare ( -- )  fanfare-sound fanfare-bank bank-sound ;   \ (the item's jingle)
: show-item-tab ( item -- )   \ (its tab slid in to show her: waited out)
    -1 tab-moving !  0 swap subscreen send tab-op  begin  tab-moving @ while  yield  repeat ;
$801B constant filed-message    \ "has been saved to your file."
: spend ( item -- )  items send take-item ;   \ one of it used up
: map-page ( page -- )  s" subscreen" actor-named send map-turn-to ;   \ the map opens at that page (its floor)
$2D constant ending-var   \ (which of the endings caught her, for the game over's kind 2)
: game-over ( kind -- )  s" gameover" actor-named send game-over-kind  caught state! ;   \ the game ends that way
: filed ( doc -- )  dup item-named  1 give-item  fanfare  filed-message say ;   \ a document filed, said so

\ ---- the panic, Fiona ----
: calm-her ( n -- )  fiona send fear-less ;
: ease-panic ( n -- )  s" panic" actor-named send calm-in ;
: recover ( n -- )  fiona send recovery-less ;
\ dressing: her costume (the story's variable $26; $28 what she wore before the menu), her steps'
\ sound set with it (bare feet in the slip and the costumes 2, 3, 8), Hewie's coat ($27; $29)
$26 constant costume-var   $27 constant coat-var   $28 constant costume-worn-var   $29 constant coat-worn-var
: var@ ( n -- v )  progress pr.vars + c@ ;
: var! ( v n -- )  progress pr.vars + c! ;
: var-same? ( a b -- flag )  var@ swap var@ = ;
: bare-feet? ( costume -- flag )  dup 1 = over 2 = or over 3 = or swap 8 = or ;
4 constant steps-bank
: wear ( costume -- )  fiona send costume ;   \ (for a scene: worn, not kept - dress keeps it)
: dress ( costume -- )
    dup costume-var var!  dup fiona send costume
    bare-feet? if  new-game-sounds state!  1  else  new-game-sounds state-off  0  then  sound-set! ;
: steps-in ( -- )  begin  yield  steps-bank sound-loaded? until ;   \ (the step sounds of the new set in)
: coat ( kind -- )  dup coat-var var!  hewie send hewie-costume ;

\ ---- Hewie ----
: hewie-in? ( -- flag )  hewie here? ;
: hewie-doing? ( act -- flag )  hewie-in? if  hewie-act-now @ =  else  drop false  then ;
: hewie-do ( a b -- )  hewie send do-action ;
: hewie-to ( room tri side -- )  hewie send hewie-place ;   \ (put in that room on the triangle, at her side: Hewie_PlaceInRoom)
100 constant scramble-2-act   \ (Hewie_SetAction's $64: into his scramble, as scramble-act)
: scrambling? ( -- flag )  hewie-act-now @ dup scramble-act = swap scramble-2-act = or  hewie-in? and ;
: hush ( id on -- )  over 0< if  2drop exit  then  swap send silent ;   \ (its own sounds kept quiet)
: trust+ ( n -- )  hewie send trust+ ;
: hewie-scent ( F: x y z -- )  f>cell f>cell f>cell swap rot  hewie send scent-at ;   \ (something to look to, if he has none)
: chance? ( pct -- flag )  rnd 100e f* f>s > ;

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

\ ---- doors her (or his) way: opened or shut as a move ----
: open-door ( id exit -- )  {: id ex :}  id open-door-move ex -1  0e 0e 0e 0e  move  id wait-move ;
: shut-door ( id exit -- )  {: id ex :}  id shut-door-move ex -1  0e 0e 0e 0e  move  id wait-move ;
: avoid-prompt ( row -- )  s" avoid" actor-named send show-avoid ;   \ (the hide / flee prompt, a second)

\ ---- creatures (as they tell): one in this room moving so ----
: creature-moving? ( mode -- flag )
    {: mode :}  #creatures 0 do   \ (cond 0x53: any of them)
        i cells cr-age + @ 2 <  i cells cr-room + @ this-room @ = and  i cells cr-mode + @ mode = and if  true unloop exit  then
    loop  false ;

\ ---- the stalker ----
: stalker-in? ( -- flag )  stalker here? ;
: stalker-told? ( -- flag )  st-heard @  st-age @ 2 < and ;
: stalker-came-by? ( exit -- flag )  stalker-in? stalker-told? and if  st-came-now @ =  else  drop false  then ;   \ (in, come by that exit)
: stalker-alert? ( mode -- flag )  stalker-told? if  st-alert-now @ =  else  drop false  then ;
: debilitas? ( -- flag )   \ (the stalker in the game Debilitas, in one of his kinds)
    stalker-told? 0= if  false exit  then
    st-kind-now @  dup debilitas-kind = over debilitas-6-kind = or swap debilitas-7-kind = or ;
: partner ( -- id | -1 )  story.words:slot2 @ ;   \ the story's stalker (its slot 2): loaded, in the game or not
: partner-kind ( -- kind )  story.words:slot2-kind @ ;
: daniellas-model ( -- model | -1 )  partner-kind daniella-kind = if  daniella-kind cast-model  else  -1  then ;   \ (the stalker loaded Daniella)
: hands-empty ( -- )  daniellas-model dup 0< if  drop exit  then  false false story.words:daniella-parts ;   \ (what she holds put away)
: hands-back ( -- )  daniellas-model dup 0< if  drop exit  then  story.words:daniella-one ;   \ (what she holds shown again)
: stalker-to ( room tri -- )  stalker dup 0< if  drop 2drop exit  then  send stalker-in ;   \ (into the game there)
: his-own-time ( -- )  -1 s" summoner" actor-named send summoner-cooldown ;   \ (the summoner's wait: his own kind's)
: summoner-sooner ( secs -- )  s" summoner" actor-named send summoner-cooldown-less ;
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
: effects-gone ( -- )  #room-effects 0 do  i effect-off  loop ;
: tell-effect ( v k -- )  effect @ dup alive? if  send effect-command  else  2drop  then ;
\ the room's tint (Tint_SetParams: the look's slot): the glow colour and the contrast colour, RGBA
$1F constant tint-slot
create tint-bytes 9 allot
: room-tint ( gr gg gb ga cr cg cb ca -- )   \ (its mode 0)
    0 tint-bytes 8 + c!  8 0 do  tint-bytes 7 i - + c!  loop  tint-slot tint-bytes 9 look-set ;
\ the look's other slots: the depth range (its far view's: Room_DepthRange), the screen's blend
$1C constant depth-slot   $1E constant blend-slot
create look-bytes 16 allot
: depth-range ( F: a b c d -- )
    look-bytes 12 + sf!  look-bytes 8 + sf!  look-bytes 4 + sf!  look-bytes sf!  depth-slot look-bytes 16 look-set ;
: depth-range-off ( -- )  depth-slot 0 0 look-set ;
: screen-blend ( rgba mode -- )  look-bytes 4 + c!  look-bytes l!  blend-slot look-bytes 5 look-set ;
: screen-blend-off ( -- )  blend-slot 0 0 look-set ;
: glow-at ( k rgba F: x y z size -- )  effects:glow swap effect! ;   \ effect k: a glow (Effect737D0)
: preload ( room -- )  rooms-actor send room-preload ;
: thing-lying? ( kind -- flag )   \ (one of the things of that kind lying about, in any room: PlacedThings_FirstOfKind)
    128 0 do  i thing-used? if  i thing-kind over = if  drop true unloop exit  then  then  loop  drop false ;
: creature-facing ( room level tri kind reach yaw -- )   \ (one let out there, turned so: the creatures' own way about -
    -1 swap 0e f>cell  s" creatures" actor-named send creature-place ;   \ reach x 10 her nearness that wakes it)
: creature-in ( room level tri kind reach -- )  0 creature-facing ;   \ (the room a forced exit leads to, loaded ahead)
\ a reflecting window (Reflection): reflection k (0..3) on the quad's corners, its side panels a window's
$10000000 constant window-panels
: window-in ( k strength F: x0 y0 z0 .. x3 y3 z3 -- )
    {: k str :}  k effect @ 0= if  k effects:reflection k effect!  then  k false str window-panels or reflect! ;
: lit-over-all ( k strength F: x0 y0 z0 .. x3 y3 z3 -- )   \ (a scene's lit quad - a Reflection with no window
    {: k str :}  k effect @ 0= if  k 3 and effects:reflection k effect!  then  k 3 and true str reflect! ;   \ panels - over the whole picture)
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
        stalker-p of st-kind-now @ cast-model endof  >r -1 r>  endcase ;
: motions-for ( id file -- )  this-room @ story.strings:room-string  rot model-of dup 0< if  drop 2drop exit  then  motions-load drop ;
: use-motions ( id -- )  model-of dup 0< if  drop exit  then  true swap motions-use ;

\ ---- exits: taken by the one played, free, in its area, its door open and not held ----
: exit-held? ( exit -- flag )  exit-door dup 0< if  drop false  else  doors:door-held?  then ;   \ (its door held - none: not)
: exit-open? ( exit -- flag )   \ (a doorway, or a door not locked with its opened mark; no door: not)
    exit-door dup 0< if  drop false exit  then
    dup door-flags 1 and if  drop true exit  then
    dup doors:door-locked? if  drop false exit  then  doors:opened-bit doors:has? ;
: fiona-free? ( -- flag )   \ (on her own or down, nobody holding her)
    fiona-mode-now @ dup own-mode = swap down-mode = or  fiona-p holder-of @ 0< and ;
: exit-out ( exit -- )   \ (each frame: she goes when she can)
    {: ex :}  fiona-free? 0= if  exit  then
    fiona ex exit-area in-area? 0= if  exit  then
    ex exit-open? 0= if  exit  then
    ex exit-held? if  exit  then
    ex take-exit ;
: light-doorway ( F: x0 y0 z0 .. x3 y3 z3 -- )  light-blocker ;   \ (this frame: the characters' shadows kept off where the wall shades)

\ ---- characters, wholesale ----
: activate ( id -- )  dup 0< if  drop exit  then  send activate ;   \ (afresh: standing, its own)
: full-health ( id -- )  dup 0< if  drop exit  then  send full-health ;
: well ( id -- )  dup 0< if  drop exit  then  0 swap send set-cond ;   \ (its condition: well)
$A constant usual-layer   $1E constant before-reflections-layer   \ (a model's draw layer)
: layer ( kind layer -- )  swap cast-model dup 0< if  2drop exit  then  actor act.layer l! ;   \ (by its kind: Fiona 0)
: hair-force ( F: x y z -- )  0 cast-model dup 0< if  drop fdrop fdrop fdrop exit  then  2 cloth-force! ;   \ (her hair blown)
: hewie-hold-still ( -- )   \ (in here: a short wait, and he sits next)
    hewie dup body? if  dup body-room this-room @ = if  true swap send hewie-hold exit  then  then  drop ;
: summon-take ( kind -- )  s" summoner" actor-named send summoner-take ;
: stalker-clock ( frames -- )  stalker dup 0< if  2drop exit  then  send stalker-clock ;

\ ---- the room's lights, the story's volume, the stage music, a knock ----
: light-brighter ( light F: k -- )  1 light-scale ;   \ (the room's light n: its intensity times k)
: shadows-longer ( light F: k -- )  2 light-scale ;   \ (its shadows' length times k)
: light-back ( light -- )  light-own ;            \ (as the room has it)
: sounds-scaled ( F: v -- )  sound-scale! ;        \ (every sound of the story's scaled: 0 silent)
: stage-music ( op a b -- )  s" music" actor-named send music-op ;
$27 constant knock-sound
: knock ( exit -- )   \ (a knock at that door, unless it's open)
    {: ex :}  ex exit-door dup 0< if  drop exit  then  door-open? if  exit  then
    ex exit-stand 0= if  fdrop fdrop fdrop exit  then  knock-sound common-bank bank-sound-at ;

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

\ Hewie finds something (the rooms' shared find, act $8B): he turns to it, barks, sits and looks to
\ her - it sparkles - until she takes it, commands him or is chased, or 300 frames pass
1 constant bark-mark   $5B constant found-bark   300 constant find-wait
: hewie-finds ( found taken fx F: x y z -- )
    {: found taken fx F: x y z :}
    hewie hold  hewie wait-move  hewie x z turn-to-xz  hewie wait-move
    hewie true hush  hewie bark-anim 10 start-blend  hewie bark-mark wait-mark
    hewie false hush  found-bark common-bank hewie sound-by  hewie true hush
    hewie wait-anim  hewie false hush
    hewie sit-down-anim play  hewie sit-anim start-anim  yield
    hewie fiona look-at  yield
    found fact!  5 trust+  x y z 0 flicker fx effect!
    find-wait 0 do
        yield  taken fact?  chased? or  fiona-cmd-now @ -1 <> or if  leave  then
    loop
    hewie -1 look-at  yield  hewie keep  hewie send hewie-stay ;
: hewie-barks-at ( F: x y z -- )   \ (the rooms' shared $88: turned to it, a bark, a moment, staying put)
    {: F: x y z :}  hewie hold  hewie wait-move  hewie x z turn-to-xz
    hewie true hush  hewie bark-anim 10 start-blend  hewie bark-mark wait-mark
    hewie false hush  found-bark common-bank hewie sound-by  hewie true hush
    hewie wait-anim  hewie 10 idle  hewie false hush  10 frames  hewie keep  hewie send hewie-stay ;
: hewie-stands-by ( F: x y z -- )   \ (the rooms' shared $8A: over to it, tense, his eyes on it a while, staying put)
    {: F: x y z :}  hewie hold  hewie wait-move  hewie spot-move 0 0  x y z 0e  move  hewie wait-move
    hewie hewie.anims:stance-anim start-anim  yield  hewie look-point-move 0 0  x y z 0e  move
    120 frames  hewie -1 look-at  yield  hewie 10 idle  10 frames  hewie keep  hewie send hewie-stay ;
: hewie-sits-by ( F: x y z -- )   \ (the rooms' shared $89: turned to it, sitting a while, staying put)
    {: F: x y z :}  hewie hold  hewie wait-move  hewie x z turn-to-xz
    hewie hewie.anims:sit-a-while-anim 10 play-blend  hewie 10 idle  10 frames  hewie keep  hewie send hewie-stay ;

\ ---- the place, as an actor ----
\ ---- more of a room's things: its floor, doors closed off, the music, effects, objects ----
: floor-on ( group flags -- )  true -rot nav-group! ;    \ (a group of its triangles: who they keep off - floor-flags.fs)
: floor-off ( group flags -- )  false -rot nav-group! ;
: on-group? ( id group -- flag )   \ standing on a triangle of that group
    over here? 0= if  2drop false exit  then  swap body-tri dup 0< if  2drop false exit  then  swap nav-in-group? ;
\ the push-boxes (the obstacles actor's): one placed on a square, one pushed home stopped there
: obstacle-in ( i n kind a b -- )  s" obstacles" actor-named send place-obstacle ;   \ (box i - model "oshi0n", its kind - on square (a, b))
: obstacle-fixed ( i -- )  s" obstacles" actor-named send stop-obstacle ;   \ (it can't be pushed any more)
: obstacle-on-tri? ( i tri -- flag )  obstacle-on-square? ;   \ (it stands on that triangle's square)
: obstacle-kept ( i n kind -- )  s" obstacles" actor-named send place-obstacle-kept ;   \ (placed where it was kept)
: obstacle-keep ( i -- )   \ (its square kept for the next visit, and where its model stands)
    dup s" obstacles" actor-named send keep-obstacle  s" obstacles" actor-named send keep-obstacle-spot ;
: floor-tri-on ( tri flags -- )  true -rot nav-tri-flags! ;   \ (one triangle's floor: who it keeps off)
: closed-off ( door -- )  dup doors-actor send close-off  lock ;   \ gone for good: its exits use another door
: door-as ( door from -- )  doors-actor send copy-door ;   \ door takes on the other's open mark, lock and closing off
: door-for ( door slot on -- )  doors-actor send lock-for ;   \ locked against the story slot's kind of character, or not
: reopened ( door -- )  dup doors-actor send reopen  unlock ;  \ back again, unlocked (its exits theirs again)
: open-mark ( door on -- )  doors-actor send door-opened ;     \ its opened mark set, or cleared
: exits-redone ( -- )  doors-actor send rebuild-exits ;
0 constant fiona-slot   1 constant hewie-slot   2 constant stalker-slot   \ (the story's character slots)
: doors-into ( room slot on -- )  doors-actor send lock-room-for ;   \ every door into the room locked against them, or not
: music ( -- id )  s" music" actor-named ;
$FF constant no-track
: background ( track F: vol -- )  0 f>cell music send bgm-want ;   \ the room's own track (no-track: none)
: music-part ( stage -- )  music send music-stage ;
: rgba ( r g b a -- rgba )  {: r g b a :}  r 24 lshift  g 16 lshift or  b 8 lshift or  a or ;
: flicker-at ( k F: x y z -- )  0 effects:flicker swap effect! ;   \ effect k: a sparkle
: moths-at ( k n F: x y z -- )  effects:butterflies swap effect! ;   \ effect k: n moths (0: 8) making for the point (0x35)
: moths-to ( k n F: x y z -- )   \ effect k's moths off for another point; n of them (0: as many as they are) - 0x33
    {: k n F: x y z :}  k effect @ dup alive? 0= if  drop exit  then  >r  n x f>cell y f>cell z f>cell  r> send effect-to ;
: moths-sound ( k id -- )   \ the room's sound id kept going where effect k's moths wander now
    {: k id :}  k effect @ dup alive? 0= if  drop exit  then  >r  id story-volume or keep-playing or room-bank  r> send effect-sound ;
: object ( obj op arg -- )  >r >r  this-room @ story.strings:room-string  r> r> placed-op ;   \ (the room's string names it)
: object-hide ( obj -- )  0 1 object ;
: object-show ( obj -- )  0 0 object ;
: object-reset ( obj -- )  4 0 object ;   \ (shown, back as it was set: PlacedObject_ToDef)
: object+! ( addr len off F: d -- )  {: a n off F: d :}  a n placed-here? 0= if  exit  then  a n off placed-f@ d f+  a n off placed-f! ;
$28 constant place-z
: morph ( a b addr len F: t -- )   \ (a morph object: its frames a and b shown, blended by t 0..1)
    {: a b o n F: t :}  o n placed-here? 0= if  exit  then
    t 0e fmax 1e fmin  o n $7C placed-f!  a o n $74 placed-l!  b o n $78 placed-l! ;
: pulse-tint ( step c3 c4 c5 c6 c7 -- )   \ (the room's tint, its first three colours pulsing by step 0..$60, a third apart)
    {: step c3 c4 c5 c6 c7 :}
    3 0 do  step i $20 * +  $60 mod  $30 swap - abs  $10 - 0 max  3 lshift  $FF min  loop   \ (its bytes 0..2)
    c3 c4 c5 c6 c7 room-tint ;
: shown ( id on -- )  over 0< if  2drop exit  then  swap send show ;   \ (drawn, or not)
: object@ ( addr len off -- ) ( F: -- v )  placed-f@ ;
: object! ( addr len off F: v -- )  {: a n off F: v :}  a n placed-here? 0= if  exit  then  v a n off placed-f! ;
$24 constant place-y
: flame-at ( k kind F: x y z -- )  effects:flame swap effect! ;   \ effect k: a flame of that kind
: dust ( F: x y z -- )  0 $80 $80 $80 16 effects:burst ;   \ grey dust thrown up there
: dust-of ( kind r g b F: x y z -- )  16 effects:burst ;   \ dust of a kind and colour (the rooms' 0xC8 with its own)
: rumble-pad ( motor strength frames -- )  story.words:rumble-pad ;   \ (motor 0 the small one, buzzing; else the large at that strength)
: reverb ( core level -- )  sound-reverb! ;   \ (core 0 the music's, 1 the effects')
: threat ( v -- )  0 max 100 min s>f f>cell s" panic" actor-named send threat ;   \ (her fright raised to it)
: fright-up-to ( v -- )   \ (her fright raised by what her panic is short of v: Panic_FrightRaw)
    panic-level-now @ -  dup 0< if  drop exit  then  s>f f>cell s" panic" actor-named send fright-raw ;
: fright+ ( v -- )  s>f f>cell s" panic" actor-named send threat-acc ;   \ (her fright raised by so much: a scare)
: room-noise ( loud tri -- )  this-room @ swap -1 world-noise s" acoustics" actor-named send noise ;
: sound-there ( id bank F: x y z -- )  0 0 sound-at ;   \ a sound at the point

\ ---- things that hang and swing (the room's objects by their strings) ----
$10 constant turn-x   $18 constant turn-z   $20 constant place-x
: catches-light ( addr len F: turn -- )   \ an object catching the light: a flash at it, turned as it is and more by turn
    {: a n F: turn :}  a n placed-here? 0= if  exit  then
    a n place-x object@  a n place-y object@  a n place-z object@  a n turn-y object@ turn f+  effects:window-flash ;
: object-mirror ( k strength model addr len F: nx ny nz -- )   \ effect k: one character's model reflected in an
    {: k str model a n F: nx ny nz :}   \ object's plane (normal n, turned with it), an 8 x 8 square there (MirrorFragment)
    a n placed-here? 0= if  k effect-off exit  then
    nx ny nz  a n turn-x object@  a n turn-y object@  a n turn-z object@
    a n place-x object@  a n place-y object@  a n place-z object@  mirror-square
    k effect @ 0= if  k 3 and effects:reflection k effect!  then  k 3 and false str reflect!  k 3 and model reflect-only ;
\ ---- dials: an object turned by the stick to a number (0..6, 30 degrees each from its base) kept in
\ a progress variable; turned a degree a frame to it (room_dial: the castle's moon dials) ----
create arrows-held 4 cells allot   \ (frames each arrow has been held: the pad's repeat)
: arrow-keys ( i -- k1 k2 )   \ 0 up, 1 right, 2 down, 3 left (the arrows, or WASD)
    case  0 of  key: Up key: W  endof  1 of  key: Right key: D  endof  2 of  key: Down key: S  endof
          >r key: Left key: A r>  endcase ;
: count-arrows ( -- )   \ (a frame's: how long each arrow has been held)
    4 0 do  i cells arrows-held +  i arrow-keys key-down? swap key-down? or if  1 swap +!  else  0 swap !  then  loop ;
: arrow-repeat? ( i -- flag )   \ (pressed, then again after 12 frames held, every 2: gMenuRepeat)
    cells arrows-held + @  dup 1 = if  drop true exit  then  dup 13 < if  drop false exit  then  13 - 2 mod 0= ;
: dial-angle ( var base -- ) ( F: -- rad )  swap var@ 30 * + s>f deg>rad ;
: dial-set ( addr len var base -- )  {: a n var base :}  a n placed-here? 0= if  exit  then  var base dial-angle a n turn-y object! ;
: dial-turns ( addr len var base -- there? )   \ (a degree a frame toward its number)
    {: a n var base :}  a n placed-here? 0= if  true exit  then
    a n turn-y object@ rad>deg  var base dial-angle rad>deg f-
    fdup 1e f> if  fdrop -1e deg>rad a n turn-y object+!  false exit  then
    -1e f< if  1e deg>rad a n turn-y object+!  false  else  true  then ;
: dial-asked ( var -- n )   \ the stick this frame: 1 turned to another number, -1 done (cross or triangle), 0 nothing
    {: var :}  count-arrows
    cross button-pressed?  triangle button-pressed? or if  -1 exit  then
    var var@  3 arrow-repeat? if  dup if  1-  then  else  1 arrow-repeat? if  dup 6 < if  1+  then  then  then
    dup var var@ <> if  var var!  1  else  drop 0  then ;
\ a dial turned by the stick (the moon dials): the stick's message up; each number she asks for
\ turned to a degree a frame - the room's sound 0 as it sets off, a click (its sound 1) at the
\ right one and its lens catching the light - till she is done (cross or triangle), its message up
0 constant dial-rest-sound   1 constant dial-click-sound
: dial-turning ( addr len var right base msg F: sx sy sz turn -- )
    {: a n var right base msg F: sx sy sz turn | asking :}
    -1 to asking  msg show
    begin
        asking if
            var dial-asked  dup 1 = if  0 to asking  dial-rest-sound sx sy sz room-sound  then
        else
            a n var base dial-turns if
                -1 to asking  var var@ right = if  dial-click-sound sx sy sz room-sound  a n turn catches-light  then
            then  0
        then
    -1 <> while  yield  repeat ;   \ (its message left up: the caller closes it)
$30 constant swing-phase  $34 constant swing-offset  $38 constant swing-left  $3C constant swing-pushed   \ (their own: floats)
4 constant swing-back   \ (a whole number: swinging back)
: hanging ( i -- addr len )  this-room @ story.strings:room-string ;
: her-step ( F: -- d )   \ (how far she moved this frame)
    fiona here? 0= if  0e exit  then  fiona-p /vec * was-at + fiona body-at vec-dist ;
\ hung things she brushes as she walks by (hangers_swing): her steps add up - past `push` they
\ swing for 20 frames, the first ringing (`on` / `off`, the room's sounds, in turn: `rung` a cell);
\ a swing: on 36 degrees a frame, tilted 1 + sin of it (in degrees), each half a swing apart
: chime-rest ( k a n -- )
    {: k a n :}  a n placed-here? 0= if  exit  then
    0e a n swing-phase object!  60e k s>f f* a n swing-offset object!  0e a n swing-left object!  0e a n swing-pushed object! ;
: chimes-rest ( base n -- )  0 ?do  i  over i + hanging  chime-rest  loop  drop ;
: chime ( k a n squared? rung on off F: push -- )
    {: k a n sq rung on off F: push :}  a n placed-here? 0= if  exit  then
    her-step  sq if  fdup f*  then  a n swing-pushed object+!
    a n swing-pushed object@ push f> if
        20e a n swing-left object!  0e a n swing-pushed object!
        k 0= if  rung @ 0= dup rung !  if  off  else  on  then  room-bank fiona sound-by  then
    then
    a n swing-left object@ f0> 0= if  exit  then
    a n swing-left object@ 1e f- 0e fmax a n swing-left object!
    36e a n swing-phase object+!
    a n swing-phase object@ a n swing-offset object@ f+ 360e f< 0= if  -360e a n swing-phase object+!  then
    a n swing-phase object@ a n swing-offset object@ f+ deg>rad fsin 1e f+ deg>rad
    fdup pi f> if  2e pi f* f-  then  a n turn-x object! ;
: chimes ( base n squared? rung on off F: push -- )   \ (each frame she is among them)
    {: base n sq rung on off F: push :}  n 0 ?do  i  base i + hanging  sq rung on off push chime  loop ;
\ a hung thing set swaying - 12 degrees, away from who struck it - and left to settle: a step
\ 22.5 degrees of its swing a frame, its size shrinking to 0.4 at each end; under half a degree, still
: sway-start ( obj who -- )
    {: obj who | a n F: dx F: dz F: d F: wx F: wz :}  obj hanging to n to a  a n placed-here? 0= if  exit  then
    0 a n swing-back placed-l!  0e a n swing-phase object!  12e deg>rad a n swing-offset object!
    who body-pos fswap fdrop to wz to wx
    a n place-x object@ wx f- to dx  a n place-z object@ wz f- to dz
    dx fdup f* dz fdup f* f+ fsqrt to d  d f0= if  exit  then
    dz d f/ a n swing-left object!  dx d f/ fnegate a n swing-pushed object! ;
: sway-step ( a n -- settled? )
    {: a n :}
    a n swing-back placed-l@ if  -22.5e  else  22.5e  then  deg>rad a n swing-phase object+!
    a n swing-phase object@ fabs 1e deg>rad f< if  a n swing-offset object@ 0.4e f* a n swing-offset object!  then
    a n swing-back placed-l@ if
        a n swing-phase object@ pi f2/ fnegate f< if  0 a n swing-back placed-l!  then
    else  a n swing-phase object@ pi f2/ f> if  1 a n swing-back placed-l!  then  then
    a n swing-offset object@ 0.5e deg>rad f<= if  0e a n turn-x object!  0e a n turn-z object!  true exit  then
    a n swing-left object@ a n swing-offset object@ f* a n swing-phase object@ fsin f*  a n turn-x object!
    a n swing-pushed object@ a n swing-offset object@ f* a n swing-phase object@ fsin f*  a n turn-z object!  false ;
: sways ( obj -- )   \ (an activity's: till it settles)
    hanging {: a n :}  a n placed-here? 0= if  exit  then  begin  a n sway-step 0= while  yield  repeat ;

\ ---- where things are seen and faced ----
: unseen? ( id -- flag )  dup here? 0= if  drop true exit  then  body-at vec@ out-of-view? ;
: point-unseen? ( F: x y z -- flag )  out-of-view? ;
: faces-point? ( id within F: x z -- flag )   \ facing the point, within so many degrees
    {: id within F: x z :}  id here? 0= if  false exit  then
    id body-pos fswap fdrop {: F: mx mz :}  x mx f-  z mz f-  fatan2
    id body-yaw f- angle-wrap fabs rad>deg  within s>f f<= ;
: close-to? ( a b margin -- flag )   \ their bodies within margin of each other across, overlapping in height
    {: a b margin | F: ay F: ah F: ar F: by F: bh F: br :}  a here?  b here? and 0= if  false exit  then
    a body-pos fdrop fswap fdrop to ay  a body-dims to ah to ar
    b body-pos fdrop fswap fdrop to by  b body-dims to bh to br
    ay by f< if  by ay f-  ah  else  ay by f-  bh  then  f> if  false exit  then
    a body-at b body-at vec-dist-xz  ar br f+ margin s>f f+  f<= ;
: touches-facing? ( a b margin within -- flag )   \ a close to b, and facing it within so many degrees
    {: a b margin within | F: bx F: bz F: ax F: az :}  a here? b here? and 0= if  false exit  then
    a b margin close-to? 0= if  false exit  then
    b body-pos fswap fdrop  to bz  to bx  a body-pos fswap fdrop  to az  to ax
    bx ax f-  bz az f-  fatan2  a body-yaw f- angle-wrap fabs rad>deg  within s>f f<= ;
: point-in? ( v F: x y z r h -- flag )   \ the point inside the upright cylinder standing on (x y z)
    {: v F: x y zz r h :}  v vec@ {: F: px py pz :}
    py  y h 2e f/ f+  f- fabs  h fabs 2e f/ f<=   pz zz f- fdup f*  px x f- fdup f* f+  r fdup f* f<=  and ;
: struck-in? ( F: x y z r h -- flag )   \ anyone's blow (its point of contact) inside the cylinder
    {: F: x y zz r h | hit :}  0 to hit
    #people 0 do
        i person-id dup here? if
            body-point if  x y zz r h point-in? if  -1 to hit  then  then
        else  drop  then
    loop  hit ;
: touching-at? ( v id F: x y z r h -- flag )   \ (its body, were it at v, overlapping the cylinder)
    {: v id F: x y zz r h :}  v vec@ id body-dims {: F: cx cy cz cr ch :}
    cy ch 2e f/ f+  y h 2e f/ f+ f- fabs  h fabs ch fabs f+ 2e f/ f<=
    zz cz f- fdup f*  x cx f- fdup f* f+  r cr f+ fdup f* f<=  and ;
: steps-into? ( id F: x y z r h -- flag )   \ touching the cylinder now, not at the last frame's end
    {: id F: x y zz r h :}  id here? 0= if  false exit  then
    id body-at id x y zz r h touching-at? 0= if  false exit  then
    id person-of /vec * was-at + id x y zz r h touching-at? 0= ;
: flame-stirred ( k F: x y z r h -- )   \ (effect k, a flame: whoever steps into its draught - it told
    {: k F: x y zz r h :}                   \ how: 1 they barely moved this frame, 2 moving - 0x87)
    #people 0 do
        i person-id  x y zz r h steps-into? if
            i person-id body-at  i /vec * was-at +  vec-dist fdup f* 0.5e f< if  1  else  2  then
            k effect @ dup alive? if  send effect-command  else  2drop  then
        then
    loop ;
: splash-from ( id count F: x y z -- )   \ water thrown up there, away from who stepped in
    {: id n F: x y zz :}  id body-pos  x y zz  n 2  $40 $50 $40 $80 rgba  effects:splash ;
: her-blow-in? ( F: x y z r h -- flag )   \ her own blow (its point of contact) inside the cylinder
    {: F: x y zz r h :}  fiona here? 0= if  false exit  then
    fiona body-point if  x y zz r h point-in?  else  false  then ;
: left? ( id area -- flag )  crossed -1 = ;
: near-xz? ( id F: x z r -- flag )   \ its body within r of the point across (any height)
    {: id F: x z r :}  id here? 0= if  false exit  then
    id body-pos fswap fdrop {: F: bx bz :}  id body-dims fdrop {: F: br :}
    bx x f- fdup f*  bz z f- fdup f* f+  r br f+ fdup f* f<= ;

\ ---- breakables (a vase, a crate): whole or in pieces by a fact. A blow in its place breaks it,
\ as every room has it: the parts swapped, its floor freed, shards thrown, the shatter heard,
\ a noise made. `breakable:` names one; `shard-look`, `shard-uv`, `frees-floor`, `noise-from`
\ say more of it; the place shows it as she comes in (`as-it-stands`) and each frame asks
\ `breaks?` - true the frame it breaks, for what the room adds (the thing inside it) ----
begin-structure /breakable
  field: bk.fact  field: bk.whole  field: bk.broken  field: bk.group  field: bk.tri  field: bk.target
  field: bk.sound  field: bk.bank
  field: bk.rgba  field: bk.kind  field: bk.tex
  ffield: bk.x  ffield: bk.y  ffield: bk.z  ffield: bk.r  ffield: bk.h
  ffield: bk.u  ffield: bk.v  ffield: bk.du  ffield: bk.dv  ffield: bk.floor
end-structure
$88 constant shatter-sound   $40 constant shatter-loudness   $80303030 constant shard-grey
: breakable: ( fact whole-part broken-part F: x y z r h "name" -- )   \ (struck within r of x z, h up from y)
    create  here /breakable allot  {: f w b a F: x y z r h :}
    f a bk.fact !  w a bk.whole !  b a bk.broken !  -1 a bk.group !  -1 a bk.tri !  breakables @ a bk.target !  1 breakables +!
    shatter-sound a bk.sound !  common-bank a bk.bank !
    shard-grey a bk.rgba !  0 a bk.kind !  0 a bk.tex !
    x a bk.x f!  y a bk.y f!  z a bk.z f!  r a bk.r f!  h a bk.h f!
    0e a bk.u f!  0e a bk.v f!  1e a bk.du f!  1e a bk.dv f!  0e a bk.floor f! ;
: shard-look ( bk rgba kind tex -- )  {: a c k t :}  c a bk.rgba !  k a bk.kind !  t a bk.tex ! ;   \ (their colour, kind, texture)
: shard-uv ( bk F: u v du dv -- )   \ (the cell of the texture they show)
    {: a F: u v du dv :}  u a bk.u f!  v a bk.v f!  du a bk.du f!  dv a bk.dv f! ;
: frees-floor ( bk group -- )  swap bk.group ! ;   \ (its floor's nav group: kept off while it stands)
: noise-from ( bk tri -- )  swap bk.tri ! ;        \ (the triangle its noise comes from; none: no noise)
: shatters-with ( bk sound bank -- )  rot {: a :}  a bk.bank !  a bk.sound ! ;   \ (its own shatter: the room's)
: broken? ( bk -- flag )  bk.fact @ fact? ;
: bk-floor ( bk on -- )  over bk.group @ 0< if  2drop exit  then  swap bk.group @ keeps-people-off nav-group! ;
: as-it-stands ( bk -- )   \ (whole or in pieces, its floor kept off while whole)
    dup broken? {: a b :}  a b 0= bk-floor
    b if  a bk.whole @ hide-part  a bk.broken @ show-part  else  a bk.whole @ show-part  a bk.broken @ hide-part  then ;
: at-it ( bk -- ) ( F: -- x y z )  dup bk.x f@  dup bk.y f@  bk.z f@ ;
: smash ( bk -- )   \ (in pieces now)
    {: a :}  a bk.fact @ fact!  a as-it-stands
    a bk.rgba @ a bk.kind @ a bk.tex @  a at-it  a bk.u f@ a bk.v f@ a bk.du f@ a bk.dv f@  a bk.floor f@  shards
    a bk.sound @ a bk.bank @ a at-it sound-there
    a bk.tri @ 0< 0= if  shatter-loudness a bk.tri @ room-noise  then ;
: a-target ( bk -- )   \ (whole: a stalker's blow may strike it instead of her - Events_NearestPoint)
    dup bk.target @ swap at-it xyz>cells broadcast zone-target ;
: breaks? ( bk -- flag )   \ (whole, a blow in its place: broken now)
    dup broken? if  drop false exit  then  dup a-target
    dup at-it  dup bk.r f@  dup bk.h f@  struck-in? 0= if  drop false exit  then  smash true ;

\ ---- what every room says the same way, as tables: its ways in (the camera for each exit she
\ comes in by), its cameras (an area and the camera whoever crosses into it takes), its ways
\ out (an area and the exit loaded ahead as she enters it), what Hewie looks up at ----
: way-in ( exit set path -- )   \ (she came by it: placed at it, its camera hers; whoever is in it, its camera)
    {: ex set path :}  came-by @ ex = if  player ex at-exit  set path player-camera  then  ex set path exit-camera ;
: ways-in ( table n -- )   \ (a set and path an exit, from 0)
    0 ?do  i  over i 2* cells + @  2 pick i 2* 1+ cells + @  way-in  loop  drop ;
: cameras-at ( table n -- )   \ (area set path, three cells a camera)
    0 ?do  dup i 3 * cells +  dup @  over cell+ @  rot 2 cells + @  area-camera  loop  drop ;
: exits-out ( n -- )  0 ?do  i exit-out  loop ;   \ (she goes by any of the first n when she can)
: leaves-by ( area exit -- )   \ (the one played, in the area, free: through - the rooms' exit-check by an area)
    {: area ex :}  player held? if  exit  then  player area in-area? 0= if  exit  then
    ex exit-open? 0= if  exit  then  ex exit-held? if  exit  then  ex take-exit ;
: way-out ( area exit -- )  swap fiona swap entered? if  prepare-exit  else  drop  then ;
: ways-out ( table n -- )   \ (area exit: crossing into the area, the room behind that exit loaded ahead)
    0 ?do  dup i 2* cells + @  over i 2* 1+ cells + @  way-out  loop  drop ;
\ Hewie's looks (the rooms' 0xC5): a zone (an upright cylinder x y z, radius, height) and how high
\ above its foot he looks; while he is in one, calm and at her command, he looks up there
6 floats constant /look
: look, ( F: x y z r h dy -- )  {: F: x y z r h dy :}  x f, y f, z f, r f, h f, dy f, ;
: looks-at ( look -- )   \ (in its zone: up there)
    {: a :}  hewie  a f@  a float+ f@  a 2 floats + f@  a 3 floats + f@  a 4 floats + f@  centred-in? 0= if  exit  then
    a f@  a float+ f@ a 5 floats + f@ f+  a 2 floats + f@  hewie-scent ;
: hewie-looks-at ( table n -- )
    hewie-in?  danger-now @ calm = and  hewie-cmdable-now @ and 0= if  2drop exit  then
    0 ?do  dup i /look * + looks-at  loop  drop ;
$2A constant replay-var   \ (the story's variable: which movie the extras' theatre replays)
\ which of the room's acts replays it (the shared script $26's list, by the variable: $FF none; three
\ go by the story's variable $2C too)
create replay-acts
    $27 c,  $28 c,  $10 c,  $08 c,  $0C c,  $0D c,  $10 c,  $0C c,  $11 c,  $12 c,  $14 c,  $19 c,  $03 c,  $16 c,  $07 c,  $29 c,
    $2A c,  $11 c,  $12 c,  $15 c,  $1A c,  $09 c,  $0D c,  $0C c,  $0D c,  $0E c,  $0E c,  $0F c,  $07 c,  $08 c,  $0A c,  $17 c,
    $09 c,  $01 c,  $09 c,  $0A c,  $FF c,  $0C c,  $0D c,  $0E c,  $0F c,  $10 c,  $09 c,  $03 c,  $06 c,  $23 c,  $04 c,  $06 c,
    $06 c,  $09 c,  $0F c,  $10 c,  $14 c,  $15 c,  $16 c,  $17 c,  $13 c,  $14 c,  $15 c,  $16 c,  $20 c,  $0E c,  $21 c,  $22 c,
    $1B c,  $FF c,  $08 c,  $0E c,  $0F c,  $0A c,  $0B c,  $0A c,  $0B c,  $08 c,  $10 c,  $11 c,  $08 c,  $07 c,  $08 c,  $04 c,
    $04 c,  $02 c,  $12 c,  $1A c,  $1B c,  $1C c,  $04 c,  $13 c,  $14 c,  $15 c,  $16 c,  $0C c,  $08 c,  $05 c,  $0B c,  $0C c,
    $0D c,  $0B c,  $04 c,  $01 c,  $0F c,  $10 c,  $11 c,  $12 c,  $10 c,  $07 c,  $FF c,  $05 c,  $FF c,  $02 c,
110 constant #replays
: replay-act ( -- act | -1 )   \ (the original act number of the room it replays in)
    replay-var var@ {: v :}  v #replays u< 0= if  -1 exit  then
    v case
        $24 of  $2C var@ 0= if  9  else  $B  then  endof
        $6A of  $2C var@ 0= if  4  else  1  then  endof
        $6C of  $2C var@ 0= if  6  else  5  then  endof
        >r  v replay-acts + c@  dup $FF = if  drop -1  then  r>
    endcase ;
\ a replayed movie over (the extras' theatre): back to the extras, out by the forced way
\ an item found (the rooms' pick-up): named; with 99 of it she can't carry it; else taken - its
\ fact, its sparkle (an effect of the place's, -1 none) off - its tab shown, the fanfare
: takes-up ( item fact fx -- )
    {: it f fx :}  it item-named  it item-full? if  cant-carry-msg say exit  then
    f fact!  fx 0< 0= if  fx effect-off  then  it 1 give-item  it show-item-tab  fanfare  obtained-msg say ;
: picks-up ( item fact fx F: x z -- )   \ (on the floor there: turned to it, bent down to it - not in a panic)
    fiona wait-move  fiona turn-to-xz  in-panic? if  drop 2drop exit  then
    fiona fiona.anims:pick-up-anim play  4 frames  takes-up  fiona fiona.anims:up-from-pick-anim play ;
: whine-due? ( addr -- flag )   \ (Hewie in the wire, now and then: a counter down a frame - at 0,
    -1 over +!  dup @ 0> if  drop false exit  then   \ 3 in 10 a whine, then 180 frames; else 60)
    30 chance?  dup if  180  else  60  then  rot ! ;
\ hidden, the stalker coming in (the rooms' hiding places): whether he will find her - by the
\ odds (by how often he has come), a creature moving about here gives her away, his alert 4 never
: will-find? ( odds -- flag )  chance?  2 creature-moving? or  4 stalker-alert? 0= and ;
: hidden-calm ( -- )   \ (a frame hidden: calmer; he came while she hid and is gone - the prompt again)
    2 ease-panic  6 calm-her  $1E recover
    came-while-hidden @  stalker-in? 0= and if  0 came-while-hidden !  1 avoid-prompt  then ;

\ ---- a hiding place (the rooms' cupboards and wardrobes): she walks to it, slips in and lies low,
\ calming; out by the circle button - or, found (or panicking, or he is in), hauled out. The room's
\ motions for it (her file): 4 the walk there, 5 in, 1 lying low, 2 out, 3 hauled out; his (his
\ file, Debilitas's): 0 the look in ----
: hid-motion ( i -- anim )  room-motion + ;
: hauled-out ( -- )
    found-her @ if  2 avoid-prompt  then
    fiona follow  fiona-hidden state-off  fiona true noclip  fiona.sounds:slide-sound common-bank fiona sound-by
    fiona 3 hid-motion 10 play-blend  fiona false noclip
    found-her @ 0= if  stalker let-go  then ;
: slips-out ( -- )
    stalker-in? if  hauled-out exit  then
    fiona follow  fiona-hidden state-off  fiona true noclip
    fiona 2 hid-motion 5 start-blend8  5 frames  fiona.sounds:slide-sound common-bank fiona sound-by  fiona wait-anim
    fiona false noclip ;
: hides-in ( file tri deg cam F: x z -- )   \ (her motions' file; where she slips in from, facing it; the
    {: file tri deg cam F: x z :}           \ camera set while she's in, -1 hers kept)
    stalkers-stay state!  0 came-while-hidden !  fiona file motions-for  fiona wait-move  fiona use-motions
    fiona walk-anim-move 4 hid-motion 5  tri x z floor-at  deg deg>yaw  move  fiona wait-move
    fiona true noclip  fiona.sounds:slide-sound common-bank fiona sound-by  fiona 5 hid-motion 10 start-blend8  fiona wait-anim
    fiona 1 hid-motion 10 start-blend  fiona false noclip
    stalkers-stay state-off  fiona-hidden state!  0 found-her !  0 avoid-prompt
    cam 0< 0= if  fiona cam -1 camera-set  then
    begin
        circle button-pressed? if  slips-out exit  then
        in-panic? found-her @ or if
            in-panic? 0= if  fiona.sounds:seized-sound common-bank fiona sound-by  then  hauled-out exit
        then
        hidden-calm  yield
    again ;
\ the stalker in while she hides: whether he finds her - by the odds in a story variable (how
\ often he has come: 25, 50, 75 in 100), a creature moving about giving her away; if not, he's off sooner
: came-on-her? ( -- flag )  fiona-hidden state?  dup came-while-hidden ! ;   \ (as he comes in: she is hidden)
: finding-odds ( var -- pct )  var@ case  1 of 25 endof  2 of 50 endof  3 of 75 endof  >r 0 r>  endcase ;
: searches ( var file xt -- )   \ (xt: his going to her, found - started; his comings counted; file -1: none)
    {: v file xt :}  debilitas?  file 0< 0= and if  stalker file motions-for  then
    v finding-odds will-find? if  xt start drop  else  120 summoner-sooner  then  v var@ 1+ v var! ;
: door-for-him ( exit -- )   \ (come by it, its door shut: he opens it)
    dup stalker-came-by?  over exit-open? 0= and if  stalker swap open-door  else  drop  then ;
: finds-her-at ( tri deg F: x z -- )   \ (to the hiding place: Debilitas looks in, the others see her)
    {: tri deg F: x z :}  stalker tri deg x z walk-to
    debilitas? if
        stalker use-motions  stalker room-motion 10 start-blend  30 frames  -1 found-her !  stalker wait-anim
    else  stalker stalker.anims:there-anim play  -1 found-her !  then
    stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: replay-over ( -- )  forced-exit take-exit  events-held state-off  hewie send part-from-fiona ;

\ ---- the figures' own (the story's slots 3..5: a figure loaded there) ----
: figure ( slot -- id | -1 )  story.words:slot-actor @ ;
: figure-play ( slot anim loop? fade -- )   \ held in it (loop? 1: over and over)
    {: slot anim lp fade :}  slot figure dup 0< if  drop exit  then  >r  anim lp fade  r> send hold-anim ;
: figure-wait ( slot -- )   \ its animation come round
    dup figure 0< if  drop yield exit  then  3 - cells figure-ended +  0 over !  begin  yield  dup @ until  drop ;
: figure-came-round? ( slot -- flag )  3 - cells figure-ended + @ 0<> ;   \ (its animation come round: the original's cond 0x56)
: figure-goes ( slot sound F: x z -- )   \ (an animal - a rabbit: grazing, off to (x, z) as one comes near - its feet's
    {: slot snd F: x z :}  slot figure dup 0< if  drop exit  then   \ sound snd, -1 none; waited till it is there)
    0 slot 3 - cells figure-done + !  >r  snd x f>cell z f>cell  r> send animal-go
    begin  yield  slot 3 - cells figure-done + @  slot figure 0< or  until ;
: figure-at ( slot F: x y z deg -- )  deg>rad angle-wrap  figure dup 0< if  drop fdrop fdrop fdrop fdrop exit  then  >r xyzw>cells r> send place ;
: figure-on ( slot -- )  figure dup 0< if  drop exit  then  send activate ;
: figure-dressed-as ( slot other -- )   \ wearing the other's model
    story.words:slot-id @ cast-model dup 0< if  2drop exit  then  swap figure dup 0< if  2drop exit  then  send cast-dress ;
: figure-shift ( slot F: dx dy dz -- )   \ moved by so much, its heading kept
    {: slot F: dx dy dz :}  slot figure dup 0< if  drop exit  then  {: a :}
    a body-pos {: F: x y z :}  x dx f+  y dy f+  z dz f+  a body-yaw  xyzw>cells a send place ;
: stalker-ready ( kind -- )  story.words:partner-load ;   \ the game's stalker: that kind, loaded and started
: stalker-unloaded ( -- )  story.words:slot-unload ;   \ the game's stalker out (the story's slot 2 emptied)
: movie-key ( a b -- )  -1 -rot movie-compose ;   \ (the keyed movie's luminance key: its threshold and range)

\ ---- items counted ----
: has-items? ( item n -- flag )  swap items:items-of swap < 0= ;

\ ---- Hewie's own moves ----
: hewie-bark ( -- )  hewie bark-move 0 0  0e 0e 0e 0e  move ;
: hewie-leap ( tri b F: x y z -- )  {: tri b F: x y z :}  hewie leap-move tri b  x y z 0e  move ;
: go-to ( id tri face b kind F: x z -- )   \ (a move to (x, z) on tri, then facing: b its animation, -1 the usual)
    {: id tri face b kind F: x z :}  id kind tri b  tri x z floor-at  face deg>yaw  move  id wait-move ;
: path-to ( id tri b kind -- )  {: id tri b kind :}  id kind tri b  tri tri-center  id body-yaw  move  id wait-move ;
: tint ( id rgba -- )  swap model-of dup 0< if  2drop exit  then  actor act.tint l! ;   \ ($80808080 as it is)
: doorway-fades ( id on -- )  0<> 1 and  swap model-of dup 0< if  2drop exit  then  actor act.doorway-fade l! ;
: to-door ( id exit -- )   \ (walking to its door's spot on her side - to try it)
    {: id ex :}  id here? 0= if  exit  then
    id body-pos  ex door-side 1 = if  2  else  0  then  ex swap door-user-spot  dup 0< if  drop fdrop fdrop fdrop fdrop exit  then
    {: tri F: x y z yaw :}  id walk-move tri -1  x y z yaw  move  id wait-move ;
: with-fiona-free ( -- )  begin  fiona-free? 0= while  yield  repeat ;
\ a door locked from this side (Progress_CharRequests' request $80000002: she in an exit's door
\ area, its door locked - the room may let her unlock it, else her own try: locked from the other side)
$72 constant unlock-sound   $8008 constant unlocked-msg   \ (the common bank's; "You unlocked the door.")
: before-locked? ( exit -- flag )
    {: ex :}  fiona here? 0=  ex door-here? 0= or if  false exit  then
    ex exit-door dup 0< if  drop false exit  then  doors:door-locked? 0= if  false exit  then
    fiona body-pos ex 0 door-in-area? ;
: unlocks ( exit -- )   \ (to her spot by it - not with the stalker on her - a try, and it gives: unlocked)
    {: ex :}  stalker-in?  fiona stalker 0 close-to? and if  exit  then
    fiona ex to-door  stalker-in?  fiona stalker 0 close-to? and  in-panic? or if  exit  then
    fiona body-pos ex 0 door-in-area? if  fiona fiona.anims:rattle-front-anim  else  fiona fiona.anims:rattle-behind-anim  then  start-anim
    20 frames  in-panic? if  exit  then
    unlock-sound common-bank fiona sound-by  ex exit-door unlock  unlocked-msg show
    fiona 20 idle  20 frames  wait-closed ;

: people-in ( -- )   \ (each person newly in the room: its arrival)
    #people 0 do
        i person-id dup 0< 0= if  dup body? if  body-room this-room @ =  else  drop false  then  else  drop false  then
        i stalker-p = if  stalker-told? and  then   \ (the stalker in once he has told us what he is)
        dup i cells in-here + @ 0= and if  i ['] d.person-in run-desc-with  then
        i cells in-here + !
    loop ;
: text-gone ( -- )  shown-msg @ 0< if  exit  then  shown-msg @ window send close-text  -1 shown-msg ! ;   \ (ours left up: closed)
: frame-start ( -- )  0 offer-xt !  0 offer-msg !  broadcast zone-targets-off ;   \ (the breakables say so again)
\ what every room does, played as a place (story/common.fs: the original's shared scripts run
\ after a room's own phases - crouching, ... - and the item uses' scripts)
defer every-room ( -- )   ' noop is every-room
defer item-used ( who ev var value -- )   :noname  2drop 2drop ; is item-used
: room-starts ( -- )   \ (as the converted rooms start: visited, no track wanted till the room asks)
    this-room @ progress pr.visited bit-set  no-track 1e background  0 onset ! ;
: camera-starts ( -- )   \ (CamDirector_RoomStart: from room to room the one followed taken again, its setup placed)
    cam-room-start  came-by @ 0< 0= if  cam-followed cam-on  then ;
: camera-placed ( -- )  came-by @ 0< 0= if  cam-update  then ;
behaviour placing
  on spawned ( -- )  #creatures 0 do  100 i cells cr-age + !  loop ;   \ (no creature heard of yet; the stalker: st-heard - story may tell him before this)
  on room-enter ( room exit -- )   \ (queued as the room came in)
      over room-id <> if  2drop exit  then
      came-by !  dup this-room !  place-of desc !  -1 entered !
      -1 going !  -1 shown-msg !  #people 0 do  -1 i holder-of !  -1 i cells move-done + !  loop   \ (none held, none moving)
      self subscribe tick  self subscribe frame-end  self subscribe moving  self subscribe danger
      self subscribe panic  self subscribe fiona-doing  self subscribe hewie-doing
      self subscribe stalker-doing  self subscribe creature-doing  self subscribe panic-onset
      self subscribe fiona-requested  self subscribe fiona-unseen  self subscribe fiona-shaking  self subscribe fiona-message  self subscribe fiona-struck-by  -1 struck-by-now !
      self subscribe hewie-health  self subscribe fiona-started  -1 started-now !  self subscribe fiona-straining
      self subscribe item-event
      room-starts  frame-start  came-by @ ['] d.came-in run-desc-with  people-in  remember-people
      camera-starts  camera-placed ;
  on tick ( -- )
      entered @ 0= if  exit  then
      people-in  frame-start  ['] d.frame run-desc  every-room  0 request-now !  -1 started-now !   \ (read: gone)
      offer-out  onset @ 0= if  run-acts  then ;
  on frame-end ( -- )
      entered @ 0= if  exit  then  remember-people
      st-age @ 100 < if  1 st-age +!  then  #creatures 0 do  i cells cr-age + dup @ 100 < if  1 swap +!  else  drop  then  loop
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
      sender person-of dup 0< if  drop  3 0 do  i 3 + story.words:slot-actor @ sender = if  over i cells figure-ended + !  2 pick i cells figure-done + !  then  loop  drop 2drop exit  then
      tuck cells marks + !  tuck cells anim-ended + !  cells move-done + ! ;
  on scene-ready ( movie? -- )  scene-movie !  scene-is-ready scene-step ! ;
  on scene-shot ( shot -- )  cue-xt @ ?dup if  execute  else  drop  then ;
  on scene-over ( -- )  scene-ended scene-step ! ;
  on tab-done ( -- )  0 tab-moving ! ;
  on danger ( level -- )  danger-now ! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      {: room mode sub cond hp hpmax kind alert came :}
      sender stalker =  sender s" story" actor-named = or  0= if  exit  then   \ (his own, or story's copy as we came in)
      room st-room-now !  mode st-mode-now !  cond st-cond-now !  kind st-kind-now !  alert st-alert-now !  came st-came-now !  0 st-age !  -1 st-heard ! ;
  on creature-doing ( slot room mode up -- )
      {: slot room mode up :}  slot 0 #creatures within 0= if  exit  then
      room slot cells cr-room + !  mode slot cells cr-mode + !  0 slot cells cr-age + ! ;
  on panic ( stage level -- )  cell>f f>s panic-level-now !  panic-stage ! ;
  on fiona-doing ( mode sub cond cmd doing -- )  fiona-doing-now !  fiona-cmd-now !  drop drop  fiona-mode-now ! ;
  on fiona-struck-by ( who -- )  struck-by-now ! ;
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      {: here act mode sub cond mood group cmdable near reach breakable way trust :}
      act hewie-act-now !  cond hewie-cond-now !  reach hewie-reach-now !  cmdable hewie-cmdable-now !  near hewie-near-now !  breakable hewie-breakable-now !
      way hewie-way-now ! ;
  on panic-onset ( on -- )  onset ! ;
  on fiona-requested ( kind -- )  request-now ! ;
  on fiona-unseen ( on -- )  unseen-now ! ;
  on hewie-health ( hp -- )  hewie-hp-now ! ;
  on fiona-started ( code -- )  started-now ! ;
  on fiona-straining ( on -- )  straining-now ! ;
  on fiona-shaking ( n -- )  shook-now ! ;
  on fiona-message ( msg -- )   \ (her locked door's: not while Hewie is the one played)
      leaving @  shown-msg @ 0< 0= or  hewie-played? or if  drop exit  then  show ;
  on item-event ( who ev var value -- )   \ (the room's own, else what every room does)
      leaving @ if  2drop 2drop exit  then
      ['] d.item hook ?dup if  >r 2over 2over r> execute if  2drop 2drop exit  then  then  item-used ;
  on room-leave ( -- )  -1 leaving ! ;
  on room-left ( -- )  scene-step @ scene-running = if  scenes send scene-stop  then  text-gone  stop-all  effects-gone  ['] d.went run-desc  s" story" actor-named send room-done ;
  on story-stop ( -- )  text-gone  stop-all  effects-gone  s" story" actor-named send room-done ;
end-behaviour

\ a place's actor for its room, ready to be entered (story.fs)
: place-spawn ( room -- id )  placing place-state rot room-name spawn ;
