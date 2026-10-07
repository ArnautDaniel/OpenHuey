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

\ a character at the outside point of this room's exit (Rooms_ExitPointOut: the exit's first
\ triangle's centre)
: at-exit ( cs exit -- )
    swap character char.actor sl@ dup 0< if  2drop exit  then   ( exit actor )
    swap 0 exit-tri dup 0< if  2drop exit  then                ( actor tri )
    tri-center  actor dup act.z sf!  dup act.y sf!  act.x sf! ;
' at-exit is place-at-exit

: leaving  playing @ event-state ev.room sl@ 0< 0= and if  leave-room  then ;
' leaving is leaving-room

: events-tick
    playing @ 0= if  exit  then
    cast
    event-state ev.room sl@ room-id <> if
        room-id came-in-by @ enter-room  -1 came-in-by !  exit
    then
    run-frame
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
