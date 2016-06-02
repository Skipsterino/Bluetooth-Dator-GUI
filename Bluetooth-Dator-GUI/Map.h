/**
* @file Map.h
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Class making a map of the labyrinth based on state
*
* A map class mapping the labyrinth which in the robot has
* walked by looking at which autonomous' states the robot entered
*/

#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include <iostream>

enum DIRECTION {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum MODE {
	AUTO,
	MANUAL,
	RACE,
	DUNNO
};

typedef enum {
	STOP = 1,
	DEAD_END = 2,
	CORRIDOR = 3,

	OUT_OF_CORRIDOR_RIGHT_WALL = 4,
	OUT_OF_CORRIDOR_LEFT_WALL = 5,
	OUT_OF_CORRIDOR_NO_WALL = 6,

	LEFT_WALL = 7,
	RIGHT_WALL = 8,
	NO_WALL = 9,

	DETERMINE_JUNCTION_RIGHT_WALL = 10,
	DETERMINE_JUNCTION_LEFT_WALL = 11,

	DETERMINE_JUNCTION_NO_WALL = 12,

	OUT_OF_JUNCTION_RIGHT_WALL = 13,
	OUT_OF_JUNCTION_LEFT_WALL = 14,
	OUT_OF_JUNCTION_NO_WALL = 15,

	INTO_CORRIDOR_NO_WALL = 16,

	INTO_HIGH_OBSTACLE = 17,
	CRAWLING_UNDER_HIGH_OBSTACLE = 18,

	PREPARE_CLIMBING_UP = 19,
	CLIMBING_UP = 20,
	LOW_OBSTACLE = 21,
	PREPARE_CLIMBING_DOWN = 22,
	CLIMBING_DOWN = 23,

	TURN_RIGHT = 24,
	TURN_LEFT = 25,
	JUNCTION_A_RIGHT = 26,
	JUNCTION_A_LEFT = 27,
	JUNCTION_B_RIGHT = 28,
	JUNCTION_B_LEFT = 29,
	JUNCTION_C_RIGHT = 30,
	JUNCTION_C_LEFT = 31,
	JUNCTION_D_RIGHT = 32,
	JUNCTION_D_LEFT = 33,
	JUNCTION_D_STRAIGHT = 34,
	JUNCTION_E_RIGHT = 35,
	JUNCTION_E_LEFT = 36,
	JUNCTION_F = 37,
	JUNCTION_G = 38,
	JUNCTION_H_RIGHT = 39,
	JUNCTION_H_LEFT = 40,
	JUNCTION_I = 41,

	JUNCTION_I_OR_END = 42,
	END_OF_COURSE = 43,

	SLOW_CORRIDOR = 44,			// TEST_STATE_1
	DEAD_END_A_RIGHT = 45,		// TEST_STATE_2	
	DEAD_END_A_LEFT = 46,		// TEST_STATE_3	
	DEAD_END_B_RIGHT = 47,		// TEST_STATE_4
	DEAD_END_B_LEFT = 48,		// TEST_STATE_5
	DEAD_END_C = 49,			// TEST_STATE_6
	DEAD_END_D = 50,			// TEST_STATE_7
	CHECK_IF_LOW_OBSTACLE = 51,
	CHECK_IF_HIGH_OBSTACLE = 52,
	CENTER_CRAB_DOWN = 53,
	CENTER_CRAB_UP = 54,
} STATES;

class Map
{
public:
	Map() = delete;
	/**
	* @brief Class constructor
	*
	* @param xpos The X-position of the map
	* @param ypos The Y-position of the map
	* @param width The width of the map
	* @param height The height of the map
	* @param gridWidth The number of blocks the map has in width
	* @param gridHeight The number of blocks the map has in height
	* @param font Pointer to the font the text in the map uses
	* @param titletext String with the given name of the map
	* @param numberOfItems Int with the number of states the chart should hold
	*/
	Map(float, float, float, float, uint8_t, uint8_t, sf::Font*, std::string = "");
	~Map();

	/**
	* @brief Pushes a state into the map
	*
	* Pushes a state into the map, which then, based on which state,
	* either draws a segment or ignore the state if its a transition state
	* @param stateNum The number corresponding to a given state
	* @param mode The mode the robot is currently in. For example AUTO or MANUAL
	*/
	void push(uint8_t, MODE);

