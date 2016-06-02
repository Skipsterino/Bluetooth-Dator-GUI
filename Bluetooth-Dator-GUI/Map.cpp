
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
	robotShape{},
	directionIndicator{},
	direction{ UP },
	mapLines{},
	curGridPos{gridWidth / 2, 3 * (gridHeight - 1) / 4},
	lastState{0}
{
	//Skapar en 3D-vektor stor nog att h?lla i kartan
	mapLines.resize(gridWidth);
	for (auto& i :mapLines) {
		i.resize(gridHeight);
	}

	//Fixar inst?llningar till robotcirkeln
	robotShape.setRadius(width / (6 * gridWidth));
	robotShape.setFillColor(sf::Color(255,192,203));
	robotShape.setOutlineThickness(1);
	robotShape.setOutlineColor(sf::Color::Black);
	robotShape.setOrigin(robotShape.getRadius(), robotShape.getRadius());

	directionIndicator.setRadius(width / (12 * gridWidth));
	directionIndicator.setFillColor(sf::Color::Black);
	directionIndicator.setOrigin(directionIndicator.getRadius(), directionIndicator.getRadius() + robotShape.getRadius());

	updateRobotIndicator();

	//v?nster lodr?t linje
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos, ypos + height), sf::Color::Black)
	});
	//h?ger lodr?t linje
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos + width, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width, ypos + height), sf::Color::Black)
	});
	//?vre horisontell linje
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos, ypos), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width, ypos), sf::Color::Black)
	});
	//undre horisontell linje
	graphLines.push_back(new sf::Vertex[2]{
		sf::Vertex(sf::Vector2f(xpos, ypos + height), sf::Color::Black),
		sf::Vertex(sf::Vector2f(xpos + width, ypos + height), sf::Color::Black)
	});
	//Alla 'mellan' horisontella linjer
	for (int i{ 1 }; i < gridHeight; ++i) {
		graphLines.push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos, ypos + i*height / gridHeight), sf::Color(0,0,255,30)),
			sf::Vertex(sf::Vector2f(xpos + width, ypos + i*height / gridHeight), sf::Color(0,0,255,30))
		});
	}
	//Alla 'mellan' lodr?ta linjer
	for (int i{ 1 }; i < gridWidth; ++i) {
		graphLines.push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + i*width/gridWidth, ypos), sf::Color(0,0,255,30)),
			sf::Vertex(sf::Vector2f(xpos + i*width/gridWidth, ypos + height), sf::Color(0,0,255,30))
		});
	}
}

Map::~Map()
{
	for (auto& i : graphLines) {
		delete[] i;
	}
	for (auto& i : mapLines) {
		for (auto& j : i) {
			for (auto& k : j) {
				delete[] k;
			}
		}
	}
}

