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

    STACK_PUSH (&stack, 2004);

    STACK_PUSH (&stack, 2378);

    STACK_PUSH (&stack, -3442);

    STACK_PUSH (&stack, 4);

    STACK_DTOR (&stack);

    return 0;
}
