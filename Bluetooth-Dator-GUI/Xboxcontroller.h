#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <utility>
#include<SFML/Graphics.hpp>

class Xboxcontroller
{
public:
	Xboxcontroller() = delete;
	Xboxcontroller(float, float, float, float);
	virtual ~Xboxcontroller();

	void update();
	void draw(sf::RenderWindow&);

	bool leftLeverActive();
	bool rightLeverActive();
	float triggerValue();
	int dpadYAxis();

	float leftStickAngle();
	float leftStickIntensity();
	float rightStickAngle();
	float rightStickIntensity();

private:
	sf::Texture controller_texture;
	sf::Texture button_overlay_texture;
	sf::Texture lever_overlay_texture;
	sf::Texture angled_lever_texture;

	sf::Sprite controller;
	sf::Sprite A_button_overlay;
	sf::Sprite B_button_overlay;
	sf::Sprite X_button_overlay;
	sf::Sprite Y_button_overlay;
	sf::Sprite left_lever_overlay;
	sf::Sprite right_lever_overlay;
	sf::Sprite left_angled_lever;
	sf::Sprite right_angled_lever;

	sf::Vector2f left_lever_speed;
	sf::Vector2f right_lever_speed;

	bool left_lever_active;
	bool right_lever_active;
	bool A_pressed;
	bool B_pressed;
	bool X_pressed;
	bool Y_pressed;

};

#endif

