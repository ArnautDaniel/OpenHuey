\ hewie/model.fs - Hewie's model: playing his animations, their groups, his poses (getting into
\ one, holding it), how he stands and walks for the danger and his mood, his voice and his bark.
IN: hewie.model
USING: engine actors common messages hewie-names hewie.state hewie.body hewie.anims hewie.sounds ;

: model ( -- n )  his-model @ ;
: him-model ( -- a )  model actor ;
: anim@ ( -- id )  model motion@ ;
\ Motion_PlayTable: with the fade and flags his motion table gives (anew: at its own pace)
: rate-1 ( -- )  1e him-model act.rate sf! ;
: entry ( anim -- blend flags )  model swap motion-entry nip ;   \ its table's fade and flags
: play ( anim -- )  rate-1  {: anim :}  model anim  anim entry  motion-play ;
: play-cut ( anim -- )  rate-1  {: anim :}  model anim 0  anim entry nip  motion-play ;   \ Motion_Play
: play-blend ( anim n -- )   \ Motion_PlayBlend: over n frames
    rate-1  {: anim n :}  model anim n  anim entry nip  motion-play ;
: play-blend8 ( anim n -- )   \ Motion_PlayBlend8: and no time of its own
    rate-1  {: anim n :}  model anim n  anim entry nip 8 or  motion-play ;
: play-if-not ( anim -- )  dup anim@ <> if  play  else  drop  then ;   \ Hewie_PlayIfNot
: play-again ( anim -- )  dup anim@ <> if  play  else  play-cut  then ;
: anim-done? ( -- flag )  him-model act.mflags l@ $20 and 0<> ;   \ came to its end this frame
: settled? ( -- flag )  him-model act.fade sf@ 0e f<= ;            \ faded in
: rate! ( F: r -- )  him-model act.rate sf! ;

\ Hewie_AnimGroup: the group of the animation he plays (hewie-group)
: anim-group ( -- g )
    anim@ case
        stand-anim of in-standing endof  alert-anim of in-standing endof  stance-anim of in-standing endof
        stance-upset-anim of in-standing endof  stand-hurt-anim of in-standing endof  crouch-anim of in-standing endof
        sit-anim of in-sitting endof
        lie-anim of in-lying endof  lie-hurt-anim of in-lying endof
        lie-low-anim of in-low endof
        up-from-sitting-anim of in-standing-up endof  up-from-lying-anim of in-standing-up endof
        yelp-anim of in-standing-up endof  get-up-anim of in-standing-up endof  scene-pose-anim of in-standing-up endof
        bark-anim of in-standing-up endof  bark-tense-anim of in-standing-up endof  bark-upset-anim of in-standing-up endof
        bark-limping-anim of in-standing-up endof  ears-up-anim of in-standing-up endof  $2212 of in-standing-up endof   \ ($2212: landed, held on)
        sit-down-anim of in-sitting-down endof  sit-from-lying-anim of in-sitting-down endof  up-from-low-anim of in-sitting-down endof
        bark-sitting-anim of in-sitting-down endof  sit-trick-2-anim of in-sitting-down endof  refuse-anim of in-sitting-down endof
        ears-down-anim of in-sitting-down endof  patted-anim of in-sitting-down endof  sit-trick-anim of in-sitting-down endof
        petted-start-anim of in-sitting-down endof  petted-anim of in-sitting-down endof  petted-end-anim of in-sitting-down endof
        lie-down-anim of in-lying-down endof  lie-from-sitting-anim of in-lying-down endof  roll-off-side-anim of in-lying-down endof
        bark-lying-anim of in-lying-down endof  sit-trick-lying-anim of in-lying-down endof
        get-low-anim of in-getting-low endof
        run-anim of in-running endof  trot-anim of in-trotting endof
        walk-anim of in-walking endof  dart-anim of in-walking endof  walk-wary-anim of in-walking endof  limp-anim of in-walking endof
        on-side-anim of in-on-side endof  roll-on-side-anim of in-rolling-over endof
        down-anim of in-down endof  go-down-anim of in-going-down endof  $2213 of in-going-down endof   \ ($2213: flung)
        >r in-other r>
    endcase ;
: basic? ( g -- flag )   \ standing, sitting, lying - or getting up, sitting or lying down
    dup in-low <  swap in-standing-up in-getting-low within or ;
: getting-into? ( g -- flag )  in-standing-up in-getting-low within ;

\ where he is to look (Hewie_TurnHead's modes: at his target - level; glances, slow or quick; looking
\ about; ahead; to a side, behind to a side, low, aside; held where a behaviour put it)
enum: look-mode  target-look slow-glance-look glance-look about-look ahead-look target-level-look
    left-look right-look held-look back-left-look back-right-look low-look aside-left-look aside-right-look ;
: look! ( mode -- )   \ taken up 10 frames later when it changes
    dup his-look-to @ <> if  his-look-to !  10 his-look-delay !  else  drop  then ;
