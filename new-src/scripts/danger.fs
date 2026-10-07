\ danger.fs - the danger state (src/game/scene_game.c SceneGame_Danger): the game mode
\ (Progress_GameMode, `game-mode`: 0 calm, 1 tense, 2 chased) each frame, which the music,
\ Fiona's fear and panic, Hewie and the scripts read. State flags force it (0x1B calm, 0x1F
\ chased, 7 tense); the stalker in Fiona's room makes it chased; otherwise it moves between the
\ three by the condition bits (2 no stalker, 6 hunted), flag 0x22, his alert (+0x16C8: 3 / 4)
\ and a creature in her room, each change holding a while. The condition bits are the frame's:
\ cleared here after.
IN: danger
USING: engine state game-state events.core events.words chars noises pursuer.core ;

2 constant stalker-slot   \ (gCharSlot2)
variable d-prev    \ +0x51 the state before the last change
variable d-last    \ +0x52 the state last frame
variable d-timer   \ +0x58 frames before it may change again
\ (not yet: the camera director's free mode - the event camera - skips it; creatures)
defer camera-free? ( -- flag )   ' false is camera-free?
: creature-here? ( -- flag )  false ;

: stalker-on? ( -- flag )  stalker-slot c-active? ;
: alert ( -- a )   \ Progress_StalkerAlert
    stalker-on? 0= if  $FF exit  then  $16C8 pu-c@ ;
\ (as the original: any exit not leading to his room counts as near)
: near? ( sroom room -- flag )
    2dup = if  2drop false exit  then
    8 0 do  dup i room-exit-leads drop 2 pick <> if  2drop true unloop exit  then  loop  2drop false ;
: mode-to ( m t -- )  d-timer !  game-mode ! ;
: from-calm ( -- )
    $22 state-flag? if  1 $1C2 mode-to exit  then
    d-timer @ 0= if  2 cond-bit? 0= creature-here? or 6 cond-bit? or if  1 $1C2 mode-to  then  then ;
: from-tense ( -- )
    6 cond-bit? $22 state-flag? or if  d-timer @ $96 + $1C2 min d-timer !  then
    2 cond-bit? creature-here? 0= and if
        d-timer @ 0= if  0 $1E mode-to  then
    else d-prev @ 0= if  $1C2 d-timer !
    else d-prev @ 2 = if  $96 d-timer !  then then then ;
: from-chased ( near? -- )
    2 cond-bit? if  drop  $22 state-flag? creature-here? or 1 and $1E mode-to exit  then
    alert 3 - 2 u< and if  d-timer @ 0= if  1 $1E mode-to  then
    else  $96 d-timer !  then ;
: by-rooms ( on -- )   \ (no flag forcing it)
    her c-room  swap if  stalker-slot c-room  else  -1  then      ( room sroom )
    2dup = if  2drop  2 $1E mode-to exit  then
    dup -1 = if  2drop false  else  swap near?  then              ( near? )
    game-mode @ case  0 of  drop from-calm  endof  1 of  drop from-tense  endof
                      2 of  from-chased  endof  >r drop r>  endcase
    d-timer @ if  -1 d-timer +!  then ;
: danger ( -- )
    stalker-on? dup if  stalker-slot character char.cond sl@ 2 =  else  false  then  ( on chasing )
    alert $FF = 2 pick 0= and if  2 cond-bit!  then
    if  3 cond-bit!  then
    game-mode @ d-last !
    $1B state-flag? if  drop 0 0 mode-to
    else $1F state-flag? if  drop 2 0 mode-to
    else 7 state-flag? if  drop 1 0 mode-to
    else  by-rooms  then then then
    d-last @ game-mode @ <> if  d-last @ d-prev !  then
    1 cond-bit?  0 cond-bits !
    game-mode @ 0= and if  6 cond-bit!  then ;
: danger-tick ( -- )
    playing @ paused @ 0= and 0= if  exit  then
    hear-all  camera-free? 0= if  danger  then ;   \ (everyone hears the frame's noises first)
' danger-tick on-tick
