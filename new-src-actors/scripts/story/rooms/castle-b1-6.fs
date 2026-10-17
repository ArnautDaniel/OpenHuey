\ story/rooms/castle-b1-6.fs - the event scripts of room castle-b1-6 ($C; Belli Castle: B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-b1-6
USING: engine room-names story.words story.shared flag-names story-names ;

\ room 0x0C (Room0C_Cmd01_ptmf): script variable byte 3 down by the player's hit (byte 4: 1 from
\ the weak blow 0x1A, else 5) or the pursuer's (+0x108), not below 0
: castle-b1-6.cmd01 ( b0 b1 -- )   \ (script variable b0 down by the hit: b1 Fiona's - 1 from her weak blow ($1A), else 5;
    \ else the stalker's - his attack's blow under way, if any - not below 0)
    if  fiona-move-sub $1A = if  1  else  5  then  else  stalker-blow?  then        ( var k )
    dup 0> if  over var @ swap - 0 max var-set  else  2drop  then ;
\ room 0x0C (Room0C_Cmd02_ptmf): the screen darkened as the cutscene runs past frame 0x4AE (32 a
\ frame, up to 0x80)
: castle-b1-6.cmd02 ( -- )   \ (the screen darkened as the scene runs past frame $4AE: 32 a frame, to $80 - full
    \ before it, as the original's unsigned count gives)
    cutscene-frame $4AE - dup 0< if  drop $80  else  5 lshift $80 min  then  screen-fill ;
\ room 0x0C (Room0C_Cmd03_ptmf): three objects (pair byte 4) swing: byte 3 0 set up (rest +0x30,
\ phase +0x34 half a turn apart, swing +0x3C 0.75 / 0.5), 1 a step (phase on 60 degrees, the
\ swing down 0.1, x = rest + swing x sin(phase); 2 once still), 2 all to x 10. 2 while any
: swings-step ( pair -- still? )   \ (Room0C_Cmd03 1: each on 60 degrees, its swing down 0.1; still - to x 10)
    {: pair | moving a n :}  0 to moving
    3 0 do  pair i 2* + 13 + room-object to n to a  a n object-here? if
        60e deg>rad a n $34 object+!  a n $34 object@ pi f> if  -2e pi f* a n $34 object+!  then
        -0.1e a n $3C object+!
        a n $30 object@ a n $3C object@ a n $34 object@ fsin f* f+ a n place-x object!
        a n $3C object@ f0> if  -1 to moving  else  10e a n place-x object!  then
    then  loop  moving 0= ;
\ swings
: castle-b1-6.cmd03 ( b0 b1 -- )   \ (three things - the room's strings b1 + 2i + 13 - swinging: 0 set up (rest, half a turn
    \ apart, swings 0.75 / 0.75 / 0.5); 1 swinging till all are still; 2 all to x 10)
    {: op pair :}
    op 1 = if  begin  pair swings-step 0= while  yield  repeat  exit  then
    3 0 do  pair i 2* + 13 + room-object 2dup object-here? if
        op 0= if
            2dup place-x object@ 2dup $30 object!  0.5e i s>f f* 2dup $34 object!
            i 2 <> if  0.75e  else  0.5e  then  $3C object!
        else  op 2 = if  10e place-x object!  else  2drop  then  then
    else  2drop  then  loop ;

: castle-b1-6.enter ( -- )   \ 003F4650
    room-sounds
    $15 1.0 0 bgm
    got-saturnus-key story-flag? not if
        0 1 $14 door-bits
    then
    1 1 $14 door-bits
    6 1 object-hidden
    cb1-6-flag4 story-flag? not if
        0 25 var-set
    else
        $D 1 object-hidden
        5 1 $14 door-bits
    then
    cb1-6-flag5 story-flag? not if
        1 25 var-set
    else
        $E 1 object-hidden
        3 1 $14 door-bits
    then
    found-file-20b story-flag? not if
        9 0 object-hidden
        $A 1 object-hidden
        cb1-6-flag2 story-flag? if
            7 1 $14 door-bits
            1 0 8 nav-group
        then
    else
        9 1 object-hidden
        $A 0 object-hidden
        1 0 8 nav-group
    then
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    1 $2300 sound-volume
;

: castle-b1-6.char-enter ( -- )   \ 003F46E0
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            0 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        0 -1 player-camera
    then then
    0 0 -1 area-camera
