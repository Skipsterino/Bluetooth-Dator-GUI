#ifndef STATECHART_H
#define STATECHART_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <vector>
#include <string>
#include <map>
#include <iostream>

class StateChart
{
public:
	StateChart(float, float, float, float, sf::Font*, std::string = "Tillst?ndstabell", int = 16);
	~StateChart();

	void push(int);
	void draw(sf::RenderWindow&);
	void clear();
private:
	int lastKey;
	std::vector<sf::Vertex*> graphLines;
	std::vector<sf::Text> stateTexts;
	const float xpos;
	const float ypos;
	const float width;
	const float height;
	sf::Font* fontPtr;
	const int numberOfItems;
	std::map<int, std::string> stateKeyMap;
	sf::Text graphTitle;
};

#endif

