#include <stdio.h>		//printf �Լ� ���

enum item_type {
	DELAY,
	SPEED
};

int main(void)
{
	enum item_type it = SPEED;
	printf("%d \n", it);
	return 0; 
}