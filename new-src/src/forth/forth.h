/* The Forth system: a small, jonesforth-style Forth written in C, shaped for game scripting.
 *
 * - Indirect threaded. A word is a Word struct in the dictionary: its `code` is the C function
 *   that runs it. A colon definition's code is docol and its body is a list of Word pointers.
 * - Cells are intptr_t, and addresses are real pointers, so Forth can read and write C structs
 *   directly (with the field-offset words the bindings define).
 * - Floats live on their own stack (doubles); memory floats are 32-bit (sf@ sf!) like the game's.
 * - Every running piece of Forth is a Task with its own stacks. The interpreter is one task;
 *   game scripts spawn more, which run a little each frame and give way with `yield` or `wait`.
 * - Errors unwind to the nearest catch point (the interpreter prompt, or the task, which dies).
 * - Words live in vocabularies (as in Factor). A file says where its definitions go (`IN: name`)
 *   and which vocabularies it uses (`USING: a b c ;`), loading them from their files
 *   (scripts/a.fs, scripts/a/b.fs for a.b) as needed. It sees only those, its own words and the
 *   core (`forth`); two used vocabularies defining one name is an error (say `vocab:name`).
 *   The interactive prompt sees every loaded vocabulary.
 *
 * Most of the language (if/then, loops, variables...) is defined in Forth on top of these
 * primitives, in scripts/prelude.fs. See docs/forth.md for the dialect. */
#ifndef FORTH_H
#define FORTH_H

#include <setjmp.h>
#include <stddef.h>
#include <stdint.h>

typedef intptr_t Cell;
typedef uintptr_t UCell;
typedef double Float;

typedef struct Forth Forth;
typedef struct Word Word;
typedef struct Task Task;
typedef struct Vocab Vocab;

/* what executing a word does */
typedef void (*Code)(Forth *f, Word *w);

enum {
    WORD_IMMEDIATE = 1,   /* runs even while compiling */
    WORD_HIDDEN = 2,      /* not found by name (a definition being compiled) */
};

#define WORD_NAME_MAX 31

struct Word {
    Word *link;     /* the previous word in its vocabulary */
    Vocab *vocab;
    Code code;
    Cell *does;     /* the threaded code after DOES>, for words made by a defining word */
    uint8_t flags;
    uint8_t len;
    uint8_t operands;   /* inline cells after it in threaded code (for `see`); 0xFF: a string */
    char name[WORD_NAME_MAX + 1];
    Cell body[];    /* the parameter field: threaded code, a variable's value, ... */
};

enum { VOCAB_NEW, VOCAB_LOADING, VOCAB_LOADED };

#define VOCAB_NAME_MAX 63

struct Vocab {
    char name[VOCAB_NAME_MAX + 1];
    Word *latest;       /* its newest word */
    int state;          /* VOCAB_*: its file loaded? */
    Vocab *parent;      /* for name.private: name */
    Vocab *next;        /* all vocabularies, newest first */
};

#define ORDER_MAX 32

/* what a piece of source sees: where definitions go, and the vocabularies it uses */
typedef struct Manifest {
    Vocab *current;
    Vocab *order[ORDER_MAX];
    int norder;
    int listener;       /* the prompt: every loaded vocabulary is visible */
} Manifest;

#define STACK_CELLS 256
#define FSTACK_CELLS 64

enum { TASK_READY, TASK_WAITING, TASK_DONE };

struct Task {
    Cell ds[STACK_CELLS];   /* data stack (grows up; sp = number of items) */
    Cell rs[STACK_CELLS];   /* return stack */
    Float fs[FSTACK_CELLS]; /* float stack */
    int sp, rp, fp;
    int marks[16], nmarks;  /* `{`: where each list being gathered starts on the data stack */
    Cell *ip;               /* the next cell of threaded code to run; NULL when stopped */
    Cell boot[2];           /* a task's first code: its word, then halt */
    int state;
    long wake;              /* TASK_WAITING: the frame it runs again */
    int id;
    char name[WORD_NAME_MAX + 1];
    Task *next;             /* the scheduler's list */
};

