#ifndef ASTEROID_H
#define ASTEROID_H

#include <SFML/Graphics.hpp>

class Asteroid {
public:
    Asteroid(const sf::Texture& texture, const sf::Vector2f& startPos, const sf::Vector2f& velocity);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& otherBounds) const;
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape hitbox;
    sf::Sprite sprite;
    sf::Vector2f velocity;
};

#endif // ASTEROID_H
