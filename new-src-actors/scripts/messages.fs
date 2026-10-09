\ messages.fs - every message the actors pass, with its arguments: the game's API. Each
\ subsystem's page (docs/subsystems/) says who sends and who takes them.
IN: messages
USING: actors ;

\ ---- acoustics (docs/subsystems/acoustics.md) ----
\ a noise this frame: at nav triangle `tri` (-1: at a door), or at `door` (-1: none)
message noise ( loud room tri door source -- )
message listen ( threshold source -- )        \ hear noises louder than this, not my own source's
message stop-listening ( -- )
message heard ( loud room tri door source -- )   \ what I heard this frame
message heard-nothing ( -- )
message noise-setting ( n -- )                \ 0..3: quiet noises quieter

\ the sources, in the order they are listened to
0 constant fiona-noise   1 constant hewie-noise   2 constant stalker-noise   3 constant world-noise
4 constant #noise-sources

\ ---- rooms (docs/subsystems/rooms.md) ----
message go-through ( exit -- )               \ I (the controlled one) went out by this exit
message go-to-room ( room exit -- )          \ play that room, arriving by exit (-1: none)
message leaving-room ( room exit -- )        \ the played room is being left by this exit
message arrived ( room exit -- )             \ the new room is in; arrivals come by this exit
message entered-room ( room exit -- )        \ the room is in play

\ ---- camera (docs/subsystems/camera.md) ----
message camera-setup ( who set path -- )     \ who's camera set and path (path -1: none)
message follow ( who -- )                    \ the camera follows who (-1: nobody)
message camera-cut ( -- )                    \ it cut to a new set

\ ---- doors (docs/subsystems/doors.md) ----
\ kinds of character, for locks: who is kept out
0 constant fiona-kind   1 constant hewie-kind   2 constant stalker-kind
message lock ( door -- )
message unlock ( door -- )
message lock-for ( door kind on -- )         \ locked against a kind of character, or not
message close-off ( door -- )                \ gone for good (story)
\ the played room's doors, by exit; `source` is the user's noise source
message use-door ( exit anim source -- )     \ the door swings along the user's animation
message swing-door ( exit open source -- )   \ swung open / shut by hand
message slam ( exit source -- )
\ any room's doors (off-screen users too): take hold of one before going through, then let
\ it go open or shut; the doors answer `door-held` or `door-refused`
message hold-door ( room exit kind -- )
message door-held ( room exit -- )
message door-refused ( room exit -- )
message let-go-open ( room exit source -- )
message let-go-shut ( room exit source -- )
message door-changed ( door -- )             \ broadcast: its state changed

\ ---- danger (docs/subsystems/danger.md) ----
message danger-signal ( bit -- )             \ this frame: 1 Fiona struck, 3 chasing, 4 Hewie alert, 6 hunted...
message stalker-here ( room alert chasing -- )   \ the stalker in play this frame
message danger ( level -- )                  \ broadcast: 0 calm, 1 followed, 2 chased

\ ---- panic (docs/subsystems/panic.md); amounts and levels are floats (f>cell) ----
message fright ( amount -- )
message fear-in ( amount -- )
message panic-stage! ( stage -- )
message panic-level! ( level -- )
message panic ( stage level -- )             \ broadcast each frame

\ ---- Fiona (docs/subsystems/fiona.md) ----
message fiona-doing ( mode sub cond cmd -- ) \ broadcast each frame: her move mode, what she does, her condition,
                                             \ her command this frame (-1 none; 3 the praise held)
message fiona-plight ( state -- )            \ broadcast each frame: how she stands for a stalker's attack
                                             \ (Pursuer_FionaState): 0 calm, 1 fleeing, 2 panicking, 3 out
                                             \ of reach (hidden, already struck), 4 / 5 the panic's stages 4 / 5