void Map::push(uint8_t stateNum, MODE mode)
{
	//M?ste vara autonomt
	if (mode != AUTO) {
		corridorTimer.restart();
		return;
	}
	//Om korridor f?r l?nge rita 2 korridorssegment ist?llet f?r ett
	if ((stateNum == CORRIDOR || stateNum == SLOW_CORRIDOR) &&
		(lastState == CORRIDOR || lastState == SLOW_CORRIDOR)) {
		if (corridorTimer.getElapsedTime().asSeconds() >= 4) {
			moveInDir();
			drawCorridor();
			corridorTimer.restart();
		}
		else {
			return;
		}
	}
	//Om samma state som f?rra, strunta i
	if (stateNum == lastState || stateNum == 0) {
		return;
	}

	//Om n?got state som ska triggas p?, rita ut korsningen
	switch (stateNum)
	{
	case SLOW_CORRIDOR:
	case CORRIDOR:
		moveInDir();
		drawCorridor();
		corridorTimer.restart();
		lastState = stateNum;
		break;
	case CRAWLING_UNDER_HIGH_OBSTACLE:
		moveInDir();
		drawHighObst();
		lastState = stateNum;
		break;
	case LOW_OBSTACLE:
		moveInDir();
		drawLowObst();
		lastState = stateNum;
		break;
	case DEAD_END:
		moveInDir();
		drawDeadEnd();
		rotateCW();
		rotateCW();
		lastState = stateNum;
		break;
	case DEAD_END_A_LEFT:
		moveInDir();
		drawDeadEndA(LEFT);
		rotateCW();
		rotateCW();
		lastState = stateNum;
		break;
	case DEAD_END_A_RIGHT:
		moveInDir();
		drawDeadEndA(RIGHT);
		rotateCW();
		rotateCW();
		lastState = stateNum;
		break;
	case DEAD_END_B_LEFT:
		moveInDir();
		drawDeadEndB(LEFT);
		rotateCW();
		rotateCW();
		lastState = stateNum;
		break;
	case DEAD_END_B_RIGHT:
		moveInDir();
		drawDeadEndB(RIGHT);
		rotateCW();
		rotateCW();
		lastState = stateNum;
		break;
	case DEAD_END_C:
		moveInDir();
		drawDeadEndC();
		rotateCW();
		rotateCW();
		lastState = stateNum;
		break;
	case DEAD_END_D:
		moveInDir();
		drawDeadEndD();
		rotateCW();
		rotateCW();
		lastState = stateNum;
		break;
	case TURN_RIGHT:
		moveInDir();
		drawTurn(RIGHT);
		rotateCW();
		lastState = stateNum;
		break;
	case TURN_LEFT:
		moveInDir();
		drawTurn(LEFT);
		rotateCCW();
		lastState = stateNum;
		break;
	case JUNCTION_A_RIGHT:
		moveInDir();
		drawJunctionA(RIGHT);
		rotateCW();
		lastState = stateNum;
		break;
	case JUNCTION_A_LEFT:
		moveInDir();
		drawJunctionA(LEFT);
		rotateCCW();
		lastState = stateNum;
		break;
	case JUNCTION_B_RIGHT:
		moveInDir();
		drawJunctionB(RIGHT);
		lastState = stateNum;
		break;
	case JUNCTION_B_LEFT:
		moveInDir();
		drawJunctionB(LEFT);
		lastState = stateNum;
		break;
	case JUNCTION_C_RIGHT:
		moveInDir();
		drawJunctionC(RIGHT);
		rotateCW();
		lastState = stateNum;
		break;
	case JUNCTION_C_LEFT:
		moveInDir();
		drawJunctionC(LEFT);
		rotateCCW();
		lastState = stateNum;
		break;
	case JUNCTION_D_RIGHT:
		moveInDir();
		drawJunctionD(RIGHT);
		rotateCW();
		lastState = stateNum;
		break;
	case JUNCTION_D_LEFT:
		moveInDir();
		drawJunctionD(LEFT);
		rotateCCW();
		lastState = stateNum;
		break;
	case JUNCTION_D_STRAIGHT:
		moveInDir();
		drawJunctionD(UP);
		lastState = stateNum;
		break;
	case JUNCTION_E_RIGHT:
		moveInDir();
		drawJunctionE(RIGHT);
		lastState = stateNum;
		break;
	case JUNCTION_E_LEFT:
		moveInDir();
		drawJunctionE(LEFT);
		rotateCCW();
		lastState = stateNum;
		break;
	case JUNCTION_F:
		moveInDir();
		drawJunctionF();
		lastState = stateNum;
		break;
	case JUNCTION_G:
		moveInDir();
		drawJunctionG();
		rotateCCW();
		lastState = stateNum;
		break;
	case JUNCTION_H_RIGHT:
		moveInDir();
		drawJunctionH(RIGHT);
		lastState = stateNum;
		break;
	case JUNCTION_H_LEFT:
		moveInDir();
		drawJunctionH(LEFT);
		rotateCCW();
		lastState = stateNum;
		break;
	case JUNCTION_I:
		moveInDir();
		drawJunctionI();
		rotateCCW();
		lastState = stateNum;
		break;
	}
}

