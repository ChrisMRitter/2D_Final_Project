#include "animatedBackground.h"

AnimatedBackground::AnimatedBackground(std::shared_ptr<sf::Texture> sheet, float fps)
    : frameTime(1.f / fps)
{
    sprite.setTexture(*sheet);
    auto sz = sheet->getSize();
    int h = sz.y;
    int cols = sz.x / h;
    frames.reserve(cols);
    for (int i = 0; i < cols; ++i) {
        frames.emplace_back(i * h, 0, h, h);
    }
    sprite.setTextureRect(frames[0]);
}

void AnimatedBackground::setScaleFactor(float scale) {
    scaleFactor = scale;
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
    // use the current view to center
    sf::View view = window.getView();
    sf::Vector2f viewSize   = view.getSize();
    sf::Vector2f viewCenter = view.getCenter();

    // compute scale to fill view * scaleFactor
    auto rect     = sprite.getTextureRect();
    float sx = viewSize.x / rect.width  * scaleFactor;
    float sy = viewSize.y / rect.height * scaleFactor;
    sprite.setScale(sx, sy);

    // center sprite in view
    float w_scaled = rect.width  * sx;
    float h_scaled = rect.height * sy;
    float x = viewCenter.x - w_scaled * 0.5f;
    float y = viewCenter.y - h_scaled * 0.5f;
    sprite.setPosition(x, y);

    window.draw(sprite);
}

