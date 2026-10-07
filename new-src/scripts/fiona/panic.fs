\ fiona/panic.fs - the panic (src/game/fiona.c Panic_*: progress +0x7B8, SceneGame +0x7F8) and
\ Fiona's side of it: the level (0..100) made of a lasting and a passing part, the stage from it
\ (0 calm, 1..3 at 60 / 75 / 90, 4 panicking at 100 - with her scream - for 450 frames and more,
\ 5 calming down), her breath and heartbeat by the stage; her stumbles and falls running in a
\ panic, her panic attack at full fear, getting up after a fall, and turning to flee (R1).
\ (The stalker's part of the fear, the screen's tint and the camera's shake: with the stalkers.)
IN: fiona.panic
USING: engine game-state events.core events.words chars relations fiona.core fiona.moves fiona.brain fiona.commands ;

\ ---- the panic object (progress +0x7B8; `panic` is its stage +0x0) ----
variable pn-sub       \ +0x1: how the inputs weigh (0..3)
variable pn-len       \ +0x2: frames of the panic left (negative: its start)
fvariable pn-level    \ +0x4: the level, 0..100
variable pn-hold      \ +0x8: frames before it may recover
fvariable pn-pass     \ +0xC: its passing part
fvariable pn-a  fvariable pn-a2   \ +0x10 / +0x14: frights this frame (the passing part)
fvariable pn-rec      \ +0x18: recovering
fvariable pn-last     \ +0x1C: its lasting part
fvariable pn-d  fvariable pn-d2   \ +0x20 / +0x24: fear this frame
fvariable pn-b  fvariable pn-b2   \ +0x28 / +0x2C: calming this frame
variable pn-beat      \ +0x30: frames to the next breath
fvariable pn-screen   \ +0x34: the screen's effect level (1: full)
variable pn-shake     \ +0x40
fvariable pn-scale  1e pn-scale f!   \ (progress +0x9E8 while +0x9EC: the fear's scale)
: panic-reset ( -- )
    0 panic !  0 pn-sub !  0 pn-len !  0e pn-level f!  0 pn-hold !  0e pn-pass f!  0e pn-a f!  0e pn-a2 f!
    0e pn-rec f!  0e pn-last f!  0e pn-d f!  0e pn-d2 f!  0e pn-b f!  0e pn-b2 f!  0 pn-beat !
    1e pn-screen f!  0 pn-shake ! ;
panic-reset
: inputs-clear ( -- )  0 pn-sub !  0e pn-pass f!  0e pn-a f!  0e pn-a2 f!  0e pn-rec f!  0e pn-d f!  0e pn-d2 f!  0e pn-b f!  0e pn-b2 f! ;
\ Panic_SetLevel: the stage by the level (at 100 its length 450 frames), the inputs cleared
: panic-level! ( n -- )
    dup s>f pn-level f!  dup s>f pn-last f!
    dup 100 >= if  450 pn-len !
    else dup 90 >= if  3 panic !  else dup 75 >= if  2 panic !  else dup 60 >= if  1 panic !  else  0 panic !
    then then then then  drop  inputs-clear ;
\ Panic_SetStage
create pn-stages  0 , 60 , 75 , 90 , 100 , 100 ,
: panic-stage! ( stage -- )
    dup 6 u< if  cells pn-stages + @ panic-level!  else  drop  then ;
\ fright: under 10 to this frame's fear; more: half of it to the passing part too, held 30 frames
: fright ( F: amount -- )
    fdup f0< if  fdrop exit  then
    fdup 10e f< if  pn-d f@ f+ pn-d f!  exit  then
    0.5e f*  30 pn-hold !  fdup pn-d f@ f+ pn-d f!  pn-a f@ f+ pn-a f! ;
\ Panic_Fright: less with a charm on (the sub screen's slot 1: 0x87 three quarters, 0x88 half)
variable amulet   \ (the sub screen's slot 1)
: panic-fright ( F: amount -- )
    amulet @ case  $87 of  0.75e f*  endof  $88 of  0.5e f*  endof  endcase  fright ;

\ ---- Panic_Breath: her breath (0x29, softer at the lower stages) ----
create breath-vol  -56 , -40 , -16 , 0 , 0 ,   \ (-0x38 -0x28 -0x10)
: breath ( -- )
    pn-screen f@ 1e f= 0= if  exit  then
    panic @ dup 1 6 within 0= if  drop exit  then
    1- cells breath-vol + @  >r  me $29 5 r> 0 f-pos vec@ actor-sound ;

\ ---- Panic_Stage ----
defer pursuer-fear ( -- )  ' noop is pursuer-fear   \ (the stalker's part of the inputs: with them)
: stage-by-level ( -- )
    pn-level f@ 100e f< 0= if   \ she panics
        100e pn-level f!  100e pn-last f!  0e pn-pass f!
        f-mode dup 0= swap 3 = or if
            me $42 5 $40 0 f-pos vec@ actor-sound
            4 panic !  -30 pn-len !  0 pn-beat !
        then  exit
    then
    pn-level f@ 90e f< 0= if  3  else pn-level f@ 75e f< 0= if  2  else pn-level f@ 60e f< 0= if  1  else  0  then then then
    panic ! ;
: panic-stage ( -- )
    pn-beat @ if  -1 pn-beat +!  then
    panic @ case
        0 of  pn-shake @ if  pn-shake @ 8 - 0 max pn-shake !  then  4  endof
        1 of  4  endof  2 of  4  endof  3 of  4  endof
        4 of  5  endof  5 of  5  endof
        >r 0 r>
    endcase
    4 = if
        stage-by-level
        panic @ 4 = if  exit  then
        panic @ 0> pn-beat @ 0= and if  4 panic @ - 30 * pn-beat !  breath  then
        pn-level f@ 60e f< if  exit  then
        pn-level f@ 60e f- 1.5e f* f>s  dup pn-shake @ > if  1 pn-shake +!  else  pn-shake @ < if  -1 pn-shake +!  then  then
        exit
    then
    panic @ 4 = if
        pn-len @ 0< if   \ its start
            1 pn-len +!  pn-len @ 0= if
                450 pn-len !  me character char.cond sl@ dup 1 = if  300 pn-len +!  then  3 = if  150 pn-len +!  then
            then  exit
        then
        pn-d f@ 500e f< 0= if  150 pn-len +!  pn-len @ 450 > if  450 pn-len !  then  5 panic !  then
    then
    -1 pn-len +!
    pn-len @ 0< if  0 panic !  50e pn-level f!  50e pn-last f!  0e pn-pass f!  exit  then
    panic @ 4 <> if  $90  else  $50  then  pn-shake !
    pn-beat @ if  exit  then
    6 panic @ - 7 * pn-beat !  breath ;

\ ---- Panic_FearInputs (the calm with time; the stalker's part: pursuer-fear) and Panic_Update ----
: fear-inputs ( -- )
    game-mode @ 0= if  pn-b2 f@ 0.0999998e f+ pn-b2 f!
    else f-mode 0= f-sub 0= and if  pn-b2 f@ 0.0333333e f+ pn-b2 f!  then then
    pn-hold @ 0= if  pn-rec f@ 0.1666653e f+ pn-rec f!  then
    pursuer-fear ;
fvariable u-a  fvariable u-b  fvariable u-c  fvariable u-d
: panic-update ( -- )
    8 state-flag? if  0e pn-screen f!  then
    panic-stage
    pn-hold @ if  -1 pn-hold +!  then
    pn-level f@ 100e f< if
        fear-inputs
        pn-a f@ u-a f!  pn-b f@ u-b f!  pn-rec f@ u-c f!  pn-d f@ u-d f!
        pn-sub @ case
            0 of  u-d f@ pn-d2 f@ f+ u-d f!  u-a f@ pn-a2 f@ f+ u-a f!  u-b f@ pn-b2 f@ f+ u-b f!  endof
            1 of  u-d f@ pn-d2 f@ 0.6e f* f+ u-d f!  u-a f@ pn-a2 f@ 0.6e f* f+ u-a f!  u-b f@ pn-b2 f@ 2e f* f+ u-b f!  endof
            2 of  u-d f@ pn-d2 f@ 0.8e f* f+ u-d f!  u-a f@ pn-a2 f@ 0.8e f* f+ u-a f!  u-b f@ pn-b2 f@ 1.6e f* f+ u-b f!  endof
            3 of  u-d f@ pn-d2 f@ 0.9e f* f+ u-d f!  u-a f@ pn-a2 f@ 0.9e f* f+ u-a f!  u-b f@ pn-b2 f@ 1.5e f* f+ u-b f!  endof
        endcase
        u-d f@ pn-scale f@ f* u-d f!  u-a f@ pn-scale f@ f* u-a f!
        amulet @ case  $86 of  u-b f@ 1.25e f* u-b f!  endof  $87 of  u-b f@ 1.25e f* u-b f!  endof
                       $88 of  u-b f@ 2e f* u-b f!  endof  endcase
        pn-pass f@ u-c f@ f- 0e fmax pn-pass f!
        pn-last f@ u-b f@ f- 0e fmax u-d f@ f+ pn-last f!
        u-a f@ f0> 0= if
            pn-last f@ pn-pass f@ f+ pn-level f!
            pn-level f@ 100e f< 0= if  99e pn-level f!  99e pn-last f!  then
        else
            pn-last f@ pn-pass f@ f+ pn-last f!  u-a f@ pn-pass f!  pn-last f@ pn-pass f@ f+ pn-level f!
        then
        $15 state-flag? if   \ (she reaches 100 only under flag 0x15 - as the original: never)
            pn-level f@ 100e f< 0= if  99e pn-level f!  99e pn-last f!  0e pn-pass f!  then
        then
    then
    inputs-clear ;

\ ---- Fiona_AddPanic: her fear (to 100: a panic attack), by her charm ----
: add-fear ( F: amount -- )
    charm @ case
        $8C of  fdup f0> if  0.5e f*  else  2e f*  then  endof
        $8B of  fdup f0> if  0.75e f*  else  1.5e f*  then  endof
        $8A of  fdup f0> if  0.75e f*  then  endof
    endcase
    f-fear f@ f+ 0e fmax 100e fmin f-fear f! ;

\ ---- her panic's moves ----
variable pa-t   \ +0x1AD6C0
: st-panic-attack ( -- )   \ Fiona_StatePanicAttack: 150 frames of it (her cry 0x44, animation 2)
    -1 pa-t +!  pa-t @ 0= if  to-idle  then
    f-settled? f-anim@ 2 <> and if  $44 5 0 0 f-sound  2 -1 f-play-table  then
    f-root-move ;
\ Fiona_AlongWall: turned along the wall she stopped at (her mouth's way out of her triangle)
variable aw-side  -1 aw-side !
: along-wall ( -- )
    f-actor dup 0< if  drop exit  then  $13 bone-pos mouth vec!   \ (her mouth: the motion's +0x60)
    f-tri dup 0< if  drop exit  then
    f-pos mouth me c-mask v-wall                           ( flag ) ( F: yaw )
    0= if  fdrop exit  then
    fdup f-yaw f- angle-wrap fabs 90e deg>rad f> if  pi f+ angle-wrap  then  f-heading f!
    f-heading f@ f-yaw f- angle-wrap  fdup fabs 0.25e f* 0.5e deg>rad fmax         ( F: a step )
    aw-side @ -1 = if  fover f0< if  0  else  1  then  aw-side !  then
    fswap fdrop  aw-side @ if  f-yaw f+  else  f-yaw fswap f-  then  angle-wrap f-yaw! ;
fvariable fall-yaw  variable fall-turn
defer st-special-entry  ' noop is st-special-entry
: st-fallen ( -- )   \ Fiona_StateFallen: sliding on; at its end getting up (0xB01)
    1 f-2a !  0 f-cam-on !
    f-end? if
        0 f-2d !  0 pa-t !  $B01 -1 f-play-table  $A f-mode!  $B f-act !
        ['] st-special-entry behave
    then
    me c-mask 1 or me c-mask!
    fall-turn @ -1 = if  f-root-move
    else
        root@ rm-x f@ rm-z f@ fall-yaw f@ rotate-by f-move
        fall-turn @ 0= if  fall-yaw f@ 20e deg>rad f-turn-toward f0= if  1 fall-turn !  then  then
    then
    me c-mask 1 invert and me c-mask!
    8 0 do  i exit-area f-pos vec@ area-in? if  unloop exit  then  loop
    along-wall ;
: st-panic-fall ( -- )   \ Fiona_StatePanicFall: the fall (0xB00, her cry 0x40, a loud noise)
    1 f-2a !  0 f-cam-on !
    f-settled? if
        -1 pa-t !  -1 fall-turn !  f-yaw fall-yaw f!
        $B00 -1 f-play-table  $40 5 0 0 f-sound  $5F noise-here
        ['] st-fallen behave
    then
    f-root-move ;
\ Fiona_StateSpecialEntry: getting up (0xB01, then 0xB02 unless still frightened), moving by
\ the motion (turning to the stick at 10 degrees a unit); shaking off the panic quickens it
defer shakes ( -- n )  :noname 0 ; is shakes
create se-ahead 12 allot
:noname ( -- )
    0 f-cam-on !
    f-settled? if
        f-anim@ $B01 = if
            f-still @ 0= if  stick-heading f-heading f!  then
            f-fear-bits @ 1 and 0= if  $B02 -1 f-play-table  then
        else f-anim@ $B02 = f-end? and if  to-idle  then then
    then
    root@
    rm-z f@ f0> if
        f-yaw  f-heading f@ rm-z f@ 10e f* deg>rad f-turn-toward fdrop
        f-yaw 4e se-ahead f-pos vec-ahead
        me se-ahead -1 c-tri-to 0< if  f-yaw!  else  fdrop  then
    then
    root-turn  f-yaw f-heading f!
    me c-mask 1 or me c-mask!  rm-x f@ rm-z f@ f-yaw rotate-by f-move  me c-mask 1 invert and me c-mask!
    pa-t @ $97 < if  shakes if  -2 pn-len +!  2 pa-t +!  then  then ; is st-special-entry
: st-stumble ( -- )  f-end? if  to-idle  then  f-root-move ;   \ (IdleAtEvent2 on 0x1001)

\ ---- Fiona_ControlCommand's panic part: running into something while panicking - a stumble, or
\ (one time in five, at most every 30 frames) a fall; at full fear the panic attack ----
create pc-at 12 allot
:noname ( -- done? )
    f-fear-bits @ 2 and if
        f-stumble-t @ if  -1 f-stumble-t +!  then
        root@  rm-x f@ rm-z f@  f-still @ if  f-heading f@  else  stick-heading  then  rotate-by
        f-pos 8 + sf@ f+ pc-at 8 + sf!  f-pos sf@ f+ pc-at sf!  f-pos 4 + sf@ pc-at 4 + sf!
        me pc-at -1 c-tri-to 0< if
            $7F 5 0 0 f-sound
            f-stumble-t @ 0= if
                30 f-stumble-t !
                rnd 0.2e f< if
                    1000e pn-d f!  4 f-mode!  $A f-act !  1 f-2d !  ['] st-panic-fall behave  true exit
                then
            then
            4 f-mode!  $A f-act !  $1001 -1 f-play-table  ['] st-stumble behave  true exit
        then
        false exit
    then
    f-fear f@ 100e f< 0= if
        150 pa-t !  75e f-fear f!  $E f-act !  0 f-2d !  ['] st-panic-attack behave  true exit
    then
    false ; is panic-control

\ ---- turning to flee (R1: request 0xB 0x22) ----
defer slam-door ( to? -- slammed? )  :noname drop false ; is slam-door   \ (Fiona_SlamDoor: with the stalkers)
: st-flee-turn ( -- )   \ Fiona_StateFleeTurn (0x403)
    f-end? if  9 hewie-react  to-idle exit  then
    f-events 2 and if  0 f-mode!  0 f-2d !  $26 pvar@ 8 <> if  10e add-fear  then  then
    f-events $20 and if  0 slam-door drop  then
    f-heading f@ 12e deg>rad f-turn-toward fdrop
    root@ rm-x f@ rm-z f@ f-heading f@ rotate-by f-move ;
: st-flee-start ( -- )   \ Fiona_StateFleeStart
    f-root-move
    f-settled? 0= if  exit  then
    f-yaw f-heading f!
    f-pu-ok @ 1 = if  pursuer-slot @ c-pos f-pos vec-dist 30e f< if  me pursuer-slot @ c-pos c-heading-to f-heading f!  then  then
    $403 -1 f-play-table  ['] st-flee-turn behave ;
:noname ( -- )   \ Fiona_StateBlock 0xB (0x22: flee; 0x20 / 0x21 caught - with the stalkers)
    me 1 relations:req-word-of dup f-sub!
    $22 = if
        $14 state-flag? 0= f-recovery @ 360 < and panic @ 4 < and if
            1 f-2d !  $B f-mode!  $D f-act !  ['] st-flee-start behave
        then
    then ; is fiona.commands:caught-request

\ ---- the scripts' commands on the panic and her fear ----
: reset-recovery ( -- )   \ Fiona_ResetRecovery: shaken again (1800 frames) - less with a charm
    charm @ case
        $8C of  exit  endof
        $8B of  rnd 0.5e f< 0= if  exit  then  endof
        $8A of  rnd 0.75e f< 0= if  exit  then  endof
    endcase  1800 f-recovery ! ;
: threat-raise ( F: v -- )   \ Threat_Raise: as a fright, into the +0x24 / +0x14 inputs
    fdup f0< if  fdrop exit  then
    fdup 10e f< if  pn-d2 f@ f+ pn-d2 f!  exit  then
    0.5e f*  30 pn-hold !  fdup pn-d2 f@ f+ pn-d2 f!  pn-a2 f@ f+ pn-a2 f! ;
:noname ( op v -- )
    swap case
        $43 of  s>f 0e fmax 100e fmin threat-raise  endof
        $93 of  dup $80 and if  $80 and s>f pn-b f@ f+ pn-b f!  else  s>f 0.0333333e f* pn-b2 f@ f+ pn-b2 f!  then  endof
        $A3 of  panic-stage!  endof
        $C0 of  s>f 0e fmax 100e fmin pn-d2 f@ f+ pn-d2 f!  endof
        $C9 of  dup s>f pn-level f@ f>= if  panic-level!  else  drop  then  endof
        $4E of  drop  0 f-recovery !  0e f-fear f!  endof
        $4F of  drop  reset-recovery  endof
        $94 of  calm-down  endof
        $95 of  lower-recovery  endof
        >r drop r>
    endcase ; is event-panic
:noname ( F: v -- )  0e fmax 100e fmin f-fear f! ; is event-fiona-fear
