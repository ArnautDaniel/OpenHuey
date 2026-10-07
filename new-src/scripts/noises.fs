\ noises.fs - what the characters hear (src/game/actor.c Character_Hearing): of the noises made
\ this frame (events.core's slots, but its own), the first still louder than the character's
\ threshold (+0x152A) once weakened - in the room being played by the distance (a tenth of it,
\ the height three times over), elsewhere by the rooms between (32 a door; quiet ones only
\ through open doors). Kept as the character's heard noise (+0x14D8; slot +0x14D4, $FF none).
\ Each frame's end: everyone hears, then the noises go (the original clears them at the next
\ frame's start).
IN: noises
USING: engine game-state events.core chars partner.route ;

6 constant #chars
create c-heard-slots #chars cells allot   create c-heard-louds #chars cells allot
create c-heard-rooms #chars cells allot   create c-heard-tris #chars cells allot
create c-heard-doors #chars cells allot   create c-thresholds #chars cells allot
c-thresholds #chars cells 0 fill
: c-heard-slot ( cs -- slot | $FF )  cells c-heard-slots + @ ;
: c-heard-room ( cs -- room )  cells c-heard-rooms + @ ;
: c-heard-tri ( cs -- tri )  cells c-heard-tris + @ ;
: c-heard-door ( cs -- door )  cells c-heard-doors + @ ;
: c-threshold! ( n cs -- )  cells c-thresholds + ! ;
: unheard ( cs -- )  $FF swap cells c-heard-slots + ! ;
: unheard-all ( -- )  #chars 0 do  i unheard  loop ;
unheard-all

\ Rooms_DoorExit: which exit of `room` door `d` is ($FF none)
: door-exit-in ( d room -- exit | $FF )
    8 0 do  dup i room-exit-door 2 pick = if  2drop i unloop exit  then  loop  2drop $FF ;

variable hn-cs  variable hn-i  variable hn-base  variable hn-exit
create hn-src 12 allot
: n@ ( arr -- v )  hn-i @ cells + @ ;
\ the noise's loudness by the setting (quiet ones, below $80, made quieter)
: setting-loud ( -- loud )
    noise-loud n@  dup $80 < if
        noise-setting @ case  1 of  $1F -  endof  2 of  $3F -  endof  3 of  $5F -  endof  endcase
    then ;
\ in the character's room, or through a door into it
: reaches? ( -- flag )
    hn-cs @ c-room noise-room n@ = if  true exit  then
    noise-door n@ $FFFF = if  false exit  then
    noise-door n@ noise-room n@ door-exit-in dup hn-exit !
    dup $FF = if  drop true exit  then
    noise-room n@ swap room-exit-leads drop  hn-cs @ c-room = ;
\ where it came from (in the room being played): its triangle, else its door, else the listener
: noise-source ( -- )
    noise-tri n@ dup -1 <> if  tri-center hn-src vec! exit  then  drop
    hn-src hn-cs @ c-pos vec-copy
    noise-door n@ $FFFF = if  exit  then
    noise-door n@ hn-cs @ c-room door-exit-in dup $FF = if  drop exit  then
    1 exit-spot drop hn-src vec! ;
: by-distance ( loud -- loud' )
    noise-source
    hn-src sf@ hn-cs @ c-pos sf@ f-  hn-src 8 + sf@ hn-cs @ c-pos 8 + sf@ f-  fsq fswap fsq f+ fsqrt
    hn-src 4 + sf@ hn-cs @ c-pos 4 + sf@ f- fabs 3e f* f+  0.1e f* f>s - ;
: last-door ( n -- d )  1- dup 0< if  drop -1  else  cells route + @  then ;
: by-rooms ( loud -- loud' )
    hn-cs @ c-room noise-room n@ 0 find-route                      ( loud n )
    noise-door n@ $FFFF <>  over 0> and if
        dup last-door noise-room n@ door-exit-in  hn-exit @ = if  1-  then
    then
    dup -1 = if  drop hn-base @ $60 < if  drop 0  then  exit  then
    hn-base @ over 5 lshift < if  2drop 0 exit  then
    hn-base @ $41 < if  last-door door-open? 0= if  drop 0  then  else  drop  then ;
: hear ( cs -- )
    hn-cs !  0 hn-exit !
    4 0 do
        i hn-i !
        i hn-cs @ <> if
            setting-loud dup hn-base !
            dup 0> if
                reaches? if  hn-cs @ c-room room-id = if  by-distance  then
                else  by-rooms  then
                hn-cs @ cells c-thresholds + @ over < if
                    drop  hn-cs @ cells
                    i over c-heard-slots + !  noise-loud n@ over c-heard-louds + !
                    noise-room n@ over c-heard-rooms + !  noise-tri n@ over c-heard-tris + !
                    noise-door n@ swap c-heard-doors + !  unloop exit
                then
            then  drop
        then
    loop
    hn-cs @ unheard ;
: hear-all ( -- )   \ (each active character; then the frame's noises go)
    #chars 0 do  i c-active? if  i hear  else  i unheard  then  loop
    noises-clear ;
:noname ( loud tri -- )  room-id swap $FFFF 3 noise-make-in ; is event-noise
