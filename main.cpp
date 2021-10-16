#include "Stack.h"
#include "Log_File.h"

int main (void)
{
    atexit (Close_Log_File);
    if (Open_Log_File () == ERROR)
        return EXIT_FAILURE;

    struct Stack stack = {};

    STACK_CTOR (&stack);

    STACK_PUSH (&stack, 22);

    STACK_PUSH (&stack, 1);

    STACK_PUSH (&stack, 2004);

    STACK_DTOR (&stack);

    stack.initialized = true;

    STACK_PUSH (&stack, 67);

    return 0;
}

