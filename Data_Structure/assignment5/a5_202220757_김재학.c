#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef struct Building {
	int index;
	union {																//visited array������ bool�� �ڷ�����, graph list������ ����ü�� ���
		struct {
			struct Building* link;
			int weight;
		};
		bool visted;
	};
}Building;

//available space list ����
Building* avail;
Building* getNode();
void retNode(Building* node);

//queue����
#define Q_SIZE 100
int queue[Q_SIZE];
int front, back;
void push(int data);
int pop();

//minHeap ����
typedef struct Edge {
	int v1;
	int v2;
	int weight;
}Edge;
int edges;			//edge����
int nh;				//heap�� �� ����
Edge* makeHeap(int edges);												//edges���� ��带 ���� �� ��� ����
void pushHeap(Edge* h, int v1, int v2, int weight);						//push to heap
void delHeap(Edge* h);													//heap ���� �� �ʱ�ȭ
void popHeap(Edge* h, int* v1, int* v2, int* weight);					//�� �����ͷ� �� ����
int searchHeap(Edge* h, int v1, int v2, int weight);					//�ߺ� üũ, �̹� �����ϸ� ���� ��ȯ

//union find ����
void simpleUnion(int* parent, int i, int j);
int simpleFind(int* parent, int i);

//�ǹ� �׷��� ����
int inputBuildingCount();												//�ǹ� �� �Է�
Building** makeList(int num);											//�׷����� ����� ����Ʈ ����
Building* visitedBuilding(int num);										//�湮 array����
int inputBuildingNum(int num, Building** list, Building* visited);		//�ǹ� ��ȣ �Է�
Building* getBuilding(int index, int weight);							//�ǹ� ��� ����
void clearBuilding(Building** list, int num, Building* visited);		//�޸� ���� �Լ�. ��� ������� �����Ҵ��� ������.
int inputLA(const char* str, Building** list, Building* visited);		//list�� ���ڿ��� �Է¹��� �ǹ� ��ȣ �Ҵ� �� visted array�� ��ȣ �Ҵ�
void inputEdges(Building** list, int num);								//��� ���� ���� �Է�
int overlapEdges(Building** list, int num, int a, int b);				//�ٸ� ������ �ߺ��� �ٸ� ����, �ߺ��� ������ ���� ��ȯ
int findBuilding(Building** list, int data, int num);					//��ȿ �ǹ� Ȯ��
void linkBuilding(Building** list, Building* building, int index);		//list�� ���� ����
int printDFS(Building** list, Building* visited, int num);				//dfs����Լ�, list[0]���� ����, dfs�� ȣ��� Ƚ���� ����
int printBFS(Building** list, Building* visited, int num);				//bfs����Լ�, list[0]���� ����, dfs�� ȣ��� Ƚ���� ����
void initVisited(Building* visited, int num);							//visited array �ʱ�ȭ
int noPrintDFS(Building** list, Building* visited, int num);			//����� ���� ��ü ���Ḹ Ȯ���ϴ� DFS�Լ�, dfs_ver2�� ȣ��� Ƚ���� ����

void dfs(Building** list, Building* visited, int data, int num);
void bfs(Building** list, Building* visited, int data, int num);
void dfs_ver2(Building** list, Building* visited, int data, int num);

void btoh(Building** list, int num, Edge* heap);						//building list to heap ���� �б�
void kruskal(Building** list, int num, Edge* heap);						//kruskal ����

int main() {

	printf("���ִ��б� ���� ��� ���� ����\n");
	for (;;) {
		
		int num = inputBuildingCount();
		if (num == 0) {													//0�� �Է¹����� ������ Ż�� �� ����
			printf("	���񽺸� �����մϴ�.\n");
			break;
		}
		
		Building** list = makeList(num);								//�׷����� ������ ����Ʈ ����
		Building* visited = visitedBuilding(num);						//visited array ����

		
		if (inputBuildingNum(num, list, visited) < 0) {
			clearBuilding(list, num, visited);							//�޸� ����
			continue;
		}

		inputEdges(list, num);

		if (noPrintDFS(list, visited, num) > 1) {						//��ü�� �����ϴ� ��ΰ� �ִ��� �˻�
			printf("��ü �ǹ����� ������ �� �����ϴ�\n");
		}
		else {
			printf("�������� ��ȹ�Դϴ�\n");
		}

		printDFS(list, visited, num);
		printBFS(list, visited, num);

		Edge* heap = makeHeap(edges);
		kruskal(list, num, heap);

		printf("----------------------------------------\n");

		clearBuilding(list, num, visited);								//�������� �޸� ����
		delHeap(heap);													//�� ����
		printf("���ִ��б� ���� ��� ���� ����\n");
	}
	
	
	return 0;
}


