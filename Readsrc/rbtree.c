/*
  Red Black Trees
  (C) 1999  Andrea Arcangeli <andrea@suse.de>
  (C) 2002  David Woodhouse <dwmw2@infradead.org>
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  linux/lib/rbtree.c
*/

#include "rbtree.h"

static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *right = node->rb_right;


    /* 注意是 =
     * node->rb_right = right->rb_left
     * if(node->rb_right) right->rb_left->rb_parent = node
     * */

	if ((node->rb_right = right->rb_left))
		right->rb_left->rb_parent = node;
	right->rb_left = node;

    /*
     * right->rb_parent = node->rb_parent
     * if(right->rb_parent) { do something }
     * */

	if ((right->rb_parent = node->rb_parent))
	{
		if (node == node->rb_parent->rb_left)
			node->rb_parent->rb_left = right;
		else
			node->rb_parent->rb_right = right;
	}
	else
        // right->rb_parent == nullptr
		root->rb_node = right;
	node->rb_parent = right;
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;

	if ((node->rb_left = left->rb_right))
		left->rb_right->rb_parent = node;
	left->rb_right = node;

	if ((left->rb_parent = node->rb_parent))
	{
		if (node == node->rb_parent->rb_right)
			node->rb_parent->rb_right = left;
		else
			node->rb_parent->rb_left = left;
	}
	else
		root->rb_node = left;
	node->rb_parent = left;
}

void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;

    // 关于插入操作的平衡调整，有两种特殊情况
    // 1. 如果插入的结点的父结点是黑色的，那我们什么都不做，它仍然满足红黑树定义(这种情况不进入while，do nothing)
    // 2. 如果插入的是根结点，那么我们直接改变颜色成黑色即可(所以我们这里的parent为null时,不进入while，最后一行变黑

    // 三种情况：
    // 1）如果关注点node，它的叔叔结点d为红色 ，执行case1
    // 2）如果关注点node，它的叔叔结点d为黑色 ，且为父节点点右子节点，执行case2
    // 3）如果关注点node，它的叔叔结点d为黑色 ，且为父节点点左子节点，执行case3

    // case 1 ：
    // 操作是
    // 1）将关注点node的父节点，叔叔节点变黑
    // 2）将关注点node的祖父节点变红
    // 3）将关注点node变成祖父结点gparent
    // 4）跳转到case2 或者case3

    // case 2
    // 1) 关注点变成父节点
    // 2）围绕这个新的关注点左旋
    // 3）跳到case 3

    // case 3
    // 1）围绕关注点的祖父节点右旋
    // 2）将关注点的父节点和当前的兄弟节点互换颜色(其实就是父和祖交换)

    // node->rb_parent 不为 nullptr && node 不是根节点
    while ((parent = node->rb_parent) && parent->rb_color == RB_RED)
	{
		gparent = parent->rb_parent;

		if (parent == gparent->rb_left)
		{

			{
                // uncle 叔叔结点
				register struct rb_node *uncle = gparent->rb_right;
				if (uncle && uncle->rb_color == RB_RED)
				{
                    // case 1
					uncle->rb_color = RB_BLACK;  // node的叔叔节点变黑
					parent->rb_color = RB_BLACK; // node的父节点变黑
					gparent->rb_color = RB_RED;  // node的祖父节点变红
					node = gparent;              // node变成祖父结点gparent
					continue;                    // 继续迭代
				}
			}

            // 叔叔为黑，且 node 是父亲右节点
            // case2
			if (parent->rb_right == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_left(parent, root);
                // swap( tmp , parents )
				tmp = parent;
				parent = node;
				node = tmp;
                // end swap
			}

            // case3
            // 这里先变父和祖父颜色，然后rotate，效果一样
			parent->rb_color = RB_BLACK;
			gparent->rb_color = RB_RED;
			__rb_rotate_right(gparent, root);
		}

        else {
            // parent == gparent->rb_right
			{
				register struct rb_node *uncle = gparent->rb_left;
				if (uncle && uncle->rb_color == RB_RED)
				{
					uncle->rb_color = RB_BLACK;
					parent->rb_color = RB_BLACK;
					gparent->rb_color = RB_RED;
					node = gparent;
					continue;
				}
			}

			if (parent->rb_left == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			parent->rb_color = RB_BLACK;
			gparent->rb_color = RB_RED;
			__rb_rotate_left(gparent, root);
		}
	}

    // 什么操作都不作 将根节点的颜色赋值为 RB_BLACK
	root->rb_node->rb_color = RB_BLACK;
}


static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
			     struct rb_root *root)
{
	struct rb_node *other;

