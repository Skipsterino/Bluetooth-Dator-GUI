#include "GUI.h"

int main() {
	sf::Font font;
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		std::cout << "Niet, laddade inte font" << std::endl;
	}
	GUI gui{font};
	gui.run();
	return 0;
}

