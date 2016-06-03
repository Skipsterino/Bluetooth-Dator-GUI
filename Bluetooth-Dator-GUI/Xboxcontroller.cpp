/**
* @file Xboxcontroller.cpp
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Wrapper class for the XBOX-controller
*
* Class to wrap the interface against the XBOX-controller
* to simplify use in code
*/

#include <Math.h>
#include <iostream>
#include "Xboxcontroller.h"

Xboxcontroller::Xboxcontroller(float xpos, float ypos, float width, float height) :
	controller_texture{},
	button_overlay_texture{},
	lever_overlay_texture{},
	angled_lever_texture{},

	controller{},
	A_button_overlay{},
	B_button_overlay{},
	X_button_overlay{},
	Y_button_overlay{},
	left_lever_overlay{},
	right_lever_overlay{},
	left_angled_lever{},
	right_angled_lever{},

	left_lever_speed{},
	right_lever_speed{},

	left_lever_active{ false },
	right_lever_active{ false },
	A_pressed{ false },
	B_pressed{ false },
	X_pressed{ false },
	Y_pressed{ false }
{
	//Laddar alla texturer
	controller_texture.loadFromFile("./Bilder/xboxcontroller.png");
	button_overlay_texture.loadFromFile("./Bilder/xboxspak.png", sf::IntRect(431, 44, 93, 93));
	lever_overlay_texture.loadFromFile("./Bilder/xboxspak.png", sf::IntRect(205, 33, 175, 175));
	angled_lever_texture.loadFromFile("./Bilder/xboxspak.png", sf::IntRect(0, 0, 186, 144));

	//S?tter alla sprites till sina texturer
	controller.setTexture(controller_texture);
	A_button_overlay.setTexture(button_overlay_texture);
	B_button_overlay.setTexture(button_overlay_texture);
	X_button_overlay.setTexture(button_overlay_texture);
	Y_button_overlay.setTexture(button_overlay_texture);
	left_lever_overlay.setTexture(lever_overlay_texture);
	right_lever_overlay.setTexture(lever_overlay_texture);
	left_angled_lever.setTexture(angled_lever_texture);
	right_angled_lever.setTexture(angled_lever_texture);

	//S?tter center f?r spakarna s? de roterar enkelt
	left_lever_overlay.setOrigin(87.5, 87.5);
	right_lever_overlay.setOrigin(87.5, 87.5);
	left_angled_lever.setOrigin(94, 126);
	right_angled_lever.setOrigin(94, 126);

	//Skalfaktorerna
	float orig_width{ controller.getGlobalBounds().width };
	float orig_height{ controller.getGlobalBounds().height };
	float x_scale{ width / orig_width };
	float y_scale{ height / orig_height };

	//Skalar om alla sprites enligt skalfaktorer
	controller.setScale(x_scale, y_scale);
	A_button_overlay.setScale(x_scale, y_scale);
	B_button_overlay.setScale(x_scale, y_scale);
	X_button_overlay.setScale(x_scale, y_scale);
	Y_button_overlay.setScale(x_scale, y_scale);
	left_lever_overlay.setScale(x_scale, y_scale);
	right_lever_overlay.setScale(x_scale, y_scale);
	left_angled_lever.setScale(x_scale, y_scale);
	right_angled_lever.setScale(x_scale, y_scale);

	//S?tter relativa koordinater p? alla sprites
	controller.setPosition(xpos, ypos);
	A_button_overlay.setPosition(xpos + 962 * x_scale, ypos + 255 * y_scale);
	B_button_overlay.setPosition(xpos + 1056 * x_scale, ypos + 162 * y_scale);
	X_button_overlay.setPosition(xpos + 868 * x_scale, ypos + 163 * y_scale);
	Y_button_overlay.setPosition(xpos + 962 * x_scale, ypos + 70 * y_scale);
	left_lever_overlay.setPosition(xpos + 279 * x_scale, ypos + 195 * y_scale);
	right_lever_overlay.setPosition(xpos + 828 * x_scale, ypos + 414 * y_scale);
	left_angled_lever.setPosition(xpos + 279 * x_scale, ypos + 195 * y_scale);
	right_angled_lever.setPosition(xpos + 828 * x_scale, ypos + 414 * y_scale);

}

bool Xboxcontroller::A_Pressed() {
	return A_pressed;
}

