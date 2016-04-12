#include "Histogram.h"
#include <iostream>

sf::Clock Histogram::timer;

Histogram::Histogram(float xpos, float ypos, float width, float height, int timeLimit, sf::Font* fontPtr) :
	maxTime{ timeLimit },
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	distTime{},
	graphLines{},
	graphPoints{},
	graphLetters{},
	fontPtr{ fontPtr }
{
	//y-axel
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width / 10.f, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width / 10.f, ypos + height), sf::Color::Black)
	});
	//x-axel
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos, ypos + height), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width, ypos + height), sf::Color::Black)
	});
	//tidsaxel 0-sträck
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width, ypos + height - height / 50), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width, ypos + height + height / 50), sf::Color::Black)
	});
	//tidsaxel halv-sträck
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + 11 * width / 20, ypos + height - height / 50), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + 11 * width / 20, ypos + height + height / 50), sf::Color::Black)
	});
	//tidsaxel hel-sträck
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width / 10, ypos + height - height / 50), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width / 10.f, ypos + height), sf::Color::Black)
	});

	sf::Text sekNoll;
	sf::Text sekHalv;
	sf::Text sekHel;

	sekNoll.setFont(*fontPtr);
	sekHalv.setFont(*fontPtr);
	sekHel.setFont(*fontPtr);

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
	for (auto& i : graphLines) {
		delete[] i;
	}
}

void Histogram::push(float value) {
	float curTime = timer.getElapsedTime().asSeconds();

	distTime.insert(distTime.begin(),{ value, curTime });

	unsigned int removecount{ 0 };
	while (timer.getElapsedTime().asSeconds() - distTime[distTime.size() - 1].second > maxTime) {
		distTime.pop_back();
		++removecount;
	}

	for (auto i{0}; i < removecount; ++i) {
		graphPoints.pop_back();
	}

	graphPoints.insert(graphPoints.begin(), { xpos + width,
		ypos + height*(1 - distTime[0].first / 100) });

	unsigned int iterations(graphPoints.size());

	for (auto i{0}; i < iterations; ++i) {
		graphPoints[i].first = xpos + width*(1 + 9 * (maxTime - (curTime - distTime[i].second)) / maxTime) / 10;
							  
	}
}

void Histogram::draw(sf::RenderWindow& window) {
	for (auto& i : graphLines){
		window.draw(i, 2, sf::Lines);
	}

	for (auto& i : graphLetters) {window.draw(i);}

	sf::VertexArray tempVerArr(sf::LinesStrip, 0);
	for (auto& i : graphPoints) {
		tempVerArr.append(sf::Vertex(sf::Vector2f(i.first, i.second), sf::Color::Blue));
	}
	window.draw(tempVerArr);
}
