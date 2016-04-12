#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <cstring>
#include "Xboxcontroller.h"
#include "Histogram.h"
#include "SerialPort.h"
#include "AngleGraph.h"

unsigned char incomingBuffer[17] = "";
unsigned char outgoingBuffer[16] = "";

struct Threadinfo {
	bool& running;
	sf::Mutex& bufMutex;
};


void bluetoothThread(Threadinfo& ti) {

	SerialPort bluetoothPort;

	std::string port = "";
	std::cout << "Enter COM port: ";
	std::cin >> port;

	int packetCount = 0;

	while (!bluetoothPort.isConnected()) {
		std::cout << "Trying to connect..." << std::endl;
		sf::sleep(sf::milliseconds(100));
		bluetoothPort.connect(port);
	}

	//C-hax för printing
	unsigned char tempIncomingBuffer[17] = "";
	tempIncomingBuffer[16] = '\0';

	while (ti.running) {

		//sf::Lock lock(bufMutex);

		if (bluetoothPort.getArray(tempIncomingBuffer, 16)) {

			//std::cout << tempIncomingBuffer << '\n';

			ti.bufMutex.lock();
			std::memcpy(incomingBuffer, tempIncomingBuffer, sizeof(tempIncomingBuffer));
			ti.bufMutex.unlock();
		}
		else
		{
			continue;
		}

		packetCount++;
		std::cout << "Data mottagen: " << packetCount * 16 << " Bytes (" << packetCount << " paket)" << std::endl;
		ti.bufMutex.lock();
		//std::memcpy(outgoingBuffer, tempIncomingBuffer, sizeof(tempIncomingBuffer));
		/*
		std::cout << "0:" << (int)outgoingBuffer[0] << std::endl;
		std::cout << "1:" << (int)outgoingBuffer[1] << std::endl;
		std::cout << "2:" << (int)outgoingBuffer[2] << std::endl;
		std::cout << "3:" << (int)outgoingBuffer[3] << std::endl;
		std::cout << "4:" << (int)outgoingBuffer[4] << std::endl;
		std::cout << "5:" << (int)outgoingBuffer[5] << std::endl;
		std::cout << "6:" << (int)outgoingBuffer[6] << std::endl;
		std::cout << "7:" << (int)outgoingBuffer[7] << std::endl;
		std::cout << "8:" << (int)outgoingBuffer[8] << std::endl;
		std::cout << "9:" << (int)outgoingBuffer[9] << std::endl;
		std::cout << "A:" << (int)outgoingBuffer[10] << std::endl;
		std::cout << "B:" << (int)outgoingBuffer[11] << std::endl;
		std::cout << "C:" << (int)outgoingBuffer[12] << std::endl;
		std::cout << "D:" << (int)outgoingBuffer[13] << std::endl;
		std::cout << "E:" << (int)outgoingBuffer[14] << std::endl;
		std::cout << "F:" << (int)outgoingBuffer[15] << std::endl;
		*/
		ti.bufMutex.unlock();

		ti.bufMutex.lock();
		if (outgoingBuffer[0] != 0) {

			std::cout << "Sending buffer" << std::endl;
			bluetoothPort.sendArray(outgoingBuffer, 16);
			memset(outgoingBuffer, 0, sizeof(outgoingBuffer));
		}


		ti.bufMutex.unlock();

	}

	bluetoothPort.disconnect();

	std::cout << "Stopping reading thread" << std::endl;
}

int main(void)
{
	bool running = true;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1600, 900, 64), "Joystick Use", sf::Style::Default, settings); //F?nstret hanteras som om det vore 1600x900 hela tiden.
	sf::Event e;
	window.setTitle("Dator GUI");

	sf::Mutex bufMutex;

	sf::Font font;

	//Ladda font
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		std::cout << "Fonten är arg och inte laddas" << std::endl;
	}

	Xboxcontroller xboxcontroller{ 100, 100, 600, 400 };
	Histogram testhist1{ 400, 50, 300, 200, 10, &font };
	Histogram testhist2{ 400, 350, 300, 200, 10 , &font };
	AngleGraph testangle1{ 100, 100, 200, 200 };
	AngleGraph testangle2{ 100, 400, 200, 200 };
	AngleGraph testangle3{ 100, 700, 200, 200 };

	Threadinfo ti{ running, bufMutex };

	sf::Thread btThread(&bluetoothThread, ti);
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
	const sf::Time frameTime = sf::seconds(1.f / 30.f);
	float increase{ 0 };
	while (running) {
		//sf::Lock lock(bufMutex);

		//testhist1.push(50 + 50 * sin(tickClock.getElapsedTime().asSeconds()));

		timeOfLastUpdate = sf::seconds(tickClock.getElapsedTime().asSeconds());

		xboxcontroller.update();

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
		unsigned char localBuffer[16];

		//Ta emot data
		bufMutex.lock();
		if (incomingBuffer[0] != 0)
		{
			memcpy(localBuffer, incomingBuffer, sizeof(incomingBuffer));
			memset(incomingBuffer, 0, sizeof(incomingBuffer));
			bufMutex.unlock();

			//Hantera data
			std::cout << "0:" << (int)localBuffer[0] << std::endl;
			std::cout << "1:" << (int)localBuffer[1] << std::endl;
			std::cout << "2:" << (int)localBuffer[2] << std::endl;
			std::cout << "3:" << (int)localBuffer[3] << std::endl;
			std::cout << "4:" << (int)localBuffer[4] << std::endl;
			std::cout << "5:" << (int)localBuffer[5] << std::endl;
			std::cout << "6:" << (int)localBuffer[6] << std::endl;
			std::cout << "7:" << (int)localBuffer[7] << std::endl;
			std::cout << "8:" << (int)localBuffer[8] << std::endl;
			std::cout << "9:" << (int)localBuffer[9] << std::endl;
			std::cout << "A:" << (int)localBuffer[10] << std::endl;
			std::cout << "B:" << (int)localBuffer[11] << std::endl;
			std::cout << "C:" << (int)localBuffer[12] << std::endl;
			std::cout << "D:" << (int)localBuffer[13] << std::endl;
			std::cout << "E:" << (int)localBuffer[14] << std::endl;
			std::cout << "F:" << (int)localBuffer[15] << std::endl;
			testhist1.push(localBuffer[0]);
			testangle1.push(localBuffer[9]);
			testangle2.push(localBuffer[10]);
			testangle3.push(localBuffer[11]);
		}
		else {
			bufMutex.unlock();
		}

		
		//Skicka data data
		bufMutex.lock();
		//memcpy(outgoingBuffer, localBuffer, sizeof(localBuffer));
		bufMutex.unlock();
		//Rita och sånt
		window.clear(sf::Color(255, 255, 255));
		xboxcontroller.draw(window);
		testhist1.draw(window);
		testhist2.draw(window);
		testangle1.draw(window);
		testangle2.draw(window);
		testangle3.draw(window);
		window.display();

		duration = sf::seconds(tickClock.getElapsedTime().asSeconds()) - timeOfLastUpdate;
		if (duration < frameTime) {
			sf::sleep(frameTime - duration);
		}

		testhist2.push(100 * (1 - duration / frameTime));

	}
	btThread.terminate();
	return 0;
}

