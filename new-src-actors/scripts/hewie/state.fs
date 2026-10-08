\ hewie/state.fs - Hewie's state: one actor's fields (docs/subsystems/hewie.md). `hewie enter`
\ at the console shows them. Where he is is his body (the engine's body operations move it);
\ the comments give the original's offsets in Hewie (+0xF35..) where they help to compare.
IN: hewie.state
USING: actors paths ;

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
  cell field his-cmd            \ what may break in (bit 31: nothing) (+0xF356C)
  cell field his-pending        \ choose what to do next
  cell field his-look-to  cell field his-look-delay   \ where he is to look, after a delay
  cell field his-yelp     cell field his-yelp-anim
  \ his body as the others see it (the original's character fields)
  cell field his-hp             \ 0..100
  cell field his-cond           \ 0 well, 1 hurt, 2 down
  cell field his-mode           \ his move mode (+0xF8): 0 his own, 4 struck, 6 off screen, 8 by her, $C with her
  cell field his-sub            \ what he is doing, for the story (+0xFC)
  cell field his-away           \ out of the room being played (the character's +0x29)
  cell field his-busy           \ a script has him (+0xE0; H4)
  cell field his-done           \ the script's move is done (+0xE1; H4)
  /path field his-path
  \ how he feels and obeys
  cell field his-mood   cell field his-mood-time   \ 0 normal, 1 pleased, 2 upset, 3 angry
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
  cell field his-follows
  3 cells field his-skill       \ his skills after fetching (out of 32), the rolls and how they came out
  8 cells field his-rolls   3 cells field his-rolled   cell field his-roll-n
  \ what he notices
  cell field his-alert  cell field his-alert-was  cell field his-alert-what
  cell field his-held           \ Fiona was held
  cell field his-panic-seen     \ her panic's stages seen (1: 4, 2: 5)
  cell field his-scene-req
  cell field his-call           \ who called him ($FF none; 0 Fiona)
  cell field his-hits  cell field his-hit-t   \ how often Fiona hit him lately
  cell field his-hold-call
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
  12 field his-spot
  cell field his-wanted         \ how many times, or a wait
  \ where he looks (Hewie_TurnHead)
  cell field his-look-now  cell field his-look-t
  1 floats field his-look-pitch  1 floats field his-look-yaw   \ a look held (mode 8, glances)
  1 floats field his-head-pitch  1 floats field his-head-yaw   \ his head turned
  1 floats field his-yaw-was
  cell field his-look-char      \ an actor he looks at (-1 none)
  cell field his-look-pt?  12 field his-look-pt
  12 field his-scent
  \ where her commands put him
  1 floats field his-side  cell field his-side-dir
  cell field his-last-idle
  cell field his-cmd-was  cell field his-cmd-act
  \ a spot to go to: its triangle, an animation, the heading, the point
  cell field his-to-tri  cell field his-to-anim  1 floats field his-to-yaw  12 field his-to
  cell field his-pet
  cell field his-hurt-t         \ frames to his next health point
  cell field his-by  cell field his-how   \ who struck him last ($FF: a door), and how
  \ off screen
  cell field his-door           \ the exit he came in by (-1 none)
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
end-state
