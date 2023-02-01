#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "corotine.h"

void test_func(void* arg) {
    printf("2. yield\n");
  
    coro_yield(coro_current());
     
    printf("4. finish\n");
}
  
void test() {
    printf("1. create\n");
    struct coro_t* coro = new_coro(test_func, NULL);

    coro_resume(coro);

    printf("3. resume\n");

    coro_resume(coro);

    printf("5. end\n");

    coro_free(coro);

    printf("6. delete\n");
}
int main() {
    test();

    return 0;
}

