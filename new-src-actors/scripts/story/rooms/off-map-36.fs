\ story/rooms/off-map-36.fs - the event scripts of room off-map-36 ($36; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-36
USING: room-names story.words story.shared flag-names ;

: off-map-36.char-enter ( -- )   \ 00443E60
    fiona-who self-is? if
        $80 exit-taken? if
            0 fiona-who 0 action
        then
    then
;

: off-map-36.enter ( -- )   \ 0047AFC0
;

: off-map-36.act00 ( -- )   \ 00443E70
    1 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    hewie-who action-end
    hewie-who char-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    $2D 0 pvar-set
    2 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: off-map-36.act01 ( -- )   \ 00443F10
    self-wait-done
    self-who 1 char-visible
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    $37 room-preload
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    self-who 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
' off-map-36.char-enter off-map-36 6 room-script!
' off-map-36.enter off-map-36 0 room-script!
' off-map-36.act00 off-map-36 $00 action-script!
' off-map-36.act01 off-map-36 $01 action-script!