void Map::draw(sf::RenderWindow& window)
{
	//Ritar ut kartan i sig
	for (auto& i : graphLines) {
		window.draw(i, 2, sf::Lines);
	}
	//Ritar ut labyrinten som kartlagts
	for (auto& i : mapLines) {
		for (auto& j : i) {
			for (auto& k : j) {
				window.draw(k, 2, sf::Lines);
			}
		}
	}
	//Ritar ut robotcirkeln
	window.draw(robotShape);
	window.draw(directionIndicator);
}

void Map::clear()
{
	for (auto& i : mapLines) {
		for (auto& j : i) {
			for (auto& k : j) {
				delete[] k;
			}
			j.clear();
		}
	}
	curGridPos = {gridSize.first / 2, 3*(gridSize.second - 1)/4};
	direction = UP;
	directionIndicator.setRotation(0);
	updateRobotIndicator();
}

void Map::updateRobotIndicator()
{
	robotShape.setPosition(xpos + width / (2 * gridSize.first) + curGridPos.first * width / gridSize.first, ypos + height / (2 * gridSize.second) + curGridPos.second * height / gridSize.second);
	directionIndicator.setPosition(xpos + width / (2 * gridSize.first) + curGridPos.first * width / gridSize.first, ypos + height / (2 * gridSize.second) + curGridPos.second * height / gridSize.second);
}

void Map::empty(uint8_t xGridPos, uint8_t yGridPos)
{
	if (xGridPos >= gridSize.first || yGridPos >= gridSize.second) {
		clear();
		return;
	}

	for (auto& i : mapLines[xGridPos][yGridPos]) {
		delete[] i;
	}
	mapLines[xGridPos][yGridPos].clear();
}

void Map::moveInDir()
{
	switch (direction)
	{
	case UP:
		curGridPos.second += -1;
		break;
	case RIGHT:
		curGridPos.first += 1;
		break;
	case DOWN:
		curGridPos.second += 1;
		break;
	case LEFT:
		curGridPos.first += -1;
		break;
	}

	if (curGridPos.first >= gridSize.first || curGridPos.second >= gridSize.second) {
		clear();
	}
	updateRobotIndicator();
}

void Map::rotateCW()
{
	direction = (DIRECTION)((direction + 1) % 4);
	directionIndicator.rotate(90);
}

void Map::rotateCCW()
{
	direction = (DIRECTION)((direction + 3) % 4);
	directionIndicator.rotate(-90);
}

void Map::basicDrawLine(uint8_t xGridPos, uint8_t yGridPos, DIRECTION dir)
{
	if (xGridPos >= gridSize.first || yGridPos >= gridSize.second) {
		clear();
		return;
	}
	switch (dir)
	{
	case UP:
		mapLines[xGridPos][yGridPos].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + xGridPos * width / gridSize.first, ypos + yGridPos * height / gridSize.second), sf::Color::Black),
			sf::Vertex(sf::Vector2f(xpos + (1 + xGridPos) * width / gridSize.first, ypos + yGridPos * height / gridSize.second), sf::Color::Black)
		});
		break;
	case RIGHT:
		mapLines[xGridPos][yGridPos].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + (1 + xGridPos) * width / gridSize.first, ypos + yGridPos * height / gridSize.second), sf::Color::Black),
			sf::Vertex(sf::Vector2f(xpos + (1 + xGridPos) * width / gridSize.first, ypos + (1 + yGridPos) * height / gridSize.second), sf::Color::Black)
		});
		break;
	case DOWN:
		mapLines[xGridPos][yGridPos].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + xGridPos * width / gridSize.first, ypos + (1 + yGridPos) * height / gridSize.second), sf::Color::Black),
			sf::Vertex(sf::Vector2f(xpos + (1 + xGridPos) * width / gridSize.first, ypos + (1 + yGridPos) * height / gridSize.second), sf::Color::Black)
		});
		break;
	case LEFT:
		mapLines[xGridPos][yGridPos].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + xGridPos * width / gridSize.first, ypos + yGridPos * height / gridSize.second), sf::Color::Black),
			sf::Vertex(sf::Vector2f(xpos + xGridPos * width / gridSize.first, ypos + (1 + yGridPos) * height / gridSize.second), sf::Color::Black)
		});
		break;
	default:
		std::cout << "Hit i basicDrawLine ska vi inte komma" << std::endl;
		break;
	}
}

