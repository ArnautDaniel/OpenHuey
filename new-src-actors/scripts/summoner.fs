\ summoner.fs - the stalker brought in from off stage and taken back out (docs/subsystems/summoner.md;
\ src/game/pursuer.c Summoner_*, summon_via, summoner_take). The stalker in the story's slot 2
\ tells it where he is each frame (`stalker-status`); at the frame's end it decides.
IN: summoner
USING: engine game-state actors messages common facts flag-names doors ;

\ how he was last taken: by the story or a wait, held off, beaten
enum: taken-by  taken-by-story taken-held-off taken-beaten ;
$FF constant no-mode   \ (his mode while he isn't in the game: Progress_StalkerAlert)
65 constant #near-max

state: summoner-state
  cell field away         \ frames of his time away left
  cell field called       \ his time away cut short by a noise: he comes in after her
  cell field cooldown     \ frames before the wait may take him back
  cell field took-room    \ the room he was taken from
  cell field waited       \ frames waited (off stage: since taken; in play: since his mode changed)
  cell field last-mode    \ his mode when last looked at (no-mode)
  cell field took-kind    \ (enum: taken-by)
  \ the stalker in slot 2 as he last told it, and how many frame ends ago (he tells it each frame
  \ while loaded: 0 this frame, 1 the last - for the messages between frames; more: none loaded)
  cell field s-id  cell field s-room  cell field s-mode  cell field s-in  cell field s-own
  cell field s-away  cell field s-cool  cell field s-age
  cell field s-beaten   \ (knocked out of health)
  \ Fiona's room and the exit she came in by; the danger
  cell field her-room  cell field her-exit  cell field the-danger
  \ working lists: the rooms near hers (SceneGame_RoomsNear), the ways in to pick from
  #near-max cells field near-list  cell field near-n
  #near-max cells field picks  cell field picks-n
  8 cells field seen   \ (neighbours: the rooms counted)
end-state

: reset ( -- )
    0 waited !  0 cooldown !  0 away !  0 called !  -1 took-room !  no-mode last-mode !  taken-by-story took-kind ! ;
: told? ( -- flag )  s-age @ 2 < ;      \ (a stalker loaded)
: random-delay ( -- frames )  s-away @  3600e rnd f* f>s + ;   \ (Pursuer_RandomDelay)
: his-mode ( -- m )  s-in @ if  s-mode @  else  no-mode  then ;

\ ---- taking him out (summoner_take; Progress_CharDone) ----
: take ( kind -- )
    told? 0= s-in @ 0= or if  drop exit  then   \ (out already: Progress_CharDone has nothing to do)
    s-room @ took-room !  random-delay away !  0 called !  0 waited !  took-kind !
    s-id @ send stalker-out ;

\ ---- the rooms: open exits (SceneGame_RoomsNear's exit_open: not locked, passable from all
\ three sides), closed to stalkers (SceneGame_RoomClosedTo kind 2), the rooms a room's usable
\ exits lead to (Rooms_NeighbourCount), an exit into another room (Rooms_ExitLeads) ----
: leads ( room exit -- room' )  room-exit-leads drop ;
: closed-to-stalkers? ( room -- flag )   \ (every door out locked against them)
    {: room :}
    8 0 do
        room i room-exit-door  dup 0< if  drop  else  stalker-kind locked-against? 0= if  false unloop exit  then  then
    loop  true ;
: held-shut? ( room exit -- flag )   \ (DoorHold_Usable: not a doorway, and locked or stuck)
    room-exit-door dup 0< if  drop false exit  then
    dup fixed? if  drop false exit  then  locked-bit stuck-bit or has? ;
: neighbours ( room except -- n )   \ the other rooms its usable exits lead to, each once
    {: room except | n :}
    8 0 do
        room i held-shut? 0= if
            room i leads {: t :}
            t 0< t room = or t except = or 0= if
                false  n 0 ?do  seen i cells + @ t = or  loop
                0= if  t seen n cells + !  n 1+ to n  then
            then
        then
    loop  n ;
: leads-elsewhere? ( room exit -- flag )   \ (its door no $20, into another room)
    {: room ex :}  room ex room-exit-door  dup 0< if  drop false exit  then
    door-flags $20 and if  false exit  then
    room ex leads  dup 0< 0=  swap room <> and ;

\ ---- the rooms within two open doors of Fiona's (doors.fs: SceneGame_RoomsNear) ----
: listed? ( room -- flag )  near-list near-n @ in-list? ;
: rooms-near ( room -- )  near-list #near-max doors:rooms-near near-n ! ;

\ ---- a way in (summon_via): a room off stage near hers, one at random, by its first exit
\ with a route from the half of her room she came in by; he comes in there ----
: pick+ ( room -- )  picks-n @ #near-max < if  picks-n @ cells picks + !  1 picks-n +!  else  drop  then ;
: next-to-hers? ( room -- flag )
    false  8 0 do  over her-room @ i leads = or  loop  nip ;
: ways-to-her ( room -- )   \ (next door: once for each exit of hers that leads to it)
    {: room :}  8 0 do  her-room @ i leads room = if  room pick+  then  loop ;
: candidates ( near? -- )
    {: near :}  0 picks-n !
    near-n @ 0 ?do
        i cells near-list + @  {: r :}
        r 0< r her-room @ = or 0= if
            near if  r her-room @ neighbours if  r ways-to-her  then
            else  r next-to-hers? 0= if  r pick+  then  then
        then
    loop ;
create no-avoid 13 cells allot  no-avoid 13 cells 0 fill
: some-way ( -- room )   \ one of the picks at random
    picks-n @ s>f rnd f* f>s  picks-n @ 1- min  cells picks + @ ;
: via ( near? plan -- came? )   \ (near: next door to hers; plan: he heads for her)
    {: near plan :}
    her-room @ 0< if  false exit  then
    her-room @ rooms-near  near candidates
    picks-n @ 0= if  false exit  then
    some-way  {: room :}
    her-exit @ 0< if  -1  else  her-room @ her-exit @ 1 exit-way  then  {: from :}   \ (the half of her room she came in by)
    8 0 do
        room i leads-elsewhere? if
            room i 1 exit-way  {: to :}
            her-room @ from room to 1  near if 1 else 2 then  no-avoid route-sides 0< 0=
            near 0=  to 2 u< 0= or  and if   \ (next door: not into halves 0 / 1)
                room i called @ plan s-id @ send stalker-summoned
                true unloop exit
            then
        then
    loop  false ;

\ ---- off stage (Summoner_Offstage): his time away counted down, then a way in tried ----
: came-in ( -- )
    0 waited !  called @ if  heading-for-her  else  waiting-about  then  last-mode !  0 called !
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
        her-room @ i leads s-room @ =  his-mode held-off <> s-own @ 0= or  and
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
    s-beaten @ waited @ 151 >= and if  taken-beaten take  true exit  then   \ (beaten: away from her 5 s)
    s-mode @ held-off = waited @ 151 >= and if  taken-held-off take  true exit  then
    cooldown @ if  false exit  then
    s-mode @ waiting-about = waited @ 91 >= and if  taken-by-story take  true exit  then
    false ;

\ ---- each frame and at the start of play in a room (Summoner_RoomStart) ----
: decide ( -- )
    s-age @ if  exit  then   \ (told this frame)
    world-frozen state-flag? if  exit  then
    $18 state-flag? s-room @ her-room @ <> and if  exit  then
    her-room @ 0< if  exit  then
    her-room @ closed-to-stalkers? if
        her-room @ s-room @ = s-in @ and if  exit  then
        his-mode held-off = s-own @ and if  taken-held-off take  then  exit
    then
    cam-scene? if  exit  then   \ (a cutscene has the camera)
    offstage 0= if  in-play drop  then ;

\ ---- Fiona's noise (Summoner_Noise): its level; it may call him in, or have her hunted ----
begin-structure /chances  cfield: call%  cfield: hunt%  end-structure
create chances  0 c, 0 c,  5 c, 10 c,  25 c, 50 c,  100 c, 0 c,    \ Pursuer_NoiseChances ($47AC90), by level
create waits  0 , 10 , 30 , 30 ,                                   \ D_00419DC0: seconds, by how he was taken
: loudness ( loud -- lvl )   \ $20 1, $40 2, $60 3, $80 4
    dup $80 >= if  drop 4  else  dup $60 >= if  drop 3  else  dup $40 >= if  drop 2  else  $20 >= 1 and  then then then ;
: level ( loud door room -- lvl )   \ one less away from a door with every door of the room shut; at most 3
    {: loud door room :}
    false  8 0 do  room i room-exit-door dup 0< 0= if  door-open? or  else  drop  then  loop  {: open :}
    loud loudness  door 0< open 0= and over 4 < and if  1-  then
    0 max 3 min ;
: roll? ( percent -- flag )
    dup 0= if  drop false exit  then  dup 100 = if  drop true exit  then
    s>f 100e rnd f* f>= ;   \ (100 x the random <= the chance)
: hunt ( -- )  being-hunted s" danger" actor-named send danger-signal ;
: wait-over? ( -- flag )   \ 3 s waited, and his last taking's wait
    waited @ 30 /  dup 3 >=  swap took-kind @ cells waits + @ >= and ;
: noise ( loud room door -- )
    {: loud room door :}  room her-room @ <> if  exit  then
    loud door room level /chances * chances +  dup call% c@ swap hunt% c@  {: call-chance hunt-chance :}
    false   ( calls? )
    told? if
        s-in @ if  drop exit  then
        call-chance roll?  summoner-on state-flag? and  summoner-waiting state-flag? 0= and if  drop wait-over?  then
    then
    if  0 away !  -1 called !  exit  then
    hunt-chance if   \ (the hunt's chance of the rest)
        100 call-chance - s>f rnd f*  hunt-chance s>f f<= if  hunt exit  then
    then
    the-danger @ followed =  call-chance hunt-chance + 0<> and if  hunt  then ;

behaviour summoning
  on spawned ( -- )
      reset  100 s-age !  -1 her-room !  -1 her-exit !  calm the-danger !
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
