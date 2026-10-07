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
' bank-sound is event-sound   ' sound-at is event-sound-at   ' sound-set! is event-sound-set
' stop-sound is event-sound-stop   ' sound-scale! is event-sound-scale
:noname  0 $1B0C00 sound-stop-voices  6 sound-load ; is event-room-sounds
:noname  dup sound-load sound-loaded? ; is event-sound-loaded?
:noname  camera cam.x sf@ camera cam.y sf@ camera cam.z sf@ ; is event-camera-eye
' sound-reverb! is event-reverb   ' room-door-at is event-door-at
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
        i character char.present sl@  i own-moves? 0= and if  i character char.move-done sl@ 0= if
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
\ when that changes; and the passage they block on the nav mesh (a shut door's doorway; locked
\ to Hewie or the stalkers from a side: Doors_Refresh)
: door-locks ( exit -- flags )
    event-exit-door dup 0< if  drop 0 exit  then
    dup door-locked if  drop $5000000 exit  then
    door-word dup 0= if  drop 0 exit  then  l@ 4 rshift
    0 over 2 and if  $1000000 or  then  swap 4 and if  $4000000 or  then ;
: doors-follow ( at-once -- )   \ (not while a scene keys them)
    8 0 do  i dup exit-open over door-locks door-passage  loop
    cutscene-active? if  drop exit  then
    8 0 do  i dup exit-open if  -90e  else  0e  then  over door-swing  loop  drop ;

: events-tick
    playing @ 0= paused @ or if  exit  then
    cast  places
    event-state ev.room sl@ room-id <> if
        started @ 0= if  start-play  -1 started !  then
        room-id came-in-by @ enter-room  -1 came-in-by !  remember-places  true doors-follow  exit
    then
    run-frame  action-button  moves  remember-places  false doors-follow  take-exit ;
' events-tick on-tick

\ ---- the background music (src/game/music.c BgmCtl): the scripts want a track (0xFF none) at a
\ level; a new one starts at once (paused if asked), none fades the playing one out over 30
\ frames, wanting it again fades it back in. The stream's loudness: the original sets it as
\ 100 log10 v tenths of a dB, so the square root of v as an amplitude ----
variable bgm-cur  $FF bgm-cur !   variable bgm-req  $FF bgm-req !   variable bgm-pause
fvariable bgm-level  1e bgm-level f!  fvariable bgm-fade  fvariable bgm-speed
:noname ( track pause -- ) ( F: level -- )
    bgm-pause !  dup bgm-req !  $FF <> if  bgm-level f!  else  fdrop  then ; is event-bgm-want
' music-pause is event-music-pause
: bgm-start ( -- )
    bgm-req @ dup bgm-cur !  bgm-track bgm-pause @ music-play drop ;
: bgm-tick ( -- )
    false                                                   ( start? )
    bgm-cur @ bgm-req @ <> if
        bgm-req @ $FF = if  -1e 30e f/ bgm-speed f!  else  drop true  1e bgm-fade f!  then
    else
        bgm-req @ $FF <> bgm-speed f@ f0< and if  1e 30e f/ bgm-speed f!  then
    then
    bgm-cur @ $FF <> if
        bgm-fade f@ bgm-speed f@ f+ 0e fmax 1e fmin bgm-fade f!
        bgm-fade f@ 1e f>= bgm-fade f@ 0e f<= or if  0e bgm-speed f!  then
        bgm-fade f@ 0e f<= if  music-stop  $FF bgm-cur !  then
    then
    bgm-fade f@ bgm-level f@ f* 0e fmax 1e fmin fsqrt music-volume!
    if  bgm-start  then ;
' bgm-tick on-tick

