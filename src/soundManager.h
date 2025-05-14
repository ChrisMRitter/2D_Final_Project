#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class SoundManager {
public:
    void loadSound(const std::string& name, const std::string& filename) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filename)) {
            buffers[name] = buffer;
        }
    }

    void playSound(const std::string& name) {
        if (buffers.find(name) != buffers.end()) {
            sf::Sound sound;
            sound.setBuffer(buffers[name]);
            sounds.push_back(sound);
            sounds.back().play();
        }
    }

    void update() {
    sounds.erase(std::remove_if(sounds.begin(), sounds.end(),
         [](const sf::Sound & sound) { return sound.getStatus() == sf::Sound::Stopped; }),
        sounds.end());
}

private:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::vector<sf::Sound> sounds;
};
