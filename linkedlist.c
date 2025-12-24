#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist
{
    struct linkedlist_node *first;
    // TODO: define linked list metadata
    int size; // Keeps track of the number of nodes in the list
};

struct linkedlist_node
{
    // TODO: define the linked list node
    int key;
    int value;
    struct linkedlist_node *next;
};
typedef struct linkedlist_node linkedlist_node_t;

linkedlist_t *ll_init()
{
    // TODO: create a new linked list

    // We have done this TODO for you as an example of how to use malloc().
    // You might want to read more about malloc() from Linux Manual page.
    // Usually free() should be used together with malloc(). For example, 
    // the linkedlist you are currently implementing usually have free() in the
    // ll_delete() function. Since we are not asking you to implement
    // the ll_delete() function, you will not be using free() in this case.

    // First, you use C's sizeof function to determine
    // the size of the linkedlist_t data type in bytes.
    // Then, you use malloc to set aside that amount of space in memory.
    // malloc returns a void pointer to the memory you just allocated, but
    // we can assign this pointer to the specific type we know we created
    linkedlist_t *list = malloc(sizeof(linkedlist_t));

    // TODO: set metadata for your new list and return the new list
    list->first = NULL;
    list->size = 0;
    return list;
}

void ll_add(linkedlist_t *list, int key, int value)
{
    // TODO: create a new node and add to the front of the linked list if a
    // node with the key does not already exist.
    // Otherwise, replace the existing value with the new value.

    // 1. Check if key already exists
    linkedlist_node_t *current = list->first;
    while (current != NULL) {
        if (current->key == key) {
            // Key found, update value and return
            current->value = value;
            return;
        }
        current = current->next;
    }

    // 2. Key does not exist, create new node
    linkedlist_node_t *new_node = malloc(sizeof(linkedlist_node_t));
    new_node->key = key;
    new_node->value = value;

    // 3. Add to front
    new_node->next = list->first;
    list->first = new_node;

    // 4. Update metadata
    list->size++;
}

int ll_get(linkedlist_t *list, int key)
{
    // TODO: go through each node in the linked list and return the value of
    // the node with a matching key.
    // If it does not exist, return 0.

    linkedlist_node_t *current = list->first;
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return 0;
}

int ll_size(linkedlist_t *list)
{
    // TODO: return the number of nodes in this linked list
    return list->size;
}
