#include<stdio.h>
#include<stdlib.h>
#include <stack> 
#include <algorithm>  
#include "Profiler.h"

using namespace std;

#define VARRY_E "Computational Effort Varry E"
#define VARRY_V "Computational Effort Varry V"

Profiler profiler("DFS");

/*
		Depth-First Search
	   --------------------
			Is the most important basic algorithm on graph because many other algorithm are derived from it. It traverses the graph always trying to go as deep
		  as possible without visiting a node twice. As for BFS, DFS can also be traced by the forest of trees it outputs or by the timestamps of the nodes. 
		  Another important aspect of the depth-first search is that it splits the graph edges in 4 categories, thus retreving precious information.

		  Running Time
				Since it adopts the same coloring strategy as BFS which asures that no node is visited twice, its running time would be V. But because the adj 
			list of each node is parsed the final running time is O(E + V) which is as we'd expected, linear.
		
		Topological Sort
	   ------------------
			This algorithm relies pretty much on depth-first search, adding just a few extras in order to obtain the result. The output of the algorithm is a
		  permutation of all the nodes of the graph such that no node has a edge to one of the nodes located at a previous position in the vector. This algorithm
		  only works for DAG (directed acycle graphs).

			Running Time
				Beacause it is mainly a DFS with minor things added which doesn't impact the running time, it remains linear thus O(E + V).

		Tarjan Algorithm
	   ------------------
			Tarjan algorithm is used to determine the SCC (strongly connected components) of a graph. Again DFS comes in handy for doing that and more specifically
		  Tarjan algortihm uses DFS_Visit function in order to search for SCC in the graph.

			Running Time
				Although the implementation needs a few more structures to reach its goal (such as a proper stack for nodes) being based on DFS and not altering
			  its running time it remains linear. Even though it hides larger multiplicative constants, Tarjan algorithm running time is O(E + V).
*/

enum Color { Black = 0, White = 1, Grey = 2 };
int OP;
int TIME;
int ID;

typedef struct nodeL {
	struct nodeG* key;
	struct nodeL* next;
} NodeL;

typedef struct nodeG {
	int key;
	struct nodeL* list;
} NodeG;

typedef struct nodeDFS {
	NodeG* node;
	Color color;
	int timeBeg;
	int timeFin;
	int parent;
} NodeDFS;

typedef struct nodeT {
	NodeG* node;
	Color color;
	bool onStack;
	int id;
	int minid;
} NodeT;

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

			nbOfEdges++;
		}
	}

	return nodes;
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

void padding(int size) {
	for (int i = 0; i < size; i++)
	{
		printf("\t");
	}
}

void prettyPrint(NodeDFS* tree, int level, int size, NodeDFS node) {

	padding(level);
	printf("%d\n", node.node->key);

	for (int i = 0; i < size; i++)
	{
		if (tree[i].parent == node.node->key - 1)
		{
			prettyPrint(tree, level + 1, size, tree[i]);
		}
	}
}

void printByTime(NodeDFS* nodesDFS, int size) {
	for (int time = 1; time <= 2 * size; time++)
	{
		for (int j = 0; j < size; j++) {
			if (nodesDFS[j].timeBeg == time) {
				printf("(%d ", nodesDFS[j].node->key);
			}
			else if (nodesDFS[j].timeFin == time)
			{
				printf("%d) ", nodesDFS[j].node->key);
			}
		}
	}
}

void DFS_Visit(NodeDFS* nodesDFS, int node, std::stack <int> *ordered)
{
	TIME++;
	nodesDFS[node].timeBeg = TIME;
	nodesDFS[node].color = Grey;

	NodeL* curr = nodesDFS[node].node->list;

	while (curr) {
		if (nodesDFS[curr->key->key - 1].color == White) {
			nodesDFS[curr->key->key - 1].parent = node;
			OP++;
			DFS_Visit(nodesDFS, curr->key->key - 1, ordered);
		}
		OP++;

		curr = curr->next;
	}

	nodesDFS[node].color = Black;
	TIME++;
	nodesDFS[node].timeFin = TIME;

	(*ordered).push(nodesDFS[node].node->key);
}

void DFS(NodeG** nodes, int V, bool demo, bool sort) {

	NodeDFS* nodesDFS = (NodeDFS*)malloc(V * sizeof(NodeDFS));

	std::stack <int> ordered; // used only for topological sort;

	for (int i = 0; i < V; i++)
	{
		nodesDFS[i].node = nodes[i];
		nodesDFS[i].color = White;
		nodesDFS[i].timeBeg = 0;
		nodesDFS[i].timeFin = 0;
		nodesDFS[i].parent = -1;

		OP++;
	}

	TIME = 0;

	for (int i = 0; i < V; i++)
	{
		if (nodesDFS[i].color == White)
		{
			DFS_Visit(nodesDFS, i, &ordered);
		}
		OP++;
	}

	if (demo) {
		printf(" The forest of trees produced by DFS\n\n");

		for (int i = 0; i < V; i++) {
			if (nodesDFS[i].parent == -1)
			{
				prettyPrint(nodesDFS, 1, V, nodesDFS[i]);
				printf("\n\n");
			}
		}

		printf(" DFS traced by timestamps\n\n");

		printByTime(nodesDFS, V);
	}

	if (sort) {
		printf("\n\n Nodes Topologically Sorted: ");

		while (!ordered.empty()) {
			int key = ordered.top();
			printf("%d ", key);
			ordered.pop();
		}

		printf("\n");
	}
}

