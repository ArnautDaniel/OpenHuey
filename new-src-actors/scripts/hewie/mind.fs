\ hewie/mind.fs - Hewie's mind and his frame (Hewie_Update): his upkeep and obedience, what he
\ notices, his own decisions, his mode's behaviour (calm / wary / tense), what he does next by
\ his trust (the weighted lists), and his body as it is shown (overlays, neck, feet, sounds).
IN: hewie.mind
USING: engine actors common facts paths vectors messages hewie-names hewie.state hewie.body hewie.tables hewie.anims hewie.sounds hewie.model hewie.moving hewie.states hewie.offscreen hewie.fight hewie.actions hewie.commands hewie.fetch hewie.exits flag-names floor-flags ;

$FF constant no-call   \ (his-call: nobody called him; 0 Fiona)
\ report_fiona_near: how near she is, for the story (1 within 20, 2 within 50, 3 further)
: report-near ( -- )
    her-with? 0= if  exit  then
    her-dist  fdup 20e f<= if  fdrop 1  else  50e f<= if  2  else  3  then  then  his-near ! ;
\ Hewie_Alert: the stalker in his room within 200, ahead of his head (125 degrees): the danger
\ is told
: alert ( -- )
    pursuer dup active? 0=  his-away @ or  his-cond @ down-cond = or if  drop no-alert his-alert !  exit  then
    dup with? if
        dup body-at his-at vec-dist 200e f<= if
            dup body-at heading-to  his-yaw his-head-yaw f@ f+ angle-wrap f- angle-wrap fabs
            125e deg>rad f<= if  his-alert-what !  sight-alert his-alert !  alerted  exit  then
        then
    then
    drop  his-alert @ unused-alert = if  no-alert his-alert !  then ;

\ ---- Hewie_Upkeep: his timers, health and mood each frame ----
: upkeep ( -- )
    his-room room-id <> away!
    his-cooldown @ if  -1 his-cooldown +!  then
    his-cond @ down-cond <>  his-hp @ 0<> and  his-hp @ 100 < and if   \ (a point of health back every 300 frames)
        -1 his-hurt-t +!  his-hurt-t @ 0<= if  300 his-hurt-t !  his-hp @ 1+ 100 min hp!  then
    then
    his-cond @ down-cond <> if
        his-hp @ 0= if  his-away @ if  1 hp!  else  down-cond cond!  normal-mood -1 set-mood  then  then
    else his-hp @ 0= if
        his-away @ if  his-action @ knocked-down-act <> if  knocked-down-act 0 want  then
        else  his-action @ choose-act = if  knocked-down-act 0 want  then  then
    else   \ (down, his health back: hurt)
        hurt-cond cond!  obeys
        his-away @ if  his-hide @ if  his-action @ hide-act <> if  hide-act 0 want  then  else  to-default  then  then
    then then
    his-cond @ down-cond <> if
        his-cond @ hurt-cond <> if
            his-hp @ 30 < his-mood @ angry-mood <> and if  hurt-cond cond!  calm-down  then
        else
            his-hp @ 30 < 0=  his-mood @ angry-mood = or if  well-cond cond!  then
        then
    then
    his-mood @ angry-mood <> his-hit-t @ 0<> and if
        -1 his-hit-t +!
        his-hit-t @ 0= his-hits @ 0<> and if  -1 his-hits +!  300 his-hit-t !  then
    then
    his-mode @ engaged-mode <> his-action @ ready-act <> and if  0 his-ready !  then
    his-yelp @ if  -1 his-yelp +!  then
    his-creature-t @ if  -1 his-creature-t +!  then
    1 his-snd-t +!  his-snd-t @ 3000 > if  3000 his-snd-t !  then
    his-mood @ normal-mood <> if
        -1 his-mood-time +!  his-mood-time @ 0<= if  calm-down  then
    then
    his-praise-b @ if  -1 his-praise-b +!  his-praise-b @ 0= if  0 his-praise-a !  then  then
    his-pet-time @ if  -1 his-pet-time +!  then
    his-wait @ 0> if  -1 his-wait +!  then
    his-wait @ 0<=  on-wait-over breaks-in? and if  1 his-pending !  then
    his-hold-call @ if  -1 his-hold-call +!  then ;

