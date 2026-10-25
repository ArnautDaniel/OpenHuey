\ fiona/model.fs - Fiona's model: playing her animations, and which ones she plays to stand,
\ walk and run as she feels (Fiona_IdleAnim, Fiona_WalkLook, Fiona_RunLook). Her place on
\ the model and her body follow from her state.
IN: fiona.model
USING: engine actors vectors messages fiona.state fiona.anims ;

: her ( -- a )  her-model @ actor ;
: her-pos ( F: -- x y z )  her-at vec@ ;
: her-sound ( id vol -- )  5 swap 0 her-pos sound-at ;   \ (Actor_PlaySound: her bank, from where she is)
\ the model and the body where she is
: show-her ( -- )
    her-at v.x sf@ her act.x sf!  her-at v.y sf@ her act.y sf!  her-at v.z sf@ her act.z sf!
    her-yaw f@ her act.yaw sf!
    room-id her-tri @ her-pos body-place  her-yaw f@ body-turn ;

\ ---- her animations ----
: anim@ ( -- id )  her-model @ motion@ ;
\ her hand's bone (the model's +0x8BC, vtable +0x78 - set by each costume's model class as it loads:
\ EventHumanModel_Loaded, FionaModel_Loaded, Costume2..8Model_Loaded)
: hand-bone ( -- bone )
    her-costume @ case  0 of $2E endof  1 of $42 endof  2 of $3D endof  3 of $39 endof
        6 of $21 endof  7 of $23 endof  8 of $1A endof  >r $42 r>  endcase ;
\ Motion_Start: the animation (with a variant: -1 none), blended over `blend` frames, with flags
\ (1 loops, 8 no time of its own); one her model lacks: nothing
: start ( anim variant blend flags -- )
    {: anim variant blend flags :}
    her-model @ anim has-motion? 0= if  exit  then
    1e her act.rate sf!
    her-model @ anim blend flags motion-play
    variant her-variant !  her-model @ variant motion-variant ;
: entry ( anim -- blend flags )  her-model @ swap motion-entry nip ;   \ its table's fade and flags
: play-table ( anim variant -- )  over entry start ;                 \ Motion_PlayTable
: play-own ( anim -- )  -1 over entry 8 or start ;   \ (Motion_PlayOwnBlend: no variant, no time of its own)
: play-cut ( anim -- )  {: anim :}  anim -1 0  anim entry nip  start ;   \ (Motion_Play: cut in, no blend)
: play-blend ( anim n variant -- )  {: anim n variant :}  anim variant n  anim entry nip  start ;   \ blended over n frames
: weight! ( F: w -- )  her act.vweight sf! ;      \ the animation against its variant
: settled? ( -- flag )  her act.fade sf@ 0e f<= ;  \ faded in
: ended? ( -- flag )  her act.mflags l@ $20 and 0<> ;   \ came to its end this frame

\ the group an animation belongs to (Fiona_AnimGroup)
enum: anim-group  standing-group walking-group running-group breathless-group lying-group resting-group pushing-group climbing-group hanging-group
    turning-to-flee-group kicking-group other-group ;
: anim-group ( anim -- g )
    case
        stand-anim of standing-group endof  attack-anim of standing-group endof  3 of standing-group endof
        4 of standing-group endof  chased-stand-anim of standing-group endof
        rest-anim of resting-group endof
        walk-anim of walking-group endof  walk-shaken-anim of walking-group endof  walk-scared-anim of walking-group endof
        walk-chased-anim of walking-group endof
        turn-left-anim of walking-group endof  turn-right-anim of walking-group endof  turn-round-anim of walking-group endof
        run-anim of running-group endof  run-shaken-anim of running-group endof  run-scared-anim of running-group endof
        panic-run-anim of running-group endof
        out-of-breath-anim of breathless-group endof  lying-anim of lying-group endof
        lean-anim of pushing-group endof  push-anim of pushing-group endof  let-go-push-anim of pushing-group endof  strain-anim of pushing-group endof
        on-ladder-top-anim of climbing-group endof  rung-down-anim of climbing-group endof  rung-down-end-anim of climbing-group endof
        off-ladder-foot-anim of climbing-group endof  on-ladder-foot-anim of climbing-group endof  rung-up-anim of climbing-group endof
        rung-up-end-anim of climbing-group endof  off-ladder-top-anim of climbing-group endof
        ladder-hold-anim of hanging-group endof  ladder-hold-2-anim of hanging-group endof
        flee-turn-anim of turning-to-flee-group endof  kick-anim of kicking-group endof
        >r other-group r>
    endcase ;
