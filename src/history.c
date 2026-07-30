#include "karla.h"

/*
 * history.c - Fixed-size command history backed by a ring buffer.
 *
 * `head` marks the next slot to write; the buffer wraps with modulo, so the
 * oldest entry is silently overwritten once MAX_HISTORY is reached.
 */

static char *ring[MAX_HISTORY];
static int   head  = 0;             /* next write position     */
static int   count = 0;             /* entries stored (caps at MAX_HISTORY) */

void history_add(const char *line) {
    free(ring[head]);               /* drop the entry we're about to replace */
    ring[head] = strdup(line);      /* own a private copy; caller frees line */
    head = (head + 1) % MAX_HISTORY;
    if (count < MAX_HISTORY)
        count++;
}

void history_print(void) {
    if (count == 0) {
        printf("(no history)\n");
        return;
    }
    /* Oldest entry sits `count` slots behind head; +MAX_HISTORY keeps the
     * result non-negative before the modulo. */
    int start = (head - count + MAX_HISTORY) % MAX_HISTORY;
    for (int i = 0; i < count; i++) {
        int idx = (start + i) % MAX_HISTORY;
        printf("  %3d  %s\n", i + 1, ring[idx]);
    }
}

int history_count(void) {
    return count;
}