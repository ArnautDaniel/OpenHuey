\ hewie/mind.fs - Hewie's mind and his frame (Hewie_Update): his upkeep and obedience, what he
\ notices, his own decisions, his mode's behaviour (calm / wary / tense), what he does next by
\ his trust (the weighted lists), and his body as it is shown (overlays, neck, feet, sounds).
IN: hewie.mind
USING: engine actors common facts paths messages hewie.state hewie.body hewie.tables hewie.model hewie.moving hewie.states hewie.offscreen hewie.actions hewie.commands ;

\ report_fiona_near: how near she is, for the story (1 within 20, 2 within 50, 3 further)
: report-near ( -- )
    her-with? 0= if  exit  then
    her-dist  fdup 20e f<= if  fdrop 1  else  50e f<= if  2  else  3  then  then  his-near ! ;
\ Hewie_Alert: the stalker in his room within 200, ahead of his head (125 degrees): the danger
\ is told (H3: no stalker yet)
: alert ( -- )
    pursuer dup active? 0=  his-away @ or  his-cond @ 2 = or if  drop 0 his-alert !  exit  then
    dup with? if
        dup body-at his-at vec-dist 200e f<= if
            dup body-at heading-to  his-yaw his-head-yaw f@ f+ angle-wrap f- angle-wrap fabs
            125e deg>rad f<= if  his-alert-what !  1 his-alert !  alerted  exit  then
        then
    then
    drop  his-alert @ 2 = if  0 his-alert !  then ;

\ ---- Hewie_Upkeep: his timers, health and mood each frame ----
: upkeep ( -- )
    his-room room-id <> away!
    his-cooldown @ if  -1 his-cooldown +!  then
    his-cond @ 2 <>  his-hp @ 0<> and  his-hp @ 100 < and if
        -1 his-hurt-t +!  his-hurt-t @ 0<= if  300 his-hurt-t !  his-hp @ 1+ 100 min hp!  then
    then
    his-cond @ 2 <> if
        his-hp @ 0= if  his-away @ if  1 hp!  else  2 cond!  0 -1 set-mood  then  then
    else his-hp @ 0= if
        his-away @ if  his-action @ $52 <> if  $52 0 want  then
        else  his-action @ 0= if  $52 0 want  then  then
    else
        1 cond!  obeys
        his-away @ if  to-default  then
    then then
    his-cond @ 2 <> if
        his-cond @ 1 <> if
            his-hp @ 30 < his-mood @ 3 <> and if  1 cond!  calm-down  then
        else
            his-hp @ 30 < 0=  his-mood @ 3 = or if  0 cond!  then
        then
    then
    his-mood @ 3 <> his-hit-t @ 0<> and if
        -1 his-hit-t +!
        his-hit-t @ 0= his-hits @ 0<> and if  -1 his-hits +!  300 his-hit-t !  then
    then
    his-mode @ 8 <> his-action @ $7A <> and if  0 his-ready !  then
    his-yelp @ if  -1 his-yelp +!  then
    1 his-snd-t +!  his-snd-t @ 3000 > if  3000 his-snd-t !  then
    his-mood @ if
        -1 his-mood-time +!  his-mood-time @ 0<= if  calm-down  then
    then
    his-praise-b @ if  -1 his-praise-b +!  his-praise-b @ 0= if  0 his-praise-a !  then  then
    his-pet-time @ if  -1 his-pet-time +!  then
    his-wait @ 0> if  -1 his-wait +!  then
    his-wait @ 0<=  his-cmd @ $80000080 and $80 = and if  1 his-pending !  then
    his-hold-call @ if  -1 his-hold-call +!  then ;

