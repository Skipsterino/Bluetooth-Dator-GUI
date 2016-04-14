#ifndef ANGLEGRAPH_H
#define ANGLEGRAPH_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class AngleGraph
{
public:
	AngleGraph() = delete;
	AngleGraph(float, float, float, float, sf::Font*, std::string);
	~AngleGraph();

	void push(float);
	void draw(sf::RenderWindow&);
private:
	std::vector<sf::Vertex*> graphLines;
	sf::Vertex* angleLine;
	std::vector<sf::Text> graphLetters;
	const float xpos;
	const float ypos;
	const float width;
	const float height;

	void formatText(sf::Text&, sf::Font*, std::string, float, float);
};
#endif
