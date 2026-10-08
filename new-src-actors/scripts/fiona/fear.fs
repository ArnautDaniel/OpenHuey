\ fiona/fear.fs - Fiona's fear and panic (Fiona_Panic, Fiona_FullPanic, Fiona_Exhausted, the
\ panic's part of Fiona_ControlCommand and its states): her fear meter (0..99, 100 a panic
\ attack); out of breath from the panic's stage 2, panicking at 4 with a scream others hear; the
\ panic run until she's out of breath; stumbling or falling as she runs into something; getting
\ up again. The panic itself is its own actor (panic.fs): she hears its stage and level.
IN: fiona.fear
USING: engine keys game-state actors messages common facts fiona.state fiona.model fiona.controls fiona.moving flag-names ;

defer to-idle   ' noop is to-idle      \ (fiona.fs)
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: panic-id ( -- id )  s" panic" actor-named ;
: noise-here ( loud -- )  room-id her-tri @ -1 fiona-noise acoustics-id send noise ;
: her-sound ( id vol -- )  5 swap 0 her-pos sound-at ;

\ ---- her fear meter (Fiona_ChangeFear; charms: with items) ----
: change-fear ( F: d -- )  her-fear f@ f+ 0e fmax 99e fmin her-fear f! ;
: fear-down ( -- )  her-danger @ 2 = if  -0.1e  else  -0.15e  then  change-fear ;   \ Fiona_FearDown

\ ---- out of breath, panicking (Fiona_Panic, Fiona_FullPanic, Fiona_Exhausted) ----
: full-panic ( -- )
    her-fear-bits @ 2 or her-fear-bits !  3e rnd f* f>s 30 * 120 + her-panic-t !  30 her-stumble-t !
    $6F noise-here ;
:noname ( -- )   \ (Fiona_Exhausted: out of breath from the panic run, 0x207; a new run length)
    3e rnd f* f>s 30 * 120 + her-panic-t !  $F her-doing !  $207 -1 play-table ; is exhausted
