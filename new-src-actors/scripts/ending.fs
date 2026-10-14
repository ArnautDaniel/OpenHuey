\ ending.fs - the ending (docs/subsystems/ending.md; src/game/scene_ending.c, the original's
\ scene mode 5). When the story ends the game (the game over's kind 2: `the-ending`) the world
\ is let go and the staff roll plays (STAFF_ROLL.SFD - Start skips it once it shows), fading out
\ over 30 frames. Then the results: the ending's picture (SYSTEM/ENDING_A..D.TEX) fades in and
\ half out, the play record shows (SUBSCR/MSG_END.BIN) - the ending, the time, the dog level,
\ her panics, the items found, Hewie's critical injuries, the enemies defeated - with the TYPE
\ it earns, the values it came from highlighted. A button, then what it unlocked (the resident
\ flags: the system data's, kept across games) and, for some types, a word for the word plate;
\ the picture fades out and the game goes back to the title. (The original offers to save the
\ clear data here, for a next game with it: not built.)
\
\ The record is kept as the game goes: the time played here, the rest by their owners (facts.fs:
\ panics, knock-outs; Hewie's counters, hewie/states.fs).
IN: ending
USING: engine game-state actors messages keys facts flag-names ps2-screen ;

13 constant ending-layer   14 constant words-layer   \ (over everything: the picture, the words)
$2E constant picture-group   \ (the texture cache's group for the ending's picture)
$2000 constant end-msg       \ MSG_END's messages: ids $2000.. (message-file)
30 constant per-second
99 3600 *  59 60 * +  59 +  per-second *  constant most-frames   \ (99:59:59)

\ its stages
0 constant playing   1 constant staff-roll   2 constant staff-fade
3 constant results   4 constant unlocks      5 constant word-plate   6 constant done

state: ending-state
  cell field stage
  cell field step      \ within the stage
  cell field timer
  cell field trust     \ Hewie's trust, as he last said (hewie-doing)
  cell field pic-a     \ the picture's alpha, 0..$80
  cell field text-a    \ the record's alpha, 0..$80
  cell field news      \ what it unlocked: 1 the first clear, 2 something new
  cell field kind      \ the TYPE earned (0..10)
  cell field dog       \ the dog level (0 A .. 4 E)
  cell field shown     \ the message up (-1 none): an unlock, the word plate's
  1 floats field volume   \ the staff roll's
end-state

: music-id ( -- id )  s" music" actor-named ;
: bgm ( track -- )  0 1e f>cell music-id send bgm-want ;

\ ==== the record ====
: letter ( -- n )  progress pr.vars $2D + c@ ;   \ the ending: 0 A .. 3 D (the story sets it)
: mistreated ( -- n )  progress pr.hewie-mistreated l@ ;
: pleased ( -- n )  progress pr.hewie-pleased l@ ;
: injuries ( -- n )  progress pr.hewie-downs l@ ;
: defeated ( -- n )  progress pr.knockouts l@ ;
: panics ( -- n )  progress pr.panics l@ ;
: seconds ( -- n )  progress pr.play-frames l@ per-second / ;
: hours ( -- n )  seconds 3600 / ;

\ the items: of the story flags these are, how many were set (SceneGame_BonusFlagsSet)
create bonus-flags
  $222 , $223 , $225 , $226 , $227 , $228 , $229 , $22A , $22B , $22C , $22D , $230 , $231 , $234 , $235 ,
  $236 , $239 , $23B , $23D , $23F , $241 , $243 , $245 , $247 , $249 , $24B , $24D , $24F , $251 , $253 ,
  $255 , $257 , $259 , $25B , $25D , $25F , $260 , $265 , $266 , $267 , $268 , $269 , $26A , $26B , $26C ,
  $26D , $26E , $26F , $270 , $271 , $272 , $273 , $274 , $275 , $276 , $277 , $278 , $27A , $27C , $27D ,
  $27E , $27F , $280 , $281 , $282 , $283 , $284 , $285 , $286 , $287 , $288 , $289 , $28A , $28B , $28C ,
  $28D , $28E , $28F , $290 , $291 , $292 , $293 , $294 , $295 , $296 , $29A , $29C , $29E , $2A0 , $2A2 ,
  $2A4 , $2A6 , $2A8 , $2AA , $2AC , $2AE , $2B0 , $2B2 , $2B4 , $2B6 , $2B8 , $2BA , $2BC , $2BE , $2C0 ,
  $2C2 , $2C3 , $2C4 , $2C5 , $2C6 , $2C7 , $2C8 , $2C9 , $2CA , $2CB , $2D3 , $2D5 , $2D7 , $2D9 , $2DB ,
  $2DC , $2DD , $2DE , $2DF , $2E0 , $2E1 , $2E2 , $2E3 , $2E4 , $2E5 , $2E6 , $2E7 , $2E8 , $2EA , $2EC ,
  $2EE , $2F0 , $2F2 , $2F4 , $2F5 , $2F6 , $2F9 , $2FC , $2FD , $2FF ,
here bonus-flags - cell / constant #bonus-flags
: items-found ( -- n )  0  #bonus-flags 0 do  i cells bonus-flags + @ story-flag? if  1+  then  loop ;
: items-percent ( -- n )  items-found 100 * #bonus-flags / ;

\ the dog level (0 A .. 4 E): his trust, less how he was mistreated (in 20s, at most 4), more
\ how he was pleased (in 10s, at most 3); 10 or more A, under 7 E
: dog-level ( -- n )
    pleased 10 / 3 min  trust @ + 1+  mistreated 20 / 4 min -
    dup 7 < if  drop 4 exit  then
    dup 10 >= if  drop 0 exit  then
    10 swap - ;

\ the TYPE: the first of these that applies (messages 8..$11, $12 for 0)
0 constant unjudged   1 constant dog-beater   2 constant fugitive   3 constant top-breeder
4 constant treasure-hunter   5 constant warmonger   6 constant track-star   7 constant dog-lover
8 constant scream-queen   9 constant speed-walker   10 constant late-bloomer
: type-earned ( -- n )
    mistreated 100 >= if  dog-beater exit  then
    letter 2 = if  fugitive exit  then
    dog @ 0= injuries 0= and if  top-breeder exit  then
    items-percent 80 >= if  treasure-hunter exit  then
    defeated 30 >= if  warmonger exit  then
    hours 3 < if  track-star exit  then
    dog @ 2 < if  dog-lover exit  then
    panics 30 >= if  scream-queen exit  then
    hours 6 < if  speed-walker exit  then
    hours 20 < if  unjudged exit  then
    late-bloomer ;
: type-msg ( n -- msg )  ?dup if  7 +  else  $12  then  end-msg + ;

\ ==== what it unlocks: the resident flags (the system data's +0x24: 0 the first clear, 1..4 by
\ the ending, 5 + the TYPE; 86 a hard game cleared, 87 / 88 with ending A / B) ====
: unlock? ( n -- flag )  resident-flag? ;
: unlock ( n -- )  resident-flag-set ;
86 constant hard-cleared   87 constant hard-a   88 constant hard-b
: some-news ( -- )  news @ 2 or news ! ;
: unlock-new ( n -- )  dup unlock? if  drop  else  unlock  some-news  then ;
: by-ending ( -- )
    letter case
        0 of  1 unlock-new  hard-mode? if  hard-a unlock  then  endof
        1 of  2 unlock      hard-mode? if  hard-b unlock  then  endof
        2 of  3 unlock-new  endof
        3 of  4 unlock  endof
    endcase ;
: by-type? ( -- flag )  kind @ dup top-breeder =  swap dog-lover = or ;
: by-type-before? ( -- flag )  3 5 + unlock?  7 5 + unlock? or ;
: hard-news ( -- )   \ (nothing else new: a hard game's own)
    news @ if  exit  then
    hard-cleared unlock? 0= if
        hard-mode?  by-type-before? by-type? or  and if  some-news  then
    else
        by-type-before? 0=  by-type? and if  some-news  then
    then ;
: unlocks! ( -- )
    0 news !
    0 unlock? 0= if  1 news !  0 unlock  then
    by-ending  hard-news
    hard-mode? if  hard-cleared unlock  then
    kind @ 5 + unlock ;
\ the type's word for the word plate (messages $14..$17: top breeder, treasure hunter,
\ warmonger, track star)
: plate-word? ( -- flag )  kind @ top-breeder track-star 1+ within ;
: plate-msg ( -- msg )  kind @ top-breeder - $14 + end-msg + ;

\ ==== the stages ====
: hold-world ( -- )  world-held state-flag-set  panic-held state-flag-set  1 still! ;
: start ( -- )   \ the game is over: the record kept, the world let be, the staff roll
    trust @ progress pr.hewie-trust l!
    hold-world  movie-close  5 0 0 music-id send music-op  $FF bgm
    1e volume f!  0 step !
    s" STAFF_ROLL.SFD" movie-open if  staff-roll  else  staff-fade  then  stage ! ;
: showing? ( -- flag )  movie-status 1 =  movie-frame 0< 0= and ;
: staff-frame ( -- )   \ (Start skips it once it shows)
    movie-status 1 <>  start-button button-pressed? showing? and  or if  staff-fade stage !  then ;
: picture ( -- addr len )
    letter case
        0 of  s" SYSTEM/ENDING_A.TEX"  endof   1 of  s" SYSTEM/ENDING_B.TEX"  endof
        2 of  s" SYSTEM/ENDING_C.TEX"  endof   >r s" SYSTEM/ENDING_D.TEX" r>
    endcase ;
: setup ( -- )   \ the results: the picture, the words, the record worked out, its music
    picture picture-group tex-bank drop
    s" SUBSCR/MSG_END.BIN" message-file drop
    dog-level dog !  type-earned kind !
    0 pic-a !  0 text-a !  0 news !  -1 shown !  0 step !  0 timer !
    $33 bgm  results stage ! ;
: fade-frame ( -- )   \ (the staff roll's sound and picture out, a 30th a frame)
    movie-status 1 <> if  movie-close  setup exit  then
    volume f@ 1e 30e f/ f- 0e fmax fdup volume f!  movie-volume!
    volume f@ f0= if  movie-close  setup  then ;

: pic-in ( -- flag )  pic-a @ $80 < if  2 pic-a +!  false  else  true  then ;
: pic-to-half ( -- flag )  pic-a @ $40 > if  -2 pic-a +!  false  else  true  then ;
: pic-out ( -- flag )  pic-a @ 0> if  -2 pic-a +!  false  else  true  then ;
: wait-for ( n -- flag )  1 timer +!  timer @ swap >= ;
: next ( -- )  1 step +!  0 timer ! ;
: confirm? ( -- flag )  circle button-pressed?  cross button-pressed? or  start-button button-pressed? or ;

: show ( msg -- )  shown ! ;
: closed? ( -- flag )   \ (the message up taken down by the button)
    shown @ 0< if  true exit  then
    confirm? if  -1 shown !  true  else  false  then ;
: to-word-plate? ( -- flag )
    plate-word? 0= if  false exit  then
    0 step !  0 text-a !  word-plate stage !  true ;
: finish ( -- )  -1 shown !  0 step !  done stage ! ;
: results-frame ( -- )   \ (SceneEnding_SeqResults)
    step @ case
        0 of  pic-in if  next  then  endof
        1 of  90 wait-for if  next  then  endof
        2 of  pic-to-half if  next  then  endof
        3 of  text-a @ $80 < if  4 text-a +!  else  next  then  endof
        4 of  150 wait-for if  next  then  endof
        5 of  confirm? if  next  then  endof
        6 of
            text-a @ 0> if  -8 text-a +!  text-a @ 0 max text-a !  exit  then
            unlocks!
            news @ if  0 step !  unlocks stage !  exit  then
            to-word-plate? 0= if  finish  then  endof
    endcase ;
: unlocks-frame ( -- )   \ (SceneEnding_SeqUnlocks: the first clear's two, then the costumes)
    step @ case
        0 of  pic-in if  next  then  endof
        1 of  news @ 1 and if  $1C end-msg + show  next  else  $1E end-msg + show  5 step !  then  endof
        2 of  closed? if  $1D end-msg + show  next  then  endof
        3 of  closed? if  $1E end-msg + show  next  then  endof
        4 of  next  endof
        5 of  closed? if  to-word-plate? 0= if  finish  then  then  endof
    endcase ;
: plate-frame ( -- )   \ (SceneEnding_SeqTitleMessage)
    step @ case
        0 of  pic-in if  plate-msg show  next  then  endof
        1 of  closed? if  finish  then  endof
    endcase ;
: done-frame ( -- )   \ (the picture out, the music stopped; back to the title)
    pic-out if  $FF bgm  0 0 message-file drop  playing stage !  soft-reset  then ;

\ ==== drawing (the PS2's 512 x 448: labels at x 32, values right-aligned at 480, a line
\ every 32 from 96; the font 16 wide) ====
variable pen  variable w1   \ (the pen; a character's width at 1)
: fit-pen ( -- )   \ (ours is fixed-width: narrower than its 16)
    1 pen-scale  char-size drop 1 max w1 !  12 sw w1 @ /  1 max  dup pen !  pen-scale ;   \ (ours is fixed-width: narrower than its 16)
: text-w ( len -- px )  char-size drop * ;
: alpha ( a -- c )  2* 255 min ;   \ (the PS2's 0..$80)
: colour ( c -- rgba )   \ (0 white, 1 red, 2 yellow - the font's own; and the record's alpha)
    case  1 of  $FF6060  endof  2 of  $FFE040  endof  >r $FFFFFF r>  endcase
    8 lshift  text-a @ alpha or ;
: msg-text ( msg -- addr len )  message-layout drop  0 0 message-line ;
: words-at ( addr len x y c -- )  {: a n x y c :}  words-layer a n  x sx  y sy  c colour  pen @ ui-text ;
: right-at ( addr len y c -- )  {: a n y c :}  words-layer a n  480 sx n text-w -  y sy  c colour  pen @ ui-text ;
: hl ( flag -- c )  if  2  else  0  then ;   \ (a value the TYPE came from: yellow)

create time-buf 16 allot   variable time-n
: put ( c -- )  time-buf time-n @ + c!  1 time-n +! ;
: two ( n -- )  dup 10 / [char] 0 + put  10 mod [char] 0 + put ;
: colon ( -- )  [char] : put ;
: time$ ( -- addr len )
    0 time-n !  seconds dup 3600 / 99 min two colon  dup 60 / 60 mod two colon  60 mod two
    time-buf time-n @ ;
create letter-buf 1 allot
: dog$ ( -- addr len )  [char] A dog @ + letter-buf c!  letter-buf 1 ;
create pct-buf 8 allot
: pct$ ( -- addr len )  items-percent n>s  dup >r pct-buf swap move  [char] % pct-buf r@ + c!  pct-buf r> 1+ ;

: labels ( -- )
    8 0 do  i end-msg + msg-text  32  i 32 * 96 +  0 words-at  loop ;
: values ( -- )
    letter $18 + end-msg + msg-text  96   kind @ fugitive = hl  right-at
    time$                            128  kind @ dup track-star =  swap speed-walker = or if  2  else  kind @ late-bloomer = if  1  else  0  then  then  right-at
    dog$                             160  kind @ dup dog-beater = if  drop 1  else  dup top-breeder =  swap dog-lover = or hl  then  right-at
    panics n>s                       192  kind @ scream-queen = if  1  else  0  then  right-at
    pct$                             224  kind @ treasure-hunter = hl  right-at
    injuries n>s                     256  kind @ top-breeder = hl  right-at
    defeated n>s                     288  kind @ warmonger = hl  right-at
    kind @ type-msg msg-text         320  0  right-at ;
: fitting ( n -- scale )  w1 @ * 1 max  432 sw swap /  1 max  pen @ min ;   \ (n characters in the box)
: centred-line ( i -- )   \ (line i of the message's first page, centred on 256, as wide as fits)
    {: i | a n k :}  0 i message-line to n to a  n fitting to k
    words-layer a n  256 sx  n w1 @ * k * 2/ -  320 i 24 * + sy  $FFFFFFFF k ui-text ;
: message-box ( -- )   \ (an unlock or the word plate's: its lines in a box low on the screen)
    shown @ 0< if  exit  then
    words-layer  32 sx 304 sy  448 sw 112 sy  $00000090 ui-rect
    shown @ message-layout drop
    0 message-lines 0 ?do  i centred-line  loop ;
: draw ( -- )
    ending-layer ui-clear  words-layer ui-clear
    stage @ playing = if  exit  then
    frame!  fit-pen
    ending-layer  0 0 screen-size  $000000FF ui-rect   \ (the world gone: black)
    stage @ staff-roll = stage @ staff-fade = or if
        movie-status 1 = if  ending-layer  0 sx 0 sy 512 sw 448 sy  ui-movie  then
        stage @ staff-fade = if  ending-layer  0 0 screen-size  1e volume f@ f- 255e f* f>s  ui-rect  then
        exit
    then
    pic-a @ if
        ending-layer  0 sx 0 sy 512 sw 448 sy  picture-group 0 -1  0 0 512 448  $FFFFFF00 pic-a @ alpha or  ui-image
    then
    stage @ results = text-a @ 0> and if  labels values  then
    message-box ;

\ ==== the actor ====
: frame ( -- )
    stage @ case
        staff-roll of  staff-frame  endof
        staff-fade of  fade-frame  endof
        results of  results-frame  endof
        unlocks of  unlocks-frame  endof
        word-plate of  plate-frame  endof
        done of  done-frame  endof
    endcase ;
: time-played ( -- )   \ (in play: the world not held)
    progress pr.play-frames dup l@ 1+ most-frames min swap l! ;

behaviour ending-the-story
  on spawned ( -- )  playing stage !  -1 shown !
      self subscribe tick  self subscribe held-tick  self subscribe frame-end  self subscribe hewie-doing  self subscribe the-ending ;
  on hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
      trust !  2drop 2drop 2drop 2drop 2drop 2drop ;
  on the-ending ( -- )  stage @ playing = if  start  then ;
  on tick ( -- )  stage @ playing = if  time-played  else  frame  then ;
  on held-tick ( -- )  stage @ playing <> if  frame  then ;   \ (the world held: its own frames)
  on frame-end ( -- )  draw ;
end-behaviour

: ending-spawn ( -- id )  ending-the-story ending-state s" ending" spawn ;
