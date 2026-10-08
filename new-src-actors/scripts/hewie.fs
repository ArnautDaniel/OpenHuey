\ hewie.fs - Hewie, Fiona's dog (docs/subsystems/hewie.md). Phase H1: his body and model, his
\ own mind (the danger's moods, what he does next by his trust), keeping with Fiona, roaming,
\ sniffing, tricks, barking, his upkeep, his sounds and steps, and following her from room to
\ room (off screen by the doors).
\
\ He is spawned with the game but comes in only when told (`join-fiona`: the story, or the
\ console); `part-from-fiona` sends him off.
IN: hewie
USING: engine actors messages common paths hewie.state hewie.body hewie.model hewie.moving hewie.states hewie.offscreen hewie.actions hewie.mind ;

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
    her active? 0= if  exit  then
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
      self subscribe danger  self subscribe panic  self subscribe fiona-doing ;
  on join-fiona ( -- )  s" fiona" actor-named his-fiona !  room-id fiona-room !  join ;
  on part-from-fiona ( -- )  part ;
  on leaving-room ( room exit -- )   \ (the old room still in)
      2dup room-exit-leads drop fiona-room !  nip  along? if  fiona-left  else  drop  then ;
  on arrived ( room exit -- )  drop fiona-room ! ;   \ (he comes in by himself: Hewie_Arrive, the doors off screen)
  on danger ( level -- )  his-danger ! ;
  on panic ( stage level -- )  drop his-panic ! ;
  on fiona-doing ( mode sub cond -- )  fiona-cond !  fiona-sub !  fiona-mode ! ;
  on tick ( -- )  along? if  frame  then ;
end-behaviour

: hewie-spawn ( -- id )  hewie-own hewie-state s" hewie" spawn ;
