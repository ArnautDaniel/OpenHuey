/* What differs between the Linux and the Windows (MinGW) builds of hg: backtraces, making a
 * directory, aligned allocations, loading a shared library. */
#ifndef HG_PORT_H
#define HG_PORT_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#include <malloc.h>
#include <windows.h>

/* the caller's frames (addresses only), skipping `skip`, to stderr */
static inline void hg_backtrace(int skip, int max) {
    void *frames[32];
    int n = CaptureStackBackTrace((DWORD)(skip + 1), (DWORD)(max < 32 ? max : 32), frames, NULL), i;

    for (i = 0; i < n; i++) {
        fprintf(stderr, "  %p\n", frames[i]);
    }
}

static inline int hg_mkdir(const char *path) { return _mkdir(path); }

/* every allocation of the game's heap functions is aligned, so that one free fits all */
static inline void *hg_aligned_alloc(size_t align, size_t size) { return _aligned_malloc(size, align); }
static inline void hg_aligned_free(void *p) { _aligned_free(p); }

static inline void *hg_dlopen(const char *name) { return (void *)LoadLibraryA(name); }
static inline void *hg_dlsym(void *lib, const char *name) { return (void *)GetProcAddress((HMODULE)lib, name); }
#else
#include <dlfcn.h>
#include <execinfo.h>
#include <sys/stat.h>
#include <sys/types.h>

static inline void hg_backtrace(int skip, int max) {
    void *frames[32];
    int n = backtrace(frames, max + skip + 1 < 32 ? max + skip + 1 : 32);

    if (n > skip + 1) {
        backtrace_symbols_fd(frames + skip + 1, n - skip - 1, 2);
    }
}

static inline int hg_mkdir(const char *path) { return mkdir(path, 0777); }

static inline void *hg_aligned_alloc(size_t align, size_t size) {
    void *p = NULL;

    return posix_memalign(&p, align, size) == 0 ? p : NULL;
}
static inline void hg_aligned_free(void *p) { free(p); }

static inline void *hg_dlopen(const char *name) { return dlopen(name, RTLD_NOW | RTLD_GLOBAL); }
static inline void *hg_dlsym(void *lib, const char *name) { return dlsym(lib, name); }
#endif

#endif
