
#include "include/stack.h"


void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line)
{
    self->data = (elem_t *) calloc (sizeof(elem_t), capacity);

    self->size = CANARY_COUNT; // CANARY_COUNT
    self->capacity = capacity;
    self->hash = 0;
    self->subhash = 0;

    self->data[0] = LEFT_COCK;    
    self->data[1] = RIGHT_COCK;  
    
    self->stack_info.hash_ignore_ptr = &(self->hash); 
    self->stack_info.hash_skip = sizeof (int64_t); // ??

    self->stack_info.data_corrupted = false;
    self->stack_info.mother_func = funcname;
    self->stack_info.mother_file = filename;
    self->stack_info.name = name + 1;       // skips '&' symbol in the name

    printf ("\n\nData ptr %p\n\n", self->data);

    HASH_FUNC;
}


elem_t StackPop (Stack* self)
{
    Verificate (self);
    StackResize (self, DECREASE);
    StackDump (self);

    elem_t tmp = self->data[self->size - 2];
    
    self->data[self->size - 2] = RIGHT_COCK;
    self->data[self->size - 1] = POISON_NUM;

    self->size--;

    HASH_FUNC;

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

    HASH_FUNC;
}


void* recalloc (void* ptr, int len_new, size_t size)
{
    void* new_ptr = nullptr;

    int len_old = _msize (ptr);

    new_ptr = (void*) calloc (len_new, size);

    memcpy (new_ptr, ptr, min(len_old, len_new) * size);
    free (ptr);
    
    return new_ptr;
}


void StackResize (Stack* self, int mode)
{
    switch (mode)
    {
    case INCREASE:
        if (self->capacity - self->size < 1)
        {
            self->data = (elem_t*) recalloc (self->data, self->capacity * MEMORY_MULTIPLIER, sizeof (elem_t));
            self->capacity *= MEMORY_MULTIPLIER;

            for (int i = self->size + 1; i < self->capacity; i++)
            {
                self->data[i] = POISON_NUM;
            }
        }

        break;
    
    case DECREASE:
        if ((self->capacity / MEMORY_MULTIPLIER) - self->size > RESIZE_OFFSET)
        {
            self->data = (elem_t*) recalloc (self->data, self->capacity / 2, sizeof (elem_t));
            self->capacity /= MEMORY_MULTIPLIER;

            for (int i = self->size + 1; i < self->capacity; i++)
            {
                self->data[i] = POISON_NUM;
            }
        }

        break;

    default:
        printf ("STACK RESIZE ERROR\n");
        break;
    }

    HASH_FUNC;
        
}


void StackDtor (Stack* self)
{
    free (self->data);
    self->size = -1;
    self->capacity = -1;
    self->hash = 0;

    StackInfo tmp = {}; // memset()
    self->stack_info = tmp;
}


//#define 200 // $$

void StackDump_ (Stack* self, const char* filename, const char* funcname, int line)
{
    Verificate (self);
    PutDividers();
    
    //Verificate (self); 

    printf ("At file: %s\n", filename);

    printf ("Observing stack[%p] - %s, function: %s (Line %d)):\n", self, self->stack_info.name, funcname, line);
    printf ("%c Created at %s, file %s\n", 200, self->stack_info.mother_func, self->stack_info.mother_file);

    printf ("%cSize: %d\n", 204, self->size);
    printf ("%cHash: %lld \n", 204, self->hash);
    printf ("%cHash-ignore ptr: %p \n", 204, self->stack_info.hash_ignore_ptr);
    printf ("%cSkip amount: %u \n", 204, self->stack_info.hash_skip);
    printf ("%cCapacity: %d\n%cData array:\n", 204, self->capacity, 200);

    if (self->stack_info.data_corrupted) 
        printf ("\n************Stack was corrupted, stop cringe.************\n\n");

    for (int i = 0; i < self->capacity; i++)
    {
        if (i < self->size) 
        {
            if (i == 0 || i == self->size - 1) printf ("\t*[%d]: %x\n", i, self->data[i]);
            else                               printf ("\t*[%d]: %d\n", i, self->data[i]);         
        }
        else
        {
            printf ("\t [%d]: %x\n", i, self->data[i]);
        }
    }

    PutDividers();
}


lld StackVerificator (Stack *self)
{
    lld err = 0;
    
    printf ("---------------------Verifying stack: %p---------------------\n", self);

    if (self == nullptr)
    {
        err |= NULL_STACK;
        return err;
    }
    if (self->data == nullptr)
        err |= NULL_DATA; 
    if (self->size < 2)
        err |= INVALID_SIZE;
    if (self->capacity < self->size)
        err |= N_ENOUGH_SIZE;
    if (self->capacity <= 0)
        err |= INVALID_CAPACITY;
    if (self->data[0] != LEFT_COCK || self->data[self->size-1] != RIGHT_COCK)
    {
        err |= 32;
        self->stack_info.data_corrupted = true;
    }

    lld hash = HashFunc (self, sizeof (Stack), self->stack_info.hash_ignore_ptr, sizeof(int64_t) * 2);
    lld subhash = HashFunc (self->data, sizeof (elem_t) * self->capacity, nullptr, 0);

    //printf ("\n\nDATA HASH IS: %lld\n\n\n", HashFunc (self->data, sizeof (elem_t) * self->capacity, nullptr, 0));
    printf ("We have hash: %u and %u, ok?\n", hash, self->hash);
    printf ("We have subhash: %u and %u, ok?\n", subhash, self->subhash);

    if (self->hash != hash || self->subhash != subhash)
    {
        err +=STACK_MEMORY_CORRUPTION;
        self->stack_info.data_corrupted = true;        
    }

    return err; 
}


void Verificate (Stack* self)
{
    lld err = StackVerificator (self);

    printf ("Error code = %d\n", err);
    PutErrCodes (err);

    if (err == 0) printf ("ok\n");
}


void PutErrCodes (lld err)
{
    // NUM_OF_ERRORS??
    for (int i = 0; i <= 10; i++)
    {
        int cur_bit = GetBit(err, i);

        if (cur_bit)
        {
            PrintError(1 << i);
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

    case STACK_MEMORY_CORRUPTION:
        printf("MEMORY GIVEN FOR CURRENT STACK WAS CORRUPTED, further actions are unsafe.\n");
        break;

    default:
        printf("Idk, man. Something spooky. Error code %d\n", error_code);
    };
}


int64_t HashFunc (void* ptr, size_t size, void* skip_ptr, size_t skip_amount)
{
    assert (ptr != nullptr);

    printf ("Got ptr %p\n", ptr);

    Stack* tmp = (Stack*) ptr;

    int64_t h = 0xFACFAC;

    char* cur_ptr = (char*) ptr;
    char* end_ptr = cur_ptr + size - 1;

    for (; cur_ptr < end_ptr; cur_ptr++)
    {
        //if (tmp->data == (elem_t*) cur_ptr) printf ("\n\nEntering data zone:\n\n\n");
        //printf ("Now at ptr: %p ", cur_ptr);
        //printf ("Cycle\n");
        if (cur_ptr == (char*) skip_ptr)
        {
            printf ("Time to skip ptr: %p\n", cur_ptr);

            cur_ptr += skip_amount - 1;
            continue;
        }

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



