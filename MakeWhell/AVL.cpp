//
// Created by yjs on 2022/6/15.
//
//#include <iostream>
//#include <string>
//
//template<class Key>
// struct AVLNode{
//    Key key;
//    int height;
//    struct AVLNode* left;
//    struct AVLNode* right;
//    AVLNode(Key key, AVLNode<Key> * left, AVLNode<Key> * right, int height){
//        this->key=key;
//        this->left=left;
//        this->right=right;
//        this->height=height;
//    }
//};
#include "AVL.h"




template<class Key>
inline int getHeight(AVLNode<Key> * node)
{
    return (node== nullptr )? 0:node->height;
}

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
AVLNode<Key> * LL_Rotate(AVLNode<Key> * k2)
{
    AVLNode<Key> * k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = std::max(getHeight(k2->right), getHeight(k2->left)) + 1;
    k1->height = std::max(getHeight(k1->left), k2->height) + 1;
    return k1;
}



/* RR (Y rotates to the left): (Left Rotation)
 * 根节点的右孩子的右子树添加了新节点

        k2                       k1
       /  \                     /  \
      X    k1         ==>      k2   Z
          /  \                /  \
         Y    Z              X    Y
 */
template<class Key>
AVLNode<Key> * RR_Rotate(  AVLNode<Key> * k2)
{
    AVLNode<Key>* k1 = k2->right;
    k2->right = k1->left;
    k1->left = k2;
    k2->height = std::max(getHeight(k2->left), getHeight(k2->right)) + 1;
    k1->height = std::max(getHeight(k1->right), k2->height) + 1;
    return k1;
}



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
AVLNode<Key> * LR_Rotate(AVLNode<Key> * k3)
{
    k3->left = RR_Rotate(k3->left);
    return LL_Rotate(k3);
}


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
AVLNode<Key> * RL_Rotate(AVLNode<Key> * k3)
{
    k3->right = LL_Rotate(k3->right);
    return RR_Rotate(k3);
}


template<class Key>
AVLNode<Key> * Insert(AVLNode<Key> * root, int key)
{
    if(root == nullptr)
    {

        root=new AVLNode<Key>(key);
        return root;
    }
    else if(key < root->key)
        root->left = Insert(root->left, key);
    else //key >= root->key
        root->right = Insert(root->right, key);

    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;

    if(getHeight(root->left) - getHeight(root->right) == 2)
    {
        // 操作root的左孩子
        if(key < root->left->key)
            root = LL_Rotate(root);
        else
            root = LR_Rotate(root);
    }
    else if(getHeight(root->right) - getHeight(root->left) == 2)
    {
        // 操作root的右孩子
        if(key < root->right->key)
            // 右孩子的左子树
            root = RL_Rotate(root);
        else
            // 操作右孩子的右子树
            root = RR_Rotate(root);
    }
    return root;
}


template<class Key>
AVLNode<Key> * Delete(AVLNode<Key> * root, Key key)
{
    if(root== nullptr)
        return nullptr;
    if(key == root->key)
    {
        if(root->right == nullptr)
        {
            AVLNode<Key> * temp = root;
            root = root->left;
            delete(temp);
            return root;
        }else if(root->left== nullptr){
            AVLNode<Key> * temp = root;
            root = root->right;
            delete(temp);
            return root;
        }
        else{
            // root's left and right all exists
            AVLNode<Key> * temp = root->right;
            while(temp->left!= nullptr) temp = temp->left;
            /* replace the value */
            root->key = temp->key;
            /* Delete the node (successor node) that should be really deleted */
            root->right = Delete(root->right, temp->key);
        }
    }
    else if(key < root->key)
        root->left = Delete(root->left, key);
    else
        root->right = Delete(root->right, key);

    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    if(getHeight(root->right) - getHeight(root->left) == 2)
    {
        if(getHeight(root->right->right) >= getHeight(root->right->left))
            root = RR_Rotate(root);
        else
            root = RL_Rotate(root);
    }
    else if(getHeight(root->left) - getHeight(root->right) == 2)
    {
        if(getHeight(root->left->left) >= getHeight(root->left->right))
            root = LL_Rotate(root);
        else
            root = LR_Rotate(root);
    }
    return root;
}


//template<class Key>
//void output_impl(AVLNode<Key> * n, bool left, std::string & indent)
//{
//    using std::cout;
//    using std::endl;
//    if (n->right)
//    {
//        output_impl(n->right, false, indent + (left ? "|     " : "      "));
//    }
//    cout << indent;
//    cout << (left ? '\\' : '/');
//    cout << "-----";
//    cout << n->key << endl;
//    if (n->left)
//    {
//        output_impl(n->left, true, indent + (left ? "      " : "|     "));
//    }
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//template<class Key>
//void output(AVLNode<Key> * root)
//{
//    if (root->right)
//    {
//        output_impl(root->right, false, "");
//    }
//    std::cout << root->key << std::endl;
//    if (root->left)
//    {
//        output_impl(root->left, true, "");
//    }
//}

template<class Key>
vector<vector<int>> levelOrder(AVLNode<Key> *root) {
    vector<vector<int>> res;
    if (root == nullptr) {
        return res;
    }

    queue<AVLNode<Key> *> queueTreeNodes;
    queueTreeNodes.push(root);
    while (!queueTreeNodes.empty()) {

        int sz = queueTreeNodes.size();
        vector<int> cens;
        for (int i = 0; i < sz; ++i) {
            AVLNode<Key> *node = queueTreeNodes.front();
            queueTreeNodes.pop();
            cens.push_back(node->key);
            if (node->left != nullptr) {
                queueTreeNodes.push(node->left);
            }
            if (node->right != nullptr) {
                queueTreeNodes.push(node->right);
            }

        }
        res.push_back(cens);


    }
    return res;


}


template<class Key>
void InOrder(AVLNode<Key> * root)
{
    if(root)
    {
        InOrder(root->left);
        printf("key: %d height: %d ", root->key, root->height);
        if(root->left)
            printf("left child: %d ", root->left->key);
        if(root->right)
            printf("right child: %d ", root->right->key);
        printf("\n");
        InOrder(root->right);
    }
}



template<class Key>
void PrintTree(AVLNode<Key> * root){
    cout << "##################" <<endl;
    auto res= levelOrder(root);
    for (auto c:res) {
        for (auto b :c) {
            cout << "\t"<< b<< "\t";
        }
        cout <<endl;

    }
    cout << "##################" <<endl;

}