/* text being interpreted (a string, or a file's contents) */
typedef struct Source {
    const char *text;
    size_t len, pos;
    const char *name;   /* for error messages */
    int line;
    char *owned;        /* freed when done (a file read by include) */
} Source;

#define SOURCE_DEPTH 16

typedef void (*OutputFn)(void *ctx, const char *s, size_t n);

struct Forth {
    /* the dictionary: one block of memory; `here` is the next free byte */
    uint8_t *mem, *here, *end;
    Word *latest;           /* the newest word (in any vocabulary) */
    Vocab *vocabs;          /* every vocabulary */
    Vocab *core;            /* `forth`: always visible */
    Manifest m;             /* the current source's view */
    Manifest listener;      /* the prompt's (kept between lines) */
    char roots[4][512];     /* where vocabularies' files are looked for */
    int nroots;
    Cell compiling;         /* STATE: 0 interpreting, -1 compiling */
    Cell base;              /* number base */
    int bye;                /* `bye` was run: the host should stop */

    Task *t;                /* the task running now */
    Task *main;             /* the interpreter's task */
    Task *tasks;            /* spawned tasks */
    int next_task_id;
    long frame;             /* advanced by forth_run_tasks */
    int depth;              /* nested forth_execute calls (yield may only leave the outermost) */
    int task_depth;         /* the depth a task runs at */
    int yielded;

    Source src[SOURCE_DEPTH];
    int nsrc;

    OutputFn out;
    void *out_ctx;
    char numbuf[128];

    jmp_buf *catch;         /* where an error unwinds to */
    int silent;             /* inside `catch`: errors are not printed */
    char error[256];        /* the last error message */

    /* words the compiler uses */
    Word *w_lit, *w_flit, *w_litstring, *w_exit, *w_halt, *w_comma, *w_does, *w_type;
    char sbuf[16][256];     /* s" in interpret mode: rotating buffers (copy what must last) */
    int nsbuf;
};

/* life */
Forth *forth_new(size_t dict_bytes);
void forth_free(Forth *f);
void forth_set_output(Forth *f, OutputFn out, void *ctx);

/* interpret text; 0 on success, -1 after an error (the message has been printed) */
int forth_eval(Forth *f, const char *text, size_t len, const char *name);
int forth_include(Forth *f, const char *path);

/* vocabularies: find or make one; make it where definitions go; add a folder to look for
 * vocabularies' files in (scripts/) */
Vocab *forth_vocab(Forth *f, const char *name);
void forth_set_current(Forth *f, Vocab *v);
void forth_add_root(Forth *f, const char *dir);

/* find a word by name (case-insensitive) as the current source sees it; NULL if there is none */
Word *forth_find(Forth *f, const char *name, size_t len);
/* run a word (from C); errors propagate to the caller's catch point */
void forth_execute(Forth *f, Word *w);
/* run a word from C with its own catch point: 0 ok, -1 error (printed) */
int forth_call(Forth *f, Word *w);

/* defining words from C */
Word *forth_prim(Forth *f, const char *name, Code code);
Word *forth_constant(Forth *f, const char *name, Cell value);
Word *forth_fconstant(Forth *f, const char *name, Float value);

/* for words that parse: the next name in the input (length 0 at the end) */
const char *forth_parse_name(Forth *f, size_t *len);
/* lay down a literal in the definition being compiled */
void forth_compile_literal(Forth *f, Cell x);

/* tasks: run a word as a task; each frame forth_run_tasks gives every ready task a turn */
Task *forth_spawn(Forth *f, Word *w);
void forth_run_tasks(Forth *f);

/* lists: growable arrays of cells, made by `list` or `{ ... }`; an address on the stack. They
 * live until `list-free` (lists made while loading scripts usually live for good) */
typedef struct List {
    Cell n, cap;
    Cell *items;
} List;

/* the stacks, for primitives */
void forth_push(Forth *f, Cell x);
Cell forth_pop(Forth *f);
void forth_fpush(Forth *f, Float x);
Float forth_fpop(Forth *f);

/* report an error: prints it and unwinds to the catch point (does not return) */
_Noreturn void forth_error(Forth *f, const char *fmt, ...);

/* output */
void forth_type(Forth *f, const char *s, size_t n);
void forth_printf(Forth *f, const char *fmt, ...);

#endif
