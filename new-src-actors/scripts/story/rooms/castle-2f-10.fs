\ story/rooms/castle-2f-10.fs - the event scripts of room castle-2f-10 ($17; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-10
USING: room-names story.words story.shared flag-names ;

: castle-2f-10.enter ( -- )   \ 00414390
    room-sounds
    0 0 0 obstacle-place-saved
    1 1 0 obstacle-place-saved
    2 2 0 obstacle-place-saved
    3 3 0 obstacle-place-saved
    1 $2300 sound-volume
;

: castle-2f-10.char-enter ( -- )   \ 004143B0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
;

: castle-2f-10.phase1 ( -- )   \ 00414430
    fiona-who 1 char-in-area? fiona-who char-busy? not and if
        0 obstacle-save
        1 obstacle-save
        2 obstacle-save
        3 obstacle-save
        0 obstacle-keep-spot
        1 obstacle-keep-spot
        2 obstacle-keep-spot
        3 obstacle-keep-spot
        0 exit-check
    then
    fiona-who 2 char-in-area? fiona-who char-busy? not and if
        0 obstacle-save
        1 obstacle-save
        2 obstacle-save
        3 obstacle-save
        0 obstacle-keep-spot
        1 obstacle-keep-spot
        2 obstacle-keep-spot
        3 obstacle-keep-spot
        1 exit-check
    then
    5 0 0 1 chars-area-camera
    6 1 1 1 chars-area-camera
    0 0 var-set
    0 $455 obstacle-on? 0 $459 obstacle-on? or if
        0 obstacle-stop
        0 var-inc
    then
    1 $455 obstacle-on? 1 $459 obstacle-on? or if
        1 obstacle-stop
        0 var-inc
    then
    2 $473 obstacle-on? if
        2 obstacle-stop
        0 var-inc
    then
    3 $4B8 obstacle-on? if
        3 obstacle-stop
        0 var-inc
    then
    0 4 var? if
        0 fiona-who 0 action
    then
;

: castle-2f-10.phase2 ( -- )   \ 0047AC10
;

: castle-2f-10.act00 ( -- )   \ 004144C0
    stalkers-stay state-flag-set
    1 self-scripted
    begin
        fiona-who char-busy? not while
        yield
    repeat
    $1202 0 self-anim-blend
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    0.0 0.0 self-turn-to-xz
    self-wait-done
    1 self-anim
    self-frames-reset
    $3C self-wait-frames
    $F $44 fade
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    things-clear
    -1 self-idle-anim
    5 room-preload
    $28 $B4 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    $1C door-reopen-unlock
    $1D door-reopen-unlock
    $38 door-close-off-lock
    $39 door-close-off-lock
    exits-rebuild
    stalker-who 5 0 room-doors-state
    stalker-who 6 0 room-doors-state
    0 camera-mode? if
        $80 exit-check
    else
        $81 exit-check
    then
    self-idle-or-end
;

: castle-2f-10.act01 ( -- )   \ 004145A0
    self-wait-done
    0 0 0 $455 $CA5 obstacle-place
    1 1 0 $459 $CCD obstacle-place
    2 2 0 $473 $CE7 obstacle-place
    3 3 0 $4B8 $D2C obstacle-place
    self-who 1 char-visible
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $28 $B4 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    self-who 1 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-2f-10 register-scripts   \ (its phases and actions, by their names)
