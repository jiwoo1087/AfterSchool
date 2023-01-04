#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite;
	int speed;
	int speed_max;
	int score;
	int life;
	float x, y;		// �÷��̾� ��ǥ
};

struct Enemy {
	RectangleShape sprite;
	int speed;
	int life;
};

enum item_type {
	SPEED,		//0
	DELAY		//1
};


struct Item {
	RectangleShape sprite;
	int delay;
	int is_presented;		// �������� ������?
	Sound sound;
	long presented_time;
	enum item_type type;
};

struct Textures {
	Texture bg;			// ��� �̹���
	Texture enemy;		// �� �̹���
	Texture gameover;	// ���ӿ��� �̹���
	Texture item_delay;	// ���� ������ �̹���
	Texture item_speed;	// �̼� ������ �̹���
	Texture player;		// �÷��̾� �̹���
};

struct SButters {
	SoundBuffer BGM;
	SoundBuffer item_sound;
	SoundBuffer rumble;
};

// obj1�� obj2�� �浹����. �浹�ϸ� 1�� ��ȯ, �浹���ϸ� 0�� ��ȯ
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// ��������
const int ENEMY_NUM = 10;					// enemy�� �ִ밳��		
const int ITEM_NUM = 2;						// item�� �ִ�����
const int W_WIDTH = 600, W_HEIGHT = 800;	// â�� ũ��
const int GO_WIDTH = 320, GO_HEIGHT = 240;	// ���ӿ��� �׸��� ũ��

