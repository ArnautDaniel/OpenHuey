\ story/rooms/castle-3f-1.fs - the event scripts of room castle-3f-1 ($6; Belli Castle: 3F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-3f-1
USING: room-names facts story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant locked-remarked             \ (bit 0) its first remark made: "The door is locked."
0 constant unlock-sound                \ (snd 0) a door unlocked with its key

\ room 0x06 (Room06_Cmd00_ptmf): its four objects (the name's 6th letter counting) to their
\ places
: castle-3f-1.cmd00 ( -- )   \ (its four objects, "oshi00".."oshi03", to their places)
    -35e -46e -55e s" oshi00" place-to   -25e -46e -55e s" oshi01" place-to
    -25e -46e 55e s" oshi02" place-to    75e -46e 35e s" oshi03" place-to ;

: castle-3f-1.enter ( -- )   \ 003F1B60
    room-sounds
    c2f-9-flag1 story-flag? not if
        1 exit-taken? if
            0 $584 $DF8 obstacle-save-at
            1 $65C $ED0 obstacle-save-at
            2 $76A $FDE obstacle-save-at
            3 $36E $BE2 obstacle-save-at
        else
            0 0 obstacle-model-back
            1 1 obstacle-model-back
            2 2 obstacle-model-back
            3 3 obstacle-model-back
        then
    else
        castle-3f-1.cmd00
    then
    c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        force-followed state-flag-set
        stalker-no-fear state-flag-set
        no-stalker-camera state-flag-set
        stalker-who char-activate
        stalker-who 1 char-silent
        $FE 6 236 2 stalker-to-room
        0 stalker-who 0 char-model-op
        0 stalker-who 2 action
    then
    1 $2300 sound-volume
;

: castle-3f-1.char-enter ( -- )   \ 003F1BD0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        2 -1 player-camera
    then
    0 2 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 -1 player-camera
    then
    1 0 -1 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        1 -1 player-camera
    then
    2 1 -1 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        0 -1 player-camera
    then
    3 0 -1 area-camera
;

: castle-3f-1.phase1 ( -- )   \ 003F1CD0
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
    8 1 -1 1 chars-area-camera
    9 0 -1 1 chars-area-camera
    $E 1 -1 1 chars-area-camera
    $F 2 -1 1 chars-area-camera
    $10 1 -1 1 chars-area-camera
    $11 2 -1 1 chars-area-camera
    fiona-who 4 char-entered-area? fiona-who $C char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? fiona-who $B char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who $A char-entered-area? or if
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? fiona-who $D char-entered-area? or if
        3 exit-prepare
    then
;

: castle-3f-1.phase2 ( -- )   \ 003F1D58
    -2147483646 scene-request? if
        0 always offer-action
    then
;

: castle-3f-1.phase3 ( -- )   \ 003F1D70
    0.0 -2.8 -39.0 -83.0 -2.8 -39.0 0.0 -2.8 -60.0 -83.0 -2.8 -60.0 lights-doorway
    40.0 -2.8 -39.0 0.0 -2.8 -39.0 40.0 -2.8 -60.0 0.0 -2.8 -60.0 lights-doorway
    83.0 -2.8 -39.0 40.0 -2.8 -39.0 83.0 -2.8 -60.0 40.0 -2.8 -60.0 lights-doorway
    0.0 -2.8 39.0 40.0 -2.8 39.0 0.0 -2.8 60.0 40.0 -2.8 60.0 lights-doorway
    40.0 -2.8 39.0 83.0 -2.8 39.0 40.0 -2.8 60.0 83.0 -2.8 60.0 lights-doorway
    -83.0 -2.8 39.0 0.0 -2.8 39.0 -83.0 -2.8 60.0 0.0 -2.8 60.0 lights-doorway
    30.0 -2.0 -57.0 -20.0 -2.0 -57.0 30.0 -2.0 -80.0 -20.0 -2.0 -80.0 lights-doorway
    38.7 19.3 -60.1 20.2 19.3 -60.1 38.7 -4.0 -60.1 20.2 -4.0 -60.1 lights-doorway
;

: castle-3f-1.phase5 ( -- )   \ 003F1F00
    force-followed state-flag-clear
    c1f-14-flag2 story-flag? c3f-2-flag1 story-flag? not and if
        1 stalker-who 0 char-model-op
        stalker-who 0 char-silent
        stalker-who action-end
        stalker-who char-done
        fiona-who 0 char-in-area? fiona-who 1 char-in-area? or if
            stalker-no-fear state-flag-clear
            no-stalker-camera state-flag-clear
        then
    then
;

: castle-3f-1.act00 ( -- )   \ 003F1F30
    self-wait-done
    0 self-through-exit
    self-wait-done
    locked-remarked ebit? not 2 game-mode? or in-panic? or if
        in-panic? 2 game-mode? or if
            rattle-behind-slow-anim play-out
            -1 self-idle-anim
            7 wait-frames
        else
            rattle-behind-anim play-out
            -1 self-idle-anim
            7 wait-frames
            1 say   \ "The door is locked."
        then
        locked-remarked ebit-set
    else
        4 wait-frames
        fiona-who $12E 0.0 -76.5 180 char-to-xz
        world-frozen state-flag-set
        1 self-scripted
        1 20.0 5.0 0.0 4.0 event-camera
        4 wait-frames
        2 say   \ "There is a strange symbol etched on the door. What could it mean?"
        4 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
        fiona-who $12E -1.25 -75.3 180 char-to-xz
        world-frozen state-flag-clear
        0 self-scripted
    then
    $258 item-give
    self-idle-or-end
;

: castle-3f-1.act01 ( -- )   \ 003F1FD0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $34 self-through-door
    self-wait-done
    over-music-b story-flag? not if
        $F $44 fade
        scene-locked state-flag-set
        3 partner-load
        2 char-start
        1 0 movie-play
        0 cutscene-start
        yield
        yield
        scene-wait-ready
        wait-fade
        hewie-who action-end
        hewie-who char-done
        $11 room-preload
        0 part-shown
        panic-eased
        0 message-prepare   \ "Dinner is served, Miss. Dinner is served, Miss."
        1 result? if
            scene-play
        then
        wait-fade
        world-held state-flag-set
        3 action-end
        3 char-done
        $80 exit-check
        0 part-hidden
    else
        $609 self-anim
        $14 wait-frames
        fiona-who unlock-sound 6 char-sound
        6 message-param-room
        6 item-use
        $34 door-unlock
        20 self-idle-anim
        $14 wait-frames
        $8019 say   \ "Used ."
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-3f-1.act02 ( -- )   \ 003F20B0
    1 self-noclip
    1 self-scripted
    stalker-who $129 -4.85 -47.03 180 char-to-xz
    stalker-who -1 -1 char-camera
    self-wait-done
    stalker-who char-file-use
    begin
        1 room-motion play-out
    again
;

: castle-3f-1.act03 ( -- )   \ 003F20D0
    self-wait-done
    3 partner-load
    2 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 part-shown
    panic-eased
    0 message-prepare   \ "Dinner is served, Miss. Dinner is served, Miss."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    3 action-end
    3 char-done
    0 part-hidden
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-3f-1 register-scripts   \ (its phases and actions, by their names)
