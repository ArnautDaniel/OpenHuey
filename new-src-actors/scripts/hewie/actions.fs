\ hewie/actions.fs - what Hewie does (Hewie_SetAction, Hewie_AdjustAction): each action sets
\ his move mode, where he looks, what may break in (his-cmd), the timers and the behaviour that
\ carries it out. Actions of the later phases fall back as noted (H2 her commands, H3 the
\ stalkers, H4 the story).
IN: hewie.actions
USING: engine game-state actors common facts paths hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states hewie.offscreen flag-names ;

: mode! ( n -- )  his-mode ! ;
: give-up ( -- )  6 0 want ;            \ nothing to do: sniffing about (steered: the default - H4)
: trust-wait ( -- n )  his-trust @ 60 * 300 + ;
\ Hewie_PickTarget: whom he goes for - the stalker in his room that he can get to (H3: none yet)
: pick-target ( -- id | -1 )
    pursuer dup with? 0= if  drop -1 exit  then
    dup body-tri over body-at plan-to 0= if  drop -1  then ;
\ Hewie_IdleAction: a trick at random (not the last one), on plain floor
create idle-tricks  $18 , $19 , $1C , $1A , $1B ,
: plain-floor? ( -- flag )  his-tri nav-flags 3 and 0= ;
: idle-action ( -- )
    plain-floor? 0= if  to-default exit  then
    begin  5 roll cells idle-tricks + @  dup his-last-idle @ = while  drop  repeat
    dup his-last-idle !  start ;
\ Hewie_RandomIdle: lying by her (1, half the time), stand by (4) or settle (5)
: random-idle ( -- )  100 roll dup 50 < if  drop 1  else  65 < if  4  else  5  then  then  start ;
\ go_for (after a call or coming out): whom - none (0), the stalker (2, left to the caller)
: go-for ( -- n )
    pick-target dup his-target2 !  dup with? 0= if  drop 0 exit  then
    his-held @ 0= swap pursuer = and if  2 exit  then
    $5A 0 want  1 ;
: after-call ( -- )  go-for case  0 of  6 0 want  endof  2 of  give-up  endof  endcase ;   \ (0x4F / 0x50)
: after-coming-out ( -- )   \ Hewie_AfterComingOut (0x4E: her call - "go")
    go-for case  0 of  6 0 want  endof  2 of  give-up  endof  endcase ;
\ Hewie_After4D: by his pose, praise-me (0x28) / lying (0x26) or a roll over (0x2A)
: after-4d ( -- )
    anim-group rnd 0.5e f<                                           ( g coin )
    swap case
        0 of  his-cond @ 1 <> and if  $28  else  $2A  then  endof
        4 of  his-cond @ 1 <> and if  $28  else  $2A  then  endof
        1 of  if  $26  else  $2A  then  endof
        5 of  if  $26  else  $2A  then  endof
        2 of  his-cond @ 1 = or if  $26  else  $2A  then  endof
        6 of  his-cond @ 1 = or if  $26  else  $2A  then  endof
        >r drop $26 r>
    endcase  0 want ;

