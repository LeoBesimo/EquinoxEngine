#pragma once

#include <Windows.h>
#include <MMSystem.h>
#include <vector>
#include <fstream>

namespace eq
{
	namespace Audio
	{
		class Sound
		{
		private:
			HWAVEOUT m_HWaveOut;
			WAVEFORMATEX m_Wfx;
			WAVEHDR m_Whdr;
			std::vector<char> m_SoundBuffer;

		public:
			Sound();
			~Sound();
			bool loadSound(const char* fileName);
			void play(float volume, float pitch);
		};
	}
}