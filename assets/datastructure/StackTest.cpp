//============================
//StackTest.cpp
//============================

//#define SEQ_STACK
#define LINKED_STACK

#if defined(SEQ_STACK)
#include "SequenceStack.h"
#elif defined(LINKED_STACK)
#include "LinkedStack.h"
#endif // SEQ_STACK

int main(){
#if defined(SEQ_STACK)
    SequenceStack<int> stack;
#elif defined(LINKED_STACK)
    LinkedStack<int> stack;
#endif // LINKED_STACK
    int nArray[10]={1,2,6,9,0,3,8,7,5,4};
    for( int nIdx = 0 ; nIdx < 10 ; nIdx++ )
        stack.push(nArray[ nIdx]);
    stack.print();

    cout<<"push data:88"<<endl;
    stack.push(88);
    stack.print();
    cout<<endl;

    cout<<"pop data: "<<stack.pop()<<endl;
    stack.print();

    cout<<"clear stack"<<endl;
    stack.clear();
    stack.print();

    cout<<"pop data: "<<endl;
    cout<<stack.pop()<<endl;
    return 1;
}