\ ---- Hewie_Obedience: obeying her for a while, then waiting (doing as he likes) for a while ----
: wait-time! ( -- )  hard? if  wait-time-hard  else  wait-time  then  trust-of his-obey ! ;
: obedience ( -- )
    -1 his-obey +!
    his-waiting @ 1 =  her-with? and if
        her-dist 30e f< if
            -2 his-obey +!
            his-action @ dup $7D <> swap $7E <> and if
                1 his-nudge +!  his-nudge @ 91 >= if  0 his-nudge !  $10 react  then
            else  0 his-nudge !  then
        else  0 his-nudge !  then
    then
    his-obey @ 0<= if
        his-obey-marked @ 1 = if
            0 his-obey !
            his-waiting @ 0= if
                his-mood @ 1 <> if
                    1 his-waiting !  wait-time!
                    his-cmd @ $80000002 and 2 = if  1 his-pending !  then
                then
            else
                0 his-waiting !  obey-time!
                his-cmd @ $80000001 and 1 = if  his-away @ if  $34 0 want  else  1 his-pending !  then  then
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
\ 0 while she panics, else 4 + her distance - the stalker's doings with H3)
: situation ( -- n )
    his-danger @ 2 <> if  her-band exit  then
    his-panic @ 4 >= if  0 exit  then
    4 her-band + ;
\ a weighted pick by his trust from a list (count, then an action and 8 weights an entry); an
\ action that `favoured` names weighs 20 more
variable pk-roll  variable pk-sum
: pick-from ( list favoured -- act )
    >r  100 roll pk-roll !  0 pk-sum !
    dup @ 0 ?do
        cell+  dup his-trust @ 1+ cells + @ pk-sum +!
        dup @ r@ = if  20 pk-sum +!  then
        pk-roll @ pk-sum @ < if  @ r> drop unloop exit  then
        8 cells +
    loop  drop r> drop 0 ;
: list-of ( table i -- list )  1+ cells + @ ;
\ Hewie_WhatNext: by the danger, obeying or waiting (H3: growling at the stalker, the creatures)
: what-next ( -- )
    his-waiting @ 0= if
        his-danger @ case  0 of  calm-lists  endof  1 of  followed-lists  endof  >r tense-lists r>  endcase
    else
        his-danger @ case  0 of  calm-wait-lists  endof  1 of  followed-wait-lists  endof
            >r hard? if  tense-wait-hard-lists  else  tense-wait-lists  then  r>  endcase
    then
    situation list-of -1 pick-from 0 want ;
\ Hewie_WhenIdle (out of the room being played): waiting a while hurt; else from the idle list,
\ the one the danger favours (waiting) or going to her (0x2C) weighted more
: idle ( -- )
    his-cond @ 1 = rnd 0.25e f< and if  idle-wait trust-of his-wanted !  $2F 0 want exit  then
    his-waiting @ if  his-danger @ case  0 of  $32  endof  1 of  $2E  endof  2 of  $2C  endof  >r -1 r>  endcase
    else  $2C  then
    idle-list swap pick-from ?dup if  0 want  then ;
' idle is when-idle

\ ---- Hewie_OwnDecisions: what he decides on his own when nothing else drives him ----
: to-fiona ( -- )   \ to her (0x62) if he can get to her, else he stops sulking
    her body-tri fiona-at plan-to if  $62 0 want  else  0 -1 set-mood  then ;
: staying ( -- done? )   \ angry (mood 3): a call, her having hit him, or whoever is here
    0 his-hold-call !
    his-call @ $FF <> if
        his-call @ 0= her-with? and if  to-fiona  else  $4F 0 want  then  true exit
    then
    his-hits @ 0> her-with? and if  to-fiona true exit  then
    her-with? if  to-fiona true exit  then
    false ;
fvariable sl-x  fvariable sl-z
: facing-down-slope? ( -- flag )   \ on sloped ground (flag 1), facing down it
    his-tri 0< if  false exit  then
    his-tri nav-flags 1 and 0= if  false exit  then
    his-tri tri-normal  sl-z f!  1e f= if  fdrop false exit  then  sl-x f!
    sl-x f@ fsq sl-z f@ fsq f+ fsqrt  fdup f0= if  fdrop false exit  then
    fdup sl-x f@ fswap f/ his-yaw fsin f*  fswap sl-z f@ fswap f/ his-yaw fcos f* f+  0.5e f> ;
: panic-brings? ( -- flag )   \ her panic newly at 4 or 5: by chance (not angry) he comes
    false
    his-panic @ 5 = if  his-panic-seen @ 2 or his-panic-seen !  his-mood @ 3 <> if  drop panic5-chance by-chance  then
    else his-panic-seen @ 1 and 0= his-panic @ 4 = and if
        his-panic-seen @ 1 or his-panic-seen !  his-mood @ 3 <> if  drop panic4-chance by-chance  then
    then then ;
