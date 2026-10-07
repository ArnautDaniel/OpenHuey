\ pursuer/search.fs - the stalker looking for Fiona (src/game/pursuer.c): his first behaviour
\ (Pursuer_Behaviour25C), searching (Pursuer_BehaviourSearch: the route's stops one after
\ another, then what he saw sends him on), his choice of what to do next from his tables
\ (Pursuer_PickFromTable over Debilitas_AttackTable) and the walk to a stop (Pursuer_SearchRoute).
IN: pursuer.search
USING: engine game-state events.core events.words chars relations fiona.doors pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave ;

\ PURSUER_BEGIN: the action asked for (+0x1758), else `dflt` (-2: none)
: begin-action ( dflt -- )
    $1758 pu-l@ dup -1 <> if  nip dup -2 <> if  $114 vcall  else  drop  then
    else  drop $114 vcall  then ;
\ a fresh behaviour: its flags cleared, his state count from 0
: fresh ( -- )
    1 $16F6 pu-c!  0 $16ED pu-c!  0 step-done!  0 $16EF pu-c!  -1 $1758 pu-l!  0 $1780 pu-l! ;

\ ---- his tables of what to do (+0x1718: rows of a kind, an argument, a percentage) ----
create attack-tables
    $3AF780 , $3AF800 , $3AF7C0 , $3AF840 , $3AF880 , $3AF8A0 , $3AF8C0 , $3AF900 , $3AF930 ,
    $3AF978 , $3AF990 , $3AF9D0 , $3AF9E0 , $3AFA10 , $3AFA70 , $3AFA80 , $3AFA44 ,
    $3AFBA0 , $3AFC40 , $3AFBF0 , $3AFC80 , $3AFCB0 , $3AFCF0 , $3AFD10 , $3AFD50 , $3AFD80 ,
    $3AFDC0 , $3AFDD0 , $3AFE00 , $3AFE14 , $3AFE50 , $3AFED0 , $3AFEE0 , $3AFEA4 ,

\ Debilitas_AttackTable (vtable +0x130): the table for a situation (0..16)
: attack-table ( situation -- )
    dup 17 u< 0= if  drop 0  then  hard-mode @ if  17 +  then  cells attack-tables + @ $1718 pu-l! ;
