\ story/rooms/castle-2f-2.fs - the event scripts of room castle-2f-2 ($20; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-2
USING: engine actors room-names story.words story.shared flag-names story-names fiona.anims fiona.sounds stalker.anims ;

\ ---- the doll ("dool") that falls: +$30 its velocity, +$3C its triangle ($80000000: landed; -1
\ none) ----
: dool ( -- addr len )  s" dool" ;
create dl-at 12 allot
: dool-at ( -- v )  dool place-x object@ dool place-y object@ dool place-z object@ dl-at vec!  dl-at ;
: open-floor? ( tri -- flag )   \ (on the mesh, not blocked for it: $20020008)
    dup 0< if  drop false exit  then  nav-flags $20020008 and 0= ;
: dool-dust ( -- )  2 $50 $50 $50 1  dool-at vec@  dust ;   \ (Room20_Cmd04: a grey cloud)
: dool-lands ( tri -- )
    7 ebit-clear  0e dool $30 object!  0e dool $34 object!  0e dool $38 object!
    dup open-floor? 0= if  1 ebit-clear  then
    dup 0< 0= if  dup nav-flags $2018000 and $10000 <> if  dool-dust  then  then
    $80000000 or dool $3C object-n!  1 ebit-set ;
: dool-falls ( -- )   \ (gravity 0.2 a frame; along the floor as it slides; spinning a degree a frame)
    dool $3C object-n@ dup -1 = if  drop exit  then
    dup $80000000 and if  $7FFFFFFF and open-floor? 0= if  1 ebit-clear  then  exit  then  drop
    -0.2e dool $34 object+!  1 ebit-clear
    dool place-y object@ dool $34 object@ f+                           ( F: y )
    dool $30 object@ dool $38 object@  dool-at $20020008 v-nav-move     ( tri ) ( F: y )
    dl-at vec@ dool place-z object! dool place-y object! dool place-x object!
    dup dool $3C object-n!
    dup -1 = if  drop fdrop exit  then
    fdup dl-at 4 + sf@ f> if
        drop dool place-y object!  1e deg>rad dool turn-y object+!
        dool turn-y object@ pi f> if  -2e pi f* dool turn-y object+!  then  exit
    then  fdrop  dool-lands ;
\ room 0x20 (Room20_Cmd01_ptmf): the falling object back up in place
: castle-2f-2.cmd01 ( -- )   \ (the doll back up in place, shown)
    dool object-here? 0= if  exit  then  dool 0 0 placed-op
    9.66e dool place-x object!  0.51e dool place-y object!  93.4e dool place-z object!
    1.57e dool turn-x object!  0.79e dool turn-y object!  -3.14e dool turn-z object! ;
\ a pendulum (room object Room20_ObjectNames[byte 3]): byte 4 0 still; 1 its phase +0x30 on by 2
\ degrees (a tick sound at (-85, 30, 90) each turn), swinging 15 degrees (+0x14)
: castle-2f-2.cmd02 ( b0 b1 -- )   \ (the pendulum, the room's string b0: b1 0 still; 1 on 2 degrees, a tick each turn,
    \ swinging 15 degrees)
    {: k op | a n :}  k room-object to n to a  a n object-here? 0= if  exit  then
    op 0= if  0e a n $30 object!  exit  then
    op 1 <> if  exit  then
    2e a n $30 object+!
    a n $30 object@ 360e f< 0= if  -360e a n $30 object+!  1 6 0 0  -85e 30e 90e sound  then
    a n $30 object@ deg>rad fsin 15e f* deg>rad
    fdup pi f> if  2e pi f* f-  then  a n turn-y object! ;
\ room 0x20 (Room20_Cmd03_ptmf): the falling object falls (gravity 0.2 a frame on its velocity
\ +0x30, spinning 1 degree a frame) along the nav mesh, events bit 1 set while it lies on open
\ floor; landing on floor that isn't 0x10000 raises dust
: castle-2f-2.cmd03 ( -- )  dool-falls ;
\ room 0x20 (Room20_Cmd05_ptmf): the character turns to face object byte 3
: castle-2f-2.cmd05 ( b0 -- )   \ (the script's character turned to face object b0)
    room-object 2dup object-here? 0= if  2drop exit  then
    2dup place-x object@ place-z object@  self-turn-to-xz ;
\ room 0x20 (Room20_Cond00_ptmf): the player, free and within 5 of the falling object, knocks it
\ - it gets a push (0, 1, 1) turned by her facing, and its nav triangle
: castle-2f-2.cond00? ( -- flag )   \ (her blow within 5 of the doll knocks it: a push (0, 1, 1) by her heading, its triangle)
    dool object-here? 0= if  false exit  then
    0 char-actor body-point 0= if  false exit  then                    ( v )
    dool-at vec-dist 5e f> if  false exit  then
    0 char-actor body-yaw fdup fsin dool $30 object!  1e dool $34 object!  fcos dool $38 object!
    dool-at v-tri dool $3C object-n!  true ;
\ room 0x20 (Room20_Cond01_ptmf): the character's script value is at least be32 bytes 3..6
: castle-2f-2.cond01? ( b0 b1 b2 b3 -- flag )   \ (the script's own count at least be32 bytes 3..6)
    >r >r >r 24 lshift r> 16 lshift or r> 8 lshift or r> or
    self-frames @ $FFFF and swap >= ;
\ room 0x20 (Room20_Cond02_ptmf): object byte 3 becomes event point byte 4 (radii 5)
: castle-2f-2.cond02? ( b0 b1 -- flag )   \ (object b0's place becomes zone b1, radius and height 5)
    swap room-object 2dup object-here? 0= if  2drop drop false exit  then
    {: z a n :}  z 5 5 0  a n place-x object@ a n place-y object@ a n place-z object@  zone  true ;

: castle-2f-2.enter ( -- )   \ 003FE460
    room-sounds
    c2f-2-flag1 story-flag? not if
        0 2 track 9 action
    then
    c2f-2-flag2 story-flag? not if
        0 0 0 $36B $3AF obstacle-place
    else
        0 0 0 $377 $3BB obstacle-place
        0 obstacle-stop
    then
    0 23.3 12.75 87.0 0 flame-in
    1 25.15 12.75 86.0 0 flame-in
    c2f-2-album-medallion-found story-flag? c2f-2-album-medallion story-flag? not and if
        2 -11.6 1.0 142.2 flicker-sprite
    then
    got-lunar-refractor story-flag? if
        c2f-2-flag3 story-flag? not if
            c2f-2-flag3 story-flag-set
            $20 0 316 $D 15 -1 0 0.0 creature-place
            $20 0 361 4 15 -1 0 0.0 creature-place
        then
    then
    c2f-3-flag1 story-flag? not if
        $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    then
    3 0 castle-2f-2.cmd02
    4 0 castle-2f-2.cmd02
    0 4 track $11 action
    5 1 object-hidden
    got-bisque-doll story-flag? not if
        1 ebit-clear
        c2f-2-flag1 story-flag? c2f-2-flag5 story-flag? not and if
            5 0 object-hidden
            castle-2f-2.cmd01
            1 ebit-set
            7 ebit-set
        then
    then
    c1f-8-flag1 story-flag? found-file-209 story-flag? not and if
        $80 exit-taken? if
        else
            0 3 track $E action
        then
    then
;

: castle-2f-2.char-enter ( -- )   \ 003FE540
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        2 2 player-camera
    then then
    0 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 1 player-camera
    then then
    1 1 1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            0 0 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        0 0 player-camera
    then then
    2 0 0 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            2 2 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        2 2 player-camera
    then then
    3 2 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 4 exit-taken? and if
            fiona-who 4 char-to-exit
            3 3 player-camera
        then
    else hewie-who self-is? 4 exit-taken? and if
        hewie-who 4 char-to-exit
        3 3 player-camera
    then then
    4 3 3 area-camera
    fiona-who self-is? if
        $80 exit-taken? if
            fiona-who $142 0 char-to-tri-facing
            0 fiona-who 0 action
        then
    then
;

: castle-2f-2.phase1 ( -- )   \ 003FE680
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    c1f-8-flag1 story-flag? found-file-209 story-flag? not and if
        2 exit-usable? if
            3 3 2 char-load
            4 4 2 char-load
            2 exit-check
        then
    else 2 exit-usable? if
        2 exit-check
    then then
    3 exit-usable? if
        3 exit-check
    then
    4 exit-usable? if
        4 exit-check
    then
    $D 3 3 1 chars-area-camera
    $E 1 1 1 chars-area-camera
    $F 1 1 1 chars-area-camera
    $10 2 2 1 chars-area-camera
    $14 3 3 1 chars-area-camera
    $15 0 0 1 chars-area-camera
    $1A 1 1 1 chars-area-camera
    fiona-who 5 char-entered-area? fiona-who $B char-entered-area? or if
        0 exit-prepare
    then
    fiona-who 6 char-entered-area? fiona-who 7 char-entered-area? or if
        1 exit-prepare
    then
    fiona-who 9 char-entered-area? if
        c1f-8-flag1 story-flag? found-file-209 story-flag? not and if
            3 3 2 char-load
            4 4 2 char-load
        then
        2 exit-prepare
    then
    fiona-who $C char-entered-area? if
        3 exit-prepare
    then
    fiona-who 8 char-entered-area? if
        4 exit-prepare
    then
    fiona-who $A char-entered-area? if
        c1f-8-flag1 story-flag? found-file-209 story-flag? not and if
            3 0 char-remove
            4 0 char-remove
        then
        4 exit-prepare
    then
    c2f-2-flag1 story-flag? not if
        fiona-who $17 char-entered-area? 8 ebit? not and if
            8 ebit-set
            panic-eased
            fiona-who 0 char-action? if
                0 fiona-who 3 action-force
            else
                1 fiona-who 3 action-force
            then
        then
    then
    c2f-2-flag2 story-flag? not if
        0 $377 obstacle-on? if
            c2f-2-flag2 story-flag-set
            0 obstacle-stop
            $12 door-unlock
            stalker-who $24 0 room-doors-state
        then
    then
    0 0 8 -4 0 zone-at-effect
    fiona-who 0 3 char-zone-bits? fiona-who 0 3 char-zone-bits-before? not and if
        fiona-who 0 char-effect-moving
    then
    hewie-who 0 3 char-zone-bits? hewie-who 0 3 char-zone-bits-before? not and if
        hewie-who 0 char-effect-moving
    then
    stalker-who 0 3 char-zone-bits? stalker-who 0 3 char-zone-bits-before? not and if
        stalker-who 0 char-effect-moving
    then
    1 1 8 -4 0 zone-at-effect
    fiona-who 1 3 char-zone-bits? fiona-who 1 3 char-zone-bits-before? not and if
        fiona-who 1 char-effect-moving
    then
    hewie-who 1 3 char-zone-bits? hewie-who 1 3 char-zone-bits-before? not and if
        hewie-who 1 char-effect-moving
    then
    stalker-who 1 3 char-zone-bits? stalker-who 1 3 char-zone-bits-before? not and if
        stalker-who 1 char-effect-moving
    then
    c2f-2-album-medallion-found story-flag? not if
        2 -11.6 0.0 142.2 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var c2f-2-album-medallion-found var-set
                taken-flag-var c2f-2-album-medallion var-set
                sparkle-fx-var 2 var-set
                sparkle-x-var -11600 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var 142200 var-set
                hewie-zone-var 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    got-bisque-doll story-flag? not if
        1 ebit? if
            castle-2f-2.cond00? if
                fiona-who 4 6 char-sound
                1 ebit-clear
                7 ebit-clear
                1 track action-end
                0 1 track $B action
            then
            5 6 castle-2f-2.cond02? if
            then
        then
    then
    c1f-8-flag1 story-flag? found-file-209 story-flag? not and if
        hewie-who 2 char-in-area? not if
            1 -144.0 0.0 109.0 $1E 20 0 zone
            3 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
                2 game-mode? not if
                    hewie-can-command? if
                        hewie-who 1 9 char-zone-bits? if
                            3 ebit-set
                            $64 chance? if
                                hewie-zone-var 1 var-set
                                0 hewie-who $8A action
                            then
                        then
                    then
                then
            then
        then
    then
    c2f-2-flag2 story-flag? hewie-who 4 char-in-area? not and if
        0 33.656 0.0 111.061 $A 20 0 zone
        2 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
            2 game-mode? not if
                hewie-can-command? if
                    hewie-who 0 9 char-zone-bits? if
                        2 ebit-set
                        $1E chance? if
                            hewie-zone-var 0 var-set
                            0 hewie-who $89 action
                        then
                    then
                then
            then
        then
    then
    stalker-who $1B char-in-area? 4 stalker-alert? and stalker-who 2 char-cond? not and if
        2 stalker-kind-here? 6 stalker-kind-here? or 7 stalker-kind-here? or if
            0 stalker-who $D action
        then
    then
    3 -177.49 0.0 89.35 $12 18 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 3 9 char-zone-bits? if
                hewie-zone-var 3 var-set
                hewie-zone-var 18.0 hewie-look-zone
            then
        then
    then
    4 55.23 0.0 -184.17 $14 14 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 4 9 char-zone-bits? if
                hewie-zone-var 4 var-set
                hewie-zone-var 14.0 hewie-look-zone
            then
        then
    then
    5 -84.16 0.0 90.89 $12 28 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                hewie-zone-var 5 var-set
                hewie-zone-var 28.0 hewie-look-zone
            then
        then
    then
    6 sound-bank-loaded? if
        c2f-2-flag1 story-flag? not if
            $C0000002 6 -82.0 0.0 92.0 0 0 sound
        then
        $C0000001 6 -85.0 30.0 90.0 0 0 sound
    then
;

: castle-2f-2.phase2 ( -- )   \ 003FE9C0
    fiona-who 2 char-before-door? if
        c1f-8-flag1 story-flag? found-file-209 story-flag? not and stalker-who char-here? not and if
            2 unhurried offer-action
        then
        found-file-209 story-flag? -2147483646 scene-request? and if
            5 always offer-action
        then
    then
    fiona-who $11 $2D char-faces-area? if
        6 unhurried offer-action
    then
    fiona-who $12 $2D char-faces-area? if
        7 unhurried offer-action
    then
    fiona-who $13 $2D char-faces-area? if
        8 unhurried offer-action
    then
    c2f-2-flag2 story-flag? not fiona-who $16 char-in-area? and if
        1 unhurried offer-action
    then
    fiona-who $18 char-in-area? fiona-who $19 char-in-area? or fiona-who 0 $32 char-heading? and if
        $A unhurried offer-action
    then
    c2f-2-album-medallion-found story-flag? c2f-2-album-medallion story-flag? not and if
        2 2 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            $C calm offer-action
        then
    then
    got-bisque-doll story-flag? not if
        1 ebit? if
            5 6 castle-2f-2.cond02? if
                fiona-who 6 3 char-zone-bits? if
                    $F unhurried offer-action
                then
            then
        then
    then
;

: castle-2f-2.phase3 ( -- )   \ 003FEA60
    107.0 8.0 -164.5 85.8 8.3 -178.2 107.3 -25.5 -164.4 85.8 -25.5 -178.2 lights-doorway
    87.9 8.3 -176.8 50.5 8.3 -179.8 87.9 -25.5 -176.8 87.9 -25.5 -176.8 lights-doorway
;

: castle-2f-2.phase5 ( -- )   \ 003FEAD0
    stalker-who char-busy? if
        4 ebit? if
            stalker-who action-end
            0 summon-take
        then
    then
    found-file-209 story-flag? $11 door-locked? and if
        $11 door-unlock
    then
;

: castle-2f-2.act00 ( -- )   \ 003FEAF0
    1 self-scripted
    self-wait-done
    pull-free-anim 0 self-anim-blend
    found-file-209 story-flag-set
    world-held state-flag-clear
    0 0 player-camera
    camera-restart
    $F $41 fade
    wait-fade
    $209 item-give
    0 ebit-set
    stalkers-stay state-flag-clear
    $64 threat-raise
    self-wait-anim
    0 self-scripted
    self-idle-or-end
;

: castle-2f-2.act01 ( -- )   \ 0047AAC0
    self-wait-done
    6 message   \ "This box is barring my path. I can't get in. I'll bet I could somehow push it out of th..."
    wait-message
    self-idle-or-end
;

: castle-2f-2.act02 ( -- )   \ 003FEB30
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $11 self-through-door
    self-wait-done
    rattle-behind-anim self-anim
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    3 track action-end
    look-close-anim self-anim
    self-frames-reset
    $16 self-wait-frames
    $F $44 fade
    wait-fade
    world-held state-flag-set
    3 char-start
    4 char-start
    fiona-who 1 char-visible
    $80 exit-check
    self-idle-or-end
;

: castle-2f-2.act03 ( -- )   \ 003FEB70
    self-wait-done
    2 track action-end
    $F $54 fade
    wait-fade
    $FE $20 552 2 stalker-to-room
    stalker-who action-end
    stalker-who char-done
    fiona-who 1 char-no-shadow
    stalker-who 1 char-no-shadow
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    stalker-who 0 char-no-shadow
    stalker-who action-end
    stalker-who char-done
    fiona-who 2 char-file-load
    fiona-who char-file-use
    stalker-who char-activate
    stalker-who 0 stalker-command
    got-bisque-doll story-flag? not if
        castle-2f-2.cmd01
        1 ebit-set
    then
    stalker-who $30A -7.812 78.949 179 char-to-xz
    stalker-who 3 3 char-camera
    0 stalker-who 4 action
    fiona-who $2E9 -8.354 49.017 -10 char-to-xz
    4 4 player-camera
    camera-restart
    force-calm state-flag-clear
    1 room-motion 0 self-anim-blend
    c2f-2-flag1 story-flag-set
    world-held state-flag-clear
    5 0 0 music
    $F $91 fade
    3 room-motion self-anim
    self-wait-anim
    summoner-own-cooldown
    wait-fade
    $224 item-give
    self-idle-or-end
;

: castle-2f-2.act04 ( -- )   \ 003FEC80
    self-wait-done
    fiona-who self-look-at
    yield
    $2302 $A self-anim-blend
    self-wait-anim
    self-frames-reset
    begin
        $19 chance? if
            look-about-anim $A self-anim-blend
            self-wait-anim
        else $19 chance? if
            look-about-3-anim $A self-anim-blend
            self-wait-anim
        then then
        4 camera-mode? not 0 0 1 $C2 castle-2f-2.cond01? or if
            self-idle-or-end
        else
            yield
        then
    again
;

: castle-2f-2.act05 ( -- )   \ 003FECB8
    self-wait-done
    0 ebit? if
        0 message   \ "I-I have to get out of here!"
        wait-message
    else
        1 message   \ "I still sense a presence inside the room. Best to trust my instincts and not go in."
        wait-message
    then
    self-idle-or-end
;

: castle-2f-2.act06 ( -- )   \ 003FECD0
    self-wait-done
    c2f-2-flag4 story-flag? not if
        c2f-2-flag4 story-flag-set
        fiona-who seized-sound 5 char-sound
        startled-anim self-anim
        $5A threat-add
        1 $FF 8 rumble
        self-frames-reset
        self-wait-16
        2 message   \ "Is this... blood?"
        self-wait-anim
        -1 self-idle-anim
        self-frames-reset
        7 self-wait-frames
        wait-message
    else
        5 message   \ "Why is there blood here?"
        wait-message
    then
    self-idle-or-end
;

: castle-2f-2.act07 ( -- )   \ 003FED00
    self-wait-done
    look-low-anim self-anim
    self-frames-reset
    $28 self-wait-frames
    3 message   \ "Nothing unusual..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-2.act08 ( -- )   \ 003FED10
    self-wait-done
    $292 -173.0 90.0 -90 $FFFF 5 self-move-to
    self-wait-done
    1 20.0 -10.0 0.0 6.0 event-camera
    self-frames-reset
    $10 self-wait-frames
    look-at-anim self-anim
    self-frames-reset
    $37 self-wait-frames
    4 message   \ "A woman with wings protruding from her back. An angel, perhaps?"
    wait-message
    self-wait-anim
    self-frames-reset
    self-wait-16
    0 0.0 0.0 0.0 0.0 event-camera
    self-idle-or-end
;

: castle-2f-2.act09 ( -- )   \ 003FED60
    0 180 var-set
    begin
        yield
        6 sound-bank-loaded? if
            0 180 var? if
                0 0 var-set
                $40000002 6 -82.0 0.0 92.0 0 0 sound
            else
                0 var-inc
            then
        then
    again
;

: castle-2f-2.act0A ( -- )   \ 003FEDA0
    self-wait-done
    0 self-turn-angle
    self-wait-done
    look-at-anim self-anim
    self-frames-reset
    $37 self-wait-frames
    7 message   \ "It's dark. I can't see a thing."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: castle-2f-2.act0B ( -- )   \ 003FEDB8
    c2f-2-flag5 story-flag-set
    begin
        castle-2f-2.cmd03
        yield
    again
;

: castle-2f-2.act0C ( -- )   \ 003FEDD0
    self-wait-done
    -11.6 142.2 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $71 message-param-room
        $71 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-2-album-medallion story-flag-set
            2 effect-remove
            $71 1 item-give-count
            0 $71 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        up-from-pick-anim self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-2.act0D ( -- )   \ 003FEE30
    4 ebit-set
    self-wait-done
    stalker-who 6 char-file-load
    $1A2 77.27 -192.48 0 $FFFF $A self-move-to
    self-wait-done
    stalker-who char-file-use
    1 self-noclip
    1 self-scripted
    0 room-motion 5 self-anim-blend8
    begin
        fiona-who self-touching? if
            stalker-who 0 fiona-thrown
        then
        self-at-motion-event? not while
        yield
    repeat
    0 self-noclip
    0 self-scripted
    4 ebit-clear
    stalker-who action-end
    0 summon-take
    self-idle-or-end
;

: castle-2f-2.act0E ( -- )   \ 003FEE70
    begin
        5 ebit? not if
            5 ebit-set
            $1E chance? if
                1 31 var-set
            else $32 chance? if
                1 61 var-set
            else
                1 1 var-set
            then then
        then
        yield
        1 var-dec
        1 0 var? 1 30 var? or 1 60 var? or 1 90 var? or if
            3 6 -145.0 10.0 150.0 0 0 sound
            1 0 var? if
                5 ebit-clear
            then
            $1E chance? if
                self-frames-reset
                $1E self-wait-frames
            else $32 chance? if
                self-frames-reset
                $2D self-wait-frames
            else
                self-frames-reset
                $5A self-wait-frames
            then then
        then
    again
;

: castle-2f-2.act0F ( -- )   \ 003FEEF0
    self-wait-done
    5 castle-2f-2.cmd05
    self-wait-done
    pick-up-anim self-anim
    self-wait-anim
    self-frames-reset
    4 self-wait-frames
    $93 message-param-room
    $93 $63 item-count? if
        $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
        wait-message
    else
        $93 1 item-give-count
        got-bisque-doll story-flag-set
        5 1 object-hidden
        0 $93 item-tab
        item-fanfare
        $8011 message   \ "You've obtained ."
        wait-message
    then
    up-from-pick-anim self-anim
    self-wait-anim
    self-idle-or-end
;

: castle-2f-2.act10 ( -- )   \ 003FEF40
    self-wait-done
    2 3 $FF char-load
    3 char-start
    0 0 0 $36B $3AF obstacle-place
    0 23.3 12.75 87.0 0 flame-in
    1 25.15 12.75 86.0 0 flame-in
    3 0 castle-2f-2.cmd02
    4 0 castle-2f-2.cmd02
    $1F $84 $60 $3C $50 $24 $2A $2A $46 0 9 effect-string
    fiona-who 1 char-no-shadow
    stalker-who 1 char-no-shadow
    1 4 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    panic-eased
    $FFFF message-prepare
    1 result? if
        scene-play
    then
    world-held state-flag-set
    fiona-who 0 char-no-shadow
    stalker-who 0 char-no-shadow
    3 0 char-remove
    $80 exit-check
    events-held state-flag-clear
    hewie-who action-end
    hewie-who char-done
    self-idle-or-end
;

: castle-2f-2.act11 ( -- )   \ 003FF018
    begin
        3 1 castle-2f-2.cmd02
        4 1 castle-2f-2.cmd02
        yield
    again
;

\ ---- registered ----
castle-2f-2 register-scripts   \ (its phases and actions, by their names)
