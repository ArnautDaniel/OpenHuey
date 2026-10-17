\ hewie/state.fs - Hewie's state: one actor's fields (docs/subsystems/hewie.md). `hewie enter`
\ at the console shows them. Where he is is his body (the engine's body operations move it);
\ the comments give the original's offsets in Hewie (+0xF35..) where they help to compare.
IN: hewie.state
USING: actors messages paths vectors ;

state: hewie-state
  cell field his-model          \ his model (an Actor)
  cell field his-fiona          \ Fiona's actor id
  \ what he is doing: the action (numbered as the original's; named as they're ported) and the
  \ behaviour that carries it out, run each frame (an xt: the original's PTMF +0xF35D0)
  cell field his-action   cell field his-last-action
  cell field his-next           \ the argument / the action after (+0xF3570)
  cell field his-act            \ the behaviour (an xt)
  cell field his-mood-act       \ his mode's behaviour: calm / wary / tense (an xt)
  cell field his-wait           \ frames before he moves on (+0xF355C)
  cell field his-target   cell field his-target2   \ whom: an actor id (-1 none)
  cell field his-cmd            \ what may break in on his action (the on-.. bits below) (+0xF356C)
  cell field his-pending        \ choose what to do next
  cell field his-look-to  cell field his-look-delay   \ where he is to look, after a delay
  cell field his-yelp     cell field his-yelp-anim
  \ his body as the others see it (the original's character fields)
  cell field his-hp             \ 0..100
  cell field his-cond           \ well-cond, hurt-cond, down-cond (messages.fs)
  cell field his-mode           \ his move mode (+0xF8; hewie-mode, hewie-names.fs)
  cell field his-sub            \ what he is doing, for the story (+0xFC; hewie-sub)
  cell field his-away           \ out of the room being played (the character's +0x29)
  cell field his-busy           \ a script has him (+0xE0; H4)
  cell field his-done           \ the script's move is done (+0xE1; H4)
  /path field his-path
  /path field his-probe          \ a way planned only to see if there is one (Hewie_FionaReachable)
  \ how he feels and obeys
  cell field his-mood   cell field his-mood-time   \ hewie-mood
  cell field his-waiting        \ waiting (1: does as he likes) or obeying (0)
  cell field his-obey           \ frames left of that
  cell field his-obey-marked
  cell field his-nudge
  cell field his-praise-due
  cell field his-trust          \ 0..7
  cell field his-trust-points   \ 0..10000
  cell field his-broke          \ an activity broken off once
  cell field his-cooldown
  cell field his-stay           \ keeps him in pose 4
  cell field his-pet-time
  cell field his-praise-a  cell field his-praise-b
  cell field his-did  cell field his-did-was  cell field his-did-2   \ what he did, for praise
  cell field his-follows        \ the thing he fetches (its index; -1 none)
  \ his skills at fetching (out of 32: 0 going after it, 1 following it with his eyes, 2 eager,
  \ 3 picking it up, 4 bringing word of it, 5 coming back when it's lost), the rolls made and
  \ how they came out (Hewie_RollReaction)
  6 cells field his-skill
  8 cells field his-rolls   6 cells field his-rolled   cell field his-roll-n
  \ what he notices
  cell field his-alert  cell field his-alert-was  cell field his-alert-what
  cell field his-held           \ Fiona was held
  cell field his-panic-seen     \ her panic's stages seen (1: 4, 2: 5)
  cell field his-scene-req
  cell field his-call           \ who called him ($FF none; 0 Fiona)
  cell field his-hits  cell field his-hit-t   \ how often Fiona hit him lately
  cell field his-hold-call   \ (+0xF35B0) 900 frames after a call while waiting, or a scuffle: no 0x4F / 0x50, no scuffle
  cell field his-ready
  cell field his-no-root        \ the behaviour moved him this frame
  cell field his-root-ok
  cell field his-smells         \ he smells something (his-scent)
  cell field his-snd  cell field his-snd-t    \ his last sound, frames since
  cell field his-2b  cell field his-2d        \ through blocked floor / left alone
  cell field his-near           \ how near she is, for the story: 1 within 20, 2 within 50, 3 further
  \ his behaviours' working values
  cell field his-t1  cell field his-t2  cell field his-t3
  1 floats field his-heading  1 floats field his-turn  1 floats field his-f36cc
  /vec field his-spot
  cell field his-hushed         \ his own sounds don't play (the scripts' 0x3D)
  cell field his-creature-t     \ a creature about in his room lately (frames left)
  cell field his-exit           \ the exit he makes for (hewie/exits.fs)
  /vec field his-cut  cell field his-cut-tri   \ the spot on the stalker's way he makes for first
  cell field his-wanted         \ how many times, or a wait
  \ where he looks (Hewie_TurnHead)
  cell field his-look-now  cell field his-look-t
  1 floats field his-look-pitch  1 floats field his-look-yaw   \ a look held (mode 8, glances)
  1 floats field his-head-pitch  1 floats field his-head-yaw   \ his head turned
  1 floats field his-yaw-was
  cell field his-look-char      \ an actor he looks at (-1 none)
  cell field his-look-pt?  /vec field his-look-pt
  /vec field his-scent
  \ where her commands put him
  1 floats field his-side  cell field his-side-dir
  cell field his-last-idle
  cell field his-cmd-was  cell field his-cmd-act
  cell field his-heard          \ her command this frame (+0xF3578 within a frame; else -1)
  \ a spot to go to: its triangle, an animation, the heading, the point
  cell field his-to-tri  cell field his-to-anim  1 floats field his-to-yaw  /vec field his-to
  cell field his-pet
  cell field his-hurt-t         \ frames to his next health point
  cell field his-by  cell field his-how   \ who struck him last ($FF: a door), and how
  \ off screen
  cell field his-door           \ the exit he came in by (-1 none)
  cell field his-way            \ +0xF3668: the half of his room he is on (exit-way: 0 / 1; 2 either;
                                \ -1 by the exit he came in by)
  cell field his-route-door     \ the door he is making for
  1 floats field his-left       \ how far there is left to it
  cell field his-at-door        \ 1 his way to it planned in the room, 0 counting
  cell field his-noise-room     \ the room of a noise he heard (-1 none)
  cell field his-hide           \ he stays where he is
  13 cells field his-avoid      \ doors that let him down (a bit a door, route-avoiding)
  cell field his-idle-depth
  \ his body fitted to the floor (DogModel_BodyFrames)
  1 floats field his-floor-k  1 floats field his-back-pitch  1 floats field his-front-pitch
  \ his sounds, overlays and feet
  cell field his-snd-anim  1 floats field his-snd-frame
  cell field ov-head  cell field ov-ears  cell field ov-tail
  cell field ov-head-t  cell field ov-ears-t  cell field ov-ears-alt  cell field ov-tail-t  cell field ov-tail-on
  4 cells field his-feet-was  cell field his-feet-n
  cell field his-was-busy
  cell field his-joining        \ asked to join before Fiona was in the house: on her arrival
  \ a joint action with her (the meeting by his side): its type (-1 none), and its part (0 asked, 1 the second)
  cell field his-meet  cell field his-meet-part
  \ told by others: the danger (0 calm, 1 followed, 2 chased), the panic's stage, what Fiona does
  cell field his-danger
  cell field his-panic
  cell field fiona-mode  cell field fiona-sub  cell field fiona-cond  cell field fiona-cmd
  cell field fiona-room         \ the room she is in, or going to (told by the rooms: leaving, arrived)
  \ the stalker as he tells it each frame (`stalker-doing`; H3): who, his room, how he moves and
  \ what he does (3 off screen, 4 struck; 9 bitten, $A down), condition, health, kind; frames
  \ since he last told (the original reads them off his character)
  cell field st-id  cell field st-room  cell field st-mode  cell field st-sub  cell field st-cond
  cell field st-hp  cell field st-hp-max  cell field st-kind  cell field st-age
  cell field st-alert            \ the stalker's alert: his mode (+0x16C8; 1..3 hunting)
  \ the creatures, as each tells (creature-doing): its actor, move mode (8 holding her down, 4
  \ struck or going), up (after her), frames since (by slot: spirits 0..6, homunculi 7..9)
  #creatures cells field cr-ids  #creatures cells field cr-modes  #creatures cells field cr-ups  #creatures cells field cr-ages
  cell field his-scuffled   \ scuffled with him this meeting (+0xF368A; till another his-hold-call)
  \ at the stalker (hewie/fight.fs): the grip (+0x104: 0 head on, 1 / 2 a flank, -1 none yet),
  \ biting (+0xF36B4: 0 / 1 biting, 2 let go, 3 thrown), bites left (+0xF36B8), the chance to
  \ hold on (+0xF36BC), his feelings about each stalker (+0xF367C: Debilitas, Daniella, Lorenzo,
  \ Riccardo; -10..10), hit by her lately (+0xF35C4, its timer +0xF35C8); the leap's step and
  \ turn, the fling (+0xF36E0 / +0xF36C8), a
  \ push (+0xF36C8 less +0xF36CC a frame, along +0xF36C4)
  cell field his-grip  cell field his-biting  cell field his-bites  cell field his-hold
  4 cells field his-feel
  1 floats field his-leap-k  1 floats field his-leap-turn  1 floats field his-leap-yaw
  1 floats field his-fly-x  1 floats field his-fly-y  1 floats field his-fly-z  1 floats field his-fly-h
  1 floats field his-push  1 floats field his-push-less  1 floats field his-push-yaw
  /vec field his-blow-pt   \ where a blow from no one came from (blow-from: from -2)
  \ the story's leap (hewie/scripted.fs): its way (level), his height, the fall a frame, the rise's scale
  /vec field his-jump-dir  1 floats field his-jump-y  1 floats field his-jump-fall  1 floats field his-jump-rise
  \ his pounce (hewie/pounce.fs): his height, its top, how fast he rises, the way he leaps
  1 floats field his-pounce-y  1 floats field his-pounce-top  1 floats field his-pounce-vy  1 floats field his-pounce-yaw
end-state

\ what may break in on an action (his-cmd's bits; the top bit: nothing does, whatever the rest)
1 constant on-obeying       \ he obeys her again (pleased, or his waiting over): he chooses afresh
2 constant on-waiting       \ his obeying over
4 constant on-danger        \ the danger changes
8 constant on-command       \ her commands
$10 constant on-noise       \ off screen: a noise from another room
$20 constant on-footing     \ his nose over an edge, her right by him
$40 constant on-story       \ off screen: the story's condition 1
$80 constant on-wait-over   \ his wait ran out
$100 constant on-100        \ (set by the original, never tested)
$200 constant on-alert      \ followed: something alerts him
$400 constant on-praise-due \ near her, due a praise
$80000000 constant nothing-breaks-in
: but ( bits bits' -- bits'' )  invert and ;   \ all of them but those
\ (most actions on his feet: all but the off-screen ones)
on-obeying on-waiting or on-danger or on-command or on-footing or on-wait-over or on-100 or on-alert or on-praise-due or
    constant breaks-any
\ (off screen)
on-obeying on-waiting or on-danger or on-command or on-noise or on-story or on-wait-over or  constant breaks-away
: breaks-in? ( bit -- flag )  his-cmd @ dup nothing-breaks-in and if  2drop false  else  and 0<>  then ;
: shut-out ( -- )  his-cmd @ nothing-breaks-in or his-cmd ! ;          \ nothing breaks in for now
: let-in ( -- )  his-cmd @ nothing-breaks-in invert and his-cmd ! ;     \ again as it was
\ what alerts him (his-alert): nothing; the stalker in sight ahead (this frame); (2: never set);
\ the stalker in his room while she is chased
enum: alert-kind  no-alert sight-alert unused-alert chase-alert ;
