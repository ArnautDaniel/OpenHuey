\ messages.fs - every message the actors pass, with its arguments: the game's API. Each
\ subsystem's page (docs/subsystems/) says who sends and who takes them.
IN: messages
USING: actors ;

\ ---- acoustics (docs/subsystems/acoustics.md) ----
\ a noise this frame: at nav triangle `tri` (-1: at a door), or at `door` (-1: none)
message noise ( loud room tri door source -- )
message listen ( threshold source -- )        \ hear noises louder than this, not my own source's
message stop-listening ( -- )
message heard ( loud room tri door source -- )   \ what I heard this frame
message heard-nothing ( -- )
message noise-setting ( n -- )                \ 0..3: quiet noises quieter

\ the sources, in the order they are listened to
0 constant fiona-noise   1 constant hewie-noise   2 constant stalker-noise   3 constant world-noise
4 constant #noise-sources
