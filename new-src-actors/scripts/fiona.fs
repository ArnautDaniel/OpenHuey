\ fiona.fs - Fiona, the player (docs/subsystems/fiona.md). Phase F1: her body and model, the
\ controls, standing, walking, running and turning, her footsteps, going out by exits; F4: struck
\ and caught (fiona/hurt.fs).
IN: fiona
USING: engine game-state facts actors messages common paths doors saves fiona.state fiona.model fiona.controls fiona.moving fiona.spots fiona.doors fiona.slam fiona.ladder fiona.push fiona.fear fiona.looks fiona.commands fiona.moves fiona.hurt fiona.seized fiona.defend fiona.voice fiona.letgo flag-names ;

: rooms-id ( -- id )  s" rooms" actor-named ;
: camera-id ( -- id )  s" camera" actor-named ;

\ ---- arriving (by an exit: on its outside spot - Rooms_ExitPointOut - her heading kept from
\ going through, as the original's: Fiona_Vt34 saves it, the scripts' 0x04 places without
\ turning) ----
fvariable ix  fvariable iy  fvariable iz
: put ( F: x y z yaw -- )
    fdup her-yaw f!  her-heading f!
    her-at 8 + sf!  her-at 4 + sf!  her-at sf!
    her-at blocked-floor v-tri-in dup 0< if  drop her-at v-tri  then  her-tri !
    her-at her-was-at 12 move  her-tri @ her-was-tri !
    show-her ;
: arrive-anywhere ( -- )   \ (no exit: by the room's first camera's target, else its middle)
    room-cameras if  0 room-camera  iz f! iy f! ix f!  fdrop fdrop fdrop fdrop  ix f@ iy f@ iz f@
    else  0e 0e 0e  then
    nav-tris if  nav-nearest  then  0e put ;
