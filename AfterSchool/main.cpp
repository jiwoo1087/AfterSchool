#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/Audio.hpp>

using namespace sf;

int main(void)
{
	//윈도창 생성
	RenderWindow window(VideoMode(640, 480), "AfterSchool");
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock();	//게임 시작시간
	long spent_time;			//게임 진행시간


	//text
	Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(24);	 //글자크기 조절
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	char info[40];
	text.setString("score");

	//배경
	Texture bg_texture;
	bg_texture.loadFromFile("./resources/image/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);
	
	
	//플레이어
	RectangleShape player;
	player.setSize(Vector2f(40, 40));
	player.setPosition(100, 100);
	player.setFillColor(Color::Red);
	int player_speed = 7;
	int player_score = 0;

	//적
	const int ENEMY_NUM = 100;
	RectangleShape enemy[ENEMY_NUM];
	int enemy_life[ENEMY_NUM];
	int enemy_speed[ENEMY_NUM];
	int enemy_score = 100;      //적을 잡을 때 얻는 점수
	SoundBuffer enemy_explosion_buffer;
	enemy_explosion_buffer.loadFromFile("./resources/sound/rumble.flac");
	Sound enemy_explosion_sound;
    enemy_explosion_sound.setBuffer(enemy_explosion_buffer);

	//enemy 초기화
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy[i].setPosition(rand() % 300 + 300, rand() % 380);
		enemy_speed[i] = -(rand() % 10 + 1);
	}


	//윈도가 열려있을 때까지 반복
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				//종류(x) 버튼을 누르면 Event::Closed(0)
			case Event::Closed:
				window.close();		//윈도를 닫는다
				break;
				//키보드 눌렀을 때(누른 순간만 감지)
			case Event::KeyPressed:
			{
				//스페이스 키 누르면 모든 enemy 출현
				if (event.key.code == Keyboard::Space)
				{
					for (int i = 0; i < ENEMY_NUM; i++)
					{
						enemy[i].setSize(Vector2f(70, 70));
						enemy[i].setFillColor(Color::Yellow);
						enemy_life[i] = 1;
						enemy[i].setPosition(rand() % 300 + 300, rand() % 380);
					}
				}
				break;
			}

			}
			
		}

		spent_time = clock()-start_time;

		//방향키 start
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			player.move(-player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			player.move(player_speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			player.move(0, -player_speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			player.move(0, player_speed);
		} //방향키 end


		//enemy와의 충돌
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy_life[i] > 0) {
				if (player.getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
				{
					printf("enemy%d과 충돌\n", i);
					enemy_life[i] -= 1;
					player_score += enemy_score;

					// TODO : 코드 
					if (enemy_life[i] == 0)
					{
						enemy_explosion_sound.play();
					}
				}
				enemy[i].move(enemy_speed[i], 0);
			}
		}


		sprintf(info, "score : %d time:%d"
			, player_score, spent_time/1000);
		text.setString(info);
		

		window.clear(Color::Black);
		window.draw(bg_sprite);

		//draw는 나중에 호출할수록 우선순위가 높아짐
		for (int i = 0; i < ENEMY_NUM; i++)
		{
			if (enemy_life[i] > 0)
			{
				window.draw(enemy[i]);
			}
		}
		window.draw(player);
		window.draw(text);
		

		window.display();
	}
	
	return 0; 
}