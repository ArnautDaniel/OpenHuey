\ story/rooms/old-mansion-1f-15.fs - the event scripts of room old-mansion-1f-15 ($54; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-15
USING: room-names story.words story.shared ;

\ Room54_Cmd00
: old-mansion-1f-15.cmd00 ( b0 b1 -- )  drop drop s" old-mansion-1f-15.cmd00" stub-step ;
\ Room54_Cmd01
: old-mansion-1f-15.cmd01 ( -- )  s" old-mansion-1f-15.cmd01" stub-step ;

: old-mansion-1f-15.enter ( -- )   \ 00426D80
    room-sounds
    0 0 var-set
    1 0 var-set
    $63 story-flag? not if
        1 1 $14 door-bits
    then
    $64 story-flag? not if
        2 1 $14 door-bits
    then
    $65 story-flag? not if
        0 1 $14 door-bits
    then
    $66 story-flag? not if
        1 4 $20000 nav-group
        1 5 $1000000 nav-group
        8 1 object-show
    else
        1 5 $20000 nav-group
        2 0 old-mansion-1f-15.cmd00
    then
    $67 story-flag? if
        $B 1 $14 door-bits
    then
    $68 story-flag? if
        $A 1 $14 door-bits
    then
    $69 story-flag? if
        9 1 $14 door-bits
    then
    $6A story-flag? not if
        1 2 $20000 nav-group
        1 3 $1000000 nav-group
        1 $18A $10000000 nav-tri-flags
        1 $1D3 $10000000 nav-tri-flags
        4 1 object-show
    else
        1 3 $20000 nav-group
        1 $187 $10000000 nav-tri-flags
        1 $1D0 $10000000 nav-tri-flags
        1 $188 $10000000 nav-tri-flags
        1 $1D1 $10000000 nav-tri-flags
        1 0 old-mansion-1f-15.cmd00
    then
    $6B story-flag? if
        8 1 $14 door-bits
    then
    $6C story-flag? if
        6 1 $14 door-bits
    then
    $6D story-flag? if
        7 1 $14 door-bits
    then
    $6E story-flag? not if
        1 0 $20000 nav-group
        1 1 $1000000 nav-group
        0 1 object-show
        0 6 $200 nav-group
        old-mansion-1f-15.cmd01
    else
        1 1 $20000 nav-group
        0 0 old-mansion-1f-15.cmd00
    then
    $62 story-flag? not if
        0 0 0 $69 $12A obstacle-place
    else
        0 0 0 $87 $148 obstacle-place
    then
    1 7 $10000000 nav-group
    $273 story-flag? not if
        0 36.86 51.0 -27.91 flicker-sprite
    then
    $2A7 story-flag? $2A8 story-flag? not and if
        1 39.16 1.0 -38.76 flicker-sprite
    then
    $2B1 story-flag? $2B2 story-flag? not and if
        2 -23.61 51.0 -61.37 flicker-sprite
    then
;

: old-mansion-1f-15.char-enter ( -- )   \ 00426EF0
    hewie-controlled? not if
        0 self-is? 1 exit-taken? and if
            0 1 char-to-exit
            hewie-controlled? not if
                0 1 -1 char-camera
                0 camera-follow
            else
                1 1 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 1 exit-taken? and if
        1 1 char-to-exit
        hewie-controlled? not if
            0 1 -1 char-camera
            0 camera-follow
        else
            1 1 -1 char-camera
            1 camera-follow
        then
    then then
    1 1 -1 area-camera
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
        0 self-is? 2 exit-taken? and if
            0 2 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 2 exit-taken? and if
        1 2 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    2 0 -1 area-camera
    hewie-controlled? not if
        0 self-is? 3 exit-taken? and if
            0 3 char-to-exit
            hewie-controlled? not if
                0 0 -1 char-camera
                0 camera-follow
            else
                1 0 -1 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 3 exit-taken? and if
        1 3 char-to-exit
        hewie-controlled? not if
            0 0 -1 char-camera
            0 camera-follow
        else
            1 0 -1 char-camera
            1 camera-follow
        then
    then then
    3 0 -1 area-camera
    0 self-is? if
        2 exit-taken? 3 exit-taken? or if
            3 map-page
        then
    then
;

: old-mansion-1f-15.phase1 ( -- )   \ 00426FF0
    1 exit-usable? if
        1 exit-check
    then
    0 exit-usable? if
        0 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    $24 0 -1 1 chars-area-camera
    $25 1 -1 1 chars-area-camera
    $26 1 -1 -1 chars-area-camera
    $27 2 0 1 chars-area-camera
    0 4 char-entered-area? if
        1 exit-prepare
    then
    0 5 char-entered-area? if
        0 exit-prepare
    then
    0 6 char-entered-area? if
        2 exit-prepare
    then
    0 7 char-entered-area? if
        3 exit-prepare
    then
    0 $148 obstacle-on? if
        $62 story-flag-set
    else
        $62 story-flag-clear
    then
    $2A7 story-flag? not if
        1 39.16 0.0 -38.76 $A 5 0 zone
        1 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 679 var-set
                $1A 680 var-set
                $1B 1 var-set
                $1C 39160 var-set
                $1D 1000 var-set
                $1E -38760 var-set
                $1F 1 var-set
                0 1 $8B action
            then
        then
    then
    $2B1 story-flag? not if
        2 -23.61 50.0 -61.37 $A 5 0 zone
        1 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 689 var-set
                $1A 690 var-set
                $1B 2 var-set
                $1C -23610 var-set
                $1D 51000 var-set
                $1E -61370 var-set
                $1F 2 var-set
                0 1 $8B action
            then
        then
    then
    $64 story-flag? not if
        3 -31.67 0.0 -62.5 $B 10 0 zone
        2 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 3 9 char-zone-bits? if
                        2 ebit-set
                        $64 chance? if
                            $1F 3 var-set
                            0 1 $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        3 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        3 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $89 action
                        then
                    then
                then
            then
        then
        4 -15.49 0.0 -61.46 $B 10 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 4 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 4 var-set
                            0 1 $88 action
                        then
                    then
                then
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000003 6 24.15 0.0 76.38 0 0 sound
                $40000008 6 -44.0 70.0 22.0 0 0 sound
            else 0 1 var? if
                $40000004 6 24.15 0.0 76.38 0 0 sound
                $40000009 6 -44.0 70.0 22.0 0 0 sound
            else 0 2 var? if
                $40000005 6 24.15 0.0 76.38 0 0 sound
                $4000000A 6 -44.0 70.0 22.0 0 0 sound
            else 0 3 var? if
                $40000006 6 24.15 0.0 76.38 0 0 sound
                $4000000B 6 -44.0 70.0 22.0 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
            then
        then
    then
;

: old-mansion-1f-15.phase2 ( -- )   \ 004272A0
    $63 story-flag? not if
        0 9 $32 char-faces-area? if
            5 9 4 scene-change
        then
    then
    $64 story-flag? not if
        0 $B char-in-area? $62 story-flag? and if
            5 $A 4 scene-change
        then
    then
    $65 story-flag? not if
        0 $A char-in-area? 0 90 $32 char-heading? and if
            5 $B 4 scene-change
        then
    then
    $66 story-flag? not if
        0 $D char-in-area? 0 -45 $32 char-heading? and if
            $67 story-flag? $68 story-flag? or if
                5 $C 4 scene-change
            else
                5 $C 0 scene-change
            then
        then
        0 $C char-in-area? 0 45 $32 char-heading? and if
            5 $10 0 scene-change
        then
    else
        0 $10 char-in-area? 0 -45 $32 char-heading? and if
            5 $F 0 scene-change
        then
        0 $F char-in-area? 0 45 $32 char-heading? and if
            5 $10 0 scene-change
        then
    then
    $6A story-flag? not if
        0 $11 char-in-area? 0 45 $32 char-heading? and if
            $69 story-flag? $6B story-flag? or if
                5 $D 4 scene-change
            else
                5 $D 0 scene-change
            then
        then
    else 0 $13 char-in-area? 0 45 $32 char-heading? and if
        5 $11 0 scene-change
    then then
    $6E story-flag? not if
        0 $14 char-in-area? 0 90 $32 char-heading? and if
            $6C story-flag? $6D story-flag? or if
                5 $E 4 scene-change
            else
                5 $E 0 scene-change
            then
        then
        0 $15 char-in-area? 0 0 $32 char-heading? and if
            5 $13 0 scene-change
        then
    else
        0 $17 char-in-area? 0 90 $32 char-heading? and if
            5 $12 0 scene-change
        then
        0 $18 char-in-area? 0 0 $32 char-heading? and if
            5 $13 0 scene-change
        then
    then
    0 $19 char-in-area? 0 90 $32 char-heading? and if
        5 $12 0 scene-change
    then
    0 $1B char-in-area? 0 0 $32 char-heading? and if
        5 $13 0 scene-change
    then
    0 $1C char-in-area? 0 90 $32 char-heading? and if
        5 $14 0 scene-change
    then
    0 $1D char-in-area? 0 0 $32 char-heading? and if
        5 $15 0 scene-change
    then
    0 $20 char-in-area? 0 -45 $32 char-heading? and if
        5 $16 0 scene-change
    then
    0 $21 char-in-area? 0 $22 char-in-area? or 0 45 $32 char-heading? and if
        5 $17 0 scene-change
    then
    0 $1E char-in-area? 0 90 $32 char-heading? and if
        5 $18 0 scene-change
    then
    0 $1F char-in-area? 0 0 $32 char-heading? and if
        5 $19 0 scene-change
    then
    $66 story-flag? not if
        0 $1A char-in-area? 0 90 $32 char-heading? and if
            5 $1A 0 scene-change
        then
    then
    0 8 char-in-area? 0 22 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $23 $32 char-faces-area? if
        5 $84 3 scene-change
    then
    $273 story-flag? not if
        0 0 5 5 0 zone-at-effect
        0 0 3 char-zone-bits? if
            5 $1B 4 scene-change
        then
    then
    $2A7 story-flag? $2A8 story-flag? not and if
        1 1 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 $1C 4 scene-change
        then
    then
    $2B1 story-flag? $2B2 story-flag? not and if
        2 2 5 5 0 zone-at-effect
        0 2 3 char-zone-bits? if
            5 $1D 4 scene-change
        then
    then
;

: old-mansion-1f-15.act00 ( -- )   \ 00427490
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    1 char-here? 0 hewie-side? and if
        4 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    8 0 object-show
    $66 story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    0 ebit-set
    2 3 old-mansion-1f-15.cmd00
    begin
        2 1 old-mansion-1f-15.cmd00
        2 4 old-mansion-1f-15.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 0.0 -52.5 10.0 0 0 sound
    0 4 $20000 nav-group
    1 5 $20000 nav-group
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $54 0 221 hewie-to-room
            1 $DD -27.0 37.0 60 char-to-xz
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act01 ( -- )   \ 00427570
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    $B 1 $14 door-bits
    $67 story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act02 ( -- )   \ 004275E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    $A 1 $14 door-bits
    $68 story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act03 ( -- )   \ 00427650
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    9 1 $14 door-bits
    $69 story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act04 ( -- )   \ 004276C0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    3 5 old-mansion-1f-15.cmd00
    0 $18A $10000000 nav-tri-flags
    0 $1D3 $10000000 nav-tri-flags
    1 $187 $10000000 nav-tri-flags
    1 $1D0 $10000000 nav-tri-flags
    1 $188 $10000000 nav-tri-flags
    1 $1D1 $10000000 nav-tri-flags
    1 25.0 20.0 50.0 2.0 event-camera
    1 char-here? 0 hewie-side? and if
        4 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    4 0 object-show
    $6A story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    40.0 15.0 25.0 self-look-at-point
    yield
    0 ebit-set
    1 3 old-mansion-1f-15.cmd00
    begin
        1 2 old-mansion-1f-15.cmd00
        1 4 old-mansion-1f-15.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 39.0 21.0 10.0 0 0 sound
    0 2 $20000 nav-group
    1 3 $20000 nav-group
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $54 0 22 hewie-to-room
            1 $16 -41.0 -4.0 -100 char-to-xz
        then
    then
    $FF self-look-at
    yield
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act05 ( -- )   \ 004277F0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    8 1 $14 door-bits
    $6B story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act06 ( -- )   \ 00427860
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    6 1 $14 door-bits
    $6C story-flag-set
    $15 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act07 ( -- )   \ 004278D0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    7 1 $14 door-bits
    $6D story-flag-set
    $16 item-use
    $905 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act08 ( -- )   \ 00427940
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    1 char-here? 0 hewie-side? and if
        4 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        4 ebit-set
    then
    $904 self-anim
    self-wait-anim
    0 $E 6 char-sound
    0 0 object-show
    $6E story-flag-set
    $17 item-use
    $905 self-anim
    self-wait-anim
    0 ebit-set
    0 3 old-mansion-1f-15.cmd00
    begin
        0 2 old-mansion-1f-15.cmd00
        0 4 old-mansion-1f-15.cmd00
        0 ebit? while
        yield
    repeat
    $C 6 sound-stop
    $D 6 0.0 0.0 10.0 0 0 sound
    0 0 $20000 nav-group
    1 1 $20000 nav-group
    1 6 $200 nav-group
    old-mansion-1f-15.cmd01
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    4 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $54 0 255 hewie-to-room
            1 $FF 38.0 -20.0 90 char-to-xz
        then
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act09 ( -- )   \ 00427A30
    self-wait-done
    -35.0 66.0 self-turn-to-xz
    self-wait-done
    0 message
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    1 0 $14 door-bits
    $63 story-flag-set
    $15 message-param-room
    $15 1 item-give-count
    0 $15 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-15.act0A ( -- )   \ 00427A80
    self-wait-done
    -25.0 -60.0 self-turn-to-xz
    self-wait-done
    1 message
    wait-message
    $900 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    2 0 $14 door-bits
    $64 story-flag-set
    $16 message-param-room
    $16 1 item-give-count
    0 $16 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $901 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-15.act0B ( -- )   \ 00427AD0
    self-wait-done
    1.0 -30.0 self-turn-to-xz
    self-wait-done
    2 message
    wait-message
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    0 0 $14 door-bits
    $65 story-flag-set
    $17 message-param-room
    $17 1 item-give-count
    0 $17 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-15.act0C ( -- )   \ 00427B20
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $36 35.0 -52.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 -50.0 2.0 event-camera
    $67 story-flag? $68 story-flag? or if
        $904 self-anim
        self-wait-anim
        $67 story-flag? if
            $B 0 $14 door-bits
            $67 story-flag-clear
            $905 self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            $A 0 $14 door-bits
            $68 story-flag-clear
            $905 self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $A message
        wait-message
        self-frames-reset
        4 self-wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act0D ( -- )   \ 00427BF0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C5 30.0 49.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 25.0 20.0 50.0 2.0 event-camera
    $69 story-flag? $6B story-flag? or if
        $904 self-anim
        self-wait-anim
        $69 story-flag? if
            9 0 $14 door-bits
            $69 story-flag-clear
            $905 self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            8 0 $14 door-bits
            $6B story-flag-clear
            $905 self-anim
            $17 message-param-room
            $17 1 item-give-count
            0 $17 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $B message
        wait-message
        self-frames-reset
        4 self-wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act0E ( -- )   \ 00427CC0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $C6 0.0 43.0 180 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 0.0 0.0 2.0 event-camera
    $6C story-flag? $6D story-flag? or if
        $904 self-anim
        self-wait-anim
        $6C story-flag? if
            6 0 $14 door-bits
            $6C story-flag-clear
            $905 self-anim
            $15 message-param-room
            $15 1 item-give-count
            0 $15 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        else
            7 0 $14 door-bits
            $6D story-flag-clear
            $905 self-anim
            $16 message-param-room
            $16 1 item-give-count
            0 $16 item-tab
            0 $F9 $8C action-force
            $83 $85 0.0 0.0 0.0 0 0 sound
            $8012 message
            wait-message
        then
        self-frames-reset
        self-wait-16
        self-frames-reset
        self-wait-16
        self-wait-anim
    else
        $C message
        wait-message
        self-frames-reset
        4 self-wait-frames
    then
    0 0.0 0.0 0.0 0.0 event-camera
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-15.act0F ( -- )   \ 00427D88
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act10 ( -- )   \ 00427D98
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act11 ( -- )   \ 00427DA8
    self-wait-done
    90 self-turn-angle
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act12 ( -- )   \ 00427DB8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act13 ( -- )   \ 00427DC8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act14 ( -- )   \ 00427DD8
    self-wait-done
    180 self-turn-angle
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act15 ( -- )   \ 00427DE8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    6 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act16 ( -- )   \ 00427DF8
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act17 ( -- )   \ 00427E08
    self-wait-done
    90 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act18 ( -- )   \ 00427E18
    self-wait-done
    180 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act19 ( -- )   \ 00427E28
    self-wait-done
    0 self-turn-angle
    self-wait-done
    7 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-15.act1A ( -- )   \ 00427E40
    self-wait-done
    1 ebit? not if
        180 self-turn-angle
        self-wait-done
        8 message
        wait-message
        1 ebit-set
    else
        0.0 -80.0 self-turn-to-xz
        self-wait-done
        $A00 self-anim
        self-frames-reset
        self-wait-16
        9 message
        wait-message
        1 ebit-clear
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-15.act1B ( -- )   \ 00427E70
    self-wait-done
    36.86 -27.91 self-turn-to-xz
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
            $273 story-flag-set
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

: old-mansion-1f-15.act1C ( -- )   \ 00427ED0
    self-wait-done
    39.16 -38.76 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $97 message-param-room
        $97 $63 item-count? if
            $8010 message
            wait-message
        else
            $2A8 story-flag-set
            1 effect-remove
            $97 1 item-give-count
            0 $97 item-tab
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

: old-mansion-1f-15.act1D ( -- )   \ 00427F30
    self-wait-done
    -23.61 -61.37 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 message
            wait-message
        else
            $2B2 story-flag-set
            2 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
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

\ ---- registered ----
' old-mansion-1f-15.enter old-mansion-1f-15 0 room-script!
' old-mansion-1f-15.char-enter old-mansion-1f-15 6 room-script!
' old-mansion-1f-15.phase1 old-mansion-1f-15 1 room-script!
' old-mansion-1f-15.phase2 old-mansion-1f-15 2 room-script!
' old-mansion-1f-15.act00 old-mansion-1f-15 $00 action-script!
' old-mansion-1f-15.act01 old-mansion-1f-15 $01 action-script!
' old-mansion-1f-15.act02 old-mansion-1f-15 $02 action-script!
' old-mansion-1f-15.act03 old-mansion-1f-15 $03 action-script!
' old-mansion-1f-15.act04 old-mansion-1f-15 $04 action-script!
' old-mansion-1f-15.act05 old-mansion-1f-15 $05 action-script!
' old-mansion-1f-15.act06 old-mansion-1f-15 $06 action-script!
' old-mansion-1f-15.act07 old-mansion-1f-15 $07 action-script!
' old-mansion-1f-15.act08 old-mansion-1f-15 $08 action-script!
' old-mansion-1f-15.act09 old-mansion-1f-15 $09 action-script!
' old-mansion-1f-15.act0A old-mansion-1f-15 $0A action-script!
' old-mansion-1f-15.act0B old-mansion-1f-15 $0B action-script!
' old-mansion-1f-15.act0C old-mansion-1f-15 $0C action-script!
' old-mansion-1f-15.act0D old-mansion-1f-15 $0D action-script!
' old-mansion-1f-15.act0E old-mansion-1f-15 $0E action-script!
' old-mansion-1f-15.act0F old-mansion-1f-15 $0F action-script!
' old-mansion-1f-15.act10 old-mansion-1f-15 $10 action-script!
' old-mansion-1f-15.act11 old-mansion-1f-15 $11 action-script!
' old-mansion-1f-15.act12 old-mansion-1f-15 $12 action-script!
' old-mansion-1f-15.act13 old-mansion-1f-15 $13 action-script!
' old-mansion-1f-15.act14 old-mansion-1f-15 $14 action-script!
' old-mansion-1f-15.act15 old-mansion-1f-15 $15 action-script!
' old-mansion-1f-15.act16 old-mansion-1f-15 $16 action-script!
' old-mansion-1f-15.act17 old-mansion-1f-15 $17 action-script!
' old-mansion-1f-15.act18 old-mansion-1f-15 $18 action-script!
' old-mansion-1f-15.act19 old-mansion-1f-15 $19 action-script!
' old-mansion-1f-15.act1A old-mansion-1f-15 $1A action-script!
' old-mansion-1f-15.act1B old-mansion-1f-15 $1B action-script!
' old-mansion-1f-15.act1C old-mansion-1f-15 $1C action-script!
' old-mansion-1f-15.act1D old-mansion-1f-15 $1D action-script!
