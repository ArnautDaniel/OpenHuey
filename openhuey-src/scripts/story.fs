\ story.fs - the story (docs/subsystems/story.md): the played room's event scripts, as an
\ actor for the room. As a room comes in its actor is spawned and entered; as Fiona leaves it,
\ it runs its leaving phase, and once the next room is in, its last, then goes.
\ The rooms' scripts: story/rooms/*.fs (converted from the game's); their words: story/words.fs.
IN: story
USING: engine actors messages story.state story.words story.shared story.rooms story.room story.kit story.places ;

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
      over dup place-of if  place-spawn  dup tell-stalker  else  room-spawn  then  dup current !  send room-enter ;   \ (a place: written anew; else its converted scripts)
  on room-done ( -- )  sender dup alive? if  kill  else  drop  then  sender old @ = if  -1 old !  then ;
  on story-stop ( -- )   \ (each room frees its scripts and reports done)
      current @ dup 0< if  drop  else  send story-stop  then  old @ dup 0< if  drop  else  send story-stop  then
      -1 current !  -1 old !  self unsubscribe leaving-room  self unsubscribe arrived ;
end-behaviour

: story-spawn ( -- id )  telling story-state s" story" spawn ;
