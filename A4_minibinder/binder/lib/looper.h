#ifndef LOOPER_H
#define LOOPER_H

#include <stddef.h>

/*
 * Minimal single-threaded event loop used by MiniBinder.
 * Binder thread pushes work items into this looper; user code runs inside it.
 */

/*
 * looper_init
 *
 * Initialize internal queue and state.
 * Must be called once before post()/looper_run().
 */
void looper_init(void);

/*
 * looper_run
 *
 * Enter the event loop.
 * Repeatedly takes pending tasks posted via post() and executes them.
 * Blocks until looper_quit() is called.
 */
void looper_run(void);

/*
 * looper_quit
 *
 * Request termination of the event loop.
 * Causes looper_run() to eventually return.
 */
void looper_quit(void);

/*
 * post
 *
 * Schedule a function to be executed on the looper thread.
 *
 * Parameters:
 *   fn  - function pointer of type void (*)(void*)
 *   arg - opaque pointer passed to fn when executed
 *
 * Returns:
 *   0 on success, negative value on failure.
 */
int post(void (*fn)(void *arg), void *arg);

#endif /* LOOPER_H */