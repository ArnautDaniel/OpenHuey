\ story/places/old-mansion-1f-19.fs - a room of the old mansion's 1F with a hole in the wall ("Enter the
\ hole?": through it, out by the far way; back through it, here) and a hiding place out of sight (the
\ kit's; Daniella's search - she looks in); after the 3F's turn a homunculus dropped in, once; the
\ scene's figure loaded on the way to the far door; Hewie barking at a spot, his look; its sound. A
\ place (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-19.fs
\ (converted from the game's, not loaded).
IN: story.places.old-mansion-1f-19
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    story.kit fiona.anims fiona.sounds ;

own-reset

0 constant hole-msg   \ "A miasma of mold and corrosion wafts from within the hole. Enter the hole? YES NO"
own: sounds-on  own: spot-noticed
8 constant room-loop-sound   0 constant her-motions   1 constant her-look-motions   $13 constant search-var
$1C constant scene-figure   3 constant figure-slot   $FE constant through-hole   7 constant hole-area   6 constant hide-area

: came-in ( exit -- )
    drop  room-sounds
    om3f-1-flag2 fact?  om1f-19-flag1 fact? 0= and if  om1f-19-flag1 fact!  old-mansion-1f-19 0 109 $80 5 $B4 0e creature-dropped  then
    1 $2300 reverb ;

\ ---- the hole: in, out by the far way; back through it ----
: the-hole ( -- )
    stalkers-stay state!  fiona hold  off-map-27 preload  figure-slot figure-out  fiona wait-move
    fiona 180 turn  fiona look-close-anim 10 play-blend
    hole-msg yes? if
        15 screen:to-black fade  wait-fade  slide-sound common-bank fiona sound-by  world-held state!  through-hole take-exit
    else  1 prepare-exit  scene-figure figure-slot 1 figure-in-by  stalkers-stay state-off  then ;
: back-through ( -- )
    fiona hold  1 prepare-exit  scene-figure figure-slot 1 figure-in-by  fiona wait-move  camera-restart
    slide-sound common-bank fiona sound-by  15 screen:from-black fade  wait-fade  world-held state-off  stalkers-stay state-off ;

\ ---- the hiding place (the kit's, out of sight); Daniella's search ----
: hides ( -- )  -1 hide-unseen !  her-motions $31 180 2 82.92e -88.12e hides-in ;
: her-looker? ( -- flag )  st-kind-now @ dup daniella-kind = swap kind34-kind = or ;
: looks-in ( -- )
    stalker hold  stalker wait-move  0 door-for-him  stalker $4F 180 83.66e -76.9e walk-to
    her-looker? if  stalker use-motions  1 her-back  stalker room-motion play  0 her-back
    else  stalker stalker.anims:there-anim play  then
    -1 found-her !  stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;
: search-odds ( -- pct )  search-var var@ case  0 of 10 endof  1 of 25 endof  2 of 50 endof  3 of 75 endof  >r 0 r>  endcase ;
: stalker-came ( -- )
    came-on-her? 0= if  exit  then
    her-looker? if  stalker her-look-motions motions-for  then
    search-odds chance?  2 creature-moving? or if  ['] looks-in start drop  else  120 summoner-sooner  then
    search-var var@ 1+ search-var var! ;

: barks-at-spot ( -- )  0.75e 0e -116.99e hewie-barks-at ;
create looks  0.75e 0e -116.99e  20e 10e 10e look,
create cameras  4 , 0 , 0 ,   5 , 1 , 1 ,   \ (area, set, path)
create entries  1 , 1 ,   0 , 0 ,   \ (by exit 0, 1: the camera)
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 2 cameras-at
    fiona 2 entered? if  0 prepare-exit  figure-slot figure-out  then
    fiona 3 entered? if  1 prepare-exit  scene-figure figure-slot 1 figure-in-by  then
    spot-noticed 30 0.75e 0e -116.99e 20e 10e notices? if  ['] barks-at-spot start drop  then
    looks 1 hewie-looks-at
    sounds-on @ if  room-loop-sound 0e 9e -125e keep-sound
    else  room-sounds-in? if  room-loop-sound 0e 9e -125e room-sound  -1 sounds-on !  then  then
    fiona hole-area in-area?  fiona 180 50 heading? and if  ['] the-hole unhurried offer  then
    fiona hide-area in-area?  fiona 180 60 heading? and if
        stalker-in? if  no-time-message offer-remark-now  else  ['] hides calm-only offer  then
    then ;
: person-in ( i -- )
    case
        fiona-p of
            entries 2 ways-in
            came-by @ through-hole = if  fiona $14 0 0e -110e place-at  0 0 player-camera  ['] back-through start drop  then
        endof
        stalker-p of  stalker-came  endof
    endcase ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-19 place!
