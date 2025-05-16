// The laser projectile implementation
#include "laser.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;

// Constructor: initializes a laser with position, speed, direction, and type (enemy or player)
Laser::Laser(float x, float y, float speed, sf::Vector2f direction, bool isEnemy) {
  // Create a rectangular shape to represent the laser
  rectangle = RectangleShape(sf::Vector2f(10.f, 5.f)); // Width x Height

  // Set color based on whether it is an enemy or player laser
  if (isEnemy) {
    rectangle.setFillColor(sf::Color::Yellow); // Enemy laser is yellow
    isEnemyLaser = true;
  } else {
    rectangle.setFillColor(sf::Color::Red); // Player laser is red
  }

  // Center the origin for proper rotation and movement
  rectangle.setOrigin(rectangle.getSize().x / 2, rectangle.getSize().y / 2);

  // Set the initial position of the laser
  rectangle.setPosition(x, y);

  // Store speed and normalized direction
  this->speed = speed;
  this->direction = direction;

  // Rotate the laser to face the direction it is moving
  float angle = atan2(direction.y, direction.x) * 180 / 3.14159f;
  rectangle.setRotation(angle);
}

// Updates the laser's position based on its speed and direction
void Laser::update(float dt) {
  rectangle.move(direction.x * speed * dt, direction.y * speed * dt);
  distanceTraveled += speed * dt; // Track how far the laser has traveled
}

// Checks if the laser has exceeded its maximum allowed travel distance
bool Laser::isOutOfBounds() {
  // Check if the laser has traveled more than the max distance
  return distanceTraveled >= maxDistance;
}

// Draws the laser to the provided render window
void Laser::draw(sf::RenderWindow &window) { window.draw(rectangle); }

//Returns the current position of the laser
sf::Vector2f Laser::getPosition() { return rectangle.getPosition(); }

// Returns the bounding box of the laser for collision detection
sf::FloatRect Laser::getGlobalBounds() { return rectangle.getGlobalBounds(); }
