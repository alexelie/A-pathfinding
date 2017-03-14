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

    nbCol=x;
    nbRow=y;
    drawingWalls = false;

    //ouverture de la enêtre SFML
    window.create(sf::VideoMode(x*10, y*10), "Pathfinding");

    graph=createGraph(x,y);
    displayInitialGrid();
}

void TileScreen::go(){

    while (window.isOpen()){

        sf::Event event;

        while (window.pollEvent(event)){

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            int colPos=floor(mousePos.x/10);
            int rowPos=floor(mousePos.y/10);

            switch(event.type){

                //Fermeture window
                case sf::Event::Closed:
                    window.close();
                break;

                //Start pathfinder
                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Return && userEventCount >= 2){

                        if(isDone)
                        {
                            displayInitialGrid();
                            break;
                        }
                        else
                        {
                            startPathFinder(startColPos,startRowPos,endColPos,endRowPos);
                            isDone = true;
                        }
                    }
                break;

                //Positionnement des éléments(start point, end point, walls) sur le grid avant de débuter la recherche
                case sf::Event::MouseButtonPressed:

                    if (event.mouseButton.button == sf::Mouse::Left){

                    if(userEventCount == 0){

                        startColPos = colPos;
                        startRowPos = rowPos;
                        graph[rowPos][colPos].element.getSprite().setColor(sf::Color(0,255,0));

                    }else if(userEventCount == 1){

                        endColPos = colPos;
                        endRowPos = rowPos;
                        graph[rowPos][colPos].element.getSprite().setColor(sf::Color(255,0,0));

                    }else if(userEventCount >= 2){

                        drawingWalls = true;
                    }

                    userEventCount++;
                    }
                break;

                //Permet de gérer le dessin des murs de façon fluide en gardant le bt gauche de la souris enfoncé
                case sf::Event::MouseButtonReleased:
                    if(drawingWalls)
                        drawingWalls = false;
                break;

                default:
                break;
            }

            //Permet de gérer le dessin des murs de façon fluide en gardant le bt gauche de la souris enfoncé
            if(drawingWalls && userEventCount >= 2){

                if(!graph[rowPos][colPos].element.wall){

                    graph[rowPos][colPos].element.getSprite().setColor(sf::Color(0,0,0));
                    graph[rowPos][colPos].element.setWall(true);
                }
            }

            //update et display du nouvel état dans le window SFML
            updateGrid();

            //======================================================END WHILE LOOP ==========================================================
            //===============================================================================================================================
        }
    }
}

void TileScreen::updateGrid(){
    window.clear();
    for(int i=0;i<nbRow;i++){
        for(int j=0;j<nbCol;j++){
            sf::Sprite sprite=graph[j][i].element.getSprite();
            window.draw(sprite);
        }
    }
    window.display();
}

void TileScreen::displayInitialGrid(){

    window.clear(sf::Color(0,0,0));

    //cpt 1=start point, 2=end point, 3+=walls
    userEventCount=0;
    isDone = false;
    userEventCount = 0;

    for(int i=0;i<nbRow;i++){
        for(int j=0;j<nbCol;j++){
            sf::Sprite sprite=graph[i][j].element.getSprite();
            sprite.setColor(sf::Color::White);
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
    return this->nbCol;
}
int TileScreen::getHeight(){
    return this->nbRow;
}

list<Coord*> TileScreen::startPathFinder(int departX,int departY,int finX,int finY){

    list<Coord*> path;
    try{
        path=findPath(window,this->graph,this->nbCol,this->nbRow,departX,departY,finX,finY);
        graph[departY][departX].element.getSprite();
        graph[departY][departX].element.getSprite().setColor(sf::Color::Green);
        graph[finY][finX].element.getSprite();
        graph[finY][finX].element.getSprite().setColor(sf::Color::Red);
        return path;
    }
    catch(int i){
        if(i==1){
            cout<<"path not found"<<endl;
        }
    }
}
