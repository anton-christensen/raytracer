
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

void llist_push_node(llist* list, llist_node* node) {
	node->prev = list->tail;
	node->next = NULL;
	
	if(list->tail != NULL)
		list->tail->next = node;
	list->tail = node;
	if(list->head == NULL)
		list->head = node;
	list->length++;
}

void llist_remove_node(llist* list, llist_node* node) {
	if(list->head == node)
		list->head = node->next;
	if(list->tail == node)
		list->tail = node->prev;

	if(node->prev != NULL)
		node->prev->next = node->next;
	if(node->next != NULL)
		node->next->prev = node->prev;
	node->prev = NULL;
	node->next = NULL;

	list->length--;
}