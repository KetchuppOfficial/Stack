#include "Stack.h"
#include "Static_Data.h"

extern FILE *LOG_FILE;

int Stack_Ctor (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,               "struct Stack *stack_ptr", NULL_STACK, ERROR);
    MY_ASSERT (!stack_ptr->initialized, "stack_ptr->initialized",  MULT_CTOR,  ERROR);

    Start_Initialization (stack_ptr);

    void *raw_ptr = calloc (2 * CANARY_SZ + CAPACITY_STEP * ELEM_SZ, sizeof (char));
    MY_ASSERT (raw_ptr, "void *raw_ptr", NE_MEM, ERROR);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    *((canary_t *)raw_ptr) = DATA_L_CANARY;
    *((canary_t *)((char *)raw_ptr + CANARY_SZ + CAPACITY_STEP * ELEM_SZ)) = DATA_R_CANARY;
    #endif

    stack_ptr->data = (ELEM_T *)((char *)raw_ptr + CANARY_SZ);

    stack_ptr->initialized = true;

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

int Start_Initialization (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_STACK, ERROR);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    stack_ptr->l_canary = STACK_L_CANARY;
    stack_ptr->r_canary = STACK_R_CANARY;
    #endif

    stack_ptr->data = NULL;

    stack_ptr->size = 0;

    stack_ptr->capacity = CAPACITY_STEP;

    return NO_ERRORS;
}

int Stack_Dtor (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,   ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINIT_STACK, ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    stack_ptr->l_canary = Stack_Poison;
    stack_ptr->r_canary = Stack_Poison;
    #endif

    for (int index = 0; index < stack_ptr->capacity; index++)
        stack_ptr->data[index] = (ELEM_T)Stack_Poison;

    free ((char *)(stack_ptr->data) - CANARY_SZ);

    stack_ptr->size = 0;
    stack_ptr->capacity = 0;

    stack_ptr->initialized = false;

    return NO_ERRORS;
}

int Stack_Push (struct Stack *stack_ptr, const ELEM_T value)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,   ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINIT_STACK, ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    if ((stack_ptr->size) >= (stack_ptr->capacity))
        MY_ASSERT (Stack_Resize_Up (stack_ptr), "Stack_Resize_Up ()", RSZ_ERR, ERROR);

    if ((stack_ptr->size) < (stack_ptr->capacity))
        stack_ptr->data[stack_ptr->size++] = value;

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

