\ story/rooms/castle-1f-10.fs - the event scripts of room castle-1f-10 ($29; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-10
USING: room-names story.words story.shared flag-names story-names ;

\ the stalker in play is chasing (+0x153C 2, 6 or 7, not +0xC4 2) with the progress state 2: in
\ this room, whether the camera sees it; elsewhere 1
: castle-1f-10.cond00? ( -- flag )   \ (out of view: Camera_OutOfView)
    hunting-debilitas? 0= if  false exit  then  2 game-mode? 0= if  false exit  then
    stalker-in-this-room? if  stalker-in-view? 0=  else  true  then ;
\ no thing of kind 3 lies about
: castle-1f-10.cond01? ( -- flag )  3 thing-lying? 0= ;

: castle-1f-10.enter ( -- )   \ 00403F90
    c1f-10-flag3 story-flag? if
        2 1 object-hidden
    then
    0 24.35 9.0 19.7 0 flame-in
    c1f-10-beef-jerky story-flag? not if
        1 11.0 12.0 -58.5 flicker-sprite
    then
    found-file-20b story-flag? c1f-10-flag1 story-flag? not and if
        1 0 $1000010 nav-group
        3 char-start
        2 char-activate
        2 $9000 1 0 char-anim-hold
        2 $83 12.913 14.864 90 char-to-xz
    then
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    0 0 $14 door-bits
    got-bisque-doll story-flag? if
        $93 1 item-count? not castle-1f-10.cond01? and if
            0 1 $14 door-bits
            2 ebit-set
        then
    then
;

: castle-1f-10.char-enter ( -- )   \ 00404040
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 -1 player-camera
    then then
    0 1 -1 area-camera
;

: castle-1f-10.phase1 ( -- )   \ 00404080
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    3 1 -1 1 chars-area-camera
    4 2 -1 1 chars-area-camera
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
    1 7.42 0.0 22.15 $F 9 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                $1F 1 var-set
                $1F 9.0 hewie-look-zone
            then
        then
    then
;

: castle-1f-10.phase2 ( -- )   \ 00404100
    found-file-20b story-flag? c1f-10-flag1 story-flag? not and if
    else c1f-10-flag3 story-flag? not if
        fiona-who 5 char-in-area? fiona-who 0 $2D char-heading? and if
            2 game-mode? in-panic? not and if
                castle-1f-10.cond00? if
                    5 0 6 scene-change
                else
                    $8016 examine-message   \ "I have to get away! I don't have time for this!"
                then
            else
                5 1 0 scene-change
            then
        then
    then then
    c1f-10-beef-jerky story-flag? not if
        0 1 7 4 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            5 3 4 scene-change
        then
    then
    found-file-20b story-flag? c1f-10-flag1 story-flag? not and if
        fiona-who 2 $14 0 $2D char-touching-facing? if
            c1f-10-flag2 story-flag? not if
                5 2 0 scene-change
            else 1 ebit? not if
                5 6 0 scene-change
            then then
        then
    then
    fiona-who 6 char-in-area? fiona-who -45 $32 char-heading? and if
        5 4 0 scene-change
    then
    fiona-who 7 char-in-area? fiona-who 45 $32 char-heading? and if
        5 5 0 scene-change
    then
    2 ebit? if
        fiona-who 8 $3C char-faces-area? if
            5 7 4 scene-change
        then
    then
;

: castle-1f-10.phase5 ( -- )   \ 00404198
    found-file-20b story-flag? c1f-10-flag1 story-flag? not and if
        2 action-end
        2 char-done
    then
;

: castle-1f-10.act00 ( -- )   \ 004041B0
    hewie-who $29 1 room-doors-state
    1 self-scripted
    $FE $29 116 2 stalker-to-room
    stalker-who $74 180 char-to-tri-facing
    stalker-who 0 -1 char-camera
    stalker-who 1 char-visible
    stalker-who action-end
    stalker-who char-done
    $F $44 fade
    3 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-here? if
        hewie-who action-end
        hewie-who char-done
        0 ebit-set
    then
    $20 door-open-set
    doors-room-in
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    stalker-who action-end
    stalker-who char-done
    0 self-scripted
    world-held state-flag-set
    0 exit-prepare
    $20 door-open-clear
    0 ebit? if
        $80 exit-check
    else
        $81 exit-check
    then
    hewie-who $29 0 room-doors-state
    c1f-10-flag3 story-flag-set
    $22 resident-flag-set
    self-idle-or-end
;

: castle-1f-10.act01 ( -- )   \ 00404280
    self-wait-done
    $98 7.28 20.316 15 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 20.0 0.0 event-camera
    $A01 $A self-anim-blend
    0 message   \ "A doll lies on display here. The owner must cherish it."
    wait-message
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-10.act02 ( -- )   \ 004042D0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    4 0 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    fiona-who $4B -13.414 -1.767 -6 char-to-xz
    0 self-idle-anim
    2 $83 12.913 14.864 90 char-to-xz
    2 $9000 1 0 char-anim-hold
    c1f-10-flag2 story-flag-set
    hewie-who 0 char-visible
    hewie-who action-end
    camera-restart
    $F $41 fade
    wait-fade
    $29 resident-flag-set
    7 message-param-room
    7 1 item-give-count
    0 7 item-tab
    item-fanfare
    $8012 message   \ "You've obtained ."
    wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-10.act03 ( -- )   \ 004043D0
    self-wait-done
    11.0 -58.5 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $61 message-param-room
        $61 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c1f-10-beef-jerky story-flag-set
            1 effect-remove
            $61 1 item-give-count
            0 $61 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-1f-10.act04 ( -- )   \ 00404428
    self-wait-done
    $A01 $A self-anim-blend
    1 message   \ "There is no way I would even consider lying down here."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-1f-10.act05 ( -- )   \ 00404440
    self-wait-done
    fiona-who $9C 12.0 15.0 90 char-to-xz
    1 10.0 10.0 0.0 2.0 event-camera
    world-frozen state-flag-set
    1 self-scripted
    self-frames-reset
    self-wait-16
    2 message   \ "It's sort of like a... Let's just forget it. 'Out of sight, out of mind.' Or so they say."
    wait-message
    self-frames-reset
    4 self-wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-1f-10.act06 ( -- )   \ 00404490
    1 self-scripted
    self-wait-done
    2 $9002 0 7 char-anim-hold
    2 wait-char-anim
    2 $9001 1 7 char-anim-hold
    1 ebit-set
    self-idle-or-end
;

: castle-1f-10.act07 ( -- )   \ 004044B0
    self-wait-done
    -3.61 2.23 self-turn-to-xz
    self-wait-done
    $902 self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $93 message-param-room
    $93 $63 item-count? if
        $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
        wait-message
    else
        $93 1 item-give-count
        2 ebit-clear
        0 0 $14 door-bits
        0 $93 item-tab
        item-fanfare
        $8011 message   \ "You've obtained ."
        wait-message
    then
    $903 self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-1f-10.act08 ( -- )   \ 00404500
    self-wait-done
    0 24.35 9.0 19.7 0 flame-in
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    2 3 $FF char-load
    3 char-start
    3 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-1f-10.act09 ( -- )   \ 004045E0
    self-wait-done
    0 24.35 9.0 19.7 0 flame-in
    8 25.0 12.0 19.0 4 $80 $80 $80 $20 specks
    8 9.4 7.3 -57.3 8 $60 $60 $60 $40 specks
    8 -12.0 21.0 -27.0 8 $70 $70 $70 $40 specks
    2 3 $FF char-load
    3 char-start
    4 0 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-1f-10.enter castle-1f-10 0 room-script!
' castle-1f-10.char-enter castle-1f-10 6 room-script!
' castle-1f-10.phase1 castle-1f-10 1 room-script!
' castle-1f-10.phase2 castle-1f-10 2 room-script!
' castle-1f-10.phase5 castle-1f-10 5 room-script!
' castle-1f-10.act00 castle-1f-10 $00 action-script!
' castle-1f-10.act01 castle-1f-10 $01 action-script!
' castle-1f-10.act02 castle-1f-10 $02 action-script!
' castle-1f-10.act03 castle-1f-10 $03 action-script!
' castle-1f-10.act04 castle-1f-10 $04 action-script!
' castle-1f-10.act05 castle-1f-10 $05 action-script!
' castle-1f-10.act06 castle-1f-10 $06 action-script!
' castle-1f-10.act07 castle-1f-10 $07 action-script!
' castle-1f-10.act08 castle-1f-10 $08 action-script!
' castle-1f-10.act09 castle-1f-10 $09 action-script!
' castle-1f-10.char-enter castle-1f-10 $38 action-script!
' castle-1f-10.char-enter castle-1f-10 $3D action-script!