	/**
	* @brief Draws the entire map
	*
	* When called the function draws all components of the map
	* on the given RenderWindow
	* @param window The RenderWindow upon which to draw the graph
	*/
	void draw(sf::RenderWindow&);

	/**
	* @brief Clear the whole chart of states
	*/
	void clear();
private:
	//Variabler
	sf::Clock corridorTimer; /**< A timer to keep track of how long the robot has been in a corridor */
	std::vector<sf::Vertex*> graphLines; /**< Holds all lines which makes the map grid */
	std::vector<sf::Text> graphLetters; /**< Holds all graph text */
	const float xpos; /**< Stores the x position of the map */
	const float ypos; /**< Stores the y position of the map */
	const float width; /**< Stores the width of the map */
	const float height; /**< Stores the height of the map */
	const std::pair <uint8_t, uint8_t> gridSize; /**< Stores the grid width och height of the map */
	sf::Font* fontPtr; /**< Pointer to the font the text uses */

	sf::CircleShape directionIndicator; /**< Stores the circle that indicate the direction of the robot */
	sf::CircleShape robotShape; /**< Holds the circle which shows the robots position in the map */

	DIRECTION direction; /**< Stores the robots direction */
	std::vector<std::vector<std::vector<sf::Vertex*> > > mapLines; /**< 3D vector holding all drawn map segments */
	std::pair <uint8_t, uint8_t> curGridPos; /**< Stores the current grid position of the robot in the map */
	uint8_t lastState; /**< Stores the last valid state of the robot */

	/**
	* @brief Updates the robot indicator in the map
	*/
	void updateRobotIndicator();

	/**
	* @brief Empties a given spot in the map
	*/
	void empty(uint8_t, uint8_t);

	/**
	* @brief Moves the robot in its current direction
	*/
	void moveInDir();

	/**
	* @brief Rotates the robot clockwise
	*/
	void rotateCW();

	/**
	* @brief Rotates the robot counterclockwise
	*/
	void rotateCCW();

	/**
	* @brief Basic function to draw a line in the map in a location and in an absolute direction
	*/
	void basicDrawLine(uint8_t, uint8_t, DIRECTION);

	/**
	* @brief Basic function to draw a wall in the map in an absolute direction
	*/
	void basicDrawWall(DIRECTION);

	/**
	* @brief Basic function to draw a dead end in the map in an absolute direction
	*/
	void basicDrawDeadEnd(DIRECTION);

	/**
	* @brief Basic function to draw a corridor in the map in an absolute direction
	*/
	void basicDrawCorridor(DIRECTION);

	/**
	* @brief Draws a high obsticle at the current position
	*/
	void drawHighObst();

	/**
	* @brief Draws a low obsticle at the current position
	*/
	void drawLowObst();

	/**
	* @brief Draws a dead end at the current position
	*/
	void drawDeadEnd();

	/**
	* @brief Draws the dead end A at the current position
	*/
	void drawDeadEndA(DIRECTION);

	/**
	* @brief Draws the dead end B at the current position
	*/
	void drawDeadEndB(DIRECTION);

	/**
	* @brief Draws the dead end C at the current position
	*/
	void drawDeadEndC();
	/**
	* @brief Draws the dead end D at the current position
	*/
	void drawDeadEndD();

	/**
	* @brief Draws a corridor at the current position
	*/
	void drawCorridor();

	/**
	* @brief Draws a turn in a direction at the current position
	*/
	void drawTurn(DIRECTION);

	/**
	* @brief Draws junction A the the current position
	*/
	void drawJunctionA(DIRECTION);

	/**
	* @brief Draws junction B the the current position
	*/
	void drawJunctionB(DIRECTION);

	/**
	* @brief Draws junction C the the current position
	*/
	void drawJunctionC(DIRECTION);

	/**
	* @brief Draws junction D the the current position
	*/
	void drawJunctionD(DIRECTION);

	/**
	* @brief Draws junction E the the current position
	*/
	void drawJunctionE(DIRECTION);

	/**
	* @brief Draws junction F the the current position
	*/
	void drawJunctionF();

	/**
	* @brief Draws junction G the the current position
	*/
	void drawJunctionG();

	/**
	* @brief Draws junction H the the current position
	*/
	void drawJunctionH(DIRECTION);

	/**
	* @brief Draws junction I the the current position
	*/
	void drawJunctionI();
};
#endif

