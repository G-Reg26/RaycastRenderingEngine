#include <iostream>
#include "SFML\Graphics.hpp"
#include "math.h"
#include "player.h"
#include "scene.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Raycast Render Engine");
	sf::Clock dC;

	sf::Vector2f windowSize = sf::Vector2f(window.getSize().x, window.getSize().y);

	Player player = Player(windowSize, 40.0f, 40.0f);

	// values are normalized between 0 and 1, then scaled appropriately through the window size
	sf::Vertex sceneLines[] = {
		// Big Square
		sf::Vertex(sf::Vector2f(0.25f, 0.25f)),
		sf::Vertex(sf::Vector2f(0.75f, 0.25f)),
		sf::Vertex(sf::Vector2f(0.75f, 0.25f)),
		sf::Vertex(sf::Vector2f(0.75f, 0.75f)),
		sf::Vertex(sf::Vector2f(0.75f, 0.75f)),
		sf::Vertex(sf::Vector2f(0.25f, 0.75f)),
		sf::Vertex(sf::Vector2f(0.25f, 0.75f)),
		sf::Vertex(sf::Vector2f(0.25f, 0.25f)),

		// Small Square
		sf::Vertex(sf::Vector2f(0.35f, 0.35f)),
		sf::Vertex(sf::Vector2f(0.45f, 0.35f)),
		sf::Vertex(sf::Vector2f(0.45f, 0.35f)),
		sf::Vertex(sf::Vector2f(0.45f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.45f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.35f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.35f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.35f, 0.35f)),

		// Small Triangle
		sf::Vertex(sf::Vector2f(0.60f, 0.35f)),
		sf::Vertex(sf::Vector2f(0.70f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.70f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.50f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.50f, 0.45f)),
		sf::Vertex(sf::Vector2f(0.60f, 0.35f)),
	};

	Scene scene = Scene(windowSize, sceneLines, sizeof(sceneLines) / sizeof(sceneLines[0]), player);
	
	while (window.isOpen())
	{
		sf::Time dT = dC.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		scene.Input(dT.asSeconds());
		scene.Update(dT.asSeconds());

		window.clear();
		scene.Draw(window);
		window.display();
	}

	return 0;
}