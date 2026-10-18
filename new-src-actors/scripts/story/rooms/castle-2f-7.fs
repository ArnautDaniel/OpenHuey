\ story/rooms/castle-2f-7.fs - the event scripts of room castle-2f-7 ($23; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-7
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant plate-press-remarked        \ (bit 0) its first remark made: "It's a plate pressing machine."
0 constant looked-var                  \ (var 0) she has looked at the cage (her remarks in turn)
1 constant cue-sound                   \ (snd 1) at the cutscene's cue $69

\ room 0x23 (Room23_Cmd00_ptmf): Fiona's model +0x9A0 / +0x9A8: 0 (byte 3 1) or 0.12 / 0.2
: castle-2f-7.cmd00 ( b0 -- )  drop ;   \ (Daniella's hair force (her model's springs: not simulated here yet))
\ room 0x23 (Room23_Cmd01_ptmf): byte 3 0 a progress name, 1 wait for character 3 (2 while not),
\ else done
: castle-2f-7.cmd01 ( b0 -- )   \ (Daniella's face: 0 loaded, 1 shown, else done)
    case  0 of  s" O_DNL\DNL_201.TEX" speech-load  endof  1 of  3 0 speak  endof  >r speech-done r>  endcase ;
\ room 0x23 (Room23_Cond00_ptmf): none of the six slots' PursuerGroup_Fields bits 0..3, and the
\ stalker is about but not active, in mode 2, 6 or 7
: castle-2f-7.cond00? ( -- flag )   \ (PursuerGroup_Fields bits 0..3 of exit 0 for all: here, nobody in its area)
    0 0 exit-area char-in-area  1 0 exit-area char-in-area or if  false exit  then
    stalker-there? stalker-told? 0= and debilitas? and ;

: castle-2f-7.enter ( -- )   \ 004010A0
    room-sounds
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 wanderer
    6 1 object-hidden
;

: castle-2f-7.char-enter ( -- )   \ 004010E0
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 -1 player-camera
    then
    0 0 -1 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            0 fiona-who 6 action
            0 part-shown
            1 part-hidden
        else
            0 part-hidden
            1 part-shown
        then
    then
;

: castle-2f-7.phase1 ( -- )   \ 00401140
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 -1 1 chars-area-camera
    0 -35.84 0.0 32.47 $12 16 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 16.0 hewie-look-zone
            then
        then
    then
    1 36.32 0.0 24.37 $F 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 1 9 char-zone-bits? if
                hewie-zone-var 1 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
;

: castle-2f-7.phase2 ( -- )   \ 004011B0
    fiona-who 3 $2D char-faces-area? if
        c2f-7-flag1 story-flag? not if
            0 unhurried offer-action
        else
            3 unhurried offer-action
        then
    then
    fiona-who 4 $2D char-faces-area? if
        1 unhurried offer-action
    then
    fiona-who 5 $2D char-faces-area? if
        2 unhurried offer-action
    then
;

: castle-2f-7.act00 ( -- )   \ 004011E0
    stalkers-stay state-flag-set
    1 self-scripted
    $F 4 fade
    self-wait-done
    2 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    panic-eased
    6 message-prepare   \ "It's not a computer, that's for sure... It has alphabetic keys, though."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    c2f-7-flag1 story-flag-set
    plate-press-remarked ebit-set
    2 subscreen-open
    [: subscreen-wanted state-flag? ;] wait-while
    yield
    plate-made story-flag? if
        3 0 movie-play
        1 cutscene-start
        yield
        yield
        scene-wait-ready
        0 cutscene-track 4 action
        panic-eased
        $FFFF message-prepare
        1 result? if
            scene-play
        then
        world-held state-flag-set
        6 1 object-hidden
    then
    fiona-who $7A 31.61 18.93 33 char-to-xz
    0 self-idle-anim
    camera-restart
    $F 1 fade
    wait-fade
    plate-made story-flag? if
        $3F message-param-room
        0 $3F item-tab
        item-fanfare
        $8012 say   \ "You've obtained ."
        [: cutscene-track char-busy? ;] wait-while
        $225 item-give
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-7.act01 ( -- )   \ 00401350
    self-wait-done
    looked-var 0 var? if
        -36.2 33.3 self-turn-to-xz
        self-wait-done
        look-at-anim self-anim
        $32 wait-frames
        $D say   \ "Is this a birdcage? It looks like the remaining husk of a bird in the cage."
        looked-var 1 var-set
    else
        $E say   \ "The terrible things that people do to animals these days. It's just so upsetting."
    then
    self-idle-or-end
;

: castle-2f-7.act02 ( -- )   \ 0047AB00
    self-wait-done
    $C say   \ "It is completely ruined. What could have happened here?"
    self-idle-or-end
;

: castle-2f-7.act03 ( -- )   \ 00401380
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    c2f-8-flag3 story-flag? c2f-7-flag2 story-flag? not and castle-2f-7.cond00? and 0 exit-door-open? and if
        $11 door-open-clear
        doors-room-in
        0 0 self-door-knock
        5 wait-frames
        fiona-who seized-sound 5 char-sound
        uneasy-2-anim 5 self-anim-blend
        1 $FF 8 rumble
        $10 wait-frames
        c2f-7-flag2 story-flag-set
        $5A threat-raise
        self-wait-anim
    else plates-under-10? if
        plate-press-remarked ebit? not if
            $11 say   \ "It's a plate pressing machine."
            plate-press-remarked ebit-set
        then
        plate-made story-flag? if
            plate-made story-flag-clear
            2 subscreen-open
            [: subscreen-wanted state-flag? ;] wait-while
            yield
            plate-made story-flag? if
                $3F message-param-room
                0 $3F item-tab
                item-fanfare
                $8012 say   \ "You've obtained ."
            then
            plate-made story-flag-set
        else
            2 subscreen-open
            [: subscreen-wanted state-flag? ;] wait-while
            yield
            plate-made story-flag? if
                $F 4 fade
                3 0 movie-play
                1 cutscene-start
                yield
                yield
                scene-wait-ready
                wait-fade
                0 cutscene-track 4 action
                panic-eased
                $FFFF message-prepare
                1 result? if
                    scene-play
                then
                fiona-who $7A 31.61 18.93 33 char-to-xz
                0 self-idle-anim
                camera-restart
                6 1 object-hidden
                $F 1 fade
                wait-fade
                $3F message-param-room
                0 $3F item-tab
                item-fanfare
                $8012 say   \ "You've obtained ."
                [: cutscene-track char-busy? ;] wait-while
                $225 item-give
            then
        then
    else plate-press-remarked ebit? not if
        $F say   \ "There's a plate pressing machine here. There are no remaining plates currently so..."
        plate-press-remarked ebit-set
    else
        $12 say   \ "There are no plates remaining. I'll have to get rid of some plate-keys before I c..."
    then then then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-7.act04 ( -- )   \ 00401500
    [: $69 cutscene-cue-reached? ;] wait-until
    cue-sound 6 35.0 10.0 25.0 0 0 sound
    self-idle-or-end
;

: castle-2f-7.act05 ( -- )   \ 0047AB08
    self-wait-done
    $10 say   \ "Returned a plate to the plate pressing machine."
    self-idle-or-end
;

: castle-2f-7.act06 ( -- )   \ 00401520
    self-wait-done
    fiona-who $7A 31.61 18.93 33 char-to-xz
    fiona-who 1 char-visible
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    0 castle-2f-7.cmd01
    0 castle-2f-7.cmd00
    0 cutscene-track 7 action
    panic-eased
    0 message-prepare   \ "Where is he?! Where is the old man? Where's he hiding? I won't let him touch he..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    2 castle-2f-7.cmd01
    1 castle-2f-7.cmd00
    fiona-who 0 char-visible
    3 0 char-remove
    4 0 char-remove
    $25 resident-flag-set
    $80 exit-check
    self-idle-or-end
;

: castle-2f-7.act07 ( -- )   \ 004015D0
    begin
        3 cutscene-shot? not while
        1 wait-frames
    repeat
    1 castle-2f-7.cmd01
    self-idle-or-end
;

: castle-2f-7.act08 ( -- )   \ 004015E0
    room-sounds
    [: 6 sound-bank-loaded? ;] wait-until
    self-wait-done
    6 1 object-hidden
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 wanderer
    2 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    6 message-prepare   \ "It's not a computer, that's for sure... It has alphabetic keys, though."
    1 result? if
        scene-play
    then
    3 0 movie-play
    1 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 4 action
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

: castle-2f-7.act09 ( -- )   \ 00401720
    self-wait-done
    0 0 -39.0 6.0 31.0 3.0 3.0 0.0 0.0 -60.0 wanderer
    6 1 object-hidden
    self-who 1 char-visible
    0 part-shown
    1 part-hidden
    3 3 $FF char-load
    4 4 $FF char-load
    3 char-start
    4 char-start
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    0 castle-2f-7.cmd01
    0 castle-2f-7.cmd00
    0 cutscene-track 7 action
    panic-eased
    0 message-prepare   \ "Where is he?! Where is the old man? Where's he hiding? I won't let him touch he..."
    1 result? if
        scene-play
    then
    world-held state-flag-set
    2 castle-2f-7.cmd01
    1 castle-2f-7.cmd00
    3 0 char-remove
    4 0 char-remove
    self-who 0 char-visible
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
castle-2f-7 register-scripts   \ (its phases and actions, by their names)
