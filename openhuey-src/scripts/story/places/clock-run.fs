\ story/places/clock-run.fs - what the rooms of the clock run share ($D9 .. $D0 the forest's, $E0 ..
\ $E9 the castle's: their rooms over again, Hewie the one played, Fiona following him): the time
\ shown at the foot of the screen each frame (story/clock.fs), gone as he leaves; the rooms ahead
\ loaded as he nears their ways; the run begun (the world let go, faded in); its end at the goal -
\ the time kept and shown, the results, out to the extras room; she slips out of a hiding place
\ as he leaves without her. A shared place file (story/kit.fs); no cells of its own.
IN: story.places.clock-run
USING: engine game-state actors messages common facts room-names flag-names story-names stalker-names
    story.kit story.clock stalker.anims ;

\ ---- the clock: shown each frame (the rooms' frame hook - their condition always holds); gone as
\ he leaves (the converted rooms' room.fs cleared it each frame) ----
: clock-shown ( -- )  clock-draw ;
: clock-gone ( -- )  clock-off ;

\ ---- his ways: crossing into an area (or out of one), the room behind that exit loaded ahead;
\ out by an exit's own area (the rooms' exit-check by an area) ----
: his-way ( area exit -- )  swap hewie swap entered? if  prepare-exit  else  drop  then ;
: his-way-back ( area exit -- )  swap hewie swap left? if  prepare-exit  else  drop  then ;
: his-ways ( table n -- )  0 ?do  dup i 2* cells + @  over i 2* 1+ cells + @  his-way  loop  drop ;
: by-areas ( n -- )  0 ?do  i i leaves-by  loop ;

\ ---- the run begun (by the forced way, Fiona played: the room's own setting out): the stage music
\ held and let go; then Hewie's - the camera afresh, the world let go, faded in ----
: music-taken ( -- )  2 0 0 stage-music  4 0 0 stage-music ;   \ (op-hold, op-release)
: run-stage ( stage -- )  music send music-stage ;   \ (the stage music for the run)
: run-begins ( -- )  hewie wait-move  camera-restart  world-held state-off  15 screen:from-black fade  wait-fade ;

\ ---- the goal (Fiona's, as she comes into its area): the time kept; the stage music down, the
\ ending's track; the results (and the next menu); faded out, the clock stopped - Hewie and she
\ into the extras room by its second forced way, the stalker gone ----
$34 constant goal-track   $81 constant to-extras   $10C constant extras-door
$C constant results-menu   9 constant after-results-menu
: goal-track-on ( -- )  goal-track 1 1e f>cell music send bgm-want ;   \ (held till resumed)
: run-over ( forest? -- )
    {: forest? :}
    stalkers-blind state!  stalkers-stay state!  fiona hold  clock-save
    forest? if  no-track 1e background  then
    goal-track-on  0 30 0 stage-music  30 frames  fiona wait-move
    music send bgm-resume  off-map-37 preload
    results-menu open-subscreen  after-results-menu open-subscreen
    forest? if  no-track 1e background  then
    15 screen:to-black with-music or fade  wait-fade
    clock-stop  off-map-37 -1 0 hewie-to  extras-door door-shut  stalker-unloaded
    stalkers-stay state-off  to-extras take-exit ;
: waits-out ( -- )  hewie hold  hewie wait-move  begin  yield  again ;   \ (Hewie's, meanwhile: held still)
: at-goal ( forest? -- )   \ (her and Hewie's reaction: her panic eased)
    panic-eased  if  [: true run-over ;]  else  [: false run-over ;]  then  fiona start-holding drop
    ['] waits-out hewie start-holding drop ;

\ ---- the castle's hiding places in the run: Fiona slips in at Hewie's command as he stands by
\ one (she here and free, nobody after her, no stalker); the stalker coming in while she hides
\ finds her by the odds (or a creature moving about gives her away), and goes to her spot ----
: hide-cue? ( idle? F: x z r -- flag )   \ (idle?: she must be idle as well - Fiona_IsIdle)
    hewie centre-across? fiona here? and  fiona held? 0= and  in-panic? 0= and  played-gesture -1 <> and
    stalker-in? 0= and  swap if  fiona-idle? and  then ;
: run-searches ( pct var xt -- )   \ (xt: his going to her, found - started; else the summoner sooner; counted)
    {: pct v xt :}  pct chance?  2 creature-moving? or if  xt start drop  else  120 summoner-sooner  then
    v var@ 1+ v var! ;
: run-finds-her ( tri deg F: x z -- )   \ (to her spot; there - she's found; he waits till she's out, then his own)
    {: tri deg F: x z :}  stalker tri deg x z walk-to  stalker there-anim play  -1 found-her !
    stalker 10 idle  begin  fiona held? while  yield  repeat  0 stalker-order ;

\ ---- a hiding place left behind: he went with her still in it - out, at its front ----
: left-hidden ( tri deg F: x z -- )
    {: tri deg F: x z :}  stalkers-stay state-off  fiona-hidden state-off  fiona tri deg x z place-at ;
