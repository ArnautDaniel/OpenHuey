\ events/map2.fs - the event scripts of the rooms on the game's map 2 (kMapRooms).
\ Converted from the game's bytecode by tools/events2forth.py, once: edit by hand.
IN: events.map2
USING: events.words ;

\ ---- room $67 ----------------------------------------------------------------------------------

: room67.enter ( -- )   \ 0041DD20
    room-sounds
    0 $F2 3 action
    $1C $2F char-to-tri
    1 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 22.0 -16.0 145.0 0 char-to-xyz
        $1C $9008 1 0 char-anim-hold
    then
    0 $F1 1 action
    $22 state-flag-set
    $336 story-flag? not if
        0 1 $14 door-bits
    then
    $10 54.3 20.2 154.5 $E $80 $80 $80 $40 specks
    $10 -53.5 20.2 154.5 $E $80 $80 $80 $40 specks
;

: room67.char-enter ( -- )   \ 0041DD90
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    0 3 3 area-camera
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
;

: room67.phase1 ( -- )   \ 0041DE10
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    $86 story-flag? not if
        0 1 char-in-area? if
            0 0 0 action
        then
    else hewie-controlled? not if
        0 1 char-in-area? 0 char-busy? not and if
            1 exit-check
        then
    else 1 1 char-in-area? 1 char-busy? not and if
        1 exit-check
    then then then
    2 0 0 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 1 1 1 chars-area-camera
    5 2 2 1 chars-area-camera
    6 4 4 1 chars-area-camera
    7 5 5 1 chars-area-camera
    8 3 3 1 chars-area-camera
    9 3 3 1 chars-area-camera
    $A 5 -1 1 chars-area-camera
    $B 2 2 1 chars-area-camera
    $C 5 5 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    $E 5 -1 1 chars-area-camera
    0 3 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 22.0 -16.0 145.0 5 10 1 zone
    $FF 0 char-in-zone? if
        0 ebit-set
    else
        0 ebit-clear
    then
    1 0 var? not if
        2 var-inc
        2 60 var? if
            $21 chance? if
                $1C 2 6 char-sound
            else $32 chance? if
                $1C 3 6 char-sound
            else
                $1C 4 6 char-sound
            then then
            2 0 var-set
        then
    then
    1 22.39 -16.0 144.46 $15 15 0 zone
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
    1 22.39 -16.0 144.46 $15 15 0 zone
    1 char-here? 1 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 15.0 hewie-look-zone
            then
        then
    then
;

: room67.phase5 ( -- )   \ 0041DF60
    2 ebit? not if
        $1C action-end
        $1C char-done
    then
;

: room67.phase2 ( -- )   \ 0041DF70
    $336 story-flag? not if
        0 $F char-in-area? 0 90 $32 char-heading? and if
            5 2 4 scene-change
        then
    then
;

: room67.act00 ( -- )   \ 0041DF90
    $18 state-flag-set
    1 self-scripted
    3 0 char-remove
    $F1 action-end
    2 ebit-set
    begin
        loader-done? while
        yield
    repeat
    $E 3 $FF char-load
    $F 4 char-load-2
    $F $44 fade
    self-wait-done
    wait-fade
    8 state-flag-set
    1 exit-check
    self-idle-or-end
;

: room67.act01 ( -- )   \ 0041DFC0
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-unload
        $1C char-activate
        $1C 22.0 -16.0 145.0 0 char-to-xyz
        $1C $9008 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        0 ebit? if
            1 0 var-set
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
            $1C $9009 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9008 0 3 char-anim-hold
            1 0 var-set
            0 5 var? if
                $1C wait-char-anim
                0 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9008 0 0 char-anim-hold
            1 0 var-set
        else
            1 var-inc
        then
        self-frames-reset
        1 self-wait-frames
    again
;

: room67.act02 ( -- )   \ 0041E080
    self-wait-done
    -324.0 322.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $336 story-flag-set
    0 0 $14 door-bits
    $1F message-param-room
    $1F 1 item-give-count
    0 $1F item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: room67.act03 ( -- )   \ 0041E0D0
    begin
        3 $1C char-loaded? not while
        yield
    repeat
    $19 1.0 0 bgm
    self-idle-or-end
;

' room67.enter $67 0 room-script!
' room67.char-enter $67 6 room-script!
' room67.phase1 $67 1 room-script!
' room67.phase5 $67 5 room-script!
' room67.phase2 $67 2 room-script!
' room67.act00 $67 $00 action-script!
' room67.act01 $67 $01 action-script!
' room67.act02 $67 $02 action-script!
' room67.act03 $67 $03 action-script!

\ ---- room $100 ---------------------------------------------------------------------------------

