\ The graphics menu (scripts/graphics.fs): F1 opens it. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/graphics/test_graphics.fs
IN: test-graphics
USING: tester engine game graphics actors messages room-names debug ;

testing F1 opens the menu
key: F1 true key-hold  game-tick  key: F1 false key-hold  game-tick
T{ menu-open @ 0<> -> -1 }T

testing the presets: as on the PS2 - the original's character lighting (per vertex), no boost,
testing no rim, none of OpenHuey's effects, 4:3; enhanced (the default) - the same lighting per pixel
: gfx-l ( xt -- n )  gfx swap execute l@ ;
: gfx-f ( xt -- n )  gfx swap execute sf@ 1000e f* fround f>s ;   \ (in thousandths)
look-original
T{ ' gfx.ps2-lighting gfx-l ' gfx.aspect gfx-l ' gfx.ssao gfx-l ' gfx.bloom gfx-l ' gfx.fog gfx-l ' gfx.tonemap gfx-l -> 1 1 0 0 0 0 }T
T{ ' gfx.rim gfx-f ' gfx.character-light gfx-f ' gfx.vignette gfx-f ' gfx.grain gfx-f -> 0 1000 0 0 }T
T{ ' gfx.room-fog gfx-l ' gfx.room-tint gfx-l ' gfx.room-bloom gfx-l ' gfx.room-lights gfx-l -> 1 1 1 1 }T
look-enhanced
T{ ' gfx.ps2-lighting gfx-l ' gfx.aspect gfx-l ' gfx.ssao gfx-l ' gfx.fog gfx-l -> 2 0 1 0 }T
T{ ' gfx.rim gfx-f ' gfx.character-light gfx-f ' gfx.vignette gfx-f -> 200 1000 200 }T
T{ ' gfx.shadow-maps gfx-l ' gfx.shadow-soft gfx-l ' gfx.msaa gfx-l -> 1 1 4 }T

testing the low-power look (the fresh start's on OpenGL 3.3): the original's look whole - the room's
testing own fog, tint, bloom, depth of field, its lighting per vertex, the characters' shadows - and
testing OpenHuey's costly extras off: no antialiasing, occlusion or bloom; shadows' edges by 4 samples
look-low-power
T{ ' gfx.room-fog gfx-l ' gfx.room-tint gfx-l ' gfx.room-bloom gfx-l ' gfx.room-dof gfx-l -> 1 1 1 1 }T
T{ ' gfx.room-lights gfx-l ' gfx.ps2-lighting gfx-l ' gfx.shadow-maps gfx-l -> 1 1 1 }T
T{ ' gfx.msaa gfx-l ' gfx.ssao gfx-l ' gfx.bloom gfx-l ' gfx.fog gfx-l ' gfx.shadow-soft gfx-l -> 1 0 0 0 0 }T
T{ ' gfx.scale gfx-f ' gfx.anisotropy gfx-f -> 1000 4000 }T
testing the fresh start's look by the OpenGL path: the low-power one on 3.3 (gl-low?), else enhanced
: fresh-wanted ( -- msaa ssao soft )  gl-low? if  1 0 0  else  4 1 1  then ;
look-original  graphics.private:fresh-look
T{ ' gfx.msaa gfx-l ' gfx.ssao gfx-l ' gfx.shadow-soft gfx-l -> fresh-wanted }T
testing a preset sets all it is about: the low-power look's resolution and filtering undone by the others
look-original
T{ ' gfx.shadow-soft gfx-l ' gfx.scale gfx-f ' gfx.anisotropy gfx-f -> 1 1000 16000 }T
look-low-power  look-enhanced
T{ ' gfx.shadow-soft gfx-l ' gfx.msaa gfx-l ' gfx.scale gfx-f ' gfx.anisotropy gfx-f ' gfx.ssao gfx-l -> 1 4 1000 16000 1 }T

testing the placed objects' layers by name (PlacedObject_Draw): "g_" $26 marking the bloom mask
testing only, "a_" $19 see-through, others 1 (castle-2f-2's chandelier glow, $21's lattice)
: named ( addr u -- i )   \ (the placed object of that name; -1 none)
    {: a u :}  placed-count 0 ?do  i placed-info drop fdrop fdrop fdrop  a u compare 0= if  i unloop exit  then  loop  -1 ;
$20 room
T{ s" g_chandelie" named placed-draws -> $26 -1 }T
$21 room
T{ s" a_nozoki" named placed-draws -> $19 0 }T
T{ 0 placed-draws nip -> 0 }T
testing an open door's shadow in layer 6 (DoorShadow_Queue): none shut; open, castle-1f-1's door
testing 0 from light 0 (the moon) at 18 of $80 - 4 cos^2 x its intensity x (64 + the ambient)
castle-1f-1 0 rooms send go-to-room  10 run-frames
0 -1 0e door-swing  2 run-frames
T{ door-shades -> 0 }T
0 -1 -90e door-swing  2 run-frames
T{ door-shades -> 2 }T
T{ 0 door-shade f>s -> 0 0 18 }T
0 -1 -70e door-swing  2 run-frames
T{ door-shades -> 2 }T                       \ (open at all: its shadow; blocking the lights is past 78.75)
full-suite   \ (on: waits, scenes, walks - the full run's)
0 -1 0e door-swing  2 run-frames
test-summary