\ a pose to hold, looking about, what may break in, the behaviour
: holding ( look cmd xt -- )  0 mode!  behave  his-cmd !  look! ;
: low-floor? ( -- flag )  his-tri nav-flags $80001 and 0<> ;   \ (no rolling over there)
: by-her? ( -- flag )  her-with? ;
: target-her ( -- )  0 look!  her his-target ! ;
\ keeping near (0x53..0x57, 0x7C): the turn, the timers, what may break in
: keep-near ( F: turn -- ) ( cmd -- )
    his-turn f!  0 his-t1 !  0 his-t2 !  his-yaw his-heading f!  his-cmd !  ['] st-keep-near behave ;

: (set-action) ( act -- )
    case
    0 of  -1 his-target2 !  0 mode!  0 his-cmd !  0 his-2d !  1 his-pending !  4 look!  ['] st-1d98 behave  endof
    1 of  by-her? if  0 mode!  target-her  $6AE his-cmd !  ['] st-1dc8 behave  else  4 0 want  then  endof
    2 of  by-her? if  0 mode!  target-her  trust-wait his-wait !  $688 his-cmd !  ['] st-1dc8 behave
          else  4 0 want  then  endof
    3 of  by-her? if  0 mode!  his-wanted @ his-wait !  target-her  $688 his-cmd !  ['] st-1dc8 behave
          else  4 0 want  then  endof
    4 of  4 $7AD ['] st-pose4 holding  endof
    5 of  45 roll dup 20 < if  drop 1  else  dup 25 < if  drop 13  else  30 < if  11  else  12  then  then  then
          $7AD ['] st-pose4 holding  endof
    6 of  0 mode!  90 his-wait !  pick-target his-target !  $6AF his-cmd !  ['] st-face-scent behave  endof
    7 of  0 mode!  trust-wait his-wait !  pick-target his-target !  $58C his-cmd !  ['] st-steer behave  endof
    8 of  pick-target his-target !
          his-alert @ 2 =  his-target @ with? or if  150 his-wait !  0 mode!  $7AF his-cmd !  ['] st-face-target behave
          else  6 0 want  then  endof
    9 of  by-her? if  0 mode!  target-her  300 his-wait !  $384 his-cmd !  ['] st-wait-for-fiona behave
          else  6 0 want  then  endof
    $A of  his-target2 @ 0< if  pick-target his-target2 !  then
           his-target2 @ with? if  0 mode!  2 roll 1+ his-t1 !  his-target2 @ his-target !  $7AF his-cmd !  ['] st-bark-at-target behave
           else  6 0 want  then  endof
    $B of  by-her? if  0 mode!  3 roll 3 + his-t1 !  target-her  $384 his-cmd !  ['] st-bark-at-fiona behave
           else  6 0 want  then  endof
    $C of  by-her? if  0 mode!  2 his-t1 !  $68F his-cmd !  ['] st-slide-to-fiona behave  else  to-default  then  endof
    $D of  by-her? if  0 mode!  2 his-t1 !  $68F his-cmd !  ['] st-slide-to-fiona behave  else  to-default  then  endof
    $E of  by-her? if  0 mode!  2 his-t1 !  $68F his-cmd !  ['] st-slide-to-fiona behave  else  to-default  then  endof
    $F of  by-her? if  0 mode!  2 his-t1 !  $58F his-cmd !  ['] st-slide-to-fiona behave  else  to-default  then  endof
    $10 of  by-her? if
                0 mode!  0 his-t1 !  0 his-t2 !  random-level his-t3 !  his-yaw his-heading f!  turn-speed
                her his-target !  3 roll 30 * 30 + his-wait !  $78D his-cmd !  ['] st-keep-away behave
            else  to-default  then  endof
    $11 of  pick-target his-target !
            his-target @ with? if
                0 mode!  0 his-t1 !  0 his-t2 !  2 his-t3 !  his-yaw his-heading f!  30e his-turn f!
                30 his-wait !  $78D his-cmd !  ['] st-keep-away behave
            else  to-default  then  endof
    $13 of  0 mode!  90 his-t1 !  0 his-t2 !  his-yaw his-heading f!  5e his-turn f!  $60F his-cmd !  ['] st-scramble behave  endof
    $64 of  0 mode!  90 his-t1 !  0 his-t2 !  his-yaw his-heading f!  5e his-turn f!  $60F his-cmd !  ['] st-scramble behave  endof
    $14 of  0 mode!  0 his-cmd !  ['] st-1e48 behave  endof
    $15 of  0 mode!  30 his-t1 !  0 his-t2 !  random-level his-t3 !  his-yaw his-head-yaw f@ f+ angle-wrap his-heading f!
            turn-speed  3 roll 30 * 30 + his-wait !  $78D his-cmd !  ['] st-roam behave  endof
    $16 of  by-her? if  0 mode!  $68E his-cmd !  ['] st-come-to-command behave  else  to-default  then  endof
    $17 of  idle-action  endof
    $18 of  plain-floor? if  2 roll his-t1 !  4 $62D ['] st-tricks holding  else  to-default  then  endof
    $19 of  plain-floor? if  4 $62D ['] st-tricks holding  else  to-default  then  endof
    $1A of  plain-floor? if  4 $62D ['] st-tricks holding  else  to-default  then  endof
    $1B of  plain-floor? if  4 $62D ['] st-tricks holding  else  to-default  then  endof
    $1C of  plain-floor? if  900 his-wait !  4 $62D ['] st-tricks holding  else  to-default  then  endof
    $1D of  0 mode!  target-her  0 his-cmd !  ['] st-1f28 behave  endof
    $1E of  0 mode!  target-her  0 his-cmd !  ['] st-1f88 behave  endof
    \ her commands and meetings (H2)
    $48 of  his-to-yaw f@ his-yaw f- angle-wrap fabs 6e deg>rad f/ f>s  dup his-t1 !  6 < if  1  else  0  then  his-t2 !
            $C mode!  target-her  8 his-cmd !  ['] st-turn-with-fiona behave  endof
    $49 of  $C mode!  4 look!  $408 his-cmd !  ['] st-2198 behave  endof
    $4A of  $C mode!  4 look!  $408 his-cmd !  ['] st-2198 behave  endof
    $4B of  $C mode!  4 look!  $408 his-cmd !  ['] st-2198 behave  endof
    $4D of  after-4d  endof
    $4E of  0 his-hold-call !  after-coming-out  endof
    $63 of  by-her? if  0 mode!  $60F his-cmd !  0 his-t1 !  2 his-t2 !  -1 his-to-anim !  ['] st-set-off behave
            else  to-default  then  endof
    $71 of  0 mode!  4 look!  $400 his-cmd !  ['] st-22b8 behave  endof
    $7A of  0 mode!  trust-wait his-wait !  60 his-t1 !  4 look-now!  $88 his-cmd !  ['] st-2388 behave  endof
    \ at the stalker (0x1F..0x23): H3
    $1F of  give-up  endof  $20 of  give-up  endof  $21 of  give-up  endof  $22 of  give-up  endof
    $23 of  give-up  endof
    \ out of the room away from her (0x24 / 0x25): with the doors he may use (later)
    $24 of  to-default  endof
    $25 of  $10 0 want  endof
    $26 of  4 $7AF ['] st-after-27 holding  endof
    $27 of  4 $7AF ['] st-after-27 holding  endof
    $28 of  low-floor? if  to-default  else  4 $7AF ['] st-after-29 holding  then  endof
    $29 of  low-floor? if  to-default  else  4 $7AF ['] st-after-29 holding  then  endof
    $2A of  low-floor? if  to-default  else  4 $7AF ['] st-after-2b holding  then  endof
    $2B of  low-floor? if  to-default  else  4 $7AF ['] st-after-2b holding  then  endof
    \ off screen (out of the room being played): toward her, waiting, out at random, to a noise
    $2C of  route-played  endof
    $2D of  route-played  endof
    $39 of  route-played  endof
    $2E of  6 mode!  $DF his-cmd !  idle-wait trust-of his-wait !  ['] st-nothing behave  endof
    $2F of  6 mode!  $88 his-cmd !  his-wanted @ his-wait !  ['] st-nothing behave  endof
    $30 of  6 mode!  $80 his-cmd !  his-wanted @ his-wait !  ['] st-nothing behave  endof
    $31 of  6 mode!  $DF his-cmd !  his-wanted @ his-wait !  ['] st-nothing behave  endof
    $32 of  false off-random  endof
    $33 of  to-noise  endof
    $35 of  true off-random  endof
    $36 of  6 mode!  0 his-cmd !  ['] st-nothing behave  endof
    $34 of  6 mode!  $80 his-cmd !  60 his-wait !  ['] st-loud-noise behave  endof
    $37 of  6 mode!  0 his-cmd !  his-path path-clear  ['] st-to-default behave  endof
    $4C of  $15 0 want  endof
    $4F of  his-waiting @ 1 = if  900 his-hold-call !  then  after-call  endof
    $50 of  his-waiting @ 1 = if  900 his-hold-call !  then  after-call  endof
    $51 of  random-idle  endof
    $52 of  0 -1 set-mood  0 mode!  4 look-now!  1 his-2d !  $1518 his-wait !  0 his-cmd !  ['] st-knocked-down behave  endof
    $54 of  her his-target !
            by-her? fiona-mode @ dup 4 <> swap 3 <> and and if
                8 mode!  60 his-wait !  30e  his-mood @ 3 <> if  $88  else  $80  then  keep-near
            else  give-up  then  endof
    $55 of  by-her? if  her his-target !  0 mode!  20e $384 keep-near  else  to-default  then  endof
    $56 of  0 mode!  60 his-wait !  30e $7AF keep-near  endof
    $57 of  0 mode!  20e $7AF keep-near  endof
    $58 of  4 $68F ['] st-pose2 holding  endof
    \ at the stalkers and creatures: H3
    $59 of  give-up  endof
    $5A of  3 his-wanted !  give-up  endof
    $61 of  by-her? if  $54 $23 want  else  give-up  then  endof
    $62 of  3 his-wanted !  by-her? if  $54 $23 want  else  give-up  then  endof
    $67 of  by-her? if  0 mode!  target-her  0 his-t1 !  $78D his-cmd !  ['] st-step-away-fiona behave
            else  to-default  then  endof
    $6A of  0 mode!  0 his-t1 !  8 his-cmd !  ['] st-off-mesh behave  endof
    $6E of  -1 his-t1 !  0 his-t2 !  0 mode!  8 his-cmd !  ['] st-slope behave  endof
    $6F of  0 his-cmd !  ['] st-run-to-fiona behave  endof
    $70 of  30 his-t1 !  0 his-t2 !  0 mode!  4 look!  0 his-cmd !  ['] st-run behave  endof
    $73 of  0 mode!  0 his-cmd !  ['] st-anim-over behave  endof
    $76 of  0 mode!  0 his-cmd !  ['] st-play-anim behave  endof
    $77 of  6 mode!  8 his-cmd !  ['] st-nothing behave  endof
    $7D of  by-her? if  0 mode!  0 his-t1 !  $32D his-cmd !  ['] st-whine behave  else  to-default  then  endof
    $7E of  by-her? if  0 mode!  0 his-t1 !  $32D his-cmd !  ['] st-whine behave  else  to-default  then  endof
    $81 of  his-scent look-at his-look-yaw f!  his-look-pitch f!  8 $7AD ['] st-pose4 holding  endof
    $85 of  0 mode!  -1 his-target !
            by-her? her-dist 150e f< and if  her his-target !  then
            0 his-cmd !  ['] st-bark behave  endof
    \ the rest (her commands H2, the stalkers H3, the story's moves H4): the default
    >r  r@ if  to-default  then  r>
    endcase ;
: set-action-now ( act arg -- )   \ Hewie_SetAction
    his-next !  dup his-action !  0 his-pending !
    dup $76 <> over $77 <> and over 0<> and if  -1 his-yelp !  then
    10 roll 30 * 90 + his-wait !
    (set-action) ;
' set-action-now is set-action

\ ---- Hewie_AdjustAction: the action his situation makes of `act` ----
: attack-act? ( act -- flag )
    dup $1F $24 within  over $4E $51 within or  over $59 = or  over $5A = or  swap $75 = or ;
: adjust ( act -- act' )
    his-busy @ 0=  over 0= and  his-hp @ 0= and  his-cond @ 2 = and if  drop $52  then
    dup $52 = if
        here?  his-tri nav-flags self body-mask and 0<> and if
            drop 0  1 hp!  1 cond!
        else
            his-action @ dup $52 <> swap $74 <> and  anim-group 13 <> and if  10 mistreated+  then
        then
    then
    cutscene-active? if
        dup $24 = over $25 = or if  drop 5
        else dup $61 = over $62 = or if  drop $B
        else dup attack-act? if  drop $A  then then then
    then
    hewie-no-attack state-flag? fiona-occupied state-flag? or if
        dup attack-act? over $61 = or over $62 = or if  drop $A  then
    then
    his-hold-call @ if  dup $50 = over $4F = or if  drop $A  then  then
    his-cond @ 1 = if
        dup $13 = over $64 = or if  his-waiting @ 1 = if  drop $10  then
        else dup $16 = over $18 $1D within or over $26 = or over $28 = or over $2A = or if  drop $58
        then then
    then
    his-mood @ 2 = if
        dup $13 = over $64 = or if  his-waiting @ 1 = if  drop $10  then
        else dup $E = if  his-danger @ 2 = if  drop $F  then
        else dup $C = over $16 = or over $18 $1D within or if  drop $2A
        then then then
    then
    his-mood @ 1 = over $C = and if  drop $16  then
    his-smells @ over dup 5 = over 4 = or swap 1 = or and his-danger @ 0= and if  drop $81  then
    his-stay @ if  drop 4  then ;
' adjust is adjust-action
