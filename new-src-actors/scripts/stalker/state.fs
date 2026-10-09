\ stalker/state.fs - a stalker's fields (docs/subsystems/stalker.md). The original keeps them in
\ its Pursuer object (src/game/pursuer.c; the offsets in the comments).
IN: stalker.state
USING: actors paths ;

state: stalker-state
  cell field my-model           \ its model (an Actor)
  cell field in-game            \ in the game (stalker-in .. stalker-out)
  cell field my-mode            \ +0x16C8: 0 after Fiona, 1 heading for her room, 2 searching it, 3 waiting about, 4 held off
  cell field hp  cell field hp-max
  cell field hard               \ the hard setting (progress +0x30 bit 0x8000: his numbers differ)
  cell field growl-t            \ +0x17EC: frames since his last growl
  cell field my-move-mode       \ +0xF8: how he moves (0 on his own)
  cell field fiona-id  cell field hewie-id
  cell field my-kind            \ +0x153C: which character (2 Debilitas, 3 Daniella: the cutscenes' cast id)
  cell field growls             \ his frame growls (Debilitas_Update; Daniella's doesn't)
  cell field loaded             \ in the story's character slot 2 (partner-load .. char-unload)
  \ his senses (Npc_Senses, Npc_WhoAround): how far Fiona and Hewie are on foot (-1: not in his
  \ room / no way; +0x1588, +0x158C), whom he sees or can reach (+0x1544, +0x1545), a noise
  \ heard this frame (+0x1546: its loudness, room, triangle), the frame's turn (+0x15A0), his
  \ sight (+0x1580 range, +0x1584 half its angle, +0x1574 the heading he looks along)
  1 floats field d-fiona  1 floats field d-hewie
  cell field sees-fiona  cell field sees-hewie
  cell field did-hear  cell field heard-loud  cell field heard-room  cell field heard-tri
  cell field sense-turn
  1 floats field view-range  1 floats field view-half  1 floats field view-heading
  cell field hear-threshold     \ (+0x14B8: noises louder than this)
  /path field probe-path        \ (a way planned only to measure it)
  32 field my-file              \ its model's files (a counted name: its sounds too, bank 7 while loaded)
  \ his moves (stalker/moving.fs): his way (+0x120..), what he plays (+0x1788), his step and
  \ what it is doing (the original's state function and its counters)
  /path field my-path
  cell field anim               \ +0x1788: the animation he plays (-1 none)
  cell field step               \ his step (an xt, run each frame: the original's Actor_SetState)
  cell field step-t             \ +0x1624: a count the step keeps
  cell field step-done          \ +0x16EE: the step is over
  cell field goal-tri  12 field goal-at      \ +0x15A4 / +0x15B0: where he heads in the room
  \ his mode's clock and his search (stalker/search.fs): the route's stops (+0x15E0: 8 of a
  \ triangle and whether it was picked at random), the next (+0x1620) and the end (+0x1621)
  cell field mode-t             \ +0x1660: frames left in his mode
  16 cells field stops  cell field route-next  cell field route-end
  cell field spot-next          \ +0x1738: the room's next search spot
  cell field stop-wait          \ +0x1798: frames to look about at a stop picked at random
  cell field route-done         \ +0x16F4: the route is over (his mode moves on)
  cell field search-phase       \ 0 none, 1 walking to a stop, 2 searching it
  \ his travel out of sight (stalker/travel.fs): the room he heads for (+0x1594), his route's
  \ doors (+0x138C, the next +0x1388, their number +0x1384), the way left to the next
  \ (+0x14C4), the exit he came in by (his door) and the one he makes for (+0x17B0), the doors
  \ that let him down (+0x148C), his search there (+0x17B4), a knock's wait
  cell field goal-room
  16 cells field doors  cell field doors-n  cell field door-i
  1 floats field to-door
  cell field came-by  cell field making-for
  13 cells field avoid
  cell field away-search
  cell field knock-t
  \ after her in her room (stalker/chase.fs, attack.fs): what he is doing in the chase (+0x175C:
  \ 1 standing, 3 turning, 5 walking at her, 6 stalking her, $13 a combo, $17.. a gesture, $1A
  \ stepping round her) and its step (an xt); how long he keeps to it (+0x162C) and before he
  \ moves on from watching her (+0x1630); frames in the chase (+0x1780); his combo (+0x1728,
  \ its step +0x172C), whether its blow has swung (+0x104: 1 swung, 2 after) and been sent,
  \ whom it struck (+0x1760: 1 Fiona), his cry for it (+0x1764), how long before he attacks
  \ again (+0x178C), frames held still by a blow landing (+0x14D0); her plight and panic as told;
  \ the way round her (+0x104, +0x1634)
  cell field chase-act  cell field act-xt
  cell field hold-t  cell field watch-t  cell field chase-t
  cell field combo  cell field combo-step  cell field swung  cell field sent  cell field took
  cell field cry  cell field hold-off  cell field freeze-t
  cell field her-plight  cell field her-stage
  cell field round-dir  1 floats field round-gone
  cell field gesture            \ the look about he plays at a stop (-1: none)
end-state
