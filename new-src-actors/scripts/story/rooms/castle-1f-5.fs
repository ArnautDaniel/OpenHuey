\ story/rooms/castle-1f-5.fs - the event scripts of room castle-1f-5 ($10; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-5
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant stalker-entrance-on         \ (bit 0) his entrance here under way (followed; Hewie not to attack)
1 constant box-remarked                \ (bit 1) its first remark made: "There is nothing inside."
2 constant in-area2                    \ (bit 2) someone in the area (entered, not yet left)
3 constant tired-remarked              \ (bit 3) her first "I don't feel so hot" made (the next time: rest)
4 constant painting-remarked           \ (bit 4) its first remark made: "A painting... Several people appear to be clasping their han"
5 constant stalker-scene-on            \ (bit 5) his scripted scene under way here (stalker): at the room's end he is put back
6 constant stalker-sent                \ (bit 6) the stalker sent on (action 4), once
0 constant shove-frames-var            \ (var 0) his shove's frames (dust at 60)
0 constant tired-sound                 \ (snd 0) her "I don't feel so hot"
11 constant shove-thud-sound           \ (snd 11) his shove landing (dust)

\ room 0x10 (Room10_Cond00_ptmf): the stalker is there, not about, in mode 2, 6 or 7
: castle-1f-5.cond00? ( -- flag )  stalker-there? stalker-told? 0= and debilitas? and ;

: castle-1f-5.enter ( -- )   \ 003F6F80
    room-sounds
    c2f-8-flag3 story-flag? c1f-5-flag1 story-flag? not and if
        1 exit-taken? if
            castle-1f-5.cond00? if
                c1f-5-flag1 story-flag-set
                force-followed state-flag-set
                $C door-open-clear
                doors-room-in
                stalker-entrance-on ebit-set
                hewie-no-attack state-flag-set
                stalker-who char-activate
                $FE $10 161 2 stalker-to-room
                stalker-who -1 -1 char-camera
                stalker-who char-full-health
                0 stalker-who 2 action
            then
        then
    then
    c1f-5-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        c1f-5-antimony-powder story-flag? not if
            3 41.0 1.0 -19.0 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 6.0 21.8 -3.2 0 flame-in
    1 29.0 41.8 41.9 0 flame-in
    2 -46.8 93.8 18.5 0 flame-in
    stalker-scene-on ebit-clear
    c1f-6-flag1 story-flag? over-music-b story-flag? not and if
        hewie-who action-end
        hewie-who char-done
    then
    1 $2300 sound-volume
;

: castle-1f-5.char-enter ( -- )   \ 003F7060
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        3 -1 player-camera
    then
    0 3 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        4 3 player-camera
    then
    2 4 3 area-camera
    fiona-who self-is? if
        1 exit-taken? if
            in-area2 ebit-set
        then
        c1f-6-flag1 story-flag? over-music-b story-flag? not and if
            fiona-who 1 char-to-exit
            0 0 player-camera
            0 fiona-who 5 action
        then
        0 exit-taken? 2 exit-taken? or if
            3 map-page
        then
    then
    stalker-who self-is? if
        stalker-sent ebit-clear
    then
;

: castle-1f-5.phase1 ( -- )   \ 003F7150
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 0 0 1 chars-area-camera
    $A 1 1 1 chars-area-camera
    $D 0 0 1 chars-area-camera
    $15 3 -1 1 chars-area-camera
    $16 4 3 1 chars-area-camera
    $17 3 -1 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-left-area? fiona-who 5 char-entered-area? or if
        2 exit-prepare
    then
    fiona-who 6 char-left-area? if
        1 exit-prepare
    then
    fiona-who $12 char-entered-area? if
        in-area2 ebit-set
    then
    fiona-who $12 char-left-area? if
        in-area2 ebit-clear
    then
    4 stalker-alert? not stalker-sent ebit? not and stalker-who 2 char-cond? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 stalker-alert? 2 stalker-alert? or if
                stalker-who $11 char-in-area? in-area2 ebit? and if
                    0 stalker-who 4 action
                    stalker-sent ebit-set
                then
            then
        then
    then
    over-music-b story-flag? saturnus-hint-on story-flag? not and if
        1 2 pad? 2 2 pad? or 3 2 pad? or 0 control-action? or 1 control-action? or 2 control-action? or 3 control-action? or 4 control-action? or fiona-free? and if
            0 fiona-who 6 action
        then
        99.0 fiona-fear
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
    fiona-who 5 char-entered-area? if
        3 map-page
    then
    fiona-who 5 char-left-area? if
        2 map-page
    then
    6 sound-bank-loaded? if
        fiona-who tired-sound kept-going 6 char-sound
    then
;

: castle-1f-5.phase2 ( -- )   \ 003F72D0
    fiona-who $F char-in-area? fiona-who 90 $32 char-heading? and if
        0 unhurried offer-action
    then
    fiona-who $10 $2D char-faces-area? if
        1 unhurried offer-action
    then
    fiona-who $13 char-in-area? fiona-who 45 $32 char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $14 char-in-area? fiona-who 45 $32 char-heading? and if
        8 unhurried offer-action
    then
    over-music-b story-flag? saturnus-hint-on story-flag? not and if
        -2147483646 scene-request? 2 scene-request? or if
            6 always offer-action
        then
    then
    c1f-5-breakable-broken story-flag? not if
        0 41.0 0.0 -19.0 5 8 1 zone
        self-who 0 char-in-zone? if
            c1f-5-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            41.0 0.0 -19.0 0 -2146430960 0 0.0 shards-at
            $88 5 41.0 0.0 -19.0 0 0 sound
            $40 $232 noise
            3 41.0 1.0 -19.0 flicker-sprite
        then
    else c1f-5-antimony-powder story-flag? not if
        0 3 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            9 calm offer-action
        then
    then then
;

: castle-1f-5.phase3 ( -- )   \ 003F73B0
    -8.0 82.0 -5.0 -8.0 82.0 45.0 -8.0 30.0 -5.0 -8.0 30.0 45.0 lights-doorway
    -5.3 54.0 -4.1 -5.3 54.0 47.0 -58.0 54.0 -4.1 -58.0 54.0 47.0 lights-doorway
    15.0 30.0 -3.0 45.0 30.0 -3.0 15.0 15.0 -3.0 45.0 15.0 -3.0 lights-doorway
    15.0 15.0 -3.0 45.0 15.0 -3.0 15.0 0.0 -3.0 45.0 0.0 -3.0 lights-doorway
    -11.0 30.0 -3.0 15.0 30.0 -3.0 -11.0 15.0 -3.0 15.0 15.0 -3.0 lights-doorway
    -11.0 15.0 -3.0 15.0 15.0 -3.0 -11.0 0.0 -3.0 15.0 0.0 -3.0 lights-doorway
;

: castle-1f-5.act03 ( -- )   \ 003F7560
    hewie-no-attack state-flag-clear
    force-followed state-flag-clear
    stalker-entrance-on ebit-clear
    stalker-who action-end
    stalker-who char-done
    exit
;

: castle-1f-5.phase5 ( -- )   \ 003F74E0
    stalker-entrance-on ebit? if
        castle-1f-5.act03
    then
    stalker-scene-on ebit? stalker-who char-busy? and if
        stalker-who action-end
        stalker-who $196 0 char-to-tri-facing
        stalker-who 0 char-no-shadow
        stalker-scene-on ebit-clear
    then
;

: castle-1f-5.act00 ( -- )   \ 003F7500
    self-wait-done
    -177 self-turn-angle
    self-wait-done
    look-close-anim play-out
    4 say   \ "The lighting is dim making it hard to see, but... I can ever so faintly make out the sh..."
    self-idle-or-end
;

: castle-1f-5.act01 ( -- )   \ 003F7510
    self-wait-done
    box-remarked ebit? not if
        40.0 -11.0 self-turn-to-xz
        self-wait-done
        reach-out-anim play-out
        2 say   \ "There is nothing inside."
        box-remarked ebit-set
    else
        3 say   \ "A second, more-thorough investigation of the box reveals that there is dust inside..."
    then
    self-idle-or-end
;

: castle-1f-5.act02 ( -- )   \ 003F7530
    1 self-scripted
    self-wait-done
    1 $FF 8 rumble
    stalker-who $A1 -90 char-to-tri-facing
    0 exit-door-open? not if
        $1B4 $FFFF $B self-move-tri
        self-wait-done
        fiona-who 3 self-move-slot
        self-wait-done
    then
    $3C $FFFF $B self-move-tri
    self-wait-done
    0 self-scripted
    castle-1f-5.act03
    self-idle-or-end
;

: castle-1f-5.act04 ( -- )   \ 003F7570
    self-wait-done
    stalker-who 0 char-file-load
    $8A 31.949 -18.333 0 $FFFF 5 self-move-to
    self-wait-done
    stalker-who char-file-use
    1 self-noclip
    1 self-scripted
    stalker-scene-on ebit-set
    0 room-motion 5 self-anim-blend8
    shove-frames-var 0 var-set
    begin
        fiona-who self-touching? if
            stalker-who 0 fiona-thrown
        then
        shove-frames-var 60 var? if
            0 29.0 19.0 25.0 0 0 0 0 dust
            0 36.0 19.0 25.0 0 0 0 0 dust
            stalker-who shove-thud-sound 6 char-sound
        then
        shove-frames-var var-inc
        0 camera-mode? not if
            stalker-who 1 char-no-shadow
        else
            stalker-who 0 char-no-shadow
        then
        self-at-motion-event? not while
        yield
    repeat
    0 self-noclip
    0 self-scripted
    stalker-who 0 char-no-shadow
    stalker-scene-on ebit-clear
    stalker-who 2 stalker-command
    self-idle-or-end
;

: castle-1f-5.act05 ( -- )   \ 003F7600
    hewie-who action-end
    hewie-who char-done
    self-wait-done
    3 partner-load
    2 char-start
    stalker-who $E 1 room-doors-state
    stalker-who $F 1 room-doors-state
    stalker-who $25 1 room-doors-state
    stalker-who $60 1 room-doors-state
    stalker-who $63 1 room-doors-state
    stalker-who $4C 1 room-doors-state
    stalker-who $66 1 room-doors-state
    stalker-who $61 1 room-doors-state
    stalker-who $53 1 room-doors-state
    stalker-who $54 1 room-doors-state
    stalker-who $6C 1 door-lock-for
    hewie-who $53 1 room-doors-state
    hewie-who $5F 1 room-doors-state
    stalker-who $5F 1 room-doors-state
    1 door-reopen-unlock
    $D door-reopen-unlock
    $58 door-close-off-lock
    $5A door-close-off-lock
    $4D door-close-off-lock
    $4E door-close-off-lock
    $50 door-close-off-lock
    $53 door-close-off-lock
    $5C door-close-off-lock
    $79 door-close-off-lock
    exits-rebuild
    camera-restart
    2 door-lock
    $34 door-lock
    $62 door-lock
    $44 door-lock
    $47 door-lock
    $4F door-lock
    $56 door-lock
    $67 door-lock
    $7D door-lock
    $51 door-lock
    $52 door-lock
    $64 door-lock
    $6C door-lock
    $74 door-lock
    $78 door-lock
    $5E door-lock
    $76 door-lock
    $7B door-lock
    $7C door-lock
    $63 door-lock
    $54 door-lock
    1 door-open-clear
    1 door-lock
    3 door-open-clear
    3 door-lock
    $C door-open-clear
    $C door-lock
    $D door-unlock
    $D door-open-set
    doors-room-in
    no-flee state-flag-set
    no-running state-flag-set
    hewie-commandable state-flag-set
    scene-locked state-flag-clear
    world-held state-flag-set
    over-music-b story-flag-set
    subscreen-locked state-flag-set
    $26 1 pvar? if
        1 fiona-costume
        1 sound-set
    else $26 0 pvar? if
        0 fiona-costume
        0 sound-set
    else $26 2 pvar? if
        2 fiona-costume
        1 sound-set
    else $26 3 pvar? if
        3 fiona-costume
        1 sound-set
    else $26 6 pvar? if
        6 fiona-costume
        0 sound-set
    else $26 7 pvar? if
        7 fiona-costume
        0 sound-set
    else $26 8 pvar? if
        8 fiona-costume
        1 sound-set
    then then then then then then then
    effects-arena-flip
    0 char-in
    begin
        yield
        4 sound-bank-loaded? until
    $27 0 pvar? if
        0 hewie-model
    else $27 1 pvar? if
        1 hewie-model
    else $27 2 pvar? if
        2 hewie-model
    then then then
    effects-arena-flip
    1 char-in
    music-stage-end
    1 music-stage
    2 0 0 music
    3 0 0 music
    4 0 0 music
    hewie-who char-activate
    $10 0 155 hewie-to-room
    hewie-who $9B 0 char-to-tri-facing
    in-play state-flag-clear
    $F 1 fade
    wait-fade
    self-idle-or-end
;

: castle-1f-5.act06 ( -- )   \ 003F77A0
    self-wait-done
    knocked-anim self-anim
    fiona-who tired-sound 6 char-sound
    $14 wait-frames
    tired-remarked ebit? not if
        5 message   \ "I... I don't feel so hot. It's like I have no energy all of a sudden."
        tired-remarked ebit-set
    else
        6 message   \ "I should try to get some rest."
        tired-remarked ebit-clear
    then
    attack-anim $A self-anim-blend
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-1f-5.act07 ( -- )   \ 003F77D0
    self-wait-done
    $17B 94.0 -27.5 90 $FFFF 5 self-move-to
    self-wait-done
    1 15.0 0.0 0.0 8.0 event-camera
    4 wait-frames
    painting-remarked ebit? not if
        7 say   \ "A painting... Several people appear to be clasping their hands in prayer."
        painting-remarked ebit-set
    else
        8 say   \ "If I had the time, I'd like to spend more time taking in the painting's full beauty..."
    then
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-5.act08 ( -- )   \ 003F7820
    self-wait-done
    $19A 94.0 27.5 90 $FFFF 5 self-move-to
    self-wait-done
    1 15.0 0.0 0.0 8.0 event-camera
    4 wait-frames
    9 say   \ "It seems like this and the painting hanging next to it make up a set of twin paintings."
    4 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-5.act09 ( -- )   \ 003F7870
    self-wait-done
    41.0 -19.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-5-antimony-powder story-flag-set
            3 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

\ ---- registered ----
castle-1f-5 register-scripts   \ (its phases and actions, by their names)
