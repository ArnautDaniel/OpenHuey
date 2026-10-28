\ story/places/off-map-e4.fs - the clock run's castle ($E4, castle-1f-4's hall again): its parts and
\ objects as the run has them; the day's light in the run's second kind; the flame; its four sounds
\ in turn; the floor plate - down while someone stands on it, with a sound as the first steps on;
\ three ways out (one by an area); its cameras; the rooms ahead loaded as Hewie nears their ways;
\ the clock. A place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for
\ checking: story/rooms-ref/off-map-e4.fs in the history (converted from the game's; gone with the
\ old machinery in 0.2.20).
IN: story.places.off-map-e4
USING: engine game-state actors messages common flag-names facts room-names story.kit story.clock
    story.places.clock-run ;

own-reset
own: on-plate   \ someone on the floor plate (its call made)

$2E constant run-kind-var   0 constant came-in-sound   5 constant first-ambient   $D constant plate-area
: plate ( at-once? -- )   \ (the plate, "fumi_yuka", down while someone stands on it: at once, or by 0.2 a frame)
    {: now :}  s" fumi_yuka" placed-here? 0= if  exit  then
    on-plate @ if  0e  else  1e  then  {: F: goal :}
    now if  goal s" fumi_yuka" place-y object!  exit  then
    goal f0= if  -0.2e  else  0.2e  then  s" fumi_yuka" place-y object@ f+  0e fmax 1e fmin  s" fumi_yuka" place-y object! ;
: came-in ( exit -- )
    drop  room-sounds  0 hide-part  1 object-show  2 object-show  3 object-show  4 object-hide  5 object-hide
    run-kind-var var@ 1 = if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then
    0 0 22.8e 14.65e 68.75e flame-at
    1 show-part  2 hide-part  3 show-part  4 hide-part  5 show-part
    1 $2300 reverb  true plate ;

\ ---- every frame ----
create cameras  $B , 1 , 1 ,  $C , 0 , 0 ,   \ (area, set, path)
: plate-call ( -- )   \ (the first onto the plate: its sound at them; all off - ready again)
    on-plate @ 0= if
        hewie plate-area in-area? if  -1 on-plate !  came-in-sound room-bank hewie sound-by exit  then
        fiona plate-area in-area? if  -1 on-plate !  came-in-sound room-bank fiona sound-by exit  then
        stalker plate-area in-area? if  -1 on-plate !  came-in-sound room-bank stalker sound-by  then  exit
    then
    hewie plate-area in-area? 0=  fiona plate-area in-area? 0= and  stalker plate-area in-area? 0= and if  0 on-plate !  then ;
: each-frame ( -- )
    1 exit-out  2 exit-out  1 3 leaves-by  cameras 2 cameras-at
    hewie 5 left?  hewie 6 entered? or if  3 prepare-exit  then
    hewie 7 left?  hewie 8 entered? or if  1 prepare-exit  then
    9 2 his-way-back
    room-sounds-in? if  first-ambient -17e 22e -9e ambient-sounds  then
    plate-call  false plate  0 22.8e 14.65e 68.75e 8e -4e flame-stirred  clock-shown ;
: person-in ( i -- )  player person-of = if  1 0 0 way-in  2 0 0 way-in  3 0 0 way-in  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e4 place!
