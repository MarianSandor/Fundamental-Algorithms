#include <stdlib.h>
#include <stdio.h>	
#include <fstream>
#include "Profiler.h"

Profiler profiler("Multi-Way Tree");

/*
		Parent representation R1
	   --------------------------
			This is the representation of our initial tree. In this representation the tree is stored in an one dimensional array where the
		  indexes from 1 to n are the acutal keys of the nodes and the values stored at each of them is the parent node key. The index which
		  marks the key of the node which is the root has the unique value -1. We can obtain the initial tree in two ways, by calling the 
		  function createTreeR1 which reads the array from the file "Input.txt" (format is size followed by the elements of the array), or
		  by calling the function generateTreeR1(size) which randomly generates an array which stores a proper tree of size n. Both of the 
		  function respect the convention that in out array T the nodes start from i=1 and T[0] stores the size of the tree.

		Transformation R1 --> R2
	   --------------------------
			As the name suggests, the goal is to obtain the same tree as the one given in representation R1 but having the characteristics
		  of the R2 representation. The strategy is as follows, find the index of the root in the R1 then recursevely check for children 
		  and if found store them in the array of their parent. This is done by the function buildTreeR22(TR1, key, children) which runs in linear 
		  time O(n) but uses O(n) additional space or function buildTreeR2(TR1, key) with no additional memory space.

		Multi-Way tree representation R2
	   ----------------------------------
			Now we enter the data structure territory, meaning that we work with proper nodes that store date and pointers to their relatives.
		  More specifically, each node has a key and a vector of their children. The structure of a node for this representation is describe 
		  by NodeR2 type. 

		Transformation R2 --> R3
	   --------------------------
			Here the goal is to obtain a tree represented in R3 out of a tree which has the structure of the R2. Now we first create the root
		  then recursevely iterate the array of children of each node in R2 and set the first to be the child of the root. Also, we link each 
		  of the nodes in the children array to brothers. This is achieved by the function buildTreeR3(NodeR2* nodeR2) which has linear running 
		  time.

		Binary tree representation R3
	   -------------------------------
			In this representation the nodes have a pretty straight foward structure. Each node stores its key, a pointer to his child and
		  a pointer to his brother. The root will have no brother. The structure of a node in this kind of representation is describe by 
		  NodeR3 type.
*/


typedef struct nodeR2 {
	int key;
	int nbOfChildren;
	struct nodeR2** children;
} NodeR2;

typedef struct treeR2 {
	int size;
	struct nodeR2* root;
} TreeR2;

typedef struct nodeR3 {
	int key;
	struct nodeR3* child;
	struct nodeR3* brother;
} NodeR3;

typedef struct treeR3 {
	int size;
	struct nodeR3* root;
} TreeR3;

NodeR2* createNodeR2(int key, int nbOfChildren) {
	NodeR2* node = (NodeR2*)malloc(sizeof(NodeR2));

	if (node)
	{
		node->key = key;
		if (nbOfChildren > 0)
		{
			node->children = (NodeR2**)malloc(nbOfChildren * sizeof(NodeR2*));
			node->nbOfChildren = nbOfChildren;
		}
		else {
			node->nbOfChildren = 0;
		}

		return node;
	}

	return NULL;
}

NodeR3* createNodeR3(int key) {
	NodeR3* node = (NodeR3*)malloc(sizeof(NodeR3));

	if (node)
	{
		node->key = key;
		node->child = NULL;
		node->brother = NULL;

		return node;
	}

	return NULL;
}

int* createTreeR1() {
	FILE* pf;
	fopen_s(&pf, "Input.txt", "r");

	int size = 0;
	fscanf_s(pf, "%d", &size);

	int* T = (int*)malloc((size + 1) * sizeof(int));
	T[0] = size;

	for (int i = 1; i <= size; i++)
	{
		fscanf_s(pf, "%d", &T[i]);
	}

	fclose(pf);

	return T;
}

int getNbOfChildren(int* T, int index) {
	int size = T[0];
	int nbOfChildren = 0;

	for (int i = 1; i <= size; i++)
	{
		if (T[i] == index)
		{
			nbOfChildren++;
		}
	}

	return nbOfChildren;
}

int getRoot(int* T) {
	int size = T[0];

	for (int i = 1; i <= size; i++)
	{
		if (T[i] == -1)
		{
			return i;
		}
	}
}

void validateTree(int* TR1, int index, int* checked) {

	(*checked)++;

	for (int i = 1; i <= TR1[0]; i++)
	{
		if (TR1[i] == index)
		{
			validateTree(TR1, i, checked);
		}
	}
}

int* generateTreeR1(int size) {
	srand(time(0));
	int index = 1 + rand() % size;
	int tries = 0;
	int* T;

	while (true)
	{
		tries++;

		T = (int*)malloc((size + 1) * sizeof(int));
		FillRandomArray((T + 1), size, 1, size, false, 0);

		T[0] = size;
		T[index] = -1;

		int checked = 0;
		validateTree(T, index, &checked);

		if (checked == size)
		{
			break;
		}

		free(T);
	}

	printf("\n\nIt took %d tries to generate a proper tree\n\n", tries);

	return T;
}

