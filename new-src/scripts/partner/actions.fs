\ partner/actions.fs - what Hewie does (src/game/hewie.c Hewie_SetAction, Hewie_AdjustAction):
\ each action sets up his move mode, where he looks, what may break in (h-cmd), the timers and
\ the behaviour that carries it out. The actions that belong to later phases - going for the
\ stalkers, leaving the room, fetching, being steered - fall back to the default action here.
IN: partner.actions
USING: engine game-state events.core events.words chars partner.core partner.tables partner.moves partner.states ;

: mode! ( n -- )  him character char.mode l! ;
: sees? ( cs -- flag )  with? ;   \ Hewie_WithChar2
\ give_up: nothing to do - wandering (6) while he is his own dog, else the default
: give-up ( -- )  hewie-control @ if  0  else  6  then  0 want ;
\ sWait: the timers by his trust
: trust-wait ( -- n )  h-trust @ 60 * 300 + ;

\ Hewie_PickTarget: whom he goes for - the stalker when in his room and he can get to it.
\ (The creatures, and whoever holds Fiona, come with the stalkers.)
: pick-target ( -- cs | -1 )
    pursuer @ dup with? 0= if  drop -1 exit  then
    dup c-tri over c-pos plan-to 0= if  drop -1  then ;

\ Hewie_IdleAction: a trick at random (not the last one), on plain floor
create idle-tricks  $18 , $19 , $1C , $1A , $1B ,
: idle-action ( -- )
    h-tri nav-flags 3 and if  to-default exit  then
    begin  5 roll cells idle-tricks + @  dup h-last-idle @ = while  drop  repeat
    dup h-last-idle !  start ;
\ Hewie_RandomIdle: lying by her (1, half the time), stand by (4) or settle (5)
: random-idle ( -- )
    100 roll dup 50 < if  drop 1  else  65 < if  4  else  5  then  then  start ;
\ Hewie_After4D: by his pose, praise-me (0x28) / lying (0x26) or a roll over (0x2A)
: after-4d ( -- )
    anim-group rnd01 0.5e f<                                         ( g coin )
    swap case
        0 of  h-cond 1 <> and if  $28  else  $2A  then  endof
        4 of  h-cond 1 <> and if  $28  else  $2A  then  endof
        1 of  if  $26  else  $2A  then  endof
        5 of  if  $26  else  $2A  then  endof
        2 of  h-cond 1 = or if  $26  else  $2A  then  endof
        6 of  h-cond 1 = or if  $26  else  $2A  then  endof
        >r drop $26 r>
    endcase  0 want ;
\ go_for (after a call or coming out): whom - none (0), the stalker (2, left to the caller)
: go-for ( -- n )
    pick-target dup h-target2 !  dup with? 0= if  drop 0 exit  then
    h-held @ 0= swap pursuer @ = and if  2 exit  then
    $5A 0 want  1 ;
: after-call ( -- )   \ Hewie_AfterCall (0x4F / 0x50)
    go-for case  0 of  6 0 want  endof  2 of  give-up  endof  endcase ;
: after-coming-out ( -- )   \ Hewie_AfterComingOut (0x4E)
    go-for case  0 of  hewie-control @ if  $87  else  6  then  0 want  endof  2 of  give-up  endof  endcase ;

\ a few behaviours only the actions start
: st-22e8 ( -- )  anim-done? if  $52 start  then  1 h-no-root ! ;   \ yelped down
: st-1c58 ( -- )
    anim-group if  -1 h-t2 +!  h-t2 @ 0= if  -1 stand-anim  then  exit  then
    settled? if  h-t1 @ start  then ;
