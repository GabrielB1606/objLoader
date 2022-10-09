#include "includes.h"

//GLOBALS
const GLint WIDTH = 1280, HEIGHT = 720;
const int VersionMaj = 4, VersionMin = 5;

int main(){

    Game game("game", WIDTH, HEIGHT, VersionMaj, VersionMin, false);
	
    //MAIN LOOP
	while ( !game.getWindowShouldClose() ){
        game.update();
        game.render();
	}

	return 0;
}