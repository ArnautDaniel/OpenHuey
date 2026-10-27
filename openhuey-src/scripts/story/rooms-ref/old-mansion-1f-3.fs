\ story/rooms/old-mansion-1f-3.fs - the event scripts of room old-mansion-1f-3 ($41; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-3
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant in-zone0                    \ (bit 0) someone in zone 0 (the figure's hops count)
1 constant hewie-zone1-noticed         \ (bit 1) Hewie has reacted to zone 1 (once a visit)
2 constant hewie-zone2-noticed         \ (bit 2) Hewie has reacted to zone 2 (once a visit)
0 constant hops-var                    \ (var 0) the figure's hops (5: away)
1 constant call-frames-var             \ (var 1) the figure's frames (its calls at 7 and 47)
0 constant figure-hit1-sound           \ (snd 0) the figure as it strikes her / is struck
1 constant figure-hit2-sound           \ (snd 1) the figure as it strikes her / is struck
2 constant figure-call1-sound          \ (snd 2) the figure's call (now and then)

: old-mansion-1f-3.enter ( -- )   \ 0041CA90
    room-sounds
    $1C $60 char-to-tri
    call-frames-var 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 6.38 0.0 -112.79 170 char-to-xyz
        $1C $9002 1 0 char-anim-hold
    then
    0 1 track 1 action
    hunted state-flag-set
    om2f-3-flag1 story-flag? if
        hewie-who char-here? hewie-who 0 char-in-nav-group? and if
            hewie-who $13 char-to-tri
        then
        0 part-shown
        0 barred-floor floor-on
    then
    om1f-3-medallion story-flag? not if
        0 -10.64 9.5 -116.64 flicker-sprite
    then
    om1f-3-refined-antimony-found story-flag? om1f-3-refined-antimony story-flag? not and if
        1 -19.52 1.0 -13.68 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-1f-3.char-enter ( -- )   \ 0041CB20
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        3 -1 player-camera
    then
    0 3 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 1 player-camera
    then
    1 1 1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        2 2 player-camera
    then
    2 2 2 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $1A 160 char-to-tri-facing
            1 1 player-camera
            0 fiona-who 2 action
        then
    then
;

: old-mansion-1f-3.phase1 ( -- )   \ 0041CC00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 1 1 1 chars-area-camera
    8 3 -1 1 chars-area-camera
    9 3 -1 1 chars-area-camera
    $A 0 0 1 chars-area-camera
    $B 0 0 1 chars-area-camera
    $C 2 2 1 chars-area-camera
    fiona-who 3 char-entered-area? fiona-who 5 char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    0 6.38 0.0 -112.79 5 10 1 zone
    self-who 0 char-in-zone? if
        in-zone0 ebit-set
    else
        in-zone0 ebit-clear
    then
    call-frames-var 7 var? if
        $1C figure-call1-sound 6 char-sound
    then
    call-frames-var 47 var? if
        $1C figure-call1-sound 6 char-sound
    then
    om1f-3-refined-antimony-found story-flag? not if
        4 -19.52 0.0 -13.68 $A 5 0 zone
        hewie-who 4 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-3-refined-antimony-found var-set
                taken-flag-var om1f-3-refined-antimony var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var -19520 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var -13680 var-set
                hewie-zone-var 4 var-set
                0 hewie-who $8B action
            then
        then
    then
    1 -25.44 0.0 -54.79 $C 10 0 zone
    hewie-zone1-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 1 9 char-zone-bits? if
                    hewie-zone1-noticed ebit-set
                    $1E chance? if
                        hewie-zone-var 1 var-set
                        0 hewie-who $89 action
                    then
                then
            then
        then
    then
    2 7.31 0.0 -110.05 $1E 6 0 zone
    hewie-zone2-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    hewie-zone2-noticed ebit-set
                    $50 chance? if
                        hewie-zone-var 2 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    2 7.31 0.0 -110.05 $1E 6 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 6.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-3.phase2 ( -- )   \ 0041CD90
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            0 always offer-action
        then
    then
    fiona-who $D char-in-area? fiona-who -70 $3C char-heading? and if
        5 unhurried offer-action
    then
    om1f-3-medallion story-flag? not if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            3 calm offer-action
        then
    then
    om1f-3-refined-antimony-found story-flag? om1f-3-refined-antimony story-flag? not and if
        4 1 5 5 0 zone-at-effect
        fiona-who 4 3 char-zone-bits? if
            4 calm offer-action
        then
    then
;

: old-mansion-1f-3.phase3 ( -- )   \ 0041CDF0
    -10.0 12.0 -89.0 6.0 12.0 -89.0 -10.0 0.0 -89.0 6.0 0.0 -89.0 lights-doorway
    -37.0 8.0 -83.5 -31.0 8.0 -83.5 -37.0 0.0 -83.5 -31.0 0.0 -83.5 lights-doorway
    -37.0 17.0 -83.5 -31.0 17.0 -83.5 -37.0 8.0 -83.5 -31.0 8.0 -83.5 lights-doorway
    -40.0 8.0 -83.5 -34.0 8.0 -83.5 -40.0 0.0 -83.5 -34.0 0.0 -83.5 lights-doorway
    -40.0 17.0 -83.5 -34.0 17.0 -83.5 -40.0 8.0 -83.5 -34.0 8.0 -83.5 lights-doorway
;

: old-mansion-1f-3.phase5 ( -- )   \ 0041CEE8
    fiona-who 2 char-in-area? if
        $1C action-end
        $1C char-done
    then
;

: old-mansion-1f-3.act00 ( -- )   \ 0041CF00
    self-wait-done
    stalker-who self-touching? not if
        $56 self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $56 char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                $14 wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $56 door-unlock
                    $8008 show-message   \ "You unlocked the door."
                    20 self-idle-anim
                    $14 wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: old-mansion-1f-3.act01 ( -- )   \ 0041CF40
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 6.38 0.0 -112.79 170 char-to-xyz
        $1C $9002 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        in-zone0 ebit? if
            call-frames-var 0 var-set
            hops-var var-inc
            hops-var 3 var? if
                $21 chance? if
                    hops-var 5 var-set
                then
            then
            hops-var 4 var? if
                $32 chance? if
                    hops-var 5 var-set
                then
            then
            hops-var 5 var? if
                0 panic-stage? if
                    1 panic-stage
                else
                    $F threat-raise
                then
                stalker-who 0 stalker-command
                $1C figure-hit1-sound 6 char-sound
            else
                $1C figure-hit2-sound 6 char-sound
            then
            fiona-who shove-hit-sound 5 char-sound
            $1C $9003 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9002 0 3 char-anim-hold
            call-frames-var 0 var-set
            hops-var 5 var? if
                $1C wait-char-anim
                hops-var 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9002 0 0 char-anim-hold
            call-frames-var 0 var-set
        else
            call-frames-var var-inc
        then
        1 wait-frames
    again
;

: old-mansion-1f-3.act02 ( -- )   \ 0041D000
    self-wait-done
    fiona-recover
    2 panic-stage
    stalker-who 3 stalker-command
    lying-anim 0 self-anim-blend
    yield
    $F $41 fade
    fiona.anims:get-up-anim play-out
    -1 self-idle-anim
    7 wait-frames
    wait-fade
    self-idle-or-end
;

: old-mansion-1f-3.act03 ( -- )   \ 0041D020
    self-wait-done
    -10.64 -116.64 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-3-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-3.act04 ( -- )   \ 0041D080
    self-wait-done
    -19.52 -13.68 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-3-refined-antimony story-flag-set
            1 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-3.act05 ( -- )   \ 0041D0E0
    self-wait-done
    2.0 -125.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    $28 wait-frames
    0 say   \ "A cart with some medicine on it. There may be some medicine here that I can use, but al..."
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-3 register-scripts   \ (its phases and actions, by their names)
