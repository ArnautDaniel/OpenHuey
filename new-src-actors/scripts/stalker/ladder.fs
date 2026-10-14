\ stalker/ladder.fs - Fiona on a ladder in his room while he is after her (src/game/pursuer.c
\ Npc_OpenDoorFionaHides - for all its name, the ladder she is on -, Pursuer_DoorApproach,
\ Npc_PlanBesideDoor): he makes for beside its foot (else its top), out from it by his own
\ offsets (vtable +0x9C), and there stands facing her till she is off it; then after her again.
\ (Her ladder comes with `fiona-plight`.)
IN: stalker.ladder
USING: engine game-state actors messages common facts flag-names paths stalker.state stalker.senses stalker.moving stalker.search stalker.doorway ;

\ ---- out from each end (the foot, the top: x, z in its frame) by family ----
create pursuer-ladder    0e f, -6e f,       0.64e f, 10.4e f,      \ (Pursuer_DoorOffset: Lorenzo, ...)
create debilitas-ladder  0e f, -6.2115e f,  0e f, 10.4e f,         \ (Debilitas_DoorOffset)
create daniella-ladder   0e f, -6.9651e f,  0e f, 6.9767e f,       \ (Daniella_DoorOffset, her later kinds')
create riccardo-ladder   0e f, -5.3136e f,  0e f, 7.572e f,        \ (Riccardo's, kind 37's, the TintStalker's)
0 constant ladder-foot   1 constant ladder-top
: end-offset ( side -- ) ( F: -- ox oz )
    2* floats ladder-offsets @ +  dup f@  float+ f@ ;

\ ---- the step: to beside it, then by it ----
: off-ladder? ( -- flag )  her-ladder-on @ 0<  fiona here? 0= or ;
: ladder-over ( -- )  -1 at-ladder !  chase-begin ;
: by-the-ladder ( -- )
    root-move
    off-ladder? if  ladder-over exit  then
    fiona body-at turn-rate turn-to fdrop ;
: to-the-ladder ( -- )
    off-ladder? if  ladder-over exit  then
    walk-stride if  stand-anim play  ['] by-the-ladder step!  then ;
: beside ( link side -- ok? )   \ (his way planned to beside that end)
    dup end-offset nav-link-front aim-at ;
: to-ladder ( link -- )
    dup at-ladder !  0 search-phase !
    dup ladder-foot beside 0= if  dup ladder-top beside 0= if  drop -1 at-ladder !  exit  then  then  drop
    walk-fast play  ['] to-the-ladder step! ;

\ ---- each frame: she gets on one while he is after her and free ----
: ladder-watch ( -- )
    at-ladder @ 0< 0= her-ladder-on @ 0< or  my-mode @ 0<> or  if  exit  then
    reeling @ bitten @ or  leading @ or  at-dog @ or  out-going @ or  fiona here? 0= or if  exit  then
    her-ladder-on @ to-ladder ;
