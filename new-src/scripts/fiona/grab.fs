\ fiona/grab.fs - Fiona seized and led away (src/game/fiona.c): the joint action a stalker gives
\ her (Fiona_JointAction kind 1: her meeting spot by him, kFionaMeetOffsets by her costume and
\ its type - 6 by the hand, 8 walking), then led there (led_away / led_step / led_arrive) and
\ dragged on - shaking free (Fiona_Shakes against her panic's stage) or dragged off: the game's
\ over (progress flag 0xC, +0x73EB00 1).
IN: fiona.grab
USING: engine game-state events.core events.words chars relations fiona.core fiona.moves fiona.brain fiona.commands fiona.doors fiona.panic fiona.react ;

\ ---- the spot she's led to (+0x104 the triangle, +0x110 the point, +0x10C the heading) ----
variable led-tri  create led-at 12 allot  fvariable led-face
variable costume   \ (+0x1AD548: her costume 0..6)
: meet-index ( type -- i )
    costume @ case
        0 of  6 = if  4  else  5  then  endof
        1 of  6 = if  7  else  8  then  endof   2 of  6 = if  7  else  8  then  endof
        3 of  6 = if  9  else  $B  then  endof   4 of  6 = if  $A  else  $C  then  endof
        5 of  drop $D  endof   6 of  6 = if  $E  else  $F  then  endof
        >r drop 0 r>
    endcase ;
: meet@ ( i -- ) ( F: -- x z deg )
    12 * $3B2460 + >r  r@ 4 exe-bytes sf@  r@ 4 + 4 exe-bytes sf@  r> 8 + 4 exe-bytes sf@ ;
create mt-at 12 allot
\ Fiona_JointAction kind 1: standing free, her spot by him (his heading turned by the offset)
\ reachable straight from him and straight to her, she not on a blocked triangle: started
: meet-action-now ( -- started? )
    f-mode if  false exit  then
    me relations:cmd-other >r
    me relations:cmd-arg meet-index meet@                            ( F: x z deg )
    3.1415927e f* 180e f/ r@ c-yaw f+ angle-wrap led-face f!         ( F: x z )
    r@ c-yaw rotate-by  r@ c-pos 8 + sf@ f+ mt-at 8 + sf!  r@ c-pos sf@ f+ mt-at sf!
    r@ c-pos 4 + sf@ mt-at 4 + sf!
    r@ c-tri r@ c-pos mt-at 0 v-walk dup 0< if  r> 2drop false exit  then   ( tri )
    r> drop
    dup f-tri f-pos mt-at 0 v-walk = 0= if  drop false exit  then  \ (her floor reaches it)
    f-tri nav-flags me c-mask and if  drop false exit  then
    dup led-tri !  led-at mt-at vec-copy
    me swap led-at me c-mask c-plan 0= if  false exit  then
    me relations:cmd-start  true ;
' meet-action-now is fiona.commands:meet-action

\ ---- led there ----
fvariable led-step-d  fvariable led-turn  fvariable led-turn-to
: leader ( -- cs )  fiona.react:f-who @ ;
: leader-gone? ( -- flag )
    leader dup c-active? 0= swap character char.disabled sl@ 0<> or ;
: led-step ( -- )   \ turned to the spot's heading, on along her path
    led-turn-to f@ led-turn f@ f-turn-toward fdrop
    led-step-d f@ me path-ahead me path-i!
    pa-pos me c-mask v-tri-in dup 0< if  drop  else  me c-tri!  f-pos pa-pos vec-copy  me c-sync  then ;
: led-arrive ( -- )
    led-tri @ me c-tri!  led-at vec@ me c-place!  led-turn-to f@ f-yaw! ;
\ led_away: her leader gone or no longer leading (his mode 8): she stands; else her way to the
\ spot, walked in 5 steps (`anim`, held while it fades in), turning by a fifth a step; no way:
\ the leader told (state 7)
: led-away ( anim next -- )
    1 f-2a !  0 f-cam-on !
    leader-gone? leader c-mode 8 <> or if  2drop to-idle exit  then
    me led-tri @ led-at me c-mask c-plan if
        me path-rest 0.2e f* led-step-d f!
        swap fiona.doors:play-own
        led-face f@ led-turn-to f!  led-face f@ f-yaw f- angle-wrap fabs 0.2e f* led-turn f!
        behave exit
    then  2drop
    leader relations:req-of 7 <> if  0e 7 0 0 0 0 leader relations:req-set  then
    to-idle ;
\ getting up after being pulled free (getting_up, busy)
: st-pulled-free ( -- )
    1 f-2a !  0 f-cam-on !
    f-end? if  0 f-2d !  to-idle  then  f-root-move ;
: pulled-free ( -- )  $F02 -1 f-play-table  ['] st-pulled-free behave ;
\ the game's over (caught for good): progress flag 0xC, +0x73EB00 1 - unless flag 0x2C
variable game-over-kind   \ (+0x73EB00)
: caught-for-good ( -- )
    $2C state-flag? if  exit  then
    1 game-over-kind !  $C state-flag-set
    not-yet" the game over (progress flag 0xC: SceneGame's)" ;
\ ---- dragged (0x1401 again and again; 6 of them: dragged off) - shaking free: past her
\ panic's stage's count (Fiona_ShakesToBreakFree: 8 12 23 35 70) the leader is told (7) and
\ she breaks away (0x1403) ----
create shakes-to-break  8 , 12 , 23 , 35 , 70 , 0 ,
variable dr-shakes  variable dr-drags  variable dr-free
: st-dragged-off ( -- )
    1 f-2a !  0 f-cam-on !
    leader-gone? if  pulled-free exit  then
    leader c-mode 8 <> if  pulled-free
    else f-end? f-anim@ $1404 = and if  caught-for-good  then then
    f-root-move ;
: st-dragged ( -- )
    1 f-2a !  0 f-cam-on !
    leader-gone? if  pulled-free exit  then
    dr-shakes @ -1 <> if
        shakes dr-shakes +!  dr-drags @ 1+ 10 * dr-shakes @ min dr-shakes !
    then
    leader c-mode 8 <> if  pulled-free
    else f-end? if
        dr-free @ if  $1403 -1 f-play  ['] st-pulled-free behave
        else
            1 dr-drags +!
            dr-drags @ 6 = if  hewie-control @ 0= if  $2B state-flag-set  then
                $1404 -1 f-play  ['] st-dragged-off behave
            else  $1401 -1 f-play  then
        then
    else dr-shakes @ -1 <> if
        hewie-control @ 0= if  dr-shakes @ panic @ cells shakes-to-break + @ >= if  -1 dr-shakes !  then
        else dr-drags @ panic @ = if  -1 dr-shakes !  then then
        dr-shakes @ -1 = dr-free @ 0= and if
            1 dr-free !  leader relations:req-of 7 <> if  0e 7 0 0 0 0 leader relations:req-set  then
        then
    then then then
    f-root-move ;
: st-hand-spot ( -- )   \ at his hand: dragged once its key comes (0x1401); he lets go: free
    1 f-2a !  0 f-cam-on !
    leader-gone? if  to-idle exit  then
    leader c-mode 8 <> if  pulled-free
    else f-end? if  $1401 -1 f-play  ['] st-dragged behave  then then
    f-root-move ;
: st-led-hand ( -- )   \ led by the hand (0x1400) to his side
    1 f-2a !  0 f-cam-on !
    leader-gone? leader c-mode 8 <> or if  to-idle exit  then
    f-settled? if
        hewie-control @ 1 = rnd 0.25e f< and if  $38 5 0 0 voice  then
        led-arrive  0 dr-free !  0 dr-shakes !  0 dr-drags !
        ['] st-hand-spot behave
    else  led-step  then ;
: st-led-spot ( -- )   \ led walking: there, at its end the game's over
    1 f-2a !  0 f-cam-on !
    leader-gone? if  to-idle exit  then
    f-end? if  caught-for-good  then  f-root-move ;
: st-led-walk ( -- )   \ led walking (0x1500) to his side
    1 f-2a !  0 f-cam-on !
    leader-gone? if  to-idle exit  then
    f-settled? if
        led-arrive  hewie-control @ 0= if  $2B state-flag-set  then  ['] st-led-spot behave
    else  led-step  then ;
: st-led-by-hand ( -- )  $1400 ['] st-led-hand led-away ;
: st-led-walking ( -- )  $1500 ['] st-led-walk led-away ;
\ Fiona_React 9: led away (his request's b: 6 by the hand, else walking)
:noname ( -- )
    1 f-2a !
    me 4 relations:req-word-of 6 = if  ['] st-led-by-hand  else  ['] st-led-walking  then
    $43 5 0 0 f-sound  behave ; is fiona.react:react-led
