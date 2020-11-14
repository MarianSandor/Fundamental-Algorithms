#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Profiler.h"

#define HS_AVG "HeapSort Average"
#define QS_AVG "QuickSort Average"

#define QS_WORST_ASC "QuickSort Worst Ascending"
#define QS_WORST_DESC "QuickSort Worst Descending"
#define QS_BEST "QuickSort Best"

/*
	QuickSort
	----------
		It is one of the best sorting algorithms being the best choice in most of the cases because of its very good running time
	  in practice. The method is based on the partitioning algorithm which "sorts" the given array such the first k elements are
	  less than element k and the rest of n - k elements are greater. Intuivetly applying this algorithm enough times we and up
	  with an array in which each element is placed such that all the elements to the left are less than it and all the elements
	  to the right are greater, in other words a completely sorted array.

		Running time
			The running time is given mostly by the pivot choice of element around which the partitioning is done. Choosing the 
		  median would result in the best performence but that is not possible ( without using a specialized algorithm for selecting
		  the ith element in an array ). The running time of the default algorithm is O(n*log n) in case of a random sorted array, but 
		  when talking about a sorted or almost-sorted array the algorithm runs in O(n^2) which is just bad. Though in case of a random
		  pivot the algorithm turns out to run in about O(n*log n) no matter the case.

		Stability
			The default algorithm is not stable because of the way partitioning swaps the elements.
----------------------------------------------------------------------------------------------------------------------------------------

	HeapSort
	---------
		This sorting algorithm takes advantage of the heap structure and mostly of the property that max-heaps implies among its
	  elements. The first step si to build a heap out of the initial array then one by one take the root elements from the heap 
	  and move them to the end of the array with the restriction that the heap property is maintained among the elemets left in it.
	  The end is reached when the heap size is just 1 and the result is an asceding sorted array.

		Running time
			The main method used by the algorithm is Heapify which has a complexity of O(log n). The building of the heap si done 
		  in a bottom-up approach and thus it runs in O(n). When the rearranging is done it iterates the whole array and applies 
		  Heapify at each step, this results in O(n*log n) time complexity. Summing everything up the final running time is O(n*log n)
		  no matter the case.

		Stability
			The algorithm is not stable
----------------------------------------------------------------------------------------------------------------------------------------

	HeapSort Best vs Worst
	-----------------------
		!This analysis is done on the standard implementation where the pivot is chosen to be the rightmost element!

	  BEST -> The best case appears when the data is sorted in a random order. This would be the average case for most of the other 
		     sorting algorithms. This is also the case in which quicksort takes the lead compared to heapsort or mergesort even though
			 all three have the time complexity of O(n*log n).
	  
	  WORST -> The worst case appears when the data is sorted ascending or descending. Here the time complexity of quicksort raises all
	          the way up to O(n^2). If the array is sorted ascending is does even much more operations than when it's descending. This
			  being the worst puts quicksort in very bad position compared to its competitors because in many cases sorting is done on 
			  almost-sorted arrays and O(n^2) is no near O(n*log n) offered by heapsort or mergesort.
*/

int DEMO_SIZE; 
int HS_OP, T_HS_OP, T_QS_OP, QS_OP;

Profiler profiler("Demo Heap & Quick");

void initOp() {
	HS_OP = QS_OP = 0;
}

void initTOp() {
	T_HS_OP = T_QS_OP = 0;
}

void addOp() {
	T_HS_OP += HS_OP;
	T_QS_OP += QS_OP;
}

void swap(int* a, int* b) {
	int aux;

	aux = *a;
	*a = *b;
	*b = aux;
}

