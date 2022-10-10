#include "includes.h"

//GLOBALS
const GLint WIDTH = 1280, HEIGHT = 720;
const int VersionMaj = 3, VersionMin = 3;

int main(){

    std::string glsl_version = "#version "+ std::to_string(VersionMaj) +std::to_string(VersionMin) + "0";
    Game game("game", WIDTH, HEIGHT, VersionMaj, VersionMin, false);

    UserInterface gui( game.getWindowReference(), glsl_version.c_str() );
	
    //MAIN LOOP
	while ( !game.getWindowShouldClose() ){
        game.update();
        gui.update();
        game.render();
        gui.render();
	}

	return 0;
}