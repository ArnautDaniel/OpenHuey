\ story/rooms/old-mansion-2f-8.fs - the event scripts of room old-mansion-2f-8 ($5C; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-8
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant camera-scene-done           \ (bit 0) its scene begun, once
1 constant found-her                   \ (bit 1) he has found her hiding: she must flee (the avoid prompt)
2 constant will-find-her               \ (bit 2) he will find her hiding (by chance, as he searches: more likely by the progress variable)
3 constant came-while-hidden           \ (bit 3) the stalker came in while she was hiding
4 constant hewie-zone2-noticed         \ (bit 4) Hewie has reacted to zone 2 (once a visit)
5 constant turned-to-look              \ (bit 5) she has turned to look (act0C)
6 constant sound-started               \ (bit 6) its sound started (then kept up)
0 constant curtain-var                 \ (var 0) the curtain's frames
1 constant sound-step-var              \ (var 1) which of its sounds next (0..3)
2 constant sound-frames-var            \ (var 2) frames to the next sound (30)
0 constant curtain-sound               \ (snd 0) the curtain
1 constant hide-sound                  \ (snd 1) her hiding
3 constant ambient1-sound              \ (snd 3) its ambient sounds in turn (every 30 frames): the first
4 constant ambient2-sound              \ (snd 4) its ambient sounds in turn (every 30 frames): the second
5 constant ambient3-sound              \ (snd 5) its ambient sounds in turn (every 30 frames): the third
6 constant ambient4-sound              \ (snd 6) its ambient sounds in turn (every 30 frames): the fourth
8 constant room-loop-sound             \ (snd 8) its steady sound (started as it comes in, then kept up)
9 constant room-loop2-sound           \ (snd 9) its steady sound (started as it comes in, then kept up)

\ room 0x5C (Room5C_Cmd00_ptmf): the dial (+0x7C, 0..1) from script variable 0 by byte 3: 0
\ 0x2B..0x38 (/ 13, +0x74 0 / +0x78 1), 1 0xC..0x20 (/ 20), 2 7..0x12 (/ 11) (+0x74 1 / +0x78 0)
: old-mansion-2f-8.cmd00 ( b0 -- )   \ (the curtain ("Curtain") by event variable 0 in b0's range:
    \ 0 opening over $2B..$38, 1 closing over $C..$20, 2 over 7..$12)
    {: b0 | v :}  s" Curtain" object-here? 0= if  exit  then  curtain-var var @ to v
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
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
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
    will-find-her ebit-clear
    $16 0 pvar? if
        $A chance? if
            will-find-her ebit-set
        then
    else $16 1 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else $16 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $16 3 pvar? if
        $4B chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    will-find-her ebit? if
        0 stalker-who 9 action
    else
        $78 1 summoner-wait
    then
    $16 pvar-inc
    exit
;

: old-mansion-2f-8.char-enter ( -- )   \ 00410FB0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        2 2 player-camera
    then
    1 2 2 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        4 -1 player-camera
    then
    2 4 -1 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 0 player-camera
    then
    3 0 0 area-camera
    player-who self-is? 4 exit-taken? and if
        player-who 4 char-to-exit
        5 3 player-camera
    then
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
            came-while-hidden ebit-set
            old-mansion-2f-8.act08
        else
            came-while-hidden ebit-clear
        then
    then
;

: old-mansion-2f-8.phase1 ( -- )   \ 00411180
    sound-started ebit? not if
        6 sound-bank-loaded? if
            room-loop-sound volume-scaled 6 5.0 18.0 -125.0 0 0 sound
            om2f-12-flag1 story-flag? not if
                room-loop2-sound volume-scaled 6 -120.0 10.0 -105.0 0 0 sound
            then
            sound-started ebit-set
        then
    else
        room-loop-sound kept-going volume-scaled 6 5.0 18.0 -125.0 0 0 sound
        om2f-12-flag1 story-flag? not if
            room-loop2-sound kept-going volume-scaled 6 -120.0 10.0 -105.0 0 0 sound
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
    camera-scene-done ebit? not if
        om2f-8-flag1 story-flag? not if
            3 camera-mode? 4 camera-mode? or if
                old-mansion-2f-8.cond00? if
                    camera-scene-done ebit-set
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
                hewie-zone-var 0 var-set
                hewie-zone-var 21.0 hewie-look-zone
            then
        then
    then
    1 59.37 0.0 0.93 $14 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
    2 5.52 0.0 -118.99 $F 18 0 zone
    hewie-zone2-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 2 9 char-zone-bits? if
                    hewie-zone2-noticed ebit-set
                    $1E chance? if
                        hewie-zone-var 2 var-set
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
                hewie-zone-var 2 var-set
                hewie-zone-var 18.0 hewie-look-zone
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
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                ambient1-sound volume-scaled 6 45.32 0.0 23.5 0 0 sound
            else sound-step-var 1 var? if
                ambient2-sound volume-scaled 6 45.32 0.0 23.5 0 0 sound
            else sound-step-var 2 var? if
                ambient3-sound volume-scaled 6 45.32 0.0 23.5 0 0 sound
            else sound-step-var 3 var? if
                ambient4-sound volume-scaled 6 45.32 0.0 23.5 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
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
    16 wait-frames
    3 part-shown
    8 wait-frames
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
        0 say   \ "There is a statue clasping an oil lamp. The wick of the lamp is unlit."
    else
        1 say   \ "A mesmerizing blue flame now dances from the oil lamp's wick."
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
    look-close-anim $A blend-out
    2 say   \ "A stale, damp gale of wind blows forth from the other side of the hole. Enter the hole?..."
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
    found-her ebit? if
        2 avoid-prompt
    then
    1 self-scripted
    self-wait-done
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    curtain-var 0 var-set
    2 room-motion 5 self-anim-blend8
    5 wait-frames
    begin
        2 old-mansion-2f-8.cmd00
        yield
        curtain-var 18 var? not while
        curtain-var 6 var? if
            fiona-who curtain-sound 6 char-sound
        then
        curtain-var var-inc
    repeat
    self-wait-anim
    0 self-noclip
    found-her ebit? not if
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
    came-while-hidden ebit-clear
    1 self-scripted
    fiona-who 0 char-file-load
    self-wait-done
    $182 -67.8 -119.5 180 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    1 self-noclip
    curtain-var 0 var-set
    0 room-motion 5 self-anim-blend8
    5 wait-frames
    begin
        0 old-mansion-2f-8.cmd00
        yield
        curtain-var 56 var? not while
        curtain-var 44 var? if
            fiona-who curtain-sound 6 char-sound
        then
        curtain-var var-inc
    repeat
    self-wait-anim
    -1 self-idle-anim
    0 self-noclip
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    found-her ebit-clear
    0 avoid-prompt
    $18 1 item-count? $19 1 item-count? or $1A 1 item-count? or $1B 1 item-count? or $1C 1 item-count? or if
        fiona-who hide-sound 6 char-sound
    then
    begin
        0 2 pad? not if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] old-mansion-2f-8.act07 goto
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
                ['] old-mansion-2f-8.act07 goto
            then
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            1 self-noclip
            curtain-var 0 var-set
            1 room-motion 5 self-anim-blend8
            5 wait-frames
            begin
                1 old-mansion-2f-8.cmd00
                yield
                curtain-var 32 var? not while
                curtain-var 13 var? if
                    fiona-who curtain-sound 6 char-sound
                then
                curtain-var var-inc
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
    there-anim play-out
    found-her ebit-set
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: old-mansion-2f-8.act0A ( -- )   \ 00411950
    self-wait-done
    48.0 -53.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $40 message-param-room
        $40 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om2f-8-camomile story-flag-set
            0 effect-remove
            $40 1 item-give-count
            0 $40 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-2f-8.act0B ( -- )   \ 004119B0
    self-wait-done
    53.1 -26.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om2f-8-lavender story-flag-set
            1 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-2f-8.act0C ( -- )   \ 00411A10
    self-wait-done
    turned-to-look ebit? not if
        turned-to-look ebit-set
        138.0 -58.0 self-turn-to-xz
        self-wait-done
        look-low-anim self-anim
        $28 wait-frames
        3 say   \ "It's full of insects."
    else
        $64 129.0 -60.5 90 $FFFF 5 self-move-to
        self-wait-done
        1 15.0 -10.0 0.0 7.0 event-camera
        look-at-anim self-anim
        $28 wait-frames
        4 say   \ "A mother is lovingly holding a child. Why is her face painted red?"
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-8.act0D ( -- )   \ 00411A60
    self-wait-done
    78.0 -70.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    $28 wait-frames
    5 say   \ "Discolored bones lay in a pile. I can lie to myself all I want to, but there is no deny..."
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-8.act0E ( -- )   \ 00411A80
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    62.0 27.0 self-turn-to-xz
    self-wait-done
    8 say   \ "Something is written here..."
    $F 6 fade
    wait-fade
    9 say   \ "Recreatio Completely restores lost stamina. Sedatio Completely calms the player and ret..."
    $F 7 fade
    wait-fade
    $43 subscreen-bit? not $45 subscreen-bit? not or if
        hand-to-mouth-anim self-anim
        $A say   \ "This book sounds like it's about some kind of medicine."
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
        rattle-front-slow-anim play-out
        -1 self-idle-anim
        7 wait-frames
    else
        rattle-front-anim play-out
        -1 self-idle-anim
        7 wait-frames
        7 say   \ "Water is flowing in. Guess there is no going back now..."
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-8 register-scripts   \ (its phases and actions, by their names)
