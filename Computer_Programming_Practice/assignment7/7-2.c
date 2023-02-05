//2번문제

#include <stdio.h>

#define SIZE 10

void print_array(int arr[], int size);
int compare_array(int arr1[], int arr2[], int size);


int main() {
	int a[SIZE] = { 3, 7, 2, 10, 5 };
	int b[SIZE] = { 3, 7, 2 };
	int c[SIZE] = { 40, 65, 11, 90, 88 };
	int d[SIZE] = { 40, 65, 11, 90, 88 };

	printf("배열 a: ");
	print_array(a, SIZE);
	printf("배열 b: ");
	print_array(b, SIZE);
	
	if (compare_array(a, b, SIZE) == 1)
		printf("2개의 배열은 같음\n");
	else if (compare_array(a, b, SIZE) == 0)
		printf("2개의 배열은 다름\n");

	printf("배열 c: ");
	print_array(c, SIZE);
	printf("배열 d: ");
	print_array(d, SIZE);
	if (compare_array(c, d, SIZE) == 1)
		printf("2개의 배열은 같음\n");
	else if (compare_array(c, d, SIZE) == 0)
		printf("2개의 배열은 다름\n");

	return 0;
}

void print_array(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int compare_array(int arr1[], int arr2[], int size) {
	for (int i = 0; i < size; i++) {
		if (arr1[i] != arr2[i])
			return 0;
	}
	return 1;
}