\ story/rooms/castle-2f-12.fs - the event scripts of room castle-2f-12 ($26; Belli Castle: 2F).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.castle-2f-12
USING: engine room-names story.words story.shared flag-names story-names floor-flags fiona.anims fiona.sounds stalker.anims ;

\ ---- this room's own event bits and script variables ----
0 constant caged-doll-remarked         \ (bit 0) its first remark made: "Why is there a doll... in a cage? Maybe someone is just tryi"
1 constant rocking-horse-remarked      \ (bit 1) its first remark made: "It's a child's rocking horse."
2 constant box-remarked                \ (bit 2) its first remark made: "It's a box, but I can't open it."
3 constant first-time                  \ (bit 3) the story flag not yet set (the first time here)
4 constant hiding-left                 \ (bit 4) she has come out of hiding (act09)
5 constant hewie-along                 \ (bit 5) Hewie came along with her (near, commandable)
6 constant hewie-in-place              \ (bit 6) Hewie at his place for the scene
7 constant came-while-hidden           \ (bit 7) the stalker came in while she was hiding
8 constant crank-turning               \ (bit 8) the crank turning (act11: cmd02 to 180)
9 constant has-both-items              \ (bit 9) she has both items ($86, $8A)
10 constant first-given                \ (bit 10) the first item just given
0 constant stage-var                   \ (var 0) ? its scene's stage (2: hiding over)
1 constant held-var                    \ (var 1) ? she is held (1)
2 constant search-result-var           \ (var 2) his search's outcome by chance: 0, 1 or 2
3 constant searched-var                \ (var 3) ? his search done (0 / 1)
4 constant crank-var                   \ (var 4) the crank's frames (to 180)

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
    1 keeps-hewie-off floor-on
    1 keeps-stalkers-off floor-on
    c2f-12-flag1 story-flag? if
        0 part-shown
        1 barred-floor floor-on
        0 1 object-hidden
        1 1 object-hidden
    else
        0 part-hidden
        1 barred-floor floor-off
        0 0 object-hidden
        1 0 object-hidden
    then
    0 castle-2f-12.cmd01
    0 1 track $13 action
    c2f-12-flag2 story-flag? not if
        2 castle-2f-12.cmd01
        c2f-12-flag2 story-flag-set
    then
    c2f-12-breakable-broken story-flag? not if
        0 keeps-people-off floor-on
        1 part-shown
        2 part-hidden
    else
        0 keeps-people-off floor-off
        1 part-hidden
        2 part-shown
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
    search-result-var 0 var-set
    6 0 pvar? if
        0 chance? if
            search-result-var 2 var-set
        then
    else 6 1 pvar? if
        $19 chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    else 6 2 pvar? if
        $32 chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    else 6 3 pvar? if
        $4B chance? if
            search-result-var 2 var-set
        else
            search-result-var 1 var-set
        then
    then then then then
    stalker-who 2 stalker-command
    2 self-is? 6 self-is? or 7 self-is? or if
    else search-result-var 1 var? if
        search-result-var 2 var-set
    then then
    2 creature-action? if
        search-result-var 2 var-set
    then
    4 stalker-alert? if
        search-result-var 0 var-set
    then
    search-result-var 0 var? if
        stalker-who 0 stalker-search-delay
        $78 1 summoner-wait
    else search-result-var 1 var? if
        stalker-who 150 stalker-search-delay
        0 stalker-who 4 action
    else search-result-var 2 var? if
        stalker-who 300 stalker-search-delay
        0 stalker-who 2 action
    then then then
    searched-var 0 var-set
    6 pvar-inc
    exit
;

