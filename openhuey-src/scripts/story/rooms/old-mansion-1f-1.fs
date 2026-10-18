\ story/rooms/old-mansion-1f-1.fs - the event scripts of room old-mansion-1f-1 ($2E; Belli Castle: Old Mansion 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-1f-1
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant area8-reacted               \ (bit 0) her reaction on entering area 8 made (once a visit)
1 constant hewie-zone0-noticed         \ (bit 1) Hewie has reacted to zone 0 (once a visit)
2 constant mad-scene-on                \ (bit 2) "She's gone utterly mad..." (act03) under way
5 constant presence-sound              \ (snd 5) his sound while he's here (kept up)

\ a lit quad at x -15.96, z -2 .. 6, height 111 / 91
: old-mansion-1f-1.cmd00 ( b0 -- )  $1B $80  -15.959e 111.0e -2.0e  -15.959e 111.0e 6.0e  -15.959e 91.0e -2.0e  -15.959e 91.0e 6.0e  lit-quad ;   \ (a reflecting glass: b0 1 removed, 2 over the whole picture)
\ the depth range (effect 0x1C) opening out with the cutscene from its frame 1260: 1 / 21 / 40 /
\ 80 on by 0.4 a frame, up to 41 / 61 / 80 / 120
: old-mansion-1f-1.cmd03 ( -- )   \ (opening out with the scene from its frame 1260: 1 / 21 / 40 / 80 on 0.4 a frame, up to
    \ 41 / 61 / 80 / 120)
    cutscene-frame 1260 - s>f 0.4e f*
    fdup 1e f+ 41e fmin  fover 21e f+ 61e fmin  frot fdup 40e f+ 80e fmin  fswap 80e f+ 120e fmin  depth-range ;
\ the pursuer's model's +0x9E8 by byte 3: 0 0.15, 1 0, else 0.05
: old-mansion-1f-1.cmd04 ( b0 -- )  drop ;   \ (the stalker's model's springs (+0x9E8): not simulated here yet)

: old-mansion-1f-1.enter ( -- )   \ 0041D2E0
    room-sounds
    found-file-234 story-flag? not if
        0 old-mansion-1f-1.cmd00
        0 part-shown
        found-file-233 story-flag? if
            force-followed state-flag-set
            stalker-no-fear state-flag-set
            no-stalker-camera state-flag-set
            3 char-activate
            stalker-who 1 char-silent
            $FE $2E 104 2 stalker-to-room
            stalker-who 1 1 char-camera
            0 3 1 action
            0 keeps-hewie-off floor-on
        then
    else
        1 part-shown
        1 water-floor floor-on
    then
;

: old-mansion-1f-1.char-enter ( -- )   \ 0041D330
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
    fiona-who self-is? if
        1 exit-taken? if
            3 map-page
        then
    then
;

: old-mansion-1f-1.phase1 ( -- )   \ 0041D3C0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 1 1 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 8 char-entered-area? if
        3 map-page
    then
    fiona-who 8 char-left-area? if
        2 map-page
    then
    fiona-who $A char-entered-area? if
        2 map-page
    then
    fiona-who $A char-left-area? if
        1 map-page
    then
    c3f-2-flag1 story-flag? found-file-233 story-flag? not and if
        0 stalker-alert? not if
            stalker-who 0 stalker-command
        then
    then
    found-file-233 story-flag? not if
        area8-reacted ebit? not if
            fiona-who 8 char-entered-area? if
                area8-reacted ebit-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 0 action-force
                else
                    1 fiona-who 0 action-force
                then
            then
        then
    else found-file-234 story-flag? not if
        fiona-who 9 char-entered-area? mad-scene-on ebit? not and if
            panic-eased
            0 fiona-who 3 action-force
        then
    then then
    found-file-234 story-flag? not found-file-233 story-flag? and if
        0 -3.91 90.0 3.23 $50 26 0 zone
        hewie-zone0-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        hewie-zone0-noticed ebit-set
                        $64 chance? if
                            hewie-zone-var 0 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
        0 -3.91 90.0 3.23 $50 26 0 zone
        hewie-who char-here? 1 game-mode? and if
            hewie-can-command? if
                hewie-who 0 9 char-zone-bits? if
                    hewie-zone-var 0 var-set
                    hewie-zone-var 15.0 hewie-look-zone
                then
            then
        then
    then
    6 sound-bank-loaded? stalker-who char-here? and if
        stalker-who presence-sound kept-going 6 char-sound
    then
;

: old-mansion-1f-1.phase5 ( -- )   \ 0041D4E0
    force-followed state-flag-clear
    found-file-233 story-flag? found-file-234 story-flag? not and if
        force-followed state-flag-clear
        stalker-no-fear state-flag-clear
        no-stalker-camera state-flag-clear
        stalker-who 0 char-silent
        stalker-who action-end
        stalker-who char-done
    then
;

: old-mansion-1f-1.act00 ( -- )   \ 0041D510
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who char-full-health
    hewie-who 0 char-cond!
    $2E 0 49 hewie-to-room
    0 hewie-who $86 action
    0 cutscene-track 2 action
    3 4 0 char-model-op
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 old-mansion-1f-1.cmd04
    wait-fade
    world-held state-flag-set
    0 self-idle-anim
    fiona-who $DF 1.052 -47.906 -79 char-to-xz
    0 0 player-camera
    camera-restart
    3 char-activate
    $FE $2E 104 2 stalker-to-room
    stalker-who 1 1 char-camera
    0 counter-set
    0 3 1 action
    hewie-who action-end
    hewie-who $31 30 char-to-tri-facing
    found-file-233 story-flag-set
    1 exit-prepare
    0 old-mansion-1f-1.cmd00
    1 wait-counter
    5 0 0 music
    $F $41 fade
    wait-fade
    $233 item-give
    stalkers-stay state-flag-clear
    no-stalker-camera state-flag-set
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-1.act01 ( -- )   \ 0041D620
    3 2 char-file-load
    self-wait-done
    1 self-noclip
    1 self-scripted
    3 $68 -4.538 3.132 -101 char-to-xz
    force-followed state-flag-set
    stalker-no-fear state-flag-set
    3 char-file-use
    [: 6 sound-bank-loaded? ;] wait-until
    stalker-who 0 char-silent
    0 barred-floor floor-on
    1 counter-set
    stalker-who presence-sound 6 char-sound
    begin
        0 room-motion play-out
    again
;

: old-mansion-1f-1.act02 ( -- )   \ 0041D670
    depth-range-off
    [: $10E cutscene-cue-reached? ;] wait-until
    0 old-mansion-1f-1.cmd04
    [: $1E0 cutscene-cue-reached? ;] wait-until
    1 old-mansion-1f-1.cmd04
    [: 5 cutscene-shot? ;] wait-until
    2 old-mansion-1f-1.cmd00
    2 old-mansion-1f-1.cmd04
    1 wait-frames
    1 old-mansion-1f-1.cmd04
    [: $3A2 cutscene-cue-reached? ;] wait-until
    0 old-mansion-1f-1.cmd00
    [: $4B0 cutscene-cue-reached? ;] wait-until
    begin
        $4EC cutscene-cue-reached? not while
        1.0 21.0 40.0 80.0 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        old-mansion-1f-1.cmd03
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: old-mansion-1f-1.act03 ( -- )   \ 0041D6E0
    mad-scene-on ebit-set
    1 self-scripted
    self-wait-done
    0 self-turn-angle
    self-wait-done
    turn-round-anim play-out
    3 self-look-at
    yield
    0 say   \ "She's gone utterly mad. Best to keep my distance..."
    self-who self-look-at
    yield
    mad-scene-on ebit-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-1f-1.act04 ( -- )   \ 0041D700
    self-wait-done
    0 part-shown
    3 partner-load
    2 char-start
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 2 action
    3 4 0 char-model-op
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    1 old-mansion-1f-1.cmd04
    2 0 char-remove
    0 old-mansion-1f-1.cmd00
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: old-mansion-1f-1.phase2 ( -- )   \ 0047ACC4
;

\ ---- registered ----
old-mansion-1f-1 register-scripts   \ (its phases and actions, by their names)
