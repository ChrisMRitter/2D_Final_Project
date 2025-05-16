//hitbox.h
//The Hitbox class defines a custom rectangular hitbox used for collision detection
#ifndef HITBOX_H
#define HITBOX_H

// Include SFML graphics module for rendering shapes
#include <SFML/Graphics.hpp>

// Hitbox class declaration
class Hitbox {
public:
  // Constructor: Initializes the hitbox with a given size and color.
  // Default size is 40x40 pixels, and default color is semi-transparent green
  Hitbox(sf::Vector2f size = {40.f, 40.f},
         sf::Color color = sf::Color(0, 255, 0, 100));

  // Sets the position of the hitbox in the game world
  void setPosition(const sf::Vector2f &pos);

  // Sets the size of the hitbox
  void setSize(const sf::Vector2f &size);

  // Sets the fill color of the hitbox
  void setColor(const sf::Color &color);

  // Returns the bounding rectangle of the hitbox for collision detection
  sf::FloatRect getBounds() const;

  // Draws the hitbox to the specified render window
  void draw(sf::RenderWindow &window);

private:
  // The rectangle shape representing the hitbox
  sf::RectangleShape box;
};

#endif
