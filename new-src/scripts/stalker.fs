\ stalker.fs - the stalker in the game: Debilitas in character slot 2 (gCharPursuer), his
\ frame each tick (pursuer/*: the original's, src/game/pursuer.c and debilitas.c). The story
\ brings him in with its scripts (char-load: not yet); from the console:
\   debilitas-in      Debilitas in this room, 40 in front of Fiona
\   debilitas-out     sends him off
IN: stalker
USING: engine state game-state events.core chars relations pursuer.core pursuer.stubs pursuer.npc pursuer.modes pursuer.steps pursuer.behave pursuer.search pursuer.moves pursuer.target pursuer.chase pursuer.debchase pursuer.react pursuer.debact pursuer.closein pursuer.attack pursuer.frame pursuer.debilitas ;

variable stalker  -1 stalker !   \ his actor
: load-debilitas ( -- )
    stalker @ 0< 0= if  exit  then
    s" O_DB0/DB0_000" actor-load dup stalker !
    dup 0< if  drop exit  then
    $3D89A0 motion-table ;       \ (DebilitasModel_SecondaryMotion: his fades and flags)
: cast-stalker ( -- )   \ (the scripts' character record for slot 2)
    stalker @ p-char char.actor l!  2 p-char char.id l!  1 p-char char.present l!
    me fiona.core:pursuer-slot ! ;
: stalker-mask ( -- )  $2C020068 me c-mask! ;   \ (Debilitas_BlockFlags)
\ a floor he may stand on near her: in front of her, else round her; 40 off, then 25
create in-front 12 allot  create try-d 40 , 25 ,
: spot-round-her ( -- found? )
    2 0 do
        8 0 do
            in-front her c-pos  her c-yaw i s>f 0.7853982e f* f+  j cells try-d + @ s>f  vec-ahead
            in-front me c-mask v-tri-in 0< 0= if  unloop unloop true exit  then
        loop
    loop  false ;
: place-it ( -- )
    in-front vec@ me c-place!
    $C vcall  $5C vcall                    \ (Pursuer_Reset, Pursuer_Activate)
    me her c-pos c-heading-to p-yaw!
    1 stalker @ actor act.visible l! ;
: debilitas-in ( -- )
    load-debilitas  stalker @ 0< if  ." no Debilitas" cr exit  then
    cast-stalker  played p-room!  stalker-mask
    spot-round-her 0= if  in-front her c-pos vec-copy  then
    place-it ;
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
