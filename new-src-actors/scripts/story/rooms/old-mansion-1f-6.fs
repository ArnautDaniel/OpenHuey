\ story/rooms/old-mansion-1f-6.fs - the event scripts of room old-mansion-1f-6 ($46; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-6
USING: room-names story.words story.shared flag-names ;

\ Room46_Cmd00
: old-mansion-1f-6.cmd00 ( b0 -- )  drop s" old-mansion-1f-6.cmd00" stub-step ;
\ Room46_Cond00
: old-mansion-1f-6.cond00? ( -- flag )  s" old-mansion-1f-6.cond00?" stub-flag ;

: old-mansion-1f-6.enter ( -- )   \ 00416A50
    room-sounds
    $1C $BB char-to-tri
    3 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 20.0 0.0 -105.0 0 char-to-xyz
        $1C $9006 1 0 char-anim-hold
    then
    0 $F1 0 action
    hunted state-flag-set
    $275 story-flag? not if
        0 -119.67 9.0 151.64 flicker-sprite
    then
    $56 story-flag? not $7B story-flag? not and if
        1 exit-taken? if
            4 char-unload
            $21 $FB char-to-tri
            $21 char-activate
            $21 -97.0 0.0 84.0 180 char-to-xyz
            $21 0 1 0 char-anim-hold
            0 $21 2 action-force
        else
            $7B story-flag-set
        then
    then
    1 -122.2 13.1 139.0 0 effect-86
    2 -120.5 12.8 140.35 0 effect-86
    1 $2300 sound-volume
;

: old-mansion-1f-6.act07 ( -- )   \ 00417050
    7 ebit-set
    6 ebit-set
    5 ebit-clear
    $22 0 pvar? if
        $A chance? if
            5 ebit-set
        then
    else $22 1 pvar? if
        $19 chance? if
            5 ebit-set
        then
    else $22 2 pvar? if
        $32 chance? if
            5 ebit-set
        then
    else $22 3 pvar? if
        $4B chance? if
            5 ebit-set
        then
    then then then then
    2 creature-action? if
        5 ebit-set
    then
    5 ebit? if
        0 $FE 8 action
    else
        $78 1 item-cooldown
        8 ebit-clear
    then
    0 stalker-alert? if
        $FE 2 stalker-mode
    then
    $22 pvar-inc
    exit
;

: old-mansion-1f-6.char-enter ( -- )   \ 00416AF0
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    0 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 3 -1 char-camera
                0 camera-follow
            else
                1 3 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 -1 char-camera
            0 camera-follow
        else
            1 3 -1 char-camera
            1 camera-follow
        then
    then then
    1 3 -1 area-camera
    0 self-is? if
        1 exit-taken? if
            2 map-page
        then
    then
    $FE self-is? if
        fiona-hidden state-flag? if
            6 ebit-set
            old-mansion-1f-6.act07
        else
            6 ebit-clear
        then
    then
;

: old-mansion-1f-6.phase1 ( -- )   \ 00416B90
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    4 0 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 1 0 1 chars-area-camera
    7 2 -1 1 chars-area-camera
    8 2 -1 1 chars-area-camera
    9 3 -1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 $E char-entered-area? if
        1 map-page
    then
    0 $E char-left-area? if
        2 map-page
    then
    $56 story-flag? not $7B story-flag? not and if
        0 $B char-entered-area? if
            2 ebit-set
        then
    then
    0 $A char-entered-area? if
        $7B story-flag? not if
            4 0 char-remove
            $46 0 82 $80 8 -1 $B4 0.0 creature-place
            $7B story-flag-set
        then
    then
    $FE 2 char-C4? not if
        fiona-hidden state-flag? 7 ebit? not and $FE char-here? and if
            $FE char-busy? not if
                8 ebit-set
                old-mansion-1f-6.act07
            then
        then
    then
    0 20.0 0.0 -105.0 5 10 1 zone
    $FF 0 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    3 0 var? not if
        4 var-inc
        4 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            4 0 var-set
        then
    then
    1 20.8 0.0 -104.26 $1E 15 0 zone
    1 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    1 ebit-set
                    $50 chance? if
                        $1F 1 var-set
                        0 1 $8A action
                    then
                then
            then
        then
    then
    1 20.8 0.0 -104.26 $1E 15 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
    3 1 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    4 2 8 -4 0 zone-at-effect
    0 4 3 char-zone-bits? 0 4 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 4 3 char-zone-bits? 1 4 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 4 3 char-zone-bits? $FE 4 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
;

: old-mansion-1f-6.phase2 ( -- )   \ 00416D60
    $275 story-flag? not if
        2 0 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
    0 $F char-in-area? 0 -45 $3C char-heading? and if
        $FE char-here? not if
            5 5 5 scene-change
        else old-mansion-1f-6.cond00? not if
            5 5 5 scene-change
        else
            $8016 scene-ending
        then then
    then
    0 $C $3C char-faces-area? if
        5 3 0 scene-change
    then
    0 $D char-in-area? 0 45 $3C char-heading? and if
        5 4 0 scene-change
    then
;

: old-mansion-1f-6.phase3 ( -- )   \ 00416DB0
    4 camera-mode? not if
        -111.5 18.0 114.0 -94.0 18.0 93.0 -111.5 0.0 114.0 -94.0 0.0 93.0 lights-doorway
    then
;

: old-mansion-1f-6.act00 ( -- )   \ 00416DF0
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 20.0 0.0 -105.0 0 char-to-xyz
        $1C $9006 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            3 0 var-set
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
            $1C $9007 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9006 0 3 char-anim-hold
            3 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9006 0 0 char-anim-hold
            3 0 var-set
        else
            3 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: old-mansion-1f-6.act01 ( -- )   \ 00416EB0
    self-wait-done
    -119.67 151.64 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $275 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: old-mansion-1f-6.act02 ( -- )   \ 00416F10
    0 old-mansion-1f-6.cmd00
    begin
        2 ebit? not while
        yield
    repeat
    $21 $200 1 4 char-anim-hold
    1 0 var-set
    2 0 var-set
    begin
        1 old-mansion-1f-6.cmd00
        3 ebit? not while
        yield
    repeat
    begin
        2 old-mansion-1f-6.cmd00
        1 0 var? while
        yield
    repeat
    self-idle-or-end
;

: old-mansion-1f-6.act03 ( -- )   \ 00416F50
    self-wait-done
    20.0 -120.0 self-turn-to-xz
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-6.act04 ( -- )   \ 00416F60
    self-wait-done
    90 self-turn-angle
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-6.act06 ( -- )   \ 00417020
    4 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    4 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-6.act05 ( -- )   \ 00416F80
    stalkers-stay state-flag-set
    6 ebit-clear
    1 self-scripted
    7 ebit-clear
    0 0 char-file-load
    self-wait-done
    $FF self-look-at
    yield
    0 char-file-use
    $92 $8004 5 -110.16 146.09 -90 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    4 ebit-clear
    0 avoid-prompt
    $FF 4 -1 char-camera
    begin
        0 2 pad? not if
            $FF panic-stage? 4 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] old-mansion-1f-6.act06 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                6 ebit? $FE char-here? not and if
                    6 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] old-mansion-1f-6.act06 goto
            then
            0 camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            $8002 5 self-anim-9
            self-frames-reset
            5 self-wait-frames
            0 $7E 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-1f-6.act08 ( -- )   \ 004170B0
    self-wait-done
    8 ebit? not if
        $FE 0 char-heading-for? 0 exit-door-open? not and if
            0 3 self-move-slot
            self-wait-done
        then
    else $FE 0 char-heading-for? 0 exit-door-open? not and $FE 0 char-in-area? and if
        0 3 self-move-slot
        self-wait-done
    then then
    8 ebit-clear
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $20 -100.0 146.09 -90 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        $FE char-file-use
        4 $FE 1 char-model-op
        $8000 self-anim
        self-wait-anim
        4 $FE 0 char-model-op
    else
        $1601 self-anim
        self-wait-anim
    then
    4 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: old-mansion-1f-6.phase5 ( -- )   \ 0047AC40
    $1C action-end
    $1C char-done
