\ fiona/brain.fs - Fiona's frame (src/game/fiona.c Fiona_Update): who is about, her fear,
\ the stick, what she is asked to do, her behaviour, keeping clear of the others, her voice
\ and her footsteps.
IN: fiona.brain
USING: engine game-state events.core events.words chars fiona.core fiona.moves ;

\ the noise the stalkers hear (Noise_Make: loudness, room, triangle; with them)
defer f-noise ( loud room tri -- )   :noname drop 2drop ; is f-noise
: noise-here ( loud -- )  f-room f-tri f-noise ;

\ ---- who is about (Fiona_UpdatePresence) ----
: in-game? ( cs -- flag )  dup c-ok? 0= if  drop false exit  then  character char.present sl@ 0<> ;
: presence ( -- )
    0 f-dog-here !  0 f-dog-ok !  0 f-pu-here !  0 f-pu-ok !
    dog in-game? if  1 f-dog-here !  dog character char.disabled sl@ 0= if  1 f-dog-ok !  then  then
    pursuer-slot @ dup 0< 0= swap in-game? and if
        1 f-pu-here !  pursuer-slot @ character char.disabled sl@ 0= if  1 f-pu-ok !  then
    then ;

\ ---- Fiona_Panic: the panic's stage taken up (frightened at 2, panicking at 4 - with a scream
\ others hear - for a while), her fear meter, her timers ----
: full-panic ( -- )   \ Fiona_FullPanic
    f-fear-bits @ 2 or f-fear-bits !  3e rnd f* f>s 30 * 120 + f-panic-t !  30 f-stumble-t !
    $6F noise-here ;
: fear-down ( -- )   \ Fiona_FearDown
    game-mode @ 2 = if  -0.1e  else  -0.15e  then  change-fear ;
