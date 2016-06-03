/**
* @file StateChart.h
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Chart displaying latest states in auto-mode
*
* A chart displaying the latest states the robot has been in
* by mapping the pushed in number against a number-to-state conversion
* chart
*/

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
	/**
	* @brief Class constructor
	*
	* @param xpos The X-position of the graphics of the graph
	* @param ypos The Y-position of the graphics of the graph
	* @param width The width of the graphics of the graph
	* @param height The height of the graphics of the graph
	* @param fontPtr Pointer to the font the text in the graph uses
	* @param title String with the given name of the graph
	* @param numberOfItems Int with the number of states the chart should hold
	*/
	StateChart(float, float, float, float, sf::Font*, std::string = "State Chart", int = 16);
	~StateChart();

	/**
	* @brief Pushes the given value into the graph
	*
	* Pushes the value given into the graph and matches
	* it to a specific state in stateKeyMap
	* @param key The value ( or state) which is pushed into the graph
	*/
	void push(int);

	/**
	* @brief Draws the entire stateChart
	*
	* When called the function draws all components of the chart
	* on the given RenderWindow
	* @param window The RenderWindow upon which to draw the graph
	*/
	void draw(sf::RenderWindow&);

	/**
	* @brief Clear the whole chart of states
	*/
	void clear();
private:
	int lastKey; /**< Int storing the value of the last valid state pushed in */
	std::vector<sf::Vertex*> graphLines; /**< Vertex array storing the lines the chart consists of */
	std::vector<sf::Text> stateTexts; /**< Array holding all the texts (or states) currently in the chart */
	const float xpos; /**< Stores the x position of the chart */
	const float ypos; /**< Stores the y position of the chart */
	const float width; /**< Stores the width of the chart */
	const float height; /**< Stores the height of the chart */
	sf::Font* fontPtr; /**< Pointer to the font the text in the graph uses */
	const int numberOfItems; /**< Stores how many states can be stored in the chart */
	std::map<int, std::string> stateKeyMap; /**< Map which matches all states with their given value */
	sf::Text graphTitle; /**< Text object holding the graphs title */
};

#endif

