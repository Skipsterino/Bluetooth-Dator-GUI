#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include "Xboxcontroller.h"
#include "Histogram.h"
#include "SerialPort.h"

void bluetoothThreadReadWrite(bool &running) {
	std::cout << "Starting reading thread" << std::endl;

	SerialPort bluetoothPort;

	bluetoothPort.connect();

	std::string test{};
	test.append("KioskKioskKiosk6");
	bluetoothPort.sendArray((unsigned char*)test.c_str(), 16);

	unsigned char buffer[17] = "";
	memset(buffer, 0, sizeof(buffer));
	//C-hax för printing
	buffer[16] = '\0';

	while (running) {

		while (!bluetoothPort.getArray(buffer, 16));

		std::printf((const char*)buffer);
	}

	bluetoothPort.disconnect();

	std::cout << "Stopping reading thread" << std::endl;
}

int main(void)
{
	bool running = true;
	sf::RenderWindow window(sf::VideoMode(1000, 600, 64), "Joystick Use", sf::Style::Default); //F?nstret hanteras som om det vore 1600x900 hela tiden.
	sf::Event e;
	window.setTitle("Dator GUI");

	//Xboxcontroller xboxcontroller{ 100, 100, 600, 400 };
	Histogram testhist1{ 400, 50, 300, 200, 10 };
	Histogram testhist2{ 400, 350, 300, 200, 10 };

	sf::Thread btThread(bluetoothThreadReadWrite, running);
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

	
	while (running) {
		//testhist1.push(50 + 50 * sin(tickClock.getElapsedTime().asSeconds()));

		timeOfLastUpdate = sf::seconds(tickClock.getElapsedTime().asSeconds());

		//xboxcontroller.update();

		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
			{
				window.close();
				running = false;
			}

			if (e.type == sf::Event::KeyPressed) {
				switch (e.key.code) {
				case sf::Keyboard::Escape:
				{
					window.close();
					running = false;
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
	btThread.terminate();
	//bluetoothPort.disconnect();
	return 0;
}

