#include "List.h"
#include <iostream>
#include <execution>

/*
Exception in case the list is empty
*/
class EmptyListException : public std::exception{
    public:
    const char * what(){
        return "Error: List is empty";
    }

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
/*
==================================
List consturctor:
Builds a empty list with only a sentinel node
Sets size to 0.
==================================
*/
List::List()
{
    sentinel = new node{0, 0 ,sentinel, sentinel};
    size = 0;
}
/*
=================================
get size:
returns the size of the list.
=================================
*/
int List::get_size(){
    return size;
}
/*
==================================
insert at start:
Add a node to the start of the list.

name - the name of the node.
value - the value of the node.

(Note: this requires that the name of each node in a list is unique)
==================================
*/
void List::insert_at_start(char * name, char * value){
    node * new_node = new node{name, value, sentinel->next, sentinel};
    sentinel->next = new_node;
    new_node->next->prev = new_node;
    size++;
}

/*
==================================
insert at end:
Add a node to the end of the list.

name - the name of the node.
value - the value of the node.

(Note: this requires that the name of each node in a list is unique)
==================================
*/
void List::insert_at_end(char * name, char * value){
    node new_node = {name, value, sentinel, sentinel->prev};
    sentinel->prev->next = &new_node;
    sentinel->prev = &new_node;
}

/*
==================================
find:
search for a node with a given name.

name - the name of the node.

Return value:
if there is a node, return it.
if no such node exists, throw an exeption.
==================================
*/
node * List::find(char * name){
    node * search_node = sentinel->next;
    // Go through the list and find a node with a matching name
    // This assumes all name are unique
    while(search_node != sentinel){
        if (!strcmp(search_node->name, name)){
            return search_node;
        }
    }
    throw NodeNotFoundException();

}

/*
==================================
insert after node:
Add a note after a specific node.
This assumes all name are unique.

prev_node_name - the name of the node after which the new node will be inserted
inserted_name - the name of the new node
value - the value of the new node

If the previous node is not in the list, the function will output a notification.
==================================
*/
void List::insert_after_node(char * prev_node_name, char * inserted_name, char * value){
    try{
        node * prev_node = find(prev_node_name);
        node new_node = {inserted_name, value, prev_node->next, prev_node};
        prev_node->next->prev = &new_node;
        prev_node->next = &new_node;
        size++;
    }
    catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
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
void List::delete_node(char * name){
    try{
        node * delete_node = find(name);
        delete_node->prev->next = delete_node->next;
        delete_node->next->prev = delete_node->prev;
        delete delete_node;
        size--;
    }
    catch(std::exception& e){
         std::cout << e.what() << std::endl;
    }
}
/*
=================================
get_head:
returns the first node in the list.

If the list is empty, throw an expetion;
=================================
*/
node * List::get_head(){
    if (size==0){
        throw EmptyListException();
    }
    return sentinel->next;
}
/*
=================================
concat:
add a list to the end of this list

suffix - the list that will be added.
=================================
*/
void List::concat(List suffix){
    try{
        node * suffix_head = suffix.get_head();
        node * suffix_sentinel = suffix_head->prev;
        node * suffix_tail = suffix_sentinel->prev;
        // attach head to the end of the list
        sentinel->prev->next = suffix_head;
        // attach tail to the sentinel
        suffix_tail->next = sentinel;
        // disconnect the suffix's sentinel
        suffix_sentinel->next = suffix_sentinel;
        suffix_sentinel->prev = suffix_sentinel;
        size += suffix.get_size();
    }
    catch(std::exception& e){
        std::cout << e.what() << std::endl;
    }
}
/*
=================================
List desturctor:
goes through each node in the list and destroys it.
=================================
*/
List::~List()
{
    node * this_node = sentinel;
    node * next_node = this_node->next;
    // While there are more nodes in the list
    while(next_node != sentinel){
        this_node = next_node;
        next_node = this_node->next;
        delete this_node;
    }
    delete sentinel;
}