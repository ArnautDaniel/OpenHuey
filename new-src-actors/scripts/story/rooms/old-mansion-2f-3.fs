\ story/rooms/old-mansion-2f-3.fs - the event scripts of room old-mansion-2f-3 ($56; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-3
USING: room-names story.words story.shared flag-names ;

\ room 0x56 (Room56_Cmd00_ptmf): creatures 7..9 in the current room on a live triangle
\ gRoomEventObj says yes to: +0x10, then the creature list's +0x28
: old-mansion-2f-3.cmd00 ( -- )  s" old-mansion-2f-3.cmd00" stub-step ;

: old-mansion-2f-3.enter ( -- )   \ 0040ED10
    room-sounds
    $322 story-flag? not if
        0 1 $14 door-bits
        1 0 $1000000 nav-group
    else
        1 0 $10020000 nav-group
    then
    1 char-here? 1 1 char-in-nav-group? and if
        1 $A8 char-to-tri
    then
    $84 story-flag? not if
        1 char-here? 119 hewie-action? and if
            1 $96 -78.0 0.0 90 char-to-xz
            18000 2 hewie-anim
            1 1 $B0 nav-group
        else
            1 1 $B8 nav-group
        then
    else
        1 1 $14 door-bits
        1 1 $88 nav-group
    then
;

: old-mansion-2f-3.char-enter ( -- )   \ 0040ED80
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
;

: old-mansion-2f-3.phase1 ( -- )   \ 0040EDC0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    $322 story-flag? not if
        0 35.0 0.0 0.0 8 4 0 zone
        0 0 2 char-zone-bits? 0 0 2 char-zone-bits-before? not and if
            0 0 6 char-sound
        then
    then
    $82 story-flag? $322 story-flag? not and if
        0 35.0 0.0 0.0 8 4 0 zone
        $FE 0 2 char-zone-bits? $FE 0 2 char-zone-bits-before? not and if
            $322 story-flag-set
            0 $FE 2 action
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 4 action-force
            else
                1 0 4 action-force
            then
        then
    then
    $84 story-flag? not if
        1 char-here? if
            118 hewie-action? not if
                35 fiona-started? 1 2 char-C4? not and $FE char-here? not and 1 camera-mode? and 0 char-unseen? not and 1 char-unseen? not and 0 -50 0 $32 char-faces-xz? and if
                    hewie-stays? if
                        0 1 5 action
                    then
                then
            else 44 fiona-started? if
                0 1 6 action
            then then
        then
    then
    1 -42.06 0.0 -0.81 $1A 5 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $32 chance? if
                        $1F 1 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    2 32.38 0.0 -0.63 $16 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 0.0 hewie-look-zone
            then
        then
    then
    3 31.11 0.0 48.2 $1E 15 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-2f-3.phase2 ( -- )   \ 0040EF30
    0 4 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 5 char-in-area? 0 -45 $32 char-heading? and if
        5 7 0 scene-change
    then
    0 6 char-in-area? 0 90 $32 char-heading? and if
        5 8 0 scene-change
    then
    $322 story-flag? if
        0 3 $32 char-faces-area? if
            5 3 0 scene-change
        then
    else
        0 35.0 0.0 0.0 8 4 0 zone
        0 0 2 char-zone-bits? if
            5 $D 0 scene-change
        then
    then
    0 7 $3C char-faces-area? if
        5 $B 0 scene-change
    then
    0 8 char-in-area? 0 5 37 $32 char-faces-xz? and if
        5 $C 0 scene-change
    then
;

: old-mansion-2f-3.phase3 ( -- )   \ 0040EFB0
    45.0 -4.0 -20.0 65.0 -4.0 -20.0 45.0 -4.0 20.0 65.0 -4.0 20.0 lights-doorway
;

: old-mansion-2f-3.act00 ( -- )   \ 0040EFF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A0 29.5 34.5 0 $FFFF 5 self-move-to
    self-wait-done
    2 ebit? not if
        $F 6 fade
        wait-fade
        3 message
        wait-message
        $F 7 fade
        wait-fade
        2 ebit-set
    else
        $1D01 $A self-anim-blend
        4 message
        wait-message
        self-wait-anim
        2 ebit-clear
    then
    $241 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act01 ( -- )   \ 0040F040
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    $41 room-preload
    wait-fade
    things-clear
    $FFFF message-close
    0 1 6 char-sound
    world-held state-flag-set
    self-frames-reset
    $3C self-wait-frames
    $80 exit-check
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act02 ( -- )   \ 0047ABB8
    self-wait-done
    begin
        yield
    again
;

: old-mansion-2f-3.act03 ( -- )   \ 0040F070
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    2 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-3.act04 ( -- )   \ 0040F080
    stalkers-stay state-flag-set
    1 self-scripted
    $FE 0 6 char-sound
    $F $44 fade
    self-wait-done
    wait-fade
    things-clear
    $FFFF message-close
    old-mansion-2f-3.cmd00
    $FE 1 6 char-sound
    $FE action-end
    3 summon-take
    self-frames-reset
    $3C self-wait-frames
    0 0 char-in-nav-group? if
        0 $68 160 char-to-tri-facing
    then
    0 0 $14 door-bits
    1 0 $10020000 nav-group
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    camera-restart
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act05 ( -- )   \ 0040F0E0
    self-wait-done
    hewie-bark
    self-wait-done
    $A8 -30.0 0.0 -90 $FFFF $A self-move-to
    self-wait-done
    1 self-scripted
    0 1 8 nav-group
    $96 -78.0 0.0 -90 $204 5 self-move-to
    self-wait-done
    18000 2 hewie-anim
    self-idle-or-end
;

: old-mansion-2f-3.act06 ( -- )   \ 0040F120
    1 self-scripted
    self-wait-done
    $A8 -30.0 0.0 90 $204 5 self-move-to
    self-wait-done
    -1 self-move-16
    self-wait-anim
    0 0 hewie-anim
    0 0 hewie-action
    1 1 8 nav-group
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act07 ( -- )   \ 0040F150
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -50.0 0.0 self-turn-to-xz
    self-wait-done
    $84 story-flag? not if
        0 ebit? not if
            6 message
            wait-message
            0 ebit-set
        else
            7 message
            wait-message
        then
    else
        $D message
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act08 ( -- )   \ 0040F190
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $A message
    wait-message
    $F 6 fade
    wait-fade
    $B message
    wait-message
    $F 7 fade
    wait-fade
    $C message
    wait-message
    $240 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act09 ( -- )   \ 0040F1C0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A0 29.5 34.5 0 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    1 message
    wait-message
    $902 self-anim
    self-wait-anim
    $10 item-use
    8 message-param-room
    8 1 item-give-count
    0 8 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act0A ( -- )   \ 0040F230
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A0 29.5 34.5 0 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    0 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 11 var? if
        $B message-param-room
        0 $B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 0 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then then
    $903 self-anim
    self-wait-anim
    0 message
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act0B ( -- )   \ 0040F370
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -3.17 12.09 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        3 ebit-set
        $E message
        wait-message
    else
        $F message
        wait-message
    then
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act0C ( -- )   \ 0040F3A0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    5.3 37.5 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $2C5 story-flag? not if
        $10 message
        wait-message
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C5 story-flag-set
            $71 1 item-give-count
            0 $71 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
    else
        $11 message
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-3.act0D ( -- )   \ 0040F410
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    $12 message
    wait-message
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-3.enter old-mansion-2f-3 0 room-script!
' old-mansion-2f-3.char-enter old-mansion-2f-3 6 room-script!
' old-mansion-2f-3.phase1 old-mansion-2f-3 1 room-script!
' old-mansion-2f-3.phase2 old-mansion-2f-3 2 room-script!
' old-mansion-2f-3.phase3 old-mansion-2f-3 3 room-script!
' old-mansion-2f-3.act00 old-mansion-2f-3 $00 action-script!
' old-mansion-2f-3.act01 old-mansion-2f-3 $01 action-script!
' old-mansion-2f-3.act02 old-mansion-2f-3 $02 action-script!
' old-mansion-2f-3.act03 old-mansion-2f-3 $03 action-script!
' old-mansion-2f-3.act04 old-mansion-2f-3 $04 action-script!
' old-mansion-2f-3.act05 old-mansion-2f-3 $05 action-script!
' old-mansion-2f-3.act06 old-mansion-2f-3 $06 action-script!
' old-mansion-2f-3.act07 old-mansion-2f-3 $07 action-script!
' old-mansion-2f-3.act08 old-mansion-2f-3 $08 action-script!
' old-mansion-2f-3.act09 old-mansion-2f-3 $09 action-script!
' old-mansion-2f-3.act0A old-mansion-2f-3 $0A action-script!
' old-mansion-2f-3.act0B old-mansion-2f-3 $0B action-script!
' old-mansion-2f-3.act0C old-mansion-2f-3 $0C action-script!
' old-mansion-2f-3.act0D old-mansion-2f-3 $0D action-script!
