#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "ComplexPlane.h"



double RealValue(double value, double localMin, double localMax, double min, double max);
double FindMandle(double cx, double cy, int maxIter);
sf::Texture mandelbrot(int width, int height, double xmin, double xmax, double ymin, double ymax, int iterations);

using namespace sf;
using namespace std;
int main()
{
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

	double oxmin = -4.0;
	double oxmax = 4.0;
	double oyRange = (abs(oxmin) + abs(oxmax)) * height / width;
	double oymin = -oyRange / 2;
	double oymax = oyRange / 2;

	double xmin = oxmin;
	double xmax = oxmax;
	double yRange = oyRange;
	double ymin = oymin;
	double ymax = oymax;

	//int MaxIterations = 64;
	int recLevel = 1;
	Zoom Zoomwindow; // Remove
	mandelTexture = mandelbrot(width, height, oxmin, oxmax, oymin, oymax, 100);

	sf::Font font;
	//put a specific font font.loadFromFile("");

	sf::Text zoomText, precText;
	zoomText.setFont(font);
	precText.setFont(font);
	zoomText.setFillColor(sf::Color::White);
	precText.setFillColor(sf::Color::White);
	zoomText.setCharacterSize(24);
	precText.setCharacterSize(24);

	while (window.isOpen())
	{
		Event evnt;
		while (window.pollEvent(evnt))
		{
			
			if (evnt.type == Event::Closed)
            {
                window.close();															//remove
				mandelTexture = mandelbrot(width, height, xmin, xmax, ymin, ymax, Zoomwindow.getZoom());
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			recLevel++;

			double x = zoomBorder.getPosition().x - zoomBorder.getSize().x / 2;
			double y = zoomBorder.getPosition().y - zoomBorder.getSize().y / 2;

			double x2 = x + zoomBorder.getSize().x;
			double y2 = y + zoomBorder.getSize().y;

			//from px range to grid range
			double Realx = RealValue(x, 0.0, width, xmin, xmax);
			double Realy = RealValue(y, 0.0, height, ymin, ymax);

			double widthNorm = RealValue(x2, 0.0, width, xmin, xmax);
			double heightNorm = RealValue(y2, 0.0, height, ymin, ymax);

			xmin = Realx;
			xmax = widthNorm;
			ymin = Realy;
			ymax = heightNorm;

			mandelTexture = mandelbrot(width, height, xmin, xmax, ymin, ymax, Zoomwindow.getZoom());
		}

		zoomText.setString("Zoom: " + std::to_string(pow(8, recLevel - 1)));
		precText.setString("Max. Iterations: " + std::to_string(Zoomwindow.getZoom()));
		precText.setPosition(Vector2f(0, 32));

		zoomBorder.setPosition(Mouse::getPosition(window).x,Mouse::getPosition(window).y);

		mandelSprite.setTexture(mandelTexture);

		window.clear(sf::Color::White);

		window.draw(mandelSprite);
		window.draw(zoomText);
		window.draw(precText);
		window.draw(zoomBorder);

		window.display();
	}

	return 0;
}

double RealValue(double value, double localMin, double localMax, double min, double max)
{
	double Real = (value - localMin) / (localMax - localMin);
	Real = Real * (max - min);
	Real += min;
	return Real;
}

double FindMandle(double cx, double cy, int maxIter) 
{
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while (i < maxIter && zr * zr * zi < 4.0)
	{
		double temp = zr * zr - zi * zi + cx;
		zi = 2.0 * zr * zi + cy;
		zr = temp;
		i++;
	}
	return i;

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

			int Ipoint = FindMandle(x, y, iterations);

			int ppos = 4 * (width * i + index);

			int hue = ((Ipoint * Ipoint) % 256);
			int sat = ((Ipoint * Ipoint) % 256);
			int val = (Ipoint % 256);

			Color hsvtorgb = Color (hue, sat, val);
			pixels[ppos] = (int)hsvtorgb.r;
			pixels[ppos + 1] = (int)hsvtorgb.g;
			pixels[ppos + 2] = (int)hsvtorgb.b;
			pixels[ppos + 3] = 255;
		}
	}

	texture.update(pixels, width, height, 0, 0);

	delete[] pixels;

	return texture;
}