: feel-the-panic ( -- )   \ (each frame: the panic's stage taken up; her fear meter; her timers)
    her-panic-stage @
    her-fear-bits @ 1 and 0= if
        dup 2 >= if
            her-fear-bits @ 1 or her-fear-bits !
            dup case
                4 of  full-panic  endof
                3 of  4e rnd f* f>s 30 * 150 + her-panic-t !  endof
                2 of  5e rnd f* f>s 30 * 240 + her-panic-t !  endof
            endcase
        then
    else dup 4 < if
        dup 2 < if  1e her-tired-w f!  her-fear-bits @ 3 invert and her-fear-bits !
        else  her-fear-bits @ 2 invert and her-fear-bits !  then
    else  her-fear-bits @ 2 and 0= if  full-panic  then  then then
    drop
    her-mode @ 0=  her-fear-bits @ 2 and 0= and  her-recovery @ 0> and if  -1 her-recovery +!  then
    her-fear f@ 100e f< if
        her-mode @ 0=  her-doing @ $E <> and if
            group case
                3 of  endof
                2 of  her-danger @ 2 = if  0.0666667e  else  0.0466767e  then  change-fear  endof   \ running frightens
                1 of  fear-down  endof
                >r  her-fear-bits @ 2 and 0= if  fear-down  then  r>
            endcase
        else her-mode @ 3 = if  fear-down  then then
    then
    her-mode @ if  0 her-run-t !  then ;

\ ---- the panic attack (at full fear: 150 frames, her cry 0x44, animation 2) ----
: in-attack ( -- )
    -1 her-attack-t +!  her-attack-t @ 0= if  to-idle exit  then
    settled? anim@ 2 <> and if  $44 0 her-sound  2 -1 play-table  then
    root-move ;

\ ---- falling, sliding, getting up (Fiona_StatePanicFall, StateFallen, StateSpecialEntry) ----
create mouth 12 allot
: along-wall ( -- )   \ (Fiona_AlongWall: turned along the wall she stopped at, by her mouth's way out)
    her-model @ $13 bone-pos mouth vec!
    her-tri @ dup 0< if  drop exit  then
    her-at mouth blocked-floor v-wall 0= if  fdrop exit  then         ( F: yaw )
    fdup her-yaw f@ f- angle-wrap fabs 90e deg>rad f> if  pi f+ angle-wrap  then  her-heading f!
    her-heading f@ her-yaw f@ f- angle-wrap  fdup fabs 0.25e f* 0.5e deg>rad fmax        ( F: a step )
    her-wall-side @ -1 = if  fover f0< if  0  else  1  then  her-wall-side !  then
    fswap fdrop  her-wall-side @ if  her-yaw f@ f+  else  her-yaw f@ fswap f-  then  angle-wrap her-yaw f! ;
: getting-up ( -- )
    settled? if
        anim@ $B01 = if
            her-still @ 0= if  stick-heading her-heading f!  then
            her-fear-bits @ 1 and 0= if  $B02 -1 play-table  then
        else anim@ $B02 = ended? and if  to-idle exit  then then
    then
    root@  rm-z f@ f0> if
        her-yaw f@ her-heading f@ rm-z f@ 10e f* deg>rad turn-toward fdrop her-yaw f!
    then
    root-turn  her-yaw f@ her-heading f!
    rm-x f@ rm-z f@ her-yaw f@ rotate-by move-by ;
: fallen ( -- )
    ended? if  0 her-attack-t !  $B01 -1 play-table  $A her-mode !  $B her-doing !  ['] getting-up her-act ! exit  then
    her-fall-turn @ -1 = if  root-move
    else
        root@ rm-x f@ rm-z f@ her-fall-yaw f@ rotate-by move-by
        her-fall-turn @ 0= if
            her-yaw f@ her-fall-yaw f@ 20e deg>rad turn-toward f0= if  1 her-fall-turn !  then  her-yaw f!
        then
    then
    8 0 do  i exit-area dup area-count < if  her-pos area-in? if  unloop exit  then  else  drop  then  loop
    along-wall ;
: falling ( -- )
    settled? if
        -1 her-attack-t !  -1 her-fall-turn !  -1 her-wall-side !  her-yaw f@ her-fall-yaw f!
        $B00 -1 play-table  $40 0 her-sound  $5F noise-here
        ['] fallen her-act !
    then
    root-move ;
: stumbling ( -- )  ended? if  to-idle exit  then  root-move ;

\ ---- the panic's part of her controls: running into something while panicking - a stumble, or
\ (one time in five, at most every 30 frames) a fall; at full fear the panic attack ----
create next-at 12 allot
: blocked-ahead? ( -- flag )   \ (the root motion's step ahead lands off her floor)
    root@  rm-x f@ rm-z f@  her-still @ if  her-heading f@  else  stick-heading  then  rotate-by
    her-at 8 + sf@ f+ next-at 8 + sf!  her-at sf@ f+ next-at sf!  her-at 4 + sf@ next-at 4 + sf!
    next-at blocked-floor v-tri-in 0< ;
: panic-controls ( -- done? )
    her-fear-bits @ 2 and if
        her-stumble-t @ if  -1 her-stumble-t +!  then
        blocked-ahead? 0= if  false exit  then
        $7F 0 her-sound
        her-stumble-t @ 0= if
            30 her-stumble-t !
            rnd 0.2e f< if
                1000e f>cell panic-id send fright   \ (the original puts 1000 into the panic's fear)
                4 her-mode !  $A her-doing !  ['] falling her-act !  true exit
            then
        then
        4 her-mode !  $A her-doing !  $1001 -1 play-table  ['] stumbling her-act !  true exit
    then
    her-fear f@ 100e f< 0= if
        150 her-attack-t !  75e her-fear f!  $E her-doing !  ['] in-attack her-act !  true exit
    then
    false ;

\ ---- turning to flee (R1 - Q on the keyboard: Fiona_StateFleeStart / FleeTurn): she whips round
\ (0x403) and runs; as she turns her fear rises 10 (not in costume 8); slamming a door behind
\ her and Hewie's reaction: with them ----
: add-fear ( F: amount -- )  her-fear f@ f+ 0e fmax 100e fmin her-fear f! ;   \ Fiona_AddPanic (charms: with items)
: events ( -- bits )  her-model @ 0 0 1 motion-events ;
: costume ( -- n )  $26 progress pr.vars + c@ ;
: flee-turn ( -- )
    ended? if  to-idle exit  then
    events 2 and if  0 her-mode !  costume 8 <> if  10e add-fear  then  then
    12e deg>rad turn-to-heading fdrop
    root@ rm-x f@ rm-z f@ her-heading f@ rotate-by move-by ;
: flee-start ( -- )
    root-move
    settled? 0= if  exit  then
    her-yaw f@ her-heading f!                          \ (the stalker within 30: toward him - with them)
    $403 -1 play-table  ['] flee-turn her-act ! ;
: flee-button? ( -- flag )  key: Q key-pressed? ;
: try-flee ( -- )   \ (free, not under flag 0x14, recovered, not panicking)
    her-mode @ 0<>  her-doing @ 0<> or  flee-button? 0= or if  exit  then
    no-flee state-flag?  her-recovery @ 360 < 0= or  her-panic-stage @ 4 < 0= or if  exit  then
    $B her-mode !  $D her-doing !  ['] flee-start her-act ! ;
