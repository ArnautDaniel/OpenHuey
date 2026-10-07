#!/usr/bin/env python3
"""The event script opcodes: one entry per command (0x00-0xDA) and condition (0x00-0x65).

    tools/event_opcodes.py           write docs/event_opcodes.md from this table
    tools/event_opcodes.py --check   check the table against the length tables

The reference for reading the rooms' scripts (tools/evdis.py) and for converting them. Read
from src/game/event.c (Event_RunScript, EventCmd_Run and its sub-dispatchers, EventCond_Eval).

Each entry: (name, operands, description). `operands` lists the bytes after the opcode, all
big-endian, as `name:type`:
    u8 s8 u16 s16 u32 s32   integers
    fx      s32 in 1/1000 (a world coordinate or a distance)
    deg     s16 degrees
    chr     u8 a character's script id (see below); 0xFF where noted: the script's own
    slot    u8 a character slot (0 player, 1 partner, 2.. stalkers / others)
    var     u8 a script variable (0..31, the event's +0x810)
    flag    u16 a progress flag number
    str     a string: its length is the byte before it (see 0x22 / 0x33)
    pad     an unused byte
`_` as a name means the byte isn't read. The operand list may be shorter than the command
(the rest unread or unknown); --check reports that.

Script context:
 - Phase scripts (entering, phases 1..5) run once per call to the end; they can't wait.
 - Action scripts run per character in one of 17 slots (+0x564); they can wait (the script
   resumes next frame) and the "self" commands below act on the slot's character.
 - Character ids: below 0xF0 a character's script id (the progress' character table);
   0xF0 the player's slot; 0xF1..0xFA script slots 7..16 (no character: scene scripts).

Commands marked "self" are handled by Event_RunScript (the action-script runner) for the
script's own character; in a phase script they reach EventCmd_Run, which ignores them.
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# ---- commands -------------------------------------------------------------------------------

CMD = {
    0x00: ('exit-check', 'exit:u8',
           'Door / exit use: with bit 7 set, flags the progress (+0x4 = 1) and keeps the exit '
           '(+0x702); otherwise, if the exit is not locked and passable for whoever is controlled '
           '(Fiona idle or walking; Hewie idle), +0x702 = progress +0x10 (empty in this game: 0).'),
    0x01: ('nop-progress-14', 'a:u8', 'Calls the progress\' +0x14 with the byte: empty in this game (no effect).'),
    0x02: ('char-to-tri', 'who:chr tri:u16', 'Places character `who` (0xFF: self) on nav triangle `tri` in the event\'s room.'),
    0x03: ('message', 'msg:u16',
           'Opens message window text `msg` (bit 0x4000: the second language table); the window '
           'belongs to the script\'s character (+0x80C). See 0x09 to wait for it.'),
    0x04: ('char-to-exit', 'who:chr exit:u8', 'Places character `who` (0xFF: self) at the outside point of exit `exit` (Rooms_ExitPointOut).'),
    0x05: ('action', 'mode:u8 who:chr act:u8',
           'Starts action script `act` for `who`. For script slots (0xF0..0xFA) only if the slot is '
           'free; for a character, only if it is not already in a scripted state (character state '
           '5, script action `act`). 0x92 is the forcing variant. `mode` is read by 0x92 only.'),
    0x06: ('self-idle-or-end', '',
           'self: waits until the character is idle again; with no character (id -1) it ends this '
           'action script (the slot is freed). Always waits a frame.'),
    0x07: ('self-anim', 'anim:u16', 'self: plays animation `anim` (character move 7). (Not "walk to triangle".)'),
    0x08: ('self-wait-anim', '', 'self: waits until the move has finished (move state 0 and its motion\'s end flag 0x20).'),
    0x09: ('wait-message', '', 'Waits while the message window is open.'),
    0x0A: ('scene-change', 'scene:u8 arg:u8 kind:u8',
           'Requests a scene change (progress +0x1134 = scene, +0x113C = arg, +0x1151 = kind) when '
           'the situation allows it (not while panicking for most kinds; kinds 0 / 3 may set the '
           'pending ending instead). Scene 5 is refused while Fiona is busy.'),
    0x0B: ('camera-value', 'which:u8 v:fx', 'Camera +0xB4 (which 0) or +0xB0 (else) with `v` (a camera distance / height).'),
    0x0C: ('self-move-to', 'tri:u16 x:fx z:fx face:deg tri2:u16 move:u8',
           'self: goes to (x, z) on triangle `tri` (tri2 0xFFFF: none), then faces `face`; `move` '
           'the character move (5 / 10 walk / run to a point, ...).'),
    0x0D: ('self-wait-done', '', 'self: waits until the current move is done (+0xE1 == 1).'),
    0x0E: ('self-move-tri', 'tri:u16 tri2:u16 move:u8', 'self: move `move` with triangles `tri` / `tri2` (0xFFFF: none) (6 / 11 walk / run to a triangle, 17 ...).'),
    0x0F: ('self-idle', '', 'self: back to idle (character move 2).'),
    0x10: ('self-move-slot', 'slot:u8 move:u8', 'self: move `move` toward character slot `slot` (14 turn to a character, ...).'),
    0x11: ('partner-load', 'id:u8', 'Brings in character `id` as the partner (slot 2: loads and starts it, resets the summoner).'),
    0x12: ('char-load', 'id:u8 slot:u8 exit:u8',
           'Brings in character `id` in slot `slot`; placed as is (exit 0xFF) or with the motions '
           'for coming in from the room behind `exit` of this room (Rooms_OtherRoom).'),
    0x13: ('char-unload', 'slot:u8', 'Takes out the character in `slot` (waits while it is still loading).'),
    0x14: ('char-done', 'id:u8', 'Character `id` leaves the scene (the partner slot: the summoner takes it).'),
    0x15: ('char-activate', 'id:u8', 'Activates character `id` (Progress_ActivateChar).'),
    0x16: ('counter-set', 'n:u8', 'The event counter (+0x703) = n.'),
    0x17: ('counter-inc', '', 'The event counter + 1.'),
    0x18: ('wait-counter', 'n:u8', 'Waits until the event counter is n.'),
    0x19: ('self-wait-frames', 'n:u16', 'self: waits until this script\'s frame count (+0x14) is n.'),
    0x1A: ('self-frames-reset', '', 'self: this script\'s frame count = 0.'),
    0x1B: ('yield', '', 'self: waits one frame (the pc moves past it first).'),
    0x1C: ('self-wait-16', '', 'self: waits until this script\'s frame count is 16.'),
    0x1D: ('nav-group', 'set:u8 group:u8 bits:u32',
           'Nav-triangle flag groups (gRoomEventObj, NavGroups: the room\'s section 14): set 1 sets '
           '`bits` on group `group`\'s triangles, else clears them. Flags in a character\'s mask block '
           'it (Fiona 0x28020018, Hewie 0x29020008).'),
    0x1E: ('self-to-exit-in', 'exit:u8', 'self: put at the inside point of exit `exit` (Rooms_ExitPointIn), facing the way through it.'),
    0x1F: ('char-visible', 'who:chr on:u8', 'Character `who` shown or hidden (+0x29), if it is in the current room.'),
    0x20: ('self-noclip', 'on:u8', 'self: the character\'s root motion ignores the nav blocking mask (+0x2B) - it walks through blocked triangles (stalkers use it at doors).'),
    0x21: ('self-scripted', 'on:u8',
           'self: the character\'s +0x2D; turning it on for Fiona while her script state is 4 '
           'resets that state.'),
    0x22: ('room-command', 'cmd:u8 len:u8 args:str',
           'The room\'s own command `cmd` (room handler +0x28: RoomXX_Command / CmdTable) with the '
           'script\'s character and the command bytes. Returns: bit 1 wait, bit 0 go on, else the '
           'room command moved the pc itself.'),
    0x23: ('loop-mark', '', 'Marks the loop point (just after this command) for 0x24.'),
    0x24: ('loop-back', '', 'Goes back to the loop point (0x23). Ends a straight-line read of a script.'),
    0x25: ('goto-script', 'id:u8', 'Continues in script `id` (room action script, or built-in at 0x80..), loop and return points cleared.'),
    0x26: ('var-set', 'var:var v:s32', 'Script variable `var` = v.'),
    0x27: ('chars-area-camera', 'area:u8 a:s8 b:s8 rel:s8',
           'Each character whose relation to area `area` is `rel`: its camera setup (+0xE8 / +0xEC) = a, b.'),
    0x28: ('camera-follow', 'id:u8', 'The camera follows character `id` (0xFF: nobody).'),
    0x29: ('area-camera', 'area:u8 a:s8 b:s8', 'The characters in this room inside area `area`: camera setup (+0xE8 / +0xEC) = a, b.'),
    0x2A: ('stalker-search-delay', 'id:u8 frames:s32', 'Stalker `id`: frames it waits (+0x1660, default 900 = 15 s) before it searches or comes after Fiona when she is in another room.'),
    0x2B: ('self-look-at', 'id:u8', 'self: looks at character `id` (character move 12; 0xFF: stop). (Not "follow".)'),
    0x2C: ('call-script', 'id:u8', 'Calls script `id`; 0x2D returns to the command after this.'),
    0x2D: ('return', '', 'Returns from 0x2C.'),
    0x2E: ('stalker-route', 'id:u8 point:u16', 'Stalker `id`: adds `point` to its route (0xFFFF: on to its next route point).'),
    0x2F: ('stalker-mode', 'id:u8 mode:u8', 'Stalker `id`: 0 go for Fiona, 2 chase her from here, 3 start searching.'),
    0x30: ('stalker-knock-down', 'id:u8', 'Stalker `id` is knocked down.'),
    0x31: ('stalker-rage', 'id:u8 on:u8', 'Stalker `id`\'s rage on / off (virtual +0x31C: Pursuer_SetRage, +0x16B8 = 2 / 0).'),
    0x32: ('sound-volume', 'ch:u8 vol:u16', 'Sound channel `ch`\'s volume (sound +0x7C).'),
    0x33: ('effect-string', 'fx:u8 len:u8 text:str', 'Sends room effect `fx` the string (if not empty).'),
    0x34: ('renderer-flag-304C04', 'on:u8', 'The renderer\'s display setting +0x304C04 on / off; nothing in the game reads it (unused by the scripts).'),
    0x35: ('butterflies', 'fx:u8 args:u8',
           'Room effect slot `fx` (under 0x20) made anew as a butterflies effect (argument 8), then '
           'sent the command\'s 7 bytes from `args` on, as they are.'),
    0x36: ('char-camera', 'id:u8 a:s8 b:s8', 'Character `id`\'s camera setup (Progress_CameraSetup) a, b.'),
    0x37: ('var-inc', 'var:var', 'Script variable + 1.'),
    0x38: ('var-dec', 'var:var', 'Script variable - 1.'),
    0x39: ('hewie-action', 'a:u32 b:u32', 'Hewie: action a with argument b (Hewie_SetAction).'),
    0x3A: ('nop-progress-18', 'a:u16', 'Calls the progress\' +0x18: empty in this game (no effect).'),
    0x3B: ('char-to-tri-facing', 'who:chr tri:u16 face:deg', 'Places character `who` (0xFF: self) on triangle `tri` facing `face`.'),
    0x3C: ('nop-progress-24', 'id:u8 v:u16 b:u8', 'Calls the progress\' +0x24 for character `id`: empty in this game (no effect).'),
    0x3D: ('char-silent', 'who:chr on:u8', 'Character `who` silent (+0x2C): its own sounds (Actor_PlaySound) don\'t play.'),
    0x3E: ('stalker-to-room', 'id:u8 room:u16 at:s16 how:u8',
           'Stalker `id` leaves the scene and is put into room `room` (0xFFFF: its own) at `at`, '
           'entering as `how` (0..2) (its virtual +0x64).'),
    0x3F: ('hewie-to-room', 'room:u16 how:u8 at:s16', 'Hewie into room `room` at `at`, entering as `how` (his +0x64).'),
    0x40: ('char-remove', 'slot:u8 how:u8', 'The script slot of character slot `slot` is cleared and the character removed (Progress_RemoveChar(slot, how)).'),
    0x41: ('pvar-inc', 'n:u8', 'Progress variable n + 1 (the progress\' byte variables, +0x9C).'),
    0x42: ('self-turn-to', 'id:u8', 'self: turns to character `id` (character move 14).'),
    0x43: ('threat-raise', 'v:u8', 'Raises the threat / panic meter (progress +0x7B8) by v (0..100).'),
    0x44: ('nop-progress-6C', '', 'Calls the progress\' +0x6C: empty in this game (no effect).'),
    0x45: ('char-sound', 'who:chr id:u32 bank:u8', 'Character `who` (0xFF: self) plays sound `id` of bank `bank` where it stands (Actor_PlaySound; banks: 4 the sound set, 5 common, 6 the room\'s).'),
    0x46: ('doors-room-in', '', 'The doors redo their setup for the current room (Doors_RoomIn).'),
    0x47: ('char-set-C4', 'who:chr v:s32', 'Character `who`: +0xC4 = v (a stalker\'s presence state: 1 / 2 seen / near ...?).'),
    0x48: ('char-full-health', 'who:chr', 'Character `who`\'s health back to its maximum (+0x14C8 = +0x14CC).'),
    0x49: ('action-end', 'who:chr',
           'Ends character `who`\'s action script (script slots 0xF0..0xFA: the slot is freed); a '
           'character in a scripted move is released (move 1).'),
    0x4A: ('camera-restart', '', 'The camera director restarts (CamDirector_Restart).'),
    0x4B: ('self-turn-angle', 'face:deg', 'self: turns to heading `face` (character move 15).'),
    0x4C: ('door-bits', 'a:u8 set:u8 b:u8', 'The room\'s door models: bit a + 1 + b of each set (`set` 1) or cleared (Doors_SetBits(set, a, b): which parts are drawn; the progress\' +0x68 is empty).'),
    0x4D: ('door-copy', 'door:u16 from:u16',
           'Door `door` takes on door `from`\'s states: open bit, lock, closed-off; and `from`\'s '
           'exit in this room saves its door state.'),
    0x4E: ('fiona-calm-reset', '', 'Fiona\'s fear (+0x1AD5F4, 0..100; over 90 she panics) and her exhaustion count (+0x1AD5F8, up to 1800 frames) reset to 0.'),
    0x4F: ('fiona-recover', '', 'Fiona recovers (Fiona_ResetRecovery).'),
    0x50: ('placed-object', 'op:u8 obj:u8 v:u8',
           'A placed object of the room (the room\'s object name `obj`, RoomXX_ObjectNames): see the 0x50 sub-commands.'),
    0x51: ('message-close', 'msg:u16', 'Closes the message window if it shows `msg` (0xFFFF: any).'),
    0x52: ('movie-loop', '', 'The playing movie loops (Movie.loop = 1).'),
    0x53: ('obstacle-place', 'i:u8 n:u8 kind:u8 a:u16 b:u16',
           'Pushable obstacle `i` (model "oshi0n", kind `kind`) placed on squares a, b (Obstacles_PlaceAt).'),
    0x54: ('obstacle-stop', 'i:u8', 'Pushable obstacle `i` can no longer move (Obstacles_Stop).'),
    0x55: ('fade-colour', 'rgba:u32 layer:u8', 'The screen fade colour `rgba`, drawn in renderer layer `layer` (Events_Fade).'),
    0x56: ('self-door-knock', 'door:u8 how:u8',
           'self (the player for script slots): knocks on / tries door `door` (sound 0x27 for how 1, else 0x28), if it isn\'t open.'),
    0x57: ('ebit-set', 'n:u8', 'Event bit n set (+0x890, cleared when the room changes).'),
    0x58: ('ebit-clear', 'n:u8', 'Event bit n cleared.'),
    0x59: ('flags', 'op:u8 n:u16', 'Flags, items and doors: see the 0x59 sub-commands.'),
    0x5A: ('item-cooldown', 'item:u16 less:u8', 'Item `item`\'s summon cooldown: set (less 0) or shortened (Summoner).'),
    0x5B: ('stalker-item-cooldown', '', 'The stalker\'s item (+0x2D4) gets its cooldown set.'),
    0x5C: ('fade', 'frames:u8 kind:u8',
           'A screen fade over `frames` frames; kind & 0xF: 1 in, 4 out; bits 0xC0 fade the music '
           'with it (0x80 at once, 0x40 over 90 frames); bits 0x30 ramp the volume (0x20 at once, '
           '0x10 over the fade). See 0x5F to wait.'),
    0x5D: ('fade-finish', '', 'Finishes the running fade now; marks a scene as playing (+0x11F3).'),
    0x5E: ('fade-over', '', 'The fade counts as over.'),
    0x5F: ('wait-fade', '', 'Waits for the fade to finish.'),
    0x60: ('movie-play', 'name:u8 class:u8', 'Plays the movie the room names as string `name` (room handler +0x34) with movie class `class` (0x62 0 / 2 to follow it).'),
    0x61: ('cutscene-start', 'name:u8', 'Every active character is told (+0x78); the cutscene director restarts on the room\'s scene script named by string `name`.'),
    0x62: ('cutscene-control', 'op:u8',
           'Movie / cutscene director control by `op`: 0 the movie\'s state into the result '
           '(+0x934: 2 paused, 1 running, -1 none); 1 stop the movie; 2 restart it (waits until it '
           'runs); 3 director +0x10; 4 director +0x10 then +0x14 unless in mode 5; 5 / 6 / 7 the '
           'director\'s cues (before / after; off; the next one from the movie frame - its button '
           '11 toggles state flag 0x29); 8 camera director back to its default mode (CamDirector_ModeDefault), director +0x48, flag 0x29 off; '
           '9 / 10 pause / resume the movie; 11 a black screen over half; 12 shows the prepared '
           'message as often as the director says.'),
    0x63: ('hewie-face', 'face:deg', 'Hewie turns to heading `face` (Hewie action 0x72).'),
    0x64: ('depth-range-off', '', 'The depth-range effect (room effect slot 0x1C) removed.'),
    0x65: ('depth-range', 'a:fx b:fx c:fx d:fx', 'Room effect slot 0x1C made anew as a depth range (DepthRange_Init) with the four values.'),
    0x66: ('lights-doorway', 'x0:fx y0:fx z0:fx x1:fx y1:fx z1:fx x2:fx y2:fx z2:fx x3:fx y3:fx z3:fx',
           'A lit doorway for the room\'s lights (lights +0x38): four corners; its facing and middle are derived.'),
    0x67: ('char-find-tri', 'who:chr', 'Character `who`\'s nav triangle looked up from its position.'),
    0x68: ('sound', 'id:u32 bank:u8 x:fx y:fx z:fx vol:s8 pitch:s8',
           'Sound `id` of bank `bank` & 0x3F; bank >> 6: 0 at (x, y, z) (only if the progress\' +0x7C '
           'allows: always in this game), 2 plain, else at the camera; vol / pitch offsets.'),
    0x69: ('sound-stop', 'id:u32 bank:u8', 'Stops sound `id` of bank `bank` (SndDriver_StopSound).'),
    0x6A: ('music', 'op:u8 a:u8 b:u8',
           'Stage music by `op`: 0 global volume fade to a over b frames (MusicDir_GlobalVolumeTo); '
           '1 the stage\'s channels (+0x40); 2 load (+0xC) and hold (+0x1C); 3 waits until its banks '
           'are in; 4 release; 5 silence.'),
    0x6B: ('nop-progress-48', 'a:u8', 'Calls the progress\' +0x48: empty in this game (no effect).'),
    0x6C: ('nop-progress-4C', '', 'Calls the progress\' +0x4C: empty in this game (no effect).'),
    0x6D: ('subscreen-open', 'mode:u8', 'Opens the sub-screen in mode `mode` (0 the in-game menu, 1 save, 2 the word plates, ...; SubScreen.mode) and sets state flag 4.'),
    0x6E: ('movie-param', 'a:u8 b:u8', 'The playing movie\'s luminance keys: clear up to a, opaque from b (Sofdec_SetParam, as mwPlySetLumiKey; for the movie classes laid over by brightness).'),
    0x6F: ('self-through-exit', 'exit:u8', 'self: walks through exit `exit` of this room (character move 5 to the door\'s far point).'),
    0x70: ('self-through-exit-back', 'exit:u8', 'self: as 0x6F, the other way through.'),
    0x71: ('rumble', 'on:u8 strength:u8 frames:u16', 'Pad rumble: on 0 the small motor (1, 1), else strength `strength`, for `frames` (gRumble). (Not a screen fade.)'),
    0x72: ('obstacle-place-saved', 'i:u8 n:u8 kind:u8', 'Pushable obstacle `i` (model "oshi0n", kind) placed at its saved squares (Obstacles_PlaceSaved).'),
    0x73: ('obstacle-save-at', 'i:u8 a:u16 b:u16', 'Pushable obstacle `i`\'s saved squares = a, b (Obstacles_SetSaved).'),
    0x74: ('obstacle-save', 'i:u8', 'Pushable obstacle `i`\'s current squares saved (Obstacles_TakeSaved).'),
    0x75: ('obstacle-keep-spot', 'i:u8', 'Pushable obstacle `i`\'s spot kept (Obstacles_KeepSpot).'),
    0x76: ('obstacle-model-back', 'i:u8 n:u8', 'Pushable obstacle `i`\'s model ("oshi0n" number n) put back at its kept spot (Obstacles_ModelBack).'),
    0x77: ('hewie-anim', 'a:s16 anim:u16', 'Hewie plays animation `anim` (Hewie_SetAnim).'),
    0x78: ('hewie-bark', '', 'Hewie steps to a pose (3, or 0 in special modes) and barks (character move 0x12 -> his action 0x45).'),
    0x79: ('char-to-xz', 'who:chr tri:u16 x:fx z:fx face:deg', 'Places character `who` at (x, z) on triangle `tri` (height from the triangle), facing `face`.'),
    0x7A: ('hewie-go-to', 'tri:u16 x:fx z:fx y:fx b:s16', 'Hewie goes to the point on triangle `tri` (operands stored x, z, y) (character move 0x13).'),
    0x7B: ('char-wait-motion', 'who:chr bits:u8', 'Waits until character `who`\'s motion event flags have any of `bits`.'),
    0x7C: ('zone', 'z:u8 x:fx y:fx z:fx r:u16 h:s16 kind:u8',
           'Zone `z` (0..31, for conditions 0x07 / 0x08 ...) on: centre (x, y, z), radius r, height h, kind `kind`.'),
    0x7D: ('zone-at-effect', 'z:u8 fx:u8 r:u16 h:s16 kind:u8', 'Zone `z` on around room effect `fx`\'s position: radius r, height h, kind.'),
    0x7E: ('bgm', 'track:u8 vol:fx pause:u8',
           'Background music track `track` (0xFF: none - the playing one fades out) wanted at volume `vol` '
           '(BgmCtl_Want), started paused if `pause`; pause 0xFF: resume the ADX stream instead.'),
    0x7F: ('flicker-sprite', 'fx:u8 x:fx y:fx z:fx', 'Room effect slot `fx` made anew as a flickering animated sprite (EvEffect7F) at (x, y, z).'),
    0x80: ('effect-remove', 'fx:u8', 'Room effect slot `fx` removed.'),
    0x81: ('self-anim-blend', 'anim:u16 b:u16', 'self: animation `anim` with b (character move 8).'),
    0x82: ('event-camera', 'on:u8 a:fx b:fx c:fx d:fx',
           'The event camera (camera director): with on, set from the four values (EventCam_Set); '
           'then held on / off (CamDirector_HoldEffect1C).'),
    0x83: ('item-give-count', 'item:u16 n:u8', 'An item counted (progress +0xFBE) and given: `item` as the script sees it (Events_ScriptRoom: in the mode of progress +0x30 bit 0x8000, 0x40 / 0x41 are 0x70), n of it (Items_Give).'),
    0x84: ('summon-take', 'a:u8', 'The summoner takes the partner (Summoner_Take(a)).'),
    0x85: ('hewie-trust', 'n:s16', 'Hewie\'s trust in Fiona + n (Hewie_AddTrust).'),
    0x86: ('effect-86', 'fx:u8 x:fx y:fx z:fx kind:u8', 'Room effect slot `fx` made anew as an EvEffect86 at (x, y, z) with `kind`.'),
    0x87: ('char-effect-moving', 'who:chr fx:u8', 'Sends room effect `fx` 1 if character `who` hasn\'t moved this frame, else 2.'),
    0x88: ('noise', 'loud:u8 tri:u16', 'A noise of loudness `loud` in this room at triangle `tri` (stalkers hear it).'),
    0x89: ('message-prepare', 'msg:u16', 'Prepares message `msg` for the window (shown later, see 0x62 12).'),
    0x8A: ('nop-progress-74', 'a:u16 b:s8 c:s16 d:u8 e:s8 f:s8 g:u16 h:fx', 'Calls the progress\' +0x74 (Progress_Noop74): no effect.'),
    0x8B: ('game-over-flag', 'v:u8', 'The game-over flag (progress +0x73EB00, also set when Fiona is caught for good) = v.'),
    0x8C: ('scene-effect-8C', 'x:fx y:fx z:fx zone:u8 n:s32 b:u8 t:fx',
           'A scene effect (Effect6FF60, 0xE40 bytes) at (x, y, z) with zone rectangle `zone` (0x8D), n, b, t.'),
    0x8D: ('zone-rect', 'z:u8 id:u8 x0:fx z0:fx x1:fx z1:fx', 'Zone rectangle `z` (+0x894): an id and x0, z0, x1, z1 (used by 0x8C and some conditions).'),
    0x8E: ('self-turn-to-xz', 'x:fx z:fx', 'self: turns to face (x, z) (character move 15).'),
    0x8F: ('char-no-shadow', 'who:chr on:u8', 'Character `who`\'s shadow volumes off (its model\'s +0x4D9; Model_DrawWithShadow).'),
    0x90: ('light', 'op:u8 light:u8 k:fx',
           'Room light `light`: op 0 back to the room\'s own; 1 / 2 its value 7 / 11 (intensity?) scaled by k.'),
    0x91: ('noise-level', 'v:u8', 'The noise level setting (progress +0x1114) = v.'),
    0x92: ('action-force', 'mode:u8 who:chr act:u8',
           'As 0x05 but always starts the action; mode 1: at once for characters; who 0 also calls '
           'the progress\' +0x44 (empty).'),
    0x93: ('panic-grow', 'v:u8', 'A panic value grows: bit 0x80 set: progress +0x7E0 + 128; else +0x7E4 + v / 30.'),
    0x94: ('fiona-calm', 'n:u8', 'Fiona calms down by n (Fiona_CalmDown).'),
    0x95: ('fiona-recovery-lower', 'n:u8', 'Fiona\'s recovery lowered by n (Fiona_LowerRecovery).'),
    0x96: ('fiona-target', 'id:u8 frames:s32', 'Fiona looks at / targets character `id` for `frames` (30 when not above 0) (Fiona_SetTarget).'),
    0x97: ('fiona-costume', 'costume:u8', 'Fiona\'s model swapped for costume `costume` (taken out, reloaded, not active).'),
    0x98: ('char-in', 'slot:u8', 'The character in `slot` comes in (waits while it is loading); it and Fiona active.'),
    0x99: ('state-flag-16', 'a:u8 b:u8', 'State flag 0x16 set; the progress\' +0x78 (Progress_Noop78: nothing).'),
    0x9A: ('self-through-door', 'door:u16', 'self: as 0x6F, the exit given by door id `door`.'),
    0x9B: ('screen-blend', 'a:u32 b:u8 on:u8',
           'Room effect slot 0x1E: on 0 removed, else made anew as a screen blend (ScreenBlend_Init) '
           'with a (little-endian) and b.'),
    0x9C: ('fog', 'a:u32 b:u32 near:fx far:fx on:u8',
           'Room effect slot 0x1D: on 0 removed, else made anew as fog (Fog_Init): a, b raw floats '
           '(little-endian), near, far.'),
    0x9D: ('char-anim-hold', 'id:u8 anim:u16 blend:u8 speed:u8',
           'Character `id` plays animation `anim` (blend, speed) and is held in a scripted state.'),
    0x9E: ('wait-char-anim', 'id:u8', 'Waits until character `id`\'s animation comes round (its end flag 0x20).'),
    0x9F: ('specks', 'a:u8 x:fx y:fx z:fx b:u8 c0:u8 c1:u8 c2:u8 c3:u8', 'A scene effect: a swarm of specks (SpeckSwarm) at (x, y, z).'),
    0xA0: ('splash', 'a:u8 b:u8 x:fx y:fx z:fx c:u8 d0:u8 d1:u8 d2:u8 d3:u8', 'A scene effect: a splash (Splash) at (x, y, z).'),
    0xA1: ('exits-rebuild', '', 'The rooms\' exits are rebuilt (room manager +0x90).'),
    0xA2: ('door-flag-82', 'door:u8 on:u8', 'Door `door`\'s second flag (+0x82; Doors_SetFlag82). Nothing in the C reads it yet.'),
    0xA3: ('panic-stage', 'stage:u8', 'The panic\'s stage = `stage` (Panic_SetStage).'),
    0xA4: ('self-anim-9', 'anim:u16 b:u16', 'self: animation `anim` with b (character move 9).'),
    0xA5: ('scene-ending', 'id:u16', 'Requests scene 5 (an ending / the results, id `id`) unless state flag 0x12 or Fiona is busy.'),
    0xA6: ('char-file-load', 'id:u8 file:u8', 'Loads the room\'s file named by string `file` into character `id`\'s model buffer (slots 0..3).'),
    0xA7: ('char-file-use', 'id:u8', 'Character `id`\'s model takes its loaded buffer (0xA6); waits while the loader is busy.'),
    0xA8: ('room-sounds', '', 'Loads the current room\'s sound set.'),
    0xA9: ('scene-effect-71000', 'var:var a:u8 v0:fx v1:fx v2:fx v3:fx v4:fx v5:fx v6:fx v7:fx',
           'A scene effect (Effect71000, 0x60 bytes) with a and eight values; its slot kept in script variable `var`.'),
    0xAA: ('self-walk-anim', 'anim:u16 tri:u16 tri2:s16 x:fx z:fx face:deg',
           'self: walks to (x, z) with animation `anim`, then faces `face` (character move 17).'),
    0xAB: ('self-look-at-point', 'x:fx y:fx z:fx', 'self: looks at (x, y, z) (character move 13). (Not "go to".)'),
    0xAC: ('self-doorway-fade', 'on:u8', 'self: the character fades at doorways (+0xE4: Character_RegionFade picks its draw layer each frame); off keeps a fixed layer (see 0xAE / 0xB5).'),
    0xAD: ('self-face-zone', 'zone:u8', 'self: turns to the point of zone `zone` (its number from script variable `zone`: Events_GetVar) (move 15).'),
    0xAE: ('char-tint', 'who:chr rgba:u32 on:u8',
           'Character `who`: on 0: +0xE4 = 1; else drawn tinted: the renderer\'s tint (+0x70) = rgba, '
           'the character in the fading layer 0x0F.'),
    0xAF: ('hewie-go-to-point', 'x:fx y:fx z:fx', 'Hewie goes to (x, y, z) (character move 0x14).'),
    0xB0: ('hewie-go-to-zone', 'zone:u8 run:u8', 'Hewie goes to zone `zone`\'s point (variable `zone`): run 0 move 0x14, 1 move 13.'),
    0xB1: ('flicker-sprite-var', 'fx:var x:var y:var z:var', 'As 0x7F with the slot and position in script variables (positions / 1000).'),
    0xB2: ('fiona-thrown', 'id:u8 recover:u8', 'Fiona is thrown down by character `id` (reaction action 4, sub 0xA, with a rumble; Fiona_StartAction4); recover 1 also resets her recovery.'),
    0xB3: ('char-to-xyz', 'who:chr x:fx y:fx z:fx face:deg', 'Places character `who` at (x, y, z) facing `face` (triangle looked up).'),
    0xB4: ('avoid-prompt', 'v:u8', 'The avoid / struggle prompt (+0xC with v).'),
    0xB5: ('char-layer', 'who:chr layer:u8', 'Character `who` drawn in renderer layer `layer` (Character_Set152C).'),
    0xB6: ('stalker-gift', '', 'A gift from the stalker\'s table: the next item Fiona has fewer than 99 of (message 0x8011), or message 0x801A when none are left; after the first only 1 in 10 times.'),
    0xB7: ('hewie-model', 'kind:u8', 'Hewie\'s model swapped for kind 0..2 (taken out, reloaded, not active).'),
    0xB8: ('char-heal', 'id:u8 n:s32', 'Character `id` (in the scene) heals by |n|, up to its maximum (+0x14CC).'),
    0xB9: ('char-load-2', 'id:u8 slot:u8', 'Brings in character `id` in slot `slot` (CharLoad_EventChar, the second loader).'),
    0xBA: ('char-hand-over', 'from:slot to:slot', 'The character in slot `from` gives its motion banks to slot `to` (waits while `to` is loading).'),
    0xBB: ('hewie-wait-5', 'on:u8', 'Hewie (if in this room): his wait timer = 5 frames and his "forced action 4" flag (+0xF3588) on / off.'),
    0xBC: ('fiona-hewie-react', '', 'Fiona reacts to Hewie (Fiona_HewieReact 8).'),
    0xBD: ('hewie-stay-300', '', 'Hewie\'s +0xF3688 = 300: for a while he obeys stay / wait commands at once (Hewie_CommandAction). Unused by the scripts.'),
    0xBE: ('item-tab', 'op:u8 item:u8', 'The in-game item tab (SubScreen_TabCommand): op 0 announces the script\'s item `item` (Events_ScriptRoom), 1 waits for its files, 2 slides the tab in, 4 out; waits while it moves.'),
    0xBF: ('fiona-fear', 'v:fx', 'Fiona\'s fear (+0x1AD5F4) = v (0..100; over 90 she panics).'),
    0xC0: ('threat-add', 'v:u8', 'The threat meter\'s accumulator (progress +0x7DC: the threat object +0x24) grows by v (0..100), as a small Threat_Raise.'),
    0xC1: ('sound-set', 'set:u8', 'Loads sound set `set` (Progress_LoadSoundSet).'),
    0xC2: ('door-lock-for', 'id:u8 door:u16 state:u8', 'Door `door`\'s lock for character `id` = state (Progress_LockDoorFor).'),
    0xC3: ('hewie-anim-root', 'anim:u16 blend:u16', 'Hewie plays animation `anim` (blend) with its root motion (character move 0x15 -> his action 0x47).'),
    0xC4: ('hewie-mode', 'mode:u32', 'Hewie\'s mode (Hewie_SetMode).'),
    0xC5: ('hewie-look-zone', 'zone:u8 dy:fx', 'Hewie looks at zone `zone`\'s point (variable `zone`), raised by dy, if he isn\'t already looking at something.'),
    0xC6: ('hewie-look-char', 'id:u8 dy:fx', 'Hewie looks at character `id` (in this room, active), raised by dy, if he isn\'t already looking at something.'),
    0xC7: ('self-move-16', 'v:s16', 'self: character move 16 with v (an animation).'),
    0xC8: ('dust', 'kind:u8 x:fx y:fx z:fx r:u8 g:u8 b:u8 own:u8',
           'A dust burst (SpriteBurst) of `kind` at (x, y, z): colour r, g, b if `own`, else grey (0x80 for kind 0, else 0x50); size 16.'),
    0xC9: ('panic-level', 'level:u8', 'The panic level set to `level` if it has reached it (Panic_SetLevel).'),
    0xCA: ('sound-volume-scale', 'v:fx', 'Every sound\'s volume scaled by v (progress +0x1118).'),
    0xCB: ('creatures-clear', 'which:u8', 'The room\'s creatures (0 all, 1 slots 0..6, 2 slots 7..9) told (+0x10) and removed.'),
    0xCC: ('char-model-op', 'op:u8 id:u8 v:u8',
           'op 3: the cutscene director\'s +0x78 with id; else stalker `id`\'s model: 0 +0x2C, 1 +0x30, '
           '2 +0x34 with v, 4 Daniella\'s capsules back (v).'),
    0xCD: ('sprites-additive', 'mode:u8', 'The renderer draws sprites additively in mode `mode` (Renderer_Additive).'),
    0xCE: ('deal-things', '', 'The placed things are dealt out (Events_DealThings).'),
    0xCF: ('movie-volume', 'v:fx', 'The playing movie\'s volume = v (0..1), applied; +0x1BC set (starts it: see the FMV notes).'),
    0xD0: ('hewie-side', 'side:u16', 'Hewie\'s side (0..2, else none).'),
    0xD1: ('map-page', 'page:s8', 'The sub-screen\'s map turns to page `page` (+0x2C: Map_TurnTo).'),
    0xD2: ('things-clear', '', 'Every placed thing is removed (PlacedThings_Clear).'),
    0xD3: ('camera-shake', 'v:fx', 'Camera shake: each frame the eye is moved randomly by up to v on each axis (camera +0x4, Camera_Set4); 0 stops it.'),
    0xD4: ('room-frames-set', 'n:s32', 'The frames-in-this-room count (+0x704: phase 1 counts it, entering resets it) = n (see condition 0x5B).'),
    0xD5: ('pvar-set', 'n:u8 v:u8', 'Progress variable n = v.'),
    0xD6: ('effects-arena-flip', '', 'If state flag 8: the renderer flips its second packet arena next frame and clears it (Renderer_Set304DE0).'),
    0xD7: ('hewie-anim-set', 'anim:u16', 'Hewie\'s motion plays `anim` (Motion_Play).'),
    0xD8: ('reward-item', '', 'By progress +0xFB6 (from 20: steps of 20): item 0x270..0x273 added, with the pickup sound.'),
    0xD9: ('dust-motes', 'x:fx y:fx z:fx size:fx', 'A scene effect: a dust mote source (DustMoteSource) at (x, y, z) of `size`.'),
    0xDA: ('nav-tri-flags', 'set:u8 tri:u16 bits:u32', 'Nav triangle `tri`\'s flags (gRoomEventObj, NavGroups_SetTri / _ClearTri): set 1 sets `bits`, else clears them.'),
}

# ---- sub-commands of 0x59 (flags, items, doors: operand 1 the sub-command, then n) ----------

FLAGS = {
    0x00: ('story-flag-set', '_:u8 n:u16', 'Story flag n set (the progress\' scenario flags, +0x1C).'),
    0x01: ('story-flag-clear', '_:u8 n:u16', 'Story flag n cleared.'),
    0x02: ('state-flag-set', '_:u8 n:u16', 'State flag n set (the progress\' 46 flags, +0x8: control, panic, ...).'),
    0x03: ('state-flag-clear', '_:u8 n:u16', 'State flag n cleared.'),
    0x04: ('door-lock', '_:u8 door:u16', 'Door `door` locked (its state bit 3 set: Progress_UnlockDoor). (Door state bit 3 is the lock: the decomp\'s Progress_UnlockDoor / LockDoor / DoorUnlocked have it the wrong way round.)'),
    0x05: ('door-unlock', '_:u8 door:u16', 'Door `door` unlocked (bit 3 cleared: Progress_LockDoor).'),
    0x06: ('door-passable', '_:u8 door:u16', 'Door `door` passable as if unlocked (its state bit 4: DoorHold_Usable), and characters can\'t hold it open (DoorHold_Open).'),
    0x07: ('door-reopen-unlock', '_:u8 door:u16', 'Door `door` no longer closed off (Rooms_Reopen), then unlocked (as 0x05).'),
    0x08: ('door-close-off-lock', '_:u8 door:u16', 'Door `door` closed off (Rooms_CloseOff), then locked (as 0x04).'),
    0x09: ('door-open-set', '_:u8 door:u16', 'Door `door`: its open bit set.'),
    0x0A: ('door-open-clear', '_:u8 door:u16', 'Door `door`: its open bit cleared.'),
    0x0B: ('message-param-room', '_:u8 n:u16', 'The message\'s parameter 0 = room id n as the script sees it (Events_ScriptRoom).'),
    0x0C: ('item-use', '_:u8 item:u16', 'Item `item` is used up (Items_UseId).'),
    0x0D: ('item-give', '_:u8 item:u16',
           'Item `item` added to the inventory, with the pickup sound; bit 0x8000 marks the hard-mode '
           'variant (only given in that mode, else only the plain one).'),
    0x0E: ('item-add', '_:u8 item:u16', 'Item `item` added to the inventory, silently (SubScreen_AddFile).'),
    0x0F: ('story-flag-set-var', '_:u8 var:u16', 'Story flag (number in script variable `var`) set.'),
    0x10: ('story-flag-clear-var', '_:u8 var:u16', 'Story flag (number in script variable `var`) cleared.'),
    0x11: ('subscreen-bit', '_:u8 n:u16', 'The sub-screen\'s bit n set (map / file entries) (SubScreen_SetBit).'),
    0x12: ('resident-flag-set', '_:u8 n:u16', 'Resident flag n set (kept across games: unlocks; the game\'s +0x24).'),
}

# ---- sub-commands of 0x50 (placed objects) --------------------------------------------------

PLACED = {
    0: ('object-show', '_:u8 obj:u8 on:u8', 'Placed object `obj` shown (on) or hidden.'),
    1: ('object-anim', '_:u8 obj:u8 anim:u8', 'Placed object `obj` plays animation `anim` once.'),
    2: ('object-anim-loop', '_:u8 obj:u8 anim:u8', 'Placed object `obj` plays animation `anim` looped.'),
    3: ('object-anim-reset', '_:u8 obj:u8 _:u8', 'Placed object `obj`\'s animation reset to its start.'),
    4: ('object-hide-reset', '_:u8 obj:u8 _:u8', 'Placed object `obj` hidden and put back as defined (PlacedObject_ToDef).'),
}

# ---- conditions -----------------------------------------------------------------------------

COND = {
    0x00: ('story-flag?', 'n:u16', 'Story flag n is set (the progress\' scenario flags, +0x1C).'),
    0x01: ('char-in-area?', 'who:chr area:u8', 'Character `who` (active) is in this room, inside event area `area`.'),
    0x02: ('char-entered-area?', 'who:chr area:u8', 'Character `who` (active, in this room) has just entered area `area`.'),
    0x03: ('char-left-area?', 'who:chr area:u8', 'Character `who` (active, in this room) has just left area `area`.'),
    0x04: ('exit-taken?', 'exit:u8', 'The exit just taken (+0x702, see command 0x00) is `exit`.'),
    0x05: ('char-heading?', 'who:chr dir:s8 within:u8', 'Character `who` faces heading dir x 2 degrees, within `within` degrees.'),
    0x06: ('char-faces-area?', 'who:chr area:u8 within:u8', 'Character `who` is inside area `area` and faces its middle, within `within` degrees.'),
    0x07: ('exit-usable?', 'exit:u8',
           'Whoever is controlled may take exit `exit` now: free (Fiona idle or walking, Hewie idle), '
           'inside the exit\'s area, its door open and the exit not marked.'),
    0x08: ('state-flag?', 'n:u16', 'State flag n is set (the progress\' 46 flags).'),
    0x09: ('scene-request?', 'v:s32', 'The pending scene request (progress +0x1134) is v.'),
    0x0A: ('exit-door-open?', 'exit:u8', 'The door at exit `exit` of this room is open.'),
    0x0B: ('door-locked?', 'door:u16', 'Door `door` is locked (state bit 3: Progress_DoorUnlocked, named the wrong way round).'),
    0x0C: ('door-not-closed-off?', 'door:u16', 'Door `door` isn\'t closed off (Rooms_DoorClosedOff).'),
    0x0D: ('game-mode?', 'mode:u8', 'The game mode is `mode` (Progress_GameMode).'),
    0x0E: ('char-busy?', 'who:chr', 'Script slots 0xF0..0xFA: that slot\'s script runs; else character `who` is in a scripted state (+0xE0).'),
    0x0F: ('hewie-near-command?', '', 'Hewie (in the scene) is near enough for Fiona\'s commands (Hewie_FionaNearCommand).'),
    0x10: ('stalker-alert?', 'v:u8', 'The stalker alert state (Progress_StalkerAlert) is v.'),
    0x11: ('room-condition?', 'cond:u8 len:u8 args:str', 'The room\'s own condition `cond` (room handler +0x2C: RoomXX_Condition / CondTable) with the script\'s character and the bytes.'),
    0x12: ('counter?', 'n:u8', 'The event counter (+0x703, commands 0x16..0x18) is n.'),
    0x13: ('frames?', 'n:u16', 'This script\'s frame count (+0x14) is n.'),
    0x14: ('var?', 'var:var v:s32', 'Script variable `var` is v.'),
    0x15: ('pad?', 'button:u8 how:u8', 'Pad test: button 0 circle, 1 square, 2 L1, 3 triangle, 4 R1, 5 cross, 6 start, held (how bit 0) or just pressed (bit 1). Button with bit 7: Fiona\'s shake flag set and Fiona_Shakes instead.'),
    0x16: ('char-here?', 'id:u8', 'Character `id` is active and in this room.'),
    0x17: ('self-is?', 'id:u8', 'This script\'s character is `id` (0xFE: the active stalker).'),
    0x18: ('char-group-bit4?', 'id:u8 group:u8', 'Character `id`\'s pursuer group fields (PursuerGroup_Fields(group)) have bit 4.'),
    0x19: ('char-on-tri?', 'who:chr tri:u16', 'Character `who` (active) is in this room on triangle `tri`.'),
    0x1A: ('result?', 'v:s32', 'The event result (+0x934: set by command 0x62 0 / 2, the movie\'s state) is v.'),
    0x1B: ('pvar?', 'n:u8 v:u8', 'Progress variable n is v.'),
    0x1C: ('chance?', 'pct:u8', 'A `pct` percent chance.'),
    0x1D: ('panic-stage?', 'stage:u8', 'The panic\'s stage (progress +0x7B8) is `stage` (0xFF: 4 or 5).'),
    0x1E: ('char-dead?', 'who:chr', 'Character `who` (active) has no health left.'),
    0x1F: ('char-touching-facing?', 'a:chr b:chr m0:u8 m1:u8 within:u8', 'Character `a` touches `b` (margins m0, m1) and faces it, within `within` degrees.'),
    0x20: ('cutscene-mode?', 'mode:u8', 'The cutscene director\'s mode (+0x2C) is `mode`.'),
    0x21: ('control-action?', 'v:s32', 'The controlled character\'s current action (Fiona +0x1AD6B8, Hewie +0xF3798) is v.'),
    0x22: ('answer?', 'v:u8', 'The message window is closed and its chosen answer (+0x750) is v.'),
    0x23: ('chars-within?', 'a:chr b:chr d:s32',
           'Characters `a` and `b` (active) are within distance |d|; Fiona or Hewie to a stalker only '
           'while the stalker is present (+0x1544).'),
    0x24: ('hewie-action?', 'v:s32', 'Hewie (in the scene)\'s current action (+0xF3564) is v.'),
    0x25: ('camera-mode?', 'v:s8', 'The camera director\'s +0x24 is v.'),
    0x26: ('fiona-started?', 'v:s32', 'The action Fiona last started (+0x1AD6BC, Fiona_MarkActionStart; -1 none) is v.'),
    0x27: ('char-faces-xz?', 'who:chr x:s16 z:s16 within:u8', 'Character `who` (in this room) faces (x, z), within `within` degrees.'),
    0x28: ('obstacle-on?', 'i:u8 tri:u16', 'Pushable obstacle `i` stands on triangle `tri` (Obstacles_IsSquare).'),
    0x29: ('ebit?', 'n:u8', 'Event bit n is set (commands 0x57 / 0x58).'),
    0x2A: ('fading?', '', 'A fade (command 0x5C) is running.'),
    0x2B: ('fade-past-40?', '', 'The fade\'s frame count (+0x11F0) has reached +0x40. Unused by the scripts.'),
    0x2C: ('camera-setup-changed?', '', 'The camera director\'s setup changed (+0x2C, CamDirector_SetupChanged).'),
    0x2D: ('sound-bank-loaded?', 'bank:u8', 'Sound bank `bank` is loaded (SndDriver_BankLoaded).'),
    0x2E: ('char-in-nav-group?', 'who:chr group:u8', 'Character `who` (in this room) stands on a triangle of nav group `group` (NavGroups +0x14).'),
    0x2F: ('item-3F-under-10?', '', 'Fewer than 10 of item 0x3F are held (Items_CountItem3F).'),
    0x30: ('char-heading-for?', 'who:chr exit:u8', 'Character `who` (active, in the current room) is heading for exit `exit` (+0x14D4).'),
    0x31: ('hewie-stays?', '', 'Hewie (in the scene) may not break off (Hewie_MayBreakOff is 0).'),
    0x32: ('char-in-room?', 'who:chr room:u16', 'Character `who` (active) is in room `room`.'),
    0x33: ('hewie-side?', 'side:s8', 'Hewie (in the scene) is on side `side` of the room (+0xF3668; command 0xD0 sets it).'),
    0x34: ('char-motion-flags?', 'who:chr bits:u8', 'Character `who` (active)\'s animation event flags have any of `bits`.'),
    0x35: ('char-zone-bits?', 'who:chr zone:u8 bits:u8', 'Character `who` (in this room, its radius / height) against zone `zone`: all of `bits` (Zone_TestCylinder).'),
    0x36: ('char-zone-bits-before?', 'who:chr zone:u8 bits:u8', 'As 0x35 with where the character was last frame.'),
    0x37: ('char-in-zone?', 'who:chr zone:u8 _:u8', 'Character `who` is in zone `zone` (Zone_HasAnyChar).'),
    0x38: ('cutscene-cue-reached?', 'cue:u16', 'The cutscene director\'s cue (+0x34) has reached `cue`.'),
    0x39: ('item-count?', 'item:u8 n:u8', 'At least n of the script\'s item `item` (Events_ScriptRoom) are held.'),
    0x3A: ('cutscene-event?', 'k:u8', 'The cutscene director reports event `k` this step (+0x54 above 0).'),
    0x3B: ('cutscene-passed?', 'k:u8 at:u8', 'The cutscene director\'s counter for `k` (+0x58) passed `at` within this step.'),
    0x3C: ('stalker-kind-here?', 'kind:u8', 'The stalker (active, in this room) is of kind `kind`.'),
    0x3D: ('fiona-free?', '', 'Fiona can be controlled (Fiona_IsIdle) and the panic\'s stage is below 4.'),
    0x3E: ('char-not-at-door?', 'who:chr door:u16', 'Character `who` (active, in this room) is not at door `door` (Doors_Side).'),
    0x3F: ('fiona-action?', 'v:u32', 'Fiona\'s action (+0x1AD580) is v.'),
    0x40: ('cutscene-near-end?', '', 'The cutscene has just come within 17 frames of its end (Cutscene_NearEnd).'),
    0x41: ('char-C4?', 'who:chr v:u8', 'Character `who`\'s state +0xC4 is v (see command 0x47).'),
    0x42: ('hewie-pool-in-use?', '', 'Hewie\'s pool (+0xF359C) is in use (+0xF3598). Unused by the scripts.'),
    0x43: ('hewie-mode?', 'mode:s32', 'Hewie\'s mode (+0xF35C0: 0 normal, 1..3 timed; command 0xC4 sets it) is `mode`. Unused by the scripts.'),
    0x44: ('char-on-nav-flags?', 'who:chr flags:u32', 'Character `who` (active, in the current room) stands on a triangle with any of `flags`.'),
    0x45: ('char-unseen?', 'id:u8', 'Character `id` is out of sight: absent, inactive, elsewhere, or off the camera.'),
    0x46: ('story-flag-var?', 'var:var', 'Story flag (number in script variable `var`) is set.'),
    0x47: ('self-touching?', 'id:u8', 'This script\'s character and character `id` (active, in this room, not +0x2A) touch (its margins).'),
    0x48: ('self-at-motion-event?', '', 'This script\'s character is idle (+0xF4 0) at a motion event: marks it done (+0xE1).'),
    0x49: ('char-action?', 'who:chr v:u8', 'Character `who`\'s action (+0xF8) is v.'),
    0x4A: ('cutscene-shot?', 'shot:u8', 'The cutscene\'s current frame is in shot `shot` (Cutscene_ShotAt).'),
    0x4B: ('stalker-stance-2?', '', 'The stalker (active) is in stance 2 playing animation 0x1805 / 0x1806 (virtual +0x10C, Pursuer_InStance2Anim). Unused by the scripts.'),
    0x4C: ('hewie-can-command?', '', 'Fiona can give Hewie a command (Hewie_FionaCanCommand).'),
    0x4D: ('self-done?', '', 'This script\'s character\'s move is done (+0xE1).'),
    0x4E: ('message-closed?', '', 'The message window is closed.'),
    0x4F: ('point-on-camera?', 'x:s16 y:s16 z:s16', 'The point (x, y, z) is in the camera\'s view (+0xD4).'),
    0x50: ('char-tri-free?', 'who:chr', 'Character `who` (active, in this room)\'s triangle is free for it (Actor_TriFreeFor).'),
    0x51: ('panic-98?', '', 'The panic level (+0x7BC) is 98 or more.'),
    0x52: ('char-full-health?', 'who:chr', 'Character `who` (active) is at full health.'),
    0x53: ('creature-action?', 'v:u8', 'One of the 10 room creatures (active, in the current room) is in action v.'),
    0x54: ('stalker-kind?', 'kind:u8', 'The stalker is of kind `kind`.'),
    0x55: ('stalker-active?', '', 'The stalker is in the scene.'),
    0x56: ('char-at-motion-event?', 'id:u8', 'Character `id` is at a motion event (its end flag 0x20).'),
    0x57: ('movie-playing?', '', 'A movie is playing.'),
    0x58: ('self-near-creature?', '', 'This script\'s character touches one of room creatures 7..9 (active, in this room).'),
    0x59: ('fiona-caught?', '', 'Fiona is caught (+0x1AD630). Unused by the scripts.'),
    0x5A: ('hewie-reachable?', '', 'Hewie can reach Fiona (Hewie_FionaReachable; no Hewie: yes).'),
    0x5B: ('event-704-reached?', 'v:u32', 'The event\'s +0x704 (command 0xD4) has reached v.'),
    0x5C: ('adx?', 'what:u8', 'The ADX stream: what 0 can start, else is playing (none: yes).'),
    0x5D: ('hewie-controlled?', '', 'Hewie is the one being controlled.'),
    0x5E: ('subscreen-bit?', 'n:u16', 'The sub-screen\'s bit n is set (SubScreen_TestBit; see 0x59 0x11).'),
    0x5F: ('pvars-equal?', 'a:u8 b:u8', 'Progress variables a and b are equal.'),
    0x60: ('resident-flag?', 'n:u16', 'Resident flag n is set (kept across games).'),
    0x61: ('hewie-trust-level?', 'v:u8', 'Hewie\'s trust level (+0xF35CC, it picks his wait timers) is v. Unused by the scripts.'),
    0x62: ('loader-done?', '', 'The file loader\'s current load is done (Loader_CurrentDone).'),
    0x63: ('char-loaded?', 'slot:u8 id:u8', 'Character `id` is in slot `slot` and finished loading.'),
    0x64: ('noise-slot-D?', '', 'The first noise slot (progress +0x1050) is of kind 0xD. Unused by the scripts.'),
    0x65: ('stalker-free?', '', 'The stalker (active) is in the current room and free: not held, not in certain attack moves, its triangle free.'),
}

# ---- formatting -----------------------------------------------------------------------------

SIZES = {'u8': 1, 's8': 1, 'chr': 1, 'slot': 1, 'var': 1, 'pad': 1, 'u16': 2, 's16': 2, 'deg': 2,
         'flag': 2, 'u32': 4, 's32': 4, 'fx': 4}


def parse(spec):
    return [tuple(x.split(':')) for x in spec.split()]


def values(spec, raw):
    """[(name, text)] of the operands in `raw` (the whole command, opcode first)"""
    out = []
    at = 1
    for name, typ in parse(spec):
        if typ == 'str':
            n = raw[at - 1] if at - 1 < len(raw) else 0
            text = bytes(raw[at:at + n]).split(b'\0')[0].decode('latin-1')
            out.append((name, repr(text)))
            at += n
            continue
        n = SIZES[typ]
        if at + n > len(raw):
            break
        v = int.from_bytes(bytes(raw[at:at + n]), 'big')
        if typ in ('s8', 's16', 's32', 'fx', 'deg') and v >= 1 << (8 * n - 1):
            v -= 1 << (8 * n)
        at += n
        if name == '_' or typ == 'pad':
            continue
        if typ == 'fx':
            text = '%g' % (v / 1000.0)
        elif typ == 'deg':
            text = '%d deg' % v
        elif typ in ('u8', 'chr', 'slot', 'var'):
            text = '0x%02X' % v if typ in ('chr', 'slot') or v > 9 else str(v)
        elif typ in ('u16', 'flag', 'u32'):
            text = '0x%X' % v if v > 9 else str(v)
        else:
            text = str(v)
        out.append((name, text))
    return out


def describe(kind, op, raw):
    """a one-line rendering of a decoded command / condition, or None if the table lacks it"""
    table = CMD if kind == 'cmd' else COND
    if op not in table:
        return None
    name, spec, _ = table[op]
    sub = None
    if kind == 'cmd' and op == 0x59 and len(raw) > 1 and raw[1] in FLAGS:
        sub = FLAGS[raw[1]]
    elif kind == 'cmd' and op == 0x50 and len(raw) > 1 and raw[1] in PLACED:
        sub = PLACED[raw[1]]
    if sub is not None:
        name = sub[0]
        spec = sub[1]
    args = ' '.join('%s=%s' % kv for kv in values(spec, raw))
    return '%s %s' % (name, args) if args else name


# ---- checks and the document ----------------------------------------------------------------

def spec_size(spec):
    n = 0
    for _, typ in parse(spec):
        if typ == 'str':
            return None
        n += SIZES[typ]
    return n


def check():
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import evdis
    cmd_len = evdis.cmd_lengths()
    r = evdis.Reader()
    bad = 0
    for op, (name, spec, _) in sorted(CMD.items()):
        n = spec_size(spec)
        want = cmd_len[op] - 1
        if n is not None and cmd_len[op] and n > want:
            print('cmd %02X %s: operands take %d bytes, the command has %d' % (op, name, n, want))
            bad += 1
    for op, (name, spec, _) in sorted(COND.items()):
        n = spec_size(spec)
        want = r.clen[op] - 1
        if n is not None and op != 0x11 and n > want:
            print('cond %02X %s: operands take %d bytes, the condition has %d' % (op, name, n, want))
            bad += 1
    names = [v[0] for table in (CMD, COND, FLAGS, PLACED) for v in table.values()]
    for n in set(names):
        if names.count(n) > 1:
            print('name used twice: %s' % n)
            bad += 1
    print('%d commands, %d conditions, %d flag sub-commands, %d placed-object sub-commands described'
          % (len(CMD), len(COND), len(FLAGS), len(PLACED)))
    missing = [op for op in range(0xDB) if op not in CMD]
    if missing:
        print('commands not described yet: ' + ' '.join('%02X' % o for o in missing))
    missing = [op for op in range(0x66) if op not in COND]
    if missing:
        print('conditions not described yet: ' + ' '.join('%02X' % o for o in missing))
    return bad


def write_doc():
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import evdis
    cmd_len = evdis.cmd_lengths()
    r = evdis.Reader()
    lines = ['# Event script opcodes', '',
             'Generated by `tools/event_opcodes.py` from its table; do not edit by hand.',
             'Listings of the scripts themselves: `tools/evdis.py ROOM`.', '']
    lines += [l for l in __doc__.split('\n')[5:] if not l.startswith('    tools/')]
    lines += ['', '## Control ops', '',
              'An if is `F0` / `F1` + a condition, then any `F2`..`F5` + condition (combined left to',
              'right); the first other op ends the chain and the block runs if the result is true.',
              'Blocks end at `F8` (or `F9`); `F7` switches to the else part. Blocks nest.', '',
              '| op | meaning |', '|----|---------|']
    for op, what in [(0xF0, 'if: opens a block run when the condition chain is true'), (0xF1, 'if not'), (0xF2, 'and (condition)'),
                     (0xF3, 'and not'), (0xF4, 'or'), (0xF5, 'or not'),
                     (0xF6, 'a block, always run (as if true)'), (0xF7, 'else'),
                     (0xF8, 'end of the block'), (0xF9, 'end, and skip the rest of the enclosing block'),
                     (0xFA, 'a plain block opens'), (0xFB, 'end of a plain block'), (0xFF, 'end of the script')]:
        lines.append('| %02X | %s |' % (op, what))
    lines += ['', 'So `F0 c1 F2 c2 F4 c3 <commands> F8` runs the commands when ((c1 and c2) or c3).', '']

    def table(title, entries, lens, prefix):
        out = ['## ' + title, '', '| op | len | name | operands | what it does |',
               '|----|-----|------|----------|--------------|']
        for op in sorted(entries):
            name, spec, desc = entries[op]
            ln = lens(op)
            out.append('| %s%02X | %s | `%s` | %s | %s |' % (prefix, op, ln, name,
                                                         ' '.join('`%s`' % s for s in spec.split()) or '-',
                                                         desc.replace('|', '\\|')))
        return out + ['']
    lines += table('Commands', CMD, lambda op: 'str' if cmd_len[op] == 0 else cmd_len[op], '')
    lines += table('0x59 sub-commands (operand 1)', FLAGS, lambda op: 4, '59 ')
    lines += table('0x50 sub-commands (operand 1)', PLACED, lambda op: 4, '50 ')
    lines += table('Conditions', COND, lambda op: 'str' if op == 0x11 else r.clen[op], '')
    path = os.path.join(ROOT, 'docs/event_opcodes.md')
    open(path, 'w').write('\n'.join(lines) + '\n')
    print('wrote', path)


if __name__ == '__main__':
    if '--check' in sys.argv:
        sys.exit(1 if check() else 0)
    write_doc()
