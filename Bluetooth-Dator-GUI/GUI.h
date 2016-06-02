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

	/**
	* @brief Runs the graphical interface
	*/
	void run();
private:
	//Variabler
	unsigned char* localMainBuffer; /**< Locally stores the recieved bluetooth data */
	unsigned char* incomingBuffer; /**< Stores the recieved bluetooth data */
	unsigned char* outgoingBuffer; /**< Stores the outgoing bluetooth data */
	Parameters param; /**< Strucs holding kp and kd */

	sf::Clock tickClock; /**< Global timer */
	sf::Time timeOfLastUpdate; /**< Focuses the tickClock when each main loop starts */
	sf::Time duration; /**< Holds the duration of the main loop */
	const sf::Time frameTime; /**< Constant value holding the expected time each main loop should take */

	bool running; /**< Bool saying if the program is running */
	sf::Mutex bufMutex; /**< Mutex for the bluetooth buffers */
	sf::ContextSettings settings; /**< Settings object for the window */
	sf::RenderWindow window; /**< The window the program is shown on */

	Xboxcontroller xboxcontroller; /**< XBOX-controller object */
	//Histogram timeHist;
	Histogram graphIR0; /**< IR0 graph object */
	Histogram graphIR1; /**< IR1 graph object */
	Histogram graphIR2; /**< IR2 graph object */
	Histogram graphIR3; /**< IR3 graph object */
	Histogram graphIR4; /**< IR4 graph object */
	Histogram graphIR5; /**< IR5 graph object */
	Histogram graphIR6; /**< IR6 graph object */
	Histogram ultraljud; /**< Ultrasound graph object */
	AngleGraph IRyawR; /**< IR yaw right graph object */
	AngleGraph IRyawL; /**< IR yaw left graph object */
	AngleGraph IMUyaw; /**< IMU yaw graph object */
	AngleGraph IMUroll; /**< IMU roll graph object */
	AngleGraph IMUpitch; /**< IMU pitch graph object */
	StateChart stateChart; /**< State chart object */
	Map map;
	sf::CircleShape modeCircle; /**< Mode circle showing mode of robot */
	sf::Text modeText; /**< Mode text of robot */
	HWND windowHandle; /**< Handle to the window which shows the graphics */
	MODE mode; /**< enum mode */
	SerialPort bluetoothPort; /**< Serial port object */

	/**
	* @brief Bluetooth thread function
	*
	* The function the bluetooth thread is running
	* to send and recieve information through bluetooth
	* @param threadinfo Parameter with references to info the thread needs to share with main thread
	*/
	static void bluetoothThread(Threadinfo&);

	/**
	* @brief Reads a text file for kp and kd
	*/
	void readFile(Parameters&);

	/**
	* @brief Converts from decimal number to two complement number
	*
	* Converts a decimal number to the corresponding
	* two complement number
	*/
	int decToTwoComp8b(int);

	/**
	* @brief Converts from two complement number to the
	*  corresponding decimal number
	*/
	int twoCompToDec(int twoComp, int bits);

	/**
	* @brief Draws the entire interface
	*/
	void draw();

	/**
	* @brief Polls for input events
	*
	* When called the function checks if any new input event
	* has occured and does a corresponding action for the event
	*/
	void pollEvent(sf::Event&, sf::Thread&);

	/**
	* @brief Grabs the info in the input buffer and push the data into the interface
	*/
	void grabAndPushIncoming();

	/**
	* @brief Push relevant data to the outgoing buffer for bluetooth transfer
	*/
	void pushOutgoing();

	/**
	* @brief Sleeps the time that remains between duration and frameTime
	*/
	void sleepTimeLeft();
};

#endif

