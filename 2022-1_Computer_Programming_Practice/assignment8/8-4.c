//4번문제

#include <stdio.h>

#define SIZE 5

void print_array2(char* name, int* arr, int size);
void merge(int* arr1, int* arr2, int* arr3, int size);

int main() {
	int a[SIZE] = { 1, 3, 10, 15, 34 }, b[SIZE] = { 7, 11, 22, 28, 40 };
	int c[SIZE * 2];

	print_array2("a", a, SIZE);
	print_array2("b", b, SIZE);
	merge(a, b, c, SIZE);
	print_array2("c", c, SIZE * 2);

	return 0;
}

void print_array2(char* name, int* arr, int size) {
	printf("배열 %s: ", name);
	for (int i = 0; i < size; i++) {
		printf("%d ", *(arr + i));
	}
	printf("\n");
}

void merge(int* arr1, int* arr2, int* arr3, int size) {
	int i = 0;	//arr1
	int k = 0;	//arr2
	int j = 0;	//arr3
	while (i < size && k < size) {

		if (*(arr1 + i) < *(arr2 + k)) {
			*(arr3 + j) = *(arr1 + i);
			i++;
			j++;
		}
		else if (*(arr1 + i) > *(arr2 + k)) {
			*(arr3 + j) = *(arr2 + k);
			k++;
			j++;
		}
		else if (*(arr1 + i) == *(arr2 + k)) {	//겹치는 숫자가 있을때
			*(arr3 + j) = *(arr1 + i);
			*(arr3 + j + 1) = *(arr2 + k);
			i++;
			k++;
			j++; j++;
		}
	}
	if (i < size)
		*(arr3 + j) = *(arr1 + i);

	if (k < size)
		*(arr3 + j) = *(arr2 + k);


}