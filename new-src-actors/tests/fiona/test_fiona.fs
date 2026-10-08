\ Fiona, phase F1: moving, footsteps, doors, going out (docs/subsystems/fiona.md). Headless:
\   build/new-src-actors/hga --test new-src-actors/tests/fiona/test_fiona.fs
IN: test-fiona
USING: tester engine game-state actors messages room-names game fiona.state fiona.model fiona.moving fiona.doors ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: her@ ( xt -- x )  fiona enter  execute  leave-actor ;          \ (one of her fields, read inside her)
: anim ( -- id )  ['] anim@ her@ ;
: sub ( -- n )  [: her-sub @ ;] her@ ;
: doing ( -- n )  [: her-doing @ ;] her@ ;
: hold ( key -- )  true key-hold ;   : release ( key -- )  false key-hold ;
: press ( key -- )  dup hold  game-tick  release ;
create from 12 allot  create now 12 allot
: moved ( -- )  fiona body-pos from vec! ;    \ (marks where she is)
: since ( F: -- d )  fiona body-pos now vec!  from now vec-dist-xz ;

2 frames
0 fiona send costume  1 frames   \ (her clothes: these are her mechanics, not the new game's slip)
game:story send story-stop  deliver  progress pr.state 8 0 fill   \ (no room scripts: this test sets the flags by hand)
testing a new game: in the cage room, standing, followed by the camera
T{ room-id  fiona body-room -> front-garden-3 front-garden-3 }T
T{ anim -> 0 }T
T{ game:camera enter  camera:followed @  leave-actor -> fiona }T

testing walking, running, stopping
moved
T{ key: W hold  30 frames  anim sub -> $200 1 }T
T{ since 5e f> -> -1 }T
T{ key: Left_Shift hold  30 frames  anim sub -> $202 2 }T
T{ key: Left_Shift release  key: W release  25 frames  anim sub -> 0 0 }T

testing her footsteps are heard: walking 4, running $14
state: ear-state  cell field dummy  end-state
variable loudest
behaviour ear
  on heard ( loud room tri door source -- )  fiona-noise = if  2drop drop loudest @ max loudest !  else  2drop 2drop  then ;
  on heard-nothing ( -- ) ;
end-behaviour
ear ear-state s" ear" spawn constant ear
deliver  ear enter  fiona body-room fiona body-tri fiona body-pos body-place  0 stalker-noise acoustics send listen  leave-actor
T{ 0 loudest !  key: W hold  40 frames  key: W release  loudest @ -> 4 }T
T{ 0 loudest !  key: W hold key: Left_Shift hold  40 frames  key: Left_Shift release key: W release  loudest @ -> $14 }T
ear kill

\ ---- doors: front-garden-2's exit 0 is a real door ----
front-garden-2 -1 rooms send go-to-room  5 frames
: door0 ( -- d )  front-garden-2 0 room-exit-door ;
: state0 ( -- w )  door0 4 * progress pr.doors + l@ ;
: clean0 ( -- )  door0 4 * progress pr.doors + 0 swap l! ;
: at-door ( -- )   \ (by its spot for her first door animation)
    fiona enter  0 0 door-user-spot drop  fdrop  her-at vec!  her-at blocked-floor v-tri-in her-tri !  leave-actor  3 frames ;
: watch-anim ( n anim -- seen? )   \ up to n frames for that animation
    false swap rot 0 ?do  game-tick  dup anim = if  nip true swap leave  then  loop  drop ;

testing at a door, the action button: she holds it, opens it by hand, lets it go open
clean0 at-door
T{ key: Space press  doing -> 3 }T
T{ 30 $600 watch-anim  state0 -> -1 1 }T          \ her animation, the door held
T{ 120 frames  state0  doing -> 2 0 }T            \ let go open; free again
\ (her animation takes her into the exit's area; the story takes the exit: tests/story/test_story.fs)
front-garden-2 -1 rooms send go-to-room  5 frames

testing a locked door: she tries it, and gives up
clean0  door0 doors send lock  deliver  at-door
T{ key: Space press  60 $609 watch-anim -> -1 }T
T{ 90 frames  doing  state0 -> 0 8 }T             \ free; still locked, never held
door0 doors send unlock  deliver

\ (going out by an exit: the story's room scripts take it - tests/story/test_story.fs)

test-summary
