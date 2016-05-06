/*
#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class Map
{
public:
	Map() = delete;
	Map(float, float, float, float, sf::Font*, std::string = "");
	~Map();

	void push(float);
	void draw(sf::RenderWindow&);
private:
	sf::Clock corridorTimer;

	std::vector<sf::Vertex*> graphLines;
	std::vector<sf::Text> graphLetters;
	const float xpos;
	const float ypos;
	const float width;
	const float height;
	sf::Font* fontPtr;
};
#endif
*/
