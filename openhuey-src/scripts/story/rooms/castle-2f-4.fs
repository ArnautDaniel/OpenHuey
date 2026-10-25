\ story/rooms/castle-2f-4.fs - the event scripts of room castle-2f-4 ($31; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-4
USING: room-names facts story.words story.shared flag-names story-names floor-flags ;

\ Room31_Cmd00
: castle-2f-4.cmd00 ( -- )  1 0 tv-in ;   \ (room effect 1 the TV showing the screen)
\ Room31_Cmd01
: castle-2f-4.cmd01 ( -- )  fan-turn ;
\ Room31_Cmd02
: castle-2f-4.cmd02 ( b0 -- )  $1B $40  0.491e 22.0e 69.502e  9.502e 22.0e 60.491e  0.491e 0.0e 69.502e  9.502e 0.0e 60.491e  lit-quad ;   \ (a reflecting window: b0 1 removed, 2 over the whole picture)

: castle-2f-4.enter ( -- )   \ 00429E40
    castle-2f-4.cmd00
    0 part-shown
    1 0 1 effect-string
    1 part-hidden
    0 1 track 2 action
;

: castle-2f-4.char-enter ( -- )   \ 00429E60
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            2 2 player-camera
            fiona-who $A8 45 char-to-tri-facing
            world-held state-flag-set
            0 fiona-who 0 action
        then
    then
    hewie-who self-is? if
    then
    stalker-who self-is? if
    then
;

: castle-2f-4.phase1 ( -- )   \ 00429F10
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
;

: castle-2f-4.act00 ( -- )   \ 00429F30
    c2f-3-tv-examined story-flag-clear
    self-wait-done
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 cutscene-track 1 action
    $21 room-preload
    movie-opaque state-flag-set
    panic-eased
    9 message-prepare   \ "No---------!! No...This isn't real... It's just a dream... This can't be happen..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    $80 exit-check
    fiona-who $A char-layer
    self-idle-or-end
;

: castle-2f-4.act01 ( -- )   \ 00429FC0
    [: 4 cutscene-shot? ;] wait-until
    fiona-who $1E char-layer
    [: 5 cutscene-shot? ;] wait-until
    fiona-who $A char-layer
    fiona-who 1 char-no-shadow
    [: 6 cutscene-shot? ;] wait-until
    fiona-who 0 char-no-shadow
    [: 9 cutscene-shot? ;] wait-until
    2 castle-2f-4.cmd02
    fiona-who $1E char-layer
    [: $A cutscene-shot? ;] wait-until
    1 castle-2f-4.cmd02
    fiona-who $A char-layer
    [: 0 cutscene-cue-reached? ;] wait-while
    wait-fade
    self-idle-or-end
;

: castle-2f-4.act02 ( -- )   \ 0047AD60
    begin
        castle-2f-4.cmd01
        yield
    again
;

: castle-2f-4.act03 ( -- )   \ 0042A010
    self-wait-done
    0 1 track 2 action
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 1 action
    movie-opaque state-flag-set
    panic-eased
    9 message-prepare   \ "No---------!! No...This isn't real... It's just a dream... This can't be happen..."
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-2f-4 register-scripts   \ (its phases and actions, by their names)
