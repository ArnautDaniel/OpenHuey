\ story/rooms/chaos-forest-4.fs - the event scripts of room chaos-forest-4 ($102; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-4
USING: room-names story.words story.shared ;

\ room 0x102: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: chaos-forest-4.cmd00 ( b0 -- )  drop s" chaos-forest-4.cmd00" stub-step ;

: chaos-forest-4.enter ( -- )   \ 004174B0
    room-sounds
    $19 1.0 0 bgm
    0 chaos-forest-4.cmd00
;

: chaos-forest-4.act02 ( -- )   \ 00417780
    $FE camera-follow
    1 ebit-clear
    $1B 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $1B 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $1B 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $1B 3 pvar? if
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
    $1B pvar-inc
    exit
;

: chaos-forest-4.char-enter ( -- )   \ 004174C0
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
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    2 1 1 area-camera
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
        9 state-flag? $FE 2 char-heading-for? and if
            2 ebit-set
            chaos-forest-4.act02
        else
            2 ebit-clear
        then
    then
;

: chaos-forest-4.phase1 ( -- )   \ 004175D0
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
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    $FF panic-stage? not if
        0 char-busy? not 2 2 pad? and if
            0 -61.3 9.0 0.24 5 10 0 zone
            0 0 8 char-zone-bits? if
                $FE char-here? not if
                    0 0 0 action
                else $FE 2 char-heading-for? not if
                    0 0 0 action
                then then
            then
        then
    then
    2 chaos-forest-4.cmd00
;

: chaos-forest-4.phase2 ( -- )   \ 00417690
    0 -61.3 9.0 0.24 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 chaos-forest-4.cmd00
        $FF panic-stage? not if
            $FE char-here? not if
                5 0 5 scene-change
            else $FE 2 char-heading-for? if
                $8016 scene-ending
            else
                5 0 5 scene-change
            then then
        then
    then
;

: chaos-forest-4.act01 ( -- )   \ 00417750
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

: chaos-forest-4.act00 ( -- )   \ 004176D0
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
                ['] chaos-forest-4.act01 goto
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
                ['] chaos-forest-4.act01 goto
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

: chaos-forest-4.act03 ( -- )   \ 004177D0
    self-wait-done
    $E0 -83.04 1.02 90 $FFFF 5 self-move-to
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
' chaos-forest-4.enter chaos-forest-4 0 room-script!
' chaos-forest-4.char-enter chaos-forest-4 6 room-script!
' chaos-forest-4.phase1 chaos-forest-4 1 room-script!
' chaos-forest-4.phase2 chaos-forest-4 2 room-script!
' chaos-forest-4.act00 chaos-forest-4 $00 action-script!
' chaos-forest-4.act01 chaos-forest-4 $01 action-script!
' chaos-forest-4.act02 chaos-forest-4 $02 action-script!
' chaos-forest-4.act03 chaos-forest-4 $03 action-script!
