#include<stdlib.h>
#include<stdio.h>
#include "Profiler.h"

/*

	Dynamic Order Statistics
   --------------------------
		DOS Trees are an example of the ways in which a BST can be improved by storing additional data about the structure
	  in each node. That is, in this case, the dimension of the tree computed as dimL + dimR + 1. This extra info can be
	  used to define two new operations on the tree:

			OS_Select(Tree, index)  --> returns the i'th node in the inorder				O(log N)
			OS_Rank(Tree, x)		--> returns the position of node x in the inorder		O(log N)

		Having this data stored in each of the tree's node implies that when a deletion is made we must update the dimension
	  of all the nodes preceding the node deleted on the same brench.

		Evaluation of the Operations

			Build Tree
				The tree is built using the method createBalancedTree which adds elements one by one to the tree by calling
			  the method insertTree.

				insertTree --> O(log n)
				createBalancedTree --> O(n)

			OS_Select
				Starts from the root and goes down level by level. In worst case it reaches a leaf therefor the running time
			  O(log n).

			OS_Delete
				Deletes the node retreived by OS_Select. The search is done in O(log n) and the deletion takes O(log n).
			  Summing them up we get O(log n).

		Chart analysis
			By looking at the running time of each operation we can clearly see that build the tree will make many more
		  operations than the other two. This is also ilustrated in the charts as Build Tree effort goes up to 500000
		  operations while the other two are toped by 200000.

			Things might not seem right at the first look when talking about OS_Select and OS_Delete because they both
		  have tha same complexity O(log n) but Delete does roughly 50000 operations more than Select. But that is because
		  of the hidden constants that are greater for the Delete operation than for the Select. But overall they are pretty
		  close and the chart shows this aswell.
*/


#define AVG_EFFORT_OS_DELETE "Average comp. effort OS_Delete"
#define AVG_EFFORT_OS_SELECT "Average comp. effort OS_Select"
#define AVG_EFFORT_BUILD_TREE "Average comp. effort Build Tree"

Profiler profiler("OS Tree");

int OS_S_OP, OS_D_OP, OS_S_OP_T, OS_D_OP_T, BUILD_OP;

int MIN, MAX;

typedef struct nodeT {
	int key;
	int dim;
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
		node->dim = 1;
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
		BUILD_OP++;
		return true;
	}
	BUILD_OP++;

	NodeT* root = T->root;

	while (root) {
		root->dim++;
		BUILD_OP++;

		if (node->key > root->key) {
			if (root->right == NULL) {
				root->right = node;
				node->parent = root;
				BUILD_OP += 2;
				return true;
			}
			BUILD_OP++;

			root = root->right;
			BUILD_OP++;
		}
		else {
			if (root->left == NULL) {
				root->left = node;
				node->parent = root;
				BUILD_OP += 2;
				return true;
			}
			BUILD_OP++;

			root = root->left;
			BUILD_OP++;
		}
		BUILD_OP++;
	}

	return false;
}

NodeT* OS_Select(NodeT* root, int index) {
	OS_S_OP++;

	if (root)
	{
		int indexRoot = 1;

		if (root->left) {
			indexRoot = root->left->dim + 1;
			OS_S_OP++;
		}
		OS_S_OP++;

		OS_S_OP++;
		if (index == indexRoot)
		{
			return root;
		}
		else {
			if (index > indexRoot)
			{
				return OS_Select(root->right, index - indexRoot);
			}
			OS_S_OP++;

			return OS_Select(root->left, index);
		}
	}

	return NULL;
}

void updateDim(NodeT* node) {
	while (node)
	{
		node->dim--;
		node = node->parent;
		OS_D_OP += 2;
	}
}

void replaceChild(Tree* T, NodeT* root, NodeT* child, NodeT* newChild) {
	updateDim(root);

	if (root == NULL)
	{
		T->root = newChild;
		OS_D_OP++;
	}
	else if (root->left == child)
	{
		root->left = newChild;
		OS_D_OP++;
	}
	else if (root->right == child)
	{
		root->right = newChild;
		OS_D_OP++;
	}

	if (newChild)
	{
		newChild->parent = root;
		OS_D_OP++;
	}
	OS_D_OP += 4;
}

