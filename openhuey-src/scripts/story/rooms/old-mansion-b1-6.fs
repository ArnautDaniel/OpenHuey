\ story/rooms/old-mansion-b1-6.fs - the event scripts of room old-mansion-b1-6 ($66; Belli Castle: Old Mansion B1).
\ Converted from the game's bytecode (new-src's tools/events2forth.py) and renamed by
\ tools/story_convert.py, once: edit by hand. The words: docs/subsystems/story.md.
IN: story.rooms.old-mansion-b1-6
USING: engine actors messages room-names story.words story.shared flag-names story-names floor-flags fiona.anims ;

\ ---- this room's own event bits, script variables and sounds ----
0 constant plate-press-remarked        \ (bit 0) its first remark made: "It's a plate pressing machine."
1 constant running                     \ (bit 1) the figure running (its run animation, its footsteps' sound)
2 constant turned                      \ (bit 2) the figure turned round this frame
3 constant race-done                   \ (bit 3) the race done (its way open)
4 constant hewie-zone4-noticed         \ (bit 4) Hewie has reacted to zone 4 (once a visit)
5 constant left-foot-down              \ (bit 5) the figure's left foot down last frame
6 constant right-foot-down             \ (bit 6) the figure's right foot down last frame
7 constant sound-started               \ (bit 7) its sounds started (then kept up)
8 constant in-area17                   \ (bit 8) someone in the area (entered, not yet left)
9 constant in-area4                    \ (bit 9) someone in the area (entered, not yet left)
10 constant area28-reacted             \ (bit 10) her reaction on entering area $1C made
11 constant calmed-remarked            \ (bit 11) its first remark made: "...... That got my mind off things. I feel a little better n"
12 constant in-area5                   \ (bit 12) she is in area 5 (the figure kept while she is)
13 constant in-area4-2                 \ (bit 13) she is in area 4 (the figures kept while she is)
0 constant letters-var                 \ (var 0) the letters' first half (and the fire's effect)
1 constant letters2-var                \ (var 1) the letters' second half (and the fire's effect)
2 constant letter-at-var               \ (var 2) the letter reached
7 constant fire-var                    \ (var 7) the fire's effect
8 constant fire2-var                   \ (var 8) the second fire's effect
9 constant figure-var                  \ (var 9) ? the figure's dance
3 constant creature-sound              \ (snd 3) the creature (8)
7 constant room-loop-sound             \ (snd 7) its steady sound (started as it comes in, then kept up)
8 constant room-loop2-sound           \ (snd 8) its steady sound (started as it comes in, then kept up)
9 constant creature2-sound             \ (snd 9) the creature (8), before omb1-6-flag5
10 constant creature3-sound            \ (snd 10) the creature (8)
6 constant unlocked-sound              \ (snd 6) the door unlocked (heard plainly)

\ Room66_Cmd00
: old-mansion-b1-6.cmd00 ( b0 -- )   \ (character 8 wading, moved by its animation - the cast does that; as a
    \ foot comes down, a step: 4 wading on (event bit 1), else 3; event bits 5 / 6 keep which
    \ feet are down. b0 0 its move stopped: the cast's own. The mud's puffs: not drawn yet)
    0= if  exit  then
    8 char-model dup 0< if  drop exit  then
    dup 1 foot-down?  swap 0 foot-down?  {: l r :}
    l left-foot-down ebit? 0= and  r right-foot-down ebit? 0= and  or if  8  running ebit? if  4  else  3  then  6 char-sound  then
    l if  left-foot-down ebit-set  else  left-foot-down ebit-clear  then
    r if  right-foot-down ebit-set  else  right-foot-down ebit-clear  then ;
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
        fdup -0.2e f> if  7  spot letters-var effects:fire  var-set  then
        fdup -24e f< if  fire-var effect-out  then
        -25e f< 0= while  yield
    repeat ;
\ Room66_Cmd03
: old-mansion-b1-6.cmd03 ( b0 b1 -- )   \ (a big fire at spot b0: b1 0 lit - kept in variable 8 - else put out)
    0= if  8 swap letters2-var effects:fire  var-set  else  drop  fire2-var effect-out  then ;
