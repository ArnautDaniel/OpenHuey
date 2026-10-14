\ creatures/state.fs - a spirit's state, and its kind's numbers (D_004164F0: a pattern, not a
\ table - see docs/subsystems/creatures.md).
IN: creatures.state
USING: engine game-state actors paths creatures.trip ;

state: spirit-state
  cell field slot                 \ 0..6
  cell field kind  cell field reach  cell field level   \ reach: how near she must come (0: at once)
  /trip field trip                \ where it is (its body only in the room being played), and its way to her
  cell field time                 \ frames it has been after her
  cell field come                 \ after her
  cell field out                  \ sent after her off screen: resting first
  cell field rest
  cell field waited               \ (kind $24) frames before it comes
  cell field glow                 \ its look: the engine's particle system
  cell field vanishing  cell field seen
  cell field move-mode            \ 0, or 2 hanging about her (the story asks: 0x53)
  cell field gave-up  cell field tries    \ straight at her on another level
  cell field centred              \ put on its triangle's centre the first time it waits
  \ about her (fiona-doing)
  cell field her-mode  cell field her-doing
  cell field her-plight   \ (fiona-plight: 3 out of reach - struck, seized, or set aside by the story's 0x21)
  1 floats field her-dist
  \ hanging about her: the spot, and whether to look for another
  cell field hang-tri  12 field hang-at  cell field hang-again
  cell field company               \ (a stalker in her room when it set off: 2/3 the pace)
  /path field my-path
end-state

: my-room ( -- a )  trip t.room ;
: my-tri ( -- a )  trip t.tri ;

\ ---- its kind ----
: k ( -- n )  kind @ 18 mod ;
: odd-one? ( -- flag )  kind @ $24 = ;                       \ (blue, quick to come, floating lower)
: red? ( -- flag )  kind @ $12 $24 within ;
: straight-only? ( -- flag )  odd-one? if  false  else  k 9 >=  then ;   \ (only from her triangle)
: lifetime ( -- frames )   \ 1, 2 or 4 minutes
    odd-one? if  1800 exit  then  k 9 mod 3 /  case  0 of 1800 endof  1 of 3600 endof  >r 7200 r>  endcase ;
: speed ( F: -- s )   \ a frame
    odd-one? if  0.25e exit  then  k 3 mod 1+ s>f 0.125e f* ;
: most-tries ( -- n )  straight-only? if  30  else  60  then ;
