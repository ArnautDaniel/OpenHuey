\ story/places/old-mansion-1f-15.fs - the library of the old mansion's 1F: three books to find (on
\ the table; under the stand, the box pushed aside; one standing out on a shelf) and three lecterns
\ to lay them on - each lectern's own author: its shelves slide back, the way through (another book
\ there may be taken back); the shelves by author (their remarks), the door atop one; two save
\ clocks; the Refined Antimony; the Antimony Tube and the Rubrum Medallion Hewie sniffs out; Hewie
\ by the stand; its sounds. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-1f-15.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-1f-15
USING: engine game-state actors messages common facts room-names flag-names story-names floor-flags
    items.names story.kit fiona.anims ;

own-reset

0 constant table-book-msg   \ "There is a book lying on the table."
1 constant stand-book-msg   \ "There is a book below the stand..."
2 constant stands-out-msg   \ "One of the books stands out from the rest."
3 constant author-1-msg     \ "The books have been arranged by author. This shelf is full of books written by someone..."
4 constant author-2-msg     \ "The books have been arranged by author. These books are all written by a person named A..."
5 constant author-3-msg     \ "The books have been arranged by author. This shelf seems to contain books by Nicholai F..."
6 constant author-4-msg     \ "The books have been arranged by author. Sangerman is the author of these books."
7 constant author-5-msg     \ "The books have been arranged by author. These books are authored by Paraculeus."
8 constant author-6-msg     \ "The books have been arranged by author. The books are all written by a person named..."
9 constant door-atop-msg    \ "There is a door atop this bookshelf... Why would anyone build a door of all things..."
$A constant lectern-0-msg   \ "The books have been arranged by author. The shelf is full of books written by some..."
$B constant lectern-1-msg   \ "The books have been arranged by author. The books are all written by a person name..."
$C constant lectern-2-msg   \ "The books have been arranged by author. The shelf seems to contain books by Nichol..."
own: shelf-remarked  own: hewie-away  own: noticed-3  own: noticed-4  own: noticed-4b
0 constant antimony-fx   1 constant tube-fx   2 constant medallion-fx
3 constant first-ambient   8 constant first-ambient-2   $C constant shelves-sound   $D constant shelves-stop-sound   $E constant book-sound
own: ambient-at  own: ambient-wait

\ ---- the moving shelves: group g's four (its colour's book and three shelves), back to their stop ----
: shelf ( i -- addr len )
    case
        0 of  s" movebook_blue"  endof    1 of  s" hondana_a0"  endof   2 of  s" hondana_a1"  endof   3 of  s" hondana_a2"  endof
        4 of  s" movebook_yellow"  endof  5 of  s" hondana_b0"  endof   6 of  s" hondana_b1"  endof   7 of  s" hondana_b2"  endof
        8 of  s" movebook_red"  endof     9 of  s" hondana_c0"  endof  10 of  s" hondana_c1"  endof  11 of  s" hondana_c2"  endof
        >r s" " r>
    endcase ;
