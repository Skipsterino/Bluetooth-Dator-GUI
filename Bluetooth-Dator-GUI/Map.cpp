
#include "Map.h"

Map::Map(float xpos, float ypos, float width, float height, uint8_t gridWidth, uint8_t gridHeight, sf::Font* fontPtr, std::string titletext) :
	corridorTimer{},
	xpos{ xpos },
	ypos{ ypos },
	width{ width },
	height{ height },
	gridSize{ gridWidth, gridHeight },
	graphLines{},
	graphLetters{},
	fontPtr{ fontPtr },
	direction{ UP },
	mapGrid{},
	curGridPos{gridWidth / 2, gridHeight}
{
}


Map::~Map()
{
}

void Map::push(float)
{
}

void Map::draw(sf::RenderWindow &)
{
}

void Map::clear()
{
}

void Map::moveInDir()
{
}

void Map::drawTurn(DIRECTION dir)
{
}

void Map::drawJunctionA(DIRECTION dir)
{
}

void Map::drawJunctionB(DIRECTION dir)
{
}

void Map::drawJunctionC(DIRECTION dir)
{
}

void Map::drawJunctionD(DIRECTION dir)
{
}

void Map::drawJunctionE(DIRECTION dir)
{
}

void Map::drawJunctionF()
{
}

void Map::drawJunctionG()
{
}

void Map::drawJunctionH(DIRECTION dir)
{
}

void Map::drawJunctionI()
{
}
