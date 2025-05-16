// The Laser class represents a single laser projectile fired by player or enemies
#ifndef LASER_H
#define LASER_H
#include <SFML/Graphics.hpp>

class Laser {

public:
  /*
  Constructor:
  - x, y: starting position of the laser
  - speed: how fast the laser moves
  - direction: normalized direction vector the laser travels in
  - isEnemy: whether the laser was fired by an enemy (default is false)
  */
  Laser(float x, float y, float speed, sf::Vector2f direction, bool isEnemy = false);

  // Updates the laser's position based on its speed and direction
  void update(float dt);

  // Draws the laser to the given render window
  void draw(sf::RenderWindow &window);

  // Returns the current position of the laser
  sf::Vector2f getPosition();

  // Checks if the laser has traveled beyond its maximum allowed distance
  bool isOutOfBounds();

  // Returns the bounding box of the laser for collision detection
  sf::FloatRect getGlobalBounds();

  // Returns true if the laser was fired by an enemy
  bool isFromEnemy() const { return isEnemyLaser;}

private:
  float speed; // Speed of the laser
  sf::Vector2f direction; // Direction the laser is moving
  float distanceTraveled = 0.f; // Distance the laser has traveled so far
  float maxDistance = 1000.f; // Maximum distance the laser can travel
  sf::RectangleShape rectangle; // Visual representation of the laser
  bool isEnemyLaser = false; // Flag to indicate if the laser is from an enemy
};

#endif
