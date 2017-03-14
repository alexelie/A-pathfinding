#ifndef TILESCREEN_H
#define TILESCREEN_H
#include <SFML/Graphics.hpp>
#include "pathfinding.h"
class TileScreen
{
    public:

        sf::RenderWindow window;

        //doit être un type ** car on ne sait pas d'avance la grandeur du graph
        //on doit allouer au runtime
        Coord** graph;
        sf::Texture texture;
        sf::Sprite sprite;
        TileScreen(int size_X,int size_y);
        //virtual ~TileScreen();
        void go();
        void setStart(int x, int y);
        void setEnd(int x, int y);
        list<Coord*> startPathFinder(int departX,int departY,int finX,int finY);
        int getWidth();
        int getHeight();
    protected:
    private:
        bool isDone = false;
        void updateGrid();
        void initGrid();
        void displayInitialGrid();
        int nbCol;
        int nbRow;
        int userEventCount;
        bool drawingWalls;
        int startColPos;
        int startRowPos;
        int endColPos;
        int endRowPos;
};

#endif // TILESCREEN_H
