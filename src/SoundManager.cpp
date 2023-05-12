#include "SoundManager.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#include <iostream>

using namespace RedFoxEngine;

#pragma region SoundClass

void Sound::Play()
{
    m_sound = m_soundEngine->play2D(m_source, m_loopState, false, true);
}

void Sound::Play3D()
{
    irrklang::vec3df posIrr{ position.x, position.y, position.z};
    m_sound = m_soundEngine->play3D(m_source, posIrr, m_loopState, false, true);
    m_sound->setIsLooped(m_loopState);
}

void Sound::Stop()
{
    m_sound->stop();
}

void Sound::SetLoop(bool loop)
{
    if (m_sound)
        m_sound->setIsLooped(loop);

    m_loopState = loop;
}

void Sound::SwapPauseState()
{
    m_sound->setIsPaused(!m_sound->getIsPaused());
}

void Sound::Pause()
{
    m_sound->setIsPaused(true);
}

void Sound::Resume()
{
    m_sound->setIsPaused(false);
}

void Sound::SetVolume(float volume)
{
    m_source->setDefaultVolume(volume);
    
    if (m_sound)
        m_sound->setVolume(volume);
}

int Sound::GetIndex()
{
    return m_index;
}

Sound::~Sound()
{
    if (m_sound)
        m_sound->drop();
}

#pragma endregion

#pragma region SoundManagerClass

void SoundManager::Init(Memory* memAllocator)
{
    m_soundEngine = irrklang::createIrrKlangDevice();
    m_sounds = (Sound*)MyMalloc(memAllocator, sizeof(Sound) * m_maxSounds);
    m_soundsName = (MyString*)MyMalloc(memAllocator, sizeof(MyString) * m_maxSounds);
    for (int i = 0; i < m_maxSounds; i++)
        m_soundsName->capacity = 64;
}

Sound* SoundManager::CreateSound(const char* file, Memory* memAllocator)
{
    if (!m_soundEngine)
    {
        std::cout << "Sound device not initialized";
        return nullptr;
    }

    if (m_soundCount - m_freedSound >= m_maxSounds)
    {
        std::cout << "Max sounds count already hitted, can't load more";
        return nullptr;
    }

    Sound newOne;
    newOne.m_source = m_soundEngine->addSoundSourceFromFile(file);
    if (!newOne.m_source)
        return nullptr;
    newOne.m_soundEngine = m_soundEngine;
    m_soundsName[m_soundCount] = initStringChar(file, strlen(file), memAllocator);
    
    if (m_freedSound > 0)
    {
        for (int i = 0; i < m_soundCount; i++)
        {
            if (m_sounds[i].m_free)
            {
                m_sounds[i] = newOne;
                m_sounds[i].m_free = false;
                m_freedSound--;
                return &m_sounds[newOne.m_index];
            }
        }
    }
    
    newOne.m_index = m_soundCount;
    m_sounds[newOne.m_index] = newOne;
    m_soundCount++;
    return &m_sounds[newOne.m_index];
}

void SoundManager::DeleteSound(int index)
{
    if (m_sounds[index].m_free)
        return;

    m_sounds[index].m_source->drop();
    m_sounds[index].m_free = true;
    m_freedSound++;
}

Sound* SoundManager::PlaySoundByIndex(int index)
{
    if (!m_soundEngine)
        return nullptr;

    m_soundEngine->play2D(m_sounds[index].m_source, m_sounds[index].m_loopState);
    return &m_sounds[index];
}

Sound* SoundManager::PlaySoundByIndex3D(int index)
{
    if (!m_soundEngine)
        return nullptr;

    irrklang::vec3df pos {m_sounds[index].position.x, m_sounds[index].position.y, m_sounds[index].position.z};
    m_soundEngine->play3D(m_sounds[index].m_source, pos, m_sounds[index].m_loopState);
    return &m_sounds[index];
}

void SoundManager::UpdateListener(RedFoxMaths::Float3 position, RedFoxMaths::Float3 rotation)
{
    if (!m_soundEngine)
        return;

    irrklang::vec3df posIrr {position.x, position.y, position.z};

    RedFoxMaths::Float3 dir = -RedFoxMaths::Float3::EulerToDir({ rotation.x, -rotation.y, rotation.z } );
    irrklang::vec3df dirIrr {dir.x, dir.y, dir.z};

    m_soundEngine->setListenerPosition(posIrr, dirIrr);
}

void SoundManager::SetMasterVolume(float volume)
{
    if (!m_soundEngine)
        return;

    m_soundEngine->setSoundVolume(volume);
}

SoundManager::~SoundManager()
{
    if (m_soundEngine)
        m_soundEngine->drop();
}

#pragma endregion 