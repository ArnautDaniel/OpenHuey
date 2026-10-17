\ story/rooms/old-mansion-2f-2.fs - the event scripts of room old-mansion-2f-2 ($4A; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-2
USING: room-names story.words story.shared flag-names story-names floor-flags ;

: old-mansion-2f-2.enter ( -- )   \ 004086A0
    1 both-halves floor-on
    om2f-2-flag1 story-flag? not if
        hewie-who $56 char-in-room? 119 hewie-action? and if
            $4A 0 65 hewie-to-room
            0 hewie-who 4 action
            2 ebit-set
        then
    else
        0 part-shown
        2 1 object-hidden
    then
    om2f-4-viride-medallion story-flag? not if
        0 5.64 9.5 5.58 flicker-sprite
    then
    om2f-2-medallion-found story-flag? om2f-2-medallion story-flag? not and if
        1 -49.36 1.0 -47.3 flicker-sprite
    then
;

: old-mansion-2f-2.char-enter ( -- )   \ 00408700
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            3 2 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        3 2 player-camera
    then then
    0 3 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            4 3 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        4 3 player-camera
    then then
    2 4 3 area-camera
;

: old-mansion-2f-2.phase1 ( -- )   \ 00408780
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    $B 3 2 1 chars-area-camera
    $C 4 3 1 chars-area-camera
    fiona-who 9 char-entered-area? if
        0 exit-prepare
    then
    fiona-who $A char-entered-area? if
        2 exit-prepare
    then
    om2f-2-flag1 story-flag? not if
        fiona-who 0 char-in-area? fiona-who 0 $50 char-heading? and if
            1 control-action? 2 ebit? and stalker-who char-here? not and fiona-who 0 char-action? and if
                0 fiona-who 1 action
            then
        then
    then
    om2f-2-medallion-found story-flag? not if
        3 -49.36 0.0 -47.3 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om2f-2-medallion-found var-set
                $1A om2f-2-medallion var-set
                $1B 1 var-set
                $1C -49360 var-set
                $1D 1000 var-set
                $1E -47300 var-set
                $1F 3 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: old-mansion-2f-2.phase2 ( -- )   \ 00408840
    2 game-mode? not in-panic? not and if
        om2f-2-flag1 story-flag? not if
            fiona-who 0 char-before-door? if
                5 always offer-action
            then
        then
    then
    om2f-2-medallion-found story-flag? om2f-2-medallion story-flag? not and if
        3 1 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            7 calm offer-action
        then
    then
;

: old-mansion-2f-2.phase5 ( -- )   \ 00408880
    om2f-2-flag1 story-flag? not hewie-who char-here? and 2 ebit? and if
        $56 0 -1 hewie-to-room
        18000 2 hewie-anim
    then
;

: old-mansion-2f-2.act00 ( -- )   \ 0047AB80
    self-idle-or-end
;

: old-mansion-2f-2.act01 ( -- )   \ 004088A0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $C02 self-anim
    fiona-who 1 char-wait-motion
    fiona-who $31 5 char-sound
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    $F $44 fade
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    world-held state-flag-set
    50 hewie-trust
    0 part-shown
    2 1 object-hidden
    0 self-idle-anim
    fiona-who $CB -38.75 -31.7 0 char-to-xz
    camera-restart
    $47 door-unlock
    om2f-2-flag1 story-flag-set
    2 ebit-clear
    $57 0 -1 hewie-to-room
    $F $41 fade
    wait-fade
    $8283 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-2.act02 ( -- )   \ 0047AB84
    self-idle-or-end
;

: old-mansion-2f-2.act03 ( -- )   \ 0047AB88
    self-idle-or-end
;

: old-mansion-2f-2.act04 ( -- )   \ 00408980
    self-wait-done
    0 self-doorway-fade
    begin
        0 0 var? if
            hewie-bark
            self-wait-done
            $32 chance? if
                0 90 var-set
            else $32 chance? if
                0 150 var-set
            else
                0 30 var-set
            then then
        else
            0 var-dec
            yield
        then
    again
;

: old-mansion-2f-2.act05 ( -- )   \ 004089B0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    2 ebit? not if
        1 ebit? not if
            0 self-through-exit
            self-wait-done
            $609 self-anim
            self-wait-anim
            -1 self-idle-anim
            self-frames-reset
            7 self-wait-frames
            self-frames-reset
            8 self-wait-frames
            fiona-who $CB -38.0 -30.0 30 char-to-xz
            5 4 player-camera
            self-frames-reset
            4 self-wait-frames
            0 message   \ "It won't budge. It appears to be blocked by something on the other side of the..."
            wait-message
            1 ebit-set
        else
            $CB -38.0 -30.0 30 $FFFF 5 self-move-to
            self-wait-done
            5 4 player-camera
            self-frames-reset
            4 self-wait-frames
            1 message   \ "I can just barely see inside from this tiny window... There is a huge furnace i..."
            wait-message
        then
        self-frames-reset
        4 self-wait-frames
        3 2 player-camera
    else
        $CB -38.0 -30.0 30 $FFFF 5 self-move-to
        self-wait-done
        2 message   \ "Hey! Hewie is inside!"
        wait-message
        0 0 var-set
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-2.act06 ( -- )   \ 0047AB8C
    self-idle-or-end
;

: old-mansion-2f-2.act07 ( -- )   \ 00408A60
    self-wait-done
    -49.36 -47.3 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-2-medallion story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-2.act08 ( -- )   \ 00408AC0
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-2 register-scripts   \ (its phases and actions, by their names)
