\ summoner.fs - the stalker brought in from off stage and taken back out (docs/subsystems/summoner.md;
\ src/game/pursuer.c Summoner_*, summon_via, summoner_take). The stalker in the story's slot 2
\ tells it where he is each frame (`stalker-status`); at the frame's end it decides.
IN: summoner
USING: engine game-state actors messages common facts flag-names doors ;

state: summoner-state
  cell field away         \ +0: frames of his time away left (low 31 bits)
  cell field called       \ (+0 bit 31) his time away cut short by a noise: he comes in after her
  cell field cooldown     \ +0x4: frames before the wait may take him back
  cell field took-room    \ +0x8: the room he was taken from
  cell field waited       \ +0xC: frames waited (off stage: since taken; in play: since his mode changed)
  cell field last-mode    \ +0x10: his mode when last looked at ($FF none)
  cell field took-kind    \ +0x11: how he was last taken (0 the story / a wait, 1 held off, 2 beaten)
  \ the stalker in slot 2 as he last told it, and how many frame ends ago (he tells it each frame
  \ while loaded: 0 this frame, 1 the last - for the messages between frames; more: none loaded)
  cell field s-id  cell field s-room  cell field s-mode  cell field s-in  cell field s-own
  cell field s-away  cell field s-cool  cell field s-age
  cell field s-beaten   \ (his +0xC4 2: knocked out of health)
  \ Fiona's room and the exit she came in by; the danger
  cell field her-room  cell field her-exit  cell field the-danger
end-state

: reset ( -- )
    0 waited !  0 cooldown !  0 away !  0 called !  -1 took-room !  $FF last-mode !  0 took-kind ! ;
: told? ( -- flag )  s-age @ 2 < ;      \ (a stalker loaded)
: random-delay ( -- frames )  s-away @  3600e rnd f* f>s + ;   \ (Pursuer_RandomDelay)
\ his mode as the progress reads it (+0x64): $FF while he isn't in the game
: his-mode ( -- m )  s-in @ if  s-mode @  else  $FF  then ;

\ ---- taking him out (summoner_take; Progress_CharDone) ----
: take ( kind -- )
    told? 0= if  drop exit  then
    s-in @ 0= if  drop exit  then   \ (out already: Progress_CharDone has nothing to do)
    s-room @ took-room !  random-delay away !  0 called !  0 waited !  took-kind !
    s-id @ send stalker-out ;

\ ---- the rooms: open exits (SceneGame_RoomsNear's exit_open: not locked, passable from all
\ three sides), closed to stalkers (SceneGame_RoomClosedTo kind 2), the rooms a room's usable
\ exits lead to (Rooms_NeighbourCount), an exit into another room (Rooms_ExitLeads) ----
: exit-open? ( room exit -- flag )
    room-exit-door dup 0< if  drop false exit  then
    dup state@ locked-bit and if  drop false exit  then  against 7 and 0= ;
: closed-to-stalkers? ( room -- flag )
    8 0 do
        dup i room-exit-door dup 0< 0= if
            against 4 and 0= if  drop false unloop exit  then
        else  drop  then
    loop  drop true ;
: held-shut? ( room exit -- flag )   \ (DoorHold_Usable: not a doorway, and locked or stuck)
    room-exit-door dup 0< if  drop false exit  then
    dup fixed? if  drop false exit  then  state@ locked-bit stuck-bit or and 0<> ;
create seen-to 8 cells allot  variable seen-n
: seen? ( t -- flag )  false  seen-n @ 0 ?do  over i cells seen-to + @ = or  loop  nip ;
: neighbours ( room except -- n )
    0 seen-n !
    8 0 do
        over i held-shut? 0= if
            over i room-exit-leads drop                      ( room except t )
            dup 0<  over 3 pick = or  over 4 pick = or  over seen? or  if  drop
            else  seen-n @ cells seen-to + !  1 seen-n +!  then
        then
    loop  2drop seen-n @ ;
: leads-elsewhere? ( room exit -- flag )
    2dup room-exit-door dup 0< if  drop 2drop false exit  then
    door-flags $20 and if  2drop false exit  then
    over swap room-exit-leads drop  dup 0< 0=  swap 2 pick <> and nip ;

