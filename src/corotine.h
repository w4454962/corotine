#pragma once

typedef void (*coro_func)(void*);

struct coro_t {
    void* context;

    coro_func func;
    void* arg;
};

struct coro_t* new_coro(coro_func func, void* arg);

void coro_free(struct coro_t* coro);

void coro_resume(struct coro_t* coro);

void coro_yield(struct coro_t* coro);

struct coro_t* coro_current();

