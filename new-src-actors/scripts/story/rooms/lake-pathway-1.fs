\ story/rooms/lake-pathway-1.fs - the event scripts of room lake-pathway-1 ($C8; Lake Pathway).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.lake-pathway-1
USING: room-names story.words story.shared ;

\ an effect TurningModel_vtable (8 bytes), not started
: lake-pathway-1.cmd00 ( -- )  s" lake-pathway-1.cmd00" stub-step ;

: lake-pathway-1.enter ( -- )   \ 00441520
    $18 1.0 0 bgm
    $8E story-flag? not $94 story-flag? or if
        1 0 $10020000 nav-group
    else
        0 1 $14 door-bits
    then
    $2E0 story-flag? not if
        0 19.59 39.0 -247.61 flicker-sprite
    then
    lake-pathway-1.cmd00
;

: lake-pathway-1.char-enter ( -- )   \ 00441560
    $8E story-flag? not $94 story-flag? or if
        hewie-controlled? not if
            0 self-is? 0 exit-taken? and if
                0 0 char-to-exit
                hewie-controlled? not if
                    0 1 1 char-camera
                    0 camera-follow
                else
                    1 1 1 char-camera
                    1 camera-follow
                then
            then
        else 1 self-is? 0 exit-taken? and if
            1 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then then
        0 1 1 area-camera
    else
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
    then
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

: lake-pathway-1.phase1 ( -- )   \ 00441620
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    $8E story-flag? not if
        1 -4.8 38.0 -171.89 $35 15 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 1 9 char-zone-bits? if
                        2 ebit-set
                        $1E chance? if
                            $1F 1 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
;

: lake-pathway-1.phase2 ( -- )   \ 004416A0
    $8E story-flag? if
        0 4 char-in-area? 0 0 $32 char-heading? and if
            5 0 0 scene-change
        then
        0 5 char-in-area? 0 90 $32 char-heading? and if
            5 0 0 scene-change
        then
    else 0 6 char-in-area? 0 0 $32 char-heading? and if
        5 1 0 scene-change
    then then
    $2E0 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then
;

: lake-pathway-1.act00 ( -- )   \ 004416F0
    self-wait-done
    0 ebit? not if
        0 4 char-in-area? if
            0 message
            $A01 self-anim
            self-wait-anim
        else
            180 self-turn-angle
            self-wait-done
            1 message
            $A00 self-anim
            self-wait-anim
        then
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        wait-message
        0 ebit-set
    else
        $900 self-anim
        self-wait-anim
        2 message
        wait-message
        $901 self-anim
        self-wait-anim
        0 ebit-clear
    then
    self-idle-or-end
;

: lake-pathway-1.act01 ( -- )   \ 00441730
    self-wait-done
    1 ebit? not if
        1 ebit-set
        3 message
        wait-message
    else
        1 ebit-clear
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        4 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: lake-pathway-1.act02 ( -- )   \ 00441750
    self-wait-done
    19.59 -247.61 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $2E0 story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
' lake-pathway-1.enter lake-pathway-1 0 room-script!
' lake-pathway-1.char-enter lake-pathway-1 6 room-script!
' lake-pathway-1.phase1 lake-pathway-1 1 room-script!
' lake-pathway-1.phase2 lake-pathway-1 2 room-script!
' lake-pathway-1.act00 lake-pathway-1 $00 action-script!
' lake-pathway-1.act01 lake-pathway-1 $01 action-script!
' lake-pathway-1.act02 lake-pathway-1 $02 action-script!
