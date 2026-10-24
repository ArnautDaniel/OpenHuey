\ The story's rules against the original (src/game/event.c, scene_game.c): a character coming into
\ the room played runs its entering script (Events_CharEnter); the script's character by its id
\ (cond 0x17, the stalker's $FE); exits by her mode, not a script's hold (cmd 0x00), from Hewie's
\ side when he is played; the hiding places' offer while busy (SceneGame_Busy); the director's own
\ camera setup (0x36 $FF); close by the other's margins (0x47); the stalker free (0x65); facing
\ with the character anywhere (0x05); the file loader (0x62); a spirit's touch asked (0x64).
\ Headless:  build/openhuey-src/openhuey --test openhuey-src/tests/story/test_story_rules.fs
IN: test-story-rules
USING: tester engine game-state actors messages common room-names game debug facts flag-names story.state story.words ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: room-actor ( -- id )  room-id room-name actor-named ;
: in-room ( xt -- x )  room-actor enter  execute  leave-actor ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
1 seed !

free-play  front-garden-2 room!  20 frames
stalkers-blind state-flag-set

\ the room's entering script for characters, noted instead (who it ran as, its id)
variable entered-who   variable entered-id
: noted ( -- )  ctx-who @ entered-who !  ctx-id @ entered-id ! ;
: not-yet ( -- )  -1 entered-who !  -1 entered-id ! ;
room-id char-enter-phase room-script @ constant own-enter
' noted room-id char-enter-phase room-script!
not-yet

testing Hewie coming into her room while it is played: its entering script, as him
hewie send part-from-fiona  deliver  3 frames
T{ entered-who @ -> -1 }T
hewie send join-fiona  deliver  3 frames
T{ entered-who @ entered-id @ -> 1 1 }T
not-yet  5 frames
T{ entered-who @ -> -1 }T

testing the stalker put in her room by a script: in, no entering script
2 partner-load  2 frames
fiona body-tri debilitas-tri  3 frames
T{ entered-who @  [: stalker-cs cells char-in-room + @ ;] in-room -> -1 -1 }T
testing ... coming in by a door: his, its id his kind
[: 0 stalker-cs cells char-in-room + ! ;] in-room  [: 1 stalker.state:came-by ! ;] his  2 frames
T{ entered-who @ entered-id @ -> 2 2 }T
own-enter room-id char-enter-phase room-script!

testing a script's character by id (cond 0x17): the stalker's ($FE) while he is in the game
T{ [: stalker-cs cs>id  fiona-cs cs>id  hewie-cs cs>id ;] in-room -> 2 0 1 }T
T{ [: 2 ctx-who !  2 ctx-id !  stalker-who self-is?  2 self-is?  3 self-is?  0 ctx-id !  stalker-who self-is? ;] in-room -> -1 -1 0 0 }T

testing busy (SceneGame_Busy): a stalker here - a hiding place's offer is "no time"
T{ [: busy-bits busy-pressed and 0<>  too-busy? ;] in-room -> -1 -1 }T
T{ [: -1 prep-scene !  5 1 calm-hewie-up scene-change  prep-scene @ prep-arg @ ;] in-room -> read-message no-time-message }T
testing the stalker free (0x65): not while a script of the room has him
T{ [: stalker-free?  -1 stalker-cs cells char-scripted + !  stalker-free?  0 stalker-cs cells char-scripted + ! ;] in-room -> -1 0 }T
[: slot-unload ;] in-room  5 frames

testing nobody about: offered; only Hewie held is not busy; in hard mode with Hewie down, none
T{ [: -1 prep-scene !  5 1 calm-hewie-up scene-change  prep-scene @ ;] in-room -> 5 }T
T{ [: -1 hewie-cs cells char-scripted + !  too-busy?  0 hewie-cs cells char-scripted + ! ;] in-room -> 0 }T
T{ [: -1 prep-scene !  hard-mode-flag story-flag-set  down-cond hewie-cond !
      5 1 calm-hewie-up scene-change  hard-mode-flag story-flag-clear  prep-scene @ ;] in-room -> -1 }T

testing an exit taken while a script holds her (cmd 0x00: her mode, not the hold); led, not
: try-exit ( mode -- exit )
    {: mode | was :}  exit-taken @ to was  -1 fiona-cs cells char-scripted + !  mode fiona-mode !
    2 exit-check  going @  -1 going !  was exit-taken !  0 fiona-cs cells char-scripted + ! ;
T{ [: own-mode try-exit  down-mode try-exit  led-mode try-exit ;] in-room -> 2 2 -1 }T
testing passable from a side: Fiona's ($10), Hewie's ($20)
: door2 ( -- addr )  2 exit-door 4 * progress pr.doors + ;
T{ [: door2 dup l@ $20 or swap l!  2 0 exit-passable  2 1 exit-passable  door2 dup l@ $20 invert and swap l!  2 1 exit-passable ;] in-room -> -1 0 -1 }T

testing the director's own camera setup (0x36 $FF), followed by nobody; then her again
[: self-who 3 -1 char-camera ;] in-room
T{ cam-set@  cam-followed -> 3 -1 }T
[: fiona-who camera-follow ;] in-room
T{ cam-followed -> fiona }T

testing close by the other's margins (0x47): Hewie a little apart is within his radius of her
: put-hewie ( F: dx -- )
    {: F: dx :}  fiona body-pos  frot dx f+ frot frot
    hewie enter  room-id  fiona body-tri  body-place  leave-actor ;
fiona body-dims fdrop  hewie body-dims fdrop  f+  1.5e f+  put-hewie
T{ [: fiona hewie 0 0 touching?  0 ctx-who !  hewie-who self-touching? ;] in-room -> 0 -1 }T

testing the file loader never busy (0x62); a spirit's touch asked of her this frame (0x64)
T{ [: loader-busy?  spirit-request-pending? ;] in-room -> 0 0 }T
spirit-request  fiona body-pos xyz>cells  0  fiona send fiona-request  deliver  deliver
T{ [: spirit-request-pending? ;] in-room -> -1 }T
2 frames
T{ [: spirit-request-pending? ;] in-room -> 0 }T

testing facing (0x05) with Hewie in the game in another room
hewie enter  front-garden-1 -1 0e 0e 0e body-place  leave-actor
T{ [: hewie-who 0 180 char-heading?  hewie-who char-here? ;] in-room -> -1 0 }T

test-summary
