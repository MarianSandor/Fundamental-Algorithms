#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

Profiler profiler("OS Tree");

int MIN, MAX;

typedef struct nodeT {
	int key;
	struct nodeT* left;
	struct nodeT* right;
} NodeT;

int* generateArray(int n, bool unique, int sort) {
	int* a = (int*)malloc(n * sizeof(int));

	if (a)
	{
		FillRandomArray(a, n, MIN, MAX, unique, sort);

		return a;
	}

	return NULL;
}

NodeT* createNode(int key) {
	NodeT* node = (NodeT*)malloc(sizeof(NodeT));

	if (node)
	{
		node->key = key;
		node->left = node->right = NULL;

		return node;
	}

	return NULL;
}

NodeT* rotateLeft(NodeT* root) {
	NodeT* newRoot = root->right;

	root->right = newRoot->left;
	newRoot->left = root;

	return newRoot;
}

NodeT* rotateRight(NodeT* root) {
	NodeT* newRoot = root->left;

	root->left = newRoot->right;
	newRoot->right = root;

	return newRoot;
}

NodeT* Transform(NodeT* root, NodeT* parrent) {
	if (root)
	{
		if (parrent) {
			if (root == parrent->left)
			{
				while (root->right) {
					root = rotateLeft(root);
				}

				root->right = parrent;
				root->left = Transform(root->left, root);
			}
			else if (root == parrent->right) {
				while (root->left) {
					root = rotateRight(root);
				}

				root->left = parrent;
				root->right = Transform(root->right, root);
			}
		}
		else {
			root->left = Transform(root->left, root);
			root->right = Transform(root->right, root);
		}
	}

	return root;
}

void TransformV2(NodeT* node, NodeT** prev) {
	if (node)
	{
		TransformV2(node->left, prev);

		if (*prev)
		{
			(*prev)->right = node;
			node->left = *prev;
		}

		*prev = node;

		TransformV2(node->right, prev);
	}
}

void freeNodes(NodeT* root) {
	if (root != NULL)
	{
		freeNodes(root->left);
		freeNodes(root->right);

		free(root);
	}
}

bool insertTree(NodeT *root, int key) {
	NodeT* node = createNode(key);

	while (root) {

		if (node->key > root->key) {
			if (root->right == NULL) {
				root->right = node;

				return true;
			}

			root = root->right;
		}
		else {
			if (root->left == NULL) {
				root->left = node;

				return true;
			}

			root = root->left;
		}
	}

	return false;
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
		puts("   ~");
	}
	else {
		printTree(root->right, level + 1);
		padding('\t', level);
		printf("%d\n", root->key);
		printTree(root->left, level + 1);
	}
}

void printList(NodeT* root) {
	NodeT* node = root->left;

	while (node->left) {
		node = node->left;
	}
	while (node != root)
	{
		printf("%d ", node->key);
		node = node->right;
	}
	
	printf("|%d| ", root->key);

	node = root->right;
	while (node) {
		printf("%d ", node->key);
		node = node->right;
	}
}

int main() {
	MIN = 1;
	MAX = 20;

	int* elements = generateArray(10, true, 0);
	NodeT* root = createNode(elements[0]);

	for (int i = 1; i < 10; i++) {
		insertTree(root, elements[i]);
	}

	printTree(root, 0);

	NodeT* prev = NULL;

	TransformV2(root, &prev);
	//Transform(root, NULL);

	printList(root);

}

