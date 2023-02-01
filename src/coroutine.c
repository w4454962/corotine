#include "cvector.h"
#include "cvector_utils.h"

#include "coroutine.h"
#include <stdlib.h>
#include "libcoro/coro.h"

_declspec(thread) cvector_vector_type(struct coro_t*) fiber_pool;

struct coro_context_t {
    coro_context ctx, mainctx;
    struct coro_stack stack;
};


void coro_run(void* ptr) {
    struct coro_t* coro = (struct coro_t*)ptr;

    coro->func(coro->arg);

    coro_yield(coro);
}

struct coro_t* new_coro(coro_func func, void* arg) {
    struct coro_t* coro = (struct coro_t*)malloc(sizeof(struct coro_t));

    coro->func = func;

    coro->arg = arg;

    struct coro_context_t* context = (struct coro_context_t*)malloc(sizeof(struct coro_context_t));

    coro->context = context;

    coro_create(&context->ctx, NULL, NULL, NULL, 0);

    coro_stack_alloc(&context->stack, 0);

    coro_create(&context->ctx, coro_run, coro, context->stack.sptr, context->stack.ssze);

    return coro;
}

void coro_resume(struct coro_t* coro) {
    struct coro_context_t* context = coro->context;

    cvector_push_back(fiber_pool, coro);

    coro_transfer(&context->mainctx, &context->ctx);
}

void coro_yield(struct coro_t* coro) {
    struct coro_context_t* context = coro->context;

    cvector_pop_back(fiber_pool);

    coro_transfer(&context->ctx, &context->mainctx);
}

void coro_free(struct coro_t* coro) {
    struct coro_context_t* context = coro->context;

    coro_stack_free(&context->stack);

    coro_destroy(&context->ctx);

    free(context);

    free(coro);
}

struct coro_t* coro_current() {
    size_t size = cvector_size(fiber_pool);

    if (size) {
        return fiber_pool[size - 1];
    }
    return NULL;
}

