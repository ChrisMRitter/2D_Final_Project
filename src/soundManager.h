// soundManager.h (currently unused)
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

// SoundManager class handles loading, playing, and managing sound effects
class SoundManager {
public:
    // Loads a sound from a file and stores it in a map with a given name
    void loadSound(const std::string& name, const std::string& filename) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filename)) {
            buffers[name] = buffer; // Store the buffer under the given name
        }
    }

    // Plays a sound by name if its has been loaded
    void playSound(const std::string& name) {
        if (buffers.find(name) != buffers.end()) {
            sf::Sound sound;
            sound.setBuffer(buffers[name]); // Set the buffer for the sound
            sounds.push_back(sound); // Store the sound in the active list
            sounds.back().play(); // Play the most recently added sound
        }
    }

    // Removes sounds that have finished playing from the active list
    void update() {
        sounds.erase(std::remove_if(sounds.begin(), sounds.end(),
             { return sound.getStatus() == sf::Sound::Stopped; }),
            sounds.end());
    }

private:
    // Map of sound names to their corresponding sound buffers
    std::unordered_map<std::string, sf::SoundBuffer> buffers;

    // List of currently playing sounds
    std::vector<sf::Sound> sounds;
};
