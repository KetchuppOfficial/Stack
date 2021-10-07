#include "Stack.h"
#include "Log_File.h"

extern FILE *LOG_FILE;

int Stack_Ctor (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", "Pointer on the stack is NULL", {return ERROR;});

    Start_Initialization (stack_ptr);

    void *raw_ptr = calloc (2 * CANARY_SZ + CAPACITY_STEP * ELEM_SZ, sizeof (char));
    MY_ASSERT (raw_ptr, "void *raw_ptr", "Impossible to allocate enough memory", {return ERROR;});

    *((canary_t *)raw_ptr) = BUFF_CANARY_1;

    stack_ptr -> data = (int *)((char *)raw_ptr + CANARY_SZ);

    *((canary_t *)((char *)raw_ptr + CANARY_SZ + CAPACITY_STEP * ELEM_SZ)) = BUFF_CANARY_2;

    return NO_ERRORS;
}

int Start_Initialization (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", "Pointer on the stack is NULL", {return ERROR;})

    if ((stack_ptr -> canary_1) != STACK_CANARY_1)
        (stack_ptr -> canary_1) = STACK_CANARY_1;

    if ((stack_ptr -> data) != NULL)
        stack_ptr -> data = NULL;

    if ((stack_ptr -> size) != 0)
        stack_ptr -> size = 0;

    if ((stack_ptr -> capacity) != CAPACITY_STEP)
        stack_ptr -> capacity = CAPACITY_STEP;

    if ((stack_ptr -> canary_2) != STACK_CANARY_2)
        (stack_ptr -> canary_2) = STACK_CANARY_2;

    return NO_ERRORS;
}

int Stack_Dtor (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", "Pointer on the stack is NULL", {return ERROR;});

    stack_ptr -> canary_1 = 0;

    *((canary_t *)((char *)(stack_ptr -> data) - CANARY_SZ)) = 0;
    *((canary_t *)((stack_ptr -> data) + (stack_ptr -> capacity) * ELEM_SZ)) = 0;
    free ((char *)(stack_ptr -> data) - CANARY_SZ);
    //stack_ptr -> data = (int *)Err_Ptr;

    stack_ptr -> size = 0;
    stack_ptr -> capacity = 0;

    stack_ptr -> canary_2 = 0;

    stack_ptr = (struct Stack *)Err_Ptr;

    return NO_ERRORS;
}

int Stack_Push (struct Stack *stack_ptr, int value)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", "Pointer on the stack is NULL", {return ERROR;});

    if ((stack_ptr -> size) >= (stack_ptr -> capacity))
        MY_ASSERT (Stack_Resize_Up (stack_ptr), "Stack_Resize_Up ()", "Resizing error", {return ERROR;});

    if ((stack_ptr -> size) <= (stack_ptr -> capacity))
        stack_ptr -> data[stack_ptr -> size++] = value;

    return NO_ERRORS;
}

int Stack_Resize_Up (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", "Pointer on the stack is NULL", {return ERROR;});

    void *raw_ptr = realloc ((char *)(stack_ptr -> data) - CANARY_SZ, ((stack_ptr -> capacity) + CAPACITY_STEP) * ELEM_SZ + 2 * CANARY_SZ);
    MY_ASSERT (raw_ptr, "void *raw_ptr", "Impossible to allocate enough memory", {return ERROR;});

    char *canary_2_ptr = (char *)raw_ptr + CANARY_SZ + (stack_ptr -> capacity) * ELEM_SZ;

    for (size_t counter = 0; counter < CANARY_SZ + CAPACITY_STEP * ELEM_SZ; counter++)
        *(canary_2_ptr + counter) = 0;

    *((canary_t *)(canary_2_ptr + CAPACITY_STEP * ELEM_SZ)) = BUFF_CANARY_2;

    (stack_ptr -> data) = (int *)((char *)raw_ptr + CANARY_SZ);

    (stack_ptr -> capacity) += CAPACITY_STEP;

    return NO_ERRORS;
}

int Stack_Pop (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", "Pointer on the stack is NULL", {return ERROR;});

    int value = 0;

    if ((stack_ptr -> size) >= (stack_ptr -> capacity) - 2 * CAPACITY_STEP)
    {
        value = stack_ptr -> data[stack_ptr -> size - 1];
        stack_ptr -> data[stack_ptr -> size - 1] = 0;
        stack_ptr -> size--;
    }

    if ((stack_ptr -> size) <= (stack_ptr -> capacity) - 2 * CAPACITY_STEP)
        MY_ASSERT (Stack_Dump (stack_ptr), "Stack_Resize_Down ()", "Resizing error", {return ERROR;});

    return value;
}

int Stack_Dump (struct Stack *stack_ptr)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", "Pointer on the stack is NULL", {return ERROR;});

    stack_ptr -> capacity -= CAPACITY_STEP;

    void *raw_ptr = realloc ((char *)(stack_ptr -> data) - CANARY_SZ, (stack_ptr -> capacity) * ELEM_SZ + 2 * CANARY_SZ);
    MY_ASSERT (raw_ptr, "void *raw_ptr", "Impossible to allocate enough memory", {return ERROR;});

    *((canary_t *)((char *)raw_ptr + CANARY_SZ + (stack_ptr -> capacity) * ELEM_SZ)) = BUFF_CANARY_2;

    return NO_ERRORS;
}

///NOT FOR RELEASE. JUST FOR DEBUG
void Show_Stack (struct Stack *stack_ptr)
{
    printf ("CANARY_1: %0x\n", stack_ptr -> canary_1);
    printf ("DATA:\n");
    for (int i = 0; i < (stack_ptr -> size); i++)
        printf ("\tdata[%d] = %d\n", i, (stack_ptr -> data)[i]);
    printf ("SIZE: %d\n", stack_ptr -> size);
    printf ("CAPACITY: %d\n", stack_ptr -> capacity);
    printf ("CANARY_2: %0x\n", stack_ptr -> canary_2);
}
