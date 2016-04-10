#include "AngleGraph.h"
#include <cmath>



AngleGraph::AngleGraph(float xpos, float ypos, float width, float height) :
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	graphLines{},
	angleLine{},
	graphLetters{},
	font{}
{
	//y-axel
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width / 2.f, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width / 2.f, ypos + height), sf::Color::Black)
	});
	//x-axel
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos, ypos + height / 2.f), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width, ypos + height/2.f), sf::Color::Black)
	});

	//l?gger in f?rsta linjen med vinkel 0
	angleLine = new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width / 4.f, ypos + height / 2.f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(xpos + 3.f*width / 4.f, ypos + height / 2.f), sf::Color::Blue)
	};

}

AngleGraph::~AngleGraph()
{
	delete[] angleLine;

	for (auto& i : graphLines) {
		delete[] i;
	}
}

void AngleGraph::push(float angle) {
	delete[] angleLine;

	float radAngle = -3.1415 * angle / 180.f;

	angleLine = new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width / 2.f + 3.f*width*std::cosf(radAngle)/8.f, ypos + height / 2.f + 3.f*height*std::sinf(radAngle) / 8.f), sf::Color::Blue),
		sf::Vertex(sf::Vector2f(xpos + width / 2.f - 3.f*width*std::cosf(radAngle) / 8.f, ypos + height / 2.f - 3.f*height*std::sinf(radAngle) / 8.f), sf::Color::Blue)
	};
}

void AngleGraph::draw(sf::RenderWindow& window) {
	for (auto& i : graphLines) {
		window.draw(i, 2, sf::Lines);
	}

	window.draw(angleLine, 2, sf::Lines);
}
