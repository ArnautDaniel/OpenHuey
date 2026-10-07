\ partner/brain.fs - Hewie's mind (src/game/hewie.c): his upkeep and obedience each frame,
\ Fiona's commands and calls (his state block), his own decisions, what he does next by his
\ trust (the weighted lists), and the frame (Hewie_Update).
IN: partner.brain
USING: engine game-state events.core events.words chars partner.core partner.tables partner.moves partner.route partner.states partner.offscreen partner.actions ;

: req@ ( -- n )  him character char.req sl@ ;
: req-arg@ ( -- n )  him character char.req-arg sl@ ;
: req! ( n -- )  him character char.req l! ;
: hp! ( n -- )  him character char.hp l! ;
: cond! ( n -- )  him character char.cond l! ;
: by-chance ( table -- flag )  trust-of 0 100 clamp  100 roll > ;   \ by_chance
defer fiona-react ( n -- )   ' drop is fiona-react   \ Fiona_HewieReact (her side: phase 2)

\ report_fiona_near: how near she is, for the scripts (+0x7B9: 1 within 20, 2 within 50, 3 further)
: report-fiona-near ( -- )
    her with? 0= if  exit  then
    h-pos her c-pos vec-dist  fdup 20e f<= if  fdrop 1  else  50e f<= if  2  else  3  then  then
    fiona-near ! ;

\ Hewie_Alert: the stalker in his room within 200, ahead of his head (125 degrees). (Next door
\ and heard come with the stalkers.)
: alert ( -- )
    pursuer @ dup c-active? 0=  h-disabled? or  h-cond 2 = or if  drop 0 h-alert !  exit  then
    dup with? if
        h-pos over c-pos vec-dist 200e f<= if
            him over c-pos c-heading-to  h-yaw h-head-yaw f@ f+ angle-wrap f- angle-wrap fabs
            125e deg>rad f<= if  h-alert-what !  1 h-alert !  4 cond-bit!  exit  then
        then
    then
    drop  h-alert @ 2 = if  0 h-alert !  then ;

\ ---- Hewie_Upkeep: his timers, health and mood each frame ----
: upkeep ( -- )
    h-room played-room <> disabled!
    req@ 5 = if  exit  then
    h-cooldown @ if  -1 h-cooldown +!  then
    h-cond 2 <>  h-hp 0<> and  h-hp 100 < and if
        -1 h-hurt-t +!  h-hurt-t @ 0<= if  300 h-hurt-t !  h-hp 1+ 100 min hp!  then
    then
    h-cond 2 <> if
        h-hp 0= if  h-disabled? if  1 hp!  else  2 cond!  0 -1 set-mode  then  then
    else h-hp 0= if
        h-disabled? if  h-action @ $52 <> if  $52 0 want  then
        else  h-action @ 0= if  $52 0 want  then  then
    else
        1 cond!  0 h-waiting !  obey-time!  0 h-praise-due !
        h-disabled? if  0 0 want  then
    then then
    h-cond 2 <> if
        h-cond 1 <> if
            h-hp 30 < h-mood @ 3 <> and if  1 cond!  0 -1 set-mode  0 h-hits !  0 h-hit-t !  then
        else
            h-hp 30 < 0=  h-mood @ 3 = or if  0 cond!  then
        then
    then
    h-mood @ 3 <> h-hit-t @ 0<> and if
        -1 h-hit-t +!
        h-hit-t @ 0= h-hits @ 0<> and if  -1 h-hits +!  300 h-hit-t !  then
    then
    h-mode 8 <> h-action @ $7A <> and if  0 h-ready !  then
    h-yelp @ if  -1 h-yelp +!  then
    1 h-snd-t +!  h-snd-t @ 3000 > if  3000 h-snd-t !  then
    h-mood @ if
        -1 h-mood-time +!  h-mood-time @ 0<= if  0 -1 set-mode  0 h-hits !  0 h-hit-t !  then
    then
    h-praise-b @ if  -1 h-praise-b +!  h-praise-b @ 0= if  0 h-praise-a !  then  then
    h-pet-time @ if  -1 h-pet-time +!  then
    h-wait @ 0> if  -1 h-wait +!  then
    h-wait @ 0<=  h-cmd @ $80000080 and $80 = and if  1 h-pending !  then
    h-hold-call @ if  -1 h-hold-call +!  then ;

