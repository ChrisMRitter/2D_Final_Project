#ifndef ASTEROID_H
#define ASTEROID_H

#include <SFML/Graphics.hpp>

// The Asteroid class represents a falling object in the game that can collide with the player
class Asteroid {
public:
  // Constructor: Initializes the asteroid with a texture, starting position, and velocity
  Asteroid(const sf::Texture &texture, const sf::Vector2f &startPos,
           const sf::Vector2f &velocity);

  // Updates the asteroid's position based on its velocity and elapsed time
  void update(float deltaTime);

  // Draws the asteroid sprite to the provided render window
  void draw(sf::RenderWindow &window) const;

  // Checks for collision between the asteroid and another object's bounding box
  bool checkCollision(const sf::FloatRect &otherBounds) const;

  // Returns the global bounds of the asteroid's hitbox (used for collision or off-screen checks)
  sf::FloatRect getBounds() const;

private:
  sf::RectangleShape hitbox; // Invisible rectangle used for collision detection
  sf::Sprite sprite; // Visual representation of the asteroid
  sf::Vector2f velocity; // Movement speed and direction of the asteroid
};

#endif // ASTEROID_H