: own-decisions ( -- )
    his-mode @ if  0 his-scene-req !  exit  then
    his-cond @ 2 = if  his-action @ dup $52 <> swap $74 <> and if  $52 0 want  then  exit  then
    his-yelp @ 0> his-action @ $76 <> and if  $76 0 want  exit  then
    his-cmd @ 0= his-cmd @ $80000000 and or if  exit  then
    her-with? if
        his-panic-seen @ 2 and 0= if  panic-brings? if  $4F 0 want  exit  then  then
        his-panic @ 4 < if  0 his-panic-seen !  then
    then
    his-mood @ 3 = if  staying if  exit  then  then
    \ Fiona held: newly so, by chance he goes for whoever holds her (H3: the holder)
    her-with? fiona-mode @ 4 = and fiona-sub @ dup 9 = swap $12 = or and  his-held !
    his-danger @ 2 <> his-alert-was @ 0= and his-alert @ 0<> and his-action @ $76 <> and if  $14 0 want  exit  then
    his-scene-req @ 1 = if  0 his-scene-req !  his-action @ $79 <> if  $79 0 want  exit  then  then
    anim-group 4 < his-action @ $6E <> and facing-down-slope? and if  $6E 0 want  exit  then
    his-cmd @ $80000020 and $20 =  anim-group dup 10 = swap 4 < or and if
        his-action @ $6A <> if
            cp-try head-at  cp-try -1 body-tri-to 0< if  $6A 0 want  exit  then
        then
        her-with? his-action @ $67 <> and  her-dist 5e f< and if  $67 0 want  exit  then
    then
    his-praise-due @ 1 = if
        0 his-praise-due !
        her-with? his-waiting @ 1 = and his-cmd @ $80000400 and $400 = and if
            his-action @ dup $7D <> swap $7E <> and if  $7E 0 want  then
        then
    then ;
\ Hewie_StandingFrame (while a cutscene plays): down stays down; a slope; Fiona right by him
: standing-frame ( -- )
    his-away @ his-mode @ or if  exit  then
    his-cond @ 2 = if  his-action @ dup $52 <> swap $74 <> and if  $52 0 want  then  exit  then
    anim-group 4 < his-action @ $6E <> and facing-down-slope? and if  $6E 0 want  exit  then
    his-action @ $82 = if  exit  then
    her-with? if  his-at fiona-at vec-dist-xz 5e f< if  her his-target !  $82 0 want  then  then ;

\ ---- his mode's behaviour (Hewie_StateCalm / Wary / Tense): as the danger changes ----
create mood-acts  3 cells allot
: mode-behaviour ( own -- )
    his-cond @ 2 = if  drop exit  then
    his-danger @ over = if
        drop  his-pending @ if
            his-away @ if  0 his-pending !  when-idle  else  what-next  0 his-pending !  then
        then  exit
    then
    his-danger @ case
        0 of  drop  0 his-hits !  0 his-did-2 !  0 his-did !  0 his-alert !  endof
        1 of  0 his-hits !  0 his-did-2 !  0 his-did !
              0= his-away @ 0= and if
                  his-cmd @ $80000200 and $200 = if  $14 0 want  then
                  1 cells mood-acts + @ his-mood-act !  exit
              then  endof
        2 of  drop  0 his-hits !  0 his-did-2 !  0 his-did !
              his-away @ 0= if  pursuer dup with? if  his-alert-what !  3 his-alert !  else  drop  then  then  endof
        >r 2drop r> drop exit
    endcase
    his-danger @ 0 max 2 min cells mood-acts + @ his-mood-act !
    his-cmd @ $80000004 and 4 = if  1 his-pending !  then ;
: st-calm ( -- )  0 mode-behaviour ;
: st-wary ( -- )  1 mode-behaviour ;
: st-tense ( -- )  2 mode-behaviour ;
' st-calm mood-acts !  ' st-wary mood-acts cell+ !  ' st-tense mood-acts 2 cells + !

