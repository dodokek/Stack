#ifndef STACK_H
#define STACK_H

typedef long long int lld;

struct Stack
{
    elem_t* data;
    int size;
    int capacity;
};


enum ERR_CODES 
{
    NULL_STACK = 1,
    NULL_DATA  = 2,
    INVALID_SIZE = 4,
    N_ENOUGH_SIZE = 8,
    INVALID_CAPACITY = 16
};

void StackCtor (Stack* self, size_t capacity);

void StackDtor (Stack* self, size_t capacity);

lld StackVerificator (Stack* self);

int GetBit(lld n, int pos);

void PutErrCodes (lld err);

void PrintError (int error_code);

#endif
