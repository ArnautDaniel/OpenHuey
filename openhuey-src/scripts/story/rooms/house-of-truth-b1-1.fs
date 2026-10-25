\ story/rooms/house-of-truth-b1-1.fs - the event scripts of room house-of-truth-b1-1 ($84; House of Truth: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-b1-1
USING: room-names facts story.words story.shared flag-names story-names floor-flags ;

\ room 0x84: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
\ shake of 0.5 (slam_shake; a frame hook).
: house-of-truth-b1-1.cmd00 ( -- )  slam-shake ;

: house-of-truth-b1-1.enter ( -- )   \ 00432420
    0 part-shown
    0 1 object-hidden
    1 1 object-hidden
    2 1 object-hidden
;

: house-of-truth-b1-1.char-enter ( -- )   \ 00432440
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $2A 0 char-to-tri-facing
            world-held state-flag-set
            0.0 sound-volume-scale
            panic-eased
            0 fiona-who 0 action-force
        then
    then
;

: house-of-truth-b1-1.phase1 ( -- )   \ 004324A0
    0 exit-usable? if
        0 exit-check
    then
    house-of-truth-b1-1.cmd00
;

: house-of-truth-b1-1.phase2 ( -- )   \ 0047AE18
;

: house-of-truth-b1-1.act00 ( -- )   \ 004324B0
    stalkers-stay state-flag-set
    1 self-scripted
    stalker-who action-end
    stalker-who char-done
    self-wait-done
    9 3 $FF char-load
    3 char-start
    0 part-hidden
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    0 message-prepare   \ "Daddy? Ah, Fiona... My dear holder of Azoth! What's going on? You're not my......"
    1 result? if
        scene-play
    then
    3 0 char-remove
    stalker-who action-end
    stalker-who char-done
    fiona-who $25 -1.96 -22.63 0 char-to-xz
    0 0 player-camera
    camera-restart
    over-music-e story-flag-set
    1 0 0 music
    $EF door-unlock
    $EF door-open-set
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
    stalker-who char-activate
    $FE $84 44 2 stalker-to-room
    stalker-who $2C -140 char-to-tri-facing
    stalker-who 0 0 char-camera
    summoner-on state-flag-set
    summoner-own-cooldown
    hewie-who action-end
    hewie-who char-done
    hewie-who char-activate
    $85 0 82 hewie-to-room
    0 part-shown
    0 1 object-hidden
    1 1 object-hidden
    2 1 object-hidden
    $F $51 fade
    wait-fade
    $4B resident-flag-set
    $253 item-give
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-b1-1.act01 ( -- )   \ 00432620
    self-wait-done
    9 3 $FF char-load
    $A 4 $FF char-load
    3 char-start
    4 char-start
    4 0 movie-play
    3 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    0 message-prepare   \ "Daddy? Ah, Fiona... My dear holder of Azoth! What's going on? You're not my......"
    1 result? if
        scene-play
    then
    3 0 char-remove
    4 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
house-of-truth-b1-1 register-scripts   \ (its phases and actions, by their names)
