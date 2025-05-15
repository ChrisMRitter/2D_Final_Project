#include "animatedBackground.h"
#include <iostream>

AnimatedBackground::AnimatedBackground(std::shared_ptr<sf::Texture> sheet,
                                       float fps)
    : frameTime(1.f / fps), texture(sheet) {
  sprite.setTexture(*sheet);
  auto sz = sheet->getSize();
  int h = sz.y;
  int cols = sz.x / h;
  frames.reserve(cols);
  for (int i = 0; i < cols; ++i)
    frames.emplace_back(i * h, 0, h, h);
  sprite.setTextureRect(frames[0]);
}

void AnimatedBackground::loadSheet(std::shared_ptr<sf::Texture> sheet,
                                   float fps) {
  // reset animation state
  frameTime = 1.f / fps;
  elapsed = 0.f;
  currentFrame = 0;
  frames.clear();
  texture.reset();

  texture = sheet; // store texture pointer

  // assign new texture and rebuild frames
  sprite.setTexture(*sheet, true);
  auto sz = sheet->getSize();

  // Frames are horizontally arranged. Each frame has same width as height of
  // spritesheet
  int frameHeight = sz.y;
  int frameWidth = frameHeight;

  int numFrames = sz.x / frameWidth;
  // debugging
  std::cout << "Loading spritesheet: " << sz.x << "x" << sz.y
            << ", frame size: " << frameWidth << "x" << frameHeight
            << ", frames: " << numFrames << std::endl;

  frames.reserve(numFrames);

  for (int i = 0; i < numFrames; ++i) {
    frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
  }

  if (!frames.empty()) {
    sprite.setTextureRect(frames[0]);
  } else {
    std::cerr << "Warning: No animation frames were created!"
              << std::endl; // debugging
  }
}
void AnimatedBackground::setScaleFactor(float scale) {
  scaleFactor = scale;
  sprite.setScale(scaleFactor, scaleFactor);
}

void AnimatedBackground::update(float dt) {
  elapsed += dt;
  if (elapsed >= frameTime) {
    elapsed -= frameTime;
    currentFrame = (currentFrame + 1) % frames.size();
    sprite.setTextureRect(frames[currentFrame]);
  }
}

void AnimatedBackground::draw(sf::RenderWindow &window) {
  // Get the current view
  sf::Vector2f viewSize = window.getView().getSize();

  // Position the sprite to cover the entire view
  sprite.setPosition(0, 0);

  // Calculate the scale needed to cover the entire view
  auto spriteSize = sprite.getLocalBounds();
  if (spriteSize.width > 0 && spriteSize.height > 0) {
    float scaleX = viewSize.x / spriteSize.width;
    float scaleY = viewSize.y / spriteSize.height;

    // Use the larger scale to ensure full coverage but multiply by scaleFactor
    // for adjustment
    float baseScale = std::max(scaleX, scaleY);
    sprite.setScale(baseScale * scaleFactor, baseScale * scaleFactor);

    // Debug output
    // std::cout << "View size: " << viewSize.x << "x" << viewSize.y
    //<< ", sprite frame: " << currentFrame << "/" << frames.size()
    //<< ", scale: " << (baseScale * scaleFactor) << std::endl;
  }
  // Draw the sprite
  window.draw(sprite);
}

// debugging:
void AnimatedBackground::debugInfo() const {
  std::cout << "=== AnimatedBackground Debug Info ===" << std::endl;
  std::cout << "Texture pointer: " << (texture ? "Valid" : "NULL") << std::endl;

  if (texture) {
    std::cout << "Texture size: " << texture->getSize().x << "x"
              << texture->getSize().y << std::endl;
  }

  std::cout << "Frame count: " << frames.size() << std::endl;
  std::cout << "Current frame: " << currentFrame << std::endl;

  if (!frames.empty() && currentFrame < frames.size()) {
    sf::IntRect rect = frames[currentFrame];
    std::cout << "Current frame rect: (" << rect.left << "," << rect.top << ","
              << rect.width << "," << rect.height << ")" << std::endl;
  }

  sf::Vector2f pos = sprite.getPosition();
  sf::Vector2f scale = sprite.getScale();

  std::cout << "Sprite position: " << pos.x << "," << pos.y << std::endl;
  std::cout << "Sprite scale: " << scale.x << "," << scale.y << std::endl;
  std::cout << "====================================" << std::endl;
}
