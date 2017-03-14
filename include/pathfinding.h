#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <SFML/Graphics.hpp>
using namespace std;
#include <list>
struct Coord;

class Node
{
    private:
    int closed;
    int priority;//length from start to this+estimate length to end
    int lengthFromStart;
    int estimateToEnd;
    Coord* come_from;
    public:
        Node(int x, int y);
        Node();
        sf::Texture texture;
        sf::Sprite sprite;
        int pos_x;
        int pos_y;
        bool wall;
        sf::Sprite& getSprite();
        void setWall(bool w);
        bool getWall();
        void setComeFrom(Coord* c);
        void setPriority(int priority);
        void setLengthFromStart(int length);
        void setEstimateToEnd(int est);
        Coord* getComeFrom();
        int getPriority();
        int getLengthFromStart();
        int getEstimateToEnd();
        void setCoord(int x, int y);
};

struct Coord{
    Node element;
    void print();
    bool operator==(const Coord* rhs);
};

Coord** createGraph(int width,int height);
int estimateTime(int x1, int y1,int x2,int y2);
Coord* find_maxPriority(const list<Coord*>& liste);
list<Coord*> find_neighbors(Coord** g,int size_x,int size_y, Coord* current);
bool coord_in_list(const Coord* coord, const list<Coord*> liste);
bool is_diag(Coord* current, Coord* neighbor);
list<Coord*> findPath(sf::RenderWindow& window,Coord** g,int size_x,int size_y,int xstart,int ystart,int xend,int yend);
#endif // PATHFINDING_H
