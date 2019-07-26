#include "player.h"

Player::Player(sf::Vector2f windowSize, float speed, float rotationSpeed)
{
	m_rect.setSize(windowSize * 0.01f);

	m_center = m_rect.getSize() / 2.0f;

	m_rect.setPosition(sf::Vector2f((windowSize.x / 2.0f) - m_center.x, (windowSize.y / 2.0f) - m_center.y));

	m_speed = speed;
	m_rotationSpeed = rotationSpeed;
	m_angle = 0.0f;
}

void Player::Input(float dT)
{
	// rotate
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		m_angle += m_rotationSpeed * dT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		m_angle -= m_rotationSpeed * dT;
	}

	m_velocity = sf::Vector2f(0.0f, 0.0f);

	// move
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_velocity += m_forward * m_speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_velocity -= m_forward * m_speed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_velocity += m_right * m_speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_velocity -= m_right * m_speed;
	}
}

void Player::Update(float dT)
{
	// update position
	m_rect.setPosition(m_rect.getPosition() + (m_velocity * dT));

	// update vectors
	m_center = m_rect.getPosition() + (m_rect.getSize() / 2.0f);
	m_target = m_center + sf::Vector2f(sinf(deg2Rad(m_angle)), cosf(deg2Rad(m_angle)));
	m_forward = m_target - m_center;
	m_right = sf::Vector2f(m_forward.y, -m_forward.x);
}

void Player::Draw(sf::RenderWindow & window)
{
	window.draw(m_rect);
	window.draw(m_fovLines, FOV * 2, sf::Lines);
}
