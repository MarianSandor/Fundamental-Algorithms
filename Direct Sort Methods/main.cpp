#include <iostream>
#include <conio.h>	
#include <stdio.h>	

#include "Profiler.h"	

using namespace std;

Profiler profiler("Demo");

/**
	Bubble Sort
	-----------
		It sorts the array iterating it two times and doing swaps between the elements. The efficiency can be
	  improved by setting the outer to go through only the first n - 1 elements and the inside loop through the
	  first n - i - 1. A boolean variable could be added so that when the array is fully sorted the algorithm 
	  is forced to stop.

		Complexity
			- Average case: ~O(n^2)
			- Worst case: O(n^2)
			- Best case: O(n^2) if there is no flag to check for swaps O(n) otherwise

		Stability
			The algorithm is stable because the elements are moved only one position to the left one at a time and
		  the condition implies that equal elements are not swapped.

		Auxiliary space - O(1)
--------------------------------------------------------------------------------------------------------------------
	
	Insertion Sort
	--------------
		It sorts the array by visualizing it as composed from two parts a sorted one and an unsorted one from 
	  from which the elements are taken. As an element is moved from the unsorted part to the sorted one and placed
	  in its specific position the sorted elements are shifted to the right. In this way at each step the sorted 
	  part increases by one and the unsorted part decreases by one until there is no unsorted part anymore. In order
	  to improve the efficiency of this algorithm the iteration of the elements could start with the second element
	  thus the first element is consider to be in the sorted part already. Also the position of an element in the 
	  sorted part could be find using binary search.

		Complexity
			- Average case: ~O(n^2)
			- Worst case: O(n^2)
			- Best case: O(n)

		Stability
			The algorithm is stable because the elements form the unsorted part are examined one by one form left
		  to right thus the duplicates are taken in asceding order (left - right) and also the comparison is done 
		  strictly and not "greater or equal" in which case the stability would've been lost.

		Auxiliary space - O(1)
--------------------------------------------------------------------------------------------------------------------

	Selction Sort
	-------------
		It is similar to Insertion Sort having the array divided in two parts (the sorted part - the first i elements
	  and the unsorted part - from i + 1). It works by selecting the minimum element from the unsorted part and adding 
	  it to the end of the sorted part (the sorted part gets extended and the unsorted part is minimized). Repeating 
	  this process results in the sorted part representing the whole array. Efficiency can be improved by setting the 
	  outer loop to go only through n - 1 elements (the last element remains on it's desired position).

		Complexity
			- Average case: ~O(n^2)
			- Worst case: O(n^2)
			- Best case: O(n)

		Stability
			The algorithm is not stable because swaps can alterate the order of the duplicates in specific cases such as
			3' 4 3" 1 (the first 3' will be swapped with 1 which will result in the new order of the array 1 4 3" 3'). To
			make the algorithm stable swaps should be replaced with shifts but this will drastically reduce the efficiency.
				
		Auxiliary space - O(1)
--------------------------------------------------------------------------------------------------------------------------

		Conclusions
		-----------
			Average case: All three of the algorithms have the same complexity in this case ( O(n^2) ) and from the analysis
						  we can say that Insertion Sort and Selection Sort act pretty much the same in terms of operations 
						  done. Bubble Sort is a bit less efficient doing about twice the operations of the Insertion/Selection
						  Sort.
			Worst case:   All three algorithm have the same time complexity ( O(n^2) ) and in terms of operations done the best
						  is Selection Sort and the worst is Bubble Sort.
			Best case:    Selection and Insertion Sort have the same time complexity of O(n) which is better than Bubble Sort 
						  which has O(n^2). When it comes to operations done, Insertion Sort clearly comes first with the least 
						  operations done, far from the other two algorithms which are very close to each other. Though it is 
						  important to mention that adding the flag contor to Bubble Sort algorithm it seriously improves its
						  efficiency in the best case matching Insertion Sort in terms of operations done.
*/

#define AVG_BUB_A "Average BubbleSort Assignments"
#define AVG_BUB_C "Average BubbleSort Comparisons"
#define AVG_BUB "Average BubbleSort"

#define AVG_INS_A "Average InsertionSort Assignments"
#define AVG_INS_C "Average InsertionSort Comparisons"
#define AVG_INS "Average InsertionSort"

#define AVG_SEL_A "Average SelectionSort Assignments"
#define AVG_SEL_C "Average SelectionSort Comparisons"
#define AVG_SEL "Average SelectionSort"

#define BEST_BUB_A "Best BubbleSort Assignments"
#define BEST_BUB_C "Best BubbleSort Comparisons"
#define BEST_BUB "Best BubbleSort"

