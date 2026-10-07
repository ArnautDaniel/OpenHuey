\ events/words.fs - the words the converted event scripts are written in: one per
\ command and condition of the original bytecode (docs/event_opcodes.md). Generated
\ as stubs by tools/events2forth.py; each is to be written for real (in Forth on top of
\ C words), after which this file is the source.
IN: events.words

\ what the stub conditions answer, and what the stub commands do (tests: random, yield)
defer stub-flag  ' false is stub-flag
defer stub-step  ' noop is stub-step

\ ---- running scripts

\ the task goes on in script `xt` and never comes back (the bytecode's 0x25)
: goto ( xt -- )  drop stub-step ;
\ the same by id, in the current room (the shared scripts don't know their room)
: goto-action ( id -- )  drop ;
: call-action ( id -- )  drop ;
: room-command ( bytes.. n cmd -- )  drop 0 ?do drop loop ;
: room-condition? ( bytes.. n cond -- flag )  drop 0 ?do drop loop false ;
\ the scripts by room: slot 0 entering, 1..5 the phases, 6 a character entering; the
\ action scripts by room and id (0..$7F); the shared ones by id ($80..$FF). 0: none
$110 constant rooms
create room-scripts    rooms 7 * cells allot     room-scripts rooms 7 * cells 0 fill
create action-scripts  rooms $80 * cells allot   action-scripts rooms $80 * cells 0 fill
create builtin-scripts $80 cells allot           builtin-scripts $80 cells 0 fill
: room-script ( room slot -- addr )  swap 7 * + cells room-scripts + ;
: action-script ( room id -- addr )  swap $80 * + cells action-scripts + ;
: builtin-script ( id -- addr )  $80 - cells builtin-scripts + ;
: room-script! ( xt room slot -- )  room-script ! ;
: action-script! ( xt room id -- )  action-script ! ;
: builtin-script! ( xt id -- )  builtin-script ! ;

\ ---- commands

\ 00: Door / exit use: with bit 7 set, flags the progress (+0x4 = 1) and keeps the exit
\ (+0x702); otherwise, if the exit is unlocked and passable for whoever is controlled (Fiona
\ idle or walking; Hewie idle), +0x702 = progress +0x10 (empty in this game: 0).
: exit-check ( exit -- )  drop stub-step ;
\ 02: Places character `who` (0xFF: self) on nav triangle `tri` in the event's room.
: char-to-tri ( who tri -- )  drop drop stub-step ;
\ 03: Opens message window text `msg` (bit 0x4000: the second language table); the window
\ belongs to the script's character (+0x80C). See 0x09 to wait for it.
: message ( msg -- )  drop stub-step ;
\ 04: Places character `who` (0xFF: self) at the outside point of exit `exit`
\ (Rooms_ExitPointOut).
: char-to-exit ( who exit -- )  drop drop stub-step ;
\ 05: Starts action script `act` for `who`. For script slots (0xF0..0xFA) only if the slot is
\ free; for a character, only if it is not already in a scripted state (character state 5,
\ script action `act`). 0x92 is the forcing variant. `mode` is read by 0x92 only.
: action ( mode who act -- )  drop drop drop stub-step ;
\ 06: self: waits until the character is idle again; with no character (id -1) it ends this
\ action script (the slot is freed). Always waits a frame.
: self-idle-or-end ( -- )  stub-step ;
\ 07: self: plays animation `anim` (character move 7). (Not "walk to triangle".)
: self-anim ( anim -- )  drop stub-step ;
\ 08: self: waits until the move has finished (move state 0 and its motion's end flag 0x20).
: self-wait-anim ( -- )  stub-step ;
\ 09: Waits while the message window is open.
: wait-message ( -- )  stub-step ;
\ 0A: Requests a scene change (progress +0x1134 = scene, +0x113C = arg, +0x1151 = kind) when the
\ situation allows it (not while panicking for most kinds; kinds 0 / 3 may set the pending
\ ending instead). Scene 5 is refused while Fiona is busy.
: scene-change ( scene arg kind -- )  drop drop drop stub-step ;
\ 0B: Camera +0xB4 (which 0) or +0xB0 (else) with `v` (a camera distance / height).
: camera-value ( which F: v -- )  drop fdrop stub-step ;
\ 0C: self: goes to (x, z) on triangle `tri` (tri2 0xFFFF: none), then faces `face`; `move` the
\ character move (5 / 10 walk / run to a point, ...).
: self-move-to ( tri face tri2 move F: x z -- )  drop drop drop drop fdrop fdrop stub-step ;
\ 0D: self: waits until the current move is done (+0xE1 == 1).
: self-wait-done ( -- )  stub-step ;
\ 0E: self: move `move` with triangles `tri` / `tri2` (0xFFFF: none) (6 / 11 walk / run to a
\ triangle, 17 ...).
: self-move-tri ( tri tri2 move -- )  drop drop drop stub-step ;
\ 0F: self: back to idle (character move 2).
: self-idle ( -- )  stub-step ;
\ 10: self: move `move` toward character slot `slot` (14 turn to a character, ...).
: self-move-slot ( slot move -- )  drop drop stub-step ;
\ 11: Brings in character `id` as the partner (slot 2: loads and starts it, resets the
\ summoner).
: partner-load ( id -- )  drop stub-step ;
\ 12: Brings in character `id` in slot `slot`; placed as is (exit 0xFF) or with the motions for
\ coming in from the room behind `exit` of this room (Rooms_OtherRoom).
: char-load ( id slot exit -- )  drop drop drop stub-step ;
\ 13: Takes out the character in `slot` (waits while it is still loading).
: char-unload ( slot -- )  drop stub-step ;
\ 14: Character `id` leaves the scene (the partner slot: the summoner takes it).
: char-done ( id -- )  drop stub-step ;
\ 15: Activates character `id` (Progress_ActivateChar).
: char-activate ( id -- )  drop stub-step ;
\ 16: The event counter (+0x703) = n.
: counter-set ( n -- )  drop stub-step ;
\ 17: The event counter + 1.
: counter-inc ( -- )  stub-step ;
\ 18: Waits until the event counter is n.
: wait-counter ( n -- )  drop stub-step ;
\ 19: self: waits until this script's frame count (+0x14) is n.
: self-wait-frames ( n -- )  drop stub-step ;
\ 1A: self: this script's frame count = 0.
: self-frames-reset ( -- )  stub-step ;
\ 1B: self: waits one frame (the pc moves past it first).
\ (the core word `yield`)
\ 1C: self: waits until this script's frame count is 16.
: self-wait-16 ( -- )  stub-step ;
\ 1D: Nav-triangle flag groups (gRoomEventObj, NavGroups): set 1: +0xC, else +0x10 with group
\ and bits (sets / clears the flags of a group of triangles).
: nav-group ( set group bits -- )  drop drop drop stub-step ;
\ 1E: self: put at the inside point of exit `exit` (Rooms_ExitPointIn), facing the way through
\ it.
: self-to-exit-in ( exit -- )  drop stub-step ;
\ 1F: Character `who` shown or hidden (+0x29), if it is in the current room.
: char-visible ( who on -- )  drop drop stub-step ;
\ 20: self: the character's root motion ignores the nav blocking mask (+0x2B) - it walks through
\ blocked triangles (stalkers use it at doors).
: self-noclip ( on -- )  drop stub-step ;
\ 21: self: the character's +0x2D; turning it on for Fiona while her script state is 4 resets
\ that state.
: self-scripted ( on -- )  drop stub-step ;
\ 26: Script variable `var` = v.
: var-set ( var v -- )  drop drop stub-step ;
\ 27: Each character whose relation to area `area` is `rel`: its camera setup (+0xE8 / +0xEC) =
\ a, b.
: chars-area-camera ( area a b rel -- )  drop drop drop drop stub-step ;
\ 28: The camera follows character `id` (0xFF: nobody).
: camera-follow ( id -- )  drop stub-step ;
\ 29: The characters in this room inside area `area`: camera setup (+0xE8 / +0xEC) = a, b.
: area-camera ( area a b -- )  drop drop drop stub-step ;
\ 2A: Stalker `id`: frames it waits (+0x1660, default 900 = 15 s) before it searches or comes
\ after Fiona when she is in another room.
: stalker-search-delay ( id frames -- )  drop drop stub-step ;
\ 2B: self: looks at character `id` (character move 12; 0xFF: stop). (Not "follow".)
: self-look-at ( id -- )  drop stub-step ;
\ 2E: Stalker `id`: adds `point` to its route (0xFFFF: on to its next route point).
: stalker-route ( id point -- )  drop drop stub-step ;
\ 2F: Stalker `id`: 0 go for Fiona, 2 chase her from here, 3 start searching.
: stalker-mode ( id mode -- )  drop drop stub-step ;
\ 30: Stalker `id` is knocked down.
: stalker-knock-down ( id -- )  drop stub-step ;
\ 31: Stalker `id`'s rage on / off (virtual +0x31C: Pursuer_SetRage, +0x16B8 = 2 / 0).
: stalker-rage ( id on -- )  drop drop stub-step ;
\ 32: Sound channel `ch`'s volume (sound +0x7C).
: sound-volume ( ch vol -- )  drop drop stub-step ;
\ 33: Sends room effect `fx` the string (if not empty).
: effect-string ( fx text.. n -- )  0 ?do drop loop drop stub-step ;
\ 34: The renderer's display setting +0x304C04 on / off; nothing in the game reads it (unused by
\ the scripts).
: renderer-flag-304C04 ( on -- )  drop stub-step ;
\ 35: Room effect slot `fx` (under 0x20) made anew as a butterflies effect (argument 8), then
\ sent the command's 7 bytes from `args` on, as they are.
: butterflies ( fx args b b b b b b -- )  drop drop drop drop drop drop drop drop stub-step ;
\ 36: Character `id`'s camera setup (Progress_CameraSetup) a, b.
: char-camera ( id a b -- )  drop drop drop stub-step ;
\ 37: Script variable + 1.
: var-inc ( var -- )  drop stub-step ;
\ 38: Script variable - 1.
: var-dec ( var -- )  drop stub-step ;
\ 39: Hewie: action a with argument b (Hewie_SetAction).
: hewie-action ( a b -- )  drop drop stub-step ;
\ 3B: Places character `who` (0xFF: self) on triangle `tri` facing `face`.
: char-to-tri-facing ( who tri face -- )  drop drop drop stub-step ;
\ 3D: Character `who` silent (+0x2C): its own sounds (Actor_PlaySound) don't play.
: char-silent ( who on -- )  drop drop stub-step ;
\ 3E: Stalker `id` leaves the scene and is put into room `room` (0xFFFF: its own) at `at`,
\ entering as `how` (0..2) (its virtual +0x64).
: stalker-to-room ( id room at how -- )  drop drop drop drop stub-step ;
\ 3F: Hewie into room `room` at `at`, entering as `how` (his +0x64).
: hewie-to-room ( room how at -- )  drop drop drop stub-step ;
\ 40: The script slot of character slot `slot` is cleared and the character removed
\ (Progress_RemoveChar(slot, how)).
: char-remove ( slot how -- )  drop drop stub-step ;
\ 41: Progress variable n + 1 (the progress' byte variables, +0x9C).
: pvar-inc ( n -- )  drop stub-step ;
\ 42: self: turns to character `id` (character move 14).
: self-turn-to ( id -- )  drop stub-step ;
\ 43: Raises the threat / panic meter (progress +0x7B8) by v (0..100).
: threat-raise ( v -- )  drop stub-step ;
\ 45: Character `who` (0xFF: self) plays sound `id` (Actor_PlaySound).
: char-sound ( who id vol -- )  drop drop drop stub-step ;
\ 46: The doors redo their setup for the current room (Doors_RoomIn).
: doors-room-in ( -- )  stub-step ;
\ 47: Character `who`: +0xC4 = v (a stalker's presence state: 1 / 2 seen / near ...?).
: char-set-C4 ( who v -- )  drop drop stub-step ;
\ 48: Character `who`'s health back to its maximum (+0x14C8 = +0x14CC).
: char-full-health ( who -- )  drop stub-step ;
\ 49: Ends character `who`'s action script (script slots 0xF0..0xFA: the slot is freed); a
\ character in a scripted move is released (move 1).
: action-end ( who -- )  drop stub-step ;
\ 4A: The camera director restarts (CamDirector_Restart).
: camera-restart ( -- )  stub-step ;
\ 4B: self: turns to heading `face` (character move 15).
: self-turn-angle ( face -- )  drop stub-step ;
\ 4C: Door `door`: Doors_SetBits(door, a, b) (the progress' +0x68 is empty).
: door-bits ( a door b -- )  drop drop drop stub-step ;
\ 4D: Door `door` takes on door `from`'s states: open bit, lock, closed-off; and `from`'s exit
\ in this room saves its door state.
: door-copy ( door from -- )  drop drop stub-step ;
\ 4E: Fiona's fear (+0x1AD5F4, 0..100; over 90 she panics) and her exhaustion count (+0x1AD5F8,
\ up to 1800 frames) reset to 0.
: fiona-calm-reset ( -- )  stub-step ;
\ 4F: Fiona recovers (Fiona_ResetRecovery).
: fiona-recover ( -- )  stub-step ;
\ 51: Closes the message window if it shows `msg` (0xFFFF: any).
: message-close ( msg -- )  drop stub-step ;
\ 52: The playing movie loops (Movie.loop = 1).
: movie-loop ( -- )  stub-step ;
\ 53: Pushable obstacle `i` (model "oshi0n", kind `kind`) placed on squares a, b
\ (Obstacles_PlaceAt).
: obstacle-place ( i n kind a b -- )  drop drop drop drop drop stub-step ;
\ 54: Pushable obstacle `i` can no longer move (Obstacles_Stop).
: obstacle-stop ( i -- )  drop stub-step ;
\ 55: The screen fade colour `rgba`, drawn in renderer layer `layer` (Events_Fade).
: fade-colour ( rgba layer -- )  drop drop stub-step ;
\ 56: self (the player for script slots): knocks on / tries door `door` (sound 0x27 for how 1,
\ else 0x28), if it isn't open.
: self-door-knock ( door how -- )  drop drop stub-step ;
\ 57: Event bit n set (+0x890, cleared when the room changes).
: ebit-set ( n -- )  drop stub-step ;
\ 58: Event bit n cleared.
: ebit-clear ( n -- )  drop stub-step ;
\ 5A: Item `item`'s summon cooldown: set (less 0) or shortened (Summoner).
: item-cooldown ( item less -- )  drop drop stub-step ;
\ 5B: The stalker's item (+0x2D4) gets its cooldown set.
: stalker-item-cooldown ( -- )  stub-step ;
\ 5C: A screen fade over `frames` frames; kind & 0xF: 1 in, 4 out; bits 0xC0 fade the music with
\ it (0x80 at once, 0x40 over 90 frames); bits 0x30 ramp the volume (0x20 at once, 0x10 over the
\ fade). See 0x5F to wait.
: fade ( frames kind -- )  drop drop stub-step ;
\ 5D: Finishes the running fade now; marks a scene as playing (+0x11F3).
: fade-finish ( -- )  stub-step ;
\ 5E: The fade counts as over.
: fade-over ( -- )  stub-step ;
\ 5F: Waits for the fade to finish.
: wait-fade ( -- )  stub-step ;
\ 60: Plays the movie the room names as string `name` (room handler +0x34) with movie class
\ `class` (0x62 0 / 2 to follow it).
: movie-play ( name class -- )  drop drop stub-step ;
\ 61: Every active character is told (+0x78); the cutscene director restarts on the room's scene
\ script named by string `name`.
: cutscene-start ( name -- )  drop stub-step ;
\ 62: Movie / cutscene director control by `op`: 0 the movie's state into the result (+0x934: 2
\ playing, 1 other, -1 none); 1 stop the movie; 2 restart it (waits until it runs); 3 director
\ +0x10; 4 director +0x10 then +0x14 unless in mode 5; 5 / 6 / 7 the director's cues (before /
\ after; off; the next one from the movie frame - its button 11 toggles state flag 0x29); 8
\ camera director back to its default mode (CamDirector_ModeDefault), director +0x48, flag 0x29
\ off; 9 / 10 pause / resume the movie; 11 a black screen over half; 12 shows the prepared
\ message as often as the director says.
: cutscene-control ( op -- )  drop stub-step ;
\ 63: Hewie turns to heading `face` (Hewie action 0x72).
: hewie-face ( face -- )  drop stub-step ;
\ 64: The depth-range effect (room effect slot 0x1C) removed.
: depth-range-off ( -- )  stub-step ;
\ 65: Room effect slot 0x1C made anew as a depth range (DepthRange_Init) with the four values.
: depth-range ( F: a b c d -- )  fdrop fdrop fdrop fdrop stub-step ;
\ 66: A lit doorway for the room's lights (lights +0x38): four corners; its facing and middle
\ are derived.
: lights-doorway ( F: x0 y0 z0 x1 y1 z1 x2 y2 z2 x3 y3 z3 -- )  fdrop fdrop fdrop fdrop fdrop fdrop fdrop fdrop fdrop fdrop fdrop fdrop stub-step ;
\ 67: Character `who`'s nav triangle looked up from its position.
: char-find-tri ( who -- )  drop stub-step ;
\ 68: Sound `id` of bank `bank` & 0x3F; bank >> 6: 0 at (x, y, z) (only if the progress' +0x7C
\ allows: always in this game), 2 plain, else at the camera; vol / pitch offsets.
: sound ( id bank vol pitch F: x y z -- )  drop drop drop drop fdrop fdrop fdrop stub-step ;
\ 69: Stops sound `id` of bank `bank` (SndDriver_StopSound).
: sound-stop ( id bank -- )  drop drop stub-step ;
\ 6A: Stage music by `op`: 0 global volume fade to a over b frames (MusicDir_GlobalVolumeTo); 1
\ the stage's channels (+0x40); 2 load (+0xC) and hold (+0x1C); 3 waits until its banks are in;
\ 4 release; 5 silence.
: music ( op a b -- )  drop drop drop stub-step ;
\ 6D: Opens the sub-screen in mode `mode` (0 the in-game menu, 1 save, 2 the word plates, ...;
\ SubScreen.mode) and sets state flag 4.
: subscreen-open ( mode -- )  drop stub-step ;
\ 6E: The playing movie's Sofdec setting (Sofdec_SetParam(a, b)).
: movie-param ( a b -- )  drop drop stub-step ;
\ 6F: self: walks through exit `exit` of this room (character move 5 to the door's far point).
: self-through-exit ( exit -- )  drop stub-step ;
\ 70: self: as 0x6F, the other way through.
: self-through-exit-back ( exit -- )  drop stub-step ;
\ 71: Pad rumble: on 0 the small motor (1, 1), else strength `strength`, for `frames` (gRumble).
\ (Not a screen fade.)
: rumble ( on strength frames -- )  drop drop drop stub-step ;
\ 72: Pushable obstacle `i` (model "oshi0n", kind) placed at its saved squares
\ (Obstacles_PlaceSaved).
: obstacle-place-saved ( i n kind -- )  drop drop drop stub-step ;
\ 73: Pushable obstacle `i`'s saved squares = a, b (Obstacles_SetSaved).
: obstacle-save-at ( i a b -- )  drop drop drop stub-step ;
\ 74: Pushable obstacle `i`'s current squares saved (Obstacles_TakeSaved).
: obstacle-save ( i -- )  drop stub-step ;
\ 75: Pushable obstacle `i`'s spot kept (Obstacles_KeepSpot).
: obstacle-keep-spot ( i -- )  drop stub-step ;
\ 76: Pushable obstacle `i`'s model ("oshi0n" number n) put back at its kept spot
\ (Obstacles_ModelBack).
: obstacle-model-back ( i n -- )  drop drop stub-step ;
\ 77: Hewie plays animation `anim` (Hewie_SetAnim).
: hewie-anim ( a anim -- )  drop drop stub-step ;
\ 78: Hewie steps to a pose (3, or 0 in special modes) and barks (character move 0x12 -> his
\ action 0x45).
: hewie-bark ( -- )  stub-step ;
\ 79: Places character `who` at (x, z) on triangle `tri` (height from the triangle), facing
\ `face`.
: char-to-xz ( who tri face F: x z -- )  drop drop drop fdrop fdrop stub-step ;
\ 7A: Hewie goes to the point on triangle `tri` (operands stored x, z, y) (character move 0x13).
: hewie-go-to ( tri b F: x z y -- )  drop drop fdrop fdrop fdrop stub-step ;
\ 7B: Waits until character `who`'s motion event flags have any of `bits`.
: char-wait-motion ( who bits -- )  drop drop stub-step ;
\ 7C: Zone `z` (0..31, for conditions 0x07 / 0x08 ...) on: centre (x, y, z), radius r, height h,
\ kind `kind`.
: zone ( z r h kind F: x y z -- )  drop drop drop drop fdrop fdrop fdrop stub-step ;
\ 7D: Zone `z` on around room effect `fx`'s position: radius r, height h, kind.
: zone-at-effect ( z fx r h kind -- )  drop drop drop drop drop stub-step ;
\ 7E: Background music track `track` wanted on / off at volume `vol` (BgmCtl_Want); on 0xFF:
\ resume the ADX stream instead.
: bgm ( track on F: vol -- )  drop drop fdrop stub-step ;
\ 7F: Room effect slot `fx` made anew as a flickering animated sprite (EvEffect7F) at (x, y, z).
: flicker-sprite ( fx F: x y z -- )  drop fdrop fdrop fdrop stub-step ;
\ 80: Room effect slot `fx` removed.
: effect-remove ( fx -- )  drop stub-step ;
\ 81: self: animation `anim` with b (character move 8).
: self-anim-blend ( anim b -- )  drop drop stub-step ;
\ 82: The event camera (camera director): with on, set from the four values (EventCam_Set); then
\ held on / off (CamDirector_HoldEffect1C).
: event-camera ( on F: a b c d -- )  drop fdrop fdrop fdrop fdrop stub-step ;
\ 83: An item counted (progress +0xFBE) and given: id = the script's room id for `room`
\ (Events_ScriptRoom), n of it (Items_Give).
: item-give-count ( room n -- )  drop drop stub-step ;
\ 84: The summoner takes the partner (Summoner_Take(a)).
: summon-take ( a -- )  drop stub-step ;
\ 85: Hewie's trust in Fiona + n (Hewie_AddTrust).
: hewie-trust ( n -- )  drop stub-step ;
\ 86: Room effect slot `fx` made anew as an EvEffect86 at (x, y, z) with `kind`.
: effect-86 ( fx kind F: x y z -- )  drop drop fdrop fdrop fdrop stub-step ;
\ 87: Sends room effect `fx` 1 if character `who` hasn't moved this frame, else 2.
: char-effect-moving ( who fx -- )  drop drop stub-step ;
\ 88: A noise of loudness `loud` in this room at triangle `tri` (stalkers hear it).
: noise ( loud tri -- )  drop drop stub-step ;
\ 89: Prepares message `msg` for the window (shown later, see 0x62 12).
: message-prepare ( msg -- )  drop stub-step ;
\ 8B: The game-over flag (progress +0x73EB00, also set when Fiona is caught for good) = v.
: game-over-flag ( v -- )  drop stub-step ;
\ 8C: A scene effect (Effect6FF60, 0xE40 bytes) at (x, y, z) with zone rectangle `zone` (0x8D),
\ n, b, t.
: scene-effect-8C ( zone n b F: x y z t -- )  drop drop drop fdrop fdrop fdrop fdrop stub-step ;
\ 8D: Zone rectangle `z` (+0x894): an id and x0, z0, x1, z1 (used by 0x8C and some conditions).
: zone-rect ( z id F: x0 z0 x1 z1 -- )  drop drop fdrop fdrop fdrop fdrop stub-step ;
\ 8E: self: turns to face (x, z) (character move 15).
: self-turn-to-xz ( F: x z -- )  fdrop fdrop stub-step ;
\ 8F: Character `who`'s shadow volumes off (its model's +0x4D9; Model_DrawWithShadow).
: char-no-shadow ( who on -- )  drop drop stub-step ;
\ 90: Room light `light`: op 0 back to the room's own; 1 / 2 its value 7 / 11 (intensity?)
\ scaled by k.
: light ( op light F: k -- )  drop drop fdrop stub-step ;
\ 91: The noise level setting (progress +0x1114) = v.
: noise-level ( v -- )  drop stub-step ;
\ 92: As 0x05 but always starts the action; mode 1: at once for characters; who 0 also calls the
\ progress' +0x44 (empty).
: action-force ( mode who act -- )  drop drop drop stub-step ;
\ 93: A panic value grows: bit 0x80 set: progress +0x7E0 + 128; else +0x7E4 + v / 30.
: panic-grow ( v -- )  drop stub-step ;
\ 94: Fiona calms down by n (Fiona_CalmDown).
: fiona-calm ( n -- )  drop stub-step ;
\ 95: Fiona's recovery lowered by n (Fiona_LowerRecovery).
: fiona-recovery-lower ( n -- )  drop stub-step ;
\ 96: Fiona looks at / targets character `id` for `frames` (30 when not above 0)
\ (Fiona_SetTarget).
: fiona-target ( id frames -- )  drop drop stub-step ;
\ 97: Fiona's model swapped for costume `costume` (taken out, reloaded, not active).
: fiona-costume ( costume -- )  drop stub-step ;
\ 98: The character in `slot` comes in (waits while it is loading); it and Fiona active.
: char-in ( slot -- )  drop stub-step ;
\ 99: State flag 0x16 set; the progress' +0x78 (Progress_Noop78: nothing).
: state-flag-16 ( a b -- )  drop drop stub-step ;
\ 9A: self: as 0x6F, the exit given by door id `door`.
: self-through-door ( door -- )  drop stub-step ;
\ 9B: Room effect slot 0x1E: on 0 removed, else made anew as a screen blend (ScreenBlend_Init)
\ with a (little-endian) and b.
: screen-blend ( a b on -- )  drop drop drop stub-step ;
\ 9C: Room effect slot 0x1D: on 0 removed, else made anew as fog (Fog_Init): a, b raw floats
\ (little-endian), near, far.
: fog ( a b on F: near far -- )  drop drop drop fdrop fdrop stub-step ;
\ 9D: Character `id` plays animation `anim` (blend, speed) and is held in a scripted state.
: char-anim-hold ( id anim blend speed -- )  drop drop drop drop stub-step ;
\ 9E: Waits until character `id`'s animation comes round (its end flag 0x20).
: wait-char-anim ( id -- )  drop stub-step ;
\ 9F: A scene effect: a swarm of specks (SpeckSwarm) at (x, y, z).
: specks ( a b c0 c1 c2 c3 F: x y z -- )  drop drop drop drop drop drop fdrop fdrop fdrop stub-step ;
\ A0: A scene effect: a splash (Splash) at (x, y, z).
: splash ( a b c d0 d1 d2 d3 F: x y z -- )  drop drop drop drop drop drop drop fdrop fdrop fdrop stub-step ;
\ A1: The rooms' exits are rebuilt (room manager +0x90).
: exits-rebuild ( -- )  stub-step ;
\ A2: Door `door`'s second flag (+0x82; Doors_SetFlag82). Nothing in the C reads it yet.
: door-flag-82 ( door on -- )  drop drop stub-step ;
\ A3: The panic's stage = `stage` (Panic_SetStage).
: panic-stage ( stage -- )  drop stub-step ;
\ A4: self: animation `anim` with b (character move 9).
: self-anim-9 ( anim b -- )  drop drop stub-step ;
\ A5: Requests scene 5 (an ending / the results, id `id`) unless state flag 0x12 or Fiona is
\ busy.
: scene-ending ( id -- )  drop stub-step ;
\ A6: Loads the room's file named by string `file` into character `id`'s model buffer (slots
\ 0..3).
: char-file-load ( id file -- )  drop drop stub-step ;
\ A7: Character `id`'s model takes its loaded buffer (0xA6); waits while the loader is busy.
: char-file-use ( id -- )  drop stub-step ;
\ A8: Loads the current room's sound set.
: room-sounds ( -- )  stub-step ;
\ A9: A scene effect (Effect71000, 0x60 bytes) with a and eight values; its slot kept in script
\ variable `var`.
: scene-effect-71000 ( var a F: v0 v1 v2 v3 v4 v5 v6 v7 -- )  drop drop fdrop fdrop fdrop fdrop fdrop fdrop fdrop fdrop stub-step ;
\ AA: self: walks to (x, z) with animation `anim`, then faces `face` (character move 17).
: self-walk-anim ( anim tri tri2 face F: x z -- )  drop drop drop drop fdrop fdrop stub-step ;
\ AB: self: looks at (x, y, z) (character move 13). (Not "go to".)
: self-look-at-point ( F: x y z -- )  fdrop fdrop fdrop stub-step ;
\ AC: self: the character fades at doorways (+0xE4: Character_RegionFade picks its draw layer
\ each frame); off keeps a fixed layer (see 0xAE / 0xB5).
: self-doorway-fade ( on -- )  drop stub-step ;
\ AD: self: turns to the point of zone `zone` (its number from script variable `zone`:
\ Events_GetVar) (move 15).
: self-face-zone ( zone -- )  drop stub-step ;
\ AE: Character `who`: on 0: +0xE4 = 1; else drawn tinted: the renderer's tint (+0x70) = rgba,
\ the character in the fading layer 0x0F.
: char-tint ( who rgba on -- )  drop drop drop stub-step ;
\ AF: Hewie goes to (x, y, z) (character move 0x14).
: hewie-go-to-point ( F: x y z -- )  fdrop fdrop fdrop stub-step ;
\ B0: Hewie goes to zone `zone`'s point (variable `zone`): run 0 move 0x14, 1 move 13.
: hewie-go-to-zone ( zone run -- )  drop drop stub-step ;
\ B1: As 0x7F with the slot and position in script variables (positions / 1000).
: flicker-sprite-var ( fx x y z -- )  drop drop drop drop stub-step ;
\ B2: Fiona is thrown down by character `id` (reaction action 4, sub 0xA, with a rumble;
\ Fiona_StartAction4); recover 1 also resets her recovery.
: fiona-thrown ( id recover -- )  drop drop stub-step ;
\ B3: Places character `who` at (x, y, z) facing `face` (triangle looked up).
: char-to-xyz ( who face F: x y z -- )  drop drop fdrop fdrop fdrop stub-step ;
\ B4: The avoid / struggle prompt (+0xC with v).
: avoid-prompt ( v -- )  drop stub-step ;
\ B5: Character `who` drawn in renderer layer `layer` (Character_Set152C).
: char-layer ( who layer -- )  drop drop stub-step ;
\ B6: A gift from the stalker's table: the next item Fiona has fewer than 99 of (message
\ 0x8011), or message 0x801A when none are left; after the first only 1 in 10 times.
: stalker-gift ( -- )  stub-step ;
\ B7: Hewie's model swapped for kind 0..2 (taken out, reloaded, not active).
: hewie-model ( kind -- )  drop stub-step ;
\ B8: Character `id` (in the scene) heals by |n|, up to its maximum (+0x14CC).
: char-heal ( id n -- )  drop drop stub-step ;
\ B9: Brings in character `id` in slot `slot` (CharLoad_EventChar, the second loader).
: char-load-2 ( id slot -- )  drop drop stub-step ;
\ BA: The character in slot `from` gives its motion banks to slot `to` (waits while `to` is
\ loading).
: char-hand-over ( from to -- )  drop drop stub-step ;
\ BB: Hewie (if in this room): his wait timer = 5 frames and his "forced action 4" flag
\ (+0xF3588) on / off.
: hewie-wait-5 ( on -- )  drop stub-step ;
\ BC: Fiona reacts to Hewie (Fiona_HewieReact 8).
: fiona-hewie-react ( -- )  stub-step ;
\ BD: Hewie's +0xF3688 = 300: for a while he obeys stay / wait commands at once
\ (Hewie_CommandAction). Unused by the scripts.
: hewie-stay-300 ( -- )  stub-step ;
\ BE: The in-game item tab (SubScreen_TabCommand): op 0 announces the script's item `item`
\ (Events_ScriptRoom), 1 waits for its files, 2 slides the tab in, 4 out; waits while it moves.
: item-tab ( op item -- )  drop drop stub-step ;
\ BF: Fiona's fear (+0x1AD5F4) = v (0..100; over 90 she panics).
: fiona-fear ( F: v -- )  fdrop stub-step ;
\ C0: The threat meter's accumulator (progress +0x7DC: the threat object +0x24) grows by v
\ (0..100), as a small Threat_Raise.
: threat-add ( v -- )  drop stub-step ;
\ C1: Loads sound set `set` (Progress_LoadSoundSet).
: sound-set ( set -- )  drop stub-step ;
\ C2: Door `door`'s lock for character `id` = state (Progress_LockDoorFor).
: door-lock-for ( id door state -- )  drop drop drop stub-step ;
\ C3: Hewie plays animation `anim` (blend) with its root motion (character move 0x15 -> his
\ action 0x47).
: hewie-anim-root ( anim blend -- )  drop drop stub-step ;
\ C4: Hewie's mode (Hewie_SetMode).
: hewie-mode ( mode -- )  drop stub-step ;
\ C5: Hewie looks at zone `zone`'s point (variable `zone`), raised by dy, if he isn't already
\ looking at something.
: hewie-look-zone ( zone F: dy -- )  drop fdrop stub-step ;
\ C6: Hewie looks at character `id` (in this room, active), raised by dy, if he isn't already
\ looking at something.
: hewie-look-char ( id F: dy -- )  drop fdrop stub-step ;
\ C7: self: character move 16 with v (an animation).
: self-move-16 ( v -- )  drop stub-step ;
\ C8: A dust burst (SpriteBurst) of `kind` at (x, y, z): colour r, g, b if `own`, else grey
\ (0x80 for kind 0, else 0x50); size 16.
: dust ( kind r g b own F: x y z -- )  drop drop drop drop drop fdrop fdrop fdrop stub-step ;
\ C9: The panic level set to `level` if it has reached it (Panic_SetLevel).
: panic-level ( level -- )  drop stub-step ;
\ CA: Every sound's volume scaled by v (progress +0x1118).
: sound-volume-scale ( F: v -- )  fdrop stub-step ;
\ CB: The room's creatures (0 all, 1 slots 0..6, 2 slots 7..9) told (+0x10) and removed.
: creatures-clear ( which -- )  drop stub-step ;
\ CC: op 3: the cutscene director's +0x78 with id; else stalker `id`'s model: 0 +0x2C, 1 +0x30,
\ 2 +0x34 with v, 4 Daniella's capsules back (v).
: char-model-op ( op id v -- )  drop drop drop stub-step ;
\ CD: The renderer draws sprites additively in mode `mode` (Renderer_Additive).
: sprites-additive ( mode -- )  drop stub-step ;
\ CE: The placed things are dealt out (Events_DealThings).
: deal-things ( -- )  stub-step ;
\ CF: The playing movie's volume = v (0..1), applied; +0x1BC set (starts it: see the FMV notes).
: movie-volume ( F: v -- )  fdrop stub-step ;
\ D0: Hewie's side (0..2, else none).
: hewie-side ( side -- )  drop stub-step ;
\ D1: The sub-screen's map turns to page `page` (+0x2C: Map_TurnTo).
: map-page ( page -- )  drop stub-step ;
\ D2: Every placed thing is removed (PlacedThings_Clear).
: things-clear ( -- )  stub-step ;
\ D3: Camera shake: each frame the eye is moved randomly by up to v on each axis (camera +0x4,
\ Camera_Set4); 0 stops it.
: camera-shake ( F: v -- )  fdrop stub-step ;
\ D4: The frames-in-this-room count (+0x704: phase 1 counts it, entering resets it) = n (see
\ condition 0x5B).
: room-frames-set ( n -- )  drop stub-step ;
\ D5: Progress variable n = v.
: pvar-set ( n v -- )  drop drop stub-step ;
\ D6: If state flag 8: the renderer flips its second packet arena next frame and clears it
\ (Renderer_Set304DE0).
: effects-arena-flip ( -- )  stub-step ;
\ D7: Hewie's motion plays `anim` (Motion_Play).
: hewie-anim-set ( anim -- )  drop stub-step ;
\ D8: By progress +0xFB6 (from 20: steps of 20): item 0x270..0x273 added, with the pickup sound.
: reward-item ( -- )  stub-step ;
\ D9: A scene effect: a dust mote source (DustMoteSource) at (x, y, z) of `size`.
: dust-motes ( F: x y z size -- )  fdrop fdrop fdrop fdrop stub-step ;
\ DA: Nav-triangle flag groups (gRoomEventObj): set 1: +0x18, else +0x1C with group and bits (as
\ 0x1D, 16-bit group).
: nav-group-2 ( set group bits -- )  drop drop drop stub-step ;
\ 59 00: Story flag n set (the progress' scenario flags, +0x1C).
: story-flag-set ( n -- )  drop stub-step ;
\ 59 01: Story flag n cleared.
: story-flag-clear ( n -- )  drop stub-step ;
\ 59 02: State flag n set (the progress' 46 flags, +0x8: control, panic, ...).
: state-flag-set ( n -- )  drop stub-step ;
\ 59 03: State flag n cleared.
: state-flag-clear ( n -- )  drop stub-step ;
\ 59 04: Door `door` unlocked.
: door-unlock ( door -- )  drop stub-step ;
\ 59 05: Door `door` locked.
: door-lock ( door -- )  drop stub-step ;
\ 59 06: Door `door` passable as if unlocked (its state bit 4: DoorHold_Usable), and characters
\ can't hold it open (DoorHold_Open).
: door-passable ( door -- )  drop stub-step ;
\ 59 07: Door `door` no longer closed off (Rooms_Reopen), then locked (as 0x05).
: door-reopen-lock ( door -- )  drop stub-step ;
\ 59 08: Door `door` closed off (Rooms_CloseOff), then unlocked (as 0x04).
: door-close-off-unlock ( door -- )  drop stub-step ;
\ 59 09: Door `door`: its open bit set.
: door-open-set ( door -- )  drop stub-step ;
\ 59 0A: Door `door`: its open bit cleared.
: door-open-clear ( door -- )  drop stub-step ;
\ 59 0B: The message's parameter 0 = room id n as the script sees it (Events_ScriptRoom).
: message-param-room ( n -- )  drop stub-step ;
\ 59 0C: Item `item` is used up (Items_UseId).
: item-use ( item -- )  drop stub-step ;
\ 59 0D: Item `item` added to the inventory, with the pickup sound; bit 0x8000 marks the
\ hard-mode variant (only given in that mode, else only the plain one).
: item-give ( item -- )  drop stub-step ;
\ 59 0E: Item `item` added to the inventory, silently (SubScreen_AddFile).
: item-add ( item -- )  drop stub-step ;
\ 59 0F: Story flag (number in script variable `var`) set.
: story-flag-set-var ( var -- )  drop stub-step ;
\ 59 10: Story flag (number in script variable `var`) cleared.
: story-flag-clear-var ( var -- )  drop stub-step ;
\ 59 11: The sub-screen's bit n set (map / file entries) (SubScreen_SetBit).
: subscreen-bit ( n -- )  drop stub-step ;
\ 59 12: Resident flag n set (kept across games: unlocks; the game's +0x24).
: resident-flag-set ( n -- )  drop stub-step ;
\ 50 00: Placed object `obj` shown (on) or hidden.
: object-show ( obj on -- )  drop drop stub-step ;
\ 50 01: Placed object `obj` plays animation `anim` once.
: object-anim ( obj anim -- )  drop drop stub-step ;
\ 50 02: Placed object `obj` plays animation `anim` looped.
: object-anim-loop ( obj anim -- )  drop drop stub-step ;
\ 50 03: Placed object `obj`'s animation reset to its start.
: object-anim-reset ( obj -- )  drop stub-step ;
\ 50 04: Placed object `obj` hidden and put back as defined (PlacedObject_ToDef).
: object-hide-reset ( obj -- )  drop stub-step ;

\ ---- conditions

\ 00: Story flag n is set (the progress' scenario flags, +0x1C).
: story-flag? ( n -- flag )  drop stub-flag ;
\ 01: Character `who` (active) is in this room, inside event area `area`.
: char-in-area? ( who area -- flag )  drop drop stub-flag ;
\ 02: Character `who` (active, in this room) has just entered area `area`.
: char-entered-area? ( who area -- flag )  drop drop stub-flag ;
\ 03: Character `who` (active, in this room) has just left area `area`.
: char-left-area? ( who area -- flag )  drop drop stub-flag ;
\ 04: The exit just taken (+0x702, see command 0x00) is `exit`.
: exit-taken? ( exit -- flag )  drop stub-flag ;
\ 05: Character `who` faces heading dir x 2 degrees, within `within` degrees.
: char-heading? ( who dir within -- flag )  drop drop drop stub-flag ;
\ 06: Character `who` is inside area `area` and faces its middle, within `within` degrees.
: char-faces-area? ( who area within -- flag )  drop drop drop stub-flag ;
\ 07: Whoever is controlled may take exit `exit` now: free (Fiona idle or walking, Hewie idle),
\ inside the exit's area, its door open and the exit not marked.
: exit-usable? ( exit -- flag )  drop stub-flag ;
\ 08: State flag n is set (the progress' 46 flags).
: state-flag? ( n -- flag )  drop stub-flag ;
\ 09: The pending scene request (progress +0x1134) is v.
: scene-request? ( v -- flag )  drop stub-flag ;
\ 0A: The door at exit `exit` of this room is open.
: exit-door-open? ( exit -- flag )  drop stub-flag ;
\ 0B: Door `door` is unlocked.
: door-unlocked? ( door -- flag )  drop stub-flag ;
\ 0C: Door `door` isn't closed off (Rooms_DoorClosedOff).
: door-not-closed-off? ( door -- flag )  drop stub-flag ;
\ 0D: The game mode is `mode` (Progress_GameMode).
: game-mode? ( mode -- flag )  drop stub-flag ;
\ 0E: Script slots 0xF0..0xFA: that slot's script runs; else character `who` is in a scripted
\ state (+0xE0).
: char-busy? ( who -- flag )  drop stub-flag ;
\ 0F: Hewie (in the scene) is near enough for Fiona's commands (Hewie_FionaNearCommand).
: hewie-near-command? ( -- flag )  stub-flag ;
\ 10: The stalker alert state (Progress_StalkerAlert) is v.
: stalker-alert? ( v -- flag )  drop stub-flag ;
\ 12: The event counter (+0x703, commands 0x16..0x18) is n.
: counter? ( n -- flag )  drop stub-flag ;
\ 13: This script's frame count (+0x14) is n.
: frames? ( n -- flag )  drop stub-flag ;
\ 14: Script variable `var` is v.
: var? ( var v -- flag )  drop drop stub-flag ;
\ 15: Pad test: button 0 circle, 1 square, 2 L1, 3 triangle, 4 R1, 5 cross, 6 start, held (how
\ bit 0) or just pressed (bit 1). Button with bit 7: Fiona's shake flag set and Fiona_Shakes
\ instead.
: pad? ( button how -- flag )  drop drop stub-flag ;
\ 16: Character `id` is active and in this room.
: char-here? ( id -- flag )  drop stub-flag ;
\ 17: This script's character is `id` (0xFE: the active stalker).
: self-is? ( id -- flag )  drop stub-flag ;
\ 18: Character `id`'s pursuer group fields (PursuerGroup_Fields(group)) have bit 4.
: char-group-bit4? ( id group -- flag )  drop drop stub-flag ;
\ 19: Character `who` (active) is in this room on triangle `tri`.
: char-on-tri? ( who tri -- flag )  drop drop stub-flag ;
\ 1A: The event result (+0x934: set by command 0x62 0 / 2, the movie's state) is v.
: result? ( v -- flag )  drop stub-flag ;
\ 1B: Progress variable n is v.
: pvar? ( n v -- flag )  drop drop stub-flag ;
\ 1C: A `pct` percent chance.
: chance? ( pct -- flag )  drop stub-flag ;
\ 1D: The panic's stage (progress +0x7B8) is `stage` (0xFF: 4 or 5).
: panic-stage? ( stage -- flag )  drop stub-flag ;
\ 1E: Character `who` (active) has no health left.
: char-dead? ( who -- flag )  drop stub-flag ;
\ 1F: Character `a` touches `b` (margins m0, m1) and faces it, within `within` degrees.
: char-touching-facing? ( a b m0 m1 within -- flag )  drop drop drop drop drop stub-flag ;
\ 20: The cutscene director's mode (+0x2C) is `mode`.
: cutscene-mode? ( mode -- flag )  drop stub-flag ;
\ 21: The controlled character's current action (Fiona +0x1AD6B8, Hewie +0xF3798) is v.
: control-action? ( v -- flag )  drop stub-flag ;
\ 22: The message window is closed and its chosen answer (+0x750) is v.
: answer? ( v -- flag )  drop stub-flag ;
\ 23: Characters `a` and `b` (active) are within distance |d|; Fiona or Hewie to a stalker only
\ while the stalker is present (+0x1544).
: chars-within? ( a b d -- flag )  drop drop drop stub-flag ;
\ 24: Hewie (in the scene)'s current action (+0xF3564) is v.
: hewie-action? ( v -- flag )  drop stub-flag ;
\ 25: The camera director's +0x24 is v.
: camera-mode? ( v -- flag )  drop stub-flag ;
\ 26: The action Fiona last started (+0x1AD6BC, Fiona_MarkActionStart; -1 none) is v.
: fiona-started? ( v -- flag )  drop stub-flag ;
\ 27: Character `who` (in this room) faces (x, z), within `within` degrees.
: char-faces-xz? ( who x z within -- flag )  drop drop drop drop stub-flag ;
\ 28: Pushable obstacle `i` stands on triangle `tri` (Obstacles_IsSquare).
: obstacle-on? ( i tri -- flag )  drop drop stub-flag ;
\ 29: Event bit n is set (commands 0x57 / 0x58).
: ebit? ( n -- flag )  drop stub-flag ;
\ 2A: A fade (command 0x5C) is running.
: fading? ( -- flag )  stub-flag ;
\ 2B: The fade's frame count (+0x11F0) has reached +0x40. Unused by the scripts.
: fade-past-40? ( -- flag )  stub-flag ;
\ 2C: The camera director's setup changed (+0x2C, CamDirector_SetupChanged).
: camera-setup-changed? ( -- flag )  stub-flag ;
\ 2D: Sound bank `bank` is loaded (SndDriver_BankLoaded).
: sound-bank-loaded? ( bank -- flag )  drop stub-flag ;
\ 2E: Character `who` (in this room) stands on a triangle of nav group `group` (NavGroups
\ +0x14).
: char-in-nav-group? ( who group -- flag )  drop drop stub-flag ;
\ 2F: Fewer than 10 of item 0x3F are held (Items_CountItem3F).
: item-3F-under-10? ( -- flag )  stub-flag ;
\ 30: Character `who` (active, in the current room) is heading for exit `exit` (+0x14D4).
: char-heading-for? ( who exit -- flag )  drop drop stub-flag ;
\ 31: Hewie (in the scene) may not break off (Hewie_MayBreakOff is 0).
: hewie-stays? ( -- flag )  stub-flag ;
\ 32: Character `who` (active) is in room `room`.
: char-in-room? ( who room -- flag )  drop drop stub-flag ;
\ 33: Hewie (in the scene) is on side `side` of the room (+0xF3668; command 0xD0 sets it).
: hewie-side? ( side -- flag )  drop stub-flag ;
\ 34: Character `who` (active)'s animation event flags have any of `bits`.
: char-motion-flags? ( who bits -- flag )  drop drop stub-flag ;
\ 35: Character `who` (in this room, its radius / height) against zone `zone`: all of `bits`
\ (Zone_TestCylinder).
: char-zone-bits? ( who zone bits -- flag )  drop drop drop stub-flag ;
\ 36: As 0x35 with where the character was last frame.
: char-zone-bits-before? ( who zone bits -- flag )  drop drop drop stub-flag ;
\ 37: Character `who` is in zone `zone` (Zone_HasAnyChar).
: char-in-zone? ( who zone -- flag )  drop drop stub-flag ;
\ 38: The cutscene director's cue (+0x34) has reached `cue`.
: cutscene-cue-reached? ( cue -- flag )  drop stub-flag ;
\ 39: At least n of the script's item `item` (Events_ScriptRoom) are held.
: item-count? ( item n -- flag )  drop drop stub-flag ;
\ 3A: The cutscene director reports event `k` this step (+0x54 above 0).
: cutscene-event? ( k -- flag )  drop stub-flag ;
\ 3B: The cutscene director's counter for `k` (+0x58) passed `at` within this step.
: cutscene-passed? ( k at -- flag )  drop drop stub-flag ;
\ 3C: The stalker (active, in this room) is of kind `kind`.
: stalker-kind-here? ( kind -- flag )  drop stub-flag ;
\ 3D: Fiona can be controlled (Fiona_IsIdle) and the panic's stage is below 4.
: fiona-free? ( -- flag )  stub-flag ;
\ 3E: Character `who` (active, in this room) is not at door `door` (Doors_Side).
: char-not-at-door? ( who door -- flag )  drop drop stub-flag ;
\ 3F: Fiona's action (+0x1AD580) is v.
: fiona-action? ( v -- flag )  drop stub-flag ;
\ 40: The cutscene has just come within 17 frames of its end (Cutscene_NearEnd).
: cutscene-near-end? ( -- flag )  stub-flag ;
\ 41: Character `who`'s state +0xC4 is v (see command 0x47).
: char-C4? ( who v -- flag )  drop drop stub-flag ;
\ 42: Hewie's pool (+0xF359C) is in use (+0xF3598). Unused by the scripts.
: hewie-pool-in-use? ( -- flag )  stub-flag ;
\ 43: Hewie's mode (+0xF35C0: 0 normal, 1..3 timed; command 0xC4 sets it) is `mode`. Unused by
\ the scripts.
: hewie-mode? ( mode -- flag )  drop stub-flag ;
\ 44: Character `who` (active, in the current room) stands on a triangle with any of `flags`.
: char-on-nav-flags? ( who flags -- flag )  drop drop stub-flag ;
\ 45: Character `id` is out of sight: absent, inactive, elsewhere, or off the camera.
: char-unseen? ( id -- flag )  drop stub-flag ;
\ 46: Story flag (number in script variable `var`) is set.
: story-flag-var? ( var -- flag )  drop stub-flag ;
\ 47: This script's character and character `id` (active, in this room, not +0x2A) touch (its
\ margins).
: self-touching? ( id -- flag )  drop stub-flag ;
\ 48: This script's character is idle (+0xF4 0) at a motion event: marks it done (+0xE1).
: self-at-motion-event? ( -- flag )  stub-flag ;
\ 49: Character `who`'s action (+0xF8) is v.
: char-action? ( who v -- flag )  drop drop stub-flag ;
\ 4A: The cutscene's current frame is in shot `shot` (Cutscene_ShotAt).
: cutscene-shot? ( shot -- flag )  drop stub-flag ;
\ 4B: The stalker (active) is in stance 2 playing animation 0x1805 / 0x1806 (virtual +0x10C,
\ Pursuer_InStance2Anim). Unused by the scripts.
: stalker-stance-2? ( -- flag )  stub-flag ;
\ 4C: Fiona can give Hewie a command (Hewie_FionaCanCommand).
: hewie-can-command? ( -- flag )  stub-flag ;
\ 4D: This script's character's move is done (+0xE1).
: self-done? ( -- flag )  stub-flag ;
\ 4E: The message window is closed.
: message-closed? ( -- flag )  stub-flag ;
\ 4F: The point (x, y, z) is in the camera's view (+0xD4).
: point-on-camera? ( x y z -- flag )  drop drop drop stub-flag ;
\ 50: Character `who` (active, in this room)'s triangle is free for it (Actor_TriFreeFor).
: char-tri-free? ( who -- flag )  drop stub-flag ;
\ 51: The panic level (+0x7BC) is 98 or more.
: panic-98? ( -- flag )  stub-flag ;
\ 52: Character `who` (active) is at full health.
: char-full-health? ( who -- flag )  drop stub-flag ;
\ 53: One of the 10 room creatures (active, in the current room) is in action v.
: creature-action? ( v -- flag )  drop stub-flag ;
\ 54: The stalker is of kind `kind`.
: stalker-kind? ( kind -- flag )  drop stub-flag ;
\ 55: The stalker is in the scene.
: stalker-active? ( -- flag )  stub-flag ;
\ 56: Character `id` is at a motion event (its end flag 0x20).
: char-at-motion-event? ( id -- flag )  drop stub-flag ;
\ 57: A movie is playing.
: movie-playing? ( -- flag )  stub-flag ;
\ 58: This script's character touches one of room creatures 7..9 (active, in this room).
: self-near-creature? ( -- flag )  stub-flag ;
\ 59: Fiona is caught (+0x1AD630). Unused by the scripts.
: fiona-caught? ( -- flag )  stub-flag ;
\ 5A: Hewie can reach Fiona (Hewie_FionaReachable; no Hewie: yes).
: hewie-reachable? ( -- flag )  stub-flag ;
\ 5B: The event's +0x704 (command 0xD4) has reached v.
: event-704-reached? ( v -- flag )  drop stub-flag ;
\ 5C: The ADX stream: what 0 can start, else is playing (none: yes).
: adx? ( what -- flag )  drop stub-flag ;
\ 5D: Hewie is the one being controlled.
: hewie-controlled? ( -- flag )  stub-flag ;
\ 5E: The sub-screen's bit n is set (SubScreen_TestBit; see 0x59 0x11).
: subscreen-bit? ( n -- flag )  drop stub-flag ;
\ 5F: Progress variables a and b are equal.
: pvars-equal? ( a b -- flag )  drop drop stub-flag ;
\ 60: Resident flag n is set (kept across games).
: resident-flag? ( n -- flag )  drop stub-flag ;
\ 61: Hewie's trust level (+0xF35CC, it picks his wait timers) is v. Unused by the scripts.
: hewie-trust-level? ( v -- flag )  drop stub-flag ;
\ 62: The file loader's current load is done (Loader_CurrentDone).
: loader-done? ( -- flag )  stub-flag ;
\ 63: Character `id` is in slot `slot` and finished loading.
: char-loaded? ( slot id -- flag )  drop drop stub-flag ;
\ 64: The first noise slot (progress +0x1050) is of kind 0xD. Unused by the scripts.
: noise-slot-D? ( -- flag )  stub-flag ;
\ 65: The stalker (active) is in the current room and free: not held, not in certain attack
\ moves, its triangle free.
: stalker-free? ( -- flag )  stub-flag ;
