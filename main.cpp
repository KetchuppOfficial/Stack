#include "Stack.h"
#include "Log_File.h"

int main (void)
{
    atexit (Close_Log_File);
    if (Open_Log_File () == ERROR)
        return EXIT_FAILURE;

    struct Stack stack =
    {
        #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
        0,
        #endif
        NULL,
        0,
        0,
        false,
        #if SECURITY_LEVEL == 2
        0,
        #endif
        #if SECURITY_LEVEL == 1 || SECURITY_LEVEL == 2
        0
        #endif
    };

    STACK_CTOR (&stack);

    STACK_PUSH (&stack, 1);

    STACK_PUSH (&stack, 2000002);

    STACK_PUSH (&stack, -34422333);

    STACK_PUSH (&stack, 4898);

    int value = 0;
    STACK_POP (&stack, &value);
    printf ("value: %d\n", value);

    STACK_DTOR (&stack);

    return 0;
}
