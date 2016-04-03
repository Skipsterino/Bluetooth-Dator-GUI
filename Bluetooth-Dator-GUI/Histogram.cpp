#include "Histogram.h"

sf::Clock Histogram::timer;

Histogram::Histogram(float xpos, float ypos, float width, float height, int timeLimit) :
	maxTime{ timeLimit },
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	distances{},
	graphLines{}
{
	sf::Vertex yAxis[] =
	{
		sf::Vertex(sf::Vector2f(xpos + width/10.f, ypos), sf::Color(0,0,0,255)),
		sf::Vertex(sf::Vector2f(xpos + width/10.f, ypos + height), sf::Color(0,0,0,255))
	};

	sf::Vertex xAxis[] =
	{
		sf::Vertex(sf::Vector2f(xpos, ypos + 9.f * height / 10.f), sf::Color(0,0,0,255)),
		sf::Vertex(sf::Vector2f(xpos + width, ypos + 9.f * height / 10.f), sf::Color(0,0,0,255))
	};

	//graphLines.push_back(test);
	//graphLines.push_back(xAxis);
}


Histogram::~Histogram()
{
}

void Histogram::push(float value) {

}

void Histogram::draw(sf::RenderWindow& window) {
	std::vector<sf::Vertex> bos(2);
	//window.draw(graphLines[0], 2, sf::Lines);
	//window.draw(graphLines[1], 2, sf::Lines);
}
