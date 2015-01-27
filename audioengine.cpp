#include "audioengine.h"
#include <QDebug>

AudioEngine::AudioEngine()
{
}

// playsound method

void AudioEngine::playSound(const std::string clip){

    buffer.loadFromFile(clip);

    if (!buffer.loadFromFile(clip)){
        qDebug() << "File not found or Format not supported";
        return;
    }

    sound.setBuffer(buffer);
    sound.play();
}

// loop soundfile

void AudioEngine::loop(const std::string clip){

    buffer.loadFromFile(clip);

    if (!buffer.loadFromFile(clip)){
        qDebug() << "File not found or Format not supported";
        return;
    }
    sound.setBuffer(buffer);
    sound.setLoop(true);
    sound.play();

}

