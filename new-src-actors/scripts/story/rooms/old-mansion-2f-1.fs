\ story/rooms/old-mansion-2f-1.fs - the event scripts of room old-mansion-2f-1 ($45; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-1
USING: room-names story.words story.shared ;

: old-mansion-2f-1.enter ( -- )   \ 00407E70
    1 1 $14 door-bits
    1 1 $20000 nav-group
    1 3 $300000 nav-group
;

: old-mansion-2f-1.char-enter ( -- )   \ 00407E90
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
;

: old-mansion-2f-1.phase2 ( -- )   \ 00407ED0
    $45 story-flag? if
        0 $10 char-in-area? 0 0 $32 char-heading? and if
            5 0 0 scene-change
        then
        0 $11 char-in-area? 0 90 $32 char-heading? and if
            5 1 0 scene-change
        then
    then
;

: old-mansion-2f-1.phase1 ( -- )   \ 0047AB50
    0 exit-usable? if
        0 exit-check
    then
;

: old-mansion-2f-1.phase5 ( -- )   \ 0047AB58
;

: old-mansion-2f-1.act00 ( -- )   \ 00407EF8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: old-mansion-2f-1.act01 ( -- )   \ 00407F08
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-1.enter old-mansion-2f-1 0 room-script!
' old-mansion-2f-1.char-enter old-mansion-2f-1 6 room-script!
' old-mansion-2f-1.phase2 old-mansion-2f-1 2 room-script!
' old-mansion-2f-1.phase1 old-mansion-2f-1 1 room-script!
' old-mansion-2f-1.phase5 old-mansion-2f-1 5 room-script!
' old-mansion-2f-1.act00 old-mansion-2f-1 $00 action-script!
' old-mansion-2f-1.act01 old-mansion-2f-1 $01 action-script!
