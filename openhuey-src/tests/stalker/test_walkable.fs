\ The walkable point nearest a goal on floor that isn't the walker's (Npc_NearestWalkable: the
\ engine's `walkable` - from the nearest triangle free of the mask, PathPlan_StepCheapNear, on toward
\ the goal while the floor is free): a stalker's way to such a goal goes there (NPC_PathLengthGoal);
\ a goal on his floor is as it is. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_walkable.fs
IN: test-walkable
USING: stalker-names facts story.words tester engine game-state actors messages room-names game debug vectors flag-names stalker.state stalker.moving ;

: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
variable mask  variable shut-tri  variable free-tri
create at /vec allot  create out /vec allot

free-play  castle-1f-1 room!  20 run-frames
stalkers-blind state-flag-set
s" fiona" actor-named body-tri debug:debilitas-tri  3 run-frames
[: self body-mask ;] his mask !
\ a triangle of the room his floor isn't, with free floor about it; and one that is his
: find-tris ( -- )
    -1 shut-tri !  -1 free-tri !
    nav-tris 0 do
        i nav-flags mask @ and if  shut-tri @ 0< if  i shut-tri !  then  else  free-tri @ 0< if  i free-tri !  then  then
    loop ;
find-tris
testing the room has floor that isn't his
T{ shut-tri @ 0< free-tri @ 0< -> 0 0 }T
testing a goal on floor not his: the walkable point nearest it - on free floor, near it
shut-tri @ tri-center at vec!
T{ shut-tri @ at out mask @ walkable  dup 0< 0=  swap nav-flags mask @ and 0=  out at vec-dist 40e f< -> -1 -1 -1 }T
testing a goal on his floor: as it is
free-tri @ tri-center at vec!
T{ free-tri @ at out mask @ walkable  out at vec-dist f0= -> free-tri @ -1 }T
testing his way to the goal not his: planned (to the walkable point nearest it)
shut-tri @ tri-center at vec!
T{ [: shut-tri @ at plan 0> ;] his -> -1 }T
test-summary
