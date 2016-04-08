#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <cstring>
#include <mutex>
#include "Xboxcontroller.h"
#include "Histogram.h"
#include "SerialPort.h"

unsigned char incomingBuffer[17] = "";
unsigned char outgoingBuffer[16] = "";
sf::Mutex incBufMutex;
sf::Mutex outBufMutex;

void lockIncBuf() {incBufMutex.lock();}
void unlockIncBuf() {incBufMutex.unlock();}
void lockOutBuf() {outBufMutex.lock();}
void unlockOutBuf() {outBufMutex.unlock();}

void bluetoothThreadReadWrite(bool& running) {
	std::cout << "Starting reading thread" << std::endl;

	std::string port = "";
	std::cout << "Enter COM port: ";
	std::cin >> port;

	SerialPort bluetoothPort;

	int packetCount = 0;

	while (!bluetoothPort.isConnected()) {
		bluetoothPort.connect(port);
	}

	//memset(incomingBuffer, 0, sizeof(incomingBuffer));
	//memset(outgoingBuffer, 0, sizeof(outgoingBuffer));

	//C-hax för printing
	unsigned char tempIncomingBuffer[17] = "";
	tempIncomingBuffer[16] = '\0';

	while (running) {

		if (bluetoothPort.getArray(tempIncomingBuffer, 16)){

			//std::cout << tempIncomingBuffer << '\n';

			//lockIncBuf();
			std::memcpy(incomingBuffer, tempIncomingBuffer, sizeof(tempIncomingBuffer));
			//unlockIncBuf();
		}
		else
		{
			continue;
		}
		//lockOutBuf();
		//lockIncBuf();
		packetCount++;
		std::cout << "Data mottagen: " << packetCount*16 << "Bytes" << std::endl;
		std::memcpy(outgoingBuffer, incomingBuffer, sizeof(incomingBuffer));
		std::cout << "0:" << (int)outgoingBuffer[0] << std::endl;
		std::cout << "1:" << (int)outgoingBuffer[1] << std::endl;
		std::cout << "2:" << (int)outgoingBuffer[2] << std::endl;
		std::cout << "3:" << (int)outgoingBuffer[3] << std::endl;
		
		//unlockOutBuf();
		//unlockIncBuf();

		//lockOutBuf();
		if (outgoingBuffer[0] != 0){

			std::cout << "Sending buffer" << std::endl;
			bluetoothPort.sendArray(outgoingBuffer, 16);
			memset(outgoingBuffer, 0, sizeof(outgoingBuffer));
		}
		//unlockOutBuf();
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
	return 0;
}

