\ stalker.fs - the stalker in the game: Debilitas in character slot 2 (gCharPursuer), his
\ frame each tick (pursuer/*: the original's, src/game/pursuer.c and debilitas.c). The story
\ brings him in with its scripts (char-load: not yet); from the console:
\   debilitas-in      Debilitas in this room, 40 in front of Fiona
\   debilitas-out     sends him off
IN: stalker
USING: engine state game-state events.core chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves pursuer.target pursuer.frame pursuer.debilitas ;

variable stalker  -1 stalker !   \ his actor
: load-debilitas ( -- )
    stalker @ 0< 0= if  exit  then
    s" O_DB0/DB0_000" actor-load dup stalker !
    dup 0< if  drop exit  then
    $3D89A0 motion-table ;       \ (DebilitasModel_SecondaryMotion: his fades and flags)
: cast-stalker ( -- )   \ (the scripts' character record for slot 2)
    stalker @ p-char char.actor l!  2 p-char char.id l!  1 p-char char.present l!
    me fiona.core:pursuer-slot ! ;
create in-front 12 allot
: debilitas-in ( -- )
    load-debilitas  stalker @ 0< if  ." no Debilitas" cr exit  then
    cast-stalker  played p-room!
    in-front her c-pos her c-yaw 40e vec-ahead
    in-front me c-mask v-tri-in 0< if  in-front her c-pos vec-copy  then
    in-front vec@ me c-place!
    $C vcall  $5C vcall                    \ (Pursuer_Reset, Pursuer_Activate)
    me her c-pos c-heading-to p-yaw!
    1 stalker @ actor act.visible l! ;
: debilitas-out ( -- )
    stalker @ 0< if  exit  then
    0 stalker @ actor act.visible l!  0 p-char char.present l!  -1 fiona.core:pursuer-slot ! ;

\ ---- his frame (Progress' characters: scripted, his Think +0x44; else his Update +0x30) ----
: stalker-tick ( -- )
    playing @ paused @ 0= and 0= if  exit  then
    stalker @ 0< if  exit  then
    p-char char.present sl@ 0= if  exit  then
    p-scripted if  $44 vcall  else  $30 vcall  then ;
' stalker-tick on-tick