\ ---- Hewie_Obedience: obeying her for a while, then waiting (doing as he likes) for a while ----
: wait-time! ( -- )  hard? if  wait-time-hard  else  wait-time  then  trust-of h-obey ! ;
: obedience ( -- )
    -1 h-obey +!
    h-waiting @ 1 =  her with? and if
        h-pos her c-pos vec-dist 30e f< if
            -2 h-obey +!
            h-action @ dup $7D <> swap $7E <> and if
                1 h-nudge +!  h-nudge @ 91 >= if  0 h-nudge !  $10 fiona-react  then
            else  0 h-nudge !  then
        else  0 h-nudge !  then
    then
    h-obey @ 0<= if
        h-obey-marked @ 1 = if
            0 h-obey !
            h-waiting @ 0= if
                h-mood @ 1 <> if
                    1 h-waiting !  wait-time!
                    h-cmd @ $80000002 and 2 = if  1 h-pending !  then
                then
            else
                0 h-waiting !  obey-time!
                h-cmd @ $80000001 and 1 = if  h-disabled? if  $34 0 want  else  1 h-pending !  then  then
                0 h-praise-due !
            then
        else  1 h-obey-marked !  then
    then
    h-waiting @ 0= if  0 h-praise-due !  then ;

\ ---- what he does next ----
\ player_band: how far she is (0 within 10 .. 6 beyond 100, or not here)
: player-band ( -- n )
    her with? 0= if  6 exit  then
    h-pos her c-pos vec-dist
    fdup 10e f< if  fdrop 0 exit  then  fdup 20e f< if  fdrop 1 exit  then
    fdup 40e f< if  fdrop 2 exit  then  fdup 60e f< if  fdrop 3 exit  then
    fdup 80e f< if  fdrop 4 exit  then  100e f< if  5  else  6  then ;
