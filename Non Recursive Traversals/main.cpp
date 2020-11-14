#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

Profiler profiler("OS Tree");

int MIN, MAX;

typedef struct nodeT {
	int key;
	struct nodeT* left;
	struct nodeT* right;
	struct nodeT* parent;
} NodeT;

typedef struct tree {
	int size;
	struct nodeT* root;
} Tree;

int* generateArray(int n, bool unique, int sort) {
	int* a = (int*)malloc(n * sizeof(int));

	if (a)
	{
		FillRandomArray(a, n, MIN, MAX, unique, sort);

		return a;
	}

	return NULL;
}

Tree* createTree() {
	Tree* tree = (Tree*)malloc(sizeof(Tree));

	if (tree)
	{
		tree->size = 0;
		tree->root = NULL;

		return tree;
	}

	return NULL;
}

NodeT* createNode(int key) {
	NodeT* node = (NodeT*)malloc(sizeof(NodeT));

	if (node)
	{
		node->key = key;
		node->left = node->right = node->parent = NULL;

		return node;
	}

	return NULL;
}

void freeNodes(NodeT* root) {
	if (root != NULL)
	{
		freeNodes(root->left);
		freeNodes(root->right);

		free(root);
	}
}

void freeTree(Tree* T) {
	freeNodes(T->root);

	free(T);
}

bool insertTree(Tree* T, int key) {
	NodeT* node = createNode(key);
	T->size++;

	if (T->root == NULL) {
		T->root = node;
		return true;
	}

	NodeT* root = T->root;

	while (root) {
		if (node->key > root->key) {
			if (root->right == NULL) {
				root->right = node;
				node->parent = root;
				return true;
			}

			root = root->right;
		}
		else {
			if (root->left == NULL) {
				root->left = node;
				node->parent = root;
				return true;
			}

			root = root->left;
		}
	}

	return false;
}

Tree* buildTree(int n) {
	int* a = generateArray(n, 1, 0);
	Tree* T = createTree();

	for (int i = 0; i < n; i++)
	{
		insertTree(T, a[i]);
	}

	return T;
}

void padding(char ch, int n) {
	int i;

	for (i = 0; i < n; i++)
		putchar(ch);
}

void printTree(NodeT* root, int level) {
	int i;

	if (root == NULL) {
		padding('\t', level);
		puts("~");
	}
	else {
		printTree(root->right, level + 1);
		padding('\t', level);
		printf("%d\n", root->key);
		printTree(root->left, level + 1);
	}
}

void traversal(Tree* T) {
	NodeT* curr = T->root;
	NodeT* prev = NULL;

	while (curr)
	{
		if (curr->parent == prev)
		{
			printf("%d ", curr->key); // PREORDER
			if (curr->left)
			{
				prev = curr;
				curr = curr->left;
			}
			else if (curr->right)
			{
				//printf("%d ", curr->key); //INODER
				prev = curr;
				curr = curr->right;
			}
			else
			{
				//printf("%d ", curr->key); //INODER POSTORDER
				prev = curr;
				curr = curr->parent;
			}
		}
		else if (curr->left == prev)
		{
			//printf("%d ", curr->key); //INODER
			if (curr->right)
			{
				prev = curr;
				curr = curr->right;
			}
			else
			{
				//printf("%d ", curr->key); // POSTORDER
				prev = curr;
				curr = curr->parent;
			}
		}
		else if (curr->right == prev)
		{
			//printf("%d ", curr->key); // POSTORDER
			prev = curr;
			curr = curr->parent;
		}
	}
}

int findHeight(Tree* T) {
	NodeT* curr = T->root;
	NodeT* prev = NULL;
	int currHeight = 0;
	int maxHeight = 0;

	while (curr)
	{
		if (curr->parent == prev)
		{
			if (curr->left)
			{
				currHeight++;
				prev = curr;
				curr = curr->left;
			}
			else if (curr->right)
			{
				currHeight++;
				prev = curr;
				curr = curr->right;
			}
			else
			{
				if (currHeight > maxHeight)
				{
					maxHeight = currHeight;
				}
				currHeight--;
				prev = curr;
				curr = curr->parent;
			}
		}
		else if (curr->left == prev)
		{
			if (curr->right)
			{
				currHeight++;
				prev = curr;
				curr = curr->right;
			}
			else
			{
				currHeight--;
				prev = curr;
				curr = curr->parent;
			}
		}
		else if (curr->right == prev)
		{
			currHeight--;
			prev = curr;
			curr = curr->parent;
		}
	}

	return maxHeight;
}

void main() {
	MIN = 1;
	MAX = 99;
	Tree* T = buildTree(10);


	printTree(T->root, 0);
	traversal(T);
}

