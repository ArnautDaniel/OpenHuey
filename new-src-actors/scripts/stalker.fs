\ stalker.fs - the stalkers (docs/subsystems/stalker.md; src/game/pursuer.c, debilitas.c): one
\ actor per stalker in the game. The family's behaviour (`stalking`) and each one's over it
\ (Debilitas: `debilitas`). P1: in the house - placed, standing, growling, telling the danger
\ he is about.
IN: stalker
USING: engine game-state actors messages common facts flag-names room-names stalker.state ;

: me ( -- id )  self ;
: him ( -- a )  my-model @ actor ;   \ (his model's fields)
: my-at ( -- v )  self body-at ;
: my-room ( -- room )  self body-room ;
: played? ( -- flag )  in-game @ if  my-room room-id =  else  false  then ;   \ (Npc_InPlayedRoom)
: hard? ( -- flag )  progress pr.vars $27 + c@ 1 = ;
: danger-id ( -- id )  s" danger" actor-named ;

: play-cut ( anim -- )   \ Motion_Play: at once, with its table's flags
    my-model @ swap  2dup motion-entry nip nip  0 swap motion-play ;
\ his model where his body is, shown in the room being played
: pose ( -- )
    my-at sf@ him act.x sf!  my-at 4 + sf@ him act.y sf!  my-at 8 + sf@ him act.z sf!
    self body-yaw him act.yaw sf!
    played? 0<> 1 and him act.visible l! ;
: sound ( id bank -- )   \ Pursuer_Sound: at him (not under a scene, nor the world held)
    world-held state-flag? if  2drop exit  then  >r >r  my-at vec@  r> r>  bank-sound-at ;

\ ---- in the game and out (Pursuer_Activate: waiting about, his first animation) ----
: come-in ( room tri -- )
    dup tri-center  body-place
    -1 in-game !  3 my-mode !  0 growl-t !  0 doing !  0 my-move-mode !
    hp @ 0> 0= if  hp-max @ hp !  then
    0 play-cut  pose ;
: go-out ( -- )  0 in-game !  body-off  0 him act.visible l! ;

\ ---- his frame (Debilitas_Update: so far the growl, his model) and his part in the danger ----
: growl ( -- )   \ every 90 frames, standing on his own: sound $2A (bank 7)
    1 growl-t +!  growl-t @ 90 < if  exit  then
    0 growl-t !
    my-move-mode @ 0= doing @ $1300 <> and doing @ $1600 <> and if  $2A 7 sound  then ;
: frame ( -- )
    played? if  growl  then
    pose
    my-room  my-mode @ 0= 1 and  my-mode @ 0= 1 and  danger-id send stalker-here ;

behaviour stalking
  on stalker-in ( room tri -- )  come-in ;
  on stalker-out ( -- )  go-out ;
  on entered-room ( room exit -- )  2drop  in-game @ if  pose  then ;
  on tick ( -- )  in-game @ if  frame  then ;
end-behaviour

\ ---- Debilitas (debilitas.c Debilitas_Setup, DebilitasModel): 70 health (hard: 110), 5 across,
\ 20 tall; his model O_DB0/DB0_000, its motion table, his sounds (bank 7) ----
$2C020068 constant debilitas-floor   \ (Debilitas_BlockFlags: the triangles he can't stand on)
behaviour debilitas
  extends stalking
  on spawned ( -- )
      s" O_DB0/DB0_000" actor-load dup my-model !  $3D89A0 motion-table
      0 him act.visible l!
      7 s" O_DB0/DB0_000" sound-bank drop
      hard? if  110  else  70  then  dup hp-max !  hp !
      5e 20e body-size  debilitas-floor body-mask!  body-off  0 in-game !
      self subscribe tick  self subscribe entered-room ;
end-behaviour

: debilitas-spawn ( -- id )  debilitas stalker-state s" debilitas" spawn ;
