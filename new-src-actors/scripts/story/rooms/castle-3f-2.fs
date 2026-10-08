\ story/rooms/castle-3f-2.fs - the event scripts of room castle-3f-2 ($2C; Belli Castle: 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-3f-2
USING: room-names story.words story.shared ;

: castle-3f-2.char-enter ( -- )   \ 0041D140
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    1 0 0 area-camera
;

: castle-3f-2.phase1 ( -- )   \ 0041D1C0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    0 2 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
            $FE 0 char-file-load
        then
        0 exit-prepare
    then
    0 3 char-entered-area? if
        $5B story-flag? $5C story-flag? not and if
        then
        1 exit-prepare
    then
    $5B story-flag? $5C story-flag? not and if
        0 4 char-left-area? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 0 action-force
        then
    then
    $5C story-flag? $71 story-flag? not and if
        0 stalker-alert? not if
            $FE 0 stalker-mode
        then
    then
;

: castle-3f-2.enter ( -- )   \ 0047ACB4
;

: castle-3f-2.act00 ( -- )   \ 0041D220
    $5C story-flag-set
    $18 state-flag-set
    1 self-scripted
    0 exit-door-open? not if
        0 1 self-door-knock
        self-frames-reset
        $10 self-wait-frames
    then
    0 0 self-door-knock
    self-wait-done
    0 counter-set
    $FE char-activate
    $FE $2C 46 2 stalker-to-room
    stalker-item-cooldown
    $FE $2E 0.0 192.0 -180 char-to-xz
    0 $FE 1 action
    $F04 self-anim
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    1 wait-counter
    0 1 char-visible
    $FE 0 0 char-camera
    $FE camera-follow
    1 20.0 10.0 180.0 0.0 event-camera
    0 exit-door-open? if
        $34 door-open-clear
    then
    doors-room-in
    $34 door-lock
    self-frames-reset
    $3C self-wait-frames
    1 char-here? not 1 0 char-in-area? or if
        $2C 0 43 hewie-to-room
        1 $2B 9.584 178.014 0 char-to-xz
    then
    0 0.0 0.0 0.0 0.0 event-camera
    0 0 char-visible
    0 camera-follow
    counter-inc
    0 self-scripted
    $18 state-flag-clear
    self-idle-or-end
;

: castle-3f-2.act01 ( -- )   \ 0041D2D0
    1 self-scripted
    self-wait-done
    0 self-anim
    counter-inc
    2 wait-counter
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
' castle-3f-2.char-enter castle-3f-2 6 room-script!
' castle-3f-2.phase1 castle-3f-2 1 room-script!
' castle-3f-2.enter castle-3f-2 0 room-script!
' castle-3f-2.act00 castle-3f-2 $00 action-script!
' castle-3f-2.act01 castle-3f-2 $01 action-script!
