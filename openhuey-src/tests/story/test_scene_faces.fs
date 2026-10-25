\ A scene's faces (src/game/cutscene.c; the original's Cutscene_Cast / ReleaseActors): taken by the
\ scene, a character's face and hands are cleared (Character_Disable's Motion_Disable - no blink
\ left half shut) and drawn with every one of its shapes by the scene's tracks (the model's +0x30:
\ func_001BCD30 - the brows, the eyes, the mouth's shapes; out of scenes only the mouth and the
\ eyelids, func_001BC8D0); given back, cleared again and drawn as before (Model_ClearDraw). Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/story/test_scene_faces.fs
IN: test-scene-faces
USING: tester engine game-state actors messages keys room-names game debug flag-names fiona.state story.state story.words pause ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: msg? ( -- flag )  [: msg @ ;] in-room 0< 0= ;
: within ( n xt -- flag )
    swap 0 ?do  msg? if  cross press  then  game-tick  dup execute if  drop true unloop exit  then  loop  drop false ;
: her ( -- a )  game:fiona enter  her-model @  leave-actor  actor ;
: lids ( F: -- l r )  her act.face 7 4 * + sf@  her act.face 8 4 * + sf@ ;
: in-scene? ( -- flag )  game:fiona enter  her-in-scene @  leave-actor ;

60 frames
testing out of a scene: her face drawn as her own (the mouth and the eyelids)
T{ her act.face-all l@ -> 0 }T
testing her eyelids half shut by a blink, then a scene takes her: open, and every shape of her face drawn by it
0.49e her act.face 7 4 * + sf!  0.49e her act.face 8 4 * + sf!
world-held state-flag-clear
[: 0 0 0 action ;] in-room
T{ 300 [: cutscene-active? ;] within -> -1 }T
T{ her act.face-all l@  lids f0= f0= -> 1 -1 -1 }T
testing ... the scene's tracks into her weights four apart (Motion_ExtraChannels: +0x58 + 16 i) - her eyelids
testing ... (7 and 8: the second track's last, the third's first) moved together
variable lids-moved  variable lids-apart
: lids-now ( -- )
    lids {: F: l r :}  l f0= 0= if  -1 lids-moved !  then  l r f- fabs 0.0001e f> if  -1 lids-apart !  then ;
: watch-lids ( n -- )  0 ?do  game-tick  lids-now  loop ;
120 watch-lids
T{ lids-moved @  lids-apart @ -> -1 0 }T
testing ... her own frame waiting meanwhile (Character_Disable): her place where the scene moves her (Fiona_FollowAnim)
: her-place-off ( F: -- d )  game:fiona enter  her-at self body-at vec-dist  leave-actor ;
T{ 60 frames  her-place-off 0.01e f<  game:fiona enter her-model @ leave-actor scene-has? -> -1 -1 }T
testing ... skipped: given back, drawn as her own again
T{ start-button press  10 frames  triangle press  300 [: in-scene? 0= ;] within -> -1 }T
T{ her act.face-all l@  game:fiona enter her-model @ leave-actor scene-has? -> 0 0 }T
test-summary
