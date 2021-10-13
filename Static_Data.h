#ifndef STATIC_DATA_H_INCLUDED
#define STATIC_DATA_H_INCLUDED

static const char *types_of_stack[] =
{
    "char",
    "int",
    "unsigned int",
    "long",
    "unsigned long",
    "long long",
    "unsigned long long",
    "float",
    "double",
    "long double",
};

static int Start_Initialization (struct Stack *stack_ptr);
static int Stack_Resize_Up (struct Stack *stack_ptr);
static int Stack_Resize_Down (struct Stack *stack_ptr);
static int Check_Canary (struct Stack *stack_ptr);
static int Check_Hash (struct Stack *stack_ptr);
static hash_t Calc_Hash (struct Stack *stack_ptr);
static int Compare_Double (const double first, const double second);
static int IsNAN (const double value);

#endif // STATIC_DATA_H_INCLUDED