: castle-2f-12.char-enter ( -- )   \ 00402E10
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        1 -1 player-camera
    then
    0 1 -1 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        1 -1 player-camera
    then
    1 1 -1 area-camera
    stalker-who self-is? if
        fiona-hidden state-flag? if
            came-while-hidden ebit-set
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
            came-while-hidden ebit-clear
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
                found-flag-var c2f-12-refined-antimony-found var-set
                taken-flag-var c2f-12-refined-antimony var-set
                sparkle-fx-var 2 var-set
                sparkle-x-var 22900 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var -9700 var-set
                hewie-zone-var 3 var-set
                0 hewie-who $8B action
            then
        then
    then
    crank-turning ebit? not if
        hewie-who char-here? hewie-who 2 char-cond? not and if
            4 2.09 0.0 -2.09 $14 5 0 zone
            0 game-mode? if
                hewie-can-command? castle-2f-12.cond00? and if
                    hewie-who 4 9 char-zone-bits? if
                        crank-var 0 var-set
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
            0 calm offer-action
        else
            $8016 examine-message   \ "I have to get away! I don't have time for this!"
        then
    then
    fiona-who 7 $3C char-faces-area? if
        6 unhurried offer-action
    then
    fiona-who 8 char-in-area? fiona-who 75 $32 char-heading? and if
        8 unhurried offer-action
    then
    fiona-who 9 $3C char-faces-area? if
        7 unhurried offer-action
    then
    fiona-who $A char-in-area? fiona-who 90 $32 char-heading? and if
        $A unhurried offer-action
    then
    fiona-who $B $3C char-faces-area? if
        $10 unhurried offer-action
    then
    c2f-12-refined-antimony-found story-flag? c2f-12-refined-antimony story-flag? not and if
        3 2 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            $F calm offer-action
        then
    then
    c2f-12-breakable-broken story-flag? not if
        2 -17.5 0.0 47.0 5 8 1 zone
        self-who 2 char-in-zone? if
            c2f-12-breakable-broken story-flag-set
            0 keeps-people-off floor-off
            1 part-hidden
            2 part-shown
            -17.5 0.0 47.0 0 -2143272896 0 0.0 shards-at
            $88 5 -17.5 0.0 47.0 0 0 sound
            $40 $48 noise
            1 -17.5 1.0 47.0 flicker-sprite
        then
    else c2f-12-medallion story-flag? not if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            $12 calm offer-action
        then
    then then
;

