#include "GUI.h"
#include <SFML/OpenGL.hpp>


GUI::GUI(sf::Font& font) :
	localMainBuffer{ new unsigned char[16] },
	incomingBuffer{ new unsigned char[17] },
	outgoingBuffer{ new unsigned char[16] },
	timeOfLastUpdate{ sf::Time::Zero },
	duration{ sf::Time::Zero },
	frameTime{ sf::seconds(1.f / UPS) },
	running{ false },
	mode{ DUNNO },
	windowHandle{ NULL },
	bluetoothPort{},
	xboxcontroller{ 70, 630, 300, 200 },
	//timeHist{ 30, 100, 600, 380, 10, &font , "Downtime" },
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
	stateChart{ 915, 330, 300, 520, &font, "State Chart", 20 },
	map{ 15, 30, 620, 450, 21, 17, &font, "Labyrinth map" }
{
	settings.antialiasingLevel = 8;
	settings.depthBits = 24;
	//F?nstret hanteras som om det vore 1600x900 hela tiden.
	window.create(sf::VideoMode(1600, 900, 64), "SpiderPig Control Center", sf::Style::Default, settings);
	window.setPosition(sf::Vector2i(10, 10));

	sf::Image icon;
	if (icon.loadFromFile("Bilder/spig_small.png"))
	{
		window.setIcon(32, 32, icon.getPixelsPtr());
	}

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
	std::memset(localMainBuffer, 0, 16);
	std::memset(incomingBuffer, 0, 16);
	std::memset(outgoingBuffer, 0, 16);
	running = true;

	//OpenGL-inställningar och definitioner
	{
		window.setActive();

		// Load an OpenGL texture.
		GLuint texture = 0;
		{
			sf::Image image;
			if (!image.loadFromFile("Bilder/metalTexture3.jpg"))
				return;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		// Enable Z-buffer read and write
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glClearDepth(1.f);
		glDisable(GL_LIGHTING);
		glViewport(0, 0, window.getSize().x, window.getSize().y);

		// Setup a perspective projection
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
		glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);

		// Bind the texture
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		// Define a 3D cube (6 faces made of 2 triangles composed by 3 vertices)
		static const GLfloat cube[] =
		{
			// positions    // texture coordinates
			-2, -0.5, -2,  0, 0,
			-2,  0.5, -2,  1, 0,
			-2, -0.5,  2,  0, 1,
			-2, -0.5,  2,  0, 1,
			-2,  0.5, -2,  1, 0,
			-2,  0.5,  2,  1, 1,

			2, -0.5, -2,  0, 0,
			2,  0.5, -2,  1, 0,
			2, -0.5,  2,  0, 1,
			2, -0.5,  2,  0, 1,
			2,  0.5, -2,  1, 0,
			2,  0.5,  2,  1, 1,

			-2, -0.5, -2,  0, 0,
			2, -0.5, -2,  1, 0,
			-2, -0.5,  2,  0, 1,
			-2, -0.5,  2,  0, 1,
			2, -0.5, -2,  1, 0,
			2, -0.5,  2,  1, 1,

			-2,  0.5, -2,  0, 0,
			2,  0.5, -2,  1, 0,
			-2,  0.5,  2,  0, 1,
			-2,  0.5,  2,  0, 1,
			2,  0.5, -2,  1, 0,
			2,  0.5,  2,  1, 1,

			-2, -0.5, -2,  0, 0,
			2, -0.5, -2,  1, 0,
			-2,  0.5, -2,  0, 1,
			-2,  0.5, -2,  0, 1,
			2, -0.5, -2,  1, 0,
			2,  0.5, -2,  1, 1,

			-2, -0.5,  2,  0, 0,
			2, -0.5,  2,  1, 0,
			-2,  0.5,  2,  0, 1,
			-2,  0.5,  2,  0, 1,
			2, -0.5,  2,  1, 0,
			2,  0.5,  2,  1, 1
		};

		// Enable position and texture coordinates vertex components
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(3, GL_FLOAT, 5 * sizeof(GLfloat), cube);
		glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(GLfloat), cube + 3);

		// Disable normal and color vertex components
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	//Hämta fönstret och sparar handtaget
	windowHandle = GetForegroundWindow();
	//Skapar trådens informationsobjekt som skickas med när den startas
	Threadinfo ti{ running, bufMutex , outgoingBuffer, incomingBuffer, windowHandle, bluetoothPort };
	//Skapar och startar blåtandstråden
	sf::Thread btThread(&GUI::bluetoothThread, ti);
	btThread.launch();
	
	//Defninierar event och senaste autonoma tillståndet
	sf::Event e;
	uint8_t lastState{ 0 };
	
	//Minimerar fönstret (för att kunna skriva in COM-port)
	ShowWindow(windowHandle, SW_MINIMIZE);

	while (running) {
		//Fixerar tiden loopen började
		timeOfLastUpdate = sf::seconds(tickClock.getElapsedTime().asSeconds());

		xboxcontroller.update();
		//Läser in senaste autonoma tillståndet
		lastState = localMainBuffer[14];
		
		//hämtar, om någon, ny inkommande data från inputarrayen
		grabAndPushIncoming();
		
		//Specialfall för om läget är RACE och vi går in i AUTO, ändra till AUTO
		if (mode == RACE && lastState != localMainBuffer[14]) {
			mode = AUTO;
			modeCircle.setFillColor(sf::Color::Red);
			modeText.setString("Auto");
		}
		
		pollEvent(e, btThread);
		//Pushar all data som ska skickas ut på blåtand
		pushOutgoing();
		
		draw();

		sleepTimeLeft();

	}

	btThread.wait();
}

