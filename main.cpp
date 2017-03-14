#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <list>
//#include "pathfinding.h"
#include "TileScreen.h"
#include <SFML/Graphics.hpp>
using namespace std;

int main(){
    cout<<"enter main"<<endl;
    const int size_x=60;
    const int size_y=60;
    TileScreen tileScreen(size_x,size_y);
    tileScreen.go();
    return 0;
}










