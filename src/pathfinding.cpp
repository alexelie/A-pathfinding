#include "pathfinding.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <list>
using namespace std;

Node::Node(int x, int y){
    setCoord(x, y);
    closed=0;
    wall=false;

    //length from start to this + estimate length to end
    priority=0;
    lengthFromStart=0;
    estimateToEnd=0;
    come_from=NULL;
    if (!texture.loadFromFile("tile2.png",sf::IntRect(0,0,9,9))){
        cout<<"-__-"<<endl;
    }
    sprite.setTexture(texture);
    sprite.setColor(sf::Color::White);
}

Node::Node(){
    closed=0;
    wall=false;

    //length from start to this+estimate length to end
    priority=0;
    lengthFromStart=0;
    estimateToEnd=0;
    come_from=NULL;
    if (!texture.loadFromFile("tile2.png",sf::IntRect(0,0,9,9))){
        cout<<"-__-"<<endl;
    }
    sprite.setTexture(texture);
    sprite.setColor(sf::Color::White);
}

sf::Sprite& Node::getSprite(){
    return this->sprite;
}
void Node::setWall(bool w){
    this->wall=w;
}
bool Node::getWall(){
    return this->wall;
}
void Node::setCoord(int x, int y){
    this->pos_x=x;
    this->pos_y=y;
    sprite.setPosition((pos_x*10),(pos_y*10));
}
void Node::setComeFrom(Coord* c){
    this->come_from=c;
}
void Node::setPriority(int priority){
    this->priority=priority;
}
void Node::setLengthFromStart(int length){
    this->lengthFromStart=length;
}
void Node::setEstimateToEnd(int est){
    this->estimateToEnd=est;
}
Coord* Node::getComeFrom(){
    return come_from;
}
int Node::getPriority(){
    return priority;
}
int Node::getLengthFromStart(){
    return lengthFromStart;
}
int Node::getEstimateToEnd(){
    return estimateToEnd;
}
void Coord::print(){
    cout << "(" << this->element.pos_x << "," << this->element.pos_y << ")" << endl;
}
bool Coord::operator==(const Coord* rhs){
    return ((this->element.pos_x == rhs->element.pos_x) && (this->element.pos_y == rhs->element.pos_y));
}


Coord** createGraph(int nbCol,int nbRow){
    //retourne référence vers le tableau/graph 2d initial
    //en principe, les objets Coord devraient être désalloués avant la fin du programme...
    Coord** g = new Coord*[nbRow];

    for(int i=0; i<nbRow; ++i){

        g[i] = new Coord[nbCol];
        for(int j = 0; j < nbCol; ++j){
            Coord c;
            c.element = Node(j, i);
            g[i][j] = c;
        }
    }
    return g;
}

int heuristicTime(int x1, int y1,int x2,int y2){
    int width=abs(x1-x2)*10;
    int height=abs(y1-y2)*10;
    int hyp=sqrt(pow(width,2.0)+pow(height,2.0));
    return hyp;
}

Coord* find_maxPriority(const list<Coord*>& liste){

    Coord* priority;
    Coord* current;
    priority=liste.front();
    for (list<Coord*>::const_iterator it = liste.begin(); it != liste.end(); ++it) {
        current=*it;
        if(current->element.getPriority()<priority->element.getPriority()){
            priority=current;
        }
    }
    return priority;
}

list<Coord*> find_neighbors(Coord** g,int size_x,int size_y,Coord* current){
    list<Coord*> liste;
    int x=current->element.pos_x;
    int y=current->element.pos_y;
    if(y+1<size_y)liste.push_back(&(g[y+1][x]));
    if(y+1<size_y&&x+1<=size_x)liste.push_back(&(g[y+1][x+1]));
    if(x+1<=size_x)liste.push_back(&(g[y][x+1]));
    if(y-1>=0&&x+1<=size_x)liste.push_back(&(g[y-1][x+1]));
    if(y-1>=0)liste.push_back(&(g[y-1][x]));
    if(y-1>=0&&x-1>=0)liste.push_back(&(g[y-1][x-1]));
    if(x-1>=0)liste.push_back(&(g[y][x-1]));
    if(y+1<size_y&&x-1>=0)liste.push_back(&(g[y+1][x-1]));
    return liste;
}

