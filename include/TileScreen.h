#ifndef TILESCREEN_H
#define TILESCREEN_H
#include <SFML/Graphics.hpp>
#include "pathfinding.h"
class TileScreen
{
    public:
        sf::RenderWindow window;
        Coord** graph;
        sf::Texture texture;
        sf::Sprite sprite;
        TileScreen(int size_X,int size_y);
        virtual ~TileScreen();
        void setStart(int x, int y);
        void setEnd(int x, int y);
        list<Coord*> pathFind(int departX,int departY,int finX,int finY);
        int getWidth();
        int getHeight();
    protected:
    private:
        void updateGrid();
        int width;
        int height;
        int dX;
        int dY;
        int eX;
        int eY;
        int wX;
        int wY;
};

#endif // TILESCREEN_H
