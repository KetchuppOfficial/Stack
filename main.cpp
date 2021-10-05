#include "Stack.h"
#include "Log_File.h"

extern FILE *LOG_FILE;

int main (void)
{
    if (Open_Log_File () == ERROR)
        return EXIT_FAILURE;

    struct Stack stack;

    Stack_Ctor (&stack);

    Stack_Dtor (&stack);

    Close_File (LOG_FILE, "log_file.log");

    return 0;
}


