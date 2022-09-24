#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "include/stack.h"

int main()
{
    Stack stk1 = {};
    StackCtor(&stk1, 5);

    elem_t test_data[] = {2, 5, 7, 9};
    Stack stk2 = {test_data, -1, 10, "&stk2"};

    StackDump (&stk1);
    StackDump (&stk2);

    StackDtor(&stk1, 5);
}


void StackCtor_ (Stack* self, size_t capacity, const char* name)
{
    self->data = (elem_t *)calloc(sizeof(elem_t), capacity);
    self->size = 0;
    self->capacity = capacity;
    self->name = ++name;
}


void StackDtor (Stack* self, size_t capacity)
{
    free (self->data);
    self->size = -1;
    self->capacity = -1;
}


void StackDump_ (Stack* self, const char* filename, const char* funcname, int line)
{
    PutDividers();
    
    Verificate (self); 

    printf ("At file: %s\n", filename);

    printf ("Observing stack[%p] - %s, function: %s (%d)):\n", self, self->name, funcname, line);
    printf ("    Size: %d\n", self->size);
    printf ("    Capacity: %d\n    Data array:\n", self->capacity);

    for (int i = 0; i < self->capacity; i++)
    {
        if (i < self->size) printf ("       *[%d]: %d\n", i, self->data[i]);
        else
        {
            printf ("        [%d]: %3d\n", i, self->data[i]);
        }
    }

    PutDividers();
}

lld StackVerificator (Stack *self)
{
    lld err = 0;

    if (self == nullptr)
    {
        err += 1;
        return err;
    }

    if (self->data == nullptr)
        err += 2;
    if (self->size < 0)
        err += 4;
    if (self->capacity < self->size)
        err += 8;
    if (self->capacity <= 0)
        err += 16;

    return err;
}


void Verificate (Stack* self)
{
    lld err = StackVerificator(self);

    printf("Error code = %d\n", err);
    PutErrCodes (err);
}


void PutErrCodes (lld err)
{
    for (int i = 0; i <= 10; i++)
    {
        int cur_bit = GetBit(err, i);

        if (cur_bit)
        {
            PrintError(pow(2, i));
        }
    }
}


int GetBit (lld n, int pos)
{
    int mask = 1 << pos;
    int masked_n = n & mask;
    int thebit = masked_n >> pos;

    return thebit;
}


void PrintError (int error_code)
{
    switch (error_code)
    {
    case NULL_STACK:
        printf("NULL STACK POINTER\n");
        break;
    case NULL_DATA:
        printf("NULL DATA POINTER\n");
        break;
    case INVALID_SIZE:
        printf("INVALID SIZE, below zero probably\n");
        break;
    case N_ENOUGH_SIZE:
        printf("N ENOUGH SIZE, Not enought size was located\n");
        break;
    case INVALID_CAPACITY:
        printf("INVALID CAPACITY\n");
        break;

    default:
        printf("Idk, man. Somethink spooky. Error code %d", error_code);
    };
}


void PutDividers()
{
    printf ("=========================================================\n");
}