;

\ ---- registered ----
' old-mansion-1f-6.enter old-mansion-1f-6 0 room-script!
' old-mansion-1f-6.char-enter old-mansion-1f-6 6 room-script!
' old-mansion-1f-6.phase1 old-mansion-1f-6 1 room-script!
' old-mansion-1f-6.phase2 old-mansion-1f-6 2 room-script!
' old-mansion-1f-6.phase3 old-mansion-1f-6 3 room-script!
' old-mansion-1f-6.act00 old-mansion-1f-6 $00 action-script!
' old-mansion-1f-6.act01 old-mansion-1f-6 $01 action-script!
' old-mansion-1f-6.act02 old-mansion-1f-6 $02 action-script!
' old-mansion-1f-6.act03 old-mansion-1f-6 $03 action-script!
' old-mansion-1f-6.act04 old-mansion-1f-6 $04 action-script!
' old-mansion-1f-6.act05 old-mansion-1f-6 $05 action-script!
' old-mansion-1f-6.act06 old-mansion-1f-6 $06 action-script!
' old-mansion-1f-6.act07 old-mansion-1f-6 $07 action-script!
' old-mansion-1f-6.act08 old-mansion-1f-6 $08 action-script!
' old-mansion-1f-6.phase5 old-mansion-1f-6 5 room-script!
