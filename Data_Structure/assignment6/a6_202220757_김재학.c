#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10005

void insertionSort(int a[]);
void insert(int e, int a[], int i);

void bubbleSort(int a[]);

void selectionSort(int a[]);

void quicksort(int a[], int left, int right);

int sorted[SIZE];
void mergeSort(int a[], int left, int right);
void merge(int mergeList[], int i, int m, int n);

void heapsort(int list[]);
void adjust(int a[], int root, int n);

int a[SIZE];	//random array
int b[SIZE];	//LOO 10%
int c[SIZE];	//sorted array
int d[SIZE];	//reverse sorted array

void initArray();
void initArray_a();

int main() {
	clock_t start, end;
	srand(time(NULL));

	double aclock = 0;	//a어레이의 평균 시간을 구하기 위한 시간 총합

	printf("====InsertionSort====\n");
	initArray();
	for (int i = 0; i < 10; i++) {
		initArray_a();
		start = clock();
		insertionSort(a);
		end = clock();
		aclock += (double)end - start;
	}
	printf("a_average: %f\n", aclock / 10);
	aclock = 0;

	start = clock();
	insertionSort(b);
	end = clock();
	printf("b: %f\n", (double)end - start);
	start = clock();
	insertionSort(c);
	end = clock();
	printf("c: %f\n", (double)end - start);
	start = clock();
	insertionSort(d);
	end = clock();
	printf("d: %f\n", (double)end - start);


	printf("====BubbleSort====\n");
	initArray();
	for (int i = 0; i < 10; i++) {
		initArray_a();
		start = clock();
		bubbleSort(a);
		end = clock();
		aclock += (double)end - start;
	}
	printf("a_average: %f\n", aclock / 10);
	aclock = 0;
	start = clock();
	bubbleSort(b);
	end = clock();
	printf("b: %f\n", (double)end - start);
	start = clock();
	bubbleSort(c);
	end = clock();
	printf("c: %f\n", (double)end - start);
	start = clock();
	bubbleSort(d);
	end = clock();
	printf("d: %f\n", (double)end - start);



	printf("====SelectionSort====\n");
	initArray();
	for (int i = 0; i < 10; i++) {
		initArray_a();
		start = clock();
		selectionSort(a);
		end = clock();
		aclock += (double)end - start;
	}
	printf("a_average: %f\n", aclock / 10);
	aclock = 0;
	start = clock();
	selectionSort(b);
	end = clock();
	printf("b: %f\n", (double)end - start);
	start = clock();
	selectionSort(c);
	end = clock();
	printf("c: %f\n", (double)end - start);
	start = clock();
	selectionSort(d);
	end = clock();
	printf("d: %f\n", (double)end - start);


	printf("====QuickSort====\n");
	initArray();
	for (int i = 0; i < 10; i++) {
		initArray_a();
		start = clock();
		quicksort(a, 0, SIZE - 1);
		end = clock();
		aclock += (double)end - start;
	}
	printf("a_average: %f\n", aclock / 10);
	aclock = 0;

	start = clock();
	quicksort(b, 0, SIZE - 1);
	end = clock();
	printf("b: %f\n", (double)end - start);
	start = clock();
	quicksort(c, 0, SIZE - 1);
	end = clock();
	printf("c: %f\n", (double)end - start);
	start = clock();
	quicksort(d, 0, SIZE - 1);
	end = clock();
	printf("d: %f\n", (double)end - start);


	printf("====MergeSort====\n");
	initArray();
	for (int i = 0; i < 10; i++) {
		initArray_a();
		start = clock();
		mergeSort(a, 0, SIZE - 1);
		end = clock();
		aclock += (double)end - start;
	}
	printf("a_average: %f\n", aclock / 10);
	aclock = 0;
	start = clock();
	mergeSort(b, 0, SIZE - 1);
	end = clock();
	printf("b: %f\n", (double)end - start);
	start = clock();
	mergeSort(c, 0, SIZE - 1);
	end = clock();
	printf("c: %f\n", (double)end - start);
	start = clock();
	mergeSort(d, 0, SIZE - 1);
	end = clock();
	printf("d: %f\n", (double)end - start);


	printf("====HeapSort====\n");
	initArray();
	for (int i = 0; i < 10; i++) {
		initArray_a();
		start = clock();
		heapsort(a);
		end = clock();
		aclock += (double)end - start;
	}
	printf("a_average: %f\n", aclock / 10);
	aclock = 0;
	start = clock();
	heapsort(b);
	end = clock();
	printf("b: %f\n", (double)end - start);
	start = clock();
	heapsort(c);
	end = clock();
	printf("c: %f\n", (double)end - start);
	start = clock();
	heapsort(d);
	end = clock();
	printf("d: %f\n", (double)end - start);



	printf("done!");
	return 0;
}

