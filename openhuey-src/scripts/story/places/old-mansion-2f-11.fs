\ story/places/old-mansion-2f-11.fs - the library of the old mansion's 2F: the rusty stepladder with
\ something on top (struck, it wobbles - the third time it falls: the Jupiter Key on the floor); the
\ books, their remarks in turn; the journals (and their file); the two notes on combinations (their
\ recipes marked); the save clock; the Magnesia; the door unlocked from this side; Hewie sitting by
\ one spot, his look; in the other game a homunculus dropped in, once; its ambient sounds; the
\ doorway's shade. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-2f-11.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-2f-11
USING: engine game-state actors messages common vectors facts room-names flag-names story-names
    items.names story.kit fiona.anims ;

own-reset

0 constant ladder-msg      \ "There is a rusty stepladder here. It really is in bad shape. If I were to try climb..."
1 constant on-top-msg      \ "There is something on top of the stepladder... I bet if I shake it a bit, I can get..."
2 constant journals-msg    \ "Mountains of books have been piled up here. Are these journals?"
3 constant dates-msg       \ "The dates on these journals are just too spread out for the same man to have writte..."
4 constant journal-msg     \ "February 8th, 1784 I have decided to summon that renowned master alchemist to the castl..."
5 constant books-msg       \ "What an incredible number of books."
6 constant languages-msg   \ "They are written in various languages. Whomever read all of these must have had a s..."
7 constant titles-msg      \ "Hmpf! What am I doing? It's not like I can even read the titles or anything."
8 constant written-msg     \ "Something is written here..."
9 constant precarious-msg  \ "The grand refining process can also yield several precarious items as well. I have list..."
$A constant animals-msg    \ "Some combinations can be used to aid animals as well. Here are a few: Esca Slightly re..."
$B constant wonder-msg     \ "I wonder what these combinations are supposed to mean?"
own: ladder-remarked  own: book-looks  own: tries  own: spot-noticed
0 constant key-fx   1 constant magnesia-fx   0 constant bounce-sound   7 constant wobble-sound   3 constant first-ambient
$237 constant journal-file   $67 constant near-door
create key-at /vec allot   fvariable key-speed

\ ---- the key on the stepladder: shaken nearer the edge, at last falling (bouncing till it rests) ----
: key-moved ( -- )  key-fx live-effect 0= if  drop exit  then  >r  key-at vec@ xyz>cells  r> send effect-place ;
: key-rest ( n -- )  key-fx live-effect if  send effect-rest  else  2drop  then ;
: key-up-there ( -- )  5.19e 35.3e -34e key-at vec!  key-fx key-at vec@ flicker-at  0e key-speed f! ;
: key-on-floor ( -- )  key-fx 7.3e 0.7e -28.3e flicker-at ;
: shaken-nearer ( -- )  key-at v.z sf@ 0.5e f+ key-at v.z sf!  key-moved ;
: bounced? ( -- flag )   \ (off the floor at 0.7, keeping 30% - slower than 0.2: at rest)
    key-at v.y sf@ 0.7e f< 0= if  false exit  then
    0.7e key-at v.y sf!  key-speed f@ -0.3e f* key-speed f!  bounce-sound room-bank key-at vec@ sound-there
    key-speed f@ 0.2e f< ;
: falls ( -- )
    begin
        0 key-rest  key-at v.z sf@ 0.16e f+ key-at v.z sf!
        key-speed f@ 0.5e f- key-speed f!  key-at v.y sf@ key-speed f@ f+ key-at v.y sf!
        bounced? if  key-moved  1 key-rest  exit  then
        key-moved  yield
    again ;

\ ---- the stepladder's two parts wobbling (the room's objects "step" and "book0") ----
: wobble-start ( addr len -- )  2dup placed-here? 0= if  2drop exit  then  {: a n :}  a n swing-phase 0e object!  a n swing-offset 0.01e object! ;
: wobbling ( addr len -- )   \ (its tilt the size times the sine of its phase; the phase on 60 a frame, the size down 0.001 till still)
    2dup placed-here? 0= if  2drop exit  then  {: a n :}
    begin
        a n swing-offset object@  a n swing-phase object@ deg>rad fsin f*  a n turn-x object!
        a n swing-phase 60e object+!  a n swing-offset -0.001e object+!
        a n swing-offset object@ f0> while  yield
    repeat ;
: wobble-heard ( -- )  s" step" placed-here? 0= if  exit  then
    wobble-sound room-bank  s" step" place-x object@  s" step" place-y object@  s" step" place-z object@  sound-there ;
