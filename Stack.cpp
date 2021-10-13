#include "Stack.h"
#include "Static_Data.h"

int Stack_Ctor (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_STACK, ERROR);
    MY_ASSERT (!stack_ptr->initialized, "stack_ptr->initialized", MULT_CTOR, ERROR);

    Start_Initialization (stack_ptr);

    void *raw_ptr = calloc (2 * CANARY_SZ + CAPACITY_STEP * ELEM_SZ, sizeof (char));
    MY_ASSERT (raw_ptr, "void *raw_ptr", MEMORY, ERROR);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    *((canary_t *)raw_ptr) = DATA_CANARY_1;
    #endif

    stack_ptr->data = (ELEM_T *)((char *)raw_ptr + CANARY_SZ);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    *((canary_t *)((char *)raw_ptr + CANARY_SZ + CAPACITY_STEP * ELEM_SZ)) = DATA_CANARY_2;
    #endif

    stack_ptr->initialized = true;

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

static int Start_Initialization (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_STACK, ERROR);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    if ((stack_ptr->canary_1) != STACK_CANARY_1)
        (stack_ptr->canary_1) = STACK_CANARY_1;
    #endif

    if ((stack_ptr->data) != NULL)
        stack_ptr->data = NULL;

    if ((stack_ptr->size) != 0)
        stack_ptr->size = 0;

    if ((stack_ptr->capacity) != CAPACITY_STEP)
        stack_ptr->capacity = CAPACITY_STEP;

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    if ((stack_ptr->canary_2) != STACK_CANARY_2)
        (stack_ptr->canary_2) = STACK_CANARY_2;
    #endif

    return NO_ERRORS;
}

int Stack_Dtor (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr != 0, "struct Stack *stack_ptr", NULL_STACK, ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized", UNINITIALISED, ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    stack_ptr->canary_1 = Stack_Poison;
    stack_ptr->canary_2 = Stack_Poison;
    #endif

    for (int index = 0; index < stack_ptr->capacity; index++)
        stack_ptr->data[index] = Stack_Poison;

    free ((char *)(stack_ptr->data) - CANARY_SZ);

    stack_ptr->size = 0;
    stack_ptr->capacity = 0;

    stack_ptr->initialized = false;

    return NO_ERRORS;
}

int Stack_Push (struct Stack *stack_ptr, const ELEM_T value)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_STACK, ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized", UNINITIALISED, ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    if ((stack_ptr->size) >= (stack_ptr->capacity))
        MY_ASSERT (Stack_Resize_Up (stack_ptr), "Stack_Resize_Up ()", RSZ_ERR, ERROR);

    if ((stack_ptr->size) < (stack_ptr->capacity))
        stack_ptr->data[stack_ptr->size++] = value;

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

static int Stack_Resize_Up (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_STACK, ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized", UNINITIALISED, ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    void *raw_ptr = realloc ((char *)(stack_ptr->data) - CANARY_SZ, (stack_ptr->capacity + CAPACITY_STEP) * ELEM_SZ + 2 * CANARY_SZ);
    MY_ASSERT (raw_ptr, "void *raw_ptr", MEMORY, ERROR);

    char *canary_2_ptr = (char *)raw_ptr + CANARY_SZ + (stack_ptr->capacity) * ELEM_SZ;

    for (size_t counter = 0; counter < CANARY_SZ + CAPACITY_STEP * ELEM_SZ; counter++)
        *(canary_2_ptr + counter) = 0;

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    *((canary_t *)(canary_2_ptr + CAPACITY_STEP * ELEM_SZ)) = DATA_CANARY_2;
    #endif

    (stack_ptr->data) = (ELEM_T *)((char *)raw_ptr + CANARY_SZ);

    (stack_ptr->capacity) += CAPACITY_STEP;

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

int Stack_Pop (struct Stack *stack_ptr, ELEM_T *elem_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,    ERROR);
    MY_ASSERT (elem_ptr,               "ELEM_T *elem_ptr",        NULL_ELEM_PTR, ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINITIALISED, ERROR);
    MY_ASSERT (stack_ptr->size > 0,    "stack_ptr->size",         ZERO_POP,      ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    int value = 0;

    if ((stack_ptr->size) >= (stack_ptr->capacity) - 2 * CAPACITY_STEP)
    {
        value = stack_ptr->data[stack_ptr->size - 1];
        stack_ptr->data[stack_ptr->size - 1] = 0;
        stack_ptr->size--;
    }

    if ((stack_ptr->size) <= (stack_ptr->capacity) - 2 * CAPACITY_STEP)
        MY_ASSERT (Stack_Resize_Down (stack_ptr), "Stack_Resize_Down ()", RSZ_ERR, ERROR);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    *elem_ptr = value;

    return NO_ERRORS;
}

static int Stack_Resize_Down (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,               "struct Stack *stack_ptr", NULL_STACK,    ERROR);
    MY_ASSERT (stack_ptr->initialized,  "stack_ptr->initialized",  UNINITIALISED, ERROR);
    MY_ASSERT (stack_ptr->capacity > 8, "stack_ptr->capacity",     BAD_RESIZE,    ERROR);

    #if SECURITY_LEVEL == 2
    CHECK_HASH (stack_ptr);
    #endif

    stack_ptr->capacity -= CAPACITY_STEP;

    void *raw_ptr = realloc ((char *)(stack_ptr->data) - CANARY_SZ, (stack_ptr->capacity) * ELEM_SZ + 2 * CANARY_SZ);
    MY_ASSERT (raw_ptr, "void *raw_ptr", MEMORY, ERROR);

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    *((canary_t *)((char *)raw_ptr + CANARY_SZ + (stack_ptr->capacity) * ELEM_SZ)) = DATA_CANARY_2;
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    CHECK_CANARY (stack_ptr);
    #endif

    #if SECURITY_LEVEL == 2
    stack_ptr->hash = Calc_Hash (stack_ptr);
    #endif

    return NO_ERRORS;
}

