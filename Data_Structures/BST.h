#ifndef _BST_
#define _BST_
#include <exception>
typedef struct tree_node{
    const char * name;
    const char * value;
    int height;
    int balance;
    tree_node * parent;
    tree_node * l_child;
    tree_node * r_child;
}tree_node;

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



/*
Search miss exception
*/
class NodeNotFoundException : public std::exception{
    public:
    const char * what(){
        return "Error: can't find node";
    }

};
#endif