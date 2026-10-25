\ fiona/companion.fs - Fiona in the mini-game's run (hewie-played?): Hewie is the one played, and
\ she goes her own way with him (Fiona_MoodAI, Fiona_Timers, Fiona_OffscreenFrame; the run's
\ parts of Fiona_MoveInput, Fiona_Vt34 and Vt38, Fiona_StateBlock 0xD).
\
\ By turns she keeps with him (following him, through the doors after him) and goes off on her
\ own (waiting, or out by an exit at random): each turn 5..20 seconds with him, then 10..21 on her
\ own, the second the shorter the longer he keeps by her. The stalker in sight, she may run (the
\ more likely the higher her panic) - out by the exit she reaches before him; panicking, at once.
\ Hewie barking low by a shut door sends her to open it (Hewie_StateLookLow); barking elsewhere,
\ she keeps with him longer. Out of sight she walks the house door by door: after him by his
\ way, away from the stalker fleeing, at random wandering - a shut door in her way opened, a
\ locked one waited at; her fear growing while she follows or flees, easing while she waits.
IN: fiona.companion
USING: engine vectors game-state actors messages common facts doors paths floor-flags hewie-names stalker-names fiona.state fiona.anims fiona.model fiona.controls fiona.moving fiona.looks fiona.slam fiona.fear fiona.doors fiona.defend ;

defer put-her ( F: x y z yaw -- )   ' noop is put-her   \ (fiona.fs: put)
defer fresh-her ( -- )   ' noop is fresh-her             \ (fiona.fs: fresh)

\ her mood (+0x1AD71C), in the room being played: by him; waiting; following him; fleeing - to
\ the exit, through it; off by an exit at random - to it, through it; after him by an exit - to
\ it, through it; sent to open an exit; at the stalker. Out of sight: after him, waiting,
\ wandering, fleeing
enum: mate-mood  by-him waiting following fleeing fleeing-through off-out off-through after-him after-through
    sent-to-exit at-stalker away-after away-waiting away-wandering away-fleeing ;

\ ---- who is where ----
: her-room ( -- room )  self body-room ;
: here? ( -- flag )  her-room room-id = ;                    \ in the room being played
: dog-pos ( -- v )  hewie-id body-at ;
: dog-near ( F: -- d )  her-at dog-pos vec-dist ;
: st ( -- id | -1 )  stalker-in-game ;                       \ (+0x1AD5D6: in the game)
: st-with-her ( -- id | -1 )   \ (+0x1AD5D7: and in her room)
    st dup 0< if  exit  then  dup body-room her-room <> if  drop -1  then ;
: st-dist ( F: -- d )  st-with-her body-at her-at vec-dist ;
: st-up? ( -- flag )  her-st-cond @ down-cond <> ;          \ (+0xC4: not down)
: panicking? ( -- flag )  her-fear-bits @ 2 and 0<> ;       \ (FIONA_PANIC)
: rnd< ( F: p -- flag )  rnd f> ;
: frames-rnd ( n base -- frames )  >r  s>f rnd f* 30e f* f>s  r> + ;   \ ((s32)(30 * (n * r)) + base)
: mood! ( mood -- )  mate-mood ! ;
: anew ( mood -- )  mood!  -1 mate-exit ! ;                 \ (a mood about an exit not chosen yet)
: mood? ( mood -- flag )  mate-mood @ = ;

\ ---- the house's doors and halves (Rooms_*) ----
: door-tri ( door room -- tri )   \ (Rooms_DoorTri: its triangle on that room's side - the original's measure of a link)
    {: room :}  door-sides {: r0 e0 t0 r1 e1 t1 :}  r1 room = if  t1  else  r0 room = if  t0  else  -1  then then ;
