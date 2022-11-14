#include "BST.h"
#include <iostream>
#include <algorithm>
#include <cstring>


/*
==================================
Tree construstor.
Sets root to null and size to 0.
==================================
*/
BST::BST(){
    size = 0;
    root = nullptr;
}

/*
==================================
insert node:
Insert the node to the tree using binary search logic over the name.

name - the name of the node.
value - the value of the node.
==================================
*/
void BST::insert_node(const char * name, const char * value){
    int match;
    //Tree is empty
    if (root == nullptr){
        tree_node * new_node = new tree_node{name, value, 0, 0, nullptr, nullptr, nullptr};
        root = new_node;
    }
    else{
        tree_node * current_parent = root;
        tree_node * current_position = current_parent;
        bool left_child = false;
        while (current_position != nullptr){
            current_parent = current_position;
            match = strcmp(name, current_parent->name);
            if (match < 0){
                current_position = current_position->l_child;
                left_child = true;
            }
            else if (match > 0){
                current_position = current_position->r_child;
                left_child = false;
            }
        }
        current_position = new tree_node{name, value, 0, 0, current_parent, nullptr, nullptr};
        if (left_child){
            current_parent->l_child = current_position;
        }
        else{
            current_parent->r_child = current_position;
        }
    update_heights_and_balances(current_parent);
    }
    size++;
}
/*
==================================
find: 
search for a tree node with a given name.
This assumes all name are unique.

name - the name of the node.

return value:
if there is a node, return it.
if no such node exists, throw an exeption.
==================================
*/
tree_node * BST::find(const char * name){
        tree_node * current_search = root;
        int match;
        while (current_search != nullptr){
            match = strcmp(name, current_search->name);
            if (!match){
                return current_search;
            }
            else if (match > 0){
                current_search = current_search->r_child;
            }
            else{
                current_search = current_search->l_child;
            }
        }
        throw NodeNotFoundException();
}
/*
==================================
delete node:
Deletes a node with a given name.
This assumes all name are unique.

name - the name of the node to be deleted

If the previous node is not in the list, the function will output a notification.
==================================
*/
void BST::delete_node(const char * name){
    try{
        tree_node * delete_node = find(name);

        if (delete_node -> l_child == nullptr){
            if (delete_node -> r_child == nullptr){
                // If the node is a leaf, removal is easy.
                // First, we check if that is the root
                if (delete_node == root){
                    root = nullptr;
                    delete delete_node;
                    size--;
                    return;
                }
                if (delete_node == delete_node->parent->l_child){
                    delete_node->parent->l_child = nullptr;
                }
                else{
                    delete_node->parent->r_child = nullptr;
                }
                update_heights_and_balances(delete_node->parent);
                delete delete_node;
            }
            else{
                // The node has only one child, which will become its parnet new child
                // If this is the root, the child will now be the root
                if (delete_node == root){
                    root = delete_node->r_child;
                    delete_node->r_child->parent = nullptr;
                    size--;
                    return;
                }
                tree_node * descendant = delete_node->r_child;
                if (delete_node == delete_node->parent->l_child){
                    delete_node->parent->l_child = descendant;
                }
                else{
                    delete_node->parent->r_child = descendant;
                }
                descendant->parent = delete_node->parent;
                update_heights_and_balances(delete_node->parent);
                delete delete_node;
            }
        }
        else{
            if (delete_node -> r_child == nullptr){
                // The node has only one child, which will become its parnet new child
                // If this is the root, the child will now be the root
                if (delete_node == root){
                    root = delete_node->l_child;
                    delete_node->l_child->parent = nullptr;
                    size--;
                    return;
                }
                tree_node * descendant = delete_node->l_child;
                if (delete_node == delete_node->parent->l_child){
                    delete_node->parent->l_child = descendant;
                }
                else{
                    delete_node->parent->r_child = descendant;
                }
                descendant->parent = delete_node->parent;
                update_heights_and_balances(delete_node->parent);
                delete delete_node;
            }
            else{
                // The most complicated case, in which the node has two children.
                // Find this node's successor
                tree_node * successor = delete_node -> r_child;
                tree_node * starting_point;
                if (successor->l_child == nullptr){
                    // the successor may end up with a different height and/or balance.
                    // In that case, we need to start to update from them.
                    starting_point = successor;
                }
                else{
                    while(successor->l_child != nullptr){
                        successor = successor->l_child;
                    }
                    // If the succesor is not the deleted node's child,
                    // then its parent may end up with a different height and/or balance.
                    // In that case, we need to start to update from them.
                    starting_point = successor->parent;
                    starting_point->l_child = successor->r_child;
                    if (successor->r_child != nullptr){
                        successor->r_child->parent = starting_point;
                    }
                    successor->r_child = delete_node->r_child;
                    delete_node->r_child->parent = successor;
                    
                }
                successor->l_child = delete_node->l_child;
                delete_node->l_child->parent = successor;
                if (delete_node == root){
                    root = successor;
                }
                else if (delete_node == delete_node->parent->l_child){
                    delete_node->parent->l_child = successor;
                }
                else{
                    delete_node->parent->r_child = successor;
                }
                successor->parent = delete_node->parent;
                update_heights_and_balances(starting_point);
                delete delete_node;
            }
        }
    size--;

    }
    catch(NodeNotFoundException& e){
         std::cout << e.what() << std::endl;
    }
}
/*
=================================
get size:
returns the size of the tree.
=================================
*/
int BST::get_size(){
    return size;
}
/*
==================================
update height and balance:
This is an internal function that is used
after inserting and deleting nodes
to re-determine the height and balance of each node
in its branch.
==================================
*/
void BST::update_heights_and_balances(tree_node * t){
    // Go up to the root, only the root's parent is null.
    if (t == nullptr){
        return;
    }
    if (t->l_child == nullptr){
        if (t->r_child == nullptr){
            //This node is now a leaf
            t->height = 0;
            t->balance = 0;    
        }
        else {
            //This node has only a right branch, so its balance is equal to
            //the negtive of its height
            t->height = t->r_child->height+1;
            t->balance = -1*t->height;
        }
    }
    else {
        if (t->r_child == nullptr){
            //This node has only a left branch, so its balance is equal to its height. 
            t->height = t->l_child->height+1;
            t->balance = t->height;
        }
        else{
            t->height = std::max(t->l_child->height, t->r_child->height)+1;
            t->balance = t->l_child->height - t->r_child->height;
        }
    }
    // The tree is balanced iff abs(balance) for each node is less than 1.
    // otherwise, it needs to be rebalanced the right way.
    if (t->balance > 1){
        reblance_tree_right(t);
    }
    else if (t->balance < -1){
        reblance_tree_left(t);
    }
    update_heights_and_balances(t->parent);
}
/*
==================================
rotate right:
This is a simple internal function to rotate a sub-tree.
Given a node t, define its left child a pivot.
Make the pivot the new root of the sub-tree,
make the pivot's right branch into t's new left branch,
and make t the pivot's new right child.
==================================
*/
void BST::rotate_right(tree_node * t){
    tree_node * pivot = t->l_child;
    pivot->parent = t->parent;
    if (t->parent != nullptr){
        if (t->parent->l_child == t){
            t->parent->l_child = pivot;
        }
        else{
            t->parent->r_child = pivot;
        }
    }
    else{
        // This means that t was the tree's root
        root = pivot;
    }
    t->l_child = pivot->r_child;
    if (pivot->r_child!=nullptr){
        pivot->r_child->parent = t;
    }
    pivot->r_child = t;
    t->parent = pivot;
    // Update the height and balance of t and the pivot.
    // The rest of the sub-tree is not affected.
    if (t->l_child == nullptr){
        if (t->r_child == nullptr){
            t->height = 0;
            t->balance = 0;
        }
        else{
            t->height = t->r_child->height+1;
            t->balance = -1*t->height;
        }
    }
    else{
        if(t->r_child == nullptr){
            t->height = t->l_child->height+1;
            t->balance = t->height;
        }
        else{
            t->height = std::max(t->l_child->height, t->r_child->height)+1;
            t->balance = t->l_child->height - t->r_child->height;
        }
    }
    if (pivot->l_child == nullptr){
        pivot->height = t->height+1;
        pivot->balance = -1*(pivot->height);
    }
    else{
        pivot->height = std::max(pivot->l_child->height, t->height)+1;
        pivot->balance = pivot->l_child->height - t->height;
    }

}
/*
==================================
rotate left:
This is a simple internal function to rotate a sub-tree.
Given a node t, define its right child a pivot.
Make the pivot the new root of the sub-tree,
make the pivot's left branch into t's new right branch,
and make t the pivot's new left child.
==================================
*/
void BST::rotate_left(tree_node * t){
    tree_node * pivot = t->r_child;
    pivot->parent = t->parent;
    if (t->parent != nullptr){
        if (t->parent->l_child == t){
            t->parent->l_child = pivot;
        }
        else{
            t->parent->r_child = pivot;
        }
    }
    else{
        // This means that t was the tree's root
        root = pivot;
    }
    t->r_child = pivot->l_child;
    if (pivot->l_child != nullptr){
        pivot->l_child->parent = t;
    }
    pivot->l_child = t;
    t->parent = pivot;
    // Update the height and balance of t and the pivot.
    // The rest of the sub-tree is not affected.
    if (t->r_child == nullptr){
       if (t->r_child == nullptr){
            t->height = 0;
            t->balance = 0;
        }
        else{
        t->height = t->l_child->height+1;
        t->balance = t->height;
        }
    }
    else{
        if (t->l_child == nullptr){
            t->height = t->r_child->height+1;
            t->balance = -1*t->height;
        }
        else{
            t->height = std::max(t->l_child->height, t->r_child->height)+1;
            t->balance = t->l_child->height - t->r_child->height;
        }
    }
    if (pivot->r_child == nullptr){
        pivot->height = t->height+1;
        pivot->balance = pivot->height;
    }
    else{
        pivot->height = std::max(pivot->r_child->height, t->height)+1;
        pivot->balance = t->height - pivot->r_child->height;
    }
}
/*
==================================
rebalance tree right:
This is an internal function that is used
in the case the depth of a node's left branch is greater than
the depth of its right branch by a differnce of at least 2.
In practise, as the roation occurs immidiatley after the node is off-balance,
the function will always be called when the difference is exactly 2.
The function works by taking the t's left child.
If that node's balance is not negative, perform a right rotation on t,
otherwise, perform a left rotation on the child, followed by a right rotation with t.
This way, the binary search logic is preserved, and the balance is set to 0.
==================================
*/
void BST::reblance_tree_right(tree_node * t){
    if (t->l_child->balance>=0){
        rotate_right(t);
    }
    else{
        rotate_left(t->l_child);
        rotate_right(t);
    }
    
}

