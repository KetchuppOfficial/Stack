#include "Log_File.h"

FILE *LOG_FILE = NULL;

int Open_Log_File (void)
{
    LOG_FILE = fopen ("log_file.log", "wb");

    if (!LOG_FILE)
    {
        LOG_FILE = stderr;
        MY_ASSERT (0, "LOG_FILE", "Impossible to open log file", ERROR);
    }

    fprintf (LOG_FILE, "%s", PREAMBLE);

    return NO_ERRORS;
}

void Close_Log_File (void)
{
    Close_File (LOG_FILE, "log_file.log");
}

int Close_File (FILE *file_ptr, const char *file_name)
{
    MY_ASSERT (file_ptr,  "FILE *file_ptr",        "Pointer on the file is NULL",      ERROR);
    MY_ASSERT (file_name, "const char *file_name", "Pointer on the file name is NULL", ERROR);

    int closing_result = fclose (file_ptr);
    MY_ASSERT (!closing_result, "int closing_result", "Impossible to close the file", ERROR);

    return NO_ERRORS;
}

int My_Assert (bool condition, const char *file, const int line, const char *fun_name, const char *var_name, const char *err_name)
{
    assert (fun_name);
    assert (var_name);
    assert (err_name);
    assert (line > 0);

    if (!condition)
    {
        fprintf (stderr, "File: %s\n",                                   file);
        fprintf (stderr, "Line: %d\n",                                   line);
        fprintf (stderr, "Function with error: %s\n",                    fun_name);
        fprintf (stderr, "Variable or function that caused error: %s\n", var_name);
        fprintf (stderr, "Error description: %s\n\n",                    err_name);

        return 1;
    }

    return 0;
}
