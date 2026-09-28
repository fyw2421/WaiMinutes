//============================
//SingleLinkedListTest.cpp
//============================
#include "SingleLinkedList.h"

int main(){
    SingleLinkedList<int> singleLinkedList;

    cout<<"insert data=index*3 in index:[0,20)"<<endl;
    for(int i=0;i<20;i++){
        singleLinkedList.insert(i*3,i);
    }
    singleLinkedList.print();
    cout<<"SingleLinkedList length:"<<singleLinkedList.length()<<endl<<endl;

    cout<<"insert 100 in index:21"<<endl;
    singleLinkedList.insert(100,21);
    singleLinkedList.print();
    cout<<"SingleLinkedList length:"<<singleLinkedList.length()<<endl<<endl;

    cout<<"insert 3 in index:[0,5)*3"<<endl;
    for(int i=0;i<5;i++){
        singleLinkedList.insert(3,i*3);
    }
    singleLinkedList.print();
    cout<<"SingleLinkedList length:"<<singleLinkedList.length()<<endl<<endl;

    cout<<"remove index:5"<<endl;
    singleLinkedList.remove(5);
    singleLinkedList.print();
    cout<<endl<<"SingleLinkedList length:"<<singleLinkedList.length()<<endl<<endl;

    cout<<"find index for data:57 "<<endl;
    cout<<endl<<singleLinkedList.find(57)<<endl;
    return 1;
}