;

: castle-b1-6.phase1 ( -- )   \ 003F4720
    0 exit-usable? if
        0 exit-check
    then
    1 0 -1 1 chars-area-camera
    2 1 0 1 chars-area-camera
    3 1 0 1 chars-area-camera
    4 2 1 1 chars-area-camera
    8 0 -1 1 chars-area-camera
    9 1 0 1 chars-area-camera
    $A 0 -1 1 chars-area-camera
    $B 1 0 1 chars-area-camera
    got-saturnus-key story-flag? cb1-6-flag1 story-flag? not and cb1-6-flag2 story-flag? not and fiona-who 7 char-entered-area? and 2 ebit? not and if
        2 ebit-set
        panic-eased
        fiona-who 0 char-action? if
            0 fiona-who 0 action-force
        else
            1 fiona-who 0 action-force
        then
    then
    cb1-6-flag4 story-flag? not if
        0 10.0 0.0 -37.0 7 20 0 zone
        stalker-who 0 char-in-zone? fiona-who 0 char-in-zone? or if
            0 $F1 8 action
        then
    then
    cb1-6-flag5 story-flag? not if
        1 10.0 0.0 37.0 7 20 0 zone
        stalker-who 1 char-in-zone? fiona-who 1 char-in-zone? or if
            0 $F2 9 action
        then
    then
    cb1-6-flag1 story-flag? cb1-6-flag2 story-flag? not and if
        stalker-who 2 char-cond? if
            $28 door-unlock
            cb1-6-flag2 story-flag-set
            force-calm state-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 1 action-force
            else
                1 fiona-who 1 action-force
            then
        then
        cb1-6-flag4 story-flag? cb1-6-flag5 story-flag? and if
            $28 door-unlock
            cb1-6-flag2 story-flag-set
            force-calm state-flag-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 2 action-force
            else
                1 fiona-who 2 action-force
            then
        then
    then
    2 166.49 6.0 0.6 $24 36 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 2 9 char-zone-bits? if
                $1F 2 var-set
                $1F 36.0 hewie-look-zone
            then
        then
    then
;

: castle-b1-6.phase2 ( -- )   \ 003F4850
    scene-5-pending state-flag? if
        -2147483646 scene-request? if
            5 3 1 scene-change
        then
    then
    fiona-who 5 char-in-area? fiona-who 45 $32 char-heading? and if
        5 4 0 scene-change
    then
    cb1-6-flag1 story-flag? not cb1-6-flag2 story-flag? or if
        fiona-who $C char-in-area? fiona-who 10 -40 $32 char-faces-xz? and if
            5 6 0 scene-change
        then
        fiona-who $D char-in-area? fiona-who 10 40 $32 char-faces-xz? and if
            5 6 0 scene-change
        then
    then
    found-file-20b story-flag? if
        fiona-who $E $32 char-faces-area? if
            5 5 0 scene-change
        then
    else cb1-6-flag2 story-flag? if
        fiona-who $E $32 char-faces-area? if
            5 $B 0 scene-change
        then
    then then
;

