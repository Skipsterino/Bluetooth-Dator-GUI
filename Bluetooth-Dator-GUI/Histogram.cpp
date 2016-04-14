#include "Histogram.h"
#include <iostream>

sf::Clock Histogram::timer;

Histogram::Histogram(float xpos, float ypos, float width, float height, int timeLimit, sf::Font* fontPtr, std::string titletext) :
	maxTime{ timeLimit },
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	distTime{},
	graphLines{},
	graphPoints{},
	graphLetters{},
	fontPtr{ fontPtr },
	maxHeight{ 1 },
	yAxisTextPosFull{ xpos + width / 12, ypos },
	yAxisTextPosHalf{ xpos + width / 12, ypos + 9 * height / 20 }
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
	//tidsaxel noll-sträck
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
		sf::Vertex(sf::Vector2f(xpos + width / 10, ypos + height + height / 50), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width / 10, ypos + height - height / 50), sf::Color::Black)
	});
	//yaxel halv-str?ck
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width / 10 + height / 50, ypos + 9 * height / 20), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width / 10 - height / 50, ypos + 9 * height / 20), sf::Color::Black)
	});
	//yaxel hel-str?ck
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width / 10 + height / 50, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width / 10 - height / 50, ypos), sf::Color::Black)
	});

	sf::Text titel;
	sf::Text sekNoll;
	sf::Text sekHalv;
	sf::Text sekHel;
	sf::Text yHel;
	sf::Text yHalv;

	formatText(titel, fontPtr, titletext, xpos + 11*width/20, ypos - height/20 - 15, true);
	formatText(sekNoll, fontPtr, "0", xpos + width, ypos + 52 * height / 50, true);
	formatText(sekHalv, fontPtr, std::to_string(maxTime/2), xpos + 11 * width / 20, ypos + 52 * height / 50, true);
	formatText(sekHel, fontPtr, std::to_string(maxTime), xpos + width / 10, ypos + 52 * height / 50, true);
	formatText(yHel, fontPtr, "Na", yAxisTextPosFull.x, yAxisTextPosFull.y, false);
	formatText(yHalv, fontPtr, "Na", yAxisTextPosHalf.x, yAxisTextPosHalf.y, false);

	graphLetters.push_back(titel);
	graphLetters.push_back(sekNoll);
	graphLetters.push_back(sekHalv);
	graphLetters.push_back(sekHel);
	graphLetters.push_back(yHel);
	graphLetters.push_back(yHalv);
}


Histogram::~Histogram()
{
	for (auto& i : graphLines) {
		delete[] i;
	}
}

void Histogram::push(float value) {
	if (value == 0) {
		return;
	}
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

	graphPoints.insert(graphPoints.begin(), { xpos + width, ypos + height*(1 - distTime[0].first / maxHeight) });

	if (distTime[0].first > maxHeight) {
		maxHeight = distTime[0].first;
		updateHeight(maxHeight);
	}
	else if (removecount != 0) {
		maxHeight = findHighestGraphPoint() + 0.5;
		updateHeight(maxHeight);
	}

	unsigned int iterations(graphPoints.size());

	for (auto i{0}; i < iterations; ++i) {
		graphPoints[i].first = xpos + width*(1 + 9 * (maxTime - (curTime - distTime[i].second)) / maxTime) / 10;						  
	}
}

void Histogram::draw(sf::RenderWindow& window) {
	//Ritar grafaxlar och dylikt
	for (auto& i : graphLines){
		window.draw(i, 2, sf::Lines);
	}
	//Ritar all text
	for (auto& i : graphLetters) {window.draw(i);}

	//Skapar och ritar grafen
	sf::VertexArray tempVerArr(sf::LinesStrip, 0);
	for (auto& i : graphPoints) {
		tempVerArr.append(sf::Vertex(sf::Vector2f(i.first, i.second), sf::Color::Blue));
	}
	window.draw(tempVerArr);
}

float Histogram::findHighestGraphPoint() {
	float highest{ 0 };
	for (auto& i : distTime) {
		if (i.first > highest) {
			highest = i.first;
		}
	}
	return highest;
}

void Histogram::updateHeight(float newHeight) {
	unsigned int iterations{ distTime.size() };
	for (auto i{ 0 }; i < iterations; ++i) {
		graphPoints[i].second = ypos + height*(1 - distTime[i].first / newHeight);
	}
	graphLetters.at(4).setString(std::to_string(maxHeight));
	graphLetters.at(4).setPosition(sf::Vector2f(yAxisTextPosFull.x - graphLetters.at(4).getLocalBounds().width,
												yAxisTextPosFull.y - graphLetters.at(4).getLocalBounds().height));
	graphLetters.at(5).setString(std::to_string(maxHeight/2));
	graphLetters.at(5).setPosition(sf::Vector2f(yAxisTextPosHalf.x - graphLetters.at(5).getLocalBounds().width,
		yAxisTextPosHalf.y - graphLetters.at(5).getLocalBounds().height));
}

void Histogram::formatText(sf::Text& text, sf::Font* fontPtr, std::string textString, float xposition, float yposition, bool isTimeAxis) {
	text.setFont(*fontPtr);

	text.setString(textString);

	text.setCharacterSize(12);

	text.setColor(sf::Color::Black);

	if (isTimeAxis) {
		text.setPosition(sf::Vector2f(xposition - text.getLocalBounds().width / 2, yposition));
	}
	else {
		text.setPosition(sf::Vector2f(xposition - text.getLocalBounds().width, yposition - text.getLocalBounds().height));
	}
}
