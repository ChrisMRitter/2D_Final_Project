#include "asteroid.h"
#include <SFML/Graphics.hpp>

// Constructor for the Asteroid class
Asteroid::Asteroid(const sf::Texture &texture, const sf::Vector2f &startPos,
                   const sf::Vector2f &velocity)
    : velocity(velocity) { // Initialize velocity member
  // Set up the asteroid sprite with the given texture
  sprite.setTexture(texture);

  // Get the texture size to center the origin
  auto ts = texture.getSize();
  sprite.setOrigin(ts.x / 2.f, ts.y / 2.f); // Center origin for rotation and positioning
  sprite.setPosition(startPos); // Set initial position

  // Set up the hitbox to match the texture size
  hitbox.setSize({static_cast<float>(ts.x), static_cast<float>(ts.y)});
  hitbox.setOrigin(ts.x / 2.f, ts.y / 2.f); // Center origin for accurate collision
  hitbox.setPosition(startPos); // Match sprite position
  hitbox.setFillColor(sf::Color::Transparent); // Make hitbox invisible (used for logic only)
}

// Updates the asteroid's position based on its velocity and elapsed time
void Asteroid::update(float dt) {
  sprite.move(velocity * dt); // Move sprite
  hitbox.move(velocity * dt); // Move hitbox to stay in sync
}

// Draws the asteroid sprite to the window
void Asteroid::draw(sf::RenderWindow &window) const { window.draw(sprite); }

// Checks for collision between the asteroid and the player's bounding box
bool Asteroid::checkCollision(const sf::FloatRect &playerBounds) const {
  return hitbox.getGlobalBounds().intersects(playerBounds);
}

// Returns the asteroid's bounds of the asteroid's hitbox (for off-screen cleanup)
sf::FloatRect Asteroid::getBounds() const { return hitbox.getGlobalBounds(); }
