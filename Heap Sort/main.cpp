#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define TOP_DOWN_AVG "Top-Down aprroach AVG"
#define BOTTOM_UP_AVG "Bottom-Up approach AVG"
#define TOP_DOWN_WORST "Top-Down aprroach WORST"
#define BOTTOM_UP_WORST "Bottom-Up approach WORST"

/*
	Build Heap Bottom UP
	--------------------
		The method is based on the property of complete binary trees which says that in such a tree half of the nodes are leaves. Keeping this in
	  mind we can consider that every leaf is a (trivial) heap. Visualizing the array like this it is intuitive that applying Heapify for the first
	  half of the elements it wil result in a heap which contains all the elements of the array.

		Running Time O(n)
			The running time would seem to be the number of iterations which call Heapify (n/2) times the running time of Heapify (log n), thus
		  O(n/2 * log n) but actually it is better.
			Lets look closely to how the algorithm applies Heapify and how t(n) looks like:
				- On leaves --> it doesn't apply
				- On parents I --> n/2 * O(1) (there are n/2 parents)
				- On parents II --> n/4 * O(2)
				.
				.										   => t(n) = (n/2 * 1) + (n/4 * 2) + ... + (n/2^(H+1) * H)  for each H = 0, log n
				.
				- On parent H --> n/2^(H+1) * O(H)

			Doing the math we obtain t(n) = O(n)

		Best case
			It occurs when Heapify takes always constant time which means no swaps are needed because each element already respects the property
		  of a heap. In other words the array is sorted in descending order ( in case of a max heap, ascending otherwise).

		Worst case
			When we think about the worst case we should look for inputs that would maximize the operations done by each call of the Heapify method.
		  One possibility would be a sorted array in asceding order ( in case of a max heap ) because in this way every new root added to the heap
		  gets sunk all the way down to the leaves floor.

		Usage
			This method is mainly used for sorting given it's low running time.
	------------------------------------------------------------------------------------------------------------------------------------------------

	Build Heap Top Down
	-------------------
		This algorithm uses the function InsertHeap which adds an element to the heap by searching for its place starting from the leaves floor.
	  Considering the first element to be already in the heap it remains to insert the rest n-1 elements one by one.

		Running Time O(n*log n)
			The way this algortihm works is pretty straight forward, for each element from 1 to n apply InsertHeap. InsertHeap in the worst case
		  has a running time of O(log n) and thus repeating it n time it result in running time of the algorithm of O(n*log n).

		Best case
			It is expected when InsertHeap doesn't enter the while loop and thus it takes constant time all the time. This would be possible if
		  the array would already be sorted in descending order (in case of a max heap, ascending otherwise).

		Worst case
			Here we look for maximizing the number of operations done by the InsertHeap method and as for the Bottom Up approach this would be
		  met when the input array is sorted in ascending order thus each new element inserted would get bubbled up all the way up to the root
		  position.

		Usage
			This method is mainly used for building heaps which are priority queues.
	------------------------------------------------------------------------------------------------------------------------------------------------

	Build Heap Bottom Up vs Top Down
	--------------------------------
		Running time
			We can say even from the way they are implemented that Bottom Up is faster than Top Down. This is easily seen in the graphs also.

		Worst case
			This is where the difference in their running time is best visualized. The Top Bottom Up approach does around 20% less operations
		  than the Top Down approach. Even though Heapify and InsertHeap have kind of the same structure for the operations the fact that 
		  Bottom Up aprroach applies Heapify only for half the array does say a lot.
	------------------------------------------------------------------------------------------------------------------------------------------------
*/

Profiler profiler("Demo Average");

int TOP_DOWN_OP, T_TOP_DOWN_OP, BOTTOM_UP_OP, T_BOTTOM_UP_OP;

void initOperations() {
	TOP_DOWN_OP = BOTTOM_UP_OP = 0;
}

void initTOperations() {
	T_TOP_DOWN_OP = T_BOTTOM_UP_OP = 0;
}

void addOperations() {
	T_TOP_DOWN_OP += TOP_DOWN_OP;
	T_BOTTOM_UP_OP += BOTTOM_UP_OP;
}

int* generateArray(int size, bool unique, int sorted) {
	int* a = (int*)malloc(size * sizeof(int));

	FillRandomArray(a, size, 10, 20, unique, sorted);

	return a;
}

int* generateCopy(int* a, int size) {
	int* copy = (int*)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		copy[i] = a[i];
	}

	return copy;
}

void printArray(int* a, int size) {
	for (int i = 0; i < size; i++)
	{
		printf("%d ", a[i]);
	}

	printf("\n");
}

int getParent(int index) {
	if (index % 2 == 0)
	{
		return index / 2 - 1;
	}

	return index / 2;
}

void swap(int* a, int* b) {
	int aux;

	aux = *a;
	*a = *b;
	*b = aux;
}

void heapify(int* a, int size, int root) {
	int right = 2 * root + 2;
	int left = 2 * root + 1;
	int index = root;

	if (left < size && a[index] < a[left])
	{
		index = left;
	}
	BOTTOM_UP_OP++;  // Counts comp

	if (right < size && a[index] < a[right])
	{
		index = right;
	}
	BOTTOM_UP_OP++; // Counts comp

	if (index != root)
	{
		swap(&a[root], &a[index]);
		heapify(a, size, index);

		BOTTOM_UP_OP += 3;  // Counts assig for the swap
	}
}

