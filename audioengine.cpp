#include "audioengine.h"

AudioEngine::AudioEngine()
{
}

// playsound method

void AudioEngine::playSound(const std::string clip){
    if (!buffer.loadFromFile(clip)){
        return;
    }
    sound.setBuffer(buffer);
    sound.play();
}

// loop soundfile

void AudioEngine::loop(const std::string clip){
    if (!buffer.loadFromFile(clip)){
        return;
    }
    sound.setBuffer(buffer);
    sound.setLoop(true);
    sound.play();

}

