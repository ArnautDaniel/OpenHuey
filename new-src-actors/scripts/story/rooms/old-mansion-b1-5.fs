\ story/rooms/old-mansion-b1-5.fs - the event scripts of room old-mansion-b1-5 ($51; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-5
USING: room-names story.words story.shared ;

\ room 0x51 (Room51_Cmd00_ptmf): byte 3 0 door 0 set going (+0xC); else wait (2) while it moves
: old-mansion-b1-5.cmd00 ( b0 -- )  drop s" old-mansion-b1-5.cmd00" stub-step ;
: old-mansion-b1-5.cmd01 ( -- )  s" old-mansion-b1-5.cmd01" stub-step ;

: old-mansion-b1-5.enter ( -- )   \ 0040CF40
    room-sounds
    1 0 $1000000 nav-group
    1 0 $4000000 nav-group
    $327 story-flag? if
        0 1 $14 door-bits
        1 0 $20000 nav-group
        8 1 object-show
        9 1 object-show
    else
        0 0 $14 door-bits
        0 0 $20000 nav-group
        8 0 object-show
        9 0 object-show
    then
    $328 story-flag? not $327 story-flag? not and if
        3 stalker-kind? $22 stalker-kind? or if
            stalker-active? not if
                $75 story-flag? not $76 story-flag? or if
                    7 ebit-set
                    7 state-flag-set
                then
            then
        then
    then
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.5 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
    old-mansion-b1-5.cmd01
;

: old-mansion-b1-5.act0E ( -- )   \ 0040DC70
    4 ebit-set
    3 self-is? $22 self-is? or if
        $FE char-file-use
    then
    $FE camera-follow
    2 0 var-set
    $12 0 pvar? if
        $A chance? if
            2 2 var-set
        then
    else $12 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $12 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else $12 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    $FE 2 stalker-mode
    3 self-is? $22 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    2 0 var? if
        $FE 0 stalker-search-delay
        $78 1 item-cooldown
    else 2 1 var? if
        $FE 150 stalker-search-delay
        0 $FE $F action
    else 2 2 var? if
        $FE 300 stalker-search-delay
        0 $FE $10 action
    then then then
    3 0 var-set
    $12 pvar-inc
    exit
;

: old-mansion-b1-5.char-enter ( -- )   \ 0040D000
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
    $FE self-is? if
        7 ebit-clear
        7 state-flag-clear
        9 state-flag? if
            4 ebit-set
            old-mansion-b1-5.act0E
        else
            4 ebit-clear
        then
    then
;

: old-mansion-b1-5.phase1 ( -- )   \ 0040D060
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 2 2 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    $328 story-flag? not $327 story-flag? not and if
        0 17.96 0.0 -27.69 $F 18 0 zone
        5 ebit? not 1 char-here? and 1 0 char-C4? and if
            2 game-mode? not if
                hewie-can-command? if
                    1 0 9 char-zone-bits? if
                        5 ebit-set
                        $64 chance? if
                            $1F 0 var-set
                            0 1 $8A action
                        then
                    then
                then
            then
        then
        0 17.96 0.0 -27.69 $F 18 0 zone
        1 char-here? 2 game-mode? not and if
            hewie-can-command? if
                1 0 9 char-zone-bits? if
                    $1F 0 var-set
                    $1F 18.0 hewie-look-zone
                then
            then
        then
    then
    1 1.58 0.0 -9.28 $10 8 0 zone
    6 ebit? not 1 char-here? and 1 0 char-C4? and if
        2 game-mode? not if
            hewie-can-command? if
                1 1 9 char-zone-bits? if
                    6 ebit-set
                    $64 chance? if
                        $1F 1 var-set
                        0 1 $92 action
                    then
                then
            then
        then
    then
    2 17.16 0.0 30.67 $16 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    3 -33.99 0.0 32.91 $16 18 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 3 9 char-zone-bits? if
                $1F 3 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    4 15.24 0.0 -0.28 $11 14 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 4 9 char-zone-bits? if
                $1F 4 var-set
                $1F 14.0 hewie-look-zone
            then
        then
    then
    8 ebit? not if
        6 sound-bank-loaded? if
            $40000003 6 -32.0 0.0 21.0 0 0 sound
            8 ebit-set
        then
    else
        $C0000003 6 -32.0 0.0 21.0 0 0 sound
    then
;

: old-mansion-b1-5.phase2 ( -- )   \ 0040D1F0
    0 5 char-in-area? 0 0 $32 char-heading? and if
        5 0 0 scene-change
    then
    0 6 char-in-area? 0 45 $32 char-heading? and if
        5 1 0 scene-change
    then
    0 7 char-in-area? 0 -45 $32 char-heading? and if
        5 2 0 scene-change
    then
    0 8 char-in-area? 0 -45 $32 char-heading? and if
        5 3 0 scene-change
    then
    0 9 char-in-area? 0 -45 $32 char-heading? and if
        5 4 0 scene-change
    then
    0 $A char-in-area? 0 -45 $32 char-heading? and if
        5 5 0 scene-change
    then
    0 $B char-in-area? 0 0 $32 char-heading? and if
        5 6 0 scene-change
    then
    -2147483646 scene-request? if
        5 7 1 scene-change
    then
    0 $C char-in-area? 0 90 $3C char-heading? and if
        $FE char-here? not if
            7 ebit? $52 door-locked? not and if
                5 $11 5 scene-change
            else
                5 9 5 scene-change
            then
        else
            $8016 scene-ending
        then
    then
;

: old-mansion-b1-5.act00 ( -- )   \ 0040D290
    $43 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-set
        0 $5F 18.0 25.5 0 char-to-xz
        1 15.0 10.0 0.0 0.0 event-camera
        self-frames-reset
        self-wait-16
        2 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
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
            wait-fade
            $17 state-flag-clear
            1 char-here? if
                9 ebit-clear
                1 2 char-C4? if
                    0 1 $86 action
                else
                    1 action-end
                    1 char-done
                then
            else
                9 ebit-set
            then
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
            things-clear
            0 self-move-16
            0 $27 15.929 21.234 -2 char-to-xz
            camera-restart
            9 ebit? not if
                1 2 char-C4? if
                    1 0 char-visible
                    1 action-end
                else
                    1 char-activate
                    $51 0 86 hewie-to-room
                    1 $56 1.98 5.93 -2 char-to-xz
                then
            then
            $4D $68 door-copy
            $68 door-close-off-lock
            $4E $69 door-copy
            $69 door-close-off-lock
            $50 $6A door-copy
            $6A door-close-off-lock
            $53 $6B door-copy
            $6B door-close-off-lock
            exits-rebuild
            1 $4D char-in-room? if
                $68 0 -1 hewie-to-room
            then
            $FE char-here? not stalker-active? and $FE 2 char-C4? not and if
                $FE $4B -1 2 stalker-to-room
            then
            $43 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $F $41 fade
            wait-fade
            1 message
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-b1-5.act01 ( -- )   \ 0040D450
    $44 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-set
        0 $1E 12.5 3.0 90 char-to-xz
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
        1 1.0 45.0 0.0 5.0 event-camera
        self-frames-reset
        self-wait-16
        3 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
            3 0 movie-play
            2 cutscene-start
            yield
            yield
            2 cutscene-control
            begin
                3 cutscene-control
                2 cutscene-mode? not while
                yield
            repeat
            wait-fade
            1 char-here? 1 2 char-C4? and if
                0 1 $86 action
            then
            $17 state-flag-clear
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
            0 self-move-16
            0 $55 10.203 -1.688 57 char-to-xz
            hewie-controlled? not if
                0 0 0 char-camera
                0 camera-follow
            else
                1 0 0 char-camera
                1 camera-follow
            then
            camera-restart
            1 0 char-visible
            1 action-end
            $58 $82 door-copy
            $82 door-close-off-lock
            $5A $83 door-copy
            $83 door-close-off-lock
            exits-rebuild
            1 $5F 0 room-doors-state
            $FE $5F 0 room-doors-state
            $44 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $2B resident-flag-set
            $F $41 fade
            wait-fade
            1 message
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-b1-5.act02 ( -- )   \ 0040D5E0
    $45 story-flag? not if
        $18 state-flag-set
        1 self-scripted
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        $17 state-flag-set
        0 6 -30.0 33.5 -90 char-to-xz
        1 10.0 5.0 0.0 2.0 event-camera
        self-frames-reset
        self-wait-16
        4 message
        wait-message
        self-frames-reset
        8 self-wait-frames
        0 answer? if
            $F $44 fade
            5 1 movie-play
            4 cutscene-start
            yield
            yield
            2 cutscene-control
            begin
                3 cutscene-control
                2 cutscene-mode? not while
                yield
            repeat
            wait-fade
            1 char-here? 1 2 char-C4? and if
                0 1 $86 action
            then
            $17 state-flag-clear
            things-clear
            $10 $78 movie-param
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
            0 self-move-16
            0 6 -24.369 32.751 -92 char-to-xz
            camera-restart
            1 0 char-visible
            1 action-end
            $78 door-unlock
            $5C $88 door-copy
            $5C door-reopen-unlock
            $88 door-close-off-lock
            exits-rebuild
            $FE char-here? not stalker-active? and $FE 2 char-C4? not and if
                $FE $4B -1 2 stalker-to-room
            then
            $45 story-flag-set
            0 0.0 0.0 0.0 0.0 event-camera
            $F $41 fade
            wait-fade
            1 message
            wait-message
            $52 door-locked? if
                self-frames-reset
                self-wait-16
                $72 5 0.0 0.0 -40.0 0 0 sound
                $52 door-unlock
            then
        else
            $17 state-flag-clear
            0 0.0 0.0 0.0 0.0 event-camera
        then
        $18 state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0 message
        wait-message
    then
    self-idle-or-end
;

: old-mansion-b1-5.act08 ( -- )   \ 0040D8E0
    self-frames-reset
    8 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    $17 state-flag-clear
    0 answer? if
        $902 self-anim
        0 exit-door-open? if
            0 old-mansion-b1-5.cmd00
            self-wait-anim
            0.0 10.0 -40.0 self-look-at-point
            yield
            $903 self-anim
            1 old-mansion-b1-5.cmd00
            self-frames-reset
            self-wait-16
        else
            self-wait-anim
            0.0 10.0 -40.0 self-look-at-point
            yield
            $903 self-anim
        then
        $72 5 0.0 0.0 -40.0 0 0 sound
        $52 door-open-clear
        doors-room-in
        $52 door-lock
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        self-wait-16
        $FF self-look-at
        yield
    then
    $18 state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act03 ( -- )   \ 0040D750
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 $71 -13.0 -21.0 -90 char-to-xz
    1 15.0 -5.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    5 message
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act04 ( -- )   \ 0040D790
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 $55 -1.0 -3.0 -90 char-to-xz
    hewie-controlled? not if
        0 0 0 char-camera
        0 camera-follow
    else
        1 0 0 char-camera
        1 camera-follow
    then
    1 5.0 20.0 0.0 5.0 event-camera
    self-frames-reset
    self-wait-16
    6 message
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act05 ( -- )   \ 0040D7E0
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 7 -28.0 20.0 -90 char-to-xz
    1 15.0 10.0 0.0 2.0 event-camera
    self-frames-reset
    self-wait-16
    7 message
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act06 ( -- )   \ 0040D820
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    $17 state-flag-set
    0 $19 -2.0 24.0 0 char-to-xz
    1 20.0 20.0 0.0 1.0 event-camera
    self-frames-reset
    self-wait-16
    8 message
    wait-message
    ['] old-mansion-b1-5.act08 goto
;

: old-mansion-b1-5.act07 ( -- )   \ 0040D860
    $2C state-flag-set
    $19 state-flag-set
    $18 state-flag-set
    1 self-scripted
    self-wait-done
    $52 self-through-door
    self-wait-done
    $F $44 fade
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    wait-fade
    1 action-end
    1 char-done
    $FE action-end
    $FE char-done
    0 $F9 $13 action
    $10 $28 movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        movie-loop
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        $F 1 fade
        $A cutscene-control
        begin
            7 cutscene-control
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
        until
    then
    $2C resident-flag-set
    0 0 char-no-shadow
    0 game-over-flag
    $C state-flag-set
    self-idle-or-end
;

: old-mansion-b1-5.act0A ( -- )   \ 0040DB20
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    0 camera-follow
    9 state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    $327 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act0B ( -- )   \ 0040DB50
    1 self-scripted
    counter-inc
    0 camera-follow
    9 state-flag-clear
    $FE char-here? if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    8 2 object-anim
    9 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    0 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    0 1 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $327 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act0C ( -- )   \ 0040DBA0
    1 self-scripted
    counter-inc
    1 ebit-set
    0 camera-follow
    9 state-flag-clear
    $FE char-here? 0 0 var? and if
        $FE 0 stalker-mode
        $FE action-end
    then
    1 self-noclip
    8 4 object-anim
    9 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    0 self-noclip
    $FE char-here? if
        $FE self-look-at
        yield
        $FE self-turn-to
        self-wait-done
    then
    $327 story-flag-set
    1 0 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act09 ( -- )   \ 0040D970
    $18 state-flag-set
    4 ebit-clear
    7 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    $327 story-flag? not if
        0 ebit-set
    else
        0 ebit-clear
    then
    0 counter-set
    2 ebit-clear
    3 ebit-clear
    1 ebit-clear
    1 char-busy? not hewie-near-command? and 0 1 30 chars-within? and if
        0 1 $D action-force
        2 ebit-set
        1 $B char-file-load
    then
    0 $A char-file-load
    $FE $C char-file-load
    $7B 16.59 -25.02 180 $FFFF 5 self-move-to
    self-wait-done
    0 char-file-use
    2 ebit? if
        1 self-look-at
        yield
        begin
            3 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    $FF self-look-at
    yield
    0 ebit? not if
        $8000 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        8 0 object-show
        9 0 object-show
        8 0 object-anim
        9 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        8 0 object-show
        9 0 object-show
        8 1 object-anim
        9 1 object-anim
        self-frames-reset
        $E self-wait-frames
        0 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        0 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 0 $20000 nav-group
    $18 state-flag-clear
    9 state-flag-set
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        0 2 6 char-sound
    then
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                0 $43 5 char-sound
                $FE char-here? if
                    ['] old-mansion-b1-5.act0A goto
                else
                    ['] old-mansion-b1-5.act0B goto
                then
            else $FF panic-stage? if
                ['] old-mansion-b1-5.act0C goto
            then then
            2 panic-grow
            6 fiona-calm
            $1E fiona-recovery-lower
            4 ebit? $FE char-here? not and if
                4 ebit-clear
                1 avoid-prompt
            then
            yield
        else
            $FE char-here? 0 $FE 50 chars-within? and if
                ['] old-mansion-b1-5.act0C goto
            then
            counter-inc
            0 camera-follow
            9 state-flag-clear
            $FE char-here? if
                $FE 0 stalker-mode
                $FE action-end
            then
            1 self-noclip
            8 2 object-anim
            9 2 object-anim
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            0 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            0 1 6 char-sound
            self-wait-anim
            0 self-noclip
            $FE char-here? if
                $FE self-look-at
                yield
                $FE self-turn-to
                self-wait-done
            then
            $327 story-flag-clear
            0 0 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-b1-5.act0D ( -- )   \ 0040DBF0
    1 self-scripted
    self-wait-done
    $7D 11.91 -21.5 180 $FFFF $A self-move-to
    self-wait-done
    1 char-file-use
    3 ebit-set
    1 wait-counter
    1 self-noclip
    0 ebit? not if
        $8000 5 self-anim-9
        self-wait-anim
    else
        $8001 5 self-anim-9
        self-wait-anim
    then
    0 self-noclip
    $B state-flag-set
    begin
        0 char-busy? if
            2 counter? not if
                yield
            else
                $B state-flag-clear
                1 self-noclip
                1 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    0 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    $8002 0 self-anim-blend
                    self-wait-anim
                else
                    $8003 0 self-anim-blend
                    self-wait-anim
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            $B state-flag-clear
            1 self-noclip
            1 ebit? if
                $8004 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                0 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                $8002 0 self-anim-blend
                self-wait-anim
            else
                $8003 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-b1-5.act0F ( -- )   \ 0040DD40
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    $7B 14.14 -24.14 180 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $19 self-wait-frames
    $FE 5 6 char-sound
    $28 threat-raise
    self-frames-reset
    9 self-wait-frames
    $FE 5 6 char-sound
    $14 threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 item-cooldown
    self-idle-or-end
;

: old-mansion-b1-5.act10 ( -- )   \ 0040DDB0
    self-wait-done
    $FE 0 char-heading-for? 0 exit-door-open? not and if
        0 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        3 0 var? if
            $7B 14.14 -24.14 180 $FFFF 5 self-move-to
            self-wait-done
        then
        3 0 var-set
    else
        $1D 9.84 -11.27 153 $FFFF 5 self-move-to
        self-wait-done
    then
    1 self-scripted
    0 1 var-set
    3 self-is? $22 self-is? or if
        8 3 object-anim
        9 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $10 self-wait-frames
        0 2 var-set
        self-frames-reset
        $A self-wait-frames
        $FE 0 6 char-sound
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        9 state-flag? if
            2 avoid-prompt
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-move-16
    begin
        0 char-busy? while
        yield
    repeat
    $FE 0 stalker-mode
    self-idle-or-end
;

: old-mansion-b1-5.act11 ( -- )   \ 0040DE60
    1 self-scripted
    $18 state-flag-set
    7 state-flag-set
    self-wait-done
    0 $D char-file-load
    $7B 16.59 -25.02 180 $FFFF 5 self-move-to
    self-wait-done
    $FE char-activate
    $FE $51 7 2 stalker-to-room
    $FE 0 0 char-camera
    stalker-item-cooldown
    $FE 1 char-visible
    $FE 1 char-silent
    0 char-file-use
    0 counter-set
    0 $FE $12 action
    1 wait-counter
    0 $43 5 char-sound
    $8000 $A self-anim-blend
    self-wait-anim
    $8003 $A self-anim-blend
    self-wait-anim
    counter-inc
    $18 state-flag-clear
    7 state-flag-clear
    7 ebit-clear
    $328 story-flag-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act12 ( -- )   \ 0040DED0
    1 self-noclip
    1 self-scripted
    self-wait-done
    $FE $77 15.58 -33.18 0 char-to-xz
    1 self-noclip
    1 self-scripted
    $FE $C char-file-load
    $FE 0 char-visible
    $FE 0 char-silent
    0 0 $14 door-bits
    $FE char-file-use
    $327 story-flag-set
    1 0 $20000 nav-group
    counter-inc
    8 0 object-show
    9 0 object-show
    8 6 object-anim
    9 6 object-anim
    $8003 self-anim
    self-frames-reset
    7 self-wait-frames
    0 0 6 char-sound
    self-wait-anim
    2 wait-counter
    0 self-noclip
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-5.act13 ( -- )   \ 0040DF30
    begin
        3 cutscene-shot? not while
        yield
    repeat
    0 1 char-no-shadow
    begin
        4 cutscene-shot? not while
        yield
    repeat
    0 0 char-no-shadow
    self-idle-or-end
;

: old-mansion-b1-5.act14 ( -- )   \ 0040DF50
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-b1-5.act15 ( -- )   \ 0040E030
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
    3 0 movie-play
    2 cutscene-start
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-b1-5.act16 ( -- )   \ 0040E110
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    $10 $78 movie-param
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-b1-5.act17 ( -- )   \ 0040E1F0
    self-wait-done
    0 -2.8 31.7 -5.1 0 effect-86
    1 -5.7 31.4 -0.3 0 effect-86
    2 -3.3 31.4 4.3 0 effect-86
    3 2.7 31.7 -5.0 0 effect-86
    4 5.6 31.5 -0.4 0 effect-86
    5 2.9 31.2 4.4 0 effect-86
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 $F9 $13 action
    $10 $28 movie-param
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
    $80 exit-check
    $26 state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-b1-5.phase5 ( -- )   \ 0047AB98
    7 state-flag-clear
;

\ ---- registered ----
' old-mansion-b1-5.enter old-mansion-b1-5 0 room-script!
' old-mansion-b1-5.char-enter old-mansion-b1-5 6 room-script!
' old-mansion-b1-5.phase1 old-mansion-b1-5 1 room-script!
' old-mansion-b1-5.phase2 old-mansion-b1-5 2 room-script!
' old-mansion-b1-5.act00 old-mansion-b1-5 $00 action-script!
' old-mansion-b1-5.act01 old-mansion-b1-5 $01 action-script!
' old-mansion-b1-5.act02 old-mansion-b1-5 $02 action-script!
' old-mansion-b1-5.act03 old-mansion-b1-5 $03 action-script!
' old-mansion-b1-5.act04 old-mansion-b1-5 $04 action-script!
' old-mansion-b1-5.act05 old-mansion-b1-5 $05 action-script!
' old-mansion-b1-5.act06 old-mansion-b1-5 $06 action-script!
' old-mansion-b1-5.act07 old-mansion-b1-5 $07 action-script!
' old-mansion-b1-5.act08 old-mansion-b1-5 $08 action-script!
' old-mansion-b1-5.act09 old-mansion-b1-5 $09 action-script!
' old-mansion-b1-5.act0A old-mansion-b1-5 $0A action-script!
' old-mansion-b1-5.act0B old-mansion-b1-5 $0B action-script!
' old-mansion-b1-5.act0C old-mansion-b1-5 $0C action-script!
' old-mansion-b1-5.act0D old-mansion-b1-5 $0D action-script!
' old-mansion-b1-5.act0E old-mansion-b1-5 $0E action-script!
' old-mansion-b1-5.act0F old-mansion-b1-5 $0F action-script!
' old-mansion-b1-5.act10 old-mansion-b1-5 $10 action-script!
' old-mansion-b1-5.act11 old-mansion-b1-5 $11 action-script!
' old-mansion-b1-5.act12 old-mansion-b1-5 $12 action-script!
' old-mansion-b1-5.act13 old-mansion-b1-5 $13 action-script!
' old-mansion-b1-5.act14 old-mansion-b1-5 $14 action-script!
' old-mansion-b1-5.act15 old-mansion-b1-5 $15 action-script!
' old-mansion-b1-5.act16 old-mansion-b1-5 $16 action-script!
' old-mansion-b1-5.act17 old-mansion-b1-5 $17 action-script!
' old-mansion-b1-5.phase5 old-mansion-b1-5 5 room-script!
