\ acoustics.fs - noises and who hears them. One actor: it keeps the loudest noise of each
\ source made this frame, and at the frame's end tells each listener what it heard. The rules
\ and their sources in the original: docs/subsystems/acoustics.md.
IN: acoustics
USING: engine actors messages ;

\ a noise this frame: how loud, the room, its triangle (-1: at a door), the door (-1: none)
begin-structure /noise  field: n.loud  field: n.room  field: n.tri  field: n.door  end-structure
\ who listens: hearing noises louder than `threshold`, not those of its own source
begin-structure /listener  field: l.id  field: l.threshold  field: l.source  end-structure
16 constant #listeners-max
$FF constant no-exit

state: acoustics-state
  #noise-sources /noise * field noises      \ this frame's from each source (loud 0: none)
  cell field setting                        \ 0..3: quiet noises quieter
  #listeners-max /listener * field listeners
  cell field #listeners
  \ hearing (Character_Hearing): the listener, and the exit the last noise came in by - carried
  \ across the sources, as the original's `exit` is (quirk: step 7)
  cell field hearer  cell field last-exit
end-state

: noise ( source -- n )  /noise * noises + ;
: forget-noises ( -- )
    #noise-sources 0 do  0 i noise n.loud !  -1 i noise n.room !  -1 i noise n.tri !  -1 i noise n.door !  loop ;

\ ---- the listeners, in the order they asked ----
: listener ( i -- l )  /listener * listeners + ;
: listener# ( id -- i | -1 )
    #listeners @ 0 ?do  i listener l.id @ over = if  drop i unloop exit  then  loop  drop -1 ;
: drop-listener ( id -- )
    listener# dup 0< if  drop exit  then  {: i :}
    i 1+ listener  i listener  #listeners @ i - 1- /listener *  move
    -1 #listeners +! ;
: add-listener ( id threshold source -- )
    #listeners @ #listeners-max < 0= if  drop 2drop exit  then
    #listeners @ listener {: l :}  l l.source !  l l.threshold !  l l.id !  1 #listeners +! ;

\ ---- hearing one noise (Character_Hearing's loop body) ----
: setting-loud ( loud -- loud' )   \ (below $80, quieter by the setting)
    dup $80 < if  setting @ case  1 of  $1F -  endof  2 of  $3F -  endof  3 of  $5F -  endof  endcase  then ;
: none>ff ( exit -- exit' )  dup 0< if  drop no-exit  then ;
: hearer-room ( -- room )  hearer @ body-room ;
\ in the listener's room, or at a door into it
: reaches? ( n -- flag )
    {: n :}  hearer-room n n.room @ = if  true exit  then
    n n.door @ 0< if  false exit  then
    n n.door @ n n.room @ door-exit-in none>ff  dup last-exit !
    dup no-exit = if  drop true exit  then
    n n.room @ swap room-exit-leads drop  hearer-room = ;
\ where it came from: its triangle's centre, else where its door stands, else the listener
: door-stand ( n -- flag ) ( F: -- x y z | )
    {: n :}  n n.door @ 0< if  false exit  then
    n n.door @ hearer-room door-exit-in  dup 0< if  drop false exit  then  exit-stand ;
: source-at ( n -- ) ( F: -- x y z )
    dup n.tri @ dup 0< 0= if  nip tri-center exit  then  drop
    door-stand 0= if  hearer @ body-pos  then ;
: by-distance ( loud n -- loud' )   \ a tenth of the level distance plus three times the rise
    source-at  {: loud | F: sx F: sy F: sz :}  to sz to sy to sx
    hearer @ body-pos  {: F: x y z :}
    y sy f- fabs 3e f*   x sx f- fsq  z sz f- fsq f+ fsqrt  f+
    0.1e f* f>s  loud swap - ;
: route-end ( doors -- door | -1 )  dup 0> if  1- route-door  else  drop -1  then ;
1 constant walker   \ (route's kind: through doors on foot)
: by-route ( loud n -- loud' )   \ through at most 2 doors, for a walker
    {: loud n | doors :}  hearer-room n n.room @ walker 2 route to doors   \ (-1: no way)
    n n.door @ 0< 0=  doors 0> and if   \ (its door the one it came in by: one door less)
        doors route-end n n.room @ door-exit-in none>ff  last-exit @ = if  doors 1- to doors  then
    then
    doors -1 = if  loud $60 < if  0  else  loud  then  exit  then
    loud doors 5 lshift < if  0 exit  then
    loud $41 < if  doors route-end door-open? 0= if  0 exit  then  then
    loud ;
: heard? ( threshold loud n -- flag )
    {: threshold loud n :}
    n reaches? if
        hearer-room room-id = if  loud n by-distance  else  loud  then
    else  loud n by-route  then
    threshold swap < ;
: tell ( id threshold source -- )   \ the first source it hears, or nothing
    {: id threshold own :}  id hearer !  0 last-exit !
    #noise-sources 0 do
        i own <> if
            i noise {: n :}
            n n.loud @ setting-loud  dup 0> if
                threshold swap n heard? if
                    n n.loud @  n n.room @  n n.tri @  n n.door @  i  id send heard  unloop exit
                then
            else  drop  then
        then
    loop  id send heard-nothing ;

behaviour hearing
  on spawned ( -- )  forget-noises  self subscribe frame-end ;
  on noise ( loud room tri door source -- )   \ the loudest of each source kept
      {: loud room tri door src :}
      src 0 #noise-sources within 0= if  exit  then
      loud 0=  room -1 = or  loud $FF and src noise n.loud @ < or if  exit  then
      src noise {: n :}
      loud $FF and n n.loud !  room n n.room !  door n n.door !
      door 0< if  tri  else  -1  then  n n.tri ! ;   \ (at a door: no triangle)
  on listen ( threshold source -- )  sender drop-listener  sender -rot add-listener ;
  on stop-listening ( -- )  sender drop-listener ;
  on noise-setting ( n -- )  setting ! ;
  on frame-end ( -- )
      \ (Fiona's loudest noise this frame, for the summoner: the original's pursuer request)
      fiona-noise noise {: hers :}
      hers n.loud @ if  hers n.loud @  hers n.room @  hers n.door @  broadcast noise-made  then
      #listeners @ 0 ?do
          i listener {: l :}
          l l.id @ body? if  l l.id @  l l.threshold @  l l.source @  tell  then
      loop
      \ (each source's noise this frame, for the rooms' scripts: the original's +0x10D4, kept a frame)
      #noise-sources 0 do  i noise n.loud @ ?dup if  i noise n.room @  broadcast noise-was  then  loop
      forget-noises ;
end-behaviour

: acoustics-spawn ( -- id )  hearing acoustics-state s" acoustics" spawn ;
