\ fiona/commands.fs - Fiona and Hewie (F3; the original's Fiona_ControlCommand,
\ Fiona_HewieCommandAction, Fiona_CommandHewie, Fiona_CallHewie, Fiona_StateActionOver and the
\ gestures and held commands after it). She reads Hewie from his broadcast (hewie-doing) and
\ his body; she tells him with `command`, `reaction` and the meeting (meet-me / meet-now).
\
\ The right stick's gestures on the keyboard: 1 up (go / attack), 2 down (come back), 3 R3
\ (stay / come), 4 right (praise; held: more), 5 left (scold). They are read only under state
\ flag 0xD (the story: Hewie is hers to command) and not 0x2B.
IN: fiona.commands
USING: engine vectors game-state actors messages hewie-names common facts keys fiona.state fiona.anims fiona.model fiona.moving fiona.spots fiona.looks flag-names ;

defer to-idle   ' noop is to-idle      \ (fiona.fs)
: acoustics-id ( -- id )  s" acoustics" actor-named ;
: dog-in? ( -- flag )  dog-here @ dog-absent <> ;            \ in the game (+0x1AD5D4)
: dog-at ( -- v )  hewie-id body-at ;
: dog-dist ( F: -- d )  dog-ok? if  her-at dog-at vec-dist  else  1000e  then ;
: react ( n -- )  hewie-id send reaction ;          \ Fiona_HewieReact (his to weigh)
: noise-here ( loud -- )  room-id her-tri @ -1 fiona-noise acoustics-id send noise ;

\ ---- her lines (fiona_voice; Fiona_OrderLine; Fiona_CallHewie) ----
: line ( id -- )  0 her-sound ;   \ (her lines to him: by their sound id)
: order-line ( -- )
    plain-commands state-flag? if  rnd 0.5e f< if  $33  else  $39  then  line exit  then
    her-sub @ case
        come-back-cmd of  her-danger @ case  chased of  $33 line  endof  followed of  $31 line  endof  calm of  $32 line  endof  endcase
                dog-ok? 0= if  $20 noise-here  then  endof
        go-for-it-cmd of  $30 line  endof
    endcase ;
: praise-scold-line ( -- )   \ (come-cmd: by what he is doing)
    dog-action @ {: act :}
    act dup at-flank-act at-leg-2-act 1+ within  over harry-act = or  swap near-target-act = or   \ (at the stalker, on the run)
    dog-group @ dup in-running = swap in-trotting = or and if  $36 line exit  then
    act dup at-flank-act at-leg-2-act 1+ within  over away-from-her-act = or  over away-act = or  over go-there-act = or  over scramble-act = or  swap bark-at-target-act = or if
        $36 line exit   \ (at him, off, away)
    then
    act face-target-act = if  $35 line exit  then
    dog-mode @ free-mode = dog-sub @ sitting-sub = and if  $35 line exit  then
    rnd 0.5e f< if  $35  else  $36  then  line ;
: call-line ( -- )   \ Fiona_CallHewie: as the command's gesture shows
    her-sub @ case
        go-there-cmd of  her-danger @ calm = if  $2E  else  $2F  then  line  endof
        rouse-cmd of  $37 line  endof
        stay-cmd of  $34 line  endof
        come-cmd of  praise-scold-line  endof
        pat-cmd of  rnd 0.75e f< if  $3A  else  $32  then  line  endof
        praise-cmd of  dog-cond @ down-cond = if  $33  else  $3A  then  line  endof
        tend-cmd of  $39 line  endof
        scold-near-cmd of  rnd 0.75e f< if  $3B  else  $30  then  line  endof
        come-back-cmd of  $31 line  dog-ok? 0= if  $20 noise-here  then  endof
        go-for-it-cmd of  her-danger @ chased = if  $2F  else  $30  then  line  endof
        attack-now-cmd of  $2F line  endof
        scold-cmd of  $3B line  endof
    endcase ;

\ ---- the command (Fiona_MarkActionStart, Fiona_CommandHewie): its code, where and which way
\ she was; for "go there" (0x23) 5 short of where he would have to stop, further on her heading.
\ Then how it strikes him ----
: mark-action-start ( code -- )
    her-sub !  her-heading f@ her-act-yaw f!  her-tri @ her-act-tri !  her-act-at her-at vec-copy ;