\ ---- blows (F4, the stalkers' P2: Relation_Request kind 4 to Fiona / Hewie) ----
message hit ( kind how fright -- )           \ from the sender: kind 1 / 2 a blow, 4 a hard one (thrown), 6 a
                                             \ hold (caught: how is the grip, 1 an arm .. 4; 5 thrown off),
                                             \ $8000 in how a stumble; fright (a float cell) for her panic
message hit-taken ( kind -- )                \ the answer: it struck home (none: it didn't)

\ ---- Hewie (docs/subsystems/hewie.md) ----
message join-fiona ( -- )                    \ he comes into the game at her heel (the story; the console)
message part-from-fiona ( -- )               \ he leaves the game
message hewie-doing ( here action mode sub cond mood group -- )   \ broadcast each frame: here 0 out of the
                                             \ game, 1 in another room, 2 with her; his action, move mode and
                                             \ its part, condition, mood, his pose's group
message command ( code tri yaw x z -- )      \ Fiona's command (0x23 go there: the spot; floats f>cell)
message reaction ( n -- )                    \ how her doing strikes him (Fiona_HewieReact's row)
\ a joint action by his side: she asks (type 0 scold, 2 praise, 4 stay); he finds her place by
\ him, or refuses; she goes there, he sits; then the second part (1 / 3 / 5)
message meet-me ( type -- )
message meet-at ( tri x y z face -- )        \ (floats f>cell) stand there, facing so
message meet-now ( type -- )
message meet-on ( -- )
message meet-refused ( -- )
message meet-off ( -- )                      \ either gives it up

\ ---- the story (docs/subsystems/story.md) ----
message to-exit ( exit -- )                  \ (a character) put yourself on the exit's outside spot, facing in
message place ( x y z yaw -- )               \ (a character) put yourself there, facing so (floats f>cell)
message go-to ( x y z -- )                   \ (a character) walk there over the nav mesh, then stand (floats f>cell; debugging, the story's moves)
message camera-restart ( -- )                \ (the camera) the director starts over
message room-enter ( room exit -- )          \ (a room actor, from the story) the room is in: phase 0, the characters, phase 3
message room-leave ( -- )                    \ its exit is taken: phase 4
message room-left ( -- )                     \ the next room is in: phase 5, then it reports
message room-done ( -- )                     \ (to the story) finished: it may go
message story-stop ( -- )                    \ (to the story) no more room scripts (tests of the others, the console)

\ ---- messages on screen (docs/subsystems/window.md) ----
message show-text ( msg -- )                 \ (the window) show message msg; its closing answered to the sender
message close-text ( msg -- )                \ (the window) close it if it shows msg ($FFFF: whatever it shows)
message text-closed ( msg answer -- )        \ (to who showed it) closed; the option chosen (-1 none)
message text-shown ( on -- )                 \ broadcast: the window opened / closed (Fiona stands while it's open)
message prompt ( kind -- )                   \ (the window) an action is offered here this frame (the action prompt)

\ ---- examining (the scripts' prepared action, Progress_PlayerButtons) ----
message offer ( scene arg -- )               \ (room -> Fiona) an action here this frame, taken with the action button
message take-offer ( scene arg -- )          \ (Fiona -> room) she took it
message scripted ( on -- )                   \ (room -> a character) a script of the room has you (on) / lets go (off)

\ ---- the story's moves (the original's +0xF4 / +0xE1; docs/subsystems/story.md S3) ----
\ kind: 1 full stop, 2 idle, 5 / 10 walk / run to a spot (b: -1 straight, else along a path),
\ 6 / 11 walk / run a path to a triangle, 7 an animation (a), 8 blended (a, b frames), 9 blended
\ with no time of its own, 16 her idle (b), 12 look at a character (a: its actor, -1 stop), 13 look
\ at the point, 14 turn to a character (a: its actor), 15 turn to the heading, 17 walk the way to
\ the spot with animation a (blend b), arriving as it ends
message scripted-move ( kind a b x y z yaw -- )   \ (to a character; floats f>cell; a / b: as the kind says)
message moving ( done ended events -- )      \ broadcast at each frame's end by a character: its move done, its animation ended, its motion's event keys
message hold-anim ( anim blend -- )          \ (to a character) play this animation and stay in it (the scripts' 0x9D)
message show ( on -- )                       \ (to a character) shown or hidden (the scripts' 0x1F)

\ ---- the screen and the music (docs/subsystems/screen.md, music.md) ----
message fade ( frames kind -- )              \ (the screen) fade in (kind 0 / 1) or out over frames
message fade-finish ( -- )                   \ (the screen) the fade counts as over
message fade-done ( -- )                     \ (to who faded) it's over
message bgm-want ( track pause level -- )    \ (the music) background track wanted ($FF none), paused, its level (f>cell)
message bgm-resume ( -- )                    \ (the music) the streamed track resumed
message music-op ( op a b -- )               \ (the music) the stage music: 0 volume to a over b frames, 2 hold, 4 release, 5 silence
message music-stage ( stage -- )             \ (the music) the stage set's music made (-1: ended)
\ ---- scenes (the cutscene director: docs/subsystems/story.md S4) ----
message scene ( on -- )                      \ broadcast: a cutscene has the characters (on) / gives them back (off)
\ ---- the rooms' effects (docs/subsystems/effects.md) ----
message effect-start ( x y z command kind -- )   \ (an effect) its place (f>cell) and how it starts
message effect-command ( v -- )                  \ (an effect) a command later (the scripts' 0x87: 1 still, 2 moving)
message specks-start ( x y z count spread rgba -- )   \ (a speck swarm) its centre (f>cell), how many, how far they stray, their colour $RRGGBBAA
message shards-start ( tex u v du dv floor -- )   \ (shards) their texture (the room's), its cell (f>cell: u v du dv), the floor below the origin (f>cell); thrown
message butterflies-start ( x y z count -- )   \ (butterflies) where they make for (whole numbers), how many (0: as they are)
message dust-start ( rgba x y z vx vy vz -- )  \ (a mote of dust) its colour $RRGGBBAA, place and velocity (f>cell)
\ ---- the stalkers (docs/subsystems/stalker.md) ----
message stalker-in ( room tri -- )           \ (a stalker) into the game, in that room on that triangle
message stalker-out ( -- )                   \ (a stalker) out of the game
message stalker-hunt ( -- )                  \ (a stalker) heads for Fiona's room (mode 1: the summoner, the console)
message costume ( n -- )                     \ (Fiona) her model for costume n (0 her clothes, 1 the slip)
message stalker-load ( -- )                  \ (a stalker) into the story's character slot 2 (the cutscenes' cast)
message stalker-unload ( -- )                \ (a stalker) out of slot 2 (and out of the game)
