#ifndef STACK_H
#define STACK_H

#define StackDump(X) StackDump_ (X, __FILE__, __func__, __LINE__)
#define StackCtor(X, Y) StackCtor_ (X, Y, #X)

#include "config.h"

struct Stack
{
    elem_t* data;
    int size;
    int capacity;
    const char* name;
};


enum ERR_CODES 
{
    NULL_STACK = 1,
    NULL_DATA  = 2,
    INVALID_SIZE = 4,
    N_ENOUGH_SIZE = 8,
    INVALID_CAPACITY = 16
};

void StackCtor_ (Stack* self, size_t capacity, const char* name);

void StackDtor (Stack* self, size_t capacity);

lld StackVerificator (Stack* self);

int GetBit (lld n, int pos);

void PutErrCodes (lld err);

void PrintError (int error_code);

void StackDump_ (Stack* self, const char* filename, const char* funcname, int line);

void PutDividers();

void Verificate (Stack* self);

#endif
