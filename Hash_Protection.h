#ifndef HASH_PROTECTION_H_INCLUDED
#define HASH_PROTECTION_H_INCLUDED

#define CHECK_HASH(condition)                       \
{                                                   \
    if (Check_Hash ((condition)) == (hash_t)ERROR)  \
        return ERROR;                               \
}

typedef long hash_t;

#endif // HASH_PROTECTION_H_INCLUDED
