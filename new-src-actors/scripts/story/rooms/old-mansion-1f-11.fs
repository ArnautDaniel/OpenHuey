\ story/rooms/old-mansion-1f-11.fs - the event scripts of room old-mansion-1f-11 ($4E; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-11
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ room 0x4E (D_0040B4D8): a lit quad at x -44 .. -36, z 60, from 54 to 71
: old-mansion-1f-11.cmd00 ( b0 -- )  $1B $80  -44.0e 71.0e 60.0e  -36.0e 71.0e 60.0e  -44.0e 54.0e 60.0e  -36.0e 54.0e 60.0e  lit-quad ;   \ (a reflecting glass: b0 1 removed, 2 over the whole picture)
\ room 0x4E: the curtain ("Curtain_2") animated by event variable 0 (var0_anim: byte 3 0
\ forward, 1 back, 2 / 3 back at rest).
: old-mansion-1f-11.cmd01 ( b0 -- )  s" Curtain" var0-anim ;
\ room 0x4E (D_0040B4F8): the 0x10-byte effect Room4EEffect_vtable made
: old-mansion-1f-11.cmd02 ( -- )  8 $80808080  -8.7654e -7.4e -0.0072e  120e 0.5e 0.1e  effects:caustic drop ;   \ (light rippling: Room4EEffect)

: old-mansion-1f-11.enter ( -- )   \ 0040AE90
    room-sounds
    $11 1.0 0 bgm
    omb1-5-flag3 story-flag? not if
        0 part-shown
        0 barred-floor hole-floor or floor-on
    else
        1 part-shown
        1 barred-floor hole-floor or floor-on
        2 both-halves floor-on
        hewie-who char-here? if
            hewie-who 1 char-in-nav-group? hewie-who 2 char-in-nav-group? or if
                hewie-who $D char-to-tri
            then
        then
    then
    om1f-11-breakable-broken story-flag? not if
        2 part-hidden
        3 part-shown
        om1f-11-flag1 story-flag? not if
            3 old-mansion-1f-11.cmd01
            1 old-mansion-1f-11.cmd00
        else
            2 old-mansion-1f-11.cmd01
            0 old-mansion-1f-11.cmd00
        then
    else
        2 part-shown
        3 part-hidden
        1 old-mansion-1f-11.cmd00
        4 water-floor floor-on
    then
    om1f-11-album-medallion-found story-flag? om1f-11-album-medallion story-flag? not and if
        0 13.96 51.0 -5.34 flicker-sprite
    then
    old-mansion-1f-11.cmd02
    1 $2300 sound-volume
    0 7 0.4 0.0 0.2 0.5 zone-rect
;

: old-mansion-1f-11.char-enter ( -- )   \ 0040AF40
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 2 player-camera
    then then
    0 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        0 0 player-camera
    then then
    1 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        1 1 player-camera
    then then
    2 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        1 1 player-camera
    then then
    3 1 1 area-camera
    fiona-who self-is? if
        0 exit-taken? 1 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-11.phase1 ( -- )   \ 0040B040
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
    8 0 0 1 chars-area-camera
    9 2 2 1 chars-area-camera
    $A 2 2 1 chars-area-camera
    $B 3 3 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
        white-mandragora-used story-flag? not if
            3 0 char-remove
        then
    then
    fiona-who 7 char-entered-area? if
        white-mandragora-used story-flag? not if
            $26 3 $FF char-load
        then
        3 exit-prepare
    then
    1 ebit? not if
        om1f-11-breakable-broken story-flag? not om1f-11-flag1 story-flag? and if
            3 stalker-kind? $22 stalker-kind? or if
                stalker-who $13 char-entered-area? stalker-who 0 char-action? and if
                    0 stalker-who 4 action
                then
            then
        then
    then
    om1f-11-breakable-broken story-flag? not om1f-11-flag1 story-flag? and if
        0 -39.93 50.0 59.59 $C 6 0 zone
        4 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        4 ebit-set
                        $32 chance? if
                            hewie-zone-var 0 var-set
                            0 hewie-who $93 action
                        then
                    then
                then
            then
        then
    then
    om1f-11-album-medallion-found story-flag? not if
        3 13.96 50.0 -5.34 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-11-album-medallion-found var-set
                taken-flag-var om1f-11-album-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var 13960 var-set
                sparkle-y-var 51000 var-set
                sparkle-z-var -5340 var-set
                hewie-zone-var 3 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: old-mansion-1f-11.phase2 ( -- )   \ 0040B160
    omb1-5-flag3 story-flag? if
        fiona-who $10 char-in-area? fiona-who 0 $32 char-heading? and if
            0 unhurried offer-action
        then
        fiona-who $11 char-in-area? fiona-who 90 $32 char-heading? and if
            1 unhurried offer-action
        then
    else fiona-who $12 char-in-area? fiona-who -45 $32 char-heading? and if
        2 unhurried offer-action
    then then
    2 ebit? not if
        fiona-who $13 char-in-area? fiona-who 0 $3C char-heading? and if
            om1f-11-breakable-broken story-flag? not if
                stalker-who char-here? not if
                    3 calm offer-action
                else stalker-who $13 char-in-area? stalker-who fiona-who 40 chars-within? or if
                    $8016 examine-message   \ "I have to get away! I don't have time for this!"
                else
                    3 calm offer-action
                then then
            else
                5 unhurried offer-action
            then
        then
    else 3 stalker-kind? $22 stalker-kind? or if
        stalker-who char-busy? not if
            2 ebit-clear
        then
    else
        2 ebit-clear
    then then
    om1f-11-album-medallion-found story-flag? om1f-11-album-medallion story-flag? not and if
        3 0 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            6 calm offer-action
        then
    then
;

: old-mansion-1f-11.phase5 ( -- )   \ 0040B200
    2 ebit? if
        om1f-11-flag1 story-flag-set
        om1f-11-breakable-broken story-flag-set
        3 stalker-kind? $22 stalker-kind? or if
            stalker-who 1 stalker-rage
        then
    then
    hewie-who char-here? hewie-who 2 char-cond? and if
        hewie-who 1 char-in-nav-group? if
            hewie-who 1 char-heal
            hewie-who 0 char-cond!
        then
    then
;

: old-mansion-1f-11.act00 ( -- )   \ 0040B230
    self-wait-done
    0 self-turn-angle
    self-wait-done
    0 message   \ "The floor has collapsed! But I literally just crossed through here seconds ago..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-11.act01 ( -- )   \ 0040B240
    self-wait-done
    180 self-turn-angle
    self-wait-done
    0 message   \ "The floor has collapsed! But I literally just crossed through here seconds ago..."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-11.act02 ( -- )   \ 0040B250
    0 ebit? not if
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        1 message   \ "The path is obstructed by water. I can't get through..."
        wait-message
        0 ebit-set
    else
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        -90 self-turn-angle
        self-wait-done
        self-frames-reset
        4 self-wait-frames
        4 4 player-camera
        2 message   \ "I can see a door on the other side. Maybe that is the way out of this hellish maze..."
        wait-message
        self-frames-reset
        4 self-wait-frames
        1 1 player-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-11.act03 ( -- )   \ 0040B2A0
    1 self-scripted
    1 ebit-set
    self-wait-done
    fiona-who 0 char-file-load
    $56 -40.0 54.64 0 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    om1f-11-flag1 story-flag? not if
        0 old-mansion-1f-11.cmd00
        0 0 var-set
        0 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        begin
            0 old-mansion-1f-11.cmd01
            yield
            0 30 var? not while
            0 12 var? if
                fiona-who 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        2 old-mansion-1f-11.cmd01
        om1f-11-flag1 story-flag-set
    else
        0 0 var-set
        1 room-motion 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        begin
            1 old-mansion-1f-11.cmd01
            yield
            0 28 var? not while
            0 12 var? if
                fiona-who 0 6 char-sound
            then
            0 var-inc
        repeat
        self-wait-anim
        3 old-mansion-1f-11.cmd01
        om1f-11-flag1 story-flag-clear
        1 old-mansion-1f-11.cmd00
    then
    1 ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-11.act04 ( -- )   \ 0040B350
    2 ebit-set
    self-wait-done
    stalker-who 1 char-file-load
    $56 -40.0 54.64 0 $FFFF 5 self-move-to
    self-wait-done
    stalker-who char-file-use
    begin
        6 sound-bank-loaded? not while
        yield
    repeat
    stalker-who 5 6 char-sound
    0 room-motion 5 self-anim-blend
    self-wait-anim
    $E00 5 self-anim-blend
    self-frames-reset
    $16 self-wait-frames
    om1f-11-breakable-broken story-flag-set
    stalker-who 1 6 char-sound
    stalker-who 1 stalker-rage
    2 part-shown
    3 part-hidden
    4 water-floor floor-on
    1 old-mansion-1f-11.cmd00
    -37.0 66.0 60.0 0 538976288 4 -15.5 shards-at
    -41.0 66.0 60.0 0 538976288 4 -15.5 shards-at
    -37.0 60.0 60.0 0 538976288 4 -9.5 shards-at
    -41.0 60.0 60.0 0 538976288 4 -9.5 shards-at
    -37.0 54.0 60.0 0 538976288 4 -3.5 shards-at
    -41.0 54.0 60.0 0 538976288 4 -3.5 shards-at
    self-wait-anim
    2 ebit-clear
    self-idle-or-end
;

: old-mansion-1f-11.act05 ( -- )   \ 0040B438
    self-wait-done
    0 self-turn-angle
    self-wait-done
    3 message   \ "The glass has been shattered. This mirror is useless."
    wait-message
    self-idle-or-end
;

: old-mansion-1f-11.act06 ( -- )   \ 0040B450
    self-wait-done
    13.96 -5.34 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            om1f-11-album-medallion story-flag-set
            0 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-11 register-scripts   \ (its phases and actions, by their names)
