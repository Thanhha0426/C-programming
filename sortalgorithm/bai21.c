
// C++ program for implementation of Heap Sort
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAXSIZE 5000000

void swap(int *x, int *y)
{
	int c = *x;
	*x = *y;
	*y = c;
}
    //=====================================================
// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
void heapify(int arr[], int n, int i)
{
	int largest = i; // Initialize largest as root
	int l = 2 * i + 1; // left = 2*i + 1
	int r = 2 * i + 2; // right = 2*i + 2

					   // If left child is larger than root
	if (l < n && arr[l] > arr[largest])
		largest = l;

	// If right child is larger than largest so far
	if (r < n && arr[r] > arr[largest])
		largest = r;

	// If largest is not root
	if (largest != i)
	{
		swap(&arr[i], &arr[largest]);

		// Recursively heapify the affected sub-tree
		heapify(arr, n, largest);
	}
}
    //=====================================================
// main function to do heap sort
void heapSort(int arr[], int n)
{
	// Build heap (rearrange array)
	int i;
	for (i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// One by one extract an element from heap
	for (i = n - 1; i >= 0; i--)
	{
		// Move current root to end
		swap(&arr[0], &arr[i]);

		// call max heapify on the reduced heap
		heapify(arr, i, 0);
	}
}
    //=====================================================
/* A utility function to print array of size n */
void printArray(int arr[], int n)
{
    int i;
	for (i = 0; i<n; ++i)
		printf("%d ", arr[i]);
	printf("\n");
}
//=========================================================
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    int *L = (int*)malloc(n1*sizeof(int));
    int * R= (int*)malloc(n2*sizeof(int));

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
    //=====================================================
void mergeSort(int arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}
//=========================================================
void quicksort(int *A, int first, int last)
{
    int i,j,pivot;
    if(first < last){
        pivot = last;
        j = last;
        i = first;
        while (i<j){
        while (A[j] >= A[pivot] && j> first) j--;
        while (A[i] < A[pivot]) i++;
        if(i<j){
            swap(&A[i], &A[j]);
        }
        }
        swap(&A[pivot], &A[i]);
        quicksort(A, first, i-1);
        quicksort(A, i+1, last);
    }
}
//=========================================================
// Driver program
int main()
{
    srand(time(NULL));
    int i;

    int *arr = (int*)malloc(MAXSIZE*sizeof(int));
	//int n = sizeof(arr) / sizeof(arr[0]);
    for ( i = 0; i< MAXSIZE; i++)
        arr[i]= rand()%1000;
    clock_t start2 = clock();
	heapSort(arr, MAXSIZE);
	clock_t end2 = clock();
	//printArray(arr, n);
	printf("Time heapSort: %f\n", (double)(end2-start2)/CLOCKS_PER_SEC);

    int *DATA1 = (int *) malloc(MAXSIZE*sizeof(int));
	for(i=0; i<MAXSIZE; i++)
		DATA1[i] = rand()%1000;
    clock_t begin1=clock();
	quicksort(DATA1,0,MAXSIZE);
	clock_t end1=clock();
	printf("Time Quick Sort: %f\n", (double) (end1-begin1) / CLOCKS_PER_SEC);

    //=====================================================*/
    int *DATA3 = (int *) malloc(MAXSIZE*sizeof(int));
	for(i=0; i<MAXSIZE; i++)
		DATA3[i] = rand()%MAXSIZE;

    clock_t start3 = clock();
    mergeSort(DATA3,0,MAXSIZE-1);
    clock_t end3 = clock();
    printf("Time mergeSort: %f\n", (double)(end3-start3)/CLOCKS_PER_SEC);


	return 0;
}
