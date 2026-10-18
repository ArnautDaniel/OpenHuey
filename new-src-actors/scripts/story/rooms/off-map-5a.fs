\ story/rooms/off-map-5a.fs - the event scripts of room off-map-5a ($5A; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
\ (The room has no data on the disc - no ST_05A.PAC: it is never entered, and its messages'
\ texts aren't known; its scripts are kept as the executable has them.)
IN: story.rooms.off-map-5a
USING: room-names story.words story.shared flag-names ;

\ ---- this room's own event bits and script variables ----
0 constant scene-done-bit              \ (bit 0) its scene (act01) played
1 constant first-remarked              \ (bit 1) its first remark made (message 3)
2 constant message-up                  \ (bit 2) the message up (act02's loop)
3 constant effect-two                  \ (bit 3) ? the effect's second step (cmd00 2)
4 constant looked-up                   \ (bit 4) ? she looks up after it (sound $72)
0 constant effect-var                  \ (var 0) ? the effect's frames

\ the three dials (Room5A_ObjectNames, progress vars gSndProgressVars: 0..3, 90 degrees each),
\ event var 0 the one picked: byte 3 0 set (the original sets the picked one three times), 1 up
\ / down picks one (event var 0), left / right turns it (event +0x5C 3), cancel leaves (+0x60
\ 2); 2 turning to it 4 degrees a step, and there: solved at 1 / 0 / 2 (+0x60 2, +0x5C 4), else
\ +0x60 3; 3 wait
: off-map-5a.cmd00 ( b0 -- )  drop s" off-map-5a.cmd00" stub-step ;

: off-map-5a.char-enter ( -- )   \ 00410BE0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        6 -1 player-camera
    then
    0 6 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        3 2 player-camera
    then
    1 3 2 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        3 2 player-camera
    then
    2 3 2 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        2 1 player-camera
    then
    3 2 1 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who 1 char-to-exit
            3 2 player-camera
            0 fiona-who 1 action
            hewie-who $59 char-in-room? if
            then
        then
    then
;

: off-map-5a.phase1 ( -- )   \ 00410D00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    2 exit-usable? if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
    $A 5 4 1 chars-area-camera
    $B 6 -1 1 chars-area-camera
    $C 0 0 1 chars-area-camera
    $D 5 4 1 chars-area-camera
    $E 0 0 1 chars-area-camera
    $F 1 -1 1 chars-area-camera
    $10 1 -1 1 chars-area-camera
    $11 4 3 1 chars-area-camera
    $12 4 3 1 chars-area-camera
    $13 3 2 1 chars-area-camera
    $14 2 1 1 chars-area-camera
    $15 3 2 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? fiona-who 6 char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 7 char-entered-area? fiona-who 8 char-entered-area? or if
        2 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        3 exit-prepare
    then
;

: off-map-5a.phase2 ( -- )   \ 00410D90
    fiona-who $16 $32 char-faces-area? if
        2 unhurried offer-action
    then
    1 exit-door-open? not scene-done-bit ebit? and if
        fiona-who 1 char-before-door? if
            0 examine-message
        then
    then
    fiona-who $17 char-in-area? fiona-who -45 $32 char-heading? and if
        3 unhurried offer-action
    then
;

: off-map-5a.phase5 ( -- )   \ 00410DC0
    scene-done-bit ebit? stalker-who char-here? not and stalker-who 2 char-cond? and if
        stalker-who action-end
        2 summon-take
    then
;

: off-map-5a.act00 ( -- )   \ 00410DD0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $A item-use
    $B message-param-room
    $B 1 item-give-count
    0 $B item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.act01 ( -- )   \ 00410E10
    scene-done-bit ebit-set
    $F $41 fade
    fiona-who $28 5 char-sound
    self-wait-done
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.act02 ( -- )   \ 00410E30
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    1 say
    0 answer? if
        fiona-who $D2 293.136 -95.0 180 char-to-xz
        world-frozen state-flag-set
        1 5.0 0.0 0.0 2.0 event-camera
        2 message
        message-up ebit-set
        effect-var 0 var-set
        begin
            effect-two ebit? not if
                1 off-map-5a.cmd00
            else
                2 off-map-5a.cmd00
            then
            message-up ebit? while
            yield
        repeat
        2 message-close
        world-frozen state-flag-clear
        0 0.0 0.0 0.0 0.0 event-camera
        looked-up ebit? if
            16 wait-frames
            fiona-who $72 5 char-sound
            318.0 10.0 80.0 self-look-at-point
            yield
            $3C wait-frames
            self-who self-look-at
            yield
        then
    then
    \ (never runs in the original: an else outside any block)
    \   7 message
    \   wait-message
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.act03 ( -- )   \ 00410ED0
    self-wait-done
    -115.0 -75.0 self-turn-to-xz
    self-wait-done
    first-remarked ebit? not if
        3 say
        first-remarked ebit-set
    else
        4 say
        first-remarked ebit-clear
    then
    self-idle-or-end
;

: off-map-5a.act04 ( -- )   \ 00410EF0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    -115.0 -75.0 self-turn-to-xz
    self-wait-done
    5 say
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: off-map-5a.enter ( -- )   \ 0047ABE0
    0 off-map-5a.cmd00
;

\ ---- registered ----
off-map-5a register-scripts   \ (its phases and actions, by their names)
