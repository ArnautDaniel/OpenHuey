\ story/places/lake-pathway-2.fs - the lake pathway by the bridge ($80): once the bridge is gone
\ (the story's over-music-d), its far end is barred, she remarks on it, and Hewie may bark at the
\ gap. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/lake-pathway-2.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.lake-pathway-2
USING: engine game-state actors messages common facts flag-names story-names room-names floor-flags
    effects story.kit fiona.anims ;

own-reset

\ ---- what is said here ----
0 constant bridge-gone-msg   \ "What!? The bridge is gone! It looks like it has disappeared!"
1 constant no-way-back-msg   \ "There's no going back now. I'll need to find a new way out now."

\ ---- its own state (a visit) ----
own: bridge-remarked
own: gap-noticed            \ Hewie has looked at the gap

\ ---- the room's things ----
0 constant bridge-part   0 constant gap-group   \ (the bridge; the floor where it was)
1 constant to-exit-1-area   2 constant near-exit-0-area   3 constant near-exit-1-area   6 constant bridge-end-area
$18 constant lake-track
: bridge-gone? ( -- flag )  over-music-d fact? ;

\ ---- as she comes in ----
: water-light ( -- )   \ (the light off the lake: wide and far off; once the bridge is gone, near and turning)
    0 $40404040 bridge-gone? if  0e 27.1e 50e 500e 0.1e 0.05e  else  775e 27.1e -420e 1500e 0.1e 0e  then
    effects:caustic drop ;
: over-the-water ( -- )   \ (two swarms of specks)
    $10 $E $80 $80 $80 $40 rgba 22.8e 95.4e 53.7e specks
    $10 $E $80 $80 $80 $40 rgba -23.1e 95.4e 53.7e specks ;
: came-in ( exit -- )
    drop  lake-track 1e background  over-the-water
    bridge-gone? if  gap-group barred-floor floor-on  else  bridge-part show-part  then
    water-light ;

\ ---- what she may say here ----
: the-gap ( -- )   \ (at the bridge's end, facing the gap)
    fiona wait-move  fiona 180 turn
    bridge-remarked @ if
        fiona hands-clasped-anim start-anim  16 frames  no-way-back-msg say
    else
        -1 bridge-remarked !  fiona look-low-anim start-anim  30 frames  bridge-gone-msg say
    then  fiona wait-anim ;
: spots ( -- )
    bridge-gone? 0= if  exit  then
    fiona bridge-end-area in-area?  fiona 180 50 heading? and if  ['] the-gap unhurried offer  then ;

\ ---- Hewie: the gap (the bridge gone) - now and then he barks at it ----
: barks-at-gap ( -- )  -2.26e 38e -25.57e hewie-barks-at ;
: hewie-looks ( -- )
    bridge-gone? 0=  gap-noticed @ or if  exit  then
    hewie-in?  hewie-cond-now @ well-cond = and  chased? 0= and  hewie-cmdable-now @ and 0= if  exit  then
    hewie -2.26e 38e -25.57e 30e 15e centred-in? 0= if  exit  then
    -1 gap-noticed !  30 chance? if  ['] barks-at-gap start drop  then ;

\ ---- every frame: its ways out, its cameras ----
create cameras  4 , 1 , 1 ,   5 , 0 , 0 ,                     \ (area, set, path)
create ahead  near-exit-0-area , 0 ,   near-exit-1-area , 1 ,  \ (area, exit loaded ahead)
: ways ( -- )  0 exit-out  to-exit-1-area 1 leaves-by  cameras 2 cameras-at  ahead 2 ways-out ;
: each-frame ( -- )  ways  hewie-looks  spots ;
: player-came ( -- )   \ (by exit 0 the view across the water - wider once the bridge is gone)
    bridge-gone? if  0 2 2 way-in  else  0 1 1 way-in  then  1 0 0 way-in ;
: person-in ( i -- )  fiona-p = if  player-came  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went lake-pathway-2 place!
