\ hewie.fs - Hewie, Fiona's dog (docs/subsystems/hewie.md). Phase H1: his body and model, his
\ own mind (the danger's moods, what he does next by his trust), keeping with Fiona, roaming,
\ sniffing, tricks, barking, his upkeep, his sounds and steps, and following her from room to
\ room (off screen by the doors).
\
\ He is spawned with the game but comes in only when told (`join-fiona`: the story, or the
\ console); `part-from-fiona` sends him off.
IN: hewie
USING: engine actors messages common paths hewie.state hewie.body hewie.model hewie.moving hewie.states hewie.offscreen hewie.actions hewie.commands hewie.mind hewie.scripted ;

\ at heel: behind her and a little to her side (where she is when that isn't floor), her way
create heel-at 12 allot
: heel ( -- )
    fiona-at vec@                                                    ( F: x y z )
    her body-yaw fcos -14e f*  her body-yaw fsin -8e f* f+ f+  frot
    her body-yaw fsin -14e f*  her body-yaw fcos 8e f* f+ f+  frot frot heel-at vec!
    heel-at his-floor v-tri-in dup 0< if  drop  heel-at fiona-at vec-copy  her body-tri  then
    >r  heel-at vec@ room-id r> body-place  her body-yaw his-yaw! ;
: along? ( -- flag )  self body? ;   \ in the game (his body in the house)
: join ( -- )
    her active? 0= if  -1 his-joining !  exit  then   \ (she isn't in yet: when she arrives)
    0 his-joining !
    1 him-model act.visible l!  heel  0 away!  fresh ;
: part ( -- )
    body-off  0 him-model act.visible l!  model 0 dog-legs ;

behaviour hewie-own
  on spawned ( -- )
      s" O_HEW/HEW_000" actor-load dup his-model !  $3D5F90 motion-table   \ (his motion table: fades and flags)
      0 him-model act.visible l!
      2.5e 5e body-size  his-floor body-mask!  body-off
      s" fiona" actor-named his-fiona !
      self subscribe tick  self subscribe leaving-room  self subscribe arrived
      self subscribe danger  self subscribe panic  self subscribe fiona-doing  self subscribe frame-end ;
  on join-fiona ( -- )  s" fiona" actor-named his-fiona !  room-id fiona-room !  join ;
  on part-from-fiona ( -- )  part ;
  on place ( x y z yaw -- )   \ (the story puts him)
      along? 0= if  2drop 2drop exit  then
      >r >r >r cell>f r> cell>f r> cell>f  his-place  r> cell>f his-yaw! ;
  on leaving-room ( room exit -- )   \ (the old room still in)
      2dup room-exit-leads drop fiona-room !  nip  along? if  fiona-left  else  drop  then ;
  on arrived ( room exit -- )  drop fiona-room !
      his-joining @ if  s" fiona" actor-named his-fiona !  join  then ;   \ (he comes in by himself: Hewie_Arrive, the doors off screen)
  on danger ( level -- )  his-danger ! ;
  on panic ( stage level -- )  drop his-panic ! ;
  on fiona-doing ( mode sub cond cmd -- )  fiona-cmd !  fiona-cond !  fiona-sub !  fiona-mode ! ;
  on tick ( -- )  along? if  frame  else  tell-doing  then ;
  \ her commands (H2)
  on command ( code tri yaw x z -- )   \ (Hewie_StateBlock's 13)
      along? 0= if  2drop 2drop drop exit  then
      >r >r cell>f  r> cell>f  r> cell>f                       ( code tri ) ( F: yaw x z )
      over $23 = if  showed  else  drop fdrop fdrop fdrop  then
      fiona-command drop  0 his-broke !  tell-doing ;
  on reaction ( n -- )  along? if  react  else  drop  then ;
  on meet-me ( type -- )
      along? if  meet-ask  else  drop -1  then
      dup 0< if  drop  sender send meet-refused exit  then
      fs-at sf@ f>cell  fs-at 4 + sf@ f>cell  fs-at 8 + sf@ f>cell  fs-yaw f@ pi f+ angle-wrap f>cell
      sender send meet-at  tell-doing ;   \ (what he does now, before her next frame)
  on meet-now ( type -- )
      along? if  meet-second?  else  drop false  then
      if  sender send meet-on  tell-doing  else  sender send meet-refused  then ;
  on meet-off ( -- )  -1 his-meet ! ;
  \ the story's moves (H4: Hewie_Requests)
  on scripted ( on -- )  along? if  his-busy !  else  drop  then ;
  on scripted-move ( kind a b x y z yaw -- )
      along? 0= if  2drop 2drop 2drop drop exit  then
      cell>f his-to-yaw f!  >r >r cell>f r> cell>f r> cell>f his-to vec!
      his-to-anim !  his-to-tri !  0 his-done !
      dup 12 = if  drop his-to-tri @ his-look-char !  0 his-look-pt? !  done! exit  then
      dup 13 = if  drop his-look-pt his-to vec-copy  1 his-look-pt? !  done! exit  then
      dup 1 = if  full-stop  then
      move-action dup 0< if  drop done! exit  then  0 want ;
  on hold-anim ( anim blend -- )  along? 0= if  2drop exit  then  his-to-anim !  his-to-tri !  $3C 0 want ;
  on show ( on -- )  along? 0= if  drop exit  then  0<> 1 and him-model act.visible l! ;
  on frame-end ( -- )  along? if  his-done @  anim-done?  broadcast moving  then ;
end-behaviour

: hewie-spawn ( -- id )  hewie-own hewie-state s" hewie" spawn ;
