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
	Texture player;		// �÷��̾� �̹���
};

const int W_WIDTH = 1200, W_HEIGHT = 600;	// â�� ũ��

int main(void)
{
	struct Textures t;
	t.player.loadFromFile("./resources/image/player.png");

	struct Player player;
	player.sprite.setTexture(&t.player);
	player.sprite.setPosition(100, 100);
	player.sprite.setSize(Vector2f(120, 170));

	// ����â ����
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
				window.close();		// ������ �ݴ´�
				break;
			}
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

		window.clear(Color::Cyan);
		window.draw(player.sprite);
		window.display();
	}

	return 0;
}
