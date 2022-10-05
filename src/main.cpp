#include "includes.h"

//GLOBALS
const GLint WIDTH = 1280, HEIGHT = 720;
const int VersionMaj = 4, VersionMin = 4;

Game game("game", WIDTH, HEIGHT, VersionMaj, VersionMin, false);

int main(){

	 //MAIN LOOP
	while ( !game.getWindowShouldClose() ){
        game.update();
        game.render();
	}

	return 0;
}