#define BEST_INS_A "Best InsertionSort Assignments"
#define BEST_INS_C "Best InsertionSort Comparisons"
#define BEST_INS "Best InsertionSort"

#define BEST_SEL_A "Best SelectionSort Assignments"
#define BEST_SEL_C "Best SelectionSort Comparisons"
#define BEST_SEL "Best SelectionSort"

#define WORST_BUB_A "Worst BubbleSort Assignments"
#define WORST_BUB_C "Worst BubbleSort Comparisons"
#define WORST_BUB "Worst BubbleSort"

#define WORST_INS_A "Worst InsertionSort Assignments"
#define WORST_INS_C "Worst InsertionSort Comparisons"
#define WORST_INS "Worst InsertionSort"

#define WORST_SEL_A "Worst SelectionSort Assignments"
#define WORST_SEL_C "Worst SelectionSort Comparisons"
#define WORST_SEL "Worst SelectionSort"

int BUB_A, BUB_C, INS_A, INS_C, SEL_A, SEL_C;			
int T_BUB_A, T_BUB_C, T_INS_A, T_INS_C, T_SEL_A, T_SEL_C;	//used to compute the average case

void initAssigComp() {
	BUB_A = BUB_C = INS_A = INS_C = SEL_A = SEL_C = 0;
}

void initTotalAssigComp() {
	T_BUB_A = T_BUB_C = T_INS_A = T_INS_C = T_SEL_A = T_SEL_C = 0;
}

void addAssigComp() {
	T_BUB_A += BUB_A;
	T_BUB_C += BUB_C;
	T_INS_A += INS_A;
	T_INS_C += INS_C;
	T_SEL_A += SEL_A;
	T_SEL_C += SEL_C;
}

int* generateCopyArray(int* src, int size) {
	int* copy = (int*)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++)
	{
		copy[i] = src[i];
	}

	return copy;
}

int* generateArray(int n, int sorted) {
	int* a = (int*)malloc(n * sizeof(int));

	FillRandomArray(a, n, 10, 50000, false, sorted);

	return a;
}

void printArray(int* a, int n) {
	for (int i = 0; i < n; i++)
	{
		std::cout << a[i] << " ";
	}
	std::cout << endl;
}

void swap(int* a, int* b) {
	int aux;

	aux = *a;
	*a = *b;
	*b = aux;
}

void bubbleSort(int* a, int n) {

	for (int i = 0; i < n - 1; i++)
	{
		//bool swapped = false; // used to stop the algorithm if the array is already sorted

		for (int j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				swap(&a[j], &a[j + 1]);
				//swapped = true;
				BUB_A += 3;
			}

			BUB_C++;
		}

	//	if (!swapped) {
	//		break;
	//	}
	}
}

void insertionSort(int* a, int n) {
	int buf;
	int j;

	for (int i = 1; i < n; i++)
	{
		j = i - 1;
		buf = a[i];
		INS_A++;

		while (j >= 0 && a[j] > buf)
		{
			INS_C++;
			a[j + 1] = a[j];
			INS_A++;
			j--;
		}

		a[j + 1] = buf;
		INS_A++;
	}
}

void selectionSort(int* a, int n) {
	int index;

	for (int i = 0; i < n - 2; i++)
	{
		index = i;

		for (int j = i + 1; j < n; j++)
		{
			if (a[index] > a[j])
				index = j;
			SEL_C++;
		}

		swap(&a[i], &a[index]);
		SEL_A += 3;
	}
}

void demoBubble() {
	std::cout << "Demo bubble" << endl;

	int a[5] = { 3,7,11,2,12 };

	printArray(a, 5);
	bubbleSort(a, 5);
	printArray(a, 5);
}

void demoInsertion() {
	std::cout << "Demo insertion" << endl;

	int a[5] = { 3,7,11,2,1 };

	printArray(a, 5);
	insertionSort(a, 5);
	printArray(a, 5);
}

void demoSelection() {
	std::cout << "Demo selection" << endl;

	int a[5] = { 3,7,11,2,1 };

	printArray(a, 5);
	selectionSort(a, 5);
	printArray(a, 5);
}

