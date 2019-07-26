#include "scene.h"

Scene::Scene(sf::Vector2f windowSize, sf::Vertex * sceneLines, int sceneLinesLength, Player &player)
{
	m_enter = false;
	m_topDownView = true;

	m_player = player;

	m_windowSize = windowSize;

	m_columnWidth = m_windowSize.x / float(Player::FOV);
	m_rowHeight = m_windowSize.y / float(Player::FOV);

	m_sceneLinesLength = sceneLinesLength;
	
	// set top/down scene lines
	for (int i = 0; i < m_sceneLinesLength; i++) {
		sceneLines[i].position = sf::Vector2f(sceneLines[i].position.x * windowSize.x, sceneLines[i].position.y * windowSize.y);
	}

	m_sceneLines = sceneLines;

	// set "3D" scene rows
	float y = 0.0f;

	for (int i = 0; i < Player::FOV; i++) {
		m_sceneRows[i].setSize(sf::Vector2f(m_windowSize.x, m_rowHeight));
		m_sceneRows[i].setPosition(sf::Vector2f(0.0f, y));

		float colorScale = 1.0f - powf(sinf(((y + (m_rowHeight / 2.0f)) / m_windowSize.y) * PI), 0.25f);

		m_sceneRows[i].setFillColor(sf::Color(175 * colorScale, 50 * colorScale, 0));

		y += m_rowHeight;
	}
}

void Scene::Input(float dT)
{
	m_player.Input(dT);

	// switch between views
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		if (!m_enter) {
			m_enter = true;
			m_topDownView = !m_topDownView;
		}
	}
	else {
		m_enter = false;
	}
}

void Scene::Update(float dT)
{
	m_player.Update(dT);

	float angle = m_player.getAngle() - ((float)Player::FOV / 2.0f);

	// set FOV lines
	for (int i = 0; i < Player::FOV * 2; i += 2) {
		sf::Vector2f initLine = m_player.getCenter() + sf::Vector2f(m_windowSize.x * sinf(deg2Rad(angle)), m_windowSize.y * cosf(deg2Rad(angle)));

		sf::Vertex line = lineIntersect(m_sceneLines, m_player.getCenter(), initLine, m_player.getForward(), m_sceneLinesLength);

		m_player.getFOVLines()[i] = m_player.getCenter();
		m_player.getFOVLines()[i + 1] = line;

		angle++;
	}

	if (!m_topDownView) {
		
	}
}

void Scene::Draw(sf::RenderWindow & window)
{
	// draw top/down view
	if (m_topDownView) {
		m_player.Draw(window);
		window.draw(m_sceneLines, m_sceneLinesLength, sf::Lines);
	}
	// draw "3D" scene
	else {
		// draw rows
		for (int i = 0; i < Player::FOV; i++) {
			window.draw(m_sceneRows[i]);
		}

		// draw columns
		float x = m_windowSize.x - m_columnWidth;

		for (int i = 0; i < Player::FOV * 2; i += 2) {
			// calculate projected distance
			float distanceScale = magnitude(m_windowSize / 75.0f);
			sf::Vector2f rawDistVector = (m_player.getFOVLines()[i + 1].position - m_player.getCenter());
			float dist = (magnitude(rawDistVector) / distanceScale) * cosTheta(m_player.getForward(), rawDistVector);

			if (dist > magnitude(m_player.getForward()) && dist < magnitude(m_player.getForward() * magnitude(m_windowSize))) {
				// calculate y components for size and position of current column
				float columnHeight = m_windowSize.y / dist;
				float y = (m_windowSize.y / 2.0f) - (columnHeight / 2.0f);

				// set scene column
				m_sceneColumns[i / 2].setSize(sf::Vector2f(m_columnWidth, columnHeight));
				m_sceneColumns[i / 2].setPosition(sf::Vector2f(x, y));
				m_sceneColumns[i / 2].setFillColor(sf::Color(255 / dist, 255 / dist, 255 / dist));

				window.draw(m_sceneColumns[i / 2]);
			}

			// decrement x
			x -= m_columnWidth;
		}
	}
}

// checks if an FOV line intersects with a scene line and returns it's intersection point
sf::Vector2f Scene::lineIntersect(sf::Vertex lines[], sf::Vector2f player, sf::Vector2f fovLine, sf::Vector2f forward, int linesLength) {
	sf::Vector2f result = sf::Vector2f(fovLine.x, fovLine.y);

	// Line AB represented as a1x + b1y = c1 
	float a1 = fovLine.y - player.y;
	float b1 = player.x - fovLine.x;
	float c1 = a1*(player.x) + b1*(player.y);

	for (int i = 0; i < linesLength; i += 2) {
		// Line CD represented as a2x + b2y = c2 
		float a2 = lines[i + 1].position.y - lines[i].position.y;
		float b2 = lines[i].position.x - lines[i + 1].position.x;
		float c2 = a2*(lines[i].position.x) + b2*(lines[i].position.y);

		float determinant = a1*b2 - a2*b1;

		// lines intersect
		if (determinant != 0)
		{
			sf::Vector2f temp = sf::Vector2f((b2*c1 - b1*c2) / determinant, (a1*c2 - a2*c1) / determinant);

			// intersection point is closer than the previous
			if (distance(player.x, temp.x, player.y, temp.y) < distance(player.x, result.x, player.y, result.y)) {
				float dotP = dotProduct(temp.x - player.x, forward.x, temp.y - player.y, forward.y);

				// intersection occurs within player's FOV and occurs on line segment
				if (dotP > 0.0f && bbInersectCheck(lines[i], lines[i + 1], temp.x, temp.y)) {
					result = temp;
				}
			}
		}
	}

	return result;
}

// check if point intersects with bounding box
bool Scene::bbInersectCheck(sf::Vertex p1, sf::Vertex p2, float x, float y) {
	float minX = p1.position.x < p2.position.x ? p1.position.x : p2.position.x;
	float maxX = p1.position.x > p2.position.x ? p1.position.x : p2.position.x;
	float minY = p1.position.y < p2.position.y ? p1.position.y : p2.position.y;
	float maxY = p1.position.y > p2.position.y ? p1.position.y : p2.position.y;

	// bounding box width = 0
	if (maxX == minX) {
		// point between box height
		if (y > minY && y < maxY) {
			return true;
		}
		else {
			return false;
		}
	}
	// bounding box height = 0
	else if (maxY == minY) {
		// point between box width
		if (x > minX && x < maxX) {
			return true;
		}
		else {
			return false;
		}
	}
	// point intersects bounding box
	else if (x > minX && x < maxX && y > minY && y < maxY) {
		return true;
	}
	else {
		return false;
	}
}