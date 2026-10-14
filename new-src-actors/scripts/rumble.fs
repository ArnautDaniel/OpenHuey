\ rumble.fs - the pad's rumble (the engine's src/game/rumble.c; the original's gRumble): who asks
\ shakes it on their own channel - the strongest ask of the moment is what is felt.
IN: rumble
USING: engine ;

0 constant fiona-channel     \ her falls, knocks, struggles, slams
1 constant story-channel     \ the scripts' own (0x93)
2 constant scene-channel     \ a cutscene's
3 constant stalker-channel   \ the stalkers' blows landing near

: fiona-jolt ( strength frames -- )  >r >r fiona-channel r> r> rumble ;
: stalker-jolt ( strength frames -- )  >r >r stalker-channel r> r> rumble ;
