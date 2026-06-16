#include <stdio.h>
#include <stdlib.h>

typedef struct Context Context;
typedef void (*State)(Context *);

struct Context {
    int x;
    State next;
};

void state_init(Context *context);
void state_count(Context *context);
void state_even(Context *context);
void state_odd(Context *context);
void state_done(Context *context);

/* 状態関数を呼び続けて、Context.next が指す次の状態へ遷移する */
int main(void) {
    Context context = {
        .x = 0,
        .next = state_init
    };

dispatch:
    context.next(&context);
    goto dispatch;

    return 0;
}

/* 初期状態: カウンタをリセットし、数値判定状態へ進める */
void state_init(Context *context) {
    puts("init");

    context->x = 0;
    context->next = state_count;
}

/* カウント状態: x の偶奇を配列の添字にして分岐先を選ぶ */
void state_count(Context *context) {
    static State branch[2] = {
        state_even,
        state_odd
    };

    context->next = branch[context->x & 1];
}

/* 偶数状態: 現在値を出力し、100 未満ならカウント状態へ戻る */
void state_even(Context *context) {
    char buf[100];

    static State next_state[2] = {
        state_done,
        state_count
    };

    sprintf(buf, "%d: even", context->x);
    puts(buf);

    context->x++;

    context->next = next_state[context->x < 100];
}

/* 奇数状態: 現在値を出力し、100 未満ならカウント状態へ戻る */
void state_odd(Context *context) {
    char buf[100];

    static State next_state[2] = {
        state_done,
        state_count
    };

    sprintf(buf, "%d: odd", context->x);
    puts(buf);

    context->x++;

    context->next = next_state[context->x < 100];
}

/* 終了状態: ステートマシンを抜けてプロセスを終了する */
void state_done(Context *context) {
    (void)context;

    puts("done");
    exit(0);
}
