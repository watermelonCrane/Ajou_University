//5¹ø¹®Á¦

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void get_dice_face(void);

int main() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < 100; i++)		
		get_dice_face();
		
	return 0;
}

void get_dice_face(void) {
	
	static int one = 0, two = 0, three = 0, four = 0, five = 0, six = 0;
	static int call_count = 0;
	
	int dice_face = rand() % 6 + 1;

	if (dice_face == 1)
		one++;
	else if (dice_face == 2)
		two++;
	else if (dice_face == 3)
		three++;
	else if (dice_face == 4)
		four++;
	else if (dice_face == 5)
		five++;
	else if (dice_face == 6)
		six++;
	call_count++;

	if (call_count == 100) {
		printf("1->%d\n", one);
		printf("2->%d\n", two);
		printf("3->%d\n", three);
		printf("4->%d\n", four);
		printf("5->%d\n", five);
		printf("6->%d\n", six);
	}
	
	
}