: room100.char-enter ( -- )   \ 00417190
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
;

: room100.phase1 ( -- )   \ 00417290
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
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
;

: room100.enter ( -- )   \ 0047AC50
    $19 1.0 0 bgm
;

' room100.char-enter $100 6 room-script!
' room100.phase1 $100 1 room-script!
' room100.enter $100 0 room-script!

\ ---- room $101 ---------------------------------------------------------------------------------

: room101.char-enter ( -- )   \ 00417320
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
;

: room101.phase1 ( -- )   \ 00417420
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
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
;

: room101.enter ( -- )   \ 0047AC58
    $19 1.0 0 bgm
;

' room101.char-enter $101 6 room-script!
' room101.phase1 $101 1 room-script!
' room101.enter $101 0 room-script!

\ ---- room $102 ---------------------------------------------------------------------------------

\ room 0x102: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: room102.cmd00 ( b0 -- )  drop stub-step ;

: room102.enter ( -- )   \ 004174B0
    room-sounds
    $19 1.0 0 bgm
    0 room102.cmd00
;

: room102.act02 ( -- )   \ 00417780
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

: room102.char-enter ( -- )   \ 004174C0
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
            room102.act02
        else
            2 ebit-clear
        then
    then
;

: room102.phase1 ( -- )   \ 004175D0
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
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
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
    2 room102.cmd00
;

: room102.phase2 ( -- )   \ 00417690
    0 -61.3 9.0 0.24 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 room102.cmd00
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

: room102.act01 ( -- )   \ 00417750
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