\ ---- Hewie_Obedience: obeying her for a while, then waiting (doing as he likes) for a while;
\ waiting near her he nudges her now and then ----
: wait-time! ( -- )  black-shepherd? if  wait-time-shepherd  else  wait-time  then  trust-of his-obey ! ;
: whining? ( -- flag )  his-action @ dup whine-act = swap whine-praise-act = or ;
: obedience ( -- )
    -1 his-obey +!
    his-waiting @ 1 =  her-with? and if
        her-dist 30e f< if
            -2 his-obey +!
            whining? 0= if
                1 his-nudge +!  his-nudge @ 91 >= if  0 his-nudge !  nudge-reaction react  then
            else  0 his-nudge !  then
        else  0 his-nudge !  then
    then
    his-obey @ 0<= if
        his-obey-marked @ 1 = if
            0 his-obey !
            his-waiting @ 0= if
                his-mood @ pleased-mood <> if
                    1 his-waiting !  wait-time!
                    on-waiting breaks-in? if  1 his-pending !  then
                then
            else
                0 his-waiting !  obey-time!
                on-obeying breaks-in? if  his-away @ if  loud-noise-act 0 want  else  1 his-pending !  then  then
                0 his-praise-due !
            then
        else  1 his-obey-marked !  then
    then
    his-waiting @ 0= if  0 his-praise-due !  then ;

\ ---- what he does next ----
\ how far she is: 0 within 10 .. 6 beyond 100, or not here
: her-band ( -- n )
    her-with? 0= if  6 exit  then
    her-dist
    fdup 10e f< if  fdrop 0 exit  then  fdup 20e f< if  fdrop 1 exit  then
    fdup 40e f< if  fdrop 2 exit  then  fdup 60e f< if  fdrop 3 exit  then
    fdup 80e f< if  fdrop 4 exit  then  100e f< if  5  else  6  then ;
\ Hewie_SituationList: the list of a table that fits (calm or followed: her distance; chased:
\ 0 while she panics; the stalker here hurt or held off 1, within 30 of him 2, of her 3; else
\ 4 + her distance)
: stalker-near? ( v -- flag )  pursuer body-at vec-dist 30e f< ;
: situation-for ( danger -- n )
    chased <> if  her-band exit  then
    his-panic @ panicking >= if  0 exit  then
    pursuer with? if
        st-cond @ hurt-cond =  pursuer mode-of struck-mode = or if  1 exit  then
        his-at stalker-near? if  2 exit  then
        her-with? if  fiona-at stalker-near? if  3 exit  then  then
    then
    4 her-band + ;
: situation ( -- n )  his-danger @ situation-for ;
\ a weighted pick by his trust from a list (count, then an action and 8 weights an entry); an
\ action that `favoured` names weighs 20 more
: pick-from ( list favoured -- act )
    {: list favoured | sum :}  100 roll {: r :}
    list cell+  list @ 0 ?do
        dup his-trust @ 1+ cells + @  sum + to sum
        dup @ favoured = if  sum 20 + to sum  then
        r sum < if  @ unloop exit  then
        9 cells +
    loop  drop 0 ;
: list-of ( table i -- list )  1+ cells + @ ;
\ Hewie_WhatNext: by the danger, obeying or waiting (growling at the stalker, the creatures)
: growl? ( -- flag )   \ (she hides from the stalker here, not caught: by chance he draws it off)
    pursuer with? hiding? and 0= if  false exit  then
    pursuer mode-of dup unreachable-mode = swap struck-mode = or if  false exit  then
    growl-chance by-chance ;
