#ifndef __LLIST__
#define __LLIST__

#include <stdlib.h>

typedef struct _llist_node {
    struct _llist_node *prev;
    struct _llist_node *next;
    int payloadType;
    char payload[1];
} llist_node;

typedef struct {
	int length;
	llist_node* head;
	llist_node* tail;
} llist;

llist* llist_create();
void* llist_add_new(llist* list, int size, int payloadType);
void llist_push_node(llist* list, llist_node* node);
void llist_remove_node(llist* list, llist_node* node);


#endif