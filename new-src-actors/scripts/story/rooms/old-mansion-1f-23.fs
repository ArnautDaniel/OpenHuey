\ story/rooms/old-mansion-1f-23.fs - the event scripts of room old-mansion-1f-23 ($6F; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-23
USING: room-names story.words story.shared flag-names story-names ;

: old-mansion-1f-23.enter ( -- )   \ 0043A8F0
    room-sounds
    0 0 var-set
    1 0 var-set
    got-mansion-sketch story-flag? not if
        0 1 $14 door-bits
    then
    om1f-23-viride-medallion-found story-flag? om1f-23-viride-medallion story-flag? not and if
        0 0.03 1.0 -85.54 flicker-sprite
    then
    om1f-23-medallion-found story-flag? om1f-23-medallion story-flag? not and if
        1 109.8 -9.0 -94.58 flicker-sprite
    then
    1 $2300 sound-volume
    1 $9D $10000000 nav-tri-flags
    1 $8D $10000000 nav-tri-flags
    1 $2B $10000000 nav-tri-flags
    1 $90 $10000000 nav-tri-flags
    1 $8A $10000000 nav-tri-flags
    1 $C3 $10000000 nav-tri-flags
    1 $87 $10000000 nav-tri-flags
    1 $7C $10000000 nav-tri-flags
    1 $BA $10000000 nav-tri-flags
    1 $9E $10000000 nav-tri-flags
    1 $8F $10000000 nav-tri-flags
    1 $2E $10000000 nav-tri-flags
    1 $92 $10000000 nav-tri-flags
    1 $8C $10000000 nav-tri-flags
    1 $C6 $10000000 nav-tri-flags
    1 $88 $10000000 nav-tri-flags
    1 $C2 $10000000 nav-tri-flags
    1 $86 $10000000 nav-tri-flags
    1 $B8 $10000000 nav-tri-flags
    1 $7A $10000000 nav-tri-flags
    1 $BE $10000000 nav-tri-flags
    1 $80 $10000000 nav-tri-flags
    1 $C0 $10000000 nav-tri-flags
    1 $84 $10000000 nav-tri-flags
    1 $B3 $10000000 nav-tri-flags
    1 $1E $10000000 nav-tri-flags
    1 $9F $10000000 nav-tri-flags
    1 $42 $10000000 nav-tri-flags
    1 $BD $10000000 nav-tri-flags
    1 $7F $10000000 nav-tri-flags
    1 $BB $10000000 nav-tri-flags
    1 $7D $10000000 nav-tri-flags
;

: old-mansion-1f-23.act02 ( -- )   \ 0043AE10
    2 ebit-set
    1 ebit-clear
    $14 0 pvar? if
        $A chance? if
            1 ebit-set
        then
    else $14 1 pvar? if
        $19 chance? if
            1 ebit-set
        then
    else $14 2 pvar? if
        $32 chance? if
            1 ebit-set
        then
    else $14 3 pvar? if
        $4B chance? if
            1 ebit-set
        then
    then then then then
    2 creature-action? if
        1 ebit-set
    then
    1 ebit? if
        0 stalker-who 3 action
    else
        $78 1 summoner-wait
    then
    $14 pvar-inc
    exit
;

: old-mansion-1f-23.char-enter ( -- )   \ 0043AA40
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 0 player-camera
    then then
    0 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        2 2 player-camera
    then then
    1 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            2 ebit-set
            old-mansion-1f-23.act02
        else
            2 ebit-clear
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
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
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
                $19 om1f-23-viride-medallion-found var-set
                $1A om1f-23-viride-medallion var-set
                $1B 0 var-set
                $1C 30 var-set
                $1D 1000 var-set
                $1E -85540 var-set
                $1F 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    om1f-23-medallion-found story-flag? not if
        1 109.8 -10.0 -94.58 $A 5 0 zone
        hewie-who 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 om1f-23-medallion-found var-set
                $1A om1f-23-medallion var-set
                $1B 1 var-set
                $1C 109800 var-set
                $1D -9000 var-set
                $1E -94580 var-set
                $1F 1 var-set
                0 hewie-who $8B action
            then
        then
    then
    6 sound-bank-loaded? if
        1 var-inc
        1 30 var? if
            1 0 var-set
            0 0 var? if
                $40000003 6 79.45 0.0 -93.49 0 0 sound
            else 0 1 var? if
                $40000004 6 79.45 0.0 -93.49 0 0 sound
            else 0 2 var? if
                $40000005 6 79.45 0.0 -93.49 0 0 sound
            else 0 3 var? if
                $40000006 6 79.45 0.0 -93.49 0 0 sound
            then then then then
            0 var-inc
            0 4 var? if
                0 0 var-set
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
    0 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    fiona-who $7E 5 char-sound
    $8003 $A self-anim-blend
    self-wait-anim
    0 self-noclip
    0 ebit? not if
        stalker-who action-end
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-23.act00 ( -- )   \ 0043AD20
    stalkers-stay state-flag-set
    2 ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    fiona-who char-file-use
    $20 $8004 5 105.64 -103.65 -90 self-walk-anim
    self-wait-done
    1 self-noclip
    1 self-scripted
    fiona-who $7E 5 char-sound
    $8005 $A self-anim-blend8
    self-wait-anim
    $8001 $A self-anim-blend
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 ebit-clear
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        fiona-who 1 6 char-sound
    then
    self-who 3 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? 0 ebit? or if
                in-panic? not if
                    fiona-who $43 5 char-sound
                then
                ['] old-mansion-1f-23.act01 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                2 ebit? stalker-who char-here? not and if
                    2 ebit-clear
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
            $8002 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            fiona-who $7E 5 char-sound
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
        $8000 self-anim
        self-wait-anim
        4 stalker-who 0 char-model-op
    else
        $1601 self-anim
        self-wait-anim
    then
    0 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-1f-23.act04 ( -- )   \ 0043AEC0
    self-wait-done
    0.03 -85.54 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $74 message-param-room
        $74 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-23-viride-medallion story-flag-set
            0 effect-remove
            $74 1 item-give-count
            0 $74 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-23.act05 ( -- )   \ 0043AF20
    self-wait-done
    109.8 -94.58 self-turn-to-xz
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
            om1f-23-medallion story-flag-set
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

: old-mansion-1f-23.act06 ( -- )   \ 0043AF80
    self-wait-done
    127.0 -125.0 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    got-mansion-sketch story-flag-set
    0 0 $14 door-bits
    $1E message-param-room
    $1E 1 item-give-count
    0 $1E item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-23.phase5 ( -- )   \ 0047AEBC
;

\ ---- registered ----
old-mansion-1f-23 register-scripts   \ (its phases and actions, by their names)