: hostile? ( -- flag )  his-creature-t @ 0> ;   \ (a creature up and about in his room)
: tense-wait-table ( -- table )  black-shepherd? if  tense-wait-shepherd-lists  else  tense-wait-lists  then ;
: what-next ( -- )
    growl? if  draw-off-act 0 want exit  then
    hostile? if
        his-waiting @ 0= if  tense-lists  else  tense-wait-table  then
        chased situation-for list-of -1 pick-from 0 want exit
    then
    his-waiting @ 0= if
        his-danger @ case  calm of  calm-lists  endof  followed of  followed-lists  endof  >r tense-lists r>  endcase
    else
        his-danger @ case  calm of  calm-wait-lists  endof  followed of  followed-wait-lists  endof  >r tense-wait-table r>  endcase
    then
    situation list-of -1 pick-from 0 want ;
\ Hewie_WhenIdle (out of the room being played): staying hidden while he hides; waiting a while
\ hurt; else from the idle list, the one the danger favours (waiting) or going to her weighted more
: idle ( -- )
    his-hide @ if  hide-act 0 want exit  then
    his-cond @ hurt-cond = rnd 0.25e f< and if  idle-wait trust-of his-wanted !  wait-a-while-act 0 want exit  then
    his-waiting @ if
        his-danger @ case  calm of  out-random-act  endof  followed of  wait-off-act  endof  chased of  to-her-room-act  endof  >r -1 r>  endcase
    else  to-her-room-act  then
    idle-list swap pick-from ?dup if  0 want  then ;
' idle is when-idle

\ ---- Hewie_OwnDecisions: what he decides on his own when nothing else drives him ----
: to-fiona ( -- )   \ at her if he can get to her, else he stops sulking
    her body-tri fiona-at plan-to if  harry-her-3-act 0 want  else  normal-mood -1 set-mood  then ;
: to-stalker ( -- )   \ answers it if he can get to it, else forgets the call
    pursuer body-tri pursuer body-at plan-to if  answer-call-act 0 want  else  0 his-call !  then ;
: nearer-her? ( -- flag )  her-dist  his-at pursuer body-at vec-dist f< ;
: staying ( -- done? )   \ angry: a call, her having hit him, or whoever is here (the nearer)
    0 his-hold-call !
    his-call @ no-call <> if
        his-call @ 0= her-with? and if  to-fiona  else  answer-call-act 0 want  then  true exit
    then
    his-hits @ 0> her-with? and if  to-fiona true exit  then
    her-with? pursuer with? and if  nearer-her? if  to-fiona  else  to-stalker  then  true exit  then
    her-with? if  to-fiona true exit  then
    pursuer with? if  to-stalker true exit  then
    false ;
: facing-down-slope? ( -- flag )   \ on sloped ground, facing down it
    his-tri 0< if  false exit  then
    his-tri nav-flags slope-floor and 0= if  false exit  then
    his-tri tri-normal {: F: nx ny nz :}
    ny 1e f= if  false exit  then
    nx fsq nz fsq f+ fsqrt {: F: n :}  n f0= if  false exit  then
    nx n f/ his-yaw fsin f*  nz n f/ his-yaw fcos f* f+  0.5e f> ;
: panic-brings? ( -- flag )   \ her panic newly at its height or calming down: by chance (not angry) he comes
    false
    his-panic @ calming-down = if  his-panic-seen @ 2 or his-panic-seen !  his-mood @ angry-mood <> if  drop panic5-chance by-chance  then
    else his-panic-seen @ 1 and 0= his-panic @ panicking = and if
        his-panic-seen @ 1 or his-panic-seen !  his-mood @ angry-mood <> if  drop panic4-chance by-chance  then
    then then ;
