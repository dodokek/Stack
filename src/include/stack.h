#ifndef STACK_H
#define STACK_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>

#include "config.h"

#define StackDump(X) StackDump_ (X, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define StackCtor(X, Y) StackCtor_ (X, Y, #X, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define HASH_STACK self->hash = HashFunc (self->data, sizeof (self->data) * self->size);


struct StackInfo
{
    const char* name;
    const char* mother_func;
    const char* mother_file;
    bool  data_corrupted;

    void* hash_ignore_ptr;
    size_t  hash_skip;

};


struct Stack
{
    int size;
    int capacity;
    StackInfo stack_info;

    elem_t* data;

    ull_i hash;    
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
    DATA_ACCESS_VIOLATION = 32,
    STACK_MEMORY_CURRUPTION = 64
};

void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line);

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

void* recalloc (void* ptr, int len_old, int len_new, size_t size);

ull_i HashFunc (void *ptr, size_t size);


#endif