void VisitTarjan(NodeT* nodesTarjan, int node, int V, stack <int> *s) {
	nodesTarjan[node].color = Grey;
	nodesTarjan[node].id = ID;
	nodesTarjan[node].minid = ID;
	nodesTarjan[node].onStack = true;
	s->push(node);
	ID++;

	NodeL* curr = nodesTarjan[node].node->list;

	while (curr)
	{
		if (nodesTarjan[curr->key->key - 1].color == White) {
			VisitTarjan(nodesTarjan, curr->key->key - 1, V, s);
		}
		
		if (nodesTarjan[curr->key->key - 1].onStack) {
			nodesTarjan[node].minid = min(nodesTarjan[node].minid, nodesTarjan[curr->key->key - 1].minid);
		}

		curr = curr->next;
	}

	nodesTarjan[node].color = Black;
	
	if (nodesTarjan[node].id == nodesTarjan[node].minid)
	{
		while (s->top() != node)
		{
			nodesTarjan[s->top()].minid = nodesTarjan[node].id;
			nodesTarjan[s->top()].onStack = false;
			s->pop();
		}

		nodesTarjan[node].onStack = false;
		s->pop();
	}
}

int* TarjanAlg(NodeG** nodes, int V) {
	NodeT* nodesTarjan = (NodeT*)malloc(V * sizeof(NodeT));
	stack <int> s;

	for (int i = 0; i < V; i++)
	{
		nodesTarjan[i].node = nodes[i];
		nodesTarjan[i].color = White;
		nodesTarjan[i].onStack = false;
		nodesTarjan[i].id = 0;
		nodesTarjan[i].minid = -1;
	}

	ID = 0;

	for (int i = 0; i < V; i++)
	{
		if (nodesTarjan[i].color == White) {
			VisitTarjan(nodesTarjan, i, V, &s);
		}
	}

	int* comp = (int*)malloc(V * sizeof(int));
	for (int i = 0; i < V; i++)
	{
		comp[i] = nodesTarjan[i].minid;
	}

	return comp;
}

void demoDFS() {
	printf("\t\t\t\t\t DEMO DFS\n\n");

	int V = 10;
	int E = 20;
	NodeG** nodes = createGraph(V, E);

	printf(" The graph: \n\n");
	printGraph(nodes, V);
	printf("\n\n");

	TIME = 0;
	DFS(nodes, V, true, false);
	printf("\n\n");

	printf("----------------------------------------DEMO END----------------------------------------------\n\n");
}

void demoTS() {
	printf("\t\t\t\t\t DEMO TOPOLOGICAL SORT\n\n");

	int V = 10;
	int E = 20;
	NodeG** nodes = createGraph(V, E);

	printf(" The graph: \n\n");
	printGraph(nodes, V);
	printf("\n\n");

	TIME = 0;
	DFS(nodes, V, false, true);
	printf("\n\n");

	printf("----------------------------------------DEMO END----------------------------------------------\n\n");
}

void demoTarjan() {
	printf("\t\t\t\t\t DEMO TARJAN\n\n");

	int V = 7;
	int E = 10;
	NodeG** nodes = createGraph(V, E);

	int* comp;

	printf(" The graph: \n\n");
	printGraph(nodes, V);
	printf("\n\n");

	printf(" Stronlgy Connected Components: \n\n");
	comp = TarjanAlg(nodes, V);
	int* flag = (int*)calloc(V, sizeof(int));
	for (int i = 0; i < V; i++)
	{
		int ok = 0;
		for (int j = 0; j < V; j++)
		{
			if (comp[i] == comp[j] && flag[j] == 0)
			{
				flag[i] = flag[j] = 1;
				printf("%d ", j + 1);
				ok = 1;
			}
		}
		if (ok == 1) {
			printf("\n");
		}
	}
	printf("\n\n");

	printf("----------------------------------------DEMO END----------------------------------------------\n\n");
}

void generateChartVarryE() {
	int V = 100;
	int E;
	NodeG** nodes;

	for (E = 1000; E <= 5000; E += 100)
	{
		OP = 0;

		nodes = createGraph(V, E);

		TIME = 0;
		DFS(nodes, V, false, false);

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

		TIME = 0;
		DFS(nodes, V, false, false);

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

	demoDFS();
	demoTS();
	demoTarjan();


	generateCharts();
}
