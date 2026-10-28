\ stalker/state.fs - a stalker's fields (docs/subsystems/stalker.md). The original keeps them in
\ its Pursuer object (src/game/pursuer.c; the offsets in the comments).
IN: stalker.state
USING: actors paths vectors ;

state: stalker-state
  cell field kit                \ his tables (stalker/tables.fs: debilitas-kit, daniella-kit)
  \ his own ways and numbers (his vtable): the Pursuer's chase (Pursuer_ChaseDecision /
  \ BehaviourStalk: Daniella) or Debilitas's own (Debilitas_Behaviour), giving up after enough
  \ damage (Debilitas_GivesUp; the Pursuer's never), his turn a frame (+0xA0), his reach in
  \ the ground he gains (+0x2F0: Debilitas 16, Daniella 12)
  cell field base-chase  cell field gives-up  1 floats field turn-k  1 floats field reach-k
  cell field give-up-hits       \ the damage he gives up at (DEBILITAS_GIVE_UP_HITS: Debilitas 20, kind 6 80)
  \ his setup's times (frames): after her (+0x16D4, Pursuer_Caught: his mode's clock), down when
  \ knocked out (+0x16D0, Pursuer_NoGiveUp)
  cell field after-frames  cell field down-frames
  cell field my-model           \ its model (an Actor)
  cell field in-game            \ in the game (stalker-in .. stalker-out)
  cell field my-mode            \ +0x16C8: stalker-mode (messages.fs: after her, heading for her room, searching it, waiting about, held off)
  cell field my-floor           \ the floor he may walk (his kind's mask; 0 while the story's 0x20 has him through anything)
  cell field drawn-tri          \ the doll's triangle he is drawn to (its search stop; -1 none)
  cell field hp  cell field hp-max
  cell field hard               \ the hard setting (progress +0x30 bit 0x8000: his numbers differ)
  cell field growl-t            \ +0x17EC: frames since his last growl
  cell field my-move-mode       \ +0xF8: how he moves (hewie-names' move modes: free-mode; unreachable-mode on a ladder)
  cell field fiona-cached  cell field hewie-cached   \ (their ids, looked up as he starts)
  cell field my-kind            \ +0x153C: which character (2 Debilitas, 3 Daniella: the cutscenes' cast id)
  cell field growls             \ his frame growls (Debilitas_Update; Daniella's doesn't)
  cell field loaded             \ in one of the story's character slots (partner-load / char-load .. char-unload)
  cell field my-slot            \ ... which: 2 the one in play (his sounds in bank 7, saved), 3..5 a scene's figure
  \ his senses (Npc_Senses, Npc_WhoAround): how far Fiona and Hewie are on foot (-1: not in his
  \ room / no way; +0x1588, +0x158C), whom he sees or can reach (+0x1544, +0x1545), a noise
  \ heard this frame (+0x1546: its loudness, room, triangle), the frame's turn (+0x15A0), his
  \ sight (+0x1580 range, +0x1584 half its angle, +0x1574 the heading he looks along)
  1 floats field d-fiona  1 floats field d-hewie
  cell field sees-fiona  cell field sees-hewie
  cell field did-hear  cell field heard-loud  cell field heard-room  cell field heard-tri
  cell field heard-source       \ (whose: fiona-noise, hewie-noise, world-noise)
  \ his lead (Pursuer_PickTarget: +0x16C9 its pull - 5 Fiona, 4 a noise, 1 Hewie, 0 none - and
  \ +0x16CA how alarming it was - 7 seeing her, 6 hearing her, 5 a noise, 2 seeing Hewie, 1 hearing
  \ him); his start at a more alarming one (Pursuer_ReactToEvent): its frames, where, then what
  cell field lead  cell field lead-alarm
  cell field start-t  /vec field start-at  cell field start-then
  cell field sense-turn
  1 floats field view-range  1 floats field view-half  1 floats field view-heading
  cell field hear-threshold     \ (+0x14B8: noises louder than this)
  /path field probe-path        \ (a way planned only to measure it)
  32 field my-file              \ its model's files (a counted name: its sounds too, bank 7 while loaded)
  2 cells field my-motions      \ its model's motion table (its name)
  cell field figure-worn        \ as a scene's figure: its kind's other model worn (stalker.fs wear-figure)
  cell field play-model         \ ... and meanwhile his own, put by
  \ his moves (stalker/moving.fs): his way (+0x120..), what he plays (+0x1788), his step and
  \ what it is doing (the original's state function and its counters)
  /path field my-path
  cell field anim               \ +0x1788: the animation he plays (-1 none)
  cell field step               \ his step (an xt, run each frame: the original's Actor_SetState)
  cell field step-t             \ +0x1624: a count the step keeps
  cell field step-done          \ +0x16EE: the step is over
  cell field goal-tri  /vec field goal-at      \ +0x15A4 / +0x15B0: where he heads in the room
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
  \ pursuer-act, stalker/names.fs) and its step (an xt); how long he keeps to it (+0x162C) and before he
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
  cell field her-unseen          \ she can't be seen: item $89 worn, still (Fiona +0x1AD630; fiona-unseen)
  /vec field strike-pt  \ where a blow from no one came from (blow-from: striker -2)
  cell field noclipped           \ through blocked floor (the story's 0x20: +0x2B) - no reach test by distance
  cell field look-mode           \ where his head looks (+0x1710: head-look, stalker/names.fs)
  1 floats field head-yaw  1 floats field head-pitch   \ his head's turn and tilt (the model's +0x858 / +0x854)
  cell field head-bone  cell field neck-bone            \ (+0x8B0 / +0x8B4; -1 none)
  cell field her-ladder-on      \ the ladder she is on (the room's nav link; -1 none: `fiona-plight`)
  cell field at-ladder          \ the ladder he makes for or climbs (stalker/ladder.fs; -1 none)
  cell field no-way             \ his last way to her planned none (+0x1590 -1: Pursuer_ChaseTarget)
  cell field ladder-side        \ the end he gets on at (+0x104: 0 its foot, going up; 1 its top, going down)
  cell field ladder-offsets     \ out from a ladder's ends: his family's (vtable +0x9C)
  cell field round-dir  1 floats field round-gone
  \ taking her (stalker/grab.fs): leading her (his mode 8 while she's his; 1 asked, -1 holding), how (6 by the hand,
  \ 8 walking), her answer (0 none yet, 1 taken, 2 refused, 3 broke free), his taunts so far
  cell field leading  cell field lead-type  cell field answer  cell field taunts
  \ struck (stalker/struck.fs): his condition (+0x14CC: well-cond, hurt-cond - stumbled, down-cond), how
  \ long he's hurt (+0x1790) and down (+0x1664), the damage he's taken since he last gave up
  \ (+0x16C4), reeling from a blow (his reactions run; his mode waits), struck while down, her
  \ blow (+0x1761 1: his counter at his flinch's key), his growl after a stumble (+0x16F5),
  \ unshaken by a shove in this move (+0x16F7), the fall's lying pose and its stage, times down
  \ for good (progress +0xFBC)
  cell field cond  cell field hurt-t  cell field down-t  cell field hits
  cell field reeling  cell field twitch  cell field by-her  cell field growl-after  cell field unshaken
  cell field lie-anim  cell field down-stage  cell field knockouts  cell field striker
  cell field gesture            \ the look about he plays at a stop (-1: none)
  cell field taunt-gesture      \ +0x104: his taunt's row of his gesture table (Pursuer_PickFromTable $17..$19)
  \ for the summoner (summoner.fs): his time away's base (+0x16E0, frames), his cooldown (+0x16E4,
  \ seconds: vtable +0x2D4), the rooms he keeps to (vtable +0x314: two, -1 none)
  cell field away-base  cell field cool-secs  cell field own-a  cell field own-b
  \ his own rooms' spots (the +0x314 list's triangles: where he rests in them); his footsteps
  \ heard through the walls (+0x1530 how far closed in since the last, +0x1534 / +0x1538 his
  \ way left now and before, +0x16A4 his foot)
  cell field own-a-tri  cell field own-b-tri
  1 floats field step-acc  1 floats field step-last  cell field step-foot
  cell field foot-l-was  cell field foot-r-was   \ his feet down last frame (+0x16A8 / +0x16A9)
  \ the doors of the room being played (stalker/doorway.fs): on his way out by an exit (+0x17B0:
  \ making-for; the room's modes wait meanwhile), his animation's kind at its door (+0x104: 0 / 2
  \ opening it from behind / in front; -1 none), the door asked for and the one held, frames
  \ waited at one held shut; a door slammed on him - the exit (-1 none: staggered back), the
  \ heading he is pushed along; frames stunned off screen after being knocked out of the room
  \ (+0x1664: 150); the exit Fiona came in by (the half of the room she is on)
  cell field wandering-out      \ out by an exit at random (his search over): sight or a lead turns him back
  cell field for-her            \ that door only opened to get at her across his room (action $B: stalker/doorway.fs)
  cell field out-going  cell field door-use  cell field asked  cell field held-door  cell field door-wait
  cell field push-exit  1 floats field push-way  cell field stun-t
  cell field her-came
  cell field own-mark           \ ( who -- ) his blow's mark on the one it struck, as they cry out (0 none)
  \ the door he came into her room by, to shut behind him once clear of it (-1 none), and his
  \ way with such doors (stalker/doorway.fs `shut-behind`: his DoorBreak)
  cell field to-shut  cell field door-manner
  1 floats field spot-yaw        \ the heading he lines up to at a door
  \ Hewie (stalker/bitten.fs; Pursuer_StateHewieBites / HewieHolds, P3c): his temper with Hewie
  \ (+0x16BC, to 1000) and how much he stands (+0x16DC), the throw's chance a hold (+0x16E8);
  \ Hewie on him - the attack's kind (10..15; 0 none: +0x104), its animation (+0x1624), how it
  \ goes (+0x1628: 0 held, 1 throwing him off, 2 let go), the holds so far (+0x1634), the heading
  \ he turns to and by how much a frame; after Hewie (his temper up): frames left for it (+0x1630,
  \ from +0x16D8 `dog-limit`), and closing in, attacking or taunting
  cell field temper  cell field tolerance  1 floats field throw-k
  cell field bitten  cell field bite-anim  cell field bite-how  1 floats field holds
  1 floats field bite-yaw  1 floats field bite-turn
  cell field dog-t  cell field dog-limit  cell field dog-phase
  cell field dog-sent            \ his blow this move has struck Hewie (stalker/attack.fs)
  cell field at-dog              \ his combo is at Hewie (after him, his temper up), not her
  \ Hewie as he tells it (hewie-doing): his condition (2 down); the chance roll that calms his temper
  \ with Hewie (Pursuer_ChanceRoll): what has built up on it (+0x16C0), his blow struck Hewie in this
  \ attack (+0x1760 bit 1)
  cell field dog-cond  1 floats field roll-build  cell field struck-dog
  \ what he tells besides (stalker-ties): a script of the room has him (`scripted`), a scene does
  \ (`scene`) - his +0xE0; out of contact (+0x2A: through a door, on a ladder); frames a joint
  \ action he took stays pending (Progress_IsLinked: its entry settled the next frame)
  cell field script-held  cell field in-scene  cell field apart  cell field link-t
  cell field in-cast            \ a scene has taken him (+0xE2: Cutscene_Cast .. ReleaseActors - his own frame waits)
  cell field raging              \ +0x16B8 2: in a rage (the story's 0x31) - his stance and walk
  \ his body set by his feet on a slope: the points back and ahead along his step - slowly walking
  \ ($201: +0x1694 / +0x169C), else (+0x1698 / +0x16A0) - his Setup's
  1 floats field feet-slow-back  1 floats field feet-slow-front  1 floats field feet-back  1 floats field feet-front
  cell field chaired            \ Lorenzo in his chair (O_LRO: LorenzoModel - tilted by the floor, no feet planted)
  cell field hushed              \ +0x2C: his own sounds don't play (the story's 0x3D)
  \ (P4c) a stalker's own: his chase (its start and its step: Riccardo's; 0 the family's), his
  \ own actions ($1000.. in his tables: a table of xts; 0 Debilitas's), how near he attacks
  cell field own-chase  cell field own-run  cell field own-acts  1 floats field attack-range
  cell field own-frame          \ his own each frame (Riccardo's rage, snort, impact)
  cell field alt-tables         \ his tables' second set (kind 37 alerted; else his rage)
  cell field tint-t  cell field tint-was  cell field unseen   \ (the TintStalker: shown a while,
  1 floats field tint-alpha  cell field tint-fade  cell field tint-shown   \ how faded: its tint;
  1 floats field tint-phase                                               \ kind 37's: its pulse)
  cell field mark-id            \ (kind 12) its marker's effect actor (-1 none)
  \ the story's zones a blow may strike instead of her (Events_NearestPoint: their kind's bit 1),
  \ this frame: on, and their centres; Riccardo's shot at one shown as his point for a while
  32 constant #targets
  #targets cells field target-on   #targets 3 * floats field target-at   cell field shot-t
  cell field comings             \ +0x1630: the times he comes on, seen by her, before backing off
  cell field blows  cell field blow-move  cell field blow-aim   \ (Riccardo's flurry: blows left, the move, his aim)
  \ (Lorenzo's other class: under the floor - frames left, where he comes up, how; whom his sweep struck)
  cell field under-t  /vec field rise-at  cell field rise-tri  cell field rise-anim  cell field rise-xt  cell field swept
  \ the mini-game's run (Hewie played): frames he waits on coming into the room played
  \ (Pursuer_ThroughDoorEnding's +0x17B4)
  cell field run-pause
  \ ... gone after Hewie instead (his room, the one played: Pursuer_Modes' mode 3 out of sight)
  cell field hunting-dog
  \ the story's moves (stalker/scripted.fs; Pursuer_EventCommand): posed in a held animation (the
  \ scripts' 0x9D: +0xE2 - his model alone moves), the move given (+0xF4 its kind, +0x104 its two
  \ arguments, +0x110 its point, +0x10C its heading), done (+0xE1), whom it last named (+0x100:
  \ 0 Fiona, 1 Hewie), the walk it plays (+0x104 [1]), hidden (+0x29: the scripts' 0x1F); his
  \ scripted look (+0x16F9) and its point (+0x1700); the story's door use under way (moves 3 / 4)
  cell field posed  cell field sm-kind  cell field sm-a  cell field sm-b  /vec field sm-at  1 floats field sm-yaw
  cell field sm-done  cell field sm-who  cell field sm-walk  cell field hidden
  cell field look-on  /vec field look-pt
  cell field door-script
  cell field take-asked         \ the story's action for him asked (+0x14E8 5: taken at his next frame, if in the room played)
  \ coming into her room by a shut door: its hold asked (Pursuer_EnterRoom's DoorHold_Take); refused, he
  \ barges it open (action $C, Pursuer_DoorBarge) and then walks in
  cell field entering  cell field barging
  cell field still-held         \ held still while she reads on the dimmed screen (OpenHuey's, not the original's)
  \ his own across frames (were shared variables): the powder's loops left and as it struck (+0x1624);
  \ the lead he started at (stalker/leads.fs); the spot 12 inside the door he comes in by and its
  \ triangle (stalker/travel.fs: walked to once the door stands open)
  cell field powder-loops  cell field powder-how  cell field go-pull
  cell field in-tri  /vec field in-at
  cell field dog-trust          \ Hewie's trust as he tells it (+0xF35CC: how long he keeps a stalker busy)
end-state