' attack-table $130 vt!
\ Pursuer_PickFromTable: a roll of 100 against the rows' percentages: the row's action, after
\ a few checks; else his step-by-step behaviour (+0x174C D_003ED350) with it as the next
defer table-behaviour   ' noop is table-behaviour   \ (D_003ED350: with the attacks)
fvariable pick-roll  variable pk-row
: row-arg ( -- n )  pk-row @ 4 + exe-l@ ;
: pick-from-table ( -- )
    100e rnd01 f* pick-roll f!
    $1718 pu-l@  begin  dup 8 + exe-f@ pick-roll f@ f<  while  12 +  repeat  pk-row !
    pk-row @ exe-l@                                                ( kind )
    dup 1 = if  drop exit  then
    dup $13 = if  row-arg $1728 pu-l!  0 $172C pu-c!  then
    dup $18 = over $19 = or if  row-arg p-104!  $114 vcall exit  then
    dup $1A = if  $114 vcall exit  then
    dup 2 = over $17 = or if
        row-arg p-104!
        mode 0<> $1544 pu-c@ 0= or  p-target her = and if  $114 vcall exit  then
    then
    dup $14 = over $15 = or if  not-yet" PickFromTable 0x14/0x15 (Actor_TriFreeFor)"  then
    dup 9 = over $D = or if  $114 vcall exit  then
    ['] table-behaviour behaviour!  $118 vcall ;

\ ---- the search route's stops ----
\ Pursuer_TargetOutOfReach: his goal (+0x15A4) round through a door (0x12), past an exit (0xB)
: target-out-of-reach? ( -- flag )
    $15A4 pu-l@ -1 = if  false exit  then
    not-yet" Pursuer_TargetOutOfReach (Npc_RoundThroughDoor, Npc_ExitsToTri, Npc_FindDoor)"
    false ;
\ Pursuer_SearchRoute (vtable +0x238, action 8's state): aim at the route's next stop (none:
\ a random one added), measured; past them all, the route given up (+0x16EF); a stop added at
\ random is searched 150 frames; then the walk to it (+0x198)
: search-route ( -- )
    0 $16EC pu-c!  1 step-next!
    begin
        route-first route-end < if  route-aim
        else  random-tri dup $15A4 pu-l!  tri-center $15B0 pu vec!  $15A4 pu-l@ route-add drop
            1 route-end 1- stop# 4 + c!  then
        $D8 vcall 0=
    while
        target-out-of-reach? if  exit  then
        route-first 1+ dup $1620 pu-c!  8 >= if  route-clear  1 $16EF pu-c!  exit  then
    repeat
    route-first stop# 4 + c@ if  $1798 pu-l@ 0= if  150 $1798 pu-l!  then  else  0 $1798 pu-l!  then
    0 $1784 pu-l!  [: $198 vcall ;] behave  $198 vcall ;
' search-route $238 vt!
\ Pursuer_NextStop / Pursuer_StopSearched / Pursuer_StopSkipped
: next-stop ( -- )
    route-first route-end <  route-end 8 < and if  8 $114 vcall
    else  route-end $1620 pu-c!  1 $16ED pu-c!  then ;
: stop-searched ( -- )
    route-first 1+ $1620 pu-c!  0 $1798 pu-l!
    route-first route-end >= if  1 $16F4 pu-c!  0 $1794 pu-l!  then
    $10 $130 vcall  pick-from-table ;
: stop-skipped ( -- flag )
    route-first 1+ $1620 pu-c!  0 $1798 pu-l!
    route-first route-end <  route-end 8 < and if  8 $114 vcall  false exit  then
    route-end $1620 pu-c!  1 $16ED pu-c!  1 $16F4 pu-c!  0 $1794 pu-l!  true ;

\ ---- Pursuer_BehaviourSearch (vtable +0x260) ----
\ (its cases)
: search-walking ( -- )   \ 4 / 8: walking to a stop, searching it
    p-faded? if
        d-path 10e f<  $1788 pu-l@ $201 = and if  $320 vcall play-anim
        else $1788 pu-l@ 0= if  0 step-done!  stop-searched  then then
    then
    $1798 pu-l@ if  $1798 pu-1-  $1798 pu-l@ 0= if  1 step-done!  then  then
    $16EF pu-c@ 1 = if  stop-skipped drop  0 $16EF pu-c!
    else step-done? if  0 step-done!  stop-searched  then then ;
: search-past ( -- )   \ 11 / 18: round a door
    $16EF pu-c@ 1 = if  stop-skipped if  1 step-next!  then  0 $16EF pu-c!
    else step-done? if  8 $114 vcall  0 step-done!  then then ;
: search-next ( -- )  step-done? if  next-stop  0 step-done!  then ;
: search-table ( -- )   \ 40 / 41
    step-done? if
        0 step-done!  $175C pu-l@ $28 = if  $E  else  $F  then  $130 vcall
        8 $114 vcall  pick-from-table
    then ;
: behaviour-search ( -- )
    p-state run
    $175C pu-l@ case
        4 of  search-walking  endof
        8 of  search-walking  endof
        11 of  search-past  endof   18 of  search-past  endof
        2 of  search-next  endof   9 of  search-next  endof   13 of  search-next  endof
        23 of  search-next  endof   28 of  search-next  endof
        40 of  search-table  endof   41 of  search-table  endof
        12 of
            step-done? if  0 step-done!  mode 2 - 2 u< if  $29  else  8  then  $114 vcall  then
        endof
        16 of  step-done? if  1 step-next!  1 $1544 pu-c!  0 step-done!  then  endof
    endcase
    step-next? 0= if  exit  then
    $1544 pu-c@ 1 = if
        reached-room? if  [: $264 vcall ;]  else  [: $280 vcall ;]  then  behaviour!
        $1C $118 vcall  0 $16F3 pu-c!  exit
    then
    $1545 pu-c@ 1 = $1546 pu-c@ 1 = or  $16DC pu-l@ $16BC pu-l@ u< and  $E state-flag? 0= and
    $16C9 pu-c@ 3 < and if
        d-hewie f0< if  p-pos 4 + sf@ dog c-pos 4 + sf@ f- fabs 100e f<  else  true  then
        if  [: $270 vcall ;] behaviour!  $1C $118 vcall  0 $16F3 pu-c!  exit  then
    then
    $16F3 pu-c@ 1 = if
        reached-room? if  [: $278 vcall ;]  else  [: $280 vcall ;]  then  behaviour!
        $1C $118 vcall  0 $16F3 pu-c!
    else $16ED pu-c@ 1 = if
        1 $16F4 pu-c!  [: $280 vcall ;] behaviour!  0 $16ED pu-c!
    then then ;
' behaviour-search $260 vt!

\ ---- Pursuer_Behaviour25C (vtable +0x25C): his first behaviour - after Fiona, searching ----
: behaviour-25c ( -- )
    8 begin-action  her p-target!  fresh
    [: $260 vcall ;] behaviour!  $260 vcall ;
' behaviour-25c $25C vt!
