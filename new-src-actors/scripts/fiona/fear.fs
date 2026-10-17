\ fiona/fear.fs - Fiona's fear and panic (Fiona_Panic, Fiona_FullPanic, Fiona_Exhausted, the
\ panic's part of Fiona_ControlCommand and its states): her fear meter (0..99, 100 a panic
\ attack); out of breath from the panic's stage 2, panicking at 4 with a scream others hear; the
\ panic run until she's out of breath; stumbling or falling as she runs into something; getting
\ up again. The panic itself is its own actor (panic.fs): she hears its stage and level.
IN: fiona.fear
USING: engine vectors keys game-state actors messages common facts items.names fiona.state fiona.anims fiona.model fiona.controls fiona.moving fiona.slam flag-names ;

defer to-idle   ' noop is to-idle      \ (fiona.fs)
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: panic-id ( -- id )  s" panic" actor-named ;
: noise-here ( loud -- )  room-id her-tri @ -1 fiona-noise acoustics-id send noise ;
: her-sound ( id vol -- )  5 swap 0 her-pos sound-at ;

\ ---- her fear meter (Fiona_ChangeFear; charms: with items) ----
: worn-scaled ( F: d -- d' )   \ (Fiona_ChangeFear's chokers: the jade a quarter off a rise; the ruby a
    \ quarter off a rise, half again on a fall; the diamond no rise, a fall doubled)
    fdup f0> if
        her-choker case  jade-choker of  0.75e f*  endof  ruby-choker of  0.75e f*  endof  diamond-choker of  fdrop 0e  endof  endcase
    else
        her-choker case  ruby-choker of  1.5e f*  endof  diamond-choker of  2e f*  endof  endcase
    then ;
: change-fear ( F: d -- )  worn-scaled her-fear f@ f+ 0e fmax 99e fmin her-fear f! ;
: fear-down ( -- )  her-danger @ chased = if  -0.1e  else  -0.15e  then  change-fear ;   \ Fiona_FearDown

\ ---- out of breath, panicking (Fiona_Panic, Fiona_FullPanic, Fiona_Exhausted) ----
: full-panic ( -- )
    her-fear-bits @ 2 or her-fear-bits !  3e rnd f* f>s 30 * 120 + her-panic-t !  30 her-stumble-t !
    $6F noise-here ;
:noname ( -- )   \ (Fiona_Exhausted: out of breath from the panic run, 0x207; a new run length)
    3e rnd f* f>s 30 * 120 + her-panic-t !  breathless-doing her-doing !  out-of-breath-anim -1 play-table ; is exhausted
