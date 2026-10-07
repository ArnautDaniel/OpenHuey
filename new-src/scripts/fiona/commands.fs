\ fiona/commands.fs - Fiona and Hewie (src/game/fiona.c): her commands to him (the right
\ stick's gestures: Fiona_HewieCommandAction), her calls and lines, the actions that go with
\ them (Fiona_StateActionOver and the looks, gestures and held commands after it), the joint
\ actions the game queues for her (Fiona_JointAction), what she is asked to do
\ (Fiona_StateBlock) and her controls' commands (Fiona_ControlCommand). Also walking to a spot
\ and facing a way there (Fiona_DoorFrame: the doors use it too).
\
\ The right stick's gestures on the keyboard: 1 up (go / attack), 2 down (come back), 3 R3
\ (stay / come), 4 right (praise), 5 left (scold).
IN: fiona.commands
USING: engine game-state events.core events.words chars relations fiona.core fiona.moves fiona.brain ;

\ ---- Hewie as she sees him (his own fields: partner.core) ----
: dog-action ( -- n )  partner.core:h-action @ ;
: dog-mood ( -- n )  partner.core:h-mood @ ;
: dog-mode ( -- n )  dog character char.mode sl@ ;
: dog-sub ( -- n )  dog character char.sub sl@ ;
: dog-cond ( -- n )  dog character char.cond sl@ ;
: dog-group ( -- g )  partner.core:anim-group ;
: dog-dist ( F: -- d )  f-pos dog c-pos vec-dist ;
\ hewie_react (Fiona_HewieReact): with her, he spends some of his patience and may be pleased
\ (D_003B2520: cost, 1 in n)
create reactions  300 , 1 , 300 , 1 , 300 , 1 , 750 , 1 , 150 , 1 , 1500 , 1 , 600 , 1 , 600 , 1 ,
    0 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 150 , 0 , 300 , 1 , 300 , 0 , 150 , 1 ,
: hewie-react ( n -- )
    f-dog-ok @ 1 <> if  drop exit  then
    2* cells reactions +
    partner.core:h-waiting @ 1 = if  dup @ negate partner.core:h-obey +!  0 partner.core:h-obey-marked !  then
    cell+ @ dup 0> dog-dist 30e f< and if
        s>f rnd f* f>s 0= if  1 partner.core:h-praise-due !  then
    else  drop  then ;

\ ---- her lines (fiona_voice: plain; Fiona_OrderLine; Fiona_CallHewie) ----
: line ( id -- )  5 0 0 f-sound ;
: order-line ( -- )
    $25 state-flag? if  rnd 0.5e f< if  $33  else  $39  then  line exit  then
    f-sub case
        $2C of  game-mode @ case  2 of  $33 line  endof  1 of  $31 line  endof  0 of  $32 line  endof  endcase
                f-dog-ok @ 0= if  $20 noise-here  then  endof
        $2D of  $30 line  endof
    endcase ;
: praise-scold-line ( -- )   \ 0x27: by what he does
    dog-action
    dup $22 = over $20 = or over $21 = or over $1F = or over $59 = or over $53 = or
    dog-group 8 - 2 u< and if  drop $36 line exit  then
    dup $25 = over $24 = or over $63 = or over $13 = or over $A = or over $22 = or
    over $20 = or over $21 = or over $1F = or if  drop $36 line exit  then
    8 = if  $35 line exit  then
    dog-mode 0= dog-sub 3 = and if  $35 line exit  then
    rnd 0.5e f< if  $35  else  $36  then  line ;
: call-hewie-line ( -- )   \ Fiona_CallHewie
    f-sub case
        $23 of  game-mode @ 0= if  $2E  else  $2F  then  line  endof
        $24 of  $37 line  endof
        $25 of  $34 line  endof
        $27 of  praise-scold-line  endof
        $28 of  rnd 0.75e f< if  $3A  else  $32  then  line  endof
        $29 of  dog-cond 2 = if  $33  else  $3A  then  line  endof
        $2A of  $39 line  endof
        $2B of  rnd 0.75e f< if  $3B  else  $30  then  line  endof
        $2C of  $31 line  f-dog-ok @ 0= if  $20 noise-here  then  endof
        $2D of  game-mode @ 2 = if  $2F  else  $30  then  line  endof
        $2E of  $2F line  endof
        $2F of  $3B line  endof
    endcase ;
' call-hewie-line is call-hewie

\ ---- the command's start (Fiona_MarkActionStart: +0x1AD6BC, its heading, triangle, place) ----
variable act-tri  fvariable act-yaw  vector act-at
: mark-action-start ( code -- )
    dup f-sub!  f-cmd-code !  f-heading f@ act-yaw f!  f-tri act-tri !  act-at f-pos vec-copy ;

\ ---- Fiona_CommandHewie: the request 13 (the command, her point - x and z in 1/100000 - its
\ triangle and her heading; for "go", 0x23, 5 short of where he'd have to stop when that is
\ further), unless he is busy or the game holds him; then his reaction ----
create ch-at 12 allot
: command-hewie ( -- )
    dog relations:req-of 0= $25 state-flag? 0= and if
        f-sub $23 = if
            act-yaw f@ 15e dog -1 c-free                            ( F: d )
            fdup 5e f> if
                5e f-  act-yaw f@ fswap ch-at act-at vec-ahead
                me ch-at $29020008 c-tri-to dup 0< if  drop  else  act-tri !  act-at ch-at vec-copy  then
            else  fdrop  then
        then
        hewie-control @ 0= dog relations:req-of 7 <> and if
            act-yaw f@  $D f-sub
            act-at sf@ 100000e f* f>s  act-at 8 + sf@ 100000e f* f>s  act-tri @  dog relations:req-set
        then
    then
    f-sub case
        $2D of  0 hewie-react  endof  $23 of  0 hewie-react  endof
        $2C of  1 hewie-react  endof
        $27 of  2 hewie-react  endof  $25 of  2 hewie-react  endof
        $2F of  3 hewie-react  endof  $29 of  4 hewie-react  endof
        $30 of  14 hewie-react  endof  $2E of  15 hewie-react  endof
    endcase ;

\ ---- Fiona_CallAction / Fiona_HewieCommandAction: the action for a command (0 up, 1 down, 2
\ R3, 3 right, 4 left) ----
: facing-diff ( F: a -- d )  f-yaw f- angle-wrap fabs ;   \ fiona_turn_to
: call-action ( -- code )
    game-mode @ 2 = if  f-dog-ok @ 1 = dog-mode 8 = and if  $2E  else  $2D  then  exit  then
    $23 ;   \ (game mode 1: a creature 10 ahead - with the creatures)
: command-code ( cmd -- code )
    case
        0 of  call-action  endof
        1 of  $2C  endof
        2 of  game-mode @ 0<> f-dog-ok @ 1 <> or if  $27
              else dog-dist 30e f< 0= if  $27
              else dog-dist 12e f< 0= if  $25
              else dog-action $4B = dog-sub 3 = dog-action 2 = and or if  $24  else  $25  then
              then then then  endof
        3 of  f-dog-ok @ 1 <> dog-dist 15e f< 0= or if  $29
              else dog-cond 2 = if  me dog c-pos c-heading-to facing-diff 60e deg>rad f< if  $2A  else  $29  then
              else game-mode @ 0<> dog-mood 3 = or if  $29  else  $28  then
              then then  endof
        4 of  game-mode @ 0<> f-dog-ok @ 1 <> or dog-dist 15e f< 0= or dog-mood 3 = or
              if  $2F  else  $2B  then  endof
        >r -1 r>
    endcase ;

\ ---- Fiona_DoorFrame: walking to a spot (spot / spot-tri) and facing f-heading there: 0 there,
\ 1 on the way, -1 she can't (no way, a wall, the pursuer). Far off she follows the planned
\ way; within 7 she settles how to come round to the heading - a quarter turn (0x400 / 0x401)
\ or a half turn (0x402) on the way, else a straight last part - and ends with a stop ----
vector spot  variable spot-tri
variable wk-flags  variable wk-n  fvariable wk-step  fvariable wk-turn  fvariable wk-speed
\ (door_walk) to the spot (tri, point, heading), then on in `next`
: walk-spot ( tri next -- ) ( F: x y z yaw -- )
    f-heading f!  spot vec!  swap spot-tri !  me c-path-end  0 wk-flags !  behave ;
: turn-left-to ( F: -- |d| )  f-heading f@ f-yaw f- angle-wrap fabs ;
: wk? ( bit -- flag )  wk-flags @ and 0<> ;
: wk+ ( bit -- )  wk-flags @ or wk-flags ! ;
: turn-anim ( anim -- ) ( F: k -- )   \ walk_turn_anim: blended with the standing pose by k
    f-mode 0= if  1 f-sub!  then             \ (under 0.5: half of it, the step doubled)
    dup f-entry 8 or >r >r  game-mode @ 2 = if  5  else  0  then  r> r> f-start   \ (Motion_PlayOwnBlend)
    fdup 1e f< if
        fdup 0.5e f< if  fdrop 0.5e f-weight!  wk-speed f@ 2e f* wk-speed f!  $100 wk+
        else  f-weight!  then
    else  fdrop 1e f-weight!  $100 wk+  then ;
: arrive-spot ( -- 0 )   \ walk_arrive
    spot-tri @ me c-tri!  me c-pos spot vec-copy  me c-sync  f-heading f@ f-yaw!  $10 wk+  0 ;
create wk-ahead 12 allot
: rest-of-way ( F: -- d )  me path-rest ;
: walk-to-spot ( -- r )
    $10 wk? if  0 exit  then
    8 wk? 0= if
        me path-left? 0= if
            me spot-tri @ spot -1 c-plan 0= if  f-root-move -1 exit  then
        then
        rest-of-way 7e f< if
            f-settled? 0= if  f-root-move  me c-path-end  1 exit  then
            8 wk+
            f-heading f@  spot sf@ f-pos sf@ f-  spot 8 + sf@ f-pos 8 + sf@ f-  fatan2  f- angle-wrap   ( F: d )
            fdup fabs 45e deg>rad f> if
                fdup fabs 135e deg>rad f> if  4 wk+  else  2 wk+  fdup f0< if  1 wk+  then  then
            then  fdrop
        then
    then
    8 wk? 0= if   \ on the way: the step by how well she faces its next point, turning 8 degrees
        f-settled? if  walk-look  then
        3e me path-ahead drop  me pa-pos c-heading-to                ( F: yaw )
        root@ rm-z f@ 0e fmax                                        ( F: yaw fwd )
        fover f-yaw f- fcos 1e f+ 2e f/ f* wk-step f!
        8e deg>rad f-turn-toward fdrop
    else
        $60 wk? 0= if
            rest-of-way fdup 0.5e f< if
                fdup 0.2e f* wk-step f!  fdrop  turn-left-to 0.2e f* wk-turn f!  $20 wk+  -1 idle-anim
            else
                $40 wk+
                4 wk? if
                    0e wk-step f!  fdup 3.15e f/ wk-speed f!  fdrop  turn-left-to 0.05e f* 2e f* wk-turn f!
                    $402 wk-speed f@ turn-anim
                else 2 wk? 0= if
                    10 wk-n !  fdup 0.072e f* wk-step f!  fdrop  turn-left-to 0.072e f* wk-turn f!  walk-look
                else
                    1 wk? 0= if  fdup 4.56e f/ wk-speed f!  $400 wk-speed f@ turn-anim
                    else  fdup 3.94e f/ wk-speed f!  $401 wk-speed f@ turn-anim  then
                    fdrop  0e wk-step f!  turn-left-to 0.045e f* 2e f* wk-turn f!
                then then
            then
        then
        6 wk? if   \ turning on the way: the step is the turn's root motion
            0e wk-step f!
            f-settled? if
                root@  2 wk? if  rm-x f@  else  rm-z f@  then  fabs
                $100 wk? if  wk-speed f@ f*  then  0.05e fmax wk-step f!
            then
        else  wk-step f@ 0.05e fmax wk-step f!  then
        wk-turn f@ 0.5e deg>rad fmax wk-turn f!
    then
    \ the step along the way
    8 wk? if  f-heading f@ wk-turn f@ f-turn-toward f0=  else  false  then   ( turned? )
    wk-step f@ me path-ahead                                         ( turned? i )
    pa-pos me c-mask v-tri-in                                        ( turned? i tri )
    f-2b @ 0= if  dup 0< if  drop 2drop -1 exit  then  then
    dup 0< if  drop pa-pos v-tri  then
    me c-tri!  me c-pos pa-pos vec-copy  me c-sync  me path-i!        ( turned? )
    f-pu-ok @ 1 = if  me pursuer-slot @ 0e 0e c-touching? if  drop -1 exit  then  then
    me dog 0e 0e c-touching? if  1 f-2a !  then
    8 wk? 0= if  drop 1 exit  then
    -1 wk-n +!
    f-settled? 0= if  drop 1 exit  then
    6 wk? if
        me path-left? if  drop 1 exit  then
        if  $20 wk? if  arrive-spot exit  then  $20 wk+  then
        f-group if  game-mode @ 2 = if  5  else  0  then  5 -1 f-play-blend  then
        1 exit
    then
    $20 wk? if  if  arrive-spot  else  1  then  exit  then  drop
    wk-n @ 0> if  1 exit  then
    wk-flags @ $60 and $40 <> if  1 exit  then
    $20 wk+  -1 idle-anim  1 ;

\ ---- after a command's gesture (Fiona_StateActionOver and the looks after it) ----
variable look-cmd   \ +0x1AD664: the one the command looked at (-1 none)
: in-sight? ( cs -- flag )   \ fiona_in_sight: walking straight from her reaches it (no mask)
    >r  f-tri f-pos r@ c-pos 0 v-walk  r> c-tri = ;
: look-cmd! ( cs -- )  look-cmd ! ;
: keep-looking ( -- )   \ the one looked at stays the one to face
    look-cmd @ 0< 0= if  1 f-look-on !  look-cmd @ f-look-who !  then ;
: idle-after-command ( -- )   \ Fiona_IdleAfterCommand
    $25 state-flag? if  rnd 0.5e f< if  1  else  $C02  then  -1 f-play-table exit  then
    f-sub case
        $29 of  $C02  endof  $2F of  $C02  endof  $2E of  $C0E  endof  $23 of  $C04  endof
        $24 of  $C06  endof  $2C of  $C02  endof  $2A of  $C0A  endof  $25 of  $C03  endof
        $27 of  $C03  endof  $2D of  $C00  endof
        >r 0 r>
    endcase  ?dup if  -1 f-play-table  then ;
: st-look-around ( -- )   \ Fiona_StateLookAround (0xC0E, then 0xC0F)
    keep-looking
    f-events 2 and if  command-hewie  then
    f-end? if  f-anim@ $C0E = if  $C0F -1 f-play exit  then  to-idle  then ;
: st-look-end ( -- )   \ Fiona_StateLookEnd
    $25 state-flag? if  -1 look-cmd !  else  keep-looking  then
    f-settled? if  idle-after-command  ['] st-look-around behave  then ;
defer st-held-start  ' noop is st-held-start
defer st-walk-then-gesture  ' noop is st-walk-then-gesture
: st-action-over ( -- )   \ Fiona_StateActionOver
    f-settled? if
        -1 look-cmd !
        f-sub case
            $23 of  f-dog-ok @ 1 = dog in-sight? and if  dog look-cmd!  then  ['] st-look-end behave  endof
            $2D of  f-pu-ok @ 1 = pursuer-slot @ dup 0< 0= swap in-sight? and if  pursuer-slot @ look-cmd!
                    else f-dog-ok @ 1 = dog in-sight? and if  dog look-cmd!  then then
                    ['] st-look-end behave  endof
            $2A of  f-dog-ok @ 1 = dog in-sight? and if  dog look-cmd!  then  ['] st-walk-then-gesture behave  endof
            $2E of  f-dog-ok @ 1 = dog in-sight? and if  dog look-cmd!  then  ['] st-look-end behave  endof
            $29 of  f-dog-here @ 1 = f-dog-ok @ 1 = and dog in-sight? and if  dog look-cmd!  then  ['] st-look-end behave  endof
            $2C of  f-dog-here @ 1 = f-dog-ok @ 1 = and dog in-sight? and if  dog look-cmd!  then  ['] st-look-end behave  endof
            $2F of  f-dog-here @ 1 = f-dog-ok @ 1 = and dog in-sight? and if  dog look-cmd!  then  ['] st-look-end behave  endof
            $27 of  f-dog-here @ 1 = f-dog-ok @ 1 = and if  dog look-cmd!  then  ['] st-look-end behave  endof
            $25 of  f-dog-here @ 1 = if  dog look-cmd!  then  ['] st-look-end behave  endof
            $24 of  2 4 me dog 0 relations:cmd-give if  ['] st-held-start behave  else  to-idle  then  endof
            $2B of  2 0 me dog 0 relations:cmd-give if  ['] st-held-start behave  else  $2F f-sub!  then  endof
            $28 of  2 2 me dog 0 relations:cmd-give if  ['] st-held-start behave  else  $29 f-sub!  then  endof
        endcase
    then
    f-root-move ;

\ ---- the gesture for "praise from a step away" (0x2A: Fiona_StateWalkThenGesture ..) ----
: caught? ( -- flag )   \ fiona_caught: the pursuer has her
    f-pu-ok @ 1 = if  me pursuer-slot @ 0e 0e c-touching?  else  false  then ;
: st-gesture-end ( -- )
    caught? if  to-idle exit  then  f-end? if  to-idle  then  f-root-move ;
: st-gesture ( -- )
    caught? if  to-idle exit  then  keep-looking
    f-end? if  command-hewie  $C0C -1 f-play  ['] st-gesture-end behave  then  f-root-move ;
: st-gesture-lead-in ( -- )
    caught? if  to-idle exit  then  keep-looking
    f-end? if  $C0B -1 f-play  ['] st-gesture behave  then  f-root-move ;
:noname ( -- )   \ Fiona_StateWalkThenGesture
    caught? if  to-idle exit  then  keep-looking
    f-settled? 0= if  walk-look exit  then
    $C0A -1 f-play-table  ['] st-gesture-lead-in behave ; is st-walk-then-gesture

\ ---- the held commands (0x24 stay, 0x28 praise, 0x2B scold: by his side) ----
variable held-n   \ +0x1AD6C0: the praise's repeats
: dog-off? ( -- flag )  f-dog-ok @ 0=  dog-action $48 <> or ;   \ held_off
: st-held-command ( -- )
    game-mode @ 0= f-sub $28 = and f-cmd @ 3 = and f-dog-ok @ 1 = and dog-mode $C = and f-anim@ $C08 = and if
        2 held-n !
    then
    f-end? if
        f-sub case
            $28 of
                f-anim@ case
                    $C07 of  1 held-n !  game-mode @ 0= f-dog-ok @ 1 = and dog-mode $C = and if  3 held-n !  then
                             $C08 -1 f-play  endof
                    $C08 of  -1 held-n +!  held-n @ 0= if  $C09  else  $C08  then  -1 f-play  endof
                    $C09 of  6 hewie-react  to-idle  endof
                endcase  endof
            $2B of  5 hewie-react  to-idle  endof
            $24 of  7 hewie-react  to-idle  endof
        endcase
    then
    f-root-move ;
defer st-wait-hewie  ' noop is st-wait-hewie
: st-held-gesture ( -- )
    f-req@ 7 = if  0 me character char.req l!  ['] st-wait-hewie behave exit  then
    f-dog-ok @ 0= if  f-root-move  to-idle exit  then
    f-sub case  $2B of  $C0D  endof  $28 of  $C07  endof  $24 of  $C06  endof  >r 0 r>  endcase
    ?dup if  -1 f-play-table  then
    ['] st-held-command behave ;
:noname ( -- )   \ Fiona_StateWaitHewie: him ready (0x48, sitting, settled), the command given
    dog-off? if  f-root-move  to-idle exit  then
    f-settled? 0= if  f-root-move exit  then
    dog-group 1 <> if  exit  then
    f-sub case  $2B of  1  endof  $28 of  3  endof  $24 of  5  endof  >r -1 r>  endcase
    dup 0< if  drop f-root-move  to-idle exit  then
    2 swap me dog 0 relations:cmd-give if  ['] st-held-gesture behave
    else  f-root-move  to-idle  then ; is st-wait-hewie
: st-held-stopped ( -- )
    dog-off? if  f-root-move  to-idle exit  then
    f-settled? if  ['] st-wait-hewie behave  then ;
: st-held-walk ( -- )
    dog-off? if  f-root-move  to-idle exit  then
    walk-to-spot  dup 0< if  drop f-root-move  to-idle exit  then
    0= if  -1 idle-anim  ['] st-held-stopped behave  then ;
:noname ( -- )   \ Fiona_StateHeldStart: by his spot (the move's tri / heading / point), facing him
    f-req@ 7 = if
        0 me character char.req l!  f-root-move
        f-sub case
            $2B of  $2F f-sub!  ['] st-action-over behave  endof
            $28 of  $29 f-sub!  ['] st-action-over behave  endof
            $24 of  to-idle  endof
        endcase  exit
    then
    f-dog-ok @ 0= if  f-root-move  to-idle exit  then
    me cells move-a + @  ['] st-held-walk
    me character char.target vec@  me character char.face sf@ pi f+ angle-wrap
    walk-spot ; is st-held-start

\ ---- turning for a request (0xC 6: Hewie behind her - Fiona_StateRequestTurn) ----
fvariable rt-yaw  fvariable rt-step
: st-idle-at-event ( -- )  f-end? if  to-idle  then ;
: st-turn-until-done ( -- )
    f-settled? 0= if  rt-yaw f@ rt-step f@ f-turn-toward fdrop exit  then
    rt-yaw f@ f-yaw!  ['] st-idle-at-event behave ;
: st-request-turn ( -- )
    $8000 -1 10 8 f-start
    me character char.face sf@ rt-yaw f!  rt-yaw f@ f-yaw f- angle-wrap fabs 0.1e f* rt-step f!
    ['] st-turn-until-done behave ;

\ ---- Fiona_JointAction: a joint action the game queued for her - kind 1 (meeting someone:
\ her place by them, kMeetOffsets), kind 2 type 6 (Hewie behind her: she turns) ----
fvariable ja-base  create ja-at 12 allot
: ja-try ( F: ang -- flag )   \ the spot behind her at that heading is open and reaches Hewie
    fdup ja-base f!
    2e 6.7e ja-base f@ rotate-by  f-pos 8 + sf@ f+ ja-at 8 + sf!  f-pos sf@ f+ ja-at sf!   \ (D_003B24A8: 2, 6.7)
    f-pos 4 + sf@ ja-at 4 + sf!
    ja-at me c-mask v-tri-in dup 0< if  drop false exit  then
    dup  dog c-tri dog c-pos ja-at $28020018 v-walk = 0= if  drop false exit  then
    dog cells move-a + !  ja-base f@ dog character char.face sf!
    ja-base f@ pi f+ angle-wrap me character char.face sf!
    me relations:cmd-start
    me relations:req2# >r  $C r@ l!  6 r@ 4 + l!  r> 8 + 24 0 fill
    true ;
: joint-action ( -- )
    me relations:cmd? 0= if  exit  then
    me relations:cmd-other c-ok? 0= me relations:cmd-other c-here? 0= or  8 state-flag? or if
        me relations:cmd-cancel exit
    then
    me relations:cmd-kind 2 = me relations:cmd-arg 6 = and f-mode 0= and if
        dog character char.face sf@                                ( F: base )
        19 0 do
            fdup i 10 * s>f deg>rad f- angle-wrap ja-try if  fdrop unloop exit  then
            i 0<> i 18 <> and if  fdup i 10 * s>f deg>rad f+ angle-wrap ja-try if  fdrop unloop exit  then  then
        loop  fdrop
    then
    me relations:cmd-cancel ;
' joint-action is fiona.brain:joint-action

\ ---- Fiona_ControlCommand: her command (calls in a panic; otherwise the command's action) ----
defer panic-control ( -- done? )   ' false is panic-control   \ (the panic's stumble / attack: fiona.panic)
: control-command ( -- )
    f-mode 0= if  panic-control if  exit  then  then
    f-cmd @ dup -1 = if  drop exit  then
    f-fear-bits @ 2 and  f-mode 4 = f-sub dup 9 = swap $12 = or and  or if
        drop   \ a call for Hewie
        f-req@ 0= hewie-control @ 0= and dog relations:req-of 7 <> and if
            0e  $D $30 0 0 0 dog relations:req-set
        then
        $38 5 0 0 f-sound  60 f-busy-t !  exit
    then
    f-mode if  drop exit  then
    f-act @ dup $E = over 1 = or swap $F = or if  drop exit  then
    command-code dup -1 = if  drop exit  then
    dup $2C = over $2D = or  f-sub 0<> and  over $2D = game-mode @ 1 = and 0= and if
        mark-action-start  command-hewie  order-line
        $25 state-flag? 0= if  30 f-target-t !  dog f-target !  then
        60 f-busy-t !  exit
    then
    $D f-mode!  $C f-act !  mark-action-start  ['] st-action-over behave ;
' control-command is fiona.brain:control-command

\ ---- Fiona_StateBlock: what she is asked to do ----
: st-cmd-done ( -- )  f-settled? if  1 f-e1 !  then ;   \ Fiona_StateCmdDone
defer react ( -- done? )   ' false is react      \ (4: a blow, a grab - with the stalkers)
defer door-request ( kind -- )  ' drop is door-request   \ (2 / 9 a door, 3 a ladder: fiona.doors)
defer kick-request ( -- )  ' noop is kick-request         \ (8 0x1A: fiona.kick)
defer caught-request ( -- )  ' noop is caught-request     \ (0xB: 0x22 flee - fiona.panic; 0x20 / 0x21 caught)
: state-block ( -- )
    f-req@ case
        5 of  0 f-act !  0 f-mode!  me 1 relations:req-word-of if  ['] st-cmd-done  else  ['] idle-step-state  then  behave
              0 me character char.req l!  exit  endof
        4 of  react drop  0 me character char.req l!  exit  endof
        7 of  exit  endof
    endcase
    f-req@ $C = me 1 relations:req-word-of 6 = and if
        $C f-mode!  $10 f-act !  ['] st-request-turn behave  0 me character char.req l!  exit
    then
    f-mode 0= f-fear f@ 100e f< and f-act @ dup $E <> over 1 <> and swap $F <> and and if
        f-req@ case
            2 of  2 door-request  endof
            9 of  9 door-request  endof
            3 of  3 door-request  endof
            8 of  kick-request  endof
            $B of  caught-request  endof
        endcase
    then
    0 me character char.req l! ;
' state-block is fiona.brain:state-block

\ ---- the gestures (keys 1..5 for the right stick's flicks; Gesture_Update) ----
: gesture ( -- cmd )
    key: 1 key-pressed? if  0 exit  then  key: 2 key-pressed? if  1 exit  then
    key: 3 key-pressed? if  2 exit  then  key: 4 key-pressed? if  3 exit  then
    key: 5 key-pressed? if  4 exit  then  -1 ;
: held-gesture ( -- cmd )   \ (held, as the stick held over: the praise's repeats)
    key: 4 key-down? if  3 exit  then  -1 ;
\ Fiona_ReadsPad: the controls are read
: reads-pad? ( -- flag )
    $D state-flag? 0= $2B state-flag? or hewie-control @ or f-busy-t @ or if  false exit  then
    f-mode dup $D = swap $A = or if  true exit  then
    f-mode 0= if  f-act @ dup 1 <> swap $E <> and exit  then
    f-mode 4 = if  f-sub dup 9 = swap $12 = or exit  then
    false ;
: read-command ( -- )
    reads-pad? if  gesture dup -1 = if  drop held-gesture  then  else  -1  then  f-cmd ! ;
' read-command is fiona.brain:read-command
