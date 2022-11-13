#ifndef _LIST_
#define _LIST_

typedef struct node{
    char * name;
    char * value;
    node * next;
    node * prev;
} node;

class List
{
private:
    node * sentinel;
    int size;
public:
    List();
    ~List();
    void insert_at_start(char * name, char * value);
    void insert_at_end(char * namr, char * value);
    node * find(char * name);
    node * get_head();
    void delete_node(char * name);
    void insert_after_node(char * prev_node_name, char * inserted_name, char * value);
    void concat(List suffix);
    int get_size();

    
};

#endif