\ fiona/commands.fs - Fiona and Hewie (F3; the original's Fiona_ControlCommand,
\ Fiona_HewieCommandAction, Fiona_CommandHewie, Fiona_CallHewie, Fiona_StateActionOver and the
\ gestures and held commands after it). She reads Hewie from his broadcast (hewie-doing) and
\ his body; she tells him with `command`, `reaction` and the meeting (meet-me / meet-now).
\
\ The right stick's gestures on the keyboard: 1 up (go / attack), 2 down (come back), 3 R3
\ (stay / come), 4 right (praise; held: more), 5 left (scold). They are read only under state
\ flag 0xD (the story: Hewie is hers to command) and not 0x2B.
IN: fiona.commands
USING: engine game-state actors messages common facts keys fiona.state fiona.model fiona.moving fiona.spots fiona.looks flag-names ;

defer to-idle   ' noop is to-idle      \ (fiona.fs)
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: dog-in? ( -- flag )  dog-here @ 0<> ;            \ in the game (+0x1AD5D4)
: dog-at ( -- v )  hewie-id body-at ;
: dog-dist ( F: -- d )  dog-ok? if  her-at dog-at vec-dist  else  1000e  then ;
: react ( n -- )  hewie-id send reaction ;          \ Fiona_HewieReact (his to weigh)
: noise-here ( loud -- )  room-id her-tri @ -1 fiona-noise acoustics-id send noise ;

\ ---- her lines (fiona_voice; Fiona_OrderLine; Fiona_CallHewie) ----
: line ( id -- )  5 0 0 her-pos sound-at ;
: order-line ( -- )
    plain-commands state-flag? if  rnd 0.5e f< if  $33  else  $39  then  line exit  then
    her-sub @ case
        $2C of  her-danger @ case  2 of  $33 line  endof  1 of  $31 line  endof  0 of  $32 line  endof  endcase
                dog-ok? 0= if  $20 noise-here  then  endof
        $2D of  $30 line  endof
    endcase ;
: praise-scold-line ( -- )   \ 0x27: by what he is doing
    dog-action @
    dup $22 = over $20 = or over $21 = or over $1F = or over $59 = or over $53 = or
    dog-group @ 8 - 2 u< and if  drop $36 line exit  then
    dup $25 = over $24 = or over $63 = or over $13 = or over $A = or over $22 = or
    over $20 = or over $21 = or over $1F = or if  drop $36 line exit  then
    8 = if  $35 line exit  then
    dog-mode @ 0= dog-sub @ 3 = and if  $35 line exit  then
    rnd 0.5e f< if  $35  else  $36  then  line ;
: call-line ( -- )   \ Fiona_CallHewie: as the command's gesture shows
    her-sub @ case
        $23 of  her-danger @ 0= if  $2E  else  $2F  then  line  endof
        $24 of  $37 line  endof
        $25 of  $34 line  endof
        $27 of  praise-scold-line  endof
        $28 of  rnd 0.75e f< if  $3A  else  $32  then  line  endof
        $29 of  dog-cond @ 2 = if  $33  else  $3A  then  line  endof
        $2A of  $39 line  endof
        $2B of  rnd 0.75e f< if  $3B  else  $30  then  line  endof
        $2C of  $31 line  dog-ok? 0= if  $20 noise-here  then  endof
        $2D of  her-danger @ 2 = if  $2F  else  $30  then  line  endof
        $2E of  $2F line  endof
        $2F of  $3B line  endof
    endcase ;

\ ---- the command (Fiona_MarkActionStart, Fiona_CommandHewie): its code, where and which way
\ she was; for "go there" (0x23) 5 short of where he would have to stop, further on her heading.
\ Then how it strikes him ----
: mark-action-start ( code -- )
    her-sub !  her-heading f@ her-act-yaw f!  her-tri @ her-act-tri !  her-act-at her-at vec-copy ;
create ch-at 12 allot
: go-spot ( -- )
    dog-ok? 0= if  exit  then
    hewie-id body-tri dog-at $29020008 her-act-yaw f@ 15e v-free     ( F: d )
    fdup 5e f> if
        5e f-  her-act-yaw f@ fswap ch-at her-act-at vec-ahead
        her-tri @ her-at ch-at $29020008 v-walk dup 0< if  drop  else  her-act-tri !  her-act-at ch-at vec-copy  then
    else  fdrop  then ;
: command-hewie ( -- )
    plain-commands state-flag? 0= if
        her-sub @ $23 = if  go-spot  then
        her-sub @  her-act-tri @  her-act-yaw f@ f>cell  her-act-at sf@ f>cell  her-act-at 8 + sf@ f>cell
        hewie-id send command
    then
    her-sub @ case
        $2D of  0 react  endof  $23 of  0 react  endof
        $2C of  1 react  endof
        $27 of  2 react  endof  $25 of  2 react  endof
        $2F of  3 react  endof  $29 of  4 react  endof
        $30 of  14 react  endof  $2E of  15 react  endof
    endcase ;

