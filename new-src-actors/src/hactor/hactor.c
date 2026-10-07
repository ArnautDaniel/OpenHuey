/* The actor kernel (hactor.h). */
#include "hactor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRIM(name) static void name(Forth *f, Word *w __attribute__((unused)))
#define PUSH(x) forth_push(f, (Cell)(x))
#define POP() forth_pop(f)

#define ROUNDS_MAX 64           /* delivery rounds in one frame before the rest waits */
#define STATE_TYPES_MAX 128
#define BEHAVIOURS_MAX 256
#define MARK ((Cell)0x6B636142)   /* `behaviour` ... `end-behaviour`: what `on` left above it */

typedef struct HKind {
    char name[HNAME_MAX + 1];
    int nargs;
} HKind;

struct HBehaviour {
    char name[HNAME_MAX + 1];
    HBehaviour *parent;
    Word *on[HKINDS_MAX];
    uint8_t reported[HKINDS_MAX / 8];   /* unhandled kinds already reported */
    int unhandled;
};

typedef struct HStateType {
    char name[HNAME_MAX + 1];
    size_t size;
} HStateType;

typedef struct HActor {
    int used;
    char name[HNAME_MAX + 1];
    HBehaviour *beh;
    int type;               /* its state's type (-1: none) */
    uint8_t *state;
    long handled;           /* messages it has handled */
} HActor;

typedef struct HMessage {
    int kind, from, to, n;
    Cell a[HMSG_ARGS];
} HMessage;

static HKind sKinds[HKINDS_MAX];
static int sNKinds;
static HBehaviour *sBehaviours[BEHAVIOURS_MAX];
static int sNBehaviours;
static HStateType sTypes[STATE_TYPES_MAX];
static int sNTypes;
static HActor sActors[HACTOR_MAX];
static uint32_t sSubs[HKINDS_MAX][HACTOR_MAX / 32];
static HMessage *sQueue;
static int sQueueN, sQueueCap;
static int sSelf = -1, sSender = -1;   /* whose message is being handled, and who sent it */
static int sDelivering;
static int sTrace;
static long sFrame;
static long sDeadLetters;   /* messages to actors no longer there */

/* ---- kinds ---- */

static int kind_new(const char *name, size_t len, int nargs) {
    int k;

    for (k = 0; k < sNKinds; k++) {
        if (strlen(sKinds[k].name) == len && strncmp(sKinds[k].name, name, len) == 0) {
            sKinds[k].nargs = nargs;   /* (declared again: a script reloaded) */
            return k;
        }
    }
    if (sNKinds == HKINDS_MAX) {
        return -1;
    }
    snprintf(sKinds[sNKinds].name, sizeof(sKinds[0].name), "%.*s", (int)len, name);
    sKinds[sNKinds].nargs = nargs;
    return sNKinds++;
}

static int kind_find(const char *name, size_t len) {
    int k;

    for (k = 0; k < sNKinds; k++) {
        if (strlen(sKinds[k].name) == len && strncmp(sKinds[k].name, name, len) == 0) {
            return k;
        }
    }
    return -1;
}

/* the next name in the input, as a message kind (an error if it isn't one) */
static int parse_kind(Forth *f, const char *what) {
    size_t len;
    const char *name = forth_parse_name(f, &len);
    int k;

    if (len == 0) {
        forth_error(f, "%s: a message name expected", what);
    }
    k = kind_find(name, len);
    if (k < 0) {
        forth_error(f, "%s: no message %.*s (declare it with `message`)", what, (int)len, name);
    }
    return k;
}

/* ---- the queue ---- */

static void enqueue(int from, int to, int kind, const Cell *args, int n) {
    HMessage *m;

    if (sQueueN == sQueueCap) {
        sQueueCap = sQueueCap ? sQueueCap * 2 : 256;
        sQueue = realloc(sQueue, (size_t)sQueueCap * sizeof(HMessage));
    }
    m = &sQueue[sQueueN++];
    m->kind = kind;
    m->from = from;
    m->to = to;
    m->n = n;
    memcpy(m->a, args, (size_t)n * sizeof(Cell));
}

void hactor_send(int from, int to, int kind, const Cell *args, int n) {
    enqueue(from, to, kind, args, n);
}

