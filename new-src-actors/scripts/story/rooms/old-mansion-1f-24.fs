\ story/rooms/old-mansion-1f-24.fs - the event scripts of room old-mansion-1f-24 ($70; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-24
USING: room-names story.words story.shared flag-names story-names ;

defer old-mansion-1f-24.act02
: old-mansion-1f-24.enter ( -- )   \ 0043B020
    room-sounds
    om1f-24-breakable-broken story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        om1f-24-beef-jerky story-flag? not if
            0 -97.5 1.0 1.0 flicker-sprite
        then
    then
    0 8 0.812 0.687 0.187 0.312 zone-rect
    white-mandragora-used story-flag? not if
        3 stalker-kind? $22 stalker-kind? or if
            stalker-active? not if
                4 ebit-set
            then
        then
    then
    0 1 8 nav-group
    4 ebit? if
        1 exit-taken? not if
            stalker-who 2 char-file-load
        then
        force-followed state-flag-set
        stalker-no-fear state-flag-set
        no-stalker-camera state-flag-set
        stalker-who char-activate
        stalker-who 1 char-silent
        $FE $70 49 2 stalker-to-room
        0 stalker-who 4 action
    then
    om1f-24-rubrum-medallion story-flag? not if
        1 -98.51 1.0 -12.98 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-1f-24.char-enter ( -- )   \ 0043B0D0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        2 2 player-camera
    then then
    1 2 2 area-camera
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
            0 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 0 player-camera
    then then
    3 0 0 area-camera
;

: old-mansion-1f-24.phase1 ( -- )   \ 0043B1D0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
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
    8 0 0 1 chars-area-camera
    9 1 1 1 chars-area-camera
    fiona-who 4 char-entered-area? fiona-who 6 char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        3 exit-prepare
    then
    white-mandragora-used story-flag? not if
        1 -76.01 0.0 -50.35 $46 17 0 zone
        5 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 1 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
        1 -76.01 0.0 -50.35 $46 17 0 zone
        hewie-who char-here? 1 game-mode? and if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    $1F 1 var-set
                    $1F 17.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? if
        stalker-who $80000000 6 char-sound
        stalker-who $80000001 6 char-sound
    then
;

: old-mansion-1f-24.phase2 ( -- )   \ 0043B2C0
    4 ebit? if
        fiona-who stalker-who $A 0 $2D char-touching-facing? if
            6 ebit? not if
                5 5 0 scene-change
            then
        then
    then
    fiona-who $A char-in-area? fiona-who -90 $1E char-heading? and if
        5 9 0 scene-change
    then
    fiona-who $B char-in-area? fiona-who -67 $3C char-heading? and if
        5 9 0 scene-change
    then
    om1f-24-rubrum-medallion story-flag? not if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            5 8 4 scene-change
        then
    then
    om1f-24-breakable-broken story-flag? not if
        0 -97.5 0.0 1.0 5 8 1 zone
        self-who 0 char-in-zone? if
            om1f-24-breakable-broken story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -97.5 0.0 1.0 0 -2145378272 0 0.0 shards-at
            $88 5 -97.5 0.0 1.0 0 0 sound
            $40 $B4 noise
            0 -97.5 1.0 1.0 flicker-sprite
        then
    else om1f-24-beef-jerky story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
;

: old-mansion-1f-24.phase5 ( -- )   \ 0043B390
    4 ebit? if
        stalker-no-fear state-flag-clear
        no-stalker-camera state-flag-clear
        1 stalker-who 0 char-model-op
        stalker-who 0 char-silent
        stalker-who action-end
        stalker-who char-done
    then
    force-followed state-flag-clear
;

: old-mansion-1f-24.act00 ( -- )   \ 0047AEC8
    self-idle-or-end
;

: old-mansion-1f-24.act01 ( -- )   \ 0047AECC
    self-idle-or-end
;

: old-mansion-1f-24.act07 ( -- )   \ 0043B4E0
    self-wait-anim
    $8005 7 self-anim-blend
    self-wait-anim
    $8001 $14 self-anim-blend
    self-wait-anim
    6 ebit-clear
    ['] old-mansion-1f-24.act02 goto
;

: old-mansion-1f-24.act06 ( -- )   \ 0043B4C0
    self-wait-anim
    $8004 $14 self-anim-blend
    self-wait-anim
    $8003 7 self-anim-blend
    self-wait-anim
    6 ebit-clear
    begin
        6 ebit? if
            ['] old-mansion-1f-24.act07 goto
        then
        $8003 self-anim
        self-wait-anim
    again
;

:noname   \ old-mansion-1f-24.act02 (0043B3B0; deferred: used before it is defined)
    begin
        6 ebit? if
            ['] old-mansion-1f-24.act06 goto
        then
        $8001 self-anim
        self-wait-anim
    again
; is old-mansion-1f-24.act02

: old-mansion-1f-24.act03 ( -- )   \ 0043B3C0
    self-wait-done
    -97.5 1.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-24-beef-jerky story-flag-set
            0 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-24.act04 ( -- )   \ 0043B420
    1 self-noclip
    1 self-scripted
    self-wait-done
    stalker-who $31 -76.0 -56.5 180 char-to-xz
    1 1 8 nav-group
    6 ebit-set
    stalker-who char-file-use
    6 ebit-clear
    0 stalker-who 0 char-model-op
    stalker-who 0 char-silent
    ['] old-mansion-1f-24.act02 goto
;

: old-mansion-1f-24.act05 ( -- )   \ 0043B450
    1 self-scripted
    hewie-no-attack state-flag-set
    self-wait-done
    6 ebit-set
    begin
        6 ebit? while
        yield
    repeat
    $1D 1 item-count? not if
        stalker-who 0 6 char-sound
        0 message   \ "Oh my. What a filthy little princess. A small present from the lord of the manor..."
        self-frames-reset
        $1E self-wait-frames
        wait-message
        $1D message-param-room
        $1D 1 item-give-count
        0 $1D item-tab
        item-fanfare
        $8012 message   \ "You've obtained ."
        wait-message
        got-hebels-perfume story-flag-set
    else
        stalker-who 1 6 char-sound
        1 message   \ "Miss... It's cleaning time now."
        self-frames-reset
        $5A self-wait-frames
        wait-message
    then
    6 ebit-set
    begin
        6 ebit? while
        yield
    repeat
    0 self-scripted
    hewie-no-attack state-flag-clear
    self-idle-or-end
;

: old-mansion-1f-24.act08 ( -- )   \ 0043B500
    self-wait-done
    -98.51 -12.98 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-24-rubrum-medallion story-flag-set
            1 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-24.act09 ( -- )   \ 0047AED0
    self-wait-done
    2 message   \ "Judging from the size and shape, there is probably a statue under there... But I probab..."
    wait-message
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-24.enter old-mansion-1f-24 0 room-script!
' old-mansion-1f-24.char-enter old-mansion-1f-24 6 room-script!
' old-mansion-1f-24.phase1 old-mansion-1f-24 1 room-script!
' old-mansion-1f-24.phase2 old-mansion-1f-24 2 room-script!
' old-mansion-1f-24.phase5 old-mansion-1f-24 5 room-script!
' old-mansion-1f-24.act00 old-mansion-1f-24 $00 action-script!
' old-mansion-1f-24.act01 old-mansion-1f-24 $01 action-script!
' old-mansion-1f-24.act02 old-mansion-1f-24 $02 action-script!
' old-mansion-1f-24.act03 old-mansion-1f-24 $03 action-script!
' old-mansion-1f-24.act04 old-mansion-1f-24 $04 action-script!
' old-mansion-1f-24.act05 old-mansion-1f-24 $05 action-script!
' old-mansion-1f-24.act06 old-mansion-1f-24 $06 action-script!
' old-mansion-1f-24.act07 old-mansion-1f-24 $07 action-script!
' old-mansion-1f-24.act08 old-mansion-1f-24 $08 action-script!
' old-mansion-1f-24.act09 old-mansion-1f-24 $09 action-script!
