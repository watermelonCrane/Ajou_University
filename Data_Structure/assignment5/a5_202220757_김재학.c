#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef struct Building {
	int index;
	union {																//visited array에서는 bool형 자료형을, graph list에서는 구조체를 사용
		struct {
			struct Building* link;
			int weight;
		};
		bool visted;
	};
}Building;

//available space list 구성
Building* avail;
Building* getNode();
void retNode(Building* node);

//queue구성
#define Q_SIZE 100
int queue[Q_SIZE];
int front, back;
void push(int data);
int pop();

//minHeap 구성
typedef struct Edge {
	int v1;
	int v2;
	int weight;
}Edge;
int edges;			//edge개수
int nh;				//heap에 들어간 개수
Edge* makeHeap(int edges);												//edges개의 노드를 갖는 힙 어레이 생성
void pushHeap(Edge* h, int v1, int v2, int weight);						//push to heap
void delHeap(Edge* h);													//heap 삭제 및 초기화
void popHeap(Edge* h, int* v1, int* v2, int* weight);					//각 포인터로 값 전달
int searchHeap(Edge* h, int v1, int v2, int weight);					//중복 체크, 이미 존재하면 음수 반환

//union find 구성
void simpleUnion(int* parent, int i, int j);
int simpleFind(int* parent, int i);

//건물 그래프 구성
int inputBuildingCount();												//건물 수 입력
Building** makeList(int num);											//그래프가 저장될 리스트 생성
Building* visitedBuilding(int num);										//방문 array생성
int inputBuildingNum(int num, Building** list, Building* visited);		//건물 번호 입력
Building* getBuilding(int index, int weight);							//건물 노드 생성
void clearBuilding(Building** list, int num, Building* visited);		//메모리 정리 함수. 모든 사용중인 동적할당을 정리함.
int inputLA(const char* str, Building** list, Building* visited);		//list에 문자열로 입력받은 건물 번호 할당 및 visted array에 번호 할당
void inputEdges(Building** list, int num);								//통로 구축 정보 입력
int overlapEdges(Building** list, int num, int a, int b);				//다리 생성시 중복된 다리 검출, 중복이 있으면 음수 반환
int findBuilding(Building** list, int data, int num);					//유효 건물 확인
void linkBuilding(Building** list, Building* building, int index);		//list에 빌딩 연결
int printDFS(Building** list, Building* visited, int num);				//dfs출력함수, list[0]부터 시작, dfs가 호출된 횟수를 리턴
int printBFS(Building** list, Building* visited, int num);				//bfs출력함수, list[0]부터 시작, dfs가 호출된 횟수를 리턴
void initVisited(Building* visited, int num);							//visited array 초기화
int noPrintDFS(Building** list, Building* visited, int num);			//출력이 없고 전체 연결만 확인하는 DFS함수, dfs_ver2가 호출된 횟수를 리턴

void dfs(Building** list, Building* visited, int data, int num);
void bfs(Building** list, Building* visited, int data, int num);
void dfs_ver2(Building** list, Building* visited, int data, int num);

void btoh(Building** list, int num, Edge* heap);						//building list to heap 정보 읽기
void kruskal(Building** list, int num, Edge* heap);						//kruskal 구현