void GUI::draw()
{
	window.pushGLStates();
	window.clear(sf::Color::White);
	xboxcontroller.draw(window);
	//timeHist.draw(window);
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
	map.draw(window);
	window.draw(modeCircle);
	window.draw(modeText);
	window.popGLStates();

	// Clear the depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Apply some transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(10.f, 10.f, 1.f);
	glTranslatef(-8.f, -3.f, -100.f);
	glRotatef(-(float)twoCompToDec(localMainBuffer[10] + (localMainBuffer[11] << 8), 16), 0.f, 1.f, 0.f);
	glRotatef((float)twoCompToDec(localMainBuffer[13], 8), 1.f, 0.f, 0.f);
	glRotatef((float)twoCompToDec(localMainBuffer[12], 8), 0.f, 0.f, 1.f);

	// Draw the cube
	glDrawArrays(GL_TRIANGLES, 0, 36);
	window.display();
}

void GUI::pollEvent(sf::Event& e, sf::Thread& btThread)
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
				map.clear();
				break;
			case sf::Keyboard::R:
				ShowWindow(windowHandle, SW_MINIMIZE);
				btThread.terminate();
				bluetoothPort.disconnect();
				btThread.launch();
				break;
			case sf::Keyboard::A:
				bufMutex.lock();
				readFile(param);
				std::cout << (int)param.kp << " " << (int)param.kd << std::endl;
				outgoingBuffer[0] |= (1 << 4);
				outgoingBuffer[0] |= (1 << 5);
				bufMutex.unlock();
				break;
			case sf::Keyboard::B:
				bufMutex.lock();
				outgoingBuffer[0] |= (1 << 3);
				outgoingBuffer[4] = 0b00111100;
				mode = AUTO;
				modeCircle.setFillColor(sf::Color::Red);
				modeText.setString("Auto");
				bufMutex.unlock();
				break;
			case sf::Keyboard::X:
				bufMutex.lock();
				outgoingBuffer[0] |= (1 << 3);
				outgoingBuffer[4] = 0b00001111;
				mode = MANUAL;
				modeCircle.setFillColor(sf::Color::Blue);
				modeText.setString("Manual");
				bufMutex.unlock();
				break;
			case sf::Keyboard::Y:
				bufMutex.lock();
				outgoingBuffer[0] |= (1 << 3);
				outgoingBuffer[4] = 0b11110000;
				mode = RACE;
				modeCircle.setFillColor(sf::Color::Yellow);
				modeText.setString("Race");
				bufMutex.unlock();
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
		case sf::Event::Resized:
			glViewport(0, 0, e.size.width, e.size.height);
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
		map.push(localMainBuffer[14], mode);
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
	if (xboxcontroller.rightLeverActive())
	{
		outgoingBuffer[0] |= 0x80;
	}
	if (xboxcontroller.dpadYAxis() != 0) {
		outgoingBuffer[0] |= 4;
	}
	if (xboxcontroller.dpadXAxis() != 0) {
		outgoingBuffer[0] |= (1 << 6);
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
	outgoingBuffer[7] = xboxcontroller.dpadXAxis();

	sf::Vector2f pos = xboxcontroller.getRightAxisPosition();

	uint8_t x = pos.x*7.f / 100.f + 8;
	uint8_t y = pos.y*7.f / 100.f + 8;

	outgoingBuffer[8] = ((x & 0x0F) << 4) + (y & 0x0F);

	bufMutex.unlock();
}

void GUI::sleepTimeLeft()
{
	duration = sf::seconds(tickClock.getElapsedTime().asSeconds()) - timeOfLastUpdate;

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

	std::string port = "";
	std::cout << "Enter COM port: ";
	std::cin >> port;

	int packetCount = 0;

	while (!ti.bluetoothPort.isConnected() && ti.running)
	{
		std::cout << "Trying to connect..." << std::endl;
		sf::sleep(sf::milliseconds(50));
		ti.bluetoothPort.connect(port);
	}

	ShowWindow(ti.windowHandle, SW_RESTORE);
	SetFocus(ti.windowHandle);

	//C-hax för printing
	unsigned char tempIncomingBuffer[17] = "";
	tempIncomingBuffer[16] = '\0';
	unsigned int checksum{ 0 };
	while (ti.running) {

		while (!ti.bluetoothPort.isConnected())
		{
			std::cout << "Disconnected!!\n" << "Trying to connect..." << std::endl;
			sf::sleep(sf::milliseconds(50));
			ti.bluetoothPort.connect(port);
		}

		if (ti.bluetoothPort.getArray(tempIncomingBuffer, 16))
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

		std::cout << "Sending buffer, here is first byte: ";
		std::cout << (int)ti.outgoingBuffer[0] << std::endl;
		ti.bluetoothPort.sendArray(ti.outgoingBuffer, 16);
		memset(ti.outgoingBuffer, 0, sizeof(outgoingBuffer));

		ti.bufMutex.unlock();

	}

	ti.bluetoothPort.disconnect();

	std::cout << "Stopping reading thread" << std::endl;
}