: wobbles ( -- )  s" step" wobble-start  s" book0" wobble-start  wobble-heard  s" step" wobbling  s" book0" wobbling ;
: struck ( -- )   \ (she strikes it: it wobbles - the third time the key falls)
    ['] wobbles start drop  1 tries +!
    tries @ 3 = if  om2f-11-flag1 fact!  falls  key-on-floor  else  shaken-nearer  then
    begin  fiona-free? 0= while  yield  repeat ;

\ ---- as she comes in ----
: came-in ( exit -- )
    drop  room-sounds  0 ambient-step !  0 ambient-frames !
    om2f-11-flag1 fact? 0= if  key-up-there  else  got-jupiter-key fact? 0= if  key-on-floor  then  then
    om2f-11-magnesia fact? 0= if  magnesia-fx -6.82e 1e -10.25e flicker-at  then
    items-40-as-70 fact?  om2f-11-flag2 fact? 0= and if
        om2f-11-flag2 fact!  old-mansion-2f-11 0 15 $80 8 $5A 0e creature-dropped
    then ;

\ ---- what she may do or say here ----
: take-key ( -- )
    fiona wait-move  fiona 7.3e -28.3e turn-to-xz  fiona pick-up-anim play  4 frames
    jupiter-key got-jupiter-key key-fx takes-the  fiona up-from-pick-anim play ;
: the-ladder ( -- )
    fiona wait-move  fiona 5.19e -34e turn-to-xz
    ladder-remarked @ 0=  om2f-11-flag1 fact? or if  ladder-msg say  -1 ladder-remarked !  exit  then
    fiona look-at-anim play  on-top-msg say  0 ladder-remarked ! ;
: the-journals ( -- )
    stalkers-stay state!  fiona hold  fiona wait-move  fiona -20 turn
    journals-examined fact? if  dates-msg  else  journals-examined fact!  journals-msg  then  say
    15 screen:to-dim fade wait-fade  journal-msg say  15 screen:from-dim fade wait-fade
    journal-file give-file  stalkers-stay state-off ;
: books-turn ( -- deg )   \ (which shelf she stands by: turned to it)
    fiona $B in-area? if  180 exit  then  fiona $C in-area? if  0 exit  then  fiona $D in-area? if  160 exit  then
    fiona $E in-area? if  180 exit  then  fiona $F in-area? if  0 exit  then  -90 ;
: the-books ( -- )
    fiona wait-move  fiona books-turn turn  fiona look-close-anim start-anim  40 frames
    book-looks @ case  0 of  books-msg  endof  1 of  languages-msg  endof  >r titles-msg r>  endcase  say
    book-looks @ 1+ 3 mod book-looks !  fiona wait-anim ;
: a-note ( msg bit bit' -- )   \ (read; its two recipes marked the first time - her wonder)
    {: msg bit bit2 :}  stalkers-stay state!  fiona hold  fiona wait-move  fiona -90 turn  written-msg say
    15 screen:to-dim fade wait-fade  msg say  15 screen:from-dim fade wait-fade
    bit items:item-bit? 0=  bit2 items:item-bit? 0= or if
        fiona hand-to-mouth-anim start-anim  wonder-msg say  fiona wait-anim
        bit items send item-bit  bit2 items send item-bit
    then  stalkers-stay state-off ;
: for-animals ( -- )  animals-msg $63 $65 a-note ;
: precarious ( -- )  precarious-msg $46 $47 a-note ;
: take-magnesia ( -- )  magnesia om2f-11-magnesia magnesia-fx -6.82e -10.25e picks-up ;
: unlocks-it ( -- )  0 unlocks ;
: sits-by ( -- )  21.19e 0e -20.13e hewie-sits-by ;

create cameras  5 , 0 , 0 ,   7 , 1 , 1 ,   \ (area, set, path - crossing into it)
create looks  19.29e 0e -1.64e  20e 22e 22e look,
: ladder-struck? ( -- flag )   \ (her blow on it - one of its three spots)
    3e 0e -35.5e 9e 15e her-blow-in?  -5.75e 0e -33.25e 9e 15e her-blow-in? or  -14.5e 0e -31e 9e 15e her-blow-in? or ;
: shelves? ( -- flag )
    fiona $B in-area?  fiona 180 50 heading? and  fiona $C in-area?  fiona 0 50 heading? and or
    fiona $D in-area?  fiona -160 50 heading? and or  fiona $E in-area?  fiona 180 50 heading? and or
    fiona $10 in-area?  fiona -90 50 heading? and or ;
: each-frame ( -- )
    0 exit-out  1 1 leaves-by  cameras 2 cameras-at  4 1 1 camera-out  6 2 -1 area-camera
    fiona 2 entered? if  0 prepare-exit  then  fiona 3 entered? if  1 prepare-exit  then
    om2f-11-flag1 fact? 0=  ladder-struck? and if  ['] struck start-only  then
    spot-noticed 30 21.19e 0e -20.13e 12e 10e notices? if  ['] sits-by start drop  then
    looks 1 hewie-looks-at
    room-sounds-in? if  first-ambient 37.86e 0e -38.87e ambient-sounds  then
    1 locked-request?  fiona 0 before-door? and if  ['] unlocks-it any-time offer  then
    om2f-11-flag1 fact?  got-jupiter-key fact? 0= and  fiona 7.3e 0.7e -28.3e 5e 5e touches? and if  ['] take-key calm-only offer  then
    fiona 8 in-area?  fiona 180 50 heading? and if  ['] save-clock calm-hewie-up offer  then
    fiona 9 in-area?  fiona 120 50 heading? and if  ['] the-ladder unhurried offer  then
    fiona $A in-area?  fiona -20 50 heading? and if  ['] the-journals unhurried offer  then
    shelves? if  ['] the-books unhurried offer  then
    om2f-11-magnesia fact? 0=  fiona -6.82e 0e -10.25e 5e 5e touches? and if  ['] take-magnesia calm-only offer  then
    fiona $11 in-area?  fiona -90 50 heading? and if  ['] for-animals unhurried offer  then
    fiona $12 in-area?  fiona -90 50 heading? and if  ['] precarious unhurried offer  then
    -7e 30e 42e  -10.8e 30e 29.2e  -7e 0e 42e  -10.8e 0e 29.2e  light-doorway ;
: person-in ( i -- )  fiona-p = if  0 1 1 way-in  1 1 1 way-in  then ;
: went ( -- ) ;

' came-in ' person-in ' each-frame ' went old-mansion-2f-11 place!
