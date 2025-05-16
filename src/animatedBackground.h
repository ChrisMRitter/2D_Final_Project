#ifndef ANIMATED_BACKGROUND_H
#define ANIMATED_BACKGROUND_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

// The AnimatedBackground class handles rendering and animating a looping background using a single-row sprite sheet
class AnimatedBackground {
public:
  /*
  Constructor: Initializes the background with a sprite sheet and desired frame rate (fps)
  The sprite sheet is expected to contain square frames arranged horizontally
  */
  AnimatedBackground(std::shared_ptr<sf::Texture> sheet, float fps);

  // Loads a new sprite sheet at runtime, useful for changing backgrounds between levels
  void loadSheet(std::shared_ptr<sf::Texture> sheet, float fps);

  // Sets a global scale factor for the background (e.g., 0.5f to shrink, 2.0f to enlarge)
  void setScaleFactor(float scale);

  // Updates the animation based on elapsed time (dt in seconds)
  void update(float dt);

  // Draws the current frame of the animated background to the given render window
  void draw(sf::RenderWindow &window);

  // Prints debug information about the animation state (for development use)
  void debugInfo() const;

private:
  std::shared_ptr<sf::Texture> texture; // Shared pointer to the sprite sheet
  sf::Sprite sprite; // Sprite used to display the current frame
  std::vector<sf::IntRect> frames; // List of texture rectangles representing animation frames
  float frameTime;     // Time (in seconds) each frame is displayed
  float elapsed = 0.f; // Time accumulated since the last frame switch
  size_t currentFrame = 0; // Index of the current frame being displayed
  float scaleFactor = 1.f; // Custom scale factor applied to the sprite
};

#endif
