#pragma once

#define CORO_API __stdcall 

typedef void (CORO_API *coro_func_t)(void*);

struct coro_t {
    void* context;

    coro_func_t func;
    void* arg;
};

struct coro_t* CORO_API new_coro(coro_func_t func, void* arg);

void CORO_API coro_free(struct coro_t* coro);

void CORO_API coro_resume(struct coro_t* coro);

void CORO_API coro_yield(struct coro_t* coro);

struct coro_t* CORO_API coro_current();

