\ story/places/off-map-e0.fs - the clock run's start in the castle ($E0, castle-2f-3's bedroom
\ again): its parts; the fan turning; the day's light in the run's second kind; the glass of the
\ window and the door; its steady sound and four others in turn; brought in by the forced way, Fiona
\ played - the run set out (the room ahead loaded, Fiona off by exit 0's way, the hiding places'
\ counts afresh, Fiona and Hewie at their spots, the stalker into $E1, the summoner his own wait,
\ the stage music by the run's kind, the clock from 0 and Hewie played, the castle's doors on the
\ way shut - the far ones locked); then Hewie's - the world let go, faded in; out by exit 0; its
\ cameras; the wardrobe - Fiona slips in at Hewie's command as he stands by it, out at his next
\ (found or panicking: hauled out); the stalker coming in while she hides finds her by the odds;
\ left behind in it, she's out in front; the clock. A place (story/kit.fs,
\ story/places/clock-run.fs). The original's scripts, for checking: story/rooms-ref/off-map-e0.fs in
\ the history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.off-map-e0
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

own-reset
own: sounds-on   \ its steady sound started (then kept up)
own: her-held    \ Fiona in the wardrobe this frame (held by it: kept for the room's end)

$2E constant run-kind-var   1 constant her-motions   $30 constant search-var
9 constant room-loop-sound   5 constant first-ambient   $1A constant door-fx   $1B constant window-fx   $40 constant glass-strength
: fan-turning ( -- )  begin  s" fan" 0.0200713e object-turn  yield  again ;   \ (1.15 degrees a frame)
: glass ( -- )
    door-fx false glass-strength  40.491e 30e -10.498e  49.502e 30e -19.509e  40.491e 1.5e -10.498e  49.502e 1.5e -19.509e  door-glass-in
    window-fx glass-strength  0.491e 22e 69.502e  9.502e 22e 60.491e  0.491e 0e 69.502e  9.502e 0e 60.491e  window-in ;
: came-in ( exit -- )
    drop  room-sounds  0 show-part  ['] fan-turning start drop  1 hide-part
    run-kind-var var@ 1 = if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then
    glass ;

\ ---- the run set out ----
: doors-on-the-way ( -- )   \ (the castle's: shut - the far ones locked too)
    $122 $11D do  i door-shut  loop  $12B $123 do  i door-shut  loop
    $12B $126 do  i lock  loop  doors-redone ;
: sets-out ( -- )
    0 prepare-exit  fiona 0 at-exit
    $34 $30 do  0 i var!  loop
    fiona activate  fiona $55 180 -40.16e 11.18e place-at  fiona 0 0 camera-set
    hewie activate  off-map-e0 276 0 hewie-to  hewie $114 -127 -32.33e 16.81e place-at  0 0 player-camera
    stalker activate  off-map-e1 406 stalker-to  his-own-time
    run-kind-var var@ 1 = if  1  else  2  then  run-stage  music-taken
    clock-start  doors-on-the-way
    ['] run-begins hewie start-holding drop ;

\ ---- the wardrobe ----
: hides ( -- )  stalkers-stay state!  fiona $27 180 14.32e -41.52e walk-to  her-motions $27 180 -1 14.5e -37.5e hides-in ;
: odds ( -- pct )  search-var var@ case  2 of 25 endof  3 of 50 endof  >r 0 r>  endcase ;
: finds-her ( -- )  stalker hold  stalker wait-move  0 door-for-him  1 door-for-him  $1C 180 -0.663e -24.791e run-finds-her ;
: stalker-came ( -- )  came-on-her? if  odds search-var ['] finds-her run-searches  then ;

\ ---- every frame ----
create cameras  $17 , 0 , 0 ,  $18 , 1 , 1 ,  $19 , 0 , 0 ,  $1A , 2 , 2 ,   \ (area, set, path)
: ambience ( -- )   \ (its steady sound once the room's sounds are in; then every 30 frames the next of four)
    sounds-on @ 0= if  room-sounds-in? if  room-loop-sound -42e 4e -63e room-sound  -1 sounds-on !  then  exit  then
    first-ambient -47e 12e -28e ambient-sounds  room-loop-sound -42e 4e -63e keep-sound ;
: each-frame ( -- )
    ambience  0 exit-out  cameras 4 cameras-at
    true 11e -45e 10e hide-cue? if  ['] hides fiona start-holding drop  then
    fiona held? her-held !  clock-shown ;
: person-in ( i -- )
    dup stalker-p = if  drop stalker-came exit  then
    dup player person-of = if  0 1 1 way-in  then
    fiona-p =  hewie-played? 0= and  came-by @ forced-exit = and if  sets-out  then ;
: went ( -- )  her-held @ if  $27 0 13.59e -38e left-hidden  then  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e0 place!
