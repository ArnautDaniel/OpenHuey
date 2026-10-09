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
  cell field doing              \ +0x1788: his action (0x1300, 0x1600...: the growl's exceptions)
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
end-state
