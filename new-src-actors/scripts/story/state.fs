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
6 constant #chars       \ characters the scripts know by slot: 0 Fiona, 1 Hewie, 2.. the others

state: room-state
  cell field this-room           \ the room these scripts are for
  cell field came-by            \ the exit it was entered by (-1: none)
  #vars cells field vars        \ the script variables (+0x810), cleared on entering
  cell field ebits              \ the event bits (+0x890)
  cell field counter            \ the event counter (+0x703)
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
  cell field camera-char        \ the character slot the camera follows ($FF: nobody)
  \ the action prepared this frame (the scripts' scene-change: +0x1134): the scene (-1 none), its
  \ argument and kind - scene 5 an action script for Fiona, taken with the action button
  cell field prep-scene  cell field prep-arg  cell field prep-kind
  cell field prepared-msg       \ a message prepared for a scene's subtitles (-1 none)
  cell field prepared-page      \ its page shown (0x62 12 turns them; -1 none)
  \ the scene: the movie's class (Progress_PlayMovie's kind: 0 not drawn - a scene's timing and
  \ sound; 1..6 drawn), the cues (+0xBE4 / +0xBE8), a fade running (the screen's)
  cell field movie-kind  cell field cue  cell field cue-prev  cell field fading
  #zones /zone * field zones    \ the zones (all off as phase 1 starts)
  #zones 5 cells * field zone-rects   \ the zone rectangles (an id, x0 z0 x1 z1: for the scene effects)
  \ what the others tell (fiona-doing, hewie-doing, danger, panic)
  cell field fiona-busy  cell field fiona-mode  cell field fiona-sub
  cell field fiona-cmd  cell field hewie-here  cell field hewie-act  cell field hewie-move-mode
  cell field the-danger  cell field the-panic
end-state
