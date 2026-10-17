\ story/rooms/house-of-truth-1f-1.fs - the event scripts of room house-of-truth-1f-1 ($81; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-1
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant rested                      \ (bit 0) she has rested here (her recovery: once a visit)
1 constant hewie-sent-off              \ (bit 1) Hewie was here and sent off for the scene
2 constant key-used                    \ (bit 2) the key used: the door unlocked, the summoner off
3 constant sound-started               \ (bit 3) its sound started (then kept up)
4 constant sound2-started              \ (bit 4) its sound started (then kept up)
5 constant flame-lit                   \ (bit 5) the flame lit

: house-of-truth-1f-1.enter ( -- )   \ 00430AB0
    room-sounds
    ht1f-1-flag1 story-flag? not if
        0 part-shown
        1 part-hidden
        1 solid-floor floor-off
        0 solid-floor floor-on
    else
        0 part-hidden
        1 part-shown
        0 solid-floor floor-off
        1 solid-floor floor-on
    then
    0 -24.6 22.1 -26.4 1 flame-in
    ht1f-1-album-medallion-found story-flag? ht1f-1-album-medallion story-flag? not and if
        1 -78.0 1.0 39.0 flicker-sprite
    then
    ht1f-1-flag1 story-flag? if
        flame-lit ebit-set
    then
    items-40-as-70 story-flag? if
        over-music-d story-flag? ht1f-1-flag2 story-flag? not and if
            ht1f-1-flag2 story-flag-set
            $81 0 624 4 10 -1 0 0.0 creature-place
            $81 0 321 4 10 -1 0 0.0 creature-place
        then
    then
    $51 hole-floor tri-floor-on
    $21C hole-floor tri-floor-on
    $2F hole-floor tri-floor-on
    $54 hole-floor tri-floor-on
    $23B hole-floor tri-floor-on
    $43 hole-floor tri-floor-on
    $22C hole-floor tri-floor-on
    $44 hole-floor tri-floor-on
    $22D hole-floor tri-floor-on
    $45 hole-floor tri-floor-on
    $22E hole-floor tri-floor-on
    $46 hole-floor tri-floor-on
    $22F hole-floor tri-floor-on
    $40 hole-floor tri-floor-on
    $229 hole-floor tri-floor-on
    $3D hole-floor tri-floor-on
    $226 hole-floor tri-floor-on
    $3A hole-floor tri-floor-on
    $224 hole-floor tri-floor-on
    $28 hole-floor tri-floor-on
    $216 hole-floor tri-floor-on
    $2D hole-floor tri-floor-on
    $21A hole-floor tri-floor-on
    $53 hole-floor tri-floor-on
    $21E hole-floor tri-floor-on
    $31 hole-floor tri-floor-on
    $56 hole-floor tri-floor-on
    $23D hole-floor tri-floor-on
    $4B hole-floor tri-floor-on
    $234 hole-floor tri-floor-on
;

: house-of-truth-1f-1.char-enter ( -- )   \ 00430C40
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        4 4 player-camera
    then
    1 4 4 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        5 5 player-camera
    then
    2 5 5 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 0 player-camera
    then
    3 0 0 area-camera
    player-who self-is? 4 exit-taken? and if
        player-who 4 char-to-exit
        2 2 player-camera
    then
    4 2 2 area-camera
    fiona-who self-is? if
        0 exit-taken? 1 exit-taken? or 2 exit-taken? or 4 exit-taken? or if
            2 map-page
        then
    then
;

: house-of-truth-1f-1.phase1 ( -- )   \ 00430D80
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    player-who $17 char-in-area? player-who char-busy? not and if
        4 exit-check
    then
    7 0 0 1 chars-area-camera
    8 2 2 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $D 2 2 1 chars-area-camera
    $E 4 4 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 5 5 1 chars-area-camera
    $11 2 2 1 chars-area-camera
    $12 2 2 1 chars-area-camera
    $13 2 2 1 chars-area-camera
    $14 6 6 1 chars-area-camera
    $15 6 6 1 chars-area-camera
    $16 6 6 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who $E char-entered-area? fiona-who $10 char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        3 exit-prepare
    then
    fiona-who $18 char-entered-area? fiona-who $D char-entered-area? or fiona-who $F char-entered-area? or fiona-who 5 char-entered-area? or if
        4 exit-prepare
    then
    fiona-who $1D char-entered-area? if
        1 map-page
    then
    fiona-who $1D char-left-area? if
        2 map-page
    then
    fiona-who $1B char-entered-area? if
        ht1f-1-flag1 story-flag? not if
            0 game-mode? not $24 1 item-count? and if
                ht1f-1-flag1 story-flag-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 1 action-force
                else
                    1 fiona-who 1 action-force
                then
            then
        then
    then
    ht1f-1-album-medallion-found story-flag? not if
        0 -78.0 0.0 39.0 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var ht1f-1-album-medallion-found var-set
                taken-flag-var ht1f-1-album-medallion var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var -78000 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 39000 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    sound-started ebit? not if
        6 sound-bank-loaded? if
            $40000002 6 -24.6 22.1 -26.4 0 0 sound
            sound-started ebit-set
        then
    else
        $C0000002 6 -24.6 22.1 -26.4 0 0 sound
    then
    flame-lit ebit? if
        sound2-started ebit? not if
            6 sound-bank-loaded? if
                $40000006 6 -21.86 0.0 33.51 0 0 sound
                sound2-started ebit-set
            then
        else
            $C0000006 6 -21.86 0.0 33.51 0 0 sound
        then
    then
;

: house-of-truth-1f-1.phase2 ( -- )   \ 00430F30
    fiona-who $19 char-in-area? fiona-who 0 $32 char-heading? and if
        0 unhurried offer-action
    then
    ht1f-1-flag1 story-flag? not if
        fiona-who $1A $32 char-faces-area? if
            2 unhurried offer-action
        then
        fiona-who $1B char-in-area? fiona-who 45 $32 char-heading? and if
            3 unhurried offer-action
        then
    else fiona-who $1C char-in-area? fiona-who 30 $32 char-heading? and key-used ebit? and if
        4 unhurried offer-action
    then then
    ht1f-1-album-medallion-found story-flag? ht1f-1-album-medallion story-flag? not and if
        0 1 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            6 calm offer-action
        then
    then
;

: house-of-truth-1f-1.phase5 ( -- )   \ 00430F90
    key-used ebit? if
        force-calm state-flag-clear
        stalker-who action-end
        stalker-who action-end
        3 summon-take
        no-stalker-camera state-flag-clear
        stalker-no-fear state-flag-clear
    then
;

: house-of-truth-1f-1.phase3 ( -- )   \ 00430FB0
    14.0 74.0 -180.0 14.0 74.0 -160.0 14.0 59.0 -180.0 14.0 59.0 -160.0 lights-doorway
    14.0 74.0 -160.0 10.0 74.0 -80.0 10.0 39.0 -160.0 7.0 39.0 -80.0 lights-doorway
    10.0 73.0 -85.5 43.0 73.0 -85.5 10.0 40.0 -85.5 43.0 40.0 -85.5 lights-doorway
    -43.0 73.0 -85.5 -10.0 73.0 -85.5 -43.0 40.0 -85.5 -10.0 40.0 -85.5 lights-doorway
    -70.0 55.0 -60.2 -38.0 78.0 -60.2 -70.0 40.0 -60.2 -38.0 60.0 -60.2 lights-doorway
;

: house-of-truth-1f-1.act00 ( -- )   \ 004310B0
    rested ebit? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        fiona-who 0 char-file-load
        $117 0.0 -1.1 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        3 say   \ "Life-giving water..."
        fiona-who char-file-use
        4 room-motion $A self-anim-blend
        $16 wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        3 avoid-prompt
        rested ebit-set
        fiona-who $84 5 char-sound
        $1E wait-frames
        got-doc-recovery story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 say   \ "'Recovery Points' Search a recovery point to calm Fiona from a state of pan..."
            $F 7 fade
            wait-fade
            got-doc-recovery story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B say   \ "has been saved to your file."
            4 wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        0.0 5.0 self-turn-to-xz
        self-wait-done
        4 say   \ "Ahh. Now that is some good water."
    then
    self-idle-or-end
;

: house-of-truth-1f-1.act01 ( -- )   \ 00431170
    scene-locked state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    $F $54 fade
    self-wait-done
    wait-fade
    hewie-who char-here? if
        hewie-sent-off ebit-set
        hewie-who action-end
        hewie-who char-done
    then
    stalker-who action-end
    stalker-who char-done
    2 1 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 5 action
    $10 $FF movie-param
    0 effect-remove
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who $EF -71.08 32.24 50 char-to-xz
    hewie-sent-off ebit? if
        hewie-who char-activate
        hewie-who $20F -2.73 -54.08 70 char-to-xz
        hewie-sent-off ebit-clear
    then
    stalker-who action-end
    stalker-who char-done
    stalker-who 3 char-file-load
    stalker-who char-file-use
    0 solid-floor floor-off
    1 keeps-fiona-off keeps-hewie-off or floor-on
    $FE $81 457 2 stalker-to-room
    stalker-who $1C9 -53.111 33.905 -82 char-to-xz
    stalker-who char-activate
    stalker-who 1 char-silent
    0 stalker-who 7 action
    no-stalker-camera state-flag-set
    stalker-no-fear state-flag-set
    0 0 player-camera
    0 part-hidden
    1 part-shown
    $E0 door-unlock
    $24 item-use
    key-used ebit-set
    summoner-on state-flag-clear
    force-calm state-flag-set
    0 -24.6 22.1 -26.4 1 flame-in
    flame-lit ebit-set
    $F $51 fade
    wait-fade
    $44 resident-flag-set
    stalkers-stay state-flag-clear
    scene-locked state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-1.act02 ( -- )   \ 004312C0
    self-wait-done
    -32.0 34.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    1 say   \ "'Caution. Extremely Flammable' I wonder if there are explosives or the like inside. If..."
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-1.act03 ( -- )   \ 004312E0
    self-wait-done
    $10C -30.0 -26.0 90 $FFFF 5 self-move-to
    self-wait-done
    look-at-anim self-anim
    16 wait-frames
    0 say   \ "The fire spouts showers of sparks into the air."
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-1.act04 ( -- )   \ 00431300
    self-wait-done
    -53.0 34.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    2 say   \ "Wreckage from the wall lies in a mass on the floor. What a powerful explosion."
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-1.act05 ( -- )   \ 00431320
    [: 6 cutscene-shot? ;] wait-until
    0 part-hidden
    1 part-shown
    [: cutscene-near-end? ;] wait-until
    self-idle-or-end
;

: house-of-truth-1f-1.act06 ( -- )   \ 00431340
    self-wait-done
    -78.0 39.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht1f-1-album-medallion story-flag-set
            1 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-1f-1.act07 ( -- )   \ 004313A0
    1 self-scripted
    self-wait-done
    stalker-who 0 char-silent
    self-who 0 char-visible
    0 room-motion self-anim
    begin
        yield
    again
;

: house-of-truth-1f-1.act08 ( -- )   \ 004313B0
    self-wait-done
    0 part-shown
    $B 3 $FF char-load
    3 char-start
    2 1 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 5 action
    $10 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-1f-1 register-scripts   \ (its phases and actions, by their names)
