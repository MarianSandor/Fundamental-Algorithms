#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

Profiler profiler("RB_Tree");

enum Color { Red = 1, Black = 0 };

typedef struct nodeRB {
	int key;
	Color color;
	struct nodeRB* parent;
	struct nodeRB* left;
	struct nodeRB* right;
} NodeRB;

typedef struct RBtree {
	struct nodeRB* root;
	struct nodeRB* null;
} RBTree;

NodeRB* createNode(RBTree* T, int key) {
	NodeRB* node = (NodeRB*)malloc(sizeof(NodeRB));

	if (node)
	{
		node->key = key;
		node->color = Red;
		node->parent = node->left = node->right = T->null;

		return node;
	}

	return NULL;
}

RBTree* createTree() {
	RBTree* T = (RBTree*)malloc(sizeof(RBTree));

	if (T)
	{
		T->null = (NodeRB*)malloc(sizeof(NodeRB));

		if (T->null)
		{
			T->null->key = 0;
			T->null->color = Black;
			T->null->parent = T->null->left = T->null->right = T->null;

			T->root = T->null;

			return T;
		}

		return NULL;
	}

	return NULL;
}

void padding(char ch, int n) {
	int i;

	for (i = 0; i < n; i++)
		putchar(ch);
}

void printTree(RBTree* T, NodeRB* root, int level) {
	if (root == T->null) {
		padding('\t', level);
		puts("   ~");
	}
	else {
		printTree(T, root->right, level + 1);
		padding('\t', level);
		if (root->color == Red)
		{
			printf("(%d | R)\n", root->key);
		}
		else
		{
			printf("(%d | B)\n", root->key);
		}

		printTree(T, root->left, level + 1);
	}
}

void leftRotation(RBTree* T, NodeRB* node) {
	NodeRB* child = node->right;
	node->right = child->left;

	if (child->left != T->null)
	{
		child->left->parent = node;
	}

	child->parent = node->parent;

	if (node->parent == T->null)
	{
		T->root = child;
	}
	else if (node == node->parent->left)
	{
		node->parent->left = child;
	}
	else
	{
		node->parent->right = child;
	}

	child->left = node;
	node->parent = child;
}

void rightRotation(RBTree* T, NodeRB* node) {
	NodeRB* child = node->left;
	node->left = child->right;

	if (child->right != T->null)
	{
		child->right->parent = node;
	}

	child->parent = node->parent;

	if (node->parent == T->null)
	{
		T->root = child;
	}
	else if (node->parent->left == node)
	{
		node->parent->left = child;
	}
	else
	{
		node->parent->right = child;
	}

	child->right = node;
	node->parent = child;
}

void leftChildInsert(RBTree* T, NodeRB** node) {

	NodeRB* uncle = (*node)->parent->parent->right;

	if (uncle->color == Red)
	{
		uncle->color = Black;
		(*node)->parent->color = Black;
		uncle->parent->color = Red;
		(*node) = uncle->parent;
	}
	else
	{
		if ((*node) == (*node)->parent->right)
		{
			(*node) = (*node)->parent;
			leftRotation(T, (*node));
		}

		(*node)->parent->color = Black;
		(*node)->parent->parent->color = Red;
		rightRotation(T, (*node)->parent->parent);
	}
}

void rightChildInsert(RBTree* T, NodeRB** node) {
	NodeRB* uncle = (*node)->parent->parent->left;

	if (uncle->color == Red)
	{
		uncle->color = Black;
		(*node)->parent->color = Black;
		uncle->parent->color = Red;
		(*node) = uncle->parent;
	}
	else
	{
		if ((*node) == (*node)->parent->left)
		{
			(*node) = (*node)->parent;
			rightRotation(T, (*node));
		}

		(*node)->parent->color = Black;
		(*node)->parent->parent->color = Red;
		leftRotation(T, (*node)->parent->parent);
	}
}

void RbInsertFixUp(RBTree* T, NodeRB* node) {

	while (node->parent->color == Red)
	{
		if (node->parent == node->parent->parent->left)
		{
			leftChildInsert(T, &node);
		}
		else
		{
			rightChildInsert(T, &node);
		}
	}

	T->root->color = Black;
}

void RbInsert(RBTree* T, int key) {
	NodeRB* node = createNode(T, key);

	NodeRB* poz = T->root;
	NodeRB* parentPoz = T->null;

	while (poz != T->null)
	{
		parentPoz = poz;

		if (poz->key > node->key)
		{
			poz = poz->left;
		}
		else
		{
			poz = poz->right;
		}
	}

	node->parent = parentPoz;

	if (parentPoz == T->null)
	{
		T->root = node;
	}
	else if (node->key < parentPoz->key)
	{
		parentPoz->left = node;
	}
	else
	{
		parentPoz->right = node;
	}

	RbInsertFixUp(T, node);
}

NodeRB* searchKey(RBTree* T, int key) {
	NodeRB* node = T->root;

	while (node != T->null)
	{
		if (node->key == key)
		{
			return node;
		}
		else if (node->key > key)
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}

	return NULL;
}

