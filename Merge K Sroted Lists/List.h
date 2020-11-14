#ifndef LIST_H
#define LIST_H

typedef struct node {
	int key;
	struct node* next;
}NodeL;

typedef struct list {
	int size;
	struct node* first;
	struct node* last;
}ListH;

extern NodeL* createNode(int key);
extern ListH* createList();
extern ListH** createArrayList(int size);
extern void push(ListH* list, int key);
extern int pop(ListH* list);
extern void printList(ListH* l);
extern bool isEmpty(ListH* L);
extern void freeArrayList(ListH** l, int size);
extern void freeList(ListH* l);

#endif // !LIST_H

