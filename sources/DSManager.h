#pragma once

#include <dsound.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

enum WAVE_FORMAT {
	WAVE,
	OGG
};

class CDSManager {
	LPDIRECTSOUND8 m_pDevice;

public:
	CDSManager();
	~CDSManager();

	BOOL Init(HWND hWnd);

	LPDIRECTSOUND8 GetDevice();
};

class CSound {
	LPDIRECTSOUNDBUFFER8 m_pBuffer;

	bool m_playing;
public:
	CSound();
	~CSound();

	BOOL Init(LPSTR filename);

	void Play(bool loop_f, bool reset_f);
	bool IsPlaying();
	void Stop();

	void SetVolume(int per);
	void SetPan(int per);
	void SetFrequency(int per);
};

struct MUSIC_DESC {
	LPSTR filename;
	DWORD start, end;

	WAVE_FORMAT format;
	DWORD dwHeadSize;
	DWORD dwFileSeek;
	DWORD dwBufferSize;
	DWORD dwBlockSize;
	DWORD dwSecSize;
	DWORD dwPcm;
	DWORD dwWriteCursor;
};

class CMusic {
	LPDIRECTSOUNDBUFFER8 m_pBuffer;
	bool m_playing;
	bool m_looping;
	bool m_end_f;
	MUSIC_DESC m_desc;
public:
	CMusic();
	~CMusic();

	BOOL Init(LPSTR filename);

	// ループの指定10ミリ秒単位
	void SetLoopPoint(DWORD start, DWORD end);

	void Play(bool loop_f, bool reset_f, int sec);
	void Update();
	bool IsPlaying();
	void Stop();

	void SetVolume(int per);
	void SetPan(int per);
	void SetFrequency(int per);
};

extern CDSManager *g_pDSManager;
