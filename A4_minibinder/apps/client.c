#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "binder.h"
#include "parcel.h"

#define TRSNACTION_ADD 1
#define TRSNACTION_SUB 2
#define TRSNACTION_MUL 3

static int do_calc(binder_t remote, uint32_t code, int a, int b, const char *name) {
    parcel_t *data  = parcel_obtain();
    parcel_t *reply = NULL;

    set_pos(data, 0);
    write_value(data, &a, sizeof(a));
    write_value(data, &b, sizeof(b));

    printf("[client] %s request: a=%d, b=%d\n", name, a, b);

    if (transact(remote, (int)code, data, &reply) != 0 || !reply) {
        fprintf(stderr, "[client] transact(%s) failed\n", name);
        parcel_recycle(data);
        return -1;
    }

    parcel_recycle(data);  

    int result = 0;
    set_pos(reply, 0);
    read_value(reply, &result, sizeof(result));

    printf("[client] %s reply: result=%d\n", name, result);
    return 0;
}

static int client_main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    binder_t remote;
    token_t  tok;

    str2tok("caculate_service", &tok);
    if (bind(&remote, &tok) != 0) {
        fprintf(stderr, "[client] bind(\"caculate_service\") failed\n");
        detach_process();
        return 1;
    }

    printf("[client] bound to 'caculate service' "
           "(token=\"caculate_service\", handle=%u)\n",
           remote.handle);

    int a = 7;
    int b = 3;

    do_calc(remote, TRSNACTION_ADD, a, b, "TRSNACTION_ADD");

    detach_process();
    return 0;
}

int main(int argc, char **argv) {
    attach_process(client_main, argc, argv);
    return 0;
}