: st-23d8 ( -- )
    settled? if
        anim-group if  -1 stand-anim  else  $1C03 play  30 h-t2 !  ['] st-1c58 behave  then
    then  1 h-no-root ! ;
: st-step-away-target ( -- )   \ Hewie_StateStepAwayTarget (action 0x82)
    h-target @ dup c-active? 0= if  drop 0 0 want exit  then
    dup c-room h-room <>  over c-tri 0< or if  drop 0 0 want exit  then
    dup c-pos h-pos vec-dist-xz 6e f> if  drop 0 0 want exit  then
    back-off ;

\ ---- Hewie_SetAction ----
: (set-action) ( act -- )
    case
    0 of  -1 h-target2 !  0 mode!  0 h-cmd !
          h-busy? if  4 look!  ['] st-1d88 behave
          else  0 h-2d !  1 h-pending !  4 look!  ['] st-1d98 behave  then  endof
    2 of  her sees? if  0 mode!  0 look!  her h-target !  trust-wait h-wait !  $688 h-cmd !  ['] st-1dc8 behave
          else  4 0 want  then  endof
    1 of  her sees? if  0 mode!  0 look!  her h-target !  $6AE h-cmd !  ['] st-1dc8 behave
          else  4 0 want  then  endof
    3 of  her sees? if  0 mode!  h-wanted @ h-wait !  0 look!  her h-target !  $688 h-cmd !  ['] st-1dc8 behave
          else  4 0 want  then  endof
    7 of  0 mode!  trust-wait h-wait !  pick-target h-target !  $58C h-cmd !  ['] st-steer behave  endof
    4 of  0 mode!  4 look!  $7AD h-cmd !  ['] st-pose4 behave  endof
    5 of  0 mode!  45 roll dup 20 < if  drop 1  else  dup 25 < if  drop 13  else  30 < if  11  else  12  then  then  then
          look!  $7AD h-cmd !  ['] st-pose4 behave  endof
    $14 of  0 mode!  0 h-cmd !  ['] st-1e48 behave  endof
    6 of  0 mode!  90 h-wait !  pick-target h-target !  $6AF h-cmd !  ['] st-face-scent behave  endof
    8 of  pick-target h-target !
          h-alert @ 2 =  h-target @ sees? or if  150 h-wait !  0 mode!  $7AF h-cmd !  ['] st-face-target behave
          else  6 0 want  then  endof
    $A of  h-target2 @ 0< if  pick-target h-target2 !  then
           h-target2 @ sees? if  0 mode!  2 roll 1+ h-t1 !  h-target2 @ h-target !  $7AF h-cmd !  ['] st-bark-at-target behave
           else  6 0 want  then  endof
    9 of  her sees? if  0 mode!  0 look!  her h-target !  300 h-wait !  $384 h-cmd !  ['] st-wait-for-fiona behave
          else  6 0 want  then  endof
    $B of  her sees? if  0 mode!  3 roll 3 + h-t1 !  0 look!  her h-target !  $384 h-cmd !  ['] st-bark-at-fiona behave
           else  6 0 want  then  endof
    $C of  her sees? if  0 mode!  2 h-t1 !  $68F h-cmd !  ['] st-slide-to-fiona behave  else  0 0 want  then  endof
    $D of  her sees? if  0 mode!  2 h-t1 !  $68F h-cmd !  ['] st-slide-to-fiona behave  else  0 0 want  then  endof
    $E of  her sees? if  0 mode!  2 h-t1 !  $68F h-cmd !  ['] st-slide-to-fiona behave  else  0 0 want  then  endof
    $F of  her sees? if  0 mode!  2 h-t1 !  $58F h-cmd !  ['] st-slide-to-fiona behave  else  0 0 want  then  endof
    $10 of  her sees? if
                0 mode!  0 h-t1 !  0 h-t2 !  random-level h-t3 !  h-yaw h-heading f!  turn-speed
                her h-target !  3 roll 30 * 30 + h-wait !  $78D h-cmd !  ['] st-keep-away behave
            else  0 0 want  then  endof
    $11 of  pick-target h-target !
            h-target @ sees? if
                0 mode!  0 h-t1 !  0 h-t2 !  2 h-t3 !  h-yaw h-heading f!  30e h-turn f!
                30 h-wait !  $78D h-cmd !  ['] st-keep-away behave
            else  0 0 want  then  endof
    $13 of  0 mode!  90 h-t1 !  0 h-t2 !  h-yaw h-heading f!  5e h-turn f!  $60F h-cmd !  ['] st-scramble behave  endof
    $64 of  0 mode!  90 h-t1 !  0 h-t2 !  h-yaw h-heading f!  5e h-turn f!  $60F h-cmd !  ['] st-scramble behave  endof
    $63 of  her sees? if  0 mode!  $60F h-cmd !  0 h-t1 !  2 h-t2 !  -1 h-to-anim !  ['] st-set-off behave
            else  0 0 want  then  endof
    $1D of  0 mode!  0 look!  her h-target !  0 h-cmd !  ['] st-1f28 behave  endof
    $15 of  0 mode!  30 h-t1 !  0 h-t2 !  random-level h-t3 !  h-yaw h-head-yaw f@ f+ angle-wrap h-heading f!
            turn-speed  3 roll 30 * 30 + h-wait !  $78D h-cmd !  ['] st-roam behave  endof
    $16 of  her sees? if  0 mode!  $68E h-cmd !  ['] st-come-to-command behave  else  0 0 want  then  endof
    $17 of  idle-action  endof
    $18 of  h-tri nav-flags 3 and if  0 0 want
            else  0 mode!  2 roll h-t1 !  4 look!  $62D h-cmd !  ['] st-tricks behave  then  endof
    $19 of  h-tri nav-flags 3 and if  0 0 want  else  0 mode!  4 look!  $62D h-cmd !  ['] st-tricks behave  then  endof
    $1A of  h-tri nav-flags 3 and if  0 0 want  else  0 mode!  4 look!  $62D h-cmd !  ['] st-tricks behave  then  endof
    $1B of  h-tri nav-flags 3 and if  0 0 want  else  0 mode!  4 look!  $62D h-cmd !  ['] st-tricks behave  then  endof
    $1C of  h-tri nav-flags 3 and if  0 0 want
            else  0 mode!  4 look!  900 h-wait !  $62D h-cmd !  ['] st-tricks behave  then  endof
    $1E of  0 mode!  0 look!  her h-target !  0 h-cmd !  ['] st-1f88 behave  endof
    \ at the stalker (0x1F..0x22), at a target (0x23): the stalkers' phase
    $1F of  give-up  endof  $20 of  give-up  endof  $21 of  give-up  endof  $22 of  give-up  endof
    $23 of  give-up  endof
    \ out of the room away from her (0x24 / 0x25): the rooms' phase
    $24 of  0 0 want  endof
    $25 of  $10 0 want  endof
    $26 of  0 mode!  4 look!  $7AF h-cmd !  ['] st-after-27 behave  endof
    $27 of  0 mode!  4 look!  $7AF h-cmd !  ['] st-after-27 behave  endof
    $28 of  h-tri nav-flags $80001 and if  0 0 want  else  0 mode!  4 look!  $7AF h-cmd !  ['] st-after-29 behave  then  endof
    $29 of  h-tri nav-flags $80001 and if  0 0 want  else  0 mode!  4 look!  $7AF h-cmd !  ['] st-after-29 behave  then  endof
    $2A of  h-tri nav-flags $80001 and if  0 0 want  else  0 mode!  4 look!  $7AF h-cmd !  ['] st-after-2b behave  then  endof
    $2B of  h-tri nav-flags $80001 and if  0 0 want  else  0 mode!  4 look!  $7AF h-cmd !  ['] st-after-2b behave  then  endof
    $34 of  6 mode!  $80 h-cmd !  60 h-wait !  ['] st-loud-noise behave  endof
    $37 of  6 mode!  0 h-cmd !  him c-path-clear  ['] st-to-default behave  endof
    $48 of  h-to-yaw f@ h-yaw f- angle-wrap fabs 6e deg>rad f/ f>s  dup h-t1 !  6 < if  1  else  0  then  h-t2 !
            $C mode!  0 look!  her h-target !  8 h-cmd !  ['] st-turn-with-fiona behave  endof
    $49 of  $C mode!  4 look!  $408 h-cmd !  ['] st-2198 behave  endof
    $4A of  $C mode!  4 look!  $408 h-cmd !  ['] st-2198 behave  endof
    $4B of  $C mode!  4 look!  $408 h-cmd !  ['] st-2198 behave  endof
    $4C of  $15 0 want  endof
    $4D of  after-4d  endof
    $4F of  h-waiting @ 1 = if  900 h-hold-call !  then  after-call  endof
    $50 of  h-waiting @ 1 = if  900 h-hold-call !  then  after-call  endof
    $4E of  0 h-hold-call !  after-coming-out  endof
    $51 of  random-idle  endof
    $52 of  0 -1 set-mode  0 mode!  4 look-now!  1 h-2d !  $1518 h-wait !  0 h-cmd !  ['] st-knocked-down behave  endof
    $53 of  h-target @ dup sees? if  c-mode dup 4 <> swap 3 <> and  else  drop false  then  if
                30e h-turn f!  8 mode!  0 h-t1 !  0 h-t2 !  h-yaw h-heading f!  60 h-wait !
                h-mood @ 3 <> if  $88  else  $80  then  h-cmd !  ['] st-keep-near behave
            else  give-up  then  endof
    $54 of  her h-target !
            her sees? her c-mode dup 4 <> swap 3 <> and and if
                30e h-turn f!  8 mode!  0 h-t1 !  0 h-t2 !  h-yaw h-heading f!  60 h-wait !
                h-mood @ 3 <> if  $88  else  $80  then  h-cmd !  ['] st-keep-near behave
            else  give-up  then  endof
    $55 of  her sees? if  her h-target !  0 mode!  0 h-t1 !  0 h-t2 !  h-yaw h-heading f!  20e h-turn f!
                $384 h-cmd !  ['] st-keep-near behave
            else  0 0 want  then  endof
    $56 of  0 mode!  30e h-turn f!  60 h-wait !  0 h-t1 !  0 h-t2 !  h-yaw h-heading f!  $7AF h-cmd !  ['] st-keep-near behave  endof
    $57 of  0 mode!  20e h-turn f!  0 h-t1 !  0 h-t2 !  h-yaw h-heading f!  $7AF h-cmd !  ['] st-keep-near behave  endof
    $58 of  0 mode!  4 look!  $68F h-cmd !  ['] st-pose2 behave  endof
    \ at the stalkers and creatures (0x59..0x60, 0x6D, 0x75, 0x7B, 0x87): the stalkers' phase
    $59 of  give-up  endof
    $5A of  3 h-wanted !  give-up  endof
    $61 of  her sees? if  $54 $23 want  else  give-up  then  endof
    $62 of  3 h-wanted !  her sees? if  $54 $23 want  else  give-up  then  endof
    $67 of  her sees? if  0 mode!  0 look!  her h-target !  0 h-t1 !  $78D h-cmd !  ['] st-step-away-fiona behave
            else  0 0 want  then  endof
    $6A of  0 mode!  0 h-t1 !  8 h-cmd !  ['] st-off-mesh behave  endof
    $6E of  -1 h-t1 !  0 h-t2 !  0 mode!  8 h-cmd !  ['] st-slope behave  endof
    $6F of  0 h-cmd !  ['] st-run-to-fiona behave  endof
    $70 of  30 h-t1 !  0 h-t2 !  0 mode!  4 look!  0 h-cmd !  ['] st-run behave  endof
    $71 of  0 mode!  4 look!  $400 h-cmd !  ['] st-22b8 behave  endof
    $73 of  0 mode!  0 h-cmd !  ['] st-anim-over behave  endof
    $74 of  0 mode!  0 h-cmd !  $2213 play  $66 make-sound  ['] st-22e8 behave  endof
    $76 of  0 mode!  0 h-cmd !  ['] st-play-anim behave  endof
    $77 of  6 mode!  8 h-cmd !  ['] st-nothing behave  endof
    $79 of  1 h-2d !  $B mode!  $1301 play  0 h-cmd !  4 look-now!  ['] st-2358 behave  endof
    $7A of  0 mode!  trust-wait h-wait !  60 h-t1 !  4 look-now!
            hewie-control @ if  8  else  $88  then  h-cmd !  ['] st-2388 behave  endof
    $7C of  pick-target h-target !
            h-target @ sees? if  30e h-turn f!  0 mode!  0 h-t1 !  0 h-t2 !  h-yaw h-heading f!
                $88 h-cmd !  ['] st-keep-near behave
            else  6 0 want  then  endof
    $7D of  her sees? if  0 mode!  0 h-t1 !  $32D h-cmd !  ['] st-whine behave  else  0 0 want  then  endof
    $7E of  her sees? if  0 mode!  0 h-t1 !  $32D h-cmd !  ['] st-whine behave  else  0 0 want  then  endof
    $80 of  0 mode!  8 h-cmd !  h-last-action @ h-t1 !  ['] st-23d8 behave  endof
    $81 of  0 mode!  8 look!  h-scent look-at h-look-yaw f!  h-look-pitch f!  $7AD h-cmd !  ['] st-pose4 behave  endof
    $82 of  h-target @ dup c-active? if  dup c-room h-room =  swap c-tri 0< 0= and  else  drop false  then  if
                0 mode!  0 look!  0 h-t1 !  8 h-cmd !  ['] st-step-away-target behave
            else  0 0 want  then  endof
    $85 of  0 mode!  -1 h-target !
            her sees? h-pos her c-pos vec-dist 150e f< and if  her h-target !  then
            h-target @ 0< pursuer @ sees? and if
                h-pos pursuer @ c-pos vec-dist 150e f< if  pursuer @ h-target !  then
            then  0 h-cmd !  ['] st-bark behave  endof
    \ under a script (Hewie_Requests): animations with their root motion, walks, turns, a bark
    $3B of  0 mode!  4 look!  0 h-cmd !  h-to-tri @ play  h-done  ['] st-root-motion behave  endof
    $3C of  0 mode!  4 look!  0 h-cmd !  h-to-tri @ h-to-anim @ play-blend  h-done  ['] st-root-motion behave  endof
    $47 of  0 mode!  4 look!  0 h-cmd !  h-to-tri @ h-to-anim @ play-blend  h-done  ['] st-root-motion behave  endof
    $3D of  0 mode!  4 look!  0 h-cmd !  h-to-tri @ h-to-anim @ play-blend8  h-done  ['] st-root-motion behave  endof
    $3E of  0 mode!  4 look!  0 h-cmd !  h-to-anim @ stand-anim  h-done  ['] st-root-motion behave  endof
    $3F of  0 mode!  0 h-cmd !  0 h-t1 !  0 h-t2 !  ['] st-set-off behave  endof
    $40 of  0 mode!  0 h-cmd !  0 h-t1 !  2 h-t2 !  ['] st-set-off behave  endof
    $41 of  0 mode!  0 h-cmd !  ['] st-2138 behave  endof
    $42 of  0 mode!  0 h-cmd !  ['] st-2138 behave  endof
    $43 of  him cells move-slot + @ dup c-active? if
                0 mode!  0 h-cmd !  him swap c-pos c-heading-to h-to-yaw f!  ['] st-turn-start behave
            else  drop h-done  then  endof
    $44 of  0 mode!  0 h-cmd !  ['] st-turn-start behave  endof
    $45 of  0 mode!  0 h-cmd !  ['] st-2168 behave  endof
    $46 of  0 mode!  0 h-cmd !  ['] st-head-for-spot behave  endof
    $7F of  h-to 4 + sf@ lp-rise f!  h-2d @ 1 = if  1  else  0  then  h-t2 !
            0 mode!  0 h-cmd !  ['] st-run-for-spot behave  endof
    \ the rest (leaving rooms, hiding, fetching, being steered): later phases
    >r  r@ 0= 0= if  0 0 want  then  r>
    endcase ;
: set-action-now ( act arg -- )   \ Hewie_SetAction
    h-next !  dup h-action !  0 h-pending !
    dup $76 <> over $77 <> and over 0<> and if  -1 h-yelp !  then
    10 roll 30 * 90 + h-wait !
    (set-action) ;
' set-action-now is set-action

\ ---- Hewie_AdjustAction: the action his situation makes of `act` ----
: attack-act? ( act -- flag )
    dup $1F $24 within  over $4E $51 within or  over $59 = or  over $5A = or  swap $75 = or ;
: adjust ( act -- act' )
    hewie-control @ if  exit  then
    h-busy? 0=  over 0= and  h-hp 0= and  h-cond 2 = and if  drop $52  then
    dup $52 = if
        h-disabled? 0=  h-tri nav-flags him c-mask and 0<> and if
            drop 0  1 him character char.hp l!  1 him character char.cond l!
        else
            h-action @ dup $52 <> swap $74 <> and  anim-group 13 <> and if  10 spoiled counter+  then
        then
    then
    cutscene-active? if
        dup $24 = over $25 = or if  drop 5
        else dup $61 = over $62 = or if  drop $B
        else dup attack-act? if  drop $A  then then then
    then
    $13 state-flag? $2B state-flag? or if
        dup attack-act? over $61 = or over $62 = or if  drop $A  then
    then
    h-hold-call @ if  dup $50 = over $4F = or if  drop $A  then  then
    h-cond 1 = if
        dup $13 = over $64 = or if  h-waiting @ 1 = if  drop $10  then
        else dup $16 = over $18 $1D within or over $26 = or over $28 = or over $2A = or if  drop $58
        then then
    then
    h-mood @ 2 = if
        dup $13 = over $64 = or if  h-waiting @ 1 = if  drop $10  then
        else dup $E = if  game-mode @ 2 = if  drop $F  then
        else dup $C = over $16 = or over $18 $1D within or if  drop $2A
        then then then
    then
    h-mood @ 1 = over $C = and if  drop $16  then
    h-look @ over dup 5 = over 4 = or swap 1 = or and game-mode @ 0= and if  drop $81  then
    h-stay @ if  drop 4  then ;
' adjust is adjust-action
