\ music.fs - the game's music (docs/subsystems/music.md): the background music (src/game/music.c
\ BgmCtl: a streamed track the scripts want) and the stage music (MusicDir: a stage set's
\ sequences - the calm music's two parts, the chase, the panic - mixed by their volumes). The
\ room's scripts ask; it plays them each frame.
IN: music
USING: engine actors messages story.strings ;

state: music-state
  \ the background music: the track playing and the one wanted ($FF none), paused, its level and fade
  cell field bgm-cur  cell field bgm-req  cell field bgm-pause
  1 floats field bgm-level  1 floats field bgm-fade  1 floats field bgm-speed
  \ the stage music: the stage set (-1 none), its step, held; the global volume, where it goes, how fast
  cell field sm-stage  cell field sm-step  cell field sm-hold
  1 floats field sm-global  1 floats field sm-to  1 floats field sm-rate
  4 cells field sm-track    \ each sequence's volume (0..255)
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
\ full, the others silent, the global volume fading in over 90 frames. (The chase and the panic
\ follow the danger: later.) ----
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
: sm-op ( op a b -- )
    sm-stage @ 0< if  2drop drop exit  then
    rot case
        0 of  sm-step @ 4 < if  drop s>f sm-to f!  else  sm-volume-to  then  endof
        2 of  2drop -1 sm-hold !  endof
        4 of  2drop 0 sm-hold !  endof
        5 of  2drop  4 0 do  0 i cells sm-track + !  loop  sm-apply  endof
        >r 2drop r>
    endcase ;
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
    sm-to f@ f>s 90 sm-volume-to ;
: sm-tick ( -- )   \ MusicDir_Update
    sm-stage @ 0< if  exit  then
    sm-step @ case
        0 of  1 sm-step !  endof
        1 of  sm-hold @ 0= if  4 0 do  i -1 seq-play  loop  2 sm-step !  then  endof
        2 of  sm-start  sm-apply  4 sm-step !  endof
        4 of
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
      $FF bgm-cur !  $FF bgm-req !  1e bgm-level f!  -1 sm-stage !  255e sm-to f!
      self subscribe tick ;
  on bgm-want ( track pause level -- )   \ (BgmCtl_Want: track $FF none - the playing one fades out)
      cell>f  bgm-pause !  dup bgm-req !  $FF <> if  bgm-level f!  else  fdrop  then ;
  on bgm-resume ( -- )  0 music-pause ;          \ (the ADX stream resumed)
  on music-op ( op a b -- )  sm-op ;               \ (0x6A)
  on music-stage ( stage -- )  sm-load ;            \ (0x6B; -1: the stage music ended, 0x6C)
  on tick ( -- )  bgm-tick  sm-tick ;
end-behaviour

: music-spawn ( -- id )  playing-music music-state s" music" spawn ;