static int subscribed(int kind, int id) {
    return (sSubs[kind][id / 32] >> (id % 32)) & 1;
}

void hactor_broadcast(int from, int kind, const Cell *args, int n) {
    int id;

    for (id = 0; id < HACTOR_MAX; id++) {
        if (sActors[id].used && subscribed(kind, id)) {
            enqueue(from, id, kind, args, n);
        }
    }
}

/* ---- delivery ---- */

static const char *actor_name(int id) {
    return id >= 0 && id < HACTOR_MAX && sActors[id].used ? sActors[id].name : id < 0 ? "engine" : "(gone)";
}

static void trace(Forth *f, const HMessage *m) {
    int i;

    forth_printf(f, "%ld  %s -> %s  %s", sFrame, actor_name(m->from), actor_name(m->to), sKinds[m->kind].name);
    for (i = 0; i < m->n; i++) {
        forth_printf(f, " %ld", (long)m->a[i]);
    }
    forth_printf(f, "\n");
}

static void dispatch(Forth *f, const HMessage *m) {
    HActor *a = m->to >= 0 && m->to < HACTOR_MAX ? &sActors[m->to] : NULL;
    HBehaviour *b;
    Word *h = NULL;
    int sp0, fp0, rp0, i, self = sSelf, sender = sSender;

    if (a == NULL || !a->used) {
        sDeadLetters++;
        return;
    }
    for (b = a->beh; b != NULL && h == NULL; b = b->parent) {
        h = b->on[m->kind];
    }
    if (sTrace) {
        trace(f, m);
    }
    if (h == NULL) {
        b = a->beh;
        if (b != NULL && m->kind >= HK_KERNEL_KINDS) {   /* (the kernel's own: ignored if unwanted) */
            b->unhandled++;
            if (!(b->reported[m->kind / 8] & (1 << (m->kind % 8)))) {
                b->reported[m->kind / 8] |= (uint8_t)(1 << (m->kind % 8));
                forth_printf(f, "%s (%s) doesn't handle %s\n", a->name, b->name, sKinds[m->kind].name);
            }
        }
        return;
    }
    sp0 = f->t->sp;
    fp0 = f->t->fp;
    rp0 = f->t->rp;
    for (i = 0; i < m->n; i++) {
        forth_push(f, m->a[i]);
    }
    sSelf = m->to;
    sSender = m->from;
    if (forth_call(f, h) != 0) {
        forth_printf(f, "  (in %s's `on %s`, behaviour %s)\n", a->name, sKinds[m->kind].name, a->beh->name);
    } else if (f->t->sp != sp0 || f->t->fp != fp0) {
        forth_printf(f, "%s's `on %s` left %d cells and %d floats on the stacks\n", a->name,
                     sKinds[m->kind].name, f->t->sp - sp0, f->t->fp - fp0);
    }
    f->t->sp = sp0;   /* (whatever happened, the stacks are as they were) */
    f->t->fp = fp0;
    f->t->rp = rp0;
    a->handled++;
    sSelf = self;
    sSender = sender;
}

/* rounds until the queue is empty (or ROUNDS_MAX) */
static void deliver(Forth *f) {
    int round, n, i;

    sDelivering = 1;
    for (round = 0; round < ROUNDS_MAX && sQueueN > 0; round++) {
        n = sQueueN;
        for (i = 0; i < n; i++) {
            HMessage m = sQueue[i];   /* (a copy: the queue may grow while it is handled) */

            dispatch(f, &m);
        }
        memmove(sQueue, sQueue + n, (size_t)(sQueueN - n) * sizeof(HMessage));
        sQueueN -= n;
    }
    if (sQueueN > 0) {
        forth_printf(f, "actors: %d messages still queued after %d rounds (they wait for the next frame)\n",
                     sQueueN, ROUNDS_MAX);
    }
    sDelivering = 0;
}

void hactor_frame(Forth *f) {
    if (sDelivering) {
        return;
    }
    hactor_broadcast(-1, HK_TICK, NULL, 0);
    deliver(f);
    hactor_broadcast(-1, HK_FRAME_END, NULL, 0);
    deliver(f);
    sFrame++;
}