void insertHeap(int* a, int index) {
	int parentIndex = getParent(index);

	while (parentIndex >= 0 && a[parentIndex] < a[index])
	{
		swap(&a[parentIndex], &a[index]);

		index = parentIndex;

		parentIndex = getParent(index);

		TOP_DOWN_OP += 3; // Counts assig for the swap
		TOP_DOWN_OP++;	  // Counts comp
	}

	if (parentIndex >= 0)
	{
		TOP_DOWN_OP++;    // Counts comp for the while loop end
	}
}

void buildHeapBottomUp(int* a, int size, bool demo) {
	for (int i = size / 2 - 1; i >= 0; i--)
	{
		heapify(a, size, i);

		if (demo)
		{
			printf("The array after %d steps: ", size / 2 - 1 - i); // Used for demo
			printArray(a, size);
			printf("\n");
		}
	}
}

void buildHeapTopDown(int* a, int size, bool demo) {
	for (int i = 1; i < size; i++)
	{
		insertHeap(a, i);

		if (demo)
		{
			printf("The array after %d steps: ", i); // Used for demo
			printArray(a, size);
			printf("\n");
		}
	}
}

void heapSort(int* a, int size, bool demo) {
	int heapSize = size;

	//buildHeapTopDown(a, heapSize, demo);
	buildHeapBottomUp(a, heapSize, demo);

	if (demo)
	{
		printf("The array as a heap: "); // Used for demo 
		printArray(a, size);
		printf("\n");
	}

	for (int i = size - 1; i > 0; i--)
	{
		swap(&a[0], &a[i]);

		heapSize--;

		heapify(a, heapSize, 0);

		if (demo)
		{
			printf("The array after %d steps: ", size - i); // Used for demo
			printArray(a, size);
			printf("\n");
		}
	}
}

void generateChartAverage() {
	int* a;
	int* sample;

	for (int size = 100; size < 10000; size += 100)
	{
		initTOperations();

		for (int j = 0; j < 5; j++) {
			a = generateArray(size, false, 0);

			initOperations();

			sample = generateCopy(a, size);
			buildHeapBottomUp(sample, size, false);
			free(sample);

			sample = generateCopy(a, size);
			buildHeapTopDown(sample, size, false);
			free(sample);

			free(a);

			addOperations();
		}

		profiler.countOperation(TOP_DOWN_AVG, size, T_TOP_DOWN_OP / 5);
		profiler.countOperation(BOTTOM_UP_AVG, size, T_BOTTOM_UP_OP / 5);
	}
}

void generateChartWorst() {
	int* a;
	int* sample;

	for (int size = 100; size < 10000; size += 100)
	{
		initOperations();

		a = generateArray(size, false, 1);

		sample = generateCopy(a, size);
		buildHeapBottomUp(sample, size, false);
		free(sample);

		sample = generateCopy(a, size);
		buildHeapTopDown(sample, size, false);
		free(sample);

		free(a);

		profiler.countOperation(TOP_DOWN_WORST, size, TOP_DOWN_OP);
		profiler.countOperation(BOTTOM_UP_WORST, size, BOTTOM_UP_OP);
	}
}

void generateCharts() {
	generateChartAverage();
	profiler.createGroup("Average Case", TOP_DOWN_AVG, BOTTOM_UP_AVG);

	profiler.reset("Demo Worst");

	generateChartWorst();
	profiler.createGroup("Worst Case", TOP_DOWN_WORST, BOTTOM_UP_WORST);

	profiler.showReport();
}

void demoHeapSort() {
	int* a = (int*)malloc(50 * sizeof(int));

	FillRandomArray(a, 50, 10, 1000, false, 0);

	printf("This is a demo for HeapSort\n\n");

	printf("Initial array: ");
	printArray(a, 50);
	printf("\n");

	heapSort(a, 50, true);

	printf("Final form: ");
	printArray(a, 50);

	printf("------------------------------------------------------------------------------------------------------------------------\n");
}

void demoBottomUp() {
	int* a = (int*)malloc(50 * sizeof(int));

	FillRandomArray(a, 50, 10, 1000, false, 0);

	printf("This is a demo for BuildHeapBottomUp\n\n");

	printf("Initial array: ");
	printArray(a, 50);
	printf("\n");

	buildHeapBottomUp(a, 50, true);

	printf("Final form: ");
	printArray(a, 50);

	printf("------------------------------------------------------------------------------------------------------------------------\n");
}

void demoTopDown() {
	int* a = (int*)malloc(50 * sizeof(int));

	FillRandomArray(a, 50, 10, 1000, false, 0);

	printf("This is a demo for BuildHeapTopDown\n\n");

	printf("Initial array: ");
	printArray(a, 50);
	printf("\n");

	buildHeapTopDown(a, 50, true);

	printf("Final form: ");
	printArray(a, 50);

	printf("------------------------------------------------------------------------------------------------------------------------\n");
}

int main() {
	demoHeapSort();
	demoBottomUp();
	demoTopDown();

	generateCharts();
}
