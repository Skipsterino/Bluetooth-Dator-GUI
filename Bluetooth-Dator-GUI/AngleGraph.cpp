#include "AngleGraph.h"
#include <cmath>



AngleGraph::AngleGraph(float xpos, float ypos, float width, float height, sf::Font* fontPtr, std::string titletext) :
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	graphLines{},
	angleLine{},
	graphLetters{}
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
	//Åã

	sf::Text titel;
	sf::Text angleText;

	formatText(titel, fontPtr, titletext, xpos + width / 2, ypos - height / 20 - 15);
	formatText(angleText, fontPtr, "Na", xpos + 3*width / 4, ypos + height / 4 - 15);

	graphLetters.push_back(titel);
	graphLetters.push_back(angleText);
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

	graphLetters.at(1).setString(std::to_string((int)angle).append("\xB0"));
}

void AngleGraph::draw(sf::RenderWindow& window) {
	//Ritar ut grafen i sig
	for (auto& i : graphLines) {
		window.draw(i, 2, sf::Lines);
	}

	//Ritar all text
	for (auto& i : graphLetters) { window.draw(i); }

	//Ritar linjen som visar vinkeln
	window.draw(angleLine, 2, sf::Lines);
}

void AngleGraph::formatText(sf::Text& text, sf::Font* fontPtr, std::string textString, float xposition, float yposition) {
	text.setFont(*fontPtr);

	text.setString(textString);

	text.setCharacterSize(12);

	text.setColor(sf::Color::Black);

	text.setPosition(sf::Vector2f(xposition - text.getLocalBounds().width / 2, yposition));

}
