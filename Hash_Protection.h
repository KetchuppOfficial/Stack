#ifndef HASH_PROTECTION_H_INCLUDED
#define HASH_PROTECTION_H_INCLUDED

#define CHECK_HASH(condition)               \
{                                           \
    if (Check_Hash ((condition)) == ERROR)  \
        return ERROR;                       \
}

typedef double hash_t;

const double ZERO_HASH = 3.141592;
const double EPSILON = 1E-7;

enum Comparison
{
    NOT_EQUAL = -2,
    LESS,
    EQUAL,
    GREATER
};

int Check_Hash (struct Stack *stack_ptr);
hash_t Calc_Hash (struct Stack *stack_ptr);
int Compare_Double (const double first, const double second);
int IsNAN (const double value);

#endif // HASH_PROTECTION_H_INCLUDED