: group ( -- g )  anim@ anim-group ;

\ ---- her looks: how she stands, walks and runs as she feels ----
: idle-base ( -- anim )  her-danger @ chased = if  chased-stand-anim  else  stand-anim  then ;
: weight-moved? ( F: w -- flag )  her-blend-w f@ f- fabs 0.1e f> ;
\ how she feels: her fear's ease - (100 - fear) / 60; her recovery's - (1800 - its count) / 1800
: fear-ease ( F: -- a )  100e her-fear f@ f- 60e f/ ;
: recovery-ease ( F: -- b )  1800 her-recovery @ - s>f 1800e f/ ;
\ the variants of her idle and moves: shaken (by her fear), scared (by her recovery), out of breath
2 constant shaken-variant   3 constant scared-variant   4 constant breathless-variant
: idle-play ( anim blend variant -- )  over -1 = if  nip play-table  else  play-blend  then ;
: playing? ( anim variant -- flag )  her-variant @ = swap anim@ = and ;
\ Fiona_IdleAnim: out of breath (variant 4, its weight eased toward the panic), shaken (2, by
\ her fear) or frightened (3, by her recovery); weight 1 is her plain idle
: idle-anim ( blend -- )
    {: blend :}
    her-mode @ own-mode = if  0 her-sub !  then
    her-fear-bits @ 1 and if
        idle-base breathless-variant playing? 0= if  idle-base blend breathless-variant idle-play  then
        90e her-panic-level f@ f- 15e f/ 0e fmax  {: F: t :}            \ (the weight eased toward the panic)
        her-tired-w f@ t f< if  her-tired-w f@ 0.05e f+ t fmin  else  her-tired-w f@ 0.05e f- t fmax  then
        fdup her-tired-w f!  weight! exit
    then
    fear-ease recovery-ease  {: F: a b :}
    b 0.5e f< 0=  b a 0.25e f+ f>= and if
        a 1e f< if
            idle-base shaken-variant playing? 0=  a weight-moved? or if  idle-base blend shaken-variant idle-play  then
            a weight!  a her-blend-w f!
        else
            idle-base -1 playing? 0= if  idle-base blend -1 idle-play  then
            1e weight!
        then  exit
    then
    idle-base scared-variant playing? 0=  b weight-moved? or if  idle-base blend scared-variant idle-play  then
    b weight!  b her-blend-w f! ;

\ Fiona_WalkLook / Fiona_RunLook: the walk (0x200; chased 0x208) or run (0x202) alone, or blended
\ with its frightened (0x204 / 0x205) or shaken (0x201 / 0x203) variant; panicking, the panic
\ run 0x206
: look-blend ( base variant -- ) ( F: w -- )   \ the move blended with a variant (anew if the weight moved on)
    {: base variant :}
    base variant playing? 0=  fdup weight-moved? or if  base variant play-table  then
    fdup weight!  her-blend-w f! ;
: move-look ( base scared shaken -- )
    {: base scared shaken :}  fear-ease recovery-ease  {: F: a b :}
    b 0.5e f<  b a 0.25e f+ f< or if  b base scared look-blend exit  then
    a 1e f< if  a base shaken look-blend exit  then
    base -1 playing? 0= if  base -1 play-table  then  1e weight! ;
\ the walk and the run (their subs: her moving on her own); the variants are their own animations
1 constant walk-sub   2 constant run-sub
: walk-look ( -- )
    her-mode @ own-mode = if  walk-sub her-sub !  then
    her-danger @ chased = if  walk-chased-anim  else  walk-anim  then  walk-scared-anim walk-shaken-anim  move-look ;
: run-look ( -- )
    her-mode @ own-mode = if  run-sub her-sub !  then
    her-fear-bits @ 2 and if  anim@ panic-run-anim <> if  panic-run-anim -1 play-table  1e weight!  then  exit  then
    run-anim run-scared-anim run-shaken-anim move-look ;
: stand ( -- )  0 her-rest !  -1 idle-anim ;   \ Fiona_Stand