: door-beyond ( door room -- room' )   \ (Rooms_DoorLeadsTo)
    {: room :}  door-sides {: r0 e0 t0 r1 e1 t1 :}  r0 room = if  r1  else  r0  then ;
: half-of ( room exit -- way )  dup 0< if  nip exit  then  0 exit-way ;   \ (Rooms_Side: the half an exit opens onto)
: st-half ( -- room way )   \ the stalker's room and the half he came in on
    st body-room  dup her-st-came @ half-of ;

\ ---- this room's exits (Progress_ExitOpen, Fiona_ExitUsable) ----
create ex-in /vec allot   create ex-out /vec allot   create ex-best /vec allot
: exit-in ( exit -- tri )  1 exit-spot >r ex-in vec! r> ;    \ (Rooms_ExitPosIn)
: exit-out ( exit -- tri )  0 exit-spot >r ex-out vec! r> ;  \ (Rooms_ExitPosOut)
: exit-room ( exit -- room )  her-room swap room-exit-leads drop ;
: exit-room-way ( exit -- way )  her-room swap room-exit-leads  over 0< if  nip exit  then  half-of ;
: her-exit-door ( exit -- door )  her-room swap room-exit-door ;
: leads? ( exit -- flag )  exit-room  dup 0< 0=  swap her-room <> and ;   \ (Rooms_ExitLeads)
: real-door? ( exit -- flag )  her-exit-door  dup 0< if  drop false exit  then  fixed? 0= ;   \ (not a doorway)
: exit-shut? ( exit -- flag )  her-room swap exit-open? 0= ;
: barred? ( door -- flag )  dup door-locked?  over door-held? or  swap fiona-kind locked-against? or ;
: usable? ( exit -- flag )   \ leading on, its door not locked, nor held, nor locked against her
    dup leads? 0= if  drop false exit  then
    her-exit-door  dup 0< if  drop true exit  then  barred? 0= ;
: my-way ( -- way )  her-room mate-door @ half-of ;          \ the half she came in on
: wrong-side? ( way tri -- flag )   \ (Fiona_WrongSide: an exit triangle on the other half)
    dup 0< if  2drop false exit  then  nav-flags both-halves and {: way f :}
    way 0= f other-half = and  way 1 = f one-half = and or ;
: score ( from to -- ) ( F: -- s )   \ (Fiona_ExitScore: the level distance and three times the rise)
    {: a b :}  b v.x sf@ a v.x sf@ f-  b v.z sf@ a v.z sf@ f-  vlen  b v.y sf@ a v.y sf@ f- fabs 3e f* f+ ;

\ ---- her way (Fiona_HeadFor): planned to (tri, v) - not across the room's divider - 0 on it, -1 none ----
: halves ( tri -- bits )  dup 0< if  drop 0 exit  then  nav-flags both-halves and ;
: across-from-her? ( tri -- flag )
    halves her-tri @ halves {: a b :}  a other-half = b one-half = and  a one-half = b other-half = and or ;
: head-for ( tri v -- 0 | -1 )
    {: tri v :}  tri across-from-her? if  -1 exit  then
    her-path her-tri @ her-at tri v blocked-floor path-plan-curved 0> if  0  else  -1  then ;
: on-way? ( -- flag )  her-path path-left? ;
: way-end ( -- )  her-path path-end ;
: touching-him? ( -- flag )  st-with-her dup 0< if  drop false exit  then  self swap 0e 0e bodies-touching? ;
: post-exit ( exit -- )  touching-him? if  drop  else  use  then ;   \ (her request 2: through it herself)

\ ---- the turns: with him, on her own (Fiona_Timers) ----
: timers ( -- )
    here? if
        dog-near 50e f< if  -1 mate-near-t +!  else  30 mate-near-t !  then
        mate-near-t @ 0= if  30 mate-near-t !  mate-alone @ if  -15  else  15  then  mate-period +!  then
        mate-period @ mate-period-len @ min mate-period !
        mate-flat @ 0= if
            dog-action @ steered-2-act = if  1 mate-flat !  mate-alone @ 0.25e rnd< and if  0 mate-period !  then  then
        else  dog-action @ steered-2-act <> if  0 mate-flat !  then  then
    then
    mate-alone @ 0=  here? or if  -1 mate-period +!  then
    mate-period @ 0> 0= if
        mate-alone @ 0= if  1 mate-alone !  16 150 frames-rnd  else  0 mate-alone !  11 300 frames-rnd  then
        dup mate-period-len !  mate-period !
    then
    mate-say-t @ if  -1 mate-say-t +!  else  20e rnd f* f>s 30 * 300 + mate-say-t !  then
    mate-look-t @ 0= if  30  else  mate-look-t @ 1-  then  mate-look-t ! ;
: nudge ( near far -- )   \ (Fiona_NudgePeriod: him within 50 / 100)
    dog-near 50e f< if  drop mate-period +! exit  then  nip
    dog-near 100e f< if  mate-period +!  else  drop  then ;

\ ---- Hewie tells her (Hewie_StateLookLow: Fiona_StateBlock's 0xD): a shut door he is at, to
\ open; else where he is - she keeps with him longer (or on her own the shorter) ----
: told ( what arg -- )
    here? 0= if  2drop exit  then
    swap 0= if
        fleeing mood?  fleeing-through mood? or  sent-to-exit mood? or  mate-alone @ or if  drop exit  then
        mate-exit !  sent-to-exit mood!
    else
        drop  mate-alone @ if  -150 -90  else  150 90  then  nudge
    then ;

\ ---- her mood out of sight (Fiona_MoodAI's first part) ----
: away-mood ( -- )
    mate-mood @ case
        by-him of  away-after  endof  following of  away-after  endof  after-him of  away-after  endof
        after-through of  away-after  endof  sent-to-exit of  away-after  endof
        waiting of  away-waiting  endof  at-stalker of  away-waiting  endof
        fleeing of  away-fleeing  endof  fleeing-through of  away-fleeing  endof
        off-out of  away-wandering  endof  off-through of  away-wandering  endof
        >r mate-mood @ r>
    endcase  mood!
    panicking? if  away-fleeing mood!  then
    mate-alone @ 0= if  away-waiting mood? if  away-after mood!  then  exit  then
    away-waiting mood? if
        -1 mate-away-t +!  mate-away-t @ 0< if  away-wandering mood!  way-end  -1e mate-link-left f!  then
    else  2 90 frames-rnd mate-away-t !  then ;

\ ---- her mood in the room being played (Fiona_MoodAI) ----
: back-in-sight ( -- )   \ (the out-of-sight moods, as she comes back: fleeing, if the stalker is a door away)
    mate-mood @ case
        away-after of  by-him mood!  endof
        away-waiting of  waiting mood!  endof
        away-wandering of  off-out anew  endof
        away-fleeing of
            st 0< 0= if  st body-room her-room <> if
                her-room my-way st-half fiona-kind 1 mate-avoid route-sides 0> if  fleeing anew exit  then
            then  then
            mate-alone @ 0= if  by-him mood!
            else  0.5e rnd< if  waiting mood!  else  off-out anew  then  then  endof
    endcase ;
: say-to-him ( -- )   \ (now and then a call to him - the controls act on it, FIONA_CMD: up, down, R3)
    mate-mood @ away-after < 0=  mate-flat @ or  mate-say-t @ or if  exit  then
    st-with-her 0< 0= if
        st-dist 50e f< if  go-gesture her-cmd ! exit  then
        dog-near 100e f> if  back-gesture her-cmd !  then  exit
    then
    dog-near 50e f< if  0.5e rnd< if  go-gesture  else  stay-gesture  then  her-cmd ! exit  then
    dog-near 100e f> if  back-gesture her-cmd !  then ;
: flee-chance ( -- flag )   \ (the stalker in sight: by her panic - the original's threat, +0x7BC)
    her-panic-level f@ f>s {: t :}
    t 80 > if  true exit  then  t 60 > if  0.2e rnd< exit  then  t 40 > if  0.15e rnd< exit  then
    t 20 > if  0.1e rnd< exit  then  t 10 > if  0.05e rnd< exit  then  false ;
: fleeing? ( -- flag )  fleeing mood?  fleeing-through mood? or ;
: his-exits ( -- bits )   \ (the exits into the stalker's room on his half - not to flee by - while he is elsewhere)
    st 0<  st-with-her 0< 0= or if  0 exit  then
    st-half {: r w :}  0  8 0 do  i exit-room r =  i exit-room-way w = and if  1 i lshift or  then  loop ;
\ the exit to flee by (ex-best its in point): the nearest - those she reaches before him first
: flee-exit ( -- exit | -1 )
    his-exits {: skip | beats e take :}  -1 to e  0e {: F: low :}
    8 0 do
        skip 1 i lshift and 0=  i usable? and if
            i exit-in {: t :}  my-way t wrong-side? 0= if
                i exit-out drop  her-at ex-out score {: F: s :}  false to take
                st-with-her dup 0< 0= if
                    body-at ex-out score s fswap f< {: first :}   \ (she there before him)
                    e 0< if  first to beats  true to take
                    else  beats 0= if  first if  true to beats  true to take  else  s low f< to take  then
                    else  first s low f< and to take  then  then
                else  drop  e 0< s low f< or to take  then
                take if  i to e  ex-best ex-in vec-copy  s to low  then
            then
        then
    loop  e ;
: random-exit ( -- exit | -1 )   \ (an exit she may use on her half, not the one she came in by; else that one)
    mate-door @ dup 0< if  drop 0  else  1 swap lshift  then  {: tried :}
    begin  tried $FF and $FF <> while
        rnd 8e f* f>s {: i :}
        tried 1 i lshift and 0= if
            tried 1 i lshift or to tried
            i usable? if  my-way i exit-in wrong-side? 0= if  i exit  then  then
        then
    repeat  mate-door @ ;
: near-in? ( -- flag )  her-at ex-in vec-dist 1e f< ;
: exit-in-way ( exit -- 0 | -1 )  dup exit-in swap drop  ex-in head-for ;
: through! ( through -- )  mood!  mate-exit @ mate-exit2 ! ;
\ on the way to the exit's in point; there, through it (opened first, if shut)
: at-exit ( through -- )
    {: through :}
    mate-exit @ exit-in-way if  by-him mood! exit  then
    near-in? if
        mate-exit @ exit-shut? if  mate-exit @ post-exit  then
        through through!
    then ;
\ an exit picked: a doorway taken at once; on her way; nearer its out point already, through
: set-off ( through -- )
    {: through :}
    mate-exit @ real-door? 0= if  through through!  then
    mate-exit @ exit-in-way if  -1 mate-exit !  by-him mood! exit  then
    mate-exit @ exit-shut? 0= if
        mate-exit @ exit-out drop  her-at ex-out vec-dist  her-at ex-in vec-dist f< if  through through!  then
    then ;
\ through the exit's out point: into the next room, out of sight
: gone-through ( -- )
    {: | door r e :}
    mate-exit @ exit-out drop
    her-at ex-out vec-dist 1e f< mate-exit @ her-exit-door 0< 0= and if
        mate-exit @ her-exit-door to door  her-room mate-exit @ room-exit-leads to e to r
        r body-room!  -1 body-tri!  e mate-door !  way-end
        door mate-link !  door r door-tri s>f mate-link-left f!  1 mate-out !  0e mate-gone f!
        2 90 frames-rnd mate-away-t !  0 her act.visible l!  exit
    then
    mate-exit @ exit-out ex-out head-for if  by-him mood!  then ;
: other-moods? ( -- flag )   \ (fleeing, after him, at the stalker: not to be turned)
    fleeing?  after-him mood? or  after-through mood? or  at-stalker mood? or ;
: mind ( -- )
    here? 0= if  away-mood exit  then
    back-in-sight
    her-mode @ led-mode = if  exit  then
    her-mode @ own-mode <> if  by-him mood! exit  then
    panicking? fleeing? 0= and if  fleeing anew  then
    say-to-him
    mate-look-t @ 0=  fleeing? 0= and  st-with-her 0< 0= and if  flee-chance if  fleeing anew  then  then
    other-moods? 0= if
        mate-alone @ if
            by-him mood?  following mood? or if  0.5e rnd< if  waiting mood!  else  off-out anew  then  then
        else  by-him mood?  following mood? or  sent-to-exit mood? or 0= if  by-him mood!  then  then
    then
    mate-mood @ case
        by-him of  dog-near 50e f> if  following mood!  then  endof
        following of
            hewie-id body-tri dog-pos head-for if  after-him anew  way-end
            else  dog-near 30e f< if  by-him mood!  then  then  endof
        fleeing of
            mate-exit @ 0< 0= if  fleeing-through at-exit
            else  flee-exit dup mate-exit !  0< if  by-him mood!  else  fleeing-through set-off  then  then  endof
        off-out of
            mate-exit @ 0< 0= if  off-through at-exit
            else  random-exit dup mate-exit !  0< if  by-him mood!  else  off-through set-off  then  then  endof
        after-him of
            mate-exit @ 0< 0= if  after-through at-exit
            else  random-exit dup mate-exit !  0< if  by-him mood!  else  after-through set-off  then  then  endof
        fleeing-through of  gone-through  endof
        off-through of  gone-through  endof
        after-through of  gone-through  endof
        sent-to-exit of
            mate-exit @ 0< if  by-him mood! exit  then
            mate-exit @ real-door? 0=  mate-exit @ exit-room her-room = or  mate-exit @ exit-shut? 0= or if  by-him mood! exit  then
            my-way mate-exit @ exit-in wrong-side? if  by-him mood! exit  then
            mate-exit @ exit-in-way if  by-him mood! exit  then
            mate-exit @ 0 her-pos door-in-area? 0= if  exit  then   \ (at its front: she opens it)
            mate-exit @ post-exit  by-him mood!  endof
        at-stalker of
            st-with-her 0<  st-up? 0= or if  by-him mood! exit  then
            st-dist  self body-dims fdrop  st-with-her body-dims fdrop f+ 1.5e f*  f>  panicking? or if  by-him mood!  then  endof
    endcase ;

\ ---- walking her (Fiona_MoveInput in the run): along her way to its point 1.5 on - walking off
\ on her own, else running; the stalker in her way, at him and a shove (her request 8) ----
: dir-to ( v -- )   \ her stick's way toward v, and her heading
    {: v :}  v v.x sf@ her-at v.x sf@ f-  v v.z sf@ her-at v.z sf@ f-  {: F: dx F: dz :}
    dx dz vlen f0= if  exit  then
    dx dx dz vlen f/ her-dir v.x sf!  0e her-dir v.y sf!  dz dx dz vlen f/ her-dir v.z sf!
    stick-heading her-heading f! ;
: in-his-way? ( v -- flag )   \ (the point ahead within reach of the stalker, on her level)
    {: v :}  st-with-her dup 0<  st-up? 0= or if  drop false exit  then  {: s :}
    her-at v.y sf@ s body-at v.y sf@ f- fabs  self body-dims fswap fdrop  s body-dims fswap fdrop  fmax f> if  false exit  then
    v s body-at vec-dist-xz  self body-dims fdrop  s body-dims fdrop f+  f<= ;
: walk-way ( -- )
    6 her-still !  0 her-run? !  0e her-dir v.x sf!  0e her-dir v.z sf!
    her-mode @ own-mode <> if  exit  then
    at-stalker mood? if
        st-with-her dup 0<  st-up? 0= or  panicking? or if  drop exit  then
        body-at dir-to  0 her-still !  -1 her-run? !
        her-heading f@ her-yaw f@ f- angle-wrap fabs 30e deg>rad f<  her-doing @ free-doing = and if  defend-by-pose  then
        exit
    then
    mate-mood @ following sent-to-exit 1+ within 0= if  exit  then
    on-way? 0= if  exit  then
    0 her-still !
    1.5e her-path her-at path-ahead drop
    ahead in-his-way? if  at-stalker mood!  then
    ahead dir-to
    off-out mood?  off-through mood? or 0= her-run? ! ;

\ ---- out of sight (Fiona_OffscreenFrame): her pace by her panic; her way left in a room walked
\ out; at a link's end the next room - a shut door opened, unless locked or held (she waits);
\ into the room being played, in by its door (opened as she comes) ----
: pace ( F: -- s )
    her-panic-stage @ calming-down = if  0.1e exit  then
    100e her-fear f@ f- 60e f/ {: F: k :}
    away-waiting mood?  away-wandering mood? or if
        her-danger @ chased = if  -0.1e  else  -0.15e  then  change-fear
        k 1e f< if  0.3e 1e k f- f*  0.45e k f* f+  else  0.45e  then  exit
    then
    away-after mood?  away-fleeing mood? or if
        her-danger @ chased = if  0.066667e  else  0.046667e  then  change-fear
        k 1e f< if  k 0.75e 1e k f- f* f+  else  1e  then  exit
    then  0e ;
: follow-way ( F: speed -- )   \ (Character_FollowWaypoints: speed along her way)
    her-path her-at path-ahead her-path path-i!  her-at ahead vec-copy ;
: in-by-door ( -- )   \ into the room being played by its door: in its doorway facing out, or opening it
    1 her act.visible l!
    room-id mate-door @ exit-open? if
        mate-door @ exit-in drop  mate-door @ exit-out drop
        ex-in vec@  ex-in ex-out vec-heading  put-her  fresh-her exit
    then
    mate-door @ exit-in drop  mate-door @ ex-in vec@ door-side 2*
    her-danger @ chased =  panicking? or 4 and +  {: kind :}
    mate-door @ kind door-user-spot 0< if  fdrop fdrop fdrop fdrop  fresh-her exit  then
    put-her  fresh-her  mate-door @ use ;
: next-link ( -- )   \ her way on from this room: after him by his, fleeing away from the stalker, else at random
    way-end  my-way {: cur | tried found :}
    st 0< 0= if
        st body-room her-room = if  true to found
        else  st-half {: r w :}  8 0 do  i exit-room r =  i exit-room-way w = and if  true to found  tried 1 i lshift or to tried  then  loop  then
    then
    away-fleeing mood? found and 0= if
        mate-alone @ if  0.5e rnd< if  away-waiting  else  away-wandering  then  else  away-after  then  mood!
    then
    away-after mood? if
        her-room cur  hewie-id body-room  dog-way @ dup 2 = if  drop cur  then
        fiona-kind -1 mate-avoid route-sides 0> 0= if  exit  then
        0 route-door mate-link !
        st 0< 0= if  mate-link @ her-room door-beyond st body-room = if  away-waiting mood! exit  then  then
        mate-link @ her-room door-tri s>f mate-link-left f!  0 mate-out ! exit
    then
    away-wandering mood? if  0 to tried  else  away-fleeing mood? 0= if  exit  then  then
    begin  tried $FF and $FF <> while
        rnd 8e f* f>s {: j :}
        tried 1 j lshift and 0= if
            tried 1 j lshift or to tried
            j leads?  j her-exit-door dup 0< if  drop true  else  door-locked? 0=  then  and
            her-room j half-of cur = and if
                j her-exit-door mate-link !  mate-link @ her-room door-tri s>f mate-link-left f!  0 mate-out ! exit
            then
        then
    repeat ;
: away-frame ( -- )
    here? if  1 her act.visible l! exit  then
    pace {: F: speed :}
    away-waiting mood? if  exit  then
    on-way? if  speed follow-way  on-way? if  exit  then  0e mate-link-left f!  0 mate-out ! exit  then
    mate-link-left f@ f0< if  next-link exit  then
    mate-link-left f@ speed f- mate-link-left f!  mate-gone f@ speed f+ mate-gone f!
    mate-link-left f@ f0< 0= if  exit  then
    way-end  mate-out @ if  exit  then  1 mate-out !
    mate-link @ her-room door-beyond {: r :}  mate-link @ r door-exit-in mate-door !  r body-room!  0e mate-gone f!
    2 90 frames-rnd mate-away-t !
    here? if  in-by-door exit  then
    -1 body-tri!
    r mate-door @ exit-open? 0= if   \ (a shut door out of sight: opened, unless she can't)
        r mate-door @ room-exit-door barred? if  -1e mate-link-left f! exit  then
        r mate-door @ fiona-kind fiona-noise doors-id send open-from-afar
    then
    mate-link @ r door-tri s>f mate-link-left f! ;

\ ---- Hewie went out by `exit` (the run's Fiona_Vt34): by him or following, she comes after him -
\ through with him if she is nearer the doorway than he is, or already in it; else after him out
\ of sight. The next room in (the run's Fiona_Vt38): through with him, placed by its door ----
: he-left ( exit -- )
    here? 0= if  drop exit  then
    0 mate-in !
    her-mode @ own-mode <> if  fresh-her  then
    by-him mood?  following mood? or 0= if  drop exit  then
    following mood!  way-end  -1e mate-link-left f!
    dup 0< if  drop exit  then
    {: e :}  e exit-in-way if  exit  then
    e her-exit-door mate-link !
    her-at ex-in vec-dist  dog-pos ex-in vec-dist f< if  1 mate-in !  0 mate-out ! exit  then
    e exit-out drop
    her-at ex-out vec-dist  ex-in ex-out vec-dist f<=  ex-out blocked-floor v-tri-in 0< 0= and if  1 mate-in !  1 mate-out !  then ;
: room-came ( room exit -- )
    drop  mate-in @ 0= if  drop  here? 0= if  0 her act.visible l!  0e mate-gone f!  2 90 frames-rnd mate-away-t !  then  exit  then
    0 mate-in !  dup body-room!
    mate-link @ swap door-exit-in dup mate-door !  dup 0< if  drop exit  then
    mate-out @ if  0  else  1  then  exit-spot  0< if  fdrop fdrop fdrop exit  then
    her-yaw f@ put-her  fresh-her  1 her act.visible l!
    mate-mood @ case
        sent-to-exit of  mate-alone @ if  waiting  else  by-him  then  mood!  endof
        at-stalker of  by-him mood!  endof
        fleeing-through of  fleeing anew  endof  after-through of  fleeing anew  endof
        off-through of  off-out anew  endof
    endcase ;

\ ---- the run's frames ----
: companion-start ( -- )   \ (the run starting: by him, a while with him)
    by-him anew  -1 mate-exit2 !  0 mate-alone !  300 mate-period !  300 mate-period-len !
    30 mate-near-t !  0 mate-flat !  300 mate-say-t !  30 mate-look-t !  -1e mate-link-left f!  0 mate-in !  0 mate-out !
    mate-avoid 13 cells 0 fill ;
: companion-frame ( -- )   \ (Fiona_Update in the run: her timers; out of sight her own frame; her mind and her way)
    timers  mind
    here? 0= if  away-frame exit  then
    walk-way ;
