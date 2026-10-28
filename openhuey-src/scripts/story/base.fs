\ story/base.fs - what the story keeps of the converted rooms' machinery (story/words.fs, gone with
\ them once every room was a place): the story's character slots (the stalker the story loaded -
\ partner-load, slot 2 - and the scenes' figures in slots 3..5), the stalkers' gifts as she
\ searches one, the maze dealt anew, the screen filled black for a frame, and the few pieces the
\ places and the kit use as the original has them (Daniella's held things, the speech faces, the
\ falling stones, the things lying about, a door swung along an animation, the pad's rumble).
IN: story.base
USING: engine game-state facts actors messages common keys vectors doors room-names flag-names
    story-names stalker-names effects items items.names things ;

\ ---- the story's character slots: 0 Fiona, 1 Hewie, 2 the stalker, 3..5 the scenes' figures ----
enum: char-slot#  fiona-cs hewie-cs stalker-cs first-figure-cs ;
6 constant #chars
: figure-cs? ( cs -- flag )  first-figure-cs #chars within ;
\ the stalker's slot: the stalker actor partner-load brought in (the original's progress
\ character table), -1 none; the kind it was loaded as (its +0x153C)
variable slot2   -1 slot2 !
variable slot2-kind   -1 slot2-kind !
strings: figure-actor-name  3 cast3 cast4 cast5 ;   \ (by slot)
: cast-actor ( slot -- id | -1 )   \ the event characters' figures (cast.fs), by their slot
    figure-actor-name dup if  actor-named  else  2drop -1  then ;
\ the actor in each slot (the stalkers by kind, the figures) and the character's id in it
create slot-actors  #chars cells allot  slot-actors #chars cells -1 fill
create slot-chars  #chars cells allot  slot-chars #chars cells -1 fill
: slot-actor ( slot -- addr )  cells slot-actors + ;
: slot-id ( slot -- addr )  cells slot-chars + ;
\ the stalker actors by kind (stalker-kind#: Debilitas and his figures, Daniella and her later
\ kinds 34..36, Riccardo, Lorenzo and his other class, ...)
strings: stalker-actor-name  2 debilitas daniella riccardo  6 debilitas debilitas  10 lorenzo2 lorenzo kind12
    23 tintstalker  27 debilitas  34 kind34 kind35 kind36 kind37  39 kind39 ;
: stalker-actor ( kind -- id | -1 )  stalker-actor-name dup if  actor-named  else  2drop -1  then ;
: partner-load ( kind -- )   \ (CharLoad_Partner) that kind into slot 2, loaded and started; the summoner afresh
    slot2 @ dup 0< 0= if  send stalker-unload  else  drop  then
    dup slot2-kind !  dup stalker-actor dup slot2 !  dup 0< if  2drop exit  then
    tuck send stalker-as  stalker-cs swap send stalker-load
    summoner-id send summoner-reset ;
: slot-unload ( -- )  slot2 @ dup 0< if  drop exit  then  send stalker-unload  -1 slot2 ! ;
: slot-clear ( slot -- )   \ whoever is in a slot let go (a figure unloaded, a stalker out)
    dup slot-actor @ dup 0< if  2drop exit  then                              ( slot a )
    over cast-actor over = if  send cast-unload  else  send stalker-unload  then
    -1 over slot-actor !  -1 swap slot-id ! ;

