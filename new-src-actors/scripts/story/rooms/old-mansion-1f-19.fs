\ story/rooms/old-mansion-1f-19.fs - the event scripts of room old-mansion-1f-19 ($6B; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-19
USING: room-names story.words story.shared ;

: old-mansion-1f-19.enter ( -- )   \ 00438D30
    room-sounds
    $56 story-flag? if
        $7C story-flag? not if
            $7C story-flag-set
            $6B 0 109 $80 5 -1 $B4 0.0 creature-place
        then
    then
    1 $2300 sound-volume
;

: old-mansion-1f-19.act06 ( -- )   \ 00439080
    5 ebit-set
    4 ebit-clear
    $13 0 pvar? if
        $A chance? if
            4 ebit-set
        then
    else $13 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $13 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $13 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    2 creature-action? if
        4 ebit-set
    then
    4 ebit? if
        0 $FE 7 action
    else
        $78 1 item-cooldown
    then
    $13 pvar-inc
    exit
;

: old-mansion-1f-19.char-enter ( -- )   \ 00438D60
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
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    1 0 0 area-camera
    0 self-is? if
        $FE exit-taken? if
            0 $14 0.0 -110.0 0 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            0 0 1 action
        then
    then
    $FE self-is? if
        9 state-flag? if
            5 ebit-set
            old-mansion-1f-19.act06
        else
            5 ebit-clear
        then
    then
;

: old-mansion-1f-19.phase1 ( -- )   \ 00438E20
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
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 3 char-entered-area? if
        $1C 3 1 char-load
    then
    0 2 char-entered-area? if
        3 0 char-remove
    then
    2 0.75 0.0 -116.99 $14 10 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 2 9 char-zone-bits? if
                    6 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 1 $88 action
                    then
                then
            then
        then
    then
    2 0.75 0.0 -116.99 $14 10 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 10.0 hewie-look-zone
            then
        then
    then
    7 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 0.0 9.0 -125.0 0 0 sound
            7 ebit-set
        then
    else
        $C0000008 6 0.0 9.0 -125.0 0 0 sound
    then
;

: old-mansion-1f-19.phase2 ( -- )   \ 00438F10
    0 7 char-in-area? 0 90 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 6 char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            5 4 5 scene-change
        else
            $8016 scene-ending
        then
    then
;

: old-mansion-1f-19.act00 ( -- )   \ 00438F40
    $18 state-flag-set
    1 self-scripted
    $27 room-preload
    3 0 char-remove
    self-wait-done
    180 self-turn-angle
    self-wait-done
    $A02 $A self-anim-blend
    self-wait-anim
    0 message
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        0 $7E 5 char-sound
        8 state-flag-set
        $FE exit-check
    else
        1 exit-prepare
        $1C 3 1 char-load
        $18 state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-19.act01 ( -- )   \ 00438F80
    1 self-scripted
    1 exit-prepare
    $1C 3 1 char-load
    self-wait-done
    camera-restart
    0 $7E 5 char-sound
    $F 1 fade
    wait-fade
    8 state-flag-clear
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-19.act02 ( -- )   \ 0047AE88
    self-idle-or-end
;

: old-mansion-1f-19.act03 ( -- )   \ 0047AE8C
    self-idle-or-end
;

: old-mansion-1f-19.act05 ( -- )   \ 00439050
    3 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    0 camera-follow
    $FF 0 char-visible
    9 state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    3 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-19.act04 ( -- )   \ 00438FA0
    $18 state-flag-set
    5 ebit-clear
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    0 char-file-use
    $31 $8004 5 82.92 -88.12 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    0 $7E 5 char-sound
    $8005 $A self-anim-9
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    $18 state-flag-clear
    9 state-flag-set
    3 ebit-clear
    0 avoid-prompt
    $FF 2 -1 char-camera
    $FF 1 char-visible
    begin
        0 2 pad? not if
            $FF panic-stage? 3 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] old-mansion-1f-19.act05 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? if
                    $FE char-here? not if
                        5 ebit-clear
                        1 avoid-prompt
                    then
                then
                yield
            then
        else
            $FE char-here? if
                ['] old-mansion-1f-19.act05 goto
            then
            0 camera-follow
            $FF 0 char-visible
            9 state-flag-clear
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

: old-mansion-1f-19.act07 ( -- )   \ 004390D0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        $FE 1 char-file-load
    then
    $4F 83.66 -76.9 180 $FFFF 5 self-move-to
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
    3 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: old-mansion-1f-19.phase5 ( -- )   \ 0047AE84
;

\ ---- registered ----
' old-mansion-1f-19.enter old-mansion-1f-19 0 room-script!
' old-mansion-1f-19.char-enter old-mansion-1f-19 6 room-script!
' old-mansion-1f-19.phase1 old-mansion-1f-19 1 room-script!
' old-mansion-1f-19.phase2 old-mansion-1f-19 2 room-script!
' old-mansion-1f-19.act00 old-mansion-1f-19 $00 action-script!
' old-mansion-1f-19.act01 old-mansion-1f-19 $01 action-script!
' old-mansion-1f-19.act02 old-mansion-1f-19 $02 action-script!
' old-mansion-1f-19.act03 old-mansion-1f-19 $03 action-script!
' old-mansion-1f-19.act04 old-mansion-1f-19 $04 action-script!
' old-mansion-1f-19.act05 old-mansion-1f-19 $05 action-script!
' old-mansion-1f-19.act06 old-mansion-1f-19 $06 action-script!
' old-mansion-1f-19.act07 old-mansion-1f-19 $07 action-script!
' old-mansion-1f-19.phase5 old-mansion-1f-19 5 room-script!
