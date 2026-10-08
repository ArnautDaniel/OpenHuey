\ story/rooms/house-of-truth-b1-11.fs - the event scripts of room house-of-truth-b1-11 ($99; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-11
USING: room-names story.words story.shared ;

\ Room99_Cmd00
: house-of-truth-b1-11.cmd00 ( -- )  s" house-of-truth-b1-11.cmd00" stub-step ;

: house-of-truth-b1-11.char-enter ( -- )   \ 00443560
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    3 1 1 area-camera
;

: house-of-truth-b1-11.phase1 ( -- )   \ 004435E0
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    house-of-truth-b1-11.cmd00
    $A8 story-flag? $A9 story-flag? not and 0 $B char-entered-area? and if
        $FE char-here? not if
            $A9 story-flag-set
            $FE action-end
            1 summon-take
        else
            0 ebit-set
        then
    then
    0 ebit? $FE 0 char-action? and if
        0 $FE 0 action
    then
    $AA story-flag? $AB story-flag? not and 0 $D char-left-area? and if
        $FE char-here? not if
            $AB story-flag-set
            $FE $99 310 2 stalker-to-room
            $FE 0 stalker-mode
            0 $FE 1 action
        else
            1 ebit-set
        then
    then
    1 ebit? $FE 0 char-action? and if
        0 $FE 2 action
    then
;

: house-of-truth-b1-11.phase5 ( -- )   \ 00443680
    0 ebit? $A9 story-flag? not and if
        $A9 story-flag-set
        $FE action-end
        1 summon-take
    then
    1 ebit? $AB story-flag? not and if
        $AB story-flag-set
        $FE $99 310 2 stalker-to-room
        $FE 0 stalker-mode
    then
;

: house-of-truth-b1-11.act00 ( -- )   \ 004436B0
    1 self-scripted
    self-wait-done
    $1304 self-anim
    self-wait-anim
    $A9 story-flag-set
    0 ebit-clear
    $FE action-end
    1 summon-take
    self-idle-or-end
;

: house-of-truth-b1-11.act01 ( -- )   \ 004436D0
    1 self-scripted
    $FF 1 char-visible
    self-wait-done
    $FF 0 char-visible
    $FE $136 0 char-to-tri-facing
    $FE 1 1 char-camera
    $1305 0 self-anim-blend
    self-wait-anim
    $FE 0 stalker-mode
    1 ebit-clear
    $AB story-flag-set
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-11.act02 ( -- )   \ 004436F8
    1 self-scripted
    self-wait-done
    $1304 self-anim
    self-wait-anim
    $FF 1 char-visible
    ['] house-of-truth-b1-11.act01 goto
;

: house-of-truth-b1-11.enter ( -- )   \ 0047AFA0
    1 1 8 nav-group
;

: house-of-truth-b1-11.phase2 ( -- )   \ 0047AFA8
;

\ ---- registered ----
' house-of-truth-b1-11.char-enter house-of-truth-b1-11 6 room-script!
' house-of-truth-b1-11.phase1 house-of-truth-b1-11 1 room-script!
' house-of-truth-b1-11.phase5 house-of-truth-b1-11 5 room-script!
' house-of-truth-b1-11.act00 house-of-truth-b1-11 $00 action-script!
' house-of-truth-b1-11.act01 house-of-truth-b1-11 $01 action-script!
' house-of-truth-b1-11.act02 house-of-truth-b1-11 $02 action-script!
' house-of-truth-b1-11.enter house-of-truth-b1-11 0 room-script!
' house-of-truth-b1-11.phase2 house-of-truth-b1-11 2 room-script!
