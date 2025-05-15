// Include SFML modules for graphics, window events, mouse input, and audio
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Audio.hpp>

// Include custom game modules
#include "enemyManager.h"
#include "laser.h"
#include "player.h"
#include "textManager.h"
#include "animatedBackground.h"
#include "levelManager.h"
#include "gameconstants.h"

// Include standard library headers
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

// Utility function to load a texture from a file, optionally using a sub-rectangle
static std::shared_ptr<sf::Texture> loadTexture(
    const std::string& path,
    const sf::IntRect& rect = sf::IntRect())
{
    auto tex = std::make_shared<sf::Texture>();
    if (rect.width && rect.height) {
        if (!tex->loadFromFile(path, rect)) return nullptr;
    } else {
        if (!tex->loadFromFile(path)) return nullptr;
    }
    return tex;
}

// Displays the start screen with a prompt to press Enter
static void startScreen(sf::RenderWindow& window,
                        const sf::Font& font)
{
    sf::Text prompt("Press Enter to Start", font, 50);
    prompt.setFillColor(sf::Color::White);
    auto b = prompt.getLocalBounds();
    prompt.setOrigin(b.width/2.f, b.height/2.f);
    prompt.setPosition(window.getView().getCenter());

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close(); return;
            } else if (ev.type == sf::Event::KeyPressed &&
                       ev.key.code == sf::Keyboard::Enter) {
                return;
            }
        }
        window.clear();
        window.draw(prompt);
        window.display();
    }
}

// Displays the game over screen with a prompt to press Escape to quit
static void gameOverScreen(sf::RenderWindow& window,
                           const sf::Font& font)
{
    sf::Text over("Game Over\nPress Esc to Quit", font, 50);
    over.setFillColor(sf::Color::Red);
    auto b = over.getLocalBounds();
    over.setOrigin(b.width/2.f, b.height/2.f);
    over.setPosition(window.getView().getCenter());

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed ||
               (ev.type == sf::Event::KeyPressed &&
                ev.key.code == sf::Keyboard::Escape)) {
                return;
            }
        }
        window.clear();
        window.draw(over);
        window.display();
    }
}

int main() {
    // Create the main game window
    sf::RenderWindow window(
        sf::VideoMode((int)VIEW_WIDTH, (int)VIEW_HEIGHT),
        "Space Defender");
    window.setFramerateLimit(60);

    // Load font for text rendering
    sf::Font font;
    if (!font.loadFromFile(
        "./Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) return -1;
    // Initialize text manager with the same font
    textManager textMgr;
    textMgr.loadFont(
        "./Assets/Fonts/VeniteAdoremus-rgRBA.ttf");

    // Show the start screen
    startScreen(window, font);

    // Initialize player with sprite and speed
    Player player(
        "./Assets/Sprites/test_sprites/PixelSpaceRage/128px/PlayerBlue_Frame_01_png_processed.png",
        600.f);
    std::vector<Laser> lasers; // Container for player lasers

    // Load enemy texture
    auto chaserTex = loadTexture(
        "./Assets/Sprites/test_sprites/enemy_green.png");
    if (!chaserTex) return -1;

    // Load background sprite sheet and initialize animated background
    auto bgSheet = loadTexture(
        "./Assets/Sprites/test_sprites/terransprite.png");
    if (!bgSheet) return -1;
    AnimatedBackground animBg(bgSheet, 6.f);
    animBg.setScaleFactor(0.5f);

    // Initialize level manager with references to game components
    LevelManager lvlMgr(
        &textMgr, &animBg, &player, chaserTex);

    // Load and play background music
    sf::Music music;
    if (music.openFromFile(
        "./Assets/Sound/bgmusic.mp3")) {
        music.setLoop(true);
        music.setVolume(50.f);
        music.play();
    }

    sf::Clock clock; // For tracking delta time
    bool paused = false;
    
    // Text to display when game is paused
    sf::Text pauseText("Paused - Press P to Resume", font, 24);
    pauseText.setFillColor(sf::Color::White);
    { auto b = pauseText.getLocalBounds();
      pauseText.setOrigin(b.width/2.f, b.height/2.f);
      pauseText.setPosition(window.getView().getCenter()); }

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); break;
            } else if (event.type == sf::Event::KeyPressed &&
                       event.key.code == sf::Keyboard::P) {
                paused = !paused;
                if (paused) window.setView(window.getDefaultView());
            } else if (event.type == sf::Event::Resized) {
                // Adjust view to maintain aspect ratio
                float winA  = (float)event.size.width / event.size.height;
                float viewA = VIEW_WIDTH / VIEW_HEIGHT;
                sf::View v = window.getView();
                if (winA > viewA)
                    v.setSize(VIEW_HEIGHT * winA, VIEW_HEIGHT);
                else
                    v.setSize(VIEW_WIDTH, VIEW_WIDTH/viewA);
                window.setView(v);
            }
        }

        // Handle pause state
        if (paused) {
            window.clear();
            window.draw(pauseText);
            window.display();
            continue;
        }

        // Check for game over
        if (player.getHealth() <= 0) {
            gameOverScreen(window, font);
            break;
        }

        // Calculate delta time and clamp it
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        // Update game objects
        animBg.update(dt);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(
            sf::Mouse::getPosition(window));
        player.update(dt, lasers, mouseWorld);
        for (auto& l : lasers) l.update(dt);
        lvlMgr.update(dt, lasers);

        // Remove lasers that are out of bounds
        lasers.erase(
            std::remove_if(
                lasers.begin(), lasers.end(),
                [](Laser& l){ return l.isOutOfBounds(); }),
            lasers.end());

        // Render everything
        window.clear();
        animBg.draw(window);
        lvlMgr.draw(window);
        player.draw(window);
        for (auto& l : lasers) l.draw(window);

        // Draw UI if not in cutscene
        if (!lvlMgr.isCutsceneActive()) {
            auto vc = window.getView().getCenter();
            auto vs = window.getView().getSize();
            textMgr.updatePlayerHealth(
                player.getHealth(), vc, vs);
            textMgr.updateScoreDisplay(vc, vs);
            textMgr.draw(window);
        }

        window.display();
    }

    return 0;
}
