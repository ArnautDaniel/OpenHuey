\ story/rooms/castle-1f-13.fs - the event scripts of room castle-1f-13 ($8; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-13
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ room 0x08 (Room08_Cmd00_ptmf): the cutscene director's +0x6C 3 (byte 3 0) or 2
: castle-1f-13.cmd00 ( b0 -- )   \ (the scene's cast leaves actor 3 (b0 0) or 2 be: Cutscene_Mark82)
    if  2  else  3  then  cutscene-leave-alone ;
\ room 0x08 (D_003F31D8): the hanging object named by the handler's string 0xA - byte 3 0 sets
\ it still (+0x30 / +0x38 0, travel +0x3C 0.9); 1: the player's travel (+0x3C, its last move's
\ length) past 5 makes it creak (sounds 4 / 5 by turns, event bit 7) and swing for 20 frames:
\ its tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 a frame
: castle-1f-13.cmd01 ( b0 -- )   \ (the hanging thing, the room's string $A: 0 still (resting at 0.9 of a push); 1 her steps
    \ push it - past 5 it creaks and swings 20 frames, event flag 7 toggled with her sound 4 / 5)
    dup 0= if  drop  0e $A room-object $30 object!  0e $A room-object $38 object!  0.9e $A room-object $3C object!  exit  then
    1 = if  0 $A 1 1 7 4 5 5e hangers-swing-one  then ;
\ door be16 cmd[3..4]: Progress_DoorOpen
: castle-1f-13.cond00? ( b0 b1 -- flag )  swap 8 lshift or door-open? ;
\ room 0x08 (Room08_Cond01_ptmf): the stalker is there but not about
: castle-1f-13.cond01? ( -- flag )  stalker-there? stalker-told? 0= and ;

: castle-1f-13.enter ( -- )   \ 003F2500
    0 $35 castle-1f-13.cond00? if
        0 part-shown
    else
        1 part-shown
    then
    found-file-227 story-flag? if
        found-file-227-2 story-flag? not if
            found-file-227-2 story-flag-set
            8 0 248 4 15 -1 0 0.0 creature-place
        then
        4 1 object-hidden
        5 1 object-hidden
        6 1 object-hidden
        7 1 object-hidden
        8 1 object-hidden
        9 1 object-hidden
        room-sounds
        $1A 1.0 0 bgm
    else
        4 0 object-hidden
        5 0 object-hidden
        6 0 object-hidden
        7 0 object-hidden
        8 0 object-hidden
        9 0 object-hidden
    then
    flowers-laid story-flag? if
        c1f-13-flag1 story-flag? not if
            c1f-13-flag1 story-flag-set
            8 0 274 4 15 -1 0 0.0 creature-place
            8 0 448 4 15 -1 0 0.0 creature-place
        then
    then
    cb1-7-flag2 story-flag? if
        castle-1f-13.cond01? summoner-on state-flag? and if
            stalker-who char-activate
            $FE 8 376 2 stalker-to-room
            stalker-who 4 -1 char-camera
            stalker-who char-full-health
            stalker-who 0 stalker-command
            stalker-who 0 stalker-search-delay
            stalker-who $178 120 char-to-tri-facing
            summoner-own-cooldown
        then
        cb1-7-flag2 story-flag-clear
    then
    c1f-13-antimony-powder-found story-flag? c1f-13-antimony-powder story-flag? not and if
        0 -254.5 1.0 -95.0 flicker-sprite
    then
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    0 castle-1f-13.cmd01
    c1f-13-chicken-jerky story-flag? not if
        1 -283.73 1.0 2.22 flicker-sprite
    then
;

: castle-1f-13.act09 ( -- )   \ 003F2E60
    stalker-who camera-follow
    4 ebit-clear
    $B 0 pvar? if
        0 chance? if
            4 ebit-set
        then
    else $B 1 pvar? if
        $19 chance? if
            4 ebit-set
        then
    else $B 2 pvar? if
        $32 chance? if
            4 ebit-set
        then
    else $B 3 pvar? if
        $4B chance? if
            4 ebit-set
        then
    then then then then
    2 creature-action? if
        4 ebit-set
    then
    4 stalker-alert? if
        4 ebit-clear
    then
    4 ebit? if
        0 stalker-who $A action
    else
        $78 1 summoner-wait
    then
    $B pvar-inc
    exit
;

: castle-1f-13.char-enter ( -- )   \ 003F2640
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        4 -1 player-camera
    then
    0 4 -1 area-camera
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
        3 3 player-camera
    then
    3 3 3 area-camera
    fiona-who self-is? if
        $80 exit-taken? $81 exit-taken? or $82 exit-taken? or if
            fiona-who $10E 70 char-to-tri-facing
            0 0 player-camera
            camera-restart
            $81 exit-taken? if
                0 fiona-who 6 action
            else
                0 fiona-who 0 action
            then
        then
    then
    stalker-who self-is? if
        fiona-hidden state-flag? if
            5 ebit-set
            castle-1f-13.act09
        else
            5 ebit-clear
        then
    then
;

: castle-1f-13.phase1 ( -- )   \ 003F2780
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
    $C 0 0 1 chars-area-camera
    $D 1 1 1 chars-area-camera
    $E 1 1 1 chars-area-camera
    $F 0 0 1 chars-area-camera
    $14 0 0 1 chars-area-camera
    $15 3 3 1 chars-area-camera
    $16 0 0 1 chars-area-camera
    $17 2 2 1 chars-area-camera
    $18 0 0 1 chars-area-camera
    $19 4 -1 1 chars-area-camera
    $1A 0 0 1 chars-area-camera
    $1B 4 -1 1 chars-area-camera
    $1C 0 0 1 chars-area-camera
    $1D 4 -1 1 chars-area-camera
    fiona-who 6 char-entered-area? fiona-who $A char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 7 char-entered-area? fiona-who 8 char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        2 exit-prepare
    then
    fiona-who $B char-entered-area? if
        3 exit-prepare
    then
    0 -277.0 0.0 -142.0 6 10 0 zone
    fiona-who 0 8 char-zone-bits? if
        1 castle-1f-13.cmd01
    then
    c1f-13-antimony-powder-found story-flag? not if
        1 -254.5 0.0 -95.0 $A 5 0 zone
        hewie-who 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var c1f-13-antimony-powder-found var-set
                taken-flag-var c1f-13-antimony-powder var-set
                sparkle-fx-var 0 var-set
                sparkle-x-var -254500 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var -95000 var-set
                hewie-zone-var 1 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: castle-1f-13.phase2 ( -- )   \ 003F28A0
    c1f-13-chicken-jerky story-flag? not if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            $D calm offer-action
        then
    then
    fiona-who $13 char-in-area? if
        stalker-who char-here? not if
            7 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    fiona-who $10 $32 char-faces-area? if
        1 unhurried offer-action
    then
    fiona-who $11 char-in-area? fiona-who 45 $32 char-heading? and if
        2 unhurried offer-action
    then
    fiona-who $12 char-in-area? fiona-who 45 $32 char-heading? and if
        3 unhurried offer-action
    then
    c1f-13-antimony-powder-found story-flag? c1f-13-antimony-powder story-flag? not and if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            $C calm offer-action
        then
    then
;

: castle-1f-13.phase3 ( -- )   \ 003F2910
    -219.9 59.3 20.8 -249.2 59.3 -7.4 -194.2 59.3 -5.9 -223.5 59.3 -34.2 lights-doorway
    -180.0 20.0 -119.9 -200.0 20.0 -119.9 -180.0 0.0 -119.9 -200.0 0.0 -119.9 lights-doorway
;

: castle-1f-13.phase5 ( -- )   \ 003F2980
    stalker-who char-busy? 6 ebit? and if
        stalker-who $1C0 -275.75 41.09 -60 char-to-xz
        stalker-who 0 0 char-camera
        6 ebit-clear
    then
;

: castle-1f-13.act00 ( -- )   \ 003F29A0
    found-file-227 story-flag? not if
        1 self-scripted
        hewie-who char-here? if
            0 hewie-who 4 action-force
        then
        3 1 movie-play
        2 cutscene-start
        yield
        yield
        scene-wait-ready
        hewie-who 1 char-visible
        1 castle-1f-13.cmd00
        $1E $C8 movie-param
        3 char-start
        4 3 char-hand-over
        room-sounds
        panic-eased
        $FFFF message-prepare
        1 result? if
            scene-play
        then
        4 1 object-hidden
        5 1 object-hidden
        6 1 object-hidden
        7 1 object-hidden
        8 1 object-hidden
        9 1 object-hidden
        fiona-who $1D0 -80 char-to-tri-facing
        0 0 player-camera
        camera-restart
        [: 6 sound-bank-loaded? ;] wait-until
        $FE 8 383 2 stalker-to-room
        stalker-who -1 -1 char-camera
        stalker-who char-activate
        summoner-own-cooldown
        0 stalker-who 5 action
        hewie-who char-here? if
            $2F 0 -1 hewie-to-room
        then
        lying-anim 0 self-anim-blend
        3 0 char-remove
        4 0 char-remove
        $1A 1.0 0 bgm
        $F $41 fade
        fiona.anims:get-up-anim play-out
        found-file-227 story-flag-set
        found-file-227-2 story-flag-set
        8 0 248 4 15 -1 0 0.0 creature-place
        wait-fade
        $21 resident-flag-set
        $227 item-give
        0 self-scripted
    else
        self-wait-done
        camera-restart
        $F 1 fade
        wait-fade
        scene-locked state-flag-clear
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    1 exit-prepare
    self-idle-or-end
;

: castle-1f-13.act01 ( -- )   \ 003F2AE0
    self-wait-done
    look-at-anim self-anim
    $28 wait-frames
    0 ebit? not if
        0 say   \ "I can't believe the balcony collapsed. It's like something out of a nightmare."
        0 ebit-set
    else
        1 say   \ "I'm not going to be able to get up there."
    then
    self-wait-anim
    self-idle-or-end
;

: castle-1f-13.act02 ( -- )   \ 003F2B00
    1 ebit? not if
        2 say   \ "A door?"
        reach-out-anim play-out
        reach-back-anim play-out
        3 say   \ "What?! It's just a wall made to look like a door."
        1 ebit-set
    else
        4 say   \ "Why would someone do this? Is it just some random prank?"
    then
    self-idle-or-end
;

: castle-1f-13.act03 ( -- )   \ 003F2B20
    2 ebit? not if
        2 say   \ "A door?"
        reach-out-anim play-out
        reach-back-anim play-out
        3 say   \ "What?! It's just a wall made to look like a door."
        2 ebit-set
    else
        4 say   \ "Why would someone do this? Is it just some random prank?"
    then
    self-idle-or-end
;

: castle-1f-13.act04 ( -- )   \ 0047A9AC
    begin
        yield
    again
;

: castle-1f-13.act05 ( -- )   \ 003F2B40
    6 ebit-set
    self-wait-done
    stalker-who $B char-file-load
    stalker-who char-file-use
    1 self-noclip
    1 self-scripted
    yield
    stalker-who -219.01 60.17 14.42 -60 char-to-xyz
    1 self-noclip
    1 self-scripted
    stalker-who 0 char-visible
    stalker-who 0 char-silent
    0 7 -147.0 60.0 14.5 0 0 sound
    $A wait-frames
    1 7 -155.0 60.0 14.5 0 0 sound
    $A wait-frames
    0 7 -163.0 60.0 14.5 0 0 sound
    $A wait-frames
    1 7 -171.0 60.0 14.5 0 0 sound
    $A wait-frames
    0 7 -179.0 60.0 14.5 0 0 sound
    $A wait-frames
    1 7 -187.0 60.0 14.5 0 0 sound
    $A wait-frames
    0 7 -195.0 60.0 14.5 0 0 sound
    $A wait-frames
    1 7 -203.0 60.0 14.5 0 0 sound
    $A wait-frames
    0 7 -211.0 60.0 14.5 0 0 sound
    $A wait-frames
    1 7 -219.0 60.0 14.5 0 0 sound
    0 room-motion 5 self-anim-blend8
    0 0 var-set
    begin
        fiona-who self-touching? if
            stalker-who 0 fiona-thrown
        then
        0 60 var? if
            0 -283.0 1.0 37.0 0 0 0 0 dust
            0 -280.0 1.0 42.0 0 0 0 0 dust
            stalker-who $B 6 char-sound
        then
        0 var-inc
        self-at-motion-event? not while
        yield
    repeat
    stalker-who 0 0 char-camera
    stalker-who char-full-health
    stalker-who 2 stalker-command
    stalker-who 0 stalker-search-delay
    summoner-own-cooldown
    6 ebit-clear
    self-idle-or-end
;

: castle-1f-13.act06 ( -- )   \ 003F2CC0
    yield
    camera-restart
    self-wait-done
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $1E $C8 movie-param
    $2F room-preload
    3 char-start
    4 3 char-hand-over
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    stalker-who action-end
    stalker-who char-done
    world-held state-flag-set
    3 0 char-remove
    4 0 char-remove
    $20 resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-1f-13.act08 ( -- )   \ 003F2E20
    3 ebit? if
        2 avoid-prompt
    then
    in-panic? not if
        fiona-who seized-sound 5 char-sound
    then
    fiona-hidden state-flag-clear
    fiona-who camera-follow
    stalkers-stay state-flag-set
    $114 -258.97 -140.95 79 $207 5 self-move-to
    self-wait-done
    4 $14 blend-out
    0 self-scripted
    0 self-noclip
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-1f-13.act07 ( -- )   \ 003F2D60
    stalkers-stay state-flag-set
    5 ebit-clear
    1 self-scripted
    3 ebit-clear
    self-wait-done
    $1D4 -267.0 -140.0 -110 $FFFF 5 self-move-to
    self-wait-done
    1 self-noclip
    1 self-scripted
    crouch-down-anim play-out
    $122 -291.573 -146.706 -110 $803 5 self-move-to
    self-wait-done
    $122 -291.573 -146.706 80 $803 5 self-move-to
    self-wait-done
    crouching-anim self-anim
    fiona-hidden state-flag-set
    stalkers-stay state-flag-clear
    0 avoid-prompt
    begin
        0 2 pad? not if
            in-panic? 3 ebit? or if
                in-panic? not if
                    fiona-who seized-sound 5 char-sound
                then
                ['] castle-1f-13.act08 goto
            else
                2 panic-calm-in
                6 fiona-calm
                $1E fiona-recovery-lower
                5 ebit? stalker-who char-here? not and if
                    5 ebit-clear
                    1 avoid-prompt
                then
                yield
            then
        else
            fiona-hidden state-flag-clear
            fiona-who camera-follow
            stalker-who action-end
            2 game-mode? if
                ['] castle-1f-13.act08 goto
            then
            stalkers-stay state-flag-set
            $1D4 -267.0 -140.0 70 $803 5 self-move-to
            self-wait-done
            crouch-up-anim play-out
            0 self-scripted
            0 self-noclip
            stalkers-stay state-flag-clear
            self-idle-or-end
        then
    again
;

: castle-1f-13.act0A ( -- )   \ 003F2EB0
    self-wait-done
    $1A1 -238.675 -139.193 -90 $FFFF 5 self-move-to
    self-wait-done
    there-anim play-out
    3 ebit-set
    10 self-idle-anim
    [: fiona-who char-busy? ;] wait-while
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-1f-13.act0B ( -- )   \ 0047A9B0
    self-idle-or-end
;

: castle-1f-13.act0C ( -- )   \ 003F2EE0
    self-wait-done
    -254.5 -95.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $91 message-param-room
        $91 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-13-antimony-powder story-flag-set
            0 effect-remove
            $91 1 item-give-count
            0 $91 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-1f-13.act0D ( -- )   \ 003F2F40
    self-wait-done
    -283.73 2.22 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $60 message-param-room
        $60 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c1f-13-chicken-jerky story-flag-set
            1 effect-remove
            $60 1 item-give-count
            0 $60 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-1f-13.act0E ( -- )   \ 003F2FA0
    self-wait-done
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    $E 3 $FF char-load
    $F 4 figure-load
    2 5 $FF char-load
    3 char-start
    4 3 char-hand-over
    5 char-start
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $1E $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    4 0 char-remove
    5 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-13.act0F ( -- )   \ 003F3090
    self-wait-done
    $10 -150.0 23.7 98.6 $A $80 $80 $80 $40 specks
    $10 -300.0 29.0 55.0 $A $80 $80 $80 $40 specks
    $A -300.0 29.0 -95.0 $A $80 $80 $80 $40 specks
    $10 -150.0 23.7 -158.6 $C $80 $80 $80 $40 specks
    $E 3 $FF char-load
    $F 4 figure-load
    2 5 $FF char-load
    3 char-start
    4 3 char-hand-over
    5 char-start
    3 1 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    $1E $C8 movie-param
    1 castle-1f-13.cmd00
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    4 0 char-remove
    5 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-1f-13 register-scripts   \ (its phases and actions, by their names)
