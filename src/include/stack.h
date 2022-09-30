#ifndef STACK_H
#define STACK_H

#define CANARY
#define HASH
#define DEBUG 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdint.h>

#include "config.h"

struct StackInfo
{
    const char* name;
    const char* mother_func;
    const char* mother_file;
    bool  data_corrupted;

    void* hash_ignore_ptr;
    size_t  hash_skip; // ??
};


struct Stack
{
    StackInfo stack_info;

    int size;
    int capacity;
    elem_t* data; 
    int64_t hash;
    int64_t subhash;   
};


#define StackDump(X) StackDump_ (X, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define StackCtor(X, Y) StackCtor_ (X, Y, #X, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define HASH_FUNC self->hash = HashFunc (self, sizeof (Stack), self->stack_info.hash_ignore_ptr, sizeof (int64_t) * 2); \
                  self->subhash =  HashFunc (self->data, sizeof (elem_t) * self->capacity, nullptr, 0);

#define min(a,b) \
   ({ __typeof__  (a) _a = (a); \
       __typeof__ (b) _b = (b); \
                      _a < _b ? _a : _b; })


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
    STACK_MEMORY_CORRUPTION = 64
};

//----------------------------------------------------

const int CANARY_COUNT = 2;

const int RESIZE_OFFSET = 2;

const int MEMORY_MULTIPLIER = 2;

//----------------------------------------------------



void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line);

void StackDtor (Stack* self);

lld StackVerificator (Stack* self);

int GetBit (lld n, int pos);

void PutErrCodes (lld err);

void PrintError (int error_code);

void StackDump_ (Stack* self, const char* filename, const char* funcname, int line);

void PutDividers ();

void Verificate (Stack* self);

elem_t StackPop (Stack* self);

void StackPush (Stack* self, elem_t value);

void StackResize (Stack* self, int mode);

void* recalloc (void* ptr, int len_new, size_t size);

int64_t HashFunc (void* ptr, size_t size, void* skip_ptr, size_t skip_amount);


#endif
