\ story/rooms/house-of-truth-1f-4.fs - the event scripts of room house-of-truth-1f-4 ($8C; House of Truth: 1F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.house-of-truth-1f-4
USING: engine common actors messages story.state room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits and script variables ----
0 constant generator-remarked          \ (bit 0) its first remark made: "Looks like a power generator. The switch is turned on, but i"
1 constant generator-on                \ (bit 1) the generator started (act06)
2 constant stalker-reacted             \ (bit 2) her reaction to the stalker in area $B made
3 constant hewie-path-open             \ (bit 3) Hewie's way kept open (until he is off it)
4 constant locked-remarked             \ (bit 4) its first remark made: "The door is locked."
6 constant demolisher-remarked         \ (bit 6) its first remark made: "This looks like a demolisher. These things are used to smash"
7 constant door-effect-pending         \ (bit 7) the door's effect (cmd04) to come as she enters area 6
0 constant generator-var               \ (var 0) ? the generator's state (3: done)
1 constant run-frames-var              \ (var 1) the generator's frames (to 240)
2 constant hewie-hp-var                \ (var 2) Hewie's health kept (cmd03)

\ (as RoomC7_Cmd01)
: house-of-truth-1f-4.cmd00 ( b0 -- )  var-down-by-hit ;
\ the room object named pstr_dynamo swung: byte 3 0 starts it (rest +0x30 from +0x20, phase
\ +0x34 0, amplitude +0x3C 1); 1 steps the phase back 60 degrees and the amplitude down 0.25,
\ height +0x28 = +0x38 + amplitude * sin, waiting (2) until it has died out
: house-of-truth-1f-4.cmd01 ( b0 -- )   \ ("dynamo" shaken in z: b0 0 starts it - rest where it is (+0x38), phase
    \ (+0x34) 0, size (+0x3C) 1; 1 a step each frame - the phase back 60 degrees, the size down
    \ 0.25, z the rest's + size x sin - till it dies out)
    {: b0 :}  s" dynamo" object-here? 0= if  exit  then
    b0 0= if
        s" dynamo" place-z object@ s" dynamo" $38 object!  0e s" dynamo" $34 object!  1e s" dynamo" $3C object!  exit
    then
    b0 1 <> if  exit  then
    begin
        s" dynamo" $34 object@ 60e deg>rad f-  fdup pi fnegate f< if  2e pi f* f+  then  s" dynamo" $34 object!
        -0.25e s" dynamo" $3C object+!
        s" dynamo" $38 object@  s" dynamo" $3C object@ s" dynamo" $34 object@ fsin f* f+  s" dynamo" place-z object!
        s" dynamo" $3C object@ f0> while  yield
    repeat ;
\ a turning machine: the wheel pstr_roller (angle +0x18, height +0x24 5.1) driven by the belt
\ pstr_belt (offset +0x20 wrapping at 10, height +0x24 -3, speed +0x30). Byte 3 0 sets it up
\ (speed 0.4); 1 runs it a frame (both shaking by up to 0.05); 2 also slows it by 0.01, waiting
\ (2) until it stops. (The wheel's wrap steps +0x10, not the angle.)
: house-of-truth-1f-4.cmd02 ( b0 -- )   \ (the machine: the wheel "roller" turned by the belt "belt" at its speed
    \ (+0x30) - b0 0 sets it up (speed 0.4); 1 runs it a frame, both shaking by up to 0.05; 2 slows
    \ it by 0.01 a frame till it stops. The original's wrap of the wheel's turn steps x, not the
    \ angle: no wrap here)
    {: b0 :}
    s" roller" object-here?  s" belt" object-here? and 0= if  exit  then
    b0 0= if
        0e s" roller" turn-y object!  5.1e s" roller" place-y object!
        0e s" belt" place-x object!  -3e s" belt" place-y object!  0.4e s" belt" $30 object!  exit
    then
    b0 2 > if  exit  then
    begin
        s" belt" $30 object@ 8e f/ s" roller" turn-y object+!   \ (2 pi x speed / 16 pi)
        rnd 0.5e f- 0.1e f* 5.1e f+ s" roller" place-y object!
        s" belt" $30 object@ s" belt" place-x object+!
        begin  s" belt" place-x object@ 10e f< 0= while  -10e s" belt" place-x object+!  repeat
        rnd 0.5e f- 0.1e f* 3e f- s" belt" place-y object!
        b0 1 = if  exit  then
        -0.01e s" belt" $30 object+!
        s" belt" $30 object@ f0> while  yield
    repeat
    0e s" belt" $30 object! ;
\ Hewie's +0x14C8 to script variable 2 (byte 3 0), or back from it (1; 0 there gives 10)
: house-of-truth-1f-4.cmd03 ( b0 -- )   \ (Hewie's health kept in variable 2 - b0 0 - or put back from it: 1, 0 there gives 10)
    case
        0 of  2 hewie-hp @ var-set  endof
        1 of  hewie-hp-var var @ ?dup 0= if  10  then  hewie-id send set-hp  endof
    endcase ;
\ the placed things of kinds 0, 2, 3, 5, 7 and 8 the event manager finds in area 0xB (+0x10):
\ their timer (+0xE4) to 300000
: house-of-truth-1f-4.cmd04 ( -- )  $B things-age-out-in ;

: house-of-truth-1f-4.enter ( -- )   \ 00433CC0
    room-sounds
    found-file-250 story-flag? not ht1f-4-flag1 story-flag? not and if
        generator-var 3 var-set
        7 1 object-hidden
        8 0 object-hidden
        9 0 object-hidden
        $A 1 object-hidden
        $B 1 object-hidden
        $C 0 object-hidden
        $D 0 object-hidden
        $E 0 object-hidden
        $F 0 object-hidden
        $10 0 object-hidden
        $11 0 object-hidden
        $12 0 object-hidden
        $13 0 object-hidden
        $14 0 object-hidden
        $15 0 object-hidden
        $16 0 object-hidden
    else
        7 1 object-hidden
        8 0 object-hidden
        9 0 object-hidden
        $A 1 object-hidden
        $B 1 object-hidden
        $C 1 object-hidden
        $D 1 object-hidden
        $E 1 object-hidden
        $F 1 object-hidden
        $10 1 object-hidden
        $11 1 object-hidden
        $12 1 object-hidden
        $13 1 object-hidden
        $14 1 object-hidden
        $15 1 object-hidden
        $16 1 object-hidden
        got-sol-key story-flag? not if
            0 0.0 -2.2 65.0 flicker-sprite
        then
    then
    0 part-hidden
    ht1f-4-flag1 story-flag? not if
        force-followed state-flag-set
    then
    1 $2300 sound-volume
;

: house-of-truth-1f-4.char-enter ( -- )   \ 00433D80
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        0 0 player-camera
    then
    0 0 0 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        3 3 player-camera
    then
    1 3 3 area-camera
;

: house-of-truth-1f-4.phase1 ( -- )   \ 00433E00
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 0 0 1 chars-area-camera
    5 1 1 1 chars-area-camera
    6 0 0 1 chars-area-camera
    7 2 2 1 chars-area-camera
    8 1 1 1 chars-area-camera
    9 3 3 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    ht1f-4-flag1 story-flag? not if
        generator-on ebit? not if
            0 50.0 0.0 -28.0 5 11 0 zone
            fiona-who 0 char-in-zone? if
                0 1 track 1 action
            then
        then
    then
    found-file-250 story-flag? ht1f-4-flag1 story-flag? not and if
        generator-on ebit? stalker-reacted ebit? not and if
            stalker-who $B char-in-area? if
                stalker-reacted ebit-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 4 action-force
                else
                    1 fiona-who 4 action-force
                then
            else fiona-who $B char-entered-area? if
                stalker-reacted ebit-set
                panic-eased
                fiona-who 0 char-action? if
                    0 fiona-who 3 action-force
                else
                    1 fiona-who 3 action-force
                then
            then then
        then
        hewie-path-open ebit? if
            fiona-who 6 char-entered-area? if
                0 house-of-truth-1f-4.cmd03
                hewie-who action-end
                hewie-who char-done
                hewie-who char-activate
                1 house-of-truth-1f-4.cmd03
                house-of-truth-1f-4.cmd04
                $21 chance? if
                    hewie-who $146 char-to-tri
                else $32 chance? if
                    hewie-who $130 char-to-tri
                else
                    hewie-who $11C char-to-tri
                then then
                0 keeps-hewie-off floor-on
                hewie-path-open ebit-clear
            else hewie-who 0 char-in-nav-group? not if
                0 keeps-hewie-off floor-on
                hewie-path-open ebit-clear
            then then
        then
        door-effect-pending ebit? if
            fiona-who 6 char-entered-area? if
                house-of-truth-1f-4.cmd04
                door-effect-pending ebit-clear
            then
        then
    then
    3 44.93 0.0 -26.49 $1B 10 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 10.0 hewie-look-zone
            then
        then
    then
    generator-on ebit? if
        $C0000002 6 24.0 5.0 65.0 0 0 sound
    then
;

: house-of-truth-1f-4.phase2 ( -- )   \ 00433F50
    fiona-who $A char-in-area? fiona-who 90 $32 char-heading? and if
        0 unhurried offer-action
    then
    -2147483646 scene-request? if
        7 always offer-action
    then
    ht1f-4-flag1 story-flag? got-sol-key story-flag? not and if
        1 0.0 -3.2 65.0 5 5 0 zone
        fiona-who 1 3 char-zone-bits? if
            5 calm offer-action
        then
    then
    found-file-250 story-flag? not if
        fiona-who $C $32 char-faces-area? if
            $A unhurried offer-action
        then
    then
;

: house-of-truth-1f-4.act00 ( -- )   \ 00433FA0
    self-wait-done
    50.0 -32.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    ht1f-4-flag1 story-flag? not if
        generator-remarked ebit? not if
            0 say   \ "Looks like a power generator. The switch is turned on, but it's not getting any..."
            generator-remarked ebit-set
        else
            1 say   \ "Maybe something is wrong with the contacts? A good jolt might knock it back int..."
        then
    else
        2 say   \ "It looks completely and utterly dead. I don't think physical abuse is going to make..."
    then
    self-wait-anim
    self-idle-or-end
;

: house-of-truth-1f-4.act01 ( -- )   \ 00433FD0
    fiona-who 0 6 char-sound
    1 $FF 4 rumble
    0 house-of-truth-1f-4.cmd00
    0 house-of-truth-1f-4.cmd01
    1 house-of-truth-1f-4.cmd01
    generator-var 0 var? if
        found-file-250 story-flag? not if
            [: fiona-free? ;] wait-until
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 2 action-force
            else
                1 fiona-who 2 action-force
            then
        else
            0 2 track 6 action
        then
    then
    $1E wait-frames
    self-idle-or-end
;

: house-of-truth-1f-4.act02 ( -- )   \ 00434020
    force-followed state-flag-clear
    scene-locked state-flag-set
    stalkers-stay state-flag-set
    1 self-scripted
    house-of-truth-1f-4.cmd04
    1 wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 9 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    fiona-who $106 46.98 -19.5 -44 char-to-xz
    hewie-who char-activate
    hewie-who $53 23.54 2.5 -94 char-to-xz
    stalker-who char-activate
    $FE $8C 103 2 stalker-to-room
    stalker-who $67 -69.97 2.34 98 char-to-xz
    stalker-who 3 3 char-camera
    $E5 door-open-clear
    1 0 self-door-knock
    doors-room-in
    $E5 door-lock
    scene-5-pending state-flag-set
    stalker-who char-full-health
    6 0 object-hidden
    7 1 object-hidden
    8 0 object-hidden
    9 0 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    2 2 player-camera
    camera-restart
    found-file-250 story-flag-set
    $F $51 fade
    wait-fade
    $46 resident-flag-set
    $250 item-give
    stalkers-stay state-flag-clear
    scene-locked state-flag-clear
    0 self-scripted
    0 2 track 6 action
    self-idle-or-end
;

: house-of-truth-1f-4.act03 ( -- )   \ 00434180
    capture-no-end state-flag-set
    no-pause state-flag-set
    scene-locked state-flag-set
    1 self-scripted
    house-of-truth-1f-4.cmd04
    1 wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    2 track action-end
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        movie-loop
        scene-play-through
    then
    $47 resident-flag-set
    0 game-over-flag
    caught state-flag-set
    1.0 sound-volume-scale
    self-idle-or-end
;

: house-of-truth-1f-4.act04 ( -- )   \ 00434200
    scene-locked state-flag-set
    1 self-scripted
    house-of-truth-1f-4.cmd04
    1 wait-frames
    $F $54 fade
    self-wait-done
    wait-fade
    $40000001 6 sound-stop
    2 track action-end
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    $64 $FF movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    stalker-who action-end
    3 summon-take
    summoner-on state-flag-clear
    $E5 door-unlock
    fiona-who $7A -10.04 37.76 18 char-to-xz
    hewie-who char-activate
    hewie-who $9D 6.01 27.57 18 char-to-xz
    50 hewie-trust
    ht1f-4-flag1 story-flag-set
    1 1 player-camera
    camera-restart
    6 0 object-hidden
    7 1 object-hidden
    8 0 object-hidden
    9 0 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $C 1 object-hidden
    $D 1 object-hidden
    $E 1 object-hidden
    $F 1 object-hidden
    $10 1 object-hidden
    $11 1 object-hidden
    $12 1 object-hidden
    $13 1 object-hidden
    $14 1 object-hidden
    $15 1 object-hidden
    $16 1 object-hidden
    0 0.0 -2.2 65.0 flicker-sprite
    0 part-hidden
    1 exit-prepare
    $F $51 fade
    wait-fade
    $48 resident-flag-set
    0 keeps-hewie-off floor-off
    scene-locked state-flag-clear
    scene-5-pending state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-4.act05 ( -- )   \ 00434350
    1 self-scripted
    self-wait-done
    0.0 65.0 self-turn-to-xz
    self-wait-done
    pick-up-anim play-out
    4 wait-frames
    0 effect-remove
    $25 message-param-room
    $25 1 item-give-count
    0 $25 item-tab
    item-fanfare
    $8012 say   \ "You've obtained ."
    up-from-pick-anim play-out
    got-sol-key story-flag-set
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-4.act06 ( -- )   \ 004343A0
    0 part-shown
    $40000002 6 24.0 5.0 65.0 0 0 sound
    generator-on ebit-set
    0 part-shown
    hewie-path-open ebit-set
    door-effect-pending ebit-set
    run-frames-var 0 var-set
    0 house-of-truth-1f-4.cmd02
    begin
        1 house-of-truth-1f-4.cmd02
        1 wait-frames
        run-frames-var var-inc
        run-frames-var 240 var? until
    $40000001 6 24.0 5.0 65.0 0 0 sound
    generator-on ebit-clear
    0 part-hidden
    2 house-of-truth-1f-4.cmd02
    0 keeps-hewie-off floor-off
    generator-var 3 var-set
    0 part-hidden
    self-idle-or-end
;

: house-of-truth-1f-4.act07 ( -- )   \ 00434410
    scene-5-pending state-flag? if
        3 message   \ "No! It's locked!"
    else
        self-wait-done
        0 self-through-exit
        self-wait-done
        locked-remarked ebit? not 2 game-mode? or in-panic? or if
            in-panic? 2 game-mode? or if
                rattle-front-slow-anim play-out
                -1 self-idle-anim
                7 wait-frames
            else
                rattle-front-anim play-out
                -1 self-idle-anim
                7 wait-frames
                4 say   \ "The door is locked."
            then
            locked-remarked ebit-set
        else
            4 wait-frames
            fiona-who $BF 90.5 0.0 90 char-to-xz
            world-frozen state-flag-set
            1 self-scripted
            1 20.0 0.0 0.0 2.0 event-camera
            4 wait-frames
            5 say   \ "A strange symbol is carved into the door. I wonder what this symbol means?"
            4 wait-frames
            0 0.0 0.0 0.0 0.0 event-camera
            world-frozen state-flag-clear
            0 self-scripted
            locked-remarked ebit-clear
        then
        $251 item-give
    then
    self-idle-or-end
;

: house-of-truth-1f-4.act08 ( -- )   \ 004344B0
    1 self-scripted
    self-wait-done
    0 self-through-exit
    self-wait-done
    $608 self-anim
    $14 wait-frames
    fiona-who 3 6 char-sound
    $25 message-param-room
    $E2 door-locked? not if
        $25 item-use
    then
    $E6 door-unlock
    20 self-idle-anim
    $14 wait-frames
    $8019 say   \ "Used ."
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: house-of-truth-1f-4.act09 ( -- )   \ 004344F0
    0 part-hidden
    [: $F cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $12 cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $13 cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $14 cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $17 cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $28 cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $29 cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $2A cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $2B cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $2C cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $31 cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $37 cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $38 cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $39 cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $3A cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $3B cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $3C cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $53 cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $54 cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: $63 cutscene-cue-reached? ;] wait-until
    0 part-hidden
    [: $64 cutscene-cue-reached? ;] wait-until
    0 part-shown
    [: cutscene-near-end? ;] wait-until
    self-idle-or-end
;

: house-of-truth-1f-4.act0A ( -- )   \ 00434600
    self-wait-done
    demolisher-remarked ebit? not if
        6 say   \ "This looks like a demolisher. These things are used to smash large rocks into bits..."
        demolisher-remarked ebit-set
    else
        hand-to-mouth-anim $A blend-out
        7 say   \ "It's not running right now... Maybe there is a power switch somewhere that needs to..."
    then
    self-idle-or-end
;

: house-of-truth-1f-4.act0B ( -- )   \ 00434620
    self-wait-done
    7 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    1 0 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    0 cutscene-track 9 action
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

: house-of-truth-1f-4.act0C ( -- )   \ 004346B0
    self-wait-done
    7 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
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

: house-of-truth-1f-4.act0D ( -- )   \ 00434740
    self-wait-done
    7 1 object-hidden
    $A 1 object-hidden
    $B 1 object-hidden
    $B 3 $FF char-load
    3 char-start
    5 1 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    $64 $FF movie-param
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

: house-of-truth-1f-4.phase5 ( -- )   \ 0047AE50
    force-followed state-flag-clear
;

\ ---- registered ----
house-of-truth-1f-4 register-scripts   \ (its phases and actions, by their names)
