\ events/core.fs - what the converted event scripts run on: the script tables, the flags, the
\ characters as the scripts see them, and the action scripts' slots - each a held task that the
\ runner (events/runner.fs) gives its turn every frame. The state itself is C's (game-state).
\ The original: src/game/event.c (Event_StartAction, Event_CharSlot, cmd_action, ...).
IN: events.core
USING: game-state ;

\ ---- the words not written yet --------------------------------------------------------------
\ A stub (events/words.fs) says so the first time it runs, then keeps quiet; `.missing` lists
\ the ones that have run.

list constant missing   list constant missing-len
: note-missing ( addr len -- )
    over missing contains? if  2drop exit  then
    over missing push  dup missing-len push
    ." event word not written yet: " type cr ;
: .missing ( -- )
    missing length 0 ?do  i missing nth  i missing-len nth  type space  loop
    cr missing length . ." of the words not written yet have run" cr ;
defer stub-step ( addr len -- )          ' note-missing is stub-step
defer stub-flag ( addr len -- flag )     :noname note-missing false ; is stub-flag

\ ---- the scripts, by room ------------------------------------------------------------------
\ slot 0 entering, 1..5 the phases, 6 a character entering; the action scripts by room and id
\ (0..$7F); the shared ones by id ($80..$FF). 0: none

room-count constant rooms
create room-scripts    rooms 7 * cells allot     room-scripts rooms 7 * cells 0 fill
create action-scripts  rooms $80 * cells allot   action-scripts rooms $80 * cells 0 fill
create builtin-scripts $80 cells allot           builtin-scripts $80 cells 0 fill
: room-script ( room slot -- addr )  swap 7 * + cells room-scripts + ;
: action-script ( room id -- addr )  swap $80 * + cells action-scripts + ;
: builtin-script ( id -- addr )  $80 - cells builtin-scripts + ;
: room-script! ( xt room slot -- )  room-script ! ;
: action-script! ( xt room id -- )  action-script ! ;
: builtin-script! ( xt id -- )  builtin-script ! ;

\ the script for an action id in the current room (Events_StartAction: $80.. the shared ones)
: script-for ( act -- xt | 0 )
    $FF and  dup $80 and if  builtin-script @ exit  then
    event-state ev.room sl@ swap action-script @ ;

\ ---- flags: arrays of 32-bit words --------------------------------------------------------

