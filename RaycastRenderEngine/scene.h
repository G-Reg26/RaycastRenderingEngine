#pragma once
#include <iostream>
#include "SFML\Graphics.hpp"
#include "math.h"
#include "player.h"

class Scene {
public:
	Scene(sf::Vector2f windowSize, sf::Vertex * sceneLines, int sceneLinesLength, Player &player);
	void Input(float dT);
	void Update(float dT);
	void Draw(sf::RenderWindow &window);
private:
	bool m_enter;
	bool m_topDownView;

	Player m_player;

	sf::Vector2f m_windowSize;

	sf::Vertex * m_sceneLines;
	int m_sceneLinesLength;

	sf::RectangleShape m_sceneColumns[Player::FOV];
	float m_columnWidth;

	sf::RectangleShape m_sceneRows[Player::FOV];
	float m_rowHeight;

	sf::Vector2f lineIntersect(sf::Vertex lines[], sf::Vector2f player, sf::Vector2f fovLine, sf::Vector2f forward, int linesLength);
	bool bbInersectCheck(sf::Vertex p1, sf::Vertex p2, float x, float y);
};