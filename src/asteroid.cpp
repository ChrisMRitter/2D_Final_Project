#include "Asteroid.h"

// Constructor
Asteroid::Asteroid(const sf::Texture& texture, const sf::Vector2f& startPos, const sf::Vector2f& velocity)
    : velocity(velocity)
{
    // Set up hitbox
    hitbox.setSize(sf::Vector2f(64.f, 64.f)); // Adjust size as needed
    hitbox.setFillColor(sf::Color(255, 0, 0, 100)); // Semi-transparent red for debugging
    hitbox.setPosition(startPos);

    // Set up sprite
    sprite.setTexture(texture);
    sprite.setPosition(startPos);
}

// Moves the asteroid based on its velocity and the time elapsed since the last frame.
void Asteroid::update(float deltaTime)
{
    sf::Vector2f movement = velocity * deltaTime;
    hitbox.move(movement);
    sprite.move(movement);
}

// Renders the asteroid's hitbox (optional) and sprite to the screen.
void Asteroid::draw(sf::RenderWindow& window)
{
    window.draw(hitbox);  // Optional: for debugging
    window.draw(sprite);
}

// Returns true if the asteroid's hitbox intersects with another object's bounds.
bool Asteroid::checkCollision(const sf::FloatRect& otherBounds) const
{
    return hitbox.getGlobalBounds().intersects(otherBounds);
}

// Returns the global bounds of the asteroid's hitbox.
sf::FloatRect Asteroid::getBounds() const
{
    return hitbox.getGlobalBounds();
}
