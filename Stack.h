#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

//-----------------------------------------------
//Including headers
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <typeinfo.h>
#include "Log_File.h"
//-----------------------------------------------

//-----------------------------------------------
//Defining type
#define CHAR      0
#define INT       1
#define U_INT     2
#define L_INT     3
#define U_L_INT   4
#define L_L_INT   5
#define U_L_L_INT 6
#define FLOAT     7
#define DOUBLE    8
#define L_DOUBLE  9

#define STACK_TYPE DOUBLE

#if STACK_TYPE == CHAR
    #define ELEM_T char
#endif

#if STACK_TYPE == INT
    #define ELEM_T int
    #define STACK_FMT "d"
#endif

#if STACK_TYPE == U_INT
    #define ELEM_T unsigned int
    #define STACK_FMT "u"
#endif

#if STACK_TYPE == L_INT
    #define ELEM_T long
    #define STACK_FMT "ld"
#endif

#if STACK_TYPE == U_L_INT
    #define ELEM_T unsigned long
    #define STACK_FMT "lu"
#endif

#if STACK_TYPE == L_L_INT
    #define ELEM_T long long
    #define STACK_FMT "lld"
#endif

#if STACK_TYPE == U_L_L_INT
    #define ELEM_T unsigned long long
    #define STACK_FMT "llu"
#endif

#if STACK_TYPE == FLOAT
    #define ELEM_T float
    #define STACK_FMT "g"
#endif

#if STACK_TYPE == DOUBLE
    #define ELEM_T double
    #define STACK_FMT "g"
#endif

#if STACK_TYPE == L_DOUBLE
    #define ELEM_T long double
    #define STACK_FMT "lg"
#endif

#define DATA_OUTPUT(index, data_src, output)                                \
    fprintf (output, "\tdata[%d] = %" STACK_FMT "\n", (index), (data_src));
//-----------------------------------------------

//-----------------------------------------------
//Defining security
#define SECURITY_LEVEL 2

#if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    #include "Canary_Protection.h"
#endif

#if SECURITY_LEVEL == 2
    #include "Hash_Protection.h"
    #define HASH_CHANGE "Hash changed its value unexpectedly"
#elif SECURITY_LEVEL == 0
    #define CANARY_SZ 0
#endif
//-----------------------------------------------

//-----------------------------------------------
//Defining macros
#define STACK_CTOR(ptr)                                                                                 \
{                                                                                                       \
    MY_ASSERT (Stack_Ctor ((ptr)) != ERROR,             "Stack_Ctor ()", "Constructing error", 0x01);   \
}

#define STACK_PUSH(ptr, value)                                                                          \
{                                                                                                       \
    MY_ASSERT (Stack_Push ((ptr), (value)) != ERROR,    "Stack_Push ()", "Pushing error",      0x02);   \
}

#define STACK_POP(ptr, elem_ptr)                                                                        \
{                                                                                                       \
    MY_ASSERT (Stack_Pop  ((ptr), (elem_ptr)) != ERROR, "Stack_Pop ()",  "Popping error",      0x03);   \
}

#define STACK_DTOR(ptr)                                                                                 \
{                                                                                                       \
    MY_ASSERT (Stack_Dtor ((ptr)) != ERROR,             "Stack_Dtor ()", "Destructing error",  0x04);   \
}
//-----------------------------------------------

const int CAPACITY_STEP = 8;
const size_t ELEM_SZ = sizeof (ELEM_T);
const unsigned long Stack_Poison = 792647;

struct Stack
{
    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    canary_t l_canary;
    #endif

    ELEM_T   *data;
    long     size;
    long     capacity;
    bool     initialized;

    #if SECURITY_LEVEL == 2
    hash_t   hash;
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    canary_t r_canary;
    #endif
};

int Stack_Ctor (struct Stack *stack_ptr);
int Stack_Dtor (struct Stack *stack_ptr);
int Stack_Push (struct Stack *stack_ptr, const ELEM_T value);
int Stack_Pop  (struct Stack *stack_ptr, ELEM_T *elem_ptr);

#endif // STACK_H_INCLUDED
