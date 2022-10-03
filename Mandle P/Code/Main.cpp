#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "ComplexPlane.h"

sf::Texture mandelbrot(int width, int height, double xmin, double xmax, double ymin, double ymax, int iterations);
ComplexPlane Plane;

using namespace sf;
using namespace std;
int main()
{
	bool in = true;
	int width = VideoMode::getDesktopMode().width;
	int height = VideoMode::getDesktopMode().height;

	RenderWindow window(sf::VideoMode(width, height), "Project");

	Texture mandelTexture;
	Sprite mandelSprite;

	RectangleShape zoomBorder(Vector2f(width / 2, height / 2));
	zoomBorder.setFillColor(Color(0, 0, 0, 0));
	zoomBorder.setOutlineColor(Color(0, 126, 126, 126));
	zoomBorder.setOutlineThickness(1.0f);
	zoomBorder.setOrigin(Vector2f(zoomBorder.getSize().x / 2, zoomBorder.getSize().y / 2));

	RectangleShape zoomBorder2(Vector2f(width * 2, height * 2));
	zoomBorder2.setFillColor(Color(0, 0, 0, 0));
	zoomBorder2.setOutlineColor(Color(0, 126, 126, 126));
	zoomBorder2.setOutlineThickness(1.0f);
	zoomBorder2.setOrigin(Vector2f(zoomBorder2.getSize().x / 2, zoomBorder2.getSize().y / 2));
	
	Plane.Initialize();
	mandelTexture = mandelbrot(width, height, Plane.get_Rx(), Plane.get_Rw(), Plane.get_Ry(), Plane.get_Rh(), Plane.getZoom());

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	Text CenterText;
	CenterText.setFont(font);
	CenterText.setFillColor(sf::Color::White);
	CenterText.setCharacterSize(24);
	

	while (window.isOpen())
	{
		Event evnt;
		while (window.pollEvent(evnt))
		{
			
			if (evnt.type == Event::Closed)
            {
                window.close();
			}
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			Plane.ZoomIn(zoomBorder);
			mandelTexture = mandelbrot(width, height, Plane.get_Rx(), Plane.get_Rw(), Plane.get_Ry(), Plane.get_Rh(), Plane.getZoom());
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			in = false;
			Plane.ZoomOut(zoomBorder2, zoomBorder);
			mandelTexture = mandelbrot(width, height, Plane.get_Rx(), Plane.get_Rw(), Plane.get_Ry(), Plane.get_Rh(), Plane.getZoom());
			
		}

		Plane.ZoomIn2(zoomBorder);
		CenterText.setPosition(Vector2f(0, 32));
		CenterText.setString(Plane.LoadText());
		
		zoomBorder.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);

		mandelSprite.setTexture(mandelTexture);

		window.clear(sf::Color::White);
		window.draw(mandelSprite);
		window.draw(CenterText);


		if (in == true)
		{
			window.draw(zoomBorder);
		}
		else
		{
			window.draw(zoomBorder2);
		}

		window.display();
	}

	return 0;
}

Texture mandelbrot(int width, int height, double xmin, double xmax, double ymin, double ymax, int iterations)
{
	Texture texture;
	texture.create(width, height);

	Uint8* pixels = new Uint8[width * height * 4];

	for (int index = 0; index < width; index++)
	{
		for (int i = 0; i < height; i++)
		{
			double x = xmin + (xmax - xmin) * index / (width - 1.0);
			double y = ymin + (ymax - ymin) * i / (height - 1.0);

			int Ipoint = Plane.FindMandle(x, y);

			int ppos = 4 * (width * i + index);

			int hue = ((Ipoint * Ipoint) % 256);
			int sat = ((sqrt(static_cast<int>(Ipoint) % 256)));
			int val = ((Ipoint * Ipoint) % 256);

			Color IterationstoRGB = Color (hue, sat, val);
			pixels[ppos] = (int)IterationstoRGB.r;
			pixels[ppos + 1] = (int)IterationstoRGB.g;
			pixels[ppos + 2] = (int)IterationstoRGB.b;
			pixels[ppos + 3] = 255;
		}
	}

	texture.update(pixels, width, height, 0, 0);

	delete[] pixels;

	return texture;
}
