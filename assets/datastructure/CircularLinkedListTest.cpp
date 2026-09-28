//============================
//CircularLinkedListTest.cpp
//============================

#include "CircularLinkedList.h"

int main(){

    CircularLinkedList<int> circularLinkedList;

    cout<<"insert data=index*3 in index:[0,20)"<<endl;
    for(int i=0;i<20;i++){
        circularLinkedList.insert(i*3,i);
    }
    circularLinkedList.print();
    cout<<"CircularLinkedList length:"<<circularLinkedList.length()<<endl<<endl;

    cout<<"insert 100 in index:21"<<endl;
    circularLinkedList.insert(100,21);
    circularLinkedList.print();
    cout<<"CircularLinkedList length:"<<circularLinkedList.length()<<endl<<endl;

    cout<<"insert 3 in index:[0,5)*3"<<endl;
    for(int i=0;i<5;i++){
        circularLinkedList.insert(3,i*3);
    }
    circularLinkedList.print();
    cout<<"CircularLinkedList length:"<<circularLinkedList.length()<<endl<<endl;

    cout<<"remove index:5"<<endl;
    circularLinkedList.remove(5);
    circularLinkedList.print();
    cout<<endl<<"CircularLinkedList length:"<<circularLinkedList.length()<<endl<<endl;

    cout<<"find index of data 57 "<<endl;
    cout<<endl<<circularLinkedList.find(57)<<endl<<endl;

    cout<<"clear"<<endl;
    circularLinkedList.clear();
    cout<<"CircularLinkedList length:"<<circularLinkedList.length()<<endl<<endl;
    return 1;
}
