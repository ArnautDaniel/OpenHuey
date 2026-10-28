\ story/places/castle-2f-13.fs - the suspended bridge's hall in Belli Castle's 2F ($28), before the
\ great hall's balcony fell: out on the balcony a while (she, or the stalker after her), it gives -
\ to black, the doors set anew, down to the great hall by the forced way (with the crows; Hewie sent
\ there or kept here); the raised bridge in turn; Hewie, pointed across the gap, leaps over to the
\ Medallion and brings it back (it had: he lies down over there); called back he leaps back; the
\ stalker grabbing him from behind on the plank; the statue in turn (the hall's own: story/places/
\ bridge.fs); the sparkles, Hewie wary on the balcony, the map's pages. A place (story/kit.fs). The
\ original's scripts, for checking: story/rooms-ref/castle-2f-13.fs in the history (converted from
\ the game's; gone with the old machinery in 0.2.20). (castle-2f-14 is the same hall after.)
IN: story.places.castle-2f-13
USING: engine game-state actors messages common facts flag-names story-names stalker-names room-names
    floor-flags items.names effects story.kit fiona.anims story.places.bridge ;

own-reset

\ ---- its own state (a visit) ----
own: wary-noticed
own: balcony-gives         \ out on the balcony a while: it gives (once a visit)
own: her-frames  own: his-frames   \ (on the balcony so far)
own: was-idle              \ she stood idle as it gave

\ ---- the room's things ----
0 constant sparkle-fx   6 constant balcony-area

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  $1C3 $1D0 $1EA far-side!  hall-track 1e background
    c2f-13-flag1 fact? 0= if  sparkle-fx -231e 61e 24e flicker-at  then  medallion-sparkle ;

\ ---- the balcony gives: to black, the doors set anew, down to the great hall by the forced way ----
: stalker-kept ( -- )  stalker hold  begin  yield  again ;
: it-gives ( -- )
    30 frames  music send bgm-resume  15 screen:to-black-2 with-music or fade  wait-fade  world-held state!
    stalker-in? if  ['] stalker-kept start drop  then
    $35 $36 door-as  $36 closed-off  exits-redone ;
: down-with-it ( way hewie-room rest? -- )   \ (her on it - idle, she rests a moment - or the stalker)
    {: way hroom rest :}  stalkers-stay state!  stalkers-blind state!  hewie-no-attack state!  fiona hold
    castle-1f-13 preload  cast:crow 3 figure-in  cast:crow-2 4 figure-in  fiona wait-move
    rest if  fiona rest-anim start-anim  then
    it-gives
    hewie-in? if  hroom -1 0 hewie-to  then
    stalkers-blind state-off  hewie-no-attack state-off  way take-exit ;
: she-gives-way ( -- )  forced-exit castle-1f-13 was-idle @ down-with-it ;
: he-gives-way ( -- )   \ (the stalker after her: the camera on him)
    stalker 0 0 camera-set  stalker follow  forced-exit 1 or castle-2f-14 false down-with-it ;
: on-the-balcony ( -- )   \ (before the great hall's balcony fell: 30 frames out on it, she or he)
    found-file-227 fact?  balcony-gives @ or if  exit  then
    fiona balcony-area in-area? if  1 her-frames +!  else  0 her-frames !  then
    stalker balcony-area in-area? if  1 his-frames +!  else  0 his-frames !  then
    her-frames @ 30 = if
        -1 balcony-gives !  panic-eased  fiona-mode-now @ own-mode = was-idle !  ['] she-gives-way start drop  exit
    then
    his-frames @ 30 = if  -1 balcony-gives !  panic-eased  ['] he-gives-way start drop  then ;

\ ---- Hewie: wary on the balcony (once a visit), his look up at its corner ----
create corner-look  -228.31e 60e 25.69e  35e 10e 0e look,
: wary-on-balcony ( -- )  -231.29e 60e 30.58e hewie-stands-by ;
: hewie-wary ( -- )
    at-his-command? 0=  wary-noticed @ or if  exit  then
    hewie -231.29e 60e 30.58e 50e 20e centred-in? if  -1 wary-noticed !  ['] wary-on-balcony start drop  then ;

\ ---- every frame ----
create cameras  2 , 2 , 2 ,   3 , 1 , 1 ,   $B , 0 , 0 ,   $C , 2 , 2 ,   \ (area, set, path)
: each-frame ( -- )
    0 exit-out  cameras 4 cameras-at
    fiona 2 entered? if  3 map-page  then   fiona 3 entered? if  4 map-page  then
    on-the-balcony  hewie-by-the-gap  hewie-wary  medallion-notice
    medallion-look 1 hewie-looks-at  corner-look 1 hewie-looks-at  bridge-spots ;
: person-in ( i -- )
    case
        fiona-p of  0 0 0 way-in  endof
        hewie-p of  hewie-comes-in  endof
    endcase ;
: went ( -- )  hewie-as-she-goes ;

' came-in ' person-in ' each-frame ' went castle-2f-13 place!
