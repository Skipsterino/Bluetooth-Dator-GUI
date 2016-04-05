#include "Histogram.h"

sf::Clock Histogram::timer;

Histogram::Histogram(float xpos, float ypos, float width, float height, int timeLimit) :
	maxTime{ timeLimit },
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	distTime{},
	graphLines{}
{
	//y-axel
	graphLines.push_back({ {xpos + width / 10.f, ypos}, 
						   { xpos + width / 10.f, ypos + height} });
	//x-axel
	graphLines.push_back({ { xpos, ypos + 9.f * height / 10.f },
						   { xpos + width, ypos + 9.f * height / 10.f } });

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
							   ypos + 9*height/10*(1 - i.first/100) });
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

	sf::VertexArray tempVerArr(sf::LinesStrip, 0);

	for (auto& i : graphPoints) {
		tempVerArr.append(sf::Vertex(sf::Vector2f(i.first, i.second), sf::Color(0, 0, 0, 255)));
	}
	window.draw(tempVerArr);
}
