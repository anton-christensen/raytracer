
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
	if(list->tail == NULL)
		list->tail = node;
	else
		list->tail->next = node;
	if(list->head == NULL)
		list->head = node;
	list->length++;
}

void llist_remove_node(llist* list, llist_node* node) {
	llist_node* itt;
	itt = list->head;
	while(itt != NULL) {
		if(itt == node) {
			if(itt->prev != NULL) {
				itt->prev->next = itt->next;
			}
			if(itt->next != NULL) {
				itt->next->prev = itt->prev;
			}
			if(list->head == itt)
				list->head = itt->next;
			if(list->tail == itt)
				list->tail = itt->prev;
			list->length--;
		}
		itt = itt->next;
	}
}