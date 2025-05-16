/*
This is a custom hitbox class made to better display collision
*/
#include "hitbox.h"

/*
Constructor for the Hitbox class
- size: dimensions of the hitbox
- color: optional fill color for visualization (default is transparent)
*/
Hitbox::Hitbox(sf::Vector2f size, sf::Color color) {
  box.setSize(size); // Set the size of the hitbox
  box.setOrigin(size / 2.f); // Center the origin for easier alignment
  box.setFillColor(color); // Set the fill color (can be transparent or visible for debugging)
}

// Set the position of the hitbox in the game world
void Hitbox::setPosition(const sf::Vector2f &pos) { box.setPosition(pos); }

// Set a new size for the hitbox and re-center its origin
void Hitbox::setSize(const sf::Vector2f &size) {
  box.setSize(size);
  box.setOrigin(size / 2.f);
}

// Set the fill color of the hitbox (useful for debugging)
void Hitbox::setColor(const sf::Color &color) { box.setFillColor(color); }

//  Get the global bounds of the hitbox (used for collision detection)
sf::FloatRect Hitbox::getBounds() const { return box.getGlobalBounds(); }

// Draw the hitbox to the window (only visible if it has a non-transparent color)
void Hitbox::draw(sf::RenderWindow &window) { window.draw(box); }