void Map::basicDrawWall(DIRECTION dir)
{
	basicDrawLine(curGridPos.first, curGridPos.second, (DIRECTION)((dir + direction) % 4));
}

void Map::basicDrawDeadEnd(DIRECTION dir)
{
	DIRECTION finalDir{ (DIRECTION)((dir + direction) % 4) };
	switch (finalDir)
	{
	case UP:
		empty(curGridPos.first, curGridPos.second - 1);
		basicDrawLine(curGridPos.first, curGridPos.second - 1, (DIRECTION)((finalDir) % 4));
		basicDrawLine(curGridPos.first, curGridPos.second - 1, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first, curGridPos.second - 1, (DIRECTION)((finalDir + 3) % 4));
		break;
	case RIGHT:
		empty(curGridPos.first + 1, curGridPos.second);
		basicDrawLine(curGridPos.first + 1, curGridPos.second, (DIRECTION)((finalDir) % 4));
		basicDrawLine(curGridPos.first + 1, curGridPos.second, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first + 1, curGridPos.second, (DIRECTION)((finalDir + 3) % 4));
		break;
	case DOWN:
		empty(curGridPos.first, curGridPos.second + 1);
		basicDrawLine(curGridPos.first, curGridPos.second + 1, (DIRECTION)((finalDir) % 4));
		basicDrawLine(curGridPos.first, curGridPos.second + 1, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first, curGridPos.second + 1, (DIRECTION)((finalDir + 3) % 4));
		break;
	case LEFT:
		empty(curGridPos.first - 1, curGridPos.second);
		basicDrawLine(curGridPos.first - 1, curGridPos.second, (DIRECTION)((finalDir) % 4));
		basicDrawLine(curGridPos.first - 1, curGridPos.second, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first - 1, curGridPos.second, (DIRECTION)((finalDir + 3) % 4));
		break;
	}
}

void Map::basicDrawCorridor(DIRECTION dir)
{
	DIRECTION finalDir{ (DIRECTION)((dir + direction) % 4) };
	switch (finalDir)
	{
	case UP:
		empty(curGridPos.first, curGridPos.second - 1);
		basicDrawLine(curGridPos.first, curGridPos.second - 1, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first, curGridPos.second - 1, (DIRECTION)((finalDir + 3) % 4));
		break;
	case RIGHT:
		empty(curGridPos.first + 1, curGridPos.second);
		basicDrawLine(curGridPos.first + 1, curGridPos.second, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first + 1, curGridPos.second, (DIRECTION)((finalDir + 3) % 4));
		break;
	case DOWN:
		empty(curGridPos.first, curGridPos.second + 1);
		basicDrawLine(curGridPos.first, curGridPos.second + 1, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first, curGridPos.second + 1, (DIRECTION)((finalDir + 3) % 4));
		break;
	case LEFT:
		empty(curGridPos.first - 1, curGridPos.second);
		basicDrawLine(curGridPos.first - 1, curGridPos.second, (DIRECTION)((finalDir + 1) % 4));
		basicDrawLine(curGridPos.first - 1, curGridPos.second, (DIRECTION)((finalDir + 3) % 4));
		break;
	}
}

