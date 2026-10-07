\ events/play.fs - the event scripts in the game: while playing, each room's scripts run as in
\ the original (events/runner.fs), with Fiona and Hewie as the scripts' characters.
\ What isn't there yet: most of the words (each says so on the console the first time it runs;
\ `.missing` lists them) and characters walking their scripted moves.
IN: events.play
USING: engine state player hewie doors game-state events.core events.words events.runner ;

\ the scripts' characters: slot 0 Fiona (script id 0), slot 1 Hewie (script id 1)
: cast ( -- )
    0 0 character char.id l!  1 1 character char.id l!
    fiona @ dup 0 character char.actor l!  0< 0= 0 character char.present l!
    hewie @ dup 1 character char.actor l!  0< 0= 1 character char.present l! ;

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
' bank-sound is event-sound   ' bank-sound-at is event-sound-at   ' sound-set! is event-sound-set
' area-in? is event-area-in?   ' area-cross is event-area-cross   ' exit-area is event-exit-area

\ the characters' places from their actors (now, and the frame before)
: places ( -- )
    characters 0 do
        i actor-of dup 0< if  drop  else
            actor dup act.x sf@  dup act.y sf@  act.z sf@  i character char.pos f3!
        then
    loop ;
: remember-places ( -- )
    characters 0 do  i character dup char.pos f3@  char.prev f3!  loop ;

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

variable started
: events-tick
    playing @ 0= if  exit  then
    cast  places
    event-state ev.room sl@ room-id <> if
        started @ 0= if  start-play  -1 started !  then
        room-id came-in-by @ enter-room  -1 came-in-by !  remember-places  exit
    then
    run-frame  remember-places  take-exit ;
' events-tick on-tick

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
