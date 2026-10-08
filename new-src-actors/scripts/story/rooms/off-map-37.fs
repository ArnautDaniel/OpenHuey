\ story/rooms/off-map-37.fs - the event scripts of room off-map-37 ($37; (on no map page)).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.off-map-37
USING: room-names story.words story.shared flag-names ;

\ Room37_Cmd00
: off-map-37.cmd00 ( -- )  s" off-map-37.cmd00" stub-step ;

defer off-map-37.act00
: off-map-37.enter ( -- )   \ 00445BC0
    room-sounds
    hewie-commandable state-flag-set
    hewie-no-attack state-flag-set
    1 resident-flag? 2 resident-flag? and 3 resident-flag? and 4 resident-flag? and if
        $15 resident-flag-set
    then
    1 resident-flag? 2 resident-flag? or if
        $55 resident-flag-set
    then
    $57 resident-flag? $58 resident-flag? or if
        $59 resident-flag-set
    then
    $10C door-lock
    $10D door-lock
    1.0 sound-volume-scale
    0 $F1 $D action
    1.0 sound-volume-scale
    0 1 $14 door-bits
;

: off-map-37.char-enter ( -- )   \ 00445C20
    0 self-is? if
        0 exit-taken? if
            $32 1.0 0 bgm
            world-held state-flag-set
            $2B 0 pvar-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $A action-force
        then
        $80 exit-taken? if
            world-held state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $B action-force
        then
        $81 exit-taken? if
            $32 1.0 0 bgm
            world-held state-flag-set
            $FF panic-stage? if
                3 panic-stage
            then
            0 0 $C action-force
        then
    then
;

: off-map-37.phase1 ( -- )   \ 00445C70
    $17 0 0 1 chars-area-camera
    $18 1 1 1 chars-area-camera
    $19 0 0 1 chars-area-camera
    $1A 2 2 1 chars-area-camera
;

: off-map-37.phase2 ( -- )   \ 00445C90
    0 $C char-in-area? 0 45 $3C char-heading? and if
        0 char-busy? not if
            3 message
        else
            3 message-close
        then
        5 7 7 scene-change
    else
        3 message-close
    then
    0 6 char-in-area? 0 -45 $3C char-heading? and if
        0 char-busy? not if
            $14 message
        else
            $14 message-close
        then
        5 6 7 scene-change
    else
        $14 message-close
    then
    0 $15 char-in-area? 0 90 $32 char-heading? and if
        0 char-busy? not if
            $40 message
        else
            $40 message-close
        then
        5 2 7 scene-change
    else
        $40 message-close
    then
    0 4 char-in-area? 0 90 $3C char-heading? and if
        0 char-busy? not if
            1 message
        else
            1 message-close
        then
        5 5 7 scene-change
    else
        1 message-close
    then
    0 $1B $2D char-faces-area? if
        0 char-busy? not if
            0 message
        else
            0 message-close
        then
        5 0 7 scene-change
    else
        0 message-close
    then
    0 0 char-group-bit4? if
        0 char-busy? not if
            $15 message
        else
            $15 message-close
        then
        5 1 7 scene-change
    else
        $15 message-close
    then
    0 1 char-group-bit4? if
        0 char-busy? not if
            $16 message
        else
            $16 message-close
        then
        5 8 7 scene-change
    else
        $16 message-close
    then
    0 5 char-in-area? 0 90 $3C char-heading? and if
        0 char-busy? not if
            2 message
        else
            2 message-close
        then
        5 4 7 scene-change
    else
        2 message-close
    then
;

: off-map-37.phase3 ( -- )   \ 00445D70
    15.0 8.0 -47.0 0.0 8.0 -47.0 15.0 0.0 -47.0 0.0 0.0 -47.0 lights-doorway
    -22.0 6.0 59.0 -20.0 6.0 32.0 -22.0 -8.0 59.0 -20.0 -8.0 32.0 lights-doorway
;

