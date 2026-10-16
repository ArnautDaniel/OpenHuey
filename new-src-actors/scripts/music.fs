\ music.fs - the game's music (docs/subsystems/music.md): the background music (src/game/music.c
\ BgmCtl: a streamed track the scripts want) and the stage music (MusicDir: a stage set's
\ sequences - the calm music's two parts, the chase, the panic - mixed by their volumes). The
\ room's scripts ask; it plays them each frame.
IN: music
USING: engine game-state actors messages facts flag-names story.strings ;

state: music-state
  \ the background music: the track playing and the one wanted ($FF none), paused, its level and fade
  cell field bgm-cur  cell field bgm-req  cell field bgm-pause
  1 floats field bgm-level  1 floats field bgm-fade  1 floats field bgm-speed
  \ the stage music: the stage set (-1 none), its step, held; the global volume, where it goes, how fast
  cell field sm-stage  cell field sm-step  cell field sm-hold
  1 floats field sm-global  1 floats field sm-to  1 floats field sm-rate
  4 cells field sm-track    \ each sequence's volume (0..255)
  \ following the danger (MusicDir_StateFrame): its state; each sequence's volume fading - where
  \ to and by how much a frame; the chase's bend (its channels flagged by the stage) and where to
  cell field sm-state
  4 floats field sm-vol  4 floats field sm-goal  4 floats field sm-by
  16 floats field ch-bend  16 floats field ch-bend-goal  16 floats field ch-bend-by   \ (the chase's channels)
  1 floats field seq3  1 floats field seq3-goal  1 floats field seq3-by           \ (the chase's sequence volume)
  \ what it follows: the danger (0 calm, 1 followed, 2 chased), the panic's stage, the stalker
  \ after her (his alert 0: he sees her), him and his room (-1: none in the game)
  cell field sm-danger  cell field sm-panic  cell field sm-seen  cell field sm-st  cell field sm-st-room
  1 floats field sm-st-reach   \ (how far she is from him on foot: -1 no way)
end-state

\ ---- the background music: a new track starts at once (paused if asked); none fades the
\ playing one out over 30 frames; wanting it again fades it back in. Its loudness: the
\ original's 100 log10 v tenths of a dB - the square root of v as an amplitude ----
: bgm-start ( -- )  bgm-req @ dup bgm-cur !  bgm-track bgm-pause @ music-play drop ;
: bgm-tick ( -- )
    false                                                   ( start? )
    bgm-cur @ bgm-req @ <> if
        bgm-req @ $FF = if  -1e 30e f/ bgm-speed f!  else  drop true  1e bgm-fade f!  then
    else
        bgm-req @ $FF <> bgm-speed f@ f0< and if  1e 30e f/ bgm-speed f!  then
    then
    bgm-cur @ $FF <> if
        bgm-fade f@ bgm-speed f@ f+ 0e fmax 1e fmin bgm-fade f!
        bgm-fade f@ 1e f>= bgm-fade f@ 0e f<= or if  0e bgm-speed f!  then
        bgm-fade f@ 0e f<= if  music-stop  $FF bgm-cur !  then
    then
    bgm-fade f@ bgm-level f@ f* 0e fmax 1e fmin fsqrt music-volume!
    if  bgm-start  then ;

\ ---- the stage music: made for a stage set, its bank and four sequences (0 panic, 1 / 2 the
\ calm music's parts, 3 the chase) loaded; held and released; then started - the calm parts
\ full, the others silent, the global volume fading in over 90 frames; then following the danger
\ (below) ----
: sm-apply ( -- )   \ the sequences' volumes through the global one
    4 0 do  i  i cells sm-track + @ s>f sm-global f@ f* 255e f/ f>s  seq-port-volume  loop ;
: sm-file ( i -- addr len )  sm-stage @ swap stage-file ;
: sm-load ( stage -- )
    seq-reset  dup sm-stage !  0 sm-step !  0 sm-hold !  0e sm-global f!  255e sm-to f!
    0< if  exit  then
    0 sm-file 3 - seq-bank drop                      \ (BGM\STAGEn_BANK: .HD and .BD)
    4 0 do  i  i 1+ sm-file seq-load drop  0 i cells sm-track + !  loop ;
: sm-volume-to ( v frames -- )   \ MusicDir_GlobalVolumeTo
    swap s>f sm-to f!  dup 0> if  s>f  sm-to f@ sm-global f@ f- fswap f/ sm-rate f!
    else  drop  sm-to f@ sm-global f!  0e sm-rate f!  then ;
\ the four sequences, and the states of following the danger (MusicDir_StateFrame, below)
0 constant panic-track   1 constant calm-a   2 constant calm-b   3 constant chase-track
0 constant sm-calm  1 constant sm-quiet  2 constant sm-fading  3 constant sm-chase-now
4 constant sm-chase-soon  5 constant sm-chase-far  6 constant sm-chase-seen  7 constant sm-panic-now
8 constant sm-panicking  9 constant sm-after-panic
\ each sequence's volume fading (MusicDir_TrackVolumeTo / Rate)
: sm-vol# ( k -- addr )  floats sm-vol + ;
: same-volume? ( k v -- k v flag )  2dup swap sm-vol# f@ f>s = ;   \ (already there: any fade going on goes on)
: track-to ( k frames v -- )   \ (MusicDir_TrackVolumeTo)
    rot swap same-volume? if  drop 2drop exit  then  rot swap        ( k frames v )
    s>f  swap >r  fdup r@ floats sm-goal + f!                             ( frames ) ( F: v )
    r@ sm-vol# f@ f-  1 max s>f f/  r> floats sm-by + f! ;
: track-rate ( k v -- ) ( F: rate -- )   \ (MusicDir_TrackVolumeRate: toward v at that rate)
    same-volume? if  2drop fdrop exit  then
    s>f  >r  fdup r@ floats sm-goal + f!                                  ( F: rate v )
    r@ sm-vol# f@ f-  f0< if  fnegate  then  r> floats sm-by + f! ;
defer stage-chans ( -- )   \ (below)
: sm-op ( op a b -- )
    sm-stage @ 0< if  2drop drop exit  then
    rot case
        0 of  sm-step @ 4 < if  nip s>f sm-to f!  else  swap sm-volume-to  then  endof   \ (a frames, b the volume)
        1 of  2drop stage-chans  endof   \ (the stage's channels by the story's flags)
        2 of  2drop -1 sm-hold !  endof
        3 of  2drop  endof               \ (waiting for the banks: loaded at once here)
        4 of  2drop 0 sm-hold !  endof
        5 of  2drop  4 0 do  0e i sm-vol# f!  0e i floats sm-by + f!  0 i cells sm-track + !  loop   \ (MusicDir_Silence:
              sm-fading sm-state !  endof                                                            \ silent at once)
        >r 2drop r>
    endcase ;
defer chase-init ( -- )   \ (the chase's bends and sequence volume as the stage's: below)
: sm-start ( -- )   \ MusicDir_Start
    4 0 do
        i  sm-stage @ 3 stage-table i + c@ 20 max seq-volume
        16 0 do
            j i  sm-stage @ 0 stage-table j 16 * + i + c@  seq-chan-volume
            j  $B0 i or  10  sm-stage @ 2 stage-table j 16 * + i + c@  seq-midi
            j  $E0 i or  0  sm-stage @ 1 stage-table j 16 * + i + c@  seq-midi
        loop
    loop
    0 sm-track !  255 1 cells sm-track + !  255 2 cells sm-track + !  0 3 cells sm-track + !
    4 0 do  i cells sm-track + @ s>f  fdup i sm-vol# f!  i floats sm-goal + f!  0e i floats sm-by + f!  loop
    chase-init
    sm-quiet sm-state !
    stage-chans   \ (the chase's channel sets by the story's flags)
    sm-to f@ f>s 90 sm-volume-to ;
\ ---- the stage music following the danger (MusicDir_StateFrame): calm - the calm parts; being
\ followed - they fade out; chased - the chase (its start from the top), loud while he sees her,
\ else by how near he is (the chase table) and its flagged channels bent with it; at the panic's
\ stage 4 the panic alone (track 0, its channels 3..8 silent), back at stage 5; after it, back
\ to the danger's music. Not while a scene plays or the world is held ----
\ MusicDir_ChaseStep's table (every stage's the same): up to each distance, the chase's volume and
\ bend; further, the last
create chase-steps  30 , 210 , 111 ,  40 , 202 , 106 ,  50 , 194 , 100 ,  60 , 186 , 95 ,  70 , 178 , 90 ,
    80 , 170 , 85 ,  90 , 162 , 79 ,  100 , 154 , 74 ,  110 , 146 , 69 ,  120 , 138 , 64 ,  130 , 130 , 58 ,
    140 , 122 , 53 ,  150 , 115 , 48 ,  0 , 105 , 43 ,
: bent-channel? ( ch -- flag )   \ (the stage's +0x2C: channel 0 bends - stage 3 channel 5 too; stage 4 none)
    sm-stage @ case  0 of  0=  endof  1 of  0=  endof  2 of  dup 0= swap 5 = or  endof  >r drop false r>  endcase ;
: tracks-fade ( -- )
    4 0 do
        i floats sm-by + f@ f0= 0= if
            i sm-vol# f@ i floats sm-by + f@ f+                             ( F: v )
            i floats sm-by + f@ f0< if  i floats sm-goal + f@ fmax  else  i floats sm-goal + f@ fmin  then
            fdup i sm-vol# f!  i floats sm-goal + f@ f= if  0e i floats sm-by + f!  then
        then
        i sm-vol# f@ f>s i cells sm-track + !
    loop ;
\ the chase's channels' bends and its sequence volume, fading (MusicDir_BendTo, SeqVolumeTo)
: bend# ( ch -- addr )  floats ch-bend + ;
: send-bend ( ch -- )  chase-track over $E0 or 0  rot bend# f@ f>s 0 127 clamp  seq-midi ;
: bend-to ( ch frames v -- )   \ (MusicDir_BendTo: the whole range over those frames, either way)
    s>f  swap >r  fdup r@ floats ch-bend-goal + f!                        ( frames ) ( F: v )
    127e  1 max s>f f/  fswap                                             ( F: step v )
    r@ bend# f@ f>s s>f f< if  fnegate  then  r> floats ch-bend-by + f! ;
: bend-now ( ch v -- )  s>f  dup bend# f!  0e dup floats ch-bend-by + f!  send-bend ;
: bends-fade ( -- )
    16 0 do
        i floats ch-bend-by + f@ f0= 0= if
            i bend# f@  i floats ch-bend-by + f@ f+
            i floats ch-bend-by + f@ f0< if  i floats ch-bend-goal + f@ fmax  else  i floats ch-bend-goal + f@ fmin  then
            fdup i bend# f!  i floats ch-bend-goal + f@ f= if  0e i floats ch-bend-by + f!  then
            i send-bend
        then
    loop ;
: seq3-to ( frames v -- )   \ (MusicDir_SeqVolumeTo: 20..255; already there, nothing)
    20 max 255 min  dup seq3 f@ f>s = if  2drop exit  then
    s>f fdup seq3-goal f!  seq3 f@ f-  1 max s>f f/  seq3-by f! ;
: seq3-fade ( -- )
    seq3-by f@ f0= if  exit  then
    seq3 f@ seq3-by f@ f+  seq3-by f@ f0< if  seq3-goal f@ fmax  else  seq3-goal f@ fmin  then
    fdup seq3 f!  seq3-goal f@ f= if  0e seq3-by f!  then
    chase-track  seq3 f@ f>s 20 max 255 min  seq-volume ;
\ ---- each stage's chase (MusicStage1..4): seen, its sequence volume up and these channels bent
\ full; lost, the volume back and them centred; the story's flags switching sets of its channels
\ (the scripts' op 1) ----
create seen-vols  $88 , $8C , $8C , $6E ,
create seen-1  7 , 0 , 1 , 3 , 4 , 5 , 6 , 7 ,    create lost-1  6 , 1 , 3 , 4 , 5 , 6 , 7 ,
create seen-2  5 , 0 , 2 , 3 , 4 , 5 ,            create lost-2  4 , 2 , 3 , 4 , 5 ,
create seen-3  6 , 0 , 1 , 2 , 4 , 5 , 6 ,        create lost-3  4 , 1 , 2 , 4 , 6 ,
create seen-4  6 , 0 , 1 , 2 , 3 , 5 , 6 ,        create lost-4  6 , 0 , 1 , 2 , 3 , 5 , 6 ,
: by-stage ( a b c d -- x )  sm-stage @ 0 3 clamp  3 swap - pick  >r 2drop 2drop r> ;
: seen-chans ( -- list )  seen-1 seen-2 seen-3 seen-4 by-stage ;
: lost-chans ( -- list )  lost-1 lost-2 lost-3 lost-4 by-stage ;
: own-seq-volume ( -- v )  sm-stage @ 3 stage-table chase-track + c@ 20 max ;
: bend-list ( list frames v -- )   \ (each channel of a counted list)
    rot dup @ 0 ?do  dup i 1+ cells + @  3 pick 3 pick bend-to  loop  drop 2drop ;
: stage-seen ( now? -- )   \ (state 6)
    sm-chase-seen sm-state !  if  1  else  30  then
    dup  seen-vols sm-stage @ 0 3 clamp cells + @  seq3-to
    seen-chans swap 127 bend-list ;
: stage-lost ( now? -- )   \ (state 5)
    sm-chase-far sm-state !  if  1  else  90  then
    dup own-seq-volume seq3-to
    lost-chans swap 64 bend-list ;
: chase-channel ( ch on? -- )   \ (MusicDir_ChannelOn / Off)
    if  chase-track swap  sm-stage @ 0 stage-table chase-track 16 * + over + c@  else  chase-track swap 0  then
    seq-chan-volume ;
: channels ( on? a.. n -- )   \ (n channels off the stack, all on or off)
    0 ?do  over chase-channel  loop  drop ;
:noname ( -- )
    16 0 do  sm-stage @ 1 stage-table chase-track 16 * + i + c@ s>f  fdup i bend# f!  i floats ch-bend-goal + f!  0e i floats ch-bend-by + f!  loop
    own-seq-volume s>f  fdup seq3 f!  seq3-goal f!  0e seq3-by f! ; is chase-init
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
: chase-step ( -- addr | 0 )   \ (MusicDir_ChaseStep: 0 with no stalker in the game; not in her
    stalker-in? 0= if  0 exit  then                                        \ room: the last)
    st-dist  fdup f0< if  fdrop 1e9  then  chase-steps
    begin  dup @ 0>  if  fdup f0< 0=  over @ s>f fover f> 0= and  else  false  then  while  3 cells +  repeat  fdrop ;
: chase-by-distance ( -- )   \ (MusicDir_ChaseTrack: its flagged channels bent by the step too)
    chase-step ?dup 0= if  exit  then
    chase-track over cell+ @  2.5e track-rate
    2 cells + @  16 0 do  i bent-channel? if  i 90 2 pick bend-to  then  loop  drop ;
: stop-fade ( k -- )  0e floats sm-by + f! ;   \ (its volume fade dropped)
: to-calm ( -- )  sm-calm sm-state !  calm-a stop-fade  calm-b stop-fade ;
: to-panic ( -- )  sm-panic-now sm-state !  panic-track stop-fade ;
: to-chase ( s -- )  sm-state !  chase-track stop-fade ;
: followed ( -- )  sm-fading sm-state ! ;
: restart ( k -- )  dup 0 seq-play  -1 seq-play ;
variable cb-v  variable cb-b   \ (the chase's volume and bend as it begins)
: chase-begins ( now? -- )   \ (MusicDir_ChaseBegins: the chase from its start)
    chase-step ?dup if  dup cell+ @ cb-v !  2 cells + @ cb-b !  else  0 cb-v !  0 cb-b !  then
    if  1  calm-a 1 0 track-to  calm-b 1 0 track-to
    else  90  panic-track 90 0 track-to  then                              ( frames )
    sm-seen @ if  true stage-seen  255 cb-v !  else  true stage-lost  then
    0 cb-b @ bend-now                                                    \ (channel 0 at the step's bend)
    chase-track restart  chase-track swap cb-v @ track-to ;
: panic-channels ( on? -- )   \ (track 0's channels 3..8: their own volumes, or silent)
    9 3 do
        dup if  panic-track i  sm-stage @ 0 stage-table panic-track 16 * + i + c@  else  panic-track i 0  then
        seq-chan-volume
    loop  drop ;
: panic-now ( -- )   \ (MusicDir_Panic)
    sm-panicking sm-state !  panic-track restart
    calm-a 1 0 track-to  calm-b 1 0 track-to  chase-track 1 0 track-to  panic-track 1 255 track-to
    false panic-channels ;
: after-panic ( -- )
    sm-panic @ 4 < 0= if  exit  then
    sm-danger @ 0= if  to-calm
    else sm-danger @ 2 = if  sm-chase-soon to-chase
    else  followed  panic-track 90 0 track-to  then then ;
\ (not while the world is held - flag 8; the original also skips the frame after a script's
\ fade-finish: Events_ScenePlaying)
: follow-danger ( -- )
    world-held state-flag? if  exit  then
    sm-state @ case
        sm-calm of  sm-quiet sm-state !
            chase-track 90 0 track-to  panic-track 90 0 track-to  calm-a 90 255 track-to  calm-b 90 255 track-to  endof
        sm-quiet of
            sm-danger @ 1 = if  followed  calm-a 90 0 track-to  calm-b 90 0 track-to
            else sm-danger @ 2 = if  sm-chase-now to-chase
            else sm-panic @ 4 = if  to-panic  then then then  endof
        sm-fading of
            sm-danger @ 0= if  to-calm
            else sm-danger @ 2 = if  sm-chase-now to-chase
            else sm-panic @ 4 = if  to-panic  then then then  endof
        sm-chase-now of  true chase-begins  endof
        sm-chase-soon of  false chase-begins  endof
        sm-chase-far of
            sm-danger @ 0= if  to-calm
            else sm-danger @ 1 = if  followed  chase-track 90 0 track-to
            else sm-seen @ if  false stage-seen  chase-track 30 255 track-to
            else sm-panic @ 4 = if  to-panic
            else  chase-by-distance  then then then then  endof
        sm-chase-seen of
            sm-danger @ 0= if  to-calm
            else sm-danger @ 1 = if  followed  chase-track 90 0 track-to
            else sm-seen @ 0= if  false stage-lost
            else sm-panic @ 4 = if  to-panic  then then then then  endof
        sm-panic-now of  panic-now  endof
        sm-panicking of   \ (over at its stage 5: the channels back; then on as after it, the same frame)
            sm-panic @ 5 = if  sm-after-panic sm-state !  true panic-channels  then  after-panic  endof
        sm-after-panic of  after-panic  endof
    endcase ;
: sm-tick ( -- )   \ MusicDir_Update
    sm-stage @ 0< if  exit  then
    sm-step @ case
        0 of  1 sm-step !  endof
        1 of  sm-hold @ 0= if  4 0 do  i -1 seq-play  loop  2 sm-step !  then  endof
        2 of  sm-start  sm-apply  4 sm-step !  endof
        4 of
            follow-danger  tracks-fade  bends-fade  seq3-fade
            sm-rate f@ f0= 0= if
                sm-global f@ sm-rate f@ f+ sm-global f!
                sm-rate f@ f0< if  sm-global f@ sm-to f@ f<  else  sm-global f@ sm-to f@ f>  then
                if  sm-to f@ sm-global f!  0e sm-rate f!  then
            then
            sm-apply
        endof
    endcase ;

behaviour playing-music
  on spawned ( -- )
      $FF bgm-cur !  $FF bgm-req !  1e bgm-level f!  -1 sm-stage !  255e sm-to f!  -1 sm-st !
      self subscribe tick  self subscribe danger  self subscribe panic  self subscribe stalker-doing  self subscribe stalker-reach ;
  on bgm-want ( track pause level -- )   \ (BgmCtl_Want: track $FF none - the playing one fades out)
      cell>f  bgm-pause !  dup bgm-req !  $FF <> if  bgm-level f!  else  fdrop  then ;
  on bgm-resume ( -- )  0 music-pause ;          \ (the ADX stream resumed)
  on music-op ( op a b -- )  sm-op ;               \ (0x6A)
  on music-stage ( stage -- )  sm-load ;            \ (0x6B; -1: the stage music ended, 0x6C)
  on danger ( level -- )  sm-danger ! ;
  on panic ( stage level -- )  drop sm-panic ! ;
  on stalker-reach ( d -- )  cell>f sm-st-reach f! ;
  on stalker-doing ( room mode sub cond hp hp-max kind alert came -- )   \ (his alert 0: after her, he sees her)
      drop 0= sm-seen !  2drop 2drop 2drop  sm-st-room !  sender sm-st ! ;
  on tick ( -- )  bgm-tick  sm-tick ;
end-behaviour

: music-spawn ( -- id )  playing-music music-state s" music" spawn ;
