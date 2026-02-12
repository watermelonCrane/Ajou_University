#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "binder.h"
#include "parcel.h"

#define TRSNACTION_ADD 1
#define TRSNACTION_SUB 2
#define TRSNACTION_MUL 3

static int caculate_service_on_transact(binder_t *self,
                                        uint32_t  code,
                                        parcel_t *data,
                                        parcel_t *reply) {
    (void)self; 

    int a = 0, b = 0, result = 0;

    set_pos(data, 0);
    read_value(data, &a, sizeof(a));
    read_value(data, &b, sizeof(b));

    switch (code) {
    case TRSNACTION_ADD:
        result = a + b;
        printf("[server] ADD: %d + %d = %d\n", a, b, result);
        break;
    case TRSNACTION_SUB:
        result = a - b;
        printf("[server] SUB: %d - %d = %d\n", a, b, result);
        break;
    case TRSNACTION_MUL:
        result = a * b;
        printf("[server] MUL: %d * %d = %d\n", a, b, result);
        break;
    default:
        printf("[server] unknown code: %u\n", code);
        return -1;
    }

    set_pos(reply, 0);
    write_value(reply, &result, sizeof(result));
    return 0;
}

static int server_main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    binder_t svc;
    token_t  tok;

    str2tok("caculate_service", &tok);
    binder_init(&svc, tok, 0);
    svc.on_transact = caculate_service_on_transact;

    if (binder_register(&svc) != 0) {
        fprintf(stderr, "[server] binder_register() failed\n");
        detach_process();
        return 1;
    }

    printf("[server] registered 'caculate service' "
           "(token=\"caculate_service\", handle=%u)\n",
           svc.handle);
    printf("[server] waiting for requests...\n");

    return 0;
}

int main(int argc, char **argv) {
    attach_process(server_main, argc, argv);
    return 0;
}