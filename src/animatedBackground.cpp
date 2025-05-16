#include "animatedBackground.h"
#include <iostream>

// Constructor: Initializes the animated background with a texture sheet and frame rate
AnimatedBackground::AnimatedBackground(std::shared_ptr<sf::Texture> sheet,
                                       float fps)
    : frameTime(1.f / fps), texture(sheet) {
  sprite.setTexture(*sheet); // Assign texture to sprite
  auto sz = sheet->getSize(); // Get texture size
  int h = sz.y; // Assume square frames: height = frame size
  int cols = sz.x / h; // Number of frames = texture width / frame height
  frames.reserve(cols); // Reserve memory for frame rectangles

  // Create frame rectangles from the sprite sheet
  for (int i = 0; i < cols; ++i)
    frames.emplace_back(i * h, 0, h, h);
  sprite.setTextureRect(frames[0]); // Set initial frame
}

// Loads a new sprite sheet and resets animation state
void AnimatedBackground::loadSheet(std::shared_ptr<sf::Texture> sheet,
                                   float fps) {
  // Reset animation state
  frameTime = 1.f / fps;
  elapsed = 0.f;
  currentFrame = 0;
  frames.clear();
  texture.reset();

  texture = sheet; // Store new texture
  
  sprite.setTexture(*sheet, true); // Assign new texture to sprite
  auto sz = sheet->getSize();
  
  // Assume square frames arranged horizontally
  int frameHeight = sz.y;
  int frameWidth = frameHeight;
  int numFrames = sz.x / frameWidth;
  
  // Debug output
  std::cout << "Loading spritesheet: " << sz.x << "x" << sz.y
            << ", frame size: " << frameWidth << "x" << frameHeight
            << ", frames: " << numFrames << std::endl;

  frames.reserve(numFrames);

  // Generate frame rectangles
  for (int i = 0; i < numFrames; ++i) {
    frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
  }

  // Set the first frame if available
  if (!frames.empty()) {
    sprite.setTextureRect(frames[0]);
  } else {
    std::cerr << "Warning: No animation frames were created!"
              << std::endl; // debugging
  }
}

// Sets a custom scale factor for the background sprite
void AnimatedBackground::setScaleFactor(float scale) {
  scaleFactor = scale;
  sprite.setScale(scaleFactor, scaleFactor);
}

// Updates the animation based on elapsed time
void AnimatedBackground::update(float dt) {
  elapsed += dt;
  
  // Advance to the next frame if enough time has passed
  if (elapsed >= frameTime) {
    elapsed -= frameTime;
    currentFrame = (currentFrame + 1) % frames.size();
    sprite.setTextureRect(frames[currentFrame]);
  }
}

// Draws the animated background, scaled to fill the entire view
void AnimatedBackground::draw(sf::RenderWindow &window) {
  sf::Vector2f viewSize = window.getView().getSize(); // Get the current view
  sprite.setPosition(0, 0); // Position sprite at top-left
  auto spriteSize = sprite.getLocalBounds();

  // Scale sprite to cover the entire view
  if (spriteSize.width > 0 && spriteSize.height > 0) {
    float scaleX = viewSize.x / spriteSize.width;
    float scaleY = viewSize.y / spriteSize.height;

    // Use the larger scale to ensure full coverage, adjusted by scaleFactor
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

// Prints debug information about the animated background
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