: castle-b1-6.act00 ( -- )   \ 003F48C0
    stalkers-stay state-flag-set
    1 self-scripted
    force-calm state-flag-set
    self-wait-done
    -1 self-idle-anim
    $FF 1.0 0 bgm
    $F $44 fade
    2 0 char-remove
    $1B partner-load
    summoner-on state-flag-clear
    2 char-start
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    1 0 $14 door-bits
    hewie-who char-activate
    $C 0 209 hewie-to-room
    $FE $C 206 2 stalker-to-room
    stalker-who 0 -1 char-camera
    stalker-who char-activate
    stalker-who 1 char-no-shadow
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    stalker-who 0 char-no-shadow
    found-file-209 story-flag-set
    $11 door-unlock
    1 1 $14 door-bits
    6 1 object-hidden
    $28 door-open-clear
    $28 door-lock
    doors-room-in
    scene-5-pending state-flag-set
    fiona-who 3 -90 char-to-tri-facing
    hewie-who char-activate
    $C 0 209 hewie-to-room
    hewie-who $D1 -90 char-to-tri-facing
    $FE $C 206 2 stalker-to-room
    stalker-who char-activate
    stalker-who 2 stalker-command
    stalker-who 0 stalker-search-delay
    stalker-who $CE 90 char-to-tri-facing
    stalker-who char-full-health
    cb1-6-flag1 story-flag-set
    2 1 player-camera
    camera-restart
    world-held state-flag-clear
    force-calm state-flag-clear
    1 1.0 0 bgm
    $F 1 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-6.act01 ( -- )   \ 003F4A00
    $FF 1.0 0 bgm
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F 4 fade
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    scene-5-pending state-flag-clear
    2 0 char-remove
    hewie-who char-activate
    50 hewie-trust
    fiona-who $11C 56.239 -3.659 -45 char-to-xz
    $C 0 78 hewie-to-room
    hewie-who $4E 57.799 0.951 0 char-to-xz
    7 1 $14 door-bits
    1 0 8 nav-group
    1 0 player-camera
    camera-restart
    $28 door-open-clear
    $28 door-unlock
    doors-room-in
    $15 1.0 0 bgm
    force-calm state-flag-clear
    $F $41 fade
    wait-fade
    $28 resident-flag-set
    $22F item-give
    $8297 item-give
    shared.act9C
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-6.act02 ( -- )   \ 003F4B10
    $FF 1.0 0 bgm
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    hewie-no-attack state-flag-set
    stalkers-blind state-flag-set
    $F 4 fade
    8 0 movie-play
    7 cutscene-start
    yield
    yield
    scene-wait-ready
    wait-fade
    hewie-who action-end
    hewie-who char-done
    0 $F9 7 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    wait-fade
    $26 resident-flag-set
    world-held state-flag-set
    9 1 object-hidden
    $F 0 object-hidden
    $10 0 object-hidden
    $11 0 object-hidden
    $12 0 object-hidden
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    self-frames-reset
    self-wait-16
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $A action
    panic-eased
    2 message-prepare   \ "It's OK, Hewie... He can't come after us anymore... Good boy. Calm down, Hewie..."
    1 result? if
        scene-play
    then
    6 0 $14 door-bits
    scene-5-pending state-flag-clear
    hewie-no-attack state-flag-clear
    stalkers-blind state-flag-clear
    50 hewie-trust
    2 0 char-remove
    hewie-who char-activate
    fiona-who $11C 56.239 -3.659 -45 char-to-xz
    $C 0 78 hewie-to-room
    hewie-who $4E 57.799 0.951 0 char-to-xz
    1 0 player-camera
    camera-restart
    $28 door-open-clear
    $28 door-unlock
    doors-room-in
    $A 0 object-hidden
    1 0 8 nav-group
    found-file-20b story-flag-set
    0 panic-stage
    $15 1.0 0 bgm
    force-calm state-flag-clear
    $F $41 fade
    wait-fade
    $20B item-give
    $8296 item-give
    shared.act9C
    $27 resident-flag-set
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-b1-6.act03 ( -- )   \ 0047A9CC
    $B message   \ "Oh no! It's locked!!"
    self-idle-or-end
;

: castle-b1-6.act04 ( -- )   \ 003F4CD0
    stalkers-stay state-flag-set
    self-wait-done
    160.0 0.0 self-turn-to-xz
    self-wait-done
    $A01 $A self-anim-blend
    self-wait-anim
    6 message   \ "There are some letters engraved here..."
    wait-message
    $F 6 fade
    wait-fade
    7 message   \ "To the seeker of the sacred technique of Ars, Obey the will of Hermes' divine cane and..."
    wait-message
    $F 7 fade
    wait-fade
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    got-saturnus-key story-flag? not if
        8 message   \ "A cane? Hmm, curious. I'll bet this is related to that goddess statue... Search mor..."
        wait-message
        self-wait-anim
        1 self-scripted
        0 answer? if
            $FF 1.0 0 bgm
            $F $44 fade
            1 1 movie-play
            0 cutscene-start
            yield
            yield
            scene-wait-ready
            wait-fade
            hewie-who action-end
            hewie-who char-done
            creatures-on state-flag-clear
            1 creatures-clear
            c1f-1-flag1 story-flag-set
            got-doc-luminessants story-flag-set
            cb1-6-flag3 story-flag-set
            found-file-227-2 story-flag-set
            c2f-2-flag3 story-flag-set
            c2f-1-flag1 story-flag-set
            c1f-13-flag1 story-flag-set
            $28 $C8 movie-param
            panic-eased
            $FFFF message-prepare
            1 result? if
                scene-play
            then
            0 self-idle-anim
            fiona-who $CD -90 char-to-tri-facing
            camera-restart
            hewie-who char-activate
            $C 0 401 hewie-to-room
            hewie-who $191 0 char-to-tri-facing
            hewie-who char-full-health
            hewie-who 0 char-cond!
            $28 door-open-set
            doors-room-in
            0 0 $14 door-bits
            got-saturnus-key story-flag-set
            $15 1.0 0 bgm
            $F $41 fade
            wait-fade
            6 message-param-room
            6 1 item-give-count
            0 6 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
            30 hewie-trust
        then
        0 self-scripted
    else
        $A message   \ "If this is the chapel... This statue must symbolize the deity worshipped here."
        wait-message
        self-wait-anim
    then
    stalkers-stay state-flag-clear
    self-idle-or-end
