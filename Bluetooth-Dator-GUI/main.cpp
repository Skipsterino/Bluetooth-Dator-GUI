#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "Xboxcontroller.h"
#include "Histogram.h"
#include "SerialPort.h"
#include "AngleGraph.h"

unsigned char incomingBuffer[17] = "";
unsigned char outgoingBuffer[16] = "";

#define UPS 30.f

struct Threadinfo {
	bool& running;
	sf::Mutex& bufMutex;
};

struct Parameters {
	unsigned char kp;
	unsigned char kd;
};

int twoCompToDec(int twoComp, int bits) {
	if (twoComp < std::pow(2, bits - 1)) {
		return twoComp;
	}
	else {
		return twoComp - std::pow(2, bits);
	}
}

int decToTwoComp8b(int dec) {
	if (dec < 0) {
		return dec + 256;
	}
	else {
		return dec;
	}
}

void readFile(Parameters& param)
{
	std::cout << "Reading new parameters..." << std::endl;
	std::string line;
	std::ifstream file("reglerparametrar.txt");
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.substr(0, 2) == "kp")
			{
				param.kp = stoi(line.substr(3));
			}
			else if (line.substr(0, 2) == "kd")
			{
				param.kd = stoi(line.substr(3));
			}
		}
	}
	file.close();
	std::cout << "Reading done!" << std::endl;

}

