\ story/rooms/old-mansion-2f-8.fs - the event scripts of room old-mansion-2f-8 ($5C; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-8
USING: room-names story.words story.shared flag-names ;

\ room 0x5C (Room5C_Cmd00_ptmf): the dial (+0x7C, 0..1) from script variable 0 by byte 3: 0
\ 0x2B..0x38 (/ 13, +0x74 0 / +0x78 1), 1 0xC..0x20 (/ 20), 2 7..0x12 (/ 11) (+0x74 1 / +0x78 0)
: old-mansion-2f-8.cmd00 ( b0 -- )  drop s" old-mansion-2f-8.cmd00" stub-step ;
\ room 0x5C (Room5C_Cond00_ptmf): the first creature within 3 of (59.1, 1.43) is put away with
\ an effect (CreatureVanish_vtable) above it - blue (+0x1571 below 0x12) or red - and its action
\ 0x8B
: old-mansion-2f-8.cond00? ( -- flag )  s" old-mansion-2f-8.cond00?" stub-flag ;

: old-mansion-2f-8.enter ( -- )   \ 00410F70
    room-sounds
    1 0 var-set
    2 0 var-set
    $5A door-not-closed-off? not if
        2 1 $14 door-bits
        1 1 8 nav-group
    then
    $4B story-flag? if
        3 1 $14 door-bits
    then
    2 48.8 10.3 -50.7 0 effect-86
    1 $2300 sound-volume
;

: old-mansion-2f-8.act08 ( -- )   \ 00411890
    $FE camera-follow
    2 ebit-clear
    $16 0 pvar? if
        $A chance? if
            2 ebit-set
        then
    else $16 1 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $16 2 pvar? if
        $32 chance? if
            2 ebit-set
        then
    else $16 3 pvar? if
        $4B chance? if
            2 ebit-set
        then
    then then then then
    2 creature-action? if
        2 ebit-set
    then
    2 ebit? if
        0 $FE 9 action
    else
        $78 1 item-cooldown
    then
    $16 pvar-inc
    exit
;

: old-mansion-2f-8.char-enter ( -- )   \ 00410FB0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    0 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    1 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 -1 char-camera
                0 camera-follow
            else
                1 4 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 -1 char-camera
            0 camera-follow
        else
            1 4 -1 char-camera
            1 camera-follow
        then
    then then
    2 4 -1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    3 0 0 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 5 3 char-camera
                0 camera-follow
            else
                1 5 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 5 3 char-camera
            0 camera-follow
        else
            1 5 3 char-camera
            1 camera-follow
        then
    then then
    4 5 3 area-camera
    $265 story-flag? not if
        0 48.0 8.0 -53.0 flicker-sprite
    then
    $266 story-flag? not if
        1 53.1 12.0 -26.2 flicker-sprite
    then
    0 self-is? if
        $4A story-flag? not if
            0.0 sound-volume-scale
            0 0 0 action
        then
        $80 exit-taken? if
            0 $B 55.0 1.0 90 char-to-xz
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
            0 0 2 action
        then
        $FE exit-taken? if
            0 $D7 5.5 -116.0 0 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 5 action
        then
    then
    $FE self-is? if
        fiona-hidden state-flag? if
            3 ebit-set
            old-mansion-2f-8.act08
        else
            3 ebit-clear
        then
    then
;

: old-mansion-2f-8.phase1 ( -- )   \ 00411180
    6 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 5.0 18.0 -125.0 0 0 sound
            $54 story-flag? not if
                $40000009 6 -120.0 10.0 -105.0 0 0 sound
            then
            6 ebit-set
        then
    else
        $C0000008 6 5.0 18.0 -125.0 0 0 sound
        $54 story-flag? not if
            $C0000009 6 -120.0 10.0 -105.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $5A door-not-closed-off? if
        2 exit-usable? if
            2 exit-check
        then
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 0 0 1 chars-area-camera
    $11 3 -1 1 chars-area-camera
    $12 3 -1 1 chars-area-camera
    $13 4 -1 1 chars-area-camera
    $14 0 0 1 chars-area-camera
    $15 5 3 1 chars-area-camera
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        2 exit-prepare
    then
    0 8 char-entered-area? 0 9 char-entered-area? or 0 $A char-entered-area? or if
        3 exit-prepare
    then
    0 $B char-entered-area? if
        4 exit-prepare
    then
    0 ebit? not if
        $4B story-flag? not if
            3 camera-mode? 4 camera-mode? or if
                old-mansion-2f-8.cond00? if
                    0 ebit-set
                    $FF panic-stage? if
                        3 panic-stage
                    then
                    0 0 char-action? if
                        0 0 1 action-force
                    else
                        1 0 1 action-force
                    then
                then
            then
        then
    then
    0 141.32 0.0 -60.6 $14 21 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 21.0 hewie-look-zone
            then
        then
    then
    1 59.37 0.0 0.93 $14 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 5.52 0.0 -118.99 $F 18 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    2 5.52 0.0 -118.99 $F 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    5 2 8 -4 0 zone-at-effect
    0 5 3 char-zone-bits? 0 5 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 5 3 char-zone-bits? 1 5 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 5 3 char-zone-bits? $FE 5 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000003 6 45.32 0.0 23.5 0 0 sound
            else 1 1 var? if
                $40000004 6 45.32 0.0 23.5 0 0 sound
            else 1 2 var? if
                $40000005 6 45.32 0.0 23.5 0 0 sound
            else 1 3 var? if
                $40000006 6 45.32 0.0 23.5 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: old-mansion-2f-8.phase2 ( -- )   \ 00411430
    0 $16 $32 char-faces-area? if
        5 3 0 scene-change
    then
    0 $17 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $18 char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 6 5 scene-change
        else
            $8016 scene-ending
        then
    then
    0 $19 char-in-area? 0 -15 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $1A char-in-area? 0 45 $3C char-heading? and if
        5 $C 0 scene-change
    then
    0 $1B char-in-area? 0 -90 $3C char-heading? and if
        5 $D 0 scene-change
    then
    0 $1C char-in-area? 0 0 $3C char-heading? and if
        5 $E 0 scene-change
    then
    -2147483646 scene-request? 0 0 char-group-bit4? and if
        5 $F 1 scene-change
    then
    $265 story-flag? not if
        3 0 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $A 4 scene-change
        then
    then
    $266 story-flag? not if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 $B 4 scene-change
        then
    then
;

: old-mansion-2f-8.phase3 ( -- )   \ 004114E0
    40.0 20.0 -77.0 17.3 20.0 -77.0 40.0 0.0 -77.0 17.3 0.0 -77.0 lights-doorway
    17.3 20.0 -42.0 40.0 20.0 -42.0 17.3 0.0 -42.0 40.0 0.0 -42.0 lights-doorway
    17.0 20.0 -77.3 17.0 20.0 -100.0 17.0 0.0 -77.3 17.0 0.0 -100.0 lights-doorway
    22.3 20.0 -6.0 22.3 20.0 -30.0 22.3 0.0 -6.0 22.3 0.0 -30.0 lights-doorway
    20.7 20.0 -30.0 20.7 20.0 -6.0 20.7 0.0 -30.0 20.7 0.0 -6.0 lights-doorway
    22.3 20.0 30.0 22.3 20.0 6.0 22.3 0.0 30.0 22.3 0.0 6.0 lights-doorway
    20.7 20.0 6.0 20.7 20.0 25.0 20.7 0.0 6.0 20.7 0.0 25.0 lights-doorway
;

: old-mansion-2f-8.act00 ( -- )   \ 00411640
    1 self-scripted
    self-wait-done
    1 char-activate
    $5C 0 302 hewie-to-room
    $62 door-open-clear
    doors-room-in
    $62 door-lock
    creatures-on state-flag-clear
    1 creatures-clear
    $4A story-flag-set
    camera-restart
    $F $51 fade
    wait-fade
    $234 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act01 ( -- )   \ 00411670
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $5D room-preload
    self-frames-reset
    self-wait-16
    3 1 $14 door-bits
    self-frames-reset
    8 self-wait-frames
    $F $44 fade
    wait-fade
    world-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: old-mansion-2f-8.act02 ( -- )   \ 00411690
    1 self-scripted
    3 1 $14 door-bits
    self-wait-done
    camera-restart
    world-held state-flag-clear
    $4B story-flag-set
    $54 door-unlock
    2 exit-prepare
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    summoner-on state-flag-set
    self-idle-or-end
;

: old-mansion-2f-8.act03 ( -- )   \ 004116C0
    self-wait-done
    60.0 0.0 self-turn-to-xz
    self-wait-done
    $4B story-flag? not if
        0 message
        wait-message
    else
        1 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-2f-8.act04 ( -- )   \ 004116E0
    stalkers-stay state-flag-set
    1 self-scripted
    $27 room-preload
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    2 message
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        0 $7E 5 char-sound
        world-held state-flag-set
        $FE exit-check
    else
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-2f-8.act05 ( -- )   \ 00411720
    1 self-scripted
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    $F 1 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act07 ( -- )   \ 00411830
    1 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    0 0 var-set
    $8002 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        2 old-mansion-2f-8.cmd00
        yield
        0 18 var? not while
        0 6 var? if
            0 0 6 char-sound
        then
        0 var-inc
    repeat
    self-wait-anim
    0 self-noclip
    1 ebit? not if
        $FE action-end
        $FE char-here? if
            $FE 0 stalker-mode
        then
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act06 ( -- )   \ 00411740
    stalkers-stay state-flag-set
    3 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    $182 -67.8 -119.5 180 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    1 self-noclip
    0 0 var-set
    $8000 5 self-anim-9
    self-frames-reset
    5 self-wait-frames
    begin
        0 old-mansion-2f-8.cmd00
        yield
        0 56 var? not while
        0 44 var? if
            0 0 6 char-sound
        then
        0 var-inc
    repeat
    self-wait-anim
    -1 self-move-16
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    1 ebit-clear
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        0 1 6 char-sound
    then
    begin
        0 2 pad? not if
            $FF panic-stage? 1 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] old-mansion-2f-8.act07 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                3 ebit? $FE char-here? not and if
                    3 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] old-mansion-2f-8.act07 goto
            then
            0 camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            0 0 var-set
            $8001 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            begin
                1 old-mansion-2f-8.cmd00
                yield
                0 32 var? not while
                0 13 var? if
                    0 0 6 char-sound
                then
                0 var-inc
            repeat
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-2f-8.act09 ( -- )   \ 004118E0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $FE 2 char-heading-for? 2 exit-door-open? not and if
        2 3 self-move-slot
        self-wait-done
    then
    $FE 3 char-heading-for? 3 exit-door-open? not and if
        3 3 self-move-slot
        self-wait-done
    then
    $FE 4 char-heading-for? 4 exit-door-open? not and if
        4 3 self-move-slot
        self-wait-done
    then
    $182 -67.8 -110.0 180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    1 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: old-mansion-2f-8.act0A ( -- )   \ 00411950
    self-wait-done
    48.0 -53.0 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $265 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-8.act0B ( -- )   \ 004119B0
    self-wait-done
    53.1 -26.2 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message
            wait-message
        else
            $266 story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8011 message
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-8.act0C ( -- )   \ 00411A10
    self-wait-done
    5 ebit? not if
        5 ebit-set
        138.0 -58.0 self-turn-to-xz
        self-wait-done
        $A01 self-anim
        self-frames-reset
        $28 self-wait-frames
        3 message
        wait-message
    else
        $64 129.0 -60.5 90 $FFFF 5 self-move-to
        self-wait-done
        1 15.0 -10.0 0.0 7.0 event-camera
        $A00 self-anim
        self-frames-reset
        $28 self-wait-frames
        4 message
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-8.act0D ( -- )   \ 00411A60
    self-wait-done
    78.0 -70.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    5 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-8.act0E ( -- )   \ 00411A80
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    62.0 27.0 self-turn-to-xz
    self-wait-done
    8 message
    wait-message
    $F 6 fade
    wait-fade
    9 message
    wait-message
    $F 7 fade
    wait-fade
    $43 subscreen-bit? not $45 subscreen-bit? not or if
        $1D01 self-anim
        $A message
        wait-message
        self-wait-anim
        $43 subscreen-bit
        $45 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act0F ( -- )   \ 00411AD0
    self-wait-done
    0 self-through-exit
    self-wait-done
    $FF panic-stage? 2 game-mode? or if
        $60A self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    else
        $608 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        7 message
        wait-message
    then
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-2f-8.enter old-mansion-2f-8 0 room-script!
' old-mansion-2f-8.char-enter old-mansion-2f-8 6 room-script!
' old-mansion-2f-8.phase1 old-mansion-2f-8 1 room-script!
' old-mansion-2f-8.phase2 old-mansion-2f-8 2 room-script!
' old-mansion-2f-8.phase3 old-mansion-2f-8 3 room-script!
' old-mansion-2f-8.act00 old-mansion-2f-8 $00 action-script!
' old-mansion-2f-8.act01 old-mansion-2f-8 $01 action-script!
' old-mansion-2f-8.act02 old-mansion-2f-8 $02 action-script!
' old-mansion-2f-8.act03 old-mansion-2f-8 $03 action-script!
' old-mansion-2f-8.act04 old-mansion-2f-8 $04 action-script!
' old-mansion-2f-8.act05 old-mansion-2f-8 $05 action-script!
' old-mansion-2f-8.act06 old-mansion-2f-8 $06 action-script!
' old-mansion-2f-8.act07 old-mansion-2f-8 $07 action-script!
' old-mansion-2f-8.act08 old-mansion-2f-8 $08 action-script!
' old-mansion-2f-8.act09 old-mansion-2f-8 $09 action-script!
' old-mansion-2f-8.act0A old-mansion-2f-8 $0A action-script!
' old-mansion-2f-8.act0B old-mansion-2f-8 $0B action-script!
' old-mansion-2f-8.act0C old-mansion-2f-8 $0C action-script!
' old-mansion-2f-8.act0D old-mansion-2f-8 $0D action-script!
' old-mansion-2f-8.act0E old-mansion-2f-8 $0E action-script!
' old-mansion-2f-8.act0F old-mansion-2f-8 $0F action-script!