\ Room66_Cmd04
: old-mansion-b1-6.cmd04 ( -- )  9 $5A $44 $46 $52 $4B colour-pulse ;
\ the letters of script variables 0 and 1, eight in a row (byte i, as the original reads them)
: letter-at ( i -- c )  dup 4 < if  letters-var var @  else  4 -  letters2-var var @  then  swap 8 * rshift $FF and ;
\ Room66_Cond00
: old-mansion-b1-6.cond00? ( b0 -- flag )   \ (Room66_Letters: in the eight letters of vars 0 and 1 from the place in var 2, the next L / R)
    {: lr | at hit :}  letter-at-var var @ to at  0 to hit
    begin  at 8 <  if  at letter-at dup [char] L <> swap [char] R <> and  else  false  then  while  at 1+ to at  repeat
    at 8 < if  at letter-at  lr if  [char] R  else  [char] L  then  = if  at 1+ to at  -1 to hit  then  then
    letter-at-var at var-set  hit ;
\ Room66_Cond01
: old-mansion-b1-6.cond01? ( b0 b1 b2 b3 b4 -- flag )   \ (Room66_CharPast: its x (byte 3 0) or z at least be32 bytes 4..7 / 1000)
    >r >r >r >r  r> 24 lshift r> 16 lshift or r> 8 lshift or r> or  l>s s>f 1000e f/
    self-char char-at  swap if  8 +  then  sf@ f<= ;

: old-mansion-b1-6.enter ( -- )   \ 0041E110
    room-sounds
    0 part-shown
    4 barred-floor floor-on
    omb1-6-flag2 story-flag? not if
        0 barred-floor floor-on
        0 0 old-mansion-b1-6.cmd01
    then
    omb1-6-flag3 story-flag? not if
        1 barred-floor floor-on
        1 0 old-mansion-b1-6.cmd01
    then
    omb1-6-flag4 story-flag? not if
        2 barred-floor floor-on
        2 0 old-mansion-b1-6.cmd01
    then
    omb1-6-flag5 story-flag? not if
        3 barred-floor floor-on
        3 0 old-mansion-b1-6.cmd01
    then
    omb1-6-medallion story-flag? not if
        0 2.06 6.5 -99.68 flicker-sprite
    then
    omb1-6-magist-medallion story-flag? not if
        2 39.98 1.0 -40.67 flicker-sprite
    then
    omb1-6-viola-medallion-found story-flag? omb1-6-viola-medallion story-flag? not and if
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
    player-who self-is? 0 exit-taken? and if
        player-who 0 char-to-exit
        6 4 player-camera
    then
    0 6 4 area-camera
    player-who self-is? 1 exit-taken? and if
        player-who 1 char-to-exit
        5 3 player-camera
    then
    1 5 3 area-camera
    player-who self-is? 2 exit-taken? and if
        player-who 2 char-to-exit
        4 2 player-camera
    then
    2 4 2 area-camera
    player-who self-is? 3 exit-taken? and if
        player-who 3 char-to-exit
        2 0 player-camera
    then
    3 2 0 area-camera
;

