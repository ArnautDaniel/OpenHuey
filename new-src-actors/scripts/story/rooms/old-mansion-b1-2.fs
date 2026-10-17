\ story/rooms/old-mansion-b1-2.fs - the event scripts of room old-mansion-b1-2 ($4B; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-2
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant locked-remarked             \ (bit 0) its first remark made: "The door is locked..."
1 constant locked2-remarked            \ (bit 1) its first remark made: "The door is locked..." (the other door)
2 constant stalker-scene-on            \ (bit 2) the scene with her (act06) under way
3 constant wall-breaking               \ (bit 3) the stalker breaking through (at the room's end: broken)
4 constant hewie-zone3-noticed         \ (bit 4) Hewie has reacted to zone 3 (once a visit)
5 constant hewie-zone4-noticed         \ (bit 5) Hewie has reacted to zone 4 (once a visit)
6 constant hewie-zone5-noticed         \ (bit 6) Hewie has reacted to zone 5 (once a visit)
7 constant hewie-zone6-noticed         \ (bit 7) Hewie has reacted to zone 6 (once a visit)
0 constant struggle-frames-var         \ (var 0) the struggle's frames (her cry at 12, over at 30)
1 constant reads-var                   \ (var 1) the book read (the file given at 2)

\ room 0x4B (D_00409910): a lit quad at x -63.65 .. -55.65, z 104.5, from 4 to 21
: old-mansion-b1-2.cmd00 ( b0 -- )  $1B $80  -63.65e 21.0e 104.5e  -55.65e 21.0e 104.5e  -63.65e 4.0e 104.5e  -55.65e 4.0e 104.5e  lit-quad ;   \ (a reflecting glass: b0 1 removed, 2 over the whole picture)
\ room 0x4B: the curtain ("Curtain") animated by event variable 0 (var0_anim: byte 3 0 forward,
\ 1 back, 2 / 3 back at rest).
: old-mansion-b1-2.cmd01 ( b0 -- )  s" Curtain" var0-anim ;
\ 1 unless the object at +0x18 exists and its byte +0x28 is 1.
: old-mansion-b1-2.cond00? ( -- flag )  6 creature-up? 0= ;

: old-mansion-b1-2.enter ( -- )   \ 00408B90
    room-sounds
    om1f-12-flag4 story-flag? not if
        old-mansion-b1-2.cond00? if
            $4B 0 422 2 8 6 0 0.0 creature-place
        then
    then
    omb1-2-breakable1-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        omb1-2-viola-medallion story-flag? not if
            0 13.5 -9.0 -75.0 flicker-sprite
        then
    then
    omb1-2-breakable2-broken story-flag? not if
        1 keeps-people-off floor-on
        2 part-shown
        3 part-hidden
    else
        1 keeps-people-off floor-off
        2 part-hidden
        3 part-shown
        omb1-2-rubrum-medallion story-flag? not if
            1 13.5 -9.0 -59.5 flicker-sprite
        then
    then
    0 4 0.5 0.5 0.25 0.5 zone-rect
    1 $B 0.4 0.0 0.2 0.5 zone-rect
    omb1-2-breakable3-broken story-flag? not if
        4 part-hidden
        5 part-shown
        omb1-2-flag1 story-flag? not if
            3 old-mansion-b1-2.cmd01
            1 old-mansion-b1-2.cmd00
        else
            2 old-mansion-b1-2.cmd01
            0 old-mansion-b1-2.cmd00
        then
    else
        4 part-shown
        5 part-hidden
        1 old-mansion-b1-2.cmd00
        2 water-floor floor-on
    then
    omb1-2-refined-magnesia story-flag? not if
        2 -109.83 12.0 72.62 flicker-sprite
    then
    3 132.4 -16.6 -76.8 1 flame-in
    4 85.7 -5.3 -16.8 1 flame-in
    5 85.7 -5.3 16.8 1 flame-in
    6 132.4 -16.6 76.8 1 flame-in
    1 $2300 sound-volume
;

: old-mansion-b1-2.char-enter ( -- )   \ 00408CD0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        2 1 player-camera
    then
    1 2 1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        5 3 player-camera
    then
    2 5 3 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        4 2 player-camera
    then
    3 4 2 area-camera
    player-who self-is? 4 exit-taken? and if
        player-who 4 char-to-exit
        1 0 player-camera
    then
    4 1 0 area-camera
    player-who self-is? 5 exit-taken? and if
        player-who 5 char-to-exit
        6 4 player-camera
    then
    5 6 4 area-camera
    fiona-who self-is? if
        0 exit-taken? 1 exit-taken? or 4 exit-taken? or 5 exit-taken? or if
            1 map-page
        then
    then
;

: old-mansion-b1-2.phase1 ( -- )   \ 00408E50
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    5 exit-usable? if
        5 exit-check
    then
    $D 0 -1 1 chars-area-camera
    $E 1 0 1 chars-area-camera
    $F 1 0 1 chars-area-camera
    $10 2 1 1 chars-area-camera
    $11 1 0 1 chars-area-camera
    $12 3 -1 1 chars-area-camera
    $13 3 -1 1 chars-area-camera
    $14 4 2 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 5 3 1 chars-area-camera
    $17 1 0 1 chars-area-camera
    $18 6 4 1 chars-area-camera
    fiona-who 6 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 8 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        3 exit-prepare
    then
    fiona-who $A char-entered-area? fiona-who $B char-entered-area? or if
        4 exit-prepare
    then
    fiona-who $C char-entered-area? if
        5 exit-prepare
    then
    fiona-who $12 char-entered-area? if
        0 map-page
    then
    fiona-who $12 char-left-area? if
        1 map-page
    then
    stalker-scene-on ebit? not if
        omb1-2-breakable3-broken story-flag? not omb1-2-flag1 story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                stalker-who $1A char-entered-area? stalker-who 0 char-action? and if
                    0 stalker-who 7 action
                then
            then
        then
    then
    2 85.21 -24.0 1.06 $14 19 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 19.0 hewie-look-zone
            then
        then
    then
    omb1-2-breakable3-broken story-flag? not omb1-2-flag1 story-flag? and if
        3 -60.85 0.0 104.12 $10 6 0 zone
        hewie-zone3-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 3 9 char-zone-bits? if
                        hewie-zone3-noticed ebit-set
                        $32 chance? if
                            hewie-zone-var 3 var-set
                            0 hewie-who $93 action
                        then
                    then
                then
            then
        then
    then
    4 -101.62 0.0 100.21 $14 6 0 zone
    hewie-zone4-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 4 9 char-zone-bits? if
                    hewie-zone4-noticed ebit-set
                    $1E chance? if
                        hewie-zone-var 4 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    4 -101.62 0.0 100.21 $14 6 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                hewie-zone-var 4 var-set
                hewie-zone-var 6.0 hewie-look-zone
            then
        then
    then
    5 -132.75 0.0 71.15 $14 12 0 zone
    hewie-zone5-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 5 9 char-zone-bits? if
                    hewie-zone5-noticed ebit-set
                    $1E chance? if
                        hewie-zone-var 5 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    5 -132.75 0.0 71.15 $14 12 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                hewie-zone-var 5 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
    6 -29.87 0.0 75.83 $14 12 0 zone
    hewie-zone6-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 6 9 char-zone-bits? if
                    hewie-zone6-noticed ebit-set
                    $1E chance? if
                        hewie-zone-var 6 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    6 -29.87 0.0 75.83 $14 12 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 6 9 char-zone-bits? if
                hewie-zone-var 6 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
;

: old-mansion-b1-2.phase2 ( -- )   \ 004090C0
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            4 always offer-action
        then
        fiona-who 2 char-before-door? if
            0 always offer-action
        then
        fiona-who 3 char-before-door? if
            2 always offer-action
        then
    then
    fiona-who $19 char-in-area? fiona-who 45 $32 char-heading? and if
        5 unhurried offer-action
    then
    wall-breaking ebit? not if
        fiona-who $1A char-in-area? fiona-who 0 $3C char-heading? and if
            omb1-2-breakable3-broken story-flag? not if
                stalker-who char-here? not if
                    6 calm offer-action
                else stalker-who $1A char-in-area? stalker-who fiona-who 40 chars-within? or if
                    $8016 examine-message   \ "I have to get away! I don't have time for this!"
                else
                    6 calm offer-action
                then then
            else
                8 unhurried offer-action
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        stalker-who char-busy? not if
            wall-breaking ebit-clear
        then
    else
        wall-breaking ebit-clear
    then then
    omb1-2-refined-magnesia story-flag? not if
        7 2 5 5 0 zone-at-effect
        fiona-who 7 3 char-zone-bits? if
            9 calm offer-action
        then
    then
    8 -29.58 0.0 75.55 7 10 0 zone
    fiona-who 8 8 char-zone-bits? if
        fiona-who -30 76 $3C char-faces-xz? if
            $C unhurried offer-action
        then
    then
    9 -133.09 0.0 71.35 7 10 0 zone
    fiona-who 9 8 char-zone-bits? if
        fiona-who -133 71 $3C char-faces-xz? if
            $C unhurried offer-action
        then
    then
    fiona-who $1D char-in-area? fiona-who -45 $32 char-heading? and if
        $D unhurried offer-action
    then
    omb1-2-breakable1-broken story-flag? not if
        0 13.5 -10.0 -75.0 5 8 1 zone
        self-who 0 char-in-zone? if
            omb1-2-breakable1-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            13.5 -10.0 -75.0 0 -2145378272 0 0.0 shards-at
            $88 5 13.5 -10.0 -75.0 0 0 sound
            $40 $23A noise
            0 13.5 -9.0 -75.0 flicker-sprite
        then
    else omb1-2-viola-medallion story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            $A calm offer-action
        then
    then then
    omb1-2-breakable2-broken story-flag? not if
        1 13.5 -10.0 -59.5 5 8 1 zone
        self-who 1 char-in-zone? if
            omb1-2-breakable2-broken story-flag-set
            1 keeps-people-off floor-off
            2 part-hidden
            3 part-shown
            13.5 -10.0 -59.5 0 -2146430960 0 0.0 shards-at
            $88 5 13.5 -10.0 -59.5 0 0 sound
            $40 $234 noise
            1 13.5 -9.0 -59.5 flicker-sprite
        then
    else omb1-2-rubrum-medallion story-flag? not if
        1 1 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            $B calm offer-action
        then
    then then
;

: old-mansion-b1-2.phase5 ( -- )   \ 004092C0
    wall-breaking ebit? if
        omb1-2-flag1 story-flag-set
        omb1-2-breakable3-broken story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            stalker-who 1 stalker-rage
        then
    then
;

: old-mansion-b1-2.phase3 ( -- )   \ 004092E0
    58.0 5.0 19.5 58.0 5.0 15.0 58.0 -24.0 19.5 58.0 -24.0 15.0 lights-doorway
    58.0 5.0 -15.0 58.0 5.0 -19.5 58.0 -24.0 -15.0 58.0 -24.0 -19.5 lights-doorway
;

: old-mansion-b1-2.act00 ( -- )   \ 00409350
    self-wait-done
    2 self-through-exit
    self-wait-done
    locked-remarked ebit? not 2 game-mode? or in-panic? or if
        in-panic? 2 game-mode? or if
            rattle-behind-slow-anim play-out
            -1 self-idle-anim
            7 wait-frames
        else
            rattle-behind-anim play-out
            -1 self-idle-anim
            7 wait-frames
            1 say   \ "The door is locked..."
        then
        locked-remarked ebit-set
    else
        4 wait-frames
        fiona-who $8F 145.0 -75.0 180 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 3.0 event-camera
        4 wait-frames
        2 say   \ "A strange symbol is written on the door. I wonder what it could mean?"
        4 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        fiona-who $8F 143.75 -75.3 180 char-to-xz
        world-frozen state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-b1-2.act01 ( -- )   \ 004093E0
    1 self-scripted
    self-wait-done
    2 self-through-exit
    self-wait-done
    $609 self-anim
    $14 wait-frames
    fiona-who 2 6 char-sound
    $D message-param-room
    $52 door-locked? not if
        $D item-use
    then
    $51 door-unlock
    20 self-idle-anim
    $14 wait-frames
    $8019 say   \ "Used ."
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: old-mansion-b1-2.act02 ( -- )   \ 00409420
    self-wait-done
    3 self-through-exit
    self-wait-done
    locked2-remarked ebit? not 2 game-mode? or in-panic? or if
        in-panic? 2 game-mode? or if
            rattle-behind-slow-anim play-out
            -1 self-idle-anim
            7 wait-frames
        else
            rattle-behind-anim play-out
            -1 self-idle-anim
            7 wait-frames
            1 say   \ "The door is locked..."
        then
        locked2-remarked ebit-set
    else
        4 wait-frames
        fiona-who $29 145.0 75.0 0 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 3.0 event-camera
        4 wait-frames
        2 say   \ "A strange symbol is written on the door. I wonder what it could mean?"
        4 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        fiona-who $29 146.25 75.3 0 char-to-xz
        world-frozen state-flag-clear
        1 self-scripted
    then
    self-idle-or-end
;

: old-mansion-b1-2.act03 ( -- )   \ 004094B0
    1 self-scripted
    self-wait-done
    3 self-through-exit
    self-wait-done
    $609 self-anim
    $14 wait-frames
    fiona-who 2 6 char-sound
    $D message-param-room
    $51 door-locked? not if
        $D item-use
    then
    $52 door-unlock
    20 self-idle-anim
    $14 wait-frames
    $8019 say   \ "Used ."
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: old-mansion-b1-2.act04 ( -- )   \ 004094F0
    self-wait-done
    stalker-who self-touching? not if
        $4F self-through-door
        self-wait-done
        stalker-who self-touching? not if
            in-panic? not if
                fiona-who $4F char-not-at-door? if
                    rattle-behind-anim self-anim
                else
                    rattle-front-anim self-anim
                then
                $14 wait-frames
                in-panic? not if
                    fiona-who $72 5 char-sound
                    $4F door-unlock
                    $8008 message   \ "You unlocked the door."
                    20 self-idle-anim
                    $14 wait-frames
                    wait-message
                then
            then
        then
    then
    self-idle-or-end
;

: old-mansion-b1-2.act05 ( -- )   \ 00409530
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    90.0 0.0 self-turn-to-xz
    self-wait-done
    $F 6 fade
    wait-fade
    0 say   \ "The right path leads to the truth, whilst the left path leads to deception. When the sh..."
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-2.act06 ( -- )   \ 00409560
    1 self-scripted
    stalker-scene-on ebit-set
    self-wait-done
    fiona-who 0 char-file-load
    $21D -59.65 99.14 0 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    [: 6 sound-bank-loaded? ;] wait-until
    omb1-2-flag1 story-flag? not if
        0 old-mansion-b1-2.cmd00
        struggle-frames-var 0 var-set
        0 room-motion 5 self-anim-blend8
        5 wait-frames
        begin
            0 old-mansion-b1-2.cmd01
            yield
            struggle-frames-var 30 var? not while
            struggle-frames-var 12 var? if
                fiona-who 0 6 char-sound
            then
            struggle-frames-var var-inc
        repeat
        self-wait-anim
        2 old-mansion-b1-2.cmd01
        omb1-2-flag1 story-flag-set
    else
        struggle-frames-var 0 var-set
        1 room-motion 5 self-anim-blend8
        5 wait-frames
        begin
            1 old-mansion-b1-2.cmd01
            yield
            struggle-frames-var 28 var? not while
            struggle-frames-var 12 var? if
                fiona-who 0 6 char-sound
            then
            struggle-frames-var var-inc
        repeat
        self-wait-anim
        3 old-mansion-b1-2.cmd01
        omb1-2-flag1 story-flag-clear
        1 old-mansion-b1-2.cmd00
    then
    stalker-scene-on ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-2.act07 ( -- )   \ 00409610
    wall-breaking ebit-set
    self-wait-done
    stalker-who 1 char-file-load
    $21D -59.65 99.14 0 $FFFF 5 self-move-to
    self-wait-done
    stalker-who char-file-use
    [: 6 sound-bank-loaded? ;] wait-until
    stalker-who 5 6 char-sound
    0 room-motion 5 blend-out
    $E00 5 self-anim-blend
    $16 wait-frames
    omb1-2-breakable3-broken story-flag-set
    stalker-who 1 6 char-sound
    stalker-who 1 stalker-rage
    4 part-shown
    5 part-hidden
    2 water-floor floor-on
    1 old-mansion-b1-2.cmd00
    -57.0 16.0 104.5 1 404232216 4 -15.5 shards-at
    -61.0 16.0 104.5 1 404232216 4 -15.5 shards-at
    -57.0 10.0 104.5 1 404232216 4 -9.5 shards-at
    -61.0 10.0 104.5 1 404232216 4 -9.5 shards-at
    -57.0 4.0 104.5 1 404232216 4 -3.5 shards-at
    -61.0 4.0 104.5 1 404232216 4 -3.5 shards-at
    self-wait-anim
    wall-breaking ebit-clear
    self-idle-or-end
;

: old-mansion-b1-2.act08 ( -- )   \ 004096F8
    self-wait-done
    0 self-turn-angle
    self-wait-done
    3 say   \ "The glass has been shattered. This mirror is useless."
    self-idle-or-end
;

: old-mansion-b1-2.act09 ( -- )   \ 00409710
    self-wait-done
    -109.83 72.62 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-2-refined-magnesia story-flag-set
            2 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-b1-2.act0A ( -- )   \ 00409770
    self-wait-done
    13.5 -75.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-2-viola-medallion story-flag-set
            0 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-b1-2.act0B ( -- )   \ 004097D0
    self-wait-done
    13.5 -59.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-2-rubrum-medallion story-flag-set
            1 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-b1-2.act0C ( -- )   \ 00409830
    self-wait-done
    fiona-who 8 8 char-zone-bits? if
        -29.58 75.55 self-turn-to-xz
        self-wait-done
    else
        -133.09 71.35 self-turn-to-xz
        self-wait-done
    then
    4 say   \ "There is a doll here as well. It feels like they are watching me or something... It's r..."
    self-idle-or-end
;

: old-mansion-b1-2.act0D ( -- )   \ 00409860
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    4 wait-frames
    fiona-who $F4 -53.0 -16.0 -90 char-to-xz
    world-frozen state-flag-set
    1 12.0 0.0 0.0 5.0 event-camera
    5 say   \ "'Glowing Sun, Radiant Star, Luminescent Moonlight'"
    world-frozen state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    4 wait-frames
    hand-to-mouth-anim $A self-anim-blend
    6 say   \ "Glowing SUN, radiant STAR, MOONLIT sky... The words from the title are highlighted in red."
    self-wait-anim
    reads-var var-inc
    reads-var 2 var? if
        $236 item-give
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

\ ---- registered ----
old-mansion-b1-2 register-scripts   \ (its phases and actions, by their names)