\ ---- his sounds (Hewie_AnimSounds): the whimpers while their animation plays; a bark's or
\ growl's sound as it starts, heard by the others ----
: bark-sound ( anim -- snd | 0 )
    case
        $1C01 of  $60  endof
        $1B00 of  $5A  endof  $1B01 of  $5A  endof  $1B02 of  $5A  endof
        $1B03 of  his-action @ $1D = if  $5A  else  $5D  then  endof
        $1B04 of  his-action @ $1D = if  $5A  else  $5E  then  endof
        $1B05 of  $5F  endof  $1C04 of  $5C  endof
        $1001 of  $66  endof  $220C of  $66  endof  $2203 of  $66  endof
        >r 0 r>
    endcase ;
: anim-sounds ( -- )
    him-model act.frame sf@                                          ( F: frame )
    anim@ case
        4 of  $6F make-sound  endof
        9 of  his-ready @ 1 = if  $70 make-sound  then  endof
        5 of  $58 make-sound  endof
        6 of  $59 make-sound  endof  7 of  $59 make-sound  endof  $206 of  $59 make-sound  endof
    endcase
    anim@ his-snd-anim @ <>  fdup his-snd-frame f@ f< or if
        anim@ bark-sound ?dup if  make-sound  then
        anim@ $1B00 $1B06 within if
            his-action @ dup $7B = over $B = or swap $A = or if  $1B  else  0  then  his-tri noise-make
        then
        anim@ $1000 = if  $80 his-tri noise-make  then
    then
    his-snd-frame f!  anim@ his-snd-anim ! ;

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
        0 of  his-cond @ 1 = if  $2002 overlay  else
                  -1 ov-ears-t +!  ov-ears-t @ 0< if
                      ov-ears-alt @ 0= if  $2000 overlay  1  else  $2001 overlay  0  then  ov-ears-alt !
                      2 roll if  900  else  300  then  10 roll 30 * + ov-ears-t !
                  then
              then  endof
        1 of  his-cond @ 1 = if  $2002  else  $2000  then  overlay  endof
        2 of  $2001 overlay  endof
    endcase ;
: tail-play ( g a b c -- )   \ by his pose: c lying (groups 2 / 6), b sitting (1 / 5), else a
    3 pick dup 6 = swap 2 = or if  >r 2drop drop r> overlay exit  then
    3 pick dup 5 = swap 1 = or if  drop nip nip overlay exit  then
    2drop nip overlay ;
create wag-a  $2100 , $2103 , $2106 ,  $2102 , $2105 , $2102 ,
create wag-b  $210D , $210E , $210F ,  $210D , $2110 , $2111 ,
: tail-wag ( g tbl -- )   \ a wag for 10..35 frames now and then, else still 300 frames
    -1 ov-tail-t +!  ov-tail-t @ 0>= if  2drop exit  then
    ov-tail-on @ if  0 ov-tail-on !  300 ov-tail-t !
    else  1 ov-tail-on !  6 roll 5 * 10 + ov-tail-t !  3 cells +  then
    >r  r@ @  r@ cell+ @  r> 2 cells + @  tail-play ;
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
    his-danger @ 0= if  overlays-calm  else  overlays-tense  then
    begin  dup @ -1 <> while  dup @ anim@ = if  exit  then  13 cells +  repeat  drop 0 ;
variable oh  variable oe  variable ot
: set-overlays ( -- )
    overlay-entry ?dup 0= if  exit  then
    ov-head @ oh !  ov-ears @ oe !  ov-tail @ ot !
    his-mood @ dup 4 u< if
        cells over +  dup cell+ @ ov-head !  dup 5 cells + @ ov-ears !  9 cells + @ ov-tail !  drop
    else  2drop  then
    ov-head @ oh @ <> ov-head @ 3 = and if  0 ov-head-t !  then
    ov-ears @ oe @ <> ov-ears @ 0= and if  0 ov-ears-t !  then
    ov-tail @ ot @ <> ov-tail @ dup 4 = swap 6 = or and if  0 ov-tail-t !  0 ov-tail-on !  then
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
variable feet-new
: step-pitch ( -- n )
    model root-delta  fswap fdrop fswap fdrop fswap fdrop  his-floor-k f@ f*
    0.28e f- 2.12e f/  0e fmax 1e fmin  2e f* f>s $7F and ;
