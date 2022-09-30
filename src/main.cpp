#include "include/stack.h"


int main ()
{
    Stack stk1 = {};
    StackCtor(&stk1, 5);
    
    StackDump (&stk1);
    
    for (int i = 0 ; i < 6; i++)
    {
        StackPush (&stk1, i);
    }

    for (int i = 0 ; i < 6; i++)
    {
        StackPop (&stk1);
    }
    //stk1.data[1] = 2;

    StackDump (&stk1);
    
    StackDtor(&stk1);
}