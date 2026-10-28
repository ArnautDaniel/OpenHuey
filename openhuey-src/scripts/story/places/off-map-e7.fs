\ story/places/off-map-e7.fs - the clock run's castle ($E7, castle-1f-8's sitting room again): four
\ flames, each stirred as someone steps into its draught; two ways out; its cameras; the rooms ahead
\ loaded as Hewie nears their ways; the hiding place - Fiona slips in at his command as he stands
\ by it, out at his next (found or panicking: hauled out); the stalker coming in while she hides
\ finds her by the odds; left behind in it, she's out at its front; the clock. A place
\ (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-e7.fs (converted from the game's, not loaded).
IN: story.places.off-map-e7
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

own-reset
own: her-held   \ Fiona in the hiding place this frame (held by it: kept for the room's end)

0 constant her-motions   $33 constant search-var
create flames  34.2e 18.3e 14.2e f, f, f,   34.2e 18.3e -14.2e f, f, f,   -34.2e 18.3e -14.2e f, f, f,   -34.2e 18.3e 14.2e f, f, f,
: flame@ ( k -- ) ( F: -- x y z )  3 * floats flames +  dup 2 floats + f@  dup float+ f@  f@ ;
: flames-lit ( -- )  4 0 do  i 1  i flame@  flame-at  loop ;
: draughts ( -- )  4 0 do  i  i flame@  8e -4e  flame-stirred  loop ;
: came-in ( exit -- )  drop  flames-lit  0 show-part  1 $2300 reverb ;

\ ---- the hiding place ----
: hides ( -- )  stalkers-stay state!  fiona $141 180 -0.12e -19.4e walk-to  her-motions $141 180 -1 -0.803e -15.664e hides-in ;
: finds-her ( -- )  stalker hold  stalker wait-move  0 door-for-him  1 door-for-him  $B2 180 0.866e 0.417e run-finds-her ;
: stalker-came ( -- )  came-on-her? if  search-var finding-odds search-var ['] finds-her run-searches  then ;

\ ---- every frame ----
create cameras  4 , 1 , 0 ,  5 , 0 , -1 ,  6 , 2 , -1 ,   \ (area, set, path)
create ahead  2 , 1 ,  3 , 0 ,   \ (area, exit loaded ahead)
: each-frame ( -- )
    2 exits-out  cameras 3 cameras-at  7 1 0 camera-out  ahead 2 his-ways
    true -0.56e -21.78e 10e hide-cue? if  ['] hides fiona start-holding drop  then
    draughts  fiona held? her-held !  clock-shown ;
: person-in ( i -- )
    dup stalker-p = if  drop stalker-came exit  then
    player person-of = if  0 0 -1 way-in  1 1 0 way-in  then ;
: went ( -- )  her-held @ if  $141 0 -0.1e -18.54e left-hidden  then  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e7 place!
