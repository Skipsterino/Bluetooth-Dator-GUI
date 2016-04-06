#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include "Xboxcontroller.h"
#include "Histogram.h"
#include "SerialPort.h"

void bluetoothThreadReadWrite(SerialPort& bluetoothPort) {
	while (true) {
		unsigned char* buffer = new unsigned char[5];

		//bluetoothPort.getArray(buffer, 5);

		//std::cout << buffer;
	}
}

int main(void)
{
	sf::RenderWindow window(sf::VideoMode(1000, 600, 64), "Joystick Use", sf::Style::Default); //F?nstret hanteras som om det vore 1600x900 hela tiden.
	sf::Event e;
	window.setTitle("Dator GUI");

	//Xboxcontroller xboxcontroller{ 100, 100, 600, 400 };
	Histogram testhist1{ 400, 50, 300, 200, 10 };
	Histogram testhist2{ 400, 350, 300, 200, 10 };

	SerialPort bluetoothPort;

	bluetoothPort.connect();

	sf::Thread btThread(bluetoothThreadReadWrite, bluetoothPort);
	btThread.launch();
								
	//query joystick for settings if it's plugged in...
	if (sf::Joystick::isConnected(0)) {
		// check how many buttons joystick number 0 has
		unsigned int buttonCount = sf::Joystick::getButtonCount(0);

		// check if joystick number 0 has a Z axis
		bool hasZ = sf::Joystick::hasAxis(0, sf::Joystick::Z);

		std::cout << "Button count: " << buttonCount << std::endl;
		std::cout << "Has a z-axis: " << hasZ << std::endl;
	}
	

	//time
	sf::Clock tickClock;
	sf::Time timeOfLastUpdate = sf::Time::Zero;
	sf::Time duration = sf::Time::Zero;
	const sf::Time frameTime = sf::seconds(1.f/30.f);

	bool running = true;
	while (running) {
		//testhist1.push(50 + 50 * sin(tickClock.getElapsedTime().asSeconds()));

		timeOfLastUpdate = sf::seconds(tickClock.getElapsedTime().asSeconds());

	std::string test{};
	test.append("Kiosk");
	bluetoothPort.sendArray((unsigned char*)test.c_str(), 5);

		//xboxcontroller.update();

		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
			{
				window.close();
				return 0;
			}

			if (e.type == sf::Event::KeyPressed) {
				switch (e.key.code) {
				case sf::Keyboard::Escape:
				{
					window.close();
					return 0;
				}
				break;
				default:
					break;
				}
			}
		}

		window.clear(sf::Color(255, 255, 255));
		//xboxcontroller.draw(window);
		testhist1.draw(window);
		testhist2.draw(window);
		window.display();

		duration = sf::seconds(tickClock.getElapsedTime().asSeconds()) - timeOfLastUpdate;
		if (duration < frameTime) {
			sf::sleep(frameTime - duration);
		}

		testhist2.push(100*(1 - duration / frameTime));

	}
	return 0;
}

