#include "ComplexPlane.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

void ComplexPlane::Initialize()
{
	RealX = Xmin;
	RealWidth = XMax;
	RealY = Ymin;
	RealHeight = YMax;
}

int ComplexPlane::getZoom()
{
	return MAX_ITER;
}


void ComplexPlane::ZoomIn(RectangleShape zoomBorder)
{
	double x = zoomBorder.getPosition().x - zoomBorder.getSize().x / 2;
	double y = zoomBorder.getPosition().y - zoomBorder.getSize().y / 2;

	double x2 = x + zoomBorder.getSize().x;
	double y2 = y + zoomBorder.getSize().y;

	RealX = MakeReal(x, 0.0, width, Xmin, XMax);
	RealY = MakeReal(y, 0.0, height, Ymin, YMax);

	RealWidth = MakeReal(x2, 0.0, width, Xmin, XMax);
	RealHeight = MakeReal(y2, 0.0, height, Ymin, YMax);

	Xmin = RealX;
	XMax = RealWidth;
	Ymin = RealY;
	YMax = RealHeight;

	SetCenterMouseCoords(RealX2, RealY2);

}
void ComplexPlane::ZoomIn2(RectangleShape zoomBorder2)
{
	double x = zoomBorder2.getPosition().x - zoomBorder2.getSize().x / 2;
	double y = zoomBorder2.getPosition().y - zoomBorder2.getSize().y / 2;

	double x2 = x + zoomBorder2.getSize().x;
	double y2 = y + zoomBorder2.getSize().y;

	RealX2 = MakeReal(x, 0.0, width, Xmin, XMax);
	RealY2 = MakeReal(y, 0.0, height, Ymin, YMax);
}

void ComplexPlane::ZoomOut(RectangleShape zoomBorder2, RectangleShape zoomBorder)
{
	double x = zoomBorder.getPosition().x - zoomBorder2.getSize().x / 2;
	double y = zoomBorder.getPosition().y - zoomBorder2.getSize().y / 2;

	double x2 = x + zoomBorder2.getSize().x;
	double y2 = y + zoomBorder2.getSize().y;

	RealX = MakeReal(x, 0.0, width, Xmin, XMax);
	RealY = MakeReal(y, 0.0, height, Ymin, YMax);

	RealWidth = MakeReal(x2, 0.0, width, Xmin, XMax);
	RealHeight = MakeReal(y2, 0.0, height, Ymin, YMax);

	Xmin = RealX;
	XMax = RealWidth;
	Ymin = RealY;
	YMax = RealHeight;

	SetCenterMouseCoords(RealX2, RealY2);
}


double ComplexPlane::MakeReal(double value, double localMin, double localMax, double min, double max)
{
	double Real = (value - localMin) / (localMax - localMin);
	Real = Real * (max - min);
	Real += min;
	return Real;
}

double ComplexPlane::FindMandle(double cx, double cy)
{
	int i = 0;
	double zr = 0.0, zi = 0.0;
	while (i < MAX_ITER && zr * zr * zi < 4.0)
	{
		double temp = zr * zr - zi * zi + cx;
		zi = 2.0 * zr * zi + cy;
		zr = temp;
		i++;
	}
	return i;
}

void ComplexPlane::SetCenterMouseCoords(double x, double y)
{
	MouseX = x;
	MouseY = y;
}

string ComplexPlane::LoadText()
{
	string txt =
		"MandelBrot Set \n"
		"Next Zoom Coords: "  + to_string(RealX2) + " " + to_string(RealY2) + '\n' +
		"Center: " + to_string(MouseX) + " " + to_string(MouseY) +
		"\nLeft Click to Zoom in \n"
		"Right Click to Zoom Out";

	return txt;
	
}

double ComplexPlane::get_Rx()
{
	return RealX;
}
double ComplexPlane::get_Ry()
{
	return RealY;
}
double ComplexPlane::get_Rw()
{
	return RealWidth;
}
double ComplexPlane::get_Rh()
{
	return RealHeight;
}

