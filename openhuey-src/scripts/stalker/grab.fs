\ stalker/grab.fs - a stalker taking Fiona (src/game/pursuer.c, debilitas.c): closing on her and
\ asking her to his side (Pursuer_StateFaceFiona / StateWalkOn, Pursuer_CloseOnFiona: in reach
\ and facing her, `seize` - by the hand, or walking) - her floor not free for him, his attack
\ tables instead; she taken: by the hand his taunting hold (again and again - each a fright to
\ her - until she breaks free, or the sixth), walking his grab; refused, his attack tables. His
\ lunge (her panic calming down: on into his seize) and his grab at Hewie (Debilitas_Actions
\ $1002 / $1003).
IN: stalker.grab
USING: engine game-state actors messages common facts flag-names paths vectors stalker.anims stalker.names stalker.state stalker.senses stalker.moving stalker.search stalker.attack stalker.chase stalker.tables floor-flags ;

: fright ( F: amount -- )  f>cell panic-id send fright ;
\ her answer to his taking her (answer): none yet, taken, refused, broke free
enum: seize-answer  no-answer she-came she-refused she-broke-free ;
\ her floor free for him to take her on (Actor_TriFreeFor: not where nothing may stand)
: her-floor-free? ( -- flag )
    fiona body-tri dup 0< if  drop false exit  then  nav-flags doorway-or-step and 0= ;
\ she may be taken (Pursuer_MayGoForTarget: not out of reach - struck, seized -, not with the
\ stalkers blind)
: may-take? ( -- flag )  her-plight @ unreachable-plight <>  stalkers-blind state-flag? 0= and ;

\ ---- taken by the hand: his taunting hold, a fright each time (10 the first, then 5); she
\ breaks free - he lets go (held off his rests' first after: Pursuer_StateTauntHold); the sixth - he holds on while she's dragged off ----
: holding-on ( -- )  root-move ;
: let-go-end ( -- )   \ (his letting go to its end: held off her a while)
    root-move  ended? if  1 took !  rest-struck hold-off !  let-her-go  -1 step-done !  then ;
: taunting ( -- )
    root-move
    ended? 0= if  exit  then
    5e fright  1 taunts +!
    answer @ she-broke-free = if  let-go-hand-anim play-now  ['] let-go-end act-xt ! exit  then
    taunts @ 6 >= if  hold-on-anim play-now  ['] holding-on act-xt ! exit  then
    taunt-hold-anim play-now ;
: taunt-start ( -- )   \ (Pursuer_StateTaunt: a break free waits for the next taunt's end)
    root-move
    ended? if  taunt-hold-anim play-now  10e fright  0 taunts !  ['] taunting act-xt !  then ;
\ ---- taken walking: his grab, holding on ----
: carrying ( -- )  root-move ;

\ ---- her answer ----
: awaiting ( -- )
    root-move
    answer @ case
        she-came of  -1 leading !
              lead-type @ by-the-hand = if  hold-hand-anim play-now  ['] taunt-start  else  carry-anim play-now  ['] carrying  then
              act-xt !  endof
        she-refused of  0 leading !  pick-attack  endof
    endcase ;
\ ---- a look round (Pursuer_StateLookAround, action $16): his look round ($1A00) to its end, a
\ fright to her (Pursuer_Threat: Pursuer_ThreatAmount 20) ----
: looking-round ( -- )  ended? if  -1 step-done !  else  root-move  then ;
: look-round ( -- )  20e threat  glance-round-anim play  ['] looking-round look-around-act act! ;
\ ---- closing on her (Pursuer_StateFaceFiona / StateWalkOn, Pursuer_CloseOnFiona): on along his
\ way while his motion fades in (Pursuer_WalkOn), turning to her - by the hand, his stand then fading in too; then
\ once: she on a ladder, his attack; in reach (20) and facing her (20 degrees), taken if she may
\ be and her floor is free for him (else his attack; not to be taken, his table for her out of
\ reach); not, a look round ----
: walk-on? ( -- walking? )   \ (Pursuer_WalkOn)
    settled? if  false exit  then
    my-path path-left? if  walk-stride drop  else  root-move  then  true ;
: close-check ( -- )
    her-ladder-on @ 0< 0= if  pick-attack exit  then
    d-her 20e f<  d-her f0< 0= and
    fiona body-at heading-to yaw f- angle-wrap fabs 20e deg>rad f< and 0= if  look-round exit  then
    may-take? 0= if  unreachable-plight choose exit  then
    her-floor-free? 0= if  pick-attack exit  then
    no-answer answer !  1 leading !  lead-type @ fiona send seize   \ (asked: let go if he moves on)
    ['] awaiting act-xt ! ;
: closing ( -- )   \ (while his motion fades in, turned toward her too)
    walk-on? if  fiona body-at turn-rate turn-to fdrop exit  then  close-check ;
: facing-up ( -- )   \ (Pursuer_StateFaceFiona: the walk faded, his stand; then closing)
    walk-on? if  exit  then  still-anim play  ['] closing act-xt !  closing ;
:noname ( type -- )
    dup lead-type !  by-the-hand = if  ['] facing-up take-by-hand-act  else  ['] closing take-walking-act  then  act! ; is take-her

\ ---- his lunge: her panic calming down, on into his seize (combo 8) ----
$1306 constant lunge-anim   $E06 constant grab-dog-anim   \ (Debilitas's)
: lunging ( -- )
    root-move
    ended? if  her-stage @ calming-down = if  8 combo-start  else  -1 step-done !  then  then ;
:noname ( -- )  lunge-anim play-now  ['] lunging own-act-3 act! ; is lunge

\ ---- his grab at Hewie: at its key, Hewie by his hand (within 5) is held (a hold, from
\ behind); over at its end, losing her, or her 60 off ----
$1E constant grab-hand-bone
create hand-at /vec allot
: dog-at-hand? ( -- flag )
    hewie here? 0= if  false exit  then
    my-model @ grab-hand-bone bone-pos hand-at vec!
    hand-at v.y sf@  hewie body-at v.y sf@ 5e f- f> 0= if  false exit  then
    hand-at v.y sf@  hewie body-at v.y sf@  hewie body-dims fswap fdrop f+ 5e f+ f< 0= if  false exit  then
    hand-at hewie body-at vec-dist-xz  hewie body-dims fdrop 5e f+ f< ;
: grabbing ( -- )
    root-move
    my-model @ 0 0 0 motion-events 2 and  sent @ 0= and if
        dog-at-hand? if  hold-request 3 10e f>cell hewie send hit  -1 sent !  then   \ (from behind)
    then
    ended?  sees-fiona @ 0= or  d-her 60e f< 0= or  d-her f0< or if  -1 step-done !  then ;
:noname ( -- )  0 sent !  grab-dog-anim play-now  ['] grabbing own-act-4 act! ; is grab-dog