: old-mansion-b1-6.phase1 ( -- )   \ 0041E360
    0 exit-usable? if
        0 exit-check
    then
    player-who 1 char-in-area? player-who char-busy? not and if
        1 exit-check
    then
    player-who 2 char-in-area? player-who char-busy? not and if
        2 exit-check
    then
    player-who 3 char-in-area? player-who char-busy? not and if
        3 exit-check
    then
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
        in-area5 ebit-set
    then
    fiona-who 5 char-left-area? if
        in-area5 ebit? not if
            3 0 char-remove
        then
        in-area5 ebit-clear
    then
    fiona-who 4 char-entered-area? if
        in-area4-2 ebit-set
    then
    fiona-who 4 char-left-area? if
        in-area4-2 ebit? not if
            3 0 char-remove
            4 0 char-remove
            5 0 char-remove
        then
        in-area4-2 ebit-clear
    then
    in-area5 ebit? loader-busy? not and if
        $1C 3 1 char-load
        in-area5 ebit-clear
    then
    in-area4-2 ebit? loader-busy? not and if
        $14 3 $FF char-load
        $15 4 $FF char-load
        $16 5 $FF char-load
        in-area4-2 ebit-clear
    then
    fiona-who $11 char-entered-area? if
        in-area17 ebit-set
    then
    fiona-who $11 char-left-area? if
        in-area17 ebit-clear
    then
    fiona-who 4 char-entered-area? if
        in-area4 ebit-set
    then
    fiona-who 4 char-left-area? if
        in-area4 ebit-clear
    then
    sound-started ebit? not if
        6 sound-bank-loaded? if
            omb1-6-flag2 story-flag? not if
                room-loop-sound volume-scaled 6 -185.0 10.0 20.0 0 0 sound
            else omb1-6-flag3 story-flag? not if
                room-loop-sound volume-scaled 6 30.0 10.0 -42.0 0 0 sound
            then then
            omb1-6-flag4 story-flag? not if
                room-loop2-sound volume-scaled 6 -20.0 10.0 10.0 0 0 sound
            else omb1-6-flag5 story-flag? not if
                room-loop2-sound volume-scaled 6 -40.0 10.0 -110.0 0 0 sound
            then then
            sound-started ebit-set
        then
    else
        omb1-6-flag2 story-flag? not omb1-6-flag3 story-flag? not or if
            in-area17 ebit? omb1-6-flag2 story-flag? not and omb1-6-flag3 story-flag? or if
                room-loop-sound kept-going volume-scaled 6 -185.0 10.0 20.0 0 0 sound
            then
            in-area17 ebit? not omb1-6-flag3 story-flag? not and omb1-6-flag2 story-flag? or if
                room-loop-sound kept-going volume-scaled 6 30.0 10.0 -42.0 0 0 sound
            then
        then
        omb1-6-flag4 story-flag? not omb1-6-flag5 story-flag? not or if
            in-area4 ebit? not omb1-6-flag4 story-flag? not and omb1-6-flag5 story-flag? or if
                room-loop2-sound kept-going volume-scaled 6 -20.0 10.0 10.0 0 0 sound
            then
            in-area4 ebit? omb1-6-flag5 story-flag? not and omb1-6-flag4 story-flag? or if
                room-loop2-sound kept-going volume-scaled 6 -40.0 10.0 -110.0 0 0 sound
            then
        then
    then
    omb1-6-flag2 story-flag? not if
        fiona-who $1C char-entered-area? area28-reacted ebit? not and if
            panic-eased
            0 fiona-who $F action-force
        then
    then
    omb1-6-flag3 story-flag? not if
        fiona-who $1E char-entered-area? area28-reacted ebit? not and if
            panic-eased
            0 fiona-who $15 action-force
        then
    then
    omb1-6-flag4 story-flag? not if
        fiona-who $20 char-entered-area? area28-reacted ebit? not and if
            panic-eased
            0 fiona-who $16 action-force
        then
    then
    omb1-6-flag5 story-flag? not if
        fiona-who $22 char-entered-area? area28-reacted ebit? not and if
            panic-eased
            0 fiona-who $17 action-force
        then
    then
    omb1-6-viola-medallion-found story-flag? not if
        2 -138.94 0.0 -114.31 $A 5 0 zone
        hewie-who 2 3 char-zone-bits? if
            19 hewie-action? 100 hewie-action? or if
                found-flag-var omb1-6-viola-medallion-found var-set
                taken-flag-var omb1-6-viola-medallion var-set
                sparkle-fx-var 1 var-set
                sparkle-x-var -138940 var-set
                sparkle-y-var 1000 var-set
                sparkle-z-var -114310 var-set
                hewie-zone-var 2 var-set
                0 hewie-who $8B action
            then
        then
    then
    0 144.66 0.0 -5.88 $12 12 0 zone
    hewie-who char-here? 0 game-mode? and if
        hewie-can-command? if
            hewie-who 0 9 char-zone-bits? if
                hewie-zone-var 0 var-set
                hewie-zone-var 12.0 hewie-look-zone
            then
        then
    then
    4 73.86 0.0 -40.16 $C 10 0 zone
    hewie-zone4-noticed ebit? not hewie-who char-here? and hewie-who 0 char-cond? and if
        2 game-mode? not if
            hewie-can-command? if
                hewie-who 4 9 char-zone-bits? if
                    hewie-zone4-noticed ebit-set
                    $1E chance? if
                        hewie-zone-var 4 var-set
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
                hewie-zone-var 5 var-set
                hewie-zone-var 20.0 hewie-look-zone
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
        8 unhurried offer-action
    then
    fiona-who $15 $32 char-faces-area? if
        $B unhurried offer-action
    then
    fiona-who $16 char-in-area? fiona-who 75 -40 $32 char-faces-xz? and if
        $C unhurried offer-action
    then
    fiona-who $1B char-in-area? if
        $E unhurried offer-action
    then
    fiona-who $1D char-in-area? if
        $12 unhurried offer-action
    then
    fiona-who $1F char-in-area? if
        $13 unhurried offer-action
    then
    fiona-who $21 char-in-area? if
        $14 unhurried offer-action
    then
    omb1-6-medallion story-flag? not if
        1 0 5 5 0 zone-at-effect
        fiona-who 1 3 char-zone-bits? if
            $18 calm offer-action
        then
    then
    omb1-6-viola-medallion-found story-flag? omb1-6-viola-medallion story-flag? not and if
        2 1 5 5 0 zone-at-effect
        fiona-who 2 3 char-zone-bits? if
            $19 calm offer-action
        then
    then
    omb1-6-magist-medallion story-flag? not if
        3 2 5 5 0 zone-at-effect
        fiona-who 3 3 char-zone-bits? if
            $1A calm offer-action
        then
    then
