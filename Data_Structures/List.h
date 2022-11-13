#ifndef _LIST_
#define _LIST_

typedef struct list_node{
    const char * name;
    const char * value;
    list_node * next;
    list_node * prev;
} list_node;

class List
{
private:
    list_node * sentinel;
    int size;
public:
    List();
    ~List();
    void insert_at_start(const char * name, const char * value);
    void insert_at_end(const char * name, const char * value);
    list_node * find(const char * name);
    list_node * get_head();
    void delete_node(const char * name);
    void insert_after_node(const char * prev_node_name, const char * inserted_name, const char * value);
    void concat(List suffix);
    int get_size();

    
};

#endif