#ifndef STACK_H
#define STACK_H

#define CANARY 1
#define HASH 1
#define DEBUG 1

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
};


struct Stack
{
    StackInfo stack_info;

    int size;
    int capacity;
    elem_t* data; 
    intmax_t hash;
    intmax_t subhash;   
};


#define StackDump(X ) StackDump_ (X, __FILE__, __PRETTY_FUNCTION__,  __LINE__) 
#define StackCtor(X, Y) StackCtor_ (X, Y, #X, __FILE__, __PRETTY_FUNCTION__, __LINE__)
  
#define DO_REHASH self->hash = self->subhash = 0;               \
                  self->hash = HashFunc (self, sizeof (Stack)); \
                  self->subhash =  HashFunc (self->data, sizeof (elem_t) * self->capacity);


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
    // Don't forget to change ERROS_COUNT const
};

//----------------------------------------------------

#ifdef CANARY
    const int CANARY_COUNT = 2;
#else 
    const int CANARY_COUNT = 0;
#endif

const int RESIZE_OFFSET = 2;

const int MEMORY_MULTIPLIER = 2;

const int ERRORS_COUNT = 7;

//----------------------------------------------------



void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line);

void StackDtor (Stack* self);

intmax_t StackVerificator (Stack* self);

int GetBit (intmax_t n, int pos);

void PutErrCodes (intmax_t err);

void PrintError (int error_code);

void StackDump_ (Stack* self, const char* filename, const char* funcname, int line);

void PutDividers ();

void Verificate (Stack* self);

elem_t StackPop (Stack* self);

void StackPush (Stack* self, elem_t value);

void StackResize (Stack* self, int mode);

void* recalloc (void* ptr, int len_new, size_t size);

intmax_t HashFunc (void* ptr, size_t size);

elem_t min (elem_t elem1, elem_t elem2);

void fill_array (elem_t* cur_ptr, elem_t* end_ptr, elem_t filler);

#endif
