#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

struct Player
{
	RectangleShape sprite;
	int speed;
};

struct Textures
{
	Texture player;		// 플레이어 이미지
};

const int W_WIDTH = 1200, W_HEIGHT = 600;	// 창의 크기

int main(void)
{
	struct Textures t;
	t.player.loadFromFile("./resources/image/player.png");

	struct Player player;
	player.sprite.setTexture(&t.player);
	player.sprite.setPosition(100, 100);
	player.sprite.setSize(Vector2f(120, 170));

	// 윈도창 생성
	RenderWindow window(VideoMode(W_WIDTH, W_HEIGHT), "AfterSchool");
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();		// 윈도를 닫는다
				break;
			}
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

		window.clear(Color::Cyan);
		window.draw(player.sprite);
		window.display();
	}

	return 0;
}
