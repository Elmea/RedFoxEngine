#pragma once
#include <Irrklang/irrKlang.h>

#include "MyMemory.hpp"
#include "RedfoxMaths.hpp"

namespace RedFoxEngine
{
	class Sound 
	{
	private:
		irrklang::ISoundEngine* m_soundEngine;

		friend class SoundManager;
		
		irrklang::ISoundSource* m_source = nullptr;
		irrklang::ISound* m_sound = nullptr;
		
		bool m_free = false;
		int m_index;
		bool m_loopState;
		
	public:
		RedFoxMaths::Float3 position;
		
		void Play();
		void Play3D();
		void Stop();
		void SetLoop(bool loop);
		void SwapPauseState();
		void Pause();
		void Resume();
		void SetVolume(float volume);
		int GetIndex();

		~Sound();
	};
	
	class SoundManager
	{
	private:
		irrklang::ISoundEngine* m_soundEngine;

		int m_maxSounds = 100;
		int m_soundCount = 0;
		int m_freedSound = 0;

		Sound* m_sounds;
		MyString* m_soundsName;
	public:
		void Init(Memory* memAllocator);
		Sound* CreateSound(const char* file, Memory* memAllocator);
		void deleteSound(int index);

		// Play a sound by his Index, return the corresponding sound
		Sound* PlaySoundByIndex(int index);

		// Play a sound by his Index with 3D, return the corresponding sound
		Sound* PlaySoundByIndex3D(int index);

		void UpdateListener(RedFoxMaths::Float3 position, RedFoxMaths::Float3 rotation);
		void SetMasterVolume(float volume);

		int GetSoundCount();
		MyString* GetSoundsName();

		~SoundManager();
	};
}
