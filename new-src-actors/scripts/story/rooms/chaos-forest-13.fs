\ story/rooms/chaos-forest-13.fs - the event scripts of room chaos-forest-13 ($10B; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-13
USING: room-names story.words story.shared ;

\ byte 3 0: a 0x4480 effect is spawned and its slot kept in event var 0; else that slot's effect
\ is removed
: chaos-forest-13.cmd00 ( b0 -- )  drop s" chaos-forest-13.cmd00" stub-step ;
\ room 0x10B: the progress' +0xFB6 count (it rises while Hewie is down, Hewie_AdjustAction) has
\ reached 100.
: chaos-forest-13.cond00? ( -- flag )  s" chaos-forest-13.cond00?" stub-flag ;

: chaos-forest-13.enter ( -- )   \ 00419A80
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    $FF panic-stage? if
        1 ebit-set
    then
;

: chaos-forest-13.char-enter ( -- )   \ 00419AA0
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

: chaos-forest-13.phase1 ( -- )   \ 00419AE0
    hewie-controlled? not if
        0 0 char-in-area? 0 char-busy? not and if
            0 exit-check
        then
    else 1 0 char-in-area? 1 char-busy? not and if
        0 exit-check
    then then
    0 2 char-entered-area? if
        0 exit-prepare
    then
    0 3 char-entered-area? if
        $87 story-flag? chaos-forest-13.cond00? not or if
            $AE story-flag? not if
                $C0 room-preload
            else
                $59 room-preload
            then
        else
            $34 room-preload
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

: chaos-forest-13.phase3 ( -- )   \ 00419B70
    2 ebit? if
        -60.0 40.0 -30.0 10.0 46.5 -46.0 -60.0 0.0 -40.0 10.0 -10.0 -59.0 lights-doorway
    then
;

: chaos-forest-13.act00 ( -- )   \ 00419BB0
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
        1 chaos-forest-13.cmd00
    then
    8 state-flag-set
    $87 story-flag? chaos-forest-13.cond00? not or if
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
            $C0 room-preload
            3 0 char-remove
            $80 exit-check
        else
            $59 room-preload
            $80 exit-check
        then
    else
        $17 action-end
        $17 char-done
        $34 room-preload
        $80 exit-check
    then
    0 self-scripted
    self-idle-or-end
;

: chaos-forest-13.act01 ( -- )   \ 00419CB0
    begin
        1 cutscene-shot? not while
        yield
    repeat
    0 chaos-forest-13.cmd00
    3 ebit-set
    begin
        2 cutscene-shot? not while
        yield
    repeat
    1 chaos-forest-13.cmd00
    3 ebit-clear
    begin
        $18 cutscene-shot? not while
        yield
    repeat
    2 ebit-set
    self-idle-or-end
;

: chaos-forest-13.act02 ( -- )   \ 00419CE0
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

\ ---- registered ----
' chaos-forest-13.enter chaos-forest-13 0 room-script!
' chaos-forest-13.char-enter chaos-forest-13 6 room-script!
' chaos-forest-13.phase1 chaos-forest-13 1 room-script!
' chaos-forest-13.phase3 chaos-forest-13 3 room-script!
' chaos-forest-13.act00 chaos-forest-13 $00 action-script!
' chaos-forest-13.act01 chaos-forest-13 $01 action-script!
' chaos-forest-13.act02 chaos-forest-13 $02 action-script!