int Stack_Dump (struct Stack *stack_ptr, FILE *output)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_STACK, ERROR);
    MY_ASSERT (output, "FILE *output", "Pointer on the file is NULL", ERROR);

    fprintf (output, "*************************************\n");
    fprintf (output, "ALL INFORMATION ABOUT STACK\n\n");
    fprintf (output, "TYPE OF DATA: %s\n", types_of_stack[STACK_TYPE]);
    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    fprintf (output, "CANARY_1: %lx\n", stack_ptr->canary_1);
    #endif

    fprintf (output, "DATA:\n");
    for (int index = 0; index < (stack_ptr->capacity); index++)
        DATA_OUTPUT (index, (stack_ptr->data)[index], output);
    fprintf (output, "SIZE: %ld\n", stack_ptr->size);
    fprintf (output, "CAPACITY: %ld\n", stack_ptr->capacity);

    #if SECURITY_LEVEL == 2
    fprintf (output, "SAVED HASH: %Lg\n", stack_ptr->hash);
    hash_t curr_hash = Calc_Hash (stack_ptr);
    fprintf (output, "CURRENT HASH: %Lg\n", curr_hash);
    #endif

    #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
    fprintf (output, "CANARY_2: %lx\n", stack_ptr->canary_2);
    #endif
    fprintf (output, "*************************************\n\n");

    return NO_ERRORS;
}
#if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
static int Check_Canary (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,    ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINITIALISED, ERROR);

    if (stack_ptr->canary_1 != STACK_CANARY_1)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        Stack_Dump (stack_ptr, stderr);
        MY_ASSERT (false, "STACK_CANARY_1", "The first stack canary changed its value", ERROR);
    }

    if (stack_ptr->canary_2 != STACK_CANARY_2)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        Stack_Dump (stack_ptr, stderr);
        MY_ASSERT (false, "STACK_CANARY_2", "The second stack canary changed its value", ERROR);
    }

    if (*((canary_t *)((char *)(stack_ptr->data) - CANARY_SZ)) != DATA_CANARY_1)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        Stack_Dump (stack_ptr, stderr);
        MY_ASSERT (false, "DATA_CANARY_1",  "The first data canary changed its value", ERROR);
    }

    if (*((canary_t *)((char *)(stack_ptr->data) + (stack_ptr->capacity) * ELEM_SZ)) != DATA_CANARY_2)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        Stack_Dump (stack_ptr, stderr);
        MY_ASSERT (false, "DATA_CANARY_2",  "The second data canary changed its value", ERROR);
    }

    return NO_ERRORS;
}
#endif

#if SECURITY_LEVEL == 2
static int Check_Hash (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,    ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINITIALISED, ERROR);

    hash_t old_hash = stack_ptr->hash;

    hash_t new_hash = Calc_Hash (stack_ptr);

    if (Compare_Double (old_hash, new_hash) != EQUAL)
    {
        Stack_Dump (stack_ptr, LOG_FILE);
        Stack_Dump (stack_ptr, stderr);
        MY_ASSERT (false, "stack_ptr->hash", "Hash changed its value unexpectedly", ERROR);
    }

    return NO_ERRORS;
}

static hash_t Calc_Hash (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr,              "struct Stack *stack_ptr", NULL_STACK,    (hash_t)ERROR);
    MY_ASSERT (stack_ptr->initialized, "stack_ptr->initialized",  UNINITIALISED, (hash_t)ERROR);

    hash_t result = 0.0;
    hash_t semi_result = 0.0;

    if (stack_ptr->capacity > 0)
    {
        for (int index = 0; index < stack_ptr->capacity; index++)
            semi_result += (stack_ptr->data[index]) * (index + 1);

        result = semi_result / stack_ptr->capacity;
    }
    else
        result = ZERO_HASH;

    return result;
}

static int Compare_Double (const double first, const double second)
{
    if (IsNAN (first) && IsNAN (second))
        return EQUAL;

    else if (IsNAN (first) || IsNAN (second))
        return NOT_EQUAL;

    else
    {
        double absolute_value = fabs (first - second);

        if (absolute_value > EPSILON)
            return (first > second) ? GREATER : LESS;
        else
            return EQUAL;
    }
}

static int IsNAN (const double value)
{
    return (value != value) ? 1 : 0;
}
#endif