\ Hewie_SituationList: the list of a table that fits him (calm: her distance; the chase: the
\ stalker's doings - with the stalkers - else 4 + her distance)
: situation ( -- n )
    game-mode @ 2 <> if  player-band exit  then
    panic @ 4 >= if  0 exit  then
    4 player-band + ;
\ a weighted pick by his trust from a list (count, then action and 8 weights an entry)
variable pk-roll  variable pk-sum  variable pk-favoured
: pick-from ( list -- act )
    100 roll pk-roll !  0 pk-sum !
    dup @ 0 ?do
        cell+  dup h-trust @ 1+ cells + @ pk-sum +!
        pk-roll @ pk-sum @ < if  @ unloop exit  then
        8 cells +
    loop  drop 0 ;
: list-of ( table i -- list )  1+ cells + @ ;
\ Hewie_WhatNext. (Growling at the stalker and the hostile creatures' lists: with the stalkers.)
: what-next ( -- )
    h-waiting @ 0= if
        game-mode @ case  0 of  calm-lists  endof  1 of  followed-lists  endof  >r tense-lists r>  endcase
    else
        game-mode @ case  0 of  calm-wait-lists  endof  1 of  followed-wait-lists  endof
            >r hard? if  tense-wait-hard-lists  else  tense-wait-lists  then  r>  endcase
    then
    situation list-of pick-from 0 want ;
\ Hewie_WhenIdle (out of the room being played)
: when-idle ( -- )
    h-cond 1 = rnd01 0.25e f< and if  idle-wait trust-of h-wanted !  $2F 0 want exit  then
    h-waiting @ if  game-mode @ case  0 of  $32  endof  1 of  $2E  endof  2 of  $2C  endof  >r -1 r>  endcase
    else  $2C  then  pk-favoured !
    100 roll pk-roll !  0 pk-sum !
    idle-list dup @ 0 ?do
        cell+  dup h-trust @ 1+ cells + @ pk-sum +!
        dup @ pk-favoured @ = if  20 pk-sum +!  then
        pk-roll @ pk-sum @ < if  @ 0 want unloop exit  then
        8 cells +
    loop  drop ;

\ ---- Fiona's commands ----
\ Hewie_CommandAction: what her command makes him do (-1 nothing; -2 he won't; -4 / -5 his
\ mood set first)
: command-action ( cmd -- act )
    h-busy? 0= if
        h-cond 2 = if  $2A = if  $2A  else  -1  then  exit  then
        h-cmd @ $80000008 and 8 <>  h-cooldown @ 0<> or  h-mode 4 = or if  drop -1 exit  then
    else  h-cond 2 = if  drop -1 exit  then  then
    h-disabled? if  exit  then
    dup $28 = over $2B = or over $30 = or if  exit  then
    dup $2F = if  drop
        h-cond 1 =  h-waiting @ 0<> or  h-mood @ 2 > or if  -1 exit  then
        h-pet-time @ 0<> rnd01 0.25e f< or if  -5  else  -1  then  exit
    then
    dup $29 = if  drop
        h-cond 1 =  h-waiting @ 0<> or  h-mood @ 1 > or  h-hp $50 < or if  -1 exit  then
        h-pet-time @ 0<> rnd01 0.25e f< or if  -4  else  -1  then  exit
    then
    h-broke @ 1 <>  h-busy? h-cmd @ $80000008 and 8 = or and
    h-waiting @ 0= h-action @ $7D = or and  h-mood @ 3 <> and
    0 h-broke !
    if  exit  then
    drop  game-mode @ if  -1 exit  then
    anim-group dup 5 = swap 1 = or if  -2  else  -1  then ;

\ Hewie_ActOnCommand: true if he took it up
: act-on-command ( cmd -- flag )
    h-disabled? if  drop false exit  then   \ (answering from another room: the rooms' phase)
    case
        $25 of  $1D $29 want  obeys  true  endof
        $26 of  game-mode @ 0= dup if  $1D $2B want  then  obeys  endof
        $27 of  game-mode @ 0= if
                    h-action @ dup 3 = over 2 = or over 1 = or over 5 = or swap 4 = or if  $29  else  $27  then
                    $1D swap want
                else  game-mode @ 2 = if  $1D $7A want  else  $1D 7 want  then  then
                obeys  true  endof
        $2C of  $1D game-mode @ 0= if  $D  else  $E  then  want  obeys  true  endof
        $2D of  $4E 0 want  obeys  true  endof
        $2A of  10 hp!  false  endof
        $23 of  h-cmd-tri @ h-to-tri !  h-cmd-yaw f@ h-to-yaw f!  h-to h-cmd-pos vec-copy
                game-mode @ 0= if  $1D $63 want  else  $63 0 want  then  obeys  true  endof
        $2E of  obeys  false  endof
        $30 of  panic @ 5 = if  panic5-chance
                else panic @ 4 = if  panic4-chance
                else her c-mode 4 = her c-sub dup 9 = swap $12 = or and if  held-answer-chance
                else  0  then then then
                dup if  by-chance dup if  $4F 0 want  then  then  endof
        >r false r>
    endcase ;

\ Hewie_FionaCommand: her command (the request's argument; the spot she shows in h-cmd-*):
\ true if he acts on it
: fiona-command ( -- flag )
    req-arg@ h-cmd-was !
    h-busy? 0= h-broke @ 1 = and if  false exit  then
    h-action @ $7D = h-cmd-was @ $30 <> and if  obeys  then
    h-cmd-was @ dup $2B <> swap $2F <> and if  0 h-praise-b !  0 h-praise-a !  then
    h-cmd-was @ $29 = if
        h-action @ h-did-was !
        1 3 praise-scold if  60 h-cooldown !  $1D 0 want  true exit  then
    then
    h-cmd-was @ $2F = if
        h-action @ h-did-was !
        0 3 praise-scold if  60 h-cooldown !  $71 0 want  true exit  then
    then
    h-cmd-was @ command-action
    dup -1 = if  drop false exit  then
    60 h-cooldown !
    case
        -2 of  $1E 0 want  true  endof
        -3 of  $6F h-action @ want  true  endof
        -4 of  1 -1 set-mode  $1D 0 want  true  endof
        -5 of  0 -1 set-mode  0 h-hits !  0 h-hit-t !  $71 0 want  true  endof
        dup h-cmd-act !  dup act-on-command swap
    endcase ;

\ ---- Hewie_StateBlock: requests from outside (her calls 12 / commands 13, the scripts' 8 / 11) ----
: sniff ( anim -- )  4 look!  play  ['] st-anim-over behave ;
: joint-turn ( -- )
    h-action @ $7D = if  obeys  180 h-obey +!  then  $48 0 want ;
: her-call ( type -- )   \ 12: she meets him (0 / 2 / 4 turn to her, 1 praise, 3 pet, 5 a pat)
    case
        0 of  h-action @ h-did-was !  joint-turn  endof
        2 of  h-action @ h-did-was !  joint-turn  endof
        4 of  joint-turn  endof
        1 of  -7  0 3 praise-scold 0=  h-cond 1 <> and  h-waiting @ 0= and  h-mood @ 3 u< and
              h-pet-time @ 0<> rnd01 0.5e f< or and if  drop -6  then
              h-t1 !  $49 0 want  0 h-broke !  endof
        3 of  -9  1 3 praise-scold 0=  h-cond 1 <> and  h-waiting @ 0= and  h-mood @ 2 u< and
              h-hp 80 >= and  h-pet-time @ 0<> rnd01 0.5e f< or and if  drop -8  then
              h-t1 !  $4A 0 want  0 h-broke !  endof
        5 of  -1
              h-broke @ 1 <>  h-busy? h-cmd @ $80000008 and 8 = or and if
                  h-waiting @ 0= h-action @ $7D = or  h-mood @ 3 <> and if  drop 0  then
                  1 h-broke !
              then
              0= if  $4B 0 want
              else  4 roll case
                      0 of  $1C06 sniff  endof  1 of  $1C01 sniff  endof
                      2 of  $1C00 sniff  endof  3 of  $55 0 want  endof
                  endcase
              then  0 h-broke !  endof
    endcase ;
: state-block ( -- )
    -1 h-cmd-was !
    h-room played-room <> if  0 req!  exit  then
    req@ 7 = if  exit  then
    req@ 5 = if  path-end  0 0 want  0 req!  exit  then
    req@ 4 = if  0 req!  then   \ (a blow: with the stalkers)
    req@ 12 = req-arg@ 6 u< and if  req-arg@ her-call  0 req!  then
    req@ 11 = if
        req-arg@ case  0 of  $79 0 want  endof  1 of  $7A 0 want  endof  2 of  $84 0 want  endof  endcase
        0 req!
    then
    req@ 8 = if  $4E 0 want  0 req!  then
    req@ 13 = if  fiona-command drop  0 h-broke !  0 req!  then
    0 req! ;

\ ---- Hewie_OwnDecisions: what he decides on his own when nothing else drives him ----
: to-fiona ( -- )   \ to her (0x62) if he can get to her, else he stops staying
    her c-tri her c-pos plan-to if  $62 0 want  else  0 -1 set-mode  then ;
: to-pursuer ( -- )
    pursuer @ dup c-tri swap c-pos plan-to if  $4F 0 want  else  0 h-call !  then ;
: staying ( -- done? )   \ angry (mood 3): a call, her having hit him, or whoever is here
    0 h-hold-call !
    h-call @ $FF <> if
        h-call @ 0= her with? and if  to-fiona  else  $4F 0 want  then  true exit
    then
    h-hits @ 0> her with? and if  to-fiona true exit  then
    her with? pursuer @ with? and if
        h-pos her c-pos vec-dist  h-pos pursuer @ c-pos vec-dist f< if  to-fiona  else  to-pursuer  then
        true exit
    then
    her with? if  to-fiona true exit  then
    pursuer @ with? if  to-pursuer true exit  then
    false ;
fvariable sl-x  fvariable sl-z
: facing-down-slope? ( -- flag )   \ on sloped ground (flag 1), facing down it
    h-tri nav-flags 1 and 0= if  false exit  then
    h-tri tri-normal  sl-z f!  1e f= if  fdrop false exit  then  sl-x f!
    sl-x f@ fsq sl-z f@ fsq f+ fsqrt  fdup f0= if  fdrop false exit  then
    fdup sl-x f@ fswap f/ h-yaw fsin f*  fswap sl-z f@ fswap f/ h-yaw fcos f* f+  0.5e f> ;
: own-decisions ( -- )
    h-mode if  0 h-scene-req !  exit  then
    h-cond 2 = if  h-action @ dup $52 <> swap $74 <> and if  $52 0 want  then  exit  then
    h-yelp @ 0> h-action @ $76 <> and if  $76 0 want  exit  then
    \ (dragged through doors - Hewie_FirstDoor - with the rooms)
    h-cmd @ 0= h-cmd @ $80000000 and or if  exit  then
    \ Fiona panicking: once at 4, once at 5, by chance he comes
    her with? if
        h-mode 0= h-panic-seen @ 2 and 0= and if
            false
            panic @ 5 = if  h-panic-seen @ 2 or h-panic-seen !  h-mood @ 3 <> if  drop panic5-chance by-chance  then
            else h-panic-seen @ 1 and 0= panic @ 4 = and if
                h-panic-seen @ 1 or h-panic-seen !  h-mood @ 3 <> if  drop panic4-chance by-chance  then
            then then
            if  $4F 0 want  exit  then
        then
        panic @ 4 < if  0 h-panic-seen !  then
    then
    h-mood @ 3 = if  staying if  exit  then  then
    \ Fiona held: newly so, by chance he goes for whoever holds her
    her with? her c-mode 4 = and her c-sub dup 9 = swap $12 = or and                ( held )
    dup  h-held @ 0= and  h-mood @ 3 <> and if
        go-for-holder-chance by-chance if
            h-held !  pick-target h-target !  $6D 0 want  exit
        then
    then  h-held !
    game-mode @ 2 <> h-alert-was @ 0= and h-alert @ 0<> and h-action @ $76 <> and if  $14 0 want  exit  then
    h-scene-req @ 1 = if  0 h-scene-req !  h-action @ $79 <> if  $79 0 want  exit  then  then
    \ (following a creature: with the creatures)
    anim-group 4 < h-action @ $6E <> and facing-down-slope? and if  $6E 0 want  exit  then
    \ (biting the stalker from behind: with the stalkers)
    h-cmd @ $80000020 and $20 =  anim-group dup 10 = swap 4 < or and if
        h-action @ $6A <> if
            cp-try head-at  him cp-try -1 c-tri-to 0< if  $6A 0 want  exit  then
        then
        her with? h-action @ $67 <> and  h-pos her c-pos vec-dist 5e f< and if  $67 0 want  exit  then
    then
    h-praise-due @ 1 = if
        0 h-praise-due !
        her with? h-waiting @ 1 = and h-cmd @ $80000400 and $400 = and if
            h-action @ dup $7D <> swap $7E <> and if  $7E 0 want  then
        then
    then ;
\ Hewie_StandingFrame (while a cutscene plays): down stays down; a slope; someone right by him
: standing-frame ( -- )
    h-disabled? h-mode or if  exit  then
    h-cond 2 = if  h-action @ dup $52 <> swap $74 <> and if  $52 0 want  then  exit  then
    anim-group 4 < h-action @ $6E <> and facing-down-slope? and if  $6E 0 want  exit  then
    h-action @ $82 = if  exit  then
    her with? if  h-pos her c-pos vec-dist-xz 5e f< if  her h-target !  $82 0 want  then  then ;

\ ---- the mood's behaviour (Hewie_StateCalm / Wary / Tense: mode_behaviour) ----
create mood-states  3 cells allot
: mode-behaviour ( own -- )
    h-cond 2 = if  drop exit  then
    game-mode @ over = if
        drop  h-pending @ if
            h-disabled? if  0 h-pending !  when-idle  else  what-next  0 h-pending !  then
        then  exit
    then
    game-mode @ case
        0 of  drop  0 h-hits !  0 h-did-2 !  0 h-did !  0 h-alert !  endof
        1 of  0 h-hits !  0 h-did-2 !  0 h-did !
              0= h-disabled? 0= and if
                  h-cmd @ $80000200 and $200 = if  $14 0 want  then
                  1 cells mood-states + @ h-mood-state !  exit
              then  endof
        2 of  drop  0 h-hits !  0 h-did-2 !  0 h-did !
              h-disabled? 0= if  pursuer @ dup with? if  h-alert-what !  3 h-alert !  else  drop  then  then  endof
        >r 2drop r> drop exit
    endcase
    game-mode @ 0 max 2 min cells mood-states + @ h-mood-state !
    h-cmd @ $80000004 and 4 = if  1 h-pending !  then ;
: st-calm ( -- )  0 mode-behaviour ;
: st-wary ( -- )  1 mode-behaviour ;
: st-tense ( -- )  2 mode-behaviour ;
' st-calm mood-states !  ' st-wary mood-states cell+ !  ' st-tense mood-states 2 cells + !

\ ---- his sounds (Hewie_AnimSounds): the whimpers while their animation plays; a bark's or
\ growl's sound as it starts (the original's is on the animation's sound event) ----
variable snd-anim  -1 snd-anim !  fvariable snd-frame
: bark-sound ( anim -- snd | 0 )
    case
        $1C01 of  $60  endof
        $1B00 of  $5A  endof  $1B01 of  $5A  endof  $1B02 of  $5A  endof
        $1B03 of  h-action @ $1D = if  $5A  else  $5D  then  endof
        $1B04 of  h-action @ $1D = if  $5A  else  $5E  then  endof
        $1B05 of  $5F  endof  $1C04 of  $5C  endof
        $1001 of  $66  endof  $220C of  $66  endof  $2203 of  $66  endof
        >r 0 r>
    endcase ;
: anim-sounds ( -- )
    h-actor dup 0< if  drop exit  then  actor act.frame sf@                ( F: frame )
    anim@ case
        4 of  $6F make-sound  endof
        9 of  h-ready @ 1 = if  $70 make-sound  then  endof
        5 of  $58 make-sound  endof
        6 of  $59 make-sound  endof  7 of  $59 make-sound  endof  $206 of  $59 make-sound  endof
    endcase
    anim@ snd-anim @ <>  fdup snd-frame f@ f< or if
        anim@ bark-sound ?dup if  make-sound  then
        anim@ $1B00 $1B06 within if
            h-action @ dup $7B = over $B = or swap $A = or if  $1B  else  0  then  h-room h-tri noise-make
        then
        anim@ $1000 = if  $80 h-room h-tri noise-make  then
    then
    snd-frame f!  anim@ snd-anim ! ;

\ ---- his overlays: head, ears and tail playing their own motions by his animation and mood
\ (Hewie_SetOverlays / OverlayAnim / IdleOverlay / TailOverlay) ----
variable ov-head  variable ov-ears  variable ov-tail          \ +0xF3640 / +0xF3648 / +0xF3654
variable ov-head-t  variable ov-ears-t  variable ov-ears-alt  variable ov-tail-t  variable ov-tail-on
variable oh  variable oe  variable ot
: overlay ( anim -- )  h-actor dup 0< if  2drop exit  then  swap motion-overlay ;
: head-overlay ( -- )   \ his mouth: 0x1F00 / 0x1F01 / 0x1F02, or (3) changing now and then
    ov-head @ case
        0 of  $1F00 overlay  endof  1 of  $1F01 overlay  endof  2 of  $1F02 overlay  endof
        3 of  -1 ov-head-t +!  ov-head-t @ 0< if
                  15 roll 30 * 30 + ov-head-t !  rnd01 0.5e f< if  $1F00  else  $1F01  then  overlay
              then  endof
    endcase ;
: ears-overlay ( -- )   \ his ears: 0x2000 / 0x2001 (hurt 0x2002), or (0) flicking between them
    ov-ears @ case
        0 of  h-cond 1 = if  $2002 overlay  else
                  -1 ov-ears-t +!  ov-ears-t @ 0< if
                      ov-ears-alt @ 0= if  $2000 overlay  1  else  $2001 overlay  0  then  ov-ears-alt !
                      2 roll if  900  else  300  then  10 roll 30 * + ov-ears-t !
                  then
              then  endof
        1 of  h-cond 1 = if  $2002  else  $2000  then  overlay  endof
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
    game-mode @ 0= if  overlays-calm  else  overlays-tense  then
    begin  dup @ -1 <> while  dup @ anim@ = if  exit  then  13 cells +  repeat  drop 0 ;
: set-overlays ( -- )
    overlay-entry ?dup 0= if  exit  then
    ov-head @ oh !  ov-ears @ oe !  ov-tail @ ot !
    h-mood @ dup 4 u< if
        cells over +  dup cell+ @ ov-head !  dup 5 cells + @ ov-ears !  9 cells + @ ov-tail !  drop
    else  2drop  then
    ov-head @ oh @ <> ov-head @ 3 = and if  0 ov-head-t !  then
    ov-ears @ oe @ <> ov-ears @ 0= and if  0 ov-ears-t !  then
    ov-tail @ ot @ <> ov-tail @ dup 4 = swap 6 = or and if  0 ov-tail-t !  0 ov-tail-on !  then
    head-overlay  ears-overlay  tail-overlay ;
\ his body as it is posed (DogModel_AdjustBone): his back and shoulders along the floor
\ (bones 0 and 16), his neck turned where his head looks (bones 0x1D, 0x1E, 0x1F)
: neck ( -- )
    h-actor dup 0< if  drop exit  then
    dup turns-clear
    back-pitch f@ f0= 0= if  back-pitch f@ 0e  dup 0 turn+  then
    front-pitch f@ back-pitch f@ f- fdup f0= if  fdrop  else  0e  dup $10 turn+  then
    h-head-pitch f@ -0.4e f*  h-head-yaw f@ 0.25e f*  dup $1D turn+
    h-head-pitch f@ -0.4e f*  h-head-yaw f@ 0.25e f*  dup $1E turn+
    h-head-pitch f@ -0.2e f*  h-head-yaw f@ 0.25e f*  $1F turn+ ;

\ Hewie_Feet: a step's sound as a foot comes down (the motions' contact tracks), by the floor
\ (triangle flags 0x8000 / 0x10000 / 0x18000 / 0x2000000); four sounds in turn. (Water steps
\ in rooms 7 / 0xD1 / 0x106: with the room effects.)
create feet-was 4 cells allot  feet-was 4 cells 0 fill
variable feet-new  variable feet-n
\ the step's pitch by how fast he goes: 0..2 semitones up from 0.28 to 2.4 a frame forward
: step-pitch ( -- n )
    h-actor root-delta  fswap fdrop fswap fdrop fswap fdrop  floor-k f@ f*
    0.28e f- 2.12e f/  0e fmax 1e fmin  2e f* f>s $7F and ;
: feet ( -- )
    h-disabled? h-tri 0< or if  exit  then
    0 feet-new !
    4 0 do
        h-actor i foot-down?
        dup i cells feet-was + @ 0= and if  1 i lshift feet-new @ or feet-new !  then
        i cells feet-was + !
    loop
    feet-new @ 0= if  exit  then
    h-tri nav-flags $2018000 and case
        $8000 of  $14 5  endof  $10000 of  $18 5  endof  $18000 of  $1C 5  endof  $2000000 of  $78 5  endof
        $2008000 of  6 sound-loaded? if  $18 6  else  $10 5  then  endof
        >r $10 5 r>
    endcase                                                    ( snd bank )
    swap feet-n @ 3 and +  swap  him -rot  0 step-pitch  h-pos vec@ actor-sound  1 feet-n +! ;

\ Hewie_MoveSubMode: what he is doing, for the scripts (+0xFC)
: move-sub ( -- )
    h-mode 0= 0= if  exit  then
    anim-group case
        0 of 0 endof  4 of 0 endof  $F of 0 endof
        1 of 3 endof  5 of 3 endof
        2 of 4 endof  3 of 4 endof  6 of 4 endof  7 of 4 endof  $D of 4 endof
        8 of 2 endof
        9 of 1 endof  $A of 1 endof  $B of 1 endof  $C of 1 endof
        >r -1 r>
    endcase  dup 0< if  drop exit  then  him character char.sub l! ;

\ his animation's root motion: turned by it, moved by it (unless the behaviour moved him)
: root-motion ( -- )  root@  him rm-x f@ rm-z f@ c-move-local ;

\ ---- under a script ----
\ Hewie_FullStop: what the scripts' request 1 clears
: full-stop ( -- )
    0 h-broke !  0 h-hits !  $FF h-look-char !  0 h-scene-req !  0 h-look-pt? !  $B state-flag-clear ;
\ fiona_reachable: under a script in the room being played, he can get to her
: fiona-reachable? ( -- flag )
    h-busy? 0= if  true exit  then
    h-room played-room <> if  true exit  then
    h-2d @ h-2b @ or if  false exit  then
    her c-active? 0= if  true exit  then
    her c-tri her c-pos plan-to ;
\ the move the scripts gave him, into his fields (the original keeps them in the character)
fvariable rq-y
: move-fields ( -- )
    him cells move-a + @ h-to-tri !  him cells move-b + @ h-to-anim !
    him character char.face sf@ h-to-yaw f!
    h-to him character char.target vec-copy
    \ (the scripts give points on the floor at height 0: onto his triangle's floor)
    h-to-tri @ dup 0 nav-tris within if
        tri-center fdrop rq-y f! fdrop
        h-to sf@ rq-y f@ 8e f+ h-to 8 + sf@ floor-below if  h-to 4 + sf!  else  rq-y f@ h-to 4 + sf!  then
    else  drop  then ;
\ Hewie_Requests: the state block's requests (7 holds, 13 her command, 5 stops him), then the
\ scripts' move as the action that carries it out
: requests ( -- )
    req@ 7 = if  exit  then
    req@ 4 = if  0 req!  then
    req@ 13 = if  fiona-reachable? if  fiona-command if  full-stop  then  0 req!  exit  then  0 req!  then
    req@ 5 = if  path-end  0 0 want  0 req!  then
    0 req!
    him character char.move sl@ ?dup 0= if  exit  then
    move-fields
    case
        1 of  full-stop  0  endof
        2 of  0  endof  3 of  0  endof  4 of  0  endof
        5 of  $3F  endof  6 of  $41  endof  7 of  $3B  endof  8 of  $3C  endof  9 of  $3D  endof
        10 of  $40  endof  11 of  $42  endof  14 of  $43  endof  15 of  $44  endof  16 of  $3E  endof
        18 of  $45  endof  19 of  $7F  endof  20 of  $46  endof  21 of  $47  endof
        12 of  0 h-look-pt? !  him cells move-slot + @ h-look-char !  h-done  -1  endof
        13 of  1 h-look-pt? !  h-look-pt h-to vec-copy  -1  endof
        >r -1 r>
    endcase
    dup 0< 0= if  0 want  else  drop  then
    0 him character char.move l! ;
\ Hewie_Think: a frame of his while a script has him
: hewie-think ( -- )
    h-2b @ if  0  else  $29020008  then  him c-mask!
    1 h-snd-t +!  h-snd-t @ 3000 > if  3000 h-snd-t !  then
    alert  h-cond 2 <> if  report-fiona-near  then
    requests
    0 h-no-root !  1 h-root-ok !
    h-state @ ?dup if  execute  then
    turn-by-anim
    h-disabled? 0= h-no-root @ 0= and if  root@  him rm-x f@ rm-z f@ c-move-local  then
    turn-head  neck  anim-sounds  feet
    h-yaw h-yaw-was f!  h-action @ h-last-action ! ;

\ Hewie_HiddenFrame: each frame out of the room being played - down he stays down; Fiona
\ panicking may bring him (0x39, once at 4 and once at 5, by his trust); a yelp's time out, he
\ sets off toward her (0x2C). (Noises he hears: with the stalkers.)
: hidden-frame ( -- )
    h-cond 2 = if  h-action @ $52 <> if  $52 0 want  then  exit  then
    h-panic-seen @ 2 and 0= if
        false
        panic @ 5 = if  h-panic-seen @ 2 or h-panic-seen !  drop true
        else h-panic-seen @ 1 and 0= panic @ 4 = and if  h-panic-seen @ 1 or h-panic-seen !  drop true  then  then
        if  hidden-out-chance by-chance h-mode $39 <> and if  $39 0 want exit  then  then
    then
    panic @ 4 < if  0 h-panic-seen !  then
    h-yelp @ if
        h-yelp @ 0> h-action @ $77 <> and if  $77 0 want exit  then
    else  -1 h-yelp !  0 h-hide !  $2C 0 want  then
    h-cmd @ $80000040 and $40 = 1 cond-bit? and if  1 h-pending !  then ;

\ ---- Hewie_Update: a frame of his ----
variable was-busy
: hewie-frame ( -- )
    h-busy? if  -1 was-busy !  hewie-think exit  then
    was-busy @ if  0 was-busy !  full-stop  to-default  then   \ (the script let him go)
    upkeep  alert  h-cond 2 <> if  report-fiona-near  then  obedience
    h-disabled? if   \ out of the room being played
        arrive 0= if
            req@ 13 = if  fiona-command drop  then  0 req!
            hidden-frame
            h-mood-state @ ?dup if  execute  then
            h-state @ ?dup if  execute  then
        then  exit
    then
    state-block
    h-2b @ if  8  else  $29020008  then  him c-mask!
    h-root-ok @ fit-floor
    0 h-no-root !  1 h-root-ok !
    cutscene-active? if  standing-frame  else  hewie-control @ 0= if  own-decisions  then  then
    h-mood-state @ ?dup if  execute  then
    h-state @ ?dup if  execute  then
    turn-by-anim
    h-no-root @ 0= if  root-motion  then
    turn-head  neck  set-overlays  anim-sounds  feet
    h-yaw h-yaw-was f!  h-action @ h-last-action !  h-alert @ h-alert-was !
    0 h-look !  move-sub ;

\ Hewie_Activate: his state as he comes into the game (in play: calm, the default action)
: hewie-start ( -- )
    h-reset-fields
    0 -1 set-mode  0 add-trust
    0 h-waiting !  obey-time!  0 h-praise-due !  1 h-pending !
    100 hp!  0 cond!  300 h-hurt-t !
    $FF h-call !  0 h-mood !  -1 h-yelp !  -1 h-target !
    ['] st-calm h-mood-state !
    0 0 set-action ;

\ Hewie_MayBreakOff: may what he does be broken off (`once`: only the first time)? 0 yes, -1 no
: may-break-off ( once -- 0 | -1 )
    h-broke @ 1 = and if  -1 exit  then
    h-busy? 0= h-cmd @ $80000008 and 8 <> and if  -1 exit  then
    1 h-broke !
    h-waiting @ 0<> h-action @ $7D <> and  h-mood @ 3 = or if  -1  else  0  then ;
' when-idle is idle-off

\ Hewie_FionaCanCommand (for the scripts): she is here and he can take a command
: can-command? ( -- flag )
    her with? 0= h-busy? or h-mode 0<> or h-mood @ 3 = or if  false exit  then
    h-cmd @ 0= h-cmd @ $80000000 and or if  false exit  then
    req@ 0= ;
