#ifndef LOG_FILE_H_INCLUDED
#define LOG_FILE_H_INCLUDED

#include <stdio.h>

#define MY_ASSERT(condition, var, description, error)                               \
{                                                                                   \
    if (!(condition))                                                               \
    {                                                                               \
        My_Assert (false, __FILE__, __LINE__, __FUNCTION__, (var), (description));  \
        return error;                                                               \
    }                                                                               \
}

extern FILE *LOG_FILE;

int My_Assert (bool condition, const char *file, const int line, const char *fun_name, const char *var_name, const char *err_name);
int Open_Log_File (void);
void Close_Log_File (void);
int Close_File (FILE *file_ptr, const char *file_name);

static const char PREAMBLE[] = "This is log file of the project \"Hamlet Sorting\".\n"
                               "This file will help you to find and fix all the errors.\n\n";
enum ERRORS
{
    NO_ERRORS = -100,
    ERROR
};

#endif // LOG_FILE_H_INCLUDED
