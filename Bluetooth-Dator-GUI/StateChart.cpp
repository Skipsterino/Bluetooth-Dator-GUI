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
	//S?tter in alla tillst?nd i mappen
	//stateKeyMap.emplace(0, "dummystate0"); Använd inte state 0
	stateKeyMap.emplace(1, "Stop");
	stateKeyMap.emplace(2, "Dead End");
	stateKeyMap.emplace(3, "Corridor");
	stateKeyMap.emplace(4, "Out of corridor no wall");
	stateKeyMap.emplace(5, "Out of corridor left wall");
	stateKeyMap.emplace(6, "Out of corridor right wall");
	stateKeyMap.emplace(7, "Into high obstacle");
	stateKeyMap.emplace(8, "Crawling under high obstacle");
	stateKeyMap.emplace(9, "Into low obstacle");
	stateKeyMap.emplace(10, "Climbing up");
	stateKeyMap.emplace(11, "Low obstacle");
	stateKeyMap.emplace(12, "Climbing down");
	stateKeyMap.emplace(13, "Into junction A right");
	stateKeyMap.emplace(14, "Into junction A left");

	
	stateKeyMap.emplace(15, "Turn right");
	stateKeyMap.emplace(16, "Turn left");
	stateKeyMap.emplace(17, "Junction A right");
	stateKeyMap.emplace(18, "Junction A left");
	stateKeyMap.emplace(19, "Junction B right");
	stateKeyMap.emplace(20, "Junction B left");
	stateKeyMap.emplace(21, "Junction C right");
	stateKeyMap.emplace(22, "Junction C left");
	stateKeyMap.emplace(23, "End of course");
	
	stateKeyMap.emplace(24, "Out of turn right");
	stateKeyMap.emplace(25, "Out of turn left");
	stateKeyMap.emplace(26, "Out of junction A");
	stateKeyMap.emplace(27, "Out of junction A right");
	stateKeyMap.emplace(28, "Out of junction A left");
	stateKeyMap.emplace(29, "Out of junction C right");
	stateKeyMap.emplace(30, "Out of junction C left");



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

	//uppdaterar alla koordinater f?r textvektorn
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

