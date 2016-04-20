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
	stateKeyMap.emplace(0, "dummystate0");
	stateKeyMap.emplace(1, "dummystate1");
	stateKeyMap.emplace(2, "dummystate2");
	stateKeyMap.emplace(3, "dummystate3");
	stateKeyMap.emplace(4, "dummystate4");
	stateKeyMap.emplace(5, "dummystate5");
	stateKeyMap.emplace(6, "dummystate6");
	stateKeyMap.emplace(7, "dummystate7");
	stateKeyMap.emplace(8, "dummystate8");
	stateKeyMap.emplace(9, "dummystate9");
	stateKeyMap.emplace(10, "dummystate10");
	stateKeyMap.emplace(11, "dummystate11");
	stateKeyMap.emplace(12, "dummystate12");
	stateKeyMap.emplace(13, "dummystate13");
	stateKeyMap.emplace(14, "dummystate14");
	stateKeyMap.emplace(15, "dummystate15");

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
	if (stateKeyMap.count(key) == 0) {
		std::cout << "KAN INTE HITTA TILLST?NDET \'" << key << "\', L?S!" << std::endl;
		return;
	}
	if (key == lastKey) {
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