Building* getNode() {
	Building* node;
	if (avail != NULL) {
		node = avail;
		avail = avail->link;
	}
	else {
		node = (Building*)malloc(sizeof(Building));
	}

	return node;
}

void retNode(Building* node) {
	node->link = avail;
	avail = node;
}


void push(int data) {
	queue[back] = data;
	back++;
	back %= Q_SIZE;
}

int pop() {
	int tmp = queue[front];
	front++;
	front = front % Q_SIZE;
	return tmp;
}


//�ǹ� �� �Է�
int inputBuildingCount() {
	for (;;) {
		printf("�ǹ� ���� �Է��� �ּ���\n");
		int building_num;
		scanf("%d", &building_num);
		if (building_num > 20) {							//�ǹ��� ���� 20�� ������ �ٽ� �Է¹���.
			printf("�ǹ� ���� 20�� �ʰ��� �� �����ϴ�.\n");
		}
		else if (building_num < 0) {						//�߰����: ���� ���� �Է��� ��� �ٽ� �Է¹���.
			printf("���� ���� �Է��� �ּ���\n");
		}
		else return building_num;
	}
}

//�׷����� ����� ����Ʈ ����
Building** makeList(int num) {
	Building** list = (Building**)malloc(sizeof(Building*) * num);
	for (int i = 0; i < num; i++) {
		list[i] = NULL;
	}
	return list;
}

//�湮 array����
Building* visitedBuilding(int num) {
	Building* visited = (Building*)malloc(sizeof(Building) * num);
	return visited;
}

//�ǹ� ��ȣ �Է�
int inputBuildingNum(int num, Building** list, Building* visited) {
	char str[100];
	printf("�ǹ� ��ȣ�� �Է��� �ּ���\n");
	getchar();
	scanf("%[^\n]s", str);

	int count = 0;
	int length = strlen(str);
	for (int i = 0; i < length; i++) {			//�Էµ� �ǹ� �� �˻�. �ùٸ��� ������ ����� ����� -1 ����
		if (str[i + 1] == ' ' || str[i + 1] == '\0') {
			count++;
		}
		if (count > num) {
			printf("���õ� ���� ������ ���� �̸��� ������ �� �����ϴ�.\n");
			return -1;
		}
	}
	if (count < num) {
		printf("���õ� ���� ������ �����̸��� ������ �����ϴ�.\n");
		return -1;
	}
	
	return inputLA(str, list, visited);
}

//�ǹ� ��� ����
Building* getBuilding(int index, int weight) {
	Building* node = getNode();
	node->index = index;
	node->link = NULL;
	node->weight = weight;

	return node;
}

//�޸� ��ȯ
void clearBuilding(Building** list, int num, Building* visited) {
	free(visited);		//visited ����

	Building* fp, * lp;
	for (int i = 0; i < num; i++) {		//list ����
		fp = list[i];
		while (fp != NULL) {
			lp = fp->link;
			retNode(fp);
			fp = lp;
		}
	}
	free(list);
}

//list�� ���ڿ��� �Է¹��� �ǹ� ��ȣ �Ҵ� �� visted array�� ��ȣ �Ҵ�
int inputLA(const char* str, Building** list, Building* visited) {
	int tmp;
	int count = 0;
	char* ptr = strtok(str, " ");			//���ڿ����� ���ڸ� �ϳ��� ����Ʈ�� ��̿� ����
	while (ptr != NULL) {
		tmp = atoi(ptr);

		if (findBuilding(list, tmp, count) == 0) {			//�߰����: �ǹ� �Է½� �ߺ� üũ ����
			printf("�ǹ���ȣ�� �ߺ��Ǿ����ϴ�.\n");
			return -3;
		}

		if (tmp > 49) {
			printf("�ǹ� ��ȣ�� 1 �̻� 49 ���Ͽ��� �մϴ�.\n");
			return -2;
		}
		else if (tmp < 1) {
			printf("�ǹ� ��ȣ�� 1 �̻� 49 ���Ͽ��� �մϴ�.\n");
			return -2;
		}

		list[count] = getBuilding(tmp, 0);
		visited[count].index = tmp;
		visited[count].visted = false;
		count++;
		ptr = strtok(NULL, " ");
	}

	return count;
}


