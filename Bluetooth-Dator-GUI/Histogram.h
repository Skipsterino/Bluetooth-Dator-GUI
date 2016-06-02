/**
* @file Histogram.h
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Graph class which plots values against a time axis
*
* Plots the last 10 seconds of values pushed into
* the graph class
*/

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class Histogram
{
public:
	Histogram() = delete;

	/**
	* @brief Class constructor
	*
	* @param xpos The X-position of the graphics of the graph
	* @param ypos The Y-position of the graphics of the graph
	* @param width The width of the graphics of the graph
	* @param height The height of the graphics of the graph
	* @param font Pointer to the font the text in the graph uses
	* @param titletext String with the given name of the graph
	*/
	Histogram(float, float, float, float, int, sf::Font*, std::string = "");
	~Histogram();

	/**
	* @brief Pushes the given value into the graph
	*
	* Pushes the value given into the graph and removes eventual
	* values exceeding their 10 seconds life frame
	* @param value The value which is pushed into the graph
	*/
	void push(float);

	/**
	* @brief Draws the entire Graph
	*
	* When called the function draws all components of the graph
	* on the given RenderWindow
	* @param window The RenderWindow upon which to draw the graph
	*/
	void draw(sf::RenderWindow&);
private:
	static sf::Clock timer;  /**< Static clock for all graphs to keep track of a values life length */

	std::vector<std::pair<float, float> > distTime; /**< Stores all the values currently in the graph and their timestamp */
	std::vector<sf::Vertex*> graphLines; /**< Vertex array storing the x-axis, y-axis and graph lines */
	std::vector<std::pair<float, float> > graphPoints; /**< distTime values converted to coordinates in the graph */
	std::vector<sf::Text> graphLetters; /**< Stores the text in the graph. For example gradings and graph name */
	const int maxTime; /**< Int storing maximum life time of values */
	const float xpos; /**< Stores the x position of the graph */
	const float ypos; /**< Stores the y position of the graph */
	const float width; /**< Stores the width of the graph */
	const float height; /**< Stores the height of the graph */
	sf::Font* fontPtr; /**< Pointer to the font the text in the graph uses */
	int maxHeight; /**< Holds the value of the largest value currently in the graph */
	sf::Vector2f yAxisTextPosFull;  /**< Stores the position of the max-value text indicator of the y axis */
	sf::Vector2f yAxisTextPosHalf; /**< Stores the position of the half-max-value text indicator of the y axis */

	/**
	* @brief Finds the largest value in distTime and returns it
	*/
	float findHighestGraphPoint();

	/**
	* @brief Updates height of all graphPoints
	*
	* Updates the height of all values in graphPoints relative
	* to the maxHeight value
	* @return The highest value currently in the graph
	*/
	void updateHeight(float);

	/**
	* @brief Formats a text object with given values
	*
	* Gives a text object the given names, fonts, sizes, etc.
	*/
	void formatText(sf::Text&, sf::Font*, std::string, float, float, bool);
};
#endif
