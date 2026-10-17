\ story/rooms/castle-1f-7.fs - the event scripts of room castle-1f-7 ($15; Belli Castle: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-1f-7
USING: engine common room-names story.words story.shared flag-names story-names floor-flags fiona.anims hewie.anims ;

\ a lid (pstr_kousi_2, +0x24 its height, +0x34 its speed): byte 3 0 up, 1 shut; 2 falling and
\ bouncing shut (the first landing clears progress flag 0x50 and, unless the director says no,
\ the lid falling shut: faster by 0.1 a frame; landed, it bounces back a fifth as fast - the first
\ time story flag $310 cleared and (the camera not in its free mode) a thud; settled below 0.02
: lid-y ( F: -- y )  s" kousi" place-y object@ ;
: lid-falls ( -- settled? )
    -0.1e s" kousi" $34 object+!  s" kousi" $34 object@ s" kousi" place-y object+!
    lid-y -25.3e f< if
        s" kousi" $30 object@ f0> if
            -1e s" kousi" $30 object!  c1f-4-flag2 story-flag-clear
            cam-scene? 0= if  2 6 0 0  0e 30e 30e sound  then
        then
        -25.3e s" kousi" place-y object!  s" kousi" $34 object@ -0.2e f* s" kousi" $34 object!
    then
    s" kousi" $34 object@ fabs 0.02e f< dup if  -25.3e s" kousi" place-y object!  then ;
\ a rattle: up 0.4 and down again by turns, a little more each time, till it is above 0
: lid-rattles ( -- done? )
    s" kousi" $34 object@ f0< if  1e 0.4e  else  -1e 0.1e  then
    s" kousi" place-y object+!  s" kousi" $34 object!
    rnd 0.1e f* s" kousi" place-y object+!
    lid-y f0> dup if  0e s" kousi" place-y object!  then ;
\ thuds), 2 while moving; 3 a random rattle up, 2 while it stays below
: castle-1f-7.cmd00 ( b0 -- )   \ (the lid, "kousi": +$24 its height, +$34 its speed, +$30 1 till it first lands)
    s" kousi" object-here? 0= if  drop exit  then
    case
        0 of  0e s" kousi" place-y object!  1e s" kousi" $30 object!  0e s" kousi" $34 object!  endof
        1 of  -25.3e s" kousi" place-y object!  0e s" kousi" $34 object!  endof
        2 of  begin  lid-falls 0= while  yield  repeat  endof
        3 of  begin  lid-rattles 0= while  yield  repeat  endof
    endcase ;
\ room 0x15 (Room15_Cond00_ptmf): Hewie is about in room 0xF in state 0x2F or 0x52
: castle-1f-7.cond00? ( -- flag )
    hewie-who $F char-in-room? 0= if  false exit  then  hewie-act-now dup $2F = swap $52 = or ;

: castle-1f-7.enter ( -- )   \ 003FA090
    room-sounds
    0 0 var-set
    $80 exit-taken? if
        1 castle-1f-7.cmd00
        c1f-4-flag1 story-flag-clear
    else
        stalker-who char-here? stalker-who 0 char-in-nav-group? and if
            stalker-who $F0 char-to-tri
        then
        1 exit-taken? if
        else
            c1f-4-flag2 story-flag-clear
            c1f-4-flag1 story-flag-clear
        then
        c1f-4-flag2 story-flag? not c1f-4-flag1 story-flag? not and if
            0 barred-floor floor-on
            1 castle-1f-7.cmd00
        else
            c1f-4-flag1 story-flag-set
            0 barred-floor floor-off
            0 1 var-set
            0 castle-1f-7.cmd00
        then
    then
    breaker-lowered story-flag? not if
        2 0 object-hidden
        0 part-hidden
        1 part-shown
        1 keeps-hewie-off floor-on
    else
        2 1 object-hidden
        0 part-shown
        1 part-hidden
    then
;

: castle-1f-7.char-enter ( -- )   \ 003FA110
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
        $80 exit-taken? if
            fiona-who $C7 -60 char-to-tri-facing
            1 30.0 15.0 0.0 0.0 event-camera
            0 0 track 7 action
        then
    then
    hewie-who self-is? if
        breaker-lowered story-flag? not if
            c1f-7-flag1 story-flag? not if
                0 hewie-who 8 action
            then
        then
    then
;

: castle-1f-7.phase1 ( -- )   \ 003FA1C0
    0 ebit? not if
        6 sound-bank-loaded? if
            breaker-lowered story-flag? not if
                $40000003 6 -10.0 15.0 0.0 0 0 sound
            then
            0 ebit-set
        then
    else
        $C0000003 6 -10.0 15.0 0.0 0 0 sound
        $C0000001 6 0.0 30.0 30.0 0 0 sound
    then
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 -1 1 chars-area-camera
    6 1 -1 1 chars-area-camera
    7 0 0 1 chars-area-camera
    fiona-who 2 char-left-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    c1f-4-flag1 story-flag? not if
        0 0 var-set
    else castle-1f-7.cond00? not if
        0 0 var-set
    then then
    hewie-who $15 0 room-doors-state
    c1f-4-flag1 story-flag? if
        0 0 var? if
            fiona-who 0 char-in-nav-group? hewie-who 0 char-in-nav-group? or stalker-who 0 char-in-nav-group? or if
                hewie-who $15 1 room-doors-state
            else
                c1f-4-flag1 story-flag-clear
                0 barred-floor floor-on
                0 1 track 3 action
            then
        then
    then
    breaker-lowered story-flag? not if
        fiona-who $11 char-entered-area? 1 ebit? not and if
            1 ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 0 action-force
            else
                1 fiona-who 0 action-force
            then
        then
    then
;

: castle-1f-7.phase2 ( -- )   \ 003FA2C0
    fiona-who 8 char-in-area? fiona-who -45 $2D char-heading? and if
        1 unhurried offer-action
    then
    breaker-lowered story-flag? not c1f-4-flag1 story-flag? not and if
        fiona-who 9 char-in-area? fiona-who -45 $2D char-heading? and if
            4 unhurried offer-action
        then
    then
    fiona-who $A char-in-area? fiona-who -45 $2D char-heading? and if
        5 unhurried offer-action
    then
    fiona-who $B char-in-area? fiona-who 90 $2D char-heading? and if
        5 unhurried offer-action
    then
    fiona-who $C char-in-area? fiona-who $D char-in-area? or fiona-who 45 $2D char-heading? and if
        5 unhurried offer-action
    then
    fiona-who $E char-in-area? fiona-who 0 $2D char-heading? and if
        5 unhurried offer-action
    then
    -2147483646 scene-request? if
        6 always offer-action
    then
;

: castle-1f-7.phase3 ( -- )   \ 003FA330
    33.0 23.0 -5.9 45.0 23.0 -5.9 33.0 11.0 -5.9 45.0 11.0 -5.9 lights-doorway
    33.0 11.0 -5.9 45.0 11.0 -5.9 33.0 0.0 -5.9 45.0 0.0 -5.9 lights-doorway
    45.0 23.0 -5.1 33.0 23.0 -5.1 45.0 0.0 -5.1 33.0 0.0 -5.1 lights-doorway
;

: castle-1f-7.act00 ( -- )   \ 003FA3D0
    capture-no-end state-flag-set
    no-pause state-flag-set
    1 self-scripted
    stalker-who action-end
    stalker-who char-done
    16 wait-frames
    yield
    $F $54 fade
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    $12 $1E movie-param
    hewie-who action-end
    hewie-who char-done
    panic-eased
    $FFFF message-prepare
    1 result? if
        movie-loop
        scene-play-through
    then
    3 6 sound-stop
    1.0 sound-volume-scale
    $1C resident-flag-set
    0 game-over-flag
    caught state-flag-set
    self-idle-or-end
;

: castle-1f-7.act01 ( -- )   \ 003FA450
    self-wait-done
    breaker-lowered story-flag? not if
        fiona-who 3 char-file-load
        $BF -39.071 35.745 -90 $FFFF 5 self-move-to
        self-wait-done
        1 20.0 -10.0 0.0 0.0 event-camera
        0 say   \ "It's a breaker box. Lower the handle? YES NO"
        fiona-who char-file-use
        0 answer? if
            2 0 object-anim
            0 room-motion $A self-anim-blend
            $32 wait-frames
            0 6 -44.0 18.0 36.0 0 0 sound
            3 6 sound-stop
            1 part-hidden
            breaker-lowered story-flag-set
            40 hewie-trust
            stalker-who $15 0 room-doors-state
            1 keeps-hewie-off floor-off
            9 door-unlock
            self-wait-anim
            $8277 item-give
        then
        16 wait-frames
        0 0.0 0.0 0.0 0.0 event-camera
    else
        1 say   \ "The handle has been lowered."
    then
    self-idle-or-end
;

: castle-1f-7.act02 ( -- )   \ 0047AA28
    self-idle-or-end
;

: castle-1f-7.act03 ( -- )   \ 003FA4F0
    c1f-4-flag2 story-flag-set
    $40000001 6 0.0 30.0 30.0 0 0 sound
    2 castle-1f-7.cmd00
    self-idle-or-end
;

: castle-1f-7.act04 ( -- )   \ 0047AA30
    self-wait-done
    5 say   \ "It's closed. I wonder if there isn't a way for me to raise this?"
    self-idle-or-end
;

: castle-1f-7.act05 ( -- )   \ 003FA510
    self-wait-done
    7 0 var? if
        2 say   \ "The doll has been stabbed with small iron sticks... This place is so freakish."
        7 1 var-set
    else 7 1 var? if
        3 say   \ "Is someone just trying to scare me?"
        7 2 var-set
    else
        4 say   \ "I just want to get out of this room as quickly as possible."
        7 0 var-set
    then then
    self-idle-or-end
;

: castle-1f-7.act06 ( -- )   \ 003FA550
    self-wait-done
    9 self-through-door
    self-wait-done
    in-panic? 2 game-mode? or if
        rattle-front-slow-anim play-out
        -1 self-idle-anim
        7 wait-frames
    else
        rattle-front-anim play-out
        -1 self-idle-anim
        7 wait-frames
        6 say   \ "It's locked and won't open. Now, how am I going to get you open?"
    then
    self-idle-or-end
;

: castle-1f-7.act07 ( -- )   \ 003FA580
    world-frozen state-flag-set
    world-held state-flag-clear
    $F 1 fade
    wait-fade
    [: 6 sound-bank-loaded? ;] wait-until
    $F room-preload
    $40000001 6 0.0 30.0 30.0 0 0 sound
    3 castle-1f-7.cmd00
    2 6 0.0 30.0 30.0 0 0 sound
    $1E wait-frames
    $F 0 fade
    wait-fade
    world-held state-flag-set
    0 0.0 0.0 0.0 0.0 event-camera
    3 6 sound-stop
    $80 exit-check
    self-idle-or-end
;

: castle-1f-7.act08 ( -- )   \ 003FA5F0
    self-wait-done
    $F5 -9.214 -23.873 -40 $FFFF $A self-move-to
    self-wait-done
    breaker-lowered story-flag? not if
        stance-anim self-anim
        $3C wait-frames
        c1f-7-flag1 story-flag-set
        breaker-lowered story-flag? not if
            bark-tense-anim play-out
            breaker-lowered story-flag? not if
                bark-tense-anim play-out
                breaker-lowered story-flag? not if
                    bark-tense-anim play-out
                    breaker-lowered story-flag? not if
                        bark-tense-anim play-out
                        breaker-lowered story-flag? not if
                            bark-tense-anim play-out
                            breaker-lowered story-flag? not if
                                bark-tense-anim play-out
                                breaker-lowered story-flag? not if
                                    fiona-who self-look-at
                                    yield
                                    stance-anim self-anim
                                    $28 wait-frames
                                    self-who self-look-at
                                    yield
                                then
                            then
                        then
                    then
                then
            then
        then
    then
    self-idle-or-end
;

: castle-1f-7.act09 ( -- )   \ 003FA660
    self-wait-done
    1 castle-1f-7.cmd00
    2 0 object-hidden
    0 part-hidden
    1 part-shown
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $12 $1E movie-param
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
castle-1f-7 register-scripts   \ (its phases and actions, by their names)