void hactor_reset(void) {
    int id;

    for (id = 0; id < HACTOR_MAX; id++) {
        free(sActors[id].state);
    }
    memset(sActors, 0, sizeof(sActors));
    memset(sSubs, 0, sizeof(sSubs));
    sQueueN = 0;
    sSelf = sSender = -1;
    sFrame = 0;
    sDeadLetters = 0;
}

/* ---- the words ---- */

static HActor *live(Forth *f, Cell id, const char *what) {
    if (id < 0 || id >= HACTOR_MAX || !sActors[id].used) {
        forth_error(f, "%s: no actor %ld", what, (long)id);
    }
    return &sActors[id];
}

static HBehaviour *behaviour_arg(Forth *f, Cell x, const char *what) {
    int i;

    for (i = 0; i < sNBehaviours; i++) {
        if ((Cell)sBehaviours[i] == x) {
            return sBehaviours[i];
        }
    }
    forth_error(f, "%s: that is not a behaviour", what);
}

/* message name ( a b -- ): the stack comment's inputs are its arguments */
PRIM(p_message) {
    size_t len, tl;
    const char *name = forth_parse_name(f, &len), *t;
    char cname[HNAME_MAX + 1];
    int nargs = 0, k;

    if (len == 0 || len > HNAME_MAX) {
        forth_error(f, "message: a name (up to %d characters) expected", HNAME_MAX);
    }
    t = forth_parse_name(f, &tl);
    if (tl != 1 || t[0] != '(') {
        forth_error(f, "message %.*s: its stack comment expected, ( args -- )", (int)len, name);
    }
    for (;;) {
        t = forth_parse_name(f, &tl);
        if (tl == 0) {
            forth_error(f, "message %.*s: the stack comment isn't closed", (int)len, name);
        }
        if (tl == 2 && t[0] == '-' && t[1] == '-') {
            break;
        }
        if (tl == 1 && t[0] == ')') {
            forth_error(f, "message %.*s: the stack comment needs --", (int)len, name);
        }
        nargs++;
    }
    do {
        t = forth_parse_name(f, &tl);
    } while (tl != 0 && !(tl == 1 && t[0] == ')'));
    if (nargs > HMSG_ARGS) {
        forth_error(f, "message %.*s: at most %d arguments", (int)len, name, HMSG_ARGS);
    }
    k = kind_new(name, len, nargs);
    if (k < 0) {
        forth_error(f, "message: too many kinds");
    }
    snprintf(cname, sizeof(cname), "%.*s", (int)len, name);
    forth_constant(f, cname, k);   /* the name gives its kind */
}

static void send_now(Forth *f, int to, int kind) {
    Cell args[HMSG_ARGS];
    int n = sKinds[kind].nargs, i;

    for (i = n - 1; i >= 0; i--) {
        args[i] = POP();
    }
    if (to >= 0) {
        live(f, to, "send");
        enqueue(sSelf, to, kind, args, n);
    } else {
        hactor_broadcast(sSelf, kind, args, n);
    }
}
PRIM(p_send_rt) {   /* (send) ( args to kind -- ) */
    int kind = (int)POP(), to = (int)POP();

    send_now(f, to, kind);
}
PRIM(p_broadcast_rt) {   /* (broadcast) ( args kind -- ) */
    send_now(f, -1, (int)POP());
}
/* parse the message name; compiling: lay down its kind and `rt`, else run it now */
static void parsing(Forth *f, const char *what, const char *rt) {
    int k = parse_kind(f, what);

    if (f->compiling) {
        forth_compile_literal(f, k);
        forth_compile_word(f, forth_find(f, rt, strlen(rt)));
    } else {
        PUSH(k);
        forth_execute(f, forth_find(f, rt, strlen(rt)));
    }
}
PRIM(p_send) { parsing(f, "send", "(send)"); }
PRIM(p_broadcast) { parsing(f, "broadcast", "(broadcast)"); }

