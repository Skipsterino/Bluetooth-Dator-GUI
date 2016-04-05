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
	graphLines.push_back({ {xpos + width / 10.f, ypos}, 
						   { xpos + width / 10.f, ypos + height} });
	graphLines.push_back({ { xpos, ypos + 9.f * height / 10.f },
						   { xpos + width, ypos + 9.f * height / 10.f } });

}


Histogram::~Histogram()
{
}

void Histogram::push(float value) {

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
	//window.draw(yAxis, 2, sf::Lines);
}
