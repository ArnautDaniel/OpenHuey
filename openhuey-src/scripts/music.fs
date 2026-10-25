\ music.fs - the game's music (docs/subsystems/music.md): the background music (src/game/music.c
\ BgmCtl: a streamed track the scripts want) and the stage music (MusicDir: a stage set's
\ sequences - the calm music's two parts, the chase, the panic - mixed by their volumes). The
\ room's scripts ask; it plays them each frame.
IN: music
USING: engine game-state actors messages facts flag-names story-names story.strings ;

\ a value fading toward a goal by a step a frame (MusicDir's volumes and bends)
begin-structure /fader  ffield: now  ffield: goal  ffield: by  end-structure

state: music-state
  \ the background music: the track playing and the one wanted (no-track), paused, its level and fade
  cell field bgm-cur  cell field bgm-req  cell field bgm-pause
  1 floats field bgm-level  1 floats field bgm-fade  1 floats field bgm-speed
  1 floats field bgm-duck   \ (turned down while the pause or the sub screen is open: Bgm +0x114)
  \ the stage music: the stage set (-1 none), its step, held; the global volume, where it goes, how fast
  cell field sm-stage  cell field sm-step  cell field sm-hold
  1 floats field sm-global  1 floats field sm-to  1 floats field sm-rate
  \ following the danger (MusicDir_StateFrame): its state; each sequence's volume (0..255); the
  \ chase's channels' bends (flagged by the stage) and its sequence's own volume
  cell field sm-state
  4 /fader * field tracks
  16 /fader * field bends
  /fader field seq3
  \ what it follows: the danger, the panic's stage, the stalker after her (he sees her), him and
  \ his room (-1: none in the game)
  cell field sm-danger  cell field sm-panic  cell field sm-seen  cell field sm-st  cell field sm-st-room
  1 floats field sm-st-reach   \ (how far she is from him on foot: -1 no way)
end-state

$FF constant no-track

\ ---- faders ----
: fader-set ( fader -- ) ( F: v -- )  {: fd :}  fdup fd now f!  fd goal f!  0e fd by f! ;
: fader-at? ( fader v -- flag )  swap now f@ f>s = ;   \ (already there: whole numbers)
: fader-over ( fader frames -- ) ( F: v -- )   \ toward v in that many frames
    {: fd n :}  fdup fd goal f!  fd now f@ f-  n 1 max s>f f/  fd by f! ;
: fader-step ( fader -- moved? )   \ a frame's step, not past the goal
    {: fd :}  fd by f@ f0= if  false exit  then
    fd now f@ fd by f@ f+  fd by f@ f0< if  fd goal f@ fmax  else  fd goal f@ fmin  then
    fdup fd now f!  fd goal f@ f= if  0e fd by f!  then  true ;

\ ---- the background music: a new track starts at once (paused if asked); none fades the
\ playing one out over 30 frames; wanting it again fades it back in. Its loudness: the
\ original's 100 log10 v tenths of a dB - the square root of v as an amplitude ----
: bgm-volume ( -- )  bgm-fade f@ bgm-level f@ f* bgm-duck f@ f* 0e fmax 1e fmin fsqrt music-volume! ;
: bgm-start ( -- )  bgm-req @ dup bgm-cur !  bgm-track bgm-pause @ music-play drop ;
: bgm-tick ( -- )
    false                                                   ( start? )
    bgm-cur @ bgm-req @ <> if
        bgm-req @ no-track = if  -1e 30e f/ bgm-speed f!  else  drop true  1e bgm-fade f!  then
    else
        bgm-req @ no-track <> bgm-speed f@ f0< and if  1e 30e f/ bgm-speed f!  then
    then
    bgm-cur @ no-track <> if
        bgm-fade f@ bgm-speed f@ f+ 0e fmax 1e fmin bgm-fade f!
        bgm-fade f@ 1e f>= bgm-fade f@ 0e f<= or if  0e bgm-speed f!  then
        bgm-fade f@ 0e f<= if  music-stop  no-track bgm-cur !  then
    then
    bgm-volume
    if  bgm-start  then ;

\ ---- the stage music: made for a stage set, its bank and four sequences loaded; held and
\ released; then started - the calm parts full, the others silent, the global volume fading in
\ over 90 frames; then following the danger (below) ----
enum: sequence  panic-track calm-a calm-b chase-track ;
: track ( k -- fader )  /fader * tracks + ;
: track-volume ( k -- v )  track now f@ f>s ;
: bend ( ch -- fader )  /fader * bends + ;
\ the stage's tables (story/strings.fs): each sequence's own volume; each channel's volume, bend, pan
: own-volume ( k -- v )  sm-stage @ 3 stage-table + c@ ;
: stage-byte ( k ch table -- byte )  sm-stage @ swap stage-table  rot 16 * + + c@ ;
: chan-volume ( k ch -- v )  0 stage-byte ;
: chan-bend ( k ch -- v )  1 stage-byte ;
: chan-pan ( k ch -- v )  2 stage-byte ;
$B0 constant midi-control   $E0 constant midi-bend   10 constant pan-control
: sm-apply ( -- )   \ the sequences' volumes through the global one
    4 0 do  i  i track-volume s>f sm-global f@ f* 255e f/ f>s  seq-port-volume  loop ;
: sm-file ( i -- addr len )  sm-stage @ swap stage-file ;
: sm-load ( stage -- )
    seq-reset  dup sm-stage !  0 sm-step !  0 sm-hold !  0e sm-global f!  255e sm-to f!
    0< if  exit  then
    0 sm-file 3 - seq-bank drop                      \ (BGM\STAGEn_BANK: .HD and .BD)
    4 0 do  i  i 1+ sm-file seq-load drop  0e i track fader-set  loop ;
: sm-volume-to ( v frames -- )   \ MusicDir_GlobalVolumeTo
    swap s>f sm-to f!  dup 0> if  s>f  sm-to f@ sm-global f@ f- fswap f/ sm-rate f!
    else  drop  sm-to f@ sm-global f!  0e sm-rate f!  then ;
\ the states of following the danger (MusicDir_StateFrame, below)
enum: music-state  sm-calm sm-quiet sm-fading sm-chase-now sm-chase-soon sm-chase-far sm-chase-seen
    sm-panic-now sm-panicking sm-after-panic ;
\ each sequence's volume fading (MusicDir_TrackVolumeTo / Rate; already there: any fade going on goes on)
: track-to ( k frames v -- )  {: k n v :}  k track v fader-at? if  exit  then  v s>f  k track n fader-over ;
: track-rate ( k v -- ) ( F: rate -- )   \ (toward v at that rate)
    {: k v :}  k track {: fd :}  fd v fader-at? if  fdrop exit  then
    v s>f fd goal f!  v s>f fd now f@ f- f0< if  fnegate  then  fd by f! ;
: stop-fade ( k -- )  track 0e by f! ;
\ the scripts' ops (0x6A)
enum: music-op  op-volume op-stage-chans op-hold op-wait-banks op-release op-silence ;
defer stage-chans ( -- )   \ (below)
: silence ( -- )   \ (MusicDir_Silence: silent at once)
    4 0 do  0e i track fader-set  loop  sm-fading sm-state ! ;
: sm-op ( op a b -- )
    {: op a b :}  sm-stage @ 0< if  exit  then
    op case
        op-volume of  sm-step @ 4 < if  b s>f sm-to f!  else  b a sm-volume-to  then  endof   \ (a frames, b the volume)
        op-stage-chans of  stage-chans  endof   \ (the stage's channels by the story's flags)
        op-hold of  -1 sm-hold !  endof
        op-release of  0 sm-hold !  endof
        op-silence of  silence  endof
    endcase ;   \ (op-wait-banks: waiting for the banks - loaded at once here)
defer chase-init ( -- )   \ (the chase's bends and sequence volume as the stage's: below)
: sm-start ( -- )   \ MusicDir_Start
    4 0 do
        i  i own-volume 20 max seq-volume
        16 0 do
            j i  j i chan-volume  seq-chan-volume
            j  midi-control i or  pan-control  j i chan-pan  seq-midi
            j  midi-bend i or  0  j i chan-bend  seq-midi
        loop
    loop
    0e panic-track track fader-set  255e calm-a track fader-set  255e calm-b track fader-set  0e chase-track track fader-set
    chase-init
    sm-quiet sm-state !
    stage-chans   \ (the chase's channel sets by the story's flags)
    sm-to f@ f>s 90 sm-volume-to ;
\ ---- the stage music following the danger (MusicDir_StateFrame): calm - the calm parts; being
\ followed - they fade out; chased - the chase (its start from the top), loud while he sees her,
\ else by how near he is (the chase table) and its flagged channels bent with it; panicking the
\ panic alone (track 0, its channels 3..8 silent), back as she calms down; after it, back to the
\ danger's music. Not while a scene plays or the world is held ----
\ MusicDir_ChaseStep's table (every stage's the same): up to each distance, the chase's volume and
\ bend; further, the last (up to 0)
begin-structure /chase-step  field: step-upto  field: step-volume  field: step-bend  end-structure
create chase-steps  30 , 210 , 111 ,  40 , 202 , 106 ,  50 , 194 , 100 ,  60 , 186 , 95 ,  70 , 178 , 90 ,
    80 , 170 , 85 ,  90 , 162 , 79 ,  100 , 154 , 74 ,  110 , 146 , 69 ,  120 , 138 , 64 ,  130 , 130 , 58 ,
    140 , 122 , 53 ,  150 , 115 , 48 ,  0 , 105 , 43 ,
: bent-channel? ( ch -- flag )   \ (the stage's +0x2C: channel 0 bends - stage 3 channel 5 too; stage 4 none)
    sm-stage @ case  0 of  0=  endof  1 of  0=  endof  2 of  dup 0= swap 5 = or  endof  >r drop false r>  endcase ;
: tracks-fade ( -- )  4 0 do  i track fader-step drop  loop ;
\ the chase's channels' bends and its sequence volume, fading (MusicDir_BendTo, SeqVolumeTo)
: send-bend ( ch -- )  {: ch :}  chase-track  midi-bend ch or  0  ch bend now f@ f>s 0 127 clamp  seq-midi ;
: bend-to ( ch frames v -- )   \ (MusicDir_BendTo: the whole range over those frames, either way)
    {: ch n v :}  v s>f ch bend goal f!
    127e n 1 max s>f f/  v s>f  ch bend now f@ f>s s>f f< if  fnegate  then  ch bend by f! ;
: bend-now ( ch v -- )  {: ch v :}  v s>f ch bend fader-set  ch send-bend ;
: bends-fade ( -- )  16 0 do  i bend fader-step if  i send-bend  then  loop ;
: seq3-to ( frames v -- )   \ (MusicDir_SeqVolumeTo: 20..255; already there, nothing)
    20 max 255 min  {: n v :}  seq3 v fader-at? if  exit  then  v s>f seq3 n fader-over ;
: seq3-fade ( -- )
    seq3 fader-step 0= if  exit  then
    chase-track  seq3 now f@ f>s 20 max 255 min  seq-volume ;
\ ---- each stage's chase (MusicStage1..4): seen, its sequence volume up and these channels bent
\ full; lost, the volume back and them centred; the story's flags switching sets of its channels
\ (the scripts' op 1) ----
create seen-vols  $88 , $8C , $8C , $6E ,
create seen-1  7 , 0 , 1 , 3 , 4 , 5 , 6 , 7 ,    create lost-1  6 , 1 , 3 , 4 , 5 , 6 , 7 ,
create seen-2  5 , 0 , 2 , 3 , 4 , 5 ,            create lost-2  4 , 2 , 3 , 4 , 5 ,
create seen-3  6 , 0 , 1 , 2 , 4 , 5 , 6 ,        create lost-3  4 , 1 , 2 , 4 , 6 ,
create seen-4  6 , 0 , 1 , 2 , 3 , 5 , 6 ,        create lost-4  6 , 0 , 1 , 2 , 3 , 5 , 6 ,
create seen-lists  seen-1 , seen-2 , seen-3 , seen-4 ,
create lost-lists  lost-1 , lost-2 , lost-3 , lost-4 ,
: by-stage ( table -- x )  sm-stage @ 0 3 clamp cells + @ ;
: own-seq-volume ( -- v )  chase-track own-volume 20 max ;
: bend-list ( list frames v -- )   \ (each channel of a counted list)
    {: list n v :}  list @ 0 ?do  list i 1+ cells + @ n v bend-to  loop ;
: stage-seen ( now? -- )   \ (state 6)
    sm-chase-seen sm-state !  if  1  else  30  then  {: n :}
    n  seen-vols by-stage  seq3-to
    seen-lists by-stage n 127 bend-list ;
: stage-lost ( now? -- )   \ (state 5)
    sm-chase-far sm-state !  if  1  else  90  then  {: n :}
    n own-seq-volume seq3-to
    lost-lists by-stage n 64 bend-list ;
: chase-channel ( ch on? -- )   \ (MusicDir_ChannelOn / Off)
    {: ch on :}  chase-track ch  on if  chase-track ch chan-volume  else  0  then  seq-chan-volume ;
: channels ( on? a.. n -- )   \ (n channels off the stack, all on or off)
    0 ?do  over chase-channel  loop  drop ;
:noname ( -- )
    16 0 do  chase-track i chan-bend s>f i bend fader-set  loop
    own-seq-volume s>f seq3 fader-set ; is chase-init
:noname ( -- )   \ (op 1: MusicStageN_StageChans)
    sm-stage @ case
        0 of  $1B story-flag?  dup 2 3 4 5 4 channels  0= 6 7 8 3 channels  endof
        2 of  $89 story-flag?  5 6 2 channels  endof
        3 of  $95 story-flag?  dup 0= 3 4 2 channels  5 6 7 3 channels  endof
    endcase ; is stage-chans
: stalker-in? ( -- flag )  sm-st @ dup 0< if  drop false exit  then  dup alive? if  body?  else  drop false  then ;
: st-dist ( F: -- d | -1 )   \ (how far she is on foot - else straight - from him; not in her room: -1)
    stalker-in? 0= if  -1e exit  then  sm-st @
    sm-st-room @ room-id <> if  drop -1e exit  then
    sm-st-reach f@ f0< 0= if  drop sm-st-reach f@ exit  then
    body-at  s" fiona" actor-named body-at vec-dist ;
: chase-step ( -- step | 0 )   \ (MusicDir_ChaseStep: 0 with no stalker in the game; not in her room: the last)
    stalker-in? 0= if  0 exit  then
    st-dist  fdup f0< if  fdrop 1e9  then  {: F: d :}
    chase-steps  begin  dup step-upto @ dup 0> swap s>f d f<= and  while  /chase-step +  repeat ;
: chase-by-distance ( -- )   \ (MusicDir_ChaseTrack: its flagged channels bent by the step too)
    chase-step ?dup 0= if  exit  then  {: step :}
    chase-track step step-volume @  2.5e track-rate
    16 0 do  i bent-channel? if  i 90 step step-bend @ bend-to  then  loop ;
: to-calm ( -- )  sm-calm sm-state !  calm-a stop-fade  calm-b stop-fade ;
: to-panic ( -- )  sm-panic-now sm-state !  panic-track stop-fade ;
: to-chase ( s -- )  sm-state !  chase-track stop-fade ;
: to-fading ( -- )  sm-fading sm-state ! ;
: seq-restart ( k -- )  dup 0 seq-play  -1 seq-play ;
: chase-begins ( now? -- )   \ (MusicDir_ChaseBegins: the chase from its start)
    {: now | frames volume bent :}
    chase-step ?dup if  dup step-volume @ to volume  step-bend @ to bent  then
    now if  1 to frames  calm-a 1 0 track-to  calm-b 1 0 track-to
    else  90 to frames  panic-track 90 0 track-to  then
    sm-seen @ if  true stage-seen  255 to volume  else  true stage-lost  then
    0 bent bend-now                                                    \ (channel 0 at the step's bend)
    chase-track seq-restart  chase-track frames volume track-to ;
: panic-channels ( on? -- )   \ (track 0's channels 3..8: their own volumes, or silent)
    {: on :}  9 3 do  panic-track i  on if  panic-track i chan-volume  else  0  then  seq-chan-volume  loop ;
: panic-now ( -- )   \ (MusicDir_Panic)
    sm-panicking sm-state !  panic-track seq-restart
    calm-a 1 0 track-to  calm-b 1 0 track-to  chase-track 1 0 track-to  panic-track 1 255 track-to
    false panic-channels ;
: after-panic ( -- )
    sm-panic @ panicking >= if  exit  then
    sm-danger @ calm = if  to-calm
    else sm-danger @ chased = if  sm-chase-soon to-chase
    else  to-fading  panic-track 90 0 track-to  then then ;
: calm-fades ( -- )  calm-a 90 0 track-to  calm-b 90 0 track-to ;
\ (not while the world is held - flag 8; the original also skips the frame after a script's
\ fade-finish: Events_ScenePlaying)
: follow-danger ( -- )
    world-held state-flag? if  exit  then
    sm-state @ case
        sm-calm of  sm-quiet sm-state !
            chase-track 90 0 track-to  panic-track 90 0 track-to  calm-a 90 255 track-to  calm-b 90 255 track-to  endof
        sm-quiet of
            sm-danger @ followed = if  to-fading  calm-fades
            else sm-danger @ chased = if  sm-chase-now to-chase
            else sm-panic @ panicking = if  to-panic  then then then  endof
        sm-fading of
            sm-danger @ calm = if  to-calm
            else sm-danger @ chased = if  sm-chase-now to-chase
            else sm-panic @ panicking = if  to-panic  then then then  endof
        sm-chase-now of  true chase-begins  endof
        sm-chase-soon of  false chase-begins  endof
        sm-chase-far of
            sm-danger @ calm = if  to-calm
            else sm-danger @ followed = if  to-fading  chase-track 90 0 track-to
            else sm-seen @ if  false stage-seen  chase-track 30 255 track-to
            else sm-panic @ panicking = if  to-panic
            else  chase-by-distance  then then then then  endof
        sm-chase-seen of
            sm-danger @ calm = if  to-calm
            else sm-danger @ followed = if  to-fading  chase-track 90 0 track-to
            else sm-seen @ 0= if  false stage-lost
            else sm-panic @ panicking = if  to-panic  then then then then  endof
        sm-panic-now of  panic-now  endof
        sm-panicking of   \ (over as she calms down: the channels back; then on as after it, the same frame)
            sm-panic @ calming-down = if  sm-after-panic sm-state !  true panic-channels  then  after-panic  endof
        sm-after-panic of  after-panic  endof
    endcase ;
: global-fade ( -- )   \ the global volume toward where it goes
    sm-rate f@ f0= if  exit  then
    sm-global f@ sm-rate f@ f+ sm-global f!
    sm-rate f@ f0< if  sm-global f@ sm-to f@ f<  else  sm-global f@ sm-to f@ f>  then
    if  sm-to f@ sm-global f!  0e sm-rate f!  then ;
: sm-tick ( -- )   \ MusicDir_Update
    sm-stage @ 0< if  exit  then
    sm-step @ case
        0 of  1 sm-step !  endof
        1 of  sm-hold @ 0= if  4 0 do  i -1 seq-play  loop  2 sm-step !  then  endof
        2 of  sm-start  sm-apply  4 sm-step !  endof
        4 of  follow-danger  tracks-fade  bends-fade  seq3-fade  global-fade  sm-apply  endof
    endcase ;

\ ---- a save loaded (SceneGame_LoadedStart): the stage music the story has come to, made anew -
\ the latest stage's flag set (none: no stage music) ----
: loaded-stage ( -- stage | -1 )
    over-music-d story-flag? if  3 exit  then
    over-music-c story-flag? if  2 exit  then
    over-music-b story-flag? if  1 exit  then
    opening-done story-flag? if  0 exit  then  -1 ;

behaviour playing-music
  on spawned ( -- )
      no-track bgm-cur !  no-track bgm-req !  1e bgm-level f!  1e bgm-duck f!  -1 sm-stage !  255e sm-to f!  -1 sm-st !
      self subscribe tick  self subscribe held-tick  self subscribe danger  self subscribe panic  self subscribe stalker-doing  self subscribe stalker-reach
      self subscribe game-loaded ;
  on bgm-want ( track pause level -- )   \ (BgmCtl_Want: no track - the playing one fades out)
      cell>f  bgm-pause !  dup bgm-req !  no-track <> if  bgm-level f!  else  fdrop  then ;
  on bgm-resume ( -- )  0 music-pause ;          \ (the ADX stream resumed)
  on music-duck ( v -- )  cell>f 0e fmax 1e fmin bgm-duck f!  bgm-volume ;   \ (at once: the menus are held)
  on music-op ( op a b -- )  sm-op ;               \ (0x6A)
  on music-stage ( stage -- )  sm-load ;            \ (0x6B; -1: the stage music ended, 0x6C)
  on game-loaded ( -- )  loaded-stage sm-load ;
  on danger ( level -- )  sm-danger ! ;
  on panic ( stage level -- )  drop sm-panic ! ;
  on stalker-reach ( d -- )  cell>f sm-st-reach f! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )   \ (after her: he sees her)
      drop after-her = sm-seen !  2drop 2drop 2drop  sm-st-room !  sender sm-st ! ;
  on tick ( -- )  bgm-tick  sm-tick ;
  on held-tick ( -- )  bgm-tick ;   \ (a menu up: the background track still changes - the music gallery's)
end-behaviour

: music-spawn ( -- id )  playing-music music-state s" music" spawn ;
