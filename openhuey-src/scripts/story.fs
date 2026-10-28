\ story.fs - the story (docs/subsystems/story.md): the played room's place, as an actor for the
\ room. As a room comes in its place is spawned and entered; as Fiona leaves it, it is told, and
\ once the next room is in, it ends, then goes. The places: story/places/*.fs (story/places.fs
\ lists them), written in the kit (story/kit.fs). A room with no place has no actor.
IN: story
USING: engine actors messages story.base story.kit story.common story.places ;

state: story-state
  cell field current       \ the played room's actor (-1: none)
  cell field old           \ the room being left (-1: none)
  9 cells field last-stalker   \ his last report (stalker-doing) ...
  cell field stalker-age       \ ... and how many frames ago: a new place is told it as it comes in
end-state
: tell-stalker ( place -- )   \ (his report of the last frame, if he gave one)
    stalker-age @ 2 < 0= if  drop exit  then
    >r  9 0 do  i cells last-stalker + @  loop  r> send stalker-doing ;

behaviour telling
  on spawned ( -- )  -1 current !  -1 old !  100 stalker-age !
      self subscribe leaving-room  self subscribe arrived  self subscribe stalker-doing  self subscribe tick ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )
      0 8 do  i cells last-stalker + !  -1 +loop  0 stalker-age ! ;
  on tick ( -- )  1 stalker-age +! ;
  on leaving-room ( room exit -- )  2drop  current @ dup 0< if  drop exit  then  dup old !  send room-leave  -1 current ! ;
  on arrived ( room exit -- )   \ (a room still current - two arrivals without a leaving, a jump - goes too)
      old @ dup 0< 0= if  send room-left  else  drop  then
      current @ dup 0< 0= if  send room-left  else  drop  then
      over place-of 0= if  2drop exit  then   \ (no place: nothing of the story's)
      over place-spawn  dup tell-stalker  dup current !  send room-enter ;
  on room-done ( -- )  sender dup alive? if  kill  else  drop  then  sender old @ = if  -1 old !  then ;
  on story-stop ( -- )   \ (each place stops its doings and reports done)
      current @ dup 0< if  drop  else  send story-stop  then  old @ dup 0< if  drop  else  send story-stop  then
      -1 current !  -1 old !  self unsubscribe leaving-room  self unsubscribe arrived ;
end-behaviour

: story-spawn ( -- id )  telling story-state s" story" spawn ;