PRIM(p_subscribe_rt) {   /* (subscribe) ( id kind -- ) */
    int kind = (int)POP(), id = (int)POP();

    live(f, id, "subscribe");
    sSubs[kind][id / 32] |= 1u << (id % 32);
}
PRIM(p_unsubscribe_rt) {
    int kind = (int)POP(), id = (int)POP();

    live(f, id, "unsubscribe");
    sSubs[kind][id / 32] &= ~(1u << (id % 32));
}
PRIM(p_subscribe) { parsing(f, "subscribe", "(subscribe)"); }
PRIM(p_unsubscribe) { parsing(f, "unsubscribe", "(unsubscribe)"); }

/* behaviour name ... end-behaviour */
static HBehaviour *sDefining;
PRIM(p_behaviour) {
    size_t len;
    const char *name = forth_parse_name(f, &len);
    char cname[HNAME_MAX + 1];
    HBehaviour *b;
    int i;

    if (len == 0 || len > HNAME_MAX) {
        forth_error(f, "behaviour: a name (up to %d characters) expected", HNAME_MAX);
    }
    snprintf(cname, sizeof(cname), "%.*s", (int)len, name);
    b = NULL;
    for (i = 0; i < sNBehaviours; i++) {   /* (defined again: a script reloaded - start it afresh) */
        if (strcmp(sBehaviours[i]->name, cname) == 0) {
            b = sBehaviours[i];
            memset(b, 0, sizeof(*b));
            snprintf(b->name, sizeof(b->name), "%s", cname);
        }
    }
    if (b == NULL) {
        if (sNBehaviours == BEHAVIOURS_MAX) {
            forth_error(f, "behaviour: too many");
        }
        b = calloc(1, sizeof(*b));
        snprintf(b->name, sizeof(b->name), "%s", cname);
        sBehaviours[sNBehaviours++] = b;
    }
    forth_constant(f, cname, (Cell)b);
    sDefining = b;
    PUSH(MARK);
}
PRIM(p_extends) {
    size_t len;
    const char *name = forth_parse_name(f, &len);
    Word *pw = forth_find(f, name, len);
    HBehaviour *p;

    if (sDefining == NULL) {
        forth_error(f, "extends: only inside behaviour ... end-behaviour");
    }
    if (pw == NULL) {
        forth_error(f, "extends: no behaviour %.*s", (int)len, name);
    }
    forth_execute(f, pw);
    p = behaviour_arg(f, POP(), "extends");
    sDefining->parent = p;
}
/* on kind ... ; - a handler (left on the stack with its kind for end-behaviour) */
PRIM(p_on) {
    int k;

    if (sDefining == NULL) {
        forth_error(f, "on: only inside behaviour ... end-behaviour");
    }
    k = parse_kind(f, "on");
    PUSH(k);
    forth_execute(f, forth_find(f, ":noname", 7));
}
PRIM(p_end_behaviour) {
    Cell x;

    if (sDefining == NULL) {
        forth_error(f, "end-behaviour without behaviour");
    }
    for (;;) {
        x = POP();
        if (x == MARK) {
            break;
        }
        sDefining->on[(int)POP()] = (Word *)x;
    }
    sDefining = NULL;
}
PRIM(p_become) {
    HBehaviour *b = behaviour_arg(f, POP(), "become");

    live(f, sSelf, "become (no actor is being run)")->beh = b;
}

