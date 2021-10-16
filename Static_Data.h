#ifndef STATIC_DATA_H_INCLUDED
#define STATIC_DATA_H_INCLUDED

static int Start_Initialization (struct Stack *stack_ptr);
static int Stack_Resize_Up (struct Stack *stack_ptr);
static int Stack_Resize_Down (struct Stack *stack_ptr);
static int Stack_Dump (struct Stack *stack_ptr, FILE *output);
static int Check_Canary (struct Stack *stack_ptr);
static int Check_Hash (struct Stack *stack_ptr);
static hash_t Calc_Hash (struct Stack *stack_ptr);

//-----------------------------------------------
//Defining error reports
static const char *NULL_STACK =    "Pointer on the stack is NULL";
static const char *UNINIT_STACK =  "It's forbidden to use uninitialized stack";
static const char *NE_MEM =        "Impossible to allocate enough memory";
static const char *ZERO_POP =      "It's forbidden to pop stack which size is 0";
static const char *NULL_ELEM_PTR = "Pointer on the variable to save popped value is NULL";
static const char *MULT_CTOR =     "It's forbidden to initialize stack more than once";
static const char *BAD_RESIZE =    "It's forbidden to resize down stack which capacity is minimal or less than minimal";
static const char *RSZ_ERR =       "Resizing error";
//---------------------------------------

#endif // STATIC_DATA_H_INCLUDED
