#ifndef HASH_PROTECTION_H_INCLUDED
#define HASH_PROTECTION_H_INCLUDED

#define CHECK_HASH(condition)               \
{                                           \
    if (Check_Hash ((condition)) == ERROR)  \
        return ERROR;                       \
}

typedef long double hash_t;

const double ZERO_HASH = 3.141592;
const double EPSILON = 1E-4;

enum Comparison
{
    NOT_EQUAL = -2,
    LESS,
    EQUAL,
    GREATER
};

#endif // HASH_PROTECTION_H_INCLUDED
