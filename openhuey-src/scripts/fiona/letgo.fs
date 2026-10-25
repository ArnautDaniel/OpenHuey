\ fiona/letgo.fs - letting go of a thing (src/game/fiona.c Fiona_StateLetGoStart / StateLetGo;
\ Progress_PlayerButtons' triangle, SubScreen_CostumeModel / Costume - the equipment's slot 2):
\ the item there ($90..$9B) says how - thrown ($31: the ball, the doll), aimed ($32: the powders -
\ at the stalker if he is within 80 and 60 degrees of her facing, else ahead) or set down ($33:
\ the magnesias, the stone). Her animation ($D00 / $D01 / $D02), turning to her aim 5 degrees a
\ frame; at its key the thing leaves her hand (thrown 4 a frame, aimed 6, set down still) and one
\ of the item is used - if her hand isn't past a wall; then she stands again. Throwing, Hewie is
\ told (Fiona_HewieReact 12).
IN: fiona.letgo
USING: things items items.names engine vectors game-state actors messages common keys facts flag-names fiona.state fiona.anims fiona.model fiona.moving fiona.slam ;

: events-this-frame ( -- bits )  her-model @ 0 -1 1 motion-events ;   \ (her motion's key events this frame)

\ ---- the item: how it is let go, and the thing it becomes ----
: held ( -- item )  throw-slot equipped ;
: let-go-as ( item -- how kind | 0 -1 )
    case
        ball of  thrown-sub ball-thing  endof
        bisque-doll of  thrown-sub doll-thing  endof
        antimony-powder of  aimed-sub antimony-thing  endof
        refined-antimony of  aimed-sub refined-antimony-thing  endof
        antimony-tube of  aimed-sub tube-thing  endof
        magnesia of  set-down-sub magnesia-thing  endof
        refined-magnesia of  set-down-sub refined-magnesia-thing  endof
        prima-magnesia of  set-down-sub prima-thing  endof
        weeping-stone of  set-down-sub stone-thing  endof
        >r 0 -1 r>
    endcase ;

\ ---- the aim: the stalker within 80 and 60 degrees of her facing (an aimed one only) ----
create aim-at /vec allot   \ (aimed: where he stood as she began)
: aim ( -- )   \ (her-letgo-yaw: the way she turns to; aimed: at him)
    her-yaw f@ her-letgo-yaw f!  0 her-aimed !
    her-sub @ aimed-sub <> if  exit  then
    stalker-in-room dup 0< if  drop exit  then
    dup body-at her-at vec-dist 80e f< 0= if  drop exit  then
    aim-at over body-at vec-copy
    her-at swap body-at vec-heading  fdup her-yaw f@ f- angle-wrap fabs 60e deg>rad f< if
        her-letgo-yaw f!  -1 her-aimed !
    else  fdrop  then ;

\ ---- at the key: the thing made at her hand ----
create hand /vec allot   create step-at /vec allot
: speed ( -- ) ( F: -- s )  her-sub @ case  thrown-sub of 4e endof  aimed-sub of 6e endof  >r 0e r>  endcase ;
: way ( -- ) ( F: -- vx vy vz )   \ (at that speed, level: her facing - aimed, from her hand at where he stood)
    0e speed  her-aimed @ if  hand aim-at vec-heading  else  her-letgo-yaw f@  then  rotate-by  0e fswap ;
$20020008 constant hand-stops   \ (the floors her hand can't pass over)
: hand-reached ( -- tri | -1 )   \ (her hand not past a wall: from where she stands at its height)
    step-at her-at vec-copy  hand v.y sf@ step-at v.y sf!
    her-tri @ step-at hand hand-stops v-walk ;
: let-go ( -- )
    her-model @ hand-bone bone-pos hand vec!   \ (fiona_hand: the bone vtable +0x78 names)
    hand-reached dup 0< if  drop exit  then  {: tri :}
    hand vec@ {: F: x y z :}  way {: F: vx vy vz :}
    held let-go-as nip  tri  x f>cell y f>cell z f>cell  vx f>cell vy f>cell vz f>cell
    s" things" actor-named send thing-made
    held s" items" actor-named send take-item
    her-sub @ thrown-sub = if  throw-reaction s" hewie" actor-named send reaction  then ;

\ ---- her states ----
: letting-go ( -- )   \ (Fiona_StateLetGo)
    her-yaw f@ her-letgo-yaw f@ 5e deg>rad turn-toward fdrop her-yaw f!
    ended? if  to-idle exit  then
    events-this-frame 2 and  her-let-gone @ 0= and if  -1 her-let-gone !  let-go  then
    root-move ;
: let-go-start ( -- )   \ (Fiona_StateLetGoStart: once what she was doing has faded in)
    root-move  settled? 0= if  exit  then
    aim  0 her-let-gone !
    her-sub @ case  thrown-sub of throw-anim endof  aimed-sub of aimed-throw-anim endof  >r set-down-anim r>  endcase  -1 play-table
    ['] letting-go her-act ! ;
\ the triangle (Progress_PlayerButtons): free, a thing in slot 2 - not panicking (the threat at
\ 4: the buttons after the action button are skipped), nor under progress flag $14
: try-let-go ( -- )
    her-mode @ own-mode <>  her-doing @ free-doing <> or  no-flee state-flag? or  her-panic-stage @ panicking >= or if  exit  then
    triangle button-pressed? 0= if  exit  then
    held let-go-as {: how kind :}  how 0=  kind 0< or if  exit  then
    how her-sub !  throw-mode her-mode !  let-go-doing her-doing !  ['] let-go-start her-act ! ;
