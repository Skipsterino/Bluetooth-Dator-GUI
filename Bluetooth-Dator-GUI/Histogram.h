#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class Histogram
{
public:
	Histogram() = delete;
	Histogram(float, float, float, float, int);
	~Histogram();

	void push(float);
	void draw(sf::RenderWindow&);
private:
	static sf::Clock timer;

	std::vector<std::pair<float, sf::Time> > distances;
	std::vector<std::pair<std::pair<float, float>,std::pair<float,float>>> graphLines;
	const int maxTime;
	const float xpos;
	const float ypos;
	const float width;
	const float height;
	sf::Vertex xAxis;
	sf::Vertex yAxis;
};
#endif
