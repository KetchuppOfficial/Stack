//----------------------------------------------------------------
#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "Log_File.h"

#define STACK_CTOR(ptr)                                                                             \
{                                                                                                   \
    MY_ASSERT (Stack_Ctor ((ptr)) != ERROR, "Stack_Ctor ()", "Constructing error", EXIT_FAILURE);   \
}

#define STACK_PUSH(ptr, value)                                                                          \
{                                                                                                       \
    MY_ASSERT (Stack_Push ((ptr), (value)) != ERROR, "Stack_Push ()", "Pushing error", EXIT_FAILURE);   \
}

#define STACK_POP(ptr, elem_ptr)                                                                        \
{                                                                                                       \
    MY_ASSERT (Stack_Pop ((ptr), (elem_ptr)) != ERROR, "Stack_Pop ()", "Popping error", EXIT_FAILURE);  \
}

#define STACK_DTOR(ptr)                                                                             \
{                                                                                                   \
    MY_ASSERT (Stack_Dtor ((ptr)) != ERROR, "Stack_Dtor ()", "Destructing error", EXIT_FAILURE);    \
}

#define NULL_STACK "Pointer on the stack is NULL"
#define UNINITIALISED "It's forbidden to use uninitialized stack"
#define MEMORY "Impossible to allocate enough memory"
#define ZERO_POP "It's forbidden to pop stack which size is 0"
#define NULL_ELEM_PTR "Pointer on the variable to save popped value is NULL"
#define MULT_CTOR "It's forbidden to initialize stack more than once"
#define BAD_RESIZE "It's forbidden to resize down stack which capacity is minimal or less than minimal"
#define RSZ_ERR "Resizing error"

#ifdef SECURITY_LEVEL
    #undef SECURITY_LEVEL
#endif

#define SECURITY_LEVEL 2

#if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    #include "Canary_Protection.h"
#endif

#if SECURITY_LEVEL == 2
    #include "Hash_Protection.h"
#elif SECURITY_LEVEL == 0
    #define CANARY_SZ 0
#endif
//----------------------------------------------------------------

typedef int elem_t;

const elem_t CAPACITY_STEP = 16;
const size_t ELEM_SZ = sizeof (elem_t);

const elem_t Stack_Poison = 89348823;

struct Stack
{
    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    canary_t canary_1;
    #endif
    elem_t   *data;
    long     size;
    long     capacity;
    bool     initialized;
    #if SECURITY_LEVEL == 2
    hash_t   hash;
    #endif
    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    canary_t canary_2;
    #endif
};

int Stack_Ctor (struct Stack *stack_ptr);
int Stack_Dtor (struct Stack *stack_ptr);
int Stack_Push (struct Stack *stack_ptr, const elem_t value);
int Stack_Pop  (struct Stack *stack_ptr, elem_t *elem_ptr);
int Stack_Dump (struct Stack *stack_ptr);

#endif // STACK_H_INCLUDED
