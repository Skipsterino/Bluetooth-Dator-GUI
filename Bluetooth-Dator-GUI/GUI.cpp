#include "GUI.h"



GUI::GUI(sf::Font& font) :
	localMainBuffer{ new unsigned char[16] },
	incomingBuffer{new unsigned char[17]},
	outgoingBuffer{ new unsigned char[16] },
	timeOfLastUpdate{ sf::Time::Zero },
	duration{ sf::Time::Zero },
	frameTime{ sf::seconds(1.f / UPS) },
	running{ false },
	mode{ DUNNO },
	xboxcontroller {70, 630, 300, 200},
	timeHist{ 30, 100, 600, 380, 10, &font , "Downtime" },
	graphIR0{ 1250, 30, 300, 100, 10, &font , "IR0" },
	graphIR1{ 1250, 170, 300, 100, 10 , &font , "IR1" },
	graphIR2{ 1250, 310, 300, 100, 10 , &font , "IR2" },
	graphIR3{ 1250, 450, 300, 100, 10 , &font , "IR3" },
	graphIR4{ 1250, 590, 300, 100, 10 , &font , "IR4" },
	graphIR5{ 1250, 730, 300, 100, 10 , &font , "IR5" },
	graphIR6{ 900, 30, 300, 100, 10 , &font , "IR6" },
	ultraljud{ 900, 170, 300, 100, 10 , &font , "Ultraljud" },
	IRyawR{ 660, 690, 180, 180 , &font, "IR Yaw Right" },
	IRyawL{ 440, 690, 180, 180 , &font, "IR Yaw Left" },
	IMUyaw{ 660, 30, 180, 180 , &font, "IMU Yaw" },
	IMUroll{ 660, 250, 180, 180 , &font, "IMU Roll" },
	IMUpitch{ 660, 470, 180, 180 , &font, "IMU Pitch" },
	stateChart{ 915, 330, 300, 520, &font, "State Chart", 20 }
{

	settings.antialiasingLevel = 8;
	//F?nstret hanteras som om det vore 1600x900 hela tiden.
	window.create(sf::VideoMode(1600, 900, 64), "SpiderPig Control Center", sf::Style::Default, settings);
	window.setPosition(sf::Vector2i(10, 10));

	setlocale(LC_ALL, "");

	//Mode-visar-cirkel och text
	modeText.setFont(font);
	modeText.setString("Dunnolol");
	modeText.setCharacterSize(12);
	modeText.setColor(sf::Color::Black);
	modeText.setPosition(sf::Vector2f(405, 706));
	modeCircle.setOutlineColor(sf::Color::Black);
	modeCircle.setOutlineThickness(1);
	modeCircle.setFillColor(sf::Color::White);
	modeCircle.setRadius(15);
	modeCircle.setPosition(sf::Vector2f(370, 700));

}


GUI::~GUI()
{
	delete[] localMainBuffer;
	delete[] incomingBuffer;
	delete[] outgoingBuffer;
}

void GUI::run()
{
	std::memset(localMainBuffer, 0, sizeof(localMainBuffer));
	std::memset(incomingBuffer, 0, sizeof(incomingBuffer));
	std::memset(outgoingBuffer, 0, sizeof(outgoingBuffer));
	running = true;
	//Tr?d k?rs
	Threadinfo ti{ running, bufMutex , outgoingBuffer, incomingBuffer};
	sf::Thread btThread(&GUI::bluetoothThread, ti);
	btThread.launch();
	sf::Event e;
	uint8_t lastState{ 0 };
	while (running) {

		timeOfLastUpdate = sf::seconds(tickClock.getElapsedTime().asSeconds());

		xboxcontroller.update();
		pollEvent(e);
		lastState = localMainBuffer[14];
		grabAndPushIncoming();
		if (mode == RACE && lastState != localMainBuffer[14]) {
			mode = AUTO;
			modeCircle.setFillColor(sf::Color::Red);
			modeText.setString("Auto");
		}
		pushOutgoing();
		draw();

		duration = sf::seconds(tickClock.getElapsedTime().asSeconds()) - timeOfLastUpdate;
		sleepTimeLeft();
		timeHist.push(100 * (1 - duration / frameTime));
	}

	btThread.wait();
}

void GUI::draw()
{
	window.clear(sf::Color::White);
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
	IRyawL.draw(window);
	IRyawR.draw(window);
	IMUyaw.draw(window);
	IMUroll.draw(window);
	IMUpitch.draw(window);
	stateChart.draw(window);
	window.draw(modeCircle);
	window.draw(modeText);
	window.display();
}

