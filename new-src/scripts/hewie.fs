\ hewie.fs - Hewie in the game: his model, put by Fiona's side as a room is entered, and his
\ mind each frame (partner.brain - the original's, src/game/hewie.c). Fiona's commands to him
\ and her side of the joint actions are hers (fiona.commands).
IN: hewie
USING: engine state game-state events.core chars partner.core partner.tables partner.moves partner.route partner.states partner.offscreen partner.actions partner.brain ;

variable hewie        -1 hewie !
variable hewie-room   -1 hewie-room !
variable hewie-ready  \ his motion table set

\ his animations the scripts' moves use (events/play.fs)
$201 constant h-walk
$202 constant h-run
$0 constant h-stand

\ a character's place and triangle from its model (Fiona's moves are still player.fs's own)
: from-actor ( cs -- )
    dup c-actor dup 0< if  2drop exit  then  actor >r
    r@ act.x sf@ r@ act.y sf@ r> act.z sf@  dup c-pos vec!
    dup c-find-tri swap c-tri! ;

\ put him at heel: behind her and a little to her side (or where she is when that isn't floor)
create heel-at 12 allot
: heel ( -- )
    her c-pos vec@                                                ( F: x y z )
    her c-yaw fcos -14e f*  her c-yaw fsin -8e f* f+ f+  frot
    her c-yaw fsin -14e f*  her c-yaw fcos 8e f* f+ f+  frot frot heel-at vec!
    heel-at him c-mask v-tri-in 0< if  heel-at her c-pos vec-copy  then
    heel-at vec@ him c-place!  her c-yaw h-yaw!
    played-room hewie-room ! ;

: spawn-hewie ( -- )
    s" O_HEW/HEW_000" actor-load dup hewie !
    dup 0< if  drop exit  then
    dup $3D5F90 motion-table   \ (his motion table: fades and flags)
    1 character char.actor l! ;

\ ---- the scripts' commands and conditions on him (src/game/event.c EventCmd_Hewie) ----
\ Hewie_SetAnim: his yelp's time (unless negative) and animation
: set-anim ( a anim -- )  h-yelp-anim !  dup 0< if  drop  else  h-yelp !  then ;
variable room-side  -1 room-side !   \ +0xF3668: which side of a divided room he is on
:noname ( op a b -- )
    rot case
        $39 of  set-action  endof
        $63 of  drop s>f deg>rad angle-wrap h-to-yaw f!  $72 0 set-action  endof
        $77 of  set-anim  endof
        $85 of  drop add-trust  endof
        $BB of  drop  h-room played-room = if  5 h-wait !  0<> h-stay !  else  drop  then  endof
        $BD of  2drop  300 h-pet-time !  endof
        $C4 of  drop -1 set-mode  endof
        $D0 of  drop dup 3 u< 0= if  drop -1  then  room-side !  endof
        $D7 of  drop play-cut  endof
        >r 2drop r>
    endcase ; is event-hewie
:noname ( op a -- flag )
    swap case
        $0F of  drop  her with? h-mood @ 3 <> and can-command? and dup if
                    drop  her c-tri her c-pos plan-to dup if  drop rest 150e f<=  then
                then  endof
        $24 of  h-action @ =  endof
        $31 of  drop 0 may-break-off 0=  endof
        $33 of  room-side @ =  endof
        $42 of  drop h-waiting @ 0<>  endof
        $43 of  h-mood @ =  endof
        $5A of  drop fiona-reachable?  endof
        $61 of  h-trust @ =  endof
        >r drop false r>
    endcase ; is event-hewie?
:noname ( F: x y z -- )   \ (only when he isn't already looking at something)
    h-look @ if  fdrop fdrop fdrop exit  then  1 h-look !  h-scent vec! ; is event-hewie-look

\ she leaves the room: he decides how to follow (Hewie_Vt34)
:noname ( exit -- )  hewie-along @ hewie-ready @ and if  fiona-left  else  drop  then ; is char-leaves

\ ---- each frame ----
:noname ( -- flag )  can-command? ; is event-hewie-can-command?
: hewie-tick ( -- )
    playing @ hewie-along @ and paused @ 0= and 0= if  exit  then
    hewie @ 0< if  spawn-hewie  hewie @ 0< if  exit  then  then
    hewie-ready @ 0= if
        hewie @ dup $3D5F90 motion-table  1 character char.actor l!  -1 hewie-ready !
        1 2.5e 5e c-size!          \ (Hewie_Reset: radius 2.5, height 5)
        -1 1 cells own-moves + !   \ (his moves are his own: Hewie_Requests)
    then
    played-room her character char.room l!
    her character char.scripted sl@ if  her from-actor  then   \ (a script walks her: her place by her model)
    hewie-room @ 0< if   \ (brought in: at her heel, in her room)
        played-room room!  heel  hewie-start  played-room hewie-room !
    then
    h-disabled? 0= 1 and hewie @ actor act.visible l!
    hewie @ h-disabled? 0= dog-legs   \ (his feet planted on the floor while he is here)
    hewie-frame ;
' hewie-tick on-tick

\ ---- from the console: bring him in (or send him off) when the story hasn't yet ----
\   s" hewie" summon      s" hewie" dismiss
: hewie-in ( -- )
    -1 hewie-along !  -1 hewie-room !            \ (placed at her heel and started next frame)
    hewie @ 0< 0= if  1 hewie @ actor act.visible l!  then ;
: hewie-out ( -- )
    0 hewie-along !  hewie @ 0< 0= if  0 hewie @ actor act.visible l!  then ;
: summon ( addr len -- )   s" hewie" compare 0= if  hewie-in  else  ." summon: only hewie for now" cr  then ;
: dismiss ( addr len -- )  s" hewie" compare 0= if  hewie-out  else  ." dismiss: only hewie for now" cr  then ;

\ ---- from the console: send him somewhere, through the house if need be ----
\   10e 0e 20e $13 hewie-goto     the point (x y z) in room 0x13
\ In his room he walks there (action 0x3F). For another room the game's route planner gives
\ the doors: he walks to the first one where you can see him and goes through, then makes his
\ way on off screen (action 0x33, as to a noise he heard); in the room being played he walks
\ to the point. (Left in another room, his own mind soon sends him back to Fiona.)
create goal 12 allot  variable goal-room  -1 goal-room !
variable goal-stage   \ 0 none, 1 walking to the door (exit goal-exit), 2 on the way, 3 to the point
variable goal-exit
: walk-to ( v -- )   \ his action 0x3F to point v of the room being played
    h-to swap vec-copy  h-to him c-mask v-tri-in dup 0< if  drop h-to v-tri  then  h-to-tri !
    -1 h-to-anim !  0e h-to-yaw f!  $3F 0 set-action ;
create door-at 12 allot
: hewie-goto ( room -- ) ( F: x y z -- )
    goal vec!  goal-room !  0 goal-stage !
    hewie-along @ 0= if  ." hewie isn't here (summon him first)" cr exit  then
    goal-room @ h-room = if
        h-disabled? if  ." he is in that room already" cr  else  goal walk-to  then  exit
    then
    h-room goal-room @ h-avoid find-route 0> 0= if  ." no way there for him" cr exit  then
    h-disabled? if  goal-room @ h-noise-room !  2 goal-stage !  $33 0 set-action exit  then
    route @ h-room door-exit dup goal-exit !
    1 exit-spot drop door-at vec!  door-at walk-to  1 goal-stage ! ;
: goal-tick ( -- )
    goal-stage @ case
        1 of  h-action @ $3F = h-action @ $64 = or 0= if   \ at the door (or stopped): through it
                  goal-exit @ through-exit drop
                  h-room goal-room @ = if  0 goal-stage !
                  else  goal-room @ h-noise-room !  2 goal-stage !  $33 0 set-action  then
              then  endof
        2 of  h-room goal-room @ = h-disabled? 0= and if  3 goal-stage !  goal walk-to  then
              h-room goal-room @ = h-disabled? and if  0 goal-stage !  then  endof
        3 of  0 goal-stage !  endof
    endcase ;
' goal-tick on-tick
