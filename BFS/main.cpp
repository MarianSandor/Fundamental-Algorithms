#include<stdio.h>
#include<stdlib.h>
#include "Profiler.h"

#define VARRY_E "Computational Effort Varry E"
#define VARRY_V "Computational Effort Varry V"

Profiler profiler("BFS");

/*
		Bread-First Search
	   --------------------
			Bread-First Search is one of the simplest algorithm for seaching a graph and it also creates the base of other important algorithm such as Prim's 
		  minumum spanning tree and Dijkstra's shortest path algortihm. As the name suggests, the strategy is to search for adjacent nodes level-wise until all
		  the nodes have been examined. This process is easily achieved using a proper queue to store the nodes waiting to be examined. In order to keep track
		  of which nodes were examined, which are to be examined and which haven't been reached the algorithm assings color to each node to define the status of
		  it.
				White --> the node has not yet been discovered
				Grey --> the node was discoverd and is waiting to be examined ( it is in the queue )
				Black --> the node was examined

			In this way, the algorithm makes sure that no node is examined twice and thus bounding the running time to be linear O(E + V).
			
			BFS algorithm can be traced by looking at the tree that it ouputs. The root will always be the starting node and further on the order of the nodes 
		  on each level may differ. It is important to mention that in case of an unconnected graph the out of BFS will be more than one tree, a forest of trees.
		  This can be obtain by applying BFS on different nodes untill all the nodes al colored black. Also to keep in mind that the number of trees that are 
		  outputed is equal to the number of connected components in the graph.

		
		Chart Analysis
	   ----------------
			For this analysis, since there are two values that define a graph and both of them are subjects of fluctuation, we performed two tests.

				I. Set V = 100 And Varry E = 1000 - 5000
					The result is a liniar function having the leading term E.

				II. Set E = 9000 And Varry V = 100 - 200
					The result is a liniar function having the leading term E.

			As expected from the running time of BFS which is O(E + V) the plot coincides with a linear function which has the explicit values V + E.
			There are no best or worst case for this algorithm, no matter what it still runs in exactly V + E time.

*/

enum Color { Black = 0, White = 1, Grey = 2 };
int OP;

typedef struct nodeL {
	struct nodeG* key;
	struct nodeL* next;
} NodeL;

typedef struct nodeG {
	int key;
	struct nodeL* list;
} NodeG;

typedef struct nodeQ {
	struct nodeG* key;
	Color color;
	struct nodeQ* next;
} NodeQ;

typedef struct queue {
	int size;
	struct nodeQ* first;
	struct nodeQ* last;
} Queue;

NodeL* createNodeL(nodeG* key) {
	NodeL* node = (NodeL*)malloc(sizeof(NodeL));

	if (node)
	{
		node->key = key;
		node->next = NULL;

		return node;
	}

	return NULL;
}

NodeG* createNodeG(int key) {
	NodeG* node = (NodeG*)malloc(sizeof(NodeG));

	if (node)
	{
		node->key = key;
		node->list = NULL;

		return node;
	}

	return NULL;
}

NodeQ* createNodeQ(NodeG* key) {
	NodeQ* node = (NodeQ*)malloc(sizeof(NodeQ));

	if (node)
	{
		node->key = key;
		node->color = White;
		node->next = NULL;

		return node;
	}

	return NULL;
}

Queue* createQueue() {
	Queue* Q = (Queue*)malloc(sizeof(Queue));

	if (Q)
	{
		Q->size = 0;
		Q->first = Q->last = NULL;

		return Q;
	}

	return NULL;
}

void freeGraph(NodeG** nodes, int size) {
	for (int i = 0; i < size; i++)
	{
		free(nodes[i]);
	}

	free(nodes);
}

void addNeighbour(NodeG* node, NodeG* neighbour) {
	NodeL* newNode = createNodeL(neighbour);

	if (node->list == NULL)
	{
		node->list = newNode;
	}
	else
	{
		NodeL* curr = node->list;

		while (curr->next)
		{
			curr = curr->next;
		}

		curr->next = newNode;
	}
}

bool checkForEdge(NodeG* nodeA, NodeG* nodeB) {
	NodeL* curr = nodeA->list;

	while (curr)
	{
		if (curr->key == nodeB)
		{
			return true;
		}

		curr = curr->next;
	}

	return false;
}

void enqueue(Queue* Q, NodeQ* node) {
	if (Q->size == 0)
	{
		Q->first = Q->last = node;
	}
	else
	{
		Q->last->next = node;
		Q->last = node;
	}

	Q->size++;
}

NodeQ* dequeue(Queue* Q) {
	if (Q->size > 0)
	{
		NodeQ* node = Q->first;

		Q->first = Q->first->next;
		Q->size--;

		return node;
	}

	return NULL;
}

NodeG** createGraph(int V, int E) {
	srand(time(0));
	NodeG** nodes = (NodeG**)malloc(V * sizeof(NodeG*));

	for (int i = 0; i < V; i++)
	{
		nodes[i] = createNodeG(i + 1);
	}

	int nbOfEdges = 0;
	while (nbOfEdges < E)
	{
		int indexA = rand() % V;
		int indexB = rand() % V;

		if (indexA != indexB && !checkForEdge(nodes[indexA], nodes[indexB]))
		{
			addNeighbour(nodes[indexA], nodes[indexB]);
			//addNeighbour(nodes[indexB], nodes[indexA]);

			nbOfEdges++;
		}
	}

	return nodes;
}

