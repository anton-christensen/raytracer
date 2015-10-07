#ifndef __LLIST__
#define __LLIST__

#include <stdlib.h>

typedef struct _node {
    struct _node *prev;
    struct _node *next;
    int payloadType;
    char payload[1];  // or use different type for alignment.
} node;

typedef struct {
	int length;
	node* head;
	node* tail;
	node* current;
} llist;

llist* llist_create();
void* llist_add_new(llist* list, int size, int payloadType);
void llist_push(llist* list, int size, int payloadType, void* payload);



#endif