\ story/rooms/chaos-forest-6.fs - the event scripts of room chaos-forest-6 ($104; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-6
USING: room-names story.words story.shared flag-names ;

\ room 0x104: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: chaos-forest-6.cmd00 ( b0 -- )  drop s" chaos-forest-6.cmd00" stub-step ;

: chaos-forest-6.act02 ( -- )   \ 00417E90
    $FE camera-follow
    1 ebit-clear
    $1D 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $1D 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $1D 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $1D 3 pvar? if
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
    $1D pvar-inc
    exit
;

: chaos-forest-6.char-enter ( -- )   \ 00417BD0
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
                0 1 1 char-camera
                0 camera-follow
            else
                1 1 1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 1 1 char-camera
            0 camera-follow
        else
            1 1 1 char-camera
            1 camera-follow
        then
    then then
    3 1 1 area-camera
    $FE self-is? if
        fiona-hidden state-flag? $FE 3 char-heading-for? and if
            2 ebit-set
            chaos-forest-6.act02
        else
            2 ebit-clear
        then
    then
;

: chaos-forest-6.phase1 ( -- )   \ 00417CE0
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
    0 6 char-entered-area? if
        2 exit-prepare
    then
    $FF panic-stage? not if
        0 char-busy? not 2 2 pad? and if
            0 3.4 9.0 62.55 5 10 0 zone
            0 0 8 char-zone-bits? if
                $FE char-here? not if
                    0 0 0 action
                else $FE 3 char-heading-for? not if
                    0 0 0 action
                then then
            then
        then
    then
    2 chaos-forest-6.cmd00
;

: chaos-forest-6.phase2 ( -- )   \ 00417DA0
    0 3.4 9.0 62.55 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 chaos-forest-6.cmd00
        $FF panic-stage? not if
            $FE char-here? not if
                5 0 5 scene-change
            else $FE 3 char-heading-for? if
                $8016 scene-ending
            else
                5 0 5 scene-change
            then then
        then
    then
;

: chaos-forest-6.act01 ( -- )   \ 00417E60
    0 ebit? if
        2 avoid-prompt
    then
    fiona-hidden state-flag-clear
    0 camera-follow
    stalkers-stay state-flag-set
    $FE self-look-at
    yield
    4 $14 self-anim-blend
    self-wait-anim
    $FF self-look-at
    yield
    0 self-scripted
    0 self-noclip
    stalkers-stay state-flag-clear
    0 0 8 nav-group
    self-idle-or-end
;

: chaos-forest-6.act00 ( -- )   \ 00417DE0
    stalkers-stay state-flag-set
    1 0 8 nav-group
    2 ebit-clear
    1 self-scripted
    1 self-noclip
    0 ebit-clear
    self-wait-done
    $800 self-anim
    self-wait-anim
    $801 self-anim
    fiona-hidden state-flag-set
    stalkers-stay state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            $FF panic-stage? 0 ebit? or if
                $FF panic-stage? not if
                    0 $43 5 char-sound
                then
                ['] chaos-forest-6.act01 goto
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
            fiona-hidden state-flag-clear
            0 camera-follow
            $FE action-end
            2 game-mode? if
                ['] chaos-forest-6.act01 goto
            then
            stalkers-stay state-flag-set
            $802 self-anim
            self-wait-anim
            0 self-scripted
            0 self-noclip
            stalkers-stay state-flag-clear
            0 0 8 nav-group
            self-idle-or-end
        then
    again
;

: chaos-forest-6.act03 ( -- )   \ 00417EE0
    self-wait-done
    $E8 3.84 83.43 -180 $FFFF 5 self-move-to
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

: chaos-forest-6.enter ( -- )   \ 0047AC60
    room-sounds
    0 chaos-forest-6.cmd00
;

\ ---- registered ----
' chaos-forest-6.char-enter chaos-forest-6 6 room-script!
' chaos-forest-6.phase1 chaos-forest-6 1 room-script!
' chaos-forest-6.phase2 chaos-forest-6 2 room-script!
' chaos-forest-6.act00 chaos-forest-6 $00 action-script!
' chaos-forest-6.act01 chaos-forest-6 $01 action-script!
' chaos-forest-6.act02 chaos-forest-6 $02 action-script!
' chaos-forest-6.act03 chaos-forest-6 $03 action-script!
' chaos-forest-6.enter chaos-forest-6 0 room-script!
