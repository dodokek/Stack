
#include "include/stack.h"


void StackCtor_ (Stack* self, size_t capacity, const char* name, const char* filename, const char* funcname, int line)
{
    #ifdef CANARY

    self->left_cock = LEFT_COCK;
    self->right_cock = RIGHT_COCK;
    
    #endif

    self->data = (elem_t *) calloc (sizeof(elem_t), capacity);

    self->size = 0; 
    self->capacity = capacity;

    #ifdef HASH

    self->hash = 0;
    self->subhash = 0; 

    #endif

    self->stack_info.data_corrupted = false;
    self->stack_info.mother_func = funcname;
    self->stack_info.mother_file = filename;
    self->stack_info.name = name + 1;       // skips '&' symbol in the name

    #ifdef HASH
    DO_REHASH;
    #endif
}


elem_t StackPop (Stack* self)
{
    #ifdef DEBUG

    Verificate (self);
    StackDump (self);

    #endif

    StackResize (self, DECREASE);

    elem_t tmp = self->data[self->size - 1];

    self->size--;

    #ifdef HASH
    DO_REHASH;
    #endif

    return tmp;     
}


void StackPush (Stack* self, elem_t value)
{
    #ifdef DEBUG

    Verificate (self);
    StackDump (self);

    #endif

    StackResize (self, INCREASE);

    self->data[self->size] = value;

    self->size++;

    #ifdef HASH
    DO_REHASH;
    #endif
}


void* recalloc (void* ptr, int len_new, size_t size)
{
    int len_old = _msize (ptr);

    ptr = (void*) realloc (ptr, len_new * size);

    fill_array ((elem_t*) (ptr + len_old), (elem_t*) (ptr + len_new * size), 0);
    
    return ptr;
}


void StackResize (Stack* self, int mode)
{ 
    switch (mode)
    {
    case INCREASE:
        if (self->capacity - self->size <= 1)
        {
            self->data = (elem_t*) recalloc (self->data, self->capacity * MEMORY_MULTIPLIER, sizeof (elem_t));
            self->capacity *= MEMORY_MULTIPLIER;

            #ifdef DEBUG
            fill_array (self->data + self->size, self->data + self->capacity, POISON_NUM);
            #endif
        }

        break;
    
    case DECREASE:
        if ((self->capacity / MEMORY_MULTIPLIER) - self->size >= RESIZE_OFFSET)
        {
            self->data = (elem_t*) recalloc (self->data, self->capacity / 2, sizeof (elem_t));
            self->capacity /= MEMORY_MULTIPLIER;

            #ifdef DEBUG
            fill_array (self->data + self->size, self->data + self->capacity, POISON_NUM);
            #endif
        }

        break;

    default:
        printf ("STACK RESIZE ERROR\n");
        break;
    }

    #ifdef HASH
    DO_REHASH;
    #endif
}


void StackDtor (Stack* self)
{
    free (self->data);
    self->data = nullptr; 
    self->size = -1;
    self->capacity = -1;
    self->hash = 0;
    self->subhash = 0;

    StackInfo tmp = {}; // memset()
    self->stack_info = tmp;
}


void StackDump_ (Stack* self, const char* filename, const char* funcname, int line)
{
    Verificate (self);
    PutDividers();

    printf ("At file: %s\n", filename);

    printf ("Observing stack[%p] - %s, function: %s (Line %d)):\n", self, self->stack_info.name, funcname, line);
    printf ("%c Created at %s, file %s\n", 200, self->stack_info.mother_func, self->stack_info.mother_file);

    printf ("%cSize: %d\n", 204, self->size);
    printf ("%cHash: %I64d \n", 204, self->hash);
    printf ("%cSubhash: %I64d \n", 204, self->subhash);
    printf ("%cLeft canary: %x\n", 204, self->left_cock);
    printf ("%cRight canary: %x\n", 204, self->right_cock);
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


intmax_t StackVerificator (Stack *self)
{
    intmax_t err = 0;
    
    printf ("---------------------Verifying stack: %p---------------------\n", self);

    if (self == nullptr)
    {
        err |= NULL_STACK;
        return err;
    }
    if (self->data == nullptr)
    {
        err |= NULL_DATA; 
        return err;
    }   
    if (self->size <= 0)
        err |= INVALID_SIZE;
    if (self->capacity < self->size)
        err |= N_ENOUGH_SIZE;
    if (self->capacity <= 0)
        err |= INVALID_CAPACITY;

    #ifdef CANARY    

    if (self->left_cock != LEFT_COCK || self->right_cock != RIGHT_COCK)
    {
        err |= 32;
        self->stack_info.data_corrupted = true;
    }

    #endif

    #ifdef HASH

    Stack tmp = *self;
    tmp.hash = 0;
    tmp.subhash = 0;

    intmax_t hash = HashFunc (&tmp, sizeof (Stack));
    intmax_t subhash = HashFunc (self->data, sizeof (elem_t) * self->capacity);

    //printf ("\n\nDATA HASH IS: %intmax_t\n\n\n", HashFunc (self->data, sizeof (elem_t) * self->capacity, nullptr, 0));
    printf ("We have hash: %I64d and %I64d, ok?\n", hash, self->hash);
    printf ("We have subhash: %I64d and %I64d, ok?\n", subhash, self->subhash);

    if (self->hash != hash || self->subhash != subhash)
    {
        err += STACK_MEMORY_CORRUPTION;
        self->stack_info.data_corrupted = true;        
    }

    #endif

    return err; 
}


void Verificate (Stack* self)
{
    intmax_t err = StackVerificator (self);

    printf ("Error code = %I64d\n", err);
    PutErrCodes (err);

    if (err == 0) printf ("ok\n");
}


void PutErrCodes (intmax_t err)
{
    for (int i = 0; i <= ERRORS_COUNT; i++)
    {
        int cur_bit = GetBit(err, i);

        if (cur_bit)
        {
            PrintError (1 << i);
        }
    }
}


int GetBit (intmax_t n, int pos)
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


intmax_t HashFunc (void* ptr, size_t size)
{
    //printf ("\n\nRecieved ptr %p\n\n", ptr);
    assert (ptr != nullptr);

    intmax_t h = 0xFACFAC;

    char* cur_ptr = (char*) ptr;
    char* end_ptr = cur_ptr + size - 1;

    for (; cur_ptr < end_ptr; cur_ptr++)
    {
        h = ((h + (*cur_ptr)) * SALT) % (HASH_MOD);
    }

    //printf ("Hash result: %intmax_t, size = %lu\n", h, size);

    return h;
} 


elem_t min (elem_t elem1, elem_t elem2)
{
    return (elem1 < elem2) ? elem1 : elem2;
}


void fill_array (elem_t* cur_ptr, elem_t* end_ptr, elem_t filler)
{
    while (cur_ptr < end_ptr)
    {
        *cur_ptr = filler;
        cur_ptr++;
    }
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



