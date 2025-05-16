// Include custom and SFML headers
#include "player.h"
#include "gameconstants.h"
#include "hitbox.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

// Constructor: initializes the player with a texture and movement speed
Player::Player(std::string texturePath, float speed) {
  // Attempt to load the texture from file
  if (!texture.loadFromFile(texturePath)) {
    std::cerr << "Error loading texture! " << texturePath << std::endl;
    // If loading fails, use a default blue rectangle
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    sprite.setColor(sf::Color::Blue);
  } else {
    std::cout << "Loaded texture: " << texture.getSize().x << "×"
              << texture.getSize().y << std::endl;
    sprite.setTexture(texture);
  }

  // Set initial position and appearance
  sprite.setPosition(VIEW_WIDTH / 2.f, VIEW_HEIGHT - 100.f);
  sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
  sprite.setScale(0.5f, 0.5f);

  // Create a hitbox for collision detection
  hitbox = new Hitbox(sf::Vector2f(44.f, 22.f)); // size of hitbox
  this->speed = speed;

  // Load laser sound effect
  if (laserBuffer.loadFromFile("./Assets/Sound/lasersound.mp3")) {
    laserSound.setBuffer(laserBuffer);
  }
}

// Destructor: clean up dynamically allocated hitbox
Player::~Player() { delete hitbox; }

// Return a reference to the player's hitbox
const Hitbox &Player::getHitbox() const { return *hitbox; } // AI-assisted during debugging

// Set the player's position and update the hitbox position
void Player::setPosition(const sf::Vector2f &pos) {
  sprite.setPosition(pos);
  hitbox->setPosition(pos);
}

// Get the player's current position
sf::Vector2f Player::getPosition() { return sprite.getPosition(); }

// Get the global bounds of the player's sprite (used for collision)
sf::FloatRect Player::getGlobalBounds() { return sprite.getGlobalBounds(); }

// Draw the player sprite to the window
void Player::draw(sf::RenderWindow &window) { window.draw(sprite); }

// Update player state each frame
void Player::update(float dt, std::vector<Laser> &lasers,
                    sf::Vector2f mousePos) {
  timeSinceLastHit += dt;
  timeSinceLastLaser += dt;

  handleInput(dt, lasers, mousePos); // Handle movement and shooting
  hitbox->setPosition(sprite.getPosition()); // Sync hitbox with sprite
}

// Handle keyboard input for movement and shooting
void Player::handleInput(float dt, std::vector<Laser> &lasers,
                         sf::Vector2f mousePos) {
  sf::Vector2f movement(0, 0);
  sf::Vector2f position = sprite.getPosition();
  float halfWidth = sprite.getGlobalBounds().width / 2.f;

  // Move left if 'A' is pressed and within bounds
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
      position.x - halfWidth > 0) {
    movement.x -= speed;
  }

  // Move right if 'D' is pressed and within bounds
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
      position.x + halfWidth < VIEW_WIDTH) {
    movement.x += speed;
  }

  // Apply movement
  sprite.move(movement * dt);

  // Fire laser if space is pressed and cooldown has passed
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) &&
      timeSinceLastLaser > laserCooldown) {
    sf::Vector2f dir = mousePos - sprite.getPosition();
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0)
      dir /= len; // Normalize direction
    createLaser(lasers, sprite.getPosition(), dir);
    laserSound.play();
    timeSinceLastLaser = 0.f;
  }
}

// Create a new laser and add it to the list
void Player::createLaser(std::vector<Laser> &lasers, sf::Vector2f playerPos,
                         sf::Vector2f fireDirection) {
  lasers.emplace_back(playerPos.x, playerPos.y, 1000.f, fireDirection, false);
}

// Get the player's current health
int Player::getHealth() const { return health; }

// Set the player's health to a new value
void Player::setHealth(int newHealth) {
  health = newHealth;
}

// Reduce health if damage cooldown has passed
void Player::takeDamage(int amount) {
  if (canTakeDamage()) {
    health -= amount;
    timeSinceLastHit = 0.f;
  }
}

// Check if the player can currently take damage
bool Player::canTakeDamage() const {
  return timeSinceLastHit >= damageCooldown;
}

// Placeholder for animation logic (not implemented)
void Player::handleAnimation(int /*direction*/, float /*dt*/) {
  // (no animation changes for now)
}

// Reduce laser cooldown based on number of kills
void Player::decreaseLaserCooldown(int killCount) {
  // Calculate total reduction based on kill count
  float reduction = cooldownReduction * killCount;
  laserCooldown -= reduction;
  
  // Clamp to minimum cooldown
  if (laserCooldown < minLaserCooldown) {
    laserCooldown = minLaserCooldown;
  }
}

// Get the current laser cooldown value
float Player::getLaserCooldown() const {
  return laserCooldown;
}

// Reset laser cooldown to default value
void Player::resetLaserCooldown() {
  laserCooldown = 0.5f; // Reset to default value
}
