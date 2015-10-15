
#include "llist.h"

llist* llist_create() {
	llist* l = (llist*)malloc(sizeof(llist));
	l->head = NULL;
	l->tail = NULL;
	l->length = 0;
	return l;
}

void* llist_add_new(llist* list, int size, int payloadType) {
	llist_node* n = (llist_node*)malloc (sizeof(llist_node) - 1 + size);
	n->prev = NULL;
	n->next = NULL;
	n->payloadType = payloadType;

	if(list->length == 0) {
		list->tail = n;
		list->head = n;
	}
	else {
		list->tail->next = n;
		n->prev = list->tail;
		list->tail = n;
	}
	list->length++;

	return n->payload;
}