//��� ���� ���� �Է�
void inputEdges(Building** list, int num) {
	printf("�ǹ� �� ��� ���� ������ �Է��ϼ���\n");

	char str[15];
	int tmp[3];
	char* ptr;

	Building* b1, * b2;	//���� ������ �ǹ� ���
	int i1, i2;			//list���� ã�� �ǹ� �ε���

	while (1) {
		getchar();
		scanf("%[^\n]s", str);
		if (str[0] == '.') break;

		ptr = strtok(str, ", ");
		for (int i = 0; i < 3; i++) {
			tmp[i] = atoi(ptr);
			ptr = strtok(NULL, ", ");
		}

		i1 = findBuilding(list, tmp[0], num);
		if (i1 < 0) {		//��� �ߺ� üũ
			printf("�ǹ��� �߸� �ԷµǾ����ϴ�\n");
			continue;
		}
		i2 = findBuilding(list, tmp[1], num);
		if (i2 < 0) {
			printf("�ǹ��� �߸� �ԷµǾ����ϴ�\n");
			continue;
		}

		if (overlapEdges(list, num, tmp[0], tmp[1]) < 0) {
			printf("�ߺ��� �����Դϴ�\n");
			continue;
		}


		b1 = getBuilding(tmp[0], tmp[2]);
		b2 = getBuilding(tmp[1], tmp[2]);

		linkBuilding(list, b1, i2);
		linkBuilding(list, b2, i1);

		edges++;
	}

}

//�ٸ� ������ �ߺ��� �ٸ� ����, �ߺ��� ������ ���� ��ȯ
int overlapEdges(Building** list, int num, int a, int b) {
	int index = findBuilding(list, a, num);
	Building* ptr = list[index];
	for (ptr = ptr->link; ptr != NULL; ptr = ptr->link) {
		if (ptr->index == b) return -1;
	}

	return 0;
}

//��ȿ�ǹ� Ȯ��, list�� �ǹ��� 
int findBuilding(Building** list, int data, int num) {
	bool find = false;

	int k;
	for (k = 0; k < num; k++) {
		if (list[k]->index == data) {
			find = true;
			break;
		}
	}

	if (find == true) return k;
	else return -1;

}

//list�� ���� ����
void linkBuilding(Building** list, Building* building, int index) {

	Building* ptr = list[index];
	while (ptr->link != NULL) {
		ptr = ptr->link;
	}

	ptr->link = building;

}

int printDFS(Building** list, Building* visited, int num) {
	int count = 0;
	initVisited(visited, num);
	printf("dfs : ");
	for (int i = 0; i < num; i++) {
		if (visited[i].visted != true) {
			if (i != 0) {
				printf(", ");
			}
			printf("( ");
			dfs(list, visited, list[i]->index, num);
			count++;			//dfs ȣ�� ī��Ʈ
			printf(")");
		}
	}

	printf("\n");

	return count;
}

int printBFS(Building** list, Building* visited, int num) {
	int count = 0;
	initVisited(visited, num);
	printf("bfs : ");
	for (int i = 0; i < num; i++) {
		if (visited[i].visted != true) {
			if (i != 0) {
				printf(", ");
			}
			printf("( ");
			bfs(list, visited, list[i]->index, num);
			count++;			//bfs ȣ�� ī��Ʈ
			printf(")");
		}
	}
	printf("\n");

	return count;
}

void dfs(Building** list, Building* visited, int data, int num) {
	
	int index = findBuilding(list, data, num);
	
	Building* ptr = list[index];

	printf("%d ", data);
	visited[index].visted = true;
	for (ptr = ptr->link; ptr != NULL; ptr = ptr->link) {
		if (visited[findBuilding(list, ptr->index, num)].visted != true) {
			dfs(list, visited, ptr->index, num);
		}
	}

}

void bfs(Building** list, Building* visited, int data, int num) {
	int index = findBuilding(list, data, num);

	Building* ptr = list[index];
	printf("%d ", ptr->index);
	visited[index].visted = true;
	push(index);
	while (back - front != 0) {
		index = pop();
		ptr = list[index];
		for (ptr = ptr->link; ptr != NULL; ptr = ptr->link) {
			int tmp = findBuilding(list, ptr->index, num);
			if (visited[tmp].visted != true) {
				printf("%d ", ptr->index);
				push(tmp);
				visited[tmp].visted = true;
			}
		}
	}

}

//visited array ��� false�� �ʱ�ȭ
void initVisited(Building* visited, int num) {
	for (int i = 0; i < num; i++) {
		visited[i].visted = false;
	}
}

//����� ���� ��ü ���Ḹ Ȯ���ϴ� DFS�Լ�, dfs�� ȣ��� Ƚ���� ����
int noPrintDFS(Building** list, Building* visited, int num) {
	initVisited(visited, num);
	int count = 0;
	for (int i = 0; i < num; i++) {
		if (visited[i].visted != true) {
			dfs_ver2(list, visited, list[i]->index, num);
			count++;			//dfs ȣ�� ī��Ʈ
		}
	}

	return count;
}

