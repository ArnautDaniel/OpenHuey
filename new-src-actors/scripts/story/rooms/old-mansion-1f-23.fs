\ story/rooms/old-mansion-1f-23.fs - the event scripts of room old-mansion-1f-23 ($6F; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-23
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits and script variables ----
0 constant found-her                   \ (bit 0) he has found her hiding: she must flee (the avoid prompt)
1 constant will-find-her               \ (bit 1) he will find her hiding (by chance, as he searches: more likely by the progress variable)
2 constant came-while-hidden           \ (bit 2) the stalker came in while she was hiding
0 constant sound-step-var              \ (var 0) which of its sounds next (0..3)
1 constant sound-frames-var            \ (var 1) frames to the next sound (30)

: old-mansion-1f-23.enter ( -- )   \ 0043A8F0
    room-sounds
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    got-mansion-sketch story-flag? not if
        0 part-shown
    then
    om1f-23-viride-medallion-found story-flag? om1f-23-viride-medallion story-flag? not and if
        0 0.03 1.0 -85.54 flicker-sprite
    then
    om1f-23-medallion-found story-flag? om1f-23-medallion story-flag? not and if
        1 109.8 -9.0 -94.58 flicker-sprite
    then
    1 $2300 sound-volume
    $9D hole-floor tri-floor-on
    $8D hole-floor tri-floor-on
    $2B hole-floor tri-floor-on
    $90 hole-floor tri-floor-on
    $8A hole-floor tri-floor-on
    $C3 hole-floor tri-floor-on
    $87 hole-floor tri-floor-on
    $7C hole-floor tri-floor-on
    $BA hole-floor tri-floor-on
    $9E hole-floor tri-floor-on
    $8F hole-floor tri-floor-on
    $2E hole-floor tri-floor-on
    $92 hole-floor tri-floor-on
    $8C hole-floor tri-floor-on
    $C6 hole-floor tri-floor-on
    $88 hole-floor tri-floor-on
    $C2 hole-floor tri-floor-on
    $86 hole-floor tri-floor-on
    $B8 hole-floor tri-floor-on
    $7A hole-floor tri-floor-on
    $BE hole-floor tri-floor-on
    $80 hole-floor tri-floor-on
    $C0 hole-floor tri-floor-on
    $84 hole-floor tri-floor-on
    $B3 hole-floor tri-floor-on
    $1E hole-floor tri-floor-on
    $9F hole-floor tri-floor-on
    $42 hole-floor tri-floor-on
    $BD hole-floor tri-floor-on
    $7F hole-floor tri-floor-on
    $BB hole-floor tri-floor-on
    $7D hole-floor tri-floor-on
;

: old-mansion-1f-23.act02 ( -- )   \ 0043AE10
    came-while-hidden ebit-set
    will-find-her ebit-clear
    $14 0 pvar? if
        $A chance? if
            will-find-her ebit-set
        then
    else $14 1 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else $14 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $14 3 pvar? if
        $4B chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    will-find-her ebit? if
        0 stalker-who 3 action
    else
        $78 1 summoner-wait
    then
    $14 pvar-inc
    exit
;

: old-mansion-1f-23.char-enter ( -- )   \ 0043AA40
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        2 2 player-camera
    then
    1 2 2 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        0 0 player-camera
    then
    2 0 0 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            old-mansion-1f-23.act02
        else
            came-while-hidden ebit-clear
        then
    then
;

: old-mansion-1f-23.phase1 ( -- )   \ 0043AB10
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    7 0 0 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 1 1 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    fiona-who 3 char-entered-area? fiona-who 5 char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    om1f-23-viride-medallion-found story-flag? not if
        0 0.03 0.0 -85.54 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-23-viride-medallion-found var-set
                taken-flag-var om1f-23-viride-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 30 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var -85540 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    om1f-23-medallion-found story-flag? not if
        1 109.8 -10.0 -94.58 $A 5 0 zone
        hewie-who 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-23-medallion-found var-set
                taken-flag-var om1f-23-medallion var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var 109800 var-set
                sparkle-y-var -9000 var-set
                sparkle-z-var -94580 var-set
                hewie-zone-var 1 var-set
                0 hewie-who $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                $40000003 6 79.45 0.0 -93.49 0 0 sound
            else sound-step-var 1 var? if
                $40000004 6 79.45 0.0 -93.49 0 0 sound
            else sound-step-var 2 var? if
                $40000005 6 79.45 0.0 -93.49 0 0 sound
            else sound-step-var 3 var? if
                $40000006 6 79.45 0.0 -93.49 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
    then
;

: old-mansion-1f-23.phase2 ( -- )   \ 0043ACB0
    fiona-who $B char-in-area? fiona-who 0 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    fiona-who $C char-in-area? fiona-who -45 $3C char-heading? and if
        stalker-who char-here? not if
            0 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    got-mansion-sketch story-flag? not if
        fiona-who $D char-in-area? fiona-who 45 $32 char-heading? and if
            6 calm offer-action
        then
    then
    om1f-23-viride-medallion-found story-flag? om1f-23-viride-medallion story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            4 calm offer-action
        then
    then
    om1f-23-medallion-found story-flag? om1f-23-medallion story-flag? not and if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then
;

: old-mansion-1f-23.act01 ( -- )   \ 0043ADE0
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

: old-mansion-1f-23.act00 ( -- )   \ 0043AD20
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    fiona-who char-file-use
    $20 $8004 5 105.64 -103.65 -90 self-walk-anim
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
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        fiona-who 1 6 char-sound
    then
    self-who 3 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] old-mansion-1f-23.act01 goto
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
                ['] old-mansion-1f-23.act01 goto
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

: old-mansion-1f-23.act03 ( -- )   \ 0043AE60
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    3 self-is? $22 self-is? or if
        stalker-who 1 char-file-load
    then
    $20 113.0 -103.65 -90 $FFFF 5 self-move-to
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

: old-mansion-1f-23.act04 ( -- )   \ 0043AEC0
    self-wait-done
    0.03 -85.54 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-23-viride-medallion story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-23.act05 ( -- )   \ 0043AF20
    self-wait-done
    109.8 -94.58 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-23-medallion story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-23.act06 ( -- )   \ 0043AF80
    self-wait-done
    127.0 -125.0 self-turn-to-xz
    self-wait-done
    reach-out-anim play-out
    4 wait-frames
    got-mansion-sketch story-flag-set
    0 part-hidden
    $1E message-param-room
    $1E 1 item-give-count
    0 $1E item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    self-idle-or-end
;

: old-mansion-1f-23.phase5 ( -- )   \ 0047AEBC
;

\ ---- registered ----
old-mansion-1f-23 register-scripts   \ (its phases and actions, by their names)
