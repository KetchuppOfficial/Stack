#include "Stack.h"
#include "Log_File.h"

extern FILE *LOG_FILE;
///TODO �������� ������������� �������� ������������, PUSH � POP � �����������
///TODO ������������� � ���������� � ������� memset
///TODO ���� ������ � enum
///TODO ������� ���
///TODO ����������� ����������� ��������� ���������� ���� ������� ������
///TODO �������� �� ����������� ���� � ��������
///TODO ����������� ������ ���� ������
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


