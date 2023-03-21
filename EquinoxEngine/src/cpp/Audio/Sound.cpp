#include "../../Equinox/Application/Audio/Sound.hpp"

eq::Audio::Sound::Sound()
{
	ZeroMemory(&m_Whdr, sizeof(m_Whdr));
	ZeroMemory(&m_Wfx, sizeof(m_Wfx));
	m_Wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_Wfx.nChannels = 1;
	m_Wfx.nSamplesPerSec = 44100;
	m_Wfx.wBitsPerSample = 16;
	m_Wfx.nBlockAlign = m_Wfx.nChannels * m_Wfx.wBitsPerSample / 8;
	m_Wfx.nAvgBytesPerSec = m_Wfx.nSamplesPerSec * m_Wfx.nBlockAlign;
	waveOutOpen(&m_HWaveOut, WAVE_MAPPER, &m_Wfx, 0, 0, CALLBACK_NULL);
}

eq::Audio::Sound::~Sound()
{
	waveOutReset(m_HWaveOut);
	waveOutUnprepareHeader(m_HWaveOut, &m_Whdr, sizeof(WAVEHDR));
	waveOutClose(m_HWaveOut);
}

bool eq::Audio::Sound::loadSound(const char* fileName)
{
	m_SoundBuffer.clear();

	std::ifstream file(fileName, std::ios::binary);
	if (!file)
	{
		return false;
	}

	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	m_SoundBuffer.resize(size);

	file.read(m_SoundBuffer.data(), size);
	file.close();

	m_Whdr.lpData = m_SoundBuffer.data();
	m_Whdr.dwBufferLength = static_cast<DWORD>(m_SoundBuffer.size());

	MMRESULT result = waveOutPrepareHeader(m_HWaveOut, &m_Whdr, sizeof(WAVEHDR));
	if (result != MMSYSERR_NOERROR) {}

	return true;
}

void eq::Audio::Sound::play(float volume, float pitch)
{
	waveOutReset(m_HWaveOut);

	DWORD dwVolume = static_cast<DWORD>(volume * 0xFFFF);
	waveOutSetVolume(m_HWaveOut, MAKELONG(dwVolume, dwVolume));

	DWORD dwPitch = static_cast<DWORD>(pitch * 100);
	waveOutSetPitch(m_HWaveOut, dwPitch);

	waveOutWrite(m_HWaveOut, &m_Whdr, sizeof(m_Whdr));


}
