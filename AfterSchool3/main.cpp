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
	float x, y;		// 플레이어 좌표
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
	int is_presented;		// 아이템이 떳는지?
	Sound sound;
	long presented_time;
	enum item_type type;
};

struct Textures {
	Texture bg;			// 배경 이미지
	Texture enemy;		// 적 이미지
	Texture gameover;	// 게임오버 이미지
	Texture item_delay;	// 공속 아이템 이미지
	Texture item_speed;	// 이속 아이템 이미지
	Texture player;		// 플레이어 이미지
};

struct SButters {
	SoundBuffer BGM;
	SoundBuffer item_sound;
	SoundBuffer rumble;
};

// obj1과 obj2의 충돌여부. 충돌하면 1을 반환, 충돌안하면 0을 반환
int is_collide(RectangleShape obj1, RectangleShape obj2)
{
	return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

// 전역변수
const int ENEMY_NUM = 10;					// enemy의 최대개수		
const int ITEM_NUM = 2;						// item의 최대종류
const int W_WIDTH = 600, W_HEIGHT = 800;	// 창의 크기
const int GO_WIDTH = 320, GO_HEIGHT = 240;	// 게임오버 그림의 크기

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

	// 윈도창 생성
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);
	srand(time(0));
	long start_time = clock();	// 게임 시작시간
	long spent_time;			// 게임 진행시간
	long fired_time = 0;		// 최근에 발사한 시간
	int is_gameover = 0;

	// BGM
	Sound BGM_sound;
	BGM_sound.setBuffer(sb.BGM);
	BGM_sound.setVolume(100);
	BGM_sound.setLoop(1);		// BGM 무한반복
	BGM_sound.play();

	// text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(30);		// 글자크기 조절
	text.setFillColor(Color::Black);
	text.setPosition(0, 0);
	char info[40];

	// 배경
	Sprite bg_sprite;
	bg_sprite.setTexture(t.bg);
	bg_sprite.setPosition(0, 0);

	// gameover
	Sprite gameover_sprite;
	gameover_sprite.setTexture(t.gameover);
	gameover_sprite.setPosition((W_WIDTH - GO_WIDTH) / 2, (W_HEIGHT - GO_HEIGHT) / 2);

	// 플레이어
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

	
	// 적(enemy)
	struct Enemy enemy[ENEMY_NUM];
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(sb.rumble);
	int enemy_score = 100;
	int enemy_respawn_time = 8;

	// enemy 초기화
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
	item[0].delay = 25000;	// 25초
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


	// 윈도가 열려있을 때까지 반복
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
				// 종료(x) 버튼을 누르면 Event::Closed(0)
			case Event::Closed:
				window.close();		// 윈도를 닫는다
				break;
			}
		}

		/* game상태 update */
		if (player.life <= 0)
		{
			is_gameover = 1;
		}
		/* Player update */
		// 방향키 start
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
		}	// 방향키 end

		// Player 이동범위 제한
		if (player.x < 0)
			player.sprite.setPosition(0, player.y);
		else if (player.x > W_WIDTH - 120)	// 175(그림의 너비)
			player.sprite.setPosition(W_WIDTH - 120, player.y);
		if (player.y < 0)
			player.sprite.setPosition(player.x, 0);
		else if (player.y > W_HEIGHT - 170)	// 105(그림의 높이)
			player.sprite.setPosition(player.x, W_HEIGHT - 170);


		/* Enemy update */
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			// 10초마다 enemy가 리스폰
			if (spent_time % (1000 * enemy_respawn_time) < 1000 / 60 + 1)
			{
				// 게임이 진행중일 때만 적을 리스폰 시키겠다.
				if (!is_gameover)
				{
					enemy[i].sprite.setSize(Vector2f(80, 80));
					enemy[i].sprite.setPosition(rand() % 200, rand() % 100);
					enemy[i].life = 1;
					// 10초마다 enemy의 속도+1
					enemy[i].speed = -(rand() % 10 + 1 + (spent_time / 1000 / enemy_respawn_time));
				}
			}
			if (enemy[i].life > 0)
			{
				// player, enemy 충돌
				if (is_collide(player.sprite, enemy[i].sprite))
				{
					enemy[i].life -= 1;
					player.score += enemy_score;

					if (enemy[i].life == 0)
					{
						enemy_explosion_sound.play();
					}
				}
				// 적이 왼쪽 끝에 진입하려는 순간
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
				//아이템 획득시 효과를 얻고 사라진다
				if (is_collide(player.sprite, item[i].sprite))
				{
					switch (item[i].type)
					{
					case SPEED: //player 이동속도
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

		// draw는 나중에 호출할수록 우선순위가 높아짐
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
			// TODO : 게임이 멈추는 것을 구현할 것
		}
		window.display();
	}
	return 0;
}