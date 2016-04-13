#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class Histogram
{
public:
	Histogram() = delete;
	Histogram(float, float, float, float, int, sf::Font*, std::string = "");
	~Histogram();

	void push(float);
	void draw(sf::RenderWindow&);
private:
	static sf::Clock timer;

	std::vector<std::pair<float, float> > distTime;
	std::vector<sf::Vertex*> graphLines;
	std::vector<std::pair<float, float> > graphPoints;
	std::vector<sf::Text> graphLetters;
	const int maxTime;
	const float xpos;
	const float ypos;
	const float width;
	const float height;
	sf::Font* fontPtr;
	int maxHeight;
	sf::Vector2f yAxisTextPosFull;
	sf::Vector2f yAxisTextPosHalf;

	float findHighestGraphPoint();
	void updateHeight(float);
	void formatText(sf::Text&, sf::Font*, std::string, float, float, bool);
};
#endif
