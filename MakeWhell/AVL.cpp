//
// Created by yjs on 2022/6/15.
//
#include <iostream>

template<class Key>
 struct AVLNode{
    Key key;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
    AVLNode(Key key, AVLNode<Key> * left, AVLNode<Key> * right, int height){
        this->key=key;
        this->left=left;
        this->right=right;
        this->height=height;
    }
};

template<class Key>
inline int getHeight(AVLNode<Key> * node)
{
    return (node== nullptr )? -1:node->height;
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
    k2->height = max(getHeight(k2->left), getHeight(k2->right)) + 1;
    k1->height = max(getHeight(k1->left), k2->height) + 1;
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
AVLNode<Key> * Insert(AVLNode<Key> * root, Key key)
{
    if(root == nullptr)
        return (root = AVLNode<Key> (key, nullptr, nullptr));
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
    else if(getHeight(root->rchild) - getHeight(root->lchild) == 2)
    {
        // 操作root的右孩子
        if(key < root->right->key)
            root = RL_Rotate(root);
        else
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
            while(temp->left!= nullptr) temp = temp->lchild;
            /* replace the value */
            root->key = temp->key;
            /* Delete the node (successor node) that should be really deleted */
            root->right = Delete(root->right, temp->key);
        }
    }
    else if(key < root->key)
        root->lchild = Delete(root->left, key);
    else
        root->rchild = Delete(root->right, key);

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    if(getHeight(root->right) - getHeight(root->left) == 2)
    {
        if(getHeight(root->rchild->rchild) >= getHeight(root->rchild->lchild))
            root = RR_Rotate(root);
        else
            root = RL_Rotate(root);
    }
    else if(getHeight(root->lchild) - getHeight(root->rchild) == 2)
    {
        if(getHeight(root->lchild->lchild) >= getHeight(root->lchild->rchild))
            root = LL_Rotate(root);
        else
            root = LR_Rotate(root);
    }
    return root;
}


