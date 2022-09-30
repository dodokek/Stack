#include "include/stack.h"


int main ()
{
    Stack stk1 = {};
    StackCtor(&stk1, 5);
    
    StackDump (&stk1);
    
    for (int i = 0; i < 5; i++)
    {
        StackPush (&stk1, i);    
    }

    stk1.data[3] = 1000;
    StackDump (&stk1);

    for (int i = 0; i < 4; i++)
    {
        printf("Popped elem: %d\n", StackPop (&stk1));    
    }
    
    StackDtor(&stk1);
}