: castle-2f-12.act03 ( -- )   \ 00403350
    1 self-scripted
    [: stage-var 2 var? ;] wait-until
    counter-inc
    2 avoid-prompt
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    1 self-noclip
    3 room-motion 0 blend-out
    0 self-noclip
    c2f-12-flag1 story-flag-set
    1 barred-floor floor-on
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
    2 room-motion 0 self-anim-blend
    $10 wait-frames
    fiona-who 0 6 char-sound
    $38 wait-frames
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
    hiding-left ebit-set
    fiona-who camera-follow
    fiona-hidden state-flag-clear
    stalker-who char-here? stage-var 0 var? and if
        stalker-who 0 stalker-command
        stalker-who action-end
    then
    1 self-noclip
    0 4 object-anim
    1 4 object-anim
    4 room-motion 0 self-anim-blend
    7 wait-frames
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
    1 barred-floor floor-on
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act00 ( -- )   \ 004030A0
    stalkers-stay state-flag-set
    came-while-hidden ebit-clear
    1 self-scripted
    self-wait-done
    stage-var 0 var-set
    held-var 0 var-set
    c2f-12-flag1 story-flag? not if
        first-time ebit-set
    else
        first-time ebit-clear
    then
    0 counter-set
    hewie-along ebit-clear
    hewie-in-place ebit-clear
    hiding-left ebit-clear
    hewie-who char-busy? not hewie-near-command? and fiona-who hewie-who 30 chars-within? and if
        0 hewie-who 1 action-force
        hewie-along ebit-set
        hewie-who 5 char-file-load
    then
    fiona-who 4 char-file-load
    stalker-who 6 char-file-load
    $B4 16.729 33.985 45 $FFFF 5 self-move-to
    self-wait-done
    fiona-who char-file-use
    hewie-along ebit? if
        hewie-who self-look-at
        yield
        [: hewie-in-place ebit? ;] wait-until
    then
    counter-inc
    1 self-noclip
    self-who self-look-at
    yield
    first-time ebit? not if
        0 room-motion 5 self-anim-blend8
        5 wait-frames
        0 part-hidden
        0 0 object-hidden
        1 0 object-hidden
        0 0 object-anim
        1 0 object-anim
        $28 wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    else
        1 room-motion 5 self-anim-blend8
        5 wait-frames
        0 part-hidden
        0 0 object-hidden
        1 0 object-hidden
        0 1 object-anim
        1 1 object-anim
        $E wait-frames
        fiona-who 0 6 char-sound
        $3C wait-frames
        fiona-who 1 6 char-sound
        self-wait-anim
    then
    0 self-noclip
    1 barred-floor floor-off
    stalkers-stay state-flag-clear
    fiona-hidden state-flag-set
    0 avoid-prompt
    begin
        0 2 pad? not held-var 1 var? or if
            stage-var 1 var? if
                held-var 0 var-set
                fiona-who seized-sound 5 char-sound
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
            came-while-hidden ebit? stalker-who char-here? not and if
                came-while-hidden ebit-clear
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
            2 room-motion 0 self-anim-blend
            $10 wait-frames
            fiona-who 0 6 char-sound
            $38 wait-frames
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
            1 barred-floor floor-off
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
    hewie-in-place ebit-set
    1 wait-counter
    1 self-noclip
    first-time ebit? not if
        0 room-motion 5 self-anim-blend8
        self-wait-anim
    else
        1 room-motion 5 self-anim-blend8
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
                hiding-left ebit? if
                    4 room-motion 0 self-anim-blend
                    7 wait-frames
                    fiona-who 0 6 char-sound
                    self-wait-anim
                else stage-var 0 var? if
                    2 room-motion 0 blend-out
                else
                    3 room-motion 0 blend-out
                then then
                0 self-noclip
                0 self-scripted
                self-idle-or-end
            then
        else
            hewie-hidden state-flag-clear
            1 self-noclip
            hiding-left ebit? if
                4 room-motion 0 self-anim-blend
                7 wait-frames
                fiona-who 0 6 char-sound
                self-wait-anim
            else stage-var 0 var? if
                2 room-motion 0 blend-out
            else
                3 room-motion 0 blend-out
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
    searched-var 0 var? if
        $B4 15.0 29.0 45 $FFFF 5 self-move-to
        self-wait-done
    then
    searched-var 0 var-set
    1 self-scripted
    stage-var 1 var-set
    2 self-is? 6 self-is? or 7 self-is? or if
        0 3 object-anim
        1 3 object-anim
        1 room-motion $A self-anim-blend
        $C wait-frames
        stalker-who 0 6 char-sound
        4 wait-frames
        stage-var 2 var-set
        self-wait-anim
        step-back-anim play-out
        0 self-scripted
    else
        there-anim play-out
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
    [: fiona-who char-busy? ;] wait-while
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
    searched-var 1 var-set
    1 self-scripted
    held-var 1 var-set
    2 room-motion $A self-anim-blend
    $1E wait-frames
    stalker-who $28 5 char-sound
    $1E threat-raise
    $19 wait-frames
    stalker-who $28 5 char-sound
    $14 threat-raise
    $17 wait-frames
    stalker-who $28 5 char-sound
    $A threat-raise
    self-wait-anim
    held-var 0 var-set
    searched-var 0 var-set
    step-back-anim play-out
    0 self-scripted
    $78 1 summoner-wait
    self-idle-or-end
;

: castle-2f-12.act06 ( -- )   \ 00403450
    self-wait-done
    23.3 4.0 self-turn-to-xz
    self-wait-done
    look-close-anim self-anim
    $28 wait-frames
    caged-doll-remarked ebit? not if
        3 say   \ "Why is there a doll... in a cage? Maybe someone is just trying to keep one of their..."
        caged-doll-remarked ebit-set
    else
        4 say   \ "The doll is so real, it looks like it could move at any second."
    then
    self-wait-anim
    self-idle-or-end
