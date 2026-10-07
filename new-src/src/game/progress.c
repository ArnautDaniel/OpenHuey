/* The game's state for the event scripts: see progress.h. */
#include "progress.h"

#include <stddef.h>

Progress gProgress;
EventState gEvents;

static void dofield(Forth *f, Word *w) {   /* a field word: ( addr -- addr+offset ) */
    forth_push(f, forth_pop(f) + w->body[0]);
}

static void field(Forth *f, const char *name, size_t offset) {
    forth_constant(f, name, (Cell)offset)->code = dofield;
}

static void p_progress(Forth *f, Word *w) { (void)w; forth_push(f, (Cell)&gProgress); }
static void p_event_state(Forth *f, Word *w) { (void)w; forth_push(f, (Cell)&gEvents); }

void bind_state(Forth *f) {
    Vocab *saved = f->m.current, *v = forth_vocab(f, "game-state");

    forth_set_current(f, v);   /* scripts say USING: game-state ; */
    v->state = VOCAB_LOADED;
    forth_prim(f, "progress", p_progress);
    forth_prim(f, "event-state", p_event_state);
    /* sizes */
    forth_constant(f, "story-flags", STORY_FLAGS);
    forth_constant(f, "state-flags", STATE_FLAGS);
    forth_constant(f, "progress-vars", PROGRESS_VARS);
    forth_constant(f, "resident-flags", RESIDENT_FLAGS);
    forth_constant(f, "room-count", ROOM_COUNT);
    forth_constant(f, "characters", CHARACTERS);
    forth_constant(f, "script-slots", SCRIPT_SLOTS);
    forth_constant(f, "script-vars", SCRIPT_VARS);
    forth_constant(f, "/slot", sizeof(ScriptSlot));
    forth_constant(f, "/char", sizeof(ScriptChar));
    /* Progress: arrays of 32-bit words of flags (l@ l!), the variables bytes (c@ c!) */
    field(f, "pr.story", offsetof(Progress, story));
    field(f, "pr.state", offsetof(Progress, state));
    field(f, "pr.vars", offsetof(Progress, vars));
    field(f, "pr.resident", offsetof(Progress, resident));
    field(f, "pr.visited", offsetof(Progress, visited));
    /* EventState: 32-bit (sl@ l!) */
    field(f, "ev.room", offsetof(EventState, room));
    field(f, "ev.vars", offsetof(EventState, vars));
    field(f, "ev.bits", offsetof(EventState, bits));
    field(f, "ev.counter", offsetof(EventState, counter));
    field(f, "ev.exit", offsetof(EventState, exit));
    field(f, "ev.room-frames", offsetof(EventState, room_frames));
    field(f, "ev.result", offsetof(EventState, result));
    field(f, "ev.message", offsetof(EventState, message));
    field(f, "ev.message-owner", offsetof(EventState, message_owner));
    field(f, "ev.slot", offsetof(EventState, slot));
    field(f, "ev.self-id", offsetof(EventState, self_id));
    field(f, "ev.self-frames", offsetof(EventState, self_frames));
    field(f, "ev.self-char", offsetof(EventState, self_char));
    field(f, "ev.leaving", offsetof(EventState, leaving));
    field(f, "ev.camera-char", offsetof(EventState, camera_char));
    field(f, "ev.slots", offsetof(EventState, slots));
    field(f, "ev.chars", offsetof(EventState, chars));
    field(f, "slot.task", offsetof(ScriptSlot, task));
    field(f, "slot.who", offsetof(ScriptSlot, who));
    field(f, "slot.id", offsetof(ScriptSlot, id));
    field(f, "slot.frames", offsetof(ScriptSlot, frames));
    field(f, "char.present", offsetof(ScriptChar, present));
    field(f, "char.id", offsetof(ScriptChar, id));
    field(f, "char.room", offsetof(ScriptChar, room));
    field(f, "char.scripted", offsetof(ScriptChar, scripted));
    field(f, "char.actor", offsetof(ScriptChar, actor));
    field(f, "char.cam-set", offsetof(ScriptChar, cam_set));
    field(f, "char.cam-path", offsetof(ScriptChar, cam_path));
    field(f, "char.pos", offsetof(ScriptChar, pos));      /* 3 floats: sf@ */
    field(f, "char.prev", offsetof(ScriptChar, prev));
    forth_set_current(f, saved);
}
