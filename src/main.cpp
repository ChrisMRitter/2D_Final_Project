// src/main.cpp
#include "enemyManager.h"
#include "laser.h"
#include "player.h"
#include "textManager.h"
#include "animatedBackground.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <memory>
#include <string>
#include <vector>

static const float VIEW_WIDTH  = 800.f;
static const float VIEW_HEIGHT = 600.f;

// Helper: load full texture or sub-rect
std::shared_ptr<sf::Texture> loadTexture(
    const std::string& path,
    const sf::IntRect& rect = sf::IntRect()
) {
    auto tex = std::make_shared<sf::Texture>();
    if (rect.width > 0 && rect.height > 0) {
        if (!tex->loadFromFile(path, rect)) return nullptr;
    } else {
        if (!tex->loadFromFile(path)) return nullptr;
    }
    return tex;
}

// Show start screen: waits for Enter
void startScreen(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text prompt("Press Enter to Start\nPress P to Pause", font, 50);
    prompt.setFillColor(sf::Color::White);
    sf::FloatRect bounds = prompt.getLocalBounds();
    prompt.setOrigin(bounds.width/2.f, bounds.height/2.f);
    prompt.setPosition(window.getView().getCenter());
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close();
                return;
            } else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
                return;
            }
        }
        window.clear();
        window.draw(prompt);
        window.display();
    }
}

// Display Game Over and wait for Escape
void gameOverScreen(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text over("Game Over\nPress Esc to Quit", font, 50);
    over.setFillColor(sf::Color::Red);
    sf::FloatRect bounds = over.getLocalBounds();
    over.setOrigin(bounds.width/2.f, bounds.height/2.f);
    over.setPosition(window.getView().getCenter());
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Escape)
                return;
            if (ev.type == sf::Event::Closed)
                return;
        }
        window.clear();
        window.draw(over);
        window.display();
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode((int)VIEW_WIDTH, (int)VIEW_HEIGHT), "Space Defender");
    window.setFramerateLimit(60);

    // Load font and HUD
    sf::Font font;
    if (!font.loadFromFile("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf")) return -1;
    textManager textMgr;
    textMgr.loadFont("./Assets/Fonts/VeniteAdoremus-rgRBA.ttf");

    // Show start screen
    startScreen(window, font);

    // Initialize player & lasers
    Player player("./Assets/Sprites/test_sprites/PixelSpaceRage/128px/PlayerBlue_Frame_01_png_processed.png", 600.f);
    std::vector<Laser> lasers;

    // Load enemy textures
    auto chaserPtr = loadTexture("./Assets/Sprites/test_sprites/enemy_green.png");
    auto turretPtr = loadTexture("./Assets/Sprites/test_sprites/enemy_red.png");
    auto mothPtr   = loadTexture("./Assets/Sprites/test_sprites/final_boss.png");
    if (!chaserPtr || !turretPtr || !mothPtr) return -1;

    // Configure managers
    EnemyManager chasers, turrets, boss;
    chasers.configure(chaserPtr, 12, 2, 2.5f, 120.f, 0.1f);
    turrets.configure(turretPtr, 6, 10, 8.f, 30.f, 0.3f);
    boss.configure(mothPtr,      1, 20, 0.f, 15.f, 1.f);

    // Animated background
    auto bgSheet = loadTexture("./Assets/Sprites/test_sprites/terransprite.png");
    if (!bgSheet) return -1;
    AnimatedBackground animBg(bgSheet, 6.f);
    animBg.setScaleFactor(0.5f);

    // Music
    sf::Music music;
    if (music.openFromFile("./Assets/Sound/bgmusic.mp3")) {
        music.setLoop(true);
        music.setVolume(50.f);
        music.play();
    }

    bool paused = false;
    sf::Text pauseText("Paused - Press P to Resume", font, 24);
    pauseText.setFillColor(sf::Color::White);
    auto ptb = pauseText.getLocalBounds();
    pauseText.setOrigin(ptb.width/2.f, ptb.height/2.f);
    pauseText.setPosition(window.getView().getCenter());

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) {
                window.close(); break;
            } else if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::P) {
                paused = !paused;
                if (paused) window.setView(window.getDefaultView());
            } else if (ev.type == sf::Event::Resized) {
                float winA = (float)ev.size.width/ev.size.height;
                float viewA = VIEW_WIDTH/VIEW_HEIGHT;
                sf::View v = window.getView();
                if (winA>viewA) v.setSize(VIEW_HEIGHT*winA, VIEW_HEIGHT);
                else            v.setSize(VIEW_WIDTH, VIEW_WIDTH/viewA);
                window.setView(v);
            }
        }
        if (paused) {
            window.clear(); window.draw(pauseText); window.display(); continue;
        }
        // Game over?
        if (player.getHealth() <= 0) { gameOverScreen(window, font); break; }

        float dt = clock.restart().asSeconds(); if (dt>0.1f) dt=0.1f;

        // Update background
        animBg.update(dt);
        // Update player & lasers
        player.update(dt, lasers, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
        for (auto& l : lasers) l.update(dt);

        int score = textMgr.getScore();
        bool spawn = score < 5000;
        bool bossOK = score >= 5000;
        auto pos = player.getPosition();
        chasers.update(dt, pos, spawn);
        turrets.update(dt, pos, spawn);
        boss.update(dt, pos, bossOK);

        int ck = chasers.handleLaserCollisions(lasers);
        int tk = turrets.handleLaserCollisions(lasers);
        int bk = boss.handleLaserCollisions(lasers);
        textMgr.addScore(ck*100 + tk*500 + bk*1000);
        chasers.handlePlayerCollisions(player,1);
        turrets.handlePlayerCollisions(player,5);
        boss.handlePlayerCollisions(player,10);

        // Draw
        window.clear();
        animBg.draw(window);
        chasers.draw(window);
        turrets.draw(window);
        player.draw(window);
        for (auto& l : lasers) l.draw(window);
        boss.draw(window);
        // HUD
        auto vc = window.getView().getCenter();
        auto vs = window.getView().getSize();
        textMgr.updatePlayerHealth(player.getHealth(),vc,vs);
        textMgr.updateScoreDisplay(vc,vs);
        textMgr.draw(window);
        window.display();
    }

    return 0;
}
