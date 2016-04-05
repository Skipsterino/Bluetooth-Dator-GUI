#include "Histogram.h"
#include <iostream>

sf::Clock Histogram::timer;

Histogram::Histogram(float xpos, float ypos, float width, float height, int timeLimit) :
	maxTime{ timeLimit },
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	distTime{},
	graphLines{},
	graphPoints{},
	graphLetters{},
	font{}
{
	//y-axel
	graphLines.push_back({ {xpos + width / 10.f, ypos}, 
						   { xpos + width / 10.f, ypos + height} });
	//x-axel
	graphLines.push_back({ { xpos, ypos + height },
						   { xpos + width, ypos + height } });
	//tidsaxel 0-sträck
	graphLines.push_back({ { xpos + width, ypos + height - height/50},
						   { xpos + width, ypos + height + height / 50 } });
	//tidsaxel halv-sträck
	graphLines.push_back({ { xpos + 11*width/20, ypos + height - height / 50 },
						   { xpos + 11*width/20, ypos + height + height / 50 } });
	//tidsaxel hel-sträck
	graphLines.push_back({ { xpos + width / 10, ypos + height - height / 50 },
						   { xpos + width / 10, ypos + height + height / 50 } });
	
	if (!font.loadFromFile("Fonts/arial.ttf")) {
		std::cout << "Fonten är arg och inte laddas" << std::endl;
	}

	sf::Text sekNoll;
	sf::Text sekHalv;
	sf::Text sekHel;

	sekNoll.setFont(font);
	sekHalv.setFont(font);
	sekHel.setFont(font);

	sekNoll.setString("0");
	sekHalv.setString(std::to_string(maxTime/2));
	sekHel.setString(std::to_string(maxTime));

	sekNoll.setCharacterSize(12);
	sekHalv.setCharacterSize(12);
	sekHel.setCharacterSize(12);

	sekNoll.setColor(sf::Color(0, 0, 0, 255));
	sekHalv.setColor(sf::Color(0, 0, 0, 255));
	sekHel.setColor(sf::Color(0, 0, 0, 255));

	sekNoll.setPosition(sf::Vector2f(xpos + 99*width/100, ypos + 52*height/50));
	sekHalv.setPosition(sf::Vector2f(xpos + 54*width/100, ypos + 52 * height / 50));
	sekHel.setPosition(sf::Vector2f(xpos + 7*width/80, ypos + 52 * height / 50));

	graphLetters.push_back(sekNoll);
	graphLetters.push_back(sekHalv);
	graphLetters.push_back(sekHel);
}


Histogram::~Histogram()
{
}

void Histogram::push(float value) {
	distTime.insert(distTime.begin(),{ value, timer.getElapsedTime().asSeconds() });

	while (timer.getElapsedTime().asSeconds() - distTime[distTime.size() - 1].second > maxTime) {
		distTime.pop_back();
	}

	graphPoints.clear();
	graphPoints.reserve(distTime.size());

	float curTime = timer.getElapsedTime().asSeconds();

	for (auto& i : distTime) {
		graphPoints.push_back({xpos + width*(1 + 9*(maxTime - (curTime - i.second))/maxTime)/10,
							   ypos + height*(1 - i.first/100) });
	}
}

void Histogram::draw(sf::RenderWindow& window) {
	for (auto& i : graphLines){
		sf::Vertex temp[] =
		{
			sf::Vertex(sf::Vector2f(i.first.first, i.first.second), sf::Color(0,0,0,255)),
			sf::Vertex(sf::Vector2f(i.second.first, i.second.second), sf::Color(0,0,0,255))
		};
		window.draw(temp, 2, sf::Lines);
	}

	for (auto& i : graphLetters) {window.draw(i);}

	sf::VertexArray tempVerArr(sf::LinesStrip, 0);
	for (auto& i : graphPoints) {
		tempVerArr.append(sf::Vertex(sf::Vector2f(i.first, i.second), sf::Color(0, 0, 0, 255)));
	}
	window.draw(tempVerArr);
}
