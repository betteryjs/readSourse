//
// Created by yjs on 2022/6/16.
//

#ifndef WORKFLOWSTUDY_AVL_H
#define WORKFLOWSTUDY_AVL_H
#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

template<class T>
struct AVLNode{
    T key;
    int height;
    struct AVLNode<T> * left;
    struct AVLNode<T> * right;
    AVLNode(T tempKey){
        key=tempKey;
        left= nullptr;
        right=nullptr;
        height=1;
    }
};


template<class Key>
inline int getHeight(AVLNode<Key> * node);

/* LL(Y rotates to the right): ( Right Rotation )
 * 根节点的左孩子的左子树添加了新节点

        k2                   k1
       /  \                 /  \
      k1   Z     ==>       X   k2
     / \                      /  \
    X   Y                    Y    Z
*/
/*
 Return which the root pointer(at a higher level) should point to
 */
template<class Key>
AVLNode<Key> * LL_Rotate(AVLNode<Key> * k2);



/* RR (Y rotates to the left): (Left Rotation)
 * 根节点的右孩子的右子树添加了新节点

        k2                       k1
       /  \                     /  \
      X    k1         ==>      k2   Z
          /  \                /  \
         Y    Z              X    Y
 */
template<class Key>
AVLNode<Key> * RR_Rotate(  AVLNode<Key> * k2);



/* LR(B rotates to the left, then C rotates to the right):
 * 左孩子的右子树添加了新节点

      k3                         k3                       k2
     /  \                       /  \                     /  \
    k1   D                     k2   D                   k1   k3
   /  \         ==>           /  \        ==>          / \   / \
  A    k2                    k1   C                   A  B  C   D
      /  \                  /  \
     B    C                A    B

*/
/*
 Return which the root pointer should point to
 */
template<typename Key>
AVLNode<Key> * LR_Rotate(AVLNode<Key> * k3);


/* RL(D rotates to the right, then C rotates to the left):
 * 右孩子的左子树添加了新节点


       k3                         k3                          k2
      /  \                       /  \                        /  \
     A    k1                    A    k2                     k3   k1
         /  \       ==>             /  \         ==>       /  \  / \
        k2   B                     C    k1                A   C D   B
       /  \                            /  \
      C    D                          D    B

 */
template<class Key>
AVLNode<Key> * RL_Rotate(AVLNode<Key> * k3);


template<class Key>
AVLNode<Key> * Insert( AVLNode<Key> * root, int key);

template<class Key>
AVLNode<Key> * Delete(AVLNode<Key> * root, Key key);

//template<class Key>
//void output_impl(AVLNode<Key> * n, bool left, std::string const& indent);
//
//template<class Key>
template<class Key>
vector<vector<int>> levelOrder(AVLNode<Key> *root);


//void output(AVLNode<Key> * root);

template<class Key>
void InOrder(AVLNode<Key> * root);

template<class Key>
void PrintTree(AVLNode<Key> * root);


//#include "AVL.cpp"

#endif //WORKFLOWSTUDY_AVL_H
