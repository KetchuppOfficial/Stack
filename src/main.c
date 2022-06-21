#include "../include/Stack.h"
#include "My_Lib.h"

int main (void)
{
    Open_Log_File ("Stack");

    struct Stack *stack = Stack_Ctor ();

    Stack_Push (stack, 1);
    Stack_Push (stack, 2);
    Stack_Push (stack, 3);
    Stack_Push (stack, 4);
    Stack_Push (stack, 5);
    Stack_Push (stack, 6);
    Stack_Push (stack, 7);
    Stack_Push (stack, 8);
    Stack_Push (stack, 9);

    Stack_Dump (stack, stdout);

    Stack_Dtor (stack);

    return 0;
}
