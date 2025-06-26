#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Node {
	char data;
	struct Node* leftChild, * rightChild;
}Node;

typedef struct {
	char data[20];
	int ptr;		//data�� ����ִ� ��ġ�� ���� ���� �ε����� ����Ŵ
}String;

Node* makeT1();
Node* newNode(char data);
void postorder(Node* T, int count, String* result);	//count������ recursive function�� ��� ���ư������� ������ Ȯ���ϱ� ���� �����̴�. ó���� 0���� ������ ���̴�.
void initString(String* str);
void postorder_iterative(Node* T, String* result);


int main() {
	Node* T1 = makeT1();

	String result;
	initString(&result);

	postorder(T1, 0, &result);
	printf("���� postorder(recursive ver.): %s\n", result.data);

	initString(&result);

	postorder_iterative(T1, &result);


	return 0;
}

void postorder_iterative(Node* T, String* result) {
	Node* stack[20];
	int top = 0;
	
	char outStack[20];
	int o_top = 0;


	stack[top++] = T;
	for (;top != 0;) {

		Node* tmp = stack[top - 1];
		top--;
		
		outStack[o_top++] = tmp->data;

		if (tmp->leftChild != NULL) {
			stack[top++] = tmp->leftChild;
		}
		if (tmp->rightChild != NULL) {
			stack[top++] = tmp->rightChild;
		}
		

	}

	printf("\npostorder output (iterative ver.): ");
	for (; o_top > 0;) {
		printf("%c", outStack[o_top - 1]);
		o_top--;
	}
	printf("\n");

}

void initString(String* str) {
	for (int i = 0; i < 20; i++) {
		str->data[i] = '\0';
	}
	str->ptr = 0;
}

void postorder(Node* T, int count, String* result) {
	printf("\n%d��° �Լ�ȣ��\nroot: %c\n", count, T->data);
	count++;
	if (T->leftChild != NULL) {
		postorder(T->leftChild, count, result);
	}
	if (T->rightChild != NULL) {
		postorder(T->rightChild, count, result);
	}

	result->data[result->ptr] = T->data;		//postorder ����� ���� ������ �߰�
	result->ptr += 1;

	printf("��������� postorder output: %s\n", result->data);

	printf("%d��° �Լ� ��\n\n", count-1);

	return;
}

Node* newNode(char data) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	return newNode;
}


Node* makeT1() {
	Node* T1 = newNode('A');
	T1->leftChild = newNode('B');
	T1->leftChild->rightChild = newNode('C');
	T1->leftChild->rightChild->rightChild = newNode('D');
	T1->leftChild->rightChild->rightChild->rightChild = newNode('E');

	T1->leftChild->leftChild = newNode('F');
	T1->leftChild->leftChild->rightChild = newNode('G');

	T1->leftChild->rightChild->rightChild->rightChild->leftChild = newNode('H');
	T1->leftChild->rightChild->rightChild->rightChild->leftChild->rightChild = newNode('I');
	T1->leftChild->rightChild->rightChild->rightChild->leftChild->rightChild->rightChild = newNode('J');

	T1->leftChild->leftChild->leftChild = newNode('K');


	return T1;
}