: bit ( n addr -- mask addr' )  over 5 rshift 4 * +  swap 31 and 1 swap lshift swap ;
: bit? ( n addr -- flag )  bit l@ and 0<> ;
: bit-on ( n addr -- )  bit dup l@ rot or swap l! ;
: bit-off ( n addr -- )  bit dup l@ rot invert and swap l! ;
: +l! ( n addr -- )  dup sl@ rot + swap l! ;

\ ---- characters ------------------------------------------------------------------------------
\ The original's character slots (gCharacters): 0 Fiona, 1 Hewie, 2.. the others; each has a
\ script id (Fiona 0, Hewie 1, ...).

: character ( slot -- addr )  /char * event-state ev.chars + ;
\ Progress_SlotOfId: the slot of the character with script id `id` ($FE: the one in slot 2,
\ the stalker's), or -1 ($FF: none)
: char-slot ( id -- slot | -1 )
    $FF and
    dup $FF = if  drop -1 exit  then
    dup $FE = if  drop 2 character char.present sl@ if  2  else  -1  then  exit  then
    characters 0 do
        i character dup char.present sl@ swap char.id sl@ 2 pick =  and
        if  drop i unloop exit  then
    loop  drop -1 ;
: char-here ( slot -- flag )   \ present and in the scripts' room
    dup 0< if  drop false exit  then
    character dup char.present sl@ 0<>  swap char.room sl@ event-state ev.room sl@ =  and ;

\ ---- the action scripts' slots --------------------------------------------------------------
\ 17 slots (+0x564): 0 the player's (script id $F0), 1..6 the characters' (script id + 1),
\ 7..16 the scene scripts' ($F1..$FA). Each runs a held task.

: script-slot ( k -- addr )  /slot * event-state ev.slots + ;
\ Event_CharSlot: a script's character id to its slot
: slot-for ( id -- k )
    $FF and
    dup $F0 < over $FA > or if  1+ $FF and exit  then
    dup $F0 = if  drop 0 exit  then
    $F1 - 7 + ;
: scene-id? ( id -- flag )  $FF and $F0 $FB within ;

: in-slot? ( -- flag )  event-state ev.slot sl@ 0< 0= ;
\ the running context's frame count (an action slot's +0x14, or the phase's own)
: self-frames ( -- addr )
    in-slot? if  event-state ev.slot sl@ script-slot slot.frames  else  event-state ev.self-frames  then ;
: self-char ( -- slot | -1 )  event-state ev.self-char sl@ ;

\ a frame's wait: only an action script waits (in the original the phase scripts ignore it)
: wait-frame ( -- )  in-slot? if  yield  then ;
\ wait until `xt` ( -- flag ) is true (at once in a phase script)
: wait-until ( xt -- )  in-slot? 0= if  drop exit  then  begin  dup execute 0= while  yield  repeat  drop ;

\ a slot freed: its task stopped (if that is the running task, it stops at once: so last)
: free-slot ( k -- )
    script-slot  dup slot.task sl@ >r
    0 over slot.task l!  -1 over slot.who l!  $FF over slot.id l!  0 swap slot.frames l!
    r> ?dup if  kill  then ;
: free-slots ( -- )  script-slots 0 do  i free-slot  loop ;

\ start `xt` in slot k for character slot `who` (-1: none) with id `id`
: fill-slot ( xt who id k -- )
    script-slot  dup slot.task sl@ >r  >r          ( xt who id )  ( R: old-task addr )
    r@ slot.id l!  r@ slot.who l!  0 r@ slot.frames l!
    spawn-held r> slot.task l!
    r> ?dup if  kill  then ;

\ Event_StartAction: start action `act` for script id `id` (its slot gets a fresh context, with
\ no character)
: start-action ( id act -- )
    script-for ?dup 0= if  drop exit  then        ( id xt )
    swap  -1 swap  dup slot-for  fill-slot ;

\ Events_GiveScript: character slot `cs` runs action `act` (in slot cs + 1, as itself)
: give-script ( cs act -- )
    script-for ?dup 0= if  drop exit  then        ( cs xt )
    swap  dup character char.id sl@  over 1+      ( xt cs id k )
    fill-slot ;

\ a character starts a scripted action (char_script_action: its script state 5)
: char-action ( cs act -- )
    over character char.scripted  -1 swap l!
    give-script ;

\ cmd_action: 0x05 (if free / not already scripted) and 0x92 (always). (`mode` 1 makes 0x92
\ start a character's action at once; the character's own update isn't modelled yet.)
: start-for ( mode who act force -- )
    event-state ev.leaving sl@ if  2drop 2drop exit  then
    >r rot drop                                     ( who act )  ( R: force )
    over scene-id? if
        over slot-for script-slot slot.task sl@ 0=  r> or
        if  start-action  else  2drop  then  exit
    then
    swap char-slot dup 0< if  2drop rdrop exit  then    ( act cs )
    swap  r> if  char-action exit  then                 ( cs act )
    over character char.scripted sl@ 0= if  char-action  else  2drop  then ;

\ cmd_action_end: a scene slot freed, or a character's action script ended and the character
\ released
: end-for ( who -- )
    dup scene-id? if  slot-for free-slot exit  then
    char-slot dup 0< if  drop exit  then
    dup 1+ free-slot
    character char.scripted 0 swap l! ;

\ the task goes on in script `xt` and never comes back (the bytecode's 0x25: its frame count
\ and loop / return points cleared)
: goto ( xt -- )
    in-slot? if  0 self-frames l!  restart  else  execute  then ;
\ the same by id, in the current room (the shared scripts don't know their room)
: goto-action ( id -- )  script-for ?dup if  goto  then ;
: call-action ( id -- )  script-for ?dup if  execute  then ;
\ the room's own commands, from the shared scripts (by number: not written yet)
: room-command ( bytes.. n cmd -- )  drop 0 ?do drop loop  s" room-command" stub-step ;
: room-condition? ( bytes.. n cond -- flag )  drop 0 ?do drop loop  s" room-condition?" stub-flag ;

\ ---- randomness (the original: gRandom) ---------------------------------------------------
variable seed  $2545F491 seed !
: random ( n -- 0..n-1 )  seed @ 1103515245 * 12345 + $7FFFFFFF and dup seed !  16 rshift swap mod ;

\ ---- what the game provides (play.fs sets these; tests leave them) ----------------------------
\ put character slot `cs` at the outside point of this room's exit `exit`
defer place-at-exit ( cs exit -- )     :noname 2drop ; is place-at-exit
\ the camera director (game/camdirector.c): the set and path to use, the character slot to
\ follow (-1: nobody), and its steps
defer director-setup ( set path -- )   :noname 2drop ; is director-setup
defer director-follow ( cs -- )        ' drop is director-follow
defer director-restart ( -- )          ' noop is director-restart
defer director-changed? ( -- flag )    ' false is director-changed?
defer director-new-room ( -- )         ' noop is director-new-room
defer director-room-start ( -- )       ' noop is director-room-start
defer director-ease ( -- )             ' noop is director-ease
defer director-track ( -- )            ' noop is director-track
defer director-update ( -- )           ' noop is director-update
\ the room's event areas (game/areas.c): inside one (only quads have an inside), and a step's
\ crossing (1 in, -1 out, 0); a room exit's area
defer event-area-in? ( area -- flag ) ( F: x y z -- )
:noname drop fdrop fdrop fdrop false ; is event-area-in?
defer event-area-cross ( area -- n ) ( F: px py pz x y z -- )
:noname drop fdrop fdrop fdrop fdrop fdrop fdrop 0 ; is event-area-cross
defer event-exit-area ( exit -- area ) :noname drop $FFFF ; is event-exit-area
defer event-area-middle ( area -- flag ) ( F: -- x y z )   :noname drop 0e 0e 0e false ; is event-area-middle
\ the room's exits and doors (the door table): exit `exit`'s door (-1 none), a door's fixed
\ flags (bit 0: a doorway, always open)
defer event-exit-door ( exit -- door )   :noname drop -1 ; is event-exit-door
defer event-door-flags ( door -- flags ) :noname drop 0 ; is event-door-flags
\ the nav mesh: a group of triangles' flags set / cleared, one triangle's, is a triangle in a group
defer event-nav-group ( set? group bits -- )      :noname 2drop drop ; is event-nav-group
defer event-nav-tri ( set? tri bits -- )          :noname 2drop drop ; is event-nav-tri
defer event-nav-in-group? ( group -- flag ) ( F: x y z -- )
:noname drop fdrop fdrop fdrop false ; is event-nav-in-group?
\ sounds: of bank `bank` (4 the sound set, 5 common, 6 the room's, 7 the pursuer's) heard plainly,
\ or at a point (louder / softer by vol / 128, pitch semitones up); stopped; which sound set bank
\ 4 holds; the room's sounds loaded (its old ones stopped); a bank loaded; the scripts' scale of
\ the sounds that take it; where the camera is
defer event-sound ( id bank -- )             ' 2drop is event-sound
defer event-sound-at ( id bank vol pitch -- ) ( F: x y z -- )
:noname 2drop 2drop fdrop fdrop fdrop ; is event-sound-at
defer event-sound-stop ( id bank -- )        ' 2drop is event-sound-stop
defer event-sound-set ( set -- )             ' drop is event-sound-set
defer event-room-sounds ( -- )               ' noop is event-room-sounds
defer event-sound-loaded? ( bank -- flag )   :noname drop true ; is event-sound-loaded?
defer event-sound-scale ( F: v -- )          ' fdrop is event-sound-scale
defer event-camera-eye ( F: -- x y z )       :noname 0e 0e 0e ; is event-camera-eye
defer event-reverb ( core v -- )             ' 2drop is event-reverb
\ where the room's door at an exit stands (false: none)
defer event-door-at ( exit -- flag ) ( F: -- x y z )
:noname drop 0e 0e 0e false ; is event-door-at
\ characters: an actor's place set, its heading (radians: 0 along +z, as atan2 dx dz) set and read,
\ shown or not; its animation played
\ (once or looping) and whether it has come to its end
defer event-char-place ( cs -- ) ( F: x y z -- )   :noname drop fdrop fdrop fdrop ; is event-char-place
defer event-char-yaw ( cs -- ) ( F: a -- )          :noname drop fdrop ; is event-char-yaw
defer event-char-heading ( cs -- ) ( F: -- a )      :noname drop 0e ; is event-char-heading
defer event-char-show ( cs on -- )                  ' 2drop is event-char-show
defer event-char-anim ( cs anim loop? -- )          :noname 2drop drop ; is event-char-anim
defer event-char-anim-done? ( cs -- flag )          :noname drop true ; is event-char-anim-done?
\ a character leaves the scene / comes back (the original: its Deactivate / Activate)
defer event-char-out ( cs -- )   ' drop is event-char-out
defer event-char-in ( cs -- )    ' drop is event-char-in
\ a nav triangle's middle (for placing on triangles)
defer event-tri-center ( tri -- ) ( F: -- x y z )   :noname drop 0e 0e 0e ; is event-tri-center
\ the room's look effects (slots $1C depth of field, $1D fog, $1E screen blend, $1F tint) from
\ parameter bytes, as the original's effects take them (addr 0: the effect removed)
defer event-look ( slot addr n -- )   :noname 2drop drop ; is event-look
\ movies (platform/movie.c): one opened paused, started, stopped, paused / resumed, its volume;
\ its state as the original's Movie_Status (2 paused, 1 running, -1 none) and the frame shown
\ (-1 none yet)
\ the background music (BgmCtl): a track wanted (0xFF none) at a level, paused or not; the
\ stream paused / resumed
defer event-bgm-want ( track pause -- ) ( F: level -- )   :noname 2drop fdrop ; is event-bgm-want
defer event-music-pause ( on -- )         ' drop is event-music-pause
\ the stage music: the director for stage set 0..3 (-1: ended)
defer event-music-stage ( stage -- )      ' drop is event-music-stage
\ its commands (0x6A): 0 the global volume to a over b frames, 1 the stage's channels, 2 load and
\ hold, 3 wait for the banks, 4 release (it starts), 5 silence
defer event-music ( op a b -- )           :noname 2drop drop ; is event-music
defer event-movie-open ( addr len -- )    ' 2drop is event-movie-open
defer event-movie-stop ( -- )             ' noop is event-movie-stop
defer event-movie-pause ( on -- )         ' drop is event-movie-pause
defer event-movie-volume ( F: v -- )      ' fdrop is event-movie-volume
defer event-movie-compose ( compo lo hi -- )  :noname 2drop drop ; is event-movie-compose
defer event-movie-state ( -- n )          :noname -1 ; is event-movie-state
defer event-movie-frame ( -- n )          :noname -1 ; is event-movie-frame
\ the cutscene director (game/cutscene.c, the original's gCutscene): start the scene in a folder,
\ its state a step, start when ready, the frame, a frame's update, the end; its status (1
\ loading, 2 ready, 3 playing, 5 over), the playing shot in, the frame, near the end, the shot
\ at a frame, how often a signal came since the last frame and its count so far (less one)
defer event-scene-start ( addr len -- )   ' 2drop is event-scene-start
defer event-scene-run ( -- )              ' noop is event-scene-run
defer event-scene-go ( -- )               ' noop is event-scene-go
defer event-scene-frame! ( n -- )         ' drop is event-scene-frame!
defer event-scene-update ( -- )           ' noop is event-scene-update
defer event-scene-end ( -- )              ' noop is event-scene-end
defer event-scene-status ( -- n )         :noname 5 ; is event-scene-status
defer event-scene-in-shot? ( -- flag )    ' false is event-scene-in-shot?
defer event-scene-frame ( -- n )          :noname -1 ; is event-scene-frame
defer event-scene-near-end? ( -- flag )   ' false is event-scene-near-end?
defer event-scene-shot-at ( frame -- shot )   :noname drop -1 ; is event-scene-shot-at
defer event-scene-signals ( bit -- n )    :noname drop 0 ; is event-scene-signals
defer event-scene-total ( bit -- n )      :noname drop -1 ; is event-scene-total
\ the room's object named (addr len): 0 shown (arg), 1 / 2 animation arg once / looped, 3 its
\ animation stopped, 4 hidden and back as defined
defer event-object ( addr len op arg -- )   :noname 2drop 2drop ; is event-object
\ the room's string `i` (scenes, movies, objects: events/strings.fs)
defer event-room-string ( i -- addr len ) :noname drop s" " ; is event-room-string
\ the message window's parameter `slot` shows system message $100 + id's first line
defer message-parameter ( slot id -- ) ' 2drop is message-parameter

\ ---- doors (src/game/progress.c: the door states) --------------------------------------------
: door-word ( door -- addr | 0 )  dup 0 doors within if  4 * progress pr.doors +  else  drop 0  then ;
: door-bit? ( door mask -- flag )  swap door-word dup if  l@ and 0<>  else  2drop false  then ;
: door-bit-on ( door mask -- )  swap door-word dup if  dup l@ rot or swap l!  else  2drop  then ;
: door-bit-off ( door mask -- )  swap door-word dup if  dup l@ rot invert and swap l!  else  2drop  then ;
: door-locked ( door -- flag )  8 door-bit? ;
: closed-off? ( door -- flag )
    dup 0 doors within 0= if  drop false exit  then  progress pr.closed-off bit? ;
\ Progress_ExitOpen: a doorway, or a door not locked with its open bit
: exit-open ( exit -- flag )
    event-exit-door dup 0< if  drop false exit  then
    dup event-door-flags 1 and if  drop true exit  then
    dup door-locked if  drop false exit  then  2 door-bit? ;
\ Progress_ExitPassable for side 0 (bits 4..7: the sides it is locked from)
: exit-passable ( exit -- flag )
    event-exit-door dup 0< if  drop false exit  then  $10 door-bit? 0= ;
\ an exit asked for (exit-check): the game takes it after the frame
variable exit-wanted  -1 exit-wanted !

\ ---- the inventory (progress: items counted, files listed) ----
: item# ( id -- addr | 0 )  dup 0 $100 within if  progress pr.items +  else  drop 0  then ;
: items-of ( id -- n )  item# dup if  c@  then ;
: items+ ( id n -- )   \ (up to 99)
    swap item# dup 0= if  2drop exit  then  dup c@ rot + 0 max 99 min swap c! ;
\ SubScreen_AddFile: true if added (false: already there, or the list is full)
: file-add ( id -- flag )
    $FFFF and  128 0 do
        progress pr.files i 2* + dup w@                     ( id addr v )
        dup 3 pick = if  2drop drop false unloop exit  then
        0= if  w! true unloop exit  then  drop
    loop  drop false ;
: file? ( id -- flag )
    $FFFF and  128 0 do  progress pr.files i 2* + w@ over = if  drop true unloop exit  then  loop  drop false ;
\ Events_ScriptRoom for items: in the second game mode (not yet: never) 0x40 / 0x41 are 0x70
: script-item ( id -- id' ) ;

\ ---- the game's state the characters' behaviour reads (progress / scene fields) ----
variable game-mode        \ Progress_GameMode: 0 calm, 1 being followed, 2 the chase
\ the progress's condition bits, the frame's (Progress_SetCondBit / Progress_CondBit; cleared by
\ danger.fs): 0 / 5 Fiona within 150 / 200 of him, 1 she was struck, 2 no stalker, 3 he chases,
\ 4 Hewie is alert to him, 6 hunted
variable cond-bits
: cond-bit? ( n -- flag )  1 swap lshift cond-bits @ and 0<> ;
: cond-bit! ( n -- )  1 swap lshift cond-bits @ or cond-bits ! ;
\ the noises made this frame (progress +0x778: a slot of 0x10 each - 0 Fiona, 1 Hewie, 2 the
\ stalker, 3 events, creatures and things; door openers their own): the loudness, room,
\ triangle (-1: at a door) and door ($FFFF none). Heard at the frame's end, then cleared
\ (Progress_PursuerRequest). The setting (+0x1114, event 0x91) makes the quiet ones quieter.
create noise-loud 4 cells allot  create noise-room 4 cells allot
create noise-tri 4 cells allot   create noise-door 4 cells allot
variable noise-setting
: noise-clear ( slot -- )   \ CharRequest_Clear
    cells  0 over noise-loud + !  -1 over noise-room + !  -1 over noise-tri + !  $FFFF swap noise-door + ! ;
: noises-clear ( -- )  4 0 do  i noise-clear  loop ;
noises-clear
\ Noise_Make: a noise in `slot` unless one louder is there already
: noise-make-in ( loud room tri door slot -- )
    >r  3 pick 0=  2 pick -1 = or  3 pick $FF and r@ cells noise-loud + @ < or if
        2drop 2drop r> drop exit  then
    r@ cells >r
    dup $FFFF and $FFFF <> if  r@ noise-door + !  drop -1 r@ noise-tri + !
    else  drop r@ noise-tri + !  $FFFF r@ noise-door + !  then
    r@ noise-room + !  $FF and r> noise-loud + !  r> drop ;
defer event-noise ( loud tri -- )   ' 2drop is event-noise   \ (event 0x88: slot 3, this room - noises.fs)
\ the panic's and Fiona's fear's commands (by opcode: 43 / 93 / A3 / C0 / C9 the panic, 4E / 4F /
\ 94 / 95 her fear and recovery; BF her fear set): fiona.panic
defer event-panic ( op v -- )   :noname 2drop ; is event-panic
defer event-fiona-fear ( F: v -- )  ' fdrop is event-fiona-fear
variable panic            \ the panic's stage (progress +0x7B8: 4 / 5 panicking)
variable fiona-near       \ how near Hewie Fiona is (+0x7B9: 1 within 20, 2 within 50, 3 further)
variable hewie-control    \ the player controls Hewie (+0x1FBEC1)
\ whether Fiona can give Hewie a command now (partner: Hewie_FionaCanCommand)
defer event-hewie-can-command? ( -- flag )   ' false is event-hewie-can-command?
\ Hewie's side of the scripts' commands and conditions (by opcode; hewie.fs fills them in)
defer event-hewie ( op a b -- )      :noname 2drop drop ; is event-hewie
defer event-hewie? ( op a -- flag )  :noname 2drop false ; is event-hewie?
defer event-hewie-look ( -- ) ( F: x y z -- )   :noname fdrop fdrop fdrop ; is event-hewie-look

\ ---- the player's request (progress +0x1134: what her action button does, set again each frame by
\ the room's scripts - 0x0A): 5 starts her action script `request-arg`; its kind (+0x1151) ----
variable request  variable request-arg  variable request-kind
variable request-b   \ (+0x113C: its word 2 - the exit for a door's)
: requests-off ( -- )  0 request !  0 request-arg !  0 request-kind !  0 request-b ! ;
\ who is where at the exits (Progress_WhoIsWhere), the player's request from where she stands
\ (Progress_CharRequests), her buttons taking it (Progress_PlayerButtons): fiona.doors
defer event-who-is-where   ' noop is event-who-is-where
defer event-exit-request   ' noop is event-exit-request
defer event-player-buttons   ' noop is event-player-buttons
\ the door a character is using (its animation keys the door's swing and floor): -1 none
variable door-in-use  -1 door-in-use !

\ ---- zones (the original's 32 at +0xBF0, 0x30 each): cylinders the scripts set (0x7C) for the
\ conditions; all off when phase 1 starts, every frame. Here 28 bytes each: on, kind, the
\ centre's x y z, radius, height (either way up) ----
32 constant zones
create zone-table  zones 28 * allot
: zone# ( z -- addr )  28 * zone-table + ;
: zones-off ( -- )  zone-table zones 28 * 0 fill ;
zones-off
\ Zone_TestCylinder: a point with radius `r` and height `h` against zone `z`: 1 they overlap in
\ height, 4 it is within it in height, 2 they overlap across, 8 its centre is inside across
fvariable zr  fvariable zh  fvariable zx  fvariable zy  fvariable zz  fvariable zdy
: zone-test ( z -- bits ) ( F: x y z r h -- )
    zh f! zr f! zz f! zy f! zx f!
    zone# dup l@ 0= if  drop 0 exit  then  >r
    zy f@ zh f@ 2e f/ f+  r@ 12 + sf@ r@ 24 + sf@ 2e f/ f+  f- fabs zdy f!
    0
    zdy f@  r@ 24 + sf@ fabs zh f@ fabs f+ 2e f/  f<= if  1 or  then
    zdy f@  r@ 24 + sf@ fabs zh f@ fabs f- 2e f/  f<= if  4 or  then
    r@ 16 + sf@ zz f@ f- fdup f*  r@ 8 + sf@ zx f@ f- fdup f* f+       ( bits ) ( F: d2 )
    fdup  r@ 20 + sf@ zr f@ f+ fdup f* f<= if  2 or  then
    r> 20 + sf@ fdup f* f<= if  8 or  then ;

\ ---- scenes: the cues (the scene frames 0x62 7 / 5 take; +0xBE4 / +0xBE8), the prepared message
\ (0x89) and the page of it shown (0x62 12 turns them: the scene's subtitles) ----
variable cue  -1 cue !  variable cue-prev  -1 cue-prev !   \ (+0xBE4 / +0xBE8)
variable prepared  -1 prepared !
\ the movie's class (Progress_PlayMovie's kind: 0 not drawn - a scene's timing and sound; 1..6 drawn)
variable movie-kind  variable prepared-page  -1 prepared-page !

\ ---- the screen fade (the event's +0x20): 0 clear .. 1000 black ------------------------------
variable fade-now  variable fade-from  variable fade-to  variable fade-frames  variable fade-t
: fade-start ( frames kind -- )
    $F and dup 1 = swap 0= or if  1000 0  else  0 1000  then   ( frames from to )
    fade-to !  fade-from !  0 max fade-frames !  0 fade-t !
    fade-frames @ 0= if  fade-to @ fade-now !  then ;
: fading ( -- flag )  fade-t @ fade-frames @ < ;
: fade-step ( -- )   \ each frame
    fading if
        1 fade-t +!
        fade-to @ fade-from @ - fade-t @ * fade-frames @ / fade-from @ + fade-now !
    then ;
: fade-done ( -- )  fade-frames @ fade-t !  fade-to @ fade-now ! ;

\ ---- characters' moves (the original's character update does them: +0xF4 the move, +0xE1 done)
\ an animation, or a place to go: play.fs steps them each frame
: move! ( cs move -- )  over character char.move l!  character char.move-done 0 swap l! ;
: move-done ( cs -- )  character char.move-done -1 swap l! ;
\ the move's details: +0x100 a character slot, +0x104 a triangle or an animation, +0x108 another
\ (0xFFFF: -1) or a blend; the point (+0x110) and heading (+0x10C) are char.target / char.face
create move-slot  characters cells allot  move-slot characters cells 0 fill
create move-a     characters cells allot  move-a characters cells 0 fill
create move-b     characters cells allot  move-b characters cells 0 fill
: move-ab! ( cs a b -- )  rot >r  r@ cells move-b + !  r> cells move-a + ! ;
\ the characters that carry out their moves themselves, as the original's do (Hewie's
\ Hewie_Requests); the others' are walked by events/play.fs
create own-moves  characters cells allot  own-moves characters cells 0 fill
: own-moves? ( cs -- flag )  dup 0< if  drop false exit  then  cells own-moves + @ ;
: anim-move ( cs anim move -- )
    rot >r  r@ character char.move-anim  2 pick swap l!
    r@ 2 pick -1 move-ab!  r@ swap move!
    r@ own-moves? if  r> 2drop exit  then
    r> swap 0 event-char-anim ;

\ ---- characters' places and areas ----------------------------------------------------------
: char-pos ( cs -- ) ( F: -- x y z )  character char.pos dup sf@ dup 4 + sf@ 8 + sf@ ;
: char-prev ( cs -- ) ( F: -- x y z )  character char.prev dup sf@ dup 4 + sf@ 8 + sf@ ;
\ Actor_PlaySound: character cs's sound at a point, unless it is silent
: actor-sound ( cs id bank vol pitch -- ) ( F: x y z -- )
    >r >r >r >r  character char.silent sl@ if  r> r> r> r> 2drop 2drop fdrop fdrop fdrop exit  then
    r> r> r> r> event-sound-at ;
\ Events_InArea for a character (where it stands)
: char-in-area ( cs area -- flag )  swap char-pos event-area-in? ;
\ EventCond_AreaCross: its last step into (1) or out of (-1) the area
: char-cross ( cs area -- n )  >r dup char-prev char-pos r> event-area-cross ;

\ ---- the camera: which character it follows (src/game/progress.c) ---------------------------
: in-room? ( cs -- flag )  character char.room sl@ event-state ev.room sl@ = ;
\ Progress_CameraFollow: can the camera follow slot `idx` (Fiona always; another one in this
\ room with a camera set); if not, Fiona it is, and the director follows her
: followable? ( idx -- flag )
    dup characters u< 0= if  drop false exit  then
    dup character char.present sl@ 0= if  drop false exit  then
    dup 0= if  drop true exit  then
    dup character char.cam-set sl@ -1 <>  swap in-room? and ;
: camera-follow-check ( idx -- idx' )
    dup $FF = if  -1 director-follow exit  then
    dup followable? if  exit  then
    drop 0 director-follow 0 ;
\ Progress_CameraOn: the camera follows slot `idx` ($FF: nobody), with its set and path
: camera-on ( idx -- )
    camera-follow-check dup event-state ev.camera-char l!
    dup $FF = if  drop -1 director-follow exit  then
    dup director-follow
    character dup char.cam-set sl@ swap char.cam-path sl@ director-setup ;
\ Progress_CameraSetup: slot `cs`'s camera set and path ($FF: straight to the director, which
\ then follows nobody's); a character in another room gets none
: char-cam! ( set path cs -- )  character >r  r@ char.cam-path l!  r> char.cam-set l! ;
: camera-setup ( cs set path -- )
    rot dup $FF = if  drop director-setup  $FF event-state ev.camera-char l!  exit  then
    dup characters u< 0= if  drop 2drop exit  then
    dup character char.present sl@ 0= if  drop 2drop exit  then
    dup 0=  over in-room? or if  char-cam!  else  nip nip -1 -1 rot char-cam!  then ;
\ each frame, after the action scripts: the followed character's set and path to the director
: camera-frame ( -- )
    event-state ev.camera-char sl@ camera-follow-check  dup event-state ev.camera-char l!
    dup $FF = if  drop exit  then
    character dup char.cam-set sl@ swap char.cam-path sl@ director-setup ;

\ ---- a fresh start ---------------------------------------------------------------------------
: reset-events ( -- )
    event-state ev.vars script-vars cells 0 fill
    0 event-state ev.bits l!   0 event-state ev.counter l!   -1 event-state ev.exit l!
    0 event-state ev.room-frames l!   0 event-state ev.result l!
    -1 event-state ev.message l!   -1 event-state ev.message-owner l!
    -1 event-state ev.slot l!   $FF event-state ev.self-id l!   -1 event-state ev.self-char l!
    0 event-state ev.leaving l!   0 event-state ev.self-frames l!
    free-slots ;
: reset-characters ( -- )
    characters 0 do
        i character  0 over char.present l!  -1 over char.id l!  -1 over char.room l!
        0 over char.scripted l!  -1 over char.actor l!  -1 over char.cam-set l!  -1 over char.cam-path l!
        0 swap char.silent l!
    loop  0 event-state ev.camera-char l! ;
-1 event-state ev.room l!  reset-characters  reset-events
