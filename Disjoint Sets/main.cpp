#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define COMP_EFF "Computational Effort Of Set Operations"

Profiler profiler("Disjoint Sets And Kruskal Algorithm");

/*
=============================================================================================================================================================

		Disjoint Sets
	   ---------------
			As the name suggests, disjoint sets contain various number of sets with the property that no element is present in more than one of them. They
		  support three different operations: Make Set, Find Set and Union. The implementation is done using trees.

			Make Set
				This is the way a set is created. It takes as parameter the element and sets its rank to 0 and his parent to be itself, thus becoming the
			  representative of this set. In this implementation the operation is done by the function CreateNode(key).

			Find Set
				Since we want to keep track of each element and the specific set that contains it, we define the operation Find Set which takes as parameter
			  an element and return the representative of the set in which it's contained. In this implementation the operation si done by the function
			  FindSet(node).

			Union
				In order to manipulate elements from different sets we define the operation union which unifies two sets ( always links the set with rank
			  smaller to the one with rank higher). In this implementation the operation si done by the function CombineSets(setA, setB).

			Performace
				The running time of performing m operations Make Set, Find Set and Union from which n of them are Make Set is O(m * alpha(n)) where alpha(n)
			  is a verry slowly growing function (most of the cases alpha(n) <= 4). This time is only achieve if two heuristics  strategies are used Path
			  Compression and Union By Rank.

					Path Compression --> when doing a Find Set operation we set each element on the path to the representative to point directly to it, thus
										making the next Find Set operation on one of those elements will require only O(1).

					Union By Rank    --> when union is performed we always link the set with the smaller rank to the one with higher rank.

		Kruskal Algorithem
	   --------------------
			This algorithm is used for finding the minimum spanning tree in a connected, weighted graph. It works by sortig the edges by their weights and
		  then choosing the edges with the smallest weight such that a node is taken only once. The algorithm is best implemented using disjoint sets, thus
		  being a good practice for working with them.

=============================================================================================================================================================
*/

int OP;
int maxWeight;

typedef struct node {
	int key;
	int maxHeight;
	struct node* parent;
} Node;

typedef struct edge {
	int weight;
	struct node* src;
	struct node* dest;
} Edge;

Node* createNode(int key) {
	Node* node = (Node*)malloc(sizeof(Node));

	if (node)
	{
		node->key = key;
		node->maxHeight = 0;
		node->parent = node;

		OP += 3;
		return node;
	}

	return NULL;
}

Node** createDisjointSets(int size) {
	Node** disjointSets = (Node**)malloc(size * sizeof(Node*));

	if (disjointSets)
	{
		for (int i = 0; i < size; i++)
		{
			disjointSets[i] = createNode(i + 1);
		}

		return disjointSets;
	}

	return NULL;
}

int** createAdjacencyMatrix(int size) {
	srand(time(0));

	int weight;
	int** adjMatrix = (int**)malloc(size * sizeof(int*));

	for (int i = 0; i < size; i++)
	{
		adjMatrix[i] = (int*)calloc(size, sizeof(int));
	}

	int* edges = (int*)malloc(size * sizeof(int));
	FillRandomArray(edges, size, 0, size - 1, true, 0);

	for (int i = 0; i < size - 1; i++)
	{
		weight = 1 + rand() % maxWeight;
		adjMatrix[edges[i]][edges[i + 1]] = weight;
		adjMatrix[edges[i + 1]][edges[i]] = weight;
	}

	int edgesLeftToAdd = 4 * size - (size - 1);
	int src, dest;

	while (edgesLeftToAdd > 0)
	{
		src = rand() % size;
		dest = rand() % size;

		if (adjMatrix[src][dest] == 0 && src != dest)
		{
			weight = 1 + rand() % maxWeight;
			adjMatrix[src][dest] = weight;
			adjMatrix[dest][src] = weight;
			edgesLeftToAdd--;
		}
	}

	return adjMatrix;
}

void swap(Edge* a, Edge* b) {
	Edge aux;

	aux = *a;
	*a = *b;
	*b = aux;
}

int partition(Edge* a, int l, int r) {
	int piv = a[r].weight;
	int i = l - 1;

	for (int j = l; j <= r - 1; j++)
	{
		if (a[j].weight < piv)
		{
			i++;
			swap(&a[i], &a[j]);
		}
	}

	swap(&a[i + 1], &a[r]);

	return i + 1;
}

int randomPartition(Edge* a, int l, int r) {
	srand(time(0));
	int random = l + rand() % (r - l + 1);

	swap(&a[r], &a[random]);

	return partition(a, l, r);
}

void quickSort(Edge* a, int l, int r) {
	if (r >= l)
	{
		int p = randomPartition(a, l, r);

		quickSort(a, l, p - 1);
		quickSort(a, p + 1, r);
	}
}

Edge* getEdgesSorted(Node** nodes, int** adjMatrix, int size) {
	int nbOfEdges = 4 * size;
	Edge* edges = (Edge*)malloc(nbOfEdges * sizeof(Edge));

	int index = 0;

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (adjMatrix[i][j] > 0)
			{
				edges[index].weight = adjMatrix[i][j];
				edges[index].dest = nodes[j];
				edges[index].src = nodes[i];

				index++;
			}
		}
	}

	quickSort(edges, 0, nbOfEdges - 1);

	return edges;
}