void Map::drawHighObst()
{
	empty(curGridPos.first, curGridPos.second);

	switch (direction)
	{
	case UP:
	case DOWN:
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Blue),
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + (1+curGridPos.second) * height / gridSize.second), sf::Color::Blue)
		});
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Blue),
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + (1+curGridPos.second) * height / gridSize.second), sf::Color::Blue)
		});
		break;
	case RIGHT:
	case LEFT:
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Blue),
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Blue)
		});
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + (1 + curGridPos.second) * height / gridSize.second), sf::Color::Blue),
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + (1 + curGridPos.second) * height / gridSize.second), sf::Color::Blue)
		});
		break;
	default:
		std::cout << "Hit i drawHighObst ska vi inte komma" << std::endl;
		break;
	}
}

void Map::drawLowObst()
{
	empty(curGridPos.first, curGridPos.second);

	switch (direction)
	{
	case UP:
	case DOWN:
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Red),
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + (1 + curGridPos.second) * height / gridSize.second), sf::Color::Red)
		});
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Red),
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + (1 + curGridPos.second) * height / gridSize.second), sf::Color::Red)
		});
		break;
	case RIGHT:
	case LEFT:
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Red),
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + curGridPos.second * height / gridSize.second), sf::Color::Red)
		});
		mapLines[curGridPos.first][curGridPos.second].push_back(new sf::Vertex[2]{
			sf::Vertex(sf::Vector2f(xpos + curGridPos.first * width / gridSize.first, ypos + (1 + curGridPos.second) * height / gridSize.second), sf::Color::Red),
			sf::Vertex(sf::Vector2f(xpos + (1 + curGridPos.first) * width / gridSize.first, ypos + (1 + curGridPos.second) * height / gridSize.second), sf::Color::Red)
		});
		break;
	default:
		std::cout << "Hit i drawLowObst ska vi inte komma" << std::endl;
		break;
	}
}

void Map::drawDeadEnd()
{
	empty(curGridPos.first, curGridPos.second);
	basicDrawWall(UP);
	basicDrawWall(RIGHT);
	basicDrawWall(LEFT);
}

void Map::drawDeadEndA(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	switch (dir) {
	case RIGHT:
		basicDrawWall(LEFT);
		basicDrawWall(UP);
		basicDrawDeadEnd(RIGHT);
		break;
	case LEFT:
		basicDrawWall(RIGHT);
		basicDrawWall(UP);
		basicDrawDeadEnd(LEFT);
		break;
	default:
		std::cout << "Hit ska vi inte komma i DeadEndA" << std::endl;
	}
}

void Map::drawDeadEndB(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	switch (dir) {
	case RIGHT:
		basicDrawWall(LEFT);
		basicDrawDeadEnd(UP);
		basicDrawDeadEnd(RIGHT);
		break;
	case LEFT:
		basicDrawWall(RIGHT);
		basicDrawDeadEnd(UP);
		basicDrawDeadEnd(LEFT);
		break;
	default:
		std::cout << "Hit ska vi inte komma i DeadEndB" << std::endl;
	}
}

void Map::drawDeadEndC()
{
	empty(curGridPos.first, curGridPos.second);
	basicDrawDeadEnd(RIGHT);
	basicDrawWall(UP);
	basicDrawDeadEnd(LEFT);
}

void Map::drawDeadEndD()
{
	empty(curGridPos.first, curGridPos.second);
	basicDrawDeadEnd(RIGHT);
	basicDrawDeadEnd(UP);
	basicDrawDeadEnd(LEFT);
}

void Map::drawCorridor()
{
	empty(curGridPos.first, curGridPos.second);
	basicDrawWall(RIGHT);
	basicDrawWall(LEFT);
}

