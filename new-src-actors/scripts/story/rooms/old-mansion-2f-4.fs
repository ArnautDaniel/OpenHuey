\ story/rooms/old-mansion-2f-4.fs - the event scripts of room old-mansion-2f-4 ($57; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-4
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

: old-mansion-2f-4.enter ( -- )   \ 0040F490
    room-sounds
    0 both-halves floor-on
    om2f-2-flag1 story-flag? if
        0 part-shown
        2 1 object-hidden
    then
    om2f-4-viride-medallion story-flag? not if
        0 5.64 9.5 5.58 flicker-sprite
    then
;

: old-mansion-2f-4.char-enter ( -- )   \ 0040F4C0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 -1 player-camera
    then then
    0 2 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
;

: old-mansion-2f-4.phase1 ( -- )   \ 0040F540
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    5 0 0 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        0 exit-prepare
        3 0 char-remove
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
        $1C 3 1 char-load
    then
    0 -44.47 0.0 54.87 $1E 16 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-2f-4.phase2 ( -- )   \ 0040F5B0
    fiona-who $D char-in-area? fiona-who -15 $32 char-heading? and if
        2 unhurried offer-action
    then
    fiona-who $F char-in-area? fiona-who 20 $3C char-heading? and if
        8 unhurried offer-action
    then
    fiona-who $10 char-in-area? fiona-who 45 $3C char-heading? and if
        9 unhurried offer-action
    then
    om2f-4-viride-medallion story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            7 calm offer-action
        then
    then
    fiona-who $11 char-in-area? fiona-who 90 $32 char-heading? and if
        1 unhurried offer-action
    then
;

: old-mansion-2f-4.act00 ( -- )   \ 0040F600
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    look-close-anim self-anim
    self-wait-anim
    6 message   \ "Oh my! It actually transformed..."
    wait-message
    reach-out-anim self-anim
    self-wait-anim
    9 item-use
    $A message-param-room
    $A 1 item-give-count
    0 $A item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    reach-back-anim self-anim
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act01 ( -- )   \ 0040F670
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    9 message   \ "There is something written here... I... think I can just about make it out..."
    wait-message
    $F 6 fade
    wait-fade
    $A message   \ "Torva Temporarily gives anxiety to those who use it, making them feel panicked. Weltig..."
    wait-message
    $F 7 fade
    wait-fade
    $48 subscreen-bit? not $49 subscreen-bit? not or if
        hand-to-mouth-anim self-anim
        $B message   \ "Names for chemicals or medicine or something... Looks like their effects are liste..."
        wait-message
        self-wait-anim
        $48 subscreen-bit
        $49 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act02 ( -- )   \ 0040F6B0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        3 message   \ "Purifier The impure yellow mass of filth will set free the shining glory of the sac..."
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        hand-to-mouth-anim $A self-anim-blend
        4 message   \ "A purifier? I've never heard of a device like this before. There is a space to inse..."
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $243 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act03 ( -- )   \ 0040F700
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A4 -36.0 41.0 -35 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    look-close-anim self-anim
    self-wait-anim
    reach-out-anim self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 10 var? if
        $A message-param-room
        0 $A item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 11 var? if
        $B message-param-room
        0 $B item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
    then then then then then
    reach-back-anim self-anim
    self-wait-anim
    5 message   \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-4.act04 ( -- )   \ 0047ABC4
    self-idle-or-end
;

: old-mansion-2f-4.act05 ( -- )   \ 0047ABC8
    self-idle-or-end
;

: old-mansion-2f-4.act06 ( -- )   \ 0047ABCC
    self-idle-or-end
;

: old-mansion-2f-4.act07 ( -- )   \ 0040F820
    self-wait-done
    5.64 5.58 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-4-viride-medallion story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-4.act08 ( -- )   \ 0040F880
    self-wait-done
    0.6 5.2 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    7 message   \ "The desk is covered in strange research equipment. What kind of research could possibly..."
    wait-message
    self-idle-or-end
;

: old-mansion-2f-4.act09 ( -- )   \ 0040F8A0
    self-wait-done
    46.8 0.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    8 message   \ "There seems to be a passage to another one here. A warm wind is blowing in from that di..."
    wait-message
    self-idle-or-end
;

: old-mansion-2f-4.phase5 ( -- )   \ 0047ABC0
;

\ ---- registered ----
old-mansion-2f-4 register-scripts   \ (its phases and actions, by their names)
