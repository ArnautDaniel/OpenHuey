\ story/rooms/old-mansion-1f-16.fs - the event scripts of room old-mansion-1f-16 ($55; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-16
USING: room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant locked-remarked             \ (bit 0) its first remark made: "The door is locked..."
1 constant area2-reacted               \ (bit 1) her reaction in area 2 made (once a visit)
0 constant unlock-sound                \ (snd 0) a door unlocked with its key

\ Room55_Cmd00
: old-mansion-1f-16.cmd00 ( b0 -- )   \ (Room55: b0 0 the ceiling's glass reflecting (60 x 60 at 45) and the light rippling
    \ up there; else the light rippling low down and drops falling)
    0= if
        0 $1B $80  30e 45e -30e  -30e 45e -30e  30e 45e 30e  -30e 45e 30e  lit-quad
        3 $80808080  0e 45.1e 0e  160e 0.5e 0.1e  effects:caustic drop
    else
        3 $80808080  0e -5.9e 0e  160e 0.5e 0.1e  effects:caustic drop  effects:rain
    then ;

: old-mansion-1f-16.enter ( -- )   \ 00420B40
    room-sounds
    $11 1.0 0 bgm
    om2f-12-flag1 story-flag? not if
        0 part-shown
        0 old-mansion-1f-16.cmd00
    else
        1 part-shown
        1 old-mansion-1f-16.cmd00
    then
    om1f-16-refined-magnesia story-flag? not if
        0 65.56 94.52 19.77 flicker-sprite
    then
    1 $3FFF sound-volume
;

: old-mansion-1f-16.char-enter ( -- )   \ 00420B80
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
        1 1 player-camera
    then
    2 1 1 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        1 1 player-camera
    then
    3 1 1 area-camera
    player-who self-is? 4 exit-taken? and if
        player-who 4 char-to-exit
        1 1 player-camera
    then
    4 1 1 area-camera
    fiona-who self-is? if
        0 exit-taken? 2 exit-taken? or if
            3 map-page
        then
        1 exit-taken? 3 exit-taken? or if
            2 map-page
        then
    then
;

: old-mansion-1f-16.phase1 ( -- )   \ 00420CD0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    om3f-2-flag1 story-flag? not area2-reacted ebit? not and if
        fiona-who 2 char-in-area? if
            area2-reacted ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    else 2 exit-usable? if
        2 exit-check
    then then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    fiona-who 5 char-entered-area? if
        found-file-234 story-flag? not found-file-233 story-flag? and if
            stalker-who 0 char-file-load
        then
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        found-file-234 story-flag? not found-file-233 story-flag? and if
        then
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 8 char-entered-area? if
        3 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        3 exit-prepare
    then
    fiona-who $A char-entered-area? if
        4 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 map-page
    then
    fiona-who 6 char-left-area? if
        3 map-page
    then
    fiona-who 8 char-entered-area? if
        2 map-page
    then
    fiona-who 8 char-left-area? if
        3 map-page
    then
    fiona-who 9 char-entered-area? if
        2 map-page
    then
    fiona-who 9 char-left-area? if
        1 map-page
    then
;

: old-mansion-1f-16.phase2 ( -- )   \ 00420D80
    found-file-234 story-flag? not if
        fiona-who 1 char-before-door? if
            5 always offer-action
        then
    then
    -2147483646 scene-request? if
        fiona-who 4 char-before-door? if
            1 always offer-action
        then
    then
    fiona-who $B char-in-area? fiona-who 83 $3C char-heading? and if
        4 unhurried offer-action
    then
    om1f-16-refined-magnesia story-flag? not if
        0 0 5 5 0 zone-at-effect
        fiona-who 0 3 char-zone-bits? if
            3 calm offer-action
        then
    then
;

: old-mansion-1f-16.act00 ( -- )   \ 00420DD0
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    wait-fade
    world-held state-flag-set
    2 exit-check
    self-idle-or-end
;

: old-mansion-1f-16.act01 ( -- )   \ 00420DF0
    self-wait-done
    locked-remarked ebit? not 2 game-mode? or in-panic? or if
        4 self-through-exit
        self-wait-done
        in-panic? 2 game-mode? or if
            rattle-behind-slow-anim play-out
            -1 self-idle-anim
            7 wait-frames
        else
            rattle-behind-anim play-out
            -1 self-idle-anim
            7 wait-frames
            0 say   \ "The door is locked..."
        then
        locked-remarked ebit-set
    else
        4 wait-frames
        fiona-who $18C -64.3 0.0 -89 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        4 wait-frames
        1 say   \ "There's an odd marking on the door. What could it mean?"
        4 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        fiona-who $18C -64.3 1.25 -89 char-to-xz
        world-frozen state-flag-clear
        0 self-scripted
    then
    self-idle-or-end
;

: old-mansion-1f-16.act02 ( -- )   \ 00420E80
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    4 self-through-exit
    self-wait-done
    $609 self-anim
    $14 wait-frames
    fiona-who unlock-sound 6 char-sound
    $11 message-param-room
    $11 item-use
    $63 door-unlock
    20 self-idle-anim
    $14 wait-frames
    $8019 say   \ "Used ."
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-16.act03 ( -- )   \ 00420EC0
    self-wait-done
    65.56 19.77 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $95 message-param-room
        $95 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            om1f-16-refined-magnesia story-flag-set
            0 effect-remove
            $95 1 item-give-count
            0 $95 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-1f-16.act04 ( -- )   \ 00420F20
    self-wait-done
    $19F -6.04 30.79 166 $FFFF 5 self-move-to
    self-wait-done
    1 self-scripted
    1 self-noclip
    yield
    world-frozen state-flag-set
    fiona-who $19E -4.65 24.47 166 char-to-xz
    1 35.0 55.0 0.0 0.0 event-camera
    2 say   \ "A large amount of water is pooled below. I wonder if this room is used as a reservoir o..."
    16 wait-frames
    0 self-scripted
    0 self-noclip
    yield
    world-frozen state-flag-clear
    fiona-who $19F -6.04 30.79 166 char-to-xz
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: old-mansion-1f-16.act05 ( -- )   \ 00420F90
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 self-through-exit
    self-wait-done
    $F $54 fade
    $FF 1.0 0 bgm
    2 1 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    0 creatures-clear
    $5A $FF movie-param
    0 part-hidden
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    $62 door-unlock
    1 exit-check
    self-idle-or-end
;

: old-mansion-1f-16.act06 ( -- )   \ 00421040
    self-wait-done
    2 1 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    $5A $FF movie-param
    0 part-hidden
    0 old-mansion-1f-16.cmd00
    panic-eased
    $FFFF message-prepare
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
old-mansion-1f-16 register-scripts   \ (its phases and actions, by their names)
