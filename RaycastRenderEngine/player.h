#pragma once
#include "SFML\Graphics.hpp"
#include "math.h"

class Player {
public:
	static int unsigned const FOV = 45;

	Player() {}
	Player(sf::Vector2f windowSize, float speed, float rotationSpeed);
	void Input(float dT);
	void Update(float dT);
	void Draw(sf::RenderWindow & window);

	sf::Vertex * getFOVLines() { return m_fovLines; }
	int getFOVLinesLength() { return FOV * 2; }

	float getAngle() { return m_angle; }
	sf::Vector2f getCenter() { return m_center; }
	sf::Vector2f getForward() { return m_forward; }
private:
	sf::Vertex m_fovLines[FOV * 2];

	sf::RectangleShape m_rect; 

	sf::Vector2f m_center;
	sf::Vector2f m_target;
	sf::Vector2f m_forward;
	sf::Vector2f m_right;
	sf::Vector2f m_velocity;

	float m_speed;
	float m_rotationSpeed;
	float m_angle;
};