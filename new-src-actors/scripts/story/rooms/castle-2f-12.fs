\ story/rooms/castle-2f-12.fs - the event scripts of room castle-2f-12 ($26; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-12
USING: engine room-names story.words story.shared flag-names story-names ;

\ the two doors ("left", "right") opening: byte 3 0 at once (2.25), else a step (0.075)
: castle-2f-12.cmd00 ( b0 -- )   \ (the two doors opening: 0 at once - 2.25 - else a step of 0.075)
    if  0.075e  else  2.25e  then  fdup fnegate s" left" turn-y object+!  s" right" turn-y object+! ;
\ the three rocking chairs ("movechair_1..3"; +0x30 the rock's phase in degrees, +0x34 its size,
\ +0x38 how fast it dies down; +0x10 the tilt), by byte 3: 0 all still; 1 a rocking step (6
\ degrees; each swing smaller, the first chair creaking at a volume by its size); 2 / 3 set
fvariable tip  fvariable tip-yaw  fvariable reach
\ ---- the three rocking chairs ("movechair_1..3": +$30 the rock's phase in degrees, +$34 its size,
\ +$38 how fast it dies down; the tilt +$10) ----
: chair ( k -- addr len )  case  0 of  s" movechair_1"  endof  1 of  s" movechair_2"  endof  >r s" movechair_3" r>  endcase ;
: creak ( id vol -- )  6 swap 0  2.0866e 0.3e -2.0861e  sound ;   \ (chair_creak: at the first chair)
: chair-rock ( k -- )   \ (a step: on 6 degrees; each swing smaller, the first creaking by its size)
    {: k | a n :}  k chair to n to a  a n object-here? 0= if  exit  then
    a n $34 object@ f0> 0= if  exit  then
    6e a n $30 object+!
    a n $30 object@ 360e f< 0= if
        -360e a n $30 object+!  a n $38 object@ fnegate a n $34 object+!
        a n $34 object@ f0< if  0e a n $34 object!  then
        k 0= if  2  -100e 1e a n $34 object@ f- f* f>s  creak  then
    then
    a n $30 object@ deg>rad fsin a n $34 object@ f* 10e f* deg>rad
    fdup pi f> if  2e pi f* f-  then  a n turn-x object! ;
: chair-set ( back? k -- )   \ (rocking at full size from its tilt now, forward or back)
    {: back k | a n :}  k chair to n to a  a n object-here? 0= if  exit  then
    a n turn-x object@ rad>deg 10e f/ deg>rad fasin rad>deg             ( F: a )
    back if  180e fswap f-  then  a n $30 object!
    1e a n $34 object!  0.05e a n $38 object!  0 a n $3C object-n!  5 0 creak ;
\ rocking at full size from their tilt now (swinging forward / back), with a creak
: castle-2f-12.cmd01 ( b0 -- )   \ (0 all still; 1 a rocking step; 2 / 3 set rocking forward / back)
    case
        0 of  3 0 do  0 i chair $3C object-n!  0 i chair $38 object-n!  0 i chair $34 object-n!  0 i chair $30 object-n!  loop  endof
        1 of  3 0 do  i chair-rock  loop  endof
        2 of  3 0 do  false i chair-set  loop  endof
        3 of  3 0 do  true i chair-set  loop  endof
    endcase ;
\ the partner's target (+0xF35E0 on, +0xF35F0) 3 above the rocking chair (movechair_2): its seat
\ 6 ahead, tipped by its rock (90 x +0x34 x sin +0x30 degrees) and turned with it
: castle-2f-12.cmd02 ( -- )   \ (Hewie's look: 3 above the second chair's seat - 6 up its back, tipped by
    \ its rock (90 x size x sin phase degrees about x) and turned with it)
    s" movechair_2" object-here? 0= if  exit  then
    s" movechair_2" $30 object@ deg>rad fsin s" movechair_2" $34 object@ f* 90e f* deg>rad tip f!
    s" movechair_2" turn-y object@ tip-yaw f!
    tip f@ fsin 6e f* reach f!                                          \ (out along its heading)
    s" movechair_2" place-x object@  reach f@ tip-yaw f@ fsin f* f+
    s" movechair_2" place-y object@  tip f@ fcos 6e f* f+ 3e f+
    s" movechair_2" place-z object@  reach f@ tip-yaw f@ fcos f* f+  hewie-look-at ;
\ the first rocking chair still rocking (+0x34 over 0.3)
: castle-2f-12.cond00? ( -- flag )  s" movechair_1" $34 placed-f@ 0.3e f> ;

: castle-2f-12.enter ( -- )   \ 00402D50
    room-sounds
    1 1 $1000000 nav-group
    1 1 $4000000 nav-group
    c2f-12-flag1 story-flag? if
        0 1 $14 door-bits
        1 1 $20000 nav-group
        0 1 object-hidden
        1 1 object-hidden
    else
        0 0 $14 door-bits
        0 1 $20000 nav-group
        0 0 object-hidden
        1 0 object-hidden
    then
    0 castle-2f-12.cmd01
    0 $F1 $13 action
    c2f-12-flag2 story-flag? not if
        2 castle-2f-12.cmd01
        c2f-12-flag2 story-flag-set
    then
    c2f-12-breakable-broken story-flag? not if
        1 0 $8000000 nav-group
        1 1 $14 door-bits
        2 0 $14 door-bits
    else
        0 0 $8000000 nav-group
        1 0 $14 door-bits
        2 1 $14 door-bits
        c2f-12-medallion story-flag? not if
            1 -17.5 1.0 47.0 flicker-sprite
        then
    then
    0 4 0.812 0.687 0.187 0.312 zone-rect
    c2f-12-refined-antimony-found story-flag? c2f-12-refined-antimony story-flag? not and if
        2 22.9 1.0 -9.7 flicker-sprite
    then
    got-jade-choker story-flag? if
        0 castle-2f-12.cmd00
    then
;

: castle-2f-12.act0D ( -- )   \ 00403680
    2 self-is? 6 self-is? or 7 self-is? or if
        stalker-who char-file-use
    then
    stalker-who camera-follow
    2 0 var-set
    6 0 pvar? if
        0 chance? if
            2 2 var-set
        then
    else 6 1 pvar? if
        $19 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else 6 2 pvar? if
        $32 chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    else 6 3 pvar? if
        $4B chance? if
            2 2 var-set
        else
            2 1 var-set
        then
    then then then then
    stalker-who 2 stalker-command
    2 self-is? 6 self-is? or 7 self-is? or if
    else 2 1 var? if
        2 2 var-set
    then then
    2 creature-action? if
        2 2 var-set
    then
    4 stalker-alert? if
        2 0 var-set
    then
    2 0 var? if
        stalker-who 0 stalker-search-delay
        $78 1 summoner-wait
    else 2 1 var? if
        stalker-who 150 stalker-search-delay
        0 stalker-who 4 action
    else 2 2 var? if
        stalker-who 300 stalker-search-delay
        0 stalker-who 2 action
    then then then
    3 0 var-set
    6 pvar-inc
    exit
;

: castle-2f-12.char-enter ( -- )   \ 00402E10
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        1 -1 player-camera
    then then
    0 1 -1 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            1 -1 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        1 -1 player-camera
    then then
    1 1 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            7 ebit-set
            2 self-is? 6 self-is? or 7 self-is? or if
                c2f-12-flag3 story-flag? not castle-2f-12.cond00? and 2 creature-action? not and if
                    stalker-who camera-follow
                    0 stalker-who $E action
                else
                    castle-2f-12.act0D
                then
            else
                castle-2f-12.act0D
            then
        else
            7 ebit-clear
        then
    then
;

: castle-2f-12.phase1 ( -- )   \ 00402EC0
    0 exit-usable? if
        0 exit-check
    then
    1 exit-usable? if
        1 exit-check
    then
    4 1 -1 1 chars-area-camera
    5 0 0 1 chars-area-camera
    fiona-who 2 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 3 char-entered-area? if
        1 exit-prepare
    then
    0 -4.0 0.0 4.0 4 7 0 zone
    1 6.0 0.0 -7.0 3 7 0 zone
    c2f-12-refined-antimony-found story-flag? not if
        3 22.9 0.0 -9.7 $A 5 0 zone
        hewie-who 3 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 c2f-12-refined-antimony-found var-set
                $1A c2f-12-refined-antimony var-set
                $1B 2 var-set
                $1C 22900 var-set
                $1D 1000 var-set
                $1E -9700 var-set
                $1F 3 var-set
                0 hewie-who $8B action
            then
        then
    then
    8 ebit? not if
        hewie-who char-here? hewie-who 2 char-cond? not and if
            4 2.09 0.0 -2.09 $14 5 0 zone
            0 game-mode? if
                hewie-can-command? castle-2f-12.cond00? and if
                    hewie-who 4 9 char-zone-bits? if
                        4 0 var-set
                        0 hewie-who $11 action
                    then
                then
            then
        then
    then
;

: castle-2f-12.phase2 ( -- )   \ 00402FA0
    fiona-who 6 char-in-area? fiona-who 22 $3C char-heading? and if
        stalker-who char-here? not if
            5 0 5 scene-change
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    fiona-who 7 $3C char-faces-area? if
        5 6 0 scene-change
    then
    fiona-who 8 char-in-area? fiona-who 75 $32 char-heading? and if
        5 8 0 scene-change
    then
    fiona-who 9 $3C char-faces-area? if
        5 7 0 scene-change
    then
    fiona-who $A char-in-area? fiona-who 90 $32 char-heading? and if
        5 $A 0 scene-change
    then
    fiona-who $B $3C char-faces-area? if
        5 $10 0 scene-change
    then
    c2f-12-refined-antimony-found story-flag? c2f-12-refined-antimony story-flag? not and if
        3 2 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            5 $F 4 scene-change
        then
    then
    c2f-12-breakable-broken story-flag? not if
        2 -17.5 0.0 47.0 5 8 1 zone
        self-who 2 char-in-zone? if
            c2f-12-breakable-broken story-flag-set
            0 0 $8000000 nav-group
            1 0 $14 door-bits
            2 1 $14 door-bits
            -17.5 0.0 47.0 0 -2143272896 0 0.0 shards-at
            $88 5 -17.5 0.0 47.0 0 0 sound
            $40 $48 noise
            1 -17.5 1.0 47.0 flicker-sprite
        then
    else c2f-12-medallion story-flag? not if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            5 $12 4 scene-change
        then
    then then
;

: castle-2f-12.act03 ( -- )   \ 00403350
    1 self-scripted
    begin
        0 2 var? not while
        yield
    repeat
    counter-inc
    2 avoid-prompt
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    $8003 0 self-anim-blend
    self-wait-anim
    0 self-noclip
    c2f-12-flag1 story-flag-set
    1 1 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act05 ( -- )   \ 00403400
    1 self-scripted
    counter-inc
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    stalker-who char-here? if
        stalker-who 0 stalker-command
        stalker-who action-end
    then
    1 self-noclip
    0 2 object-anim
    1 2 object-anim
    $8002 0 self-anim-blend
    self-frames-reset
    $10 self-wait-frames
    fiona-who 0 6 char-sound
    self-frames-reset
    $38 self-wait-frames
    fiona-who 1 6 char-sound
    self-wait-anim
    0 self-noclip
    stalker-who char-here? if
        stalker-who self-look-at
        yield
        stalker-who self-turn-to
        self-wait-done
    then
    c2f-12-flag1 story-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act09 ( -- )   \ 004034A0
    1 self-scripted
    counter-inc
    4 ebit-set
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    stalker-who char-here? 0 0 var? and if
        stalker-who 0 stalker-command
        stalker-who action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    $8004 0 self-anim-blend
    self-frames-reset
    7 self-wait-frames
    fiona-who 0 6 char-sound
    self-wait-anim
    0 self-noclip
    stalker-who char-here? if
        stalker-who self-look-at
        yield
        stalker-who self-turn-to
        self-wait-done
    then
    c2f-12-flag1 story-flag-set
    1 1 $20000 nav-group
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act00 ( -- )   \ 004030A0
    stalkers-stay state-flag-set
    7 ebit-clear
    1 self-scripted
    self-wait-done
    0 0 var-set
    1 0 var-set
    c2f-12-flag1 story-flag? not if
        3 ebit-set
    else
        3 ebit-clear
    then
    0 counter-set
    5 ebit-clear
    6 ebit-clear
    4 ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who 1 action-force
        5 ebit-set
        hewie-who 5 char-file-load
    then
    fiona-who 4 char-file-load
    stalker-who 6 char-file-load
    $B4 16.729 33.985 45 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    5 ebit? if
        hewie-who self-look-at
        yield
        begin
            6 ebit? not while
            yield
        repeat
    then
    counter-inc
    1 self-noclip
    self-who self-look-at
    yield
    3 ebit? not if
        $8000 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-hidden
        1 0 object-hidden
        0 0 object-anim
        1 0 object-anim
        self-frames-reset
        $28 self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    else
        $8001 5 self-anim-blend8
        self-frames-reset
        5 self-wait-frames
        0 0 $14 door-bits
        0 0 object-hidden
        1 0 object-hidden
        0 1 object-anim
        1 1 object-anim
        self-frames-reset
        $E self-wait-frames
        fiona-who 0 6 char-sound
        self-frames-reset
        $3C self-wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    0 1 $20000 nav-group
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    begin
        0 2 pad? not 1 1 var? or if
            0 1 var? if
                1 0 var-set
                fiona-who $43 5 char-sound
                stalker-who char-here? if
                    ['] castle-2f-12.act03 goto
                else
                    ['] castle-2f-12.act05 goto
                then
            else in-panic? if
                ['] castle-2f-12.act09 goto
            then then
            2 panic-calm-in
            6 fiona-calm
            $1E fiona-recovery-lower
            7 ebit? stalker-who char-here? not and if
                7 ebit-clear
                1 avoid-prompt
            then
            yield
        else
            stalker-who char-here? fiona-who stalker-who 50 chars-within? and if
                ['] castle-2f-12.act09 goto
            then
            counter-inc
            fiona-who camera-follow
            fiona-hidden state-flag-clear
            stalker-who char-here? if
                stalker-who 0 stalker-command
                stalker-who action-end
            then
            1 self-noclip
            0 2 object-anim
            1 2 object-anim
            $8002 0 self-anim-blend
            self-frames-reset
            $10 self-wait-frames
            fiona-who 0 6 char-sound
            self-frames-reset
            $38 self-wait-frames
            fiona-who 1 6 char-sound
            self-wait-anim
            0 self-noclip
            stalker-who char-here? if
                stalker-who self-look-at
                yield
                stalker-who self-turn-to
                self-wait-done
            then
            c2f-12-flag1 story-flag-clear
            0 1 $20000 nav-group
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-12.act01 ( -- )   \ 00403230
    1 self-scripted
    self-wait-done
    $B4 17.237 27.889 45 $FFFF $A self-move-to
    self-wait-done
    hewie-who char-file-use
    6 ebit-set
    1 wait-counter
    1 self-noclip
    3 ebit? not if
        $8000 5 self-anim-blend8
        self-wait-anim
    else
        $8001 5 self-anim-blend8
        self-wait-anim
    then
    0 self-noclip
    hewie-hidden state-flag-set
    begin
        fiona-who char-busy? if
            2 counter? not if
                yield
            else
                hewie-hidden state-flag-clear
                1 self-noclip
                4 ebit? if
                    $8004 0 self-anim-blend
                    self-frames-reset
                    7 self-wait-frames
                    fiona-who 0 6 char-sound
                    self-wait-anim
                else 0 0 var? if
                    $8002 0 self-anim-blend
                    self-wait-anim
                else
                    $8003 0 self-anim-blend
                    self-wait-anim
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            4 ebit? if
                $8004 0 self-anim-blend
                self-frames-reset
                7 self-wait-frames
                fiona-who 0 6 char-sound
                self-wait-anim
            else 0 0 var? if
                $8002 0 self-anim-blend
                self-wait-anim
            else
                $8003 0 self-anim-blend
                self-wait-anim
            then then
            0 self-noclip
            0 self-scripted
            self-idle-or-end
        then
    again
;

: castle-2f-12.act02 ( -- )   \ 004032B0
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    3 0 var? if
        $B4 15.0 29.0 45 $FFFF 5 self-move-to
        self-wait-done
    then
    3 0 var-set
    1 self-scripted
    0 1 var-set
    2 self-is? 6 self-is? or 7 self-is? or if
        0 3 object-anim
        1 3 object-anim
        $8001 $A self-anim-blend
        self-frames-reset
        $C self-wait-frames
        stalker-who 0 6 char-sound
        self-frames-reset
        4 self-wait-frames
        0 2 var-set
        self-wait-anim
        $404 self-anim
        self-wait-anim
        0 self-scripted
    else
        $1601 self-anim
        self-wait-anim
        fiona-hidden state-flag? if
            2 avoid-prompt
            panic-eased
            0 fiona-who 9 action-force
            0 self-scripted
        else
            0 self-scripted
        then
    then
    10 self-idle-anim
    begin
        fiona-who char-busy? while
        yield
    repeat
    stalker-who 0 stalker-command
    self-idle-or-end
;

: castle-2f-12.act04 ( -- )   \ 00403380
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $B4 15.0 29.0 45 $FFFF 5 self-move-to
    self-wait-done
    3 1 var-set
    1 self-scripted
    1 1 var-set
    $8002 $A self-anim-blend
    self-frames-reset
    $1E self-wait-frames
    stalker-who $28 5 char-sound
    $1E threat-raise
    self-frames-reset
    $19 self-wait-frames
    stalker-who $28 5 char-sound
    $14 threat-raise
    self-frames-reset
    $17 self-wait-frames
    stalker-who $28 5 char-sound
    $A threat-raise
    self-wait-anim
    1 0 var-set
    3 0 var-set
    $404 self-anim
    self-wait-anim
    0 self-scripted
    $78 1 summoner-wait
    self-idle-or-end
;

: castle-2f-12.act06 ( -- )   \ 00403450
    self-wait-done
    23.3 4.0 self-turn-to-xz
    self-wait-done
    $A02 self-anim
    self-frames-reset
    $28 self-wait-frames
    0 ebit? not if
        3 message   \ "Why is there a doll... in a cage? Maybe someone is just trying to keep one of their..."
        wait-message
        0 ebit-set
    else
        4 message   \ "The doll is so real, it looks like it could move at any second."
        wait-message
    then
    self-wait-anim
    self-idle-or-end
;

: castle-2f-12.act07 ( -- )   \ 0047AB28
    self-wait-done
    5 message   \ "There is a doll slumped in the chair. Perhaps this is the nursery?"
    wait-message
    self-idle-or-end
;

: castle-2f-12.act08 ( -- )   \ 00403480
    self-wait-done
    1 ebit? not if
        6 message   \ "It's a child's rocking horse."
        wait-message
        1 ebit-set
    else
        7 message   \ "Even if I wanted to revisit my childhood with a quick ride, I don't think this will..."
        wait-message
    then
    self-idle-or-end
;

: castle-2f-12.act0A ( -- )   \ 004034F0
    self-wait-done
    $35 0.0 -31.5 180 $FFFF 5 self-move-to
    self-wait-done
    world-frozen state-flag-set
    1 self-scripted
    1 20.0 10.0 0.0 0.0 event-camera
    got-jade-choker story-flag? not if
        2 ebit? not if
            0 message   \ "It's a box, but I can't open it."
            wait-message
            2 ebit-set
        else
            1 message   \ "There is a slot here. Don't tell me this is going to be like the last one...?"
            wait-message
        then
    else
        2 message   \ "The box is open."
        wait-message
    then
    0 0.0 0.0 0.0 0.0 event-camera
    world-frozen state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act0B ( -- )   \ 00403550
    stalkers-stay state-flag-set
    1 self-scripted
    $86 1 item-count? $8A 1 item-count? and if
        9 ebit-clear
    else
        9 ebit-set
    then
    self-wait-done
    fiona-who $35 0.0 -31.5 180 char-to-xz
    1 7.0 30.0 0.0 0.0 event-camera
    world-frozen state-flag-set
    4 6 0.0 5.0 -39.0 0 0 sound
    self-frames-reset
    $10 self-wait-frames
    9 ebit? if
        0 0.0 3.0 -39.0 flicker-sprite
    then
    self-frames-reset
    fiona-who 3 6 char-sound
    begin
        $1E frames? not while
        1 castle-2f-12.cmd00
        yield
    repeat
    self-frames-reset
    self-wait-16
    9 ebit? if
        $A ebit-clear
        $86 1 item-count? not if
            $86 message-param-room
            $86 1 item-give-count
            0 $86 item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
            $A ebit-set
        then
        $8A 1 item-count? not if
            $A ebit? if
                self-frames-reset
                self-wait-16
            then
            $8A message-param-room
            $8A 1 item-give-count
            0 $8A item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        0 effect-remove
    else
        $A message   \ "Just my luck... it's empty."
        wait-message
    then
    got-jade-choker story-flag-set
    self-frames-reset
    4 self-wait-frames
    world-frozen state-flag-clear
    stalkers-stay state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act0C ( -- )   \ 00403660
    self-wait-done
    4 6 0.0 5.0 -39.0 0 0 sound
    self-frames-reset
    $1E self-wait-frames
    8 message   \ "Nothing happened. Nothing at all."
    wait-message
    self-idle-or-end
;

: castle-2f-12.act0E ( -- )   \ 00403760
    self-wait-done
    stalker-who 0 char-heading-for? 0 exit-door-open? not and if
        fiona-who 3 self-move-slot
        self-wait-done
    then
    stalker-who 1 char-heading-for? 1 exit-door-open? not and if
        hewie-who 3 self-move-slot
        self-wait-done
    then
    $3A 3.123 11.435 -165 $FFFF 5 self-move-to
    self-wait-done
    $1305 self-anim
    self-wait-anim
    stalker-who 3 stalker-command
    c2f-12-flag3 story-flag-set
    self-idle-or-end
;

: castle-2f-12.act0F ( -- )   \ 004037A0
    self-wait-done
    22.9 -9.7 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-12-refined-antimony story-flag-set
            2 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-12.act10 ( -- )   \ 00403800
    self-wait-done
    -19.0 -19.0 self-turn-to-xz
    self-wait-done
    9 message   \ "It's a crib for a small child. The sheets have become discolored from lack of use. I gu..."
    wait-message
    self-idle-or-end
;

: castle-2f-12.act11 ( -- )   \ 00403810
    self-wait-done
    2.0 -2.0 self-turn-to-xz
    self-wait-done
    8 ebit-set
    begin
        castle-2f-12.cmd02
        4 var-inc
        4 180 var? not while
        yield
    repeat
    self-idle-or-end
;

: castle-2f-12.act12 ( -- )   \ 00403830
    self-wait-done
    -17.5 47.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            c2f-12-medallion story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: castle-2f-12.act13 ( -- )   \ 00403890
    begin
        self-who 0 char-in-zone? if
            3 castle-2f-12.cmd01
        then
        self-who 1 char-in-zone? if
            2 castle-2f-12.cmd01
        then
        1 castle-2f-12.cmd01
        yield
    again
;

\ ---- registered ----
' castle-2f-12.enter castle-2f-12 0 room-script!
' castle-2f-12.char-enter castle-2f-12 6 room-script!
' castle-2f-12.phase1 castle-2f-12 1 room-script!
' castle-2f-12.phase2 castle-2f-12 2 room-script!
' castle-2f-12.act00 castle-2f-12 $00 action-script!
' castle-2f-12.act01 castle-2f-12 $01 action-script!
' castle-2f-12.act02 castle-2f-12 $02 action-script!
' castle-2f-12.act03 castle-2f-12 $03 action-script!
' castle-2f-12.act04 castle-2f-12 $04 action-script!
' castle-2f-12.act05 castle-2f-12 $05 action-script!
' castle-2f-12.act06 castle-2f-12 $06 action-script!
' castle-2f-12.act07 castle-2f-12 $07 action-script!
' castle-2f-12.act08 castle-2f-12 $08 action-script!
' castle-2f-12.act09 castle-2f-12 $09 action-script!
' castle-2f-12.act0A castle-2f-12 $0A action-script!
' castle-2f-12.act0B castle-2f-12 $0B action-script!
' castle-2f-12.act0C castle-2f-12 $0C action-script!
' castle-2f-12.act0D castle-2f-12 $0D action-script!
' castle-2f-12.act0E castle-2f-12 $0E action-script!
' castle-2f-12.act0F castle-2f-12 $0F action-script!
' castle-2f-12.act10 castle-2f-12 $10 action-script!
' castle-2f-12.act11 castle-2f-12 $11 action-script!
' castle-2f-12.act12 castle-2f-12 $12 action-script!
' castle-2f-12.act13 castle-2f-12 $13 action-script!
