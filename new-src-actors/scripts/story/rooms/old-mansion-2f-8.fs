\ story/rooms/old-mansion-2f-8.fs - the event scripts of room old-mansion-2f-8 ($5C; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-8
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ room 0x5C (Room5C_Cmd00_ptmf): the dial (+0x7C, 0..1) from script variable 0 by byte 3: 0
\ 0x2B..0x38 (/ 13, +0x74 0 / +0x78 1), 1 0xC..0x20 (/ 20), 2 7..0x12 (/ 11) (+0x74 1 / +0x78 0)
: old-mansion-2f-8.cmd00 ( b0 -- )   \ (the curtain ("Curtain") by event variable 0 in b0's range:
    \ 0 opening over $2B..$38, 1 closing over $C..$20, 2 over 7..$12)
    {: b0 | v :}  s" Curtain" object-here? 0= if  exit  then  0 var @ to v
    b0 case
        0 of  v $2B max $38 min $2B - s>f 13e f/  0 1 s" Curtain" morph!  endof
        1 of  v $C max $20 min $C - s>f 20e f/  1 0 s" Curtain" morph!  endof
        2 of  v 7 max $12 min 7 - s>f 11e f/  1 0 s" Curtain" morph!  endof
    endcase ;
\ room 0x5C (Room5C_Cond00_ptmf): the first creature within 3 of (59.1, 1.43) is put away with
\ an effect (CreatureVanish_vtable) above it - blue (+0x1571 below 0x12) or red - and its action
\ 0x8B
: old-mansion-2f-8.cond00? ( -- flag )  59.1e 1.43e 3e put-away-near ;   \ (Room5C_Cond00: one within 3 put away)

: old-mansion-2f-8.enter ( -- )   \ 00410F70
    room-sounds
    1 0 var-set
    2 0 var-set
    $5A door-not-closed-off? not if
        2 part-shown
        1 solid-floor floor-on
    then
    om2f-8-flag1 story-flag? if
        3 part-shown
    then
    2 48.8 10.3 -50.7 0 flame-in
    1 $2300 sound-volume
;

: old-mansion-2f-8.act08 ( -- )   \ 00411890
    stalker-who camera-follow
    2 ebit-clear
    $16 0 pvar? if
        $A chance? if
            2 ebit-set
        then
    else $16 1 pvar? if
        $19 chance? if
            2 ebit-set
        then
    else $16 2 pvar? if
        $32 chance? if
            2 ebit-set
        then
    else $16 3 pvar? if
        $4B chance? if
            2 ebit-set
        then
    then then then then
    2 creature-action? if
        2 ebit-set
    then
    2 ebit? if
        0 stalker-who 9 action
    else
        $78 1 summoner-wait
    then
    $16 pvar-inc
    exit
;

: old-mansion-2f-8.char-enter ( -- )   \ 00410FB0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 1 player-camera
    then then
    0 1 1 area-camera
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
            4 -1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        4 -1 player-camera
    then then
    2 4 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        0 0 player-camera
    then then
    3 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 4 exit-taken? and if
            fiona-who 4 char-to-exit
            5 3 player-camera
        then
    else hewie-who self-is? 4 exit-taken? and if
        hewie-who 4 char-to-exit
        5 3 player-camera
    then then
    4 5 3 area-camera
    om2f-8-camomile story-flag? not if
        0 48.0 8.0 -53.0 flicker-sprite
    then
    om2f-8-lavender story-flag? not if
        1 53.1 12.0 -26.2 flicker-sprite
    then
    fiona-who self-is? if
        found-file-234 story-flag? not if
            0.0 sound-volume-scale
            0 fiona-who 0 action
        then
        $80 exit-taken? if
            fiona-who $B 55.0 1.0 90 char-to-xz
            3 -1 player-camera
            0 fiona-who 2 action
        then
        $FE exit-taken? if
            fiona-who $D7 5.5 -116.0 0 char-to-xz
            0 0 player-camera
            0 fiona-who 5 action
        then
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            3 ebit-set
            old-mansion-2f-8.act08
        else
            3 ebit-clear
        then
    then
;

: old-mansion-2f-8.phase1 ( -- )   \ 00411180
    6 ebit? not if
        6 sound-bank-loaded? if
            $40000008 6 5.0 18.0 -125.0 0 0 sound
            om2f-12-flag1 story-flag? not if
                $40000009 6 -120.0 10.0 -105.0 0 0 sound
            then
            6 ebit-set
        then
    else
        $C0000008 6 5.0 18.0 -125.0 0 0 sound
        om2f-12-flag1 story-flag? not if
            $C0000009 6 -120.0 10.0 -105.0 0 0 sound
        then
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    $5A door-not-closed-off? if
        2 exit-usable? if
            2 exit-check
        then
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 2 2 1 chars-area-camera
    $10 0 0 1 chars-area-camera
    $11 3 -1 1 chars-area-camera
    $12 3 -1 1 chars-area-camera
    $13 4 -1 1 chars-area-camera
    $14 0 0 1 chars-area-camera
    $15 5 3 1 chars-area-camera
    fiona-who 5 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 8 char-entered-area? fiona-who 9 char-entered-area? or fiona-who $A char-entered-area? or if
        3 exit-prepare
    then
    fiona-who $B char-entered-area? if
        4 exit-prepare
    then
    0 ebit? not if
        om2f-8-flag1 story-flag? not if
            3 camera-mode? 4 camera-mode? or if
                old-mansion-2f-8.cond00? if
                    0 ebit-set
                    panic-eased
                    fiona-who 0 char-action? if
                        0 fiona-who 1 action-force
                    else
                        1 fiona-who 1 action-force
                    then
                then
            then
        then
    then
    0 141.32 0.0 -60.6 $14 21 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 21.0 hewie-look-zone
            then
        then
    then
    1 59.37 0.0 0.93 $14 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    2 5.52 0.0 -118.99 $F 18 0 zone
    4 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 2 var-set
                        0 hewie-who $88 action
                    then
                then
            then
        then
    then
    2 5.52 0.0 -118.99 $F 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 18.0 hewie-look-zone
            then
        then
    then
    5 2 8 -4 0 zone-at-effect
    fiona-who 5 3 char-zone-bits? fiona-who 5 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 5 3 char-zone-bits? hewie-who 5 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 5 3 char-zone-bits? stalker-who 5 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000003 6 45.32 0.0 23.5 0 0 sound
            else 1 1 var? if
                $40000004 6 45.32 0.0 23.5 0 0 sound
            else 1 2 var? if
                $40000005 6 45.32 0.0 23.5 0 0 sound
            else 1 3 var? if
                $40000006 6 45.32 0.0 23.5 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: old-mansion-2f-8.phase2 ( -- )   \ 00411430
    fiona-who $16 $32 char-faces-area? if
        3 unhurried offer-action
    then
    fiona-who $17 char-in-area? fiona-who 90 $32 char-heading? and if
        4 unhurried offer-action
    then
    fiona-who $18 char-in-area? fiona-who 90 $3C char-heading? and if
        stalker-who char-here? not if
            6 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    fiona-who $19 char-in-area? fiona-who -15 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    fiona-who $1A char-in-area? fiona-who 45 $3C char-heading? and if
        $C unhurried offer-action
    then
    fiona-who $1B char-in-area? fiona-who -90 $3C char-heading? and if
        $D unhurried offer-action
    then
    fiona-who $1C char-in-area? fiona-who 0 $3C char-heading? and if
        $E unhurried offer-action
    then
    -2147483646 scene-request? fiona-who 0 char-before-door? and if
        $F always offer-action
    then
    om2f-8-camomile story-flag? not if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            $A calm offer-action
        then
    then
    om2f-8-lavender story-flag? not if
        4 1 5 5 0 zone-at-effect
        fiona-who 4 3 char-zone-bits? if
            $B calm offer-action
        then
    then
;

: old-mansion-2f-8.phase3 ( -- )   \ 004114E0
    40.0 20.0 -77.0 17.3 20.0 -77.0 40.0 0.0 -77.0 17.3 0.0 -77.0 lights-doorway
    17.3 20.0 -42.0 40.0 20.0 -42.0 17.3 0.0 -42.0 40.0 0.0 -42.0 lights-doorway
    17.0 20.0 -77.3 17.0 20.0 -100.0 17.0 0.0 -77.3 17.0 0.0 -100.0 lights-doorway
    22.3 20.0 -6.0 22.3 20.0 -30.0 22.3 0.0 -6.0 22.3 0.0 -30.0 lights-doorway
    20.7 20.0 -30.0 20.7 20.0 -6.0 20.7 0.0 -30.0 20.7 0.0 -6.0 lights-doorway
    22.3 20.0 30.0 22.3 20.0 6.0 22.3 0.0 30.0 22.3 0.0 6.0 lights-doorway
    20.7 20.0 6.0 20.7 20.0 25.0 20.7 0.0 6.0 20.7 0.0 25.0 lights-doorway
;

: old-mansion-2f-8.act00 ( -- )   \ 00411640
    1 self-scripted
    self-wait-done
    hewie-who char-activate
    $5C 0 302 hewie-to-room
    $62 door-open-clear
    doors-room-in
    $62 door-lock
    creatures-on state-flag-clear
    1 creatures-clear
    found-file-234 story-flag-set
    camera-restart
    $F $51 fade
    wait-fade
    $234 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act01 ( -- )   \ 00411670
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $5D room-preload
    self-frames-reset
    self-wait-16
    3 part-shown
    self-frames-reset
    8 self-wait-frames
    $F $44 fade
    wait-fade
    world-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: old-mansion-2f-8.act02 ( -- )   \ 00411690
    1 self-scripted
    3 part-shown
    self-wait-done
    camera-restart
    world-held state-flag-clear
    om2f-8-flag1 story-flag-set
    $54 door-unlock
    2 exit-prepare
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    summoner-on state-flag-set
    self-idle-or-end
;

: old-mansion-2f-8.act03 ( -- )   \ 004116C0
    self-wait-done
    60.0 0.0 self-turn-to-xz
    self-wait-done
    om2f-8-flag1 story-flag? not if
        0 message   \ "There is a statue clasping an oil lamp. The wick of the lamp is unlit."
        wait-message
    else
        1 message   \ "A mesmerizing blue flame now dances from the oil lamp's wick."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-2f-8.act04 ( -- )   \ 004116E0
    stalkers-stay state-flag-set
    1 self-scripted
    $27 room-preload
    self-wait-done
    180 self-turn-angle
    self-wait-done
    look-close-anim $A self-anim-blend
    self-wait-anim
    2 message   \ "A stale, damp gale of wind blows forth from the other side of the hole. Enter the hole?..."
    wait-message
    0 answer? if
        $F 0 fade
        wait-fade
        fiona-who slide-sound 5 char-sound
        world-held state-flag-set
        $FE exit-check
    else
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-2f-8.act05 ( -- )   \ 00411720
    1 self-scripted
    self-wait-done
    camera-restart
    fiona-who slide-sound 5 char-sound
    $F 1 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act07 ( -- )   \ 00411830
    1 ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    0 0 var-set
    2 room-motion 5 self-anim-blend8
    self-frames-reset
    5 self-wait-frames
    begin
        2 old-mansion-2f-8.cmd00
        yield
        0 18 var? not while
        0 6 var? if
            fiona-who 0 6 char-sound
        then
        0 var-inc
    repeat
    self-wait-anim
    0 self-noclip
    1 ebit? not if
        stalker-who action-end
        stalker-who char-here? if
            stalker-who 0 stalker-command
        then
    then
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act06 ( -- )   \ 00411740
    stalkers-stay state-flag-set
    3 ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    $182 -67.8 -119.5 180 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    1 self-noclip
    0 0 var-set
    0 room-motion 5 self-anim-blend8
    self-frames-reset
    5 self-wait-frames
    begin
        0 old-mansion-2f-8.cmd00
        yield
        0 56 var? not while
        0 44 var? if
            fiona-who 0 6 char-sound
        then
        0 var-inc
    repeat
    self-wait-anim
    -1 self-idle-anim
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    1 ebit-clear
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        fiona-who 1 6 char-sound
    then
    begin
        0 2 pad? not if
            in-panic? 1 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] old-mansion-2f-8.act07 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                3 ebit? stalker-who char-here? not and if
                    3 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            stalker-who char-here? if
                ['] old-mansion-2f-8.act07 goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            0 0 var-set
            1 room-motion 5 self-anim-blend8
            self-frames-reset
            5 self-wait-frames
            begin
                1 old-mansion-2f-8.cmd00
                yield
                0 32 var? not while
                0 13 var? if
                    fiona-who 0 6 char-sound
                then
                0 var-inc
            repeat
            self-wait-anim
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: old-mansion-2f-8.act09 ( -- )   \ 004118E0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 2 char-heading-for? 2 exit-door-open? not and if
        2 3 self-move-slot
        self-wait-done
    then
    stalker-who 3 char-heading-for? 3 exit-door-open? not and if
        3 3 self-move-slot
        self-wait-done
    then
    stalker-who 4 char-heading-for? 4 exit-door-open? not and if
        4 3 self-move-slot
        self-wait-done
    then
    $182 -67.8 -110.0 180 $FFFF 5 self-move-to
    self-wait-done
    there-anim self-anim
    self-wait-anim
    1 ebit-set
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-2f-8.act0A ( -- )   \ 00411950
    self-wait-done
    48.0 -53.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-8-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-8.act0B ( -- )   \ 004119B0
    self-wait-done
    53.1 -26.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om2f-8-lavender story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        reach-back-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-2f-8.act0C ( -- )   \ 00411A10
    self-wait-done
    5 ebit? not if
        5 ebit-set
        138.0 -58.0 self-turn-to-xz
        self-wait-done
        look-low-anim self-anim
        self-frames-reset
        $28 self-wait-frames
        3 message   \ "It's full of insects."
        wait-message
    else
        $64 129.0 -60.5 90 $FFFF 5 self-move-to
        self-wait-done
        1 15.0 -10.0 0.0 7.0 event-camera
        look-at-anim self-anim
        self-frames-reset
        $28 self-wait-frames
        4 message   \ "A mother is lovingly holding a child. Why is her face painted red?"
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-8.act0D ( -- )   \ 00411A60
    self-wait-done
    78.0 -70.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    5 message   \ "Discolored bones lay in a pile. I can lie to myself all I want to, but there is no deny..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-8.act0E ( -- )   \ 00411A80
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    62.0 27.0 self-turn-to-xz
    self-wait-done
    8 message   \ "Something is written here..."
    wait-message
    $F 6 fade
    wait-fade
    9 message   \ "Recreatio Completely restores lost stamina. Sedatio Completely calms the player and ret..."
    wait-message
    $F 7 fade
    wait-fade
    $43 subscreen-bit? not $45 subscreen-bit? not or if
        hand-to-mouth-anim self-anim
        $A message   \ "This book sounds like it's about some kind of medicine."
        wait-message
        self-wait-anim
        $43 subscreen-bit
        $45 subscreen-bit
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-8.act0F ( -- )   \ 00411AD0
    self-wait-done
    0 self-through-exit
    self-wait-done
    in-panic? 2 game-mode? or if
        rattle-front-slow-anim self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
    else
        rattle-front-anim self-anim
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        7 message   \ "Water is flowing in. Guess there is no going back now..."
        wait-message
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-8 register-scripts   \ (its phases and actions, by their names)
