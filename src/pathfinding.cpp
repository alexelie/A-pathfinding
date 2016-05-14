#include "pathfinding.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <list>
using namespace std;

Node::Node(){
    closed=0;
    wall=false;
    priority=0;//length from start to this+estimate length to end
    lengthFromStart=0;
    estimateToEnd=0;
    come_from=NULL;
    if (!texture.loadFromFile("tile2.png",sf::IntRect(0,0,10,10))){/*ereur*/cout<<"wo"<<endl;}
    sprite.setTexture(texture);
    //sprite.setTextureRect(sf::IntRect(10,10, 20, 25));

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
    sprite.setPosition((pos_x*10+2*pos_x),(pos_y*10+2*pos_y));
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
    cout<<"("<<x<<","<<y<<")"<<endl;
}
bool Coord::operator==(const Coord* rhs){
    return ((this->x==rhs->x)&&(this->y==rhs->y));
}


Coord** createGraph(int width,int height){//retourne un tableau 2d de Coord aucun obstacle
    Coord** g=new Coord*[height];

    for(int i=0;i<height;++i){
        g[i]=new Coord[width];
        for(int j=0;j<width;++j){
            Coord c;
            Node node;
            c.x=j;
            c.y=i;
            //c.id=i+j;
            c.element=node;
            c.element.setCoord(j,i);
            g[i][j]=c;
        }
    }
    return g;
}

int estimateTime(int x1, int y1,int x2,int y2){
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
    int x=current->x;
    int y=current->y;
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

bool coord_in_list(const Coord* coord, const list<Coord*> liste){//utilisé pour chercher un noeud dans un std::list à partir de pointeur
    for(list<Coord*>::const_iterator it=liste.begin(); it!=liste.end(); it++){
        Coord* current=*it;
        if(current==coord){
            return true;
        }
    }
    return false;
}
bool is_diag(Coord* current, Coord* neighbor){
    if((neighbor->x==(current->x+1)&&neighbor->y==(current->y+1))
       ||(neighbor->x==(current->x-1)&&neighbor->y==(current->y+1))
       ||(neighbor->x==(current->x-1)&&neighbor->y==(current->y-1))
       ||(neighbor->x==(current->x+1)&&neighbor->y==(current->y-1))){
        return true;
    }
    return false;

}
list<Coord*> findPath(sf::RenderWindow& window,Coord** g,int size_x,int size_y,int xstart,int ystart,int xend,int yend){//g est le tableau de coord(struct) contenant les objets nodes et la position
    list<Coord*> open_nodes,closed_nodes;
    int xs=xstart;
    int ys=ystart;
    int xe=xend;
    int ye=yend;
    Coord start=g[ys][xs];
    Coord dest=g[ye][xe];
    start.element.setPriority(estimateTime(xs,ys,xe,ye));
    open_nodes.push_back(&start);
    while(open_nodes.empty()!=true){//open_nodes.empty()!=true){
        sf::Color color;
        Coord* current=find_maxPriority(open_nodes);//current est le noeud ds open_node avec la meilleure estimation pour arriver à end node
        //current->element.getSprite().setColor(color.Cyan);
        if (current->x==dest.x&&current->y==dest.y){
            cout<<"PATH FOUND "<<endl;
            //reconstruct path
            list<Coord*> path;
            Coord* curr=current;
            path.push_back(curr);
            while(curr->element.getComeFrom()!=NULL){
                curr->print();
                curr->element.getSprite().setColor(color.Blue);
                curr=curr->element.getComeFrom();
                path.push_back(curr);
            }
            curr->print();
            curr->element.getSprite().setColor(color.Red);
            path.push_back(curr);
            return path;
        }
        open_nodes.remove(current);
        closed_nodes.push_back(current);
        list<Coord*> neighbors=find_neighbors(g,size_x,size_y,current);
        //voir ts les voisins et updater priority lengthFromStart estimateToEnd etc...
        for (list<Coord*>::iterator it = neighbors.begin(); it != neighbors.end(); it++){
            Coord* currNeighbor=*it;

            bool closed=coord_in_list(currNeighbor,closed_nodes);
            if (closed == true || currNeighbor->element.getWall()==true)//si le voisin est deja ds closed nodes on fait rien
                continue;
            currNeighbor->element.getSprite().setColor(color.Magenta);
            window.draw(currNeighbor->element.getSprite());
            window.display();
            //calculer lengthFromStart
            //currNeighbor->element.getSprite().setColor(color.Magenta);
            int priority=current->element.getLengthFromStart()+estimateTime(currNeighbor->x,currNeighbor->y,current->x,current->y);
            bool open=coord_in_list(currNeighbor,open_nodes);
            if (open==false || priority<currNeighbor->element.getLengthFromStart()){//si neighbor n'est pas ds open_node ou si la nouv priorité<ancienne priorité du neighbor
                currNeighbor->element.setComeFrom(current);//on modifie le "parent" du neighbor
                if(is_diag(current,currNeighbor)==false){//assignation de la nouvelle longueur depuis depart:+2 si horiz ou vert
                    currNeighbor->element.setLengthFromStart(current->element.getLengthFromStart()+10);
                    currNeighbor->element.setPriority(currNeighbor->element.getLengthFromStart()+estimateTime(currNeighbor->x,currNeighbor->y,xe,ye));
                }else{//+3 si diagonal
                    currNeighbor->element.setLengthFromStart(current->element.getLengthFromStart()+14);
                    currNeighbor->element.setPriority(currNeighbor->element.getLengthFromStart()+estimateTime(currNeighbor->y,currNeighbor->y,xe,ye));
                }
                if(open==false){//si it n'est pas ds open_node, on lajoute
                    open_nodes.push_back(currNeighbor);
                }
            }
        }

    }
    int notFound=1;
    throw notFound;
}
