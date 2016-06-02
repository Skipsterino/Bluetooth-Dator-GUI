/**
* File: StateChart.cpp
* Version: 1.0
* Last edited: 2 juni 2016
* Author(s): Kevin Kjell?n
*/

#include "StateChart.h"

StateChart::StateChart(float xpos, float ypos, float width, float height, sf::Font* fontPtr, std::string title, int numberOfItems) :
	lastKey{0},
	xpos{xpos},
	ypos{ypos},
	width{width},
	height{height},
	fontPtr{fontPtr},
	numberOfItems{numberOfItems},
	graphTitle{}
{
	//S?tter in alla tillst?nd i mappen, skippa 0 pls
	stateKeyMap.emplace(1, "Stop");
	stateKeyMap.emplace(2, "Dead End");
	stateKeyMap.emplace(3, "Corridor");

	stateKeyMap.emplace(4, "Out of corridor right wall");
	stateKeyMap.emplace(5, "Out of corridor left wall");
	stateKeyMap.emplace(6, "Out of corridor no wall");

	stateKeyMap.emplace(7, "Left wall");
	stateKeyMap.emplace(8, "Right wall");
	stateKeyMap.emplace(9, "No wall");

	stateKeyMap.emplace(10, "Determine junction right wall");
	stateKeyMap.emplace(11, "Determine junction left wall");
	stateKeyMap.emplace(12, "Determine junction no wall");

	stateKeyMap.emplace(13, "Out of junction right wall");
	stateKeyMap.emplace(14, "Out of junction left wall");
	stateKeyMap.emplace(15, "Out of junction no wall");

	stateKeyMap.emplace(16, "Into corridor no wall");

	stateKeyMap.emplace(17, "Into high obstacle");
	stateKeyMap.emplace(18, "Crawling under high obstacle");

	stateKeyMap.emplace(19, "Prepare climbing up");
	stateKeyMap.emplace(20, "Climbing up");
	stateKeyMap.emplace(21, "Low obstacle");
	stateKeyMap.emplace(22, "Prepare climbing down");
	stateKeyMap.emplace(23, "Climbing down");

	stateKeyMap.emplace(24, "Turn right");
	stateKeyMap.emplace(25, "Turn left");

	stateKeyMap.emplace(26, "Junction A right");
	stateKeyMap.emplace(27, "Junction A left");
	stateKeyMap.emplace(28, "Junction B right");
	stateKeyMap.emplace(29, "Junction B left");
	stateKeyMap.emplace(30, "Junction C right");
	stateKeyMap.emplace(31, "Junction C left");
	stateKeyMap.emplace(32, "Junction D right");
	stateKeyMap.emplace(33, "Junction D left");
	stateKeyMap.emplace(34, "Junction D straight");
	stateKeyMap.emplace(35, "Junction E right");
	stateKeyMap.emplace(36, "Junction E left");
	stateKeyMap.emplace(37, "Junction F");
	stateKeyMap.emplace(38, "Junction G");
	stateKeyMap.emplace(39, "Junction H right");
	stateKeyMap.emplace(40, "Junction H left");
	stateKeyMap.emplace(41, "Junction I");

	stateKeyMap.emplace(42, "Junction I or end");
	stateKeyMap.emplace(43, "End of course");

	stateKeyMap.emplace(44, "Slow corridor");
	stateKeyMap.emplace(45, "Dead end A right");
	stateKeyMap.emplace(46, "Dead end A left");
	stateKeyMap.emplace(47, "Dead end B right");
	stateKeyMap.emplace(48, "Dead end B left");
	stateKeyMap.emplace(49, "Dead end C");
	stateKeyMap.emplace(50, "Dead end D");
	stateKeyMap.emplace(51, "Check if low obstacle");
	stateKeyMap.emplace(52, "Check if high obstacle");
	stateKeyMap.emplace(53, "Center crab down");
	stateKeyMap.emplace(54, "Center crab up");

	//v?nster lodr?t linje
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos, ypos + height), sf::Color::Black)
	});
	//h?ger lodr?t linje
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width, ypos + height), sf::Color::Black)
	});
	//Alla horisontella linjer
	for (int i{ 0 }; i < numberOfItems + 1; ++i) {
		graphLines.push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos, ypos + i*height/numberOfItems), sf::Color::Black),
			sf::Vertex(sf::Vector2f(xpos + width, ypos + i*height / numberOfItems), sf::Color::Black)
		});
	}

	graphTitle.setFont(*fontPtr);
	graphTitle.setString(title);
	graphTitle.setCharacterSize(12);
	graphTitle.setColor(sf::Color::Black);
	graphTitle.setPosition(sf::Vector2f(xpos + width / 2 - graphTitle.getLocalBounds().width / 2, ypos - height / 50 - graphTitle.getLocalBounds().height));
}

StateChart::~StateChart(){
	for (auto& i : graphLines) {
		delete[] i;
	}
}

void StateChart::push(int key){
	//Kollar s? nyckeln finns och att det inte ?r samma som f?rra

	if (!key || key == lastKey)
	{
		return;
	}

	if (stateKeyMap.count(key) == 0) {
		std::cout << "KAN INTE HITTA TILLST?NDET \'" << key << "\', L?S!" << std::endl;
		return;
	}

	lastKey = key;

	//Skapar och s?tter in nya texten i textvektorn
	sf::Text newStateText;
	newStateText.setFont(*fontPtr);
	newStateText.setString(stateKeyMap.at(key));
	newStateText.setCharacterSize((int) 3 * height / (4 * numberOfItems));
	newStateText.setColor(sf::Color::Black);
	stateTexts.insert(stateTexts.begin(), newStateText);

	//g?r vektorn lagom stor
	if (stateTexts.size() > numberOfItems) {
		stateTexts.pop_back();
	}

	//uppdaterar alla koordinater f?r texterna
	for (int i{ 0 }; i < stateTexts.size(); ++i) {
		stateTexts[i].setPosition(sf::Vector2f(xpos + width / 50, ypos + height / (2 * numberOfItems) - stateTexts[i].getLocalBounds().height/ 1.5 + i * height / numberOfItems));
	}
}

void StateChart::draw(sf::RenderWindow& window){
	//Ritar grafaxlar och dylikt
	for (auto& i : graphLines) {
		window.draw(i, 2, sf::Lines);
	}
	//Ritar graftiteln
	window.draw(graphTitle);
	//Ritar all text
	for (auto& i : stateTexts) { window.draw(i); }

}

void StateChart::clear()
{
	stateTexts.clear();
}

