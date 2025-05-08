#include "enemy.h"
#include <cmath>

Enemy::Enemy(float x,
             float y,
             float speed,
             int health,
             std::shared_ptr<sf::Texture> texture,
             float scale,
             sf::IntRect texRect)
    : texture(texture)
    , speed(speed)
    , health(health)
    , maxHealth(health)
    , scale(scale)
{
    sprite.setTexture(*texture);
    if (texRect.width == 0 || texRect.height == 0) {
        auto ts = texture->getSize();
        texRect = sf::IntRect(0, 0, int(ts.x), int(ts.y));
    }
    sprite.setTextureRect(texRect);
    sprite.setOrigin(texRect.width  / 2.f,
                     texRect.height / 2.f);
    sprite.setPosition(x, y);
    sprite.setScale(scale, scale);
}

void Enemy::update(float dt, sf::Vector2f playerPos) {
    sf::Vector2f dir = playerPos - sprite.getPosition();
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len > 0.f) {
        dir /= len;
        sprite.move(dir * speed * dt);
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

int Enemy::getHealth() const { return health; }
void Enemy::setHealth(int h) { health = h; }
int Enemy::takeDamage() { health--; if (health <= 0) alive = false; return health; }
void Enemy::killEnemy() { alive = false; }
bool Enemy::isAlive() const { return alive; }
