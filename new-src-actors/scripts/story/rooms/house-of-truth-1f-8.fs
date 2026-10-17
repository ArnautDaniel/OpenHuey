\ story/rooms/house-of-truth-1f-8.fs - the event scripts of room house-of-truth-1f-8 ($91; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-8
USING: engine common actors messages room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ room 0x91 (Room91_Cmd00_ptmf): door 0's +0x68 (0)
: house-of-truth-1f-8.cmd00 ( -- )  0 true -1 doors-id send swing-door ;   \ (door 0 set opening, Doors_SetOpened: swung open here)
\ script variable 0 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0
: house-of-truth-1f-8.cmd01 ( -- )  0 var-down-by-hit ;
\ room 0x91: three grey smoke effects (Effect79B00, size 70) at the room's spots 1, 3, 0xB
\ (grey_three).
: house-of-truth-1f-8.cmd02 ( -- )   \ (the ceiling coming down: stones of three kinds falling from 70)
    1 70e 0e 0e 0.5e stones-falling  3 70e 0e 0e 0.5e stones-falling  11 70e 0e 0.5e 0.5e stones-falling ;
\ room 0x91: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-1f-8.cmd03 ( -- )  slam-shake ;

: house-of-truth-1f-8.enter ( -- )   \ 00436600
    room-sounds
    0 72.4 17.2 117.2 1 flame-in
    1 101.9 66.0 87.1 4 flame-in
    2 97.6 29.5 144.8 1 flame-in
    4 101.9 66.0 87.4 4 flame-in
    ht1f-9-flag2 story-flag? if
        0 barred-floor floor-on
        0 part-shown
    then
    tremor-warned story-flag? if
        5 1.0 0 bgm
    then
    ht1f-8-magist-medallion-found story-flag? ht1f-8-magist-medallion story-flag? not and if
        3 86.0 1.0 68.0 flicker-sprite
    then
    $80 exit-taken? not if
        shared.act95
        ht1f-9-flag2 story-flag? if
            house-of-truth-1f-8.cmd02
        then
    else
        2 ebit-set
    then
;

: house-of-truth-1f-8.char-enter ( -- )   \ 00436690
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 0 player-camera
    then
    1 0 0 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        2 2 player-camera
    then
    2 2 2 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $1FC 22.0 100.0 -90 char-to-xz
            0 0 player-camera
            0 fiona-who 1 action
            0.0 sound-volume-scale
        then
    then
;

: house-of-truth-1f-8.phase1 ( -- )   \ 00436780
    2 ebit? if
        $C0000030 $47 0.0 0.0 0.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 2 2 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 4 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        2 exit-prepare
    then
    ht1f-9-flag2 story-flag? over-music-f story-flag? not and if
        fiona-who 6 char-entered-area? 1 ebit? not and if
            1 ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
    $EB door-locked? if
        4 -37.0 0.0 177.0 7 20 0 zone
        fiona-who 4 char-in-zone? if
            0 1 track 4 action
        then
        hewie-who char-here? hewie-who 2 char-cond? not and fiona-who $C char-in-area? and hewie-who $C char-in-area? and fiona-who -37 177 $3C char-faces-xz? and 0 control-action? and if
            hewie-may-break-off? if
                0 hewie-who 5 action
            then
        then
    then
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    2 2 8 -4 0 zone-at-effect
    fiona-who 2 3 char-zone-bits? fiona-who 2 3 char-zone-bits-before? not and if
        fiona-who 2 char-effect-moving
    then
    hewie-who 2 3 char-zone-bits? hewie-who 2 3 char-zone-bits-before? not and if
        hewie-who 2 char-effect-moving
    then
    stalker-who 2 3 char-zone-bits? stalker-who 2 3 char-zone-bits-before? not and if
        stalker-who 2 char-effect-moving
    then
    ht1f-8-flag1 story-flag? not if
        fiona-who $B char-entered-area? if
            ht1f-8-flag1 story-flag-set
            $91 0 467 $88 0 -1 $5A 0.0 creature-place
            $91 0 131 $89 0 -1 $FFA6 0.0 creature-place
        then
    then
    ht1f-8-magist-medallion-found story-flag? not if
        3 86.0 0.0 68.0 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var ht1f-8-magist-medallion-found var-set
                taken-flag-var ht1f-8-magist-medallion var-set
                sparkle-fx-var 3 var-set
                sparkle-x-var 86000 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 68000 var-set
                hewie-zone-var 3 var-set
                0 hewie-who $8B action
            then
        then
    then
    house-of-truth-1f-8.cmd03
    6 sound-bank-loaded? if
        2 var-inc
        2 30 var? if
            2 0 var-set
            1 0 var? if
                $40000005 6 48.0 0.0 115.0 0 0 sound
            else 1 1 var? if
                $40000006 6 48.0 0.0 115.0 0 0 sound
            else 1 2 var? if
                $40000007 6 48.0 0.0 115.0 0 0 sound
            else 1 3 var? if
                $40000008 6 48.0 0.0 115.0 0 0 sound
            then then then then
            1 var-inc
            1 4 var? if
                1 0 var-set
            then
        then
    then
;

: house-of-truth-1f-8.phase2 ( -- )   \ 00436A00
    fiona-who $A char-in-area? fiona-who -22 $32 char-heading? and if
        $84 calm-hewie-up offer-action
    then
    -2147483646 scene-request? if
        fiona-who 0 char-before-door? if
            6 always offer-action
        then
    then
    ht1f-8-magist-medallion-found story-flag? ht1f-8-magist-medallion story-flag? not and if
        3 3 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            2 calm offer-action
        then
    then
;

: house-of-truth-1f-8.phase3 ( -- )   \ 00436A40
    ht1f-9-flag2 story-flag? 2 ebit? not and if
        $40000030 $47 0.0 0.0 0.0 0 0 sound
        2 ebit-set
    then
;

: house-of-truth-1f-8.act00 ( -- )   \ 00436A60
    stalkers-stay state-flag-set
    1 self-scripted
    2 exit-prepare
    $F $54 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    $80 exit-check
    self-idle-or-end
;

: house-of-truth-1f-8.act01 ( -- )   \ 00436A80
    1 self-scripted
    0 exit-prepare
    self-wait-done
    over-music-f story-flag-set
    $91 0 493 hewie-to-room
    hewie-who $1ED 45 char-to-tri-facing
    stalker-who char-activate
    $FE $92 -1 2 stalker-to-room
    stalker-who 0 stalker-command
    stalker-who $8D 1 room-doors-state
    hewie-no-attack state-flag-set
    camera-restart
    house-of-truth-1f-8.cmd02
    $40000030 $47 0.0 0.0 0.0 0 0 sound
    $F $11 fade
    wait-fade
    0 1 track 3 action
    8 wait-frames
    fiona-who seized-sound 5 char-sound
    crouch-down-anim play-out
    crouching-anim self-anim
    $1E wait-frames
    crouch-up-anim self-anim
    2 say   \ "Whoa! What was that? It felt like a tremor... I should crouch down during large tremors..."
    tremor-warned story-flag-set
    5 1.0 0 bgm
    tremor-warned-2 story-flag-set
    1 track action-end
    shared.act95
    0 5 var-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-8.act02 ( -- )   \ 00436B10
    self-wait-done
    86.0 68.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $75 message-param-room
        $75 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            ht1f-8-magist-medallion story-flag-set
            3 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: house-of-truth-1f-8.act03 ( -- )   \ 00436B70
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    0 0 $1E rumble
    1 $FF $1E rumble
    $31 $87 0.0 0.0 0.0 0 0 sound
    self-frames-reset
    begin
        $1E frames? not while
        1.0 camera-shake
        yield
    repeat
    1 $80 $1E rumble
    self-frames-reset
    begin
        $1E frames? not while
        0.5 camera-shake
        yield
    repeat
    begin
        0.1 camera-shake
        yield
    again
;

: house-of-truth-1f-8.act04 ( -- )   \ 00436BD0
    0 exit-door-open? not if
        house-of-truth-1f-8.cmd01
        $91 5 -37.0 10.0 177.0 0 0 sound
        0 0 var? if
            $EB door-unlock
            house-of-truth-1f-8.cmd00
            $EB door-open-set
        then
    then
    16 wait-frames
    self-idle-or-end
;

: house-of-truth-1f-8.act05 ( -- )   \ 00436C10
    self-wait-done
    hewie-bark
    self-wait-done
    $202 -4.422 167.237 -73 $FFFF $A self-move-to
    self-wait-done
    $90 -32.0 177.5 0.5 50 hewie-go-to
    8 wait-frames
    0 exit-door-open? not if
        $EB door-unlock
        $91 5 -37.0 10.0 177.0 0 0 sound
        house-of-truth-1f-8.cmd00
        $EB door-open-set
    then
    self-wait-done
    self-idle-or-end
;

: house-of-truth-1f-8.act06 ( -- )   \ 00436C60
    self-wait-done
    0 ebit? not if
        0 self-through-exit
        self-wait-done
        rattle-behind-slow-anim play-out
        -1 self-idle-anim
        7 wait-frames
        0 ebit-set
    then
    0 say   \ "Something seems to be snagged on the other side of the door, keeping it from opening. D..."
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-1f-8 register-scripts   \ (its phases and actions, by their names)