int main() {

	printf("아주대학교 지하 통로 구축 서비스\n");
	for (;;) {
		
		int num = inputBuildingCount();
		if (num == 0) {													//0을 입력받으면 루프문 탈출 및 종료
			printf("	서비스를 종료합니다.\n");
			break;
		}
		
		Building** list = makeList(num);								//그래프를 저장할 리스트 생성
		Building* visited = visitedBuilding(num);						//visited array 생성

		
		if (inputBuildingNum(num, list, visited) < 0) {
			clearBuilding(list, num, visited);							//메모리 정리
			continue;
		}

		inputEdges(list, num);

		if (noPrintDFS(list, visited, num) > 1) {						//전체를 연결하는 경로가 있는지 검사
			printf("전체 건물들을 연결할 수 없습니다\n");
		}
		else {
			printf("성공적인 계획입니다\n");
		}

		printDFS(list, visited, num);
		printBFS(list, visited, num);

		Edge* heap = makeHeap(edges);
		kruskal(list, num, heap);

		printf("----------------------------------------\n");

		clearBuilding(list, num, visited);								//루프마다 메모리 정리
		delHeap(heap);													//힙 삭제
		printf("아주대학교 지하 통로 구축 서비스\n");
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


//건물 수 입력
int inputBuildingCount() {
	for (;;) {
		printf("건물 수를 입력해 주세요\n");
		int building_num;
		scanf("%d", &building_num);
		if (building_num > 20) {							//건물의 수가 20을 넘으면 다시 입력받음.
			printf("건물 수는 20을 초과할 수 없습니다.\n");
		}
		else if (building_num < 0) {						//추가기능: 음의 수를 입력할 경우 다시 입력받음.
			printf("양의 수를 입력해 주세요\n");
		}
		else return building_num;
	}
}

//그래프가 저장될 리스트 생성
Building** makeList(int num) {
	Building** list = (Building**)malloc(sizeof(Building*) * num);
	for (int i = 0; i < num; i++) {
		list[i] = NULL;
	}
	return list;
}

//방문 array생성
Building* visitedBuilding(int num) {
	Building* visited = (Building*)malloc(sizeof(Building) * num);
	return visited;
}

//건물 번호 입력
int inputBuildingNum(int num, Building** list, Building* visited) {
	char str[100];
	printf("건물 번호를 입력해 주세요\n");
	getchar();
	scanf("%[^\n]s", str);

	int count = 0;
	int length = strlen(str);
	for (int i = 0; i < length; i++) {			//입력된 건물 수 검사. 올바르지 않으면 경고문구 출력후 -1 리턴
		if (str[i + 1] == ' ' || str[i + 1] == '\0') {
			count++;
		}
		if (count > num) {
			printf("제시된 도시 수보다 도시 이름의 개수가 더 많습니다.\n");
			return -1;
		}
	}
	if (count < num) {
		printf("제시된 도시 수보다 도시이름의 개수가 적습니다.\n");
		return -1;
	}
	
	return inputLA(str, list, visited);
}

//건물 노드 생성
Building* getBuilding(int index, int weight) {
	Building* node = getNode();
	node->index = index;
	node->link = NULL;
	node->weight = weight;

	return node;
}

//메모리 반환
void clearBuilding(Building** list, int num, Building* visited) {
	free(visited);		//visited 정리

	Building* fp, * lp;
	for (int i = 0; i < num; i++) {		//list 정리
		fp = list[i];
		while (fp != NULL) {
			lp = fp->link;
			retNode(fp);
			fp = lp;
		}
	}
	free(list);
}

//list에 문자열로 입력받은 건물 번호 할당 및 visted array에 번호 할당
int inputLA(const char* str, Building** list, Building* visited) {
	int tmp;
	int count = 0;
	char* ptr = strtok(str, " ");			//문자열에서 숫자를 하나씩 리스트와 어레이에 넣음
	while (ptr != NULL) {
		tmp = atoi(ptr);

		if (findBuilding(list, tmp, count) == 0) {			//추가기능: 건물 입력시 중복 체크 가능
			printf("건물번호가 중복되었습니다.\n");
			return -3;
		}

		if (tmp > 49) {
			printf("건물 번호는 1 이상 49 이하여야 합니다.\n");
			return -2;
		}
		else if (tmp < 1) {
			printf("건물 번호는 1 이상 49 이하여야 합니다.\n");
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


//통로 구축 정보 입력
void inputEdges(Building** list, int num) {
	printf("건물 간 통로 구축 정보를 입력하세요\n");

	char str[15];
	int tmp[3];
	char* ptr;

	Building* b1, * b2;	//새로 연결할 건물 노드
	int i1, i2;			//list에서 찾은 건물 인덱스

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
		if (i1 < 0) {		//통로 중복 체크
			printf("건물이 잘못 입력되었습니다\n");
			continue;
		}
		i2 = findBuilding(list, tmp[1], num);
		if (i2 < 0) {
			printf("건물이 잘못 입력되었습니다\n");
			continue;
		}

		if (overlapEdges(list, num, tmp[0], tmp[1]) < 0) {
			printf("중복된 정보입니다\n");
			continue;
		}


		b1 = getBuilding(tmp[0], tmp[2]);
		b2 = getBuilding(tmp[1], tmp[2]);

		linkBuilding(list, b1, i2);
		linkBuilding(list, b2, i1);

		edges++;
	}

}

//다리 생성시 중복된 다리 검출, 중복이 있으면 음수 반환
int overlapEdges(Building** list, int num, int a, int b) {
	int index = findBuilding(list, a, num);
	Building* ptr = list[index];
	for (ptr = ptr->link; ptr != NULL; ptr = ptr->link) {
		if (ptr->index == b) return -1;
	}

	return 0;
}

//유효건물 확인, list에 건물이 
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

//list에 빌딩 연결
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
			count++;			//dfs 호출 카운트
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
			count++;			//bfs 호출 카운트
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

//visited array 모두 false로 초기화
void initVisited(Building* visited, int num) {
	for (int i = 0; i < num; i++) {
		visited[i].visted = false;
	}
}

//출력이 없고 전체 연결만 확인하는 DFS함수, dfs가 호출된 횟수를 리턴
int noPrintDFS(Building** list, Building* visited, int num) {
	initVisited(visited, num);
	int count = 0;
	for (int i = 0; i < num; i++) {
		if (visited[i].visted != true) {
			dfs_ver2(list, visited, list[i]->index, num);
			count++;			//dfs 호출 카운트
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

//edges개의 노드를 갖는 힙 어레이 생성
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
//heap 삭제 및 초기화
void delHeap(Edge* h) {														
	edges = 0;
	nh = 0;
	free(h);
}
//각 포인터로 값 전달
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


//building list to heap 정보 읽기
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
	int* parent = (int*)malloc(sizeof(int) * num);		//union find를 위한 vertex 어레이 생성, 여기선 index를 parent로 여김.
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