\ ---- the rooms within two open doors of Fiona's (SceneGame_RoomsNear) ----
create near-list 65 cells allot  variable near-n
: listed? ( room -- flag )  false  near-n @ 0 ?do  over i cells near-list + @ = or  loop  nip ;
: list+ ( room -- )  near-n @ 65 < if  near-n @ cells near-list + !  1 near-n +!  else  drop  then ;
: rooms-near ( room -- )
    dup near-list !  1 near-n !
    8 0 do
        dup i room-exit-leads drop                                   ( room r )
        dup 0<  2 pick i exit-open? 0= or  over listed? or if  drop
        else
            dup list+
            8 0 do
                dup i room-exit-leads drop  dup 0< 0=  2 pick i exit-open? and  over listed? 0= and
                if  list+  else  drop  then
            loop  drop
        then
    loop  drop ;

\ ---- a way in (summon_via): a room off stage near hers, one at random, by its first exit
\ with a route from the half of her room she came in by; he comes in there ----
create picks 65 cells allot  variable picks-n
: pick+ ( room -- )  picks-n @ 65 < if  picks-n @ cells picks + !  1 picks-n +!  else  drop  then ;
: next-to-hers? ( room -- flag )
    false  8 0 do  over her-room @ i room-exit-leads drop = or  loop  nip ;
: candidates ( near -- )
    0 picks-n !
    near-n @ 0 ?do
        i cells near-list + @  dup 0<  over her-room @ = or  if  drop
        else
            over 0= if  dup next-to-hers? if  drop  else  pick+  then
            else
                dup her-room @ neighbours if
                    8 0 do  dup her-room @ i room-exit-leads drop = if  dup pick+  then  loop
                then  drop
            then
        then
    loop  drop ;
create no-avoid 13 cells allot  no-avoid 13 cells 0 fill
variable v-near  variable v-plan
: via ( near plan -- came? )
    v-plan !  v-near !
    her-room @ 0< if  false exit  then
    her-room @ rooms-near  v-near @ candidates
    picks-n @ 0= if  false exit  then
    picks-n @ s>f rnd f* f>s  picks-n @ 1- min  cells picks + @      ( room )
    her-room @ her-exit @ dup 0< if  2drop -1  else  1 exit-way  then  ( room from )
    8 0 do
        over i leads-elsewhere? if
            over i 1 exit-way                                          ( room from to )
            her-room @ 2 pick 4 pick 3 pick 1  v-near @ if 1 else 2 then  no-avoid route-sides  0< 0=
            v-near @ 0= 2 pick 2 u< 0= or  and if
                drop drop  i  called @  v-plan @  s-id @ send stalker-summoned
                true unloop exit
            then  drop
        then
    loop  2drop false ;

\ ---- off stage (Summoner_Offstage): his time away counted down, then a way in tried ----
: came-in ( -- )
    0 waited !  called @ if  1  else  3  then  last-mode !  0 called !
    summoner-waiting state-flag-clear  s-cool @ 30 * cooldown ! ;
: try-ways ( -- came? )   \ from a random first one on: next door with a plan, next door, further
    3e rnd f* f>s 2 min
    begin  dup 3 < while
        dup case  0 of  1 1 via  endof  1 of  1 0 via  endof  >r 0 0 via r>  endcase
        if  drop true exit  then  1+
    repeat  drop false ;
: offstage ( -- came? )
    summoner-on state-flag? 0= if  false exit  then
    summoner-timing state-flag? 0= if
        random-delay away !  0 called !  0 waited !  summoner-timing state-flag-set
    then
    s-in @ if  false exit  then
    summoner-waiting state-flag? 0= away @ 0<> and if  -1 away +!  1 waited +!  false exit  then
    try-ways dup if  came-in  then ;

\ ---- in play (Summoner_InPlay): taken back once he has kept away from her long enough ----
: near-her? ( -- flag )   \ next to her room where he isn't hunting her
    8 0 do
        her-room @ i room-exit-leads drop s-room @ =  his-mode 4 <> s-own @ 0= or  and
        if  true unloop exit  then
    loop  false ;
