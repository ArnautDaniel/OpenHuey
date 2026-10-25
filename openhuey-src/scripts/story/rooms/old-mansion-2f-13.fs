\ story/rooms/old-mansion-2f-13.fs - the event scripts of room old-mansion-2f-13 ($6A; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-13
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant came-in-forward             \ (bit 0) she came in this way: no going back (until he is here)
1 constant stalker-scene-on            \ (bit 1) the scene with her (act01) under way
2 constant wall-breaking               \ (bit 2) the stalker breaking through (at the room's end: broken)
0 constant struggle-frames-var         \ (var 0) the struggle's frames (her cry at 12, over at 30)
0 constant struggle-sound              \ (snd 0) her struggling
1 constant smash-sound                 \ (snd 1) him smashing it
5 constant smash-start-sound           \ (snd 5) him going to smash it

\ room 0x6A (D_00438CE0): a lit quad at x 120, z 47 .. 39, from 4 to 21
: old-mansion-2f-13.cmd00 ( b0 -- )  $1B $80  120.0e 21.0e 47.0e  120.0e 21.0e 39.0e  120.0e 4.0e 47.0e  120.0e 4.0e 39.0e  lit-quad ;   \ (a reflecting glass: b0 1 removed, 2 over the whole picture)
\ room 0x6A (D_00438CF0): its object's animation (+0x74 forward, +0x78 back) at a point +0x7C
\ (0..1) by byte 3 - 0 / 1 from event variable 0 (12..30 over 18, 12..28 over 16), 2 / 3 at the
\ start / end
: old-mansion-2f-13.cmd01 ( b0 -- )  s" Curtain" var0-anim ;

: old-mansion-2f-13.enter ( -- )   \ 00438730
    room-sounds
    2 0 object-hidden
    om2f-13-breakable-broken story-flag? not if
        0 part-shown
        1 part-hidden
        om2f-13-flag1 story-flag? not if
            3 old-mansion-2f-13.cmd01
            1 old-mansion-2f-13.cmd00
        else
            2 old-mansion-2f-13.cmd01
            0 old-mansion-2f-13.cmd00
        then
    else
        0 part-hidden
        1 part-shown
        1 old-mansion-2f-13.cmd00
        0 water-floor floor-on
    then
    om2f-13-rubrum-medallion-found story-flag? om2f-13-rubrum-medallion story-flag? not and if
        0 88.5 1.0 71.56 flicker-sprite
    then
    1 $3FFF sound-volume
    0 9 0.4 0.0 0.2 0.5 zone-rect
;

: old-mansion-2f-13.char-enter ( -- )   \ 004387A0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 1 player-camera
    then
    0 1 1 area-camera
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
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 0 player-camera
    then
    3 0 0 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who 0 char-to-exit
            1 1 player-camera
            0 fiona-who 0 action
            hewie-who $59 char-in-room? if
                $6A 0 55 hewie-to-room
            then
        then
    then
;

: old-mansion-2f-13.phase1 ( -- )   \ 004388C0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    $A 2 2 1 chars-area-camera
    $B 1 1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who 6 char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 7 char-entered-area? fiona-who 8 char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        3 0 char-remove
    then
    fiona-who 7 char-entered-area? if
        $1C 3 1 char-load
    then
    fiona-who 8 char-entered-area? if
        om3f-1-flag2 story-flag? not om1f-6-flag1 story-flag? not and if
            4 0 char-remove
        then
    then
    fiona-who 9 char-entered-area? if
        om3f-1-flag2 story-flag? not om1f-6-flag1 story-flag? not and if
            $21 4 3 char-load
        then
        3 exit-prepare
    then
    stalker-scene-on ebit? not if
        om2f-13-breakable-broken story-flag? not om2f-13-flag1 story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                stalker-who $E char-entered-area? stalker-who 0 char-action? and if
                    0 stalker-who 2 action
                then
            then
        then
    then
    om2f-13-rubrum-medallion-found story-flag? not if
        0 88.5 0.0 71.56 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om2f-13-rubrum-medallion-found var-set
                taken-flag-var om2f-13-rubrum-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 88500 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 71560 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
    came-in-forward ebit? if
        stalker-who char-here? if
            came-in-forward ebit-clear
        then
    then
;

: old-mansion-2f-13.phase2 ( -- )   \ 004389F0
    0 exit-door-open? not came-in-forward ebit? and if
        fiona-who 0 char-before-door? if
            0 examine-message   \ "Only a fool would even think of going back now!!"
        then
    then
    wall-breaking ebit? not if
        fiona-who $E char-in-area? fiona-who 45 $3C char-heading? and if
            om2f-13-breakable-broken story-flag? not if
                stalker-who char-here? not if
                    1 calm offer-action
                else stalker-who $E char-in-area? stalker-who fiona-who 40 chars-within? or if
                    $8016 examine-message   \ "I have to get away! I don't have time for this!"
                else
                    1 calm offer-action
                then then
            else
                3 unhurried offer-action
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        stalker-who char-busy? not if
            wall-breaking ebit-clear
        then
    else
        wall-breaking ebit-clear
    then then
    om2f-13-rubrum-medallion-found story-flag? om2f-13-rubrum-medallion story-flag? not and if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            4 calm offer-action
        then
    then
;

: old-mansion-2f-13.phase5 ( -- )   \ 00438A70
    wall-breaking ebit? if
        om2f-13-flag1 story-flag-set
        om2f-13-breakable-broken story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            stalker-who 1 stalker-rage
        then
    then
;

: old-mansion-2f-13.act00 ( -- )   \ 00438A90
    1 self-scripted
    came-in-forward ebit-set
    $F $41 fade
    fiona-who $28 5 char-sound
    self-wait-done
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-13.act01 ( -- )   \ 00438AB0
    1 self-scripted
    stalker-scene-on ebit-set
    self-wait-done
    fiona-who 0 char-file-load
    $F8 114.64 43.0 90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    [: 6 sound-bank-loaded? ;] wait-until
    om2f-13-flag1 story-flag? not if
        0 old-mansion-2f-13.cmd00
        struggle-frames-var 0 var-set
        0 room-motion 5 self-anim-blend8
        5 wait-frames
        begin
            0 old-mansion-2f-13.cmd01
            yield
            struggle-frames-var 12 var? if
                fiona-who struggle-sound 6 char-sound
            then
            struggle-frames-var 30 var? not while
            struggle-frames-var var-inc
        repeat
        self-wait-anim
        2 old-mansion-2f-13.cmd01
        om2f-13-flag1 story-flag-set
    else
        struggle-frames-var 0 var-set
        1 room-motion 5 self-anim-blend8
        5 wait-frames
        begin
            1 old-mansion-2f-13.cmd01
            yield
            struggle-frames-var 12 var? if
                fiona-who struggle-sound 6 char-sound
            then
            struggle-frames-var 28 var? not while
            struggle-frames-var var-inc
        repeat
        self-wait-anim
        3 old-mansion-2f-13.cmd01
        om2f-13-flag1 story-flag-clear
        1 old-mansion-2f-13.cmd00
    then
    stalker-scene-on ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-13.act02 ( -- )   \ 00438B60
    wall-breaking ebit-set
    self-wait-done
    stalker-who 1 char-file-load
    $F8 114.64 43.0 90 $FFFF 5 self-move-to
    self-wait-done
    stalker-who char-file-use
    [: 6 sound-bank-loaded? ;] wait-until
    stalker-who smash-start-sound 6 char-sound
    0 room-motion 5 blend-out
    $E00 5 self-anim-blend
    $16 wait-frames
    stalker-who smash-sound 6 char-sound
    om2f-13-breakable-broken story-flag-set
    stalker-who 1 stalker-rage
    0 part-hidden
    1 part-shown
    0 water-floor floor-on
    1 old-mansion-2f-13.cmd00
    120.0 16.0 45.0 0 943208504 4 -15.5 shards-at
    120.0 16.0 41.0 0 943208504 4 -15.5 shards-at
    120.0 10.0 45.0 0 943208504 4 -9.5 shards-at
    120.0 10.0 41.0 0 943208504 4 -9.5 shards-at
    120.0 4.0 45.0 0 943208504 4 -3.5 shards-at
    120.0 4.0 41.0 0 943208504 4 -3.5 shards-at
    self-wait-anim
    wall-breaking ebit-clear
    self-idle-or-end
;

: old-mansion-2f-13.act03 ( -- )   \ 00438C48
    self-wait-done
    90 self-turn-angle
    self-wait-done
    6 say   \ "The glass has been shattered. This mirror is useless."
    self-idle-or-end
;

: old-mansion-2f-13.act04 ( -- )   \ 00438C60
    self-wait-done
    88.5 71.56 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $73 message-param-room
        $73 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om2f-13-rubrum-medallion story-flag-set
            0 effect-remove
            $73 1 item-give-count
            0 $73 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-13 register-scripts   \ (its phases and actions, by their names)
