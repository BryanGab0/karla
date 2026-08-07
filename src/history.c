#include "karla.h"

static char *ring[MAX_HISTORY];
static int   head  = 0;
static int   count = 0;

void history_add(const char *line) {
    free(ring[head]);
    ring[head] = strdup(line);
    head = (head + 1) % MAX_HISTORY;
    if (count < MAX_HISTORY)
        count++;
}

void history_print(void) {
    if (count == 0) {
        printf("(no history)\n");
        return;
    }
    int start = (head - count + MAX_HISTORY) % MAX_HISTORY;
    for (int i = 0; i < count; i++) {
        int idx = (start + i) % MAX_HISTORY;
        printf("  %3d  %s\n", i + 1, ring[idx]);
    }
}

int history_count(void) {
    return count;
}