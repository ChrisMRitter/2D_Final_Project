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
#include "asteroid.h"
#include "Level3Manager.h"
#include "enemyTypes.h"

// Standard library includes
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

// Utility function to load a texture from file, optionally using a sub-rectangle
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

// Display the start screen with instructions until the player presses Enter
static void startScreen(sf::RenderWindow& window, const sf::Font& font)
{
    sf::Text prompt(
        "Press Enter to Start\n"
        "Press P to Pause\n"
        "Press Esc to Quit\n"
        "Press A and D to Move\n"
        "Press Space to Shoot\n"
        "Use Mouse to Aim",
        font, 50);
    prompt.setFillColor(sf::Color::White);
    auto b = prompt.getLocalBounds();
    prompt.setOrigin(b.width/2.f, b.height/2.f);
    prompt.setPosition(window.getView().getCenter());

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return;
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

// Display the game over screen until the player presses Escape or closes the window
static void gameOverScreen(sf::RenderWindow& window, const sf::Font& font)
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
        sf::VideoMode(static_cast<int>(VIEW_WIDTH), static_cast<int>(VIEW_HEIGHT)),
        "Space Defender");
    window.setFramerateLimit(60);

    // Load font for UI text
    sf::Font font;
    if (!font.loadFromFile("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf"))
        return -1;
    textManager textMgr;
    textMgr.loadFont("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf");

    // Show the start screen
    startScreen(window, font);

    // Initialize player and laser container
    Player player(
        "./Assets/Sprites/test_sprites/PixelSpaceRage/128px/PlayerBlue_Frame_01_png_processed.png",
        600.f);
    std::vector<Laser> lasers;

    // Load enemy textures
    auto greenEnemyTex = loadTexture("./Assets/Sprites/test_sprites/enemy_green.png");
    if (!greenEnemyTex) return -1;
    auto redEnemyTex   = loadTexture("./Assets/Sprites/test_sprites/enemy_red.png");
    if (!redEnemyTex)   return -1;
    auto mothershipTex = loadTexture("./Assets/Sprites/test_sprites/final_boss.png");
    if (!mothershipTex) return -1;

    // Load and configure animated background
    auto bgSheet = loadTexture("./Assets/Sprites/test_sprites/terransprite.png");
    if (!bgSheet) return -1;
    AnimatedBackground animBg(bgSheet, 6.f);
    animBg.setScaleFactor(0.5f);

    // Load asteroid texture
    auto asteroidTex = loadTexture("./Assets/Sprites/Asteroid 01_png_processed.png");
    if (!asteroidTex) return -1;

    // Initialize level manager with all game assets
    LevelManager lvlMgr(
        &textMgr, &animBg, &player,
        greenEnemyTex, redEnemyTex, mothershipTex, asteroidTex
    );

    // Load and play background music
    sf::Music music;
    if (music.openFromFile("./Assets/Sound/Music/Cosmic_Assault.mp3")) {
        music.setLoop(true);
        music.setVolume(50.f);
        music.play();
    }

    sf::Clock clock;
    bool paused = false;

    // Set up pause text
    sf::Text pauseText("Paused - Press P to Resume", font, 24);
    {
        auto b = pauseText.getLocalBounds();
        pauseText.setOrigin(b.width/2.f, b.height/2.f);
        pauseText.setPosition(window.getView().getCenter());
    }

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Handle window close and pause/resume
            if (event.type == sf::Event::Closed ||
               (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed &&
                       event.key.code == sf::Keyboard::P) {
                paused = !paused;
                if (paused) window.setView(window.getDefaultView());
            } else if (event.type == sf::Event::Resized) {
                // Adjust view to maintain aspect ratio on resize
                float winA  = static_cast<float>(event.size.width) / event.size.height;
                float viewA = VIEW_WIDTH / VIEW_HEIGHT;
                sf::View v = window.getView();
                if (winA > viewA)
                    v.setSize(VIEW_HEIGHT * winA, VIEW_HEIGHT);
                else
                    v.setSize(VIEW_WIDTH, VIEW_WIDTH/viewA);
                window.setView(v);
            }
        }

        // If paused, show pause screen
        if (paused) {
            window.clear();
            window.draw(pauseText);
            window.display();
            continue;
        }

        // If player is dead, show game over screen
        if (player.getHealth() <= 0) {
            gameOverScreen(window, font);
            break;
        }

        // Calculate delta time
        float dt = clock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;

        // Update game objects
        animBg.update(dt);
        sf::Vector2f mouseWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        player.update(dt, lasers, mouseWorld);
        for (auto& l : lasers) l.update(dt);
        lvlMgr.update(dt, lasers);

        // Remove lasers that are off-screen
        lasers.erase(
            std::remove_if(
                lasers.begin(), lasers.end(),
                [](Laser& l){ return l.isOutOfBounds(); }
            ),
            lasers.end()
        );

        // Draw all game elements
        window.clear();
        animBg.draw(window);
        lvlMgr.draw(window);
        player.draw(window);
        for (auto& l : lasers) l.draw(window);

        // Draw UI if not in cutscene
        if (!lvlMgr.isCutsceneActive()) {
            auto vc = window.getView().getCenter();
            auto vs = window.getView().getSize();
            textMgr.updatePlayerHealth(player.getHealth(), vc, vs);
            textMgr.updateScoreDisplay(vc, vs);
            textMgr.updateLaserCooldownDisplay(player.getLaserCooldown(), vc, vs);
            textMgr.draw(window);
        }

        window.display();
    }

    return 0;
}
