#ifndef CANARY_PROTECTION_H_INCLUDED
#define CANARY_PROTECTION_H_INCLUDED

#define CHECK_CANARY(condition)                \
{                                              \
    if (Check_Canary ((condition)) == ERROR)   \
        return ERROR;                          \
}

typedef unsigned long canary_t;

const canary_t STACK_CANARY_1 = 0xDEDBEDA;
const canary_t STACK_CANARY_2 = 0xDEDBAD;
const canary_t DATA_CANARY_1  = 0xBADEDA;
const canary_t DATA_CANARY_2  = 0xDEBAC;

const size_t CANARY_SZ = sizeof (canary_t);

int Check_Canary (struct Stack *stack_ptr);

#endif // CANARY_PROTECTION_H_INCLUDED
