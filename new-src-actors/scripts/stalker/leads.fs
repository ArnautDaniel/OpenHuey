\ stalker/leads.fs - what draws a stalker searching the room being played (src/game/pursuer.c
\ Pursuer_PickTarget, Pursuer_LocateTarget, Pursuer_ReactToEvent): what he hears gives him a lead -
\ its pull (Fiona's noise 5, any other 4, Hewie's 1) and how alarming it is (hearing her 6, a noise
\ 5, hearing Hewie 1; seeing her 7). A lead pulling as hard as the one he follows, or harder, is
\ followed: to where she is, to the noise's spot (his search's one stop), to Hewie. One more
\ alarming than his last makes him start: turned to it, his cry, a moment (90 frames), then on;
\ seeing her anew, his "there". (A noise he can see is nothing: let be.) His
\ leads are forgotten as a new search begins.
IN: stalker.leads
USING: engine game-state actors messages common facts flag-names paths vectors rumble stalker.anims stalker.sounds stalker.state stalker.senses stalker.moving stalker.search stalker.chase ;

5 constant pull-fiona   4 constant pull-noise   1 constant pull-hewie
7 constant saw-her   6 constant heard-her   5 constant heard-noise   2 constant saw-hewie   1 constant heard-hewie
90 constant start-frames

: lead-heard ( -- pull alarm | 0 0 )   \ (the noise heard this frame, by whose)
    heard-source @ case
        fiona-noise of  pull-fiona heard-her  endof
        hewie-noise of  pull-hewie heard-hewie  endof
        world-noise of  pull-noise heard-noise  endof
        >r 0 0 r>
    endcase ;
: forget-leads ( -- )  0 lead !  0 lead-alarm !  0 did-hear ! ;

\ ---- his start (Pursuer_ReactToEvent: the stand, turned to it, his cry; the pad shaken) ----
create there /vec allot
: starting ( -- )   \ (turned to it; over at its frames' end - a gesture's: at its end)
    start-at turn-rate turn-to fdrop  root-move
    -1 start-t +!  start-t @ 0>  anim @ still-anim <> ended? and 0= and if  exit  then
    0 start-t !  start-then @ execute ;
: start ( then-xt anim -- ) ( F: x y z -- )   \ (a start: `anim` played to its end, or - 0 - his stand a
    \ moment with his cry)
    start-at vec!  swap start-then !  no-search search-phase !   \ (his search held meanwhile)
    $80 30 stalker-jolt
    ?dup if  play-now  1000  else  still-anim play  start-cry 7 sound  start-frames  then  start-t !
    ['] starting step! ;

\ ---- going after the lead (Pursuer_LocateTarget) ----
: to-the-noise ( -- )   \ (its spot his one stop)
    heard-tri @ dup 0< if  drop search-again exit  then
    -1 drawn-tri !  searching mode!  route-clear  0 route-add  search-start ;
: to-hewie ( -- )
    hewie here? 0= if  search-again exit  then
    -1 drawn-tri !  searching mode!  route-clear  hewie body-tri 0 route-add  search-start ;
: go-after ( pull -- )
    case
        pull-fiona of  after-her mode!  chase-start  endof   \ (to where she is)
        pull-noise of  to-the-noise  endof
        pull-hewie of  to-hewie  endof
    endcase ;
variable go-pull   \ (the lead he started at: followed once the start is over)
: go-after-it ( -- )  go-pull @ go-after ;

\ ---- each frame, searching the room being played: true when a lead took him ----
: seen-already? ( pull -- flag )   \ (a noise where he can see: nothing there)
    pull-noise = 0= if  false exit  then
    heard-tri @ dup 0< if  drop false exit  then  tri-center there vec!  there in-view? ;
: follow-heard ( -- took? )
    did-hear @ 0= if  false exit  then
    heard-room @ self body-room <> if  false exit  then   \ (elsewhere: the summoner's)
    lead-heard {: pull alarm :}
    pull 0= pull lead @ < or if  false exit  then
    pull seen-already? if  false exit  then
    alarm lead-alarm @ >  pull lead !  alarm lead-alarm @ max lead-alarm !  if
        pull go-pull !  ['] go-after-it 0
        heard-tri @ dup 0< if  drop my-at vec@  else  tri-center  then  start  true exit
    then
    pull go-after  true ;
\ seeing Hewie (pull 1, alarm 2 - Pursuer_PickTarget each frame): to where he is, his search's one
\ stop; seen still once he is past it, there again (Pursuer_ModesSearching: a new lead with his
\ route under way searches anew)
: follow-seen-dog ( -- took? )
    sees-hewie @ 0=  pull-hewie lead @ <  or if  false exit  then
    lead @ pull-hewie =  route-next @ 0= and  my-mode @ searching = and if  false exit  then   \ (on his way to it)
    saw-hewie lead-alarm @ >  pull-hewie lead !  saw-hewie lead-alarm @ max lead-alarm !  if
        pull-hewie go-pull !  ['] go-after-it 0  hewie body-at vec@  start  true exit
    then
    pull-hewie go-after  true ;
: lead-now ( -- took? )  follow-heard dup if  exit  then  drop  follow-seen-dog ;
\ seeing her anew (alarm 7 over what he had): his "there" before the chase
: chase-seen ( -- )
    lead-alarm @ saw-her < if
        pull-fiona lead !  saw-her lead-alarm !
        ['] chase-now there-anim  fiona body-at vec@  start exit
    then
    chase-now ;
' chase-seen is chase-begin
' lead-now is lead-taken?
:noname ( -- flag )  step @ ['] starting = ; is starting?
' forget-leads is leads-forgotten
