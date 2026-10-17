\ story/rooms/old-mansion-2f-6.fs - the event scripts of room old-mansion-2f-6 ($59; Belli Castle: Old Mansion 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-2f-6
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ room 0x59 (Room59_Cmd00_ptmf): character 0xFE's model +0x9E0 = 0.1 (byte 3 0) or 0
: old-mansion-2f-6.cmd00 ( b0 -- )  drop ;   \ (the stalker's model's springs (+0x9E0): not simulated here yet)

: old-mansion-2f-6.enter ( -- )   \ 00410070
    room-sounds
    om2f-6-flag2 story-flag? if
        8 1 object-hidden
    then
    om2f-6-flag1 story-flag? if
        9 1 object-hidden
    then
    0 part-hidden
    cf-13-flag1 story-flag? x-37-flag2 story-flag? not and if
        $80 exit-taken? if
            4 ebit-set
        then
    then
    4 ebit? not if
        om2f-6-lavender story-flag? not if
            0 4.58 9.0 14.22 flicker-sprite
        then
    then
;

: old-mansion-2f-6.char-enter ( -- )   \ 004100B0
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
    fiona-who self-is? if
        4 ebit? if
            fiona-who $4B 0 char-to-tri-facing
            world-held state-flag-set
            0.0 sound-volume-scale
            panic-eased
            0 fiona-who 6 action-force
        then
    then
;

: old-mansion-2f-6.phase1 ( -- )   \ 00410110
    0 exit-usable? if
        0 exit-check
    then
    1 0 0 1 chars-area-camera
    2 1 1 1 chars-area-camera
    3 1 1 1 chars-area-camera
    4 2 2 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
;

: old-mansion-2f-6.phase2 ( -- )   \ 00410140
    om2f-6-flag2 story-flag? not if
        fiona-who 7 char-in-area? fiona-who 45 $32 char-heading? and if
            3 stalker-kind? $22 stalker-kind? or 4 stalker-kind? or 2 game-mode? and in-panic? not and stalker-who 2 char-cond? not and if
                stalker-who char-here? if
                    $8016 examine-message   \ "I have to get away! I don't have time for this!"
                else
                    1 calm offer-action
                then
            else
                3 unhurried offer-action
            then
        then
    then
    om2f-6-flag1 story-flag? not if
        fiona-who 8 char-in-area? fiona-who -45 $32 char-heading? and if
            3 stalker-kind? $22 stalker-kind? or 4 stalker-kind? or 2 game-mode? and in-panic? not and stalker-who 2 char-cond? not and if
                stalker-who char-here? if
                    $8016 examine-message   \ "I have to get away! I don't have time for this!"
                else
                    2 calm offer-action
                then
            else
                4 unhurried offer-action
            then
        then
    then
    fiona-who 9 char-in-area? fiona-who 0 $32 char-heading? and if
        0 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who -15 $3C char-heading? and if
        7 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who -16 -17 $3C char-faces-xz? and if
        7 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who -35 $3C char-heading? and if
        8 unhurried offer-action
    then
    fiona-who $E char-in-area? fiona-who 0 $3C char-heading? and if
        9 unhurried offer-action
    then
    fiona-who $D char-in-area? fiona-who 10 $3C char-heading? and if
        $A unhurried offer-action
    then
    om2f-6-lavender story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    fiona-who $F $3C char-faces-area? if
        $B unhurried offer-action
    then
;

: old-mansion-2f-6.act00 ( -- )   \ 00410230
    2 ebit? not if
        stalkers-stay state-flag-set
        1 self-scripted
        self-wait-done
        fiona-who $A char-file-load
        $7A 30.0 40.5 0 $FFFF 5 self-move-to
        self-wait-done
        1 25.0 10.0 0.0 0.0 event-camera
        7 message   \ "I hope this water won't make me sick if I drink it."
        wait-message
        fiona-who char-file-use
        4 room-motion $A self-anim-blend
        self-frames-reset
        $16 self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        fiona-who $84 5 char-sound
        3 avoid-prompt
        2 ebit-set
        self-frames-reset
        $1E self-wait-frames
        got-doc-recovery story-flag? not if
            $F 6 fade
            wait-fade
            $40A8 message   \ "'Recovery Points' Search a recovery point to calm Fiona from a state of pan..."
            wait-message
            $F 7 fade
            wait-fade
            got-doc-recovery story-flag-set
            $A8 message-param-room
            $A8 1 item-give-count
            $83 $85 0.0 0.0 0.0 0 0 sound
            $801B message   \ "has been saved to your file."
            wait-message
            self-frames-reset
            4 self-wait-frames
        then
        0 0.0 0.0 0.0 0.0 event-camera
        stalkers-stay state-flag-clear
        0 self-scripted
    else
        self-wait-done
        8 message   \ "Ahh. That hit the spot."
        wait-message
    then
    self-idle-or-end
;

: old-mansion-2f-6.act01 ( -- )   \ 004102F0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    3 stalker-kind? $22 stalker-kind? or if
        3 1 movie-play
        2 cutscene-start
        yield
        yield
        scene-wait-ready
        3 4 0 char-model-op
    else
        7 1 movie-play
        6 cutscene-start
        yield
        yield
        scene-wait-ready
        3 5 0 char-model-op
    then
    wait-fade
    hewie-who char-here? if
        3 ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        3 ebit-set
    then
    $10 $FF movie-param
    3 stalker-kind? $22 stalker-kind? or if
        0 cutscene-track $D action
    else
        0 cutscene-track $C action
    then
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 stalker-kind? $22 stalker-kind? or if
        1 old-mansion-2f-6.cmd00
    then
    wait-fade
    8 1 object-hidden
    0 self-idle-anim
    3 stalker-kind? $22 stalker-kind? or if
        fiona-who $4A -7.575 18.619 90 char-to-xz
        $FE $59 16 2 stalker-to-room
        stalker-who 2 2 char-camera
        stalker-who $10 -90 char-to-tri-facing
        stalker-who 0 stalker-command
        $35 resident-flag-set
    else
        fiona-who $4A -7.719 18.622 90 char-to-xz
        $FE $59 130 2 stalker-to-room
        stalker-who 2 2 char-camera
        stalker-who $82 -90 char-to-tri-facing
        stalker-who 0 stalker-command
        $37 resident-flag-set
    then
    2 2 player-camera
    camera-restart
    3 ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $59 0 3 hewie-to-room
            hewie-who 3 -7.85 29.94 52 char-to-xz
        then
    then
    om2f-6-flag2 story-flag-set
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-2f-6.act02 ( -- )   \ 00410470
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    3 stalker-kind? $22 stalker-kind? or if
        1 1 movie-play
        0 cutscene-start
        yield
        yield
        scene-wait-ready
        $10 $FF movie-param
        3 4 0 char-model-op
    else
        5 1 movie-play
        4 cutscene-start
        yield
        yield
        scene-wait-ready
        $10 $FF movie-param
        3 5 0 char-model-op
    then
    wait-fade
    hewie-who char-here? if
        3 ebit-clear
        hewie-who 2 char-cond? if
            0 hewie-who $86 action
        else
            hewie-who action-end
            hewie-who char-done
        then
    else
        3 ebit-set
    then
    force-followed state-flag-set
    yield
    force-followed state-flag-clear
    0 cutscene-track $C action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    world-held state-flag-set
    9 1 object-hidden
    0 self-idle-anim
    3 ebit? not if
        hewie-who 2 char-cond? if
            hewie-who 0 char-visible
            hewie-who action-end
        else
            hewie-who char-activate
            $59 0 56 hewie-to-room
            hewie-who $38 -4.97 35.12 -145 char-to-xz
        then
    then
    om2f-6-flag1 story-flag-set
    3 stalker-kind? $22 stalker-kind? or if
        fiona-who $4A 180 char-to-tri-facing
        $FE $59 16 2 stalker-to-room
        stalker-who $10 23.591 11.878 -30 char-to-xz
        stalker-who char-activate
        stalker-who stalker-knock-down
        $34 resident-flag-set
        $80 door-open-clear
        $80 exit-check
    else
        fiona-who $4A -6.968 18.812 -130 char-to-xz
        2 2 player-camera
        camera-restart
        stalker-who action-end
        3 summon-take
        $F $41 fade
        wait-fade
        $36 resident-flag-set
        stalkers-stay state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-2f-6.act03 ( -- )   \ 004105E0
    self-wait-done
    32.0 -3.5 self-turn-to-xz
    self-wait-done
    0 ebit? not if
        3 message   \ "There's some kind of medicine bottle on the shelf."
        wait-message
        0 ebit-set
    else
        5 message   \ "The label says Acetic Acid on it."
        wait-message
        0 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-6.act04 ( -- )   \ 00410600
    self-wait-done
    -29.0 -1.0 self-turn-to-xz
    self-wait-done
    1 ebit? not if
        4 message   \ "Several bottles containing various chemicals have been left out."
        wait-message
        1 ebit-set
    else
        6 message   \ "The words Sulfuric Acid are written on one bottle."
        wait-message
        1 ebit-clear
    then
    self-idle-or-end
;

: old-mansion-2f-6.act05 ( -- )   \ 00410620
    self-wait-done
    4.58 14.22 self-turn-to-xz
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
            om2f-6-lavender story-flag-set
            0 effect-remove
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

: old-mansion-2f-6.act06 ( -- )   \ 00410680
    stalker-who action-end
    stalker-who char-done
    hewie-who action-end
    hewie-who char-done
    self-wait-done
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    scene-wait-ready
    0 part-shown
    panic-eased
    0 message-prepare   \ "Another 2 or 3 days, at most..."
    1 result? if
        scene-play
    then
    0 part-hidden
    3 0 char-remove
    $C0 room-preload
    world-held state-flag-set
    $3A resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: old-mansion-2f-6.act07 ( -- )   \ 00410720
    self-wait-done
    fiona-who $A char-in-area? if
        -30 self-turn-angle
        self-wait-done
        look-close-anim self-anim
    else
        -16.0 -17.0 self-turn-to-xz
        self-wait-done
        look-low-anim self-anim
    then
    self-frames-reset
    $28 self-wait-frames
    5 ebit? not if
        5 ebit-set
        9 message   \ "There is a device which appears to be used for measuring things... I wonder what th..."
        wait-message
    else
        $E message   \ "For some reason this room seems to contain comparatively modern equipment. Still t..."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act08 ( -- )   \ 00410750
    self-wait-done
    -70 self-turn-angle
    self-wait-done
    6 ebit? not if
        6 ebit-set
        $A message   \ "I wonder if this could serve as a hiding spot if I'm ever in a pinch? Oh darn. It'..."
        wait-message
    else
        rattle-front-slow-anim self-anim
        self-frames-reset
        $B self-wait-frames
        fiona-who $28 5 char-sound
        self-wait-anim
        $B message   \ "I don't think I'll be able to open it. Maybe they have locked some important chemi..."
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        wait-message
    then
    self-idle-or-end
;

: old-mansion-2f-6.act09 ( -- )   \ 00410780
    self-wait-done
    2.0 45.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    $C message   \ "I wonder what's inside? It's not like it's going to explode or anything... right?"
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act0A ( -- )   \ 004107A0
    self-wait-done
    14.2 44.7 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    $D message   \ "Looks like plastic containers used to store chemicals. I can make out the word 'Ethano..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act0B ( -- )   \ 004107C0
    self-wait-done
    6.5 14.5 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    $F message   \ "A rather dirty and unsanitary operating table. No matter how serious the illness, you..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-2f-6.act0C ( -- )   \ 004107D8
    0 exit-door-open? not if
        self-frames-reset
        $F self-wait-frames
        0 1 self-door-knock
    then
    self-idle-or-end
;

: old-mansion-2f-6.act0D ( -- )   \ 004107F0
    0 exit-door-open? not if
        self-frames-reset
        $F self-wait-frames
        0 1 self-door-knock
    then
    begin
        6 cutscene-shot? not while
        yield
    repeat
    0 old-mansion-2f-6.cmd00
    self-idle-or-end
;

: old-mansion-2f-6.act0E ( -- )   \ 00410810
    self-wait-done
    0 part-hidden
    3 partner-load
    2 char-start
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $FF movie-param
    3 4 0 char-model-op
    0 cutscene-track $C action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    9 1 object-hidden
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-2f-6.act0F ( -- )   \ 004108B0
    self-wait-done
    0 part-hidden
    3 partner-load
    2 char-start
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    3 4 0 char-model-op
    $10 $FF movie-param
    0 cutscene-track $D action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 old-mansion-2f-6.cmd00
    8 1 object-hidden
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-2f-6.act10 ( -- )   \ 00410950
    self-wait-done
    0 part-hidden
    4 partner-load
    2 char-start
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    $10 $FF movie-param
    3 5 0 char-model-op
    0 cutscene-track $C action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    9 1 object-hidden
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-2f-6.act11 ( -- )   \ 004109F0
    self-wait-done
    0 part-hidden
    4 partner-load
    2 char-start
    7 1 movie-play
    6 cutscene-start
    yield
    yield
    scene-wait-ready
    3 5 0 char-model-op
    $10 $FF movie-param
    0 cutscene-track $C action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    8 1 object-hidden
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-2f-6.act12 ( -- )   \ 00410A90
    self-wait-done
    0 part-hidden
    $17 3 $FF char-load
    3 char-start
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    scene-wait-ready
    0 part-shown
    panic-eased
    0 message-prepare   \ "Another 2 or 3 days, at most..."
    1 result? if
        scene-play
    then
    0 part-hidden
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
old-mansion-2f-6 register-scripts   \ (its phases and actions, by their names)
