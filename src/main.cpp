#include "includes.h"

//GLOBALS
const GLint WIDTH = 1280, HEIGHT = 720;
const int VersionMaj = 3, VersionMin = 3;
const char*  glsl_version = "330";

int main(){

    Game game("game", WIDTH, HEIGHT, glsl_version, VersionMaj, VersionMin, true);

    // std::string version = glsl_version;
    // version = "#version "+version;

    // UserInterface gui( game.getWindowReference(), version.c_str() );

    //MAIN LOOP
	while ( !game.getWindowShouldClose() ){

        game.update();
        game.render();

	}

	return 0;
}