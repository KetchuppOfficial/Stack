#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long CANARY;

CANARY STACK_CANARY_1 = 0xDEDBEDA;
CANARY STACK_CANARY_2 = 0xDEDBAD;
CANARY BUFF_CANARY_1 = 0xBADEDA;
CANARY BUFF_CANARY_2 = 0xDEBAC;
const int CONSTRUCTED = 175;
const int INITIALIZED = 9255;
const int CAPACITY_STEP = 8;
const size_t CANARY_SZ = sizeof (CANARY);
const size_t ELEM_SZ = sizeof (int);

const void *const Err_Ptr = (void *)((3 - 14 + 15 - 92 + 65 - 35) * -2718);

struct Stack
{
    CANARY canary_1;
    int *data;
    size_t size;
    size_t capacity;
    CANARY canary_2;
};

int Stack_Ctor (struct Stack *stack_ptr);
int Start_Initialization (struct Stack *stack_ptr);
int Stack_Dtor (struct Stack *stack_ptr);
int Stack_Push (struct Stack *stack_ptr, int value);
int Stack_Resize_Up (struct Stack *stack_ptr);
void Show_Stack (struct Stack *stack_ptr);

#endif // STACK_H_INCLUDED
