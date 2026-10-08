\ hewie/commands.fs - Hewie and Fiona's commands (H2): what her command makes him do
\ (Hewie_FionaCommand / CommandAction / ActOnCommand), how her doings strike him
\ (Fiona_HewieReact: his patience and the praise he's due), and the meeting by his side for a
\ praise, a scolding or "stay" (Hewie_JointAction, Hewie_FindSpot, his calls 12: Hewie_StateBlock).
IN: hewie.commands
USING: engine actors common facts paths messages hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states hewie.actions ;

\ ---- how her doing strikes him (Fiona_HewieReact; D_003B2520: the patience it costs while he
\ waits, and 1 in n that he's due a praise when she is near) ----
create reactions  300 , 1 , 300 , 1 , 300 , 1 , 750 , 1 , 150 , 1 , 1500 , 1 , 600 , 1 , 600 , 1 ,
    0 , 1 , 0 , 1 , 0 , 1 , 0 , 1 , 150 , 0 , 300 , 1 , 300 , 0 , 150 , 1 ,
: react ( n -- )
    her-with? 0= here? 0= or if  drop exit  then
    2* cells reactions +
    his-waiting @ 1 = if  dup @ negate his-obey +!  0 his-obey-marked !  then
    cell+ @ dup 0> her-dist 30e f< and if
        roll 0= if  1 his-praise-due !  then
    else  drop  then ;

\ ---- Hewie_CommandAction: what her command makes him do (-1 nothing; -2 he won't; -4 / -5 his
\ mood set first) ----
: command-action ( code -- act )
    his-busy @ 0= if
        his-cond @ 2 = if  $2A = if  $2A  else  -1  then  exit  then
        his-cmd @ $80000008 and 8 <>  his-cooldown @ 0<> or  his-mode @ 4 = or if  drop -1 exit  then
    else  his-cond @ 2 = if  drop -1 exit  then  then
    his-away @ if  exit  then
    dup $28 = over $2B = or over $30 = or if  exit  then
    dup $2F = if  drop   \ scolded from afar
        his-cond @ 1 =  his-waiting @ 0<> or  his-mood @ 2 > or if  -1 exit  then
        his-pet-time @ 0<> rnd 0.25e f< or if  -5  else  -1  then  exit
    then
    dup $29 = if  drop   \ praised from afar
        his-cond @ 1 =  his-waiting @ 0<> or  his-mood @ 1 > or  his-hp @ $50 < or if  -1 exit  then
        his-pet-time @ 0<> rnd 0.25e f< or if  -4  else  -1  then  exit
    then
    his-broke @ 1 <>  his-busy @ 0<> his-cmd @ $80000008 and 8 = or and
    his-waiting @ 0= his-action @ $7D = or and  his-mood @ 3 <> and
    0 his-broke !
    if  exit  then
    drop  his-danger @ if  -1 exit  then
    anim-group dup 5 = swap 1 = or if  -2  else  -1  then ;

\ Hewie_ActOnCommand: true if he took it up
: act-on-command ( code -- flag )
    his-away @ if  drop false exit  then   \ (answering from another room: with the story's doors)
    case
        $25 of  $1D $29 want  obeys  true  endof                       \ stay
        $26 of  his-danger @ 0= dup if  $1D $2B want  then  obeys  endof
        $27 of  his-danger @ 0= if                                     \ come / stay by her
                    his-action @ dup 3 = over 2 = or over 1 = or over 5 = or swap 4 = or if  $29  else  $27  then
                    $1D swap want
                else  his-danger @ 2 = if  $1D $7A want  else  $1D 7 want  then  then
                obeys  true  endof
        $2C of  $1D his-danger @ 0= if  $D  else  $E  then  want  obeys  true  endof   \ come back
        $2D of  $4E 0 want  obeys  true  endof                         \ go for it (chased)
        $2A of  10 hp!  false  endof
        $23 of  his-danger @ 0= if  $1D $63 want  else  $63 0 want  then  obeys  true  endof   \ go there
        $2E of  obeys  false  endof
        $30 of  his-panic @ 5 = if  panic5-chance                      \ her cry for help
                else his-panic @ 4 = if  panic4-chance
                else fiona-mode @ 4 = fiona-sub @ dup 9 = swap $12 = or and if  held-answer-chance
                else  0  then then then
                dup if  by-chance dup if  $4F 0 want  then  then  endof
        >r false r>
    endcase ;

\ Hewie_FionaCommand: her command; true if he acts on it
: fiona-command ( code -- flag )
    his-cmd-was !
    his-busy @ 0= his-broke @ 1 = and if  false exit  then
    his-action @ $7D = his-cmd-was @ $30 <> and if  obeys  then
    his-cmd-was @ dup $2B <> swap $2F <> and if  0 his-praise-b !  0 his-praise-a !  then
    his-cmd-was @ $29 = if
        his-action @ his-did-was !
        1 3 praise-scold if  60 his-cooldown !  $1D 0 want  true exit  then
    then
    his-cmd-was @ $2F = if
        his-action @ his-did-was !
        0 3 praise-scold if  60 his-cooldown !  $71 0 want  true exit  then
    then
    his-cmd-was @ command-action
    dup -1 = if  drop false exit  then
    60 his-cooldown !
    case
        -2 of  $1E 0 want  true  endof
        -3 of  $6F his-action @ want  true  endof
        -4 of  1 -1 set-mood  $1D 0 want  true  endof
        -5 of  calm-down  $71 0 want  true  endof
        dup his-cmd-act !  dup act-on-command swap
    endcase ;
\ the spot she showed (0x23): its triangle, her heading, the point (at her height)
: showed ( tri F: yaw x z -- )
    his-to 8 + sf!  his-to sf!  fiona-at 4 + sf@ his-to 4 + sf!  his-to-yaw f!  his-to-tri ! ;

\ ---- her calls by his side (Hewie_StateBlock's 12): 0 / 2 / 4 turn to her, 1 scolded close up,
\ 3 petted, 5 a pat ----
: sniff ( anim -- )  4 look!  play  ['] st-anim-over behave ;
: turn-with-her ( -- )  his-action @ $7D = if  obeys  180 his-obey +!  then  $48 0 want ;
: her-call ( type -- )
    case
        0 of  his-action @ his-did-was !  turn-with-her  endof
        2 of  his-action @ his-did-was !  turn-with-her  endof
        4 of  turn-with-her  endof
        1 of  -7  0 3 praise-scold 0=  his-cond @ 1 <> and  his-waiting @ 0= and  his-mood @ 3 u< and
              his-pet-time @ 0<> rnd 0.5e f< or and if  drop -6  then
              his-t1 !  $49 0 want  0 his-broke !  endof
        3 of  -9  1 3 praise-scold 0=  his-cond @ 1 <> and  his-waiting @ 0= and  his-mood @ 2 u< and
              his-hp @ 80 >= and  his-pet-time @ 0<> rnd 0.5e f< or and if  drop -8  then
              his-t1 !  $4A 0 want  0 his-broke !  endof
        5 of  -1
              his-broke @ 1 <>  his-busy @ 0<> his-cmd @ $80000008 and 8 = or and if
                  his-waiting @ 0= his-action @ $7D = or  his-mood @ 3 <> and if  drop 0  then
                  1 his-broke !
              then
              0= if  $4B 0 want
              else  4 roll case
                      0 of  $1C06 sniff  endof  1 of  $1C01 sniff  endof
                      2 of  $1C00 sniff  endof  3 of  $55 0 want  endof
                  endcase
              then  0 his-broke !  endof
    endcase ;

\ ---- Hewie_FindSpot: her place by him for a meeting (kHewieMeetOffsets[kind]) round him,
\ turning 0, -+10 .. 180 degrees: open floor (not flags 0x80001) that a walk from her reaches ----
create fs-at 12 allot  fvariable fs-ox  fvariable fs-oz  fvariable fs-yaw
: spot-at ( F: yaw -- tri )
    fs-yaw f!
    fs-ox f@ fs-yaw f@ fcos f*  fs-oz f@ fs-yaw f@ fsin f* f+  his-at sf@ f+  fs-at sf!   \ (Mtx_AtHeading)
    his-at 4 + sf@ fs-at 4 + sf!
    fs-oz f@ fs-yaw f@ fcos f*  fs-ox f@ fs-yaw f@ fsin f* f-  his-at 8 + sf@ f+  fs-at 8 + sf!
    fs-at v-tri dup 0< if  exit  then
    dup nav-flags $80001 and if  drop -1 exit  then
    dup  her body-tri fiona-at fs-at $280A0019 v-walk <> if  drop -1  then ;
: find-spot ( kind -- tri | -1 )
    2* floats meet-offsets + dup f@ fs-ox f!  1 floats + f@ fs-oz f!
    19 0 do
        his-yaw i 10 * s>f deg>rad f- angle-wrap spot-at dup 0< 0= if  unloop exit  then  drop
        i 0<> i 18 <> and if
            his-yaw i 10 * s>f deg>rad f+ angle-wrap spot-at dup 0< 0= if  unloop exit  then  drop
        then
    loop  -1 ;

\ ---- Hewie_JointAction: the meeting. She asks (type 0 / 2 / 4): he - calm, standing or turning
\ to her, listening, on open floor, her near in his room and straight reachable - finds her
\ place and turns to her. Then (1 / 3 / 5), sitting, settled, facing the agreed way, the call.
\ True if he takes it up ----
: meet-ask ( type -- tri | -1 )   \ (her place: fs-at, facing fs-yaw + 180 degrees)
    his-busy @ if  drop -1 exit  then
    his-danger @ 0=  his-mode @ dup 0= swap $C = or and  his-cmd @ $80000008 and 8 = and
    his-tri nav-flags $80001 and 0= and  her-with? and  her-dist 30e f< and
    fiona-at $60088 body-tri-to her body-tri = and 0= if  drop -1 exit  then
    dup 4 = if  3  else dup 0= if  1  else  2  then then  find-spot      ( type tri )
    dup 0< if  nip exit  then
    fs-yaw f@ his-to-yaw f!
    swap  dup his-meet !  0 his-meet-part !  her-call ;                    ( tri )
: meet-second? ( type -- flag )
    his-mode @ $C = settled? and anim-group 1 = and  his-to-yaw f@ his-yaw f- angle-wrap fabs 1e-5 f< and   \ (facing it: his heading is a single float)
    her body-room his-room = and  his-meet @ 0< 0= and
    over 1- his-meet @ = and                                              ( type ok? )
    dup if  over his-meet !  1 his-meet-part !  swap her-call  else  nip  then ;
