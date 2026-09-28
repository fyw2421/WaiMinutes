//============================
//SequenceListTest.cpp
//============================

#include "SequenceList.h"

int main(){
    cout<<"Init size:15 SequenceList and insert 15 data"<<endl;
    SequenceList<int> sequenceList(15);
    int array[15]={2,5,8,1,9,9,7,6,4,3,2,9,7,7,9};
    for(int i=0;i<15;i++){
        if( !sequenceList.insert(array[i],0) )
            cout<<"insert failure"<<endl;
    }
    cout<<"SequenceList Length: "<<sequenceList.length()<<endl;
    sequenceList.print();


    cout<<"insert data:1 in index:0"<<endl;
    if( !sequenceList.insert(1,0) )
        cout<<"insert failure over maxsize:15"<<endl;
    sequenceList.print();

    cout<<(sequenceList.find(0)?"can't be found ":"Be found ")<< 0 << endl<<endl;

    cout<<"remove data:7"<<endl;
    sequenceList.remove(7);
    sequenceList.print();
    cout<<endl<<endl;

    cout<<"remove data:9"<<endl;
    sequenceList.remove(9);
    sequenceList.print();
    cout<<endl<<endl;

    cout<<"remove data:0"<<endl;
    sequenceList.remove(0);
    sequenceList.print();
    cout<<endl<<endl;

    cout<<"insert data in index:2"<<endl;
    sequenceList.insert(10,2);
    sequenceList.print();
    cout<<endl<<endl;
    return 1;
}
