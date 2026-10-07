\ events/play.fs - the event scripts in the game: while playing, each room's scripts run as in
\ the original (events/runner.fs), with Fiona and Hewie as the scripts' characters.
\ What isn't there yet: most of the words (each says so on the console the first time it runs;
\ `.missing` lists them), the message window (a line of text for now: Enter closes it), and
\ characters walking their scripted moves.
IN: events.play
USING: engine state player hewie game-state events.core events.runner ;

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
\ the room's event areas
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

variable started
: events-tick
    playing @ 0= if  exit  then
    cast  places
    event-state ev.room sl@ room-id <> if
        started @ 0= if  start-play  -1 started !  then
        room-id came-in-by @ enter-room  -1 came-in-by !  remember-places  exit
    then
    run-frame  remember-places
    event-state ev.message sl@ 0< 0= key: Return key-pressed? and if
        -1 event-state ev.message l!
    then ;
' events-tick on-tick

\ the message window, for now: which message it would show
: message-line
    event-state ev.message sl@ dup 0< if  drop exit  then
    $FFFFFFFF pen-color 2 pen-scale
    s" message " 40 40 draw-text  n>s 140 40 draw-text  s" (Enter)" 220 40 draw-text ;
' message-line on-draw
