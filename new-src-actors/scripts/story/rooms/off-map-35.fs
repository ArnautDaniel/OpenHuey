\ story/rooms/off-map-35.fs - the event scripts of room off-map-35 ($35; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-35
USING: room-names story.words story.shared flag-names story-names ;

: off-map-35.char-enter ( -- )   \ 00443080
    fiona-who self-is? if
        $80 exit-taken? if
            0 fiona-who 0 action
        then
        $81 exit-taken? if
            found-file-20b story-flag? if
                0 fiona-who 1 action
            else
                0 fiona-who 2 action
            then
        then
    then
;

: off-map-35.act00 ( -- )   \ 004430A0
    3 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    stalkers-stay state-flag-set
    hewie-who char-activate
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    fiona-who 1 char-no-shadow
    hewie-who 1 char-no-shadow
    $1F $44 $4E $6C $64 $66 $6C $60 $46 0 9 effect-string
    $B090C8BA 0 1 screen-blend
    $40424C74 $4244489C 8.54 25.61 1 fog
    wait-fade
    panic-eased
    2 message-prepare   \ "Thanks, Hewie. Come on, boy!"
    1 result? if
        scene-play
    then
    $E room-preload
    fiona-who 0 char-no-shadow
    hewie-who 0 char-no-shadow
    $80 exit-check
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: off-map-35.act01 ( -- )   \ 00443170
    1 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    2 3 $FF char-load
    3 char-start
    hewie-who char-activate
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 cutscene-track 3 action
    panic-eased
    0 message-prepare   \ "Thanks, Hewie. Hewie!"
    1 result? if
        scene-play
    then
    $36 room-preload
    fiona-who 0 char-no-shadow
    $80 exit-check
    self-idle-or-end
;

: off-map-35.act02 ( -- )   \ 00443210
    2 resident-flag? not if
        no-pause state-flag-set
    then
    world-held state-flag-set
    hewie-who char-activate
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    panic-eased
    1 message-prepare   \ "Thanks, Hewie. Come on, boy. Hewie... Wait up, boy!"
    1 result? if
        scene-play
    then
    $2D 1 pvar-set
    2 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: off-map-35.act03 ( -- )   \ 004432A0
    begin
        $11 cutscene-shot? not while
        yield
    repeat
    fiona-who 1 char-no-shadow
    begin
        $12 cutscene-shot? not while
        yield
    repeat
    fiona-who 0 char-no-shadow
    self-idle-or-end
;

: off-map-35.act04 ( -- )   \ 004432C0
    self-wait-done
    2 3 $FF char-load
    3 char-start
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 cutscene-track 3 action
    panic-eased
    0 message-prepare   \ "Thanks, Hewie. Hewie!"
    1 result? if
        scene-play
    then
    $36 room-preload
    fiona-who 0 char-no-shadow
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: off-map-35.act05 ( -- )   \ 00443350
    self-wait-done
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    panic-eased
    1 message-prepare   \ "Thanks, Hewie. Come on, boy. Hewie... Wait up, boy!"
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: off-map-35.act06 ( -- )   \ 004433E0
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    fiona-who 1 char-no-shadow
    hewie-who 1 char-no-shadow
    $1F $44 $4E $6C $64 $66 $6C $60 $46 0 9 effect-string
    $B090C8BA 0 1 screen-blend
    $40424C74 $4244489C 8.54 25.61 1 fog
    wait-fade
    panic-eased
    2 message-prepare   \ "Thanks, Hewie. Come on, boy!"
    1 result? if
        scene-play
    then
    $E room-preload
    fiona-who 0 char-no-shadow
    hewie-who 0 char-no-shadow
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: off-map-35.enter ( -- )   \ 0047AF98
;

\ ---- registered ----
off-map-35 register-scripts   \ (its phases and actions, by their names)
