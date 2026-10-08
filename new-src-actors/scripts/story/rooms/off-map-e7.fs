\ story/rooms/off-map-e7.fs - the event scripts of room off-map-e7 ($E7; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-e7
USING: room-names story.words story.shared flag-names ;

\ room 0xE7: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-e7.cmd00 ( -- )  s" off-map-e7.cmd00" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-e7.cond00? ( -- flag )  s" off-map-e7.cond00?" stub-flag ;

: off-map-e7.enter ( -- )   \ 00449AD0
    0 34.2 18.3 14.2 1 effect-86
    1 34.2 18.3 -14.2 1 effect-86
    2 -34.2 18.3 -14.2 1 effect-86
    3 -34.2 18.3 14.2 1 effect-86
    0 1 $14 door-bits
    1 $2300 sound-volume
;

: off-map-e7.act02 ( -- )   \ 00449E00
    1 ebit-clear
    $33 0 pvar? if
        0 chance? if
            1 ebit-set
        then
    else $33 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $33 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $33 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 $FE 3 action
    else
        $78 1 item-cooldown
    then
    $33 pvar-inc
    exit
;

: off-map-e7.char-enter ( -- )   \ 00449B20
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
                0 1 0 char-camera
                0 camera-follow
            else
                1 1 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 0 char-camera
            0 camera-follow
        else
            1 1 0 char-camera
            1 camera-follow
        then
    then then
    1 1 0 area-camera
    $FE self-is? if
        fiona-hidden state-flag? if
            2 ebit-set
            off-map-e7.act02
        else
            2 ebit-clear
        then
    then
;

: off-map-e7.phase1 ( -- )   \ 00449BB0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 0 1 chars-area-camera
    5 0 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 0 -1 chars-area-camera
    1 2 char-entered-area? if
        1 exit-prepare
    then
    1 3 char-entered-area? if
        0 exit-prepare
    then
    4 -0.56 0.0 -21.78 $A 5 0 zone
    1 4 8 char-zone-bits? if
        0 char-here? if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        0 0 0 action
                    then
                then
            then
        then
    then
    0 0 8 -4 0 zone-at-effect
    0 0 3 char-zone-bits? 0 0 3 char-zone-bits-before? not and if
        0 0 char-effect-moving
    then
    1 0 3 char-zone-bits? 1 0 3 char-zone-bits-before? not and if
        1 0 char-effect-moving
    then
    $FE 0 3 char-zone-bits? $FE 0 3 char-zone-bits-before? not and if
        $FE 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    0 1 3 char-zone-bits? 0 1 3 char-zone-bits-before? not and if
        0 1 char-effect-moving
    then
    1 1 3 char-zone-bits? 1 1 3 char-zone-bits-before? not and if
        1 1 char-effect-moving
    then
    $FE 1 3 char-zone-bits? $FE 1 3 char-zone-bits-before? not and if
        $FE 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    0 2 3 char-zone-bits? 0 2 3 char-zone-bits-before? not and if
        0 2 char-effect-moving
    then
    1 2 3 char-zone-bits? 1 2 3 char-zone-bits-before? not and if
        1 2 char-effect-moving
    then
    $FE 2 3 char-zone-bits? $FE 2 3 char-zone-bits-before? not and if
        $FE 2 char-effect-moving
    then
    3 3 8 -4 0 zone-at-effect
    0 3 3 char-zone-bits? 0 3 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 3 3 char-zone-bits? 1 3 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 3 3 char-zone-bits? $FE 3 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
;

: off-map-e7.phase3 ( -- )   \ 00449CE0
    off-map-e7.cond00? not if
        3 ebit-set
    else 3 ebit? if
        3 ebit-clear
    else
        off-map-e7.cmd00
    then then
;

: off-map-e7.phase5 ( -- )   \ 00449D00
    0 char-busy? if
        stalkers-stay state-flag-clear
        fiona-hidden state-flag-clear
        0 action-end
        0 $141 -0.1 -18.54 0 char-to-xz
    then
;

: off-map-e7.act01 ( -- )   \ 00449DD0
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-hidden state-flag-clear
    1 self-noclip
    0 $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        $FE action-end
    then
    0 self-scripted
    self-idle-or-end
;

: off-map-e7.act00 ( -- )   \ 00449D20
    stalkers-stay state-flag-set
    1 self-scripted
    0 0 char-file-load
    self-wait-done
    $141 -0.12 -19.4 180 $FFFF $A self-move-to
    self-wait-done
    2 ebit-clear
    0 char-file-use
    $141 $8004 5 -0.803 -15.664 180 self-walk-anim
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
    0 ebit-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] off-map-e7.act01 goto
            else
                2 panic-grow
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? $FE char-here? not and if
                    2 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            $FE char-here? if
                ['] off-map-e7.act01 goto
            then
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

: off-map-e7.act03 ( -- )   \ 00449E50
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $FE 1 char-heading-for? 1 exit-door-open? not and if
        1 3 self-move-slot
        self-wait-done
    then
    $B2 0.866 0.417 180 $FFFF 5 self-move-to
    self-wait-done
    $1601 self-anim
    self-wait-anim
    0 ebit-set
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: off-map-e7.phase2 ( -- )   \ 0047B118
;

\ ---- registered ----
' off-map-e7.enter off-map-e7 0 room-script!
' off-map-e7.char-enter off-map-e7 6 room-script!
' off-map-e7.phase1 off-map-e7 1 room-script!
' off-map-e7.phase3 off-map-e7 3 room-script!
' off-map-e7.phase5 off-map-e7 5 room-script!
' off-map-e7.act00 off-map-e7 $00 action-script!
' off-map-e7.act01 off-map-e7 $01 action-script!
' off-map-e7.act02 off-map-e7 $02 action-script!
' off-map-e7.act03 off-map-e7 $03 action-script!
' off-map-e7.phase2 off-map-e7 2 room-script!
