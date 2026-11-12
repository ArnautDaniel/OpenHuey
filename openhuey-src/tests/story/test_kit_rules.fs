\ The story kit's shared rules read again against event.c (2026-11, 0.2.47): the scripts' items
\ through Events_ScriptRoom, the crouch's gate, touching by both margins, the offers' order, busy,
\ the camera on no stalker, the floor's height on a triangle. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_kit_rules.fs
IN: test-kit-rules
USING: tester engine game-state actors messages keys room-names game debug flag-names facts vectors ;

: in-place ( xt -- x )  room-id room-name actor-named enter  execute  leave-actor ;
: count-of ( item -- n )  game:items enter  items:items-of  leave-actor ;

free-play  10 run-frames  0 game:fiona send costume  castle-1f-13 debug:room!  40 run-frames

testing the scripts' items in a hard game: Camomile and Lavender ($40 / $41) given, named and counted as the Medallion ($70)
$70 count-of constant medallions
hard-mode-flag story-flag-set
T{ [: $40 story.kit:as-scripts  $41 story.kit:as-scripts  $42 story.kit:as-scripts ;] in-place -> $70 $70 $42 }T
T{ [: $40 1 story.kit:give-item ;] in-place  deliver  $70 count-of medallions -  $40 count-of -> 1 0 }T
T{ [: $41 1 story.kit:has-items? ;] in-place -> -1 }T   \ (the Medallion had)
hard-mode-flag story-flag-clear
T{ [: $40 story.kit:as-scripts ;] in-place -> $40 }T

testing the offers' order (cmd_scene_change / 0xA5): a doing replaces a message to read, the first doing stays, 0xA5 replaces either
: offers ( -- xt? msg )  story.kit:offer-xt @ 0<>  story.kit:offer-msg @ ;
: none-offered ( -- )  0 story.kit:offer-xt !  0 story.kit:offer-msg ! ;
T{ [: none-offered  $8016 story.kit:offer-remark-now  ['] dup story.kit:any-time story.kit:offer  offers ;] in-place -> -1 0 }T
T{ [: none-offered  ['] dup story.kit:any-time story.kit:offer  ['] drop story.kit:any-time story.kit:offer  story.kit:offer-xt @ ;] in-place -> ' dup }T
T{ [: none-offered  ['] dup story.kit:any-time story.kit:offer  $8016 story.kit:offer-remark-now  offers ;] in-place -> 0 $8016 }T
T{ [: none-offered  ['] dup story.kit:any-time story.kit:offer  $8001 story.kit:any-time story.kit:offer-remark  offers ;] in-place -> -1 0 }T

testing busy (SceneGame_Busy): her panic's onset counts
T{ [: story.kit:busy?  -1 story.kit:onset !  story.kit:busy?  0 story.kit:onset ! ;] in-place -> 0 -1 }T

testing the crouch's gate: not while she backs off, pushes or is held
T{ [: story.kit:fiona-doing-now @  backstep-doing story.kit:fiona-doing-now !  story.kit:fiona-idle?  story.kit:fiona-doing-now @ backstep-doing <> and  swap story.kit:fiona-doing-now ! ;] in-place -> 0 }T
T{ [: story.kit:fiona-doing-now @  push-doing story.kit:fiona-doing-now !  story.kit:fiona-idle?  swap story.kit:fiona-doing-now ! ;] in-place -> 0 }T

testing half hidden with no stalker in the game: the camera stays on her (the scripts' 0x28 $FE does nothing)
T{ [: story.kit:stalker ;] in-place -> -1 }T
T{ [: fiona-half-hidden story.kit:state!  story.common:stalker-watched  fiona-half-hidden story.kit:state-off ;] in-place  cam-followed -> game:fiona }T

testing the floor's height on a triangle (NavMesh_OntoPlane): its middle on its plane, and on a slope it isn't the middle's height away from it
: middle-on-plane? ( tri -- flag )  dup tri-center {: F: x y z :}  x z tri-floor  y f- fabs 0.01e f< ;
: all-on-plane? ( -- flag )  true  nav-tris 0 ?do  i middle-on-plane? and  loop ;
T{ all-on-plane? -> -1 }T
: sloped ( -- tri | -1 )   \ (the first steep enough to tell)
    nav-tris 0 ?do  i tri-normal fdrop fswap fdrop fabs 0.9e f< if  i unloop exit  then  loop  -1 ;
: off-middle ( tri -- ) ( F: -- dy )   \ 5 along x from its middle: the height there less the middle's
    dup tri-center {: F: x y z :}  x 5e f+ z tri-floor  y f- ;
T{ sloped dup 0< 0= swap off-middle fabs 0.01e f> or -> -1 }T   \ (a slope here, the height moves with it)

testing touching (Actor_Touching): Hewie raised clear of her height - near, facing - is touching only with a margin up
create spot 16 allot
: hewie-before ( up -- )   \ Hewie 5 ahead of her, `up` above her top
    {: up :}  game:fiona body-dims fswap fdrop {: F: h :}
    spot game:fiona body-at  game:fiona body-yaw 5e vec-ahead
    spot vec@ fswap h f+ up s>f f+ fswap
    game:hewie enter  body-place-at  leave-actor ;
game:hewie send join-fiona  30 run-frames
T{ 2 hewie-before  [: story.kit:fiona story.kit:hewie 10 45 story.kit:touches-facing?  story.kit:fiona story.kit:hewie 10 10 45 story.kit:touches-facing-up? ;] in-place -> 0 -1 }T
T{ 0 hewie-before  [: story.kit:fiona story.kit:hewie 10 45 story.kit:touches-facing? ;] in-place -> -1 }T
testing touching as the rooms' 0x47 has it (Actor_Touching with the other's radius and height as margins)
: hewie-ahead ( F: d -- )   \ Hewie d ahead of her, on her height
    spot game:fiona body-at  game:fiona body-yaw fswap vec-ahead  spot vec@  game:hewie enter  body-place-at  leave-actor ;
: radii ( F: -- rf rh )  game:fiona body-dims fdrop  game:hewie body-dims fdrop ;
T{ radii fover fover f+ fswap 0.5e f* f+ hewie-ahead  [: story.kit:fiona story.kit:hewie 0 story.kit:close-to?  story.kit:fiona story.kit:hewie story.kit:self-touching? ;] in-place -> 0 -1 }T
T{ radii fover fover f+ fswap 2e f* f+ 0.5e f+ hewie-ahead  [: story.kit:fiona story.kit:hewie story.kit:self-touching? ;] in-place -> 0 }T
testing the second clock (shared script $85): its own words, "It's another clock..."
T{ story.kit:another-clock-msg story.kit:clock-msg -> $8017 $8014 }T
full-suite   \ (on: waits, scenes, walks - the full run's)

test-summary
