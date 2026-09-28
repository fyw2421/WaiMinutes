//============================
//BinaryTreeTest.cpp
//============================

#include "BinaryTree.h"
#include <typeinfo>

#define INPUT_DATA_BY_INT_ARRAY
//#define INPUT_DATA_BY_CHAR_ARRAY
//#define INPUT_DATA_BY_KEYBOARD

int main(){

#if defined(INPUT_DATA_BY_INT_ARRAY)
    cout<<"Test data from int array"<<endl<<endl;
    int anData[]={1, 2 ,4 ,-1, -1, 5, -1, -1, 3, -1, 6, -1, -1};

    for( auto data : anData)
        cout<<data<<" ";
    cout<<endl<<endl;

    auto emptySubTreeData = -1;
    auto *pData = anData  ;
#elif defined(INPUT_DATA_BY_CHAR_ARRAY)
    cout<<"Test data from char array"<<endl<<endl;;
    char szData[]="adbf..g..e.h..c..";
    for( auto data : szData)
        cout<<data<<" ";
    cout<<endl<<endl;

    auto emptySubTreeData = '.';
    auto *pData = szData;
#elif defined(INPUT_DATA_BY_KEYBOARD)
    //test data
    //adbf..g..e.h..c..
    //1 2 4 -1 -1 5 -1 -1 3 -1 6 -1 -1
    auto emptySubTreeData = -1; //-1 or '.'
    decltype(emptySubTreeData) *pData = nullptr;
    cout<<"Test data from keyboard input"<<endl;
#endif // defined

    cout<<"Tree Data Type : "<<typeid(emptySubTreeData).name()<<endl;
    cout<<"Empty SubTree Vaule : "<<emptySubTreeData<<endl<<endl;

    BinaryTree<decltype(emptySubTreeData)> binaryTree;
    binaryTree.create(pData,emptySubTreeData);
    cout<<endl<<endl;

    cout<<"PreOrder : "<<endl;
    binaryTree.preOrder();
    cout<<endl<<endl;;

    cout<<"InOrder "<<endl;
    binaryTree.inOrder();
    cout<<endl<<endl;

    cout<<"PostOrder : "<<endl;
    binaryTree.postOrder();
    cout<<endl<<endl;

    cout<<"Tree Height : "<<binaryTree.getHeight()<<endl<<endl;
    cout<<"Tree Size : "<<binaryTree.getSize()<<endl<<endl;
    cout<<"Tree Leaf : "<<binaryTree.getLeaf()<<endl;

    binaryTree.destroy();
    return 1;
}