\ ---- Fiona_HewieCommandAction: the code for a gesture (0 up, 1 down, 2 R3, 3 right, 4 left) ----
: facing-diff ( F: a -- d )  her-yaw f@ f- angle-wrap fabs ;
: call-action ( -- code )   \ up: go there (calm), go for it (chased); (followed: a creature ahead - later)
    her-danger @ 2 = if  dog-ok? dog-mode @ 8 = and if  $2E  else  $2D  then  exit  then
    $23 ;
: command-code ( cmd -- code )
    case
        0 of  call-action  endof
        1 of  $2C  endof
        2 of  her-danger @ 0<> dog-ok? 0= or if  $27
              else dog-dist 30e f< 0= if  $27
              else dog-dist 12e f< 0= if  $25
              else dog-action @ $4B = dog-sub @ 3 = dog-action @ 2 = and or if  $24  else  $25  then
              then then then  endof
        3 of  dog-ok? 0= dog-dist 15e f< 0= or if  $29
              else dog-cond @ 2 = if  her-at dog-at vec-heading facing-diff 60e deg>rad f< if  $2A  else  $29  then
              else her-danger @ 0<> dog-mood @ 3 = or if  $29  else  $28  then
              then then  endof
        4 of  her-danger @ 0<> dog-ok? 0= or dog-dist 15e f< 0= or dog-mood @ 3 = or
              if  $2F  else  $2B  then  endof
        >r -1 r>
    endcase ;

\ ---- after a command's gesture (Fiona_StateActionOver, the looks after it) ----
: play-now ( anim -- )  dup entry nip >r -1 0 r> start ;   \ (Motion_Play: cut in)
: idle-after-command ( -- )   \ Fiona_IdleAfterCommand
    plain-commands state-flag? if  rnd 0.5e f< if  1  else  $C02  then  -1 play-table exit  then
    her-sub @ case
        $29 of  $C02  endof  $2F of  $C02  endof  $2E of  $C0E  endof  $23 of  $C04  endof
        $24 of  $C06  endof  $2C of  $C02  endof  $2A of  $C0A  endof  $25 of  $C03  endof
        $27 of  $C03  endof  $2D of  $C00  endof
        >r 0 r>
    endcase  ?dup if  -1 play-table  then ;
: motion-bits ( -- bits )  her-model @ 0 0 1 motion-events ;   \ (its key frames this frame)
: face-after ( -- )   \ (the one looked at after the command stays the one she faces)
    her-look-after @ 0< 0= if  her-look-after @ her-look-who !  1 her-look-on !  then ;
: look-around ( -- )   \ Fiona_StateLookAround (0xC0E, then 0xC0F): the command given on its event
    face-after
    motion-bits 2 and if  command-hewie  then
    ended? if  anim@ $C0E = if  $C0F play-now exit  then  to-idle  then ;