void freeDisjointSets(Node** disjointSets, int size) {
	for (int i = 0; i < size; i++)
	{
		free(disjointSets[i]);
	}

	free(disjointSets);
}

void freeMatrix(int** matrix, int size) {
	for (int i = 0; i < size; i++)
	{
		free(matrix[i]);
	}

	free(matrix);
}

Node* findSet(Node* node) {
	if (node != node->parent)
	{
		node->parent = findSet(node->parent);
		OP++;
	}

	OP++;
	return node->parent;
}

void combineSets(Node* setA, Node* setB) {

	if (setA->maxHeight > setB->maxHeight)
	{
		setB->parent = setA;
		setA->maxHeight++;

		setB = setA;

		OP += 3;
	}
	else
	{
		setA->parent = setB;
		setB->maxHeight++;

		setA = setB;

		OP += 3;
	}

	OP++;
}

void printDisjointSets(Node** sets, int size) {
	for (int i = 0; i < size; i++)
	{
		printf("Set %d representative is %d\n", i + 1, findSet(sets[i])->key);
	}

	printf("\n");
}

void printAdjMatrix(int** adjMatrix, int size) {
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", adjMatrix[i][j]);
		}

		printf("\n");
	}
}

void printEdges(Edge* edges, int size) {
	for (int i = 0; i < size; i++)
	{
		printf("Src = %d Dest = %d Weight = %d\n", edges[i].src->key, edges[i].dest->key, edges[i].weight);
	}
}

bool isOneSet(Node** disjointSets, int size) {
	Node* representant = findSet(disjointSets[0]);

	for (int i = 1; i < size; i++)
	{
		if (representant != findSet(disjointSets[i]))
		{
			return false;
		}
	}

	return true;
}

void demoDisjointSets() {
	srand(time(0));
	int size = 10;

	Node** disjointSets = createDisjointSets(size);

	printf("\t\t\t\t\t DEMO DISJOINT SETS\n\n");
	printf("\t\t Sets After Creation\n");
	printDisjointSets(disjointSets, size);

	int indexA;
	int indexB;
	int nbOfUnions = 0;

	while (nbOfUnions < 5)
	{
		indexA = rand() % size;
		indexB = rand() % size;

		if (findSet(disjointSets[indexA]) != findSet(disjointSets[indexB]))
		{
			printf("\t\t Union Between Sets %d And %d\n", disjointSets[indexA]->key, disjointSets[indexB]->key);

			combineSets(findSet(disjointSets[indexA]), findSet(disjointSets[indexB]));

			printDisjointSets(disjointSets, size);

			nbOfUnions++;
		}
	}

	freeDisjointSets(disjointSets, size);
	printf("\n--------------------------------------------------DEMO END--------------------------------------------------------------\n");
}

void demoKruskal() {
	printf("\t\t\t\t\t DEMO KRUSKAL\n");

	maxWeight = 50;
	int size = 10;

	Node** disjointSets = createDisjointSets(size);

	int** adjMatrix = createAdjacencyMatrix(size);
	printf("\n\t\t GRAPH BY ADJANCENCY MATRIX\n\n");
	printAdjMatrix(adjMatrix, size);

	Edge* edges = getEdgesSorted(disjointSets, adjMatrix, size);
	printf("\n\t\t EDGES OF THE GRAPH ORDERED\n\n");
	printEdges(edges, 4 * size);

	Edge* selectedEdges = (Edge*)malloc((size - 1) * sizeof(Edge));

	int indexEdges = 0;
	int indexSelEdges = 0;

	while (!isOneSet(disjointSets, size))
	{
		if (findSet(edges[indexEdges].src) != findSet(edges[indexEdges].dest))
		{
			combineSets(findSet(edges[indexEdges].src), findSet(edges[indexEdges].dest));
			selectedEdges[indexSelEdges] = edges[indexEdges];
			indexSelEdges++;
		}

		indexEdges++;
	}

	printf("\n\t\t THE DISJOINT SETS AFTER APLYING KRUSKAL ALG.\n\n");
	printDisjointSets(disjointSets, size);

	printf("\n\t\t THE EDGES WHICH CONSTITUE THE MINIMUM SPANNING TREE\n\n");
	printEdges(selectedEdges, indexSelEdges);

	freeDisjointSets(disjointSets, size);
	free(edges);
	free(selectedEdges);
	freeMatrix(adjMatrix, size);

	printf("\n--------------------------------------------------DEMO END--------------------------------------------------------------\n");
}

void generateChart() {
	maxWeight = 50000;

	Node** disjointSets;
	Edge* edges;
	int** adjMatrix;

	for (int size = 100; size <= 10000; size += 100)
	{
		OP = 0;

		disjointSets = createDisjointSets(size);
		adjMatrix = createAdjacencyMatrix(size);
		edges = getEdgesSorted(disjointSets, adjMatrix, size);

		int index = 0;

		for (int i = 0; i < 4 * size; i++)
		{
			if (findSet(edges[index].src) != findSet(edges[index].dest))
			{
				combineSets(findSet(edges[index].src), findSet(edges[index].dest));
			}

			index++;
		}

		freeDisjointSets(disjointSets, size);
		free(edges);
		freeMatrix(adjMatrix, size);

		profiler.countOperation(COMP_EFF, size, OP);
	}

	profiler.showReport();
}

int main() {
	demoDisjointSets();
	demoKruskal();

	generateChart();
}