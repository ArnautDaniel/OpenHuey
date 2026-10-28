\ story/places/old-mansion-1f-24.fs - a hall of the old mansion's 1F where Daniella scrubs the floor
\ (the White Mandragora not used, she not about): no fright from her, no camera on her; spoken to,
\ she looks up - Hebel's Perfume the first time ("a filthy little princess"), else "It's cleaning
\ time" - and back to scrubbing; Hewie wary of her; the covered statue; the vase and its Beef Jerky,
\ the Rubrum Medallion. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-1f-24.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-1f-24
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags items.names story.kit fiona.anims ;

own-reset

0 constant princess-msg   \ "Oh my. What a filthy little princess. A small present from the lord of the manor..."
1 constant cleaning-msg   \ "Miss... It's cleaning time now."
2 constant statue-msg     \ "Judging from the size and shape, there is probably a statue under there... But I probab..."
own: scrubbing      \ Daniella here scrubbing (the encounter to come)
own: turning        \ asked to look up, or down again: her turn under way
own: she-noticed
0 constant jerky-fx   1 constant medallion-fx   1 constant her-floor
0 constant presence-sound   1 constant presence-2-sound   2 constant her-motions
om1f-24-breakable-broken 0 1  -97.5e 0e 1e 5e 8e  breakable: the-vase
the-vase $80202020 0 8 shard-look  the-vase 0.812e 0.687e 0.187e 0.312e shard-uv
the-vase 0 frees-floor  the-vase $B4 noise-from

\ ---- Daniella scrubbing; looking up when asked, back down again ----
: her-motion ( i -- anim )  room-motion + ;
: scrubs ( -- )
    begin
        begin  turning @ 0= while  stalker 1 her-motion play  yield  repeat
        stalker wait-anim  stalker 4 her-motion 20 play-blend  stalker 3 her-motion 7 play-blend  0 turning !   \ (up)
        begin  turning @ 0= while  stalker 3 her-motion play  yield  repeat
        stalker wait-anim  stalker 5 her-motion 7 play-blend  stalker 1 her-motion 20 play-blend  0 turning !   \ (down)
    again ;
: at-her-task ( -- )
    stalker-present  stalker hold  stalker true noclip  stalker $31 180 -76e -56.5e place-at  her-floor solid-floor floor-on
    stalker use-motions  0 turning !  hands-empty  partner false hush  scrubs ;
: turns ( -- )  -1 turning !  begin  turning @ while  yield  repeat ;
: spoken-to ( -- )
    fiona hold  hewie-no-attack state!  fiona wait-move  turns
    hebels-perfume items:items-of 0= if
        presence-sound room-bank stalker sound-by  princess-msg show  30 frames  wait-closed
        hebels-perfume item-named  hebels-perfume 1 give-item  hebels-perfume show-item-tab  fanfare  obtained-2-msg say
        got-hebels-perfume fact!
    else  presence-2-sound room-bank stalker sound-by  cleaning-msg show  90 frames  wait-closed  then
    turns  hewie-no-attack state-off ;

\ ---- as she comes in ----
: jerky-shows ( -- )  om1f-24-beef-jerky fact? 0= if  jerky-fx -97.5e 1e 1e flicker-at  then ;
: came-in ( exit -- )
    room-sounds  the-vase as-it-stands  the-vase broken? if  jerky-shows  then
    white-mandragora-used fact? 0=  partner-kind dup daniella-kind = swap kind34-kind = or and  stalker-told? 0= and scrubbing !
    her-floor solid-floor floor-off
    scrubbing @ if
        1 <> if  partner her-motions motions-for  then
        force-followed state!  stalker-no-fear state!  no-stalker-camera state!
        partner activate  partner true hush  old-mansion-1f-24 49 stalker-to  ['] at-her-task start drop
    else  drop  then
    om1f-24-rubrum-medallion fact? 0= if  medallion-fx -98.51e 1e -12.98e flicker-at  then
    1 $2300 reverb ;

\ ---- what she may do here ----
: the-statue ( -- )  fiona wait-move  statue-msg say ;
: take-jerky ( -- )  beef-jerky om1f-24-beef-jerky jerky-fx -97.5e 1e picks-up ;
: take-medallion ( -- )  rubrum-medallion om1f-24-rubrum-medallion medallion-fx -98.51e -12.98e picks-up ;
: wary-of-her ( -- )  -76.01e 0e -50.35e hewie-stands-by ;
: faces-at? ( F: x z -- flag )  {: F: x z :}  fiona 45 x z faces-point? ;
: faces-her? ( -- flag )  stalker body-pos fswap fdrop faces-at? ;

create cameras  8 , 0 , 0 ,   9 , 1 , 1 ,   \ (area, set, path)
create entries  1 , 1 ,   2 , 2 ,   1 , 1 ,   0 , 0 ,   \ (by exit 0..3: the camera)
create ahead  4 , 0 ,   6 , 0 ,   7 , 2 ,   5 , 3 ,   \ (area, exit loaded ahead)
create looks  -76.01e 0e -50.35e  70e 17e 17e look,
: each-frame ( -- )
    2 exits-out  2 2 leaves-by  3 3 leaves-by  cameras 2 cameras-at  ahead 4 ways-out
    white-mandragora-used fact? 0= if
        she-noticed 100 -76.01e 0e -50.35e 70e 17e notices? if  ['] wary-of-her start drop  then
        looks 1 danger-now @ followed = hewie-looks-if
    then
    room-sounds-in?  stalker-in? and if
        presence-sound keep-playing or room-bank stalker sound-by  presence-2-sound keep-playing or room-bank stalker sound-by
    then
    scrubbing @  stalker-in? and  turning @ 0= and  fiona stalker 10 close-to? and if  faces-her? if  ['] spoken-to unhurried offer  then  then
    fiona $A in-area?  fiona -180 30 heading? and  fiona $B in-area?  fiona -134 60 heading? and or if  ['] the-statue unhurried offer  then
    om1f-24-rubrum-medallion fact? 0=  fiona -98.51e 1e -12.98e 5e 5e touches? and if  ['] take-medallion calm-only offer  then
    the-vase broken? if
        om1f-24-beef-jerky fact? 0=  fiona -97.5e 1e 1e 5e 5e touches? and if  ['] take-jerky calm-only offer  then
    else  the-vase breaks? if  jerky-shows  then  then ;
: person-in ( i -- )  fiona-p = if  entries 4 ways-in  then ;
: went ( -- )
    scrubbing @ if  stalker-no-fear state-off  no-stalker-camera state-off  hands-back  partner false hush  stalker let-go  then
    force-followed state-off ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-24 place!