void deleteNode(Tree* T, NodeT* node) {
	if (node)
	{
		if (node->left == NULL && node->right == NULL)
		{
			replaceChild(T, node->parent, node, NULL);
			free(node);
		}
		else if (node->left != NULL && node->right == NULL)
		{
			replaceChild(T, node->parent, node, node->left);
			free(node);
		}
		else if (node->left == NULL && node->right != NULL)
		{
			replaceChild(T, node->parent, node, node->right);
			free(node);
		}
		else
		{
			NodeT* min = node->right;
			OS_D_OP++;

			while (min->left)
			{
				min = min->left;
				OS_D_OP++;
			}

			node->key = min->key;
			OS_D_OP++;

			deleteNode(T, min);
		}
		OS_D_OP += 3;
	}
	OS_D_OP++;
}

void OS_Delete(Tree* T, int index) {
	if (T->size > 0)
	{
		NodeT* node = OS_Select(T->root, index);

		deleteNode(T, node);

		T->size--;
	}
}

void createBalancedTree(Tree* T, int* a, int l, int r) {
	if (r >= l)
	{
		int middle = (r + l) / 2;

		insertTree(T, a[middle]);

		createBalancedTree(T, a, l, middle - 1);
		createBalancedTree(T, a, middle + 1, r);
	}
}

Tree* buildTree(int n) {
	int* a = generateArray(n, 0, 1);
	Tree* T = createTree();

	createBalancedTree(T, a, 0, n - 1);

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
		puts("   ~");
	}
	else {
		printTree(root->right, level + 1);
		padding('\t', level);
		printf("(%d | %d)\n", root->key, root->dim);
		printTree(root->left, level + 1);
	}
}

void demo() {
	printf("\n\t\t\t\t THIS IS A DEMO BUILD TREE OS_SELECT OS_DELETE\n");
	MIN = 1;
	MAX = 11;
	Tree* T = buildTree(11);

	printf("\t TREE AFTER CREATION\n");
	printTree(T->root, 0);

	printf("\n\t OS_SELECT AND OS_DELETE OPERATIONS\n");

	int index;
	srand(time(NULL));

	for (int i = 0; i < 11; i++)
	{
		//index = 1 + rand() % T->size;
		index = T->size / 2 + 1;

		printf("Element at index %d is: %d\n", index, OS_Select(T->root, index)->key);
		printf("Tree after deletion of element at index %d :\n", index);

		OS_Delete(T, index);

		printTree(T->root, 0);
	}

	freeTree(T);
	printf("\n\t\t\t\t END DEMO\n");
}

void generateChart() {
	MIN = 10;
	MAX = 50000;

	Tree* T;

	int index;
	srand(time(NULL));

	for (int size = 100; size <= 10000; size += 100)
	{
		OS_D_OP_T = 0;
		OS_S_OP_T = 0;
		BUILD_OP = 0;

		for (int j = 0; j < 5; j++) {
			T = buildTree(size);

			for (int i = 0; i < T->size; i++)
			{
				OS_S_OP = 0;
				OS_D_OP = 0;

				index = 1 + rand() % T->size;

				OS_Select(T->root, index);
				OS_S_OP_T += OS_S_OP;
				OS_D_OP_T += OS_S_OP;

				OS_Delete(T, index);
				OS_D_OP_T += OS_D_OP;
			}

			freeTree(T);
		}

		profiler.countOperation(AVG_EFFORT_OS_DELETE, size, OS_D_OP_T / 5);
		profiler.countOperation(AVG_EFFORT_OS_SELECT, size, OS_S_OP_T / 5);
		profiler.countOperation(AVG_EFFORT_BUILD_TREE, size, BUILD_OP / 5);
	}

	profiler.showReport();
}

void main() {
	demo();

	//generateChart();
}

