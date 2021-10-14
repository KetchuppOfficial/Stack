#ifndef STATIC_DATA_H_INCLUDED
#define STATIC_DATA_H_INCLUDED

static int Start_Initialization (struct Stack *stack_ptr);
static int Stack_Resize_Up (struct Stack *stack_ptr);
static int Stack_Resize_Down (struct Stack *stack_ptr);
static int Check_Canary (struct Stack *stack_ptr);
static int Check_Hash (struct Stack *stack_ptr);
static hash_t Calc_Hash (struct Stack *stack_ptr);

#endif // STATIC_DATA_H_INCLUDED
