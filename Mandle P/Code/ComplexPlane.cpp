#include "ComplexPlane.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <complex>

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

string ComplexPlane::LoadText(Vector2f mouseWorldPosition)
{
    cout << "elli" << m_mouseLocation.x << endl;
    string txt1 =
        "MandelBrot Set \n"
        "Next Zoom Coords: " + to_string(RealX2) + " " + to_string(RealY2) + '\n' +
        "Center: " + to_string(MouseX) + " " + to_string(MouseY) +
        "\nLeft Click to Zoom in \n"
        "Right Click to Zoom Out";


    // me -----
    string txt =
        "MandelBrot Set \n"
        "Next Zoom Coords: " + to_string(mouseWorldPosition.x) + " " + to_string(mouseWorldPosition.y) + '\n' +
        "Center: " + to_string(m_view.getCenter().x) + " " + to_string(m_view.getCenter().x) +
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





// me --------------------------------------

//constructor
ComplexPlane::ComplexPlane(float aspectRatioParam)
{
    m_aspectRatio = aspectRatioParam;
    //m_view.setSize(BASE_WIDTH, -BASE_HEIGHT  * m_aspectRatio);    // turning off aspect ratio part here for now
    //m_view.setSize(4.0, -4.0 );


    m_view.setCenter(0.0, 0.0); // Makes the view object center itself around the world coordinates 0.0, 0.0?       // was 0, 0
    m_view.setSize(BASE_WIDTH, -BASE_HEIGHT);       //Sets the "portion" of the world that is viewed by the view object.            // was 4, 4
    // the world coordinates go on to infinity i guess. the "world" is just an abstraction
    m_zoomCount = 0;
}


void ComplexPlane::setMouseLocation(Vector2f mouseWorldPosition)
{
    //MouseX = mouseWorldPosition.x;
    //MouseY = mouseWorldPosition.y;

    m_mouseLocation = mouseWorldPosition;


}

// ZOOMING
void ComplexPlane::myZoomIn(Vector2f mousePos)
{
    m_zoomCount++;
    cout << "sdfsdfsdfdsgfhdgkdjshgkjdsghkfjghfkgfkfkfgkfgjfg";

    float localXSize = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    float localYSize = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_view.setSize(localXSize, localYSize);
    m_view.setCenter(mousePos);

    cout << "x size is now" << localXSize << "y siwe is now " << localYSize << endl;







}

void ComplexPlane::myZoomOut(Vector2f mousePos)
{
    m_zoomCount--;

    float localXSize = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    float localYSize = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_view.setSize(localXSize, localYSize);
    m_view.setCenter(mousePos);
}

void ComplexPlane::setCenter(Vector2f coord)
{
    m_view.setCenter(coord);
}



size_t ComplexPlane::countIterations(Vector2f coord)
{
    // x value is real, y value is imaginary

    //float absVal = sqrt( (coord.x * coord.x) + (coord.y * coord.y) );    //manual Pythagorean way
    //absVal = abs(absVal);

    complex<double> complexNum(coord.x, coord.y);
    //float absVal = abs(complexNum); // this one

    complex<double> z = { 0.0, 0.0 };
    size_t counter = 0;

    while (counter != 64)
    {
        z = (z * z) + complexNum;
        counter++;


        //cout << endl << "z: " << z << endl;

        if (abs(z) > 2.0)
        {
            return counter;

        }

        if (abs(z) < 2.0 && counter == MAX_ITER)
        {
            return counter;
        }


    }


}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    // stuff

    if (count == 64)
    {

        r = 0;
        g = 0;
        b = 0;
        //Color color1(r, g, b);

    }

    else if (count > 60 && count < 64)
    {
        r = 50;
        g = 70;
        b = 90;
        //Color color2(r, g, b);
    }

    else if (count > 55 && count < 60)
    {
        r = 100;
        g = 130;
        b = 200;
        //Color color3(r, g, b);
    }

    else if (count > 50 && count < 55)
    {
        r = 200;
        g = 200;
        b = 200;
        //Color color4(r, g, b);
    }

    else if (count > 45 && count < 50)
    {
        r = 275;
        b = 250;
        g = 200;
        //Color color5(r, g, b);
    }

    else if (count > 40 && count < 45)
    {
        r = 245;
        g = 20;
        b = 244;
        //Color color6(r, g, b);

    }
    else if (count > 35 && count < 40)
    {
        r = 0;
        g = 10;
        b = 40;
        //Color color7(r, g, b);
    }

    // here
    else if (count > 35 && count < 40)
    {
        r = 0;
        g = 10;
        b = 40;
        //Color color7(r, g, b);
    }

    else if (count > 30 && count < 35)
    {
        r = 80;  //232
        g = 111;
        b = 230;
    }

    else if (count > 25 && count < 30)
    {
        r = 12;
        g = 45;
        b = 140;
    }

    else if (count > 20 && count < 25)
    {
        r = 100;
        g = 50;
        b = 150;
    }

    else if (count > 15 && count < 20)
    {
        r = 200;
        g = 175;
        b = 43;
    }
    else if (count > 10 && count < 15)
    {
        r = 167;
        g = 233;
        b = 233;
    }

    else if (count > 5 && count < 15)
    {
        r = 240;
        g = 20;
        b = 248;
    }

    else
    {

        r = 20;
        g = 40;
        b = 70;
    }


}

View ComplexPlane::getView()
{
    return m_view;
}
