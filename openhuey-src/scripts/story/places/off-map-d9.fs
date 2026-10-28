\ story/places/off-map-d9.fs - the clock run's start in the forest ($D9, chaos-forest-1's ground
\ again): brought in by the forced way, Fiona played - the run set out (the room ahead loaded, the
\ clock from 0 and Hewie played, his searches counted afresh, Fiona and Hewie at their spots, the
\ stalker into $D8, the summoner his own wait, the stage music by the run's kind, held and let go,
\ Fiona off by exit 0's way); then Hewie's - the world let go, faded in; out by exit 0; the clock. A
\ place (story/kit.fs, story/places/clock-run.fs). The original's scripts, for checking:
\ story/rooms-ref/off-map-d9.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.off-map-d9
USING: engine game-state actors messages common flag-names facts room-names story-names story.kit story.clock
    story.places.clock-run ;

$19 constant forest-track   $2F constant search-var   $2E constant run-kind-var
: came-in ( exit -- )  drop  forest-track 1e background ;

\ ---- the run set out ----
: sets-out ( -- )
    0 prepare-exit  clock-start  0 search-var var!
    fiona activate  fiona $23 81 -2.79e -21.69e place-at  fiona 0 0 camera-set
    hewie activate  off-map-d9 119 0 hewie-to  hewie $77 120 -14.67e -14.87e place-at  0 0 player-camera
    stalker activate  off-map-d8 123 stalker-to  his-own-time
    run-kind-var var@ 0= if  0  else  3  then  run-stage  music-taken
    fiona 0 at-exit  ['] run-begins hewie start-holding drop ;

\ ---- every frame ----
: each-frame ( -- )  0 0 leaves-by  clock-shown ;
: person-in ( i -- )
    dup player person-of = if  0 0 0 way-in  then
    fiona-p =  hewie-played? 0= and  came-by @ forced-exit = and if  sets-out  then ;
: went ( -- )  clock-gone ;

' came-in ' person-in ' each-frame ' went off-map-d9 place!
