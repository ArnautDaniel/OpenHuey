\ story/rooms/castle-1f-8.fs - the event scripts of room castle-1f-8 ($18; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-8
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant found-her                   \ (bit 0) he has found her hiding: she must flee (the avoid prompt)
1 constant will-find-her               \ (bit 1) he will find her hiding (by chance, as he searches: more likely by the progress variable)
2 constant came-while-hidden           \ (bit 2) the stalker came in while she was hiding
3 constant hewie-away                  \ (bit 3) Hewie not here (or sent off) as the scene ends
4 constant stalker-scene-on            \ (bit 4) his scene (act0A) under way
0 constant uneasy-sound                \ (snd 0) her unease (before the scene)

\ room 0x18 (Room18_Cmd00_ptmf): object byte 3's PlacedObject_ToDef
: castle-1f-8.cmd00 ( b0 -- )   \ (Room18_ObjectNames[b0] back as defined: the telephones' receivers, 4 / 5)
    case
        4 of  s" telephone_a" object-to-def  endof
        5 of  s" telephone_b" object-to-def  endof
    endcase ;

: castle-1f-8.enter ( -- )   \ 003FAA20
    c1f-8-flag1 story-flag? not if
        room-sounds
    then
    0 34.2 18.3 14.2 1 flame-in
    1 34.2 18.3 -14.2 1 flame-in
    2 -34.2 18.3 -14.2 1 flame-in
    3 -34.2 18.3 14.2 1 flame-in
    c1f-8-album-medallion-found story-flag? c1f-8-album-medallion story-flag? not and if
        4 -25.6 1.0 27.6 flicker-sprite
    then
    c1f-8-magnesia story-flag? not if
        5 0.0 8.5 25.0 flicker-sprite
    then
    0 part-shown
    1 $2300 sound-volume
;

: castle-1f-8.act07 ( -- )   \ 003FB0A0
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who 3 char-file-load
    then
    will-find-her ebit-clear
    $D 0 pvar? if
        0 chance? if
            will-find-her ebit-set
        then
    else $D 1 pvar? if
        $19 chance? if
            will-find-her ebit-set
        then
    else $D 2 pvar? if
        $32 chance? if
            will-find-her ebit-set
        then
    else $D 3 pvar? if
        $4B chance? if
            will-find-her ebit-set
        then
    then then then then
    2 creature-action? if
        will-find-her ebit-set
    then
    4 stalker-alert? if
        will-find-her ebit-clear
    then
    will-find-her ebit? if
        0 stalker-who 8 action
    else
        $78 1 summoner-wait
    then
    $D pvar-inc
    exit
;

: castle-1f-8.char-enter ( -- )   \ 003FAAA0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 0 player-camera
    then
    1 1 0 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
            castle-1f-8.act07
        else
            came-while-hidden ebit-clear
        then
    then
;

: castle-1f-8.phase1 ( -- )   \ 003FAB30
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 0 1 chars-area-camera
    5 0 -1 1 chars-area-camera
    6 2 -1 1 chars-area-camera
    7 1 0 -1 chars-area-camera
    fiona-who 2 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    c1f-8-flag1 story-flag? not if
        fiona-who 2 char-left-area? if
            c1f-8-flag1 story-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    1 0 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    2 1 8 -4 0 zone-at-effect
    fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 2 3 char-zone-bits? hewie-who 2 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    3 2 8 -4 0 zone-at-effect
    fiona-who 3 3 char-zone-bits? fiona-who 3 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 3 3 char-zone-bits? hewie-who 3 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 3 3 char-zone-bits? stalker-who 3 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
    4 3 8 -4 0 zone-at-effect
    fiona-who 4 3 char-zone-bits? fiona-who 4 3 char-zone-bits-before? not and if
        fiona-who 3 char-effect-moving
    then
    hewie-who 4 3 char-zone-bits? hewie-who 4 3 char-zone-bits-before? not and if
        hewie-who 3 char-effect-moving
    then
    stalker-who 4 3 char-zone-bits? stalker-who 4 3 char-zone-bits-before? not and if
        stalker-who 3 char-effect-moving
    then
    c1f-8-album-medallion-found story-flag? not if
        0 -25.6 0.0 27.6 $A 5 0 zone
        hewie-who 0 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var c1f-8-album-medallion-found var-set
                taken-flag-var c1f-8-album-medallion var-set
                sparkle-fx-var 4 var-set
                sparkle-x-var -25600 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 27600 var-set
                hewie-zone-var 0 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: castle-1f-8.phase2 ( -- )   \ 003FACB0
    c1f-8-magnesia story-flag? not if
        5 5 5 5 0 zone-at-effect
        fiona-who 5 3 char-zone-bits? if
            $B calm offer-action
        then
    then
    fiona-who $C char-in-area? fiona-who 90 $3C char-heading? and if
        stalker-who char-here? not if
            5 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    c1f-8-flag1 story-flag? if
        fiona-who $B char-in-area? fiona-who 75 $2D char-heading? and if
            1 unhurried offer-action
        then
    then
    fiona-who 8 char-in-area? fiona-who 45 $2D char-heading? and if
        2 unhurried offer-action
    then
    fiona-who 9 $2D char-faces-area? if
        3 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who -67 $2D char-heading? and if
        4 unhurried offer-action
    then
    c1f-8-album-medallion-found story-flag? c1f-8-album-medallion story-flag? not and if
        0 4 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            9 calm offer-action
        then
    then
;

: castle-1f-8.phase3 ( -- )   \ 003FAD40
    34.0 20.0 -15.0 42.0 20.0 -15.0 34.0 0.0 -15.0 42.0 0.0 -15.0 lights-doorway
    42.0 20.0 15.0 34.0 20.0 15.0 42.0 0.0 15.0 34.0 0.0 15.0 lights-doorway
    -43.0 20.0 -15.0 -35.0 20.0 -15.0 -43.0 0.0 -15.0 -35.0 0.0 -15.0 lights-doorway
    -35.0 20.0 15.0 -43.0 20.0 15.0 -35.0 0.0 15.0 -43.0 0.0 15.0 lights-doorway
;

: castle-1f-8.act00 ( -- )   \ 003FAE10
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    [: 6 sound-bank-loaded? ;] wait-until
    uneasy-sound 6 22.0 10.0 -24.0 0 0 sound
    uneasy-anim $A blend-out
    $F $44 fade
    $19 3 $FF char-load
    3 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 cutscene-track $C action
    hewie-who char-here? if
        hewie-away ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        hewie-away ebit-set
    then
    0 part-hidden
    panic-eased
    0 message-prepare   \ "Fiona... Who is this? You must pay attention now, Fiona. Be wary of Riccardo......"
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    3 0 char-remove
    0 part-shown
    4 0 object-hidden
    5 0 object-hidden
    4 castle-1f-8.cmd00
    5 castle-1f-8.cmd00
    hewie-away ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $18 0 299 hewie-to-room
            hewie-who $12B 73.05 6.36 -100 char-to-xz
        then
    then
    0 self-idle-anim
    fiona-who $48 19.0 -19.0 150 char-to-xz
    2 -1 player-camera
    c1f-8-flag1 story-flag-set
    $11 door-open-clear
    $11 door-lock
    summoner-on state-flag-set
    stalker-who char-activate
    $FE $18 218 2 stalker-to-room
    summoner-own-cooldown
    stalker-who 0 -1 char-camera
    stalker-who 2 stalker-command
    0 stalker-who $A action
    found-file-208 story-flag-set
    5 0 0 music
    $F $81 fade
    wait-fade
    $208 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-8.act01 ( -- )   \ 003FAF80
    self-wait-done
    $48 19.0 -19.0 150 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 1.0 10.0 0.0 0.0 event-camera
    7 say   \ "The line must have been cut... There is no dial tone."
    16 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-8.act02 ( -- )   \ 0047AA48
    self-wait-done
    8 say   \ "There is an oddly placed chair here as well. This place continues to weird me out."
    self-idle-or-end
;

: castle-1f-8.act03 ( -- )   \ 0047AA50
    self-wait-done
    9 say   \ "There is nothing out of the ordinary here."
    self-idle-or-end
;

: castle-1f-8.act04 ( -- )   \ 0047AA58
    self-wait-done
    $A say   \ "Various ornaments have been placed here. They all look quite dusty, though."
    self-idle-or-end
;

: castle-1f-8.act06 ( -- )   \ 003FB070
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

: castle-1f-8.act05 ( -- )   \ 003FAFD0
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    1 self-scripted
    fiona-who 2 char-file-load
    self-wait-done
    fiona-who char-file-use
    $141 $8004 5 -0.803 -15.664 180 self-walk-anim
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
    self-who 3 -1 char-camera
    begin
        0 2 pad? not if
            in-panic? found-her ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] castle-1f-8.act06 goto
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
                ['] castle-1f-8.act06 goto
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

: castle-1f-8.act08 ( -- )   \ 003FB100
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $B2 0.866 0.417 180 $FFFF 5 self-move-to
    self-wait-done
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who char-file-use
        0 room-motion $A self-anim-blend
        $1E wait-frames
        found-her ebit-set
        self-wait-anim
    else
        there-anim play-out
        found-her ebit-set
    then
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-1f-8.act09 ( -- )   \ 003FB160
    self-wait-done
    -25.6 27.6 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-8-album-medallion story-flag-set
            4 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-1f-8.act0A ( -- )   \ 003FB1C0
    self-wait-done
    stalker-scene-on ebit-set
    stalker-who $DA 100 char-to-tri-facing
    fiona-who 3 self-move-slot
    self-wait-done
    stalker-scene-on ebit-clear
    self-idle-or-end
;

: castle-1f-8.act0B ( -- )   \ 003FB1D0
    self-wait-done
    0.0 25.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $92 message-param-room
        $92 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-8-magnesia story-flag-set
            5 effect-remove
            $92 1 item-give-count
            0 $92 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: castle-1f-8.act0C ( -- )   \ 003FB228
    [: 8 cutscene-shot? ;] wait-until
    fiona-who 1 char-no-shadow
    self-idle-or-end
;

: castle-1f-8.act0D ( -- )   \ 003FB240
    self-wait-done
    $19 3 $FF char-load
    3 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track $C action
    0 34.2 18.3 14.2 1 flame-in
    1 34.2 18.3 -14.2 1 flame-in
    2 -34.2 18.3 -14.2 1 flame-in
    3 -34.2 18.3 14.2 1 flame-in
    0 part-hidden
    panic-eased
    0 message-prepare   \ "Fiona... Who is this? You must pay attention now, Fiona. Be wary of Riccardo......"
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-8.phase5 ( -- )   \ 0047AA40
    stalker-scene-on ebit? if
        stalker-who 0 stalker-command
    then
;

\ ---- registered ----
castle-1f-8 register-scripts   \ (its phases and actions, by their names)