;

: old-mansion-b1-6.act05 ( -- )   \ 0041EB40
    8 creature-sound 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        0 $FF $FF $15 $A0 old-mansion-b1-6.cond01? not while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    omb1-6-flag5 story-flag? not if
        3 1 old-mansion-b1-6.cmd01
    then
    8 creature-sound 6 char-sound
    8 0 1 8 char-anim-hold
    8 wait-frames
    omb1-6-flag4 story-flag? if
        room-loop2-sound 6 sound-stop
    then
    omb1-6-flag5 story-flag? not if
        8 creature2-sound 6 char-sound
    then
    8 creature3-sound 6 char-sound
    3 old-mansion-b1-6.cmd02
    omb1-6-flag5 story-flag? not if
        $76 door-unlock
        unlocked-sound $86 0.0 0.0 0.0 0 0 sound
        $5A wait-frames
    then
    omb1-6-flag5 story-flag-set
    3 barred-floor floor-off
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act04 ( -- )   \ 0041EA90
    in-area4 ebit-set
    8 creature-sound 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 $FF $FE $52 $50 old-mansion-b1-6.cond01? while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
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
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    3 0 old-mansion-b1-6.cmd03
    8 creature3-sound 6 char-sound
    begin
        1 $FF $FD $B6 $10 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    3 1 old-mansion-b1-6.cmd03
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act03 ( -- )   \ 0041E9A0
    in-area17 ebit-set
    8 creature-sound 6 char-sound
    8 $401 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FD $DD $20 old-mansion-b1-6.cond01? while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
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
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    omb1-6-flag2 story-flag? not if
        0 1 old-mansion-b1-6.cmd01
    then
    8 creature-sound 6 char-sound
    8 0 1 8 char-anim-hold
    8 wait-frames
    omb1-6-flag3 story-flag? if
        room-loop-sound 6 sound-stop
    then
    omb1-6-flag2 story-flag? not if
        8 creature2-sound 6 char-sound
    then
    8 creature3-sound 6 char-sound
    0 old-mansion-b1-6.cmd02
    omb1-6-flag2 story-flag? not if
        $5E door-unlock
        unlocked-sound $86 0.0 0.0 0.0 0 0 sound
        $5A wait-frames
    then
    omb1-6-flag2 story-flag-set
    0 barred-floor floor-off
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act07 ( -- )   \ 0041ECE0
    8 creature-sound 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    1 self-noclip
    begin
        1 $FF $FF $EC $78 old-mansion-b1-6.cond01? not while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        8 $F char-entered-area? if
            8 4 2 char-camera
        then
        yield
    repeat
    omb1-6-flag4 story-flag? not if
        2 1 old-mansion-b1-6.cmd01
    then
    8 creature-sound 6 char-sound
    8 0 1 8 char-anim-hold
    8 wait-frames
    omb1-6-flag5 story-flag? if
        room-loop2-sound 6 sound-stop
    then
    omb1-6-flag4 story-flag? not if
        8 creature2-sound 6 char-sound
    then
    8 creature3-sound 6 char-sound
    2 old-mansion-b1-6.cmd02
    omb1-6-flag4 story-flag? not if
        $7B door-unlock
        unlocked-sound $86 0.0 0.0 0.0 0 0 sound
        $5A wait-frames
    then
    omb1-6-flag4 story-flag-set
    2 barred-floor floor-off
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act06 ( -- )   \ 0041EC00
    8 creature-sound 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FF $B1 $E0 old-mansion-b1-6.cond01? not while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
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
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    omb1-6-flag3 story-flag? not if
        1 1 old-mansion-b1-6.cmd01
    then
    8 creature-sound 6 char-sound
    8 0 1 8 char-anim-hold
    8 wait-frames
    omb1-6-flag2 story-flag? if
        room-loop-sound 6 sound-stop
    then
    omb1-6-flag3 story-flag? not if
        8 creature2-sound 6 char-sound
    then
    8 creature3-sound 6 char-sound
    1 old-mansion-b1-6.cmd02
    omb1-6-flag3 story-flag? not if
        unlocked-sound $86 0.0 0.0 0.0 0 0 sound
        $5A wait-frames
    then
    omb1-6-flag3 story-flag-set
    1 barred-floor floor-off
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act02 ( -- )   \ 0041E8D0
    8 creature-sound 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        1 0 0 $4E $20 old-mansion-b1-6.cond01? while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        1 old-mansion-b1-6.cmd00
        yield
    repeat
    0 old-mansion-b1-6.cond00? if
        ['] old-mansion-b1-6.act03 goto
    then
    begin
        1 $FF $FF $63 $C0 old-mansion-b1-6.cond01? while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
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
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    2 0 old-mansion-b1-6.cmd03
    8 creature3-sound 6 char-sound
    begin
        1 $FF $FE $C7 $80 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    2 1 old-mansion-b1-6.cmd03
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act01 ( -- )   \ 0041E830
    8 creature-sound 6 char-sound
    8 $400 0 $A char-anim-hold
    begin
        self-at-motion-event? not while
        3 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? not if
        8 $200 1 4 char-anim-hold
    else
        8 $201 1 4 char-anim-hold
    then
    begin
        0 $FF $FE $C7 $80 old-mansion-b1-6.cond01? while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
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
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    1 0 old-mansion-b1-6.cmd03
    8 creature3-sound 6 char-sound
    begin
        0 $FF $FE $52 $50 old-mansion-b1-6.cond01? while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    1 1 old-mansion-b1-6.cmd03
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act00 ( -- )   \ 0041E7A0
    letter-at-var 0 var-set
    0 old-mansion-b1-6.cmd00
    begin
        1 0 1 $38 $80 old-mansion-b1-6.cond01? not while
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
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
        turned ebit? if
            8 $201 1 $A char-anim-hold
            turned ebit-clear
        then
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    running ebit? if
        8 $200 1 8 char-anim-hold
    then
    6 message-close
    0 0 old-mansion-b1-6.cmd03
    8 creature3-sound 6 char-sound
    begin
        1 0 1 $AD $B0 old-mansion-b1-6.cond01? not while
        2 old-mansion-b1-6.cmd00
        yield
    repeat
    0 1 old-mansion-b1-6.cmd03
    race-done ebit-set
    self-idle-or-end
;

: old-mansion-b1-6.act08 ( -- )   \ 0041EDA0
    stalkers-stay state-flag-set
    1 self-scripted
    self-wait-done
    plates-under-10? if
        plate-press-remarked ebit? not if
            $11 say   \ "It's a plate pressing machine."
            plate-press-remarked ebit-set
        then
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
    else plate-press-remarked ebit? not if
        $F say   \ "There's a plate pressing machine here. There are no remaining plates currently so..."
        plate-press-remarked ebit-set
    else
        $12 say   \ "There are no remaining plates left. I'll have to get rid of some plate-keys befor..."
    then then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act09 ( -- )   \ 0047ACD0
    self-wait-done
    $10 say   \ "Returned a plate to the plate pressing machine."
    self-idle-or-end
;

: old-mansion-b1-6.act0A ( -- )   \ 0041EE10
    stalkers-stay state-flag-set
    1 self-scripted
    running ebit-clear
    turned ebit-clear
    race-done ebit-clear
    left-foot-down ebit-set
    right-foot-down ebit-set
    $F $44 fade
    [: loader-busy? ;] wait-while
    8 3 $FF char-load
    self-wait-done
    omb1-6-flag1 story-flag? not if
        1 1 movie-play
        0 cutscene-start
        yield
        yield
        scene-wait-ready
        3 char-start
        wait-fade
        0 part-hidden
        4 barred-floor floor-off
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
        omb1-6-flag1 story-flag-set
    else
        wait-fade
        panic-eased
        panic-held state-flag-set
        hewie-who char-here? if
            0 hewie-who $10 action-force
        then
        3 char-start
        0 part-hidden
        4 barred-floor floor-off
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
        race-done ebit? not while
        0 2 pad? running ebit? not and if
            6 message-close
            running ebit-set
            turned ebit-set
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
    0 part-shown
    4 barred-floor floor-on
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
    7 say   \ "A corpse lies on the altar clutching a stone tablet in a death grip."
    $F 6 fade
    wait-fade
    0 say   \ "The golem sacrifice must be offered to the ever-burning wall of Hades that hinders your..."
    $F 7 fade
    wait-fade
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act0C ( -- )   \ 0041EFB0
    self-wait-done
    look-at-anim self-anim
    16 wait-frames
    5 say   \ "I wonder if this is the same type of golem statue that I saw in the main castle? Look!..."
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act0D ( -- )   \ 0047ACD8
    self-wait-done
    2 say
    self-idle-or-end
;

: old-mansion-b1-6.act0E ( -- )   \ 0041EFC0
    self-wait-done
    -170.0 20.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    1 say   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act0F ( -- )   \ 0041EFE0
    area28-reacted ebit-set
    1 self-scripted
    self-wait-done
    fiona-who -45 $3C char-heading? if
        startled-anim self-anim
    else fiona-who 45 $3C char-heading? if
        look-behind-anim self-anim
    else fiona-who 0 $1E char-heading? if
        stagger-anim self-anim
    else
        doubled-over-anim self-anim
    then then then
    stalker-who char-here? not if
        16 wait-frames
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    7 wait-frames
    0 self-scripted
    area28-reacted ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act10 ( -- )   \ 0041F020
    [: hewie-who char-busy? ;] wait-until
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
    [: loader-busy? ;] wait-while
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
    0 part-hidden
    8 char-activate
    4 barred-floor floor-off
    8 $196 75.0 -40.0 0 char-to-xz
    4 barred-floor floor-on
    [: 0 adx? ;] wait-until
    $F 1 fade
    8 $1300 0 0 char-anim-hold
    figure-var 0 var-set
    old-mansion-b1-6.cmd04
    $A wait-frames
    0 0.0 $FF bgm
    begin
        8 char-at-motion-event? not while
        old-mansion-b1-6.cmd04
        yield
    repeat
    $F 0 fade
    wait-fade
    $1F $7C $74 $78 $5A $44 $46 $52 $4B 0 9 effect-string
    0 part-shown
    8 action-end
    8 char-done
    3 0 char-remove
    self-who 0 char-visible
    0 0.0 0.0 0.0 0.0 event-camera
    panic-held state-flag-clear
    no-pause state-flag-clear
    $F $41 fade
    wait-fade
    calmed-remarked ebit? not if
        hand-to-mouth-anim self-anim
        8 say   \ "...... That got my mind off things. I feel a little better now."
        self-wait-anim
        $E4 panic-calm-in
        fiona-calm-reset
        3 avoid-prompt
        fiona-who $84 5 char-sound
        calmed-remarked ebit-set
        $1E wait-frames
    else
        9 say   \ "...... Just staring at this all day isn't going to get me out of this place any soo..."
    then
    stalkers-stay state-flag-clear
    0 self-scripted
    self-idle-or-end
;

: old-mansion-b1-6.act12 ( -- )   \ 0041F140
    self-wait-done
    12.0 -41.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    1 say   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act13 ( -- )   \ 0041F160
    self-wait-done
    -19.0 -6.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    1 say   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act14 ( -- )   \ 0041F180
    self-wait-done
    -60.0 -110.0 self-turn-to-xz
    self-wait-done
    look-low-anim self-anim
    16 wait-frames
    1 say   \ "An unusual figure has been burned into the ground... It reads: 'METH'"
    self-wait-anim
    self-idle-or-end
;

: old-mansion-b1-6.act15 ( -- )   \ 0041F1A0
    area28-reacted ebit-set
    1 self-scripted
    self-wait-done
    fiona-who 45 $3C char-heading? if
        startled-anim self-anim
    else fiona-who -45 $3C char-heading? if
        look-behind-anim self-anim
    else fiona-who 90 $1E char-heading? if
        stagger-anim self-anim
    else
        doubled-over-anim self-anim
    then then then
    stalker-who char-here? not if
        16 wait-frames
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    7 wait-frames
    0 self-scripted
    area28-reacted ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act16 ( -- )   \ 0041F1E0
    area28-reacted ebit-set
    1 self-scripted
    self-wait-done
    fiona-who 0 $3C char-heading? if
        startled-anim self-anim
    else fiona-who 90 $3C char-heading? if
        look-behind-anim self-anim
    else fiona-who 45 $1E char-heading? if
        stagger-anim self-anim
    else
        doubled-over-anim self-anim
    then then then
    stalker-who char-here? not if
        16 wait-frames
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    7 wait-frames
    0 self-scripted
    area28-reacted ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act17 ( -- )   \ 0041F220
    area28-reacted ebit-set
    1 self-scripted
    self-wait-done
    fiona-who 45 $3C char-heading? if
        startled-anim self-anim
    else fiona-who -45 $3C char-heading? if
        look-behind-anim self-anim
    else fiona-who 90 $1E char-heading? if
        stagger-anim self-anim
    else
        doubled-over-anim self-anim
    then then then
    stalker-who char-here? not if
        16 wait-frames
        3 message   \ "A wall of flame is shooting up from the ground!"
    then
    self-wait-anim
    -1 self-idle-anim
    7 wait-frames
    0 self-scripted
    area28-reacted ebit-clear
    self-idle-or-end
;

: old-mansion-b1-6.act18 ( -- )   \ 0041F260
    self-wait-done
    2.06 -99.68 self-turn-to-xz
    self-wait-done
    in-panic? not if
        reach-out-anim play-out
        4 wait-frames
        $70 message-param-room
        $70 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-6-medallion story-flag-set
            0 effect-remove
            $70 1 item-give-count
            0 $70 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        reach-back-anim play-out
    then
    self-idle-or-end
;

: old-mansion-b1-6.act19 ( -- )   \ 0041F2C0
    self-wait-done
    -138.94 -114.31 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $72 message-param-room
        $72 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-6-viola-medallion story-flag-set
            1 effect-remove
            $72 1 item-give-count
            0 $72 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
    then
    self-idle-or-end
;

: old-mansion-b1-6.act1A ( -- )   \ 0041F320
    self-wait-done
    39.98 -40.67 self-turn-to-xz
    self-wait-done
    in-panic? not if
        pick-up-anim play-out
        4 wait-frames
        $75 message-param-room
        $75 $63 item-count? if
            $8010 say   \ "It's a . Unfortunately you can't carry anymore items."
        else
            omb1-6-magist-medallion story-flag-set
            2 effect-remove
            $75 1 item-give-count
            0 $75 item-tab
            item-fanfare
            $8011 say   \ "You've obtained ."
        then
        up-from-pick-anim play-out
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
old-mansion-b1-6 register-scripts   \ (its phases and actions, by their names)
