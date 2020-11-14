#include<stdio.h>
#include<stdlib.h>
#include "List.h"
#include "Profiler.h"

#define VARY_N_K1 "Vary n with fixed value k = 5"
#define VARY_N_K2 "Vary n with fixed value k = 10"
#define VARY_N_K3 "Vary n with fixed value k = 100"

#define VARY_K "Vary k with fixed value n = 10000"

/*
	Merge K Sorted Lists
   ----------------------
		
		As the name suggests, the algorithm combines K sorted lists with a total of N elements in just one list
	  which includes all the N elements in ascending order. What makes the algorithm so good is the way it takes 
	  use of the MinHeap structure in order to solve the problem. The K lists have their head node stored in an 
	  array of length K. The algorithm builds a MinHeap from this array ( the keys being the weights of the first 
	  nodes from each list ) and always extracts the root node and inserts it into the destination list. After each
	  extraction the MinHeap property is maintained applying Heapify. When one list runs out of nodes the HeapSize 
	  is decreased and the algorithm continues until the HeapSize is 0.

		Running Time
		------------
			Analyzing the algorithm we observe:

				- Build Heap --> O(K)
				- Heapify --> O(log K)
				
			The inner loop is repeated N times (until all nodes were evaluated) and each time it apllies Heapify
				-->O(N * log K)

			Summing all up:
			 
				Buld Heap + Inner Loop = O(log K) + O(N * log K) 

			==> Final running time is --O(N*log K)--

		Chart Analysis
		--------------
			Knowing that the time complexity is O(N*log K) where N is the total number of elements and K is the number
		  of list in which the N elements are stored it is quite intuitive what would happen when one of the value is 
		  fixed.
			
			-- Fixing K and varying N would result in a complexity like O(N*c) where c is just a constant ( K being fixed
			 means that log K will take constant value) and thus the graph will ilustrate a linear function.

			-- When N is fix and K will vary the complexity will transform in O(c*log K) where c is just a constant ( N 
			 being fixed means it is constant value) and the resulted graph should look similar to a logarithmic function.
			
			Ofcourse increasing either of them (N , K) the number of operation will increase too it is important to say 
		  that changes on the number of elements (the N parameter) will have far greater impact on the time complexity 
		  than changes on the number of the lists (the K parameter). This is because the logarithm function grows 
		  significantly slower than the liniar function.
*/

Profiler profiler("Merge K lists");

int LEFT, RIGHT; // Represent the limits for the numbers when generating the arrays
				// ! Set them accordingly before use !

int OP;

void swapList(ListH** l, int indexA, int indexB) {
	ListH* aux;

	aux = l[indexA];
	l[indexA] = l[indexB];
	l[indexB] = aux;
}

int* generateArray(int size, int sort, bool unique) {
	int* a = (int*)malloc(size * sizeof(int));

	FillRandomArray(a, size, LEFT, RIGHT, unique, sort);

	return a;
}

void insertArrayList(ListH* l, int* a, int size) {
	for (int i = 0; i < size; i++)
	{
		push(l, a[i]);
	}
}

ListH** createLists(int k, int n) {
	ListH** l = createArrayList(k);
	int* a;

	for (int i = 0; i < k; i++)
	{
		if (i < n % k) {
			a = generateArray(n / k + 1, 1, false);

			insertArrayList(l[i], a, n / k + 1);
		}
		else
		{
			a = generateArray(n / k, 1, false);

			insertArrayList(l[i], a, n / k);
		}

		free(a);
	}

	return l;
}

void heapifyList(ListH** l, int root, int size) {
	int left = 2 * root + 1;
	int right = 2 * root + 2;
	int index = root;

	if (right < size && l[index]->first->key > l[right]->first->key)
	{
		index = right;
	}
	OP++;

	if (left < size && l[index]->first->key > l[left]->first->key)
	{
		index = left;
	}
	OP++;


	if (index != root)
	{
		swapList(l, root, index);
		OP += 3;
		heapifyList(l, index, size);
	}
}

void buildHeapList(ListH** l, int size) {
	for (int i = size / 2 - 1; i >= 0; i--)
	{
		heapifyList(l, i, size);
	}
}

void mergeLists(ListH** lists, int size, ListH* L, bool demo) {
	int heapSize = size;

	buildHeapList(lists, size);

	if (demo)
	{
		printf("Lists after the heap was created: \n");
		for (int i = 0; i < size; i++)
		{
			printList(lists[i]);
		}
		printf("\n\n");
	}


	while (heapSize > 0) {
		int key = pop(lists[0]);
		push(L, key);

		if (isEmpty(lists[0]))
		{
			swapList(lists, 0, heapSize - 1);
			OP += 3;
			heapSize--;
		}

		heapifyList(lists, 0, heapSize);

		if (demo)
		{
			printf("Lists after heapify was applied:\n");
			for (int i = 0; i < heapSize; i++)
			{
				printList(lists[i]);
			}
			printf("\n");
		}
	}
}

void demo() {
	ListH** lists;
	ListH* L;
	
	int k = 4;
	int n = 20;

	LEFT = 10;
	RIGHT = 99;

	lists = createLists(k, n);
	L = createList();

	printf("Initial lists: \n");
	for (int i = 0; i < k; i++)
	{
		printList(lists[i]);
	}
	printf("\n\n");

	mergeLists(lists, k, L, true);

	printf("The resulting merged list: \n");
	printList(L);

	freeArrayList(lists, k);
	freeList(L);
}

void generateChartVaryN1() {
	int k = 5;

	ListH** lists;
	ListH* L = createList();

	for (int size = 100; size < 10000; size += 100)
	{
		OP = 0;

		lists = createLists(k, size);
		mergeLists(lists, k, L, false);
		freeArrayList(lists, k);

		profiler.countOperation(VARY_N_K1, size, OP);
	}

	freeList(L);
}

void generateChartVaryN2() {
	int k = 10;

	ListH** lists;
	ListH* L = createList();

	for (int size = 100; size < 10000; size += 100)
	{
		OP = 0;

		lists = createLists(k, size);
		mergeLists(lists, k, L, false);
		freeArrayList(lists, k);

		profiler.countOperation(VARY_N_K2, size, OP);
	}

	freeList(L);
}

void generateChartVaryN3() {
	int k = 100;

	ListH** lists;
	ListH* L = createList();

	for (int size = 100; size < 10000; size += 100)
	{
		OP = 0;

		lists = createLists(k, size);
		mergeLists(lists, k, L, false);
		freeArrayList(lists, k);

		profiler.countOperation(VARY_N_K3, size, OP);
	}

	freeList(L);
}

void generateChartVaryK() {
	int size = 10000;

	ListH** lists;
	ListH* L = createList();

	for (int k = 10; k < 500; k += 10)
	{
		OP = 0;

		lists = createLists(k, size);
		mergeLists(lists, k, L, false);
		freeArrayList(lists, k);

		profiler.countOperation(VARY_K, k, OP);
	}

	freeList(L);
}

void generateCharts() {
	LEFT = 10;
	RIGHT = 50000;

	generateChartVaryN1();
	generateChartVaryN2();
	generateChartVaryN3();

	profiler.createGroup("K fixed N varies", VARY_N_K1, VARY_N_K2, VARY_N_K3);

	generateChartVaryK();

	profiler.createGroup("N fixed K varies", VARY_K);

	profiler.showReport();
}

int main() {
	demo();

	generateCharts();
}