\ fiona/kick.fs - Fiona's kick and shove (src/game/fiona.c Fiona_StateKick*, Fiona_StateShove*,
\ Fiona_StateStrike), what she is asked to do with them (Fiona_StateBlock 8: the square
\ button's request 0x1A) and her answers to the others' requests (Fiona_CanInteract).
\
\ A blow is a request to those it meets (Relation_Request: kind 1 a shove, 2 a kick; the
\ damage; -0x8000 a stumble): the next frame's resolve marks who took it (relations'
\ ask-accepted), and she recoils (5 frames frozen). (The rumble, the creatures, and the bonus
\ costumes' weapons and sparks: later.)
IN: fiona.kick
USING: engine game-state events.core events.words chars relations fiona.core fiona.moves fiona.brain fiona.commands fiona.panic ;

\ ---- the hit freeze (+0x14D0: Motion_Freeze for 5 frames; counted down in her frame) ----
: freeze ( -- )
    5 f-freeze-t !  f-actor dup 0< if  drop exit  then  actor act.mflags dup l@ $40 or swap l! ;

\ ---- her working fields ----
variable k-hit      \ FI 0x1AD6C0: who she has hit so far
variable k-dog      \ FI 0x1AD6C4: Hewie was hit (no reaction for it after)
fvariable k-pace    \ FI 0x1AD6D0: the shove's pace (negative: a strong hit)
: k-reset ( -- )  0 k-hit !  0 k-dog !  1e k-pace f! ;

\ the kick count (progress +0xFB6: +3 a time Hewie is hit, to 10000)
variable kicks
: kicked-dog ( -- )  kicks @ 3 + 0 max 10000 min kicks ! ;
\ who took her blow (the request she made last frame: progress +0x1020 + 16 x her slot)
: hit@ ( -- mask )  me ask-accepted ;
\ the damage scale (progress +0xA04: by difficulty)
fvariable dmg-scale  1e dmg-scale f!
: dmg ( n -- n' )  s>f dmg-scale f@ f* f>s $FFFF and ;

\ Fiona_ChaseRoll: being chased, a roll against the chance by how worn the pursuer is (its
\ health over its most: over 0.8, 0.6, 0.4, 0.2, 0.1, under)
create chance-by-health  0 c, 2 c, 5 c, 10 c, 10 c, 10 c,
defer pursuer-worn ( F: -- ratio )   :noname 1e ; is pursuer-worn   \ (the stalkers)
: chase-roll ( -- flag )
    f-pu-here @ 1 <> if  false exit  then
    pursuer-worn
    fdup 0.8e f> if  0  else fdup 0.6e f> if  1  else fdup 0.4e f> if  2
    else fdup 0.2e f> if  3  else fdup 0.1e f> if  4  else  5  then then then then then  fdrop
    chance-by-health + c@  100e rnd f* f>s swap < ;

\ Progress_CharNear: character `b` at `pos`, within its height and radius widened by `margin`
fvariable cn-m  fvariable cn-y  fvariable cn-by
: char-near? ( pos b -- flag ) ( F: margin -- )
    cn-m f!
    dup c-ok? 0= if  2drop false exit  then
    dup character char.present sl@ 0=  over character char.disabled sl@ 0<> or if  2drop false exit  then
    over 4 + sf@ cn-y f!  dup c-pos 4 + sf@ cn-by f!
    cn-y f@ cn-by f@ cn-m f@ f- f<= if  2drop false exit  then
    cn-y f@ cn-by f@ dup c-height f+ cn-m f@ f+ f< 0= if  2drop false exit  then
    dup c-pos rot swap vec-dist-xz  c-radius cn-m f@ f+ f< ;

\ ---- the kick (0xE01) ----
\ a step ahead (0x3F89B08A) a frame while it's on
: step-ahead ( -- )  0e 1.0756e f-yaw rotate-by f-move ;
: touching? ( cs -- flag )  me swap 0e 0e c-touching? ;
: took-hit ( -- )   \ the blow taken (the rumble 0xC0 for the pursuer's)
    hit@ 2 and if  kicked-dog  then
    me $90 5 0 0 f-pos vec@ actor-sound
    freeze  hit@ k-hit @ or k-hit ! ;
: st-kick ( -- )   \ Fiona_StateKick
    hit@ if  took-hit  then
    f-freeze-t @ 0= if  f-settled? 0= if  step-ahead  else  f-root-move  then  then
    f-end? if
        k-dog @ 0= if  11 hewie-react  then
        10e add-fear  0 f-run-t !  to-idle exit
    then
    f-events 2 and if  exit  then           \ (its hit window: event bit 2 clear)
    me c-radius at-ahead 1 slam-door if  exit  then
    0
    f-dog-ok @ 1 = k-hit @ 2 and 0= and dog touching? and if  2 or  1 k-dog !  then
    f-pu-ok @ 1 = k-hit @ 4 and 0= and if  pursuer-slot @ touching? if  4 or  then  then
    ?dup if  2 5 dmg  chase-roll if  -32768  else  0  then  me 0e ask  then ;
: st-kick-start ( -- )   \ Fiona_StateKickStart
    f-root-move
    f-settled? 0= if  exit  then
    me $3D 5 0 0 f-pos vec@ actor-sound
    k-reset  $E01 -1 f-play-table  ['] st-kick behave ;

\ ---- the shove (0xE00, its end 0x101) ----
: shoved ( -- )   \ a shove that met someone
    hit@ 2 and if  1 k-dog !  kicked-dog  then
    $26 pvar@ dup 6 = if  drop $22 5 0 0 f-sound  else  7 <> if  $8F 5 0 0 f-sound  then  then
    freeze ;
: shove-over ( -- )
    k-dog @ 0= if  10 hewie-react  then  0 f-run-t !  to-idle ;
: st-shove ( -- )   \ Fiona_StateShove
    hit@ if  shoved  then
    f-settled? if
        f-anim@ $E00 = if  $101 10 -1 f-play-blend  else  shove-over  then
    then
    f-root-move ;
\ Fiona_StateStrike: the shove's reach (her hand, motion +0x74's bone: 9), 2 across; to Hewie
\ and the pursuer kind 1, damage 1 (the shoes worn: 0x83 a tenth of the time a kick for 50,
\ 0x82 a kick for 5 and a fifth of the time a stumble, 0x81 2 and a tenth a stumble)
create hand 12 allot
variable s-kind  variable s-dmg  variable s-extra
: strike-power ( -- )
    1 s-kind !  1 s-dmg !  0 s-extra !
    worn @ case
        $83 of  rnd 0.1e f< if  2 s-kind !  50 s-dmg !  -1e k-pace f!  then  endof
        $82 of  rnd 0.2e f< if  -32768 s-extra !  then  2 s-kind !  5 s-dmg !  endof
        $81 of  rnd 0.1e f< if  -32768 s-extra !  then  2 s-dmg !  endof
    endcase ;
: st-strike ( -- )
    hit@ if  shoved  hit@ k-hit @ or k-hit !  then
    f-root-move
    f-end? if  0 f-run-t !  to-idle exit  then
    -1 f-events-at 2 and 0= if  exit  then
    f-actor 9 bone-pos hand vec!
    hand 1 slam-door                                      ( who )
    hand me c-mask v-tri-in 0< if                                     \ (a wall in the way: it ends)
        drop  f-settled? if  $101 10 -1 f-play-blend  then  ['] st-shove behave exit
    then
    if  exit  then
    0
    f-dog-ok @ 1 = k-hit @ 2 and 0= and if  hand dog 2e char-near? if  2 or  then  then
    f-pu-ok @ 1 = k-hit @ 4 and 0= and if  hand pursuer-slot @ 2e char-near? if  4 or  then  then
    1e k-pace f!  strike-power
    ?dup if  s-kind @ s-dmg @ dmg s-extra @ me 0e ask  then ;
: st-shove-start ( -- )   \ Fiona_StateShoveStart: slower the longer she's been shaken and the
                          \ more frightened (the costumes 6 / 7 at their own pace)
    f-settled? if
        $26 pvar@ dup 6 = swap 7 = or if
            $E00 -1 f-play-table
        else
            1e  f-recovery @ $1C3 < 0= if  fdrop 3150e f-recovery @ s>f f- 1800e f/ 1.5e f*  then
            1e  f-fear f@ 40e f> if  fdrop 160e f-fear f@ f- 120e f/  then
            fmin k-pace f!
            $E00 2 f-play-table  k-pace f@ f-weight!
        then
        k-reset  ['] st-strike behave
    then
    f-root-move ;

\ ---- Fiona_StateBlock 8: the square button's 0x1A - standing, walking or shaken a shove; running
\ (not yet 0x3D frames) a kick; fleeing a kick ----
:noname ( -- )
    $14 state-flag? if  exit  then
    0 f-target-t !
    me 1 req-word-of $1A <> if  exit  then
    f-act @ $D = if  8 f-mode!  6 f-act !  $1B f-sub!  ['] st-kick-start behave exit  then
    f-group dup 5 = over 1 = or over 0= or
    over 2 = f-recovery @ 0<> f-run-t @ $3D < or and or if
        drop  8 f-mode!  5 f-act !  $1A f-sub!  ['] st-shove-start behave exit
    then
    2 = if  8 f-mode!  6 f-act !  $1B f-sub!  ['] st-kick-start behave  then ; is kick-request

\ ---- Fiona_CanInteract (her vtable +0x68): whether she takes a request of `kind` ----
:noname ( cs kind asker b -- flag )
    >r >r nip r> r>                                    ( kind asker b )
    2 pick 5 = if  nip nip has-door? exit  then   \ (kind 5: through the door at exit b)
    drop  dup $FF <> if
        dup c-ok? 0= if  2drop false exit  then
        dup character char.present sl@ 0= swap character char.disabled sl@ 0<> and if  drop false exit  then
    else  drop  then
    f-mode swap case
        1 of  4 <>  endof  2 of  4 <>  endof  4 of  4 <>  endof
        3 of  drop true  endof
        6 of  dup 4 = over $A = or if  drop false  else  0= if  f-act @ $F <>  else  true  then  then  endof
        9 of  dup 4 = over 3 = or swap $A = or 0=  endof
        10 of  dup 4 = over 3 = or swap $A = or 0=  endof
        12 of  dup 4 = if  drop f-sub $12 =  else  dup 3 = swap $A = or 0=  then  endof
        >r drop false r>
    endcase ;  me accepts!
