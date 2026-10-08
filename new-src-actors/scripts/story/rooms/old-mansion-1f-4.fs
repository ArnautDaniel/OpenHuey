\ story/rooms/old-mansion-1f-4.fs - the event scripts of room old-mansion-1f-4 ($42; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-4
USING: room-names story.words story.shared flag-names ;

: old-mansion-1f-4.enter ( -- )   \ 004070F0
    room-sounds
    $12 1.0 0 bgm
    $77 story-flag? not if
        0 1 $14 door-bits
    else
        1 1 $14 door-bits
    then
    $1C 3 char-to-tri
    2 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
    then
    0 $F1 4 action
    hunted state-flag-set
    $278 story-flag? not if
        0 -51.07 1.0 19.59 flicker-sprite
    then
    1 $2300 sound-volume
    1 $77 $10000000 nav-tri-flags
    1 $48 $10000000 nav-tri-flags
    1 $75 $10000000 nav-tri-flags
    1 $46 $10000000 nav-tri-flags
    1 $52 $10000000 nav-tri-flags
    1 $81 $10000000 nav-tri-flags
    1 2 $10000000 nav-tri-flags
    1 $61 $10000000 nav-tri-flags
    1 $70 $10000000 nav-tri-flags
    1 $3E $10000000 nav-tri-flags
    1 $39 $10000000 nav-tri-flags
    1 0 $10000000 nav-tri-flags
    1 $71 $10000000 nav-tri-flags
    1 $3F $10000000 nav-tri-flags
    1 $4B $10000000 nav-tri-flags
    1 $7A $10000000 nav-tri-flags
    1 $4A $10000000 nav-tri-flags
    1 $79 $10000000 nav-tri-flags
    1 $45 $10000000 nav-tri-flags
    1 $74 $10000000 nav-tri-flags
    1 $5F $10000000 nav-tri-flags
    1 $85 $10000000 nav-tri-flags
    1 $51 $10000000 nav-tri-flags
    1 $80 $10000000 nav-tri-flags
    1 $4C $10000000 nav-tri-flags
    1 $7B $10000000 nav-tri-flags
    1 $3C $10000000 nav-tri-flags
    1 $6F $10000000 nav-tri-flags
    1 $4E $10000000 nav-tri-flags
    1 $7D $10000000 nav-tri-flags
    1 $86 $10000000 nav-tri-flags
    1 $60 $10000000 nav-tri-flags
    1 $54 $10000000 nav-tri-flags
    1 $83 $10000000 nav-tri-flags
    1 6 $10000000 nav-tri-flags
    1 $63 $10000000 nav-tri-flags
    1 $14 $10000000 nav-tri-flags
    1 $6D $10000000 nav-tri-flags
    1 $4D $10000000 nav-tri-flags
    1 $7C $10000000 nav-tri-flags
    1 $18 $10000000 nav-tri-flags
    1 $23 $10000000 nav-tri-flags
    1 $41 $10000000 nav-tri-flags
    1 $72 $10000000 nav-tri-flags
    1 $76 $10000000 nav-tri-flags
    1 $47 $10000000 nav-tri-flags
    1 $78 $10000000 nav-tri-flags
    1 $49 $10000000 nav-tri-flags
    1 $53 $10000000 nav-tri-flags
    1 $82 $10000000 nav-tri-flags
    1 $5A $10000000 nav-tri-flags
    1 $84 $10000000 nav-tri-flags
    1 $3B $10000000 nav-tri-flags
;

: old-mansion-1f-4.char-enter ( -- )   \ 00407300
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

: old-mansion-1f-4.phase1 ( -- )   \ 00407340
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 0 0 1 chars-area-camera
    4 2 2 1 chars-area-camera
    0 55.0 0.0 -20.0 5 10 1 zone
    $FF 0 char-in-zone? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 21 var? if
        $1C 2 6 char-sound
    then
    2 66 var? if
        $1C 2 6 char-sound
    then
    1 45.1 0.0 31.88 $1C 18 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    2 54.4 0.0 -20.46 $19 16 0 zone
    2 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    2 ebit-set
                    $50 chance? if
                        $1F 2 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    2 54.4 0.0 -20.46 $19 16 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 16.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-4.phase2 ( -- )   \ 00407430
    0 5 char-in-area? 0 20 $32 char-heading? and if
        5 2 0 scene-change
    then
    $278 story-flag? not if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 5 4 scene-change
        then
    then
    0 7 char-in-area? 0 90 $3C char-heading? and if
        5 6 0 scene-change
    then
    0 8 char-in-area? 0 90 $3C char-heading? and if
        5 7 0 scene-change
    then
    0 9 char-in-area? 0 90 $3C char-heading? and if
        5 8 0 scene-change
    then
    0 $A char-in-area? 0 0 $3C char-heading? and if
        5 9 0 scene-change
    then
    0 $B char-in-area? 0 0 $3C char-heading? and if
        5 $A 0 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        5 9 0 scene-change
    then
    0 $D char-in-area? 0 90 $3C char-heading? and if
        5 $A 0 scene-change
    then
    0 $E char-in-area? 0 0 $3C char-heading? and if
        5 $B 0 scene-change
    then
    0 $F char-in-area? 0 0 $3C char-heading? and if
        5 $C 0 scene-change
    then
    0 $10 char-in-area? 0 0 $3C char-heading? and if
        5 $D 0 scene-change
    then
;

: old-mansion-1f-4.act0E ( -- )   \ 00407A20
    4 6 -30.0 10.0 20.0 0 0 sound
    self-frames-reset
    self-wait-16
    0 $43 5 char-sound
    $3C threat-raise
    1 $FF 8 rumble
    $F04 self-anim
    self-wait-anim
    0 0 $14 door-bits
    1 1 $14 door-bits
    $42 0 105 $80 5 -1 $68 40.0 creature-place
    $42 0 23 $80 10 -1 $5A 0.0 creature-place
    $77 story-flag-set
    exit
;

: old-mansion-1f-4.act00 ( -- )   \ 004074F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    5 message
    wait-message
    $902 self-anim
    self-wait-anim
    $B item-use
    $C message-param-room
    $C 1 item-give-count
    0 $C item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $360 story-flag-set
    $903 self-anim
    self-wait-anim
    $77 story-flag? not if
        old-mansion-1f-4.act0E
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act01 ( -- )   \ 00407560
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    0 3 6 char-sound
    self-frames-reset
    $78 self-wait-frames
    $A02 self-anim
    self-wait-anim
    $902 self-anim
    self-wait-anim
    1 8 var? if
        8 message-param-room
        0 8 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 9 var? if
        9 message-param-room
        0 9 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 10 var? if
        $A message-param-room
        0 $A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 12 var? if
        $C message-param-room
        0 $C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    else 1 16 var? if
        $10 message-param-room
        0 $10 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
    then then then then then
    $903 self-anim
    self-wait-anim
    2 message
    wait-message
    $77 story-flag? not if
        old-mansion-1f-4.act0E
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act02 ( -- )   \ 00407680
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    0 ebit? not if
        $F 6 fade
        wait-fade
        0 message
        wait-message
        0 ebit-set
        $F 7 fade
        wait-fade
    else
        $1D01 $A self-anim-blend
        1 message
        wait-message
        self-wait-anim
        0 ebit-clear
    then
    $245 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act03 ( -- )   \ 004076D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    $F 6 fade
    wait-fade
    4 message
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act04 ( -- )   \ 00407700
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        1 ebit? if
            2 0 var-set
            0 var-inc
            0 3 var? if
                $21 chance? if
                    0 5 var-set
                then
            then
            0 4 var? if
                $32 chance? if
                    0 5 var-set
                then
            then
            0 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                $FE 0 stalker-mode
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            0 $8F 5 char-sound
            $1C $9001 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9000 0 3 char-anim-hold
            2 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9000 0 0 char-anim-hold
            2 0 var-set
        else
            2 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: old-mansion-1f-4.act05 ( -- )   \ 004077C0
    self-wait-done
    -51.07 19.59 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 message
            wait-message
        else
            $278 story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act06 ( -- )   \ 00407820
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    0 $62 6.25 -23.4 173 char-to-xz
    $FF 3 -1 char-camera
    self-frames-reset
    4 self-wait-frames
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act07 ( -- )   \ 00407880
    self-wait-done
    -20.0 -40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act08 ( -- )   \ 004078B0
    self-wait-done
    -48.0 -40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act09 ( -- )   \ 004078E0
    self-wait-done
    -10.0 0.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act0A ( -- )   \ 00407910
    self-wait-done
    -40.0 0.0 self-turn-to-xz
    self-wait-done
    $77 story-flag? not if
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 ebit? not if
            6 message
            wait-message
            3 ebit-set
        else
            7 message
            wait-message
        then
        self-wait-anim
    else
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        8 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act0B ( -- )   \ 00407950
    self-wait-done
    8.0 40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act0C ( -- )   \ 00407980
    $77 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        0 $50 -17.5 26.5 -12 char-to-xz
        $FF 4 -1 char-camera
        self-frames-reset
        4 self-wait-frames
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 ebit? not if
            6 message
            wait-message
            3 ebit-set
        else
            7 message
            wait-message
        then
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        $A02 self-anim
        self-frames-reset
        $28 self-wait-frames
        8 message
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act0D ( -- )   \ 004079F0
    self-wait-done
    -48.0 40.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    3 ebit? not if
        6 message
        wait-message
        3 ebit-set
    else
        7 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-4.enter old-mansion-1f-4 0 room-script!
' old-mansion-1f-4.char-enter old-mansion-1f-4 6 room-script!
' old-mansion-1f-4.phase1 old-mansion-1f-4 1 room-script!
' old-mansion-1f-4.phase2 old-mansion-1f-4 2 room-script!
' old-mansion-1f-4.act00 old-mansion-1f-4 $00 action-script!
' old-mansion-1f-4.act01 old-mansion-1f-4 $01 action-script!
' old-mansion-1f-4.act02 old-mansion-1f-4 $02 action-script!
' old-mansion-1f-4.act03 old-mansion-1f-4 $03 action-script!
' old-mansion-1f-4.act04 old-mansion-1f-4 $04 action-script!
' old-mansion-1f-4.act05 old-mansion-1f-4 $05 action-script!
' old-mansion-1f-4.act06 old-mansion-1f-4 $06 action-script!
' old-mansion-1f-4.act07 old-mansion-1f-4 $07 action-script!
' old-mansion-1f-4.act08 old-mansion-1f-4 $08 action-script!
' old-mansion-1f-4.act09 old-mansion-1f-4 $09 action-script!
' old-mansion-1f-4.act0A old-mansion-1f-4 $0A action-script!
' old-mansion-1f-4.act0B old-mansion-1f-4 $0B action-script!
' old-mansion-1f-4.act0C old-mansion-1f-4 $0C action-script!
' old-mansion-1f-4.act0D old-mansion-1f-4 $0D action-script!
' old-mansion-1f-4.act0E old-mansion-1f-4 $0E action-script!