: feet ( -- )
    his-away @ his-tri 0< or if  exit  then
    0 feet-new !
    4 0 do
        model i foot-down?
        dup i cells his-feet-was + @ 0= and if  1 i lshift feet-new @ or feet-new !  then
        i cells his-feet-was + !
    loop
    feet-new @ 0= if  exit  then
    his-tri nav-flags $2018000 and case
        $8000 of  $14 5  endof  $10000 of  $18 5  endof  $18000 of  $1C 5  endof  $2000000 of  $78 5  endof
        $2008000 of  6 sound-loaded? if  $18 6  else  $10 5  then  endof
        >r $10 5 r>
    endcase                                                    ( snd bank )
    swap his-feet-n @ 3 and +  swap  0 step-pitch  his-at vec@ sound-at  1 his-feet-n +! ;

\ Hewie_MoveSubMode: what he is doing, for the story
: move-sub ( -- )
    his-mode @ if  exit  then
    anim-group case
        0 of 0 endof  4 of 0 endof  $F of 0 endof
        1 of 3 endof  5 of 3 endof
        2 of 4 endof  3 of 4 endof  6 of 4 endof  7 of 4 endof  $D of 4 endof
        8 of 2 endof
        9 of 1 endof  $A of 1 endof  $B of 1 endof  $C of 1 endof
        >r -1 r>
    endcase  dup 0< if  drop exit  then  his-sub ! ;

\ Hewie_HiddenFrame: each frame out of the room being played - down he stays down; Fiona
\ panicking may bring him (0x39, once at 4 and once at 5, by his trust); a yelp's time out, he
\ sets off toward her (0x2C). (Noises he hears: with the stalkers, H3.)
: hidden-frame ( -- )
    his-cond @ 2 = if  his-action @ $52 <> if  $52 0 want  then  exit  then
    his-panic-seen @ 2 and 0= if
        false
        his-panic @ 5 = if  his-panic-seen @ 2 or his-panic-seen !  drop true
        else his-panic-seen @ 1 and 0= his-panic @ 4 = and if  his-panic-seen @ 1 or his-panic-seen !  drop true  then  then
        if  hidden-out-chance by-chance his-mode @ $39 <> and if  $39 0 want exit  then  then
    then
    his-panic @ 4 < if  0 his-panic-seen !  then
    his-yelp @ if
        his-yelp @ 0> his-action @ $77 <> and if  $77 0 want exit  then
    else  -1 his-yelp !  0 his-hide !  $2C 0 want  then ;

\ ---- Hewie_Update: a frame of his ----
: show-him ( -- )   \ the model where his body is
    his-at sf@ him-model act.x sf!  his-at 4 + sf@ him-model act.y sf!  his-at 8 + sf@ him-model act.z sf!
    his-yaw him-model act.yaw sf!
    his-away @ 0= 1 and him-model act.visible l!
    model his-away @ 0= dog-legs ;
: tell-doing ( -- )   \ what he is doing, for Fiona
    self body? 0= if  0 0 0 0 0 0 0
    else  his-away @ if  1  else  2  then  his-action @ his-mode @ his-sub @ his-cond @ his-mood @ anim-group  then
    broadcast hewie-doing ;
: frame ( -- )
    upkeep  alert  his-cond @ 2 <> if  report-near  then  obedience
    his-away @ if   \ out of the room being played
        arrive 0= if
            hidden-frame
            his-mood-act @ ?dup if  execute  then
            his-act @ ?dup if  execute  then
            show-him  tell-doing exit
        then
    then
    his-2b @ if  8  else  his-floor  then  body-mask!
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
    -1 his-meet !  -1 his-target !  -1 his-target2 !  -1 his-look-char !  -1 his-door !  -1 his-noise-room !
    $FF his-call !  $FF his-by !  -1 his-yelp !  -1 his-snd-anim !  1e his-floor-k f!
    16 his-skill !  16 his-skill cell+ !  16 his-skill 2 cells + !
    0 -1 set-mood  0 add-trust
    obeys  1 his-pending !
    100 hp!  0 cond!  300 his-hurt-t !
    ['] st-calm his-mood-act !
    0 0 set-action ;
