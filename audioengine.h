#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <SFML/Audio.hpp>

class AudioEngine
{
public:
    AudioEngine();

private:
    sf::SoundBuffer buffer;
    sf::Sound sound;

public:
    void playSound(const std::string clip);
    void loop(const std::string clip);


};

#endif // AUDIOENGINE_H
