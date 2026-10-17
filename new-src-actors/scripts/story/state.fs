\ story/state.fs - a room's event state: the fields of a room actor (docs/subsystems/story.md).
\ The original keeps these in its event object (src/game/event.c; the offsets in the comments).
\ The room's scripts all run as its actor - the phases in its handlers, the action scripts as
\ its coroutines - so they share this state as the original's scripts share the event object.
IN: story.state
USING: actors ;

32 constant #vars       \ script variables
32 constant #zones      \ zones (cylinders the scripts set each frame)
28 constant /zone       \ a zone: on, kind, the centre's x y z, radius, height
17 constant #slots      \ action scripts at once: 0..5 the characters' (slot = character + 1), 6.. the scenes' (ids 0xF0..0xFA)
6 constant #chars
32 constant #effects    \ the room's effect slots (RoomEffects: 0x20)       \ characters the scripts know by slot: 0 Fiona, 1 Hewie, 2.. the others

state: room-state
  cell field this-room           \ the room these scripts are for
  cell field entered            \ its entry ran (a stale room-enter - the room already left - is ignored)
  cell field came-by            \ the exit it was entered by (-1: none)
  #vars cells field vars        \ the script variables (+0x810), cleared on entering
  cell field ebits              \ the event bits (+0x890)
  cell field counter            \ the event counter (+0x703)
  cell field tab-waiting        \ the item's tab moving (the scripts' 0xBE waits for the sub screen)
  cell field exit-taken         \ the exit taken, or arrived by (+0x702)
  cell field room-frames        \ phase 1 calls in this room (+0x704)
  cell field result             \ the event result (+0x934)
  cell field leaving            \ an exit was taken: no more actions start
  cell field going              \ the exit asked for this frame (-1 none): taken after the frame
  \ the message window (+0x708): its text (-1 closed), the chosen option, whose script opened it
  cell field msg  cell field answer  cell field msg-owner
  \ the action scripts: each slot's task (0 free), its character slot (-1 none), its id, frames
  #slots cells field slot-tasks  #slots cells field slot-who  #slots cells field slot-ids  #slots cells field slot-frames
  \ the script running now: its slot (-1 a phase script), its character slot and id ($FF none)
  cell field ctx-slot  cell field ctx-who  cell field ctx-id  cell field phase-frames
  \ the characters: where each was at the last frame's end (for areas entered / left), whether
  \ it was seen, and whether a script of this room has it (+0xE0: its scripted state)
  #chars 12 * field char-was  #chars cells field char-seen  #chars cells field char-scripted
  \ each character's move (+0xE1: done) and whether its animation ended, as it last told (moving)
  #chars cells field move-done-of  #chars cells field anim-ended-of  #chars cells field events-of
  \ the action prepared this frame (the scripts' scene-change: +0x1134): the scene (-1 none), its
  \ argument and kind - scene 5 an action script for Fiona, taken with the action button
  cell field prep-scene  cell field prep-arg  cell field prep-kind
  cell field prepared-msg       \ a message prepared for a scene's subtitles (-1 none)
  cell field prepared-page      \ its page shown (0x62 12 turns them; -1 none)
  \ the scene: the movie's class (Progress_PlayMovie's kind: 0 not drawn - a scene's timing and
  \ sound; 1..6 drawn), the cues (+0xBE4 / +0xBE8), a fade running (the screen's)
  cell field movie-kind  cell field cue  cell field cue-prev  cell field fading
  #zones /zone * field zones    \ the zones (all off as phase 1 starts)
  \ the room's effects: each slot's actor (-1 none) and where it was made
  #effects cells field fx-ids  #effects 3 * floats field fx-at
  #zones 5 cells * field zone-rects   \ the zone rectangles (an id, x0 z0 x1 z1: for the scene effects)
  \ what the others tell (fiona-doing, hewie-doing, danger, panic)
  cell field fiona-busy  cell field fiona-mode  cell field fiona-sub
  cell field fiona-cmd  cell field hewie-here  cell field hewie-act  cell field hewie-move-mode
  cell field the-danger  cell field the-panic  cell field the-panic-level
  \ their conditions (the original's +0xC4 each: 0 well, 1 hurt, 2 down), and the stalker as he
  \ tells it (stalker-doing: his room, how he moves and what, condition, health, his alert - his
  \ mode, +0x16C8 - and the exit he came in by, +0x14D4; frames since he told: in the game)
  cell field fiona-cond  cell field hewie-cond  cell field fiona-act
  cell field hewie-cmdable  cell field hewie-near  cell field hewie-reach  cell field hewie-breakable
  cell field hewie-way  cell field hewie-trust-lv
  cell field st-room  cell field st-mode  cell field st-sub  cell field st-cond  cell field st-hp
  cell field st-hp-max  cell field st-alert  cell field st-came  cell field st-age
  cell field st-blow   \ (his attack's blow under way, -1 none: stalker-blow)
  \ the creatures tell it (creature-doing, each frame): each slot's room and move mode, and frames since
  4 cells field arrows-held     \ the menu arrows held (frames: the pad's repeat, for the rooms' dials)
  cell field grab-asked  cell field grab-ok   \ a grab of Hewie from behind asked of the stalker; his answer
  cell field fiona-hit-by        \ who struck or caught her (-1 nobody)
  cell field hewie-hp            \ his health (hewie-health)
  cell field keri-fallen         \ the story's debris fallen through since last counted (keri-fell)
  cell field fiona-shook         \ her shakes counted (count-shakes)
  cell field fiona-shakes-now    \ her shakes this frame (fiona-shaking)
  cell field summoner-secs       \ the summoner's cooldown left (seconds)
  cell field loud-here           \ a noise of $20 or more made in this room lately (frames left)
  cell field fiona-unseen         \ she can't be seen (fiona-unseen: item $89, still)
  10 cells field cr-rooms  10 cells field cr-modes  10 cells field cr-ages  10 cells field cr-ids
end-state