: fiona-panic ( -- )
    panic @                                                            ( threat )
    f-fear-bits @ 1 and 0= if
        dup 2 >= if
            f-fear-bits @ 1 or f-fear-bits !
            dup case
                4 of  full-panic  endof
                3 of  4e rnd f* f>s 30 * 150 + f-panic-t !  endof
                2 of  5e rnd f* f>s 30 * 240 + f-panic-t !  endof
            endcase
        then
    else dup 4 < if
        dup 2 < if  1e f-tired-w f!  f-fear-bits @ 3 invert and f-fear-bits !
        else  f-fear-bits @ 2 invert and f-fear-bits !  then
    else  f-fear-bits @ 2 and 0= if  full-panic  then  then then
    drop
    f-mode 0= f-fear-bits @ 2 and 0= and f-recovery @ 0> and if  -1 f-recovery +!  then
    f-fear f@ 100e f< if
        f-mode 0= f-act @ $E <> and if
            f-group case
                3 of  endof
                2 of  charm @ $8C <> if  game-mode @ 2 = if  0.0666667e  else  0.0466767e  then  change-fear  then  endof
                1 of  fear-down  endof
                >r  f-fear-bits @ 2 and 0= if  fear-down  then  r>
            endcase
        else f-mode 3 = if  fear-down  then then
    then
    f-freeze-t @ 0> if   \ (the blow's hold: Motion_Unfreeze)
        -1 f-freeze-t +!  f-freeze-t @ 0<= if
            f-actor dup 0< 0= if  actor act.mflags dup l@ $40 invert and swap l!  else  drop  then
        then
    then
    f-door-t @ if  -1 f-door-t +!  then
    f-busy-t @ if  -1 f-busy-t +!  then
    f-mode if  0 f-run-t !  then ;

\ ---- Fiona_KeepApart: out of Hewie when she walks into him (the stalker's push: with them) ----
: keep-apart ( -- )
    hewie-control @  f-dog-ok @ 1 <> or  if  exit  then
    me dog 0e 0e c-touching? 0= if  exit  then
    me dog c-push-out if  me c-pos f-prev vec-copy  f-prev-tri @ me c-tri!  me c-sync  then
    me c-path-end ;

\ ---- her voice (Fiona_Voice): muffled under water (her feet's or her mouth's floor flags
\ 0x2008000: sound 0x1C of the room's bank) ----
create mouth 12 allot
: under-water? ( -- flag )
    f-tri dup 0< if  drop false exit  then  nav-flags $2008000 and $2008000 = if  true exit  then
    f-actor dup 0< if  drop false exit  then  $13 bone-pos mouth vec!
    mouth me c-mask v-tri-in dup 0< if  drop false exit  then  nav-flags $2008000 and $2008000 = ;
: f-sound ( id bank vol pitch -- )  >r >r >r >r  me r> r> r> r>  f-pos vec@ actor-sound ;
: voice ( id bank vol pitch -- )
    under-water? if  2drop 2drop  $1C 6 0 0 f-sound exit  then  f-sound ;

\ ---- Fiona_MotionSounds: on her animations' key frames (event bits 1 and 0x10) ----
defer call-hewie   ' noop is call-hewie     \ Fiona_CallHewie (fiona.commands)
: motion-sounds ( -- )
    f-events dup 1 and if
        f-anim@ case
            1 of  f-mode $D = if  $39 5 0 0 f-sound  then  endof
            $C0E of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C0D of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C0A of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C07 of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C06 of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C04 of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C03 of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C02 of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $C00 of  f-mode $D = if  $25 state-flag? 0= if  call-hewie  else  $33 5 0 0 f-sound  then  then  endof
            $D01 of  $3C 5 0 0 f-sound  endof
            $D00 of  $3C 5 0 0 f-sound  endof
            $E00 of  $26 pvar@ dup 7 <> swap 6 <> and if  $3C 5 0 0 f-sound  then  $1F noise-here  endof
        endcase
    then
    $10 and if
        f-anim@ case
            $403 of  $F 5 0 0 f-sound  endof
            $F02 of  $7D 5 0 0 voice  endof  $1404 of  $7D 5 0 0 voice  endof
            $1403 of  $7D 5 0 0 voice  endof  $1500 of  $7D 5 0 0 voice  endof
            $B01 of  $7E 5 0 0 f-sound  endof
            $100B of  $80 5 0 0 voice  endof  $1008 of  $80 5 0 0 voice  endof  $B00 of  $80 5 0 0 voice  endof
            $609 of  $71 5 0 0 f-sound  endof  $608 of  $71 5 0 0 f-sound  endof
            $E00 of  $26 pvar@ dup 6 = if  drop $21 5 0 0 f-sound  else  7 = if  $20 5 0 0 f-sound  then  then  endof
        endcase
    then ;

\ ---- Fiona_Footsteps: as a foot comes down (the motion's foot contacts -4; standing: planted,
\ or the motion fading out's), its floor's sound (the sound set's, bank 4: by the triangle's
\ material bits, a variant a step), louder the faster she goes; the stalkers hear it (louder
\ running) ----
fvariable ft-x  fvariable ft-z  create foot-at 12 allot
: contact ( prev? -- l r )   \ the contact track: left, right down
    f-actor swap -4 swap motion-track 0= if  fdrop fdrop fdrop 0 0 exit  then
    fdrop  f0> 1 and  f0> 1 and ;
: foot-place ( left? -- )   \ where the foot is in the room (its place track, turned by her)
    f-actor swap if  -3  else  -2  then  0 motion-track drop
    fswap fdrop  f-yaw rotate-by  f-pos 8 + sf@ f+ ft-z f!  f-pos sf@ f+ ft-x f!
    ft-x f@ foot-at sf!  f-pos 4 + sf@ foot-at 4 + sf!  ft-z f@ foot-at 8 + sf! ;
variable st-l  variable st-r  variable st-step  variable ft-base  variable ft-bank
: footsteps ( -- )
    f-disabled? f-tri 0< or if  exit  then
    8 state-flag? if  exit  then
    0 contact st-r ! st-l !
    f-group 0= if
        f-actor actor act.fade sf@ f0> if  -1 contact st-r ! st-l !  else  1 st-l !  1 st-r !  then
    then
    0 st-step !
    st-l @ 1 = f-step-l @ 0= and if  1 st-step !  else  st-r @ 1 = f-step-r @ 0= and if  -1 st-step !  then  then
    st-l @ f-step-l !  st-r @ f-step-r !
    st-step @ 0= if  f-settled? f-group 0= and if  0 f-steps !  then  exit  then
    st-step @ 1 = foot-place
    0 ft-base !  4 ft-bank !
    foot-at me c-mask v-tri-in dup 0< if  drop f-tri  then  nav-flags
    $2018000 and case
        $2008000 of  6 sound-loaded? if  $10 ft-base !  6 ft-bank !  else  $15 ft-base !  then  endof
        $2000000 of  $10 ft-base !  endof
        $18000 of  $C ft-base !  endof
        $10000 of  8 ft-base !  endof
        $8000 of  4 ft-base !  endof
    endcase
    ft-base @ f-steps @ 3 and +  ft-bank @  1 f-steps +!               ( base bank )
    root@ rm-z f@ 0.4e f- 0.7e f/ 0e fmax 1e fmin 2e f* f>s $7F and     ( base bank pitch )
    worn @ $80 = if
        -48 swap f-sound  f-mode 0= f-sub 2 = and if  5  else  1  then
    else
        0 swap f-sound  f-mode 0= f-sub 2 = and if  $14  else  4  then
    then  noise-here ;

\ ---- Fiona_Update ----
defer state-block   ' noop is state-block          \ Fiona_StateBlock (fiona.commands)
defer control-command   ' noop is control-command  \ Fiona_ControlCommand (fiona.commands)
defer read-command   ' noop is read-command        \ her commands to Hewie (Gesture_Update)
defer joint-action   ' noop is joint-action        \ Fiona_JointAction (fiona.commands)
: f-req@ ( -- n )  me character char.req sl@ ;
create f-end-pos 12 allot   \ where her frame left her (moved since by the rooms or a script: her
                            \ triangle found anew)
: fiona-frame ( -- )
    f-actor 0< if  exit  then
    f-tri 0<  f-pos f-end-pos vec-dist 0.01e f> or if  me c-find-tri me c-tri!  then
    f-prev f-pos vec-copy  f-tri f-prev-tri !  f-yaw f-yaw-was f!
    f-2b @ if  0  else  $28020018  then  me c-mask!
    0 f-look-on !  1 f-cam-on !
    presence  fiona-panic
    move-input
    -1 f-cmd-code !
    f-req@ if  state-block  else  control-command  then
    read-command
    f-state @ ?dup if  execute  then
    keep-apart  joint-action
    motion-sounds  footsteps
    f-pu-ok @ if   \ (him within 200: condition bit 5; within 150: 0 too)
        me pursuer-slot @ c-pos c-dist-to  fdup 200e f<= if  5 cond-bit!  150e f<= if  0 cond-bit!  then  else  fdrop  then
    then
    f-mode 0= f-act @ 1 <> and if  f-sub 2 > if  0 f-sub!  then  then
    f-req@ 7 = if  0 me character char.req l!  then
    f-end-pos f-pos vec-copy ;