create shelf-stops  0e f, 0e f,  39e f, 21e f,  0e f, -52.5e f,   \ (x, z by group)
: shelf-stop ( g off -- ) ( F: -- v )  place-z = 1 and  swap 2* +  floats shelf-stops + f@ ;
: shelf-step ( g off i -- moving? )   \ (one of the group's back 0.25, held at its stop)
    {: g off i | a n :}  g 4 * i + shelf to n to a
    a n placed-here? 0= if  false exit  then
    a n off -0.25e object+!
    a n off object@  g off shelf-stop  f< if  a n off  g off shelf-stop  object!  false exit  then  true ;
: shelves-step ( g off -- moving? )  {: g off :}  false  4 0 do  g off i shelf-step or  loop ;
: shelves-at-stop ( g -- )
    {: g :}  4 0 do  g 4 * i + shelf  2dup placed-here? if  2dup place-x  g place-x shelf-stop  object!  place-z  g place-z shelf-stop  object!  else  2drop  then  loop ;
: shelf-sound-at ( g id -- )   \ (at the group's book)
    {: g id | a n :}  g 4 * shelf to n to a  a n placed-here? 0= if  exit  then
    id room-bank  a n place-x object@  a n place-y object@  a n place-z object@  sound-there ;
: shelves-back ( g off -- )   \ (their sound kept going till they stop)
    {: g off :}  g shelves-sound shelf-sound-at
    begin  g off shelves-step while  g shelves-sound keep-playing or shelf-sound-at  yield  repeat
    shelves-sound room-bank stop-sound ;

\ ---- the books ----
: book-part ( lectern book -- part | -1 )   \ (a book lying on a lectern not its own)
    swap 3 * + case  1 of $B endof  2 of $A endof  3 of 9 endof  5 of 8 endof  6 of 6 endof  7 of 7 endof  >r -1 r>  endcase ;
: on-lectern? ( lectern book -- flag )  swap 3 * + books-placed + fact? ;
: book-item ( book -- item )  cathedral-secrets + ;

\ ---- as she comes in ----
: lecterns-shown ( -- )
    3 0 do  3 0 do  j i book-part dup 0< if  drop  else  j i on-lectern? if  show-part  else  drop  then  then  loop  loop ;
: came-in ( exit -- )
    drop  room-sounds  0 ambient-at !  0 ambient-wait !
    got-cathedral-secrets fact? 0= if  1 show-part  then
    got-occult-philosophy fact? 0= if  2 show-part  then
    got-wisemans-crafts fact? 0= if  0 show-part  then
    books-placed fact? if  5 barred-floor floor-on  2 shelves-at-stop
    else  4 barred-floor floor-on  5 keeps-hewie-off floor-on  8 object-hide  then
    lectern-1-book-1 fact? if
        3 barred-floor floor-on  $187 hole-floor floor-tri-on  $1D0 hole-floor floor-tri-on  $188 hole-floor floor-tri-on  $1D1 hole-floor floor-tri-on
        1 shelves-at-stop
    else
        2 barred-floor floor-on  3 keeps-hewie-off floor-on  $18A hole-floor floor-tri-on  $1D3 hole-floor floor-tri-on  4 object-hide
    then
    lectern-2-book-2 fact? if  1 barred-floor floor-on  0 shelves-at-stop
    else  0 barred-floor floor-on  1 keeps-hewie-off floor-on  0 object-hide  6 floor-flag-200 floor-off  exits-redone  then
    lecterns-shown
    om1f-15-flag1 fact? if  0 0 0 $87 $148 obstacle-in  else  0 0 0 $69 $12A obstacle-in  then
    7 hole-floor floor-on
    om1f-15-refined-antimony fact? 0= if  antimony-fx 36.86e 51e -27.91e flicker-at  then
    om1f-15-antimony-tube-found fact?  om1f-15-antimony-tube fact? 0= and if  tube-fx 39.16e 1e -38.76e flicker-at  then
    om1f-15-rubrum-medallion-found fact?  om1f-15-rubrum-medallion fact? 0= and if  medallion-fx -23.61e 51e -61.37e flicker-at  then ;

\ ---- Hewie through the shelves' moving: down, hidden away; else let go, then back by her ----
: hewie-held ( -- )  hewie hold  hewie false shown  begin  yield  again ;
: hewie-for-scene ( -- )
    hewie-in?  hewie-way-now @ 0= and 0= dup hewie-away !  if  exit  then
    hewie-cond-now @ down-cond = if  ['] hewie-held start drop  else  hewie let-go  then ;
: hewie-after ( tri deg F: x z -- )
    {: tri deg F: x z :}  hewie-away @ if  exit  then
    hewie-cond-now @ down-cond = if  ['] hewie-held stop-doing  hewie true shown  hewie let-go  exit  then
    hewie activate  old-mansion-1f-15 tri 0 hewie-to  hewie tri deg x z place-at ;

\ ---- a book laid on a lectern (book b on lectern p: the items' event p * 3 + b) ----
create lectern-spots   $36 , -90 , 35e f, -52e f, 25e f, 20e f, -50e f, 2e f,
    $C5 , 90 , 30e f, 49e f, 25e f, 20e f, 50e f, 2e f,   $C6 , 180 , 0e f, 43e f, 30e f, 0e f, 0e f, 2e f,
2 cells 6 floats + constant /lectern-spot
: to-lectern ( spot -- )   \ (walked to it, its shot)
    {: a :}  stalkers-stay state!  fiona hold  fiona wait-move
    fiona  a @  a cell+ @  a 2 cells + f@  a 2 cells + float+ f@  walk-to
    1  a 2 cells + 2 floats + f@  a 2 cells + 3 floats + f@  a 2 cells + 4 floats + f@  a 2 cells + 5 floats + f@  shot ;
: at-lectern ( p -- )  /lectern-spot * lectern-spots + to-lectern ;
: laid ( p b -- )   \ (the book up on it)
    {: p b :}  fiona reach-up-anim play  book-sound room-bank fiona sound-by
    p b book-part dup 0< if  drop  else  show-part  then
    p 3 * b + books-placed + fact!  b book-item spend  fiona reach-down-anim play ;
: its-own ( p -- )   \ (the lectern's own author: its shelves slide back - the way through)
    {: p :}  hewie-for-scene
    fiona reach-up-anim play  book-sound room-bank fiona sound-by
    p case  0 of  8  endof  1 of  4  endof  >r 0 r>  endcase  object-hide
    p p 3 * + books-placed + fact!  p book-item spend  fiona reach-down-anim play ;
: lays-0 ( b -- )
    {: b :}  0 at-lectern
    b 0= if
        0 its-own  2 place-x shelves-back  shelves-stop-sound room-bank 0e -52.5e 10e sound-there
        4 barred-floor floor-off  5 barred-floor floor-on  16 frames  shot-off  $DD 60 -27e 37e hewie-after
    else  0 b laid  4 frames  shot-off  then
    stalkers-stay state-off ;
: lays-1 ( b -- )
    {: b :}  1 at-lectern
    b 1 = if
        3 story.base:things-off-group
        $18A hole-floor floor-tri-off  $1D3 hole-floor floor-tri-off
        $187 hole-floor floor-tri-on  $1D0 hole-floor floor-tri-on  $188 hole-floor floor-tri-on  $1D1 hole-floor floor-tri-on
        1 its-own  fiona look-point-move 0 0  40e 15e 25e 0e  move  yield
        1 place-z shelves-back  shelves-stop-sound room-bank 39e 21e 10e sound-there
        2 barred-floor floor-off  3 barred-floor floor-on  16 frames  shot-off  $16 -100 -41e -4e hewie-after
        fiona -1 look-at  yield
    else  1 b laid  4 frames  shot-off  then
    stalkers-stay state-off ;
: lays-2 ( b -- )
    {: b :}  2 at-lectern
    b 2 = if
        2 its-own  0 place-z shelves-back  shelves-stop-sound room-bank 0e 0e 10e sound-there
        0 barred-floor floor-off  1 barred-floor floor-on  6 floor-flag-200 floor-on  exits-redone
        16 frames  shot-off  $FF 90 38e -20e hewie-after
    else  2 b laid  4 frames  shot-off  then
    stalkers-stay state-off ;
own: laid-book
: lay-0 ( -- )  laid-book @ lays-0 ;   : lay-1 ( -- )  laid-book @ lays-1 ;   : lay-2 ( -- )  laid-book @ lays-2 ;
: items-used ( who ev var value -- handled? )
    2drop  swap 0<> if  drop false exit  then
    dup 0 9 within 0= if  drop false exit  then
    3 /mod swap laid-book !  case  0 of  ['] lay-0  endof  1 of  ['] lay-1  endof  >r ['] lay-2 r>  endcase  start drop  true ;

\ ---- a book taken back off a lectern not its own; else the lectern's author ----
: takes-back ( p -- )
    {: p :}  p at-lectern
    3 0 do  p i on-lectern?  p i book-part 0< 0= and if
        fiona reach-up-anim play  p i book-part hide-part  p 3 * i + books-placed + fact-off
        fiona reach-down-anim start-anim  i book-item item-named  i book-item 1 give-item  i book-item show-item-tab
        fanfare  obtained-2-msg say  32 frames  fiona wait-anim  shot-off  stalkers-stay state-off  unloop exit
    then  loop
    p case  0 of  lectern-0-msg  endof  1 of  lectern-1-msg  endof  >r lectern-2-msg r>  endcase  say  4 frames
    shot-off  stalkers-stay state-off ;
: other-book? ( p -- flag )  {: p :}  false  3 0 do  p i on-lectern?  p i book-part 0< 0= and or  loop ;
: lectern-0 ( -- )  0 takes-back ;   : lectern-1 ( -- )  1 takes-back ;   : lectern-2 ( -- )  2 takes-back ;
: lectern-offer ( p xt -- )  swap other-book? if  calm-only  else  unhurried  then  offer ;

\ ---- the books to find ----
: finds ( part fact book msg F: x z -- )   \ (turned to it, said, reached for, taken)
    {: part f b msg F: x z :}  fiona wait-move  fiona x z turn-to-xz  msg say
    fiona reach-out-anim play  4 frames  part hide-part  b book-item f -1 takes-the  fiona reach-back-anim play ;
: on-the-table ( -- )  1 got-cathedral-secrets 0 table-book-msg -35e 66e finds ;
: under-the-stand ( -- )
    fiona wait-move  fiona -25e -60e turn-to-xz  stand-book-msg say
    fiona pick-up-anim play  4 frames  2 hide-part  occult-philosophy got-occult-philosophy -1 takes-the  fiona up-from-pick-anim play ;
: on-a-shelf ( -- )  0 got-wisemans-crafts 2 stands-out-msg 1e -30e finds ;

\ ---- the shelves by author ----
: author ( deg msg -- )  {: deg msg :}  fiona wait-move  fiona deg turn  msg say ;
: door-atop ( -- )
    fiona wait-move
    shelf-remarked @ 0= if  fiona 180 turn  author-6-msg say  -1 shelf-remarked !  exit  then
    fiona 0e -80e turn-to-xz  fiona look-at-anim start-anim  16 frames  door-atop-msg say  0 shelf-remarked !  fiona wait-anim ;

\ ---- what she may take here ----
: take-antimony ( -- )  refined-antimony om1f-15-refined-antimony antimony-fx 36.86e -27.91e picks-up ;
: take-tube ( -- )  antimony-tube om1f-15-antimony-tube tube-fx 39.16e -38.76e picks-up ;
: take-medallion ( -- )  rubrum-medallion om1f-15-rubrum-medallion medallion-fx -23.61e -61.37e picks-up ;
: tube-sniffed ( -- )  om1f-15-antimony-tube-found om1f-15-antimony-tube tube-fx  39.16e 1e -38.76e  hewie-finds ;
: medallion-sniffed ( -- )  om1f-15-rubrum-medallion-found om1f-15-rubrum-medallion medallion-fx  -23.61e 51e -61.37e  hewie-finds ;
: sits-3 ( -- )  -31.67e 0e -62.5e hewie-sits-by ;   : sits-4 ( -- )  -15.49e 0e -61.46e hewie-sits-by ;
: barks-4 ( -- )  -15.49e 0e -61.46e hewie-barks-at ;

\ ---- its sounds: two sets of four in turn ----
: ambient ( -- )
    room-sounds-in? 0= if  exit  then
    1 ambient-wait +!  ambient-wait @ 30 < if  exit  then  0 ambient-wait !
    first-ambient ambient-at @ +  24.15e 0e 76.38e room-sound
    first-ambient-2 ambient-at @ +  -44e 70e 22e room-sound
    ambient-at @ 1+ 4 mod ambient-at ! ;

create cameras  $24 , 0 , -1 ,   $25 , 1 , -1 ,   $27 , 2 , 0 ,   \ (area, set, path)
create entries  0 , -1 ,   1 , -1 ,   0 , -1 ,   0 , -1 ,   \ (by exit 0..3: the camera)
create ahead  4 , 1 ,   5 , 0 ,   6 , 2 ,   7 , 3 ,   \ (area, exit loaded ahead)
: facing? ( area deg -- flag )  {: a d :}  fiona a in-area?  fiona d 50 heading? and ;
: hewie-about ( -- )
    got-occult-philosophy fact? if  exit  then
    noticed-3 100 -31.67e 0e -62.5e 11e 10e notices? if  ['] sits-3 start drop  then
    noticed-4 100 -15.49e 0e -61.46e 11e 10e notices? if  ['] sits-4 start drop  then
    noticed-4b 100 -15.49e 0e -61.46e 11e 10e notices? if  ['] barks-4 start drop  then ;
: sniffed ( -- )
    scrambling? 0= if  exit  then
    om1f-15-antimony-tube-found fact? 0=  hewie 39.16e 0e -38.76e 10e 5e touches? and if  ['] tube-sniffed start-only  then
    om1f-15-rubrum-medallion-found fact? 0=  hewie -23.61e 50e -61.37e 10e 5e touches? and if  ['] medallion-sniffed start-only  then ;
: shelves-by-author ( -- )
    books-placed fact? 0= if
        $C 45 facing? if  [: 90 author-1-msg author ;] unhurried offer  then
    else
        $10 -45 facing? if  [: -90 author-1-msg author ;] unhurried offer  then
        $F 45 facing? if  [: 90 author-1-msg author ;] unhurried offer  then
    then
    lectern-1-book-1 fact? if  $13 45 facing? if  [: 90 author-2-msg author ;] unhurried offer  then  then
    lectern-2-book-2 fact? if
        $17 90 facing? if  [: 180 author-3-msg author ;] unhurried offer  then
        $18 0 facing? if  [: 0 author-3-msg author ;] unhurried offer  then
    else  $15 0 facing? if  [: 0 author-3-msg author ;] unhurried offer  then  then
    $19 90 facing? if  [: 180 author-3-msg author ;] unhurried offer  then
    $1B 0 facing? if  [: 0 author-3-msg author ;] unhurried offer  then
    $1C 90 facing? if  [: 180 author-4-msg author ;] unhurried offer  then
    $1D 0 facing? if  [: 0 author-4-msg author ;] unhurried offer  then
    $20 -45 facing? if  [: -90 author-5-msg author ;] unhurried offer  then
    fiona $21 in-area?  fiona $22 in-area? or  fiona 45 50 heading? and if  [: 90 author-5-msg author ;] unhurried offer  then
    $1E 90 facing? if  [: 180 author-5-msg author ;] unhurried offer  then
    $1F 0 facing? if  [: 0 author-5-msg author ;] unhurried offer  then
    books-placed fact? 0=  $1A 90 facing? and if  ['] door-atop unhurried offer  then ;
: each-frame ( -- )
    4 exits-out  cameras 3 cameras-at  $26 1 -1 camera-out  ahead 4 ways-out  ambient
    0 $148 obstacle-on-tri? if  om1f-15-flag1 fact!  else  om1f-15-flag1 fact-off  then
    sniffed  hewie-about
    got-cathedral-secrets fact? 0=  fiona 9 50 faces-area? and if  ['] on-the-table calm-only offer  then
    got-occult-philosophy fact? 0=  fiona $B in-area? and  om1f-15-flag1 fact? and if  ['] under-the-stand calm-only offer  then
    got-wisemans-crafts fact? 0=  $A 90 facing? and if  ['] on-a-shelf calm-only offer  then
    books-placed fact? 0=  $D -45 facing? and if  0 ['] lectern-0 lectern-offer  then
    lectern-1-book-1 fact? 0=  $11 45 facing? and if  1 ['] lectern-1 lectern-offer  then
    lectern-2-book-2 fact? 0=  $14 90 facing? and if  2 ['] lectern-2 lectern-offer  then
    shelves-by-author
    8 22 facing?  fiona $23 50 faces-area? or if  ['] save-clock calm-hewie-up offer  then
    om1f-15-refined-antimony fact? 0=  fiona 36.86e 50e -27.91e 5e 5e touches? and if  ['] take-antimony calm-only offer  then
    om1f-15-antimony-tube-found fact?  om1f-15-antimony-tube fact? 0= and  fiona 39.16e 0e -38.76e 5e 5e touches? and if
        ['] take-tube calm-only offer
    then
    om1f-15-rubrum-medallion-found fact?  om1f-15-rubrum-medallion fact? 0= and  fiona -23.61e 50e -61.37e 5e 5e touches? and if
        ['] take-medallion calm-only offer
    then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    entries 4 ways-in  came-by @ dup 2 =  swap 3 = or if  3 map-page  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-1f-15 place!
' items-used old-mansion-1f-15 items-here
