\ stalker/state.fs - a stalker's fields (docs/subsystems/stalker.md). The original keeps them in
\ its Pursuer object (src/game/pursuer.c; the offsets in the comments).
IN: stalker.state
USING: actors ;

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
end-state
