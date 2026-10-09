\ fiona/seized.fs - Fiona seized by a stalker (F4b; src/game/fiona.c): taken to his side
\ (Fiona_JointAction kind 1: her spot by him - kFionaMeetOffsets by her costume and how he
\ takes her) and led there - by the hand (6: then dragged, $1401 again and again, shaking free
\ by the stick and the buttons against her panic's stage - Fiona_Shakes; six drags and she's
\ dragged off) or walking (8: carried off); grabbed outright (Fiona_StateGrabbed /
\ Fiona_StateHeld: held, at its end the game's over). The game's end is the `caught` flag.
IN: fiona.seized
USING: engine game-state actors messages common facts flag-names keys paths fiona.state fiona.model fiona.moving fiona.fear fiona.doors fiona.hurt ;

defer to-idle ( -- )
: leader ( -- id )  her-hit-who @ ;
: leader-gone? ( -- flag )  her-led @ 0=  leader 0< or  leader alive? 0= or ;
: broke-away ( -- )   \ (he's told, if he's still there)
    leader 0< if  exit  then  leader alive? 0= if  exit  then  leader send broke-free ;
\ the game's end: dragged or carried off, held (unless the story keeps it from ending)
: caught-for-good ( -- )   \ (once)
    her-ended @ if  exit  then  -1 her-ended !
    capture-no-end state-flag? if  exit  then  caught state-flag-set ;
: free! ( -- )  0 her-led !  fiona-occupied state-flag-clear ;

\ ---- her spot by him (Fiona_JointAction kind 1) ----
: meet-index ( type -- i )   \ (Fiona_MeetIndex: by her costume, by the hand or not)
    6 = her-costume @ case
        0 of  if  4  else  5  then  endof
        1 of  if  7  else  8  then  endof   2 of  if  7  else  8  then  endof
        3 of  if  9  else  $B  then  endof   4 of  if  $A  else  $C  then  endof
        5 of  drop $D  endof   6 of  if  $E  else  $F  then  endof
        >r drop 5 r>
    endcase ;
: meet@ ( i -- ) ( F: -- x z deg )   \ (kFionaMeetOffsets, $3B2460)
    12 * $3B2460 +  dup 12 exe-bytes  nip  dup 0= if  drop 0e 0e 0e exit  then
    dup sf@  dup 4 + sf@  8 + sf@ ;
create mt-at 12 allot  fvariable sdx  fvariable sdz
: spot-by ( who type -- tri | -1 )   \ (the spot, reachable straight from him and from her; her floor free; a way there)
    meet-index meet@ deg>rad  dup body-yaw f+ angle-wrap her-led-face f!       ( who ) ( F: x z )
    dup body-yaw rotate-by  sdz f!  sdx f!
    dup body-at vec@  sdz f@ f+  frot sdx f@ f+  frot frot  mt-at vec!
    dup body-tri swap body-at mt-at 0 v-walk dup 0< if  exit  then               ( tri )
    dup mt-at her-at 0 v-walk her-tri @ <> if  drop -1 exit  then
    her-tri @ dup 0< if  2drop -1 exit  then  nav-flags $80001 and if  drop -1 exit  then
    dup her-tri @ = if   \ (in her own triangle: straight there)
        her-path 0 path-point mt-at vec-copy  1 her-path !  0 her-path path-i!  exit
    then
    her-path her-tri @ her-at 2 pick mt-at blocked-floor path-plan 0> 0= if  drop -1 exit  then ;

\ ---- led there: turned to the spot's heading a fifth at a time, along her way a fifth of it a
\ step while the animation fades in; there, on the spot ----
: led-step ( -- )
    her-yaw f@ her-led-face f@ her-led-turn f@ turn-toward fdrop her-yaw f!
    her-led-step f@ her-path her-at path-ahead her-path path-i!
    ahead blocked-floor v-tri-in dup 0< if  drop  else  her-tri !  her-at ahead vec-copy  then ;
: led-arrive ( -- )
    her-led-tri @ her-tri !  her-at her-led-at vec-copy  her-led-face f@ fdup her-yaw f! her-heading f! ;

\ ---- by the hand: at his side, dragged; she shakes ($1401 again and again: each drag lets ten
\ more shakes count), past her panic's stage's count (8 12 23 35 70; calming 0) she breaks away
\ ($1403) and he's told; the sixth drag she's dragged off ($1404: at its end the game's over).
\ He lets go: she pulls free ($F02) ----
create shakes-to-break  8 , 12 , 23 , 35 , 70 , 0 ,
: shakes-now ( -- n )   \ (Fiona_Shakes: the stick swung past 120 degrees, or out from rest; the buttons)
    0  her-stick sf@ her-stick 8 + sf@ vlen                                   ( n ) ( F: len )
    fdup 0.8e f<= if  0.2e f< if  -1 her-shake-rest !  then
    else
        fdrop  her-stick sf@ her-stick 8 + sf@ fatan2
        her-shake-rest @ if  1+  0 her-shake-rest !  her-shake-a f!
        else  fdup her-shake-a f@ f- angle-wrap fabs 2.0943952e f> if  1+  her-shake-a f!  else  fdrop  then  then
    then
    6 0 do  i start-button <> if  i button-pressed? if  1+  then  then  loop ;
: pulled-up ( -- )  ended? if  free! to-idle exit  then  root-move ;
: pulled-free ( -- )  $F02 -1 play-table  ['] pulled-up her-act ! ;
: dragged-off ( -- )
    leader-gone? if  pulled-free exit  then
    ended? anim@ $1404 = and if  caught-for-good  then
    root-move ;
: dragged ( -- )
    leader-gone? if  pulled-free exit  then
    her-shakes @ -1 <> if  shakes-now her-shakes +!  her-drags @ 1+ 10 *  her-shakes @ min her-shakes !  then
    ended? if
        her-free @ if  $1403 -1 play-table  ['] pulled-up her-act ! exit  then
        1 her-drags +!
        her-drags @ 6 = if  $1404 -1 play-table  ['] dragged-off her-act ! exit  then
        $1401 -1 play-table
    else her-shakes @ -1 <> if
        her-shakes @  her-panic-stage @ 0 max 5 min cells shakes-to-break + @  >= if
            -1 her-shakes !  -1 her-free !  broke-away
        then
    then then
    root-move ;
: at-his-hand ( -- )
    leader-gone? if  pulled-free exit  then
    ended? if  $1401 -1 play-table  ['] dragged her-act !  then
    root-move ;
: led-by-hand ( -- )
    leader-gone? if  free! to-idle exit  then
    settled? if
        led-arrive  0 her-free !  0 her-shakes !  0 her-drags !  -1 her-shake-rest !
        ['] at-his-hand her-act !
    else  led-step  then ;
\ ---- walking: at his side, carried off - at its end the game's over ----
: carried ( -- )
    leader-gone? if  free! to-idle exit  then
    ended? if  caught-for-good  then  root-move ;
: led-walking ( -- )
    leader-gone? if  free! to-idle exit  then
    settled? if  led-arrive  fiona-occupied state-flag-set  ['] carried her-act !
    else  led-step  then ;

\ ---- taken (the request kind 9 answered): her gasp, led away by the hand ($1400) or walking
\ ($1500) - unless she isn't free, or there's no spot by him ----
: seized ( who type -- taken? )
    world-held state-flag? her-mode @ 0<> or if  2drop false exit  then
    over 0< if  2drop false exit  then  over alive? 0= if  2drop false exit  then
    2dup spot-by dup 0< if  drop 2drop false exit  then
    her-led-tri !  her-led-at mt-at vec-copy                                    ( who type )
    her-path her-at path-rest 0.2e f* her-led-step f!
    her-led-face f@ her-yaw f@ f- angle-wrap fabs 0.2e f* her-led-turn f!
    swap her-hit-who !  -1 her-led !  0 her-ended !
    4 her-mode !  $A her-doing !  9 her-sub !  1 s" danger" actor-named send danger-signal
    $43 0 her-sound
    6 = if  $1400 play-own  ['] led-by-hand  else  $1500 play-own  ['] led-walking  then  her-act !
    true ;

\ ---- grabbed outright (Fiona_StateGrabbed: kind 3): getting up she's caught as she rises
\ ($1503 / $B03); else held ($1100), pulled back by him when 17 ahead of her is free ($1101);
\ her cry; at its end the game's over ----
create gb-at 12 allot
: held ( -- )
    her-cried @ 0= if  -1 her-cried !  $41 0 her-sound  then
    ended? if  caught-for-good  then   \ (the motion's flag $20: at its end)
    anim@ $1101 = if  pull-back  else  root-move  then ;
:noname ( -- )
    0 her-cried !  0 her-ended !  -1 her-fall-turn !  her-yaw f@ her-fall-yaw f!
    anim@ $100D = events 2 and 0= and if  $1503 -1 play-table  ['] held her-act ! exit  then
    anim@ dup $100A = over $B02 = or swap $B01 = or  events 2 and 0= and if
        $B03 -1 play-table  ['] held her-act ! exit
    then
    her-yaw f@ 17e gb-at her-at vec-ahead
    her-tri @ dup 0< if  drop -1  else  her-at gb-at $80001 v-walk  then
    0< if  $1100  else  0 her-fall-turn !  $1101  then  -1 play-table
    ['] held her-act ! ; is grabbed
