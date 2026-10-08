\ flag-names.fs - the game's state flags by what they do (the progress' 46 flags, +0x8). Use
\ the names, never the numbers. Each says what the original's code does with it (src/game/,
\ where it is tested / set); a "?" marks a meaning only partly known - rename when it's clear.
\ (Story flags - the scenario's, +0x1C - are named as they are worked out: story-flag-names.)
IN: flag-names

\ ---- the summoner (Summoner_Offstage / Summoner_Noise: the stalker brought in from off stage) ----
$0  constant summoner-on          \ the summoner may bring the stalker in at all
$1  constant summoner-timing      \ its delay has been rolled (set once the countdown starts)
$2  constant summoner-waiting     \ the countdown holds; cleared when it brings him in
\ ---- the game scene (scene_game.c SceneGame_SubPlay, StateEntry) ----
$3  constant new-game-sounds      \ the new game's sound set (set by the new-game entry; Progress_LoadSoundSet)
$4  constant subscreen-wanted     \ open the sub screen (the menu) - the scripts' 0x6D, the button
$5  constant subscreen-locked     \ the button doesn't open the sub screen
$6  constant pause-wanted         \ open the pause screen (Start)
$8  constant world-held           \ a movie or transition holds the world: the room and characters
                                  \ aren't drawn, their sounds are held, no hits (cleared by the
                                  \ scenes' words as they end)
$C  constant caught               \ Fiona caught, dragged off, or the game over's black: the game ends
$F  constant panic-held           \ the panic doesn't update, no screen fade
$12 constant scene-locked         \ no scene changes (the scripts' 0x0A / 0xA5) and her buttons aren't read
$16 constant creatures-on         \ the rooms' creatures are placed (the scripts' 0x99)
$17 constant world-frozen         \ creatures and things don't update, the characters aren't drawn (but see 0x24)
$19 constant no-pause             \ Start doesn't pause
$1A constant movie-skipped        \ the movie was skipped (the movie pause's skip)
$1C constant quit-wanted          \ quitting / saving from the pause screen
$24 constant draw-when-frozen     \ the characters are drawn even while world-frozen
$27 constant keep-room-sounds     \ entering a room doesn't reload its sounds
$28 constant in-play              \ play has started (set at the game's entry; the glow, the dice)
$29 constant movie-opaque         \ the playing movie is drawn opaque (its button toggles it)
$2A constant movie-flag-2a        \ ? the movie's per-frame update (func_0021C840) is held
$2D constant room-flag-2d         \ ? cleared as each room is entered; nothing tests it here
\ ---- the danger (SceneGame_Danger) ----
$7  constant force-followed       \ the danger is "followed" (1)
$1B constant force-calm           \ the danger is calm (0) - before the others
$1F constant force-chased         \ the danger is "chased" (2)
$22 constant hunted               \ she is hunted: calm turns to followed, followed lasts; cleared by phase 5
\ ---- the stalkers ----
$9  constant fiona-hidden         \ she's hidden: stalkers don't see or reach her (cleared by her full stop)
$A  constant fiona-half-hidden    \ partly hidden: reached at 10, not 20 (cleared with fiona-hidden)
$E  constant stalkers-blind       \ the stalkers don't notice her or raise the threat
$18 constant stalkers-stay        \ stalkers and creatures in other rooms don't travel to hers
$20 constant traps-off            \ the placed things (traps) don't arm or make noise
$21 constant stalker-no-fear      \ the stalker in her room doesn't frighten her
\ ---- Fiona ----
$14 constant no-flee              \ she can't flee, and some requests (0x1A kicks, 0x22) are refused
$15 constant panic-can-max        \ her panic may reach 100
$1E constant no-running           \ the run button does nothing
$25 constant plain-commands       \ her commands to Hewie get plain lines, no gesture look-around
$2B constant fiona-occupied       \ at a door, led, dragged, reacting: no gestures (cleared at her idle)
$2C constant capture-no-end       \ being dragged off doesn't end the game
\ ---- Hewie ----
$B  constant hewie-hidden         \ stalkers don't see him (cleared by his full stop)
$D  constant hewie-commandable    \ he's hers to command: her gestures are read
$11 constant hewie-timid          \ he goes for the stalker only from behind (cleared from trust 2)
$13 constant hewie-no-attack      \ he doesn't attack (with fiona-occupied too)
$1D constant hewie-no-dodge       \ he doesn't dare to dodge (cleared from trust 3)
\ ---- the room scripts (and only they) ----
$26 constant events-held          \ only the scripts' phase 3 runs (and a shared script for arrivals)
$10 constant scene-5-pending      \ ? some rooms' phase 2: with a scene request pending, change to scene 5 (an ending / results)
$23 constant no-stalker-camera    \ ? the shared script doesn't turn the camera on the stalker while she is half hidden