/*
==================================
rebalance tree left:
This is an internal function that is used
in the case the depth of a node's right branch is greater than
the depth of its left branch by a differnce of at least 2.
In practise, as the roation occurs immidiatley after the node is off-balance,
the function will always be called when the difference is exactly 2.
The function works by taking the t's right child.
If that node's balance is not positive, perform a left rotation on t,
otherwise, perform a right rotation on the child, followed by a left rotation with t.
This way, the binary search logic is preserved, and the balance is set to 0.
==================================
*/
void BST::reblance_tree_left(tree_node * t){
    if (t->r_child->balance<=0){
        rotate_left(t);
    }
    else{
        rotate_right(t->r_child);
        rotate_left(t);
    }
    
}

/*
==================================
recursive delete:
This is an internal destrcutor function which will delete all tree nodes in order.
==================================
*/
void BST::recursive_delete(tree_node * t){
    if (t->l_child != nullptr){
        recursive_delete(t->l_child);
    }
    if (t->r_child != nullptr){
        recursive_delete(t->r_child);
    }
    delete t;
}
/*
==================================
traverse tree:
A testing function that prints all the nodes in order
==================================
*/

void BST::traverse_tree(){
    std::cout << "Printing tree data:" << std::endl;
    if (root == nullptr){
        std::cout << "Nothing to print, the tree is empty." << std::endl;
        return;
    }
    recursive_traversal(root->l_child);
    std::cout << "Name: " << root->name << ", Value: " << root->value << ", Parent: None (is root), Height: " << root->height << ", Balance: " << root->balance << std::endl;
    recursive_traversal(root->r_child);
    std::cout << "The size of the tree is " << get_size() << "." << std::endl;
    
}
/*
==================================
recursive traversal:
An internal helper function of traverse_tree
==================================
*/
void BST::recursive_traversal(tree_node * t){
    if (t == nullptr){
        return;
    }
    recursive_traversal(t->l_child);
    std::cout << "Name: " << t->name << ", Value: " << t->value << ", Parent: " << t->parent->name << ", Height: " << t->height << ", Balance: " << t->balance << std::endl;
    recursive_traversal(t->r_child);
}
/*
==================================
Tree destructor:
uses recursive delete to delete all nodes.
==================================
*/
BST::~BST(){
    recursive_delete(root);
}