void dfs_ver2(Building** list, Building* visited, int data, int num) {

	int index = findBuilding(list, data, num);

	Building* ptr = list[index];
	visited[index].visted = true;
	for (ptr = ptr->link; ptr != NULL; ptr = ptr->link) {
		if (visited[findBuilding(list, ptr->index, num)].visted != true) {
			dfs_ver2(list, visited, ptr->index, num);
		}
	}
}

//edges���� ��带 ���� �� ��� ����
Edge* makeHeap(int edges) {												
	Edge* heap = (Edge**)malloc(sizeof(Edge) * (edges + 1));
	return heap;
}
//push to heap
void pushHeap(Edge* h, int v1, int v2, int weight) {					
	int i = ++nh;
	while ((i != 1) && (weight < h[i / 2].weight)) {
		h[i] = h[i / 2];
		i /= 2;
	}
	h[i].v1 = v1;
	h[i].v2 = v2;
	h[i].weight = weight;
}
//heap ���� �� �ʱ�ȭ
void delHeap(Edge* h) {														
	edges = 0;
	nh = 0;
	free(h);
}
//�� �����ͷ� �� ����
void popHeap(Edge* h, int* v1, int* v2, int* weight) {					
	int parent, child;
	*v1 = h[1].v1;
	*v2 = h[1].v2;
	*weight = h[1].weight;

	int t1, t2, tw;
	t1 = h[nh].v1;
	t2 = h[nh].v2;
	tw = h[nh].weight;
	nh--;

	parent = 1;
	child = 2;

	while (child <= nh) {
		if ((child < nh) && (h[child].weight > h[child + 1].weight)) child++;
		if (tw <= h[child].weight) break;
		h[parent] = h[child];
		parent = child;
		child *= 2;
	}
	h[parent].v1 = t1;
	h[parent].v2 = t2;
	h[parent].weight = tw;

}


//building list to heap ���� �б�
void btoh(Building** list, int num, Edge* heap) {
	Building* ptr;
	for (int i = 0; i < num; i++) {
		ptr = list[i];
		while (ptr->link != NULL) {
			ptr = ptr->link;
			if (searchHeap(heap, list[i]->index, ptr->index, ptr->weight) == 0) {
				pushHeap(heap, list[i]->index, ptr->index, ptr->weight);
			}
			
		}
	}
}


int searchHeap(Edge* h, int v1, int v2, int weight) {
	int i = 1;
	if (nh == 0) return 0;
	while (i != nh + 1) {
		if ((h[i].v1 == v1) && (h[i].v2 == v2)) {
			if (h[i].weight == weight) {
				return -1;
			}
		}
		else if ((h[i].v1 == v2) && (h[i].v2 == v1)) {
			if (h[i].weight == weight) {
				return -1;
			}
		}
		i++;
	}

	return 0;
}

void kruskal(Building** list, int num, Edge* heap) {
	btoh(list, num, heap);
	int* parent = (int*)malloc(sizeof(int) * num);		//union find�� ���� vertex ��� ����, ���⼱ index�� parent�� ����.
	for (int i = 0; i < num; i++) {
		parent[i] = -1;
	}

	printf("Kruskal : (");
	int v1, v2, w;
	int costResult = 0;
	int count = 0;
	for (int i = 0; count != num - 1 && nh != 0; i++) {
		popHeap(heap, &v1, &v2, &w);
		if (v1 > v2) {
			int tmp = v1;
			v1 = v2;
			v2 = tmp;
		}
		int tmp1 = simpleFind(parent, findBuilding(list, v1, num));
		int tmp2 = simpleFind(parent, findBuilding(list, v2, num));
		if (i == 0) {
			printf("(%d, %d)", v1, v2);
			costResult += w;
			simpleUnion(parent, findBuilding(list, v1, num), findBuilding(list, v2, num));
			count++;
		}
		else if ((tmp1 < 0 || tmp2 < 0) || tmp1 != tmp2) {
			printf(", (%d, %d)", v1, v2);
			costResult += w;
			simpleUnion(parent, findBuilding(list, v1, num), findBuilding(list, v2, num));
			count++;
		}

	}
	printf("), %d\n", costResult);

}

void simpleUnion(int* parent, int i, int j) {
	parent[simpleFind(parent, i)] = j;
}

int simpleFind(int* parent, int i) {
	for (; parent[i] >= 0; i = parent[i]);
	return i;
}