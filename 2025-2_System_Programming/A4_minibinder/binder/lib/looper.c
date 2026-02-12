#include "looper.h"

struct task {
    void   (*fn)(void *arg);
    void   *arg;
};

#define MAX_TASKS 32
static struct task queue[MAX_TASKS]; 
static size_t head = 0;
static size_t tail = 0;
static int looper = 0;

static int _is_empty(void) { return head == tail; }
static int _is_full(void) { return ((tail + 1) % MAX_TASKS) == head; }

static int _next(struct task *out) {
    if (_is_empty()) return 0;
    if (out) *out = queue[head];  
    head = (head + 1) % MAX_TASKS;
    return 1;
}

void looper_init(void) {
    head = 0;
    tail = 0;
    looper = 0;
}

void looper_run(void) {
    looper = 1;
    while (looper) {
        struct task t;
        while (_next(&t)) if (t.fn) t.fn(t.arg); 
    }
}

void looper_quit(void) {
    looper = 0;
}

int post(void (*fn)(void *arg), void *arg) {
    if (!fn) return -1;
    if (_is_full()) { return -1; }
    queue[tail].fn = fn;
    queue[tail].arg = arg;
    tail = (tail + 1) % MAX_TASKS;
    return 0;
}