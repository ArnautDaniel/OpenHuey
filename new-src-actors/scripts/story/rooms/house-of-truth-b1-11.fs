\ story/rooms/house-of-truth-b1-11.fs - the event scripts of room house-of-truth-b1-11 ($99; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-11
USING: room-names story.words story.shared ;

\ Room99_Cmd00
: house-of-truth-b1-11.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-11.char-enter ( -- )   \ 00443560
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        1 1 player-camera
    then then
    3 1 1 area-camera
;

: house-of-truth-b1-11.phase1 ( -- )   \ 004435E0
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        3 exit-prepare
    then
    house-of-truth-b1-11.cmd00
    $A8 story-flag? $A9 story-flag? not and fiona-who $B char-entered-area? and if
        stalker-who char-here? not if
            $A9 story-flag-set
            stalker-who action-end
            1 summon-take
        else
            0 ebit-set
        then
    then
    0 ebit? stalker-who 0 char-action? and if
        0 stalker-who 0 action
    then
    $AA story-flag? $AB story-flag? not and fiona-who $D char-left-area? and if
        stalker-who char-here? not if
            $AB story-flag-set
            $FE $99 310 2 stalker-to-room
            stalker-who 0 stalker-command
            0 stalker-who 1 action
        else
            1 ebit-set
        then
    then
    1 ebit? stalker-who 0 char-action? and if
        0 stalker-who 2 action
    then
;

: house-of-truth-b1-11.phase5 ( -- )   \ 00443680
    0 ebit? $A9 story-flag? not and if
        $A9 story-flag-set
        stalker-who action-end
        1 summon-take
    then
    1 ebit? $AB story-flag? not and if
        $AB story-flag-set
        $FE $99 310 2 stalker-to-room
        stalker-who 0 stalker-command
    then
;

: house-of-truth-b1-11.act00 ( -- )   \ 004436B0
    1 self-scripted
    self-wait-done
    $1304 self-anim
    self-wait-anim
    $A9 story-flag-set
    0 ebit-clear
    stalker-who action-end
    1 summon-take
    self-idle-or-end
;

: house-of-truth-b1-11.act01 ( -- )   \ 004436D0
    1 self-scripted
    self-who 1 char-visible
    self-wait-done
    self-who 0 char-visible
    stalker-who $136 0 char-to-tri-facing
    stalker-who 1 1 char-camera
    $1305 0 self-anim-blend
    self-wait-anim
    stalker-who 0 stalker-command
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
    self-who 1 char-visible
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