//array initialize
void initArray() {

	b[0] = rand();
	for (int i = 1; i < SIZE / 10; i++) {	//b init
		b[i] = rand() - b[0];
	}
	for (int i = SIZE / 10; i < SIZE; i++) {
		b[i] = rand();
	}
	for (int i = 0; i < SIZE; i++) {	//c init
		c[i] = i;
	}
	for (int i = 0; i < SIZE; i++) {	//d init
		d[i] = SIZE - i;
	}
}

void initArray_a() {
	for (int i = 0; i < SIZE; i++) {	//a init
		a[i] = rand();
	}
}

void insertionSort(int a[]) {
	int j;
	for (j = 2; j <= SIZE; j++) {
		int temp = a[j];
		insert(temp, a, j - 1);
	}
}
void insert(int e, int a[], int i) {
	a[0] = e;
	while (e < a[i]) {
		a[i + 1] = a[i];
		i--;
	}
	a[i + 1] = e;
}

void bubbleSort(int a[]) {
	int tmp, i, j;
	for (i = 0; i < SIZE - 1; i++) {
		for (j = 0; j < SIZE - 1 - i; j++) {
			if (a[j + 1] < a[j]) {
				tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
		}
	}
}

void selectionSort(int a[]) {
	int i, j, min, temp;
	for (i = 0; i < SIZE - 1; i++) {
		min = i;
		for (j = i + 1; j < SIZE; j++) {
			if (a[j] < a[min]) min = j;
		}
		temp = a[i];
		a[i] = a[min];
		a[min] = temp;
	}
}

void quicksort(int a[], int L, int R) {
	int left = L, right = R;
	if (left > right) return;
	int pivot = a[(L + R) / 2];    // pivot을 median으로 설정
	int temp;
	do {
		while (a[left] < pivot)
			left++;
		while (a[right] > pivot)
			right--;
		if (left <= right) {
			temp = a[left];
			a[left] = a[right];
			a[right] = temp;

			left++;
			right--;
		}
	} while (left <= right);

	// 분할된 배열중 길이가 짧은 것부터 실행
	if ((right - L) < (R - left)) {
		quicksort(a, L, right);
		quicksort(a, left, R);
	}
	else {
		quicksort(a, left, R);
		quicksort(a, L, right);
	}
}

void mergeSort(int a[], int left, int right) {
	if (left >= right) return;

	int mid = (left + right) / 2;
	mergeSort(a, left, mid);
	mergeSort(a, mid + 1, right);
	merge(a, left, mid, right);

}

void merge(int list[], int left, int mid, int right) {
	int i, j, k, l;
	i = left;
	j = mid + 1;
	k = left;

	while (i <= mid && j <= right) {
		if (list[i] <= list[j])
			sorted[k++] = list[i++];
		else
			sorted[k++] = list[j++];
	}

	if (i > mid) {
		for (l = j; l <= right; l++)
			sorted[k++] = list[l];
	}
	else {
		for (l = i; l <= mid; l++)
			sorted[k++] = list[l];
	}

	for (l = left; l <= right; l++) {
		list[l] = sorted[l];
	}
}


void heapsort(int list[]) {
	int i, j;
	int temp;
	for (i = SIZE / 2; i > 0; i--) adjust(list, i, SIZE);
	for (i = SIZE - 1; i > 0; i--) {
		temp = list[1];
		list[1] = list[i + 1];
		list[i + 1] = temp;
		adjust(list, 1, i);
	}

}

void adjust(int list[], int root, int n) {
	int child;
	int temp;
	temp = list[root];
	child = 2 * root;
	while (child <= n) {
		if (child < n && list[child] < list[child + 1]) child++;
		if (temp > list[child]) break;
		else {
			list[child / 2] = list[child];
			child *= 2;
		}
	}
	list[child / 2] = temp;
}