: in-play ( -- took? )
    s-in @ 0= if  false exit  then
    cooldown @ if  -1 cooldown +!  then
    s-mode @ last-mode @ <> if  s-mode @ last-mode !  0 waited !  false exit  then
    her-room @ 0< 0= if
        her-room @ s-room @ =  near-her? or if  0 waited !  false exit  then
    then
    1 waited +!
    s-beaten @ waited @ 151 >= and if  2 take  true exit  then   \ (beaten: away from her 5 s - longer away)
    s-mode @ 4 = waited @ 151 >= and if  1 take  true exit  then
    cooldown @ if  false exit  then
    s-mode @ 3 = waited @ 91 >= and if  0 take  true exit  then
    false ;

\ ---- each frame and at the start of play in a room (Summoner_RoomStart) ----
: decide ( -- )
    s-age @ if  exit  then   \ (told this frame)
    world-frozen state-flag? if  exit  then
    $18 state-flag? s-room @ her-room @ <> and if  exit  then
    her-room @ 0< if  exit  then
    her-room @ closed-to-stalkers? if
        her-room @ s-room @ = s-in @ and if  exit  then
        his-mode 4 = s-own @ and if  1 take  then  exit
    then
    cam-event? if  exit  then
    offstage 0= if  in-play drop  then ;

\ ---- Fiona's noise (Summoner_Noise): its level; it may call him in, or have her hunted ----
create chances  0 c, 0 c,  5 c, 10 c,  25 c, 50 c,  100 c, 0 c,    \ Pursuer_NoiseChances ($47AC90): call, hunt
create waits  0 , 10 , 30 , 30 ,                                   \ D_00419DC0: seconds, by how he was taken
: level ( loud door room -- lvl )
    false  8 0 do  over i room-exit-door dup 0< 0= if  door-open? or  else  drop  then  loop  nip  ( loud door open )
    rot  dup $80 >= if 4 else dup $60 >= if 3 else dup $40 >= if 2 else dup $20 >= if 1 else 0 then then then then nip
    rot 0< rot 0= and over 4 < and if  1-  then
    0 max 3 min ;
: roll? ( percent -- flag )
    dup 0= if  drop false exit  then  dup 100 = if  drop true exit  then
    s>f 100e rnd f* f>= ;   \ (100 x the random <= the chance)
: hunt ( -- )  6 s" danger" actor-named send danger-signal ;
: noise ( loud room door -- )
    over her-room @ <> if  drop 2drop exit  then
    swap level  2* chances +  dup c@ swap 1+ c@               ( call% hunt% )
    false                                                     ( c h call? )
    told? if
        s-in @ if  drop 2drop exit  then
        2 pick roll?  summoner-on state-flag? and  summoner-waiting state-flag? 0= and if
            drop  waited @ 30 /  dup 3 >=  swap took-kind @ cells waits + @ >= and
        then
    then
    if  2drop  0 away !  -1 called !  exit  then
    dup if  over 100 swap - s>f rnd f* dup s>f f<= if  2drop hunt exit  then  then
    the-danger @ 1 = rot rot + 0<> and if  hunt  then ;

behaviour summoning
  on spawned ( -- )
      reset  100 s-age !  -1 her-room !  -1 her-exit !  0 the-danger !
      self subscribe frame-end  self subscribe entered-room  self subscribe danger  self subscribe noise-made ;
  on stalker-status ( room mode in-game own away cool beaten -- )
      s-beaten !  s-cool !  s-away !  s-own !  s-in !  s-mode !  s-room !  sender s-id !  0 s-age ! ;
  on entered-room ( room exit -- )  her-exit !  her-room ! ;
  on danger ( level -- )  the-danger ! ;
  on noise-made ( loud room door -- )  noise ;
  on summoner-take ( kind -- )  take ;
  on summoner-cooldown ( secs -- )  dup 0< if  drop s-cool @  then  30 * cooldown ! ;
  on summoner-cooldown-less ( secs -- )  30 * cooldown @ swap - 0 max cooldown ! ;
  on summoner-reset ( -- )  reset ;
  on frame-end ( -- )  decide  s-age @ 100 < if  1 s-age +!  then
      cooldown @ dup 30 /  swap 0<> over 0= and if  drop 1  then   \ (Countdown_Seconds: at least 1 while it runs)
      broadcast summoner-cooldown-is ;
end-behaviour

: summoner-spawn ( -- id )  summoning summoner-state s" summoner" spawn ;
