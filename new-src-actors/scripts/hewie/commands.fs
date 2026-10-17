\ hewie/commands.fs - Hewie and Fiona's commands: what her command makes him do
\ (Hewie_FionaCommand / CommandAction / ActOnCommand), how her doings strike him
\ (Fiona_HewieReact: his patience and the praise he's due), and the meeting by his side for a
\ praise, a scolding or "stay" (Hewie_JointAction, Hewie_FindSpot, his calls 12: Hewie_StateBlock).
IN: hewie.commands
USING: engine actors common facts paths vectors messages hewie-names hewie.state hewie.body hewie.tables hewie.anims hewie.model hewie.moving hewie.states hewie.actions hewie.fetch ;

\ ---- how her doing strikes him (Fiona_HewieReact; D_003B2520 by hewie-reaction: the patience it
\ costs while he waits, and 1 in n that he's due a praise when she is near) ----
create reactions  300 , 1 ,  300 , 1 ,  300 , 1 ,  750 , 1 ,  150 , 1 ,  1500 , 1 ,  600 , 1 ,  600 , 1 ,
    0 , 1 ,  0 , 1 ,  0 , 1 ,  0 , 1 ,  150 , 0 ,  300 , 1 ,  300 , 0 ,  150 , 1 ,  0 , 1 ,
: react ( n -- )
    her-with? 0= here? 0= or if  drop exit  then
    2* cells reactions +
    his-waiting @ 1 = if  dup @ negate his-obey +!  0 his-obey-marked !  then
    cell+ @ dup 0> her-dist 30e f< and if
        roll 0= if  1 his-praise-due !  then
    else  drop  then ;

\ ---- Hewie_CommandAction: what her command makes him do - itself, or one of these ----
-1 constant no-heed          \ nothing
-2 constant wont-heed        \ he won't
-4 constant pleased-heed     \ his mood pleased first
-5 constant scolded-heed     \ his mood calmed, scolded
: commandable-now? ( -- flag )  on-command breaks-in?  his-cooldown @ 0= and  his-mode @ struck-mode <> and ;
: command-action ( code -- act )
    his-busy @ 0= if
        his-cond @ down-cond = if  tend-cmd = if  tend-cmd  else  no-heed  then  exit  then
        commandable-now? 0= if  drop no-heed exit  then
    else  his-cond @ down-cond = if  drop no-heed exit  then  then
    his-away @ if  exit  then
    dup pat-cmd = over scold-near-cmd = or over help-cmd = or if  exit  then
    dup scold-cmd = if  drop   \ scolded from afar
        his-cond @ hurt-cond =  his-waiting @ 0<> or  his-mood @ upset-mood > or if  no-heed exit  then
        his-pet-time @ 0<> rnd 0.25e f< or if  scolded-heed  else  no-heed  then  exit
    then
    dup praise-cmd = if  drop   \ praised from afar
        his-cond @ hurt-cond =  his-waiting @ 0<> or  his-mood @ pleased-mood > or  his-hp @ 80 < or if  no-heed exit  then
        his-pet-time @ 0<> rnd 0.25e f< or if  pleased-heed  else  no-heed  then  exit
    then
    his-broke @ 1 <>  his-busy @ 0<> on-command breaks-in? or and
    his-waiting @ 0= his-action @ whine-act = or and  his-mood @ angry-mood <> and
    0 his-broke !
    if  exit  then
    drop  his-danger @ calm <> if  no-heed exit  then
    anim-group dup in-sitting-down = swap in-sitting = or if  wont-heed  else  no-heed  then ;

\ from another room (Hewie_ActOnCommand while disabled): "come back" - hidden away (the story's
\ hidden-act time running) only that time cut short (its pending steer: with the Hewie-played
\ sections), else back to her; her cry for help - to her; the others nothing
: act-away ( code -- flag )
    case
        come-back-cmd of
            his-yelp @ 0> if  -1 his-yelp !  false
            else  his-action @ back-to-her-act <> if  back-to-her-act 0 want  true  else  false  then  then  endof
        help-cmd of  -1 his-yelp !  to-her-cry-act 0 want  true  endof
        tend-cmd of  10 hp!  false  endof
        >r false r>
    endcase ;
\ her cry for help: by her panic, or held, by chance he answers
: answers-cry? ( -- flag )
    his-panic @ calming-down = if  panic5-chance
    else his-panic @ panicking = if  panic4-chance
    else fiona-mode @ thrown-mode = fiona-sub @ dup led-away-r = swap held-down-r = or and if  held-answer-chance
    else  0  then then then
    dup if  by-chance  then ;
\ Hewie_ActOnCommand: true if he took it up
: act-on-command ( code -- flag )
    his-away @ if  act-away exit  then
    case
        stay-cmd of  heard-her-act sit-then-stay-act want  obeys  true  endof
        cmd-26 of  his-danger @ calm = dup if  heard-her-act lie-then-stay-act want  then  obeys  endof
        come-cmd of
            his-danger @ calm = if   \ (by her already: sitting and staying; else standing and staying)
                his-action @ dup stay-a-while-act = over stay-act = or over lie-by-her-act = or over settle-act = or swap hold-low-act = or
                if  sit-then-stay-act  else  stand-then-stay-act  then  heard-her-act swap want
            else  his-danger @ chased = if  heard-her-act ready-act want  else  heard-her-act watch-act want  then  then
            obeys  true  endof
        come-back-cmd of  heard-her-act  his-danger @ calm = if  come-back-act  else  hurry-to-her-act  then  want  obeys  true  endof
        go-for-it-cmd of  after-coming-out-act 0 want  obeys  true  endof
        tend-cmd of  10 hp!  false  endof
        go-there-cmd of  his-danger @ calm = if  heard-her-act go-there-act want  else  go-there-act 0 want  then  obeys  true  endof
        attack-now-cmd of  obeys  false  endof
        help-cmd of  answers-cry? dup if  answer-call-act 0 want  then  endof
        >r false r>
    endcase ;

\ (Actor_CanWalkBetween with no mask: straight from him to her on the floor)
: can-get-to-her? ( -- flag )  his-tri his-at fiona-at 0 v-walk  her body-tri = ;
\ Hewie_FionaCommand: her command; true if he acts on it
: fiona-command ( code -- flag )
    dup his-heard !  his-cmd-was !
    his-away @ if   \ (in another room: what it makes him do, straight - Hewie_FionaCommand's disabled)
        his-cmd-was @ command-action dup 0< if  drop false  else  act-on-command  then  exit
    then
    his-busy @ 0= his-broke @ 1 = and if  false exit  then
    his-action @ whine-act = his-cmd-was @ help-cmd <> and if  obeys  then
    his-cmd-was @ dup scold-near-cmd <> swap scold-cmd <> and if  0 his-praise-b !  0 his-praise-a !  then
    his-cmd-was @ go-there-cmd = his-waiting @ 0= and  his-broke @ 1 <> and  his-danger @ calm = and  ball 0< and  can-get-to-her? and if
        nearest-ball dup his-follows !  0< 0= if  heard-her-act fetch-act want  true exit  then   \ (fetch: the ball nearest her)
    then
    his-cmd-was @ praise-cmd = if
        his-action @ his-did-was !
        1 3 praise-scold if  60 his-cooldown !  heard-her-act 0 want  true exit  then
    then
    his-cmd-was @ scold-cmd = if
        his-action @ his-did-was !
        0 3 praise-scold if  60 his-cooldown !  scolded-afar-act 0 want  true exit  then
    then
    his-cmd-was @ command-action
    dup no-heed = if  drop false exit  then
    60 his-cooldown !
    case
        wont-heed of  wont-act 0 want  true  endof
        -3 of  run-to-her-act his-action @ want  true  endof
        pleased-heed of  pleased-mood -1 set-mood  heard-her-act 0 want  true  endof
        scolded-heed of  calm-down  scolded-afar-act 0 want  true  endof
        dup his-cmd-act !  dup act-on-command swap
    endcase ;
\ the spot she showed (go there): its triangle, her heading, the point (at her height)
: showed ( tri F: yaw x z -- )
    his-to v.z sf!  his-to v.x sf!  fiona-at v.y sf@ his-to v.y sf!  his-to-yaw f!  his-to-tri ! ;

\ ---- her calls by his side (Hewie_StateBlock's 12): she asks - for a scolding, a praise, to
\ stay: he turns to her; then the call - scolded close up, petted, patted (a pat he may not take)
enum: meeting  scold-ask scold-call praise-ask praise-call stay-ask stay-call ;
: sniff ( anim -- )  ahead-look look!  play  ['] st-anim-over behave ;
: turn-with-her ( -- )  his-action @ whine-act = if  obeys  180 his-obey +!  then  turn-with-her-act 0 want ;
: her-call ( type -- )
    case
        scold-ask of  his-action @ his-did-was !  turn-with-her  endof
        praise-ask of  his-action @ his-did-was !  turn-with-her  endof
        stay-ask of  turn-with-her  endof
        scold-call of  -7  0 3 praise-scold 0=  his-cond @ hurt-cond <> and  his-waiting @ 0= and  his-mood @ angry-mood u< and
              his-pet-time @ 0<> rnd 0.5e f< or and if  drop -6  then
              his-t1 !  scolded-close-act 0 want  0 his-broke !  endof
        praise-call of  -9  1 3 praise-scold 0=  his-cond @ hurt-cond <> and  his-waiting @ 0= and  his-mood @ upset-mood u< and
              his-hp @ 80 >= and  his-pet-time @ 0<> rnd 0.5e f< or and if  drop -8  then
              his-t1 !  petted-act 0 want  0 his-broke !  endof
        stay-call of  -1
              his-broke @ 1 <>  his-busy @ 0<> on-command breaks-in? or and if
                  his-waiting @ 0= his-action @ whine-act = or  his-mood @ angry-mood <> and if  drop 0  then
                  1 his-broke !
              then
              0= if  patted-act 0 want
              else  4 roll case   \ (he won't: one of these)
                      0 of  sit-trick-anim sniff  endof  1 of  refuse-anim sniff  endof
                      2 of  sit-trick-2-anim sniff  endof  3 of  close-by-her-act 0 want  endof
                  endcase
              then  0 his-broke !  endof
    endcase ;

\ ---- Hewie_FindSpot: her place by him for a meeting (kHewieMeetOffsets[kind]) round him,
\ turning 0, -+10 .. 180 degrees: open floor (not a step or a doorway) that a walk from her
\ reaches; found, his-to-yaw the way ----
create fs-at /vec allot
$280A0019 constant her-walk-stops   \ (the floors her walk there may not cross)
: spot-at ( F: ox oz yaw -- ) ( -- tri )   \ (Mtx_AtHeading: the offset turned by yaw, round him)
    {: F: ox oz yaw :}
    ox yaw fcos f*  oz yaw fsin f* f+  his-at v.x sf@ f+  fs-at v.x sf!
    his-at v.y sf@ fs-at v.y sf!
    oz yaw fcos f*  ox yaw fsin f* f-  his-at v.z sf@ f+  fs-at v.z sf!
    fs-at v-tri dup 0< if  exit  then
    dup nav-flags $80001 and if  drop -1 exit  then
    dup  her body-tri fiona-at fs-at her-walk-stops v-walk <> if  drop -1  then ;
: find-spot ( kind -- tri | -1 )
    2* floats meet-offsets +  dup f@  float+ f@  {: F: ox oz :}
    19 0 do
        his-yaw i 10 * s>f deg>rad f- angle-wrap {: F: yaw :}
        ox oz yaw spot-at dup 0< 0= if  yaw his-to-yaw f!  unloop exit  then  drop
        i 0<> i 18 <> and if
            his-yaw i 10 * s>f deg>rad f+ angle-wrap to yaw
            ox oz yaw spot-at dup 0< 0= if  yaw his-to-yaw f!  unloop exit  then  drop
        then
    loop  -1 ;

\ ---- Hewie_JointAction: the meeting. She asks: he - calm, standing or turning to her,
\ listening, on open floor, her near in his room and straight reachable - finds her place and
\ turns to her. Then, sitting, settled, facing the agreed way, the call. True if he takes it up ----
: meet-ask ( type -- tri | -1 )   \ (her place: fs-at, facing his-to-yaw + 180 degrees)
    his-busy @ if  drop -1 exit  then
    his-danger @ calm =  his-mode @ dup free-mode = swap meeting-mode = or and  on-command breaks-in? and
    his-tri nav-flags $80001 and 0= and  her-with? and  her-dist 30e f< and
    fiona-at $60088 body-tri-to her body-tri = and 0= if  drop -1 exit  then
    dup stay-ask = if  3  else dup scold-ask = if  1  else  2  then then  find-spot      ( type tri )
    dup 0< if  nip exit  then
    swap  dup his-meet !  0 his-meet-part !  her-call ;                    ( tri )
: meet-second? ( type -- flag )
    his-mode @ meeting-mode = settled? and anim-group in-sitting = and  his-to-yaw f@ his-yaw f- angle-wrap fabs 1e-5 f< and   \ (facing it: his heading is a single float)
    her body-room his-room = and  his-meet @ 0< 0= and
    over 1- his-meet @ = and                                              ( type ok? )
    dup if  over his-meet !  1 his-meet-part !  swap her-call  else  nip  then ;