: feel-the-panic ( -- )   \ (each frame: the panic's stage taken up; her fear meter; her timers)
    her-panic-stage @
    her-fear-bits @ 1 and 0= if
        dup scared >= if
            her-fear-bits @ 1 or her-fear-bits !
            dup case
                panicking of  full-panic  endof
                terrified of  4e rnd f* f>s 30 * 150 + her-panic-t !  endof
                scared of  5e rnd f* f>s 30 * 240 + her-panic-t !  endof
            endcase
        then
    else dup panicking < if
        dup scared < if  1e her-tired-w f!  her-fear-bits @ 3 invert and her-fear-bits !
        else  her-fear-bits @ 2 invert and her-fear-bits !  then
    else  her-fear-bits @ 2 and 0= if  full-panic  then  then then
    drop
    her-mode @ own-mode =  her-fear-bits @ 2 and 0= and  her-recovery @ 0> and if  -1 her-recovery +!  then
    her-fear f@ 100e f< if
        her-mode @ own-mode =  her-doing @ attack-doing <> and if
            group case
                breathless-group of  endof
                running-group of  her-choker diamond-choker <> if  her-danger @ chased = if  0.0666667e  else  0.0466667e  then  change-fear  then  endof   \ running frightens (not with the diamond choker)
                walking-group of  fear-down  endof
                >r  her-fear-bits @ 2 and 0= if  fear-down  then  r>
            endcase
        else her-mode @ ladder-mode = if  fear-down  then then
    then
    her-mode @ own-mode <> if  0 her-run-t !  then ;

\ ---- the panic attack (at full fear: 150 frames, her cry 0x44, animation 2) ----
: in-attack ( -- )
    -1 her-attack-t +!  her-attack-t @ 0= if  to-idle exit  then
    settled? anim@ attack-anim <> and if  $44 0 her-sound  attack-anim -1 play-table  then
    root-move ;

\ ---- falling, sliding, getting up (Fiona_StatePanicFall, StateFallen, StateSpecialEntry) ----
create mouth /vec allot
: along-wall ( -- )   \ (Fiona_AlongWall: turned along the wall she stopped at, by her mouth's way out)
    her-model @ $13 bone-pos mouth vec!
    her-tri @ dup 0< if  drop exit  then
    her-at mouth blocked-floor v-wall 0= if  fdrop exit  then         ( F: yaw )
    fdup her-yaw f@ f- angle-wrap fabs 90e deg>rad f> if  pi f+ angle-wrap  then  her-heading f!
    her-heading f@ her-yaw f@ f- angle-wrap  fdup fabs 0.25e f* 0.5e deg>rad fmax        ( F: a step )
    her-wall-side @ -1 = if  fover f0< if  0  else  1  then  her-wall-side !  then
    fswap fdrop  her-wall-side @ if  her-yaw f@ f+  else  her-yaw f@ fswap f-  then  angle-wrap her-yaw f! ;
create probe /vec allot  create up-was /vec allot  variable up-tri
: probe-ok? ( -- flag )   \ (the point 4 ahead of her reached straight over her floor: Actor_TriTo)
    her-yaw f@ 4e probe her-at vec-ahead  her-tri @ her-at probe blocked-floor v-walk 0< 0= ;
: getting-up ( -- )   \ (Fiona_StateSpecialEntry)
    settled? if
        anim@ lying-anim = if
            her-still @ 0= if  stick-heading her-heading f!  then
            her-fear-bits @ 1 and 0= if  get-up-anim -1 play-table  then
        else anim@ get-up-anim = ended? and if  to-idle exit  then then
    then
    root-step {: F: rx rz :}
    rz f0> dup if   \ (turned toward the stick - not where 4 ahead can't be got to)
        her-yaw f@  her-yaw f@ her-heading f@ rz 10e f* deg>rad turn-toward fdrop her-yaw f!
        probe-ok? if  fdrop  else  her-yaw f!  then
    then
    root-turn fdrop fdrop fdrop  her-yaw f@ her-heading f!
    up-was her-at vec-copy  her-tri @ up-tri !
    rx rz her-yaw f@ rotate-by  her-at blocked-floor 1 or v-nav-move  dup 0< if  drop  else  her-tri !  then
    if  probe-ok? 0= if  her-at up-was vec-copy  up-tri @ her-tri !  then  then   \ (moved off: back)
    her-attack-t @ $97 < if  shakes-now if  2 panic-id send panic-shorten  2 her-attack-t +!  then  then ;
: fallen ( -- )
    ended? if  0 her-attack-t !  lying-anim -1 play-table  down-mode her-mode !  down-doing her-doing !  -1 her-shake-rest !  0e her-shake-a f!
        ['] getting-up her-act ! exit  then
    her-fall-turn @ -1 = if  root-move
    else
        root-step her-fall-yaw f@ rotate-by move-by
        her-fall-turn @ 0= if
            her-yaw f@ her-fall-yaw f@ 20e deg>rad turn-toward f0= if  1 her-fall-turn !  then  her-yaw f!
        then
    then
    8 0 do  i exit-area dup area-count < if  her-pos area-in? if  unloop exit  then  else  drop  then  loop
    along-wall ;
: falling ( -- )
    settled? if
        -1 her-attack-t !  -1 her-fall-turn !  -1 her-wall-side !  her-yaw f@ her-fall-yaw f!
        fall-anim -1 play-table  $40 0 her-sound  $5F noise-here
        ['] fallen her-act !
    then
    root-move ;
: stumbling ( -- )  ended? if  to-idle exit  then  root-move ;

\ ---- the panic's part of her controls: running into something while panicking - a stumble, or
\ (one time in five, at most every 30 frames) a fall; at full fear the panic attack ----
create next-at /vec allot
: blocked-ahead? ( -- flag )   \ (the root motion's step ahead lands off her floor)
    root-step  her-still @ if  her-heading f@  else  stick-heading  then  rotate-by  {: F: dx dz :}
    her-at v.x sf@ dx f+ next-at v.x sf!  her-at v.y sf@ next-at v.y sf!  her-at v.z sf@ dz f+ next-at v.z sf!
    her-tri @ her-at next-at blocked-floor v-walk 0< ;   \ (not reached straight over her floor: Actor_TriTo)
: panic-controls ( -- done? )
    her-fear-bits @ 2 and if
        her-stumble-t @ if  -1 her-stumble-t +!  then
        blocked-ahead? 0= if  false exit  then
        $7F 0 her-sound
        her-stumble-t @ 0= if
            30 her-stumble-t !
            rnd 0.2e f< if
                1000e f>cell panic-id send fear-set   \ (the original sets the panic's fear to 1000)
                thrown-mode her-mode !  thrown-doing her-doing !  ['] falling her-act !  true exit
            then
        then
        thrown-mode her-mode !  thrown-doing her-doing !  stumble-anim -1 play-table  ['] stumbling her-act !  true exit
    then
    her-fear f@ 100e f< 0= if
        150 her-attack-t !  75e her-fear f!  attack-doing her-doing !  ['] in-attack her-act !  true exit
    then
    false ;

\ ---- turning to flee (R1 - Q on the keyboard: Fiona_StateFleeStart / FleeTurn): she whips round
\ (0x403) and runs; as she turns her fear rises 10 (not in costume 8), and a door she turns from
\ is slammed (fiona/slam.fs); Hewie's reaction: with him ----
: add-fear ( F: amount -- )  choker-fear her-fear f@ f+ 0e fmax 100e fmin her-fear f! ;   \ Fiona_AddPanic
: events ( -- bits )  her-model @ 0 0 1 motion-events ;
: costume ( -- n )  $26 progress pr.vars + c@ ;
create behind-pt /vec allot   \ (her radius behind her: a door slammed as she turns from it)
: flee-turn ( -- )
    recoil-down
    ended? if  9 s" hewie" actor-named send reaction  to-idle exit  then   \ (Hewie told: Fiona_HewieReact 9)
    events $20 and if
        her-yaw f@ pi f+ angle-wrap self body-dims fdrop behind-pt her-at vec-ahead  behind-pt false slam-door drop
    then
    events 2 and if  own-mode her-mode !  costume 8 <> if  10e add-fear  then  then
    12e deg>rad turn-to-heading fdrop
    root-step her-heading f@ rotate-by move-by ;
: flee-start ( -- )
    root-move
    settled? 0= if  exit  then
    her-yaw f@ her-heading f!                          \ (the stalker within 30: toward him - the turn spins her from him)
    stalker-in-room dup 0< 0= if
        dup body-at her-at vec-dist 30e f< if  her-at swap body-at vec-heading her-heading f!  else  drop  then
    else  drop  then
    flee-turn-anim -1 play-table  ['] flee-turn her-act ! ;
: flee-button? ( -- flag )  r1 button-pressed? ;   \ (Progress_PlayerButtons: R1)
: try-flee ( -- )   \ (free, not under flag 0x14, recovered, not panicking)
    her-mode @ own-mode <>  her-doing @ free-doing <> or  flee-button? 0= or if  exit  then
    no-flee state-flag?  her-recovery @ 360 < 0= or  her-panic-stage @ panicking < 0= or if  exit  then
    react-mode her-mode !  flee-doing her-doing !  ['] flee-start her-act ! ;
