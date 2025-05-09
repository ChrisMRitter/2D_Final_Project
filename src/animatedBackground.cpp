#include "animatedBackground.h"

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

void AnimatedBackground::draw(sf::RenderWindow& window) {
    // make sure the sprite covers the whole view;
    // if you want it centered, you could call:
    // sprite.setPosition(0,0);
    window.draw(sprite);
}
