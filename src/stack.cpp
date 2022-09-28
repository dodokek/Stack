

#include "include/stack.h"

int main()
{
    Stack stk1 = {};
    StackCtor(&stk1, 5);
    
    for (int i = 0; i < 5; i++)
    {
        StackPush (&stk1, i);    
    }

    stk1.data[1] = 4545;
    StackDump (&stk1);

    for (int i = 0; i < 4; i++)
    {
        printf("Popped elem: %d\n", StackPop (&stk1));    
    }
    

    StackDtor(&stk1);
}


elem_t StackPop (Stack* self)
{
    Verificate (self);
    StackResize (self, DECREASE);
    StackDump(self);

    elem_t tmp = self->data[self->size - 2];
    
    self->data[self->size - 2] = RIGHT_COCK;
    self->data[self->size - 1] = POISON_NUM;

    self->size--;

    HASH_STACK;

    return tmp;     
}


void StackPush (Stack* self, elem_t value)
{
    Verificate (self);
    StackResize (self, INCREASE);
    StackDump (self);

    self->data[self->size - 1] = value;
    self->data[self->size]     = RIGHT_COCK;
    self->size++;

    HASH_STACK;
}


void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line)
{
    self->data = (elem_t *) calloc (sizeof(elem_t), capacity);

    self->size = 2;
    self->capacity = capacity;
    self->hash = 0;

    self->data[0] = LEFT_COCK;    
    self->data[1] = RIGHT_COCK;  

    self->stack_info.data_corrupted = false;
    self->stack_info.hash_ignore_ptr = &(self->hash);
    self->stack_info.hash_skip = sizeof (ull_i);
    self->stack_info.mother_func = funcname;
    self->stack_info.name = name + 1;       // skips '&' symbol in the name
    self->stack_info.mother_file = filename;

    HASH_STACK;
}


void* recalloc (void* ptr, int len_old, int len_new, size_t size)
{
    void* new_ptr = nullptr;

    new_ptr = (void*) calloc (len_new, size);

    memcpy (new_ptr, ptr, (len_old < len_new) ? len_old : len_new);
    free (ptr);
    
    return new_ptr;
}


void StackResize (Stack* self, int mode)
{
    switch (mode)
    {
    case INCREASE:
        if (self->capacity - self->size < 2)
        {
            self->data = (elem_t*) recalloc (self->data, self->capacity, self->capacity * 2, sizeof (elem_t));
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
            self->data = (elem_t*) recalloc (self->data, self->capacity, self->capacity / 1.5, sizeof (elem_t));
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

    HASH_STACK;
        
}


void StackDtor (Stack* self)
{
    free (self->data);
    self->size = -1;
    self->capacity = -1;
    self->hash = 0;

    StackInfo tmp = {};
    self->stack_info = tmp;
}


void StackDump_ (Stack* self, const char* filename, const char* funcname, int line)
{
    PutDividers();
    
    //Verificate (self); 

    printf ("At file: %s\n", filename);

    printf ("Observing stack[%p] - %s, function: %s (Line %d)):\n", self, self->stack_info.name, funcname, line);
    printf ("%c Created at %s, file %s**\n", 200, self->stack_info.mother_func, self->stack_info.mother_file);

    printf ("    %cSize: %d\n", 204, self->size);
    printf ("    %cHash: %lld \n", 204, self->hash);
    printf ("    %cCapacity: %d\n    %cData array:\n", 204, self->capacity, 200);


    if (self->stack_info.data_corrupted) printf ("\n************Stack was corrupted, go fuck yourself.************\n\n");

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
    
    printf ("-------------Verifying stack: %p---------------------\n", self);

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

    lld hash = HashFunc (self->data, sizeof (self->data) * self->size);

    if (self->hash != hash)
    {
        err += 64;
        self->stack_info.data_corrupted = true;        
    }

    return err; 
}


void Verificate (Stack* self)
{
    lld err = StackVerificator(self);

    printf("Error code = %d\n", err);
    PutErrCodes (err);

    if (err == 0) printf ("ok\n");
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
        printf("INVALID SIZE, below zero probably.\n");
        break;
    case N_ENOUGH_SIZE:
        printf("N ENOUGH SIZE, Not enought size was located.\n");
        break;
    case INVALID_CAPACITY:
        printf("INVALID CAPACITY\n");
        break;

    case DATA_ACCESS_VIOLATION:
        printf("DATA ARRAY WAS CHANGED WTIHOUT THE PERMISSION, HANDS OFF!\n");
        break;

    case STACK_MEMORY_CURRUPTION:
        printf("MEMORY GIVEN FOR CURRENT STACK WAS CORRUPTED, further actions are unsafe.\n");
        break;

    default:
        printf("Idk, man. Something spooky. Error code %d\n", error_code);
    };
}


ull_i HashFunc (void* ptr, size_t size)
{
    assert (ptr != nullptr);

    ull_i h = 0xFACFAC;

    char* cur_ptr = (char*) ptr;
    char* end_ptr = cur_ptr + size - 1;

    for (; cur_ptr != end_ptr; cur_ptr++)
    {
        h = ((h % (1 << 30)) * 2 + *cur_ptr);
    }

    //printf ("Hash result: %lld, size = %lu\n", h, size);

    return h;
} 


void PutDividers()
{
    putc ('\n', stdout);
    for (int i = 0; i < 50; i++)
    {
        putc (219, stdout);
    }
    putc ('\n', stdout);
}
