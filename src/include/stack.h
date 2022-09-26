#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

#include "config.h"

#define StackDump(X) StackDump_ (X, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define StackCtor(X, Y) StackCtor_ (X, Y, #X)

struct Stack
{
    elem_t* data;
    int size;
    int capacity;
    const char* name;
};


enum RESIZE_MODE
{
    INCREASE = 0,
    DECREASE
};


enum ERR_CODES 
{
    NULL_STACK = 1,
    NULL_DATA  = 2,
    INVALID_SIZE = 4,
    N_ENOUGH_SIZE = 8,
    INVALID_CAPACITY = 16,
    DATA_ACCESS_VIOLATION = 32
};

void StackCtor_ (Stack* self, size_t capacity, const char* name);

void StackDtor (Stack* self);

lld StackVerificator (Stack* self);

int GetBit (lld n, int pos);

void PutErrCodes (lld err);

void PrintError (int error_code);

void StackDump_ (Stack* self, const char* filename, const char* funcname, int line);

void PutDividers();

void Verificate (Stack* self);

elem_t StackPop (Stack* self);

void StackPush (Stack* self, elem_t value);

void StackResize (Stack* self, int mode);

elem_t* recalloc (elem_t* self, size_t size, int amount);

#endif
