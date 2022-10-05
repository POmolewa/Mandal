#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "ComplexPlane.h"
#include <sstream>

const int NUM_VERTICES_IN_SET = 1;

sf::Texture mandelbrot(int width, int height, double xmin, double xmax, double ymin, double ymax, int iterations, ComplexPlane&);
//ComplexPlane Plane;

using namespace sf;
using namespace std;
int main()
{
    bool in = true;

    //Need this to create the View object
    int widthInt = VideoMode::getDesktopMode().width;
    int heightInt = VideoMode::getDesktopMode().height;

    float width = widthInt;
    float height = heightInt;

    // me -----------------
    float aspectRatio = height / width;

    // Passing aspect ratio when declaring the ComplexPlane obj
    ComplexPlane Plane(aspectRatio);

    // View object for zoom rectangle
    View recView(FloatRect(0, 0, 1920, 1080));


    // --------------------------------

    RenderWindow window(sf::VideoMode(width, height), "Project");

    window.setView(Plane.getView());



    Texture mandelTexture;
    Sprite mandelSprite;

    RectangleShape zoomBorder(Vector2f(width / 2, height / 2));     // rectangle the user manipulates by moving mouse
    zoomBorder.setFillColor(Color(0, 0, 0, 0));
    zoomBorder.setOutlineColor(Color(0, 126, 126, 126));
    zoomBorder.setOutlineThickness(1.0f);
    zoomBorder.setOrigin(Vector2f(zoomBorder.getSize().x / 2, zoomBorder.getSize().y / 2));
    //zoomBorder.setOrigin(Vector2f(0, 0));

    RectangleShape zoomBorder2(Vector2f(width * 2, height * 2));        // same as above??
    zoomBorder2.setFillColor(Color(0, 0, 0, 0));
    zoomBorder2.setOutlineColor(Color(0, 126, 126, 126));
    zoomBorder2.setOutlineThickness(1.0f);
    zoomBorder2.setOrigin(Vector2f(zoomBorder2.getSize().x / 2, zoomBorder2.getSize().y / 2));

    //Plane.Initialize();     // ComplexPlane object
    //mandelTexture = mandelbrot(width, height, Plane.get_Rx(), Plane.get_Rw(), Plane.get_Ry(), Plane.get_Rh(), Plane.getZoom(), Plane);

    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    Text CenterText;
    CenterText.setFont(font);
    CenterText.setFillColor(sf::Color::White);
    CenterText.setCharacterSize(24);



    // me --------------------------------------------
    Vector2f mouseWorldPosition;


    VertexArray vertexArray;
    vertexArray.setPrimitiveType(Points);
    vertexArray.resize(width * height * NUM_VERTICES_IN_SET);

    enum class State { CALCULATING, DISPLAYING };
    State currentJob = State::CALCULATING;


    // ---------------------------------------------------------

    float pixelWidthh = 4.0 / width;
    // FIRST UPDATE ***********************************************************************************************8
    if (currentJob == State::CALCULATING)
    {
        int counter = 0;
        for (int j = 0; j < widthInt; j++)
        {

            for (int i = 0; i < heightInt; i++)
            {
                float jj = j;
                float ii = i;
                vertexArray[j + i * widthInt].position = { jj, ii };




                Vector2f worldCoordinatesToPass = window.mapPixelToCoords(Vector2i((float)j, (float)i));
                // experiment
                //worldCoordinatesToPass.x = worldCoordinatesToPass.x * 0.8;
                vertexArray[j + i * widthInt].position = worldCoordinatesToPass;

                size_t numIterations = Plane.countIterations(worldCoordinatesToPass);
                //size_t numIterations = Plane.countIterations( window.mapPixelToCoords(Vector2i((float)j, (float)i   )) );


                Uint8 r = 200;
                Uint8 g = 5;
                Uint8 b = 0;

                Plane.iterationsToRGB(numIterations, r, g, b);

                Color color(r, g, b);

                vertexArray[j + i * widthInt].color = color;
                //vertexArray[j + i].color = color;



                counter++;
            }


        }



        currentJob = State::DISPLAYING;
    }


    // ********************* MAIN LOOP **************************************************************
    bool firstLoopRun = true;
    while (window.isOpen())
    {



        window.setView(Plane.getView());

        //cout << "debug5" << endl; // when not moving mouse this prints

        // EXPERIMENTAL ADDED JUST ADDED -------------------------
        if (currentJob == State::CALCULATING)
        {

            int counter = 0;
            for (int j = 0; j < widthInt; j++)
            {
                for (int i = 0; i < heightInt; i++)
                {

                    vertexArray[j + i * widthInt].position = { (float)j, (float)i };

                    Vector2f worldCoordinatesToPass = window.mapPixelToCoords(Vector2i((float)j, (float)i));


                    //worldCoordinatesToPass.x = worldCoordinatesToPass.x * 0.25;
                    //worldCoordinatesToPass.y = worldCoordinatesToPass.y * 0.25;
                    vertexArray[j + i * widthInt].position = worldCoordinatesToPass;

                    size_t numIterations = Plane.countIterations(worldCoordinatesToPass);

                    Uint8 r = 200;
                    Uint8 g = 5;
                    Uint8 b = 0;

                    Plane.iterationsToRGB(numIterations, r, g, b);

                    Color color(r, g, b);

                    vertexArray[j + i * widthInt].color = color;

                    counter++;





                }

            }



            currentJob = State::DISPLAYING;
        }
        // --------------------------------------------------------------------------------



        mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(window)); // testing
        // output 
        cout << mouseWorldPosition.x << endl;
        cout << mouseWorldPosition.y << endl;

        Plane.setMouseLocation(mouseWorldPosition);
        cout << endl << endl;



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
            mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(window));
            Plane.myZoomIn(mouseWorldPosition);           // deactivate
            window.setView(Plane.getView());    // this needs to be here otherwise it will not take into account the changes made to Plane.m_view

            // me --------------------------------

            //Converting current mouse pixel coords to world coords
            mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(window));


            Plane.setMouseLocation(mouseWorldPosition);




            // ------------------------------

            currentJob = State::CALCULATING;
            //if (currentJob == State::CALCULATING)
            //{
                //mandelTexture = mandelbrot(width, height, Plane.get_Rx(), Plane.get_Rw(), Plane.get_Ry(), Plane.get_Rh(), Plane.getZoom(), Plane);
                //currentJob = State::DISPLAYING;             
            //}
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            //cout << "debug1" << endl;
            mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(window));
            Plane.myZoomOut(mouseWorldPosition);
            window.setView(Plane.getView());            // THIS COULD BE KEY TO RECTANGLE ISSUE

            mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(window));
            Plane.setMouseLocation(mouseWorldPosition);

            currentJob = State::CALCULATING;





            //currentJob = State::CALCULATING;
            in = false;
            //Plane.ZoomOut(zoomBorder2, zoomBorder);

        }

        //Plane.ZoomIn2(zoomBorder);
        //CenterText.setPosition(Vector2f(0, 32));      // optimizing
        CenterText.setString(Plane.LoadText(mouseWorldPosition));       // optimizing

        zoomBorder.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);     // optimizing
        //window.draw(zoomBorder);
        cout << "TEST" << endl;         // ZOOM WORK


        window.draw(vertexArray);

        window.setView(recView);
        CenterText.setPosition(1, 1);

        //window.draw(zoomBorder);
        window.draw(CenterText);

        window.display();
        //window.clear();
        firstLoopRun = false;
    }

    return 0;
}

Texture mandelbrot(int width, int height, double xmin, double xmax, double ymin, double ymax, int iterations, ComplexPlane& Plane)
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

            Color IterationstoRGB = Color(hue, sat, val);
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


// me-----------------------------

Vector2f mapPixelToCoords(int x, int y)
{

    Vector2f thing;
    return thing;
}
