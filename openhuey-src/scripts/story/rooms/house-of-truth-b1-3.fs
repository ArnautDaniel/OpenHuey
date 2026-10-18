\ story/rooms/house-of-truth-b1-3.fs - the event scripts of room house-of-truth-b1-3 ($86; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-3
USING: room-names story.words story.shared flag-names story-names ;

\ room 0x86: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-3.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-3.enter ( -- )   \ 0042C770
    htb1-3-flag1 story-flag? not if
        summoner-on state-flag-clear
        htb1-3-flag1 story-flag-set
        stalker-who action-end
        1 summon-take
    then
    1 $2300 sound-volume
;

: house-of-truth-b1-3.char-enter ( -- )   \ 0042C790
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
;

: house-of-truth-b1-3.phase1 ( -- )   \ 0042C810
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    house-of-truth-b1-3.cmd00
;

: house-of-truth-b1-3.act00 ( -- )   \ 0047ADB0
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-b1-3 register-scripts   \ (its phases and actions, by their names)
