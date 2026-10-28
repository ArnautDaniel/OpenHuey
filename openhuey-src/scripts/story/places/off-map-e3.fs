\ story/places/off-map-e3.fs - the clock run's castle ($E3, castle-b1-1's kitchen again): the flame; the
\ day's light in the run's second kind; the two things wandering about; the plywood's parts away, its
\ floor let through; two ways out; its cameras; the rooms ahead loaded as Hewie nears their ways;
\ the two cupboards - Fiona slips into the near one at Hewie's command as he stands by it (she here,
\ nobody after her, no stalker), or the far one (she idle too), out at his next (found or
\ panicking: hauled out); the stalker coming in while she hides, or here as she hides, searches -
\ and always finds her; left behind in one, she's out in front; the walls' shade; the clock. A
\ place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-e3.fs (converted from the game's, not loaded).
IN: story.places.off-map-e3
USING: engine game-state actors messages common flag-names facts room-names floor-flags story.kit
    story.clock story.places.clock-run ;

own-reset
own: far-side         \ the cupboard she hides in: the far one
own: searched         \ he has searched her hiding place (this time hidden)
own: search-started   \ his search begun as she hid (not as he came in)
own: her-held         \ Fiona in a cupboard this frame (held by it: kept for the room's end)

$2E constant run-kind-var   0 constant her-motions   $31 constant search-var   2 constant plywood-floor
: came-in ( exit -- )
    drop  0 0 -35.8e -50.5e -27.25e flame-at
    run-kind-var var@ 1 = if  $84 $60 $3C $50 $24 $2A $2A $46 room-tint  then
    1 -19.5e -26e -31e  4e 4e 100e 0e 10e wanderer-at  2 -53e -22e -11e  4e 5e 80e 0e 30e wanderer-at
    3 hide-part  4 hide-part  plywood-floor barred-floor floor-off  1 $2300 reverb ;

\ ---- the cupboards ----
: hides ( -- )
    stalkers-stay state!
    far-side @ if  fiona $1FA 90 -48.07e -31.71e walk-to  else  fiona $14A -90 -24.3e -32.94e walk-to  then
    0 searched !
    far-side @ if  her-motions $1FA 90 -1 -49.507e -33.083e hides-in
    else  her-motions $14A -90 -1 -22.548e -33.225e hides-in  then ;
: door-if-by ( exit -- )   \ (his search begun as she hid: a shut door only if he stands in its area)
    search-started @ if  stalker over exit-area in-area? 0= if  drop exit  then  then  door-for-him ;
: finds-her ( -- )
    stalker hold  stalker wait-move  0 door-if-by  1 door-if-by  0 search-started !
    $1FE -180 -35.74e -5.079e run-finds-her ;
: searches-here ( -- )   \ (always found: the odds 100 in 100 whatever the count)
    -1 searched !  -1 came-while-hidden !  100 search-var ['] finds-her run-searches ;
: stalker-came ( -- )  came-on-her? if  searches-here  then ;
: search-on ( -- )   \ (each frame: hidden, he here and up, not yet searched)
    st-cond-now @ down-cond =  stalker-told? and  searched @ or if  exit  then
    fiona-hidden state?  stalker-in? and if  -1 search-started !  searches-here  then ;

\ ---- every frame ----
create cameras  2 , 1 , 0 ,  3 , 2 , -1 ,  5 , 1 , 0 ,  6 , 3 , 1 ,  7 , 0 , -1 ,  8 , 3 , 1 ,  $10 , 1 , 0 ,  $11 , 5 , -1 ,   \ (area, set, path)
create ahead  2 , 0 ,  3 , 1 ,   \ (area, exit loaded ahead)
: walls-shade ( -- )
    -19e 8e 0e  13e 8e 0e  -19e 0e 0e  13e 0e 0e  light-doorway
    -18.1e 13e -2.7e  -16.2e 13e -2.7e  -18.1e 0e -2.7e  -16.2e 0e -2.7e  light-doorway
    -18.1e 13e -1e  -18.1e 13e -2.7e  -18.1e 0e -1e  -18.1e 0e -2.7e  light-doorway
    -19e 8e 15e  -19e 8e 0e  -19e 0e 15e  -19e 0e 0e  light-doorway ;
: cupboards ( -- )   \ (Hewie by one, at his command: she in it)
    false -27.83e -33.59e 10e hide-cue? if  0 far-side !  ['] hides fiona start-holding drop exit  then
    true -44.45e -32.32e 10e hide-cue? if  -1 far-side !  ['] hides fiona start-holding drop  then ;
: each-frame ( -- )
    2 exits-out  cameras 8 cameras-at  ahead 2 his-ways
    0 -35.8e -50.5e -27.25e 8e -4e flame-stirred
    search-on  cupboards  fiona held? her-held !  walls-shade  clock-shown ;
: person-in ( i -- )
    dup stalker-p = if  drop stalker-came exit  then
    player person-of = if  0 5 -1 way-in  1 2 -1 way-in  then ;
: went ( -- )  her-held @ if  $161 90 -26.17e -25.52e left-hidden  then  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-e3 place!
