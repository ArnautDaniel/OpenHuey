\ creatures.fs - the house's lesser creatures (docs/subsystems/creatures.md; src/game/creature.c
\ Creatures_*, scene_game.c SceneGame_PlaceCreature): ten slots - 0..6 spirits, 7..9 homunculi
\ - each creature an actor of its own. This actor places them for the story,
\ clears them, and after a loaded game brings them back from the progress, where each keeps
\ itself.
IN: creatures
USING: engine game-state actors messages common facts flag-names doors creatures.state creatures.spirit creatures.homunculus ;


state: creatures-state
  #creatures cells field slots      \ each slot's actor (-1: free)
  cell field st-room  cell field st-age     \ the stalker's room, as he tells (stalker-doing), frames since
  cell field scatter  cell field scatter-other   \ the story's count: how many, of which kind (homunculus-kind: homunculi)
end-state

: slot@ ( i -- id )  cells slots + @ ;
: slot! ( id i -- )  cells slots + ! ;
: free? ( i -- flag )  slot@ alive? 0= ;
: first-free ( from to -- i | -1 )  swap ?do  i free? if  i unloop exit  then  loop  -1 ;
: record ( i -- addr )  /kept-creature *  progress pr.creatures + ;
: clear-slot ( i -- )   \ its creature gone, its record cleared
    dup slot@ dup alive? if  kill  else  drop  then
    -1 over slot!  record /kept-creature 0 fill ;
: clear-all ( -- )  #creatures 0 do  i clear-slot  loop ;

: spirit-in ( i -- id )  spirit-spawn  tuck swap slot! ;
: homunculus-in ( i -- id )  homunculus-spawn  tuck swap slot! ;
: place ( room level tri kind reach which yaw height -- )   \ (SceneGame_PlaceCreature) a homunculus (kind
    {: room level tri kind reach which yaw height | i :}       \ bit 7) in a free slot 7..9; a spirit in a free
    room room-id = 0= if  -1 to tri  then                          \ slot 0..5 (which -1), else the story's own
    kind homunculus-kind and if
        first-homunculus #creatures first-free to i  i 0< if  exit  then
        i  room level tri kind reach yaw height  i homunculus-in send homunculus-setup  exit
    then
    which -1 = if  0 story-spirit first-free  else  story-spirit  dup clear-slot  then  to i
    i 0< if  exit  then
    i  room level tri kind reach  i spirit-in send spirit-setup ;
: from-progress ( -- )   \ a loaded game: each kept spirit back
    #creatures 0 do
        i slot@ dup alive? if  kill  else  drop  then  -1 i slot!
        i record kc.used l@ if
            i first-homunculus < if  i  i spirit-in send spirit-kept  else  i  i homunculus-in send homunculus-kept  then
        then
    loop ;

enum: which-creatures  all-creatures spirits-only homunculi-only ;   \ (creatures-clear's)
5 constant scattered-after-load

\ ---- the story's count (0x99, SceneGame_RoomCreatures): as each room comes in, each of the
\ first `scatter` slots that is free gets a new spirit in a room next to hers (within two open
\ doors, SceneGame_RoomsNear; a room more likely the more of her exits lead to it), sent after
\ her. (The homunculi's kind no room's script asks for: not built) ----
65 constant most-rooms
create near-rooms most-rooms cells allot  create next-rooms most-rooms cells allot  variable next-n
: next-room+ ( room -- )  next-n @ most-rooms < if  next-n @ cells next-rooms + !  1 next-n +!  else  drop  then ;
: rooms-next-to-hers ( -- n )   \ (once for each of her exits that leads there)
    0 next-n !
    room-id near-rooms most-rooms rooms-near 0 ?do
        i cells near-rooms + @  dup room-id = if  drop
        else  8 0 do  room-id i room-exit-leads drop  over = if  dup next-room+  then  loop  drop  then
    loop  next-n @ ;
: figure-here? ( addr len -- flag )   \ (a figure of the story in her room)
    actor-named  dup 0< if  drop false exit  then  dup body? if  body-room room-id =  else  drop false  then ;
: someone-up-in-her-room? ( -- flag )   \ (the stalker or a figure of the story, slots 2..5)
    st-age @ 2 <  st-room @ room-id = and
    s" cast3" figure-here? or  s" cast4" figure-here? or  s" cast5" figure-here? or ;
: scatter-about ( -- )
    scatter-other @ homunculus-kind and if  exit  then
    scatter @ story-spirit min 0 ?do
        i free? if
            rooms-next-to-hers ?dup if
                random cells next-rooms + @                                  ( room )
                i swap someone-up-in-her-room?  i spirit-in send spirit-sent
            then
        then
    loop ;

behaviour keeping-creatures
  on spawned ( -- )  slots #creatures cells -1 fill  self subscribe game-loaded  self subscribe entered-room
      self subscribe stalker-doing  self subscribe tick  100 st-age ! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )  2drop 2drop 2drop 2drop  st-room !  0 st-age ! ;
  on tick ( -- )  st-age @ 100 < if  1 st-age +!  then ;
  on entered-room ( room exit -- )  2drop  creatures-on state-flag? if  scatter-about  then ;
  on creature-place ( room level tri kind reach which yaw height -- )  place ;
  on creatures-clear ( which -- )   \ all-creatures, the spirits, the homunculi
      case  spirits-only of  first-homunculus 0  endof  homunculi-only of  #creatures first-homunculus  endof
          >r #creatures 0 r>  endcase  ?do  i clear-slot  loop ;
  on creature-clear ( slot -- )  clear-slot ;   \ (one creature gone: a room's command)
  on creature-count ( n other -- )  scatter-other !  scatter ! ;
  on creature-gone ( slot -- )  -1 over slot!  record /kept-creature 0 fill ;
  on game-loaded ( -- )  from-progress  creatures-on state-flag? if  scattered-after-load scatter !  0 scatter-other !  then ;
end-behaviour

: creatures-spawn ( -- id )  keeping-creatures creatures-state s" creatures" spawn ;