bool Xboxcontroller::B_Pressed() {
	return B_pressed;
}

bool Xboxcontroller::X_Pressed() {
	return X_pressed;
}

bool Xboxcontroller::Y_Pressed() {
	return Y_pressed;
}

bool Xboxcontroller::leftLeverActive() {
	return left_lever_active;
}
bool Xboxcontroller::rightLeverActive() {
	return right_lever_active;
}

float Xboxcontroller::triggerValue() {
	return -sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
}

int Xboxcontroller::dpadYAxis() {
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == 100) {
		return 2;
	}
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY) == -100) {
		return 1;
	}
	return 0;
}

int Xboxcontroller::dpadXAxis() {
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == 100) {
		return 2;
	}
	else if (sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX) == -100) {
		return 1;
	}
	return 0;
}

float Xboxcontroller::leftStickAngle() {

	if (left_lever_speed.x > 10 || left_lever_speed.x < -10) {
		return left_lever_speed.x;
	}
	return 0;
}

float Xboxcontroller::leftStickIntensity() {
	if (left_lever_active) {
		return std::sqrt(std::powf(left_lever_speed.x, 2) + std::powf(left_lever_speed.y, 2));
	}
	else {
		return 0;
	}
}

float Xboxcontroller::rightStickAngle() {
	float angle{ 180 / (float)3.141592 * std::atan2f(right_lever_speed.x, right_lever_speed.y) };

	if (right_lever_active) {
		if (angle < 0) {
			return angle + 180;
		}
		else {
			return angle - 180;
		}
	}
	else {
		return 0;
	}
}

float Xboxcontroller::rightStickIntensity() {
	if (right_lever_active) {
		return std::sqrt(std::powf(right_lever_speed.x, 2) + std::powf(right_lever_speed.y, 2));
	}
	else {
		return 0;
	}
}

sf::Vector2f Xboxcontroller::getRightAxisPosition()
{
	return right_lever_speed;
}

Xboxcontroller::~Xboxcontroller()
{

}

void Xboxcontroller::update() {
	left_lever_speed = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::X), sf::Joystick::getAxisPosition(0, sf::Joystick::Y));
	right_lever_speed = sf::Vector2f(sf::Joystick::getAxisPosition(0, sf::Joystick::U), sf::Joystick::getAxisPosition(0, sf::Joystick::R));

	if (left_lever_speed.x > 20.f || left_lever_speed.x < -20.f || left_lever_speed.y > 20.f || left_lever_speed.y < -20.f) {
		left_lever_active = true;
		left_angled_lever.setRotation(90 + 180 / 3.1415 * std::atan2f(left_lever_speed.y, left_lever_speed.x));
	}
	else{
		left_lever_active = false;
	}

	if (right_lever_speed.x > 20.f || right_lever_speed.x < -20.f || right_lever_speed.y > 20.f || right_lever_speed.y < -20.f) {
		right_lever_active = true;
		right_angled_lever.setRotation(90 + 180 / 3.1415 * std::atan2f(right_lever_speed.y, right_lever_speed.x));
	}
	else {
		right_lever_active = false;
	}

	if (sf::Joystick::isButtonPressed(0, 0)) {
		A_pressed = true;
	}
	else {
		A_pressed = false;
	}

	if (sf::Joystick::isButtonPressed(0, 1)) {
		B_pressed = true;
	}
	else {
		B_pressed = false;
	}

	if (sf::Joystick::isButtonPressed(0, 2)) {
		X_pressed = true;
	}
	else {
		X_pressed = false;
	}

	if (sf::Joystick::isButtonPressed(0, 3)) {
		Y_pressed = true;
	}
	else {
		Y_pressed = false;
	}
}

void Xboxcontroller::draw(sf::RenderWindow& window) {

	window.draw(controller);

	if (left_lever_active) {
		window.draw(left_lever_overlay);
		window.draw(left_angled_lever);
	}
	if (right_lever_active) {
		window.draw(right_lever_overlay);
		window.draw(right_angled_lever);
	}
	if (A_pressed) {
		window.draw(A_button_overlay);
	}
	if (B_pressed) {
		window.draw(B_button_overlay);
	}
	if (X_pressed) {
		window.draw(X_button_overlay);
	}
	if (Y_pressed) {
		window.draw(Y_button_overlay);
	}
}
