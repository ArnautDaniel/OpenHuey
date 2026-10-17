\ story/rooms/castle-1f-18.fs - the event scripts of room castle-1f-18 ($1C; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-18
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ room 0x1C (Room1C_Cmd00_ptmf): room effect 0x1C (a depth range) with the cutscene from frame
\ 0x14A: near 1 .. 1 + 1.4 t (at most 67.6), far 48.6 + 4 t (at most 230)
: castle-1f-18.cmd00 ( -- )   \ (the depth range widening with the scene from its frame $14A: near 1 .. 1 + 1.4 t (at most
    \ 67.6), far 48.6 + 4 t (at most 230))
    cutscene-frame $14A - s>f  {: | t :}  fdup 1.4e f* 1e f+ 67.6e fmin  fswap 4e f* 48.6e f+ 230e fmin
    1e frot fdup frot  depth-range ;
\ room 0x1C (D_003FD350): a sound (0xC0000000, bank 6) at the room's effect 1
: castle-1f-18.cmd01 ( -- )   \ (its sound 0, kept on, at room effect 1's place)
    $C0000000 6 0 0  1 fx-place vec@  sound-at ;

: castle-1f-18.act06 ( -- )   \ 003FD070
    0 $FF $E2 0 $D $FF $D8 1 butterflies
    1 $FF $E2 0 $A $FF $D8 0 butterflies
    2 $FF $E1 0 $B $FF $D9 0 butterflies
    3 $FF $E2 0 $C $FF $D7 0 butterflies
    4 $FF $E3 0 $E $FF $DA 0 butterflies
    5 $FF $E3 0 $D $FF $D9 0 butterflies
    6 $FF $E4 0 $C $FF $DA 0 butterflies
    7 $FF $E3 0 $D $FF $DB 0 butterflies
    8 $FF $E5 0 $E $FF $D8 0 butterflies
    9 $FF $E3 0 $F $FF $D8 0 butterflies
    $A $FF $E5 0 $C $FF $D7 0 butterflies
    exit
;

: castle-1f-18.enter ( -- )   \ 003FCB30
    room-sounds
    $26 door-locked? if
        0 0 $33 0 $14 $FF $E3 1 butterflies
        1 0 $33 0 $16 $FF $E2 0 butterflies
        2 0 $33 0 $12 $FF $E2 0 butterflies
        3 0 $33 0 $E $FF $E2 0 butterflies
        4 0 $33 0 $C $FF $E2 0 butterflies
        5 0 $33 0 8 $FF $E2 0 butterflies
        6 0 $33 0 $10 $FF $E6 0 butterflies
        7 0 $33 0 $18 $FF $E2 0 butterflies
        8 0 $32 0 $14 $FF $E2 0 butterflies
        9 0 $30 0 $10 $FF $E2 0 butterflies
        $A 0 $30 0 $C $FF $E6 0 butterflies
        0 keeps-people-off-2 floor-on
        1 keeps-hewie-off floor-on
    else
        0 part-shown
        castle-1f-18.act06
        1 keeps-people-off-2 floor-on
    then
;

: castle-1f-18.char-enter ( -- )   \ 003FCBC0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 0 player-camera
    then
    0 1 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        0 -1 player-camera
    then
    1 0 -1 area-camera
;

: castle-1f-18.phase1 ( -- )   \ 003FCC40
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 -1 1 chars-area-camera
    5 1 0 1 chars-area-camera
    6 1 0 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    flowers-laid story-flag? not if
        0 55.0 4.0 -30.0 $A 5 0 zone
        1 55.0 4.0 -30.0 $14 50 0 zone
        1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        1 ebit-set
                        $64 chance? if
                            hewie-zone-var 1 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
    else
        0 -34.0 0.0 -44.0 $A 5 0 zone
        1 -34.0 0.0 -44.0 $14 50 0 zone
        1 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 1 9 char-zone-bits? if
                        1 ebit-set
                        $1E chance? if
                            hewie-zone-var 1 var-set
                            0 hewie-who $8A action
                        then
                    then
                then
            then
        then
    then
    2 ebit? not if
        6 sound-bank-loaded? if
            flowers-laid story-flag? not if
                $40000000 6 55.0 4.0 -30.0 0 0 sound
            else
                $40000000 6 -34.0 0.0 -44.0 0 0 sound
            then
            2 ebit-set
        then
    else flowers-laid story-flag? not if
        castle-1f-18.cmd01
    else
        $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    then then
;

: castle-1f-18.phase2 ( -- )   \ 003FCD70
    flowers-laid story-flag? not if
        fiona-who 9 $5A char-faces-area? if
            2 unhurried offer-action
        then
    then
    fiona-who 0 char-action? in-panic? not and if
        fiona-who 0 2 char-zone-bits? fiona-who 0 2 char-zone-bits-before? not and if
            0 fiona-who 1 action
        then
    then
    fiona-who 7 char-in-area? fiona-who 90 $32 char-heading? and if
        $85 calm-hewie-up offer-action
    then
    fiona-who 8 char-in-area? fiona-who 45 $32 char-heading? and if
        5 unhurried offer-action
    then
;

: castle-1f-18.act00 ( -- )   \ 003FCDC0
    stalkers-stay state-flag-set
    1 self-scripted
    $F $44 fade
    self-wait-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    0 part-shown
    0 cutscene-track 3 action
    hewie-who char-here? hewie-who 2 char-cond? and if
        0 hewie-who $86 action
    then
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    0 self-idle-anim
    fiona-who $10B -33.329 -29.383 -177 char-to-xz
    camera-restart
    hewie-who 0 char-visible
    hewie-who action-end
    1 item-use
    2 item-use
    $26 door-unlock
    flowers-laid story-flag-set
    castle-1f-18.act06
    0 keeps-people-off-2 floor-off
    1 keeps-hewie-off floor-off
    1 keeps-people-off-2 floor-on
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    100 hewie-trust
    self-idle-or-end
;

: castle-1f-18.act01 ( -- )   \ 003FCEA0
    self-wait-done
    flowers-laid story-flag? not if
        55.0 -30.0 self-turn-to-xz
        self-wait-done
    else
        -34.0 -44.0 self-turn-to-xz
        self-wait-done
    then
    turn-round-anim play-out
    -1 self-idle-anim
    flowers-laid story-flag? not if
        1 say   \ "Yuck! Look at all those moths! I'm not going there. Looks like they are attracted t..."
        $229 item-give
    else
        4 say   \ "Guess I'll have to chalk that up to another strange occurrence here. Moths that are..."
    then
    self-idle-or-end
;

: castle-1f-18.act02 ( -- )   \ 003FCEE0
    self-wait-done
    $EC -32.523 -39.444 -130 $FFFF 5 self-move-to
    self-wait-done
    1 30.0 10.0 0.0 0.0 event-camera
    look-low-anim $A self-anim-blend
    0 say   \ "An empty flower vase."
    self-wait-anim
    16 wait-frames
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-1f-18.act03 ( -- )   \ 003FCF30
    0 0 $A 0 $14 $FF $E3 1 butterflies
    0 $FF $DC 0 $B $FF $D1 0 7 effect-string
    1 0 $34 0 $15 $FF $E0 0 butterflies
    2 0 $33 0 $12 $FF $E1 0 butterflies
    3 0 $34 0 $E $FF $E0 0 butterflies
    4 0 $35 0 $17 $FF $E4 0 butterflies
    5 0 $36 0 $13 $FF $E2 0 butterflies
    6 0 $35 0 $F $FF $E1 0 butterflies
    7 0 $36 0 $10 $FF $E0 0 butterflies
    8 0 $33 0 $D $FF $E2 0 butterflies
    9 0 $34 0 $12 $FF $E1 0 butterflies
    $A 0 $32 0 $D $FF $E4 0 butterflies
    1 $FF $E2 0 $A $FF $D8 0 7 effect-string
    2 $FF $E1 0 $B $FF $D9 0 7 effect-string
    3 $FF $E2 0 $C $FF $D7 0 7 effect-string
    4 $FF $E3 0 $E $FF $DA 0 7 effect-string
    5 $FF $E3 0 $D $FF $D9 0 7 effect-string
    6 $FF $E4 0 $C $FF $DA 0 7 effect-string
    7 $FF $E3 0 $D $FF $DB 0 7 effect-string
    8 $FF $E5 0 $E $FF $D8 0 7 effect-string
    9 $FF $E3 0 $F $FF $D8 0 7 effect-string
    $A $FF $E5 0 $C $FF $D7 0 7 effect-string
    [: $D2 cutscene-cue-reached? ;] wait-until
    $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    begin
        $14A cutscene-cue-reached? not while
        1.0 1.0 1.0 48.6 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        castle-1f-18.cmd00
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: castle-1f-18.act04 ( -- )   \ 0047AA70
    self-idle-or-end
;

: castle-1f-18.act05 ( -- )   \ 003FD050
    self-wait-done
    0 ebit? not if
        2 say   \ "This picture has been ripped out and discarded. I'm no critic, but it doesn't seem..."
        0 ebit-set
    else
        3 say   \ "Who would do such a thing?!"
        0 ebit-clear
    then
    self-idle-or-end
;

: castle-1f-18.act07 ( -- )   \ 003FD0E0
    room-sounds
    self-wait-done
    0 0 $33 0 $14 $FF $E3 1 butterflies
    1 0 $33 0 $16 $FF $E2 0 butterflies
    2 0 $33 0 $12 $FF $E2 0 butterflies
    3 0 $33 0 $E $FF $E2 0 butterflies
    4 0 $33 0 $C $FF $E2 0 butterflies
    5 0 $33 0 8 $FF $E2 0 butterflies
    6 0 $33 0 $10 $FF $E6 0 butterflies
    7 0 $33 0 $18 $FF $E2 0 butterflies
    8 0 $32 0 $14 $FF $E2 0 butterflies
    9 0 $30 0 $10 $FF $E2 0 butterflies
    $A 0 $30 0 $C $FF $E6 0 butterflies
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 part-shown
    0 cutscene-track 8 action
    [: 6 sound-bank-loaded? ;] wait-until
    $40000000 6 55.0 4.0 -30.0 0 0 sound
    castle-1f-18.cmd01
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

: castle-1f-18.act08 ( -- )   \ 003FD1F0
    0 0 $A 0 $14 $FF $E3 1 butterflies
    0 $FF $DC 0 $B $FF $D1 0 7 effect-string
    1 0 $34 0 $15 $FF $E0 0 butterflies
    2 0 $33 0 $12 $FF $E1 0 butterflies
    3 0 $34 0 $E $FF $E0 0 butterflies
    4 0 $35 0 $17 $FF $E4 0 butterflies
    5 0 $36 0 $13 $FF $E2 0 butterflies
    6 0 $35 0 $F $FF $E1 0 butterflies
    7 0 $36 0 $10 $FF $E0 0 butterflies
    8 0 $33 0 $D $FF $E2 0 butterflies
    9 0 $34 0 $12 $FF $E1 0 butterflies
    $A 0 $32 0 $D $FF $E4 0 butterflies
    1 $FF $E2 0 $A $FF $D8 0 7 effect-string
    2 $FF $E1 0 $B $FF $D9 0 7 effect-string
    3 $FF $E2 0 $C $FF $D7 0 7 effect-string
    4 $FF $E3 0 $E $FF $DA 0 7 effect-string
    5 $FF $E3 0 $D $FF $D9 0 7 effect-string
    6 $FF $E4 0 $C $FF $DA 0 7 effect-string
    7 $FF $E3 0 $D $FF $DB 0 7 effect-string
    8 $FF $E5 0 $E $FF $D8 0 7 effect-string
    9 $FF $E3 0 $F $FF $D8 0 7 effect-string
    $A $FF $E5 0 $C $FF $D7 0 7 effect-string
    begin
        $D2 cutscene-cue-reached? not while
        castle-1f-18.cmd01
        yield
    repeat
    $C0000000 6 -34.0 0.0 -44.0 0 0 sound
    yield
    begin
        $14A cutscene-cue-reached? not while
        castle-1f-18.cmd01
        1.0 1.0 1.0 48.6 depth-range
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        castle-1f-18.cmd01
        castle-1f-18.cmd00
        yield
    repeat
    wait-fade
    self-idle-or-end
;

\ ---- registered ----
castle-1f-18 register-scripts   \ (its phases and actions, by their names)
