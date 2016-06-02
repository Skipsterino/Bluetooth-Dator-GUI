/**
* @file GUI.h
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Base class for the entire GUI
*
* A base class to hold all nessesary GUI-components
* and charts
*/

#ifndef GUI_H

#define GUI_H

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
#include "StateChart.h"
#include "Map.h"

#define UPS 30.f

struct Threadinfo {
	bool& running;
	sf::Mutex& bufMutex;
	unsigned char*& outgoingBuffer;
	unsigned char*& incomingBuffer;
	HWND& windowHandle;
	SerialPort& bluetoothPort;
};

struct Parameters {
	unsigned char kp;
	unsigned char kd;
};

class GUI
{
public:
	GUI(sf::Font&);
	~GUI();

	void run();
private:
	//Variabler
	unsigned char* localMainBuffer;
	unsigned char* incomingBuffer;
	unsigned char* outgoingBuffer;
	Parameters param;

	sf::Clock tickClock;
	sf::Time timeOfLastUpdate;
	sf::Time duration;
	const sf::Time frameTime;

	bool running;
	sf::Mutex bufMutex;
	sf::ContextSettings settings;
	sf::RenderWindow window;

	Xboxcontroller xboxcontroller;
	//Histogram timeHist;
	Histogram graphIR0;
	Histogram graphIR1;
	Histogram graphIR2;
	Histogram graphIR3;
	Histogram graphIR4;
	Histogram graphIR5;
	Histogram graphIR6;
	Histogram ultraljud;
	AngleGraph IRyawR;
	AngleGraph IRyawL;
	AngleGraph IMUyaw;
	AngleGraph IMUroll;
	AngleGraph IMUpitch;
	StateChart stateChart;
	Map map;
	sf::CircleShape modeCircle;
	sf::Text modeText;
	HWND windowHandle;
	MODE mode;
	SerialPort bluetoothPort;

	//Funktioner
	static void bluetoothThread(Threadinfo&);
	void readFile(Parameters&);
	int decToTwoComp8b(int);
	int twoCompToDec(int twoComp, int bits);
	void draw();
	void pollEvent(sf::Event&, sf::Thread&);
	void grabAndPushIncoming();
	void pushOutgoing();
	void sleepTimeLeft();
};

#endif

