\ story/rooms/off-map-34.fs - the event scripts of room off-map-34 ($34; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-34
USING: room-names story.words story.shared flag-names story-names floor-flags ;

: off-map-34.char-enter ( -- )   \ 00442950
    fiona-who self-is? if
        cf-13-flag1 story-flag? if
            $80 exit-taken? if
                fiona-who 0 0 char-to-tri-facing
                world-held state-flag-set
                panic-eased
                0 fiona-who 0 action-force
            then
        then
    then
;

: off-map-34.act00 ( -- )   \ 00442980
    4 resident-flag? not if
        no-pause state-flag-set
    then
    stalker-who action-end
    stalker-who char-done
    hewie-who action-end
    hewie-who char-done
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 1 action
    panic-eased
    0 message-prepare   \ "Ah, you're up. Fiona... I've decided you shall give birth to me. Don't worry, I..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    $17 action-end
    $17 char-done
    $20 4 $FF char-load
    $33 room-preload
    $80 exit-check
    self-idle-or-end
;

: off-map-34.act01 ( -- )   \ 00442A20
    0 part-shown
    1 part-shown
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-hidden
    begin
        $547 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-shown
    1 part-hidden
    2 part-hidden
    3 part-hidden
    4 part-shown
    5 part-hidden
    begin
        $569 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-shown
    1 part-shown
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-hidden
    begin
        $587 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-hidden
    1 part-shown
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-shown
    begin
        $592 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-hidden
    1 part-hidden
    2 part-hidden
    3 part-shown
    4 part-hidden
    5 part-shown
    begin
        $59E cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-shown
    1 part-hidden
    2 part-shown
    3 part-hidden
    4 part-hidden
    5 part-hidden
    begin
        $5AE cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-shown
    1 part-hidden
    2 part-hidden
    3 part-shown
    4 part-hidden
    5 part-hidden
    begin
        $5D4 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-hidden
    1 part-shown
    2 part-hidden
    3 part-shown
    4 part-hidden
    5 part-shown
    begin
        $5EE cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-shown
    1 part-hidden
    2 part-hidden
    3 part-hidden
    4 part-shown
    5 part-hidden
    begin
        $610 cutscene-cue-reached? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    0 part-shown
    1 part-shown
    2 part-hidden
    3 part-hidden
    4 part-hidden
    5 part-hidden
    begin
        cutscene-near-end? not while
        self-frames-reset
        1 self-wait-frames
    repeat
    self-idle-or-end
;

: off-map-34.act02 ( -- )   \ 00442B80
    self-wait-done
    $17 3 $FF char-load
    3 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 1 action
    panic-eased
    0 message-prepare   \ "Ah, you're up. Fiona... I've decided you shall give birth to me. Don't worry, I..."
    1 result? if
        scene-play
    then
    $20 4 $FF char-load
    $33 room-preload
    3 0 char-remove
    $80 exit-check
    $2C 1 pvar-set
    self-idle-or-end
;

: off-map-34.act4D ( -- )   \ 003F263F
    $F1 exit-check
    fade-finish
    fiona-who self-is? 0 exit-taken? and if
        fiona-who 0 char-to-exit
        4 -1 player-camera
    then
    \ (never runs in the original: an else outside any block)
    \   1 self-is? 0 exit-taken? and if
    \   1 0 char-to-exit
    \   hewie-controlled? not if
    \   0 4 -1 char-camera
    \   0 camera-follow
    \   else
    \   1 4 -1 char-camera
    \   1 camera-follow
    \   then
    \   then
    0 4 -1 area-camera
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
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        2 2 player-camera
    then then
    2 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        3 3 player-camera
    then then
    3 3 3 area-camera
    fiona-who self-is? if
        $80 exit-taken? $81 exit-taken? or $82 exit-taken? or if
            fiona-who $10E 70 char-to-tri-facing
            0 0 player-camera
            camera-restart
            $81 exit-taken? if
                0 fiona-who 6 action
            else
                0 fiona-who 0 action
            then
        then
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            5 ebit-set
            9 call-action   \ (this room has no script 9: the original reads past its table)
        else
            5 ebit-clear
        then
    then
;

: off-map-34.enter ( -- )   \ 0047AF58
;

: off-map-34.phase1 ( -- )   \ 0047AF5C
;

\ ---- registered ----
off-map-34 register-scripts   \ (its phases and actions, by their names)