;

: castle-2f-12.act07 ( -- )   \ 0047AB28
    self-wait-done
    5 say   \ "There is a doll slumped in the chair. Perhaps this is the nursery?"
    self-idle-or-end
;

: castle-2f-12.act08 ( -- )   \ 00403480
    self-wait-done
    rocking-horse-remarked ebit? not if
        6 say   \ "It's a child's rocking horse."
        rocking-horse-remarked ebit-set
    else
        7 say   \ "Even if I wanted to revisit my childhood with a quick ride, I don't think this will..."
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
        box-remarked ebit? not if
            0 say   \ "It's a box, but I can't open it."
            box-remarked ebit-set
        else
            1 say   \ "There is a slot here. Don't tell me this is going to be like the last one...?"
        then
    else
        2 say   \ "The box is open."
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
        has-both-items ebit-clear
    else
        has-both-items ebit-set
    then
    self-wait-done
    fiona-who $35 0.0 -31.5 180 char-to-xz
    1 7.0 30.0 0.0 0.0 event-camera
    world-frozen state-flag-set
    4 6 0.0 5.0 -39.0 0 0 sound
    $10 wait-frames
    has-both-items ebit? if
        0 0.0 3.0 -39.0 flicker-sprite
    then
    self-frames-reset
    fiona-who 3 6 char-sound
    begin
        $1E frames? not while
        1 castle-2f-12.cmd00
        yield
    repeat
    16 wait-frames
    has-both-items ebit? if
        first-given ebit-clear
        $86 1 item-count? not if
            $86 message-param-room
            $86 1 item-give-count
            0 $86 item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
            first-given ebit-set
        then
        $8A 1 item-count? not if
            first-given ebit? if
                16 wait-frames
            then
            $8A message-param-room
            $8A 1 item-give-count
            0 $8A item-tab
            item-fanfare
            $8012 say   \ "You've obtained ."
        then
        0 effect-remove
    else
        $A say   \ "Just my luck... it's empty."
    then
    got-jade-choker story-flag-set
    4 wait-frames
    world-frozen state-flag-clear
    stalkers-stay state-flag-clear
    0 0.0 0.0 0.0 0.0 event-camera
    0 self-scripted
    self-idle-or-end
;

: castle-2f-12.act0C ( -- )   \ 00403660
    self-wait-done
    4 6 0.0 5.0 -39.0 0 0 sound
    $1E wait-frames
    8 say   \ "Nothing happened. Nothing at all."
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
    look-about-3-anim play-out
    stalker-who 3 stalker-command
    c2f-12-flag3 story-flag-set
    self-idle-or-end
;

: castle-2f-12.act0F ( -- )   \ 004037A0
    self-wait-done
    22.9 -9.7 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $94 message-param-room
        $94 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c2f-12-refined-antimony story-flag-set
            2 effect-remove
            $94 1 item-give-count
            0 $94 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: castle-2f-12.act10 ( -- )   \ 00403800
    self-wait-done
    -19.0 -19.0 self-turn-to-xz
    self-wait-done
    9 say   \ "It's a crib for a small child. The sheets have become discolored from lack of use. I gu..."
    self-idle-or-end
;

: castle-2f-12.act11 ( -- )   \ 00403810
    self-wait-done
    2.0 -2.0 self-turn-to-xz
    self-wait-done
    crank-turning ebit-set
    [: castle-2f-12.cmd02 crank-var var-inc crank-var 180 var? ;] wait-until
    self-idle-or-end
;

: castle-2f-12.act12 ( -- )   \ 00403830
    self-wait-done
    -17.5 47.0 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            c2f-12-medallion story-flag-set
            1 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
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
castle-2f-12 register-scripts   \ (its phases and actions, by their names)
