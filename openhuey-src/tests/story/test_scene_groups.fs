\ A scene's object groups (src/game/cutscene.c; the original's Cutscene_StartShot, GroupKeys,
\ RestoreGroups): the script's group entries are the objects' names; each shot shows the groups it
\ moves and hides the rest, and the scene's end - played or skipped - puts them back where they
\ were defined. castle-2f-3's change of clothes (act0D, EV0003) moves the picture `a_nozoki`
\ (issue #73: left floating by the fireplace). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_scene_groups.fs
IN: test-scene-groups
USING: tester engine game-state actors messages keys room-names game debug flag-names fiona.state story.state story.words pause ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: msg? ( -- flag )  room-actor 0< if  false exit  then  room-actor enter  msg @  leave-actor  0< 0= ;
: answering ( n -- )  0 ?do  msg? if  cross press  then  game-tick  loop ;
: pic-y ( F: -- y )  s" a_nozoki" $24 placed-f@ ;
: pic-shown? ( -- flag )  s" a_nozoki" placed-shown? ;

1 game:fiona send costume deliver
castle-2f-3 room!  30 frames
testing before the scene: the picture on the wall, shown
T{ pic-y 80e f=  pic-shown? -> -1 -1 }T
testing the change of clothes (act0D: EV0003): the picture moved by the scene's keys
world-held state-flag-clear
room-actor enter  0 0 $D action  leave-actor
20 answering  600 answering
T{ cutscene-active?  pic-y 80e f= -> -1 0 }T
testing ... skipped: the picture back where it was defined (Cutscene_End: RestoreGroups)
start-button press  10 frames  triangle press  600 answering
T{ cutscene-active?  pic-y 80e f= -> 0 -1 }T
test-summary
