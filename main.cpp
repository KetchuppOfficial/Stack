#include "Stack.h"
#include "Log_File.h"

extern FILE *LOG_FILE;
///TODO опнбепйю бнгбпюыюелнцн гмювемхъ йнмярпсйрнпю, PUSH х POP х деярпсйрнпю
///TODO хмхжхюкхгюжхч х пюгпсьемхе я онлныэч memset
///TODO йндш ньханй б enum
///TODO ядекюрэ ущь
///TODO пеюкхгнбюрэ бнглнфмнярэ бйкчвемхъ бшйкчвемхъ бяеу спнбмеи гюыхрш
///TODO опнбепйю мю жекнярмнярэ ущью х йюмюпеей
///TODO пеюкхгнбюрэ пюгмше рхош дюммшу
int main (void)
{
    if (Open_Log_File () == ERROR)
        return EXIT_FAILURE;

    struct Stack stack;

    Stack_Ctor (&stack);

    for (int i = 0; i < 10; i++)
    {
        Stack_Push (&stack, i + 1);
        Show_Stack (&stack);
        printf ("*********************************\n");
    }

    int value = 0;

    for (int i = 0; i < 10; i++)
    {
        value = Stack_Pop (&stack);
        Show_Stack (&stack);
        printf ("value: %d\n", value);
        printf ("*********************************\n");
    }


    Stack_Dtor (&stack);

    Close_File (LOG_FILE, "log_file.log");

    return 0;
}