;

: castle-b1-6.act05 ( -- )   \ 003F4E30
    1 ebit? not if
        $F message   \ "What a fancy chandelier. It must have been quite expensive."
        wait-message
        1 ebit-set
    else
        $A01 $A self-anim-blend
        $10 message   \ "I-I could have been killed... I guess fate was smiling on me today... for now."
        wait-message
        self-wait-anim
    then
    self-idle-or-end
;

: castle-b1-6.act06 ( -- )   \ 003F4E50
    self-wait-done
    found-file-20b story-flag? if
        $E message   \ "It's broken beyond repair..."
        wait-message
    else 0 ebit? not if
        $C message   \ "A pulley system used to wind up rope... Looks like this is used to raise and lower..."
        wait-message
        0 ebit-set
    else
        $D message   \ "It is tightly fastened... Must be to make sure the chandelier doesn't accidentally..."
        wait-message
        0 ebit-clear
    then then
    self-idle-or-end
;

: castle-b1-6.act07 ( -- )   \ 003F4E70
    begin
        3 cutscene-shot? not while
        yield
    repeat
    begin
        4 cutscene-shot? not while
        46.0 46.0 116.5 116.5 depth-range
        yield
    repeat
    begin
        $4AE cutscene-cue-reached? not while
        yield
    repeat
    begin
        0 cutscene-cue-reached? while
        castle-b1-6.cmd02
        yield
    repeat
    wait-fade
    self-idle-or-end
;

: castle-b1-6.act08 ( -- )   \ 003F4EB0
    cb1-6-flag1 story-flag? cb1-6-flag2 story-flag? not and if
        stalker-who 0 char-in-zone? if
            0 0 castle-b1-6.cmd01
        else
            0 1 castle-b1-6.cmd01
        then
    then
    0 0 var? if
        10.0 10.0 -35.0 0 -2144325584 3 -9.0 shards-at
        $D 1 object-hidden
        5 1 $14 door-bits
        cb1-6-flag4 story-flag-set
        1 6 10.0 10.0 -40.0 0 0 sound
    else
        0 6 10.0 10.0 -40.0 0 0 sound
    then
    0 0 castle-b1-6.cmd03
    1 0 castle-b1-6.cmd03
    self-frames-reset
    $14 self-wait-frames
    self-idle-or-end
;

: castle-b1-6.act09 ( -- )   \ 003F4F30
    cb1-6-flag1 story-flag? cb1-6-flag2 story-flag? not and if
        stalker-who 1 char-in-zone? if
            1 0 castle-b1-6.cmd01
        else
            1 1 castle-b1-6.cmd01
        then
    then
    1 0 var? if
        10.0 10.0 35.0 0 -2144325584 3 -9.0 shards-at
        $E 1 object-hidden
        3 1 $14 door-bits
        cb1-6-flag5 story-flag-set
        1 6 10.0 10.0 40.0 0 0 sound
    else
        0 6 10.0 10.0 40.0 0 0 sound
    then
    0 1 castle-b1-6.cmd03
    1 1 castle-b1-6.cmd03
    self-frames-reset
    $14 self-wait-frames
    self-idle-or-end
;

: castle-b1-6.act0A ( -- )   \ 003F4FB0
    begin
        5 cutscene-shot? if
            6 1 $14 door-bits
        then
        6 cutscene-shot? if
            6 0 $14 door-bits
        then
        cutscene-near-end? not while
        yield
    repeat
    self-idle-or-end