void Map::drawTurn(DIRECTION dir)
{	
	empty(curGridPos.first, curGridPos.second);
	if (dir == LEFT) {
		basicDrawWall(UP);
		basicDrawWall(RIGHT);
	}
	else if (dir == RIGHT) {
		basicDrawWall(UP);
		basicDrawWall(LEFT);
	}
	else {
		std::cout << "Kan inte rita en sv?ng som inte sv?nger..." << std::endl;
	}
}

void Map::drawJunctionA(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	if (dir == LEFT) {
		basicDrawDeadEnd(UP);
		basicDrawCorridor(LEFT);
		basicDrawWall(RIGHT);
	}
	else if (dir == RIGHT) {
		basicDrawDeadEnd(UP);
		basicDrawCorridor(RIGHT);
		basicDrawWall(LEFT);
	}
	else {
		std::cout << "junc A fel" << std::endl;
	}
}

void Map::drawJunctionB(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	if (dir == LEFT) {
		basicDrawDeadEnd(LEFT);
		basicDrawCorridor(UP);
		basicDrawWall(RIGHT);
	}
	else if (dir == RIGHT) {
		basicDrawDeadEnd(RIGHT);
		basicDrawCorridor(UP);
		basicDrawWall(LEFT);
	}
	else {
		std::cout << "junc B fel" << std::endl;
	}
}

void Map::drawJunctionC(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	if (dir == LEFT) {
		basicDrawDeadEnd(RIGHT);
		basicDrawCorridor(LEFT);
		basicDrawWall(UP);
	}
	else if (dir == RIGHT) {
		basicDrawDeadEnd(LEFT);
		basicDrawCorridor(RIGHT);
		basicDrawWall(UP);
	}
	else {
		std::cout << "junc C fel" << std::endl;
	}
}

void Map::drawJunctionD(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	if (dir == LEFT) {
		basicDrawDeadEnd(RIGHT);
		basicDrawDeadEnd(UP);
		basicDrawCorridor(LEFT);
	}
	else if (dir == RIGHT) {
		basicDrawDeadEnd(LEFT);
		basicDrawDeadEnd(UP);
		basicDrawCorridor(RIGHT);
	}
	else if (dir == UP) {
		basicDrawDeadEnd(LEFT);
		basicDrawDeadEnd(RIGHT);
		basicDrawCorridor(UP);
	}
	else {
		std::cout << "junc D fel" << std::endl;
	}
}

void Map::drawJunctionE(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	if (dir == LEFT) {
		basicDrawCorridor(UP);
		basicDrawCorridor(LEFT);
		basicDrawWall(RIGHT);
	}
	else if (dir == RIGHT) {
		basicDrawCorridor(UP);
		basicDrawCorridor(RIGHT);
		basicDrawWall(LEFT);
	}
	else {
		std::cout << "junc E fel" << std::endl;
	}
}

void Map::drawJunctionF()
{
	empty(curGridPos.first, curGridPos.second);
	basicDrawCorridor(RIGHT);
	basicDrawCorridor(LEFT);
	basicDrawWall(UP);
}

void Map::drawJunctionG()
{
	empty(curGridPos.first, curGridPos.second);
	basicDrawDeadEnd(UP);
	basicDrawCorridor(RIGHT);
	basicDrawCorridor(LEFT);
}

void Map::drawJunctionH(DIRECTION dir)
{
	empty(curGridPos.first, curGridPos.second);
	if (dir == LEFT) {
		basicDrawCorridor(UP);
		basicDrawCorridor(LEFT);
		basicDrawDeadEnd(RIGHT);
	}
	else if (dir == RIGHT) {
		basicDrawCorridor(UP);
		basicDrawCorridor(RIGHT);
		basicDrawDeadEnd(LEFT);
	}
	else {
		std::cout << "junc H fel" << std::endl;
	}
}

void Map::drawJunctionI()
{
	empty(curGridPos.first, curGridPos.second);
	basicDrawCorridor(UP);
	basicDrawCorridor(RIGHT);
	basicDrawCorridor(LEFT);
}