bool allBlacks(NodeQ** Qnodes, int size) {
	for (int i = 0; i < size; i++)
	{
		if (Qnodes[i]->color != Black) {
			return false;
		}
	}

	return true;
}

void BFS(NodeG** nodes, int size) {
	int* indexes = (int*)malloc(size * sizeof(int));
	FillRandomArray(indexes, size, 0, size - 1, true, 0);

	NodeQ** Qnodes = (NodeQ**)malloc(size * sizeof(NodeQ*));
	Queue* Q = createQueue();
	OP++;

	for (int i = 0; i < size; i++)
	{
		Qnodes[i] = createNodeQ(nodes[i]);
		OP++;
	}

	int index = 0;

	while (!allBlacks(Qnodes, size)) {

		while (Qnodes[indexes[index]]->color == Black) {
			index++;
		}

		Qnodes[indexes[index]]->color = Grey;
		OP++;
		enqueue(Q, Qnodes[indexes[index]]);
		OP++;

		while (Q->size > 0)
		{
			NodeQ* node = dequeue(Q);
			OP++;
			node->color = Black;
			OP++;

			NodeL* curr = node->key->list;

			while (curr)
			{
				if (Qnodes[curr->key->key - 1]->color == White)
				{
					Qnodes[curr->key->key - 1]->color = Grey;
					OP++;
					enqueue(Q, Qnodes[curr->key->key - 1]);
					OP++;
				}
				OP++;

				curr = curr->next;
			}
		}
	}
}

void padding(int size) {
	for (int i = 0; i < size; i++)
	{
		printf("\t");
	}
}

void prettyPrint(int* tree, int level, int size, int node) {
	
	if (node != 0)
	{
		padding(level);
		printf("%d\n", node);

		for (int i = 1; i <= size; i++)
		{
			if (tree[i] == node)
			{
				prettyPrint(tree, level + 1, size, i);
			}
		}
	}
}

void printGraph(NodeG** nodes, int V) {
	for (int i = 0; i < V; i++)
	{
		printf("Node %d neighbours ", nodes[i]->key);

		NodeL* curr = nodes[i]->list;

		while (curr)
		{
			printf("%d ", curr->key->key);
			curr = curr->next;
		}

		printf("\n");
	}
}

int findRoot(int* tree, int size) {
	for (int i = 1; i <= size; i++)
	{
		if (tree[i] == -1)
		{
			return i;
		}
	}

	return -1;
}

void BFSDemo() {
	printf("\t\t\t\t\t DEMO BFS\n\n");

	printf("\t The Graph is...\n\n");
	int E = 20;
	int V = 10;
	NodeG** nodes = createGraph(V, E);
	printGraph(nodes, V);
	printf("\n\n");

	int* indexes = (int*)malloc(V * sizeof(int));
	FillRandomArray(indexes, V, 0, V - 1, true, 0);

	NodeQ** Qnodes = (NodeQ**)malloc(V * sizeof(NodeQ*));
	Queue* Q = createQueue();

	for (int i = 0; i < V; i++)
	{
		Qnodes[i] = createNodeQ(nodes[i]);
	}

	int index = 0;
	int* tree = (int*)malloc((V + 1)* sizeof(int));

	printf("\t Applying BFS...\n\n");

	while (!allBlacks(Qnodes, V)) {

		while (Qnodes[indexes[index]]->color == Black) {
			index++;
		}

		for (int i = 1; i <= V; i++)
		{
			tree[i] = 0;
		}

		printf(" Starting Node %d\n", Qnodes[indexes[index]]->key->key);

		Qnodes[indexes[index]]->color = Grey;
		enqueue(Q, Qnodes[indexes[index]]);

		tree[Qnodes[indexes[index]]->key->key] = -1;

		while (Q->size > 0)
		{
			NodeQ* node = dequeue(Q);
			node->color = Black;

			NodeL* curr = node->key->list;

			while (curr)
			{
				if (Qnodes[curr->key->key - 1]->color == White)
				{
					Qnodes[curr->key->key - 1]->color = Grey;
					enqueue(Q, Qnodes[curr->key->key - 1]);

					tree[curr->key->key] = node->key->key;
				}

				curr = curr->next;
			}

		}

		printf(" The Parent Array Of The Found Tree: ");
		for (int k = 1; k <= V; k++)
		{
			printf("%d ", tree[k]);
		}

		printf("\n The Resulting Tree:\n\n");
		prettyPrint(tree, 1, V, findRoot(tree, V));
		printf("\n\n");
	}

	printf("============================================= DEMO END =================================================================\n\n");
}

void generateChartVarryE() {
	int V = 100;
	int E;
	NodeG** nodes;

	for (E = 1000; E <= 5000; E += 100)
	{
		OP = 0;

		nodes = createGraph(V, E);

		BFS(nodes, V);

		freeGraph(nodes, V);

		profiler.countOperation(VARRY_E, E, OP);
	}
}

void generateChartVarryV() {
	int V;
	int E = 9000;
	NodeG** nodes;

	for (V = 100; V <= 200; V += 10)
	{
		OP = 0;

		nodes = createGraph(V, E);

		BFS(nodes, V);

		freeGraph(nodes, V);

		profiler.countOperation(VARRY_V, V, OP);
	}
}

void generateCharts() {
	generateChartVarryE();

	generateChartVarryV();

	profiler.showReport();
}

void main() {
	BFSDemo();
	
	generateCharts();
}