: look-end ( -- )   \ Fiona_StateLookEnd (plain-commands: the look forgotten)
    plain-commands state-flag? if  -1 her-look-after !  else  face-after  then
    settled? if  idle-after-command  ['] look-around her-act !  then ;

\ ---- the gesture for "praise" while he is down (0x2A: Fiona_StateWalkThenGesture ..) ----
: gesture-end ( -- )  ended? if  to-idle  then  root-move ;
: gesturing ( -- )  ended? if  command-hewie  $C0C play-now  ['] gesture-end her-act !  then  root-move ;
: gesture-lead-in ( -- )  ended? if  $C0B play-now  ['] gesturing her-act !  then  root-move ;
: walk-then-gesture ( -- )
    settled? 0= if  walk-look exit  then
    $C0A -1 play-table  ['] gesture-lead-in her-act ! ;

\ ---- the meeting by his side (0x24 stay, 0x28 praise, 0x2B scold): she asks; at the place he
\ finds she faces him, he sits, the second part and her gesture (Fiona_StateHeldStart ..) ----
: meet-type ( -- t )  her-sub @ case  $2B of  0  endof  $28 of  2  endof  >r 4 r>  endcase ;
: dog-off? ( -- flag )  dog-ok? 0=  dog-action @ $48 <> or ;   \ held_off
: give-up ( -- )  hewie-id send meet-off  root-move  to-idle ;
: held-command ( -- )
    her-danger @ 0= her-sub @ $28 = and her-cmd @ 3 = and dog-ok? and dog-mode @ $C = and anim@ $C08 = and if
        2 her-held-n !
    then
    ended? if
        her-sub @ case
            $28 of
                anim@ case
                    $C07 of  1 her-held-n !  her-danger @ 0= dog-ok? and dog-mode @ $C = and if  3 her-held-n !  then
                             $C08 play-now  endof
                    $C08 of  -1 her-held-n +!  her-held-n @ 0= if  $C09  else  $C08  then  play-now  endof
                    $C09 of  6 react  to-idle  endof
                endcase  endof
            $2B of  5 react  to-idle  endof
            $24 of  7 react  to-idle  endof
        endcase
    then
    root-move ;
: meet-waiting ( -- )  root-move ;   \ (the answer is on its way)
: wait-hewie ( -- )   \ Fiona_StateWaitHewie: him ready (0x48, sitting, settled), the second part
    dog-off? if  give-up exit  then
    settled? 0= if  root-move exit  then
    dog-group @ 1 <> if  exit  then
    meet-type 1+ hewie-id send meet-now  ['] meet-waiting her-act ! ;
: held-stopped ( -- )  dog-off? if  give-up exit  then  settled? if  ['] wait-hewie her-act !  then ;
: held-walk ( -- )
    dog-off? if  give-up exit  then
    walk-to-spot  dup 0< if  drop give-up exit  then
    0= if  -1 idle-anim  ['] held-stopped her-act !  then ;
: meet-placed ( tri F: x y z face -- )  walk-spot  ['] held-walk her-act ! ;   \ (his answer: her place)
: meet-started ( -- )   \ (his answer to the second part: her gesture)
    dog-ok? 0= if  give-up exit  then
    her-sub @ case  $2B of  $C0D  endof  $28 of  $C07  endof  $24 of  $C06  endof  >r 0 r>  endcase
    ?dup if  -1 play-table  then
    ['] held-command her-act ! ;
: meet-failed ( -- )   \ refused: asked - from afar instead (scold, praise) or nothing; sitting - wait on
    her-act @ ['] meet-waiting = if  ['] wait-hewie her-act ! exit  then
    root-move
    her-sub @ case
        $2B of  $2F her-sub !  ['] look-end her-act !  endof
        $28 of  $29 her-sub !  ['] look-end her-act !  endof
        >r to-idle r>
    endcase ;

\ ---- Fiona_StateActionOver ----
: in-sight? ( id -- flag )  >r her-tri @ her-at r> body-at $40080 v-walk 0< 0= ;   \ fiona_in_sight
: action-over ( -- )
    settled? if
        -1 her-look-after !   \ (whom she glances at after it: Hewie, with her and in sight)
        her-sub @ dup $24 = over $2B = or swap $28 = or 0=  dog-ok? and if
            hewie-id dup in-sight? if  her-look-after !  else  drop  then
        then
        her-sub @ case
            $2A of  ['] walk-then-gesture her-act !  endof
            $24 of  meet-type hewie-id send meet-me  ['] meet-waiting her-act !  endof
            $2B of  meet-type hewie-id send meet-me  ['] meet-waiting her-act !  endof
            $28 of  meet-type hewie-id send meet-me  ['] meet-waiting her-act !  endof
            >r ['] look-end her-act ! r>
        endcase
    then
    root-move ;
: asked? ( -- flag )  her-act @ ['] meet-waiting =  her-sub @ dup $24 = over $28 = or swap $2B = or and ;

\ ---- Fiona_ControlCommand: her command - in a panic or held a cry for him; otherwise the
\ command's action (on the move, "come back" and "go for it" without stopping) ----
: control-command ( -- )
    her-cmd @ dup -1 = if  drop exit  then
    her-fear-bits @ 2 and  her-mode @ 4 = her-sub @ dup 9 = swap $12 = or and  or if
        drop  $30 -1 0 0 0 hewie-id send command   \ (a cry for help)
        $38 line  60 her-busy-t !  exit
    then
    her-mode @ if  drop exit  then
    her-doing @ dup $E = over 1 = or swap $F = or if  drop exit  then
    command-code dup -1 = if  drop exit  then
    dup $2C = over $2D = or  her-sub @ 0<> and  over $2D = her-danger @ 1 = and 0= and if
        mark-action-start  command-hewie  order-line  60 her-busy-t !  exit
    then
    $D her-mode !  $C her-doing !  mark-action-start  ['] action-over her-act ! ;

\ ---- her lines on the gestures' key frames (Fiona_MotionSounds' event 1, commanding) ----
create call-anims  $C0E , $C0D , $C0A , $C07 , $C06 , $C04 , $C03 , $C02 , $C00 ,
: call-anim? ( anim -- flag )  false  9 0 do  over i cells call-anims + @ = or  loop  nip ;
: command-sounds ( -- )
    her-mode @ $D <> if  exit  then
    motion-bits 1 and 0= if  exit  then
    anim@ 1 = if  $39 line exit  then
    anim@ call-anim? if  plain-commands state-flag? if  $33 line  else  call-line  then  then ;

\ ---- the gestures (Gesture_Update; Fiona_ReadsPad) ----
: gesture ( -- cmd )
    key: 1 pressed? if  0 exit  then  key: 2 pressed? if  1 exit  then
    key: 3 pressed? if  2 exit  then  key: 4 pressed? if  3 exit  then
    key: 5 pressed? if  4 exit  then
    key: 4 held? if  3 exit  then  -1 ;   \ (held: as the stick held over - the praise's repeats)
: reads-pad? ( -- flag )
    hewie-commandable state-flag? 0=  fiona-occupied state-flag? or  her-busy-t @ 0<> or if  false exit  then
    her-mode @ dup $D = swap $A = or if  true exit  then
    her-mode @ 0= if  her-doing @ dup 1 <> swap $E <> and exit  then
    her-mode @ 4 = if  her-sub @ dup 9 = swap $12 = or exit  then
    false ;
: read-command ( -- )
    her-busy-t @ if  -1 her-busy-t +!  then
    reads-pad? if  gesture  else  -1  then  her-cmd ! ;