: off-map-37.act03 ( -- )   \ 004461B0
    $2A 0 pvar? if
        $21 room-preload
    else $2A 1 pvar? if
        $21 room-preload
    else $2A 2 pvar? if
        $20 room-preload
    else $2A 3 pvar? if
        $23 room-preload
    else $2A 4 pvar? if
        $24 room-preload
    else $2A 5 pvar? if
        $24 room-preload
    else $2A 6 pvar? if
        9 room-preload
    else $2A 7 pvar? if
        $25 room-preload
    else $2A 8 pvar? if
        9 room-preload
    else $2A 9 pvar? if
        9 room-preload
    else $2A $A pvar? if
        $12 room-preload
    else $2A $B pvar? if
        $F room-preload
    else $2A $C pvar? if
        $31 room-preload
    else $2A $D pvar? if
        $2A room-preload
    else $2A $E pvar? if
        $13 room-preload
    else $2A $F pvar? if
        $21 room-preload
    else $2A $10 pvar? if
        $21 room-preload
    else $2A $11 pvar? if
        0 room-preload
    else $2A $12 pvar? if
        0 room-preload
    else $2A $13 pvar? if
        $12 room-preload
    else $2A $14 pvar? if
        $F room-preload
    else $2A $15 pvar? if
        $15 room-preload
    else $2A $16 pvar? if
        $18 room-preload
    else $2A $17 pvar? if
        $19 room-preload
    else $2A $18 pvar? if
        $19 room-preload
    else $2A $19 pvar? if
        $19 room-preload
    else $2A $1A pvar? if
        8 room-preload
    else $2A $1B pvar? if
        8 room-preload
    else $2A $1C pvar? if
        $1C room-preload
    else $2A $1D pvar? if
        $29 room-preload
    else $2A $1E pvar? if
        $1E room-preload
    else $2A $1F pvar? if
        2 room-preload
    else $2A $20 pvar? if
        3 room-preload
    else $2A $21 pvar? if
        $17 room-preload
    else $2A $22 pvar? if
        $23 room-preload
    else $2A $23 pvar? if
        4 room-preload
    else $2A $24 pvar? if
        $A room-preload
    else $2A $25 pvar? if
        $C room-preload
    else $2A $26 pvar? if
        $C room-preload
    else $2A $27 pvar? if
        $C room-preload
    else $2A $28 pvar? if
        $C room-preload
    else $2A $29 pvar? if
        $C room-preload
    else $2A $2B pvar? if
        6 room-preload
    else $2A $2C pvar? if
        $11 room-preload
    else $2A $2D pvar? if
        $32 room-preload
    else $2A $2A pvar? if
        $29 room-preload
    else $2A $31 pvar? if
        $49 room-preload
    else $2A $2E pvar? if
        $2E room-preload
    else $2A $2F pvar? if
        $55 room-preload
    else $2A $30 pvar? if
        $5D room-preload
    else $2A $32 pvar? if
        $4F room-preload
    else $2A $33 pvar? if
        $4F room-preload
    else $2A $34 pvar? if
        $51 room-preload
    else $2A $35 pvar? if
        $51 room-preload
    else $2A $36 pvar? if
        $51 room-preload
    else $2A $37 pvar? if
        $51 room-preload
    else $2A $38 pvar? if
        $50 room-preload
    else $2A $39 pvar? if
        $4C room-preload
    else $2A $3A pvar? if
        $4C room-preload
    else $2A $3B pvar? if
        $4C room-preload
    else $2A $45 pvar? if
        $63 room-preload
    else $2A $46 pvar? if
        $63 room-preload
    else $2A $3C pvar? if
        $48 room-preload
    else $2A $3D pvar? if
        $52 room-preload
    else $2A $3E pvar? if
        $48 room-preload
    else $2A $3F pvar? if
        $48 room-preload
    else $2A $40 pvar? if
        $66 room-preload
    else $2A $42 pvar? if
        $53 room-preload
    else $2A $43 pvar? if
        $59 room-preload
    else $2A $44 pvar? if
        $59 room-preload
    else $2A $47 pvar? if
        $60 room-preload
    else $2A $48 pvar? if
        $60 room-preload
    else $2A $49 pvar? if
        $47 room-preload
    else $2A $4A pvar? if
        $59 room-preload
    else $2A $4B pvar? if
        $59 room-preload
    else $2A $4C pvar? if
        $4A room-preload
    else $2A $4D pvar? if
        $40 room-preload
    else $2A $4E pvar? if
        $40 room-preload
    else $2A $4F pvar? if
        $106 room-preload
    else $2A $50 pvar? if
        $108 room-preload
    else $2A $51 pvar? if
        $10B room-preload
    else $2A $52 pvar? if
        $59 room-preload
    else $2A $53 pvar? if
        $C0 room-preload
    else $2A $54 pvar? if
        $C0 room-preload
    else $2A $55 pvar? if
        $C0 room-preload
    else $2A $56 pvar? if
        $C5 room-preload
    else $2A $57 pvar? if
        $C7 room-preload
    else $2A $58 pvar? if
        $C7 room-preload
    else $2A $59 pvar? if
        $C7 room-preload
    else $2A $5A pvar? if
        $C7 room-preload
    else $2A $5B pvar? if
        $83 room-preload
    else $2A $5C pvar? if
        $81 room-preload
    else $2A $5D pvar? if
        $8A room-preload
    else $2A $5E pvar? if
        $8C room-preload
    else $2A $5F pvar? if
        $8C room-preload
    else $2A $60 pvar? if
        $8C room-preload
    else $2A $61 pvar? if
        $8E room-preload
    else $2A $62 pvar? if
        $85 room-preload
    else $2A $63 pvar? if
        $84 room-preload
    else $2A $64 pvar? if
        $8F room-preload
    else $2A $65 pvar? if
        $92 room-preload
    else $2A $66 pvar? if
        $92 room-preload
    else $2A $67 pvar? if
        $92 room-preload
    else $2A $68 pvar? if
        $8F room-preload
    else $2A $69 pvar? if
        $8D room-preload
    else $2A $6A pvar? if
        $35 room-preload
    else $2A $6B pvar? if
        $35 room-preload
    else $2A $6C pvar? if
        $35 room-preload
    else $2A $6D pvar? if
        $34 room-preload
    then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then then
    exit
