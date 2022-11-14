#ifndef _NODES_
#define _NODES_
#include <exception>

typedef struct list_node{
    const char * name;
    const char * value;
    list_node * next;
    list_node * prev;
} list_node;

typedef struct tree_node{
    const char * name;
    const char * value;
    int height;
    int balance;
    tree_node * parent;
    tree_node * l_child;
    tree_node * r_child;
}tree_node;

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