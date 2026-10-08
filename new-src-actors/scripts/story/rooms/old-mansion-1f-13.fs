\ story/rooms/old-mansion-1f-13.fs - the event scripts of room old-mansion-1f-13 ($52; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-13
USING: room-names story.words story.shared ;

: old-mansion-1f-13.cmd00 ( -- )  s" old-mansion-1f-13.cmd00" stub-step ;
\ 1 unless the object at +0x18 exists and its byte +0x28 is 1.
: old-mansion-1f-13.cond00? ( -- flag )  s" old-mansion-1f-13.cond00?" stub-flag ;

: old-mansion-1f-13.enter ( -- )   \ 0040E3B0
    room-sounds
    $35 1.0 0 bgm
    $4B story-flag? not if
        old-mansion-1f-13.cond00? if
            $52 1 323 2 5 6 0 0.0 creature-place
        then
    then
    old-mansion-1f-13.cmd00
    $75 story-flag? not if
        2 0 object-show
        3 0 object-show
        4 0 object-show
        5 0 object-show
        6 0 object-show
        7 0 object-show
        8 0 object-show
        9 0 object-show
        0 0 $14 door-bits
    else
        2 1 object-show
        3 1 object-show
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        9 1 object-show
        0 1 $14 door-bits
    then
    $10 -54.6 28.6 -20.0 $E $80 $80 $80 $40 specks
    $10 54.6 29.0 -20.0 $E $80 $80 $80 $40 specks
    $10 -7.5 5.0 53.5 $E $80 $80 $80 $40 specks
    $10 0.0 64.8 -20.5 $E $80 $80 $80 $40 specks
    1 0 $10000000 nav-group
;

: old-mansion-1f-13.char-enter ( -- )   \ 0040E480
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
    0 self-is? if
        1 exit-taken? 2 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-13.phase1 ( -- )   \ 0040E550
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    0 3 char-entered-area? if
        1 exit-prepare
    then
    0 4 char-entered-area? if
        2 exit-prepare
    then
    $75 story-flag? not if
        0 5 char-left-area? if
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 char-action? if
                0 0 1 action-force
            else
                1 0 1 action-force
            then
        then
    then
    0 -48.85 0.0 -48.91 $21 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    1 48.42 -6.0 28.0 $1A 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    2 2.46 -6.0 33.29 $16 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    3 -22.19 -6.0 36.99 $14 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
    4 26.75 0.0 -36.99 $E 11 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 11.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-13.phase2 ( -- )   \ 0040E680
    0 6 $3C char-faces-area? if
        5 2 0 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        5 3 0 scene-change
    then
    $76 story-flag? not if
        $18 1 item-count? not $19 1 item-count? not and $1A 1 item-count? not and $1B 1 item-count? not and $1C 1 item-count? not and if
            0 $A $3C char-faces-area? if
                5 4 0 scene-change
            then
            0 9 $3C char-faces-area? if
                5 5 0 scene-change
            then
            0 8 $3C char-faces-area? if
                5 6 0 scene-change
            then
            0 $B $3C char-faces-area? if
                5 7 0 scene-change
            then
            0 7 char-in-area? 0 90 $3C char-heading? and if
                5 8 0 scene-change
            then
        else
            0 $A $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 9 $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 8 $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 $B $3C char-faces-area? if
                5 0 0 scene-change
            then
            0 7 char-in-area? 0 90 $3C char-heading? and if
                5 0 0 scene-change
            then
        then
    else
        0 $A $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 9 $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 8 $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 $B $3C char-faces-area? if
            5 9 0 scene-change
        then
        0 7 char-in-area? 0 90 $3C char-heading? and if
            5 9 0 scene-change
        then
    then
    0 $D char-in-area? 0 -90 $3C char-heading? and if
        5 $D 0 scene-change
    then
    0 $E char-in-area? 0 -90 $3C char-heading? and if
        5 $D 0 scene-change
    then
;

: old-mansion-1f-13.act00 ( -- )   \ 0047ABA0
    self-wait-done
    4 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-13.act01 ( -- )   \ 0040E780
    $75 story-flag-set
    $FE action-end
    $FE char-done
    0 state-flag-clear
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    0 0 door-flag-82
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    1 char-here? 0 hewie-side? and if
        0 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        0 ebit-set
    then
    0 1 char-no-shadow
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
    8 state-flag-set
    0 0 char-no-shadow
    2 1 object-show
    3 1 object-show
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    9 1 object-show
    0 1 $14 door-bits
    0 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $52 0 182 hewie-to-room
            1 $B6 35.67 -1.81 99 char-to-xz
        then
    then
    0 $1C 54.36 -8.26 -43 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    yield
    camera-restart
    $58 story-flag? not if
        $58 story-flag-set
    then
    0 1 door-flag-82
    $F $51 fade
    $35 1.0 0 bgm
    wait-fade
    $18 state-flag-clear
    $12 state-flag-clear
    $FE $6C 0 door-lock-for
    self-idle-or-end
;

: old-mansion-1f-13.act02 ( -- )   \ 0047ABA8
    self-wait-done
    0 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-13.act03 ( -- )   \ 0040E8D0
    1 self-scripted
    $18 state-flag-set
    self-wait-done
    1 message
    wait-message
    $F 6 fade
    wait-fade
    2 message
    wait-message
    $F 7 fade
    wait-fade
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act0A ( -- )   \ 0040EAD0
    $75 story-flag? $76 story-flag? not and if
        $FE $48 1 room-doors-state
        stalker-active? not if
            $FE $4B 217 2 stalker-to-room
            $FE 2 stalker-mode
        then
        stalker-item-cooldown
    then
    exit
;

: old-mansion-1f-13.act04 ( -- )   \ 0040E8F0
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $18 message-param-room
        $18 1 item-give-count
        0 $18 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act05 ( -- )   \ 0040E950
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $19 message-param-room
        $19 1 item-give-count
        0 $19 item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $362 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act06 ( -- )   \ 0040E9B0
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1A message-param-room
        $1A 1 item-give-count
        0 $1A item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $363 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act07 ( -- )   \ 0040EA10
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1B message-param-room
        $1B 1 item-give-count
        0 $1B item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $364 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act08 ( -- )   \ 0040EA70
    $18 state-flag-set
    $13 state-flag-set
    1 self-scripted
    self-wait-done
    3 message
    wait-message
    0 answer? if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $1C message-param-room
        $1C 1 item-give-count
        0 $1C item-tab
        0 $F9 $8C action-force
        $83 $85 0.0 0.0 0.0 0 0 sound
        $8012 message
        wait-message
        $365 story-flag-set
        $3C threat-add
        0 2 6 char-sound
        $903 self-anim
        self-wait-anim
        old-mansion-1f-13.act0A
    then
    $18 state-flag-clear
    $13 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act09 ( -- )   \ 0047ABB0
    self-wait-done
    5 message
    wait-message
    self-idle-or-end
;

: old-mansion-1f-13.act0B ( -- )   \ 0040EAF0
    $18 state-flag-set
    1 self-scripted
    0 counter-set
    0 1 $C action
    self-wait-done
    1 self-turn-to
    self-wait-done
    1 wait-counter
    $C06 self-anim
    self-wait-anim
    0 self-anim
    self-frames-reset
    1 self-wait-frames
    1 self-look-at
    yield
    2 counter-set
    3 wait-counter
    0 self-scripted
    $18 state-flag-clear
    5 hewie-trust
    self-idle-or-end
;

: old-mansion-1f-13.act0C ( -- )   \ 0040EB20
    1 self-scripted
    self-wait-done
    0 self-turn-to
    self-wait-done
    1 counter-set
    2 wait-counter
    hewie-bark
    self-wait-done
    1 0 0 char-camera
    1 camera-follow
    $8D -18.0 29.0 0 $FFFF 5 self-move-to
    self-wait-done
    hewie-bark
    self-wait-done
    self-frames-reset
    $1E self-wait-frames
    0 0 0 char-camera
    0 camera-follow
    3 counter-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-13.act0D ( -- )   \ 0040EB60
    self-wait-done
    -180 self-turn-angle
    self-wait-done
    $A01 self-anim
    self-frames-reset
    $28 self-wait-frames
    6 message
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-13.act0E ( -- )   \ 0040EB80
    self-wait-done
    old-mansion-1f-13.cmd00
    $10 -54.6 28.6 -20.0 $E $80 $80 $80 $40 specks
    $10 54.6 29.0 -20.0 $E $80 $80 $80 $40 specks
    $10 -7.5 5.0 53.5 $E $80 $80 $80 $40 specks
    $10 0.0 64.8 -20.5 $E $80 $80 $80 $40 specks
    0 0 door-flag-82
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 1 char-no-shadow
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
    0 0 char-no-shadow
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-13.enter old-mansion-1f-13 0 room-script!
' old-mansion-1f-13.char-enter old-mansion-1f-13 6 room-script!
' old-mansion-1f-13.phase1 old-mansion-1f-13 1 room-script!
' old-mansion-1f-13.phase2 old-mansion-1f-13 2 room-script!
' old-mansion-1f-13.act00 old-mansion-1f-13 $00 action-script!
' old-mansion-1f-13.act01 old-mansion-1f-13 $01 action-script!
' old-mansion-1f-13.act02 old-mansion-1f-13 $02 action-script!
' old-mansion-1f-13.act03 old-mansion-1f-13 $03 action-script!
' old-mansion-1f-13.act04 old-mansion-1f-13 $04 action-script!
' old-mansion-1f-13.act05 old-mansion-1f-13 $05 action-script!
' old-mansion-1f-13.act06 old-mansion-1f-13 $06 action-script!
' old-mansion-1f-13.act07 old-mansion-1f-13 $07 action-script!
' old-mansion-1f-13.act08 old-mansion-1f-13 $08 action-script!
' old-mansion-1f-13.act09 old-mansion-1f-13 $09 action-script!
' old-mansion-1f-13.act0A old-mansion-1f-13 $0A action-script!
' old-mansion-1f-13.act0B old-mansion-1f-13 $0B action-script!
' old-mansion-1f-13.act0C old-mansion-1f-13 $0C action-script!
' old-mansion-1f-13.act0D old-mansion-1f-13 $0D action-script!
' old-mansion-1f-13.act0E old-mansion-1f-13 $0E action-script!
