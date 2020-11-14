#include<stdio.h>	
#include<stdlib.h>
#include "List.h"

NodeL* createNode(int key) {
	NodeL* p = (NodeL*)malloc(sizeof(NodeL));

	if (p)
	{
		p->key = key;
		p->next = NULL;
		return p;
	}

	return NULL;
}

ListH* createList() {
	ListH* L = (ListH*)malloc(sizeof(ListH));

	if (L)
	{
		L->size = 0;
		L->first = L->last = NULL;
		return L;
	}

	return NULL;
}

ListH** createArrayList(int size) {
	ListH **L = (ListH**)malloc(size * sizeof(ListH *));

	if (L)
	{
		for (int i = 0; i < size; i++) {
			 *(L + i) = createList();
		}

		return L;
	}

	return NULL;
}

void push(ListH* list, int key) {
	NodeL* node = createNode(key);

	if (list->size == 0)
	{
		list->first = list->last = node;
	}
	else
	{
		list->last->next = node;
		list->last = node;
	}

	list->size++;
}

int pop(ListH* list) {
	if (list->size > 0)
	{
		NodeL* node = list->first;
		int key = node->key;

		list->first = node->next;
		list->size--;

		free(node);

		return key;
	}

	return 0;
}

void printList(ListH* l) {
	NodeL* p = l->first;

	while (p)
	{
		printf("%d ", p->key);
		p = p->next;
	}

	printf("\n");
}

bool isEmpty(ListH* L) {
	if (L->size > 0)
	{
		return false;
	}

	return true;
}

void freeList(ListH* l) {
	if (!isEmpty(l))
	{
		NodeL* node = l->first;

		while (node)
		{
			NodeL* del = node;
			node = node->next;
			free(del);
		}
	}

	free(l);
}

void freeArrayList(ListH** l, int size) {

	for (int i = 0; i < size; i++)
	{
		freeList(l[i]);
	}

	free(l);
}