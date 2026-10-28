\ story/places/old-mansion-1f-18.fs - a crossing of the old mansion's 1F: the red-painted walls (her
\ remark, then the smell); four ways out; in from the first, the scene's figure loaded (gone as she
\ stands at that way); Daniella's motions loaded on the way to the far ways while the White
\ Mandragora isn't used; Hewie barking tense at one corner, wary at the other, his looks. A place
\ (story/kit.fs). The original's scripts, for checking: story/rooms-ref/old-mansion-1f-18.fs in the
\ history (converted from the game's; gone with the old machinery in 0.2.20).
IN: story.places.old-mansion-1f-18
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names story.kit fiona.anims ;

own-reset

0 constant red-msg     \ "Why would someone intentionally paint this completely red?"
1 constant smell-msg   \ "Something smells... like something living. Certainly that is really just red paint..."
own: corner-1-noticed  own: corner-2-noticed
$11 constant scene-figure   3 constant figure-slot   0 constant her-motions

: came-in ( exit -- )  3 1e background  0= if  scene-figure figure-slot figure-in  then ;
: red-wall ( F: x z -- )
    fiona wait-move  fiona turn-to-xz  fiona look-low-anim start-anim  40 frames
    red-painting-examined fact? if  smell-msg  else  red-painting-examined fact!  red-msg  then  say ;
: wall-1 ( -- )  6.92e -26.68e red-wall ;
: wall-2 ( -- )  -20.57e -19.02e red-wall ;
: wall-3 ( -- )  -26.62e 7.92e red-wall ;
: barks-at-corner ( -- )  -26.02e 2.5e -26.23e hewie-barks-tense ;
: wary-of-corner ( -- )  26.56e 2.5e 25.45e hewie-stands-by ;
create looks  -26.02e 2.5e -26.23e  20e 6e 6e look,   26.56e 2.5e 25.45e  20e 6e 6e look,
create entries  0 , 0 ,   0 , 0 ,   1 , 1 ,   1 , 1 ,   \ (by exit 0..3: the camera)
create ahead  4 , 0 ,   5 , 1 ,   6 , 2 ,   7 , 3 ,     \ (area, exit loaded ahead)
: her-motions-due ( -- )   \ (Daniella about, the White Mandragora not used)
    white-mandragora-used fact? 0=  partner-kind dup daniella-kind = swap kind34-kind = or and if
        partner her-motions motions-for
    then ;
: each-frame ( -- )
    4 exits-out  ahead 4 ways-out
    fiona 7 entered? if  her-motions-due  then
    corner-1-noticed 30 -26.02e 2.5e -26.23e 20e 6e notices? if  ['] barks-at-corner start drop  then
    corner-2-noticed 30 26.56e 2.5e 25.45e 20e 6e notices? if  ['] wary-of-corner start drop  then
    looks 2 hewie-looks-unchased
    fiona 8 in-area?  fiona 8 60 faces-area? and if  ['] wall-1 unhurried offer  then
    fiona 9 in-area?  fiona 9 60 faces-area? and if  ['] wall-2 unhurried offer  then
    fiona $A in-area?  fiona $A 60 faces-area? and if  ['] wall-3 unhurried offer  then
    fiona 0 in-area? if  figure-slot figure-out  then ;
: person-in ( i -- )  fiona-p = if  entries 4 ways-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-18 place!
