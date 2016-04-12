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
sf::Mutex bufMutex;


void bluetoothThread(bool& running) {

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
	
	while (running) {
		
		//sf::Lock lock(bufMutex);

		if (bluetoothPort.getArray(tempIncomingBuffer, 16)){

			//std::cout << tempIncomingBuffer << '\n';

			bufMutex.lock();
			std::memcpy(incomingBuffer, tempIncomingBuffer, sizeof(tempIncomingBuffer));
			bufMutex.unlock();
		}
		else
		{
			continue;
		}
		
		packetCount++;
		std::cout << "Data mottagen: " << packetCount*16 << " Bytes (" << packetCount << " paket)" << std::endl;
		bufMutex.lock();
		std::memcpy(outgoingBuffer, incomingBuffer, sizeof(incomingBuffer));
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
//		bufMutex.unlock();

//		bufMutex.lock();
		if (outgoingBuffer[0] != 0){

			std::cout << "Sending buffer" << std::endl;
			bluetoothPort.sendArray(outgoingBuffer, 16);
			memset(outgoingBuffer, 0, sizeof(outgoingBuffer));
		}


		bufMutex.unlock();

	}

	bluetoothPort.disconnect();

	std::cout << "Stopping reading thread" << std::endl;
}

int main(void)
{
	bool running = true;
	sf::RenderWindow window(sf::VideoMode(1600, 900, 64), "Joystick Use", sf::Style::Default); //F?nstret hanteras som om det vore 1600x900 hela tiden.
	sf::Event e;
	window.setTitle("Dator GUI");

	sf::Font font;

	//Ladda font
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		std::cout << "Fonten är arg och inte laddas" << std::endl;
	}

	Xboxcontroller xboxcontroller{ 100, 100, 600, 400 };
	Histogram testhist1{ 400, 50, 300, 200, 10, &font };
	Histogram testhist2{ 400, 350, 300, 200, 10 , &font};
	AngleGraph testangle1{ 100, 100, 200, 200};

	testangle1.push(50);

	sf::Thread btThread(&bluetoothThread, running);
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

		window.clear(sf::Color(255, 255, 255));
		xboxcontroller.draw(window);
		testhist1.draw(window);
		testhist2.draw(window);
		testangle1.draw(window);
		window.display();

		duration = sf::seconds(tickClock.getElapsedTime().asSeconds()) - timeOfLastUpdate;
		if (duration < frameTime) {
			sf::sleep(frameTime - duration);
		}

		testhist2.push(100*(1 - duration / frameTime));

	}
	btThread.terminate();
	return 0;
}

