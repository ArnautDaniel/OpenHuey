\ story/rooms/old-mansion-2f-5.fs - the event scripts of room old-mansion-2f-5 ($58; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-5
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant in-zone2                    \ (bit 0) someone in zone 2 (the figure's hops count)
1 constant found-her                   \ (bit 1) he has found her hiding: she must flee (the avoid prompt)
2 constant will-find-her               \ (bit 2) he will find her hiding (by chance, as he searches: more likely by the progress variable)
3 constant came-while-hidden           \ (bit 3) the stalker came in while she was hiding
4 constant hewie-zone3-noticed         \ (bit 4) Hewie has reacted to zone 3 (once a visit)
0 constant hops-var                    \ (var 0) the figure's hops (5: away)
1 constant still-var                   \ (var 1) the figure still (0) or hopping
3 constant sound-step-var              \ (var 3) which of its sounds next (0..3)
4 constant sound-frames-var            \ (var 4) frames to the next sound (30)
0 constant figure-hit1-sound           \ (snd 0) the figure as it strikes her / is struck
1 constant figure-hit2-sound           \ (snd 1) the figure as it strikes her / is struck
2 constant figure-hit3-sound           \ (snd 2) the figure as it strikes her / is struck

: old-mansion-2f-5.enter ( -- )   \ 0040F910
    room-sounds
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    om2f-5-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
    then
    om2f-5-breakable2-broken story-flag? not if
        1 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        1 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
        om2f-5-medallion story-flag? not if
            0 10.5 1.0 -28.0 flicker-sprite
        then
    then
    0 3 0.0 0.5 0.312 0.187 zone-rect
    $1C $17F char-to-tri
    still-var 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
    then
    0 1 track 0 action
    hunted state-flag-set
    om3f-1-flag2 story-flag? if
        om2f-5-flag1 story-flag? not if
            om2f-5-flag1 story-flag-set
            $58 0 291 $81 3 -1 0 0.0 creature-place
            $58 0 426 $81 3 -1 0 0.0 creature-place
            $58 0 205 $81 3 -1 0 0.0 creature-place
        then
    then
    om2f-5-viride-medallion-found story-flag? om2f-5-viride-medallion story-flag? not and if
        1 7.38 1.0 20.58 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-2f-5.act03 ( -- )   \ 0040FEC0
    came-while-hidden ebit-set
    will-find-her ebit-clear
    $15 0 pvar? if
        $A chance? if
            will-find-her ebit-set
        then
    else $15 1 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else $15 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $15 3 pvar? if
        $4B chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    will-find-her ebit? if
        0 stalker-who 4 action
    else
        $78 1 summoner-wait
    then
    $15 pvar-inc
    exit
;

: old-mansion-2f-5.char-enter ( -- )   \ 0040FA20
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 6 exit-taken? and if
        player-who 6 char-to-exit
        1 -1 player-camera
    then
    6 1 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            old-mansion-2f-5.act03
        else
            came-while-hidden ebit-clear
        then
    then
;

: old-mansion-2f-5.phase1 ( -- )   \ 0040FAB0
    0 exit-usable? if
        0 exit-check
    then
    6 exit-usable? if
        6 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        6 exit-prepare
    then
    om2f-5-breakable1-broken story-flag? not if
        0 -10.3 0.0 -93.8 5 8 1 zone
        self-who 0 char-in-zone? if
            om2f-5-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            -10.3 0.0 -93.8 0 -2145378256 0 0.0 shards-at
            $88 5 -10.3 0.0 -93.8 0 0 sound
            $40 $E1 noise
        then
    then
    2 -3.0 0.0 43.0 5 10 1 zone
    self-who 2 char-in-zone? if
        in-zone2 ebit-set
    else
        in-zone2 ebit-clear
    then
    still-var 1 var? if
        $1C figure-hit3-sound 6 char-sound
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    hewie-zone3-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 3 9 char-zone-bits? if
                    hewie-zone3-noticed ebit-set
                    $50 chance? if
                        hewie-zone-var 3 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    3 -2.18 0.0 42.94 $20 11 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 11.0 hewie-look-zone
            then
        then
    then
    om2f-5-viride-medallion-found story-flag? not if
        4 7.38 0.0 20.58 $A 5 0 zone
        hewie-who 4 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om2f-5-viride-medallion-found var-set
                taken-flag-var om2f-5-viride-medallion var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var 7380 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 20580 var-set
                hewie-zone-var 4 var-set
                0 hewie-who $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
            else sound-step-var 1 var? if
            else sound-step-var 2 var? if
            else sound-step-var 3 var? if
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
    then
;

: old-mansion-2f-5.phase2 ( -- )   \ 0040FC70
    fiona-who 6 char-in-area? fiona-who 90 $3C char-heading? and if
        stalker-who char-here? not if
            1 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    om2f-5-breakable2-broken story-flag? not if
        1 10.5 0.0 -28.0 5 8 1 zone
        self-who 1 char-in-zone? if
            om2f-5-breakable2-broken story-flag-set
            1 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            10.5 0.0 -28.0 0 -2145378256 0 0.0 shards-at
            $88 5 10.5 0.0 -28.0 0 0 sound
            $40 $1AD noise
            0 10.5 1.0 -28.0 flicker-sprite
        then
    else om2f-5-medallion story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then then
    om2f-5-viride-medallion-found story-flag? om2f-5-viride-medallion story-flag? not and if
        4 1 5 5 0 zone-at-effect
        fiona-who 4 3 char-zone-bits? if
            6 calm offer-action
        then
    then
;

: old-mansion-2f-5.act00 ( -- )   \ 0040FD30
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C -3.0 0.0 43.0 -45 char-to-xyz
        $1C $9004 1 0 char-anim-hold
        $1C 0 char-visible
    then
    begin
        in-zone2 ebit? if
            still-var 0 var-set
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
            $1C $9005 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9004 0 3 char-anim-hold
            still-var 0 var-set
            hops-var 5 var? if
                $1C wait-char-anim
                hops-var 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9004 0 0 char-anim-hold
            still-var 0 var-set
        else
            still-var var-inc
        then
        1 wait-frames
    again
;

: old-mansion-2f-5.act02 ( -- )   \ 0040FE90
    found-her ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who slide-sound 5 char-sound
    3 room-motion $A blend-out
    0 self-noclip
    found-her ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-5.act01 ( -- )   \ 0040FDF0
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    fiona-who char-file-use
    $106 $8004 5 -4.6 -231.1 180 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who slide-sound 5 char-sound
    5 room-motion $A self-anim-blend8
    self-wait-anim
    1 room-motion $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    found-her ebit-clear
    0 avoid-prompt
    self-who 2 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] old-mansion-2f-5.act02 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                came-while-hidden ebit? stalker-who char-here? not and if
                    came-while-hidden ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] old-mansion-2f-5.act02 goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            2 room-motion 5 self-anim-blend8
            5 wait-frames
            fiona-who slide-sound 5 char-sound
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-2f-5.act04 ( -- )   \ 0040FF10
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 6 char-heading-for? 6 exit-door-open? not and if
        6 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        stalker-who 1 char-file-load
    then
    $106 -4.6 -222.0 180 $FFFF 5 self-move-to
    self-wait-done
    3 self-is? $22 self-is? or if
        stalker-who char-file-use
        4 stalker-who 1 char-model-op
        0 room-motion play-out
        4 stalker-who 0 char-model-op
    else
        there-anim play-out
    then
    found-her ebit-set
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-2f-5.act05 ( -- )   \ 0040FF70
    self-wait-done
    10.5 -28.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om2f-5-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-2f-5.act06 ( -- )   \ 0040FFD0
    self-wait-done
    7.38 20.58 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om2f-5-viride-medallion story-flag-set
            1 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-2f-5.phase5 ( -- )   \ 0047ABD0
    $1C action-end
    $1C char-done
;

\ ---- registered ----
old-mansion-2f-5 register-scripts   \ (its phases and actions, by their names)
