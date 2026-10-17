\ story/rooms/castle-b1-7.fs - the event scripts of room castle-b1-7 ($1D; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-7
USING: engine common room-names story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

\ room 0x1D (Room1D_Cmd00_ptmf): script variable 0 = 2 .. 5 at random
: castle-b1-7.cmd00 ( -- )  0  4 random 2 +  var-set ;   \ (script variable 0: 2 .. 5 at random)
\ room 0x1D (Room1D_Cmd01_ptmf): door 0's +0x74 (0, or -0.08 by byte 3)
: castle-b1-7.cmd01 ( b0 -- )   \ (door 0 turned from rest: 0, or -0.08 - a crack open)
    if  -0.08e  else  0e  then  rad>deg  0 true door-swing ;
\ Fiona in move 5, room 0x1D's flag 0 not set and its exit 0's door shut
: castle-b1-7.cond00? ( -- flag )
    fiona-move-sub 5 <> if  false exit  then
    $1D 0 room-door-flag? if  false exit  then  $1D 0 room-exit-open? 0= ;

: castle-b1-7.act04 ( -- )   \ 003FD720
    room-flag-2d state-flag-clear
    hewie-who char-here? 1 hewie-side? and if
        $1E 0 -1 hewie-to-room
        1800 2 hewie-anim
    then
    exit
;

: castle-b1-7.enter ( -- )   \ 003FD370
    room-sounds
    hewie-who $1E char-in-room? 119 hewie-action? and if
        room-flag-2d state-flag-set
        $1D 1 252 hewie-to-room
        got-study-key story-flag? not if
            0 hewie-who 2 action
        then
    else
        castle-b1-7.act04
    then
    0 30.0 -27.65 -9.8 0 flame-in
    got-study-key story-flag? not if
        1 -12.0 -40.0 -25.0 flicker-sprite
    then
    cb1-7-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        0 part-shown
        1 part-hidden
    else
        0 keeps-people-off floor-off
        0 part-hidden
        1 part-shown
        cb1-7-medallion story-flag? not if
            2 31.0 -40.0 13.0 flicker-sprite
        then
    then
    0 9 0.812 0.687 0.187 0.312 zone-rect
    cb1-7-medallion-found story-flag? cb1-7-medallion-2 story-flag? not and if
        3 29.0 1.0 21.0 flicker-sprite
    then
    0 0 0 $1ED $245 obstacle-place
;

: castle-b1-7.char-enter ( -- )   \ 003FD420
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
    fiona-who self-is? if
        0 exit-taken? if
            2 map-page
        then
    then
;

: castle-b1-7.phase1 ( -- )   \ 003FD470
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    3 1 0 1 chars-area-camera
    4 2 -1 1 chars-area-camera
    5 3 1 1 chars-area-camera
    fiona-who 3 char-entered-area? if
        2 map-page
    then
    fiona-who 4 char-entered-area? if
        1 map-page
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
    0 $1ED obstacle-on? if
        $26 door-locked? if
            $26 door-unlock
        then
    then
    -2147483644 scene-request? not castle-b1-7.cond00? and if
        $26 door-locked? not if
            $26 door-lock
        then
    then
    1 ebit? not if
        $26 door-locked? if
            stalker-who $1C char-in-room? if
                0 $F2 9 action
            then
        then
    else $26 door-locked? not if
        $F2 action-end
        0 castle-b1-7.cmd01
        stalker-who 1 stalker-command
        stalkers-stay state-flag-clear
        1 ebit-clear
    then then
    cb1-7-medallion-found story-flag? not if
        1 29.0 0.0 21.0 $A 5 0 zone
        hewie-who 1 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var cb1-7-medallion-found var-set
                taken-flag-var cb1-7-medallion-2 var-set
                sparkle-fx-var 3 var-set
                sparkle-x-var 29000 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 21000 var-set
                hewie-zone-var 1 var-set
                0 hewie-who $8B action
            then
        then
    then
;

: castle-b1-7.phase2 ( -- )   \ 003FD580
    cb1-7-breakable-broken story-flag? not if
        0 31.0 -41.0 13.0 5 8 1 zone
        self-who 0 char-in-zone? if
            cb1-7-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            0 part-hidden
            1 part-shown
            31.0 -41.0 13.0 0 -2143272896 0 0.0 shards-at
            $88 5 31.0 -41.0 13.0 0 0 sound
            $40 $1B0 noise
            2 31.0 -40.0 13.0 flicker-sprite
        then
    else cb1-7-medallion story-flag? not if
        0 2 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            7 calm offer-action
        then
    then then
    fiona-who 7 $3C char-faces-area? if
        1 unhurried offer-action
    then
    hewie-who char-here? not 1 hewie-side? not or got-study-key story-flag? not and fiona-who 6 char-in-area? and fiona-who -12 -25 $32 char-faces-xz? and if
        5 unhurried offer-action
    then
    -2147483646 scene-request? if
        8 always offer-action
    then
    fiona-who 8 char-in-area? fiona-who 0 $32 char-heading? and if
        $B unhurried offer-action
    then
    cb1-7-medallion-found story-flag? cb1-7-medallion-2 story-flag? not and if
        1 3 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            $A calm offer-action
        then
    then
;

: castle-b1-7.phase3 ( -- )   \ 003FD660
    camera-setup-changed? if
        1 camera-mode? if
            20.0 30.0 2000.0 2000.0 depth-range
        else
            depth-range-off
        then
    then
;

: castle-b1-7.act00 ( -- )   \ 003FD680
    1 self-scripted
    self-wait-done
    2 wait-counter
    got-study-key story-flag-set
    1 effect-remove
    10 hewie-trust
    3 message-param-room
    3 1 item-give-count
    0 3 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    40 hewie-trust
    $827C item-give
    counter-inc
    0 self-scripted
    self-idle-or-end
;

: castle-b1-7.act01 ( -- )   \ 0047AA88
    self-wait-done
    2 message   \ "The furniture is covered with freshly washed sheets."
    wait-message
    self-idle-or-end
;

: castle-b1-7.act03 ( -- )   \ 003FD700
    1 wait-counter
    hewie-bark
    self-wait-done
    $92 -17.0 -30.0 45 $FFFF 5 self-move-to
    self-wait-done
    $1C03 self-anim
    self-wait-anim
    counter-inc
    3 wait-counter
    self-idle-or-end
;

: castle-b1-7.act02 ( -- )   \ 003FD6C0
    self-wait-done
    hewie-who $FC 0 char-to-tri-facing
    sit-anim self-anim
    self-wait-anim
    fiona-who self-look-at
    yield
    begin
        35 fiona-started? 44 fiona-started? or 2 game-mode? not and fiona-who 6 char-in-area? and fiona-who -12 -25 $32 char-faces-xz? and if
            0 counter-set
            0 $F1 6 action
            ['] castle-b1-7.act03 goto
        then
        yield
    again
;

: castle-b1-7.act05 ( -- )   \ 003FD740
    self-wait-done
    look-low-anim $A self-anim-blend
    self-wait-anim
    0 message   \ "Looks like a key or something is lying on the ground. I can't reach it from here..."
    wait-message
    self-frames-reset
    8 self-wait-frames
    self-who 4 -1 char-camera
    self-frames-reset
    8 self-wait-frames
    1 message   \ "There is a small hole-floor in the wall. That could be my way to the key..."
    wait-message
    self-frames-reset
    8 self-wait-frames
    3 1 player-camera
    camera-restart
    self-idle-or-end
;

: castle-b1-7.act06 ( -- )   \ 003FD780
    begin
        fiona-free? not while
        yield
    repeat
    counter-inc
    panic-eased
    0 fiona-who 0 action-force
    self-idle-or-end
;

: castle-b1-7.act07 ( -- )   \ 003FD7A0
    self-wait-done
    31.0 13.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cb1-7-medallion story-flag-set
            2 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-b1-7.act08 ( -- )   \ 003FD7F8
    self-wait-done
    -90 self-turn-angle
    self-wait-done
    3 message   \ "All of this stuff is blocking the door and I can't get out."
    wait-message
    self-idle-or-end
;

: castle-b1-7.act09 ( -- )   \ 003FD810
    1 ebit-set
    stalkers-stay state-flag-set
    self-frames-reset
    $3C self-wait-frames
    castle-b1-7.cmd00
    begin
        1 6 -40.0 0.0 -20.0 0 0 sound
        1 castle-b1-7.cmd01
        yield
        0 castle-b1-7.cmd01
        $19 chance? if
            self-frames-reset
            self-wait-16
        then
        self-frames-reset
        self-wait-16
        1 6 -40.0 0.0 -20.0 0 0 sound
        1 castle-b1-7.cmd01
        yield
        0 castle-b1-7.cmd01
        $32 chance? if
            self-frames-reset
            self-wait-16
            self-frames-reset
            self-wait-16
        then
        self-frames-reset
        $3C self-wait-frames
        0 0 var? not while
        0 var-dec
        yield
    repeat
    $26 door-locked? if
        cb1-7-flag1 story-flag? not if
            cb1-7-flag1 story-flag-set
        else $4B chance? if
            cb1-7-flag2 story-flag-set
        then then
        stalker-who action-end
        stalker-who char-done
    else
        stalker-who 1 stalker-command
    then
    stalkers-stay state-flag-clear
    1 ebit-clear
    self-idle-or-end
;

: castle-b1-7.act0A ( -- )   \ 003FD8A0
    self-wait-done
    29.0 21.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cb1-7-medallion-2 story-flag-set
            3 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-b1-7.act0B ( -- )   \ 003FD900
    self-wait-done
    cb1-7-medallion-3 story-flag? not if
        0 self-turn-angle
        self-wait-done
        4 message   \ "Huh? There is something in the drain."
        wait-message
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            cb1-7-medallion-3 story-flag-set
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
    else
        5 message   \ "There's nothing particularly interesting here."
        wait-message
    then
    self-idle-or-end
;

: castle-b1-7.phase5 ( -- )   \ 0047AA80
    castle-b1-7.act04
;

\ ---- registered ----
castle-b1-7 register-scripts   \ (its phases and actions, by their names)
