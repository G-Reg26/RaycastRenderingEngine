#pragma once
#include <iostream>
#include "SFML\Graphics.hpp"

#define PI 3.14159f

// convert degrees to radians
static float deg2Rad(float degrees) {
	return (degrees * PI) / 180.0f;
}

// get distance between two points
static float distance(float x1, float x2, float y1, float y2) {
	return sqrtf(powf(x2 - x1, 2) + powf(y2 - y1, 2));
}

static float distance(sf::Vector2f a, sf::Vector2f b) {
	return distance(a.x, b.x, a.y, b.y);
}

// get magnitude of vector
static float magnitude(float x, float y) {
	return sqrtf(pow(x, 2) + pow(y, 2));
}

static float magnitude(sf::Vector2f a) {
	return magnitude(a.x, a.y);
}

// get dot product between to vectors
static float dotProduct(float x1, float x2, float y1, float y2) {
	return (x1 * x2) + (y1 * y2);
}

static float dotProduct(sf::Vector2f a, sf::Vector2f b) {
	return dotProduct(a.x, b.x, a.y, b.y);
}

// get cos(theta) of two vectors
static float cosTheta(float x1, float x2, float y1, float y2) {
	float A = magnitude(x1, y1);
	float B = magnitude(x2, y2);

	float dotAB = dotProduct(x1, x2, y1, y2);

	return dotAB / (A * B);
}

static float cosTheta(sf::Vector2f a, sf::Vector2f b) {
	return cosTheta(a.x, b.x, a.y, b.y);
}