int main(void)
{
	struct Textures t;
	t.bg.loadFromFile("./resources/image/background.jpg");
	t.enemy.loadFromFile("./resources/image/enemy.png");
	t.gameover.loadFromFile("./resources/image/gameover.png");
	t.item_delay.loadFromFile("./resources/image/item_delay.png");
	t.item_speed.loadFromFile("./resources/image/item_speed.png");
	t.player.loadFromFile("./resources/image/player.png");

	struct SButters sb;
	sb.BGM.loadFromFile("./resources/sound/BGM.wav");
	sb.rumble.loadFromFile("./resources/sound/rumble.wav");
	sb.item_sound.loadFromFile("./resources/sound/item_sound.flac");

	// ����â ����
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);
	srand(time(0));
	long start_time = clock();	// ���� ���۽ð�
	long spent_time;			// ���� ����ð�
	long fired_time = 0;		// �ֱٿ� �߻��� �ð�
	int is_gameover = 0;

	// BGM
	Sound BGM_sound;
	BGM_sound.setBuffer(sb.BGM);
	BGM_sound.setVolume(100);
	BGM_sound.setLoop(1);		// BGM ���ѹݺ�
	BGM_sound.play();

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);		// ����ũ�� ����
	text.setFillColor(Color::Black);
	text.setPosition(0, 0);
	char info[40];

	// ���
	Sprite bg_sprite;
	bg_sprite.setTexture(t.bg);
	bg_sprite.setPosition(0, 0);

	// gameover
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t.gameover);
	gameover_sprite.setPosition((W_WIDTH - GO_WIDTH) / 2, (W_HEIGHT - GO_HEIGHT) / 2);

	// �÷��̾�
	struct Player player;
	player.sprite.setTexture(&t.player);
	player.sprite.setPosition(300, 650);
	player.sprite.setSize(Vector2f(150, 150));
	player.x = player.sprite.getPosition().x;
	player.y = player.sprite.getPosition().y;
	player.speed = 5;
	player.speed_max = 15;
	player.score = 0;
	player.life = 10;

	
	// ��(enemy)
	struct Enemy enemy[ENEMY_NUM];
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(sb.rumble);
	int enemy_score = 100;
	int enemy_respawn_time = 8;

	// enemy �ʱ�ȭ
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].sprite.setTexture(&t.enemy);
		enemy[i].sprite.setSize(Vector2f(80, 80));
		enemy[i].sprite.setPosition(rand() % 200 , rand() % 100);
		enemy[i].life = 1;
		enemy[i].speed = -(rand() % 10 + 1);
	}

	// item
	struct Item item[ITEM_NUM];
	item[0].sprite.setTexture(&t.item_speed);
	item[0].delay = 25000;	// 25��
	item[0].type = SPEED;
	item[0].sound.setBuffer(sb.item_sound);
	item[1].sprite.setTexture(&t.item_delay);
	item[1].delay = 20000;
	item[1].type = DELAY;
	item[1].sound.setBuffer(sb.item_sound);

	for (int i = 0; i < ITEM_NUM; i++)
	{
		item[i].sprite.setSize(Vector2f(50, 50));
		item[i].is_presented = 0;
		item[i].presented_time = 0;
	}


	// ������ �������� ������ �ݺ�
	while (window.isOpen())
	{
		spent_time = clock() - start_time;
		player.x = player.sprite.getPosition().x;
		player.y = player.sprite.getPosition().y;
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// ����(x) ��ư�� ������ Event::Closed(0)
			case Event::Closed:
				window.close();		// ������ �ݴ´�
				break;
			}
		}

		/* game���� update */
		if (player.life <= 0)
		{
			is_gameover = 1;
		}
		/* Player update */
		// ����Ű start
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.sprite.move(-player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.sprite.move(player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.sprite.move(0, -player.speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.sprite.move(0, player.speed);
		}	// ����Ű end

		// Player �̵����� ����
		if (player.x < 0)
			player.sprite.setPosition(0, player.y);
		else if (player.x > W_WIDTH - 120)	// 175(�׸��� �ʺ�)
			player.sprite.setPosition(W_WIDTH - 120, player.y);
		if (player.y < 0)
			player.sprite.setPosition(player.x, 0);
		else if (player.y > W_HEIGHT - 170)	// 105(�׸��� ����)
			player.sprite.setPosition(player.x, W_HEIGHT - 170);


		/* Enemy update */
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10�ʸ��� enemy�� ������
			if (spent_time % (1000 * enemy_respawn_time) < 1000 / 60 + 1)
			{
				// ������ �������� ���� ���� ������ ��Ű�ڴ�.
				if (!is_gameover)
				{
					enemy[i].sprite.setSize(Vector2f(80, 80));
					enemy[i].sprite.setPosition(rand() % 200, rand() % 100);
					enemy[i].life = 1;
					// 10�ʸ��� enemy�� �ӵ�+1
					enemy[i].speed = -(rand() % 10 + 1 + (spent_time / 1000 / enemy_respawn_time));
				}
			}
			if (enemy[i].life > 0)
			{
				// player, enemy �浹
				if (is_collide(player.sprite, enemy[i].sprite))
				{
					enemy[i].life -= 1;
					player.score += enemy_score;

					if (enemy[i].life == 0)
					{
						enemy_explosion_sound.play();
					}
				}
				// ���� ���� ���� �����Ϸ��� ����
				else if (enemy[i].sprite.getPosition().x < 0)
				{
					player.life -= 1;
					enemy[i].life = 0;
				}
				
				enemy[i].sprite.move(enemy[i].speed, 0);
			}
		}

		for (int i = 0; i < ITEM_NUM; i++)
		{
			if (!item[i].is_presented)
			{
				if (spent_time - item[i].presented_time > item[i].delay)
				{
					item[i].sprite.setPosition((rand() % W_WIDTH) * 0.8, (rand() % W_HEIGHT) * 0.8);
					item[i].is_presented = 1;
				}
			}

			if (item[i].is_presented)
			{
				//������ ȹ��� ȿ���� ��� �������
				if (is_collide(player.sprite, item[i].sprite))
				{
					switch (item[i].type)
					{
					case SPEED: //player �̵��ӵ�
						player.speed += 2;
						if (player.speed > player.speed_max)
							player.speed = player.speed_max;
						break;
					}
					item[i].is_presented = 0;
					item[i].presented_time = spent_time;
					item[i].sound.play();
				}
			}
		}


		sprintf(info, "life:%d score:%d time:%d"
			, player.life, player.score, spent_time / 1000);
		text.setString(info);
		window.clear(Color::Black);
		window.draw(bg_sprite);

		// draw�� ���߿� ȣ���Ҽ��� �켱������ ������
		for (int i = 0; i < ENEMY_NUM; i++)
			if (enemy[i].life > 0)
				window.draw(enemy[i].sprite);
		for (int i = 0; i < ITEM_NUM; i++)
		{
			if (item[i].is_presented)
				window.draw(item[i].sprite);
		}

		window.draw(player.sprite);
		window.draw(text);

		if (is_gameover)
		{
			window.draw(gameover_sprite);
			// TODO : ������ ���ߴ� ���� ������ ��
		}
		window.display();
	}
	return 0;
}