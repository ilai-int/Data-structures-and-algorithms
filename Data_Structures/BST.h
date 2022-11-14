#ifndef _BST_
#define _BST_
#include "Nodes.h"

class BST
{
private:
    int size;
    tree_node * root;
    void update_heights_and_balances(tree_node * t);
    void reblance_tree_right(tree_node * t);
    void reblance_tree_left(tree_node * t);
    void rotate_left(tree_node * t);
    void rotate_right(tree_node * t);
    void recursive_delete(tree_node * t);
    void recursive_traversal(tree_node * t);
public:
    BST();
    int get_size();
    void insert_node(const char * name, const char * value);
    void delete_node(const char * name);
    void traverse_tree();
    tree_node * find(const char * name);
    ~BST();
};


#endif