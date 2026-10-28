\ story/places/old-mansion-3f-2.fs - the armour's corridor of the old mansion's 3F: the first time
\ in, its scene; till the button on the armour's back is pushed, the floor's plates set it off - her
\ first step onto a new plate a warning shot (its scene), the second another, the third her death
\ (the game over); Hewie, kept off the plates, can be sent along them (her "go there" toward either
\ end; "come back" to her; called while going, he sits and waits); the button (pushed: the trap
\ disarmed, his trust up); Hewie tense at the corridor (its files), his look; the specks in the
\ light; the replays. A place (story/kit.fs). The original's scripts, for checking:
\ story/rooms-ref/old-mansion-3f-2.fs in the history (converted from the game's; gone with the old
\ machinery in 0.2.20).
IN: story.places.old-mansion-3f-2
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    floor-flags effects story.kit fiona.anims scenes ;

own-reset

0 constant button-msg   \ "There is a button located on the back of the armor. Push the button? YES NO"
1 constant trap-msg     \ "Looks like the button was the activation switch for a booby-trap."
own: plate-on   \ the plate (nav group 1..3) she stood on last frame, 0 none
own: shots      \ the plates' scenes so far (0..2)
own: armor-on   \ a plate's scene under way
own: order      \ Hewie's: 0 none, 1 to the far end, 2 to the near end, 3 to her, 4 sit and wait
own: corridor-noticed
1 constant armor-sound   2 constant button-sound
1 constant intro-movie   0 constant intro-cutscene
create shot-movies  3 , 2 ,   5 , 4 ,   7 , 6 ,   \ (each plate's movie and cutscene)
$187 constant far-tri   $11F constant near-tri   $195 constant back-tri   $30 constant death-seen
1 constant to-far   2 constant to-near   3 constant to-her   4 constant sit-wait

\ ---- as she comes in ----
: specks-about ( -- )
    6 $A $80 $80 $80 $40 rgba 0e 24e -60e specks   6 $A $80 $80 $80 $40 rgba 0e 24e 12e specks
    6 $A $80 $80 $80 $40 rgba 0e 24e 36e specks    6 $A $80 $80 $80 $40 rgba 0e 24e 90e specks ;
: hewie-kept-off ( on? -- )  {: on :}  4 0 do  i keeps-hewie-off  on if  floor-on  else  floor-off  then  loop ;
: hewie-off-plates ( far? -- )   \ (left on the plates' floor: put at one end)
    hewie-in?  hewie 0 on-group? and 0= if  drop exit  then  if  hewie far-tri  else  hewie back-tri  then  put-on-tri ;
: came-in ( exit -- )
    armor-button-pushed fact? 0= if  dup 0= hewie-off-plates  true hewie-kept-off  then  drop
    room-sounds  specks-about  1 $2300 reverb ;

\ ---- Hewie through a scene: down, hidden away till it's over ----
: hewie-held ( -- )  hewie hold  hewie false shown  begin  yield  again ;
: hewie-hidden-if-down ( -- )  hewie-in?  hewie-cond-now @ down-cond = and if  ['] hewie-held start drop  then ;
: hewie-back ( -- )  hewie-in?  hewie-cond-now @ down-cond = and if  ['] hewie-held stop-doing  hewie true shown  hewie let-go  then ;

\ ---- the first time in: its scene ----
: intro-scene ( -- )  intro-movie scenes:unseen-movie intro-cutscene load-scene  0 hide-part  panic-eased  if  -1 run-scene  then ;
: first-in ( -- )
    stalkers-stay state!  fiona hold  intro-movie scenes:unseen-movie intro-cutscene load-scene  hewie-hidden-if-down
    0 hide-part  panic-eased  if  -1 run-scene  then  wait-fade
    fiona 0 idle  fiona $135 171 0.064e 100.888e place-at  camera-restart  hewie-back  om3f-2-flag1 fact!
    15 fade-in  stalkers-stay state-off ;

\ ---- the plates: a warning shot, another, then her death ----
: armor-back ( -- )  $17 8 do  i object-hide  loop  0 show-part ;   \ (the bolts and fixings gone, the armour as it was)
: shot-scene ( n -- movie? )   \ (the plate's scene n)
    2* cells shot-movies +  dup @ scenes:keyed-movie  rot cell+ @ load-scene ;
: warned ( n -- )
    stalkers-stay state!  fiona hold  -1 armor-on !  15 screen:to-black-2 with-music or fade
    shot-scene  wait-fade  hewie held? 0= if  hewie-hidden-if-down  then
    fiona false shown  0 hide-part  $14 $78 movie-key  panic-eased  if  -1 run-scene  then  wait-fade
    hewie-back  fiona true shown  armor-back  15 fade-in  0 armor-on !  stalkers-stay state-off ;
: first-shot ( -- )  0 warned ;   : second-shot ( -- )  1 warned ;
: shot-dead ( -- )
    capture-no-end state!  stalkers-stay state!  fiona hold  no-pause state!  -1 armor-on !
    15 screen:to-black-2 with-music or fade  2 shot-scene  wait-fade
    hewie let-go  fiona false shown  0 hide-part  $10 $46 movie-key  panic-eased  if  -1 run-through  then
    death-seen resident-flag-set  gameover:movie-death game-over ;
: plate-now ( -- group )   \ (the plate she stands on, 0 none)
    4 1 do  fiona i on-group? if  i unloop exit  then  loop  0 ;
: plates ( -- )   \ (her step onto another plate: the armour)
    plate-now  dup plate-on @ <>  over 0<> and  armor-on @ 0= and if
        armor-sound room-bank fiona sound-by  panic-eased
        shots @ case  0 of  ['] first-shot  endof  1 of  ['] second-shot  endof  >r ['] shot-dead r>  endcase  start drop
        1 shots +!  -1 armor-on !
    then  plate-on ! ;

\ ---- Hewie sent along the plates: to either end, to her; called on the way, he sits and waits ----
: far-way? ( -- flag )  fiona 50 0e -55e faces-point?  fiona 50 0e -100e faces-point? and ;
: near-way? ( -- flag )  fiona 50 0e 55e faces-point?  fiona 50 0e 110e faces-point? and ;
: sent ( -- order | 0 )   \ (her "go there" toward an end)
    started-now @ dup go-there-cmd =  swap go-for-it-cmd = or 0= if  0 exit  then
    far-way? if  to-far exit  then  near-way? if  to-near exit  then  0 ;
: new-order ( -- order | 0 )   \ (what she asks of him now, by what he is doing)
    order @ {: o :}
    o to-far =  o to-near = or 0= if  sent ?dup if  exit  then  then
    o to-her <>  started-now @ come-back-cmd = and if  to-her exit  then
    o 1 4 within  started-now @ come-cmd =  armor-on @ or and if  sit-wait exit  then
    0 ;
: hewie-moved? ( -- flag )  hewie person-of dup 0< if  drop true exit  then  cells move-done + @ 0<> ;
: runs-to ( tri -- )   \ (barking, off along the plates)
    {: t :}  hewie-bark  hewie wait-move  0 keeps-hewie-off floor-off
    hewie run-move t $204  t tri-center 0e  move ;
: along ( o -- done? )   \ (till he gets there, or is asked otherwise)
    begin  order @ over <> if  drop false exit  then  hewie-moved? if  drop true exit  then  yield  again ;
: halted ( -- )  hewie idle-step-move 0 -1  0e 0e 0e 0e  move  hewie wait-move ;
: to-end ( tri -- )
    runs-to  order @ along if  0 keeps-hewie-off floor-on  0 order !  else  halted  then ;
: to-her-tri ( -- )
    fiona body-tri dup 0< if  drop 0 order ! exit  then  runs-to
    to-her along if  hewie 0 on-group? if  sit-wait  else  0 keeps-hewie-off floor-on  0  then  order !  else  halted  then ;
: sits ( -- )
    hewie $100 play  hewie 1 start-anim  hewie wait-move  hewie fiona look-at  yield
    begin  order @ sit-wait = while  yield  repeat
    hewie -1 look-at  yield  hewie $101 play  hewie -1 idle ;
: guided ( -- )   \ (an activity: his orders, till he's done)
    hewie hold
    begin  order @ while
        order @ case
            to-far of  far-tri to-end  endof
            to-near of  near-tri to-end  endof
            to-her of  to-her-tri  endof
            sit-wait of  sits  endof
        endcase
    repeat ;
: orders ( -- )
    armor-button-pushed fact? if  exit  then
    ['] guided running? if  new-order ?dup if  order !  then  exit  then
    hewie-in? 0=  hewie-cond-now @ down-cond = or  stalker-in? or  hewie-breakable-now @ 0= or if  exit  then
    sent ?dup 0= if  started-now @ come-back-cmd = if  to-her  else  0  then  then
    ?dup if  order !  ['] guided start drop  then ;

\ ---- the button on the armour's back ----
: the-armor ( -- )
    fiona wait-move  fiona 0e -68e turn-to-xz
    armor-button-pushed fact? if  trap-msg say exit  then
    fiona look-at-anim start-anim  16 frames  button-msg ask  fiona wait-anim  0<> if  exit  then
    fiona reach-up-anim play  button-sound room-bank fiona sound-by  armor-button-pushed fact!  30 trust+
    false hewie-kept-off  fiona reach-down-anim play ;
: tense ( -- )  -0.27e 0e 10.71e hewie-barks-tense ;

\ ---- the replays (the extras) ----
: replay-intro ( -- )  fiona hold  specks-about  intro-scene  replay-over ;
: replay-plates ( -- )
    fiona hold  specks-about  fiona false shown
    0 shot-scene  $14 $78 movie-key  panic-eased  if  -1 run-scene  then
    1 shot-scene  $14 $78 movie-key  panic-eased  if  -1 run-scene  then
    2 shot-scene  $10 $46 movie-key  panic-eased  if  -1 run-scene  then
    fiona true shown  replay-over ;

create cameras  4 , 0 , -1 ,   5 , 1 , 1 ,   \ (area, set, path)
create entries  1 , 1 ,   0 , -1 ,   \ (by exit 0, 1: the camera)
create ahead  2 , 0 ,   3 , 1 ,   \ (area, exit loaded ahead)
create looks  0e 0e -68.2e  18e 19e 19e look,
: each-frame ( -- )
    2 exits-out  cameras 2 cameras-at  ahead 2 ways-out
    armor-button-pushed fact? 0= if
        orders  plates
        corridor-noticed @ if  $23C give-file  $8281 give-file  then
        corridor-noticed 100 -0.27e 0e 10.71e 81e 8e notices? if  ['] tense start drop  then
    then
    looks 1 hewie-looks-unchased
    fiona 6 in-area?  fiona 65 0e -68e faces-point? and if  ['] the-armor unhurried offer  then ;
: person-in ( i -- )
    fiona-p <> if  exit  then
    events-held state? if
        hewie send part-from-fiona  replay-act $A = if  ['] replay-intro  else  ['] replay-plates  then  start drop  exit
    then
    entries 2 ways-in
    om3f-2-flag1 fact? 0= if  panic-eased  ['] first-in start drop  then ;
: went ( -- )  armor-button-pushed fact? 0= if  fiona 0 in-area? hewie-off-plates  then ;

' came-in ' person-in ' each-frame ' went old-mansion-3f-2 place!
