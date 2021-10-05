#ifndef LOG_FILE_H_INCLUDED
#define LOG_FILE_H_INCLUDED

#include <stdio.h>
#include <assert.h>

#define MY_ASSERT(condition, var, description, ...)                                 \
{                                                                                   \
    if (!(condition))                                                               \
    {                                                                               \
        My_Assert (false, __FILE__, __LINE__, __FUNCTION__, (var), (description));  \
        __VA_ARGS__;                                                                \
    }                                                                               \
}

int My_Assert (bool condition, const char *file, int line, const char *fun_name, const char *var_name, const char *err_name);
int Open_Log_File (void);
int Close_File (FILE *file_ptr, const char *file_name);

const char PREAMBLE[] = "This is log file of the project \"Hamlet Sorting\".\n"
                        "This file will help you to find and fix all the errors.\n\n";
enum ERRORS
{
    NO_ERRORS = -100,
    ERROR
};


#endif // LOG_FILE_H_INCLUDED
