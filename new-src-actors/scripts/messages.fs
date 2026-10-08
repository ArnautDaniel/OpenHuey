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
