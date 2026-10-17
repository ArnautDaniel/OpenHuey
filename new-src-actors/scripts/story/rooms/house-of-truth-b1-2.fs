\ story/rooms/house-of-truth-b1-2.fs - the event scripts of room house-of-truth-b1-2 ($85; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-2
USING: room-names story.words story.shared flag-names story-names fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant sound-step-var              \ (var 0) which of its sounds next (0..3)
1 constant sound-frames-var            \ (var 1) frames to the next sound (30)

\ room 0x85: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-2.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-2.enter ( -- )   \ 00432700
    room-sounds
    sound-step-var 0 var-set
    sound-frames-var 0 var-set
    htb1-2-lavender story-flag? not if
        0 -45.77 5.75 19.99 flicker-sprite
    then
;

: house-of-truth-b1-2.char-enter ( -- )   \ 00432730
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        2 2 player-camera
    then
    1 2 2 area-camera
    fiona-who self-is? if
        found-file-252 story-flag? not $80 exit-taken? and if
            world-held state-flag-set
            panic-eased
            0 fiona-who 1 action-force
        then
    then
;

: house-of-truth-b1-2.phase1 ( -- )   \ 004327D0
    0 exit-usable? if
        0 exit-check
    then
    over-music-e story-flag? if
        1 exit-usable? if
            1 exit-check
        then
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 2 2 1 chars-area-camera
    7 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    house-of-truth-b1-2.cmd00
    6 sound-bank-loaded? if
        sound-frames-var var-inc
        sound-frames-var 30 var? if
            sound-frames-var 0 var-set
            sound-step-var 0 var? if
                $40000003 6 -41.0 20.0 25.0 0 0 sound
            else sound-step-var 1 var? if
                $40000004 6 -41.0 20.0 25.0 0 0 sound
            else sound-step-var 2 var? if
                $40000005 6 -41.0 20.0 25.0 0 0 sound
            else sound-step-var 3 var? if
                $40000006 6 -41.0 20.0 25.0 0 0 sound
            then then then then
            sound-step-var var-inc
            sound-step-var 4 var? if
                sound-step-var 0 var-set
            then
        then
    then
;

: house-of-truth-b1-2.phase2 ( -- )   \ 004328A0
    fiona-who 8 $32 char-faces-area? if
        $84 calm-hewie-up offer-action
    then
    over-music-e story-flag? not if
        fiona-who 1 char-before-door? if
            0 always offer-action
        then
    then
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            3 always offer-action
        then
    then
    htb1-2-lavender story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            2 calm offer-action
        then
    then
;

: house-of-truth-b1-2.act00 ( -- )   \ 004328E0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $F $54 fade
    wait-fade
    scene-locked state-flag-set
    1 exit-prepare
    stalkers-stay state-flag-clear
    0 self-scripted
    $80 exit-check
    scene-locked state-flag-clear
    self-idle-or-end
;

: house-of-truth-b1-2.act01 ( -- )   \ 00432910
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    0 message-prepare   \ "The great Aureolus Belli once said, 'The most valuable thing in this world is t..."
    1 result? if
        scene-play
    then
    fiona-who $27 -0.18 14.47 -79 char-to-xz
    1 1 player-camera
    camera-restart
    $EF door-open-clear
    $E9 door-lock
    $EF door-lock
    doors-room-in
    found-file-252 story-flag-set
    $F $41 fade
    wait-fade
    $4A resident-flag-set
    $252 item-give
    scene-locked state-flag-clear
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-2.act02 ( -- )   \ 004329D0
    self-wait-done
    -45.77 19.99 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $41 message-param-room
        $41 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            htb1-2-lavender story-flag-set
            0 effect-remove
            $41 1 item-give-count
            0 $41 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-b1-2.act03 ( -- )   \ 00432A30
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
        1 say   \ "It's locked. I can't open it. I guess they have no intention of letting me go home..."
    then
    self-idle-or-end
;

: house-of-truth-b1-2.act04 ( -- )   \ 00432A60
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    0 message-prepare   \ "The great Aureolus Belli once said, 'The most valuable thing in this world is t..."
    1 result? if
        scene-play
    then
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-b1-2 register-scripts   \ (its phases and actions, by their names)
