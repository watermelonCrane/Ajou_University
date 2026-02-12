// tests/parcel_test.c
#include <stdio.h>
#include <string.h>
#include "parcel.h"
#include "binder.h"

int main(void)
{
    parcel_t p = *parcel_obtain();
    printf("[test] parcel_init: pos=%zu len=%zu\n", p.pos, p.len);

    /* 1) 일반 값 쓰기 */
    int v = 1234;
    write_value(&p, &v, sizeof(v));
    printf("[test] wrote int=%d, pos=%zu len=%zu\n", v, p.pos, p.len);

    /* 2) 토큰 쓰기 */
    token_t tok1;
    memset(tok1, 0, sizeof(tok1));
    str2tok("service.alpha", &tok1);
    write_token(&p, tok1);
    printf("[test] wrote token='%s', pos=%zu len=%zu\n", tok1, p.pos, p.len);

    /* 3) 바인더 쓰기 */
    token_t tok2;
    memset(tok2, 0, sizeof(tok2));
    str2tok("binder.demo", &tok2);

    binder_t b;
    binder_init(&b, tok2, 77);
    write_binder(&p, b);
    printf("[test] wrote binder token='%s' handle=%u, pos=%zu len=%zu\n",
           b.token, b.handle, p.pos, p.len);

    /* 오프셋 정보 확인 (얼마나 들어갔나) */
    printf("[test] binder_len=%zu\n", p.binder_len);

    /* ==== 이제 읽기 ==== */
    set_pos(&p, 0);
    parcel_dump(&p);

    int v2 = 0;
    read_value(&p, &v2, sizeof(v2));
    printf("[test] read  int=%d\n", v2);

    token_t rtok;
    memset(rtok, 0, sizeof(rtok));
    read_token(&p, &rtok);
    printf("[test] read  token='%s'\n", rtok);

    binder_t rb;
    read_binder(&p, &rb);
    printf("[test] read  binder token='%s' handle=%u\n",
           rb.token, rb.handle);

    parcel_dump(&p);
    /* 간단 검증 */
    if (v == v2 && rb.handle == 77 && strcmp(rtok, tok1) == 0) {
        printf("[test] ✅ OK\n");
    } else {
        printf("[test] ❌ FAIL\n");
    }

    parcel_recycle(&p);
    parcel_dump(&p);
    return 0;
}