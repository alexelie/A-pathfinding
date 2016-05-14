#include "TileScreen.h"
#include <SFML/Graphics.hpp>
#include "pathfinding.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <list>
TileScreen::TileScreen(int x, int y){
    window.create(sf::VideoMode(720, 720), "Pathfinding");
    graph=createGraph(x,y);//retourne un pointeur vers un tableau de Coord(noeuds)
    sf::Color color;
    /*for(int i=18;i<51;i++){
        graph[20][i].element.getSprite().setColor(color.Black);
        graph[20][i].element.setWall(true);
    }
    for(int i=23;i<=60;i++){
        graph[35][i].element.getSprite().setColor(color.Black);
        graph[35][i].element.setWall(true);
    }
    for(int i=5;i<=17;i++){
        graph[35][i].element.getSprite().setColor(color.Black);
        graph[35][i].element.setWall(true);
    }
    for(int i=5;i<=55;i++){
        graph[i][35].element.getSprite().setColor(color.Black);
        graph[i][35].element.setWall(true);
    }*/
    width=x;
    height=y;
    int cpt=0;
    updateGrid();
    //pathFind(dx,dy,ex,ey);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            switch(event.type){

                case sf::Event::Closed:
                    window.close();
                break;
                case sf::Event::KeyPressed:
                    if(event.key.code==sf::Keyboard::Return){
                        pathFind(dX,dY,eX,eY);
                        updateGrid();
                    }
                break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if(cpt==0){
                            dX=(floor(mousePos.x/10))-(2*(floor(mousePos.x/10)/10));
                            dY=floor(mousePos.y/10)-(2*(floor(mousePos.y/10)/10));
                            graph[dY][dX].element.getSprite().setColor(color.Green);
                            updateGrid();
                        }else if(cpt==1){
                            eX=floor(mousePos.x/10)-(2*(floor(mousePos.x/10)/10));
                            eY=floor(mousePos.y/10)-(2*(floor(mousePos.y/10)/10));
                            graph[eY][eX].element.getSprite().setColor(color.Red);
                            updateGrid();
                        }
                        cpt++;
                    }
                break;

                default:
                break;
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if(cpt>=3){
                    wX=floor(mousePos.x/10)-(2*(floor(mousePos.x/10)/10));
                    wY=floor(mousePos.y/10)-(2*(floor(mousePos.y/10)/10));
                    graph[wY][wX].element.getSprite().setColor(color.Black);
                    graph[wY][wX].element.setWall(true);
                    updateGrid();
                }
            }

        }

        //window.clear();

    }
}

TileScreen::~TileScreen(){
    //dtor
}
void TileScreen::updateGrid(){
    window.clear();
    for(int i=0;i<height;i++){

        for(int j=0;j<width;j++){
            sf::Sprite sprite=graph[j][i].element.getSprite();
            window.draw(sprite);
        }
    }
    window.display();

}
void TileScreen::setStart(int x, int y){

}
void TileScreen::setEnd(int x, int y){

}
int TileScreen::getWidth(){
    return this->width;
}
int TileScreen::getHeight(){
    return this->height;
}
list<Coord*> TileScreen::pathFind(int departX,int departY,int finX,int finY){
    int dx=departX;
    int dy=departY;
    int ex=finX;
    int ey=finY;
    sf::Color color;
    try{
        list<Coord*> path=findPath(window,this->graph,this->width,this->height,dx,dy,ex,ey);//retourne une liste de Coord(Noeuds) qui menent du point de depart à la fin lance une exception sinon
        graph[dy][dx].element.getSprite();
        graph[dy][dx].element.getSprite().setColor(color.Green);
        graph[ey][ex].element.getSprite();
        graph[ey][ex].element.getSprite().setColor(color.Red);
        return path;
    }
    catch(int i){
        if(i==1){
            cout<<"path not found"<<endl;
        }
    }
}
