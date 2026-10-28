\ story/places/castle-2f-9.fs - the stair landing in Belli Castle's 2F ($5): while the chapter has
\ Daniella the maid (after the Saturnus hint, before castle-3f-2), her cleaning here - on her knees
\ scrubbing, harmless: spoken to she stands, "It's cleaning time now" (after, her strangeness); her
\ steady sound, Hewie wary of her and looking her way; the first time in, the screen back. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/castle-2f-9.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.castle-2f-9
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags effects story.kit ;

own-reset

\ ---- what is said here ----
0 constant cleaning-msg   \ "Miss... It's cleaning time now."
1 constant attacked-msg   \ "What?! Y-You just attacked me. How can you be so nonchalant? Could it have been... some..."

\ ---- its own state (a visit) ----
own: wary-noticed          \ Hewie wary of her once a visit
own: stood-up              \ she has stood to speak (back to her scrubbing after)

\ ---- the room's things ----
0 constant her-floor   0 constant presence-sound   3 constant to-exit-1-area   4 constant to-exit-0-area
: cleaning? ( -- flag )  c1f-14-flag2 fact?  c3f-2-flag1 fact? 0= and ;

\ ---- Daniella cleaning: on her knees scrubbing, over and over ----
: scrubs ( -- )  begin  stalker room-motion 1 + play  again ;
: cleans ( -- )
    begin  stalker-in? 0= while  yield  repeat
    stalker hold  stalker true noclip  stalker use-motions
    stalker $162 180 -4.85e -47.03e place-at  stalker 0 0 camera-set  scrubs ;
: she-stands ( -- )   \ (spoken to: up, her words, down to it again)
    stalker hold  stalker true noclip  stalker room-motion 4 + 20 play-blend  stalker room-motion 3 + 7 start-blend
    begin  room-sounds-in? 0= while  yield  repeat
    presence-sound room-bank stalker sound-by  cleaning-msg show  stalker wait-anim  wait-closed
    stalker room-motion 5 + 7 play-blend  stalker room-motion 1 + 20 play-blend
    c2f-9-flag2 fact!  -1 stood-up !  scrubs ;
: speaks-to-her ( -- )
    fiona hold  hewie-no-attack state!  0 stood-up !  ['] she-stands start drop
    begin  stood-up @ 0= while  yield  repeat  hewie-no-attack state-off ;
: her-strangeness ( -- )  fiona hold  hewie-no-attack state!  attacked-msg say  hewie-no-attack state-off ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop
    cleaning? if
        room-sounds  force-followed state!  stalker-no-fear state!  no-stalker-camera state!
        her-floor keeps-fiona-off keeps-hewie-off or floor-on
        partner dup 0< 0= if  dup activate  >r castle-2f-9 354 r> send stalker-in  else  drop  then
        hands-empty  partner true hush  ['] cleans start drop
    then
    1 $2300 sound-reverb! ;

\ ---- the first time in: the screen back, the room ahead loaded ----
: first-in ( -- )
    yield  camera-restart  1 prepare-exit  16 frames  world-held state-off
    15 screen:from-black with-music or fade  c2f-9-flag1 fact!  stalkers-stay state-off ;

\ ---- what she may do or say here ----
: spots ( -- )
    cleaning?  fiona stalker 3 45 touches-facing? and if
        c2f-9-flag2 fact? if  ['] her-strangeness unhurried offer  else  ['] speaks-to-her unhurried offer  then
    then ;

\ ---- Hewie: wary of her (once a visit), looking her way while she is followed ----
create looks  -4.53e -46e -47.5e  30e 15e 15e look,
: wary-of-her ( -- )  -4.53e -46e -47.5e hewie-stands-by ;
: hewie-notices ( -- )
    cleaning? 0= if  exit  then
    hewie-in?  hewie-cmdable-now @ and 0= if  exit  then
    wary-noticed @ 0=  hewie-cond-now @ well-cond = and  chased? 0= and  hewie -4.53e -46e -47.5e 30e 15e centred-in? and if
        -1 wary-noticed !  ['] wary-of-her start drop
    then
    danger-now @ followed = if  looks looks-at  then ;

\ ---- every frame ----
create cameras  7 , 0 , 0 ,   8 , 1 , 1 ,   9 , 0 , 0 ,   $A , 2 , 2 ,   $B , 1 , 1 ,   $C , 2 , 2 ,   \ (area, set, path)
create ahead  3 , 1 ,   4 , 0 ,   \ (area, exit)
create entries  0 , 0 ,   2 , 2 ,   1 , 1 ,   \ (by exit 0..2: the camera)
: each-frame ( -- )
    0 0 leaves-by  1 1 leaves-by  2 2 leaves-by  cameras 6 cameras-at  ahead 2 ways-out
    hewie-notices
    room-sounds-in?  stalker-in? and if  presence-sound keep-playing or room-bank stalker sound-by  then
    spots ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 3 ways-in
    c2f-9-flag1 fact? 0= if  came-by @ forced-exit = if  2 2  else  1 1  then  player-camera  ['] first-in start drop  then ;
: went ( -- )
    force-followed state-off
    cleaning? if
        hands-back  stalker false hush  stalker let-go  0 summon-take
        fiona 0 in-area? if  stalker-no-fear state-off  no-stalker-camera state-off  then
    then ;

' came-in ' person-in ' each-frame ' went castle-2f-9 place!
