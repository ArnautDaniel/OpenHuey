\ fiona/letgo.fs - letting go of a thing (src/game/fiona.c Fiona_StateLetGoStart / StateLetGo;
\ Progress_PlayerButtons' triangle, SubScreen_CostumeModel / Costume - the equipment's slot 2):
\ the item there ($90..$9B) says how - thrown ($31: the ball, the doll), aimed ($32: the powders -
\ at the stalker if he is within 80 and 60 degrees of her facing, else ahead) or set down ($33:
\ the magnesias, the stone). Her animation ($D00 / $D01 / $D02), turning to her aim 5 degrees a
\ frame; at its key the thing leaves her hand (thrown 4 a frame, aimed 6, set down still) and one
\ of the item is used - if her hand isn't past a wall; then she stands again. Throwing, Hewie is
\ told (Fiona_HewieReact 12).
IN: fiona.letgo
USING: engine game-state actors messages common keys facts flag-names fiona.state fiona.model fiona.moving fiona.slam ;

defer to-idle   ' noop is to-idle      \ (fiona.fs)
: events ( -- bits )  her-model @ 0 -1 1 motion-events ;   \ (her motion's key events this frame)

\ ---- the item: how it is let go, and the thing it becomes ----
: held ( -- item )  progress pr.equipped 8 + sl@ ;   \ (the equipment's slot 2)
create hows   $31 c, $32 c, $33 c, $31 c, $32 c, $33 c, 0 c, $32 c, $33 c, 0 c, 0 c, $33 c,
create kinds   0 c, 1 c, 2 c, 3 c, 4 c, 5 c, -1 c, 6 c, 7 c, -1 c, -1 c, 8 c,
: slot ( item -- k | -1 )  $90 - dup 12 u< 0= if  drop -1  then ;
: how ( item -- how | 0 )  slot dup 0< if  drop 0  else  hows + c@  then ;
: thing-kind ( item -- kind )  slot dup 0< if  exit  then  kinds + c@ dup $FF = if  drop -1  then ;

\ ---- the aim: the stalker within 80 and 60 degrees of her facing (an aimed one only) ----
create aim-at 12 allot   \ (aimed: where he stood as she began)
: aim ( -- )   \ (her-letgo-yaw: the way she turns to; aimed: at him)
    her-yaw f@ her-letgo-yaw f!  0 her-aimed !
    her-sub @ $32 <> if  exit  then
    stalker-in-room dup 0< if  drop exit  then
    dup body-at her-at vec-dist 80e f< 0= if  drop exit  then
    aim-at over body-at vec-copy
    her-at swap body-at vec-heading  fdup her-yaw f@ f- angle-wrap fabs 60e deg>rad f< if
        her-letgo-yaw f!  -1 her-aimed !
    else  fdrop  then ;

\ ---- at the key: the thing made at her hand ----
create hand 12 allot   create step-at 12 allot
: speed ( -- ) ( F: -- s )  her-sub @ case  $31 of 4e endof  $32 of 6e endof  >r 0e r>  endcase ;
: way ( -- ) ( F: -- vx vy vz )   \ (at that speed, level: her facing - aimed, from her hand at where he stood)
    0e speed  her-aimed @ if  hand aim-at vec-heading  else  her-letgo-yaw f@  then  rotate-by  0e fswap ;
: hand-reached ( -- tri | -1 )   \ (her hand not past a wall: from where she stands at its height)
    step-at her-at vec-copy  hand 4 + sf@ step-at 4 + sf!
    her-tri @ step-at hand $20020008 v-walk ;
: let-go ( -- )
    her-model @ 9 bone-pos hand vec!
    hand-reached dup 0< if  drop exit  then                  ( tri )
    held thing-kind swap  hand vec@ f>cell >r f>cell >r f>cell  r> r>   ( kind tri x y z )
    way f>cell >r f>cell >r f>cell  r> r>
    s" things" actor-named send thing-made
    held s" items" actor-named send take-item
    her-sub @ $31 = if  12 s" hewie" actor-named send reaction  then ;

\ ---- her states ----
: letting-go ( -- )   \ (Fiona_StateLetGo)
    her-yaw f@ her-letgo-yaw f@ 5e deg>rad turn-toward fdrop her-yaw f!
    ended? if  to-idle exit  then
    events 2 and  her-let-gone @ 0= and if  -1 her-let-gone !  let-go  then
    root-move ;
: let-go-start ( -- )   \ (Fiona_StateLetGoStart: once what she was doing has faded in)
    root-move  settled? 0= if  exit  then
    aim  0 her-let-gone !
    her-sub @ case  $31 of $D00 endof  $32 of $D01 endof  >r $D02 r>  endcase  -1 play-table
    ['] letting-go her-act ! ;
\ the triangle (Progress_PlayerButtons): free, a thing in slot 2 (not under progress flag $14)
: try-let-go ( -- )
    her-mode @ 0<>  her-doing @ 0<> or  no-flee state-flag? or if  exit  then
    triangle button-pressed? 0= if  exit  then
    held how ?dup 0= if  exit  then
    held thing-kind 0< if  drop exit  then
    her-sub !  $E her-mode !  7 her-doing !  ['] let-go-start her-act ! ;
