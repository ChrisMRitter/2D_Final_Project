#include "asteroid.h"
#include <SFML/Graphics.hpp>

// Constructor for the Asteroid class
Asteroid::Asteroid(const sf::Texture &texture, const sf::Vector2f &startPos,
                   const sf::Vector2f &velocity)
    : velocity(velocity) { // Initializes velocity
  // Sets up the asteroid sprite
  sprite.setTexture(texture);

  // Gets the texture size to center it
  auto ts = texture.getSize();
  sprite.setOrigin(ts.x / 2.f, ts.y / 2.f);
  sprite.setPosition(startPos); // Sets the initial position

  // Sets up the hitbox to match the texture size
  hitbox.setSize({static_cast<float>(ts.x), static_cast<float>(ts.y)});
  hitbox.setOrigin(ts.x / 2.f, ts.y / 2.f);
  hitbox.setPosition(startPos);
  hitbox.setFillColor(sf::Color::Transparent); // Makes the hitbox invisible
}

// Updates the asteroid's position based on time/velocity
void Asteroid::update(float dt) {
  sprite.move(velocity * dt); // Moves the sprite
  hitbox.move(velocity * dt); // Move the hitbox to stay in sync with the sprite
}

// Draws the asteroid sprite to the window
void Asteroid::draw(sf::RenderWindow &window) const { window.draw(sprite); }

// Checks for collision between the asteroid and the player
bool Asteroid::checkCollision(const sf::FloatRect &playerBounds) const {
  return hitbox.getGlobalBounds().intersects(playerBounds);
}

// Returns the asteroid's bounds of the asteroid's hitbox (for off-screen cleanup)
sf::FloatRect Asteroid::getBounds() const { return hitbox.getGlobalBounds(); }