void createChartAverage() {
	int* array;
	int* sample;

	for (int size = 100; size <= 10000; size += 100)
	{
		initTotalAssigComp();

		for (int i = 0; i < 5; i++)
		{
			sample = generateArray(size, 0);
			initAssigComp();

			array = generateCopyArray(sample, size);
			bubbleSort(array, size);
			free(array);

			array = generateCopyArray(sample, size);
			insertionSort(array, size);
			free(array);

			array = generateCopyArray(sample, size);
			selectionSort(array, size);
			free(array);

			addAssigComp();

			free(sample);
		}

		profiler.countOperation(AVG_BUB_A, size, T_BUB_A / 5);
		profiler.countOperation(AVG_BUB_C, size, T_BUB_C / 5);
		profiler.countOperation(AVG_INS_A, size, T_INS_A / 5);
		profiler.countOperation(AVG_INS_C, size, T_INS_C / 5);
		profiler.countOperation(AVG_SEL_A, size, T_SEL_A / 5);
		profiler.countOperation(AVG_SEL_C, size, T_SEL_C / 5);
	}

	profiler.addSeries(AVG_BUB, AVG_BUB_A, AVG_BUB_C);
	profiler.addSeries(AVG_INS, AVG_INS_A, AVG_INS_C);
	profiler.addSeries(AVG_SEL, AVG_SEL_A, AVG_SEL_C);
}

void createChartBest() {
	int* sample;

	for (int size = 100; size <= 10000; size += 100)
	{
		sample = generateArray(size, 1);
		initAssigComp();

		bubbleSort(sample, size);
		insertionSort(sample, size);
		selectionSort(sample, size);

		free(sample);

		profiler.countOperation(BEST_BUB_A, size, BUB_A);
		profiler.countOperation(BEST_BUB_C, size, BUB_C);
		profiler.countOperation(BEST_INS_A, size, INS_A);
		profiler.countOperation(BEST_INS_C, size, INS_C);
		profiler.countOperation(BEST_SEL_A, size, SEL_A);
		profiler.countOperation(BEST_SEL_C, size, SEL_C);
	}

	profiler.addSeries(BEST_BUB, BEST_BUB_A, BEST_BUB_C);
	profiler.addSeries(BEST_INS, BEST_INS_A, BEST_INS_C);
	profiler.addSeries(BEST_SEL, BEST_SEL_A, BEST_SEL_C);
}

void createChartWorst() {
	int* array;
	int* sample;

	for (int size = 100; size <= 10000; size += 100)
	{
			sample = generateArray(size, 2);
			initAssigComp();

			array = generateCopyArray(sample, size);
			bubbleSort(array, size);
			free(array);

			array = generateCopyArray(sample, size);
			insertionSort(array, size);
			free(array);

			array = generateCopyArray(sample, size);
			selectionSort(array, size);
			free(array);

			addAssigComp(); // adds the assig and comp to the total assg and comp 

			free(sample);

			profiler.countOperation(WORST_BUB_A, size, BUB_A);
			profiler.countOperation(WORST_BUB_C, size, BUB_C);
			profiler.countOperation(WORST_INS_A, size, INS_A);
			profiler.countOperation(WORST_INS_C, size, INS_C);
			profiler.countOperation(WORST_SEL_A, size, SEL_A);
			profiler.countOperation(WORST_SEL_C, size, SEL_C);
	}

	profiler.addSeries(WORST_BUB, WORST_BUB_A, WORST_BUB_C);
	profiler.addSeries(WORST_INS, WORST_INS_A, WORST_INS_C);
	profiler.addSeries(WORST_SEL, WORST_SEL_A, WORST_SEL_C);
}

void createCharts() {
	
	createChartAverage();
	createChartBest();
	createChartWorst();

	profiler.createGroup("Best Case Operations", BEST_BUB, BEST_SEL, BEST_INS);
	profiler.createGroup("Best Case Assignments", BEST_BUB_A, BEST_SEL_A, BEST_INS_A);
	profiler.createGroup("Best Case Comparisons", BEST_BUB_C, BEST_SEL_C, BEST_INS_C);

	profiler.createGroup("Average Case Operations", AVG_BUB, AVG_SEL, AVG_INS);
	profiler.createGroup("Average Case Assignments", AVG_BUB_A, AVG_SEL_A, AVG_INS_A);
	profiler.createGroup("Average Case Comparisons", AVG_BUB_C, AVG_SEL_C, AVG_INS_C);

	profiler.createGroup("Worst Case Operations", WORST_BUB, WORST_SEL, WORST_INS);
	profiler.createGroup("Worst Case Assignments", WORST_BUB_A, WORST_SEL_A, WORST_INS_A);
	profiler.createGroup("Worst Case Comparisons", WORST_BUB_C, WORST_SEL_C, WORST_INS_C);
}

int main() {
	demoBubble();
	demoInsertion();
	demoSelection();

	//createChartBest();
	//createChartAverage();
	//createChartWorst();

	createCharts();

	profiler.showReport();

	return 0;
}