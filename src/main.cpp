#include "includes.h"

//GLOBALS
const GLint WIDTH = 1280, HEIGHT = 720;
const int VersionMaj = 3, VersionMin = 3;
const char*  glsl_version = "330";

int main(){

    Game game("game", WIDTH, HEIGHT, glsl_version, VersionMaj, VersionMin, true);

    //MAIN LOOP
	while ( !game.getWindowShouldClose() ){

        game.update();
        game.render();

	}

	return 0;
}