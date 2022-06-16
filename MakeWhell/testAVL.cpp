//
// Created by yjs on 2022/6/15.
//

#include "AVL.h"
#include "AVL.cpp"





int main(){


//    Test test;
//    test.Go();
//
//    auto * root= new AVLNode<int> (12);
    AVLNode<int>*  root= nullptr;
//    root=Insert(root,25);
//    cout << root->key<<endl;
    vector<int> nums{12,15,25,63,23};
    for (const int & c:nums) {
        root= Insert(root,c);

    }
    PrintTree(root);
    Delete(root,15);
    PrintTree(root);
    return 0;


}