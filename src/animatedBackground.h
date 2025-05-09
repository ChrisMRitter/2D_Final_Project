#ifndef ANIMATED_BACKGROUND_H
#define ANIMATED_BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class AnimatedBackground {
public:
  // sheet: full spritesheet (single row), fps: frames per second
  AnimatedBackground(std::shared_ptr<sf::Texture> sheet, float fps);

  // Set overall scale factor for the background
  void setScaleFactor(float scale);

  // Advance the animation by dt seconds
  void update(float dt);

  // Draw scaled to window
  void draw(sf::RenderWindow &window);

private:
  sf::Sprite sprite;
  std::vector<sf::IntRect> frames;
  float frameTime;
  float elapsed = 0.f;
  size_t currentFrame = 0;
  float scaleFactor = 1.f;
};

#endif
