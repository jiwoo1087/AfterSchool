#include <stdio.h>

struct human {
	int hp;
	int speed;
};

int main(void) 
{
	struct human player;
	player.hp = 5;
	player.speed = 7;

	struct human enemy;
	enemy.hp = 2;
	enemy.speed = 5;

	printf("�÷��̾� ü��%d �÷��̾� �ӵ�%d\n", player.hp, player.speed);

	return 0;

}