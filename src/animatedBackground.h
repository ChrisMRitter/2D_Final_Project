#ifndef ANIMATED_BACKGROUND_H
#define ANIMATED_BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class AnimatedBackground {
public:
    // ctor: sheet is a single‐row spritesheet, fps is frames per second
    AnimatedBackground(std::shared_ptr<sf::Texture> sheet, float fps);

    // Reload a new spritesheet at runtime (e.g. when switching levels)
    void loadSheet(std::shared_ptr<sf::Texture> sheet, float fps);

    // Overall scale factor (e.g. 0.5f)
    void setScaleFactor(float scale);

    // Advance the animation by dt seconds
    void update(float dt);

    // Draw to the given window
    void draw(sf::RenderWindow& window);
//debugging
    void debugInfo() const;

private:
    std::shared_ptr<sf::Texture> texture; //stores texture
    sf::Sprite               sprite;
    std::vector<sf::IntRect> frames;
    float                    frameTime;     // seconds per frame
    float                    elapsed  = 0.f; // time since last frame switch
    size_t                   currentFrame = 0;
    float                    scaleFactor  = 1.f;
};

#endif