static int Stack_Resize_Up (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,   ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINIT_STACK, ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    void *raw_ptr = realloc ((char *)(stack_ptr->data) - CANARY_SZ, (stack_ptr->capacity + CAPACITY_STEP) * ELEM_SZ + 2 * CANARY_SZ);
    MY_ASSERT (raw_ptr, "void *raw_ptr", NE_MEM, ERROR);

    char *r_canary_ptr = (char *)raw_ptr + CANARY_SZ + (stack_ptr->capacity) * ELEM_SZ;

    for (size_t counter = 0; counter < CANARY_SZ + CAPACITY_STEP * ELEM_SZ; counter++)
        *(r_canary_ptr + counter) = 0;

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    *((canary_t *)(r_canary_ptr + CAPACITY_STEP * ELEM_SZ)) = DATA_R_CANARY;
    #endif

    (stack_ptr->data) = (ELEM_T *)((char *)raw_ptr + CANARY_SZ);

    (stack_ptr->capacity) += CAPACITY_STEP;

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

int Stack_Pop (struct Stack *stack_ptr, ELEM_T *elem_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,    ERROR);
    MY_ASSERT (elem_ptr,               "ELEM_T *elem_ptr",        NULL_ELEM_PTR, ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINIT_STACK,  ERROR);
    MY_ASSERT (stack_ptr->size > 0,    "stack_ptr->size",         ZERO_POP,      ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    int value = 0;

    if ((stack_ptr->size) >= (stack_ptr->capacity) - 2 * CAPACITY_STEP)
    {
        value = stack_ptr->data[stack_ptr->size - 1];
        stack_ptr->data[stack_ptr->size - 1] = 0;
        stack_ptr->size--;
    }

    if ((stack_ptr->size) < (stack_ptr->capacity) - 2 * CAPACITY_STEP)
        MY_ASSERT (Stack_Resize_Down (stack_ptr), "Stack_Resize_Down ()", RSZ_ERR, ERROR);

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    *elem_ptr = value;

    return NO_ERRORS;
}

static int Stack_Resize_Down (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,                           "struct Stack *stack_ptr", NULL_STACK,    ERROR);
    MY_ASSERT (stack_ptr->initialized,              "stack_ptr->initialized",  UNINIT_STACK,  ERROR);
    MY_ASSERT (stack_ptr->capacity > CAPACITY_STEP, "stack_ptr->capacity",     BAD_RESIZE,    ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    stack_ptr->capacity -= CAPACITY_STEP;

    void *raw_ptr = realloc ((char *)(stack_ptr->data) - CANARY_SZ, (stack_ptr->capacity) * ELEM_SZ + 2 * CANARY_SZ);
    MY_ASSERT (raw_ptr, "void *raw_ptr", NE_MEM, ERROR);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    *((canary_t *)((char *)raw_ptr + CANARY_SZ + (stack_ptr->capacity) * ELEM_SZ)) = DATA_R_CANARY;
    #endif

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

static int Stack_Dump (struct Stack *stack_ptr, FILE *output)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_STACK, ERROR);
    MY_ASSERT (output,    "FILE *output",            NULL_FILE,  ERROR);

    fprintf (output, "*************************************\n");
    fprintf (output, "ALL INFORMATION ABOUT STACK\n\n");
    fprintf (output, "POINTER ON STACK: %p\n", stack_ptr);

    fprintf (output, "TYPE OF DATA: %s\n", typeid(stack_ptr->data[0]).name());
    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    fprintf (output, "L_CANARY: %lx\n", stack_ptr->l_canary);
    #endif

    fprintf (output, "DATA:\n");
    for (int index = 0; index < (stack_ptr->capacity); index++)
        DATA_OUTPUT (index, (stack_ptr->data)[index], output);
    fprintf (output, "SIZE: %ld\n",     stack_ptr->size);
    fprintf (output, "CAPACITY: %ld\n", stack_ptr->capacity);

    #if SECURITY_LEVEL == 2
    fprintf (output, "SAVED HASH: %lu\n",   stack_ptr->hash);
    hash_t curr_hash = Calc_Hash (stack_ptr);
    fprintf (output, "CURRENT HASH: %lu\n", curr_hash);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    fprintf (output, "r_canary: %lx\n", stack_ptr->r_canary);
    #endif
    fprintf (output, "*************************************\n\n");

    return NO_ERRORS;
}
#if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
static int Check_Canary (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,    ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINIT_STACK,  ERROR);

    if (stack_ptr->l_canary != STACK_L_CANARY)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        MY_ASSERT (false, "l_STACK_CANARY", L_S_CANARY_CG, ERROR);
    }

    if (stack_ptr->r_canary != STACK_R_CANARY)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        MY_ASSERT (false, "R_STACK_CANARY", R_S_CANARY_CG, ERROR);
    }

    if (*((canary_t *)((char *)(stack_ptr->data) - CANARY_SZ)) != DATA_L_CANARY)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        MY_ASSERT (false, "L_DATA_CANARY",  L_D_CANARY_CG, ERROR);
    }

    if (*((canary_t *)((char *)(stack_ptr->data) + (stack_ptr->capacity) * ELEM_SZ)) != DATA_R_CANARY)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        MY_ASSERT (false, "R_DATA_CANARY",  R_D_CANARY_CG, ERROR);
    }

    return NO_ERRORS;
}
#endif

#if SECURITY_LEVEL == 2
static int Check_Hash (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,   ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINIT_STACK, ERROR);

    hash_t old_hash = stack_ptr->hash;

    hash_t new_hash = Calc_Hash (stack_ptr);

    if (old_hash != new_hash)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        MY_ASSERT (false, "stack_ptr->hash", HASH_CHANGE, ERROR);
    }

    return NO_ERRORS;
}

static hash_t Calc_Hash (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,   (hash_t)ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINIT_STACK, (hash_t)ERROR);

    hash_t hash = 0;

    for (int index = 0; index < stack_ptr->capacity; index++)
        HASH_VAR (hash, stack_ptr->data[index]);

    HASH_VAR (hash, stack_ptr->size);
    HASH_VAR (hash, stack_ptr->capacity);
    HASH_VAR (hash, *((char *)(stack_ptr->data)));
    HASH_VAR (hash, stack_ptr->initialized);

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
#endif
