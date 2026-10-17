\ story/rooms/old-mansion-1f-9.fs - the event scripts of room old-mansion-1f-9 ($49; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-9
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ a lit quad at x -43, z -15.12 .. 5.07, height 30.05 / 10.05
: old-mansion-1f-9.cmd00 ( b0 -- )  $1B $40  -43.0e 30.053e -15.124e  -43.0e 30.053e 5.068e  -43.0e 10.053e -15.124e  -43.0e 10.053e 5.068e  lit-quad ;   \ (a reflecting window: b0 1 removed, 2 over the whole picture)
\ the depth range (effect 0x1C) opening with the cutscene from its frame 1156: 1 / 1 / 40 / 100,
\ the far two on by 1 a frame up to 80 / 140
: old-mansion-1f-9.cmd01 ( -- )   \ (opening with the scene from its frame 1156: 1 / 1 / 40 / 100, the far two on 1 a frame to 80 / 140)
    cutscene-frame 1156 - s>f  1e 1e  frot fdup 40e f+ 80e fmin  fswap 100e f+ 140e fmin  depth-range ;
\ byte 3 0: the effect Room49Effect_vtable spawned, its slot in event var 0; 1: removed
: old-mansion-1f-9.cmd02 ( b0 -- )  0 ['] effects:haze effect-in-var ;   \ (the haze: b0 0 made - kept in variable 0 - else gone)

: old-mansion-1f-9.enter ( -- )   \ 00407F30
    room-sounds
    0 old-mansion-1f-9.cmd00
    om1f-9-refined-antimony story-flag? not if
        0 -7.97 12.42 21.3 flicker-sprite
    then
    om1f-9-refined-magnesia-found story-flag? om1f-9-refined-magnesia story-flag? not and if
        1 7.72 1.0 15.08 flicker-sprite
    then
    $78 hole-floor tri-floor-on
    $41 hole-floor tri-floor-on
    $79 hole-floor tri-floor-on
    $42 hole-floor tri-floor-on
;

: old-mansion-1f-9.char-enter ( -- )   \ 00407F80
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
    om1f-13-flag1 story-flag? if
        2 1 object-hidden
        3 1 object-hidden
        4 1 object-hidden
        5 1 object-hidden
        0 part-shown
    then
;

: old-mansion-1f-9.phase1 ( -- )   \ 00407FE0
    0 exit-usable? if
        0 exit-check
    then
    0 21.03 0.0 -17.8 $14 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
    1 -41.22 4.5 -4.36 $1C 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
    om1f-9-refined-magnesia-found story-flag? not if
        3 7.72 0.0 15.08 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-9-refined-magnesia-found var-set
                taken-flag-var om1f-9-refined-magnesia var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var 7720 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 15080 var-set
                hewie-zone-var 3 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: old-mansion-1f-9.phase2 ( -- )   \ 004080A0
    fiona-who 2 $32 char-faces-area? if
        0 unhurried offer-action
    then
    fiona-who 1 char-in-area? fiona-who -45 $32 char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 3 char-in-area? fiona-who 15 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who 4 char-in-area? fiona-who -6 -22 $32 char-faces-xz? and if
        4 unhurried offer-action
    then
    fiona-who 5 char-in-area? fiona-who 0 $32 char-heading? and if
        8 unhurried offer-action
    then
    om1f-9-refined-antimony story-flag? not if
        2 0 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    om1f-9-refined-magnesia-found story-flag? om1f-9-refined-magnesia story-flag? not and if
        3 1 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            7 calm offer-action
        then
    then
;

: old-mansion-1f-9.act00 ( -- )   \ 00408120
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    plates-under-10? if
        0 ebit? not if
            $11 message   \ "It's a plate pressing machine."
            wait-message
            0 ebit-set
        then
        plate-made story-flag-clear
        2 subscreen-open
        begin
            subscreen-wanted state-flag? while
            yield
        repeat
        yield
        plate-made story-flag? if
            $3F message-param-room
            0 $3F item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        plate-made story-flag-set
    else 0 ebit? not if
        $F message   \ "There's a plate pressing machine here. There are no remaining plates currently so..."
        wait-message
        0 ebit-set
    else
        $12 message   \ "There are no plates remaining. I'll have to get rid of some plate-keys before I c..."
        wait-message
    then then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-9.act01 ( -- )   \ 0047AB78
    self-wait-done
    $10 message   \ "Returned a plate to the plate pressing machine."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-9.act02 ( -- )   \ 00408190
    om1f-13-flag1 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        0 message   \ "I wonder what's going on outside of this steel cage I seemed to have landed myself..."
        wait-message
        0 answer? if
            om1f-13-flag1 story-flag-set
            om1f-9-flag1 story-flag-set
            $F $44 fade
            $13 3 $FF char-load
            3 char-start
            self-wait-done
            1 0 movie-play
            0 cutscene-start
            yield
            yield
            scene-wait-ready
            wait-fade
            0 cutscene-track 6 action
            4 ebit-clear
            hewie-who char-here? if
                1 ebit-set
                hewie-who action-end
                hewie-who char-done
            then
            3 4 0 char-model-op
            $13 char-activate
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            world-held state-flag-set
            fiona-who $74 -32.419 -4.78 -107 char-to-xz
            camera-restart
            3 0 char-remove
            fiona-who $A char-layer
            2 1 object-hidden
            3 1 object-hidden
            4 1 object-hidden
            5 1 object-hidden
            0 part-shown
            4 ebit? if
                1 old-mansion-1f-9.cmd02
            then
            1 ebit? if
                hewie-who char-activate
                $49 0 88 hewie-to-room
            then
            $FE $49 41 2 stalker-to-room
            stalker-who $29 33.75 -4.38 -91 char-to-xz
            stalker-who 0 -1 char-camera
            summoner-own-cooldown
            $F $41 fade
            wait-fade
            $235 item-give
            $50 threat-raise
            $2A resident-flag-set
        then
        stalkers-stay state-flag-clear
        0 self-scripted
    else om1f-9-flag1 story-flag? if
        self-wait-done
        1 message   \ "It's pitch black outside. I can't see anything. Who could possibly fathom that this..."
        wait-message
    else
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        look-close-anim self-anim
        self-frames-reset
        $28 self-wait-frames
        5 message   \ "Who opened these blinds? It's too dark out to see anything."
        wait-message
        self-wait-anim
    then then
    self-idle-or-end
;

: old-mansion-1f-9.act03 ( -- )   \ 004082D0
    2 ebit? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        fiona-who 6 char-file-load
        $22 18.764 15.099 30 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        2 message   \ "Is this water safe to drink?"
        wait-message
        fiona-who char-file-use
        4 room-motion $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        fiona-who 3 6 char-sound
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        fiona-who $84 5 char-sound
        3 avoid-prompt
        2 ebit-set
        self-frames-reset
        $1E self-wait-frames
        got-doc-recovery story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message   \ "'Recovery Points' Search a recovery point to calm Fiona from a state of pan..."
            wait-message
            $F 7 fade
            wait-fade
            got-doc-recovery story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message   \ "has been saved to your file."
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        3 message   \ "Ah, I feel much better now."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-1f-9.act04 ( -- )   \ 00408390
    self-wait-done
    -6.0 -17.0 self-turn-to-xz
    self-wait-done
    4 message   \ "The tub is full of stagnating reddish water. It's probably just rust discoloration. Unl..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-9.act05 ( -- )   \ 004083A0
    self-wait-done
    -7.97 21.3 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-9-refined-antimony story-flag-set
            0 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-9.act06 ( -- )   \ 00408400
    depth-range-off
    begin
        1 cutscene-shot? not while
        yield
    repeat
    fiona-who $1E char-layer
    begin
        3 cutscene-shot? not while
        yield
    repeat
    fiona-who $A char-layer
    0 old-mansion-1f-9.cmd02
    4 ebit-set
    begin
        5 cutscene-shot? not while
        yield
    repeat
    1 old-mansion-1f-9.cmd02
    4 ebit-clear
    begin
        6 cutscene-shot? not while
        yield
    repeat
    begin
        $446 cutscene-cue-reached? not while
        $80808000 $404A6038 14.0 420.732 1 fog
        yield
    repeat
    begin
        $44E cutscene-cue-reached? not while
        yield
    repeat
    begin
        $484 cutscene-cue-reached? not while
        1.0 1.0 40.0 100.0 depth-range
        yield
    repeat
    begin
        $4B0 cutscene-cue-reached? not while
        old-mansion-1f-9.cmd01
        yield
    repeat
    depth-range-off
    begin
        0 cutscene-cue-reached? while
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: old-mansion-1f-9.act07 ( -- )   \ 00408480
    self-wait-done
    7.72 15.08 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-9-refined-magnesia story-flag-set
            1 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-9.act08 ( -- )   \ 004084E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    self-frames-reset
    4 self-wait-frames
    fiona-who $3A -19.0 21.0 0 char-to-xz
    world-frozen state-flag-set
    1 12.0 -10.0 0.0 7.0 event-camera
    6 message   \ "'The Three Spiritual Substances'"
    wait-message
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    self-frames-reset
    4 self-wait-frames
    hand-to-mouth-anim $A self-anim-blend
    7 message   \ "A strange jumble of letters and numbers... I wonder if this title holds some deeper, un..."
    wait-message
    self-wait-anim
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-9.act09 ( -- )   \ 00408540
    self-wait-done
    0 old-mansion-1f-9.cmd00
    3 partner-load
    2 char-start
    $13 3 $FF char-load
    3 char-start
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 cutscene-track 6 action
    3 4 0 char-model-op
    $13 char-activate
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who $74 -32.419 -4.78 -107 char-to-xz
    camera-restart
    3 0 char-remove
    fiona-who $A char-layer
    2 1 object-hidden
    3 1 object-hidden
    4 1 object-hidden
    5 1 object-hidden
    0 part-shown
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-9 register-scripts   \ (its phases and actions, by their names)
