#include "audioengine.h"

AudioEngine::AudioEngine()
{
}

void AudioEngine::playSound(const std::string clip){
    if (!buffer.loadFromFile(clip)){
        return;
    }
    sound.setBuffer(buffer);
    sound.play();
}

void AudioEngine::loop(const std::string clip){
    if (!buffer.loadFromFile(clip)){
        return;
    }
    sound.setBuffer(buffer);
    sound.setLoop(true);
    sound.play();

}

