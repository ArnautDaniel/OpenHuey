\ The story: each room's event scripts as its actor (docs/subsystems/story.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/story/test_story.fs
IN: test-story
USING: tester engine game-state actors messages room-names game fiona.state story.state story.words ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: setup-of ( id -- set path )  game:camera enter  camera:setup dup @ swap cell+ @  leave-actor ;
: visited? ( room -- flag )  progress pr.visited bit? ;

3 frames
testing a new game: the cage room's actor, its entering script run: Fiona's camera setup, followed
T{ room-actor 0< -> 0 }T
T{ [: this-room @ came-by @ ;] in-room -> front-garden-3 -1 }T
T{ fiona setup-of -> 2 1 }T
T{ game:camera enter camera:followed @ leave-actor -> fiona }T
T{ front-garden-3 visited? -> -1 }T

testing to another room: the old room's actor goes, the new one's comes
front-garden-2 -1 rooms send go-to-room  3 frames
T{ front-garden-3 room-name actor-named  room-actor 0< -> -1 0 }T
T{ [: this-room @ ;] in-room -> front-garden-2 }T

\ ---- an exit: in its area and free, the room's phase 1 takes it ----
create spot 12 allot
: to-area-of ( exit -- )   \ (Fiona put in the middle of the exit's area)
    exit-area area-middle drop  spot vec!
    fiona enter  her-at spot vec-copy  her-at $28020018 v-tri-in her-tri !  leave-actor ;
testing exit 2 of front-garden-2: Fiona in its area, the room's script takes it
T{ 2 to-area-of  3 frames  room-id -> front-garden-2 2 room-exit-leads drop }T
T{ [: came-by @ ;] in-room -> front-garden-2 2 room-exit-leads nip }T
testing arriving, she is put on the exit's outside spot and isn't sent straight back
T{ 60 frames  room-id -> front-garden-2 2 room-exit-leads drop }T

testing a door (front-garden-2's exit 0): she opens it with her animation, walks into its area, the script takes it
front-garden-2 -1 rooms send go-to-room  5 frames
: door0 ( -- d )  front-garden-2 0 room-exit-door ;
door0 4 * progress pr.doors + 0 swap l!
: at-door ( -- )   \ (by her spot for her first door animation)
    fiona enter  0 0 door-user-spot drop  fdrop  her-at vec!  her-at $28020018 v-tri-in her-tri !  leave-actor  3 frames ;
: press ( key -- )  dup true key-hold  game-tick  false key-hold ;
: went? ( n -- flag )  0 ?do  game-tick  room-id front-garden-2 <> if  true unloop exit  then  loop  false ;
at-door
T{ key: Space press  240 went?  room-id -> -1 front-garden-2 0 room-exit-leads drop }T

testing an action script runs as the room's coroutine, a turn a frame
: started ( -- n )  [: 0 #slots 0 do  i slot-task @ if  1+  then  loop ;] in-room ;
variable before
started before !
T{ [: 0 $F6 $80 action-force ;] in-room  started -> before @ 1+ }T   \ (a scene slot, 0xF6: the shared script 0x80)
test-summary
