#include <stdio.h>

int main(void) 
{
	int a = 1;

	//a값이 정수로 딱 하나만 떨어지는 경우, switch를 쓸 것
	switch (a)
	{
	case 1:
		printf("apple");
		break;
	case 2:
		printf("banana");
		break;
	default:
		printf("mango");
		break;
	}

	return 0;
}