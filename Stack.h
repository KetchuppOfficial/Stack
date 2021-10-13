#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

//-----------------------------------------------
//Including headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "Log_File.h"
//-----------------------------------------------

//-----------------------------------------------
//Defining type
#define CHAR 0
#define INT 1
#define U_INT 2
#define L_INT 3
#define U_L_INT 4
#define L_L_INT 5
#define U_L_L_INT 6
#define FLOAT 7
#define DOUBLE 8
#define L_DOUBLE 9

#define STACK_TYPE INT

#if STACK_TYPE == CHAR
    #define ELEM_T char
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %c\n", (index), (data_src));
#endif

#if STACK_TYPE == INT
    #define ELEM_T int
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %d\n", (index), (data_src));
#endif

#if STACK_TYPE == U_INT
    #define ELEM_T unsigned int
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %u\n", (index), (data_src));
#endif

#if STACK_TYPE == L_INT
    #define ELEM_T long
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %ld\n", (index), (data_src));
#endif

#if STACK_TYPE == U_L_INT
    #define ELEM_T unsigbned long
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %lu\n", (index), (data_src));
#endif

#if STACK_TYPE == L_L_INT
    #define ELEM_T long long
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %lld\n", (index), (data_src));
#endif

#if STACK_TYPE == U_L_L_INT
    #define ELEM_T unsigned long long
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %llu\n", (index), (data_src));
#endif

#if STACK_TYPE == FLOAT
    #define ELEM_T float
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %g\n", (index), (data_src));
#endif

#if STACK_TYPE == DOUBLE
    #define ELEM_T double
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %g\n", (index), (data_src));
#endif

#if STACK_TYPE == L_DOUBLE
    #define ELEM_T long double
    #define DATA_OUTPUT(index, data_src, output)                        \
       fprintf (output, "\tdata[%d] = %Lg\n", (index), (data_src));
#endif
//-----------------------------------------------

//-----------------------------------------------
//Defining security
#define SECURITY_LEVEL 2

#if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    #include "Canary_Protection.h"
#endif

#if SECURITY_LEVEL == 2
    #include "Hash_Protection.h"
#elif SECURITY_LEVEL == 0
    #define CANARY_SZ 0
#endif
//-----------------------------------------------

//-----------------------------------------------
//Defining macros
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
//-----------------------------------------------

//-----------------------------------------------
//Defining error reports
#define NULL_STACK "Pointer on the stack is NULL"
#define UNINITIALISED "It's forbidden to use uninitialized stack"
#define MEMORY "Impossible to allocate enough memory"
#define ZERO_POP "It's forbidden to pop stack which size is 0"
#define NULL_ELEM_PTR "Pointer on the variable to save popped value is NULL"
#define MULT_CTOR "It's forbidden to initialize stack more than once"
#define BAD_RESIZE "It's forbidden to resize down stack which capacity is minimal or less than minimal"
#define RSZ_ERR "Resizing error"
//---------------------------------------

const int CAPACITY_STEP = 8;
const size_t ELEM_SZ = sizeof (ELEM_T);

const ELEM_T Stack_Poison = 89348823;

struct Stack
{
    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    canary_t canary_1;
    #endif
    ELEM_T   *data;
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
int Stack_Push (struct Stack *stack_ptr, const ELEM_T value);
int Stack_Pop  (struct Stack *stack_ptr, ELEM_T *elem_ptr);
int Stack_Dump (struct Stack *stack_ptr, FILE *output);

#endif // STACK_H_INCLUDED