NodeR2* buildTreeR2(int* TR1, int key) {

	int nbOfChildren = getNbOfChildren(TR1, key);       

	NodeR2* root = createNodeR2(key, nbOfChildren);		

	if (nbOfChildren > 0)
	{
		int j = 0;
		for (int i = 1; i <= TR1[0]; i++)
		{
			if (TR1[i] == root->key)
			{
				root->children[j] = buildTreeR2(TR1, i);
				j++;
			}
		}
	}

	return root;
}

TreeR2* createTreeR2(int* TR1) {
	TreeR2* TR2 = (TreeR2*)malloc(sizeof(TR2));
	TR2->size = TR1[0];

	int keyRoot = getRoot(TR1);

	TR2->root = buildTreeR2(TR1, keyRoot);

	return TR2;
}

NodeR2* buildTreeR22(int* TR1, int key, int* children) {

	int nbOfChildren = children[key];

	NodeR2* root = createNodeR2(key, nbOfChildren);

	if (nbOfChildren > 0)
	{
		int j = 0;
		for (int i = 1; i <= TR1[0]; i++)
		{
			if (TR1[i] == root->key)
			{
				root->children[j] = buildTreeR2(TR1, i);
				j++;
			}
		}
	}

	return root;
}

TreeR2* createTreeR22(int* TR1) {
	TreeR2* TR2 = (TreeR2*)malloc(sizeof(TR2));
	TR2->size = TR1[0];

	int* children = (int *) calloc(TR1[0] + 1, sizeof(int));
	for (int i = 1; i <= TR1[0]; i++) {
		children[TR1[i]] ++;
	}

	int keyRoot = getRoot(TR1);

	TR2->root = buildTreeR22(TR1, keyRoot, children);

	return TR2;
}

NodeR3* buildTreeR3(NodeR2* nodeR2) {

	NodeR3* child = NULL;
	NodeR3* brother;
	NodeR3* prevBrother = NULL;

	for (int i = 0; i < nodeR2->nbOfChildren; i++)
	{
		brother = createNodeR3(nodeR2->children[i]->key);

		if (prevBrother)
		{
			prevBrother->brother = brother;
		}

		brother->child = buildTreeR3(nodeR2->children[i]);

		prevBrother = brother;

		if (i == 0)
		{
			child = brother;
		}
	}

	return child;

}

TreeR3* createTreeR3(TreeR2* TR2) {
	TreeR3* TR3 = (TreeR3*)malloc(sizeof(TreeR3));
	TR3->size = TR2->size;

	TR3->root = createNodeR3(TR2->root->key);

	TR3->root->child = buildTreeR3(TR2->root);

	return TR3;
}

void printT1(int* T1) {

	for (int i = 1; i <= T1[0]; i++)
	{
		printf("%d ", T1[i]);
	}
}

void printT2(TreeR2* T, NodeR2* root) {

	if (T->root == root)
	{
		printf("The root key is %d and children: ", root->key);
	}
	else
	{
		printf("The node key is %d and children: ", root->key);
	}

	if (root->nbOfChildren > 0)
	{
		for (int i = 0; i < root->nbOfChildren; i++)
		{
			printf("%d ", root->children[i]->key);
		}
	}
	else
	{
		printf("~");
	}

	printf("\n");

	for (int i = 0; i < root->nbOfChildren; i++)
	{
		printT2(T, root->children[i]);
	}
}

void prettyPrint(NodeR3* root, int level) {
	if (root)
	{
		NodeR3* node = root;

		while (node)
		{
			for (int i = 0; i < level; i++)
			{
				printf("\t");
			}

			printf("%d\n", node->key);

			prettyPrint(node->child, level + 1);

			node = node->brother;
		}
	}
}

void demo(int* TR1) {

	TreeR2* TR2 = createTreeR22(TR1);
	TreeR3* TR3 = createTreeR3(TR2);

	printf("\t\t\t\t\tTree in representation form 1:\n\n");
	printT1(TR1);
	printf("\n\n");

	printf("------------------------------------------------------------------------------------------------------------------\n\n");

	printf("\t\t\t\t\tTree in representation form 2:\n\n");
	printT2(TR2, TR2->root);
	printf("\n\n");

	printf("------------------------------------------------------------------------------------------------------------------\n\n");

	printf("\t\t\t\t\tTree in representation form 3:\n\n");
	prettyPrint(TR3->root, 1);
	printf("\n\n");

	printf("------------------------------------------------------------------------------------------------------------------\n");
}

void main() {

	int* TR1;

	TR1 = createTreeR1();
	demo(TR1);
	free(TR1);

	TR1 = generateTreeR1(10);
	demo(TR1);
	free(TR1);
}