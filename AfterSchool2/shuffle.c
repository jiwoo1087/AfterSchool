#include <stdio.h>
#include <stdlib.h>		//srand(), rand()
#include <time.h>		//time()

void swap(int* pa, int* pb)
{
	int temp;
	temp = *pa;
	*pa = *pb;
	*pb = temp;
}

// _arr의 내용뭏을 무작위로 섞는다.
void shuffle(int(* _arr)[4])
{
	srand(time(0));
	for (int i = 0; i < 100; i++)
	{
		swap(&(_arr[rand() % 4][rand() % 4]), & (_arr[rand() % 4][rand % 4]));
	}
}

int main(void)
{
	const int S = 4;
	int arr[4][4] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	
	shuffle(arr);

	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%d", arr[i]);
		}
	}
}