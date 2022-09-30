
#include "include/stack.h"


int main ()
{
    Stack stk1 = {};
    StackCtor(&stk1, 5);

    //Cringeeeeeee
    //stk1.data = (elem_t*)1;
    //stk1.data = NULL;

    //Ne poluchilos
    //stk1.data[3] = 100; 

    //fprintf (stderr, "%d\n", stk1.stack_info.mother_file);

    StackDump (&stk1);

    //Poluchilos
    //printf ("\n\nWe have %p and %p \n\n\n\n", "You down", stk1.stack_info.mother_file);
    //stk1.stack_info.mother_file = "Bebra amongus";
    //stk1.capacity = 10;
        
    

    StackDump (&stk1);

    //fprintf (stderr, "%d\n", stk1.stack_info.mother_file);

    //stk1.data[3] = 10;
    
    //StackDump (&stk1);
    
    for (int i = 0 ; i < 6; i++)
    {
        StackPush (&stk1, i);
    }

    for (int i = 0 ; i < 6; i++)
    {
        StackPop (&stk1);
    }
    
    //stk1.data[1] = 2;

    //StackDump (&stk1);
    
    StackDtor(&stk1);
}