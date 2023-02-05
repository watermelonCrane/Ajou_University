//2번문제
#include <stdio.h>

enum shape_type {TRIANGLE, RECTANGLE, CIRCLE};

struct shape {
	int type;
	union {
		struct {
			int base, height;
		}tri;
		struct {
			int width, height;
		}rect;
		struct {
			int radius;
		}circ;
	} p;
};

int main() {
	struct shape s;
	enum shape_type type;

	printf("도형의 타입을 입력하시오(0-TRIANGLE, 1-RECTANGLE, 2-CIRCLE): ");
	scanf("%d", &type);

	switch (type) {
		case TRIANGLE: 
			printf("밑변과 반지름을 입력하시오(예를 들어서 10 20): ");
			scanf("%d %d", &s.p.tri.base, &s.p.tri.height);
			printf("면적은 %d\n", s.p.tri.base * s.p.tri.height / 2);
			break;
		case RECTANGLE:
			printf("가로와 세로의 길이를 입력하시오(예를 들어서 10 20): ");
			scanf("%d %d", &s.p.rect.width, &s.p.rect.height);
			printf("면적은 %d\n", s.p.rect.width * s.p.rect.height);
			break;
		case CIRCLE:
			printf("반지름을 입력하시오(예를 들어서 10): ");
			scanf("%d", &s.p.circ.radius);
			printf("면적은 %d\n", (int)(s.p.circ.radius * s.p.circ.radius * 3.14));
			break;
		default:
			printf("도형의 타입이 잘못 선택되었습니다.\n");
			break;
	}

	return 0;
}