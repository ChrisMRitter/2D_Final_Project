#include "asteroid.h"
#include <SFML/Graphics.hpp>

// Constructor
Asteroid::Asteroid(const sf::Texture &texture, const sf::Vector2f &startPos,
                   const sf::Vector2f &velocity)
    : velocity(velocity) {
  // Sprite setup
  sprite.setTexture(texture);
  auto ts = texture.getSize();
  sprite.setOrigin(ts.x / 2.f, ts.y / 2.f);
  sprite.setPosition(startPos);

  // Hitbox setup
  hitbox.setSize({static_cast<float>(ts.x), static_cast<float>(ts.y)});
  hitbox.setOrigin(ts.x / 2.f, ts.y / 2.f);
  hitbox.setPosition(startPos);
  hitbox.setFillColor(sf::Color::Transparent);
}

// Moves the asteroid down
void Asteroid::update(float dt) {
  sprite.move(velocity * dt);
  hitbox.move(velocity * dt);
}

// Draws the asteroid
void Asteroid::draw(sf::RenderWindow &window) const { window.draw(sprite); }

// Collision check with player's bounds
bool Asteroid::checkCollision(const sf::FloatRect &playerBounds) const {
  return hitbox.getGlobalBounds().intersects(playerBounds);
}

// Returns the asteroid's bounds (for off-screen cleanup)
sf::FloatRect Asteroid::getBounds() const { return hitbox.getGlobalBounds(); }