int* generateArray(int size, bool unique, int sorted) {
	int* a = (int*)malloc(size * sizeof(int));

	FillRandomArray(a, size, 10, 50000, unique, sorted);

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

void insertionSort(int* a, int l, int r) {

	for (int i = l; i <= r; i++)
	{
		int key = a[i];
		QS_OP++;
		int j = i - 1;

		while (j >= l && a[j] > key) {
			QS_OP++;
			swap(&a[j + 1], &a[j]);
			QS_OP += 3;
			j--;
		}
		QS_OP++;
		a[j + 1] = key;
	}
}

int partition(int* a, int l, int r) {
	int piv = a[r];
	QS_OP++;
	int i = l - 1;

	for (int j = l; j <= r - 1; j++)
	{
		if (a[j] < piv)
		{
			i++;
			swap(&a[i], &a[j]);
			QS_OP += 3;
		}
		QS_OP++;
	}

	swap(&a[i + 1], &a[r]);
	QS_OP += 3;

	return i + 1;
}

int randomPartition(int* a, int l, int r) {
	srand(time(0));
	int random = l + rand() % (r - l + 1);

	swap(&a[r], &a[random]);
	QS_OP += 3;

	return partition(a, l, r);
}

void quickSort(int* a, int l, int r, bool demo) {
	if (r >= l)
	{
		if (r - l + 1 <= 5) {
			insertionSort(a, l, r);

			if (demo)
			{
				printf("The array after sorting l = %d and r = %d: ",l ,r); // Used for demo
				printArray(a, DEMO_SIZE);
				printf("\n");
			}
		}
		else {
		//	int p = randomPartition(a, l, r);
			int p = partition(a, l, r);

			if (demo)
			{
				printf("The array after partition p = %d and element = %d: ", p, a[p]); // Used for demo
				printArray(a, DEMO_SIZE);
				printf("\n");
			}

			quickSort(a, l, p - 1, demo);
			quickSort(a, p + 1, r, demo);
		}
	}
}

void heapify(int* a, int size, int root) {
	int right = 2 * root + 2;
	int left = 2 * root + 1;
	int index = root;

	if (left < size && a[index] < a[left])
	{
		index = left;
	}
	HS_OP++;

	if (right < size && a[index] < a[right])
	{
		index = right;
	}
	HS_OP++;

	if (index != root)
	{
		swap(&a[root], &a[index]);
		HS_OP += 3;
		heapify(a, size, index);
	}
}

void buildHeapBottomUp(int* a, int size, bool demo) {
	for (int i = size / 2 - 1; i >= 0; i--)
	{
		heapify(a, size, i);
	}
}

void heapSort(int* a, int size, bool demo) {
	int heapSize = size;

	buildHeapBottomUp(a, heapSize, demo);

	for (int i = size - 1; i > 0; i--)
	{
		swap(&a[0], &a[i]);
		HS_OP += 3;

		heapSize--;

		heapify(a, heapSize, 0);
	}
}

int quickSelect(int* a, int l, int r, int index, bool demo) {
	if (r == l)
	{
		return a[l];
	}
	else
	{
		int q = randomPartition(a, l, r);

		if (demo)
		{
			printf("The array after partition q = %d and element = %d: ", q, a[q]); // Used for demo
			printArray(a + l, r + 1);
			printf("\n");
		}

		if (q == index)
		{
			return a[q];
		}

		if (q < index)
		{
			quickSelect(a, q + 1, r, index, demo);
		}
		else
		{
			quickSelect(a, l, q - 1, index, demo);
		}
	}
}

void demoQuickSort() {
	DEMO_SIZE = 50;
	int* a = (int*)malloc(DEMO_SIZE * sizeof(int));

	FillRandomArray(a, DEMO_SIZE, 10, 1000, false, 0);

	printf("This is a demo for QuickSort\n\n");

	printf("Initial array: ");
	printArray(a, DEMO_SIZE);
	printf("\n");

	quickSort(a, 0, DEMO_SIZE - 1, true);

	printf("Final form: ");
	printArray(a, DEMO_SIZE);

	printf("------------------------------------------------------------------------------------------------------------------------\n");
}

void demoQuickSelect() {
	DEMO_SIZE = 50;
	int* a = (int*)malloc(DEMO_SIZE * sizeof(int));
	
	srand(time(0));
	int randomIndex = rand() % DEMO_SIZE;

	FillRandomArray(a, DEMO_SIZE, 10, 1000, false, 0);
	int* copy = generateCopy(a, DEMO_SIZE);

	printf("This is a demo for QuickSelect\n\n");

	printf("Initial array: ");
	printArray(a, DEMO_SIZE);
	printf("\n");

	quickSort(copy, 0, DEMO_SIZE - 1, false);

	printf("Initial array sorted: ");
	printArray(copy, DEMO_SIZE);
	printf("\n");

	int element = quickSelect(a, 0, DEMO_SIZE - 1, randomIndex, true);

	printf("The %dth element is: %d\n",randomIndex, element);

	printf("------------------------------------------------------------------------------------------------------------------------\n");
}

void generateChartAverage() {
	int* a;
	int* sample;

	for (int size = 100; size < 10000; size += 100)
	{
		initTOp();

		for (int j = 0; j < 5; j++) {
			a = generateArray(size, false, 0);

			initOp();

			sample = generateCopy(a, size);
			heapSort(sample, size, false);
			free(sample);

			sample = generateCopy(a, size);
			quickSort(sample, 0, size - 1, false);
			free(sample);

			free(a);

			addOp();
		}

		profiler.countOperation(HS_AVG, size, T_HS_OP / 5);
		profiler.countOperation(QS_AVG, size, T_QS_OP / 5);
	}
}

void generateChartBest() {
	int* a;

	for (int size = 100; size < 10000; size += 100)
	{
		initOp();

		a = generateArray(size, false, 0);
		quickSort(a, 0, size - 1, false);
		free(a);

		profiler.countOperation(QS_BEST, size, QS_OP);
	}
}

void generateChartWorst() {
	int* a;

	for (int size = 100; size < 10000; size += 100)
	{
		initOp();

		a = generateArray(size, false, 1);
		quickSort(a, 0, size - 1, false);
		free(a);

		profiler.countOperation(QS_WORST_ASC, size, QS_OP);
	}

	for (int size = 100; size < 10000; size += 100)
	{
		initOp();

		a = generateArray(size, false, 2);
		quickSort(a, 0, size - 1, false);
		free(a);

		profiler.countOperation(QS_WORST_DESC, size, QS_OP);
	}
}

void generateCharts() {
	generateChartAverage();
	profiler.createGroup("Average Case QuickSort HeapSort", HS_AVG, QS_AVG);

	profiler.reset("Demo Quick");

	generateChartBest();
	generateChartWorst();

	profiler.createGroup("Worst And Best Case QuickSort", QS_WORST_ASC, QS_WORST_DESC);

	profiler.showReport();
}

void main() {
	demoQuickSort();
	demoQuickSelect();

	generateCharts();
}