NodeRB* findMinimum(RBTree* T, NodeRB* root) {
	while (root->left != T->null)
	{
		root = root->left;
	}

	return root;
}

void transplant(RBTree* T, nodeRB* nodeX, nodeRB* nodeY) {

	nodeY->parent = nodeX->parent;

	if (nodeX->parent == T->null)
	{
		T->root = nodeY;
	}
	else if (nodeX == nodeX->parent->left)
	{
		nodeX->parent->left = nodeY;
	}
	else
	{
		nodeX->parent->right = nodeY;
	}
}

void leftChildDelete(RBTree* T, NodeRB** node) {
	NodeRB* brother = (*node)->parent->right;

	if (brother->color == Red)
	{
		brother->color = Black;
		(*node)->parent->color = Red;
		leftRotation(T, (*node)->parent);
		brother = (*node)->parent->right;
	}

	if (brother->left->color == Black && brother->right->color == Black)
	{
		brother->color = Red;
		(*node) = (*node)->parent;

		return;
	}
	else if (brother->right->color == Black)
	{
		brother->color = Red;
		brother->left->color = Black;
		rightRotation(T, brother);
		brother = (*node)->parent->right;
	}

	brother->color = (*node)->parent->color;
	brother->right->color = Black;
	(*node)->parent->color = Black;
	leftRotation(T, (*node)->parent);

	(*node) = T->root;
}

void rightChildDelete(RBTree* T, NodeRB** node) {
	NodeRB* brother = (*node)->parent->left;

	if (brother->color == Red)
	{
		brother->color = Black;
		(*node)->parent->color = Red;
		rightRotation(T, (*node)->parent);
		brother = (*node)->parent->left;
	}

	if (brother->left->color == Black && brother->right->color == Black)
	{
		brother->color = Red;
		(*node) = (*node)->parent;

		return;
	}
	else if (brother->left->color == Black)
	{
		brother->right->color = Black;
		brother->color = Red;
		leftRotation(T, brother);
		brother = (*node)->parent->left;
	}

	brother->color = (*node)->parent->color;
	brother->left->color = Black;
	(*node)->parent->color = Black;
	rightRotation(T, (*node)->parent);

	(*node) = T->root;
}

void RBDeleteFixUp(RBTree* T, NodeRB* node) {
	while (node != T->root && node->color == Black)
	{
		if (node == node->parent->left)
		{
			leftChildDelete(T, &node);
		}
		else
		{
			rightChildDelete(T, &node);
		}
	}

	node->color = Black;
}

void RBDelete(RBTree* T, NodeRB* nodeZ) {
	NodeRB* nodeY = nodeZ;
	NodeRB* nodeX;
	Color yInitialColor = nodeY->color;

	if (nodeZ->right == T->null)
	{
		nodeX = nodeZ->left;
		transplant(T, nodeZ, nodeZ->left);
	}
	else if (nodeZ->left == T->null)
	{
		nodeX = nodeZ->right;
		transplant(T, nodeZ, nodeZ->right);
	}
	else
	{
		nodeY = findMinimum(T, nodeZ->right);
		yInitialColor = nodeY->color;
		nodeX = nodeY->right;

		if (nodeY->parent == nodeZ)
		{
			nodeX->parent = nodeY;
		}
		else
		{
			transplant(T, nodeY, nodeY->right);
			nodeY->right = nodeZ->right;
			nodeZ->right->parent = nodeY;
		}

		transplant(T, nodeZ, nodeY);
		nodeY->left = nodeZ->left;
		nodeZ->left->parent = nodeY;
		nodeY->color = nodeZ->color;
	}

	if (yInitialColor == Black)
	{
		RBDeleteFixUp(T, nodeX);
	}

	free(nodeZ);
}

int* generateArray(int size) {
	int* a = (int*)malloc(size * sizeof(int));

	if (a)
	{
		FillRandomArray(a, size, 1, 100, true, 0);

		return a;
	}

	return NULL;
}

void main() {

	int size = 10;
	int* a = generateArray(size);

	for (int i = 0; i < size; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n-------------------------------------------------------------------------------------------------------------------\n");

	RBTree* T = createTree();

	for (int i = 0; i < size; i++)
	{
		RbInsert(T, a[i]);
	}

	printTree(T, T->root, 1);
	printf("\n-------------------------------------------------------------------------------------------------------------------\n");

	int* index = (int*)malloc(size * sizeof(int));
	FillRandomArray(index, size, 0, size - 1, true, 0);

	for (int i = 0; i < size; i++)
	{
		NodeRB* nodeToDelete = searchKey(T, a[index[i]]);
		printf("\nKey to be deleted %d\n", nodeToDelete->key);

		RBDelete(T, nodeToDelete);

		printTree(T, T->root, 1);
		printf("\n-------------------------------------------------------------------------------------------------------------------\n");
	}
}