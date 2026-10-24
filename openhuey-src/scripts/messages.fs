\ messages.fs - every message the actors pass, with its arguments: the game's API. Each
\ subsystem's page (docs/subsystems/) says who sends and who takes them.
IN: messages
USING: actors ;

\ ---- acoustics (docs/subsystems/acoustics.md) ----
\ a noise this frame: at nav triangle `tri` (-1: at a door), or at `door` (-1: none)
message noise ( loud room tri door source -- )
message zone-target ( zone x y z -- )       \ broadcast (the story) a zone set this frame that a stalker's blow may strike instead of her (its kind's bit 1: Events_NearestPoint) - its centre, cells of floats
message zone-targets-off ( -- )             \ broadcast (the story) its zones cleared (each frame, before they are set again)
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
message room-preload ( room -- )              \ (the story) load that room ahead: a forced exit's ($80..) way
message exit-prepare ( exit -- )              \ (the story) load the room behind that exit ahead
message leaving-room ( room exit -- )        \ the played room is being left by this exit
message arrived ( room exit -- )             \ the new room is in; arrivals come by this exit
message entered-room ( room exit -- )        \ the room is in play

\ ---- camera (docs/subsystems/camera.md) ----
message camera-cut ( -- )                    \ it cut to a new set
message hewie-told ( what arg -- )            \ (Hewie to Fiona, barking low: Hewie_StateLookLow) 0 a shut door at his exit `arg` - to open; 1 where he is
message open-from-afar ( room exit kind source -- )   \ (to the doors) taken hold of and let go open, out of sight (DoorHold_Take / DoorHold_Open)
message stalker-as ( kind -- )               \ (to a stalker, as the story loads him: partner-load) the kind he is
message hewie-gesture ( g -- )               \ (Hewie, played: each frame) his gesture this frame (stick-gesture; the scripts' 0x21)
message hewie-restart ( -- )                 \ (the run's clock starting: Hewie_Restart) him afresh - played, in the run
message hewie-yelp ( time anim -- )          \ (Hewie) hidden away, the animation he plays while `time` counts (-1: as it is) (0x77)
message hewie-play ( anim -- )               \ (Hewie) his motion plays `anim` (0xD7)
message hewie-hold ( on -- )                 \ (Hewie) a 5-frame wait, and his next choice forced to sit (or not) (0xBB)
message hewie-costume ( kind -- )           \ (Hewie) his model swapped for kind 0..2 (the story's 0xB7)
message show-avoid ( row -- )                 \ (the avoid prompt) its row shown for a second (the story's 0xB4)
message event-shot ( on dist pitch yaw rise -- )   \ the story's own shot of the one followed, or back (thousandths; 0x82)

\ ---- creatures (docs/subsystems/creatures.md) ----
\ the slots: spirits 0..6 (the story's own one 6), homunculi 7..9; a homunculus's kind has bit 7
10 constant #creatures   6 constant story-spirit   7 constant first-homunculus   $80 constant homunculus-kind
\ how a creature moves (creature-doing's mode, the original's +0xF8): as it will; hanging about
\ her (she can't be reached); struck or going; holding her down
enum: creature-move  creature-free 2 creature-about 4 creature-struck 8 creature-holding ;
message creature-place ( room level tri kind reach which yaw height -- )   \ (creatures) the story's 0x8A: one placed (height f>cell)
message creature-clear ( slot -- )           \ (creatures) the creature in that slot gone (a room's command: Room56)
message creatures-clear ( which -- )         \ (creatures) 0 all, 1 the spirits (0..6), 2 the homunculi (7..9) gone (0xCB)
message creature-count ( n other -- )        \ (creatures) how many to scatter about (0x99)
message spirit-setup ( slot room level tri kind reach -- )   \ (a spirit) who it is and where
message homunculus-setup ( slot room level tri kind sight yaw height -- )   \ (a homunculus) who it is, where, facing (degrees), dropped from (f>cell)
message homunculus-kept ( slot -- )         \ (a homunculus) as its progress record says
message spirit-sent ( slot room company -- )  \ (a spirit) new in that room, sent after her: resting first (company: someone up in her room)
message spirit-kept ( slot -- )              \ (a spirit) as its progress record says (a loaded game)
message creature-gone ( slot -- )            \ (creatures) it has vanished: its slot is free
message creature-doing ( slot room mode up -- )  \ broadcast each frame by each creature: where, how it moves (creature-move), up (after her: what Hewie goes for)
message fiona-request ( kind x y z threat -- )   \ (Fiona) a request from there (Progress_Noise; xyz>cells, threat f>cell): request-kind
message game-loaded ( -- )                   \ broadcast: a save's progress is back (saves.fs) - rebuild from it

\ ---- doors (docs/subsystems/doors.md) ----
\ kinds of character, for locks: who is kept out
0 constant fiona-kind   1 constant hewie-kind   2 constant stalker-kind
message lock ( door -- )
message unlock ( door -- )
message lock-for ( door kind on -- )         \ locked against a kind of character, or not
message lock-room-for ( room kind on -- )    \ the same for every door into a room
message redo-doors ( -- )                    \ each door in the played room stands as its state says (Doors_RoomIn)
message copy-door ( door from -- )           \ door takes on from's open bit, lock and closed-off (story's 0x4D)
message close-off ( door -- )                \ gone for good (story): its exits use another door once rebuilt
message reopen ( door -- )                   \ back (story; Rooms_Reopen)
message rebuild-exits ( -- )                 \ the rooms' exits through the doors not closed off (story's 0xA1)
\ the played room's doors, by exit; `source` is the user's noise source
message use-door ( exit anim source -- )     \ the door swings along the user's animation
message swing-door ( exit open source -- )   \ swung open / shut by hand
message pull-shut ( exit source -- )         \ taken hold of and swung shut behind its user (DoorHold_Take, DoorHold_Shut)
message slam ( exit source -- )
message barge ( exit source -- )             \ burst open (Fiona running into it)
\ any room's doors (off-screen users too): take hold of one before going through, then let
\ it go open or shut; the doors answer `door-held` or `door-refused`
message hold-door ( room exit kind -- )
message door-held ( room exit -- )
message door-refused ( room exit -- )
message let-go-open ( room exit source -- )
message let-go-shut ( room exit source -- )
message door-changed ( door -- )             \ broadcast: its state changed

\ ---- danger (docs/subsystems/danger.md) ----
message danger-signal ( signal -- )          \ something this frame (below)
message stalker-here ( room mode down -- )   \ the stalker in play this frame: his mode, knocked down?
message danger ( level -- )                  \ broadcast: calm, followed, chased
enum: danger-level  calm followed chased ;
\ the frame's signals (the original's condition bits): Fiona within 150 / 200 of the stalker,
\ Fiona struck, no stalker in play, the stalker knocked down, Hewie alert to him, she's hunted
enum: danger-signal  stalker-close fiona-struck no-stalker stalker-down hewie-alert stalker-near being-hunted ;
\ a stalker's mode (the original's +0x16C8, Progress_StalkerAlert): what he is about
enum: stalker-mode  after-her heading-for-her searching waiting-about held-off ;
\ what he is doing as he tells it (stalker-doing's sub, the part of his move Hewie and the story
\ read): bitten (Hewie holding on), down, leading her off
9 constant bitten-sub   $A constant down-sub   $18 constant leading-sub

\ ---- panic (docs/subsystems/panic.md); amounts and levels are floats (f>cell) ----
message fright ( amount -- )                 \ (Panic_Fright: less with earrings $87 / $88 worn; f>cell)
message fright-raw ( amount -- )             \ (Panic_FrightRaw: as it is)
message panic-shorten ( frames -- )          \ (the panic) its time left shortened (the progress' +0x7BA: her shaking as she gets up)
message fear-set ( amount -- )               \ the fear this frame set outright (the progress' +0x7D8 =: f>cell)
message fear-in ( amount -- )
message panic-overlay ( thousandths -- )       \ (the panic) its screen overlay's level (ScreenFade_Level of the progress' +0x7B8): the game over sets it full and fades it with its tints
message panic-stage! ( stage -- )
message panic-level! ( level -- )
message panic ( stage level -- )             \ broadcast each frame
\ the panic's stages: under 60, from 60 / 75 / 90, panicking (at 100), calming down after
enum: panic-stage  composed shaken scared terrified panicking calming-down ;

\ ---- Fiona (docs/subsystems/fiona.md) ----
\ Fiona's mode (the original's moveMode): moving on her own; led (a door, a scripted move, a walk
\ to a spot); on a ladder; thrown or stumbling; defending (a shove, a kick); request 9; down;
\ reacting (a flinch, fleeing, caught); held; commanding Hewie; letting go of a thing
enum: fiona-mode  own-mode 2 led-mode ladder-mode thrown-mode 8 defend-mode request-9-mode down-mode
    react-mode held-mode command-mode throw-mode ;
\ what she is doing (+0x1AD580, the story's condition 0x3F): nothing; turning to push; on a
\ ladder; at a door; a shove; a kick; letting go of a thing; flinching; thrown; down;
\ commanding; fleeing; a panic attack; out of breath; the story's moves - an animation, walking
\ to a spot, along a path, running there, along a path, turning to someone, to a heading,
\ walking in an animation
enum: fiona-doing  free-doing push-doing ladder-doing door-doing 5 shove-doing kick-doing let-go-doing
    flinch-doing 10 thrown-doing down-doing command-doing flee-doing attack-doing breathless-doing
    $11 anim-doing walk-to-doing walk-path-doing run-to-doing run-path-doing turn-who-doing
    turn-heading-doing walk-anim-doing ;
\ her condition: well, hurt, down
enum: fiona-cond  well-cond hurt-cond down-cond ;
\ her reactions (Fiona_Reaction: her sub while she is thrown-mode): knocked off a ladder; led away;
\ thrown; thrown by a door; knocked down on a step or in a doorway, harder; knocked down, harder;
\ grabbed; held down by homunculi; a spirit's fall; caught in a hold
8 constant off-ladder-r   9 constant led-away-r   $A constant thrown-r   $B constant door-thrown-r
$C constant step-knock-r   $D constant step-knock-hard-r   $E constant knock-r   $F constant knock-hard-r
$10 constant grabbed-r   $12 constant held-down-r   $13 constant spirit-fall-r   $20 constant caught-r
-1 constant no-reaction
\ her costumes (the story's variable $26, the `costume` message; the wardrobe's names): her
\ clothes, the slip she wakes in, Normal Outfit 2, Under the Scalpel, Illegal in Some States,
\ Texas Cowgirl, Fiona the Frog (4 and 5: none)
enum: costume  normal-outfit slip-dress normal-outfit-2 scalpel-outfit 6 illegal-outfit cowgirl-outfit frog-outfit ;
\ what a stalker or a creature asks of her (Fiona_Reaction's requests): a blow, a harder blow, a
\ grab, a throw, a door pushed into her, a hold; led away; held down by homunculi; a spirit
enum: request-kind  1 blow-request heavy-blow-request grab-request throw-request door-request
    hold-request $A lead-request $C homunculi-request spirit-request ;
\ her commands to Hewie (the right stick's gestures; Fiona_HewieCommandAction): go there; get up;
\ stay; ($26: nobody gives it); come; a pat; praise from afar; tending him when he's down; a scold
\ near; come back; go for it; go for it now (chased); a scold from afar; her cry for help
\ her gestures to him (the right stick, her-cmd: told in fiona-doing): up, down, R3, right, left
enum: stick-gesture  -1 no-gesture go-gesture back-gesture stay-gesture praise-gesture scold-gesture ;
enum: hewie-command  $23 go-there-cmd rouse-cmd stay-cmd cmd-26 come-cmd pat-cmd praise-cmd
    tend-cmd scold-near-cmd come-back-cmd go-for-it-cmd attack-now-cmd scold-cmd help-cmd ;
message flinch ( -- )                       \ (Fiona) flinching as if caught, nobody holding her (a room's command: action $B / $21)
message count-shakes ( on? -- )             \ (Fiona) her shakes counted from now (Fiona_Shakes: the story's old-mansion-b1-3), or not
message fiona-shakes ( n -- )               \ broadcast each frame while counting: her shakes so far
message fiona-shaking ( n -- )              \ broadcast each frame: her shakes this frame (the stick swung, a button - Fiona_Shakes)
message fiona-struck-by ( who -- )           \ broadcast each frame after fiona-doing: who struck or caught her (+0x100; -1 nobody)
message fiona-doing ( mode sub cond cmd doing -- ) \ broadcast each frame: her move mode, what she does, her
                                             \ condition, her command this frame (-1 none; 3 the praise held), her
                                             \ action (+0x1AD580: her-doing)
message fiona-blow ( n damage x y z reach -- )   \ broadcast: her blow `n` there (f>cell; reach -1: what touches her) - for the homunculi
message throw-down ( who recover -- )        \ (Fiona) thrown down by who (an actor, -1 none); recover: her recovery reset (the story's 0xB2)
message look-hold ( who frames -- )          \ (Fiona) her head on who for so many frames (the story's 0x96)
message fiona-unseen ( on -- )               \ broadcast when it changes: she can't be seen (item $89 worn, still)
message fiona-plight ( state ladder -- )     \ broadcast each frame: how she stands for a stalker's attack (plight); the ladder she is on (-1 none)
\ how she stands for a stalker's attack (Pursuer_FionaState; his tables by it): free; fleeing; frightened
\ (her fear past 90, her panic attack); out of reach (struck, seized, the stalkers blind); panicking; calming down
enum: plight  free-plight fleeing-plight frightened-plight unreachable-plight panicking-plight calming-plight ;
                                             \ (Pursuer_FionaState): 0 calm, 1 fleeing, 2 panicking, 3 out
                                             \ of reach (hidden, already struck), 4 / 5 the panic's stages 4 / 5
\ ---- blows (F4, the stalkers' P2: Relation_Request kind 4 to Fiona / Hewie) ----
message hit ( kind how fright -- )           \ from the sender: kind 1 / 2 a blow, 4 a hard one (thrown), 6 a
                                             \ hold (caught: how is the grip, 1 an arm .. 4; 5 thrown off),
                                             \ $8000 in how a stumble; fright (a float cell) for her panic
message hit-taken ( kind -- )                \ the answer: it struck home (none: it didn't)
\ ---- seized (F4b, the stalkers' P2b: Relation_Request kind 9, Fiona_JointAction kind 1) ----
message seize ( type -- )                    \ to Fiona: taken to the sender's side - 6 by the hand (dragged),
                                             \ 8 walking (carried off)
6 constant by-the-hand   8 constant walked-off   \ (seize's types)
message seize-taken ( -- )                   \ her answer: she's being led
message seize-refused ( -- )                 \ her answer: no spot by him she can be led to (or she isn't free)
message broke-free ( -- )                    \ to the stalker leading her: she shook free
\ ---- her blows on him (F4, the stalkers' P3: Relation_Request kinds 1 / 2 from Fiona; 5 a door) ----
message blow-from ( kind damage how x y z -- )   \ a blow from a point, not from anyone (Progress_Noise: the slot's +0x1060; xyz>cells)
message blow ( kind damage how -- )          \ to a stalker (or Hewie): its kind (blow-kind); how: $8000 he stumbles,
                                             \ 1 he may (her kick: by how worn he is - Fiona_ChaseRoll, rolled by
                                             \ him); a door's: its exit
\ the blows' kinds: a shove, a kick, a killing one, a hard one, a door; (6: one Hewie takes no
\ notice of); a powder trodden on (how: its loops); a scuffle off screen (Hewie's and the
\ stalker's); Hewie's hold, his bite (on the stalker); his throw (Hewie thrown off by it)
enum: blow-kind  1 shove-blow kick-blow killing-blow hard-blow door-blow ignored-blow powder-blow scuffle-blow
    $A hold-blow bite-blow  $B throw-off-blow ;
message blow-taken ( -- )                    \ the answer: it struck him
message game-over-kind ( kind -- )           \ (the game over) how the next one goes: 0 the movie's, 1 faded out, 2 the ending
message the-ending ( -- )                     \ (the game over, kind 2) the story is over: the staff roll and the results (ending.fs)
                                             \ (caught), 2 none (a continue), 3 faded out with its own music
message unhand ( -- )                        \ to Fiona: the stalker leading her no longer does

\ ---- Hewie (docs/subsystems/hewie.md) ----
message write-save ( -- )                    \ broadcast: print your line of the save being written (saves.fs)
message hewie-health ( hp -- )              \ broadcast each frame with hewie-doing: his health, 0..100
message set-hp ( n -- )                    \ (Hewie) his health set (a room's command)
message hewie-restore ( trust points hp -- )   \ (Hewie) as a save had him
message hewie-mood-restore ( mood time hide f0 f1 f2 f3 -- )   \ (Hewie) his mood and its time, staying put, his feelings about the stalkers, as a save had them
message hewie-skills ( s0 s1 s2 s3 s4 s5 -- )  \ (Hewie) his skills at fetching, as a save had them
message fetch-this ( thing -- )                \ (Hewie) Fiona threw the ball: his to fetch (things.fs)
message effect-place ( x y z -- )           \ (a room effect) moved there (f>cell)
message effect-rest ( n -- )                \ (a flickering sprite) its rest before its next run
message reflect-in ( slot -- )            \ (a reflection, effects.fs) its engine slot
message keri-drop ( k tri x y z -- )         \ (things) the story's debris: a kind-9 thing carrying room object "keri<k>", falling from there (f>cell)
message trap-place ( tri x y z -- )          \ (things) the story's trap: a kind-10 thing set down there (f>cell)
message story-things-off ( -- )             \ (things) the story's kinds 9 and 10 all gone
message keri-fell ( k -- )                   \ broadcast by the things: a kind-9 thing fell through, its "keri<k>" free again
message thing-taken ( thing -- )               \ (things) Hewie picked it up: gone
message join-fiona ( -- )                    \ he comes into the game at her heel (the story; the console)
message part-from-fiona ( -- )               \ he leaves the game
message hewie-doing ( here action mode sub cond mood group commandable near reachable breakable way trust -- )
                                             \ broadcast each frame: here 0 out of the game, 1 in another room, 2
                                             \ with her; his action, move mode and its part, condition, mood, his
                                             \ pose's group; for the story: she may command him (Hewie_FionaCanCommand),
                                             \ near enough for it (Hewie_FionaNearCommand), he can reach her
                                             \ (Hewie_FionaReachable), he may be broken off (Hewie_MayBreakOff), his
                                             \ half of the room (+0xF3668), his trust level
message command ( code tri yaw x z -- )      \ Fiona's command (0x23 go there: the spot; floats f>cell)
message reaction ( n -- )                    \ how her doing strikes him (Fiona_HewieReact's row: hewie-reaction)
\ what she did, as it strikes Hewie: her "go", "come back", "come" / "stay", a scolding and a praise
\ from afar, a scolding close up, a pat, rousing him; the story's (its 0xBC); her turn to flee, her
\ shove, her kick, a throw; (13: none); her cry for help, her "attack now"; his nudge (obedience)
enum: hewie-reaction  go-reaction come-back-reaction come-reaction scold-reaction praise-reaction
    scold-close-reaction pat-reaction rouse-reaction story-reaction flee-reaction shove-reaction
    kick-reaction throw-reaction 14 help-reaction attack-now-reaction nudge-reaction ;
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
message camera-hold ( -- )                   \ (the game over -> the camera) the room's next set, nobody followed, from there on
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
message thing-made ( kind tri x y z vx vy vz -- )   \ (Fiona's let go) a thing at her hand, on its way (floats f>cell; things.fs)
message fiona-message ( msg -- )             \ broadcast (Fiona: a locked door's built-in script) the room shows system message msg
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
\ the story's moves (Fiona_Requests' commands): stand; her idle's step; through a door at exit a,
\ opening it / shutting it; walk / run to the spot; an animation (a), blended (a over b frames),
\ blended with no time of its own; walk / run there by a path; look at a character (a) / a point;
\ turn to a character / to the move's heading; an idle (b); walk to the spot in an animation (a)
enum: move-kind  1 stand-move idle-step-move open-door-move shut-door-move walk-move run-move
    anim-move blend-move blend8-move walk-path-move run-path-move look-move look-point-move
    turn-to-who-move turn-to-heading-move idle-anim-move walk-anim-move
    bark-move leap-move spot-move still-anim-move ;   \ (Hewie's own: a bark, a leap to the spot, milling about it, an animation not moving him)
message moving ( done ended events -- )      \ broadcast at each frame's end by a character: its move done, its animation ended, its motion's event keys
message hold-anim ( anim flags blend -- )    \ (to a character) play this animation and stay in it (the scripts' 0x9D: its flags - 1 loops - and frames faded in)
message show ( on -- )                       \ (to a character) shown or hidden (the scripts' 0x1F)

\ ---- the screen and the music (docs/subsystems/screen.md, music.md) ----
message fade ( frames kind -- )              \ (the screen) fade in (kind 0 / 1) or out over frames
message fade-finish ( -- )                   \ (the screen) the fade counts as over
message fade-done ( -- )                     \ (to who faded) it's over
message bgm-want ( track pause level -- )    \ (the music) background track wanted ($FF none), paused, its level (f>cell)
message bgm-resume ( -- )                    \ (the music) the streamed track resumed
message music-op ( op a b -- )               \ (the music) the stage music: 0 over a frames to volume b, 2 hold, 4 release, 5 silence
message music-duck ( v -- )                   \ (the music) the stream turned down to v (f>cell, 0..1) - the pause, the sub screen (Bgm +0x114)
message music-stage ( stage -- )             \ (the music) the stage set's music made (-1: ended)
\ ---- scenes (the cutscene director: docs/subsystems/story.md S4) ----
message scene ( on -- )                      \ broadcast: a cutscene has the characters (on) / gives them back (off)
\ ---- the rooms' effects (docs/subsystems/effects.md) ----
message particles-made ( p -- )               \ (a particles actor) the engine's system it keeps
message effect-command ( v -- )                  \ (an effect) a command later (the scripts' 0x87: 1 still, 2 moving)
\ ---- the stalkers (docs/subsystems/stalker.md) ----
message stalker-in ( room tri -- )           \ (a stalker) into the game, in that room on that triangle
message stalker-restore-more ( cond hp-max rage temper hits mode-t down-t yaw -- )   \ (a stalker) the rest of how a save had him (Pursuer_LoadState)
message stalker-restore ( hp mode -- )      \ (a stalker) as a save had him
message stalker-out ( -- )                   \ (a stalker) out of the game
message stalker-hunt ( -- )                  \ (a stalker) heads for Fiona's room (mode 1: the summoner, the console)
message costume ( n -- )                     \ (Fiona) her model for costume n (0 her clothes, 1 the slip)
message stalker-load ( slot -- )            \ (a stalker) into the story's character slot (2: the one in play; 3..5 a scene's figure) - the cutscenes' cast
message stalker-unload ( -- )                \ (a stalker) out of slot 2 (and out of the game)
message stalker-doing ( room mode sub cond hp hp-max kind alert came -- )   \ broadcast each frame by a stalker in the
message stalker-reach ( d -- )                 \ broadcast each frame by a stalker: how far Fiona is on foot (a float in a cell; -1 no way, elsewhere - Pursuer +0x1588)
                                             \ game: his room, how he moves (0 his own, 3 off screen, 4 struck, 8 taking her),
                                             \ what (9 Hewie on him, $A down, $18 leading her), condition, health, his kind,
                                             \ his alert (his mode: Progress_StalkerAlert), the exit he came in by (-1 none)
\ ---- the story's orders to Hewie, Fiona and the panic (the scripts' commands; H4 / F5) ----
message drawn-to ( tri -- )                  \ (the things: the doll) a stalker searching the room being played drawn to
                                             \ it, if he sees it and can get there - his search's one stop
message stalker-blow ( n -- )                \ broadcast each frame by a stalker in the game, after stalker-doing:
                                             \ the blow of his attack under way (its move's damage), -1 none
                                             \ (Pursuer_PathNodeSound, as hg names it)
message do-action ( action arg -- )          \ 0x39: his action (Hewie_SetAction)
message trust+ ( n -- )                      \ 0x85: his trust (Hewie_AddTrust)
message mood! ( mood -- )                    \ 0xC4: his mood (Hewie_SetMode, its own time)
message scent-now ( x y z -- )               \ (Hewie) his point to look to, whatever he had (a room's command)
message scent-at ( x y z -- )                \ 0xC5 / 0xC6: a point to look to, if he has none (floats f>cell)
message hewie-place ( room tri side -- )     \ 0x3F: put in a room on a triangle, on its half (his +0x64)
message hewie-side! ( side -- )              \ 0xD0: his half of the room (0..2, else none)
message hewie-stay ( -- )                    \ 0xBD: +0xF3688 300 frames
message noclip ( on -- )                     \ 0x20: through blocked floor (+0x2B)
message aside ( on -- )                      \ 0x21: left alone (+0x2D)
message fear-less ( n -- )                   \ 0x94: Fiona calmer by n / 30 (Fiona_CalmDown)
message recovery-less ( n -- )               \ 0x95: her recovery less 60 / n frames (Fiona_LowerRecovery)
message threat ( amount -- )                 \ 0x43: the panic's threat (Threat_Raise; f>cell)
message threat-acc ( amount -- )             \ 0xC0: the threat's accumulator (+0x7DC; f>cell)
message calm-in ( v -- )                     \ 0x93: calming (bit $80: 128 at once, else v / 30)
message panic-reach ( level -- )             \ 0xC9: the level set to it if not above it (Panic_SetLevel)
\ ---- the story's orders to a character (the scripts' commands; P4) ----
message stalker-order ( mode -- )            \ 0x2F: 0 after her, 2 to her room / searching here, 3 searching
message stalker-clock ( frames -- )          \ 0x2A: his mode's clock (+0x1660)
message stalker-knock-out ( -- )             \ 0x30: knocked down, out of health
message rage ( on -- )                       \ 0x31: in a rage (Pursuer_SetRage: his stance and walk)
message activate ( -- )                      \ 0x15: (re)started where it is, afresh (Pursuer_ / Fiona_ / Hewie_Activate)
message set-cond ( v -- )                    \ 0x47: his condition (+0xC4: 0 well, 1 hurt, 2 down)
message full-health ( -- )                   \ 0x48
message heal ( n -- )                        \ 0xB8: by n, to his most
message silent ( on -- )                     \ 0x3D: his own sounds don't play
\ ---- Hewie at the stalker (H3, the stalkers' P3c): his attack is a joint action (Relation_Request
\ kind 9 / Pursuer_GrabOrder): asked, then taken - with the spot to leap to - or refused; his bites
\ (kind $B) while he holds on; either lets go (their state 7) ----
message dog-attack ( kind -- )               \ (Hewie -> the stalker) his attack (dog-attack-kind)
\ Hewie's attacks: at the near flank (charged), the far one (charged), head on, his leg
enum: dog-attack-kind  10 near-flank-attack near-charged-attack far-flank-attack far-charged-attack head-on-attack leg-attack ;
message dog-grip ( tri x y z yaw -- )        \ (the stalker's answer) taken: leap to there, facing so (floats f>cell)
message dog-refused ( -- )                   \ (the stalker's answer) not now
message bite ( damage hard -- )              \ (Hewie -> the stalker he holds) a bite; hard: he stumbles
message shaken-off ( -- )                    \ (Hewie -> the stalker) he let go / (the stalker -> Hewie) thrown off
message snap ( -- )                          \ (Hewie -> the stalker) growled at, barked at (Relation kind 6): his temper up

\ ---- the summoner (docs/subsystems/summoner.md) ----
message stalker-status ( room mode in-game own away cool beaten -- )   \ (the stalker in slot 2, each frame while loaded) where he is, his mode, in the game, in his own rooms, his time-away base (frames), his cooldown (s), knocked out of health
message stalker-summoned ( room exit found plan -- )  \ (a stalker) in out of sight in `room` by `exit`; found: after her; else waiting about, plan: heading for her
message summoner-cooldown-is ( secs -- )      \ broadcast each frame by the summoner: its cooldown left, in seconds (the rooms' scripts)
message animal-go ( sound x z -- )            \ (a rabbit, the cast) grazing where it is; off to (x, z) when one comes near (Kind26_MoveToB)
message grab-hewie ( -- )                    \ (to the stalker) a room's scene: grab Hewie from behind, if there's a spot
message grab-answer ( ok -- )                 \ (from the stalker) whether he could
message hold-spot ( tri x y z yaw -- )        \ (to Hewie) where he is held from behind (floats f>cell)
message set-level ( level -- )              \ (a creature) its floor of a split room (CreatureA_SetSaveSlot: a room's command)
message put-away ( -- )                      \ (a spirit) put away by a room's command: it vanishes, out of play
message noise-was ( loud room -- )            \ broadcast at the frame's end: each noise made this frame (the rooms' scripts)
message noise-made ( loud room door -- )      \ broadcast at the frame's end: Fiona's loudest noise this frame (door -1: none)
message summoner-take ( kind -- )             \ (the summoner) take the stalker out (0 the story / a wait, 1 held off, 2 beaten)
message summoner-cooldown ( secs -- )         \ (the summoner) not before secs seconds
message summoner-cooldown-less ( secs -- )    \ (the summoner) that much sooner
message summoner-reset ( -- )                 \ (the summoner) afresh (a stalker loaded)
message summoner-restore ( away called cooldown room waited mode kind -- )   \ (the summoner) as a save had it (its progress +0x764): time away, called in, cooldown (frames), the room he was taken from, frames waited, his mode last seen, how he was taken

\ ---- the obstacles (docs/subsystems/obstacles.md): the things pushed about ----
message place-obstacle ( i n kind a b -- )    \ obstacle i (model "oshi0n", of a kind) placed at square (a, b)
message place-obstacle-kept ( i n kind -- )   \ ... at the place kept for it
message stop-obstacle ( i -- )                \ it can't be pushed any more
message keep-obstacle ( i -- )                \ its place kept (where it is now)
message keep-obstacle-at ( i a b -- )         \ ... that square
message keep-obstacle-spot ( i -- )           \ where it stands kept (for its model)
message obstacle-model-to-spot ( i n -- )     \ model "oshi0n" its model, at the spot kept
message push-obstacle ( i dx dz -- )          \ (Fiona) pushed two squares along (dx, dz; floats f>cell)
message obstacle-moved ( i -- )               \ (Fiona) its move done

\ ---- the event characters (docs/subsystems/cast.md): the figures in slots 3..5 ----
message cast-load ( id -- )                   \ (a figure) be character `id` (its kind's model), loaded, not yet placed
message cast-unload ( -- )                    \ (a figure) let it go
message cast-out ( -- )                       \ (a figure) out of the scene, still loaded (the scripts' 0x14)
message cast-dress ( model -- )               \ (a figure) wear that model (the scripts' hand-over 0xBA)

\ ---- the items (docs/subsystems/items.md): the inventory, kept in the progress ----
message give-item ( id n -- )                 \ (the items) n of item id into the inventory (Items_Give)
message take-item ( id -- )                   \ (the items) one of item id spent, wherever it is (Items_UseId; the scripts' 0x0C)
message add-file ( id sound? -- )             \ (the items) a file found (a key thing, a document: message ids); its sound if new
message use-item ( list place -- )            \ (the items) the item there used (the menu); answered with item-used
message item-used ( id result -- )            \ (from the items) 0 nothing happened, 1 used up, 2 a mark set, 4 an event
                                              \ started (the menu closes on it), 5 she / Hewie act on it, 8 a hint only
message discard-item ( list place -- )        \ (the items) one of the item there thrown away
message equip-item ( list place -- )          \ (the items) worn, in its kind's slot
message unequip-item ( list place -- )        \ (the items) taken off
message sort-items ( list -- )                \ (the items) a list put in order
message item-bit ( n -- )                     \ (the items) item n marked as used (its note changes; the scripts' 0x5D)
message equipment ( boots earrings costume choker -- )   \ broadcast: what she wears changed (ids, -1 none)
message item-event ( who ev var value -- )    \ (items -> the room) character slot who's action script ev; first the
                                              \ script variable var set to value (var -1: none) - an item used here
message remedy ( fear recovery -- )           \ (items -> Fiona) her fear and her recovery's count changed by these
message soothe ( calm fear -- )               \ (items -> the panic) calming at once, and fear, by these
message fed ( how -- )                        \ (items -> Hewie) given a bait (bait)
\ the baits: a treat (20 health), a big one (100), an onion (it hurts him), a fury (Sylvesta Esca:
\ angry 450 frames), one that gets him up
enum: bait  small-treat big-treat onion-bait fury-bait rouse-bait ;
message tab-op ( op item -- )                \ (the sub screen) the tab: 0 an item handed her (its picture), 1 loaded, 2 in, 3 shown, 4 out
message tab-done ( -- )                       \ (from the sub screen) the tab's step is done (it waits on 2 / 4)
message menu-mode ( mode -- )                 \ (the sub screen) the mode the scripts open it in (0x6D: 0 the menu, 1 saving ...)
message map-turn-to ( page -- )              \ (the sub screen) her room shown on that page of her map (the scripts' 0xD1)
message options-changed ( vib out vol x y layout -- )   \ (the sub screen -> saves) the options as now set: kept in the system data
message options-restore ( vib out vol x y layout -- )   \ (saves -> the sub screen) the options the system data kept
message give-plate ( c0 c1 c2 c3 c4 c5 c6 c7 -- )   \ (the items) a word plate made, its 8 letters (0: none)
message door-opened ( door on -- )            \ (the doors) its open bit set / cleared (the story's 0x59 09 / 0A)
message calm-reset ( -- )                     \ (Fiona) her fear and her recovery's count back to 0 (the story's 0x4E)
message recovery-reset ( -- )                 \ (Fiona) her recovery's count back to 1800 as after a fright (the story's 0x4F; the chokers spare her)
message fear-to ( v -- )                      \ (Fiona) her fear set (0..100; the story's 0xBF)
message save-to-slot ( n place -- )   \ (saves) the game into save slot n (0..7), made at that place (its name's message)
message load-from-slot ( n -- )       \ (saves) the game in save slot n back
message game-saved ( -- )             \ (saves) to who asked: the save is written
message save-to-file ( addr len -- )  \ (saves) the game saved into that file (a bug report's: debug.fs)
message load-from-file ( addr len -- )   \ (saves) the game in that file back
message item-var ( var value -- )          \ (items -> the room) its script variable var set to value (an item used: the word plate)
message item-event-bit ( n on -- )         \ (items -> the room) its event bit n set or cleared (the word plate: the word was right)
message meters? ( -- )                       \ (debug: to Fiona) her meters, please
message her-meters ( fear breathless run -- )  \ (debug: Fiona's reply) her fear, out of breath, the running left
