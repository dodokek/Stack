

#include "include/stack.h"

int main()
{
    Stack stk1 = {};
    StackCtor(&stk1, 5);

    for (int i = 0; i < 10; i++)
    {
        StackPush (&stk1, i);    
    }

    for (int i = 0; i < 8; i++)
    {
        printf("Popped elem: %d\n", StackPop (&stk1));    
    }

    stk1.data[0] = 0;
    StackDump (&stk1);

    elem_t test_elem = StackPop(&stk1);

    StackPush(&stk1, test_elem + 1);

    printf("Test elem %d", test_elem);

    StackDtor(&stk1);
}


elem_t StackPop (Stack* self)
{
    StackResize (self, DECREASE);
    StackDump(self);

    elem_t tmp = self->data[self->size - 2];
    
    self->data[self->size - 2] = RIGHT_COCK;
    self->data[self->size - 1] = POISON_NUM;

    self->size--;

    return tmp;     
}


void StackPush (Stack* self, elem_t value)
{
    StackResize (self, INCREASE);
    StackDump (self);
    self->data[self->size - 1] = value;
    self->data[self->size]     = RIGHT_COCK;
    self->size++;
}


void StackResize (Stack* self, int mode)
{
    switch (mode)
    {
    case INCREASE:
        if (self->capacity - self->size < 2)
        {
            self->data = (elem_t*) realloc((elem_t*) self->data, sizeof(elem_t)*(self->capacity) * 2);
            self->capacity *= 2;

            for (int i = self->size; i < self->capacity; i++)
            {
                self->data[i] = POISON_NUM;
            }
        }

        break;
    
    case DECREASE:
        if (self->capacity - self->size > 4)
        {
            self->data = (elem_t*) realloc((elem_t*) self->data, sizeof(elem_t)*(self->capacity) * 2);
            self->capacity /= 1.5;

            for (int i = self->size; i < self->capacity; i++)
            {
                self->data[i] = POISON_NUM;
            }
        }

        break;

    default:
        printf ("STACK RESIZE ERROR\n");
        break;
    }
        
}


void StackCtor_ (Stack* self, size_t capacity, const char* name)
{
    self->data = (elem_t *)calloc(sizeof(elem_t), capacity);
    //StackPush (self, RIGHT_COCK);
    self->size = 2;
    self->capacity = capacity;
    self->name = ++name;
    self->data[0] = LEFT_COCK;    
    self->data[1] = RIGHT_COCK;    
}


void StackDtor (Stack* self)
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

    printf ("Observing stack[%p] - %s, function: %s (Line %d)):\n", self, self->name, funcname, line);
    printf ("    Size: %d\n", self->size);
    printf ("    Capacity: %d\n    Data array:\n", self->capacity);

    for (int i = 0; i < self->capacity; i++)
    {
        if (i < self->size) 
        {
            if (i == 0 || i == self->size - 1) printf ("       *[%d]: %x\n", i, self->data[i]);
            else
            {
                printf ("       *[%d]: %d\n", i, self->data[i]);
            }
            
        }
        else
        {
            printf ("        [%d]: %x\n", i, self->data[i]);
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
    if (self->size <= 0)
        err += 4;
    if (self->capacity < self->size)
        err += 8;
    if (self->capacity <= 0)
        err += 16;
    if (self->data[0] != LEFT_COCK || self->data[self->size-1] != RIGHT_COCK)
        err += 32;

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

    case DATA_ACCESS_VIOLATION:
        printf("DATA ARRAY WAS CHANGED WTIHOUT THE PERMISSION, HANDS OFF!\n");
        break;

    default:
        printf("Idk, man. Something spooky. Error code %d", error_code);
    };
}


void PutDividers()
{
    printf ("=========================================================\n");
}