: arrive-at ( exit -- )   \ (the room scripts' char-to-exit: its outside spot, her heading kept)
    dup 0 exit-spot 0< if  fdrop fdrop fdrop  drop arrive-anywhere exit  then
    drop  her-yaw f@ put ;
: idle ( -- )   \ Fiona_ToIdle: free, standing, her moves her own
    0 her-doing !  0 her-mode !  her-yaw f@ her-heading f!  0 her-rest !   \ (the turn mode kept: Fiona_ToIdle leaves +0x1AD588)
    -1 idle-anim  ['] idle-move her-act ! ;
' idle is fiona.defend:to-idle   ' idle is fiona.hurt:to-idle   ' idle is fiona.seized:to-idle   ' idle is fiona.doors:to-idle   ' idle is fiona.fear:to-idle   ' idle is fiona.commands:to-idle   ' idle is fiona.moves:to-idle   ' idle is fiona.ladder:to-idle   ' idle is fiona.push:to-idle   ' idle is fiona.letgo:to-idle
' fiona.ladder:knocked-off is fiona.hurt:knocked-off
' fiona.doors:scripted-door is fiona.moves:door-use
' fiona.doors:drop-door is fiona.hurt:drop-door
' try-push is fiona.moving:push-check
\ Fiona_Vt34: going out by a door - with the stick held, she faces where it points (by the camera
\ she leaves) if that is less than a quarter turn round from her heading, and that camera keeps
\ steering in the next room while the stick stays (turn mode 3: controls.fs after-cut)
fvariable dl
: fatan2-xz ( F: dx dz -- a )  fatan2 ;   \ (a direction's heading, as stick-heading takes it)
: through-door ( -- )
    her-scripted @ if  exit  then
    her-stick sf@ fabs 0.5e f> her-stick 8 + sf@ fabs 0.5e f> or 0= if  exit  then
    her-stick sf@ her-stick 8 + sf@ vlen fdup f0= if  fdrop exit  then  dl f!
    cam-yaw  her-stick sf@ dl f@ f/  her-stick 8 + sf@ dl f@ f/  turned  fatan2-xz     ( F: heading )
    fdup her-yaw f@ f- angle-wrap pi f2/ f< if
        fdup her-yaw f!  her-heading f!  3 her-turn-mode !
    else  fdrop  then ;
: fresh ( -- )   \ standing, the controls as new
    0 her-doing !  0 her-mode !  0 her-sub !  6 her-still !  0 her-lock !
    her-turn-mode @ 3 <> if  0 her-turn-mode !  then   \ (a door's held stick kept through the arrival)
    1e her-stick-k f!  -1 her-variant !  1e her-tired-w f!  0 her-rest !  0 her-run-t !
    -1 her-cmd !  0 her-busy-t !  0 her-scripted !  0 her-move !  -1 her-move-done !  1 her-step-l !  1 her-step-r !  -1 her-door-exit !  her-path path-clear  stand  idle ;

\ arriving by an exit while walking or running on her own, she keeps on into the room: her
\ stride and the held stick carry over (Fiona_Vt34 stops her only when she was busy)
: going-on? ( -- flag )
    her-mode @ 0=  her-doing @ 0= and  her-scripted @ 0= and  her-sub @ 1 3 within and ;
: carry-on ( -- )
    -1 her-cmd !  0 her-busy-t !  0 her-move !  -1 her-move-done !  -1 her-door-exit !  her-path path-clear ;
: arrive ( exit -- )  arrive-at  going-on? if  carry-on  else  fresh  then ;

\ ---- walking to a point (go-to: the console's, later the story's moves): her way over the nav
\ mesh, arriving facing the way she walked ----
create goal 12 allot
: going ( -- )  walk-to-spot dup 0< if  drop idle exit  then  if  exit  then  idle ;
: go-to-point ( F: x y z -- )
    goal vec!  goal blocked-floor v-tri-in dup 0< if  drop exit  then
    goal vec@  her-at goal vec-heading  walk-spot
    2 her-mode !  $14 her-doing !  ['] going her-act ! ;

\ ---- keeping out of the others (Fiona_KeepApart), while she is free: overlapping the stalker
\ she is pushed out to their radii apart - along his way when he is coming at her (his heading
\ 20 on, plus the way from him to her), else straight away from him - unless that is off the
\ floor; overlapping Hewie (not at the frame's start) back to where she was ----
fvariable ap-x  fvariable ap-z  create ap-at 12 allot
: overlaps? ( id -- flag )  dup 0< if  drop false exit  then  self swap 0e 0e bodies-touching? ;
: coming-at-her? ( id -- flag )   \ (his heading against the way from him to her)
    dup body-yaw fsin ap-x f@ f*  body-yaw fcos ap-z f@ f* f+ f0> ;
: push-from ( id -- )
    {: him :}
    her-at sf@ him body-at sf@ f- ap-x f!  her-at 8 + sf@ him body-at 8 + sf@ f- ap-z f!
    self body-dims fdrop  him body-dims fdrop f+  ap-x f@ ap-z f@ vlen f-     ( F: gap )
    him coming-at-her? if
        0e 20e him body-yaw rotate-by  ap-z f@ f+ ap-z f!  ap-x f@ f+ ap-x f!
    then
    ap-x f@ ap-z f@ vlen fdup f0= if  fdrop fdrop exit  then               ( F: gap len )
    f/  fdup ap-x f@ f* her-at sf@ f+ ap-at sf!
    ap-z f@ f* her-at 8 + sf@ f+ ap-at 8 + sf!  her-at 4 + sf@ ap-at 4 + sf!
    ap-at blocked-floor v-tri-in dup 0< if  drop exit  then
    her-tri !  her-at ap-at vec-copy ;
: keep-apart ( hewie-was? -- )
    her-mode @ 0<>  her-scripted @ or if  drop exit  then
    stalker-in-room dup overlaps? if  nip push-from exit  then  drop
    if  exit  then
    dog-in-room overlaps? if  her-at her-was-at vec-copy  her-was-tri @ her-tri !  then ;

\ ---- each frame (Fiona_Update, the parts built so far) ----
: frame ( -- )
    her-at her-was-at 12 move  her-tri @ her-was-tri !  her-yaw f@ her-yaw-was f!
    look-cleared
    feel-the-panic
    read-controls
    her-mode @ 0= if  panic-controls  else  false  then  0=  hands-off? 0= and if  control-command  then
    hands-off? if  -1 her-cmd !  else  read-command  try-flee  try-defend  try-doors  try-ladders  try-let-go  then
    dog-in-room overlaps?  her-act @ execute  keep-apart
    looks  tell-point
    command-sounds  motion-sounds
    footsteps
    show-her ;

\ her model by costume (progress variable $26, the story's 0x97: event.c fiona_model) - 0 her
\ clothes (O_FIN), 1 the slip she wakes in (O_FIS), 2 O_FIH, 3 O_FIO, 6 O_FIB, 7 O_FIC, 8 O_FIF -
\ and by how far the story has gone, worn and torn (Fiona_LoadFiles: the last of story flags $41,
\ $6F, $82, $88, $94, $95 set - variants 1..6; the slip has none). A variant has no textures of its
\ own (its _000's); the markers are the costume's own or her clothes' (CharModel_MarkerFile)
: costume-dir ( n -- addr len )   \ (its files' names but the number)
    case
        1 of  s" O_FIS/FIS_"  endof   2 of  s" O_FIH/FIH_"  endof   3 of  s" O_FIO/FIO_"  endof
        6 of  s" O_FIB/FIB_"  endof   7 of  s" O_FIC/FIC_"  endof   8 of  s" O_FIF/FIF_"  endof
        >r s" O_FIN/FIN_" r>
    endcase ;
: own-markers? ( n -- flag )  dup 0= over 6 = or swap 7 = or ;
: worn ( -- v )
    0  $41 story-flag? if  drop 1  then  $6F story-flag? if  drop 2  then  $82 story-flag? if  drop 3  then
    $88 story-flag? if  drop 4  then  $94 story-flag? if  drop 5  then  $95 story-flag? if  drop 6  then ;
create model-name 32 allot  create tex-name 32 allot  create marker-name 32 allot
: named ( buf costume v -- addr len )   \ ("O_FIN/FIN_003")
    {: buf c v | n :}
    c costume-dir dup to n  buf swap move
    [char] 0 buf n + c!  [char] 0 buf n + 1+ c!  v [char] 0 + buf n + 2 + c!
    buf n 3 + ;
: wear ( n -- )
    her-costume !
    her-model @ dup 0< 0= if  actor-free  else  drop  then   \ (-1: none yet)
    model-name her-costume @  her-costume @ 1 = if  0  else  worn  then  named
    tex-name her-costume @ 0 named  actor-load-textured dup her-model !
    dup 0< 0= if  0 over actor act.doorway-fade l!  then   \ (she never fades in doorways: Fiona_LightChange keeps her layer)
    0 over cast-as  $3D5CC0 motion-table   \ (CharModel_SecondaryMotion: her fades and flags)
    her-costume @ own-markers? if  marker-name her-costume @  her-costume @ 1 = if  0  else  worn  then  named
    else  marker-name 0 worn named  then
    her-model @ -rot actor-markers
    self body? 0<> 1 and her act.visible l!  0 -1 play-table ;   \ (shown once she is in a room)

\ out of sight (Fiona_Frame's area fade, +0x1AD630): wearing item $89 in the equipment's slot 1 and
\ standing still, after 60 frames she can't be seen - the homunculi only circle her; any move, and
\ she can be seen again (told when it changes: fiona-unseen). On screen (Fiona_LightChange) she
\ fades out as she goes unseen, and once gone she shows softened (layer $1C: drawn at half size) -
\ only while she wears it; else her layer is the story's.
: unseen! ( flag -- )  dup her-unseen @ = if  drop exit  then  dup her-unseen !  broadcast fiona-unseen ;
$89 constant camouflage   \ (the item worn in the equipment's slot 1 that hides her)
$1C constant softened-layer
: camouflaged? ( -- flag )  progress pr.equipped 4 + l@ camouflage = ;
: fade-frame ( -- )   \ (FIONA_FADE: 8 a frame out while unseen, else back in)
    her-unseen @ if  -8  else  8  then  her-fade +!  her-fade @ 0 max $80 min her-fade !
    her-model @ dup 0< if  drop exit  then  actor  {: a :}
    camouflaged? 0= if  a act.layer l@ softened-layer = if  0 a act.layer l!  then  1e a act.alpha sf! exit  then
    her-fade @ 0= if  softened-layer a act.layer l!  1e a act.alpha sf! exit  then   \ (gone: softened)
    0 a act.layer l!  her-fade @ s>f 128e f/  a act.alpha sf! ;
\ faded from sight, the stalker walking into her (on his feet, not busy; she not held): he has
\ her (a hold, Fiona_Update's request 4 / 6, how 3)
: bumped ( -- )
    her-fade @ 0<>  her-st-cond @ 2 = or  her-mode @ 4 = or if  exit  then
    stalker-in-room dup 0< if  drop exit  then
    self over 1e 0e bodies-touching? 0= if  drop exit  then
    0 her-still-t !  false unseen!  6 3 rot 0e struck drop ;
: unseen-frame ( -- )
    fade-frame
    her-carried-t @ 0> if  -1 her-carried-t +!  then
    progress pr.equipped 4 + l@ $89 =  her-mode @ 0= and  her-sub @ 0= and 0= if  0 her-still-t !  false unseen! exit  then
    her-unseen @ if  bumped exit  then
    1 her-still-t +!  her-still-t @ 60 >= if  true unseen!  then ;

\ a script letting go of her (its end, or cut off: the room left) - no longer crouched where she
\ was hiding (Fiona_FullStop: its flags 9 and $A cleared; only the crouch sets $A)
: crouch-over ( -- )  fiona-half-hidden state-flag-clear ;

behaviour free
  on spawned ( -- )
      -1 her-model !  -1 her-hit-who !  -1 her-shake-count !  her-creatures 10 cells -1 fill  progress pr.vars $26 + c@ wear  2e 15e body-size
      self subscribe tick  self subscribe leaving-room  self subscribe arrived  self subscribe camera-cut  self subscribe write-save  self subscribe creature-doing
      self subscribe danger  self subscribe panic  self subscribe hewie-doing  self subscribe frame-end  self subscribe text-shown  self subscribe scene  self subscribe stalker-doing ;
  on leaving-room ( room exit -- )  2drop  through-door ;
  on arrived ( room exit -- )  nip arrive  1 her act.visible l!  self camera-id send follow ;   \ (the story's scripts place her too)
  on to-exit ( exit -- )  arrive ;
  \ examining (Progress_PlayerButtons): an action offered here, taken with the action button
  \ while she is free; a script of the room has her, or lets go; a message on screen
  on offer ( scene arg -- )
      her-mode @ 0= her-doing @ 0= and  hands-off? 0= and  action-button? and
      sender alive? and  if  sender send take-offer  else  2drop  then ;   \ (its room gone meanwhile: nobody to answer)
  on scripted ( on -- )  idle  dup 0= her-scripted @ 0<> and if  crouch-over  then  her-scripted ! ;
  on noclip ( on -- )  her-noclip ! ;   \ (the story's 0x20)
  on aside ( on -- )  her-aside ! ;     \ (0x21: out of reach of attacks)
  on activate ( -- )  0 her-noclip !  0 her-aside !  idle ;   \ (the story's 0x15, Fiona_Activate: afresh, standing)
  on costume ( n -- )  wear  show-her ;
  on fiona-request ( kind x y z threat -- )   \ (a creature's, Progress_Noise: $D a spirit - she falls; 3 a red one
      >r cells>xyz from-pt vec!   \ going - grabbed; $C homunculi - held down)
      0 -1 r> cell>f struck drop ;   \ (its threat her fright: Fiona_React)
  on throw-down ( who recover -- )  thrown-down ;   \ (the story's 0xB2)
  on look-hold ( who frames -- )  her-look-hold !  her-look-who !  1 her-look-on ! ;   \ (the story's 0x96: Fiona_SetTarget)
  on creature-doing ( slot room mode up -- )   \ (a homunculus in her room holding her; each creature remembered)
      drop
      2 pick 0 10 within if  sender  3 pick cells her-creatures + !  then
      8 =  swap self body-room =  and  swap 7 >= and if  2 her-carried-t !  then ;
  on write-save ( -- )   \ (saves.fs) her costume and place
      her-costume @ .  self body-pos .at  self body-yaw .f  ." fiona-was" cr ;   \ (the new game; the story's 0x97)
  on text-shown ( on -- )  her-reading ! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )  2drop 2drop drop  her-st-cond !  drop her-st-mode !  drop ;
  on scene ( on -- )  her-in-scene ! ;   \ (a cutscene has her)
  on go-to ( x y z -- )  cells>xyz  go-to-point ;
  on place ( x y z yaw -- )  cells>xyzw  put  fresh ;   \ (the story puts her)   \ (the room's scripts: Rooms_ExitPointOut, facing in)
  on camera-cut ( -- )  -1 her-cut ! ;
  \ struck or caught (F4: fiona.hurt)
  on hit ( kind how fright -- )
      cell>f  over >r  sender struck if  r> sender send hit-taken  else  r> drop  then ;
  on seize ( type -- )  sender swap seized if  sender send seize-taken  else  sender send seize-refused  then ;
  on unhand ( -- )  0 her-led ! ;
  on flinch ( -- )  flinch ;   \ (a room's command)
  on count-shakes ( on? -- )  if  0 her-shake-count !  -1 her-shake-rest !  else  -1 her-shake-count !  then ;
  on blow-taken ( -- )  struck-home ;   \ (her shove or kick struck home)
  on danger ( level -- )  her-danger ! ;
  on panic ( stage level -- )  cell>f her-panic-level f!  her-panic-stage ! ;
  on door-held ( room exit -- )  2drop  got-hold ;
  on door-refused ( room exit -- )  2drop  -1 her-move-done !  idle ;
  on tick ( -- )  self body? if  frame  then ;
  on frame-end ( -- )   \ what she is doing, as the frame left her; her move
      self body? 0= if  exit  then
      unseen-frame
      her-mode @ her-sub @ her-cond @ her-cmd @ her-doing @ broadcast fiona-doing
      her-hit-who @ broadcast fiona-struck-by
      shook-from-rest broadcast fiona-shaking
      her-shake-count @ 0< 0= if  shakes-now her-shake-count +!  her-shake-count @ broadcast fiona-shakes  then   \ (a story counting them)
      plight  her-mode @ 3 = if  her-ladder @  else  -1  then  broadcast fiona-plight
      tell-head
      her-move-done @  ended?  her-model @ 0 0 1 motion-events  broadcast moving ;
  \ the story's moves (fiona.moves)
  on scripted-move ( kind a b x y z yaw -- )
      cell>f her-move-yaw f!  cells>xyz her-move-at vec!
      her-move-b !  her-move-a !  dup her-move !  start-move ;
  on hold-anim ( anim flags blend -- )   \ (the scripts' 0x9D: in it, held - its flags hers)
      nip -1 play-blend  2 her-mode !  $11 her-doing !  ['] root-move her-act !  done ;
  on show ( on -- )  0<> 1 and her act.visible l! ;
  \ the story's (F5): calmer (Fiona_CalmDown: n / 30 off her fear), her recovery shorter
  \ (Fiona_LowerRecovery: 60 / n frames)
  on remedy ( fear recovery -- )   \ (the items': item_meters - her fear 0..100, her recovery's count 0..1800)
      her-recovery @ + 0 max 1800 min her-recovery !  s>f her-fear f@ f+ 0e fmax 100e fmin her-fear f! ;
  on calm-reset ( -- )  0e her-fear f!  0 her-recovery ! ;
  on recovery-reset ( -- )  reset-recovery ;
  on fear-to ( v -- )  0 max 100 min s>f her-fear f! ;
  on fear-less ( n -- )  s>f 30e f/ fnegate her-fear f@ f+ 0e fmax her-fear f! ;
  on recovery-less ( n -- )  1 max 60 swap / negate her-recovery +!  her-recovery @ 0 max her-recovery ! ;
  \ Hewie (F3)
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      2drop 2drop 2drop  dog-group !  dog-mood !  dog-cond !  dog-sub !  dog-mode !  dog-action !  dog-here ! ;
  on meet-at ( tri x y z face -- )
      asked? 0= if  2drop 2drop drop  hewie-id send meet-off exit  then
      >r >r >r cell>f  r> cell>f  r> cell>f  r> cell>f  meet-placed ;
  on meet-on ( -- )  her-act @ ['] meet-waiting = if  meet-started  else  hewie-id send meet-off  then ;
  on meet-refused ( -- )  her-act @ ['] meet-waiting = if  meet-failed  then ;
end-behaviour

: fiona-spawn ( -- id )  free fiona-state s" fiona" spawn ;