	while ((!node || node->rb_color == RB_BLACK) && node != root->rb_node)
	{
		if (parent->rb_left == node)
		{
			other = parent->rb_right;
			if (other->rb_color == RB_RED)
			{
                // case 1
				other->rb_color = RB_BLACK;
				parent->rb_color = RB_RED;
				__rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			if ((!other->rb_left ||
			     other->rb_left->rb_color == RB_BLACK)
			    && (!other->rb_right ||
				other->rb_right->rb_color == RB_BLACK))
			{
                // case 2
				other->rb_color = RB_RED;
				node = parent;
				parent = node->rb_parent;
			}
			else
			{
				if (!other->rb_right ||
				    other->rb_right->rb_color == RB_BLACK)
				{
                    // case 3
					register struct rb_node *o_left;
					if ((o_left = other->rb_left))
						o_left->rb_color = RB_BLACK;
					other->rb_color = RB_RED;
					__rb_rotate_right(other, root);
					other = parent->rb_right;
				}
                // case 4
				other->rb_color = parent->rb_color;
				parent->rb_color = RB_BLACK;
				if (other->rb_right)
					other->rb_right->rb_color = RB_BLACK;
				__rb_rotate_left(parent, root);
				node = root->rb_node;
				break;
			}
		}
		else
		{
			other = parent->rb_left;
			if (other->rb_color == RB_RED)
			{
				other->rb_color = RB_BLACK;
				parent->rb_color = RB_RED;
				__rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			if ((!other->rb_left ||
			     other->rb_left->rb_color == RB_BLACK)
			    && (!other->rb_right ||
				other->rb_right->rb_color == RB_BLACK))
			{
				other->rb_color = RB_RED;
				node = parent;
				parent = node->rb_parent;
			}
			else
			{
				if (!other->rb_left ||
				    other->rb_left->rb_color == RB_BLACK)
				{
					register struct rb_node *o_right;
					if ((o_right = other->rb_right))
						o_right->rb_color = RB_BLACK;
					other->rb_color = RB_RED;
					__rb_rotate_left(other, root);
					other = parent->rb_left;
				}
				other->rb_color = parent->rb_color;
				parent->rb_color = RB_BLACK;
				if (other->rb_left)
					other->rb_left->rb_color = RB_BLACK;
				__rb_rotate_right(parent, root);
				node = root->rb_node;
				break;
			}
		}
	}
	if (node)
		node->rb_color = RB_BLACK;
}



void rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	int color;

    // case 1 node->rb_left=NULL
	if (!node->rb_left)
		child = node->rb_right;

    // case 2 node->rb_right=NULL
    else if (!node->rb_right)
		child = node->rb_left;
	// case 3 node have left nodes and right nodes
    else
	{
		struct rb_node *old = node, *left;

		node = node->rb_right;
		while ((left = node->rb_left))
			node = left;
		child = node->rb_right;
		parent = node->rb_parent;
		color = node->rb_color;



        {
            // RB-TRANSPLANT ( T , node , node->rb_right )
            // RB-TRANSPLANT ( T , node , child )
            if (child)
                child->rb_parent = parent;
            if (parent) {
                if (parent->rb_left == node)
                    parent->rb_left = child;
                else
                    parent->rb_right = child;
            } else
                root->rb_node = child;
            // end RB-TRANSPLANT ( T , node , child )
        }

        if (node->rb_parent == old)
			parent = node;


        {
            // RB-TRANSPLANT ( T , old , node )
            node->rb_parent = old->rb_parent;
            node->rb_color = old->rb_color;
            node->rb_right = old->rb_right;
            node->rb_left = old->rb_left;
            if (old->rb_parent) {
                if (old->rb_parent->rb_left == old)
                    old->rb_parent->rb_left = node;
                else
                    old->rb_parent->rb_right = node;
            } else
                root->rb_node = node;
            // end RB-TRANSPLANT ( T , old , node )

        }

		old->rb_left->rb_parent = node;
		if (old->rb_right)
			old->rb_right->rb_parent = node;
		goto color;
	}


    // RB-TRANSPLANT ()
    // RB-TRANSPLANT ( T , node , child )
	parent = node->rb_parent;
	color = node->rb_color;

	if (child)
		child->rb_parent = parent;
	if (parent)
	{
		if (parent->rb_left == node)
			parent->rb_left = child;
		else
			parent->rb_right = child;
	}
	else
		root->rb_node = child;
    // end  RB-TRANSPLANT ()


color:
	if (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

/*
 * This function returns the first node (in sort order) of the tree.
 */
struct rb_node *rb_first(struct rb_root *root)
{
	struct rb_node *n;

	n = root->rb_node;
	if (!n)
		return (struct rb_node *)0;
	while (n->rb_left)
		n = n->rb_left;
	return n;
}

struct rb_node *rb_last(struct rb_root *root)
{
	struct rb_node *n;

	n = root->rb_node;
	if (!n)
		return (struct rb_node *)0;
	while (n->rb_right)
		n = n->rb_right;
	return n;
}


// such as BST get next node
struct rb_node *rb_next(struct rb_node *node)
{
	/* If we have a right-hand child, go down and then left as far
	   as we can. */
	if (node->rb_right) {
		node = node->rb_right; 
		while (node->rb_left)
			node = node->rb_left;
		return node;
	}

	/* No right-hand children.  Everything down and left is
	   smaller than us, so any 'next' node must be in the general
	   direction of our parent. Go up the tree; any time the
	   ancestor is a right-hand child of its parent, keep going
	   up. First time it's a left-hand child of its parent, said
	   parent is our 'next' node. */
	while (node->rb_parent && node == node->rb_parent->rb_right)
		node = node->rb_parent;

	return node->rb_parent;
}

struct rb_node *rb_prev(struct rb_node *node)
{
	/* If we have a left-hand child, go down and then right as far
	   as we can. */
	if (node->rb_left) {
		node = node->rb_left; 
		while (node->rb_right)
			node = node->rb_right;
		return node;
	}

	/* No left-hand children. Go up till we find an ancestor which
	   is a right-hand child of its parent */
	while (node->rb_parent && node == node->rb_parent->rb_left)
		node = node->rb_parent;

	return node->rb_parent;
}

void rb_replace_node(struct rb_node *victim, struct rb_node *newnode,
		     struct rb_root *root)
{
	struct rb_node *parent = victim->rb_parent;

	/* Set the surrounding nodes to point to the replacement */
	if (parent) {
		if (victim == parent->rb_left)
			parent->rb_left = newnode;
		else
			parent->rb_right = newnode;
	} else {
		root->rb_node = newnode;
	}
	if (victim->rb_left)
		victim->rb_left->rb_parent = newnode;
	if (victim->rb_right)
		victim->rb_right->rb_parent = newnode;

	/* Copy the pointers/colour from the victim to the replacement */
	*newnode = *victim;
}

