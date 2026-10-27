\ A door locked from this side (Progress_CharRequests' request $80000002: she in an exit's door
\ area, its door locked): a room's own action answers it - unlocked - and takes the action button
\ from her own try (a place: house-of-truth-1f-7's door $EA at exit 0, unlocked from this side). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_locked_doors.fs
IN: test-locked-doors
USING: tester engine game-state actors messages keys room-names game debug facts flag-names doors story.kit ;
: window-shown ( -- msg )  game:window enter  window:shown @  leave-actor ;
: read-on ( n -- )  0 ?do  window-shown 0< 0= if  cross press  then  game-tick  loop ;
: in-room ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
variable requested   \ (her request at the locked door, as the place sees it)
: asking ( -- )  [: 8 locked-request? ;] in-room requested ! ;
$EA constant the-door
create spot 12 allot
: before-it ( -- )   \ (on her spot for it, in its door area)
    0 0 door-user-spot drop fdrop spot vec!  spot vec@ tp  10 run-frames ;

free-play  10 run-frames  0 game:fiona send costume
the-door s" doors" actor-named send lock  house-of-truth-1f-7 room!  30 run-frames
testing locked, she before it: her request (2), for the place's action to answer
before-it
T{ s" fiona" actor-named body-pos 0 0 door-in-area?  asking  requested @ -> -1 -1 }T
testing ... the action button: to the door, a try - unlocked, said so
circle press  300 read-on
T{ the-door door-locked? -> 0 }T
testing not locked: no request
T{ asking  requested @ -> 0 }T
test-summary
