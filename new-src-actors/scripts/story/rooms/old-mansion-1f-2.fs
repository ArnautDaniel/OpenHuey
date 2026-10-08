\ story/rooms/old-mansion-1f-2.fs - the event scripts of room old-mansion-1f-2 ($40; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-2
USING: room-names story.words story.shared flag-names ;

\ the scales ("tenbin") and their pans ("sara_l", "sara_r"): level (byte 3 0) or tipped
: old-mansion-1f-2.cmd00 ( b0 -- )  drop s" old-mansion-1f-2.cmd00" stub-step ;

: old-mansion-1f-2.enter ( -- )   \ 00406550
    room-sounds
    1 0 var-set
    2 0 var-set
    1 old-mansion-1f-2.cmd00
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    $85 story-flag? not if
        1 0 $20000 nav-group
    else
        0 1 $14 door-bits
        9 1 object-show
        $A 1 object-show
    then
    $27A story-flag? not if
        0 -72.34 59.0 -34.25 flicker-sprite
    then
    2 -19.7 18.3 22.5 0 effect-86
    3 -19.7 18.3 25.0 0 effect-86
    4 -17.2 18.3 22.4 0 effect-86
    5 -17.2 18.3 25.0 0 effect-86
    6 17.2 18.3 22.4 0 effect-86
    7 17.2 18.3 25.0 0 effect-86
    8 19.7 18.3 22.5 0 effect-86
    9 19.7 18.3 25.0 0 effect-86
    1 $2300 sound-volume
    $1C 3 $FF char-load
;

: old-mansion-1f-2.char-enter ( -- )   \ 00406630
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
        0 self-is? 4 exit-taken? and if
            0 4 char-to-exit
            hewie-controlled? not if
                0 3 3 char-camera
                0 camera-follow
            else
                1 3 3 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 4 exit-taken? and if
        1 4 char-to-exit
        hewie-controlled? not if
            0 3 3 char-camera
            0 camera-follow
        else
            1 3 3 char-camera
            1 camera-follow
        then
    then then
    4 3 3 area-camera
    hewie-controlled? not if
        0 self-is? 5 exit-taken? and if
            0 5 char-to-exit
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
        then
    else 1 self-is? 5 exit-taken? and if
        1 5 char-to-exit
        hewie-controlled? not if
            0 2 2 char-camera
            0 camera-follow
        else
            1 2 2 char-camera
            1 camera-follow
        then
    then then
    5 2 2 area-camera
    0 self-is? if
        2 exit-taken? 4 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-2.phase1 ( -- )   \ 00406770
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    hewie-controlled? not if
        0 5 char-in-area? 0 char-busy? not and if
            5 exit-check
        then
    else 1 5 char-in-area? 1 char-busy? not and if
        5 exit-check
    then then
    $B 0 0 1 chars-area-camera
    $C 3 3 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 2 2 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    $10 4 -1 1 chars-area-camera
    0 6 char-entered-area? if
        5 exit-prepare
    then
    0 7 char-entered-area? if
        0 exit-prepare
    then
    0 8 char-entered-area? if
        1 exit-prepare
    then
    0 9 char-entered-area? if
        2 exit-prepare
    then
    0 $A char-entered-area? if
        4 exit-prepare
    then
    0 0.56 0.0 14.62 $23 22 0 zone
    1 char-here? 0 game-mode? and if
        hewie-can-command? if
            1 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 22.0 hewie-look-zone
            then
        then
    then
    $85 story-flag? if
        4 ebit? not if
            6 sound-bank-loaded? if
                $40000000 6 0.0 0.0 130.0 0 0 sound
                4 ebit-set
            then
        else
            $C0000000 6 0.0 0.0 130.0 0 0 sound
        then
    then
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000005 6 -109.95 50.0 -5.34 0 0 sound
                $4000000A 6 -42.0 20.0 117.0 0 0 sound
            else 1 1 var? if
                $40000006 6 -109.95 50.0 -5.34 0 0 sound
                $4000000B 6 -42.0 20.0 117.0 0 0 sound
            else 1 2 var? if
                $40000007 6 -109.95 50.0 -5.34 0 0 sound
                $4000000C 6 -42.0 20.0 117.0 0 0 sound
            else 1 3 var? if
                $40000008 6 -109.95 50.0 -5.34 0 0 sound
                $4000000D 6 -42.0 20.0 117.0 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: old-mansion-1f-2.phase2 ( -- )   \ 00406930
    0 $11 char-in-area? 0 90 $32 char-heading? and if
        5 4 0 scene-change
    then
    $85 story-flag? not if
        0 $12 char-in-area? 0 0 $32 char-heading? and if
            5 5 0 scene-change
        then
    then
    0 $13 char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    0 $14 char-in-area? 0 0 $32 char-heading? and if
        5 $84 3 scene-change
    then
    $27A story-flag? not if
        1 0 5 5 0 zone-at-effect
        0 1 3 char-zone-bits? if
            5 6 4 scene-change
        then
    then
;

: old-mansion-1f-2.act00 ( -- )   \ 00406990
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $FE action-end
    $FE char-done
    summoner-on state-flag-clear
    $F $54 fade
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
    1 char-here? if
        3 ebit-clear
        1 2 char-C4? if
            0 1 $86 action
        else
            1 action-end
            1 char-done
        then
    else
        3 ebit-set
    then
    0 $F9 2 action
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    wait-fade
    0 self-move-16
    0 $158 3.0 24.0 83 char-to-xz
    camera-restart
    3 ebit? not if
        1 2 char-C4? if
            1 0 char-visible
            1 action-end
        else
            1 char-activate
            $40 1 229 hewie-to-room
            1 $E5 26.55 54.18 -129 char-to-xz
        then
    then
    $F9 action-end
    4 1 object-show
    5 1 object-show
    6 1 object-show
    0 11 var? if
        7 0 object-show
        8 1 object-show
        $B item-use
    else
        7 1 object-show
        8 0 object-show
        $C item-use
    then
    $B 0 object-show
    $C 0 object-show
    $D 0 object-show
    $E 0 object-show
    0 old-mansion-1f-2.cmd00
    0 0 $20000 nav-group
    0 1 $14 door-bits
    9 1 object-show
    $A 1 object-show
    0 ebit-set
    1 6.2 18.0 17.1 flicker-sprite
    $85 story-flag-set
    $5D door-unlock
    $F $51 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-2.act01 ( -- )   \ 00406AF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $54 fade
    3 1 movie-play
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
    0 $F9 3 action
    $10 $FF movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    wait-fade
    $F9 action-end
    0 8 var? if
        8 item-use
    else 0 9 var? if
        9 item-use
    else 0 10 var? if
        $A item-use
    then then then
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    $B 0 object-show
    $C 0 object-show
    $D 0 object-show
    $E 0 object-show
    1 old-mansion-1f-2.cmd00
    0 self-move-16
    0 $158 5.733 23.039 180 char-to-xz
    camera-restart
    1 0 char-visible
    1 action-end
    $F $51 fade
    wait-fade
    $38 resident-flag-set
    $902 self-anim
    self-wait-anim
    $10 message-param-room
    $10 1 item-give-count
    0 $10 item-tab
    0 $F9 $8C action-force
    $83 $85 0.0 0.0 0.0 0 0 sound
    $8012 message
    wait-message
    $361 story-flag-set
    $903 self-anim
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-2.act02 ( -- )   \ 00406C30
    begin
        4 1 object-show
        5 1 object-show
        6 1 object-show
        0 11 var? if
            7 0 object-show
            8 1 object-show
        else
            7 1 object-show
            8 0 object-show
        then
        yield
    again
;

: old-mansion-1f-2.act03 ( -- )   \ 00406C60
    begin
        $1CC cutscene-cue-reached? not while
        4 1 object-show
        5 1 object-show
        6 1 object-show
        7 1 object-show
        8 1 object-show
        0 8 var? if
            4 0 object-show
        else 0 9 var? if
            5 0 object-show
        else 0 10 var? if
            6 0 object-show
        then then then
        yield
    repeat
    4 1 object-show
    5 1 object-show
    6 1 object-show
    7 1 object-show
    8 1 object-show
    self-idle-or-end
;

: old-mansion-1f-2.act04 ( -- )   \ 00406CC0
    self-wait-done
    0 ebit? not if
        4.0 17.0 self-turn-to-xz
        self-wait-done
        1 ebit? not if
            0 message
            wait-message
            1 ebit-set
        else
            1 message
            wait-message
        then
    else
        2 message
        wait-message
        0 answer? if
            $158 4.619 23.5 180 $FFFF 5 self-move-to
            self-wait-done
            $902 self-anim
            self-wait-anim
            self-frames-reset
            4 self-wait-frames
            0 ebit-clear
            1 effect-remove
            1 old-mansion-1f-2.cmd00
            0 11 var? if
                7 1 object-show
                $B message-param-room
                $B 1 item-give-count
                0 $B item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
            else
                8 1 object-show
                $C message-param-room
                $C 1 item-give-count
                0 $C item-tab
                0 $F9 $8C action-force
                $83 $85 0.0 0.0 0.0 0 0 sound
                $8012 message
                wait-message
            then
            $903 self-anim
            self-wait-anim
        then
    then
    self-idle-or-end
;

: old-mansion-1f-2.act05 ( -- )   \ 00406D70
    self-wait-done
    2 ebit? not if
        0 self-turn-angle
        self-wait-done
        $A00 self-anim
        self-wait-anim
        3 message
        wait-message
        stalkers-stay state-flag-set
        1 self-scripted
        $F 6 fade
        wait-fade
        4 message
        wait-message
        2 ebit-set
        $F 7 fade
        wait-fade
        $246 item-give
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        $1B9 -3.806 112.773 0 $FFFF 5 self-move-to
        self-wait-done
        $1200 self-anim
        self-wait-anim
        $1203 self-anim
        self-wait-anim
        self-frames-reset
        self-wait-16
        $1202 self-anim
        self-wait-anim
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
        5 message
        wait-message
        2 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-1f-2.act06 ( -- )   \ 00406DD0
    self-wait-done
    -72.34 -34.25 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message
            wait-message
        else
            $27A story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
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

: old-mansion-1f-2.act07 ( -- )   \ 00406E30
    self-wait-done
    2 -19.7 18.3 22.5 0 effect-86
    3 -19.7 18.3 25.0 0 effect-86
    4 -17.2 18.3 22.4 0 effect-86
    5 -17.2 18.3 25.0 0 effect-86
    6 17.2 18.3 22.4 0 effect-86
    7 17.2 18.3 25.0 0 effect-86
    8 19.7 18.3 22.5 0 effect-86
    9 19.7 18.3 25.0 0 effect-86
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
    0 11 var-set
    0 $F9 2 action
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

: old-mansion-1f-2.act08 ( -- )   \ 00406F40
    self-wait-done
    2 -19.7 18.3 22.5 0 effect-86
    3 -19.7 18.3 25.0 0 effect-86
    4 -17.2 18.3 22.4 0 effect-86
    5 -17.2 18.3 25.0 0 effect-86
    6 17.2 18.3 22.4 0 effect-86
    7 17.2 18.3 25.0 0 effect-86
    8 19.7 18.3 22.5 0 effect-86
    9 19.7 18.3 25.0 0 effect-86
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    2 cutscene-control
    begin
        3 cutscene-control
        2 cutscene-mode? not while
        yield
    repeat
    0 9 var-set
    0 $F9 3 action
    $10 $FF movie-param
    $FF panic-stage? if
        3 panic-stage
    then
    $FFFF message-prepare
    1 result? if
        6 cutscene-control
        1.0 movie-volume
        effects-arena-flip
        yield
        movie-skipped state-flag-clear
        $F 1 fade
        $A cutscene-control
        begin
            movie-skipped state-flag? not if
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
        world-held state-flag-set
        effects-arena-flip
        begin
            movie-playing? while
            yield
        repeat
        yield
        pause-wanted state-flag-set
        8 cutscene-control
    then
    $80 exit-check
    events-held state-flag-clear
    1 action-end
    1 char-done
    self-idle-or-end
;

\ ---- registered ----
' old-mansion-1f-2.enter old-mansion-1f-2 0 room-script!
' old-mansion-1f-2.char-enter old-mansion-1f-2 6 room-script!
' old-mansion-1f-2.phase1 old-mansion-1f-2 1 room-script!
' old-mansion-1f-2.phase2 old-mansion-1f-2 2 room-script!
' old-mansion-1f-2.act00 old-mansion-1f-2 $00 action-script!
' old-mansion-1f-2.act01 old-mansion-1f-2 $01 action-script!
' old-mansion-1f-2.act02 old-mansion-1f-2 $02 action-script!
' old-mansion-1f-2.act03 old-mansion-1f-2 $03 action-script!
' old-mansion-1f-2.act04 old-mansion-1f-2 $04 action-script!
' old-mansion-1f-2.act05 old-mansion-1f-2 $05 action-script!
' old-mansion-1f-2.act06 old-mansion-1f-2 $06 action-script!
' old-mansion-1f-2.act07 old-mansion-1f-2 $07 action-script!
' old-mansion-1f-2.act08 old-mansion-1f-2 $08 action-script!