: room102.act00 ( -- )   \ 004176D0
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
                ['] room102.act01 goto
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
                ['] room102.act01 goto
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

: room102.act03 ( -- )   \ 004177D0
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

' room102.enter $102 0 room-script!
' room102.char-enter $102 6 room-script!
' room102.phase1 $102 1 room-script!
' room102.phase2 $102 2 room-script!
' room102.act00 $102 $00 action-script!
' room102.act01 $102 $01 action-script!
' room102.act02 $102 $02 action-script!
' room102.act03 $102 $03 action-script!

\ ---- room $103 ---------------------------------------------------------------------------------

\ room 0x103: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: room103.cmd00 ( b0 -- )  drop stub-step ;

: room103.enter ( -- )   \ 00417840
    room-sounds
    $19 1.0 0 bgm
    0 room103.cmd00
;

: room103.act02 ( -- )   \ 00417B10
    $FE camera-follow
    1 ebit-clear
    $1C 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $1C 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $1C 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $1C 3 pvar? if
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
    $1C pvar-inc
    exit
;

: room103.char-enter ( -- )   \ 00417850
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
            room103.act02
        else
            2 ebit-clear
        then
    then
;

: room103.phase1 ( -- )   \ 00417960
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
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    $FF panic-stage? not if
        0 char-busy? not 2 2 pad? and if
            0 70.5 9.0 -5.5 5 10 0 zone
            0 0 8 char-zone-bits? if
                $FE char-here? not if
                    0 0 0 action
                else $FE 1 char-heading-for? not if
                    0 0 0 action
                then then
            then
        then
    then
    2 room103.cmd00
;

: room103.phase2 ( -- )   \ 00417A20
    0 70.5 9.0 -5.5 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 room103.cmd00
        $FF panic-stage? not if
            $FE char-here? not if
                5 0 5 scene-change
            else $FE 1 char-heading-for? if
                $8016 scene-ending
            else
                5 0 5 scene-change
            then then
        then
    then
;

: room103.act01 ( -- )   \ 00417AE0
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

: room103.act00 ( -- )   \ 00417A60
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
                ['] room103.act01 goto
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
                ['] room103.act01 goto
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

: room103.act03 ( -- )   \ 00417B60
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

' room103.enter $103 0 room-script!
' room103.char-enter $103 6 room-script!
' room103.phase1 $103 1 room-script!
' room103.phase2 $103 2 room-script!
' room103.act00 $103 $00 action-script!
' room103.act01 $103 $01 action-script!
' room103.act02 $103 $02 action-script!
' room103.act03 $103 $03 action-script!

\ ---- room $104 ---------------------------------------------------------------------------------

\ room 0x104: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: room104.cmd00 ( b0 -- )  drop stub-step ;

: room104.act02 ( -- )   \ 00417E90
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

: room104.char-enter ( -- )   \ 00417BD0
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
        9 state-flag? $FE 3 char-heading-for? and if
            2 ebit-set
            room104.act02
        else
            2 ebit-clear
        then
    then
;

: room104.phase1 ( -- )   \ 00417CE0
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
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
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
    2 room104.cmd00
;

: room104.phase2 ( -- )   \ 00417DA0
    0 3.4 9.0 62.55 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 room104.cmd00
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

: room104.act01 ( -- )   \ 00417E60
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

: room104.act00 ( -- )   \ 00417DE0
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
                ['] room104.act01 goto
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
                ['] room104.act01 goto
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

: room104.act03 ( -- )   \ 00417EE0
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

: room104.enter ( -- )   \ 0047AC60
    room-sounds
    0 room104.cmd00
;

' room104.char-enter $104 6 room-script!
' room104.phase1 $104 1 room-script!
' room104.phase2 $104 2 room-script!
' room104.act00 $104 $00 action-script!
' room104.act01 $104 $01 action-script!
' room104.act02 $104 $02 action-script!
' room104.act03 $104 $03 action-script!
' room104.enter $104 0 room-script!

\ ---- room $105 ---------------------------------------------------------------------------------

\ room 0x105: three hanging things that Fiona pushes as she walks by, swinging (swing_three).
: room105.cmd00 ( b0 -- )  drop stub-step ;

: room105.enter ( -- )   \ 00417F40
    room-sounds
    $19 1.0 0 bgm
    0 room105.cmd00
;

: room105.act02 ( -- )   \ 00418210
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

: room105.char-enter ( -- )   \ 00417F50
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
            room105.act02
        else
            2 ebit-clear
        then
    then
;

: room105.phase1 ( -- )   \ 00418060
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
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
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
    2 room105.cmd00
;

: room105.phase2 ( -- )   \ 00418120
    0 -4.49 9.0 -66.28 5 10 0 zone
    0 0 8 char-zone-bits? if
        1 room105.cmd00
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

: room105.act01 ( -- )   \ 004181E0
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

: room105.act00 ( -- )   \ 00418160
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
                ['] room105.act01 goto
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
                ['] room105.act01 goto
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

: room105.act03 ( -- )   \ 00418260
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

' room105.enter $105 0 room-script!
' room105.char-enter $105 6 room-script!
' room105.phase1 $105 1 room-script!
' room105.phase2 $105 2 room-script!
' room105.act00 $105 $00 action-script!
' room105.act01 $105 $01 action-script!
' room105.act02 $105 $02 action-script!
' room105.act03 $105 $03 action-script!

\ ---- room $106 ---------------------------------------------------------------------------------

\ room 0x106: starts a water drip effect (OneDrip, 0xC0 bytes).
: room106.cmd00 ( -- )  stub-step ;

: room106.enter ( -- )   \ 004182D0
    $86 story-flag? if
        room-sounds
        $19 1.0 0 bgm
    then
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $215 story-flag? not if
        1 2 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 2 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2C8 story-flag? not if
            0 -367.0 1.0 -118.7 flicker-sprite
        then
    then
    $216 story-flag? not if
        1 1 $8000000 nav-group
        2 1 $14 door-bits
        3 0 $14 door-bits
    else
        0 1 $8000000 nav-group
        2 0 $14 door-bits
        3 1 $14 door-bits
    then
    $217 story-flag? not if
        1 0 $8000000 nav-group
        4 1 $14 door-bits
        5 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        4 0 $14 door-bits
        5 1 $14 door-bits
    then
    $218 story-flag? not if
        1 3 $8000000 nav-group
        7 1 $14 door-bits
        6 0 $14 door-bits
    else
        0 3 $8000000 nav-group
        7 0 $14 door-bits
        6 1 $14 door-bits
    then
    $219 story-flag? not if
        1 4 $8000000 nav-group
        9 1 $14 door-bits
        8 0 $14 door-bits
    else
        0 4 $8000000 nav-group
        9 0 $14 door-bits
        8 1 $14 door-bits
        $2C9 story-flag? not if
            1 -275.5 1.0 -96.0 flicker-sprite
        then
    then
    $21A story-flag? not if
        1 5 $8000000 nav-group
        $B 1 $14 door-bits
        $A 0 $14 door-bits
    else
        0 5 $8000000 nav-group
        $B 0 $14 door-bits
        $A 1 $14 door-bits
    then
    $21B story-flag? not if
        1 6 $8000000 nav-group
        $D 1 $14 door-bits
        $C 0 $14 door-bits
    else
        0 6 $8000000 nav-group
        $D 0 $14 door-bits
        $C 1 $14 door-bits
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    room106.cmd00
;

: room106.char-enter ( -- )   \ 00418430
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
    0 self-is? if
        $86 story-flag? not if
            0 0 0 action
        then
    then
;

: room106.phase1 ( -- )   \ 004184C0
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
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        $1A 4 1 char-load
        $E 5 1 char-load
        $86 story-flag? if
            3 0 char-remove
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-left-area? if
        $86 story-flag? if
            $1C 3 0 char-load
        then
        4 0 char-remove
        5 0 char-remove
    then
    $216 story-flag? not if
        1 -376.0 0.0 -115.9 5 8 1 zone
        $FF 1 char-in-zone? if
            $216 story-flag-set
            0 1 $8000000 nav-group
            2 0 $14 door-bits
            3 1 $14 door-bits
            -376.0 0.0 -115.9 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -376.0 0.0 -115.9 0 0 sound
            $40 $F0 noise
        then
    then
    $217 story-flag? not if
        2 -381.0 0.0 -106.4 5 8 1 zone
        $FF 2 char-in-zone? if
            $217 story-flag-set
            0 0 $8000000 nav-group
            4 0 $14 door-bits
            5 1 $14 door-bits
            -381.0 0.0 -106.4 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -381.0 0.0 -106.4 0 0 sound
            $40 $E6 noise
        then
    then
    $218 story-flag? not if
        3 -275.5 0.0 -105.0 5 8 1 zone
        $FF 3 char-in-zone? if
            $218 story-flag-set
            0 3 $8000000 nav-group
            7 0 $14 door-bits
            6 1 $14 door-bits
            -275.5 0.0 -105.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -275.5 0.0 -105.0 0 0 sound
            $40 $84 noise
        then
    then
    $21A story-flag? not if
        5 -244.5 0.0 -105.0 5 8 1 zone
        $FF 5 char-in-zone? if
            $21A story-flag-set
            0 5 $8000000 nav-group
            $B 0 $14 door-bits
            $A 1 $14 door-bits
            -244.5 0.0 -105.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -244.5 0.0 -105.0 0 0 sound
            $40 $8E noise
        then
    then
    $21B story-flag? not if
        6 -244.5 0.0 -96.0 5 8 1 zone
        $FF 6 char-in-zone? if
            $21B story-flag-set
            0 6 $8000000 nav-group
            $D 0 $14 door-bits
            $C 1 $14 door-bits
            -244.5 0.0 -96.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -244.5 0.0 -96.0 0 0 sound
            $40 $F7 noise
        then
    then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
;

: room106.phase2 ( -- )   \ 00418730
    0 6 $32 char-faces-area? if
        $334 story-flag? not if
            5 1 0 scene-change
        else
            5 $85 3 scene-change
        then
    then
    $215 story-flag? not if
        0 -367.0 0.0 -118.7 5 8 1 zone
        $FF 0 char-in-zone? if
            $215 story-flag-set
            0 2 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -367.0 0.0 -118.7 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -367.0 0.0 -118.7 0 0 sound
            $40 $E8 noise
            0 -367.0 1.0 -118.7 flicker-sprite
        then
    else $2C8 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 2 4 scene-change
        then
    then then
    $219 story-flag? not if
        4 -275.5 0.0 -96.0 5 8 1 zone
        $FF 4 char-in-zone? if
            $219 story-flag-set
            0 4 $8000000 nav-group
            9 0 $14 door-bits
            8 1 $14 door-bits
            -275.5 0.0 -96.0 0 -2143272896 0 0.0 scene-effect-8C
            $88 5 -275.5 0.0 -96.0 0 0 sound
            $40 $FE noise
            1 -275.5 1.0 -96.0 flicker-sprite
        then
    else $2C9 story-flag? not if
        4 1 5 5 0 zone-at-effect
        0 4 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
;

: room106.act00 ( -- )   \ 00418860
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $34 door-unlock
    \ (nop-progress-24: no effect in this game)
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    3 char-unload
    4 3 char-hand-over
    room-sounds
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    wait-fade
    50 hewie-trust
    1 action-end
    1 char-done
    3 0 char-remove
    4 0 char-remove
    begin
        loader-done? while
        yield
    repeat
    0 self-move-16
    0 $BD -78.171 -25.963 -73 char-to-xz
    camera-restart
    $86 story-flag-set
    $19 1.0 0 bgm
    $F $41 fade
    wait-fade
    $247 item-give
    $18 state-flag-clear
    0 self-scripted
    $25 state-flag-set
    self-idle-or-end
;

: room106.act01 ( -- )   \ 00418930
    self-wait-done
    0 message
    wait-message
    $334 story-flag-set
    self-idle-or-end
;

: room106.act02 ( -- )   \ 00418940
    self-wait-done
    -367.0 -118.7 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C8 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: room106.act03 ( -- )   \ 004189A0
    self-wait-done
    -275.5 -96.0 self-turn-to-xz
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
            $2C9 story-flag-set
            1 effect-remove
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

: room106.act04 ( -- )   \ 00418A00
    self-wait-done
    room106.cmd00
    $E 3 $FF char-load
    $F 4 char-load-2
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    3 char-unload
    4 3 char-hand-over
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room106.enter $106 0 room-script!
' room106.char-enter $106 6 room-script!
' room106.phase1 $106 1 room-script!
' room106.phase2 $106 2 room-script!
' room106.act00 $106 $00 action-script!
' room106.act01 $106 $01 action-script!
' room106.act02 $106 $02 action-script!
' room106.act03 $106 $03 action-script!
' room106.act04 $106 $04 action-script!

\ ---- room $107 ---------------------------------------------------------------------------------

\ room 0x107: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 2 a frame (room_nudge).
: room107.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x107: starts the slowly turning backdrop model (Effect7A3D0: model 0x30 far off at
\ (-225, -64, -1125)).
: room107.cmd01 ( -- )  stub-step ;
\ room 0x107: a noise of loudness 0x20 or more was made in this room last frame (the progress'
\ noise requests kept at +0x10D4).
: room107.cond00? ( -- flag )  stub-flag ;

: room107.enter ( -- )   \ 00418AF0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $21C story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2CB story-flag? not if
            0 -298.4 72.6 -365.4 flicker-sprite
        then
    then
    0 $B 0.812 0.687 0.187 0.312 zone-rect
    $35D story-flag? not if
        $E 5 $FF char-load
        0 $F1 0 action
    then
    room107.cmd01
;

: room107.char-enter ( -- )   \ 00418B60
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

: room107.phase1 ( -- )   \ 00418BA0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 ebit? $35D story-flag? not and if
        room107.cond00? if
            $35D story-flag-set
            $F1 action-end
            0 $F1 2 action
        then
    then
;

: room107.phase2 ( -- )   \ 00418BF0
    $21C story-flag? not if
        0 -298.4 71.6 -365.4 5 8 1 zone
        $FF 0 char-in-zone? if
            $21C story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            -298.4 71.6 -365.4 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 -298.4 71.6 -365.4 0 0 sound
            $40 $BA noise
            0 -298.4 72.6 -365.4 flicker-sprite
        then
    else $2CB story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then then
    0 1 char-in-area? 0 90 $2D char-heading? and if
        5 3 0 scene-change
    then
;

: room107.act00 ( -- )   \ 00418C90
    5 char-unload
    2 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -339.86 125.95 -391.22 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: room107.act01 ( -- )   \ 00418CD0
    self-wait-done
    -298.4 -365.4 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message
            wait-message
        else
            $2CB story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
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

: room107.act02 ( -- )   \ 00418D30
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 room107.cmd00
    1 room107.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

: room107.act03 ( -- )   \ 00418D50
    self-wait-done
    3 ebit? not if
        $A02 5 self-anim-blend
        self-wait-anim
        0 message
        wait-message
        3 ebit-set
    else
        $1D02 5 self-anim-blend
        self-wait-anim
        1 message
        wait-message
    then
    self-idle-or-end
;

: room107.phase5 ( -- )   \ 0047AC78
    $E action-end
    $E char-done
;

' room107.enter $107 0 room-script!
' room107.char-enter $107 6 room-script!
' room107.phase1 $107 1 room-script!
' room107.phase2 $107 2 room-script!
' room107.act00 $107 $00 action-script!
' room107.act01 $107 $01 action-script!
' room107.act02 $107 $02 action-script!
' room107.act03 $107 $03 action-script!
' room107.phase5 $107 5 room-script!

\ ---- room $108 ---------------------------------------------------------------------------------

\ byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(1, 0), then the partner's
\ message slot shows progress +0x73EDC0; else Progress_SpeechCall and the slot is closed
: room108.cmd00 ( b0 -- )  drop stub-step ;

: room108.enter ( -- )   \ 00418DC0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $21D story-flag? not if
        1 0 $8000000 nav-group
        0 1 $14 door-bits
        1 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        0 0 $14 door-bits
        1 1 $14 door-bits
        $2CA story-flag? not if
            0 76.5 -25.5 134.5 flicker-sprite
        then
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    $86 story-flag? $88 story-flag? not and if
        1 char-activate
        1 $118 4.7 54.573 180 char-to-xz
        0 1 1 action
    then
;

: room108.char-enter ( -- )   \ 00418E40
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    0 2 2 area-camera
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
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    3 3 3 area-camera
;

: room108.phase1 ( -- )   \ 00418F40
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
    8 2 2 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 4 4 1 chars-area-camera
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 $D char-entered-area? if
        $86 story-flag? $87 story-flag? not and if
            $87 story-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
    then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
    $86 story-flag? $88 story-flag? not and if
        0 0 var? if
            $1E chance? if
                1 $40000001 6 char-sound
                0 180 var-set
            else
                0 60 var-set
            then
        else
            0 var-dec
            1 $C0000001 6 char-sound
        then
    then
;

: room108.phase2 ( -- )   \ 00419050
    $86 story-flag? $88 story-flag? not and if
        $87 story-flag? 0 1 $A $A $2D char-touching-facing? and if
            5 2 0 scene-change
        then
    then
    $21D story-flag? not if
        0 76.5 -26.5 134.5 5 8 1 zone
        $FF 0 char-in-zone? if
            $21D story-flag-set
            0 0 $8000000 nav-group
            0 0 $14 door-bits
            1 1 $14 door-bits
            76.5 -26.5 134.5 0 -2144325584 0 0.0 scene-effect-8C
            $88 5 76.5 -26.5 134.5 0 0 sound
            $40 $262 noise
            0 76.5 -25.5 134.5 flicker-sprite
        then
    else $2CA story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then then
;

: room108.phase5 ( -- )   \ 004190F0
    $86 story-flag? $88 story-flag? not and if
        1 action-end
        1 char-done
        $27 0 pvar? if
            3 room108.cmd00
        then
    then
;

: room108.act00 ( -- )   \ 00419110
    1 self-scripted
    $18 state-flag-set
    $FE action-end
    $FE char-done
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    1 action-end
    self-frames-reset
    1 self-wait-frames
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    30 hewie-trust
    8 state-flag-set
    $87 story-flag-set
    0 $9E 12.16 49.91 -7 char-to-xz
    1 $118 4.7 54.573 180 char-to-xz
    $27 0 pvar? if
        3 room108.cmd00
    then
    0 counter-set
    0 1 1 action
    1 wait-counter
    $FE char-activate
    $FE $108 305 2 stalker-to-room
    $FE 3 3 char-camera
    $FE 2 stalker-mode
    0 state-flag-set
    stalker-item-cooldown
    camera-restart
    $F $51 fade
    $19 1.0 0 bgm
    wait-fade
    $39 resident-flag-set
    $259 item-give
    $8285 item-give
    0 self-scripted
    $18 state-flag-clear
    $25 state-flag-clear
    $D state-flag-clear
    self-idle-or-end
;

: room108.act01 ( -- )   \ 00419220
    1 self-noclip
    1 self-scripted
    self-wait-done
    $1004 0 self-anim-blend
    $27 0 pvar? if
        $87 story-flag? not if
            0 room108.cmd00
        else
            1 room108.cmd00
        then
        2 room108.cmd00
    then
    1 counter-set
    begin
        yield
    again
;

: room108.act02 ( -- )   \ 00419250
    self-wait-done
    $800 self-anim
    self-wait-anim
    1 message
    wait-message
    1 $5C 5 char-sound
    $802 self-anim
    self-wait-anim
    self-idle-or-end
;

: room108.act03 ( -- )   \ 00419270
    self-wait-done
    76.5 134.5 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message
            wait-message
        else
            $2CA story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
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

: room108.act04 ( -- )   \ 004192D0
    self-wait-done
    $27 0 pvar? if
        0 room108.cmd00
        2 room108.cmd00
    then
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    $27 0 pvar? if
        3 room108.cmd00
    then
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room108.enter $108 0 room-script!
' room108.char-enter $108 6 room-script!
' room108.phase1 $108 1 room-script!
' room108.phase2 $108 2 room-script!
' room108.phase5 $108 5 room-script!
' room108.act00 $108 $00 action-script!
' room108.act01 $108 $01 action-script!
' room108.act02 $108 $02 action-script!
' room108.act03 $108 $03 action-script!
' room108.act04 $108 $04 action-script!

\ ---- room $109 ---------------------------------------------------------------------------------

\ character kind 0x1A: byte 3 0 starts Kind26_MoveTo(2, -6, 257); else waits (2) until
\ Kind26_MoveDone says done
: room109.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x109: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 1 a frame (room_nudge).
: room109.cmd01 ( b0 -- )  drop stub-step ;

: room109.enter ( -- )   \ 004193D0
    room-sounds
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $1E chance? if
        $1A 4 $FF char-load
        0 $F1 0 action
    then
    $8A story-flag? not if
        $E 5 $FF char-load
        0 $F2 1 action
    then
;

: room109.char-enter ( -- )   \ 00419400
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
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    1 3 3 area-camera
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
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    3 2 2 area-camera
    hewie-controlled? not if
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    4 2 2 area-camera
;

: room109.phase1 ( -- )   \ 00419530
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
    hewie-controlled? not if
        0 4 char-in-area? 0 char-busy? not and if
            4 exit-check
        then
    else 1 4 char-in-area? 1 char-busy? not and if
        4 exit-check
    then then
    $A 3 3 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 3 3 1 chars-area-camera
    0 5 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 6 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 8 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 9 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 ebit? $8A story-flag? not and if
        0 -24.0 8.31 -132.0 $19 10 0 zone
        0 0 2 char-zone-bits? 1 0 2 char-zone-bits? or if
            $8A story-flag-set
            $F2 action-end
            0 $F2 2 action
        then
    then
;

: room109.phase5 ( -- )   \ 00419638
    $1A action-end
    $1A char-done
    $E action-end
    $E char-done
;

: room109.act00 ( -- )   \ 00419650
    4 char-unload
    $1A char-activate
    $1A 1 3.0 235.0 180 char-to-xz
    $1A $9000 1 0 char-anim-hold
    0 room109.cmd00
    1 room109.cmd00
    $1A action-end
    $1A char-done
    self-idle-or-end
;

: room109.act01 ( -- )   \ 00419680
    5 char-unload
    2 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -21.17 31.84 -137.07 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: room109.act02 ( -- )   \ 004196C0
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 room109.cmd01
    1 room109.cmd01
    $E action-end
    $E char-done
    self-idle-or-end
;

' room109.enter $109 0 room-script!
' room109.char-enter $109 6 room-script!
' room109.phase1 $109 1 room-script!
' room109.phase5 $109 5 room-script!
' room109.act00 $109 $00 action-script!
' room109.act01 $109 $01 action-script!
' room109.act02 $109 $02 action-script!

\ ---- room $10A ---------------------------------------------------------------------------------

\ room 0x10A: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
\ sideways 1 a frame (room_nudge).
: room10A.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x10A: a noise of loudness 0x20 or more was made in this room last frame (the progress'
\ noise requests kept at +0x10D4).
: room10A.cond00? ( -- flag )  stub-flag ;

: room10A.enter ( -- )   \ 00419730
    room-sounds
    $19 1.0 0 bgm
    $344 story-flag? not if
        $E 5 $FF char-load
        0 $F1 0 action
    then
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
    $2C7 story-flag? not if
        0 -253.83 5.54 -217.28 flicker-sprite
    then
;

: room10A.char-enter ( -- )   \ 00419770
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
;

: room10A.phase1 ( -- )   \ 00419870
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
    0 2 char-in-area? 0 char-busy? not and if
        $86 story-flag? $88 story-flag? not and if
            $17 3 2 char-load
        then
        2 exit-check
    then
    hewie-controlled? not if
        0 3 char-in-area? 0 char-busy? not and if
            3 exit-check
        then
    else 1 3 char-in-area? 1 char-busy? not and if
        3 exit-check
    then then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    0 4 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 5 char-entered-area? 0 6 char-entered-area? or 0 8 char-entered-area? or if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-entered-area? if
        $86 story-flag? $88 story-flag? not and if
            $17 3 2 char-load
        then
        \ (nop-progress-14: no effect in this game)
    then
    0 9 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 7 char-left-area? if
        $86 story-flag? $88 story-flag? not and if
            3 0 char-remove
        then
    then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
    2 ebit? $344 story-flag? not and if
        room10A.cond00? if
            $344 story-flag-set
            $F1 action-end
            0 $F1 2 action
        then
    then
;

: room10A.phase2 ( -- )   \ 00419950
    $2C7 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 1 4 scene-change
        then
    then
;

: room10A.act00 ( -- )   \ 00419970
    5 char-unload
    2 ebit-set
    $E char-activate
    $E $9000 1 0 char-anim-hold
    $E -25.61 38.13 -136.96 0 char-to-xyz
    begin
        $32 chance? if
            $E $9000 1 0 char-anim-hold
        else
            $E $9001 1 0 char-anim-hold
        then
        $E wait-char-anim
    again
;

: room10A.act01 ( -- )   \ 004199B0
    self-wait-done
    -253.83 -217.28 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C7 story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: room10A.act02 ( -- )   \ 00419A10
    $E $A 6 char-sound
    $E $9002 1 0 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    0 room10A.cmd00
    1 room10A.cmd00
    $E action-end
    $E char-done
    self-idle-or-end
;

: room10A.phase5 ( -- )   \ 0047AC80
    $E action-end
    $E char-done
;

' room10A.enter $10A 0 room-script!
' room10A.char-enter $10A 6 room-script!
' room10A.phase1 $10A 1 room-script!
' room10A.phase2 $10A 2 room-script!
' room10A.act00 $10A $00 action-script!
' room10A.act01 $10A $01 action-script!
' room10A.act02 $10A $02 action-script!
' room10A.phase5 $10A 5 room-script!

\ ---- room $10B ---------------------------------------------------------------------------------

\ byte 3 0: a 0x4480 effect is spawned and its slot kept in event var 0; else that slot's effect
\ is removed
: room10B.cmd00 ( b0 -- )  drop stub-step ;
\ room 0x10B: the progress' +0xFB6 count (it rises while Hewie is down, Hewie_AdjustAction) has
\ reached 100.
: room10B.cond00? ( -- flag )  stub-flag ;

: room10B.enter ( -- )   \ 00419A80
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
;

: room10B.char-enter ( -- )   \ 00419AA0
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

: room10B.phase1 ( -- )   \ 00419AE0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    0 2 char-entered-area? if
        \ (nop-progress-14: no effect in this game)
    then
    0 3 char-entered-area? if
        $87 story-flag? room10B.cond00? not or if
            $AE story-flag? not if
                \ (nop-progress-18: no effect in this game)
            else
                \ (nop-progress-18: no effect in this game)
            then
        else
            \ (nop-progress-18: no effect in this game)
        then
    then
    0 1 char-entered-area? if
        $86 story-flag? $88 story-flag? not and if
            4 0 char-remove
            5 0 char-remove
            $88 story-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 0 action-force
            else
                1 0 0 action-force
            then
        then
    then
    0 ebit? not if
        1 ebit? not $FF panic-stage? and if
            0 ebit-set
            deal-things
        then
    then
    $FF panic-stage? if
        1 ebit-set
    else
        1 ebit-clear
    then
;

: room10B.phase3 ( -- )   \ 00419B70
    2 ebit? if
        -60.0 40.0 -30.0 10.0 46.5 -46.0 -60.0 0.0 -40.0 10.0 -10.0 -59.0 lights-doorway
    then
;

: room10B.act00 ( -- )   \ 00419BB0
    1 self-scripted
    0 state-flag-clear
    $FE action-end
    $FE char-done
    1 action-end
    1 char-done
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    $AF story-flag? if
        $16 state-flag-clear
        1 creatures-clear
    then
    0 creatures-clear
    $88 story-flag-set
    $17 3 $FF char-load
    3 char-unload
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    3 5 0 char-model-op
    0 $F9 1 action
    3 ebit-clear
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    $FE action-end
    $FE char-done
    2 ebit-clear
    3 ebit? if
        1 room10B.cmd00
    then
    8 state-flag-set
    $87 story-flag? room10B.cond00? not or if
        3 state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
        effects-arena-flip
        0 char-in
        begin
            yield
            4 sound-bank-loaded? until
        $AE story-flag? not if
            \ (nop-progress-18: no effect in this game)
            3 0 char-remove
            $80 exit-check
        else
            \ (nop-progress-18: no effect in this game)
            $80 exit-check
        then
    else
        $17 action-end
        $17 char-done
        \ (nop-progress-18: no effect in this game)
        $80 exit-check
    then
    0 self-scripted
    self-idle-or-end
;

: room10B.act01 ( -- )   \ 00419CB0
    begin
        1 cutscene-shot? not while
        yield
    repeat
    0 room10B.cmd00
    3 ebit-set
    begin
        2 cutscene-shot? not while
        yield
    repeat
    1 room10B.cmd00
    3 ebit-clear
    begin
        $18 cutscene-shot? not while
        yield
    repeat
    2 ebit-set
    self-idle-or-end
;

: room10B.act02 ( -- )   \ 00419CE0
    self-wait-done
    4 partner-load
    $17 3 $FF char-load
    2 char-unload
    3 char-unload
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    3 5 0 char-model-op
    0 $F9 1 action
    $FF panic-stage? if
        3 panic-stage
    then
    0 message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $1A state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            $1A state-flag? not if
                7 cutscene-control
                cutscene-near-end? if
                    $F 0 fade
                then
                $C cutscene-control
                0 cutscene-control
                4 cutscene-control
                -1 result? not if
                    5 cutscene-mode? not 4 cutscene-mode? not and if
                        yield
                        false
                    else
                        true
                    then
                else
                    true
                then
            else
                true
            then
        until
        $FFFF message-close
        $F9 action-end
        $B cutscene-control
        wait-fade
        1 cutscene-control
        8 state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        6 state-flag-set
        8 cutscene-control
    then
    2 0 char-remove
    3 0 char-remove
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

' room10B.enter $10B 0 room-script!
' room10B.char-enter $10B 6 room-script!
' room10B.phase1 $10B 1 room-script!
' room10B.phase3 $10B 3 room-script!
' room10B.act00 $10B $00 action-script!
' room10B.act01 $10B $01 action-script!
' room10B.act02 $10B $02 action-script!
