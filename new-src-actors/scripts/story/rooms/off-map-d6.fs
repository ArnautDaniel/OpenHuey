\ story/rooms/off-map-d6.fs - the event scripts of room off-map-d6 ($D6; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-d6
USING: room-names story.words story.shared ;

\ (as Room105_Cmd00) pushed by the character slot byte 4 names
: off-map-d6.cmd00 ( b0 b1 -- )  drop drop s" off-map-d6.cmd00" stub-step ;
\ room 0xD6: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
\ (clock_draw; a frame hook).
: off-map-d6.cmd01 ( -- )  s" off-map-d6.cmd01" stub-step ;
\ the progress object's +0x7C with the caller's arguments
: off-map-d6.cond00? ( -- flag )  s" off-map-d6.cond00?" stub-flag ;

: off-map-d6.act02 ( -- )   \ 00447990
    1 ebit-clear
    $2F 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $2F 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $2F 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $2F 3 pvar? if
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
    $2F pvar-inc
    exit
;

: off-map-d6.char-enter ( -- )   \ 00447680
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
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 0 char-camera
            0 camera-follow
        else
            1 0 0 char-camera
            1 camera-follow
        then
    then then
    2 0 0 area-camera
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
    $FE self-is? if
        9 state-flag? $FE 1 char-heading-for? and if
            2 ebit-set
            off-map-d6.act02
        else
            2 ebit-clear
        then
    then
;

: off-map-d6.phase1 ( -- )   \ 00447790
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        0 2 char-in-area? 0 char-busy? not and if
            2 exit-check
        then
    else 1 2 char-in-area? 1 char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    1 4 char-entered-area? if
        0 exit-prepare
    then
    1 6 char-entered-area? if
        2 exit-prepare
    then
    1 7 char-entered-area? if
        3 exit-prepare
    then
    1 70.5 5.0 -5.5 $14 10 0 zone
    1 1 8 char-zone-bits? if
        0 char-here? 0 1 char-heading-for? and if
            0 char-busy? not $FF panic-stage? not and fiona-free? and if
                -1 control-action? not if
                    $FE char-here? not if
                        0 0 0 action
                    else $FE 1 char-heading-for? not if
                        0 0 0 action
                    then then
                then
            then
        then
    then
    0 70.5 5.0 -5.5 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 0 off-map-d6.cmd00
    else 1 0 8 char-zone-bits? if
        1 1 off-map-d6.cmd00
    else $FE 0 8 char-zone-bits? if
        1 2 off-map-d6.cmd00
    then then then
    2 0 off-map-d6.cmd00
;

: off-map-d6.phase3 ( -- )   \ 00447890
    off-map-d6.cond00? not if
        4 ebit-set
    else 4 ebit? if
        4 ebit-clear
    else
        off-map-d6.cmd01
    then then
;

: off-map-d6.phase5 ( -- )   \ 004478B0
    0 char-busy? if
        9 state-flag-clear
        $18 state-flag-clear
        0 action-end
        0 $E7 90.99 -3.17 90 char-to-xz
    then
;

: off-map-d6.act01 ( -- )   \ 00447960
    0 ebit? if
        2 avoid-prompt
    then
    9 state-flag-clear
    $18 state-flag-set
    $FE self-look-at
    yield
    4 $14 self-anim-blend
    self-wait-anim
    $FF self-look-at
    yield
    0 self-scripted
    0 self-noclip
    $18 state-flag-clear
    0 0 $40000 nav-group
    self-idle-or-end
;

: off-map-d6.act00 ( -- )   \ 004478D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $19A 70.5 -5.5 90 $FFFF $A self-move-to
    self-wait-done
    1 0 $40000 nav-group
    2 ebit-clear
    1 self-noclip
    0 ebit-clear
    $800 self-anim
    self-wait-anim
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        -1 control-action? if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] off-map-d6.act01 goto
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
            9 state-flag-clear
            $FE action-end
            2 game-mode? if
                ['] off-map-d6.act01 goto
            then
            $18 state-flag-set
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            $18 state-flag-clear
            0 0 $40000 nav-group
            self-idle-or-end
        then
    again
;

: off-map-d6.act03 ( -- )   \ 004479E0
    self-wait-done
    $E7 90.5 -4.5 -90 $FFFF 5 self-move-to
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

: off-map-d6.enter ( -- )   \ 0047B070
    room-sounds
    0 0 off-map-d6.cmd00
;

: off-map-d6.phase2 ( -- )   \ 0047B078
;

\ ---- registered ----
' off-map-d6.char-enter off-map-d6 6 room-script!
' off-map-d6.phase1 off-map-d6 1 room-script!
' off-map-d6.phase3 off-map-d6 3 room-script!
' off-map-d6.phase5 off-map-d6 5 room-script!
' off-map-d6.act00 off-map-d6 $00 action-script!
' off-map-d6.act01 off-map-d6 $01 action-script!
' off-map-d6.act02 off-map-d6 $02 action-script!
' off-map-d6.act03 off-map-d6 $03 action-script!
' off-map-d6.enter off-map-d6 0 room-script!
' off-map-d6.phase2 off-map-d6 2 room-script!