/* state: name ... end-state; field ( type offset size "name" -- type offset' ) */
static void dofield(Forth *f, Word *w) {
    Cell x = w->body[0];
    int type = (int)(x / 0x10000), off = (int)(x % 0x10000);
    HActor *a;

    if (sSelf < 0) {
        forth_error(f, "%s: a field, but no actor is being run", w->name);
    }
    a = &sActors[sSelf];
    if (a->type != type) {
        forth_error(f, "%s: a field of %s, but %s's state is %s", w->name, sTypes[type].name, a->name,
                    a->type >= 0 ? sTypes[a->type].name : "nothing");
    }
    PUSH(a->state + off);
}
PRIM(p_state) {
    size_t len;
    const char *name = forth_parse_name(f, &len);
    char cname[HNAME_MAX + 1];
    int t;

    if (len == 0 || len > HNAME_MAX) {
        forth_error(f, "state: a name expected");
    }
    snprintf(cname, sizeof(cname), "%.*s", (int)len, name);
    for (t = 0; t < sNTypes && strcmp(sTypes[t].name, cname) != 0; t++) {
    }
    if (t == sNTypes) {
        if (sNTypes == STATE_TYPES_MAX) {
            forth_error(f, "state: too many types");
        }
        snprintf(sTypes[sNTypes++].name, sizeof(sTypes[0].name), "%s", cname);
    }
    forth_constant(f, cname, t);
    PUSH(t);
    PUSH(0);
}
PRIM(p_field) {
    size_t len;
    Cell size = POP(), off = POP(), type = forth_pop(f);
    const char *name = forth_parse_name(f, &len);
    char cname[WORD_NAME_MAX + 1];

    if (type < 0 || type >= sNTypes) {
        forth_error(f, "field: not inside state: ... end-state");
    }
    if (len == 0 || len > WORD_NAME_MAX) {
        forth_error(f, "field: a name expected");
    }
    off = (off + 7) & ~(Cell)7;   /* (every field 8-aligned: cells and floats) */
    snprintf(cname, sizeof(cname), "%.*s", (int)len, name);
    forth_constant(f, cname, type * 0x10000 + off)->code = dofield;
    PUSH(type);
    PUSH(off + size);
}
PRIM(p_end_state) {
    Cell size = POP(), type = POP();

    if (type < 0 || type >= sNTypes) {
        forth_error(f, "end-state without state:");
    }
    sTypes[type].size = (size_t)size;
}

/* spawn ( behaviour type addr len -- id ): a new actor, its state zeroed */
PRIM(p_spawn) {
    Cell len = POP(), addr = POP(), type = POP();
    HBehaviour *b = behaviour_arg(f, POP(), "spawn");
    int id;

    if (type < -1 || type >= sNTypes) {
        forth_error(f, "spawn: no state type %ld", (long)type);
    }
    for (id = 0; id < HACTOR_MAX && sActors[id].used; id++) {
    }
    if (id == HACTOR_MAX) {
        forth_error(f, "spawn: too many actors");
    }
    memset(&sActors[id], 0, sizeof(HActor));
    sActors[id].used = 1;
    snprintf(sActors[id].name, sizeof(sActors[id].name), "%.*s", (int)len, (const char *)addr);
    sActors[id].beh = b;
    sActors[id].type = (int)type;
    sActors[id].state = calloc(1, type >= 0 && sTypes[type].size > 0 ? sTypes[type].size : 8);
    enqueue(sSelf, id, HK_SPAWNED, NULL, 0);
    PUSH(id);
}
PRIM(p_kill) {
    int id = (int)POP(), k;
    HActor *a = live(f, id, "kill");

    free(a->state);
    memset(a, 0, sizeof(*a));
    for (k = 0; k < HKINDS_MAX; k++) {
        sSubs[k][id / 32] &= ~(1u << (id % 32));
    }
}
PRIM(p_self) { PUSH(sSelf); }
/* the console inside an actor: its fields, `self`, `become` and sends as that actor's (until
 * `leave`); messages delivered meanwhile still run as their own actors */
