\ events/play.fs - the event scripts in the game: while playing, each room's scripts run as in
\ the original (events/runner.fs), with Fiona and Hewie as the scripts' characters.
\ What isn't there yet: most of the words (each says so on the console the first time it runs;
\ `.missing` lists them) and characters walking their scripted moves.
IN: events.play
USING: engine state rooms player hewie doors game-state events.core events.words events.runner events.strings ;

\ the scripts' characters: slot 0 Fiona (script id 0), slot 1 Hewie (script id 1)
: cast ( -- )
    0 0 character char.id l!  1 1 character char.id l!
    fiona @ dup 0 character char.actor l!  0< 0= 0 character char.present l!
    hewie @ dup 1 character char.actor l!  0< 0= hewie-along @ and 1 character char.present l! ;

\ Hewie out of the scene and back (the scripts' char-done / char-activate)
: show-hewie ( on -- )  hewie @ 0< if  drop exit  then  0<> 1 and hewie @ actor act.visible l! ;
:noname ( cs -- )  1 = if  0 hewie-along !  0 show-hewie  then ; is event-char-out
:noname ( cs -- )  1 = if  -1 hewie-along !  -1 show-hewie  then ; is event-char-in

\ a character at the outside point of this room's exit (Rooms_ExitPointOut: exit-spot 0)
: f3! ( addr -- ) ( F: x y z -- )  dup 8 + sf!  dup 4 + sf!  sf! ;
: f3@ ( addr -- ) ( F: -- x y z )  dup sf@  dup 4 + sf@  8 + sf@ ;
\ (the scripts' copy of where it is moves with it, as the original's char_place moves the
\ character itself)
: at-exit ( cs exit -- )
    over character char.actor sl@ 0< if  2drop exit  then
    0 exit-spot 0< if  fdrop fdrop fdrop drop exit  then       ( cs )
    character dup char.pos f3!  dup char.pos f3@  dup char.prev f3!
    dup char.pos f3@  char.actor sl@ actor dup act.z sf!  dup act.y sf!  act.x sf! ;
' at-exit is place-at-exit

\ the camera: the game's director (game/camdirector.c) instead of player.fs's
: actor-of ( cs -- actor | -1 )  dup 0< if  exit  then  character char.actor sl@ ;
:noname ( set path -- )  cam-setup ; is director-setup
:noname ( cs -- )  actor-of cam-follow ; is director-follow
' cam-restart is director-restart       ' cam-changed? is director-changed?
' cam-new-room is director-new-room     ' cam-room-start is director-room-start
' cam-ease is director-ease             ' cam-track is director-track
\ (a room come into without an exit - a jump, the start - may leave the camera no set: then
\ player.fs's chase camera stands in)
: directed? ( -- flag )
    event-state ev.camera-char sl@ dup $FF = if  drop true exit  then
    character char.cam-set sl@ 0< 0= ;
:noname  directed? if  cam-update  then ; is director-update
:noname  directed? 0= if  follow  then ; is steer-camera
\ the room's doors and nav mesh
' exit-door is event-exit-door   ' door-flags is event-door-flags
' nav-group! is event-nav-group   ' nav-tri-flags! is event-nav-tri
:noname ( group -- flag ) ( F: x y z -- )  nav-tri swap nav-in-group? ; is event-nav-in-group?
\ the room's event areas
' message-param! is message-parameter
' look-set is event-look
' tri-center is event-tri-center
' bank-sound is event-sound   ' bank-sound-at is event-sound-at   ' sound-set! is event-sound-set
' area-middle is event-area-middle   ' area-in? is event-area-in?   ' area-cross is event-area-cross   ' exit-area is event-exit-area
' room-string is event-room-string   ' placed-op is event-object
\ movies and the cutscene director (as the original's, a movie's frames time a scene played in
\ the room)
:noname ( addr len -- )  movie-open drop ; is event-movie-open
:noname  movie-close ; is event-movie-stop
' movie-compose is event-movie-compose
' movie-pause is event-movie-pause   ' movie-volume! is event-movie-volume   ' movie-frame is event-movie-frame
:noname ( -- n )  movie-status 1 <> if  -1  else  movie-paused? if  2  else  1  then  then ; is event-movie-state
:noname ( addr len -- )  cutscene-load drop ; is event-scene-start
' cutscene-run is event-scene-run       ' cutscene-go is event-scene-go
' cutscene-frame! is event-scene-frame!  ' cutscene-update is event-scene-update
' cutscene-end is event-scene-end       ' cutscene-status is event-scene-status
' cutscene-in-shot? is event-scene-in-shot?   ' cutscene-frame is event-scene-frame
' cutscene-near? is event-scene-near-end?    ' cutscene-shot-at is event-scene-shot-at
' cutscene-signals is event-scene-signals    ' cutscene-signal-total is event-scene-total

\ the characters' places from their actors (now, and the frame before)
: places ( -- )
    characters 0 do
        i actor-of dup 0< if  drop  else
            actor dup act.x sf@  dup act.y sf@  act.z sf@  i character char.pos f3!
        then
    loop ;
: remember-places ( -- )
    characters 0 do  i character dup char.pos f3@  char.prev f3!  loop ;

\ ---- characters under the scripts (the original's character update carries out their moves) ----
:noname ( n -- flag )  character char.scripted sl@ 0<> ; is scripted?

fvariable pl-x  fvariable pl-y  fvariable pl-z
\ placed: on the floor below the point (looked for from a little above it), else as given
: place-char ( cs -- ) ( F: x y z -- )
    pl-z f! pl-y f! pl-x f!
    pl-x f@ pl-y f@ 8e f+ pl-z f@ floor-below if  pl-y f!  then
    dup character char.pos  pl-x f@ pl-y f@ pl-z f@ f3!
    dup character char.prev pl-x f@ pl-y f@ pl-z f@ f3!
    actor-of dup 0< if  drop exit  then  actor >r
    pl-x f@ r@ act.x sf!  pl-y f@ r@ act.y sf!  pl-z f@ r> act.z sf! ;
' place-char is event-char-place
:noname ( cs -- ) ( F: a -- )  actor-of dup 0< if  drop fdrop exit  then  actor act.yaw sf! ; is event-char-yaw
:noname ( cs -- ) ( F: -- a )  actor-of dup 0< if  drop 0e exit  then  actor act.yaw sf@ ; is event-char-heading
:noname ( cs on -- )  swap actor-of dup 0< if  2drop exit  then  actor act.visible >r  0<> 1 and r> l! ; is event-char-show
\ (a motion its model hasn't: nothing plays, and it counts as played)
create no-anim  characters cells allot  no-anim characters cells 0 fill
: char-anim ( cs anim loop? -- )
    rot dup >r actor-of dup 0< if  r> drop drop 2drop exit  then
    rot 2dup has-motion? 0= if  2drop drop  true r> cells no-anim + !  exit  then
    false r> cells no-anim + !  rot                            ( actor anim loop? )
    >r over actor act.loop r> 0<> 1 and swap l!  motion! ;
' char-anim is event-char-anim
:noname ( cs -- flag )
    dup cells no-anim + @ if  drop true exit  then
    actor-of dup 0< if  drop true exit  then  motion-done? ; is event-char-anim-done?

\ walking / running to a point (moves 5 / 10): straight there over the nav mesh, then facing
: anim-move? ( move -- flag )  dup 7 = over 8 = or swap $10 = or ;
: walk-move? ( move -- flag )  dup 5 = swap $A = or ;
: idle-anim ( cs -- )  dup 0= if  m-idle  else  h-stand  then  -1 char-anim ;
: going-anim ( cs run? -- )
    over 0= if  if  m-run  else  m-walk  then  else  if  h-run  else  h-walk  then  then  -1 char-anim ;
: arrive ( cs -- )
    dup character char.face sf@ fdup 9e f< if  dup event-char-yaw  else  fdrop  then
    dup idle-anim  move-done ;
fvariable go-x  fvariable go-z  fvariable go-dx  fvariable go-dz  fvariable go-d  fvariable go-s
: go-step ( cs -- )
    dup character char.target dup sf@ go-x f!  8 + sf@ go-z f!
    dup char-pos  go-z f@ fswap f- go-dz f!  fdrop  go-x f@ fswap f- go-dx f!
    go-dx f@ fsq go-dz f@ fsq f+ fsqrt go-d f!
    dup character char.move sl@ $A = if  1.4e  else  0.5e  then  go-s f!   \ (units a frame)
    go-d f@ go-s f@ f<= if                               \ there: on the point
        dup char-pos fdrop fswap fdrop  go-x f@ fswap go-z f@  dup place-char  arrive exit
    then
    dup dup character char.move sl@ $A = going-anim
    go-dx f@ go-dz f@ fatan2  dup event-char-yaw
    dup 0= if  $28020018  else  $29020008  then  nav-block!   \ (the game's masks)
    dup char-pos  go-dx f@ go-d f@ f/ go-s f@ f*  go-dz f@ go-d f@ f/ go-s f@ f*  step-up 2.5e nav-move
    0 nav-block!  place-char ;
: moves ( -- )   \ each frame, after the scripts
    characters 0 do
        i character char.present sl@ if  i character char.move-done sl@ 0= if
            i character char.move sl@
            dup walk-move? if  drop i go-step  else
            anim-move? if  i event-char-anim-done? if  i move-done  then  then  then
        then  then
    loop ;

\ ---- a new game: as the original sets up its entry 0x2A (SceneGame_StateEntry): a fresh
\ progress, state flags 3, 8 and $28, Fiona's costume variable $26 = 1 and $27 = 0, then room
\ $2A, whose entering script does the rest (Fiona in her cage, Hewie away, the doors locked) ----
variable started   \ (the camera director set up for play)
: start-new-game ( -- )
    progress-reset  reset-characters  reset-events  -1 event-state ev.room l!  0 started !
    3 state-flag-set  8 state-flag-set  $28 state-flag-set
    $26 1 pvar-set  $27 0 pvar-set
    fiona @ 0< 0= if  fiona @ actor-free  -1 fiona !  then   \ (her model for that costume)
    $2A go  -1 came-in-by !  start-playing ;
' start-new-game is new-game
\ her model by costume (the original's CharLoad_*: 0 her clothes, 1 the slip she wakes in)
:noname ( -- addr len )
    progress pr.vars $26 + c@ 1 = if  s" O_FIS/FIS_000"  else  s" O_FIN/FIN_000"  then ; is fiona-model

: leaving  playing @ event-state ev.room sl@ 0< 0= and if  leave-room  then ;
' leaving is leaving-room

\ exits: a locked door stays shut; otherwise the door opens and she steps into the doorway (the
\ exit's "in" spot, in its area), where the room's phase 1 sees her and takes the exit
:noname ( exit -- flag )  event-exit-door dup 0< if  drop false  else  door-locked  then ; is exit-locked?
: step-in ( exit -- )
    dup event-exit-door dup 0< 0= if  door-open-set  else  drop  then
    1 exit-spot 0< if  fdrop fdrop fdrop exit  then  place-fiona ;
' step-in is use-exit
\ an exit the scripts took (exit-check): through it after the frame; its door shuts behind
: take-exit ( -- )
    exit-wanted @ dup 0< if  drop exit  then  -1 exit-wanted !
    dup event-exit-door dup 0< 0= if  door-open-clear  else  drop  then
    go-through ;

\ her action button (Enter): a request the room's scripts made this frame (Progress_PlayerButtons:
\ 5 starts her action script)
: action-button ( -- )
    request @ 5 <>  0 scripted? or  event-state ev.message sl@ 0< 0= or if  exit  then
    s" Enter: look" hud
    key: Return key-pressed? if  0 0 request-arg @ action  0 0 hud  then ;

\ the doors' models: open (a quarter turn) or shut as their exits are (Doors_RoomIn), swinging
\ when that changes
: doors-follow ( at-once -- )
    8 0 do  i dup exit-open if  -90e  else  0e  then  over door-swing  loop  drop ;

: events-tick
    playing @ 0= if  exit  then
    cast  places
    event-state ev.room sl@ room-id <> if
        started @ 0= if  start-play  -1 started !  then
        room-id came-in-by @ enter-room  -1 came-in-by !  remember-places  true doors-follow  exit
    then
    run-frame  action-button  moves  remember-places  false doors-follow  take-exit ;
' events-tick on-tick

\ ---- movies ----
\ the movie, for the classes that show it (src/game/movie.c): 1 keyed by its brightness, 2 opaque
\ but only while state flag $29 (a scene's signal 11 turns it), 3 by its own alpha, 5 opaque - all
\ over the picture at 4:3; 6 a 256 x 64 strip at (64, 176) of 512 x 448
variable mx  variable my  variable mw2  variable mh2
: movie-area ( -- )   \ the 4:3 picture in the window
    screen-size  over 3 * 4 / over min  dup mh2 !  4 * 3 / mw2 !
    mh2 @ - 2/ my !  mw2 @ - 2/ mx ! ;
: scene-movie ( -- )
    playing @ 0= movie-status 1 <> or if  exit  then
    movie-kind @ case
        0 of  exit  endof
        2 of  $29 progress pr.state bit? 0= if  exit  then  endof
        4 of  exit  endof   \ (the game over screen's: drawn by it)
    endcase
    movie-area
    movie-kind @ 6 = if
        mx @ mw2 @ 64 * 512 / +  my @ mh2 @ 176 * 448 / +  mw2 @ 384 * 512 /  mh2 @ 96 * 448 /  movie-draw  exit
    then
    movie-kind @ dup 1 <> swap 3 <> and if  $000000FF pen-color  0 0 screen-size draw-rect  then   \ (the opaque ones)
    mx @ my @ mw2 @ mh2 @ movie-draw ;
' scene-movie on-draw

\ ---- the screen fade (black over the picture, the message window above it) ----
: fade-over-picture ( -- )
    fade-now @ dup 0> if
        255 * 1000 / 0 max 255 min  pen-color   \ (black, that much opaque)
        0 0 screen-size draw-rect
    else  drop  then ;
' fade-over-picture on-draw

\ ---- the message window (the original's Task: src/game/text.c) ----
\ The text laid out by game/messages.c; Enter turns the page; on the last page of a choice, the
\ arrows pick an option and Enter answers it (the window then shows the message the option leads
\ to, or closes). The answer is what `answer?` asks.
variable shown  -1 shown !        \ the message laid out
variable pages  variable pg  variable pick
: open? ( -- flag )  event-state ev.message sl@ 0< 0= ;
: lay-out ( -- )
    event-state ev.message sl@ dup shown !  message-layout pages !  0 pg !
    message-choice-flags 2 and if  message-options 1- 0 max  else  0  then  pick ! ;
: last-page? ( -- flag )  pg @ pages @ 1- >= ;
: choosing? ( -- flag )  last-page? message-options 0> and ;
: close-window ( -- )  -1 event-state ev.message l!  -1 shown ! ;
: answer ( -- )
    pick @ event-state ev.answer l!
    pick @ message-option >r drop 2drop r>  dup $FFFF = if  drop close-window exit  then
    event-state ev.message sl@ $C000 and or event-state ev.message l! ;
: window-keys ( -- )
    open? 0= if  -1 shown !  exit  then
    event-state ev.message sl@ shown @ <> if  lay-out  then
    choosing? if
        key: Up key-pressed?  key: Left key-pressed? or if  pick @ 1- 0 max pick !  then
        key: Down key-pressed?  key: Right key-pressed? or if  pick @ 1+ message-options 1- min pick !  then
    then
    key: Return key-pressed? if
        choosing? if  answer exit  then
        last-page? if  close-window  else  1 pg +!  then
    then ;

variable sw  variable sh  variable lh
: at-line ( n -- x y )  sw @ 10 / 24 +  swap lh @ *  sh @ 3 * 4 / 12 + + ;
: window ( -- )
    open? 0= shown @ 0< or if  exit  then
    screen-size sh ! sw !  2 pen-scale  char-size nip 5 * 4 / lh !
    $101018C0 pen-color                                    \ the box: the lower part of the screen
    sw @ 10 /  sh @ 3 * 4 /  sw @ 8 * 10 /  sh @ 5 /  draw-rect
    $FFFFFFFF pen-color
    pg @ message-lines 0 ?do  pg @ i message-line  i at-line draw-text  loop
    choosing? if                                           \ the cursor at the option picked
        s" >"  pick @ message-option drop  char-size drop * >r  nip at-line swap r> + swap draw-text
    then
    last-page? 0= if  s" (Enter)"  sw @ 8 * 10 /  sh @ 9 * 10 /  draw-text  then ;
' window-keys on-tick
' window on-draw

\ the prepared message's page (0x62 12 turns them during a scene: its subtitles), along the bottom
: subtitles ( -- )
    open? prepared @ 0< or prepared-page @ 0< or if  exit  then
    prepared @ message-layout prepared-page @ > 0= if  exit  then
    screen-size sh ! sw !  2 pen-scale  char-size nip 5 * 4 / lh !  $FFFFFFFF pen-color
    prepared-page @ message-lines 0 ?do
        prepared-page @ i message-line  sw @ 8 /  sh @ 7 * 8 / 8 +  i lh @ * +  draw-text
    loop ;
' subtitles on-draw
