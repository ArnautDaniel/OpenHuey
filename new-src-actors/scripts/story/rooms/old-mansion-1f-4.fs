\ story/rooms/old-mansion-1f-4.fs - the event scripts of room old-mansion-1f-4 ($42; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-4
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ ---- this room's own event bits and script variables ----
0 constant transmuter-remarked         \ (bit 0) its first remark made: "Transmuter Synthesizing two objects together is a task any f"
1 constant in-zone0                    \ (bit 1) someone in zone 0 (the figure's hops count)
2 constant hewie-zone2-noticed         \ (bit 2) Hewie has reacted to zone 2 (once a visit)
3 constant incubator-remarked          \ (bit 3) its first remark made: "Is this some kind of incubator? There is something inside..."
0 constant hops-var                    \ (var 0) the figure's hops (5: away)
1 constant used-item-var               \ (var 1) the item she used (8 .. 10)
2 constant call-frames-var             \ (var 2) the figure's frames (its calls at 21 and 66)

: old-mansion-1f-4.enter ( -- )   \ 004070F0
    room-sounds
    $12 1.0 0 bgm
    om1f-4-flag1 story-flag? not if
        0 part-shown
    else
        1 part-shown
    then
    $1C 3 char-to-tri
    call-frames-var 0 var-set
    3 $1C char-loaded? if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
    then
    0 1 track 4 action
    hunted state-flag-set
    om1f-4-chicken-jerky story-flag? not if
        0 -51.07 1.0 19.59 flicker-sprite
    then
    1 $2300 sound-volume
    $77 hole-floor tri-floor-on
    $48 hole-floor tri-floor-on
    $75 hole-floor tri-floor-on
    $46 hole-floor tri-floor-on
    $52 hole-floor tri-floor-on
    $81 hole-floor tri-floor-on
    2 hole-floor tri-floor-on
    $61 hole-floor tri-floor-on
    $70 hole-floor tri-floor-on
    $3E hole-floor tri-floor-on
    $39 hole-floor tri-floor-on
    0 hole-floor tri-floor-on
    $71 hole-floor tri-floor-on
    $3F hole-floor tri-floor-on
    $4B hole-floor tri-floor-on
    $7A hole-floor tri-floor-on
    $4A hole-floor tri-floor-on
    $79 hole-floor tri-floor-on
    $45 hole-floor tri-floor-on
    $74 hole-floor tri-floor-on
    $5F hole-floor tri-floor-on
    $85 hole-floor tri-floor-on
    $51 hole-floor tri-floor-on
    $80 hole-floor tri-floor-on
    $4C hole-floor tri-floor-on
    $7B hole-floor tri-floor-on
    $3C hole-floor tri-floor-on
    $6F hole-floor tri-floor-on
    $4E hole-floor tri-floor-on
    $7D hole-floor tri-floor-on
    $86 hole-floor tri-floor-on
    $60 hole-floor tri-floor-on
    $54 hole-floor tri-floor-on
    $83 hole-floor tri-floor-on
    6 hole-floor tri-floor-on
    $63 hole-floor tri-floor-on
    $14 hole-floor tri-floor-on
    $6D hole-floor tri-floor-on
    $4D hole-floor tri-floor-on
    $7C hole-floor tri-floor-on
    $18 hole-floor tri-floor-on
    $23 hole-floor tri-floor-on
    $41 hole-floor tri-floor-on
    $72 hole-floor tri-floor-on
    $76 hole-floor tri-floor-on
    $47 hole-floor tri-floor-on
    $78 hole-floor tri-floor-on
    $49 hole-floor tri-floor-on
    $53 hole-floor tri-floor-on
    $82 hole-floor tri-floor-on
    $5A hole-floor tri-floor-on
    $84 hole-floor tri-floor-on
    $3B hole-floor tri-floor-on
;

: old-mansion-1f-4.char-enter ( -- )   \ 00407300
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
;

: old-mansion-1f-4.phase1 ( -- )   \ 00407340
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 0 0 1 chars-area-camera
    4 2 2 1 chars-area-camera
    0 55.0 0.0 -20.0 5 10 1 zone
    self-who 0 char-in-zone? if
        in-zone0 ebit-set
    else
        in-zone0 ebit-clear
    then
    call-frames-var 21 var? if
        $1C 2 6 char-sound
    then
    call-frames-var 66 var? if
        $1C 2 6 char-sound
    then
    1 45.1 0.0 31.88 $1C 18 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 18.0 hewie-look-zone
            then
        then
    then
    2 54.4 0.0 -20.46 $19 16 0 zone
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
    2 54.4 0.0 -20.46 $19 16 0 zone
    hewie-who char-here? 1 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 16.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-1f-4.phase2 ( -- )   \ 00407430
    fiona-who 5 char-in-area? fiona-who 20 $32 char-heading? and if
        2 unhurried offer-action
    then
    om1f-4-chicken-jerky story-flag? not if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    fiona-who 7 char-in-area? fiona-who 90 $3C char-heading? and if
        6 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who 90 $3C char-heading? and if
        7 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who 90 $3C char-heading? and if
        8 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 0 $3C char-heading? and if
        9 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who 0 $3C char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 90 $3C char-heading? and if
        9 unhurried offer-action
    then
    fiona-who $D char-in-area? fiona-who 90 $3C char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $E char-in-area? fiona-who 0 $3C char-heading? and if
        $B unhurried offer-action
    then
    fiona-who $F char-in-area? fiona-who 0 $3C char-heading? and if
        $C unhurried offer-action
    then
    fiona-who $10 char-in-area? fiona-who 0 $3C char-heading? and if
        $D unhurried offer-action
    then
;

: old-mansion-1f-4.act0E ( -- )   \ 00407A20
    4 6 -30.0 10.0 20.0 0 0 sound
    16 wait-frames
    fiona-who seized-sound 5 char-sound
    $3C threat-raise
    1 $FF 8 rumble
    uneasy-2-anim play-out
    0 part-hidden
    1 part-shown
    $42 0 105 $80 5 -1 $68 40.0 creature-place
    $42 0 23 $80 10 -1 $5A 0.0 creature-place
    om1f-4-flag1 story-flag-set
    exit
;

: old-mansion-1f-4.act00 ( -- )   \ 004074F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    $78 wait-frames
    look-close-anim play-out
    5 say   \ "Oh my! It actually transformed..."
    reach-out-anim play-out
    $B item-use
    $C message-param-room
    $C 1 item-give-count
    0 $C item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    got-red-godstone story-flag-set
    reach-back-anim play-out
    om1f-4-flag1 story-flag? not if
        old-mansion-1f-4.act0E
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act01 ( -- )   \ 00407560
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    $78 wait-frames
    look-close-anim play-out
    reach-out-anim play-out
    used-item-var 8 var? if
        8 message-param-room
        0 8 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 9 var? if
        9 message-param-room
        0 9 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 10 var? if
        $A message-param-room
        0 $A item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 12 var? if
        $C message-param-room
        0 $C item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 16 var? if
        $10 message-param-room
        0 $10 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    then then then then then
    reach-back-anim play-out
    2 say   \ "No transformation occurred. I guess it didn't work. Maybe I should try a different item?"
    om1f-4-flag1 story-flag? not if
        old-mansion-1f-4.act0E
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act02 ( -- )   \ 00407680
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $E 38.0 18.0 20 $FFFF 5 self-move-to
    self-wait-done
    transmuter-remarked ebit? not if
        $F 6 fade
        wait-fade
        0 say   \ "Transmuter Synthesizing two objects together is a task any fool could surely comple..."
        transmuter-remarked ebit-set
        $F 7 fade
        wait-fade
    else
        hand-to-mouth-anim $A self-anim-blend
        1 say   \ "A transmuter? I've never heard of such a device before... It appears you can insert..."
        self-wait-anim
        transmuter-remarked ebit-clear
    then
    $245 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act03 ( -- )   \ 004076D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    90 self-turn-angle
    self-wait-done
    3 say
    $F 6 fade
    wait-fade
    4 say
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act04 ( -- )   \ 00407700
    3 $1C char-loaded? not if
        $1C 3 $FF char-load
        3 char-start
        $1C char-activate
        $1C 55.0 0.0 -20.0 90 char-to-xyz
        $1C $9000 1 0 char-anim-hold
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
                $1C 0 6 char-sound
            else
                $1C 1 6 char-sound
            then
            fiona-who shove-hit-sound 5 char-sound
            $1C $9001 0 3 char-anim-hold
            $1C wait-char-anim
            $1C $9000 0 3 char-anim-hold
            call-frames-var 0 var-set
            hops-var 5 var? if
                $1C wait-char-anim
                hops-var 0 var-set
            then
        then
        $1C char-at-motion-event? if
            $1C $9000 0 0 char-anim-hold
            call-frames-var 0 var-set
        else
            call-frames-var var-inc
        then
        1 wait-frames
    again
;

: old-mansion-1f-4.act05 ( -- )   \ 004077C0
    self-wait-done
    -51.07 19.59 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-4-chicken-jerky story-flag-set
            0 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-4.act06 ( -- )   \ 00407820
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    4 wait-frames
    fiona-who $62 6.25 -23.4 173 char-to-xz
    self-who 3 -1 char-camera
    4 wait-frames
    look-close-anim self-anim
    $28 wait-frames
    incubator-remarked ebit? not if
        6 say   \ "Is this some kind of incubator? There is something inside..."
        incubator-remarked ebit-set
    else
        7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
    then
    self-wait-anim
    4 wait-frames
    2 2 player-camera
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-4.act07 ( -- )   \ 00407880
    self-wait-done
    -20.0 -40.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    incubator-remarked ebit? not if
        6 say   \ "Is this some kind of incubator? There is something inside..."
        incubator-remarked ebit-set
    else
        7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act08 ( -- )   \ 004078B0
    self-wait-done
    -48.0 -40.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    incubator-remarked ebit? not if
        6 say   \ "Is this some kind of incubator? There is something inside..."
        incubator-remarked ebit-set
    else
        7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act09 ( -- )   \ 004078E0
    self-wait-done
    -10.0 0.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    incubator-remarked ebit? not if
        6 say   \ "Is this some kind of incubator? There is something inside..."
        incubator-remarked ebit-set
    else
        7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act0A ( -- )   \ 00407910
    self-wait-done
    -40.0 0.0 self-turn-to-xz
    self-wait-done
    om1f-4-flag1 story-flag? not if
        look-close-anim self-anim
        $28 wait-frames
        incubator-remarked ebit? not if
            6 say   \ "Is this some kind of incubator? There is something inside..."
            incubator-remarked ebit-set
        else
            7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
        then
        self-wait-anim
    else
        look-close-anim self-anim
        $28 wait-frames
        8 say   \ "It's empty now. I can't believe that thing inside was alive..."
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act0B ( -- )   \ 00407950
    self-wait-done
    8.0 40.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    incubator-remarked ebit? not if
        6 say   \ "Is this some kind of incubator? There is something inside..."
        incubator-remarked ebit-set
    else
        7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-1f-4.act0C ( -- )   \ 00407980
    om1f-4-flag1 story-flag? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        4 wait-frames
        fiona-who $50 -17.5 26.5 -12 char-to-xz
        self-who 4 -1 char-camera
        4 wait-frames
        look-close-anim self-anim
        $28 wait-frames
        incubator-remarked ebit? not if
            6 say   \ "Is this some kind of incubator? There is something inside..."
            incubator-remarked ebit-set
        else
            7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
        then
        self-wait-anim
        4 wait-frames
        1 1 player-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        look-close-anim self-anim
        $28 wait-frames
        8 say   \ "It's empty now. I can't believe that thing inside was alive..."
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-1f-4.act0D ( -- )   \ 004079F0
    self-wait-done
    -48.0 40.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    incubator-remarked ebit? not if
        6 say   \ "Is this some kind of incubator? There is something inside..."
        incubator-remarked ebit-set
    else
        7 say   \ "It just looks like a clump of meat. It's all fatty and gross."
    then
    self-wait-anim
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-4 register-scripts   \ (its phases and actions, by their names)
