#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include <cmath>
const double PI = 3.1415926535897932384626433832795;

using namespace sf;
using namespace std;



class Player 
{
public:
	float x, y, w, h, dx, dy, speed, rotation;
	int dir, playerScore, health;
	bool life;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;

	Player(String F, float X, float Y, float W, float H) {
		dir = 0; speed = 0; playerScore = 0; health = 100; dx = 0; dy = 0;
		life = true;
		File = F;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(0, 0, 255));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X; y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setOrigin(w/2, h/2);
	}

	void update(float time)
	{
		switch (dir)
		{
			case 0:
				dx = cos(rotation + PI / 2) * speed;
				dy = sin(rotation + PI / 2) * speed;
				break;
			case 1:
				dx = cos(rotation - PI / 2) * speed;
				dy = sin(rotation - PI / 2) * speed;
				break;
			case 2:
				dx = -cos(rotation) * speed;
				dy = -sin(rotation) * speed;
				break;
			case 3:
				dx = cos(rotation) * speed;
				dy = sin(rotation) * speed;
				break;
			case 4:
				dx = cos(rotation + PI/4) * speed;
				dy = -sin(rotation + PI/4)* speed;
				break;
			case 5:
				dx = cos(rotation - PI / 4) * speed;
				dy = -sin(rotation - PI / 4) * speed;
				break;
			case 6:
				dx = cos(rotation - PI / 4) * speed;
				dy = sin(rotation - PI / 4) * speed;
				break;
			case 7:
				dx = cos(rotation + PI / 4) * speed;
				dy = sin(rotation + PI / 4) * speed;
				break;
		}


		x += dx * time;
		y += dy * time;
		speed = 0;
		sprite.setPosition(x, y);
		
		if (health <= 0) { life = false; }

	}

	float getplayercoordinateX() {
		return x;
	}

	float getplayercoordinateY() {
		return y;
	}
};

int main()
{
	 
	RenderWindow window(VideoMode(WIDTH_MAP*16, HEIGHT_MAP*16), "Game");
	view.reset(FloatRect(0, 0, 640, 480));
	//window.setMouseCursorVisible(false);

	Font font;
	font.loadFromFile("CyrilicOld.ttf");
	Text text("", font, 20);
	


	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Player p("heroForRotate.png", 250, 250, 32.0, 32.0);

	bool showMissionText = true;
	bool isMove = false;

	float currentFrame = 0, dX = 0, dY = 0;
	Clock clock;
	Clock gameTimeClock;
	int gameTime = 0;
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dX = pos.x - p.x;
		float dY = pos.y - p.y;
		float rotationDegrees = (atan2(dY, dX)) * 180 / 3.14159265;
		float rotationRadian = atan2(dY, dX);
		p.rotation = rotationRadian;
		p.sprite.setRotation(rotationDegrees);

		// управление персонажем
		if (p.life) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				p.dir = 1; p.speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				p.dir = 0; p.speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				p.dir = 3; p.speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				p.dir = 2; p.speed = 0.1;
			}			
			if (Keyboard::isKeyPressed(Keyboard::W) and Keyboard::isKeyPressed(Keyboard::D)) {
				p.dir = 7; p.speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::W) and Keyboard::isKeyPressed(Keyboard::D)) {
				p.dir = 7; p.speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::W) and Keyboard::isKeyPressed(Keyboard::A)) {
				p.dir = 6; p.speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::S) and Keyboard::isKeyPressed(Keyboard::A)) {
				p.dir = 5; p.speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::S) and Keyboard::isKeyPressed(Keyboard::D)) {
				p.dir = 4; p.speed = 0.1;
			}
			getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}

		p.update(time);
		window.setView(view);
		window.clear();

		
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				
				
				s_map.setTextureRect(IntRect(193, 0, 32, 32));
				
				
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
		window.draw(p.sprite);
		
		
		
		int count = 300;
		for (int i = 0; i <= count; i++)
		{

			for (float lenght = 0; lenght < 1000; lenght += 1)
			{
				float x = p.x + lenght * cos(rotationRadian - (PI / 4) + (i * PI / (count * 2)));
				float y = p.y + lenght * sin(rotationRadian - (PI / 4) + (i * PI / (count * 2)));
				VertexArray lines(Lines, 2);
				lines[0].color = Color::Blue;
				lines[1].color = Color::Blue;
				lines[0].position = Vector2f(p.x, p.y);
				lines[1].position = Vector2f(x, y);
				window.draw(lines);
				if (TileMap[int(x / 32)][int(y / 32)] == '3')
				{
					break;
				}
			}	
		}
		window.display();
	}

	return 0;
}