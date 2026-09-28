//============================
//ThreadedBinaryTreeTest.cpp
//============================

#include "ThreadedBinaryTree.h"

int main(){
    cout<<"Test data from char array"<<endl<<endl;;
    char szData[]= "adbf..g..e.h..c..";
    for( auto data : szData)
        cout<<data<<" ";
    cout<<endl<<endl;

    auto emptySubTreeData = '.';
    auto *pData = szData;

    //----preOrder thread and traverse begin----
    ThreadedBinaryTree<decltype(emptySubTreeData)> preOrderThreadedBinaryTree(BinaryTreeThreadType::PRE_THREAD);
    preOrderThreadedBinaryTree.create(pData,emptySubTreeData);

    cout<<"PreOrder : "<<endl;
    preOrderThreadedBinaryTree.preOrder();
    cout<<endl<<endl;
    cout<<"PreOrder Thread : "<<endl;
    preOrderThreadedBinaryTree.thread();
    preOrderThreadedBinaryTree.traverse();
    cout<<endl<<endl;
    //----preOrder thread and traverse end----

    //----inOrder thread and traverse begin----
    ThreadedBinaryTree<decltype(emptySubTreeData)> inOrderThreadedBinaryTree(BinaryTreeThreadType::IN_THREAD);
    pData = szData;
    inOrderThreadedBinaryTree.create(pData,emptySubTreeData);

    cout<<"InOrder : "<<endl;
    inOrderThreadedBinaryTree.inOrder();
    cout<<endl<<endl;
    cout<<"InOrder Thread : "<<endl;
    inOrderThreadedBinaryTree.thread();
    inOrderThreadedBinaryTree.traverse();
    cout<<endl<<endl;
    //----inOrder thread and traverse end----

    //----postOrder thread and traverse begin----
    ThreadedBinaryTree<decltype(emptySubTreeData)> postOrderThreadedBinaryTree(BinaryTreeThreadType::POST_THREAD);
    pData = szData;
    postOrderThreadedBinaryTree.create(pData,emptySubTreeData);

    cout<<"PostOrder : "<<endl;
    postOrderThreadedBinaryTree.postOrder();
    cout<<endl<<endl;
    cout<<"PostOrder Thread : "<<endl;
    postOrderThreadedBinaryTree.thread();
    postOrderThreadedBinaryTree.traverse();
    cout<<endl<<endl;
    //----postOrder thread and traverse end----
    return 1;
}
