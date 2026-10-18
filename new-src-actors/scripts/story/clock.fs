\ story/clock.fs - the clock of the run through rooms $D0..$E9: the original's play time
\ { hours, minutes, seconds, frames } (progress +$FC4), zeroed as the run starts (clock_start)
\ and shown at the foot of the screen as MM:SS - "59:59" once an hour has gone - by each room's
\ frame hook (clock_draw). At the end the time is kept in event variables 0..2 (clock_save) and
\ shown from them while event flag 1 is set (clock_draw_saved). The run's camera follows its paths
\ quicker (the director's rate 14). Its start hands the player Hewie (facts.fs hewie-played:
\ progress +$1FBEC1) and restarts him, played; its stop gives Fiona back.
IN: story.clock
USING: engine game-state actors messages facts story.words ps2-screen ;   \ (ps2-screen last: its sx / sy, not the story's)

17 constant clock-layer   \ (the UI layers: ... 16 the cursor's fill; 17 this)
30 constant per-second
$FF6020FF constant clock-colour   \ (the original's 0x80 0x30 0x10: PS2 colours, 0x80 full)

: time-played ( -- frames )  progress pr.play-frames l@ ;
: past-an-hour? ( frames -- flag )  per-second 3600 * < 0= ;

create clock-text 5 allot
: two-digits ( n at -- )  >r  10 /mod [char] 0 + r@ c!  [char] 0 + r> 1+ c! ;
: mm:ss ( minutes seconds -- addr len )
    clock-text 3 + two-digits  clock-text two-digits  [char] : clock-text 2 + c!  clock-text 5 ;
: time$ ( frames -- addr len )
    dup past-an-hour? if  drop s" 59:59" exit  then
    per-second /  dup 60 / 60 mod  swap 60 mod  mm:ss ;

\ at (431, 395) of the PS2's screen, its glyphs 21 high
: pen ( -- scale )  1 pen-scale  21 sy  char-size nip 1 max /  1 max ;
: show ( addr len -- )
    {: a n :}  frame!  clock-layer ui-clear  clock-layer a n  431 sx 395 sy  clock-colour pen ui-text ;
: clock-off ( -- )  clock-layer ui-clear ;   \ (each frame before the rooms' hooks draw it again)

: clock-start ( -- )   \ clock_start: Hewie played, afresh; the camera quicker along its paths; the time from 0
    -1 hewie-played !  s" hewie" actor-named send hewie-restart  14e cam-rate!  0 progress pr.play-frames l! ;
: clock-draw ( -- )  time-played time$ show ;                  \ clock_draw
: clock-save ( -- )                                            \ clock_save (and for the results: subscreen/extras.fs)
    time-played dup run-took !  0 over past-an-hour? 1 and var-set
    per-second /  1 over 60 / 60 mod var-set  2 swap 60 mod var-set ;
: clock-draw-saved ( -- )                                      \ clock_draw_saved
    1 ebit? 0= if  clock-draw exit  then
    0 var @ if  s" 59:59" show exit  then
    1 var @ 2 var @ mm:ss show ;
: clock-stop ( -- )  0 hewie-played !  -1e cam-rate! ;   \ clock_stop: Fiona played again; the camera at its usual pace
