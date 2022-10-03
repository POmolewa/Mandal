#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class ComplexPlane 
{
    public:
        void SetCenterMouseCoords(double x, double y);
        void Initialize();
        int getZoom();
        void 
            ZoomIn(RectangleShape),
            ZoomIn2(RectangleShape),
            ZoomOut(RectangleShape, RectangleShape);
        double MakeReal(double value, double localMin, double localMax, double min, double max);
        double FindMandle(double value, double localMin);
        double
            get_Rx(),
            get_Ry(),
            get_Rw(),
            get_Rh();
        string LoadText();


    private:
    const unsigned int MAX_ITER = 64;
    const float BASE_WIDTH = 4.0;
    const float BASE_HEIGHT = 4.0;
    const float BASE_ZOOM = 0.5;

    int width = VideoMode::getDesktopMode().width;
    int height = VideoMode::getDesktopMode().height;

    double 
        Xmin = -4.0,
        XMax = 4.0,
        Ymin = ((-1 * (abs(Xmin) + abs(XMax)) * height / width)) / 2,
        YMax = ((abs(Xmin) + abs(XMax)) * height / width) / 2;

    double
        MouseX,
        MouseY,
        RealX,
        RealY,
        RealX2,
        RealY2,
        RealWidth,
        RealHeight;

};





#endif