/**
* @file Xboxcontroller.h
* @version 1.0
* @author Kevin Kjell?n
* @date 2 june 2016
* @brief Wrapper class for the XBOX-controller
*
* Class to wrap the interface against the XBOX-controller
* to simplify use in code
*/

#ifndef XBOXCONTROLLER_H
#define XBOXCONTROLLER_H

#include <utility>
#include<SFML/Graphics.hpp>

class Xboxcontroller
{
public:
	Xboxcontroller() = delete;

	/**
	* @brief Class constructor
	*
	* @param xpos The X-position of the graphics of the XBOX-controller
	* @param ypos The Y-position of the graphics of the XBOX-controller
	* @param width The width of the graphics of the XBOX-controller
	* @param height The height of the graphics of the XBOX-controller
	*/
	Xboxcontroller(float, float, float, float);
	virtual ~Xboxcontroller();

	/**
	* @brief Updates all components of the XBOX-controller
	*/
	void update();

	/**
	* @brief Draws the entire XBOX-controller
	*
	* When called the function draws all components of the controller
	* on the given RenderWindow
	* @param window The RenderWindow upon which to draw the controller
	*/
	void draw(sf::RenderWindow&);

	/**
	* @brief Left lever active function
	*
	* @return Returns true if the left lever is active, else false
	*/
	bool leftLeverActive();

	/**
	* @brief Right lever active function
	*
	* @return Returns true if the right lever is active, else false
	*/
	bool rightLeverActive();

	/**
	* @brief Returns the XBOX-controller trigger value
	*
	* Returns the value given by the connected XBOX-controller
	* trigger value, rangeing from -100 to 100
	*/
	float triggerValue();

	/**
	* @brief Gives the XBOX-controller dpad Y axis value
	*
	* Returns the XBOX-controller dpad Y axis value, rangeing
	* from -100 to 100
	*/
	int dpadYAxis();

	/**
	* @brief Gives the XBOX-controller dpad X axis value
	*
	* Returns the XBOX-controller dpad X axis value, rangeing
	* from -100 to 100
	*/
	int dpadXAxis();

	/**
	* @brief "A" button status function
	*
	* @return Returns true if the "A" is active, else false
	*/
	bool A_Pressed();

	/**
	* @brief "B" button status function
	*
	* @return Returns true if the "B" is active, else false
	*/
	bool B_Pressed();

	/**
	* @brief "X" button status function
	*
	* @return Returns true if the "X" is active, else false
	*/
	bool X_Pressed();

	/**
	* @brief "Y" button status function
	*
	* @return Returns true if the "Y" is active, else false
	*/
	bool Y_Pressed();

	/**
	* @brief Returns the left XBOX-controller stick angle
	*
	* Returns the angle of the left analog stick on the XBOX-controller,
	* rangeing from -180 to 180 degrees 
	*/
	float leftStickAngle();

	/**
	* @brief Returns the left XBOX-controller stick intensity
	*
	* Returns the intensity of the left analog stick on the XBOX-controller,
	* rangeing from 0 to 100
	*/
	float leftStickIntensity();

	/**
	* @brief Returns the right XBOX-controller stick angle
	*
	* Returns the angle of the right analog stick on the XBOX-controller,
	* rangeing from -180 to 180 degrees
	*/
	float rightStickAngle();

	/**
	* @brief Returns the right XBOX-controller stick intensity
	*
	* Returns the angle of the right analog stick on the XBOX-controller,
	* rangeing from 0 to 100
	*/
	float rightStickIntensity();

	/**
	* @brief Returns the right XBOX-controller stick position
	*
	* Returns a Vector2f with the x and y position of the right analog
	* XBOX-controller stick, both rangeing from -100 to 100,
	*/
	sf::Vector2f getRightAxisPosition();

private:
	sf::Texture controller_texture; /**< Holds the texture of the XBOX-controller */
	sf::Texture button_overlay_texture; /**< Holds the texture of the transparent button overlay */
	sf::Texture lever_overlay_texture; /**< Holds the texture of the white lever overlay */
	sf::Texture angled_lever_texture; /**< Holds the texture of the angled lever overlay */

	sf::Sprite controller; /**< Sprite containing pointer to controller texture and coordinates */
	sf::Sprite A_button_overlay; /**< Sprite containing pointer to button overlay texture and coordinates */
	sf::Sprite B_button_overlay; /**< Sprite containing pointer to button overlay texture and coordinates */
	sf::Sprite X_button_overlay; /**< Sprite containing pointer to button overlay texture and coordinates */
	sf::Sprite Y_button_overlay; /**< Sprite containing pointer to button overlay texture and coordinates */
	sf::Sprite left_lever_overlay; /**< Sprite containing pointer to white lever overlay texture and coordinates */
	sf::Sprite right_lever_overlay; /**< Sprite containing pointer to white lever overlay texture and coordinates */
	sf::Sprite left_angled_lever; /**< Sprite containing pointer to lever overlay texture and coordinates */
	sf::Sprite right_angled_lever; /**< Sprite containing pointer to lever overlay texture and coordinates */

	sf::Vector2f left_lever_speed; /**< Vector holding the speed of the left analog stick */
	sf::Vector2f right_lever_speed; /**< Vector holding the speed of the right analog stick */

	bool left_lever_active; /**< Bool saying if the left lever is active or not */
	bool right_lever_active; /**< Bool saying if the right lever is active or not */
	bool A_pressed; /**< Bool saying if the "A" button is currently pressed */
	bool B_pressed; /**< Bool saying if the "B" button is currently pressed */
	bool X_pressed; /**< Bool saying if the "X" button is currently pressed */
	bool Y_pressed; /**< Bool saying if the "Y" button is currently pressed */

};

#endif