void GUI::pollEvent(sf::Event& e)
{
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
				std::cout << (int)param.kp << " " << (int)param.kd << std::endl;
				break;
			case sf::Keyboard::C:
				stateChart.clear();
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
				std::cout << (int)param.kp << " " << (int)param.kd << std::endl;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void GUI::grabAndPushIncoming()
{
	bufMutex.lock();
	if (incomingBuffer[0] != 0)
	{
		std::memcpy(localMainBuffer, incomingBuffer, 16);
		std::memset(incomingBuffer, 0, sizeof(incomingBuffer));
		bufMutex.unlock();
		//Hantera data
		graphIR0.push(localMainBuffer[0]);
		graphIR1.push(localMainBuffer[1]);
		graphIR2.push(localMainBuffer[2]);
		graphIR3.push(localMainBuffer[3]);
		graphIR4.push(localMainBuffer[4]);
		graphIR5.push(localMainBuffer[5]);
		graphIR6.push(localMainBuffer[6]);
		ultraljud.push(localMainBuffer[7]);
		IRyawL.push(twoCompToDec(localMainBuffer[8], 8));
		IRyawR.push(twoCompToDec(localMainBuffer[9], 8));
		IMUyaw.push(twoCompToDec(localMainBuffer[10] + (localMainBuffer[11] << 8), 16));
		IMUpitch.push(twoCompToDec(localMainBuffer[12], 8));
		IMUroll.push(twoCompToDec(localMainBuffer[13], 8));
		stateChart.push(localMainBuffer[14]);
	}
	else {
		bufMutex.unlock();
	}
}

void GUI::pushOutgoing()
{
	bufMutex.lock();
	if (xboxcontroller.leftLeverActive() || (int)xboxcontroller.triggerValue() != 0) {
		outgoingBuffer[0] |= 3;
	}
	if (xboxcontroller.dpadYAxis() != 0) {
		outgoingBuffer[0] |= 4;
	}
	if (xboxcontroller.A_Pressed()) {
		outgoingBuffer[0] |= (1 << 4);
		outgoingBuffer[0] |= (1 << 5);
	}
	else if (xboxcontroller.B_Pressed()) {
		outgoingBuffer[0] |= (1 << 3);
		outgoingBuffer[4] = 0b00111100;
		mode = AUTO;
		modeCircle.setFillColor(sf::Color::Red);
		modeText.setString("Auto");
	}
	else if (xboxcontroller.X_Pressed()) {
		outgoingBuffer[0] |= (1 << 3);
		outgoingBuffer[4] = 0b00001111;
		mode = MANUAL;
		modeCircle.setFillColor(sf::Color::Blue);
		modeText.setString("Manual");
	}
	else if (xboxcontroller.Y_Pressed()) {
		outgoingBuffer[0] |= (1 << 3);
		outgoingBuffer[4] = 0b11110000;
		mode = RACE;
		modeCircle.setFillColor(sf::Color::Yellow);
		modeText.setString("Race");
	}
	outgoingBuffer[1] = 120 + xboxcontroller.leftStickAngle();
	outgoingBuffer[2] = 120 + xboxcontroller.triggerValue();
	outgoingBuffer[3] = xboxcontroller.dpadYAxis();
	outgoingBuffer[5] = param.kp;
	outgoingBuffer[6] = param.kd;
	bufMutex.unlock();
}

void GUI::sleepTimeLeft()
{
	if (duration < frameTime) {
		sf::sleep(frameTime - duration);
	}
}

int GUI::twoCompToDec(int twoComp, int bits) {
	if (twoComp < std::pow(2, bits - 1)) {
		return twoComp;
	}
	else {
		return twoComp - std::pow(2, bits);
	}
}

int GUI::decToTwoComp8b(int dec) {
	if (dec < 0) {
		return dec + 256;
	}
	else {
		return dec;
	}
}

void GUI::readFile(Parameters& param)
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

void GUI::bluetoothThread(Threadinfo& ti) {

	SerialPort bluetoothPort;

	std::string port = "";
	std::cout << "Enter COM port: ";
	std::cin >> port;

	int packetCount = 0;

	while (!bluetoothPort.isConnected() && ti.running) 
	{
		std::cout << "Trying to connect..." << std::endl;
		sf::sleep(sf::milliseconds(50));
		bluetoothPort.connect(port);
	}

	//C-hax för printing
	unsigned char tempIncomingBuffer[17] = "";
	tempIncomingBuffer[16] = '\0';
	unsigned int checksum{ 0 };
	while (ti.running) {

		while (!bluetoothPort.isConnected()) 
		{
			std::cout << "Disconnected!!\n" << "Trying to connect..." << std::endl;
			sf::sleep(sf::milliseconds(50));
			bluetoothPort.connect(port);
		}

		if (bluetoothPort.getArray(tempIncomingBuffer, 16))
		{
			ti.bufMutex.lock();
			std::memcpy(ti.incomingBuffer, tempIncomingBuffer, 16);
			ti.bufMutex.unlock();
		}
		else 
		{
			continue;
		}

		packetCount++;
		std::cout << "Data mottagen: " << packetCount * 16 << " Bytes (" << packetCount << " paket)" << std::endl;

		ti.bufMutex.lock();
		checksum = 0;
		for (int i{ 0 }; i < 15; ++i) {
			checksum += ti.outgoingBuffer[i];
		}
		ti.outgoingBuffer[15] = checksum & 0x000000FF;
		
		std::cout << "Sending buffer" << std::endl;
		std::cout << (int)ti.outgoingBuffer[0] << ", " << (int)ti.outgoingBuffer[1] << ", " << (int)ti.outgoingBuffer[2] << std::endl;
		bluetoothPort.sendArray(ti.outgoingBuffer, 16);
		memset(ti.outgoingBuffer, 0, sizeof(outgoingBuffer));

		ti.bufMutex.unlock();

	}

	bluetoothPort.disconnect();

	std::cout << "Stopping reading thread" << std::endl;
}