PRIM(p_enter) {
    int id = (int)POP();

    if (sDelivering) {
        forth_error(f, "enter: not from a handler");
    }
    live(f, id, "enter");
    sSelf = id;
    sSender = -1;
}
PRIM(p_leave) { sSelf = -1; sSender = -1; }
PRIM(p_sender) { PUSH(sSender); }
PRIM(p_alive) { Cell id = POP(); PUSH(id >= 0 && id < HACTOR_MAX && sActors[id].used ? -1 : 0); }
PRIM(p_actor_name) {   /* ( id -- addr len ) */
    HActor *a = live(f, POP(), "actor-name");

    PUSH(a->name);
    PUSH(strlen(a->name));
}
PRIM(p_behaviour_of) { PUSH(live(f, POP(), "behaviour-of")->beh); }
PRIM(p_frame) { hactor_frame(f); }
PRIM(p_deliver) {
    if (sDelivering) {
        forth_error(f, "deliver: already delivering (not from a handler)");
    }
    deliver(f);
}
PRIM(p_reset) {
    if (sDelivering) {
        forth_error(f, "actors-reset: not from a handler");
    }
    hactor_reset();
}
PRIM(p_trace_on) { sTrace = 1; }
PRIM(p_trace_off) { sTrace = 0; }
PRIM(p_queued) { PUSH(sQueueN); }
PRIM(p_actor_frame) { PUSH(sFrame); }
PRIM(p_dot_actors) {
    int id, k, n;

    for (id = 0; id < HACTOR_MAX; id++) {
        HActor *a = &sActors[id];

        if (!a->used) {
            continue;
        }
        for (n = 0, k = 0; k < sQueueN; k++) {
            n += sQueue[k].to == id;
        }
        forth_printf(f, "%3d %-20s %-16s %-16s %ld handled, %d queued\n", id, a->name, a->beh ? a->beh->name : "-",
                     a->type >= 0 ? sTypes[a->type].name : "-", a->handled, n);
    }
    forth_printf(f, "%ld dead letters\n", sDeadLetters);
}
PRIM(p_dot_unhandled) {
    int i;

    for (i = 0; i < sNBehaviours; i++) {
        if (sBehaviours[i]->unhandled) {
            forth_printf(f, "%-20s %d unhandled\n", sBehaviours[i]->name, sBehaviours[i]->unhandled);
        }
    }
}
PRIM(p_f_to_cell) {   /* f>cell ( F: x -- ) ( -- c ): a float to travel in a message */
    double d = forth_fpop(f);
    Cell c;

    memcpy(&c, &d, sizeof(c));
    PUSH(c);
}
PRIM(p_cell_to_f) {
    Cell c = POP();
    double d;

    memcpy(&d, &c, sizeof(d));
    forth_fpush(f, d);
}

void bind_hactor(Forth *f) {
    static const struct { const char *name; Code code; int immediate; } words[] = {
        {"message", p_message, 0}, {"(send)", p_send_rt, 0}, {"(broadcast)", p_broadcast_rt, 0},
        {"send", p_send, 1}, {"broadcast", p_broadcast, 1},
        {"(subscribe)", p_subscribe_rt, 0}, {"(unsubscribe)", p_unsubscribe_rt, 0},
        {"subscribe", p_subscribe, 1}, {"unsubscribe", p_unsubscribe, 1},
        {"behaviour", p_behaviour, 0}, {"extends", p_extends, 0}, {"on", p_on, 0},
        {"end-behaviour", p_end_behaviour, 0}, {"become", p_become, 0},
        {"state:", p_state, 0}, {"field", p_field, 0}, {"end-state", p_end_state, 0},
        {"spawn", p_spawn, 0}, {"kill", p_kill, 0}, {"self", p_self, 0}, {"enter", p_enter, 0}, {"leave", p_leave, 0}, {"sender", p_sender, 0},
        {"alive?", p_alive, 0}, {"actor-name", p_actor_name, 0}, {"behaviour-of", p_behaviour_of, 0},
        {"actors-frame", p_frame, 0}, {"deliver", p_deliver, 0}, {"actors-reset", p_reset, 0},
        {"trace-on", p_trace_on, 0}, {"trace-off", p_trace_off, 0}, {"queued", p_queued, 0},
        {"actors-frames", p_actor_frame, 0}, {".actors", p_dot_actors, 0}, {".unhandled", p_dot_unhandled, 0},
        {"f>cell", p_f_to_cell, 0}, {"cell>f", p_cell_to_f, 0},
    };
    Vocab *saved = f->m.current, *v = forth_vocab(f, "actors");
    size_t i;

    forth_set_current(f, v);   /* scripts say USING: actors ; */
    v->state = VOCAB_LOADED;
    for (i = 0; i < sizeof(words) / sizeof(words[0]); i++) {
        Word *wd = forth_prim(f, words[i].name, words[i].code);

        if (words[i].immediate) {
            wd->flags |= WORD_IMMEDIATE;
        }
    }
    forth_constant(f, "nobody", -1);
    if (sNKinds == 0) {
        kind_new("tick", 4, 0);
        kind_new("frame-end", 9, 0);
        kind_new("spawned", 7, 0);
        kind_new("killed", 6, 0);
    }
    forth_constant(f, "tick", HK_TICK);
    forth_constant(f, "frame-end", HK_FRAME_END);
    forth_constant(f, "spawned", HK_SPAWNED);
    forth_constant(f, "killed", HK_KILLED);
    forth_set_current(f, saved);
}
