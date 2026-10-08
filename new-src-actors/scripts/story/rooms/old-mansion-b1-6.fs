\ story/rooms/old-mansion-b1-6.fs - the event scripts of room old-mansion-b1-6 ($66; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-6
USING: room-names story.words story.shared ;

\ Room66_Cmd00
: old-mansion-b1-6.cmd00 ( b0 -- )  drop s" old-mansion-b1-6.cmd00" stub-step ;
\ Room66_Cmd01
: old-mansion-b1-6.cmd01 ( b0 b1 -- )  drop drop s" old-mansion-b1-6.cmd01" stub-step ;
\ Room66_Cmd02
: old-mansion-b1-6.cmd02 ( b0 -- )  drop s" old-mansion-b1-6.cmd02" stub-step ;
\ Room66_Cmd03
: old-mansion-b1-6.cmd03 ( b0 b1 -- )  drop drop s" old-mansion-b1-6.cmd03" stub-step ;
\ Room66_Cmd04
: old-mansion-b1-6.cmd04 ( -- )  s" old-mansion-b1-6.cmd04" stub-step ;
\ Room66_Cond00
: old-mansion-b1-6.cond00? ( b0 -- flag )  drop s" old-mansion-b1-6.cond00?" stub-flag ;
\ Room66_Cond01
: old-mansion-b1-6.cond01? ( b0 b1 b2 b3 b4 -- flag )  drop drop drop drop drop s" old-mansion-b1-6.cond01?" stub-flag ;

: old-mansion-b1-6.enter ( -- )   \ 0041E110
    room-sounds
    0 1 $14 door-bits
    1 4 $20000 nav-group
    $5D story-flag? not if
        1 0 $20000 nav-group
        0 0 old-mansion-b1-6.cmd01
    then
    $5E story-flag? not if
        1 1 $20000 nav-group
        1 0 old-mansion-b1-6.cmd01
    then
    $5F story-flag? not if
        1 2 $20000 nav-group
        2 0 old-mansion-b1-6.cmd01
    then
    $60 story-flag? not if
        1 3 $20000 nav-group
        3 0 old-mansion-b1-6.cmd01
    then
    $271 story-flag? not if
        0 2.06 6.5 -99.68 flicker-sprite
    then
    $2C3 story-flag? not if
        2 39.98 1.0 -40.67 flicker-sprite
    then
    $2A9 story-flag? $2AA story-flag? not and if
        1 -138.94 1.0 -114.31 flicker-sprite
    then
    3 117.5 11.7 -21.8 0 effect-86
    4 -217.0 20.1 20.0 1 effect-86
    5 -175.0 20.1 4.7 1 effect-86
    6 -70.0 20.1 -124.7 1 effect-86
    7 -20.0 20.1 -96.7 1 effect-86
    8 11.7 20.1 -55.8 1 effect-86
    9 43.0 20.1 -26.5 1 effect-86
    $A -33.2 20.1 -6.2 1 effect-86
    $B 89.3 20.1 50.0 1 effect-86
    $C -12.4 28.3 24.6 1 effect-86
    $D -40.0 20.1 95.6 1 effect-86
    $E -65.5 20.1 0.0 1 effect-86
    1 $2300 sound-volume
;

: old-mansion-b1-6.char-enter ( -- )   \ 0041E260
    hewie-controlled? not if
        0 self-is? 0 exit-taken? and if
            0 0 char-to-exit
            hewie-controlled? not if
                0 6 4 char-camera
                0 camera-follow
            else
                1 6 4 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 0 exit-taken? and if
        1 0 char-to-exit
        hewie-controlled? not if
            0 6 4 char-camera
            0 camera-follow
        else
            1 6 4 char-camera
            1 camera-follow
        then
    then then
    0 6 4 area-camera
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 5 3 char-camera
                0 camera-follow
            else
                1 5 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 5 3 char-camera
            0 camera-follow
        else
            1 5 3 char-camera
            1 camera-follow
        then
    then then
    1 5 3 area-camera
    hewie-controlled? not if
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 4 2 char-camera
                0 camera-follow
            else
                1 4 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 4 2 char-camera
            0 camera-follow
        else
            1 4 2 char-camera
            1 camera-follow
        then
    then then
    2 4 2 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 2 0 char-camera
                0 camera-follow
            else
                1 2 0 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 2 0 char-camera
            0 camera-follow
        else
            1 2 0 char-camera
            1 camera-follow
        then
    then then
    3 2 0 area-camera
;

: old-mansion-b1-6.phase1 ( -- )   \ 0041E360
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
    8 0 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    $A 0 -1 1 chars-area-camera
    $B 2 0 1 chars-area-camera
    $C 1 -1 1 chars-area-camera
    $D 3 1 1 chars-area-camera
    $E 3 1 1 chars-area-camera
    $F 4 2 1 chars-area-camera
    $10 3 1 1 chars-area-camera
    $11 5 3 1 chars-area-camera
    $12 5 3 1 chars-area-camera
    $13 6 4 1 chars-area-camera
    0 4 char-entered-area? if
        0 exit-prepare
    then
    0 5 char-entered-area? if
        1 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    0 5 char-entered-area? if
        $C ebit-set
    then
    0 5 char-left-area? if
        $C ebit? not if
            3 0 char-remove
        then
        $C ebit-clear
    then
    0 4 char-entered-area? if
        $D ebit-set
    then
    0 4 char-left-area? if
        $D ebit? not if
            3 0 char-remove
            4 0 char-remove
            5 0 char-remove
        then
        $D ebit-clear
    then
    $C ebit? loader-done? not and if
        $1C 3 1 char-load
        $C ebit-clear
    then
    $D ebit? loader-done? not and if
        $14 3 $FF char-load
        $15 4 $FF char-load
        $16 5 $FF char-load
        $D ebit-clear
    then
    0 $11 char-entered-area? if
        8 ebit-set
    then
    0 $11 char-left-area? if
        8 ebit-clear
    then
    0 4 char-entered-area? if
        9 ebit-set
    then
    0 4 char-left-area? if
        9 ebit-clear
    then
    7 ebit? not if
        6 sound-bank-loaded? if
            $5D story-flag? not if
                $40000007 6 -185.0 10.0 20.0 0 0 sound
            else $5E story-flag? not if
                $40000007 6 30.0 10.0 -42.0 0 0 sound
            then then
            $5F story-flag? not if
                $40000008 6 -20.0 10.0 10.0 0 0 sound
            else $60 story-flag? not if
                $40000008 6 -40.0 10.0 -110.0 0 0 sound
            then then
            7 ebit-set
        then
    else
        $5D story-flag? not $5E story-flag? not or if
            8 ebit? $5D story-flag? not and $5E story-flag? or if
                $C0000007 6 -185.0 10.0 20.0 0 0 sound
            then
            8 ebit? not $5E story-flag? not and $5D story-flag? or if
                $C0000007 6 30.0 10.0 -42.0 0 0 sound
            then
        then
        $5F story-flag? not $60 story-flag? not or if
            9 ebit? not $5F story-flag? not and $60 story-flag? or if
                $C0000008 6 -20.0 10.0 10.0 0 0 sound
            then
            9 ebit? $60 story-flag? not and $5F story-flag? or if
                $C0000008 6 -40.0 10.0 -110.0 0 0 sound
            then
        then
    then
    $5D story-flag? not if
        0 $1C char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $F action-force
        then
    then
    $5E story-flag? not if
        0 $1E char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $15 action-force
        then
    then
    $5F story-flag? not if
        0 $20 char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $16 action-force
        then
    then
    $60 story-flag? not if
        0 $22 char-entered-area? $A ebit? not and if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $17 action-force
        then
    then
    $2A9 story-flag? not if
        2 -138.94 0.0 -114.31 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 681 var-set
                $1A 682 var-set
                $1B 1 var-set
                $1C -138940 var-set
                $1D 1000 var-set
                $1E -114310 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    0 144.66 0.0 -5.88 $12 12 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    4 73.86 0.0 -40.16 $C 10 0 zone
    4 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 4 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 4 var-set
                        0 1 $89 action
                    then
                then
            then
        then
    then
    5 73.86 0.0 -40.16 $19 20 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 20.0 hewie-look-zone
            then
        then
    then
    6 3 8 -4 0 zone-at-effect
    0 6 3 char-zone-bits? 0 6 3 char-zone-bits-before? not and if
        0 3 char-effect-moving
    then
    1 6 3 char-zone-bits? 1 6 3 char-zone-bits-before? not and if
        1 3 char-effect-moving
    then
    $FE 6 3 char-zone-bits? $FE 6 3 char-zone-bits-before? not and if
        $FE 3 char-effect-moving
    then
;

: old-mansion-b1-6.phase2 ( -- )   \ 0041E700
    0 $14 $32 char-faces-area? if
        5 8 0 scene-change
    then
    0 $15 $32 char-faces-area? if
        5 $B 0 scene-change
    then
    0 $16 char-in-area? 0 75 -40 $32 char-faces-xz? and if
        5 $C 0 scene-change
    then
    0 $1B char-in-area? if
        5 $E 0 scene-change
    then
    0 $1D char-in-area? if
        5 $12 0 scene-change
    then
    0 $1F char-in-area? if
        5 $13 0 scene-change
    then
    0 $21 char-in-area? if
        5 $14 0 scene-change
    then
    $271 story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $18 4 scene-change
        then
    then
    $2A9 story-flag? $2AA story-flag? not and if
        2 1 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $19 4 scene-change
        then
    then
    $2C3 story-flag? not if
        3 2 5 5 0 zone-at-effect
        0 3 3 char-zone-bits? if
            5 $1A 4 scene-change
        then
    then
;

: old-mansion-b1-6.act05 ( -- )   \ 0041EB40
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        0 $FF $FF $15 $A0 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    $60 story-flag? not if
        3 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5F story-flag? if
        8 6 sound-stop
    then
    $60 story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    3 old-mansion-b1-6.cmd02
    $60 story-flag? not if
        $76 door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $60 story-flag-set
    0 3 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act04 ( -- )   \ 0041EA90
    9 ebit-set
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 $FF $FE $52 $50 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $13 char-entered-area? if
            8 6 4 char-camera
        then
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act05 goto
    then
    1 self-noclip
    begin
        1 $FF $FD $DD $20 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    3 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        1 $FF $FD $B6 $10 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    3 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act03 ( -- )   \ 0041E9A0
    8 ebit-set
    8 3 6 char-sound
    8 $401 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FD $DD $20 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $11 char-entered-area? if
            8 5 3 char-camera
        then
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act04 goto
    then
    1 self-noclip
    begin
        0 $FF $FD $67 $F0 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    $5D story-flag? not if
        0 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5E story-flag? if
        7 6 sound-stop
    then
    $5D story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    0 old-mansion-b1-6.cmd02
    $5D story-flag? not if
        $5E door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5D story-flag-set
    0 0 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act07 ( -- )   \ 0041ECE0
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        1 $FF $FF $EC $78 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $F char-entered-area? if
            8 4 2 char-camera
        then
        yield
    repeat
    $5F story-flag? not if
        2 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $60 story-flag? if
        8 6 sound-stop
    then
    $5F story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    2 old-mansion-b1-6.cmd02
    $5F story-flag? not if
        $7B door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5F story-flag-set
    0 2 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act06 ( -- )   \ 0041EC00
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FF $B1 $E0 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act07 goto
    then
    1 self-noclip
    begin
        0 0 0 $3A $98 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    $5E story-flag? not if
        1 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5D story-flag? if
        7 6 sound-stop
    then
    $5E story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    1 old-mansion-b1-6.cmd02
    $5E story-flag? not if
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5E story-flag-set
    0 1 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act02 ( -- )   \ 0041E8D0
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 0 0 $4E $20 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    0 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act03 goto
    then
    begin
        1 $FF $FF $63 $C0 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act06 goto
    then
    1 self-noclip
    begin
        1 $FF $FE $EE $90 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    2 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        1 $FF $FE $C7 $80 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    2 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act01 ( -- )   \ 0041E830
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FE $C7 $80 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act02 goto
    then
    1 self-noclip
    begin
        0 $FF $FE $79 $60 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    1 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        0 $FF $FE $52 $50 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act00 ( -- )   \ 0041E7A0
    2 0 var-set
    0 old-mansion-b1-6.cmd00
    begin
        1 0 1 $38 $80 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $D char-entered-area? if
            8 3 1 char-camera
        then
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act01 goto
    then
    1 self-noclip
    begin
        1 0 1 $86 $A0 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    0 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        1 0 1 $AD $B0 old-mansion-b1-6.cond01? not while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    0 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act08 ( -- )   \ 0041EDA0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    item-3F-under-10? if
        0 ebit? not if
            $11 message
            wait-message
            0 ebit-set
        then
        $20 story-flag-clear
        2 subscreen-open
        begin
            4 state-flag? while
            yield
        repeat
        yield
        $20 story-flag? if
            $3F message-param-room
            0 $3F item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        $20 story-flag-set
    else 0 ebit? not if
        $F message
        wait-message
        0 ebit-set
    else
        $12 message
        wait-message
    then then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act09 ( -- )   \ 0047ACD0
    self-wait-done
    $10 message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-6.act0A ( -- )   \ 0041EE10
    $18 state-flag-set
    1 self-scripted
    1 ebit-clear
    2 ebit-clear
    3 ebit-clear
    5 ebit-set
    6 ebit-set
    $F $44 fade
    begin
        loader-done? while
        yield
    repeat
    8 3 $FF char-load
    self-wait-done
    $5A story-flag? not if
        1 1 movie-play
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
        wait-fade
        0 0 $14 door-bits
        0 4 $20000 nav-group
        1 char-here? if
            0 1 $10 action-force
        then
        $28 $78 movie-param
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
        $F state-flag-set
        0 self-move-16
        0 $56 83.405 -17.137 -28 char-to-xz
        8 $126 74.6 13.8 0 char-to-xz
        8 3 1 char-camera
        $5A story-flag-set
    else
        wait-fade
        $FF panic-stage? if
            3 panic-stage
        then
        $F state-flag-set
        1 char-here? if
            0 1 $10 action-force
        then
        3 char-unload
        0 0 $14 door-bits
        0 4 $20000 nav-group
        0 $5E 92.522 -37.951 -62 char-to-xz
        8 $196 75.0 -40.0 0 char-to-xz
        8 1 -1 char-camera
    then
    8 char-activate
    8 $200 1 0 char-anim-hold
    8 camera-follow
    camera-restart
    0 8 0 action-force
    6 message
    $F 1 fade
    wait-fade
    begin
        3 ebit? not while
        0 2 pad? 1 ebit? not and if
            6 message-close
            1 ebit-set
            2 ebit-set
        then
        yield
    repeat
    $F 4 fade
    wait-fade
    $F state-flag-clear
    6 message-close
    1 char-busy? if
        1 action-end
    then
    8 action-end
    8 char-done
    3 0 char-remove
    0 1 $14 door-bits
    1 4 $20000 nav-group
    0 camera-follow
    camera-restart
    $F $41 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act0B ( -- )   \ 0041EF90
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    7 message
    wait-message
    $F 6 fade
    wait-fade
    0 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act0C ( -- )   \ 0041EFB0
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    5 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act0D ( -- )   \ 0047ACD8
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-6.act0E ( -- )   \ 0041EFC0
    self-wait-done
    -170.0 20.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act0F ( -- )   \ 0041EFE0
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 -45 $3C char-heading? if
        $F00 self-anim
    else 0 45 $3C char-heading? if
        $F01 self-anim
    else 0 0 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act10 ( -- )   \ 0041F020
    begin
        1 char-busy? not while
        yield
    repeat
    $FF 1 char-visible
    begin
        yield
    again
;

: old-mansion-b1-6.act11 ( -- )   \ 0041F030
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    begin
        loader-done? while
        yield
    repeat
    8 3 $FF char-load
    3 char-unload
    $37 1.0 1 bgm
    wait-fade
    $19 state-flag-set
    $FF panic-stage? if
        3 panic-stage
    then
    $F state-flag-set
    0 $5D 75.0 -30.0 180 char-to-xz
    1 30.0 -10.0 0.0 4.0 event-camera
    $FF 1 char-visible
    0 0 $14 door-bits
    8 char-activate
    0 4 $20000 nav-group
    8 $196 75.0 -40.0 0 char-to-xz
    1 4 $20000 nav-group
    begin
        0 adx? not while
        yield
    repeat
    $F 1 fade
    8 $1300 0 0 char-anim-hold
    9 0 var-set
    old-mansion-b1-6.cmd04
    self-frames-reset
    $A self-wait-frames
    0 0.0 $FF bgm
    begin
        8 char-at-motion-event? not while
        old-mansion-b1-6.cmd04
        yield
    repeat
    $F 0 fade
    wait-fade
    $1F $7C $74 $78 $5A $44 $46 $52 $4B 0 9 effect-string
    0 1 $14 door-bits
    8 action-end
    8 char-done
    3 0 char-remove
    $FF 0 char-visible
    0 0.0 0.0 0.0 0.0 event-camera
    $F state-flag-clear
    $19 state-flag-clear
    $F $41 fade
    wait-fade
    $B ebit? not if
        $1D01 self-anim
        8 message
        wait-message
        self-wait-anim
        $E4 panic-grow
        fiona-calm-reset
        3 avoid-prompt
        0 $84 5 char-sound
        $B ebit-set
        self-frames-reset
        $1E self-wait-frames
    else
        9 message
        wait-message
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act12 ( -- )   \ 0041F140
    self-wait-done
    12.0 -41.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act13 ( -- )   \ 0041F160
    self-wait-done
    -19.0 -6.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act14 ( -- )   \ 0041F180
    self-wait-done
    -60.0 -110.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act15 ( -- )   \ 0041F1A0
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 45 $3C char-heading? if
        $F00 self-anim
    else 0 -45 $3C char-heading? if
        $F01 self-anim
    else 0 90 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act16 ( -- )   \ 0041F1E0
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 0 $3C char-heading? if
        $F00 self-anim
    else 0 90 $3C char-heading? if
        $F01 self-anim
    else 0 45 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act17 ( -- )   \ 0041F220
    $A ebit-set
    1 self-scripted
    self-wait-done
    0 45 $3C char-heading? if
        $F00 self-anim
    else 0 -45 $3C char-heading? if
        $F01 self-anim
    else 0 90 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    $FE char-here? not if
        self-frames-reset
        self-wait-16
        3 message
    then
    self-wait-anim
    -1 self-move-16
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act18 ( -- )   \ 0041F260
    self-wait-done
    2.06 -99.68 self-turn-to-xz
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
            $271 story-flag-set
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

: old-mansion-b1-6.act19 ( -- )   \ 0041F2C0
    self-wait-done
    -138.94 -114.31 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message
            wait-message
        else
            $2AA story-flag-set
            1 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
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

: old-mansion-b1-6.act1A ( -- )   \ 0041F320
    self-wait-done
    39.98 -40.67 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $75 message-param-room
        $75 $63 item-count? if
            $8010 message
            wait-message
        else
            $2C3 story-flag-set
            2 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
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

: old-mansion-b1-6.act1B ( -- )   \ 0041F380
    self-wait-done
    0 0 old-mansion-b1-6.cmd01
    1 0 old-mansion-b1-6.cmd01
    2 0 old-mansion-b1-6.cmd01
    3 0 old-mansion-b1-6.cmd01
    3 117.5 11.7 -21.8 0 effect-86
    4 -217.0 20.1 20.0 1 effect-86
    5 -175.0 20.1 4.7 1 effect-86
    6 -70.0 20.1 -124.7 1 effect-86
    7 -20.0 20.1 -96.7 1 effect-86
    8 11.7 20.1 -55.8 1 effect-86
    9 43.0 20.1 -26.5 1 effect-86
    $A -33.2 20.1 -6.2 1 effect-86
    $B 89.3 20.1 50.0 1 effect-86
    $C -12.4 28.3 24.6 1 effect-86
    $D -40.0 20.1 95.6 1 effect-86
    $E -65.5 20.1 0.0 1 effect-86
    8 3 $FF char-load
    3 char-unload
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $28 $78 movie-param
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-b1-6.phase5 ( -- )   \ 0047ACC8
;

\ ---- registered ----
' old-mansion-b1-6.enter old-mansion-b1-6 0 room-script!
' old-mansion-b1-6.char-enter old-mansion-b1-6 6 room-script!
' old-mansion-b1-6.phase1 old-mansion-b1-6 1 room-script!
' old-mansion-b1-6.phase2 old-mansion-b1-6 2 room-script!
' old-mansion-b1-6.act00 old-mansion-b1-6 $00 action-script!
' old-mansion-b1-6.act01 old-mansion-b1-6 $01 action-script!
' old-mansion-b1-6.act02 old-mansion-b1-6 $02 action-script!
' old-mansion-b1-6.act03 old-mansion-b1-6 $03 action-script!
' old-mansion-b1-6.act04 old-mansion-b1-6 $04 action-script!
' old-mansion-b1-6.act05 old-mansion-b1-6 $05 action-script!
' old-mansion-b1-6.act06 old-mansion-b1-6 $06 action-script!
' old-mansion-b1-6.act07 old-mansion-b1-6 $07 action-script!
' old-mansion-b1-6.act08 old-mansion-b1-6 $08 action-script!
' old-mansion-b1-6.act09 old-mansion-b1-6 $09 action-script!
' old-mansion-b1-6.act0A old-mansion-b1-6 $0A action-script!
' old-mansion-b1-6.act0B old-mansion-b1-6 $0B action-script!
' old-mansion-b1-6.act0C old-mansion-b1-6 $0C action-script!
' old-mansion-b1-6.act0D old-mansion-b1-6 $0D action-script!
' old-mansion-b1-6.act0E old-mansion-b1-6 $0E action-script!
' old-mansion-b1-6.act0F old-mansion-b1-6 $0F action-script!
' old-mansion-b1-6.act10 old-mansion-b1-6 $10 action-script!
' old-mansion-b1-6.act11 old-mansion-b1-6 $11 action-script!
' old-mansion-b1-6.act12 old-mansion-b1-6 $12 action-script!
' old-mansion-b1-6.act13 old-mansion-b1-6 $13 action-script!
' old-mansion-b1-6.act14 old-mansion-b1-6 $14 action-script!
' old-mansion-b1-6.act15 old-mansion-b1-6 $15 action-script!
' old-mansion-b1-6.act16 old-mansion-b1-6 $16 action-script!
' old-mansion-b1-6.act17 old-mansion-b1-6 $17 action-script!
' old-mansion-b1-6.act18 old-mansion-b1-6 $18 action-script!
' old-mansion-b1-6.act19 old-mansion-b1-6 $19 action-script!
' old-mansion-b1-6.act1A old-mansion-b1-6 $1A action-script!
' old-mansion-b1-6.act1B old-mansion-b1-6 $1B action-script!
' old-mansion-b1-6.phase5 old-mansion-b1-6 5 room-script!