;

: castle-b1-6.act0B ( -- )   \ 003F4FC8
    self-wait-done
    $1D02 $A self-anim-blend
    $11 message   \ "He's not moving... I-I think he's dead..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-b1-6.act0C ( -- )   \ 003F4FE0
    self-wait-done
    0 1 $14 door-bits
    1 1 $14 door-bits
    6 1 object-hidden
    9 0 object-hidden
    $A 1 object-hidden
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $A00 self-anim
    self-frames-reset
    $28 self-wait-frames
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $28 $C8 movie-param
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    0 0 $14 door-bits
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-b1-6.act0D ( -- )   \ 003F50A0
    self-wait-done
    9 0 object-hidden
    $A 1 object-hidden
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    -1 self-idle-anim
    $1B partner-load
    2 char-start
    5 0 movie-play
    4 cutscene-start
    yield
    yield
    scene-wait-ready
    1 0 $14 door-bits
    hewie-who char-activate
    $C 0 209 hewie-to-room
    $FE $C 206 2 stalker-to-room
    stalker-who char-activate
    stalker-who 1 char-no-shadow
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    stalker-who 0 char-no-shadow
    2 0 char-remove
    1 1 $14 door-bits
    6 1 object-hidden
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-b1-6.act0E ( -- )   \ 003F5170
    self-wait-done
    $D 1 object-hidden
    5 1 $14 door-bits
    $E 1 object-hidden
    3 1 $14 door-bits
    9 0 object-hidden
    $A 1 object-hidden
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-start
    8 0 movie-play
    7 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 7 action
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-b1-6.act0F ( -- )   \ 003F5230
    self-wait-done
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-start
    9 1 object-hidden
    $F 0 object-hidden
    $10 0 object-hidden
    $11 0 object-hidden
    $12 0 object-hidden
    hewie-who 0 char-visible
    hewie-who action-end
    hewie-who action-end
    hewie-who char-done
    stalker-who action-end
    stalker-who char-done
    self-frames-reset
    self-wait-16
    3 0 movie-play
    2 cutscene-start
    yield
    yield
    scene-wait-ready
    0 $F9 $A action
    panic-eased
    2 message-prepare   \ "It's OK, Hewie... He can't come after us anymore... Good boy. Calm down, Hewie..."
    1 result? if
        scene-play
    then
    6 0 $14 door-bits
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-b1-6.act10 ( -- )   \ 003F5300
    self-wait-done
    9 0 object-hidden
    $A 1 object-hidden
    0 8 0.0 0.0 0.25 0.25 zone-rect
    6 0 $14 door-bits
    $1B partner-load
    2 char-start
    $C 0 movie-play
    $B cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    2 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

\ ---- registered ----
' castle-b1-6.enter castle-b1-6 0 room-script!
' castle-b1-6.char-enter castle-b1-6 6 room-script!
' castle-b1-6.phase1 castle-b1-6 1 room-script!
' castle-b1-6.phase2 castle-b1-6 2 room-script!
' castle-b1-6.act00 castle-b1-6 $00 action-script!
' castle-b1-6.act01 castle-b1-6 $01 action-script!
' castle-b1-6.act02 castle-b1-6 $02 action-script!
' castle-b1-6.act03 castle-b1-6 $03 action-script!
' castle-b1-6.act04 castle-b1-6 $04 action-script!
' castle-b1-6.act05 castle-b1-6 $05 action-script!
' castle-b1-6.act06 castle-b1-6 $06 action-script!
' castle-b1-6.act07 castle-b1-6 $07 action-script!
' castle-b1-6.act08 castle-b1-6 $08 action-script!
' castle-b1-6.act09 castle-b1-6 $09 action-script!
' castle-b1-6.act0A castle-b1-6 $0A action-script!
' castle-b1-6.act0B castle-b1-6 $0B action-script!
' castle-b1-6.act0C castle-b1-6 $0C action-script!
' castle-b1-6.act0D castle-b1-6 $0D action-script!
' castle-b1-6.act0E castle-b1-6 $0E action-script!
' castle-b1-6.act0F castle-b1-6 $0F action-script!
' castle-b1-6.act10 castle-b1-6 $10 action-script!