: look-now! ( mode -- )  his-look-to !  0 his-look-delay ! ;

\ Hewie_StandAnim: his standing animation for the danger and his mood (blend -1: the table's)
: stand-still ( blend -- )
    his-danger @ chased = his-mood @ angry-mood = and if  stance-anim
    else his-cond @ hurt-cond = if  stand-hurt-anim
    else his-danger @ chased = his-mood @ upset-mood = and if  stance-upset-anim
    else his-danger @ calm <> his-alert @ 1 = and if  stance-anim   \ (alert this frame)
    else his-danger @ calm <> if  alert-anim
    else his-action @ face-target-act = his-action @ bark-at-target-act = or if  stance-anim
    else  stand-anim  then then then then then then
    {: blend anim :}  anim anim@ = if  exit  then
    blend -1 = if  anim play  else  anim blend play-blend  then ;
\ Hewie_WalkAnim: limping hurt, wary while followed, else walking
: walk-on ( -- )
    his-mood @ angry-mood = if  walk-anim
    else his-cond @ hurt-cond = if  limp-anim
    else his-mood @ upset-mood = if  walk-anim
    else his-danger @ followed = if  walk-wary-anim  else  walk-anim  then then then then  play-if-not ;

\ ---- poses: getting into one (Hewie_StepToPose) and holding it (Hewie_KeepPose) ----
\ the poses: standing, sitting, lying; at rest in one of those, or low too; as he is (still or
\ on the move); on his side; walking, trotting, running; down
enum: pose  stand-pose sit-pose lie-pose resting-pose resting-or-low-pose as-is-pose side-pose
    walk-pose trot-pose run-pose down-pose ;
: pose-leave ( g -- )   \ the way out of group g most poses share
    case
        in-low of  up-from-low-anim play  endof
        in-getting-low of  anim-done? if  up-from-low-anim play  then  endof
        in-running of  his-cond @ hurt-cond = if  limp-anim play-if-not  else  trot-anim play-if-not  then  endof
        in-trotting of  walk-on  endof
        in-walking of  -1 stand-still  endof
        in-other of  -1 stand-still  endof
        in-on-side of  roll-off-side-anim play  endof
        in-rolling-over of  anim-done? if  roll-off-side-anim play  then  endof
        in-down of  get-up-anim play  endof
        in-going-down of  anim-done? if  get-up-anim play  then  endof
    endcase ;
\ from a basic group, the animation into the pose (0: in it already)
: pose-basic ( g from-standing from-sitting from-lying -- 0 | -1 )
    {: g a0 a1 a2 :}
    g in-standing =  g in-standing-up = or if  a0
    else g in-sitting =  g in-sitting-down = or if  a1  else  a2  then then
    g in-standing-up >= anim-done? 0= and if  drop -1 exit  then   \ (getting into one: till it ends)
    dup 0= if  exit  then  play -1 ;
: step-to-pose ( pose -- 0 | -1 )   \ 0: in it
    settled? 0= if  drop -1 exit  then
    dup down-pose > if  drop -1 exit  then
    dup walk-pose run-pose 1+ within if  drop as-is-pose recurse 0= if  0  else  -1  then  exit  then
    anim-group                                                     ( pose g )
    swap case
        stand-pose of  dup basic? if  0 up-from-sitting-anim up-from-lying-anim pose-basic exit  then  endof
        sit-pose of  dup basic? if  sit-down-anim 0 sit-from-lying-anim pose-basic exit  then  endof
        lie-pose of  dup basic? if  lie-down-anim lie-from-sitting-anim 0 pose-basic exit  then  endof
        resting-pose of
            dup in-low < if  drop 0 exit  then
            dup getting-into? if  drop anim-done? if 0 else -1 then exit  then  endof
        resting-or-low-pose of
            dup in-standing-up < if  drop 0 exit  then
            dup getting-into? if  drop anim-done? if 0 else -1 then exit  then
            dup in-getting-low = if  drop anim-done? if  lie-low-anim play  then  -1 exit  then  endof
        as-is-pose of
            dup in-low <  over in-running in-rolling-over within or if  drop 0 exit  then
            dup getting-into? if  drop anim-done? if 0 else -1 then exit  then  endof
        side-pose of
            dup in-on-side = if  drop 0 exit  then
            dup in-rolling-over = if  drop anim-done? if 0 else -1 then exit  then
            drop lie-pose recurse 0= if  roll-on-side-anim play  then  -1 exit  endof
        down-pose of
            dup in-down = if  drop 0 exit  then
            dup in-going-down = if  drop anim-done? if 0 else -1 then exit  then
            dup basic? if  go-down-anim up-from-sitting-anim up-from-lying-anim pose-basic exit  then  endof
    endcase
    pose-leave -1 ;
: pose-walk ( anim -- )  his-cond @ hurt-cond = if  drop limp-anim  then  play-if-not ;
: pose-settle ( base -- )
    case
        in-standing of  -1 stand-still  endof
        in-sitting of  sit-anim play-if-not  endof
        in-lying of  his-cond @ hurt-cond = if  lie-hurt-anim  else  lie-anim  then  play-if-not  endof
    endcase ;
\ the animations between the basic poses, [to][from] (to 3: going down)
create pose-into
    0 ,                up-from-sitting-anim ,   up-from-lying-anim ,
    sit-down-anim ,    0 ,                      sit-from-lying-anim ,
    lie-down-anim ,    lie-from-sitting-anim ,  0 ,
    go-down-anim ,     up-from-sitting-anim ,   up-from-lying-anim ,
: keep-pose ( pose -- )
    settled? 0= if  drop exit  then
    anim-group {: pose g :}
    pose walk-pose = if
        g in-walking = if  true  else  as-is-pose step-to-pose 0=  then  if  walk-on  then  exit
    then
    pose trot-pose = if
        g in-walking =  g in-trotting = or if  true  else  as-is-pose step-to-pose 0=  then
        if  trot-anim pose-walk  then  exit
    then
    pose run-pose = if
        g in-walking =  g in-running = or if  true  else  as-is-pose step-to-pose 0=  then
        if  run-anim pose-walk  then  exit
    then
    pose down-pose > if  exit  then
    pose side-pose = if
        g in-rolling-over = if  anim-done? if  on-side-anim play  then  exit  then
        g in-on-side <> if  lie-pose step-to-pose 0= if  roll-on-side-anim play  then  then  exit
    then
    g basic? if
        g in-standing-up >= anim-done? 0= and if  exit  then
        g dup in-standing-up >= if  in-standing-up -  then  {: base :}   \ (the pose it is, or is getting into)
        pose resting-pose as-is-pose 1+ within if  base pose-settle exit  then
        pose down-pose = if  3  else  pose  then  3 * base + cells pose-into + @
        dup 0= if  drop base pose-settle  else  play  then  exit
    then
    pose resting-or-low-pose = g in-low = and if  exit  then
    pose resting-or-low-pose = g in-getting-low = and if  anim-done? if  lie-low-anim play  then  exit  then
    pose as-is-pose = g in-running in-on-side within and if
        g in-walking = if  walk-on  else  g in-running = if  run-anim  else  trot-anim  then  pose-walk  then  exit
    then
    pose down-pose = g in-down = and if  exit  then
    pose down-pose = g in-going-down = and if  anim-done? if  down-anim play  then  exit  then
    g pose-leave ;
\ the stance: once standing, upset when mistreated
: stance ( -- )  stand-pose step-to-pose 0= if  his-mood @ upset-mood = if  stance-upset-anim  else  stance-anim  then  play-if-not  then ;

\ ---- his voice (Hewie_MakeSound): not within 10 frames of the last (but after a whine); some
\ only after 40..60; his yelps heard far, his growls near ----
$80 constant far-noise   $1B constant near-noise   \ (how far the others hear him)
: his-sound ( id bank vol pitch -- ) ( F: x y z -- )   \ (Actor_PlaySound: not while hushed)
    his-hushed @ if  2drop 2drop fdrop fdrop fdrop exit  then  sound-at ;
: soon-after? ( snd t -- flag )  his-snd @ rot = his-snd-t @ rot < and ;
: whine? ( snd -- flag )  dup hurt-whimper =  over upset-whine = or  over ready-growl = or  swap stance-growl = or ;
: too-soon? ( snd -- flag )
    dup hurt-whimper = if  drop  hurt-whimper 40 soon-after? exit  then
    whine? if  upset-whine 50 soon-after?  ready-growl 60 soon-after? or  stance-growl 40 soon-after? or  else  false  then ;
: make-sound ( snd -- )
    his-snd @ whine? 0=  his-snd-t @ 10 < and if  drop exit  then
    dup shoved-yelp =  over yelp-sound = or if  far-noise his-tri noise-make  then
    dup growl-sound =  over upset-growl = or if  near-noise his-tri noise-make  then
    dup too-soon? if  drop exit  then
    dup 5 0 0 his-at vec@ his-sound  his-snd !  0 his-snd-t ! ;

\ Hewie_Bark: by his pose when calm (limping, lying, sitting, standing), else by his mood;
\ restarted when already barking
: bark ( -- )
    his-danger @ calm =  his-action @ bark-at-target-act <> and  his-action @ bark-at-her-act <> and if
        his-cond @ hurt-cond = if  bark-limping-anim play-again exit  then
        anim-group dup in-lying-down = over in-lying = or if  drop bark-lying-anim
        else dup in-sitting-down = swap in-sitting = or if  bark-sitting-anim  else  bark-anim  then then  play-again
    else
        his-mood @ upset-mood = if  bark-upset-anim  else  bark-tense-anim  then  play-again
    then ;
