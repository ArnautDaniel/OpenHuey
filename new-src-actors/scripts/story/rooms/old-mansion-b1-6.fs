\ story/rooms/old-mansion-b1-6.fs - the event scripts of room old-mansion-b1-6 ($66; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-6
USING: engine actors messages room-names story.words story.shared flag-names ;

\ Room66_Cmd00
: old-mansion-b1-6.cmd00 ( b0 -- )   \ (character 8 wading, moved by its animation - the cast does that; as a
    \ foot comes down, a step: 4 wading on (event bit 1), else 3; event bits 5 / 6 keep which
    \ feet are down. b0 0 its move stopped: the cast's own. The mud's puffs: not drawn yet)
    0= if  exit  then
    8 char-model dup 0< if  drop exit  then
    dup 1 foot-down?  swap 0 foot-down?  {: l r :}
    l 5 ebit? 0= and  r 6 ebit? 0= and  or if  8  1 ebit? if  4  else  3  then  6 char-sound  then
    l if  5 ebit-set  else  5 ebit-clear  then
    r if  6 ebit-set  else  6 ebit-clear  then ;
\ Room66_Cmd01
: old-mansion-b1-6.cmd01 ( b0 b1 -- )   \ (puffs and a glow at spot b0: b1 0 started - kept in variable b0 + 3 - else
    \ dying down)
    swap {: k :}  0= if  k 3 +  k effects:steam  var-set  else  k 3 + effect-out  then ;
\ Room66_Cmd02
fvariable sink-y   \ (where character 8 is sinking to: its place moves a frame behind)
: old-mansion-b1-6.cmd02 ( b0 -- )   \ (character 8 sinks 0.1 a frame till below -25: a small fire at
    \ spot b0 as it starts - kept in variable 7 - put out below -24)
    {: spot | cs :}  8 who>cs to cs  cs 0< if  exit  then  cs char-here 0= if  exit  then
    cs char-at 4 + sf@ sink-y f!
    begin
        cs char-here 0= if  exit  then
        8 0e -0.1e 0e char-shift
        sink-y f@ 0.1e f-  fdup sink-y f!
        fdup -0.2e f> if  7  spot 0 effects:fire  var-set  then
        fdup -24e f< if  7 effect-out  then
        -25e f< 0= while  yield
    repeat ;
\ Room66_Cmd03
: old-mansion-b1-6.cmd03 ( b0 b1 -- )   \ (a big fire at spot b0: b1 0 lit - kept in variable 8 - else put out)
    0= if  8 swap 1 effects:fire  var-set  else  drop  8 effect-out  then ;
\ Room66_Cmd04
: old-mansion-b1-6.cmd04 ( -- )  9 $5A $44 $46 $52 $4B colour-pulse ;
\ the letters of script variables 0 and 1, eight in a row (byte i, as the original reads them)
: letter-at ( i -- c )  dup 4 < if  0 var @  else  4 -  1 var @  then  swap 8 * rshift $FF and ;
\ Room66_Cond00
: old-mansion-b1-6.cond00? ( b0 -- flag )   \ (Room66_Letters: in the eight letters of vars 0 and 1 from the place in var 2, the next L / R)
    {: lr | at hit :}  2 var @ to at  0 to hit
    begin  at 8 <  if  at letter-at dup [char] L <> swap [char] R <> and  else  false  then  while  at 1+ to at  repeat
    at 8 < if  at letter-at  lr if  [char] R  else  [char] L  then  = if  at 1+ to at  -1 to hit  then  then
    2 at var-set  hit ;
\ Room66_Cond01
: old-mansion-b1-6.cond01? ( b0 b1 b2 b3 b4 -- flag )   \ (Room66_CharPast: its x (byte 3 0) or z at least be32 bytes 4..7 / 1000)
    >r >r >r >r  r> 24 lshift r> 16 lshift or r> 8 lshift or r> or  l>s s>f 1000e f/
    self-char char-at  swap if  8 +  then  sf@ f<= ;

: old-mansion-b1-6.enter ( -- )   \ 0041E110
    room-sounds
    0 1 $14 door-bits
    1 4 $20000 nav-group
    $5D story-flag? not if
        1 0 $20000 nav-group
        0 0 old-mansion-b1-6.cmd01
    then
    $5E story-flag? not if
        1 1 $20000 nav-group
        1 0 old-mansion-b1-6.cmd01
    then
    $5F story-flag? not if
        1 2 $20000 nav-group
        2 0 old-mansion-b1-6.cmd01
    then
    $60 story-flag? not if
        1 3 $20000 nav-group
        3 0 old-mansion-b1-6.cmd01
    then
    $271 story-flag? not if
        0 2.06 6.5 -99.68 flicker-sprite
    then
    $2C3 story-flag? not if
        2 39.98 1.0 -40.67 flicker-sprite
    then
    $2A9 story-flag? $2AA story-flag? not and if
        1 -138.94 1.0 -114.31 flicker-sprite
    then
    3 117.5 11.7 -21.8 0 flame-in
    4 -217.0 20.1 20.0 1 flame-in
    5 -175.0 20.1 4.7 1 flame-in
    6 -70.0 20.1 -124.7 1 flame-in
    7 -20.0 20.1 -96.7 1 flame-in
    8 11.7 20.1 -55.8 1 flame-in
    9 43.0 20.1 -26.5 1 flame-in
    $A -33.2 20.1 -6.2 1 flame-in
    $B 89.3 20.1 50.0 1 flame-in
    $C -12.4 28.3 24.6 1 flame-in
    $D -40.0 20.1 95.6 1 flame-in
    $E -65.5 20.1 0.0 1 flame-in
    1 $2300 sound-volume
;

: old-mansion-b1-6.char-enter ( -- )   \ 0041E260
    hewie-controlled? not if
        fiona-who self-is? 0 exit-taken? and if
            fiona-who 0 char-to-exit
            6 4 player-camera
        then
    else hewie-who self-is? 0 exit-taken? and if
        hewie-who 0 char-to-exit
        6 4 player-camera
    then then
    0 6 4 area-camera
    hewie-controlled? not if
        fiona-who self-is? 1 exit-taken? and if
            fiona-who 1 char-to-exit
            5 3 player-camera
        then
    else hewie-who self-is? 1 exit-taken? and if
        hewie-who 1 char-to-exit
        5 3 player-camera
    then then
    1 5 3 area-camera
    hewie-controlled? not if
        fiona-who self-is? 2 exit-taken? and if
            fiona-who 2 char-to-exit
            4 2 player-camera
        then
    else hewie-who self-is? 2 exit-taken? and if
        hewie-who 2 char-to-exit
        4 2 player-camera
    then then
    2 4 2 area-camera
    hewie-controlled? not if
        fiona-who self-is? 3 exit-taken? and if
            fiona-who 3 char-to-exit
            2 0 player-camera
        then
    else hewie-who self-is? 3 exit-taken? and if
        hewie-who 3 char-to-exit
        2 0 player-camera
    then then
    3 2 0 area-camera
;

: old-mansion-b1-6.phase1 ( -- )   \ 0041E360
    0 exit-usable? if
        0 exit-check
    then
    hewie-controlled? not if
        fiona-who 1 char-in-area? fiona-who char-busy? not and if
            1 exit-check
        then
    else hewie-who 1 char-in-area? hewie-who char-busy? not and if
        1 exit-check
    then then
    hewie-controlled? not if
        fiona-who 2 char-in-area? fiona-who char-busy? not and if
            2 exit-check
        then
    else hewie-who 2 char-in-area? hewie-who char-busy? not and if
        2 exit-check
    then then
    hewie-controlled? not if
        fiona-who 3 char-in-area? fiona-who char-busy? not and if
            3 exit-check
        then
    else hewie-who 3 char-in-area? hewie-who char-busy? not and if
        3 exit-check
    then then
    8 0 -1 1 chars-area-camera
    9 1 -1 1 chars-area-camera
    $A 0 -1 1 chars-area-camera
    $B 2 0 1 chars-area-camera
    $C 1 -1 1 chars-area-camera
    $D 3 1 1 chars-area-camera
    $E 3 1 1 chars-area-camera
    $F 4 2 1 chars-area-camera
    $10 3 1 1 chars-area-camera
    $11 5 3 1 chars-area-camera
    $12 5 3 1 chars-area-camera
    $13 6 4 1 chars-area-camera
    fiona-who 4 char-entered-area? if
        0 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        1 exit-prepare
    then
    fiona-who 6 char-entered-area? if
        2 exit-prepare
    then
    fiona-who 7 char-entered-area? if
        3 exit-prepare
    then
    fiona-who 5 char-entered-area? if
        $C ebit-set
    then
    fiona-who 5 char-left-area? if
        $C ebit? not if
            3 0 char-remove
        then
        $C ebit-clear
    then
    fiona-who 4 char-entered-area? if
        $D ebit-set
    then
    fiona-who 4 char-left-area? if
        $D ebit? not if
            3 0 char-remove
            4 0 char-remove
            5 0 char-remove
        then
        $D ebit-clear
    then
    $C ebit? loader-done? not and if
        $1C 3 1 char-load
        $C ebit-clear
    then
    $D ebit? loader-done? not and if
        $14 3 $FF char-load
        $15 4 $FF char-load
        $16 5 $FF char-load
        $D ebit-clear
    then
    fiona-who $11 char-entered-area? if
        8 ebit-set
    then
    fiona-who $11 char-left-area? if
        8 ebit-clear
    then
    fiona-who 4 char-entered-area? if
        9 ebit-set
    then
    fiona-who 4 char-left-area? if
        9 ebit-clear
    then
    7 ebit? not if
        6 sound-bank-loaded? if
            $5D story-flag? not if
                $40000007 6 -185.0 10.0 20.0 0 0 sound
            else $5E story-flag? not if
                $40000007 6 30.0 10.0 -42.0 0 0 sound
            then then
            $5F story-flag? not if
                $40000008 6 -20.0 10.0 10.0 0 0 sound
            else $60 story-flag? not if
                $40000008 6 -40.0 10.0 -110.0 0 0 sound
            then then
            7 ebit-set
        then
    else
        $5D story-flag? not $5E story-flag? not or if
            8 ebit? $5D story-flag? not and $5E story-flag? or if
                $C0000007 6 -185.0 10.0 20.0 0 0 sound
            then
            8 ebit? not $5E story-flag? not and $5D story-flag? or if
                $C0000007 6 30.0 10.0 -42.0 0 0 sound
            then
        then
        $5F story-flag? not $60 story-flag? not or if
            9 ebit? not $5F story-flag? not and $60 story-flag? or if
                $C0000008 6 -20.0 10.0 10.0 0 0 sound
            then
            9 ebit? $60 story-flag? not and $5F story-flag? or if
                $C0000008 6 -40.0 10.0 -110.0 0 0 sound
            then
        then
    then
    $5D story-flag? not if
        fiona-who $1C char-entered-area? $A ebit? not and if
            panic-eased
            0 fiona-who $F action-force
        then
    then
    $5E story-flag? not if
        fiona-who $1E char-entered-area? $A ebit? not and if
            panic-eased
            0 fiona-who $15 action-force
        then
    then
    $5F story-flag? not if
        fiona-who $20 char-entered-area? $A ebit? not and if
            panic-eased
            0 fiona-who $16 action-force
        then
    then
    $60 story-flag? not if
        fiona-who $22 char-entered-area? $A ebit? not and if
            panic-eased
            0 fiona-who $17 action-force
        then
    then
    $2A9 story-flag? not if
        2 -138.94 0.0 -114.31 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                $19 681 var-set
                $1A 682 var-set
                $1B 1 var-set
                $1C -138940 var-set
                $1D 1000 var-set
                $1E -114310 var-set
                $1F 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    0 144.66 0.0 -5.88 $12 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                $1F 0 var-set
                $1F 12.0 hewie-look-zone
            then
        then
    then
    4 73.86 0.0 -40.16 $C 10 0 zone
    4 ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 4 9 char-zone-bits? if
                    4 ebit-set
                    $1E chance? if
                        $1F 4 var-set
                        0 hewie-who $89 action
                    then
                then
            then
        then
    then
    5 73.86 0.0 -40.16 $19 20 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 5 9 char-zone-bits? if
                $1F 5 var-set
                $1F 20.0 hewie-look-zone
            then
        then
    then
    6 3 8 -4 0 zone-at-effect
    fiona-who 6 3 char-zone-bits? fiona-who 6 3 char-zone-bits-before? not and if
        fiona-who 3 char-effect-moving
    then
    hewie-who 6 3 char-zone-bits? hewie-who 6 3 char-zone-bits-before? not and if
        hewie-who 3 char-effect-moving
    then
    stalker-who 6 3 char-zone-bits? stalker-who 6 3 char-zone-bits-before? not and if
        stalker-who 3 char-effect-moving
    then
;

: old-mansion-b1-6.phase2 ( -- )   \ 0041E700
    fiona-who $14 $32 char-faces-area? if
        5 8 0 scene-change
    then
    fiona-who $15 $32 char-faces-area? if
        5 $B 0 scene-change
    then
    fiona-who $16 char-in-area? fiona-who 75 -40 $32 char-faces-xz? and if
        5 $C 0 scene-change
    then
    fiona-who $1B char-in-area? if
        5 $E 0 scene-change
    then
    fiona-who $1D char-in-area? if
        5 $12 0 scene-change
    then
    fiona-who $1F char-in-area? if
        5 $13 0 scene-change
    then
    fiona-who $21 char-in-area? if
        5 $14 0 scene-change
    then
    $271 story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            5 $18 4 scene-change
        then
    then
    $2A9 story-flag? $2AA story-flag? not and if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            5 $19 4 scene-change
        then
    then
    $2C3 story-flag? not if
        3 2 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            5 $1A 4 scene-change
        then
    then
;

: old-mansion-b1-6.act05 ( -- )   \ 0041EB40
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        0 $FF $FF $15 $A0 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    $60 story-flag? not if
        3 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5F story-flag? if
        8 6 sound-stop
    then
    $60 story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    3 old-mansion-b1-6.cmd02
    $60 story-flag? not if
        $76 door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $60 story-flag-set
    0 3 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act04 ( -- )   \ 0041EA90
    9 ebit-set
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 $FF $FE $52 $50 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $13 char-entered-area? if
            8 6 4 char-camera
        then
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act05 goto
    then
    1 self-noclip
    begin
        1 $FF $FD $DD $20 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    3 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        1 $FF $FD $B6 $10 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    3 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act03 ( -- )   \ 0041E9A0
    8 ebit-set
    8 3 6 char-sound
    8 $401 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FD $DD $20 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $11 char-entered-area? if
            8 5 3 char-camera
        then
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act04 goto
    then
    1 self-noclip
    begin
        0 $FF $FD $67 $F0 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    $5D story-flag? not if
        0 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5E story-flag? if
        7 6 sound-stop
    then
    $5D story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    0 old-mansion-b1-6.cmd02
    $5D story-flag? not if
        $5E door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5D story-flag-set
    0 0 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act07 ( -- )   \ 0041ECE0
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        1 $FF $FF $EC $78 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $F char-entered-area? if
            8 4 2 char-camera
        then
        yield
    repeat
    $5F story-flag? not if
        2 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $60 story-flag? if
        8 6 sound-stop
    then
    $5F story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    2 old-mansion-b1-6.cmd02
    $5F story-flag? not if
        $7B door-unlock
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5F story-flag-set
    0 2 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act06 ( -- )   \ 0041EC00
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FF $B1 $E0 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act07 goto
    then
    1 self-noclip
    begin
        0 0 0 $3A $98 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    $5E story-flag? not if
        1 1 old-mansion-b1-6.cmd01
    then
    8 3 6 char-sound
    8 0 1 8 char-anim-hold
    self-frames-reset
    8 self-wait-frames
    $5D story-flag? if
        7 6 sound-stop
    then
    $5E story-flag? not if
        8 9 6 char-sound
    then
    8 $A 6 char-sound
    1 old-mansion-b1-6.cmd02
    $5E story-flag? not if
        6 $86 0.0 0.0 0.0 0 0 sound
        self-frames-reset
        $5A self-wait-frames
    then
    $5E story-flag-set
    0 1 $20000 nav-group
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act02 ( -- )   \ 0041E8D0
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 0 0 $4E $20 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    0 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act03 goto
    then
    begin
        1 $FF $FF $63 $C0 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act06 goto
    then
    1 self-noclip
    begin
        1 $FF $FE $EE $90 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    2 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        1 $FF $FE $C7 $80 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    2 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act01 ( -- )   \ 0041E830
    8 3 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FE $C7 $80 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act02 goto
    then
    1 self-noclip
    begin
        0 $FF $FE $79 $60 old-mansion-b1-6.cond01? while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    1 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        0 $FF $FE $52 $50 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act00 ( -- )   \ 0041E7A0
    2 0 var-set
    0 old-mansion-b1-6.cmd00
    begin
        1 0 1 $38 $80 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $D char-entered-area? if
            8 3 1 char-camera
        then
        yield
    repeat
    1 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act01 goto
    then
    1 self-noclip
    begin
        1 0 1 $86 $A0 old-mansion-b1-6.cond01? not while
        2 ebit? if
            8 $201 1 $A char-anim-hold
            2 ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    0 0 old-mansion-b1-6.cmd03
    8 $A 6 char-sound
    begin
        1 0 1 $AD $B0 old-mansion-b1-6.cond01? not while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    0 1 old-mansion-b1-6.cmd03
    3 ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act08 ( -- )   \ 0041EDA0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    plates-under-10? if
        0 ebit? not if
            $11 message   \ "It's a plate pressing machine."
            wait-message
            0 ebit-set
        then
        $20 story-flag-clear
        2 subscreen-open
        begin
            subscreen-wanted state-flag? while
            yield
        repeat
        yield
        $20 story-flag? if
            $3F message-param-room
            0 $3F item-tab
            item-fanfare
            $8012 message   \ "You've obtained ."
            wait-message
        then
        $20 story-flag-set
    else 0 ebit? not if
        $F message   \ "There's a plate pressing machine here. There are no remaining plates currently so..."
        wait-message
        0 ebit-set
    else
        $12 message   \ "There are no remaining plates left. I'll have to get rid of some plate-keys befor..."
        wait-message
    then then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act09 ( -- )   \ 0047ACD0
    self-wait-done
    $10 message   \ "Returned a plate to the plate pressing machine."
    wait-message
    self-idle-or-end
;

: old-mansion-b1-6.act0A ( -- )   \ 0041EE10
    stalkers-stay state-flag-set
    1 self-scripted
    1 ebit-clear
    2 ebit-clear
    3 ebit-clear
    5 ebit-set
    6 ebit-set
    $F $44 fade
    begin
        loader-done? while
        yield
    repeat
    8 3 $FF char-load
    self-wait-done
    $5A story-flag? not if
        1 1 movie-play
        0 cutscene-start
        yield
        yield
        scene-wait-ready
        3 char-start
        wait-fade
        0 0 $14 door-bits
        0 4 $20000 nav-group
        hewie-who char-here? if
            0 hewie-who $10 action-force
        then
        $28 $78 movie-param
        panic-eased
        $FFFF message-prepare
        1 result? if
            scene-play
        then
        wait-fade
        panic-held state-flag-set
        0 self-idle-anim
        fiona-who $56 83.405 -17.137 -28 char-to-xz
        8 $126 74.6 13.8 0 char-to-xz
        8 3 1 char-camera
        $5A story-flag-set
    else
        wait-fade
        panic-eased
        panic-held state-flag-set
        hewie-who char-here? if
            0 hewie-who $10 action-force
        then
        3 char-start
        0 0 $14 door-bits
        0 4 $20000 nav-group
        fiona-who $5E 92.522 -37.951 -62 char-to-xz
        8 $196 75.0 -40.0 0 char-to-xz
        8 1 -1 char-camera
    then
    8 char-activate
    8 $200 1 0 char-anim-hold
    8 camera-follow
    camera-restart
    0 8 0 action-force
    6 message   \ "? Button: Run"
    $F 1 fade
    wait-fade
    begin
        3 ebit? not while
        0 2 pad? 1 ebit? not and if
            6 message-close
            1 ebit-set
            2 ebit-set
        then
        yield
    repeat
    $F 4 fade
    wait-fade
    panic-held state-flag-clear
    6 message-close
    hewie-who char-busy? if
        hewie-who action-end
    then
    8 action-end
    8 char-done
    3 0 char-remove
    0 1 $14 door-bits
    1 4 $20000 nav-group
    fiona-who camera-follow
    camera-restart
    $F $41 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act0B ( -- )   \ 0041EF90
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    7 message   \ "A corpse lies on the altar clutching a stone tablet in a death grip."
    wait-message
    $F 6 fade
    wait-fade
    0 message   \ "The golem sacrifice must be offered to the ever-burning wall of Hades that hinders your..."
    wait-message
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act0C ( -- )   \ 0041EFB0
    self-wait-done
    $A00 self-anim
    self-frames-reset
    self-wait-16
    5 message   \ "I wonder if this is the same type of golem statue that I saw in the main castle? Look!..."
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act0D ( -- )   \ 0047ACD8
    self-wait-done
    2 message
    wait-message
    self-idle-or-end
;

: old-mansion-b1-6.act0E ( -- )   \ 0041EFC0
    self-wait-done
    -170.0 20.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act0F ( -- )   \ 0041EFE0
    $A ebit-set
    1 self-scripted
    self-wait-done
    fiona-who -45 $3C char-heading? if
        $F00 self-anim
    else fiona-who 45 $3C char-heading? if
        $F01 self-anim
    else fiona-who 0 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    stalker-who char-here? not if
        self-frames-reset
        self-wait-16
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act10 ( -- )   \ 0041F020
    begin
        hewie-who char-busy? not while
        yield
    repeat
    self-who 1 char-visible
    begin
        yield
    again
;

: old-mansion-b1-6.act11 ( -- )   \ 0041F030
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    $F $44 fade
    begin
        loader-done? while
        yield
    repeat
    8 3 $FF char-load
    3 char-start
    $37 1.0 1 bgm
    wait-fade
    no-pause state-flag-set
    panic-eased
    panic-held state-flag-set
    fiona-who $5D 75.0 -30.0 180 char-to-xz
    1 30.0 -10.0 0.0 4.0 event-camera
    self-who 1 char-visible
    0 0 $14 door-bits
    8 char-activate
    0 4 $20000 nav-group
    8 $196 75.0 -40.0 0 char-to-xz
    1 4 $20000 nav-group
    begin
        0 adx? not while
        yield
    repeat
    $F 1 fade
    8 $1300 0 0 char-anim-hold
    9 0 var-set
    old-mansion-b1-6.cmd04
    self-frames-reset
    $A self-wait-frames
    0 0.0 $FF bgm
    begin
        8 char-at-motion-event? not while
        old-mansion-b1-6.cmd04
        yield
    repeat
    $F 0 fade
    wait-fade
    $1F $7C $74 $78 $5A $44 $46 $52 $4B 0 9 effect-string
    0 1 $14 door-bits
    8 action-end
    8 char-done
    3 0 char-remove
    self-who 0 char-visible
    0 0.0 0.0 0.0 0.0 event-camera
    panic-held state-flag-clear
    no-pause state-flag-clear
    $F $41 fade
    wait-fade
    $B ebit? not if
        $1D01 self-anim
        8 message   \ "...... That got my mind off things. I feel a little better now."
        wait-message
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        3 avoid-prompt
        fiona-who $84 5 char-sound
        $B ebit-set
        self-frames-reset
        $1E self-wait-frames
    else
        9 message   \ "...... Just staring at this all day isn't going to get me out of this place any soo..."
        wait-message
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act12 ( -- )   \ 0041F140
    self-wait-done
    12.0 -41.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act13 ( -- )   \ 0041F160
    self-wait-done
    -19.0 -6.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act14 ( -- )   \ 0041F180
    self-wait-done
    -60.0 -110.0 self-turn-to-xz
    self-wait-done
    $A01 self-anim
    self-frames-reset
    self-wait-16
    1 message   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    wait-message
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act15 ( -- )   \ 0041F1A0
    $A ebit-set
    1 self-scripted
    self-wait-done
    fiona-who 45 $3C char-heading? if
        $F00 self-anim
    else fiona-who -45 $3C char-heading? if
        $F01 self-anim
    else fiona-who 90 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    stalker-who char-here? not if
        self-frames-reset
        self-wait-16
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act16 ( -- )   \ 0041F1E0
    $A ebit-set
    1 self-scripted
    self-wait-done
    fiona-who 0 $3C char-heading? if
        $F00 self-anim
    else fiona-who 90 $3C char-heading? if
        $F01 self-anim
    else fiona-who 45 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    stalker-who char-here? not if
        self-frames-reset
        self-wait-16
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act17 ( -- )   \ 0041F220
    $A ebit-set
    1 self-scripted
    self-wait-done
    fiona-who 45 $3C char-heading? if
        $F00 self-anim
    else fiona-who -45 $3C char-heading? if
        $F01 self-anim
    else fiona-who 90 $1E char-heading? if
        $1002 self-anim
    else
        $1003 self-anim
    then then then
    stalker-who char-here? not if
        self-frames-reset
        self-wait-16
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    self-frames-reset
    7 self-wait-frames
    0 self-scripted
    $A ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act18 ( -- )   \ 0041F260
    self-wait-done
    2.06 -99.68 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $902 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $271 story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $903 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-b1-6.act19 ( -- )   \ 0041F2C0
    self-wait-done
    -138.94 -114.31 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $2AA story-flag-set
            1 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-b1-6.act1A ( -- )   \ 0041F320
    self-wait-done
    39.98 -40.67 self-turn-to-xz
    self-wait-done
    $FF panic-stage? not if
        $900 self-anim
        self-wait-anim
        self-frames-reset
        4 self-wait-frames
        $75 message-param-room
        $75 $63 item-count? if
            $8010 message   \ "It's a . Unfortunately you can't carry anymore items."
            wait-message
        else
            $2C3 story-flag-set
            2 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8011 message   \ "You've obtained ."
            wait-message
        then
        $901 self-anim
        self-wait-anim
    then
    self-idle-or-end
;

: old-mansion-b1-6.act1B ( -- )   \ 0041F380
    self-wait-done
    0 0 old-mansion-b1-6.cmd01
    1 0 old-mansion-b1-6.cmd01
    2 0 old-mansion-b1-6.cmd01
    3 0 old-mansion-b1-6.cmd01
    3 117.5 11.7 -21.8 0 flame-in
    4 -217.0 20.1 20.0 1 flame-in
    5 -175.0 20.1 4.7 1 flame-in
    6 -70.0 20.1 -124.7 1 flame-in
    7 -20.0 20.1 -96.7 1 flame-in
    8 11.7 20.1 -55.8 1 flame-in
    9 43.0 20.1 -26.5 1 flame-in
    $A -33.2 20.1 -6.2 1 flame-in
    $B 89.3 20.1 50.0 1 flame-in
    $C -12.4 28.3 24.6 1 flame-in
    $D -40.0 20.1 95.6 1 flame-in
    $E -65.5 20.1 0.0 1 flame-in
    8 3 $FF char-load
    3 char-start
    1 1 movie-play
    0 cutscene-start
    yield
    yield
    scene-wait-ready
    $28 $78 movie-param
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

: old-mansion-b1-6.phase5 ( -- )   \ 0047ACC8
;

\ ---- registered ----
' old-mansion-b1-6.enter old-mansion-b1-6 0 room-script!
' old-mansion-b1-6.char-enter old-mansion-b1-6 6 room-script!
' old-mansion-b1-6.phase1 old-mansion-b1-6 1 room-script!
' old-mansion-b1-6.phase2 old-mansion-b1-6 2 room-script!
' old-mansion-b1-6.act00 old-mansion-b1-6 $00 action-script!
' old-mansion-b1-6.act01 old-mansion-b1-6 $01 action-script!
' old-mansion-b1-6.act02 old-mansion-b1-6 $02 action-script!
' old-mansion-b1-6.act03 old-mansion-b1-6 $03 action-script!
' old-mansion-b1-6.act04 old-mansion-b1-6 $04 action-script!
' old-mansion-b1-6.act05 old-mansion-b1-6 $05 action-script!
' old-mansion-b1-6.act06 old-mansion-b1-6 $06 action-script!
' old-mansion-b1-6.act07 old-mansion-b1-6 $07 action-script!
' old-mansion-b1-6.act08 old-mansion-b1-6 $08 action-script!
' old-mansion-b1-6.act09 old-mansion-b1-6 $09 action-script!
' old-mansion-b1-6.act0A old-mansion-b1-6 $0A action-script!
' old-mansion-b1-6.act0B old-mansion-b1-6 $0B action-script!
' old-mansion-b1-6.act0C old-mansion-b1-6 $0C action-script!
' old-mansion-b1-6.act0D old-mansion-b1-6 $0D action-script!
' old-mansion-b1-6.act0E old-mansion-b1-6 $0E action-script!
' old-mansion-b1-6.act0F old-mansion-b1-6 $0F action-script!
' old-mansion-b1-6.act10 old-mansion-b1-6 $10 action-script!
' old-mansion-b1-6.act11 old-mansion-b1-6 $11 action-script!
' old-mansion-b1-6.act12 old-mansion-b1-6 $12 action-script!
' old-mansion-b1-6.act13 old-mansion-b1-6 $13 action-script!
' old-mansion-b1-6.act14 old-mansion-b1-6 $14 action-script!
' old-mansion-b1-6.act15 old-mansion-b1-6 $15 action-script!
' old-mansion-b1-6.act16 old-mansion-b1-6 $16 action-script!
' old-mansion-b1-6.act17 old-mansion-b1-6 $17 action-script!
' old-mansion-b1-6.act18 old-mansion-b1-6 $18 action-script!
' old-mansion-b1-6.act19 old-mansion-b1-6 $19 action-script!
' old-mansion-b1-6.act1A old-mansion-b1-6 $1A action-script!
' old-mansion-b1-6.act1B old-mansion-b1-6 $1B action-script!
' old-mansion-b1-6.phase5 old-mansion-b1-6 5 room-script!
