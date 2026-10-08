\ story/rooms/chaos-forest-7.fs - the event scripts of room chaos-forest-7 ($105; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-7
USING: room-names story.words story.shared ;

\ room 0x105: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: chaos-forest-7.cmd00 ( b0 -- )  drop s" chaos-forest-7.cmd00" stub-step ;

: chaos-forest-7.enter ( -- )   \ 00417F40
    room-sounds
    $19 1.0 0 bgm
    0 chaos-forest-7.cmd00
;

: chaos-forest-7.act02 ( -- )   \ 00418210
    $FE camera-follow
    1 ebit-clear
    $1E 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $1E 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $1E 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $1E 3 pvar? if
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
    $1E pvar-inc
    exit
;

: chaos-forest-7.char-enter ( -- )   \ 00417F50
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    2 2 2 area-camera
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
        9 state-flag? $FE 0 char-heading-for? and if
            2 ebit-set
            chaos-forest-7.act02
        else
            2 ebit-clear
        then
    then
;

: chaos-forest-7.phase1 ( -- )   \ 00418060
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
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    $FF panic-stage? not if
        0 char-busy? not 2 2 pad? and if
            0 -4.49 9.0 -66.28 5 10 0 zone
            0 0 8 char-zone-bits? if
                $FE char-here? not if
                    0 0 0 action
                else $FE 0 char-heading-for? not if
                    0 0 0 action
                then then
            then
        then
    then
    2 chaos-forest-7.cmd00
;

: chaos-forest-7.phase2 ( -- )   \ 00418120
    0 -4.49 9.0 -66.28 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 chaos-forest-7.cmd00
        $FF panic-stage? not if
            $FE char-here? not if
                5 0 5 scene-change
            else $FE 0 char-heading-for? if
                $8016 scene-ending
            else
                5 0 5 scene-change
            then then
        then
    then
;

: chaos-forest-7.act01 ( -- )   \ 004181E0
    0 ebit? if
        2 avoid-prompt
    then
    9 state-flag-clear
    0 camera-follow
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
    0 0 8 nav-group
    self-idle-or-end
;

: chaos-forest-7.act00 ( -- )   \ 00418160
    $18 state-flag-set
    1 0 8 nav-group
    2 ebit-clear
    1 self-scripted
    1 self-noclip
    0 ebit-clear
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    9 state-flag-set
    $18 state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] chaos-forest-7.act01 goto
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
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] chaos-forest-7.act01 goto
            then
            $18 state-flag-set
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            $18 state-flag-clear
            0 0 8 nav-group
            self-idle-or-end
        then
    again
;

: chaos-forest-7.act03 ( -- )   \ 00418260
    self-wait-done
    $E0 -2.89 -89.23 0 $FFFF 5 self-move-to
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

\ ---- registered ----
' chaos-forest-7.enter chaos-forest-7 0 room-script!
' chaos-forest-7.char-enter chaos-forest-7 6 room-script!
' chaos-forest-7.phase1 chaos-forest-7 1 room-script!
' chaos-forest-7.phase2 chaos-forest-7 2 room-script!
' chaos-forest-7.act00 chaos-forest-7 $00 action-script!
' chaos-forest-7.act01 chaos-forest-7 $01 action-script!
' chaos-forest-7.act02 chaos-forest-7 $02 action-script!
' chaos-forest-7.act03 chaos-forest-7 $03 action-script!
