\ story.fs - the story (docs/subsystems/story.md): the played room's event scripts, as an
\ actor for the room. As a room comes in its actor is spawned and entered; as Fiona leaves it,
\ it runs its leaving phase, and once the next room is in, its last, then goes.
\ The rooms' scripts: story/rooms/*.fs (converted from the game's); their words: story/words.fs.
IN: story
USING: engine actors messages story.state story.words story.shared story.rooms story.room ;

state: story-state
  cell field current       \ the played room's actor (-1: none)
  cell field old           \ the room being left (-1: none)
end-state

behaviour telling
  on spawned ( -- )  -1 current !  -1 old !  self subscribe leaving-room  self subscribe arrived ;
  on leaving-room ( room exit -- )  2drop  current @ dup 0< if  drop exit  then  dup old !  send room-leave  -1 current ! ;
  on arrived ( room exit -- )   \ (a room still current - two arrivals without a leaving, a jump - goes too)
      old @ dup 0< 0= if  send room-left  else  drop  then
      current @ dup 0< 0= if  send room-left  else  drop  then
      over room-spawn dup current !  send room-enter ;
  on room-done ( -- )  sender dup alive? if  kill  else  drop  then  sender old @ = if  -1 old !  then ;
  on story-stop ( -- )   \ (each room frees its scripts and reports done)
      current @ dup 0< if  drop  else  send story-stop  then  old @ dup 0< if  drop  else  send story-stop  then
      -1 current !  -1 old !  self unsubscribe leaving-room  self unsubscribe arrived ;
end-behaviour

: story-spawn ( -- id )  telling story-state s" story" spawn ;
