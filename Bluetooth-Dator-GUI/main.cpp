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

	//C-hax f�r printing
	unsigned char tempIncomingBuffer[17] = "";
	tempIncomingBuffer[16] = '\0';

	while (ti.running) {

		if (bluetoothPort.getArray(tempIncomingBuffer, 16)) {

			ti.bufMutex.lock();
			std::memcpy(incomingBuffer, tempIncomingBuffer, sizeof(tempIncomingBuffer));
			ti.bufMutex.unlock();
		}
		else{
			continue;
		}

		packetCount++;
		std::cout << "Data mottagen: " << packetCount * 16 << " Bytes (" << packetCount << " paket)" << std::endl;

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
	//Skapar f?nster, variabler osv
	bool running = true;
	unsigned char localBuffer[16];
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1600, 900, 64), "Joystick Use", sf::Style::Default, settings); //F?nstret hanteras som om det vore 1600x900 hela tiden.
	sf::Event e;
	sf::Mutex bufMutex;
	sf::Font font;

	window.setTitle("Dator GUI");

	//Ladda font
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		std::cout << "Fonten �r arg och inte laddas" << std::endl;
	}

	//Skapar alla grafer och andra grafiska objekt
	Xboxcontroller xboxcontroller{ 70, 630, 300, 200 };
	Histogram timeHist{ 600, 250, 600, 400, 10, &font };
	Histogram graphIR0{ 1250, 30, 300, 100, 10, &font };
	Histogram graphIR1{ 1250, 170, 300, 100, 10 , &font };
	Histogram graphIR2{ 1250, 310, 300, 100, 10 , &font };
	Histogram graphIR3{ 1250, 450, 300, 100, 10 , &font };
	Histogram graphIR4{ 1250, 590, 300, 100, 10 , &font };
	Histogram graphIR5{ 1250, 730, 300, 100, 10 , &font };
	AngleGraph testangle1{ 100, 100, 200, 200 };
	AngleGraph testangle2{ 100, 400, 200, 200 };
	AngleGraph testangle3{ 100, 700, 200, 200 };

	//Tr?d k?rs
	Threadinfo ti{ running, bufMutex };
	sf::Thread btThread(&bluetoothThread, ti);
	btThread.launch();

	//tidsvariabler f?r loopen
	sf::Clock tickClock;
	sf::Time timeOfLastUpdate = sf::Time::Zero;
	sf::Time duration = sf::Time::Zero;
	const sf::Time frameTime = sf::seconds(1.f / 30.f);

	while (running) {
		

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

		//Ta emot data
		bufMutex.lock();
		if (incomingBuffer[0] != 0)
		{
			memcpy(localBuffer, incomingBuffer, sizeof(incomingBuffer));
			memset(incomingBuffer, 0, sizeof(incomingBuffer));
			bufMutex.unlock();

			//Hantera data
			/*
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
			*/
			graphIR0.push(localBuffer[0]);
			graphIR1.push(localBuffer[1]);
			graphIR2.push(localBuffer[2]);
			graphIR3.push(localBuffer[3]);
			graphIR4.push(localBuffer[4]);
			graphIR5.push(localBuffer[5]);
			testangle1.push(localBuffer[9]);
			testangle2.push(localBuffer[10]);
			testangle3.push(localBuffer[11]);
		}
		else {
			bufMutex.unlock();
		}

		//Skicka data data
		bufMutex.lock();
		outgoingBuffer[0] = 3;
		outgoingBuffer[1] = xboxcontroller.leftStickAngle();
		outgoingBuffer[2] = xboxcontroller.leftStickIntensity();
		bufMutex.unlock();

		//Rita och s�nt
		window.clear(sf::Color(255, 255, 255));
		xboxcontroller.draw(window);
		timeHist.draw(window);
		graphIR0.draw(window);
		graphIR1.draw(window);
		graphIR2.draw(window);
		graphIR3.draw(window);
		graphIR4.draw(window);
		graphIR5.draw(window);
		testangle1.draw(window);
		testangle2.draw(window);
		testangle3.draw(window);
		window.display();

		duration = sf::seconds(tickClock.getElapsedTime().asSeconds()) - timeOfLastUpdate;
		if (duration < frameTime) {
			sf::sleep(frameTime - duration);
		}

		timeHist.push(100*(1 - duration / frameTime));

	}
	btThread.terminate();
	return 0;
}