\ ---- the stage music (src/game/music.c MusicDir): made by the scripts for a stage set (0x6B),
\ its bank and four sequences (0 panic, 1 / 2 the calm music's parts, 3 the chase) loaded; held
\ and released (0x6A 2 / 4); then started: the sequences from their start, the stage's
\ sequence volumes and channel volumes / pans / bends, the calm parts full and the others
\ silent, the global volume fading in over 90 frames. Not yet: the chase and the panic (they
\ follow the stalkers) ----
variable sm-stage  -1 sm-stage !  variable sm-step  variable sm-hold
fvariable sm-global  fvariable sm-to  fvariable sm-rate   255e sm-to f!
create sm-track 4 cells allot   \ each track's volume (0..255)
: sm-apply ( -- )   \ the tracks' volumes through the global one (vol_out)
    4 0 do  i  i cells sm-track + @ s>f sm-global f@ f* 255e f/ f>s  seq-port-volume  loop ;
: sm-file ( i -- addr len )  sm-stage @ swap stage-file ;
: sm-load ( stage -- )
    seq-reset  dup sm-stage !  0 sm-step !  0 sm-hold !  0e sm-global f!  255e sm-to f!
    0< if  exit  then
    0 sm-file 3 - seq-bank drop                      \ (BGM\STAGEn_BANK: .HD and .BD)
    4 0 do  i  i 1+ sm-file seq-load drop  0 i cells sm-track + !  loop ;
' sm-load is event-music-stage
: sm-volume-to ( v frames -- )   \ (MusicDir_GlobalVolumeTo)
    swap s>f sm-to f!  dup 0> if  s>f  sm-to f@ sm-global f@ f- fswap f/ sm-rate f!  else  drop  sm-to f@ sm-global f! 0e sm-rate f!  then ;
:noname ( op a b -- )
    sm-stage @ 0< if  2drop drop exit  then
    rot case
        0 of  sm-step @ 4 < if  drop s>f sm-to f!  else  sm-volume-to  then  endof
        2 of  2drop -1 sm-hold !  endof
        4 of  2drop 0 sm-hold !  endof
        5 of  2drop  4 0 do  0 i cells sm-track + !  loop  sm-apply  endof
        >r 2drop r>
    endcase ; is event-music
: sm-start ( -- )   \ (MusicDir_Start)
    4 0 do
        i  sm-stage @ 3 stage-table i + c@ 20 max seq-volume
        16 0 do
            j i  sm-stage @ 0 stage-table j 16 * + i + c@  seq-chan-volume
            j  $B0 i or  10  sm-stage @ 2 stage-table j 16 * + i + c@  seq-midi
            j  $E0 i or  0  sm-stage @ 1 stage-table j 16 * + i + c@  seq-midi
        loop
    loop
    0 sm-track !  255 1 cells sm-track + !  255 2 cells sm-track + !  0 3 cells sm-track + !
    sm-to f@ f>s 90 sm-volume-to ;
: sm-tick ( -- )   \ (MusicDir_Update)
    sm-stage @ 0< paused @ or if  exit  then
    sm-step @ case
        0 of  1 sm-step !  endof
        1 of  sm-hold @ 0= if  4 0 do  i -1 seq-play  loop  2 sm-step !  then  endof
        2 of  sm-start  sm-apply  4 sm-step !  endof
        4 of
            sm-rate f@ f0= 0= if
                sm-global f@ sm-rate f@ f+ sm-global f!
                sm-rate f@ f0< if  sm-global f@ sm-to f@ f<  else  sm-global f@ sm-to f@ f>  then
                if  sm-to f@ sm-global f!  0e sm-rate f!  then
            then
            sm-apply
        endof
    endcase ;
' sm-tick on-tick

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

\ ---- the inventory (I): what she carries - the items with their names (system message $8100 +
\ id) and counts, and how many files; the game stands still while it is up ----
variable inv-open
: inventory-keys ( -- )
    playing @ 0= title @ or if  exit  then
    key: I key-pressed? if
        inv-open @ 0= dup inv-open !  dup paused !  pause!
    then ;
' inventory-keys on-tick
: item-name ( id -- addr len )   \ (its first line)
    $8100 + message-layout 0> if  0 message-lines 0> if  0 0 message-line exit  then  then  s" ?" ;
variable iy
: inventory ( -- )
    inv-open @ 0= if  exit  then
    screen-size sh ! sw !  2 pen-scale  char-size nip 5 * 4 / lh !
    $101018E0 pen-color  sw @ 8 /  sh @ 8 /  sw @ 3 * 4 /  sh @ 3 * 4 /  draw-rect
    $D8D0C8FF pen-color  s" ITEMS"  sw @ 8 / 16 +  sh @ 8 / 12 +  draw-text
    sh @ 8 / 12 + lh @ 2* + iy !
    $100 0 do
        i items-of ?dup if
            iy @ sh @ 7 * 8 / < if
                $FFFFFFFF pen-color  i item-name  sw @ 8 / 24 +  iy @  draw-text
                s" x" sw @ 5 * 8 / iy @ draw-text  n>s  sw @ 5 * 8 / char-size drop + iy @ draw-text
                lh @ iy +!
            then
        then
    loop
    0  128 0 do  progress pr.files i 2* + w@ 0<> -  loop
    $B0A890FF pen-color  s" files: " sw @ 8 / 24 +  sh @ 7 * 8 / lh @ -  draw-text
    n>s  sw @ 8 / 24 + 7 char-size drop * +  sh @ 7 * 8 / lh @ -  draw-text
    s" (I: back)"  sw @ 5 * 8 /  sh @ 7 * 8 / lh @ -  draw-text ;
' inventory on-draw

\ the prepared message's page (0x62 12 turns them during a scene: its subtitles), along the bottom
: subtitles ( -- )
    open? prepared @ 0< or prepared-page @ 0< or if  exit  then
    prepared @ message-layout prepared-page @ > 0= if  exit  then
    screen-size sh ! sw !  2 pen-scale  char-size nip 5 * 4 / lh !  $FFFFFFFF pen-color
    prepared-page @ message-lines 0 ?do
        prepared-page @ i message-line  sw @ 8 /  sh @ 7 * 8 / 8 +  i lh @ * +  draw-text
    loop ;
' subtitles on-draw
