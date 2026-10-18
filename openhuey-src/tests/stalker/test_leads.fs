\ What draws a stalker searching the room being played (stalker/leads.fs; Pursuer_PickTarget,
\ LocateTarget, ReactToEvent): a noise starts him (his cry, turned to it) and draws him to it -
\ his search's one stop; her noise draws him to her; a weaker lead than his is let be. Headless:
\   build/openhuey-src/openhuey --test openhuey-src/tests/stalker/test_leads.fs
IN: test-leads
USING: tester engine game-state actors messages room-names game debug stalker stalker.state stalker.senses stalker.search stalker.leads ;

: frames ( n -- )  0 ?do  game-tick  loop ;
: his ( xt -- x )  game:debilitas enter  execute  leave-actor ;
: her-tri ( -- tri )  s" fiona" actor-named body-tri ;
: her-y ( F: -- y )  s" fiona" actor-named body-pos fdrop fswap fdrop ;

free-play  front-garden-2 room!  10 frames
140.64e her-y -217.33e 0e tp-facing  2 frames
her-tri  -500e her-y -500e tp  2 frames      \ (her well out of his sight)
debug:debilitas-tri  2 frames                 \ (him on the spot she was on)
variable spot
testing a noise in his room (one of the world's): he starts - turned to it, a moment - and it is his search's one stop
: noise-at ( tri source -- )  >r >r $40 room-id r> -1 r> game:acoustics send noise ;
[: self body-tri ;] his spot !
T{ [: lead @ ;] his -> 0 }T
spot @ world-noise noise-at  3 frames
T{ [: lead @  lead-alarm @  start-t @ 0> ;] his -> 4 5 -1 }T
95 frames
T{ [: start-t @  route-end @  0 stop @ ;] his  spot @ = -> 0 1 -1 }T
testing a weaker lead than his (Hewie's noise after a noise): let be
spot @ hewie-noise noise-at  3 frames
T{ [: lead @ ;] his -> 4 }T
testing her noise (pulls harder): he starts, then goes for her (after her, mode 0)
spot @ fiona-noise noise-at  3 frames
T{ [: lead @  lead-alarm @ ;] his -> 5 6 }T
95 frames
T{ [: my-mode @ ;] his -> 0 }T
testing out of the game and in again: his leads forgotten
debug:debilitas-out  2 frames  spot @ debug:debilitas-tri  2 frames
T{ [: lead @  lead-alarm @ ;] his -> 0 0 }T
test-summary
