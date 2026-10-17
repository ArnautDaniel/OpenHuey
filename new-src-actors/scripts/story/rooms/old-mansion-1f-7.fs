\ story/rooms/old-mansion-1f-7.fs - the event scripts of room old-mansion-1f-7 ($47; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-7
USING: room-names story.words story.shared flag-names story-names fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant extractor-remarked          \ (bit 0) its first remark made: "Extractor Born from the swirling sands of chaos, the primord"
1 constant caught-remarked             \ (bit 1) its first remark made: "Why is this happening to me?!..."
2 constant hewie-zone0-noticed         \ (bit 2) Hewie has reacted to zone 0 (once a visit)
3 constant large-device-remarked       \ (bit 3) its first remark made: "It's quite a large and complicated device. It looks like nut"
4 constant sound-started               \ (bit 4) its sound started (then kept up)
0 constant used-item-var               \ (var 0) the item she used (9 .. 11)

: old-mansion-1f-7.enter ( -- )   \ 00424B50
    room-sounds
    om1f-7-album-medallion-found story-flag? om1f-7-album-medallion story-flag? not and if
        0 -30.81 1.0 -21.85 flicker-sprite
    then
    1 $2300 sound-volume
;

: old-mansion-1f-7.char-enter ( -- )   \ 00424B70
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        2 2 player-camera
    then
    0 2 2 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            over-music-c story-flag? not if
                world-held state-flag-set
                0.0 sound-volume-scale
                0 fiona-who 0 action
                over-music-c story-flag-set
            then
        then
    then
;

: old-mansion-1f-7.phase1 ( -- )   \ 00424C10
    sound-started ebit? not if
        6 sound-bank-loaded? if
            $40000000 6 0.0 11.0 0.0 0 0 sound
            sound-started ebit-set
        then
    else
        $C0000000 6 0.0 11.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 1 1 1 chars-area-camera
    7 2 2 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    2 -68.08 0.0 55.74 $1E 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                hewie-zone-var 2 var-set
                hewie-zone-var 18.0 hewie-look-zone
            then
        then
    then
    0 -1.06 0.0 -0.54 $26 17 0 zone
    hewie-zone0-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    hewie-zone0-noticed ebit-set
                    $32 chance? if
                        hewie-zone-var 0 var-set
                        0 hewie-who $8A action
                    then
                then
            then
        then
    then
    0 -1.06 0.0 -0.54 $26 17 0 zone
    hewie-who char-here? 2 game-mode? not and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 17.0 hewie-look-zone
            then
        then
    then
    om1f-7-album-medallion-found story-flag? not if
        1 -30.81 0.0 -21.85 $A 5 0 zone
        hewie-who 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var om1f-7-album-medallion-found var-set
                taken-flag-var om1f-7-album-medallion var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var -30810 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var -21850 var-set
                hewie-zone-var 1 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: old-mansion-1f-7.phase2 ( -- )   \ 00424D70
    fiona-who 8 char-in-area? fiona-who -45 $32 char-heading? and if
        3 unhurried offer-action
    then
    fiona-who 9 char-in-area? fiona-who 0 0 $32 char-faces-xz? and if
        4 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 0 0 $32 char-faces-xz? and if
        5 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who -25 $3C char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who 45 $3C char-heading? and if
        7 unhurried offer-action
    then
    om1f-7-album-medallion-found story-flag? om1f-7-album-medallion story-flag? not and if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            6 calm offer-action
        then
    then
;

: old-mansion-1f-7.act00 ( -- )   \ 00424DE0
    stalkers-stay state-flag-set
    1 self-scripted
    hewie-who action-end
    hewie-who char-done
    2 0 char-remove
    self-wait-done
    4 partner-load
    2 char-start
    in-play state-flag-clear
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $FE $47 145 2 stalker-to-room
    3 5 0 char-model-op
    panic-eased
    0 message-prepare   \ "Miss Fiona... Now, if you would please come with me... No, I don't want to. Ple..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    stalker-who $91 -13.81 11.69 125 char-to-xz
    0 exit-prepare
    $80 exit-check
    music-stage-end
    2 music-stage
    2 0 0 music
    3 0 0 music
    4 0 0 music
    $44 door-lock
    $47 door-lock
    $56 door-lock
    $5D door-lock
    $7D door-lock
    $34 door-lock
    $79 door-close-off-lock
    exits-rebuild
    stalker-who $60 1 room-doors-state
    stalker-who $63 1 room-doors-state
    stalker-who $10B 1 room-doors-state
    summoner-on state-flag-set
    summoner-own-cooldown
    self-idle-or-end
;

: old-mansion-1f-7.act01 ( -- )   \ 00424ED0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    $78 wait-frames
    look-close-anim play-out
    6 say   \ "The composition of this item has changed!"
    reach-out-anim play-out
    8 item-use
    9 message-param-room
    9 1 item-give-count
    0 9 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    reach-back-anim play-out
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-7.act02 ( -- )   \ 00424F40
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    fiona-who 3 6 char-sound
    $78 wait-frames
    look-close-anim play-out
    reach-out-anim play-out
    used-item-var 9 var? if
        9 message-param-room
        0 9 item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 10 var? if
        $A message-param-room
        0 $A item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
    else used-item-var 11 var? if
        $B message-param-room
        0 $B item-tab
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
    3 say   \ "No extraction occurred. I guess it didn't work... Maybe I should try a different item?"
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-7.act03 ( -- )   \ 00425060
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $50 -57.0 55.0 -90 $FFFF 5 self-move-to
    self-wait-done
    extractor-remarked ebit? not if
        $F 6 fade
        wait-fade
        1 say   \ "Extractor Born from the swirling sands of chaos, the primordial element, Prima Mate..."
        extractor-remarked ebit-set
        $F 7 fade
        wait-fade
    else
        hand-to-mouth-anim $A self-anim-blend
        2 say   \ "An extractor? This place is full of odd inventions that I've never heard of. It app..."
        self-wait-anim
        extractor-remarked ebit-clear
    then
    $242 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-7.act04 ( -- )   \ 004250B0
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    caught-remarked ebit? not if
        4 wait-frames
        fiona-who $BD 12.41 -5.83 -62 char-to-xz
        1 10.0 -10.0 0.0 3.0 event-camera
        world-frozen state-flag-set
        4 say   \ "Why is this happening to me?! Dear god! If I get caught now, I'm sure they will kil..."
        caught-remarked ebit-set
        4 wait-frames
        world-frozen state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
    else
        5 say   \ "No luck. I can't see any higher than this."
    then
    self-idle-or-end
;

: old-mansion-1f-7.act05 ( -- )   \ 00425110
    self-wait-done
    0.0 0.0 self-turn-to-xz
    self-wait-done
    caught-remarked ebit? not if
        4 wait-frames
        fiona-who $C0 -11.799 6.147 121 char-to-xz
        1 10.0 -10.0 0.0 3.0 event-camera
        world-frozen state-flag-set
        4 say   \ "Why is this happening to me?! Dear god! If I get caught now, I'm sure they will kil..."
        caught-remarked ebit-set
        4 wait-frames
        world-frozen state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
    else
        5 say   \ "No luck. I can't see any higher than this."
    then
    self-idle-or-end
;

: old-mansion-1f-7.act06 ( -- )   \ 00425170
    self-wait-done
    -30.81 -21.85 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-7-album-medallion story-flag-set
            0 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-7.act07 ( -- )   \ 004251D0
    self-wait-done
    12.8 28.7 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    large-device-remarked ebit? not if
        7 say   \ "It's quite a large and complicated device. It looks like nutrients are fed in throu..."
        large-device-remarked ebit-set
    else
        8 say   \ "However, come to think of it, maybe nutrients are not fed in... Perhaps they are su..."
    then
    self-idle-or-end
;

: old-mansion-1f-7.act08 ( -- )   \ 00425200
    self-wait-done
    4 partner-load
    2 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    3 5 0 char-model-op
    panic-eased
    0 message-prepare   \ "Miss Fiona... Now, if you would please come with me... No, I don't want to. Ple..."
    1 result? if
        scene-play
    then
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-1f-7 register-scripts   \ (its phases and actions, by their names)
