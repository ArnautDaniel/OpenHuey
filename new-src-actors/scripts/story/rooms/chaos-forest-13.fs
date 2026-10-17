\ story/rooms/chaos-forest-13.fs - the event scripts of room chaos-forest-13 ($10B; Chaos Forest).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.chaos-forest-13
USING: actors room-names story.words story.shared flag-names story-names ;

\ byte 3 0: a 0x4480 effect is spawned and its slot kept in event var 0; else that slot's effect
\ is removed
: chaos-forest-13.cmd00 ( b0 -- )  0 ['] effects:ash effect-in-var ;   \ (ash flakes rising: b0 0 made - kept in variable 0 - else gone)
\ room 0x10B: the progress' +0xFB6 count (it rises while Hewie is down, Hewie_AdjustAction) has
\ reached 100.
: chaos-forest-13.cond00? ( -- flag )  hewie-mistreated 100 >= ;

: chaos-forest-13.enter ( -- )   \ 00419A80
    $19 1.0 0 bgm
    $1F 0 pvar? if
        deal-things
    then
    in-panic? if
        1 ebit-set
    then
;

: chaos-forest-13.char-enter ( -- )   \ 00419AA0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
;

: chaos-forest-13.phase1 ( -- )   \ 00419AE0
    player-who 0 char-in-area? player-who char-busy? not and if
        0 exit-check
    then
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        cf-10-flag1 story-flag? chaos-forest-13.cond00? not or if
            fg-3-flag3 story-flag? not if
                $C0 room-preload
            else
                $59 room-preload
            then
        else
            $34 room-preload
        then
    then
    fiona-who 1 char-entered-area? if
        found-file-247 story-flag? cf-13-flag1 story-flag? not and if
            4 0 char-remove
            5 0 char-remove
            cf-13-flag1 story-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    0 ebit? not if
        1 ebit? not in-panic? and if
            0 ebit-set
            deal-things
        then
    then
    in-panic? if
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
    summoner-on state-flag-clear
    stalker-who action-end
    stalker-who char-done
    hewie-who action-end
    hewie-who char-done
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
    wait-fade
    items-40-as-70 story-flag? if
        creatures-on state-flag-clear
        1 creatures-clear
    then
    0 creatures-clear
    cf-13-flag1 story-flag-set
    $17 3 $FF char-load
    3 char-start
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    3 5 0 char-model-op
    0 cutscene-track 1 action
    3 ebit-clear
    panic-eased
    0 message-prepare   \ "Why are you doing this? What did I do? You inherited your father's Azoth. That..."
    1 result? if
        scene-play
    then
    stalker-who action-end
    stalker-who char-done
    2 ebit-clear
    3 ebit? if
        1 chaos-forest-13.cmd00
    then
    world-held state-flag-set
    cf-10-flag1 story-flag? chaos-forest-13.cond00? not or if
        new-game-sounds state-flag-set
        1 sound-set
        3 fiona-costume
        $26 3 pvar-set
        effects-arena-flip
        0 char-in
        begin
            yield
            4 sound-bank-loaded? until
        fg-3-flag3 story-flag? not if
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
    [: 1 cutscene-shot? ;] wait-until
    0 chaos-forest-13.cmd00
    3 ebit-set
    [: 2 cutscene-shot? ;] wait-until
    1 chaos-forest-13.cmd00
    3 ebit-clear
    [: $18 cutscene-shot? ;] wait-until
    2 ebit-set
    self-idle-or-end
;

: chaos-forest-13.act02 ( -- )   \ 00419CE0
    self-wait-done
    4 partner-load
    $17 3 $FF char-load
    2 char-start
    3 char-start
    1 2 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    3 5 0 char-model-op
    0 cutscene-track 1 action
    panic-eased
    0 message-prepare   \ "Why are you doing this? What did I do? You inherited your father's Azoth. That..."
    1 result? if
        scene-play
    then
    2 0 char-remove
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
chaos-forest-13 register-scripts   \ (its phases and actions, by their names)