;

:noname   \ off-map-37.act00 (00445DE0; deferred: used before it is defined)
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    0 camera-mode? if
        $115 -34.0 27.0 0 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 10.0 -20.0 0.0 event-camera
    else
        $11B -34.0 60.0 180 $FFFF 5 self-move-to
        self-wait-done
        1 40.0 10.0 160.0 0.0 event-camera
    then
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    8 subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    $2A $FF pvar? not if
        $2A $41 pvar? if
            1 1 char-silent
            0 5 movie-play
            yield
            yield
            2 cutscene-control
            1 result? if
                1.0 movie-volume
                yield
                world-held state-flag-set
                begin
                    0 cutscene-control
                    -1 result? not if
                        6 2 pad? not if
                            yield
                            false
                        else
                            true
                        then
                    else
                        true
                    then
                until
                1 cutscene-control
                begin
                    movie-playing? while
                    yield
                repeat
                yield
                world-held state-flag-clear
            then
            world-held state-flag-set
            0 $11B -34.0 60.0 180 char-to-xz
            hewie-controlled? not if
                0 2 2 char-camera
                0 camera-follow
            else
                1 2 2 char-camera
                1 camera-follow
            then
            $37 0 267 hewie-to-room
            1 $10B 0 char-to-tri-facing
            1 1 1 char-camera
            yield
            camera-restart
            1 0 char-silent
            ['] off-map-37.act00 goto
        else
            off-map-37.act03
            $2C 0 pvar-set
            $80 exit-check
            world-held state-flag-set
            summoner-on state-flag-clear
            events-held state-flag-set
        then
    else
        $2B 0 pvar-set
        $32 1.0 0 bgm
        $F $41 fade
    then
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
; is off-map-37.act00

: off-map-37.act01 ( -- )   \ 00445EF0
    $2E $FF pvar-set
    self-wait-done
    $FF 1.0 0 bgm
    $A subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    $2E 0 pvar? if
        $D9 room-preload
        7 partner-load
        $1B story-flag-set
        $41 story-flag-clear
        $6F story-flag-clear
        $82 story-flag-clear
        $88 story-flag-clear
        $89 story-flag-clear
        $94 story-flag-clear
        $95 story-flag-clear
    else $2E 1 pvar? if
        $E0 room-preload
        $22 partner-load
        $1B story-flag-set
        $41 story-flag-set
        $6F story-flag-set
        $82 story-flag-clear
        $88 story-flag-clear
        $89 story-flag-clear
        $94 story-flag-clear
        $95 story-flag-clear
    else $2E 2 pvar? if
        $D9 room-preload
        $C partner-load
        $1B story-flag-set
        $41 story-flag-set
        $6F story-flag-set
        $82 story-flag-set
        $88 story-flag-set
        $89 story-flag-set
        $94 story-flag-set
        $95 story-flag-set
    else $2E 3 pvar? if
        $E0 room-preload
        $17 partner-load
        $1B story-flag-set
        $41 story-flag-set
        $6F story-flag-set
        $82 story-flag-set
        $88 story-flag-set
        $89 story-flag-set
        $94 story-flag-clear
        $95 story-flag-clear
    then then then then
    $F 1 fade
    wait-fade
    $2E $FF pvar? not if
        $10C door-unlock
        0 3 self-move-slot
        self-wait-done
        0 1 9 action
        $76 -16.0 -82.0 180 $FFFF 5 self-move-to
        self-wait-done
        2 char-unload
        $FF 1.0 0 bgm
        $F 0 fade
        wait-fade
        fiona-calm-reset
        0 panic-stage
        1 action-end
        1 char-done
        world-held state-flag-set
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
        1 action-end
        1 char-done
        hewie-no-attack state-flag-clear
        stalkers-blind state-flag-clear
        $80 exit-check
    else
        $32 1.0 0 bgm
    then
    self-idle-or-end
;

: off-map-37.act02 ( -- )   \ 00446070
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    7 subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    0 0.0 0.0 0.0 0.0 event-camera
    $26 $28 pvars-equal? not $27 $29 pvars-equal? not or if
        0 $10 6 char-sound
    then
    $26 $28 pvars-equal? not if
        $26 1 pvar? if
            new-game-sounds state-flag-set
            1 sound-set
            1 fiona-costume
            $26 1 pvar-set
        else $26 0 pvar? if
            new-game-sounds state-flag-clear
            0 sound-set
            0 fiona-costume
            $26 0 pvar-set
        else $26 2 pvar? if
            new-game-sounds state-flag-set
            1 sound-set
            2 fiona-costume
            $26 2 pvar-set
        else $26 3 pvar? if
            new-game-sounds state-flag-set
            1 sound-set
            3 fiona-costume
            $26 3 pvar-set
        else $26 6 pvar? if
            new-game-sounds state-flag-clear
            0 sound-set
            6 fiona-costume
            $26 6 pvar-set
        else $26 7 pvar? if
            new-game-sounds state-flag-clear
            0 sound-set
            7 fiona-costume
            $26 7 pvar-set
        else $26 8 pvar? if
            new-game-sounds state-flag-set
            1 sound-set
            8 fiona-costume
            $26 8 pvar-set
        then then then then then then then
        effects-arena-flip
    then
    $27 $29 pvars-equal? not if
        $27 0 pvar? if
            0 hewie-model
            $27 0 pvar-set
        else $27 1 pvar? if
            1 hewie-model
            $27 1 pvar-set
        else $27 2 pvar? if
            2 hewie-model
            $27 2 pvar-set
        then then then
        effects-arena-flip
    then
    $26 $28 pvars-equal? not if
        0 char-in
        begin
            yield
            4 sound-bank-loaded? until
        -1 self-move-16
        self-frames-reset
        7 self-wait-frames
    then
    $27 $29 pvars-equal? not if
        1 char-in
        1 char-here? if
            1 2 char-C4? if
                $1002 hewie-anim-set
            else
                1 hewie-anim-set
                $2100 hewie-anim-set
                $1F00 hewie-anim-set
                $2000 hewie-anim-set
            then
        then
    then
    $32 1.0 0 bgm
    $F $41 fade
    wait-fade
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: off-map-37.act04 ( -- )   \ 00446520
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    180 self-turn-angle
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $B subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: off-map-37.act05 ( -- )   \ 00446560
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $121 0.0 10.5 180 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    $FF 1.0 0 bgm
    self-frames-reset
    $1E self-wait-frames
    $D subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    self-frames-reset
    8 self-wait-frames
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: off-map-37.act06 ( -- )   \ 004465C0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    9 -41.0 -38.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $E subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    self-frames-reset
    8 self-wait-frames
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: off-map-37.act07 ( -- )   \ 00446620
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $D7 45.0 -39.0 90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 10.0 0.0 0.0 event-camera
    self-frames-reset
    $1E self-wait-frames
    $FF 1.0 0 bgm
    $F subscreen-open
    begin
        subscreen-wanted state-flag? while
        yield
    repeat
    $32 1.0 0 bgm
    self-frames-reset
    8 self-wait-frames
    0 self-scripted
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: off-map-37.act08 ( -- )   \ 00446680
    self-wait-done
    $10D door-unlock
    1 3 self-move-slot
    self-wait-done
    $F 0 fade
    $FF 1.0 0 bgm
    $7C 17.142 22.877 90 $FFFF 5 self-move-to
    self-wait-done
    wait-fade
    world-held state-flag-set
    begin
        1 adx? not while
        yield
    repeat
    self-frames-reset
    $1E self-wait-frames
    begin
        quit-wanted state-flag-set
        yield
    again
;

: off-map-37.act09 ( -- )   \ 004466C0
    1 char-full-health
    1 0 char-set-C4
    self-wait-done
    $76 -16.0 -82.0 180 $FFFF $A self-move-to
    self-wait-done
    begin
        yield
    again
;

: off-map-37.act0A ( -- )   \ 004466E0
    self-wait-done
    $2A 0 pvar-set
    $2B 0 pvar-set
    0 $15 0 char-to-tri-facing
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    $37 0 267 hewie-to-room
    1 $10B 0 char-to-tri-facing
    1 1 1 char-camera
    yield
    camera-restart
    in-play state-flag-clear
    $F 1 fade
    wait-fade
    $10 resident-flag? not if
        $F 6 fade
        wait-fade
        $40AC message
        wait-message
        $F 7 fade
        wait-fade
        $10 resident-flag-set
        $AC message-param-room
        $AC 1 item-give-count
        $83 $85 0.0 0.0 0.0 0 0 sound
        $801B message
        self-frames-reset
        $1E self-wait-frames
        wait-message
    else
        $AC 1 item-give-count
    then
    $11 resident-flag? not if
        $56 resident-flag? 1 resident-flag? and if
            $25A item-give
            $11 resident-flag-set
        then
    else
        $25A item-add
    then
    $12 resident-flag? not if
        8 resident-flag? $C resident-flag? or $56 resident-flag? and 1 resident-flag? and 3 resident-flag? and $59 resident-flag? and if
            $25B item-give
            $12 resident-flag-set
        then
    else
        $25B item-add
    then
    $13 resident-flag? not if
        1 resident-flag? if
            $25C item-give
            $13 resident-flag-set
        then
    else
        $25C item-add
    then
    $14 resident-flag? not if
        $15 resident-flag? if
            $25D item-give
            $14 resident-flag-set
        then
    else
        $25D item-add
    then
    $5A resident-flag? not if
        8 resident-flag? $C resident-flag? or $56 resident-flag? and 1 resident-flag? and 3 resident-flag? and if
            $25E item-give
            $5A resident-flag-set
        then
    else
        $25E item-add
    then
    $5B resident-flag? not if
        5 resident-flag? 6 resident-flag? and 7 resident-flag? and 8 resident-flag? and 9 resident-flag? and $A resident-flag? and $B resident-flag? and $C resident-flag? and $D resident-flag? and $E resident-flag? and $F resident-flag? and if
            $25F item-give
            $5B resident-flag-set
        then
    else
        $25F item-add
    then
    $5C resident-flag? not if
        $52 resident-flag? $53 resident-flag? and if
            $260 item-give
            $5C resident-flag-set
        then
    else
        $260 item-add
    then
    $5D resident-flag? not if
        $11 resident-flag? $12 resident-flag? and $13 resident-flag? and $14 resident-flag? and $5A resident-flag? and $5B resident-flag? and $5C resident-flag? and if
            $261 item-give
            $5D resident-flag-set
        then
    else
        $261 item-add
    then
    self-idle-or-end
;

: off-map-37.act0B ( -- )   \ 00446880
    self-wait-done
    0 $11B -34.0 60.0 180 char-to-xz
    hewie-controlled? not if
        0 2 2 char-camera
        0 camera-follow
    else
        1 2 2 char-camera
        1 camera-follow
    then
    $37 0 267 hewie-to-room
    1 $10B 0 char-to-tri-facing
    1 1 1 char-camera
    yield
    camera-restart
    ['] off-map-37.act00 goto
;

: off-map-37.act0C ( -- )   \ 004468C0
    4 0 char-remove
    5 0 char-remove
    self-wait-done
    0 panic-stage
    fiona-calm-reset
    0 $13 -16.0 -66.5 0 char-to-xz
    0 0 self-anim-blend
    hewie-controlled? not if
        0 1 1 char-camera
        0 camera-follow
    else
        1 1 1 char-camera
        1 camera-follow
    then
    music-stage-end
    1 $C4 -10.03 -61.53 0 char-to-xz
    1 1 1 char-camera
    yield
    camera-restart
    $F $51 fade
    wait-fade
    $5C resident-flag? not if
        $52 resident-flag? $53 resident-flag? and if
            $260 item-give
            $5C resident-flag-set
        then
    else
        $260 item-add
    then
    $5D resident-flag? not if
        $11 resident-flag? $12 resident-flag? and $13 resident-flag? and $14 resident-flag? and $5A resident-flag? and $5B resident-flag? and $5C resident-flag? and if
            $261 item-give
            $5D resident-flag-set
        then
    else
        $261 item-add
    then
    self-idle-or-end
;

: off-map-37.act0D ( -- )   \ 0047B020
    begin
        off-map-37.cmd00
        yield
    again
;

\ ---- registered ----
' off-map-37.enter off-map-37 0 room-script!
' off-map-37.char-enter off-map-37 6 room-script!
' off-map-37.phase1 off-map-37 1 room-script!
' off-map-37.phase2 off-map-37 2 room-script!
' off-map-37.phase3 off-map-37 3 room-script!
' off-map-37.act00 off-map-37 $00 action-script!
' off-map-37.act01 off-map-37 $01 action-script!
' off-map-37.act02 off-map-37 $02 action-script!
' off-map-37.act03 off-map-37 $03 action-script!
' off-map-37.act04 off-map-37 $04 action-script!
' off-map-37.act05 off-map-37 $05 action-script!
' off-map-37.act06 off-map-37 $06 action-script!
' off-map-37.act07 off-map-37 $07 action-script!
' off-map-37.act08 off-map-37 $08 action-script!
' off-map-37.act09 off-map-37 $09 action-script!
' off-map-37.act0A off-map-37 $0A action-script!
' off-map-37.act0B off-map-37 $0B action-script!
' off-map-37.act0C off-map-37 $0C action-script!
' off-map-37.act0D off-map-37 $0D action-script!