\ ---- the stalker's gifts (0xB6, the shared $80: she searches one down on his front): after the
\ first only 1 in 10 times; the next of his kind's row she has under 99 of (one "only if missing"
\ she has becomes the Magist Medallion), with message $8011 naming it; none left: $801A ----
: script-item ( id -- id' )  items-40-as-70 story-flag? over $40 - 2 u< and if  drop $70  then ;
8 constant gifts-a-row
: gift ( item -- )  0 c, c, ;   : gift-if-missing ( item -- )  1 c, c, ;
create gift-rows   \ (D_003D6A90: eight a row, each an item - or one given only if she lacks it)
    \ row 0
    chicken-jerky gift  beef-jerky gift  chicken-jerky gift  beef-jerky gift
    chicken-jerky gift  beef-jerky gift  chicken-jerky gift  beef-jerky gift
    \ row 1
    beef-jerky gift  camomile gift  lavender gift  medallion gift
    refined-antimony gift  refined-magnesia gift  antimony-tube gift  prima-magnesia gift
    \ row 2
    camomile gift  lavender gift  medallion gift  refined-antimony gift
    refined-magnesia gift  antimony-tube gift  prima-magnesia gift  ruby-choker gift-if-missing
    \ row 3
    medallion gift  medallion gift  medallion gift  album-medallion gift
    album-medallion gift  antimony-tube gift  prima-magnesia gift  philosopher-earrings gift-if-missing
    \ row 4
    medallion gift  album-medallion gift  album-medallion gift  antimony-tube gift
    prima-magnesia gift  philosopher-earrings gift-if-missing  medallion gift  medallion gift
    \ row 5
    antimony-powder gift  magnesia gift  refined-antimony gift  refined-magnesia gift
    camomile gift  lavender gift  refined-antimony gift  refined-magnesia gift
    \ row 6
    weeping-stone gift  antimony-tube gift  prima-magnesia gift  album-medallion gift
    viola-medallion gift  rubrum-medallion gift  viride-medallion gift  silver-boots gift-if-missing
    \ row 7
    weeping-stone gift  antimony-tube gift  prima-magnesia gift  album-medallion gift
    viola-medallion gift  rubrum-medallion gift  viride-medallion gift  silver-boots gift-if-missing
    \ row 8
    medallion gift  album-medallion gift  viola-medallion gift  rubrum-medallion gift
    viride-medallion gift  magist-medallion gift  ruby-choker gift-if-missing  philosopher-earrings gift-if-missing
40 constant #gift-kinds   \ (the row by his kind - D_003D6A60; the others: row 0)
create gift-row-of  #gift-kinds cells allot  gift-row-of #gift-kinds cells 0 fill
: row-for ( row kind -- )  cells gift-row-of + ! ;
    3 daniella-kind row-for   6 riccardo-kind row-for   1 debilitas-6-kind row-for   2 debilitas-7-kind row-for   8 lorenzo-kind row-for   7 tintstalker-kind row-for   4 kind34-kind row-for   5 kind35-kind row-for   5 kind36-kind row-for   7 kind37-kind row-for
: gift-entry ( kind k -- addr )
    swap dup 0 #gift-kinds within 0= if  drop 0  then  cells gift-row-of + @ gifts-a-row 2* *  swap 2* +  gift-rows + ;
: gift-item ( kind k -- item )   \ (the entry's item; one only-if-missing she has: the Magist Medallion)
    gift-entry dup 1+ c@ script-item  swap c@ if  dup items-of if  drop magist-medallion  then  then ;
$8011 constant gift-message   $801A constant no-gift-message   \ (the first names the item: parameter 0)
: gift-due? ( -- flag )   \ (the first; after it 1 in 10 - counted, up to 8)
    progress pr.gifts l@ 0=  rnd 100e f* 10e f< or  dup if  progress pr.gifts l@ 1+ 8 min progress pr.gifts l!  then ;
: next-gift ( kind -- item | -1 )   \ (the next of his row she has under 99 of, the row moved on past it; -1 none left)
    {: kind :}
    begin  progress pr.gift-next l@ dup gifts-a-row < if  kind swap gift-item items-of 99 <  else  drop true  then  0= while
        progress pr.gift-next dup l@ 1+ swap l!
    repeat
    progress pr.gift-next l@ dup gifts-a-row < 0= if  drop -1 exit  then
    kind swap gift-item  progress pr.gift-next dup l@ 1+ swap l! ;
\ 0xD8: by how Hewie has been treated (from 20, in steps of 20) a hint filed, with its sound
$270 constant first-reward-hint
: reward-item ( -- )
    progress pr.hewie-mistreated l@ dup 20 < if  drop exit  then
    20 - 20 / 3 min first-reward-hint +  -1 items-id send add-file ;

\ ---- two characters close (Actor_Touching): the higher within the lower's height (and a vertical
\ margin) of it, and their radii and a margin apart across the floor; the heading from one to another ----
: x-of ( id -- ) ( F: -- x )  body-at v.x sf@ ;
: y-of ( id -- ) ( F: -- y )  body-at v.y sf@ ;
: z-of ( id -- ) ( F: -- z )  body-at v.z sf@ ;
: radius-of ( id -- ) ( F: -- r )  body-dims fdrop ;
: height-of ( id -- ) ( F: -- h )  body-dims fswap fdrop ;
: touching-by? ( a b -- flag ) ( F: margin vmargin -- )
    {: a b F: margin vmargin :}
    a y-of b y-of f< if  b y-of a y-of f-  a height-of  else  a y-of b y-of f-  b height-of  then
    vmargin f+  f> if  false exit  then
    a body-at b body-at vec-dist-xz   a radius-of b radius-of f+ margin f+  f<= ;
: touching? ( a b margin vmargin -- flag )  {: margin vmargin :}  margin s>f vmargin s>f touching-by? ;
: heading-between ( a b -- ) ( F: -- yaw )   \ from character a toward b
    {: a b :}  b x-of a x-of f-  b z-of a z-of f-  fatan2 ;

\ ---- the maze (0xCE): its six rooms dealt anew while the deals (a progress variable) are under 4 -
\ a row at random opens each room's doors for the rooms it names and closes off the rest; a stalker
\ in them sent on; the exits rebuilt ----
$100 constant first-maze-room   6 constant #maze-rooms   $95 constant first-maze-door
$1F constant deals-var   $20 constant dealt-row-var   4 constant most-deals   79 constant #deal-rows
: deal-rows ( -- addr len )  s" deal-rows" exe-table ;   \ (D_003D6B20: three rooms a row)
$109 constant maze-way-out   $10A constant maze-way-out-2
: deal-var ( n -- addr )  progress pr.vars + ;
: deal-row ( i -- room )   \ the dealt row's i-th room
    dealt-row-var deal-var c@ 3 *  +  deal-rows {: at addr n :}  at n < if  addr at + c@  else  0  then ;
: maze-door ( room side k -- door )  {: r side k :}  first-maze-door  r 12 * +  side 4 * +  k + ;
: deal-room ( r -- )   \ room r's twelve doors - four on each of three sides: open where the row puts it, else closed off
    {: r :}
    3 0 do  i deal-row r =
        4 0 do  r j i maze-door  over if  doors-id send reopen  else  doors-id send close-off  then  loop
        drop
    loop ;
: in-maze? ( room -- flag )  first-maze-room dup #maze-rooms + within ;
: stalker-sent-on ( -- )   \ a stalker in the maze sent elsewhere (its way out, or the other from it)
    slot2 @ dup 0< if  drop exit  then  dup body? 0= if  drop exit  then
    dup body-room in-maze? 0= if  drop exit  then
    room-id maze-way-out = if  maze-way-out-2  else  maze-way-out  then  -1 rot send stalker-in ;
: deal-things ( -- )
    deals-var deal-var c@ most-deals >= if  exit  then
    deals-var deal-var dup c@ 1+ swap c!   #deal-rows random dealt-row-var deal-var c!
    stalker-sent-on
    #maze-rooms 0 do  i deal-room  loop
    doors-id send rebuild-exits ;

\ ---- the screen filled black for a frame (Renderer_FillScreen: alpha 0..$80 as the GS has it),
\ drawn for each frame it is asked for (the place's frame end draws it) ----
16 constant fill-layer   \ (the UI layers: ... 15 the cursor below; 16 this)
15 constant cursor-layer   \ (the aiming cursor's: water-tower-8f-1)
variable fill-alpha
: screen-fill ( a -- )  fill-alpha ! ;
: fill-frame ( -- )   \ (each frame's end)
    fill-layer ui-clear
    fill-alpha @ ?dup if  >r  fill-layer 0 0 screen-size  r> 2* 255 min  ui-rect  then  0 fill-alpha ! ;

\ ---- small pieces as the original has them ----
\ the movie classes (Progress_PlayMovie's kind) as the movie compositor's: keyed by brightness,
\ by its own alpha, a strip; the rest opaque (scenes.fs: unseen-movie ...)
1 constant keyed-class   3 constant alpha-class   6 constant strip-class
: class>compo ( class -- compo )
    case  keyed-class of $31 endof  alpha-class of $21 endof  strip-class of $41 endof  $11 swap  endcase ;
: gs>rgba ( gs -- rgba )   \ (a GS colour, R in the low byte, as $RRGGBBAA)
    dup $FF and 24 lshift  over 8 rshift $FF and 16 lshift or  over 16 rshift $FF and 8 lshift or  swap 24 rshift $FF and or ;
\ the faces' textures (Progress_LoadSpeech: a .TEX file's first texture shown in place of one of
\ the character's own) - the file's name, its \ as /
create speech-file 64 allot  variable speech-len
: speech-load ( addr len -- )
    63 min dup speech-len !  speech-file swap move
    speech-len @ 0 ?do  speech-file i + c@ [char] \ = if  [char] / speech-file i + c!  then  loop ;
\ Daniella's held things (her model's parts 17 and 31): both shown or hidden; one of them shown, her
\ right hand's own shape with it (DaniellaModel_Vt30: the first)
17 constant daniella-part-a   31 constant daniella-part-b
: daniella-parts ( model a? b? -- )  {: m a b :}  m daniella-part-a a actor-part-show  m daniella-part-b b actor-part-show ;
: daniella-one ( model -- )  {: m :}  m true false daniella-parts  1 m actor act.hand-default 4 + l! ;
\ (grey_three) stones falling from `height` (Effect79B00, grey $303030), the room's texture
\ `tex`'s part from (u, v), w across
: stones-falling ( tex F: height u v w -- )  $303030 swap  fdup  effects:stones ;
$1A constant weak-blow-sub   \ (her move's sub for the weak blow)
\ (Room8C) the things lying here in the area, of the kinds kept in the save (0 2 3 5 7 8), aged
\ past every time they keep: they go, or go off
$1AD constant kept-kinds   300000 constant past-keeping
: things-age-out-in ( area -- )
    #things 0 do
        i thing-used? if
            i thing-room room-id =  1 i thing-kind lshift kept-kinds and 0<>  and if
                dup i thing-at vec@ area-in? if  past-keeping i thing-age!  then
            then
        then
    loop  drop ;
\ (Room54) the things lying here on a triangle of nav group g gone
: things-off-group ( g -- )
    #things 0 do
        i thing-used? if
            i thing-room room-id =  i thing-tri 0< 0= and if
                i thing-tri over nav-in-group? if  i s" things" actor-named send thing-taken  then
            then
        then
    loop  drop ;
: door-play ( exit anim -- )  0 doors-id send use-door ;   \ (Doors_StartAnim: its door swung along an animation)
: door-wait ( exit -- )   \ (Doors_IsIdle: waited for - in an activity)
    yield  begin  dup door-animating? while  yield  repeat  drop ;
: door-not-closed-off? ( door -- flag )
    dup 0 door-count within 0= if  drop true exit  then  progress pr.closed-off bit? 0= ;
: rumble-pad ( motor strength frames -- )   \ (0x71) the pad shaken - motor 0 the small one (on), else the large at that strength
    {: motor strength frames :}
    motor 0= if  1 true frames rumble-buzz exit  then   \ (on the story's channel: 1)
    1 strength frames rumble ;
