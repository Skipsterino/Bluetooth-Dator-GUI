/**
* @file AngleGraph.h
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Graph displaying pushed angle
*
* A class to display angles pushed 
* into the graph
*/

#ifndef ANGLEGRAPH_H
#define ANGLEGRAPH_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class AngleGraph
{
public:
	AngleGraph() = delete;

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
	AngleGraph(float, float, float, float, sf::Font*, std::string);
	~AngleGraph();

	/**
	* @brief Pushes the given value into the graph
	*
	* Pushes the value given into the graph, removing the
	* previous value
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
	std::vector<sf::Vertex*> graphLines; /**< Vertex array storing axis' and graph lines */
	sf::Vertex* angleLine; /**< Vertex with the line which displays the current value of the angle */
	std::vector<sf::Text> graphLetters; /**< Stores the text in the graph. For example gradings and graph name */
	const float xpos; /**< Stores the x position of the graph */
	const float ypos; /**< Stores the y position of the graph */
	const float width; /**< Stores the width of the graph */
	const float height; /**< Stores the height of the graph */

	/**
	* @brief Formats a text object with given values
	*
	* Gives a text object the given names, fonts, sizes, etc.
	*/
	void formatText(sf::Text&, sf::Font*, std::string, float, float);
};
#endif
