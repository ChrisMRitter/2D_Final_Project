#include "player.h"
#include "gameconstants.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

Player::Player(std::string texturePath, float speed) {
  if (!texture.loadFromFile(texturePath)) {
    std::cerr << "Error loading texture! " << texturePath << std::endl;
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setColor(sf::Color::Blue);
  } else {
    std::cout << "Loaded texture: " << texture.getSize().x << "×"
              << texture.getSize().y << std::endl;
    sprite.setTexture(texture);
  }

  sprite.setPosition(VIEW_WIDTH / 2.f, VIEW_HEIGHT - 100.f);
  sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
  sprite.setScale(0.5f, 0.5f);

  hitbox = new Hitbox(sf::Vector2f(64.f, 32.f));
  this->speed = speed;

  if (laserBuffer.loadFromFile("./Assets/Sound/lasersound.mp3")) {
    laserSound.setBuffer(laserBuffer);
  }
}

Player::~Player() { delete hitbox; }

const Hitbox &Player::getHitbox() const { return *hitbox; }

void Player::setPosition(const sf::Vector2f &pos) {
  sprite.setPosition(pos);
  hitbox->setPosition(pos);
}

sf::Vector2f Player::getPosition() { return sprite.getPosition(); }

sf::FloatRect Player::getGlobalBounds() { return sprite.getGlobalBounds(); }

void Player::draw(sf::RenderWindow &window) { window.draw(sprite); }

void Player::update(float dt, std::vector<Laser> &lasers,
                    sf::Vector2f mousePos) {
  timeSinceLastHit += dt;
  timeSinceLastLaser += dt;

  handleInput(dt, lasers, mousePos);
  hitbox->setPosition(sprite.getPosition());
}

void Player::handleInput(float dt, std::vector<Laser> &lasers,
                         sf::Vector2f mousePos) {
  sf::Vector2f movement(0, 0);
  sf::Vector2f position = sprite.getPosition();
  float halfWidth = sprite.getGlobalBounds().width / 2.f;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
      position.x - halfWidth > 0) {
    movement.x -= speed;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
      position.x + halfWidth < VIEW_WIDTH) {
    movement.x += speed;
  }
  sprite.move(movement * dt);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
      timeSinceLastLaser > laserCooldown) {
    sf::Vector2f dir = mousePos - sprite.getPosition();
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0)
      dir /= len;
    createLaser(lasers, sprite.getPosition(), dir);
    laserSound.play();
    timeSinceLastLaser = 0.f;
  }
}

void Player::createLaser(std::vector<Laser> &lasers, sf::Vector2f playerPos,
                         sf::Vector2f fireDirection) {
  lasers.emplace_back(playerPos.x, playerPos.y, 1000.f, fireDirection);
}

int Player::getHealth() const { return health; }

void Player::takeDamage(int amount) {
  if (canTakeDamage()) {
    health -= amount;
    timeSinceLastHit = 0.f;
  }
}

bool Player::canTakeDamage() const {
  return timeSinceLastHit >= damageCooldown;
}

void Player::handleAnimation(int /*direction*/, float /*dt*/) {
  // (no animation changes for now)
}

// Implementation of the decreaseLaserCooldown method
void Player::decreaseLaserCooldown(int killCount) {
  // Calculate total reduction based on kill count
  float reduction = cooldownReduction * killCount;
  
  // Apply the reduction
  laserCooldown -= reduction;
  
  // Ensure cooldown doesn't go below the minimum value
  if (laserCooldown < minLaserCooldown) {
    laserCooldown = minLaserCooldown;
  }
}

// Implementation of the getLaserCooldown method
float Player::getLaserCooldown() const {
  return laserCooldown;
}

// Implementation of the resetLaserCooldown method
void Player::resetLaserCooldown() {
  laserCooldown = 0.5f; // Reset to default value
}