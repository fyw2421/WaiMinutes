//============================
//DoubleLinkedListTest.cpp
//============================
#include "DoubleLinkedList.h"

int main(){
    DoubleLinkedList<int> doubleLinkedList;

    cout<<"insert data=index*3 in index:[0,20)"<<endl;
    for(int i=0;i<20;i++){
        doubleLinkedList.insert(i*3,i);
    }
    doubleLinkedList.print();
    cout<<"DoubleLinkedList length:"<<doubleLinkedList.length()<<endl<<endl;

    cout<<"insert 100 in index:21"<<endl;
    doubleLinkedList.insert(100,21);
    doubleLinkedList.print();
    cout<<"DoubleLinkedList length:"<<doubleLinkedList.length()<<endl<<endl;

    cout<<"insert 3 in index:[0,5)*3"<<endl;
    for(int i=0;i<5;i++){
        doubleLinkedList.insert(3,i*3);
    }
    doubleLinkedList.print();
    cout<<"DoubleLinkedList length:"<<doubleLinkedList.length()<<endl<<endl;

    cout<<"remove index:5"<<endl;
    doubleLinkedList.remove(5);
    doubleLinkedList.print();
    cout<<endl<<"DoubleLinkedList length:"<<doubleLinkedList.length()<<endl<<endl;

    cout<<"find index of data 57 "<<endl;
    cout<<endl<<doubleLinkedList.find(57)<<endl;
    return 1;
}