create ch-at /vec allot
$29020008 constant his-floor-stops   \ (the floors Hewie can't stand on)
: go-spot ( -- )
    dog-ok? 0= if  exit  then
    hewie-id body-tri dog-at his-floor-stops her-act-yaw f@ 15e v-free     ( F: d )
    fdup 5e f> if
        5e f-  her-act-yaw f@ fswap ch-at her-act-at vec-ahead
        her-tri @ her-at ch-at his-floor-stops v-walk dup 0< if  drop  else  her-act-tri !  her-act-at ch-at vec-copy  then
    else  fdrop  then ;
: command-hewie ( -- )
    plain-commands state-flag? 0= if
        her-sub @ go-there-cmd = if  go-spot  then
        her-sub @  her-act-tri @  her-act-yaw f@ f>cell  her-act-at v.x sf@ f>cell  her-act-at v.z sf@ f>cell
        hewie-id send command
    then
    her-sub @ case
        go-for-it-cmd of  go-reaction react  endof  go-there-cmd of  go-reaction react  endof
        come-back-cmd of  come-back-reaction react  endof
        come-cmd of  come-reaction react  endof  stay-cmd of  come-reaction react  endof
        scold-cmd of  scold-reaction react  endof  praise-cmd of  praise-reaction react  endof
        help-cmd of  help-reaction react  endof  attack-now-cmd of  attack-now-reaction react  endof
    endcase ;

\ ---- Fiona_HewieCommandAction: the code for a gesture (0 up, 1 down, 2 R3, 3 right, 4 left) ----
: facing-diff ( F: a -- d )  her-yaw f@ f- angle-wrap fabs ;
\ a creature ahead (Fiona_CallAction, followed): in her room, within 10, reached straight from her,
\ within 45 degrees of her heading
: creature-ahead? ( id -- flag )
    dup alive? 0= if  drop false exit  then  dup body? 0= if  drop false exit  then
    dup body-room room-id <> if  drop false exit  then
    dup body-at her-at vec-dist 10e f< 0= if  drop false exit  then
    her-tri @ her-at 2 pick body-at blocked-floor v-walk  over body-tri <> if  drop false exit  then   \ (Actor_TriTo: her own floor)
    her-at swap body-at vec-heading facing-diff 45e deg>rad f< ;
: any-creature-ahead? ( -- flag )
    #creatures 0 do  i cells her-creatures + @ creature-ahead? if  true unloop exit  then  loop  false ;
: call-action ( -- code )   \ up: go there (calm), go for it (chased, or a creature ahead while followed)
    her-danger @ chased = if  dog-ok? dog-mode @ engaged-mode = and if  attack-now-cmd  else  go-for-it-cmd  then  exit  then
    her-danger @ followed = any-creature-ahead? and if  go-for-it-cmd exit  then
    go-there-cmd ;
: command-code ( cmd -- code )
    case
        go-gesture of  call-action  endof
        back-gesture of  come-back-cmd  endof
        stay-gesture of  her-danger @ calm <> dog-ok? 0= or if  come-cmd
              else dog-dist 30e f< 0= if  come-cmd
              else dog-dist 12e f< 0= if  stay-cmd
              else dog-action @ patted-act = dog-sub @ sitting-sub = dog-action @ stay-act = and or if  rouse-cmd  else  stay-cmd  then
              then then then  endof
        praise-gesture of  dog-ok? 0= dog-dist 15e f< 0= or if  praise-cmd
              else dog-cond @ down-cond = if  her-at dog-at vec-heading facing-diff 60e deg>rad f< if  tend-cmd  else  praise-cmd  then
              else her-danger @ calm <> dog-mood @ angry-mood = or if  praise-cmd  else  pat-cmd  then
              then then  endof
        scold-gesture of  her-danger @ calm <> dog-ok? 0= or dog-dist 15e f< 0= or dog-mood @ angry-mood = or
              if  scold-cmd  else  scold-near-cmd  then  endof
        >r -1 r>
    endcase ;

\ ---- after a command's gesture (Fiona_StateActionOver, the looks after it) ----
: play-now ( anim -- )  -1 0  2 pick entry nip  start ;   \ (Motion_Play: cut in)
: idle-after-command ( -- )   \ Fiona_IdleAfterCommand
    plain-commands state-flag? if  rnd 0.5e f< if  rest-anim  else  call-gesture-anim  then  -1 play-table exit  then
    her-sub @ case
        praise-cmd of  call-gesture-anim  endof  scold-cmd of  call-gesture-anim  endof  attack-now-cmd of  look-around-anim  endof
        go-there-cmd of  go-there-gesture-anim  endof  rouse-cmd of  rouse-gesture-anim  endof  come-back-cmd of  call-gesture-anim  endof
        tend-cmd of  tend-start-anim  endof  stay-cmd of  stay-gesture-anim  endof  come-cmd of  stay-gesture-anim  endof
        go-for-it-cmd of  go-for-it-gesture-anim  endof
        >r 0 r>
    endcase  ?dup if  -1 play-table  then ;
: motion-bits ( -- bits )  her-model @ 0 0 1 motion-events ;   \ (its key frames this frame)
: face-after ( -- )   \ (the one looked at after the command stays the one she faces)
    her-look-after @ 0< 0= if  her-look-after @ her-look-who !  1 her-look-on !  then ;
: look-around ( -- )   \ Fiona_StateLookAround (0xC0E, then 0xC0F): the command given on its event
    face-after
    motion-bits 2 and if  command-hewie  then
    ended? if  anim@ look-around-anim = if  look-around-end-anim play-now exit  then  to-idle  then ;
: look-end ( -- )   \ Fiona_StateLookEnd (plain-commands: the look forgotten)
    plain-commands state-flag? if  -1 her-look-after !  else  face-after  then
    settled? if  idle-after-command  ['] look-around her-act !  then ;

\ ---- the gesture for "praise" while he is down (0x2A: Fiona_StateWalkThenGesture ..) ----
: gesture-end ( -- )  ended? if  to-idle  then  root-move ;
: gesturing ( -- )  ended? if  command-hewie  tend-anim play-now  ['] gesture-end her-act !  then  root-move ;
: gesture-lead-in ( -- )  ended? if  tend-lead-in-anim play-now  ['] gesturing her-act !  then  root-move ;
: walk-then-gesture ( -- )
    settled? 0= if  walk-look exit  then
    tend-start-anim -1 play-table  ['] gesture-lead-in her-act ! ;

\ ---- the meeting by his side (0x24 stay, 0x28 praise, 0x2B scold): she asks; at the place he
\ finds she faces him, he sits, the second part and her gesture (Fiona_StateHeldStart ..) ----
: meet-type ( -- t )  her-sub @ case  scold-near-cmd of  0  endof  pat-cmd of  2  endof  >r 4 r>  endcase ;
: dog-off? ( -- flag )  dog-ok? 0=  dog-action @ turn-with-her-act <> or ;   \ held_off
: give-up ( -- )  hewie-id send meet-off  root-move  to-idle ;
: held-command ( -- )
    her-danger @ calm = her-sub @ pat-cmd = and her-cmd @ praise-gesture = and dog-ok? and dog-mode @ meeting-mode = and anim@ pat-anim = and if
        2 her-held-n !
    then
    ended? if
        her-sub @ case
            pat-cmd of
                anim@ case
                    pat-start-anim of  1 her-held-n !  her-danger @ calm = dog-ok? and dog-mode @ meeting-mode = and if  3 her-held-n !  then
                             pat-anim play-now  endof
                    pat-anim of  -1 her-held-n +!  her-held-n @ 0= if  pat-end-anim  else  pat-anim  then  play-now  endof
                    pat-end-anim of  pat-reaction react  to-idle  endof
                endcase  endof
            scold-near-cmd of  scold-close-reaction react  to-idle  endof
            rouse-cmd of  rouse-reaction react  to-idle  endof
        endcase
    then
    root-move ;
: meet-waiting ( -- )  root-move ;   \ (the answer is on its way)
: wait-hewie ( -- )   \ Fiona_StateWaitHewie: him ready (0x48, sitting, settled), the second part
    dog-off? if  give-up exit  then
    settled? 0= if  root-move exit  then
    dog-group @ in-sitting <> if  exit  then
    meet-type 1+ hewie-id send meet-now  ['] meet-waiting her-act ! ;
: held-stopped ( -- )  dog-off? if  give-up exit  then  settled? if  ['] wait-hewie her-act !  then ;
: held-walk ( -- )
    dog-off? if  give-up exit  then
    walk-to-spot  dup 0< if  drop give-up exit  then
    0= if  -1 idle-anim  ['] held-stopped her-act !  then ;
: meet-placed ( tri F: x y z face -- )  walk-spot  ['] held-walk her-act ! ;   \ (his answer: her place)
: meet-started ( -- )   \ (his answer to the second part: her gesture)
    dog-ok? 0= if  give-up exit  then
    her-sub @ case  scold-near-cmd of  scold-near-anim  endof  pat-cmd of  pat-start-anim  endof  rouse-cmd of  rouse-gesture-anim  endof  >r 0 r>  endcase
    ?dup if  -1 play-table  then
    ['] held-command her-act ! ;
: meet-failed ( -- )   \ refused: asked - from afar instead (scold, praise) or nothing; sitting - wait on
    her-act @ ['] meet-waiting = if  ['] wait-hewie her-act ! exit  then
    root-move
    her-sub @ case
        scold-near-cmd of  scold-cmd her-sub !  ['] look-end her-act !  endof
        pat-cmd of  praise-cmd her-sub !  ['] look-end her-act !  endof
        >r to-idle r>
    endcase ;

\ ---- Fiona_StateActionOver ----
: meeting? ( -- flag )  her-sub @ dup rouse-cmd = over scold-near-cmd = or swap pat-cmd = or ;   \ (by his side)

: action-over ( -- )
    settled? if
        -1 her-look-after !   \ (whom she glances at after it: Hewie, with her and in sight)
        meeting? 0=  dog-ok? and if
            hewie-id dup in-sight? if  her-look-after !  else  drop  then
        then
        her-sub @ case
            tend-cmd of  ['] walk-then-gesture her-act !  endof
            rouse-cmd of  meet-type hewie-id send meet-me  ['] meet-waiting her-act !  endof
            scold-near-cmd of  meet-type hewie-id send meet-me  ['] meet-waiting her-act !  endof
            pat-cmd of  meet-type hewie-id send meet-me  ['] meet-waiting her-act !  endof
            >r ['] look-end her-act ! r>
        endcase
    then
    root-move ;
: asked? ( -- flag )  her-act @ ['] meet-waiting =  meeting? and ;

\ ---- Fiona_ControlCommand: her command - in a panic or held a cry for him; otherwise the
\ command's action (on the move, "come back" and "go for it" without stopping) ----
: control-command ( -- )
    her-cmd @ dup no-gesture = if  drop exit  then
    her-fear-bits @ 2 and  her-mode @ thrown-mode = her-sub @ dup led-away-r = swap held-down-r = or and  or if
        drop  help-cmd -1 0 0 0 hewie-id send command   \ (a cry for help)
        $38 line  60 her-busy-t !  exit
    then
    her-mode @ own-mode <>  still? or if  drop exit  then
    command-code dup -1 = if  drop exit  then
    dup come-back-cmd = over go-for-it-cmd = or  her-sub @ 0<> and  over go-for-it-cmd = her-danger @ followed = and 0= and if   \ (on the move)
        mark-action-start  command-hewie  order-line  60 her-busy-t !
        plain-commands state-flag? 0= if  hewie-id her-look-who !  30 her-look-hold !  then   \ (a glance at him)
        exit
    then
    command-mode her-mode !  command-doing her-doing !  mark-action-start  ['] action-over her-act ! ;

\ ---- her lines on the gestures' key frames (Fiona_MotionSounds' event 1, commanding) ----
: call-anim? ( anim -- flag )   \ (the gestures that call to him)
    case
        look-around-anim of  true  endof  scold-near-anim of  true  endof  tend-start-anim of  true  endof
        pat-start-anim of  true  endof  rouse-gesture-anim of  true  endof  go-there-gesture-anim of  true  endof
        stay-gesture-anim of  true  endof  call-gesture-anim of  true  endof  go-for-it-gesture-anim of  true  endof
        >r false r>
    endcase ;
: command-sounds ( -- )
    her-mode @ command-mode <> if  exit  then
    motion-bits 1 and 0= if  exit  then
    anim@ rest-anim = if  $39 line exit  then
    anim@ call-anim? if  plain-commands state-flag? if  $33 line  else  call-line  then  then ;

\ ---- the gestures (Gesture_Update; Fiona_ReadsPad) ----
: gesture ( -- g )
    \ (the right stick - I up, K down, L right, J left - and R3, F: as hg's keys for them)
    key: I pressed? if  go-gesture exit  then  key: K pressed? if  back-gesture exit  then
    key: F pressed? if  stay-gesture exit  then  key: L pressed? if  praise-gesture exit  then
    key: J pressed? if  scold-gesture exit  then
    key: L held? if  praise-gesture exit  then  no-gesture ;   \ (held: as the stick held over - the praise's repeats)
: reads-pad? ( -- flag )
    hewie-commandable state-flag? 0=  fiona-occupied state-flag? or  her-busy-t @ 0<> or if  false exit  then
    her-mode @ dup command-mode = swap down-mode = or if  true exit  then
    her-mode @ own-mode = if  her-doing @ dup push-doing <> swap attack-doing <> and exit  then
    her-mode @ thrown-mode = if  her-sub @ dup led-away-r = swap held-down-r = or exit  then
    false ;
: read-command ( -- )
    her-busy-t @ if  -1 her-busy-t +!  then
    reads-pad? if  gesture  else  no-gesture  then  her-cmd ! ;
