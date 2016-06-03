/**
* @file main.cpp
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
*/

#include "GUI.h"
#include <SFML/OpenGL.hpp>

/**
* @brief main function of the graphical user interface.
*/
int main() {
	sf::Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		std::cout << "Niet, laddade inte font" << std::endl;
	}
	GUI gui{font};
	gui.run();
	return 0;
}


