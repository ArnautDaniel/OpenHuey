\ story/rooms/castle-b1-3.fs - the event scripts of room castle-b1-3 ($16; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-3
USING: room-names story.words story.shared story-names ;

: castle-b1-3.enter ( -- )   \ 003FA780
    1 $2300 sound-volume
    got-metal-boots story-flag? not if
        0 -2.0 9.0 -2.0 flicker-sprite
    then
    fg-3-flag3 story-flag? if
        got-castle-gate-key story-flag? not if
            $28 1 item-count? not if
                got-castle-gate-key story-flag? not if
                    1 -6.0 9.0 3.0 flicker-sprite
                then
            then
        then
    then
;

: castle-b1-3.char-enter ( -- )   \ 003FA7C0
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
;

: castle-b1-3.phase1 ( -- )   \ 003FA800
    0 exit-usable? if
        0 exit-check
    then
    3 0 0 1 chars-area-camera
    4 1 -1 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
;

: castle-b1-3.phase2 ( -- )   \ 003FA820
    got-metal-boots story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 0 4 scene-change
        then
    then
    got-castle-gate-key story-flag? not if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
;

: castle-b1-3.phase3 ( -- )   \ 003FA850
    -6.7 20.0 24.7 -3.5 20.0 24.7 -6.7 0.0 24.7 -3.5 0.0 24.7 lights-doorway
    -3.5 20.0 24.8 -6.7 20.0 24.8 -3.5 0.0 24.8 -6.7 0.0 24.8 lights-doorway
    12.7 20.0 24.7 15.9 20.0 24.7 12.7 0.0 24.7 15.9 0.0 24.7 lights-doorway
    15.9 20.0 24.8 12.7 20.0 24.8 15.9 0.0 24.8 12.7 0.0 24.8 lights-doorway
;

: castle-b1-3.act00 ( -- )   \ 003FA920
    $81 1 item-count? not if
        self-wait-done
        -2.0 -2.0 self-turn-to-xz
        self-wait-done
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        0 effect-remove
        got-metal-boots story-flag-set
        $81 message-param-room
        $81 1 item-give-count
        0 $81 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        $903 self-anim
        self-wait-anim
        wait-message
    else
        self-wait-done
        -2.0 -2.0 self-turn-to-xz
        self-wait-done
        in-panic? not if
            $902 self-anim
            self-wait-anim
            self-frames-reset
            4 self-wait-frames
            $75 message-param-room
            $75 $63 item-count? if
                $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
                wait-message
            else
                got-metal-boots story-flag-set
                0 effect-remove
                $75 1 item-give-count
                0 $75 item-tab
                item-fanfare
                $8011 message   \ "You've obtained ."
                wait-message
            then
            $903 self-anim
            self-wait-anim
        then
    then
    self-idle-or-end
;

: castle-b1-3.act01 ( -- )   \ 003FA9C0
    self-wait-done
    -6.0 3.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    got-castle-gate-key story-flag-set
    1 effect-remove
    $28 message-param-room
    $28 1 item-give-count
    0 $28 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
' castle-b1-3.enter castle-b1-3 0 room-script!
' castle-b1-3.char-enter castle-b1-3 6 room-script!
' castle-b1-3.phase1 castle-b1-3 1 room-script!
' castle-b1-3.phase2 castle-b1-3 2 room-script!
' castle-b1-3.phase3 castle-b1-3 3 room-script!
' castle-b1-3.act00 castle-b1-3 $00 action-script!
' castle-b1-3.act01 castle-b1-3 $01 action-script!
