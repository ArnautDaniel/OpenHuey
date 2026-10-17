\ rumble.fs - the pad's rumble (the engine's src/game/rumble.c; the original's gRumble): who asks
\ shakes it on their own channel - the strongest ask of the moment is what is felt.
IN: rumble
USING: engine ;

\ the channels: Fiona's falls, knocks, struggles, slams; the scripts' own (0x93); a cutscene's;
\ the stalkers' blows landing near
enum: rumble-channel  fiona-channel story-channel scene-channel stalker-channel ;

: fiona-jolt ( strength frames -- )  fiona-channel -rot rumble ;
: stalker-jolt ( strength frames -- )  stalker-channel -rot rumble ;