bool coord_in_list(const Coord* coord, const list<Coord*> liste){

    for(list<Coord*>::const_iterator it=liste.begin(); it!=liste.end(); it++){
        Coord* current=*it;
        if(current==coord){
            return true;
        }
    }
    return false;
}

bool is_diag(Coord* current, Coord* neighbor){
    if((neighbor->element.pos_x==(current->element.pos_x+1)&&neighbor->element.pos_y==(current->element.pos_y+1))
       ||(neighbor->element.pos_x==(current->element.pos_x-1)&&neighbor->element.pos_y==(current->element.pos_y+1))
       ||(neighbor->element.pos_x==(current->element.pos_x-1)&&neighbor->element.pos_y==(current->element.pos_y-1))
       ||(neighbor->element.pos_x==(current->element.pos_x+1)&&neighbor->element.pos_y==(current->element.pos_y-1))){
        return true;
    }
    return false;

}
list<Coord*> findPath(sf::RenderWindow& window,Coord** g,int size_x,int size_y,int xstart,int ystart,int xend,int yend){

    //A* lists
    //noeuds à vérifier, noeuds déjà vérifiés
    list<Coord*> open_nodes,closed_nodes;

    Coord startPos = g[ystart][xstart];
    Coord destPos = g[yend][xend];

    startPos.element.setPriority(0 + heuristicTime(xstart,ystart,xend,yend));

    open_nodes.push_back(&startPos);

    while(open_nodes.empty()!= true){

        sf::Color color;
        Coord* current = find_maxPriority(open_nodes);

        //Si on est arrivé au but
        if (current->element.pos_x == destPos.element.pos_x && current->element.pos_y == destPos.element.pos_y){

            //reconstruct path
            //TODO: refactor dans fonction
            list<Coord*> path;
            Coord* curr=current;
            path.push_back(curr);

            while(curr->element.getComeFrom() != NULL){

                curr->element.getSprite().setColor(sf::Color::Blue);
                curr=curr->element.getComeFrom();
                path.push_back(curr);
            }
            curr->element.getSprite().setColor(sf::Color::Red);
            path.push_back(curr);

            return path;
        }

        //changement de liste de "à vérifier" vers "vérifié"
        open_nodes.remove(current);
        closed_nodes.push_back(current);

        list<Coord*> neighbors=find_neighbors(g,size_x,size_y,current);

        //voir ts les voisins et updater priority lengthFromStart estimateToEnd etc...
        for (list<Coord*>::iterator it = neighbors.begin(); it != neighbors.end(); it++){

            Coord* currNeighbor=*it;

            //vérifie que le voisin choisi n'est pas déjà vérifié ou que ce n'est pas un mur
            bool closed = coord_in_list(currNeighbor,closed_nodes);
            if (closed == true || currNeighbor->element.getWall() == true){
                continue;
            }

            //Ajout de couleur au voisin choisi.
            //currNeighbor->element.getSprite().setColor(sf::Color::Magenta);
            //window.draw(currNeighbor->element.getSprite());
            //window.display();

            //distance from start du voisin choisi
            int distFromStart = current->element.getLengthFromStart() + heuristicTime(currNeighbor->element.pos_x, currNeighbor->element.pos_y, current->element.pos_x, current->element.pos_y);
            bool is_in_open = coord_in_list(currNeighbor,open_nodes);

            //on update l'état du voisin seulement si on obtient un distance moins grande venant du currentNode
            if (is_in_open == false || distFromStart < currNeighbor->element.getLengthFromStart()){

                currNeighbor->element.setComeFrom(current);

                int DistanceFromCurrentToNeighbor;
                if(!is_diag(current,currNeighbor)){
                    DistanceFromCurrentToNeighbor = 10;
                }else{
                    DistanceFromCurrentToNeighbor = 14;
                }

                currNeighbor->element.setLengthFromStart(current->element.getLengthFromStart() + DistanceFromCurrentToNeighbor);
                currNeighbor->element.setPriority(currNeighbor->element.getLengthFromStart() + heuristicTime(currNeighbor->element.pos_x, currNeighbor->element.pos_y, xend, yend));

                //si le voisin n'est pas déjà dans open_list pour être un candidat à considérer, on l'ajoute
                if(is_in_open == false){
                    open_nodes.push_back(currNeighbor);
                }
            }
        }

    }
    int notFound=1;
    throw notFound;
}