\ the ball he follows: lost once it lies still; going (not hidden) ahead of him (within 120
\ degrees of his head's way) where he can get straight to: after it
: ball-ahead? ( -- flag )
    ball-there? 0= his-action @ dup fetch-act = swap yelp-anim-act = or or if  false exit  then
    ball-speed 0.5e f< if  forget-ball false exit  then
    ball-at heading-to  his-yaw his-head-yaw f@ f+ angle-wrap f- angle-wrap fabs  120e deg>rad f< 0= if  false exit  then
    self body-mask on-ball-line? 0= if  false exit  then
    his-action @ whine-act = if  obeys  then  true ;
: down-stays-down ( -- )   \ (down: knocked down, unless going limp)
    his-action @ dup knocked-down-act <> swap go-limp-act <> and if  knocked-down-act 0 want  then ;
: off-slope? ( -- flag )   \ (standing, sitting or lying on a slope, facing down it)
    anim-group in-standing-up < his-action @ off-slope-act <> and facing-down-slope? and ;
: own-decisions ( -- )
    his-mode @ free-mode <> if  0 his-scene-req !  exit  then
    his-cond @ down-cond = if  down-stays-down exit  then
    his-yelp @ 0> his-action @ yelp-anim-act <> and if  yelp-anim-act 0 want  exit  then
    first-door case   \ (in a door's way: Hewie_FirstDoor)
        0 of  his-action @ squeeze-out-act <> if  squeeze-out-act 0 want  then  exit  endof
        1 of  his-action @ dup walk-out-act <> swap squeeze-out-act <> and if  walk-out-act 0 want  then  exit  endof
    endcase
    his-cmd @ 0= his-cmd @ nothing-breaks-in and or if  exit  then
    her-with? if
        his-panic-seen @ 2 and 0= if  panic-brings? if  answer-call-act 0 want  exit  then  then
        his-panic @ panicking < if  0 his-panic-seen !  then
    then
    his-mood @ angry-mood = if  staying if  exit  then  then
    \ Fiona held: newly so, by chance he goes for whoever holds her - the stalker
    her-with? fiona-mode @ thrown-mode = and fiona-sub @ dup led-away-r = swap held-down-r = or and
    dup  his-held @ 0= and  his-mood @ angry-mood <> and  pursuer with? and if
        go-for-holder-chance by-chance if  his-held !  harry-stalker-act 0 want exit  then
    then  his-held !
    his-danger @ chased <> his-alert-was @ no-alert = and his-alert @ no-alert <> and his-action @ yelp-anim-act <> and if
        prick-ears-act 0 want  exit
    then
    his-scene-req @ 1 = if  0 his-scene-req !  his-action @ scene-pose-act <> if  scene-pose-act 0 want  exit  then  then
    his-danger @ calm <> if  forget-ball  else  ball-ahead? if  fetch-act 0 want  exit  then  then
    off-slope? if  off-slope-act 0 want  exit  then
    on-footing breaks-in?  anim-group dup in-walking = swap in-standing-up < or and if
        his-action @ off-edge-act <> if
            cp-try head-at  cp-try -1 body-tri-to 0< if  off-edge-act 0 want  exit  then
        then
        her-with? his-action @ step-off-her-act <> and  her-dist 5e f< and if  step-off-her-act 0 want  exit  then
    then
    his-praise-due @ 1 = if
        0 his-praise-due !
        her-with? his-waiting @ 1 = and on-praise-due breaks-in? and if
            whining? 0= if  whine-praise-act 0 want  then
        then
    then ;
\ Hewie_StandingFrame (while a cutscene plays): down stays down; a slope; Fiona right by him
: standing-frame ( -- )
    his-away @ his-mode @ or if  exit  then
    his-cond @ down-cond = if  down-stays-down exit  then
    off-slope? if  off-slope-act 0 want  exit  then
    his-action @ step-off-target-act = if  exit  then
    her-with? if  his-at fiona-at vec-dist-xz 5e f< if  her his-target !  step-off-target-act 0 want  then  then ;

\ ---- his mode's behaviour (Hewie_StateCalm / Wary / Tense): as the danger changes ----
create mood-acts  3 cells allot
: mode-behaviour ( own -- )
    his-cond @ down-cond = if  drop exit  then
    his-danger @ over = if
        drop  his-pending @ if
            his-away @ if  0 his-pending !  when-idle  else  what-next  0 his-pending !  then
        then  exit
    then
    his-danger @ case
        calm of  drop  0 his-hits !  0 his-did-2 !  0 his-did !  no-alert his-alert !  endof
        followed of  0 his-hits !  0 his-did-2 !  0 his-did !
              calm = his-away @ 0= and if
                  his-alert @ if  feel-pursuer  then
                  on-alert breaks-in? if  prick-ears-act 0 want  then
                  followed cells mood-acts + @ his-mood-act !  exit
              then  endof
        chased of  drop  0 his-hits !  0 his-did-2 !  0 his-did !
              his-away @ 0= if
                  pursuer dup with? if  his-alert-what !  chase-alert his-alert !  else  drop  then
                  his-alert @ dup chase-alert = swap sight-alert = or if  feel-pursuer  then
              then  endof
        >r 2drop r> drop exit
    endcase
    his-danger @ calm max chased min cells mood-acts + @ his-mood-act !
    on-danger breaks-in? if  1 his-pending !  then ;
: st-calm ( -- )  calm mode-behaviour ;
: st-wary ( -- )  followed mode-behaviour ;
: st-tense ( -- )  chased mode-behaviour ;
' st-calm mood-acts !  ' st-wary mood-acts cell+ !  ' st-tense mood-acts 2 cells + !

\ ---- his sounds (Hewie_AnimSounds): the whimpers while their animation plays; a bark's or
\ growl's sound as it starts, heard by the others ----
: anim-cry ( anim -- snd | 0 )
    case
        refuse-anim of  whine-sound  endof
        bark-anim of  bark-sound  endof  bark-sitting-anim of  bark-sound  endof  bark-lying-anim of  bark-sound  endof
        bark-tense-anim of  his-action @ heard-her-act = if  bark-sound  else  growl-sound  then  endof
        bark-upset-anim of  his-action @ heard-her-act = if  bark-sound  else  upset-growl  then  endof
        bark-limping-anim of  limping-bark  endof  ears-down-anim of  cowed-whine  endof
        go-down-anim of  yelp-sound  endof  hanging-far-anim of  yelp-sound  endof  hanging-near-anim of  yelp-sound  endof
        >r 0 r>
    endcase ;
: barking-at? ( -- flag )  his-action @ dup draw-off-act = over bark-at-her-act = or swap bark-at-target-act = or ;
: anim-sounds ( -- )
    him-model act.frame sf@                                          ( F: frame )
    anim@ case
        stance-anim of  stance-growl make-sound  endof
        crouch-anim of  his-ready @ 1 = if  ready-growl make-sound  then  endof
        stance-upset-anim of  upset-whine make-sound  endof
        stand-hurt-anim of  hurt-whimper make-sound  endof  lie-hurt-anim of  hurt-whimper make-sound  endof
        limp-anim of  hurt-whimper make-sound  endof
    endcase
    anim@ his-snd-anim @ <>  fdup his-snd-frame f@ f< or if   \ (it starts)
        anim@ anim-cry ?dup if  make-sound  then
        anim@ bark-anim barks-end within if  barking-at? if  near-noise  else  0  then  his-tri noise-make  then
        anim@ yelp-anim = if  far-noise his-tri noise-make  then
    then
    his-snd-frame f!  anim@ his-snd-anim !
    anim@ go-down-anim = $10 event? and if  splash-here  then ;   \ (his splash in water)

\ ---- his overlays: head, ears and tail playing their own motions by his animation and mood
\ (Hewie_SetOverlays / OverlayAnim / IdleOverlay / TailOverlay) ----
: overlay ( anim -- )  model swap motion-overlay ;
: head-overlay ( -- )   \ his mouth: 0x1F00 / 0x1F01 / 0x1F02, or (3) changing now and then
    ov-head @ case
        0 of  $1F00 overlay  endof  1 of  $1F01 overlay  endof  2 of  $1F02 overlay  endof
        3 of  -1 ov-head-t +!  ov-head-t @ 0< if
                  15 roll 30 * 30 + ov-head-t !  rnd 0.5e f< if  $1F00  else  $1F01  then  overlay
              then  endof
    endcase ;
: ears-overlay ( -- )   \ his ears: 0x2000 / 0x2001 (hurt 0x2002), or (0) flicking between them
    ov-ears @ case
        0 of  his-cond @ hurt-cond = if  $2002 overlay  else
                  -1 ov-ears-t +!  ov-ears-t @ 0< if
                      ov-ears-alt @ 0= if  $2000 overlay  1  else  $2001 overlay  0  then  ov-ears-alt !
                      2 roll if  900  else  300  then  10 roll 30 * + ov-ears-t !
                  then
              then  endof
        1 of  his-cond @ hurt-cond = if  $2002  else  $2000  then  overlay  endof
        2 of  $2001 overlay  endof
    endcase ;
: tail-play ( g a b c -- )   \ by his pose: c lying, b sitting, else a
    {: g a b c :}
    g dup in-lying-down = swap in-lying = or if  c
    else g dup in-sitting-down = swap in-sitting = or if  b  else  a  then then  overlay ;
create wag-a  $2100 , $2103 , $2106 ,  $2102 , $2105 , $2102 ,
create wag-b  $210D , $210E , $210F ,  $210D , $2110 , $2111 ,
: tail-wag ( g tbl -- )   \ a wag for 10..35 frames now and then, else still 300 frames
    -1 ov-tail-t +!  ov-tail-t @ 0>= if  2drop exit  then
    ov-tail-on @ if  0 ov-tail-on !  300 ov-tail-t !
    else  1 ov-tail-on !  6 roll 5 * 10 + ov-tail-t !  3 cells +  then
    {: tbl :}  tbl @  tbl cell+ @  tbl 2 cells + @  tail-play ;
: tail-overlay ( -- )
    anim-group  ov-tail @ case
        0 of  $2107 $2108 $2109 tail-play  endof
        1 of  $210A $210B $210C tail-play  endof
        2 of  $2100 $2103 $2106 tail-play  endof
        3 of  $210D $210E $210F tail-play  endof
        4 of  wag-a tail-wag  endof
        5 of  $2101 $2104 $2101 tail-play  endof
        6 of  wag-b tail-wag  endof
        7 of  $2102 $2105 $2102 tail-play  endof
        >r drop r>
    endcase ;
: overlay-entry ( -- e | 0 )
    his-danger @ calm = if  overlays-calm  else  overlays-tense  then
    begin  dup @ -1 <> while  dup @ anim@ = if  exit  then  13 cells +  repeat  drop 0 ;
: set-overlays ( -- )
    overlay-entry ?dup 0= if  exit  then  {: e :}
    ov-head @  ov-ears @  ov-tail @  {: was-head was-ears was-tail :}
    his-mood @ 4 u< if   \ (the entry's modes for his mood: head, ears, tail)
        e his-mood @ cells +  dup cell+ @ ov-head !  dup 5 cells + @ ov-ears !  9 cells + @ ov-tail !
    then
    ov-head @ was-head <> ov-head @ 3 = and if  0 ov-head-t !  then
    ov-ears @ was-ears <> ov-ears @ 0= and if  0 ov-ears-t !  then
    ov-tail @ was-tail <> ov-tail @ dup 4 = swap 6 = or and if  0 ov-tail-t !  0 ov-tail-on !  then
    head-overlay  ears-overlay  tail-overlay ;
\ his body as it is posed (DogModel_AdjustBone): his back and shoulders along the floor (bones 0
\ and 16), his neck turned where his head looks (bones 0x1D, 0x1E, 0x1F)
: neck ( -- )
    model dup turns-clear
    his-back-pitch f@ f0= 0= if  his-back-pitch f@ 0e  dup 0 turn+  then
    his-front-pitch f@ his-back-pitch f@ f- fdup f0= if  fdrop  else  0e  dup $10 turn+  then
    his-head-pitch f@ -0.4e f*  his-head-yaw f@ 0.25e f*  dup $1D turn+
    his-head-pitch f@ -0.4e f*  his-head-yaw f@ 0.25e f*  dup $1E turn+
    his-head-pitch f@ -0.2e f*  his-head-yaw f@ 0.25e f*  $1F turn+ ;

\ Hewie_Feet: a step's sound as a foot comes down (the motions' contact tracks), by the floor
\ (triangle flags 0x8000 / 0x10000 / 0x18000 / 0x2000000); four sounds in turn. Pitched 0..2
\ semitones up from 0.28 to 2.4 a frame forward. (Water steps in rooms 7 / 0xD1 / 0x106: with
\ the room effects.)
: step-pitch ( -- n )  stride-len  0.28e f- 2.12e f/  0e fmax 1e fmin  2e f* f>s $7F and ;
create wet-at /vec allot
create foot-bones  $B , $F , $17 , $1C ,   \ (front right, front left, hind right, hind left)
: wet-feet ( new -- )   \ (in the flooded rooms each foot come down ripples the water: deeper running)
    wading? 0= if  drop exit  then  {: new :}
    4 0 do  new 1 i lshift and if
        anim-group in-running =  his-model @ i cells foot-bones + @ bone-pos  wet-at vec!  wet-at water-step
    then  loop ;
: feet ( -- )
    his-away @ his-tri 0< or  world-held state-flag? or if  exit  then
    0 {: new :}
    4 0 do
        model i foot-down?
        dup i cells his-feet-was + @ 0= and if  1 i lshift new or to new  then
        i cells his-feet-was + !
    loop
    new 0= if  exit  then
    new wet-feet
    his-tri nav-flags $2018000 and case
        $8000 of  $14 5  endof  $10000 of  $18 5  endof  $18000 of  $1C 5  endof  $2000000 of  $78 5  endof
        water-floor of  6 sound-loaded? if  $18 6  else  $10 5  then  endof
        >r $10 5 r>
    endcase                                                    ( snd bank )
    swap his-feet-n @ 3 and +  swap  0 step-pitch  his-at vec@ his-sound  1 his-feet-n +! ;

\ Hewie_MoveSubMode: what he is doing, for the story
: move-sub ( -- )
    his-mode @ free-mode <> if  exit  then
    anim-group case
        in-standing of standing-sub endof  in-standing-up of standing-sub endof  in-other of standing-sub endof
        in-sitting of sitting-sub endof  in-sitting-down of sitting-sub endof
        in-lying of lying-sub endof  in-low of lying-sub endof  in-lying-down of lying-sub endof
        in-getting-low of lying-sub endof  in-down of lying-sub endof
        in-running of running-sub endof
        in-trotting of walking-sub endof  in-walking of walking-sub endof  in-on-side of walking-sub endof
        in-rolling-over of walking-sub endof
        >r -1 r>
    endcase  dup 0< if  drop exit  then  his-sub ! ;

\ Hewie_HiddenFrame: each frame out of the room being played - down he stays down; Fiona
\ panicking may bring him (once at its height, once calming down, by his trust); a yelp's time
\ out, he sets off toward her
: hidden-frame ( -- )
    his-cond @ down-cond = if  his-action @ knocked-down-act <> if  knocked-down-act 0 want  then  exit  then
    his-panic-seen @ 2 and 0= if
        false
        his-panic @ calming-down = if  his-panic-seen @ 2 or his-panic-seen !  drop true
        else his-panic-seen @ 1 and 0= his-panic @ panicking = and if  his-panic-seen @ 1 or his-panic-seen !  drop true  then  then
        if  hidden-out-chance by-chance his-mode @ to-her-cry-act <> and if  to-her-cry-act 0 want exit  then  then
    then
    his-panic @ panicking < if  0 his-panic-seen !  then
    his-yelp @ if
        his-yelp @ 0> his-action @ hidden-act <> and if  hidden-act 0 want exit  then
    else  -1 his-yelp !  0 his-hide !  to-her-room-act 0 want  then ;

\ ---- Hewie_Update: a frame of his ----
: show-him ( -- )   \ the model where his body is
    his-at v.x sf@ him-model act.x sf!  his-at v.y sf@ him-model act.y sf!  his-at v.z sf@ him-model act.z sf!
    his-yaw him-model act.yaw sf!
    his-away @ 0= 1 and him-model act.visible l!
    model his-away @ 0= dog-legs ;
: free-for-command? ( -- flag )   \ (Hewie_FreeForCommand: idle, a command queued, nothing linked)
    his-busy @ 0=  his-mode @ free-mode = and  his-cmd @ 0<> and  his-cmd @ nothing-breaks-in and 0= and  his-meet @ 0< and ;
: commandable? ( -- flag )  her-with?  his-mood @ angry-mood <> and  free-for-command? and ;   \ Hewie_FionaCanCommand
: near-command? ( -- flag )   \ Hewie_FionaNearCommand (by the straight distance: his way's length not planned here)
    commandable? if  her-dist 150e f<=  else  false  then ;
: reachable? ( -- flag )   \ Hewie_FionaReachable (only while a script has him: a way to her, planned apart)
    his-busy @ 0=  his-away @ or if  true exit  then
    his-2d @ his-2b @ or if  false exit  then
    her-with? 0= if  true exit  then
    his-probe his-tri his-at her body-tri fiona-at self body-mask path-plan 0> ;
: breakable? ( -- flag )   \ Hewie_MayBreakOff (not `once`): scripted or his command word lets it, not angry
    his-busy @  on-command breaks-in? or  his-mood @ angry-mood <> and ;
: tell-doing ( -- )   \ what he is doing, for Fiona and the story
    self body? 0= if  0 0 0 0 0 0 0  0 0 -1 0 -1 his-trust @
    else  his-away @ if  dog-elsewhere  else  dog-with-her  then  his-action @ his-mode @ his-sub @ his-cond @ his-mood @ anim-group
          commandable? near-command? reachable? breakable? his-way @ his-trust @  then
    broadcast hewie-doing
    his-hp @ broadcast hewie-health ;
: full-stop ( -- )   \ Hewie_FullStop: what a script's letting go clears
    0 his-broke !  0 his-hits !  -1 his-look-char !  0 his-scene-req !  0 his-look-pt? !
    hewie-hidden state-flag-clear ;
: think ( -- )   \ Hewie_Think: a frame of his while a script has him
    his-2b @ if  0  else  hewie-floor  then  body-mask!
    1 his-snd-t +!  his-snd-t @ 3000 > if  3000 his-snd-t !  then
    alert  his-cond @ down-cond <> if  report-near  then
    0 his-no-root !  1 his-root-ok !
    his-act @ ?dup if  execute  then
    turn-by-anim
    his-away @ 0= his-no-root @ 0= and if  root-move  then
    turn-head  neck  anim-sounds  feet
    his-yaw his-yaw-was f!  his-action @ his-last-action !  show-him  tell-doing ;
: frame ( -- )
    his-busy @ if  -1 his-was-busy !  think exit  then
    his-was-busy @ if  0 his-was-busy !  full-stop  to-default  then   \ (the script let him go)
    upkeep  alert  his-cond @ down-cond <> if  report-near  then  obedience
    his-away @ if   \ out of the room being played
        arrive 0= if
            hidden-frame  scuffle
            his-mood-act @ ?dup if  execute  then
            his-act @ ?dup if  execute  then
            show-him  tell-doing exit
        then
    then
    his-2b @ if  through-mask  else  hewie-floor  then  body-mask!
    his-root-ok @ fit-floor
    0 his-no-root !  1 his-root-ok !
    cutscene-active? if  standing-frame  else  own-decisions  then
    his-mood-act @ ?dup if  execute  then
    his-act @ ?dup if  execute  then
    turn-by-anim
    his-no-root @ 0= if  root-move  then
    turn-head  neck  set-overlays  anim-sounds  feet
    his-yaw his-yaw-was f!  his-action @ his-last-action !  his-alert @ his-alert-was !
    0 his-smells !  move-sub  show-him  tell-doing ;

\ Hewie_Activate: his state as he comes into the game (calm, the default action)
: fresh ( -- )
    -1 his-done !  0 his-busy !  -1 his-meet !  -1 his-target !  -1 his-target2 !  -1 his-look-char !  -1 his-door !  -1 his-noise-room !
    2 his-way !   \ (Hewie_Activate: either half)
    no-call his-call !  $FF his-by !  -1 his-yelp !  -1 his-snd-anim !  1e his-floor-k f!
    -1 his-follows !
    normal-mood -1 set-mood  0 add-trust
    obeys  1 his-pending !
    100 hp!  well-cond cond!  300 his-hurt-t !
    ['] st-calm his-mood-act !
    choose-act 0 set-action ;
