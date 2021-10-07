#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long canary_t;

const canary_t STACK_CANARY_1 = 0xDEDBEDA;
const canary_t STACK_CANARY_2 = 0xDEDBAD;
const canary_t BUFF_CANARY_1 = 0xBADEDA;
const canary_t BUFF_CANARY_2 = 0xDEBAC;

const int INITIALIZED = 9255;
const int CAPACITY_STEP = 8;
const size_t CANARY_SZ = sizeof (canary_t);
const size_t ELEM_SZ = sizeof (int);

const void *const Err_Ptr = (void *)((3 - 14 + 15 - 92 + 65 - 35) * -2718);

struct Stack
{
    canary_t canary_1;
    int *data;
    long long size;
    long long capacity;
    canary_t canary_2;
};

int Stack_Ctor (struct Stack *stack_ptr);
int Start_Initialization (struct Stack *stack_ptr);
int Stack_Dtor (struct Stack *stack_ptr);
int Stack_Push (struct Stack *stack_ptr, int value);
int Stack_Resize_Up (struct Stack *stack_ptr);
int Stack_Pop (struct Stack *stack_ptr);
int Stack_Dump (struct Stack *stack_ptr);
void Show_Stack (struct Stack *stack_ptr);

#endif // STACK_H_INCLUDED