void bluetoothThread(Threadinfo& ti) {

	SerialPort bluetoothPort;

	std::string port = "";
	std::cout << "Enter COM port: ";
	std::cin >> port;

	int packetCount = 0;

	while (!bluetoothPort.isConnected() && ti.running) {
		std::cout << "Trying to connect..." << std::endl;
		sf::sleep(sf::milliseconds(100));
		bluetoothPort.connect(port);
	}

	//C-hax för printing
	unsigned char tempIncomingBuffer[17] = "";
	tempIncomingBuffer[16] = '\0';

	while (ti.running) {

		if (bluetoothPort.getArray(tempIncomingBuffer, 16)) {

			ti.bufMutex.lock();
			std::memcpy(incomingBuffer, tempIncomingBuffer, sizeof(tempIncomingBuffer));
			ti.bufMutex.unlock();
		}
		else {
			continue;
		}

		packetCount++;
		std::cout << "Data mottagen: " << packetCount * 16 << " Bytes (" << packetCount << " paket)" << std::endl;

		ti.bufMutex.lock();
		if (outgoingBuffer[0] != 0) {

			std::cout << "Sending buffer" << std::endl;
			std::cout << (int)outgoingBuffer[0] << ", " << (int)outgoingBuffer[1] << ", " << (int)outgoingBuffer[2] << std::endl;
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
	sf::RenderWindow window(sf::VideoMode(1600, 900, 64), "SpiderPig Control Center", sf::Style::Default, settings); //F?nstret hanteras som om det vore 1600x900 hela tiden.
	sf::Event e;
	sf::Mutex bufMutex;
	sf::Font font;

	//Ladda font
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		std::cout << "Fonten är arg och inte laddas" << std::endl;
	}

	//Skapar alla grafer och andra grafiska objekt
	Xboxcontroller xboxcontroller{ 70, 630, 300, 200 };
	Histogram timeHist{ 100, 100, 600, 400, 10, &font , "Downtime" };
	Histogram graphIR0{ 1250, 30, 300, 100, 10, &font , "IR0" };
	Histogram graphIR1{ 1250, 170, 300, 100, 10 , &font , "IR1" };
	Histogram graphIR2{ 1250, 310, 300, 100, 10 , &font , "IR2" };
	Histogram graphIR3{ 1250, 450, 300, 100, 10 , &font , "IR3" };
	Histogram graphIR4{ 1250, 590, 300, 100, 10 , &font , "IR4" };
	Histogram graphIR5{ 1250, 730, 300, 100, 10 , &font , "IR5" };
	Histogram graphIR6{ 900, 30, 300, 100, 10 , &font , "IR6" };
	Histogram ultraljud{ 900, 170, 300, 100, 10 , &font , "Ultraljud" };
	AngleGraph IRyaw{ 400, 650, 180, 180 , &font, "IR Yaw" };
	AngleGraph IMUyaw{ 600, 650, 180, 180 , &font, "IMU Yaw"};
	AngleGraph IMUroll{ 800, 650, 180, 180 , &font, "IMU Roll" };
	AngleGraph IMUpitch{ 1000, 650, 180, 180 , &font, "IMU Pitch" };

	Parameters param;
	readFile(param);

	//Tr?d k?rs
	Threadinfo ti{ running, bufMutex };
	sf::Thread btThread(&bluetoothThread, ti);
	btThread.launch();

	//tidsvariabler f?r loopen
	sf::Clock tickClock;
	sf::Time timeOfLastUpdate = sf::Time::Zero;
	sf::Time duration = sf::Time::Zero;
	const sf::Time frameTime = sf::seconds(1.f / UPS);

	while (running) {


		timeOfLastUpdate = sf::seconds(tickClock.getElapsedTime().asSeconds());

		xboxcontroller.update();

		while (window.pollEvent(e)) {

			switch (e.type)
			{
			case sf::Event::Closed:

				window.close();
				running = false;
				break;

			case sf::Event::KeyPressed:
				switch (e.key.code) {
				case sf::Keyboard::Escape:
					window.close();
					running = false;
					break;
				case sf::Keyboard::L:
					readFile(param);
					//std::cout << (int)param.kp << " " << (int)param.kd << std::endl;
					break;
				default:
					break;
				}
				break;

			case sf::Event::JoystickButtonPressed:

				switch (e.joystickButton.button)
				{
				case 0:
					readFile(param);
					//std::cout << (int)param.kp << " " << (int)param.kd << std::endl;
					break;
				default:
					break;
				}
				break;
			default:
				break;
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
			graphIR0.push(localBuffer[0]);
			graphIR1.push(localBuffer[1]);
			graphIR2.push(localBuffer[2]);
			graphIR3.push(localBuffer[3]);
			graphIR4.push(localBuffer[4]);
			graphIR5.push(localBuffer[5]);
			graphIR6.push(localBuffer[6]);
			ultraljud.push(localBuffer[7]);
			IRyaw.push(twoCompToDec(localBuffer[8] + (localBuffer[9] << 8), 16));
			IMUyaw.push(twoCompToDec(localBuffer[10] + (localBuffer[11] << 8), 16));
			IMUroll.push(twoCompToDec(localBuffer[12], 8));
			IMUpitch.push(twoCompToDec(localBuffer[13], 8));
		}
		else {
			bufMutex.unlock();
		}

		//std::cout << xboxcontroller.leftStickAngle() << std::endl;
		//std::cout << 100 - sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) << std::endl;
		
		//Skicka data data
		bufMutex.lock();
		outgoingBuffer[0] = 0;
		if (xboxcontroller.leftLeverActive() || (int)xboxcontroller.triggerValue() != 0) {
			outgoingBuffer[0] += 3;
		}
		outgoingBuffer[1] = xboxcontroller.leftStickAngle();
		outgoingBuffer[2] = 100 - xboxcontroller.triggerValue();
		bufMutex.unlock();

		//Rita och sånt
		window.clear(sf::Color(255, 255, 255));
		xboxcontroller.draw(window);
		timeHist.draw(window);
		graphIR0.draw(window);
		graphIR1.draw(window);
		graphIR2.draw(window);
		graphIR3.draw(window);
		graphIR4.draw(window);
		graphIR5.draw(window);
		graphIR6.draw(window);
		ultraljud.draw(window);
		IRyaw.draw(window);
		IMUyaw.draw(window);
		IMUroll.draw(window);
		IMUpitch.draw(window);
		window.display();

		duration = sf::seconds(tickClock.getElapsedTime().asSeconds()) - timeOfLastUpdate;
		if (duration < frameTime) {
			sf::sleep(frameTime - duration);
		}

		timeHist.push(100 * (1 - duration / frameTime));

	}

	btThread.wait();
	btThread.terminate();
	return 0;
}

