#include "animatedBackground.h"
#include <iostream>

AnimatedBackground::AnimatedBackground(std::shared_ptr<sf::Texture> sheet, float fps)
    : frameTime(1.f / fps)
{
    sprite.setTexture(*sheet);
    auto sz   = sheet->getSize();
    int  h    = sz.y;
    int  cols = sz.x / h;
    frames.reserve(cols);
    for (int i = 0; i < cols; ++i)
        frames.emplace_back(i * h, 0, h, h);
    sprite.setTextureRect(frames[0]);
}

void AnimatedBackground::loadSheet(std::shared_ptr<sf::Texture> sheet, float fps) {
    // reset animation state
    frameTime    = 1.f / fps;
    elapsed      = 0.f;
    currentFrame = 0;
    frames.clear();

    // assign new texture and rebuild frames
    sprite.setTexture(*sheet);
    auto sz   = sheet->getSize();
    int  h    = sz.y;
    int  cols = sz.x / h;
    frames.reserve(cols);
    for (int i = 0; i < cols; ++i)
        frames.emplace_back(i * h, 0, h, h);
    sprite.setTextureRect(frames[0]);
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
    // Get the current view size
    sf::Vector2f viewSize = window.getView().getSize();
    
    // Position the sprite to cover the entire view
    sprite.setPosition(0, 0);
    
    // Calculate the scale needed to cover the entire view
    auto spriteSize = sprite.getLocalBounds();
    if (spriteSize.width > 0 && spriteSize.height > 0) {
      float scaleX = viewSize.x / spriteSize.width;
      float scaleY = viewSize.y / spriteSize.height;
      
      // Use the larger scale to ensure full coverage but multiply by scaleFactor for adjustment
      float baseScale = std::max(scaleX, scaleY);
      sprite.setScale(baseScale * scaleFactor, baseScale * scaleFactor);
    
    
    // Debug output
        std::cout << "Applying scale: " << (baseScale * scaleFactor) 
        << " (baseScale=" << baseScale 
        << ", scaleFactor=" << scaleFactor << ")" << std::endl;


    // Draw the sprite
    window.draw(sprite);
}
}
