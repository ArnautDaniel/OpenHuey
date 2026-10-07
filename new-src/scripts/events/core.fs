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
\ the room's exits and doors (the door table): exit `exit`'s door (-1 none), a door's fixed
\ flags (bit 0: a doorway, always open)
defer event-exit-door ( exit -- door )   :noname drop -1 ; is event-exit-door
defer event-door-flags ( door -- flags ) :noname drop 0 ; is event-door-flags
\ the nav mesh: a group of triangles' flags set / cleared, one triangle's, is a triangle in a group
defer event-nav-group ( set? group bits -- )      :noname 2drop drop ; is event-nav-group
defer event-nav-tri ( set? tri bits -- )          :noname 2drop drop ; is event-nav-tri
defer event-nav-in-group? ( group -- flag ) ( F: x y z -- )
:noname drop fdrop fdrop fdrop false ; is event-nav-in-group?
\ sounds: of bank `bank` (4 the sound set, 5 common, 6 the room's) heard plainly, or at a point;
\ which sound set bank 4 holds
defer event-sound ( id bank -- )             ' 2drop is event-sound
defer event-sound-at ( id bank -- ) ( F: x y z -- )
:noname 2drop fdrop fdrop fdrop ; is event-sound-at
defer event-sound-set ( set -- )             ' drop is event-sound-set
\ characters: an actor's place set, its heading (radians), shown or not; its animation played
\ (once or looping) and whether it has come to its end
defer event-char-place ( cs -- ) ( F: x y z -- )   :noname drop fdrop fdrop fdrop ; is event-char-place
defer event-char-yaw ( cs -- ) ( F: a -- )          :noname drop fdrop ; is event-char-yaw
defer event-char-show ( cs on -- )                  ' 2drop is event-char-show
defer event-char-anim ( cs anim loop? -- )          :noname 2drop drop ; is event-char-anim
defer event-char-anim-done? ( cs -- flag )          :noname drop true ; is event-char-anim-done?
\ a character leaves the scene / comes back (the original: its Deactivate / Activate)
defer event-char-out ( cs -- )   ' drop is event-char-out
defer event-char-in ( cs -- )    ' drop is event-char-in
\ a nav triangle's middle (for placing on triangles)
defer event-tri-center ( tri -- ) ( F: -- x y z )   :noname drop 0e 0e 0e ; is event-tri-center
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
: anim-move ( cs anim move -- )
    rot >r  r@ character char.move-anim  2 pick swap l!  r@ swap move!  r> swap 0 event-char-anim ;

\ ---- characters' places and areas ----------------------------------------------------------
: char-pos ( cs -- ) ( F: -- x y z )  character char.pos dup sf@ dup 4 + sf@ 8 + sf@ ;
: char-prev ( cs -- ) ( F: -- x y z )  character char.prev dup sf@ dup 4 + sf@ 8 + sf@ ;
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
        0 over char.scripted l!  -1 over char.actor l!  -1 over char.cam-set l!  -1 swap char.cam-path l!
    loop  0 event-state ev.camera-char l! ;
-1 event-state ev.room l!  reset-characters  reset-events
