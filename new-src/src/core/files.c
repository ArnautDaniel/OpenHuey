#include "files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char sRoot[1024] = ".";

void files_set_root(const char *dir) {
    snprintf(sRoot, sizeof(sRoot), "%s", dir);
}

const char *files_root(void) {
    return sRoot;
}

static void full_path(char *out, size_t n, const char *path) {
    char *p;

    snprintf(out, n, "%s/%s", sRoot, path);
    for (p = out; *p != 0; p++) {   /* the game writes DOS paths */
        if (*p == '\\') {
            *p = '/';
        }
    }
}

uint8_t *files_read(const char *path, size_t *size) {
    char full[1200];
    FILE *fp;
    uint8_t *buf;
    long n;

    full_path(full, sizeof(full), path);
    fp = fopen(full, "rb");
    if (fp == NULL) {
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    n = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buf = malloc((size_t)n + 1);
    if (buf == NULL || fread(buf, 1, (size_t)n, fp) != (size_t)n) {
        free(buf);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    buf[n] = 0;
    if (size != NULL) {
        *size = (size_t)n;
    }
    return buf;
}

int files_exist(const char *path) {
    char full[1200];
    FILE *fp;

    full_path(full, sizeof(full), path);
    fp = fopen(full, "rb");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}
