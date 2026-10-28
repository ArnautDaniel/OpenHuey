\ fiona/seized.fs - Fiona seized by a stalker (F4b; src/game/fiona.c): taken to his side
\ (Fiona_JointAction kind 1: her spot by him - kFionaMeetOffsets by her costume and how he
\ takes her) and led there - by the hand (6: then dragged, $1401 again and again, shaking free
\ by the stick and the buttons against her panic's stage - Fiona_Shakes; six drags and she's
\ dragged off) or walking (8: carried off); grabbed outright (Fiona_StateGrabbed /
\ Fiona_StateHeld: held, at its end the game's over). The game's end is the `caught` flag.
IN: fiona.seized
USING: engine vectors game-state actors messages common facts flag-names keys paths rumble fiona.state fiona.anims fiona.sounds fiona.model fiona.controls fiona.moving fiona.fear fiona.doors fiona.hurt floor-flags ;

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
    by-the-hand = her-costume @ case
        normal-outfit of  4 5 ?  endof
        slip-dress of  7 8 ?  endof   normal-outfit-2 of  7 8 ?  endof
        scalpel-outfit of  9 $B ?  endof   4 of  $A $C ?  endof
        5 of  drop $D  endof   illegal-outfit of  $E $F ?  endof
        >r drop 5 r>
    endcase ;
: meet@ ( i -- ) ( F: -- x z deg )   \ (kFionaMeetOffsets, $3B2460: three floats a row)
    12 *  s" fiona-meet-offsets" exe-table  {: at addr n :}
    at 12 + n > if  0e 0e 0e exit  then
    addr at + {: row :}  row sf@  row 4 + sf@  row 8 + sf@ ;
create mt-at /vec allot
: spot-by ( who type -- tri | -1 )   \ (the spot, reachable straight from him and from her; her floor free; a way there)
    meet-index meet@ deg>rad  {: who F: x z turn :}
    turn who body-yaw f+ angle-wrap her-led-face f!
    x z who body-yaw rotate-by  {: F: dx dz :}
    who body-at vec@  {: F: wx wy wz :}  wx dx f+  wy  wz dz f+  mt-at vec!
    who body-tri who body-at mt-at 0 v-walk  dup 0< if  exit  then          ( tri )
    dup mt-at her-at 0 v-walk her-tri @ <> if  drop -1 exit  then
    her-tri @ dup 0< if  2drop -1 exit  then  nav-flags $80001 and if  drop -1 exit  then
    dup her-tri @ = if   \ (in her own triangle: straight there)
        her-path 0 path-point mt-at vec-copy  1 her-path !  0 her-path path-i!  exit
    then
    her-path her-tri @ her-at 3 pick mt-at blocked-floor path-plan-curved 0> 0= if  drop -1 exit  then ;   \ (curved: led_away)

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
: tugged ( -- )   \ (each of his steps leading her by the hand, or dragging her: a tug felt)
    her-model @ 0 0 1 motion-events 2 and if  $60 8 fiona-jolt  then ;
: pulled-up ( -- )  ended? if  free! to-idle exit  then  root-move ;
: pulled-free ( -- )  pull-free-anim -1 play-table  ['] pulled-up her-act ! ;
: dragged-off ( -- )
    leader-gone? if  pulled-free exit  then
    ended? anim@ dragged-off-anim = and if  caught-for-good  then
    root-move ;
: dragged ( -- )
    leader-gone? if  pulled-free exit  then  tugged
    her-shakes @ -1 <> if  shakes-now her-shakes +!  her-drags @ 1+ 10 *  her-shakes @ min her-shakes !  then
    ended? if
        her-free @ if  shake-free-anim -1 play-table  ['] pulled-up her-act ! exit  then
        1 her-drags +!
        her-drags @ 6 = if
            hewie-played? 0= if  fiona-occupied state-flag-set  then
            dragged-off-anim -1 play-table  ['] dragged-off her-act ! exit
        then
        dragged-anim -1 play-table
        hewie-played? if  0.25e rnd f> if  help-cry 0 her-sound  then  then   \ (the run: she cries for him, now and then)
    else her-shakes @ -1 <> if
        \ (out of her shakes - in the run, as many drags as her panic's stage)
        hewie-played? if  her-drags @ her-panic-stage @ =  else  her-shakes @  her-panic-stage @ 0 max 5 min cells shakes-to-break + @  >=  then  if
            -1 her-shakes !  -1 her-free !  broke-away
        then
    then then
    root-move ;
: at-his-hand ( -- )
    leader-gone? if  pulled-free exit  then  tugged
    ended? if  dragged-anim -1 play-table  ['] dragged her-act !  then
    root-move ;
: led-by-hand ( -- )
    leader-gone? if  free! to-idle exit  then  tugged
    settled? if
        hewie-played? if  0.25e rnd f> if  help-cry 0 her-sound  then  then   \ (the run: she may cry for him)
        led-arrive  0 her-free !  0 her-shakes !  0 her-drags !  -1 her-shake-rest !
        ['] at-his-hand her-act !
    else  led-step  then ;
\ ---- walking: at his side, carried off - at its end the game's over ----
: carried ( -- )
    leader-gone? if  free! to-idle exit  then
    ended? if  caught-for-good  then  root-move ;
: led-walking ( -- )
    leader-gone? if  free! to-idle exit  then
    settled? if  led-arrive  hewie-played? 0= if  fiona-occupied state-flag-set  then  ['] carried her-act !
    else  led-step  then ;

\ ---- taken (the request kind 9 answered): her gasp, led away by the hand ($1400) or walking
\ ($1500) - unless she isn't free, or there's no spot by him ----
: seized ( who type -- taken? )
    world-held state-flag?  her-mode @ dup ladder-mode = over thrown-mode = or swap down-mode = or  or if  2drop false exit  then   \ (Fiona_Reaction $A: not on a ladder, reacting or down)
    over 0< if  2drop false exit  then  over alive? 0= if  2drop false exit  then
    2dup spot-by dup 0< if  drop 2drop false exit  then
    her-led-tri !  her-led-at mt-at vec-copy                                    ( who type )
    her-path her-at path-rest 0.2e f* her-led-step f!
    her-led-face f@ her-yaw f@ f- angle-wrap fabs 0.2e f* her-led-turn f!
    swap her-hit-who !  -1 her-led !  0 her-ended !
    thrown-mode her-mode !  thrown-doing her-doing !  led-away-r her-sub !  fiona-struck s" danger" actor-named send danger-signal
    seized-sound 0 her-sound
    by-the-hand = if  led-by-hand-anim play-own  ['] led-by-hand  else  led-walking-anim play-own  ['] led-walking  then  her-act !
    true ;

\ ---- grabbed outright (Fiona_StateGrabbed: kind 3): getting up she's caught as she rises
\ ($1503 / $B03); else held ($1100), pulled back by him when 17 ahead of her is free ($1101);
\ her cry; at its end the game's over ----
create gb-at /vec allot
: held ( -- )
    her-cried @ 0= if  -1 her-cried !  caught-sound 0 her-sound  then
    ended? if  caught-for-good  then   \ (the motion's flag $20: at its end)
    anim@ pulled-back-anim = if  pull-back along-wall  else  root-move  then ;   \ (pull_back: along any wall)
:noname ( -- )
    0 her-cried !  0 her-ended !  -1 her-fall-turn !  her-yaw f@ her-fall-yaw f!
    anim@ up-from-back-anim = events-now 2 and 0= and if  held-front-anim -1 play-table  ['] held her-act ! exit  then
    anim@ dup up-from-forward-anim = over get-up-anim = or swap lying-anim = or  events-now 2 and 0= and if
        held-down-anim -1 play-table  ['] held her-act ! exit
    then
    her-yaw f@ 17e gb-at her-at vec-ahead   \ (pulled back where she's free to be and 17 ahead is clear)
    room-to-throw? if  her-tri @ dup 0< if  drop -1  else  her-at gb-at doorway-or-step v-walk  then  else  -1  then
    0< if  held-anim  else  0 her-fall-turn !  pulled-back-anim  then  -1 play-table
    ['] held her-act ! ; is grabbed

\ ---- held by homunculi (Fiona_StateHold / StateStruggle: reaction $12): down under them ($1405,
\ then $1406), struggling - each shake takes a frame off the hold (180) while shakes count (60) -
\ until the hold runs out or none in her room is holding her (move 8); then up ($1407) ----
: struggle ( -- )
    -1 her-hold +!
    her-hold @ 0> her-hold-k @ 0> and if  shakes-now  dup negate her-hold +!  negate her-hold-k +!  then
    anim@ case
        up-from-under-anim of  ended? if  to-idle  then  endof
        struggling-anim of
            settled? if  her-hold @ 0> her-carried-t @ 0> and 0= if  own-mode her-mode !  up-from-under-anim -1 play-table  then  then
        endof
        held-under-anim of  ended? if  struggling-anim -1 play-table  then  endof
    endcase ;
: pinned ( -- )
    settled? 0= if  exit  then
    anim@ dup held-under-anim <> swap struggling-anim <> and if  held-under-anim -1 play-table  then
    ['] struggle her-act ! ;
:noname ( -- )  180 her-hold !  60 her-hold-k !  ['] pinned her-act ! ; is held-down
