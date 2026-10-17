\ story/rooms/old-mansion-2f-1.fs - the event scripts of room old-mansion-2f-1 ($45; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-1
USING: room-names story.words story.shared story-names ;

: old-mansion-2f-1.enter ( -- )   \ 00407E70
    1 1 $14 door-bits
    1 1 $20000 nav-group
    1 3 $300000 nav-group
;

: old-mansion-2f-1.char-enter ( -- )   \ 00407E90
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 2 player-camera
    then then
    0 2 2 area-camera
;

: old-mansion-2f-1.phase2 ( -- )   \ 00407ED0
    omb1-5-flag3 story-flag? if
        fiona-who $10 char-in-area? fiona-who 0 $32 char-heading? and if
            0 unhurried offer-action
        then
        fiona-who $11 char-in-area? fiona-who 90 $32 char-heading? and if
            1 unhurried offer-action
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
    0 message   \ "The floor has collapsed! But I literally just crossed through here seconds ago..."
    wait-message
    self-idle-or-end
;

: old-mansion-2f-1.act01 ( -- )   \ 00407F08
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 message   \ "The floor has collapsed! But I literally just crossed through here seconds ago..."
    wait-message
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-1 register-scripts   \ (its phases and actions, by their names)
