\ story/rooms/house-of-truth-b1-10.fs - the event scripts of room house-of-truth-b1-10 ($98; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-10
USING: room-names story.words story.shared story-names floor-flags ;

\ Room98_Cmd00
: house-of-truth-b1-10.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-10.char-enter ( -- )   \ 00442F60
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

: house-of-truth-b1-10.phase1 ( -- )   \ 00442FE0
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
    house-of-truth-b1-10.cmd00
    htb1-10-flag1 story-flag? not if
        fiona-who $A char-entered-area? if
            htb1-10-flag1 story-flag-set
            $FE $98 380 2 stalker-to-room
            stalker-who 0 stalker-command
            0 stalker-who 0 action
        then
    then
;

: house-of-truth-b1-10.enter ( -- )   \ 0047AF88
    1 solid-floor floor-on
;

: house-of-truth-b1-10.phase2 ( -- )   \ 0047AF90
;

: house-of-truth-b1-10.act00 ( -- )   \ 00443040
    1 self-scripted
    self-who 1 char-visible
    self-wait-done
    self-who 0 char-visible
    stalker-who $168 65 char-to-tri-facing
    stalker-who 1 1 char-camera
    $1305 0 self-anim-blend
    self-wait-anim
    0 self-scripted
    stalker-who 0 stalker-command
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-b1-10 register-scripts   \ (its phases and actions, by their names)
