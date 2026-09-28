//============================
//QueueTest.cpp
//============================

#define SEQ_QUEUE
//#define LINKED_QUEUE

#if defined(SEQ_QUEUE)
#include "SequenceQueue.h"
#elif defined(LINKED_QUEUE)
#include "LinkedQueue.h"
#endif

int main(){
#if defined(SEQ_QUEUE)
    SequenceQueue<int> queue;
#elif defined(LINKED_QUEUE)
    LinkedQueue<int> queue;
#endif
    int nArray[8]={1,6,9,0,2,5,8,3};
    for( int nIdx = 0 ; nIdx < 8 ; nIdx++)
        queue.enter(nArray[nIdx]);
    queue.print();

    cout<<"Leave Queue Data: "<<queue.leave()<<endl;
    queue.print();

    return 1;
}
