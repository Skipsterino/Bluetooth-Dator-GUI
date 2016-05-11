
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
	TEST_STATE_10 = 53,
} STATES;

class Map
{
public:
	Map() = delete;
	Map(float, float, float, float, uint8_t, uint8_t, sf::Font*, std::string = "");
	~Map();

	void push(uint8_t, MODE);
	void draw(sf::RenderWindow&);
	void clear();
private:
	//Variabler
	sf::Clock corridorTimer;
	std::vector<sf::Vertex*> graphLines;
	std::vector<sf::Text> graphLetters;
	const float xpos;
	const float ypos;
	const float width;
	const float height;
	const std::pair <uint8_t, uint8_t> gridSize;
	sf::Font* fontPtr;

	sf::CircleShape directionIndicator;
	sf::CircleShape robotShape;

	DIRECTION direction;
	std::vector<std::vector<std::vector<sf::Vertex*> > > mapLines;
	std::pair <uint8_t, uint8_t> curGridPos;
	uint8_t lastState;

	//Funktioner
	void updateRobotIndicator();
	void empty(uint8_t, uint8_t);

	void moveInDir();
	void rotateCW();
	void rotateCCW();

	void basicDrawLine(uint8_t, uint8_t, DIRECTION);
	void basicDrawWall(DIRECTION);
	void basicDrawDeadEnd(DIRECTION);
	void basicDrawCorridor(DIRECTION);
	void drawHighObst();
	void drawLowObst();
	void drawDeadEnd();
	void drawCorridor();
	void drawTurn(DIRECTION);
	void drawJunctionA(DIRECTION);
	void drawJunctionB(DIRECTION);
	void drawJunctionC(DIRECTION);
	void drawJunctionD(DIRECTION);
	void drawJunctionE(DIRECTION);
	void drawJunctionF();
	void drawJunctionG();
	void drawJunctionH(DIRECTION);
	void drawJunctionI();
};
#endif

