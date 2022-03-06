#include "Stack.h"
#include "../My_Lib/My_Lib.h"

#define hash        DO_NOT_CHANGE_HASH_EVER
#define initialized DO_NOT_CHANGE_INITIALIZATION_STATUS_EVER

int main (void)
{
    OPEN_LOG_FILE;

    struct Stack stack = {};

    Stack_Ctor (&stack);

    Stack_Push (&stack, 7);

    Stack_Dtor (&stack);

    return 0;
}
