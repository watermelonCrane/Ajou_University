//2번 문제

#include <stdio.h>

#define SIZE 10

void print_array(int* arr, int size);
void copy_array(int* arr1, int* arr2, int size);

int main() {
	int a[SIZE] = { 3, 7, 25, 10, 5, 1, 85, 100, 10, 8 };
	int b[SIZE] = { 0 };
	printf("배열 a: ");
	print_array(&a, SIZE);
	printf("배열 b: ");
	print_array(&b, SIZE);
	copy_array(&a, &b, SIZE);

	printf("복사 후 배열 a: ");
	print_array(&a, SIZE);
	printf("복사 후 배열 b: ");
	print_array(&b, SIZE);

	return  0;
}

void print_array(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		printf("%d ", *(arr + i));
	}
	printf("\n");
}

void copy_array(int* arr1, int* arr2, int size) {
	for (int i = 0; i < size; i++) {
		*(arr2 + i) = *(arr1 + i);
	}
}