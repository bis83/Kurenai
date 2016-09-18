#include "DSManager.h"
#include "ifwave.h"

CDSManager *g_pDSManager = NULL;

CDSManager::CDSManager() {
	m_pDevice = NULL;
}

CDSManager::~CDSManager() {
	if(m_pDevice) m_pDevice->Release();
}

BOOL CDSManager::Init(HWND hWnd) {
	if(FAILED(DirectSoundCreate8(NULL, &m_pDevice, NULL))) {
		MessageBox(NULL, "DirectSound‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½", NULL, MB_OK);
		return FALSE;
	}
	if(FAILED(m_pDevice->SetCooperativeLevel(hWnd, DSSCL_EXCLUSIVE))) {
		return FALSE;
	}
	return TRUE;
}

LPDIRECTSOUND8 CDSManager::GetDevice() {
	return m_pDevice;
}

CSound::CSound() {
	m_pBuffer = NULL;
	m_playing = false;
}

CSound::~CSound() {
	if(m_pBuffer) m_pBuffer->Release();
}

BOOL CSound::Init(LPSTR filename) {
	if(m_playing) {
		return FALSE;
	}
	if(IsWaveFile(filename)) {
		WAVEFORMATEX wfex;
		DWORD dwWavSize, dwDataOffset;
		if(!ReadWaveFormatFromWave(filename, &wfex, &dwWavSize, &dwDataOffset)) {
			return FALSE;
		}

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(dsbd));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;
		dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
		dsbd.dwBufferBytes = dwWavSize;
		dsbd.lpwfxFormat = &wfex;

		LPDIRECTSOUNDBUFFER pBuffer = NULL;
		if(FAILED(g_pDSManager->GetDevice()->CreateSoundBuffer(&dsbd, &pBuffer, NULL))) {
			return FALSE;
		}
		if(FAILED(pBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pBuffer))) {
			return FALSE;
		}
		void *p = NULL;
		DWORD dwBufferSize = 0;
		m_pBuffer->Lock(0, dwWavSize, &p, &dwBufferSize, NULL, NULL, NULL);
		if(!ReadWaveData(p, dwBufferSize, filename, dwDataOffset, 0)) {
			return FALSE;
		}
		m_pBuffer->Unlock(p, dwBufferSize, NULL, NULL);
	} else if(IsOggFile(filename)) {
		WAVEFORMATEX wfex;
		DWORD dwWavSize, dwDataOffset;
		if(!ReadWaveFormatFromOgg(filename, &wfex, &dwWavSize, &dwDataOffset)) {
			return FALSE;
		}

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(dsbd));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;
		dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
		dsbd.dwBufferBytes = dwWavSize;
		dsbd.lpwfxFormat = &wfex;

		LPDIRECTSOUNDBUFFER pBuffer = NULL;
		HRESULT hr;
		if(FAILED(hr = g_pDSManager->GetDevice()->CreateSoundBuffer(&dsbd, &pBuffer, NULL))) {
			return FALSE;
		}
		if(FAILED(pBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pBuffer))) {
			return FALSE;
		}
		void *p = NULL;
		DWORD dwBufferSize = 0;
		m_pBuffer->Lock(0, dwWavSize, &p, &dwBufferSize, NULL, NULL, NULL);
		if(!ReadOggData(p, dwBufferSize, filename, dwDataOffset, 0)) {
			return FALSE;
		}
		m_pBuffer->Unlock(p, dwBufferSize, NULL, NULL);
	}
	return TRUE;
}

void CSound::Play(bool loop_f, bool reset_f) {
	if(reset_f) {
		m_pBuffer->Stop();
		m_pBuffer->SetCurrentPosition(0);
	}
	if(loop_f) {
		m_pBuffer->Play(NULL, NULL, DSBPLAY_LOOPING);
	} else {
		m_pBuffer->Play(NULL, NULL, NULL);
	}
	m_playing = true;
}

bool CSound::IsPlaying() {
	return m_playing;
}

void CSound::Stop() {
	m_pBuffer->Stop();
	m_playing = false;
}

void CSound::SetVolume(int per) {
	m_pBuffer->SetVolume(10000 * per / 100);
}

void CSound::SetPan(int per) {
	m_pBuffer->SetPan(per * 100);
}

void CSound::SetFrequency(int per) {
	DWORD freq;
	m_pBuffer->SetFrequency(DSBFREQUENCY_ORIGINAL);
	m_pBuffer->GetFrequency(&freq);
	m_pBuffer->SetFrequency(freq * per / 100);
}


CMusic::CMusic() {
	m_pBuffer = NULL;
	m_playing = false;
	m_looping = false;
	m_end_f = false;
	ZeroMemory(&m_desc, sizeof(m_desc));
}

CMusic::~CMusic() {
	if(m_pBuffer) m_pBuffer->Release();
	if(m_desc.filename) delete[] m_desc.filename;
}

BOOL CMusic::Init(LPSTR filename) {
	m_desc.filename = new TCHAR[strlen(filename)+1];
	strcpy(m_desc.filename, filename);

	if(IsWaveFile(m_desc.filename)) {
		WAVEFORMATEX wfex;
		DWORD dwWavSize, dwDataOffset;
		if(!ReadWaveFormatFromWave(filename, &wfex, &dwWavSize, &dwDataOffset)) {
			return FALSE;
		}

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(dsbd));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;
		dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
		dsbd.dwBufferBytes = wfex.nAvgBytesPerSec * 16;
		dsbd.lpwfxFormat = &wfex;

		LPDIRECTSOUNDBUFFER pBuffer = NULL;
		if(FAILED(g_pDSManager->GetDevice()->CreateSoundBuffer(&dsbd, &pBuffer, NULL))) {
			return FALSE;
		}
		if(FAILED(pBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pBuffer))) {
			return FALSE;
		}

		m_desc.format = WAVE;
		m_desc.dwHeadSize = dwDataOffset;
		m_desc.dwFileSeek = 0;
		m_desc.dwBufferSize = dsbd.dwBufferBytes;
		m_desc.dwBlockSize = m_desc.dwBufferSize / 4;
		m_desc.dwSecSize = m_desc.dwBlockSize / 4;
		m_desc.dwPcm = wfex.nBlockAlign;
		m_desc.dwWriteCursor = 0;

		m_desc.start = 0;
		m_desc.end = dwWavSize / (m_desc.dwSecSize / 100);
	} else if(IsOggFile(m_desc.filename)) {
		WAVEFORMATEX wfex;
		DWORD dwWavSize, dwDataOffset;
		if(!ReadWaveFormatFromOgg(filename, &wfex, &dwWavSize, &dwDataOffset)) {
			return FALSE;
		}

		DSBUFFERDESC dsbd;
		ZeroMemory(&dsbd, sizeof(dsbd));
		dsbd.dwSize = sizeof(DSBUFFERDESC);
		dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;
		dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
		dsbd.dwBufferBytes = wfex.nAvgBytesPerSec * 16;
		dsbd.lpwfxFormat = &wfex;

		LPDIRECTSOUNDBUFFER pBuffer = NULL;
		if(FAILED(g_pDSManager->GetDevice()->CreateSoundBuffer(&dsbd, &pBuffer, NULL))) {
			return FALSE;
		}
		if(FAILED(pBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pBuffer))) {
			return FALSE;
		}

		m_desc.format = OGG;
		m_desc.dwHeadSize = dwDataOffset;
		m_desc.dwFileSeek = 0;
		m_desc.dwBufferSize = dsbd.dwBufferBytes;
		m_desc.dwBlockSize = m_desc.dwBufferSize / 4;
		m_desc.dwSecSize = wfex.nAvgBytesPerSec;
		m_desc.dwPcm = wfex.nBlockAlign;
		m_desc.dwWriteCursor = 0;

		m_desc.start = 0;
		m_desc.end = dwWavSize / (m_desc.dwSecSize / 100);
	}
	return TRUE;
}

void CMusic::SetLoopPoint(DWORD start, DWORD end) {
}

void CMusic::Play(bool loop_f, bool reset_f, int sec) {
	m_looping = loop_f;
	m_end_f = false;
	if(reset_f) {
		m_pBuffer->Stop();
		m_pBuffer->SetCurrentPosition(0);
		m_desc.dwWriteCursor = 0;
		m_desc.dwFileSeek = 0;
		if(sec > 0) {
			m_desc.dwFileSeek += sec * m_desc.dwSecSize;
		}

		void *p;
		DWORD dwBufSize;
		if(m_desc.format == WAVE) {
			if(SUCCEEDED(m_pBuffer->Lock(0, m_desc.dwBufferSize, &p, &dwBufSize, NULL, NULL, NULL))) {
				if(m_desc.dwFileSeek + m_desc.dwBufferSize > m_desc.end  * m_desc.dwSecSize / 100) {
					if(m_looping) {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadWaveData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek);
						m_desc.dwFileSeek = m_desc.start * m_desc.dwSecSize / 100;
						p = (void*)((char*)p + dwTempSize);
						ReadWaveData(p, m_desc.dwBufferSize - dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek);
						m_desc.dwFileSeek += m_desc.dwBufferSize - dwTempSize;
					} else {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadWaveData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek);
						p = (void*)((char*)p + dwTempSize);
						m_desc.dwFileSeek = m_desc.end * m_desc.dwSecSize / 100;
						ZeroMemory(&p, m_desc.dwBlockSize - dwTempSize);
						m_end_f = true;
					}
				} else {
					ReadWaveData(p, m_desc.dwBufferSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek);
					m_desc.dwFileSeek += m_desc.dwBufferSize;
				}
				m_pBuffer->Unlock(p, dwBufSize, NULL, NULL);
			}
		} else if(m_desc.format == OGG) {
			if(SUCCEEDED(m_pBuffer->Lock(0, m_desc.dwBufferSize, &p, &dwBufSize, NULL, NULL, NULL))) {
				if(m_desc.dwFileSeek + m_desc.dwBufferSize > m_desc.end  * m_desc.dwSecSize / 100) {
					if(m_looping) {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadOggData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek/m_desc.dwPcm);
						m_desc.dwFileSeek = m_desc.start * m_desc.dwSecSize / 100;
						void *p2 = (void*)((char*)p + dwTempSize);
						ReadOggData(p2, m_desc.dwBufferSize-dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek/m_desc.dwPcm);
						m_desc.dwFileSeek += m_desc.dwBufferSize-dwTempSize;
					} else {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadOggData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek/m_desc.dwPcm);
						void *p2 = (void*)((char*)p + dwTempSize);
						m_desc.dwFileSeek = m_desc.end * m_desc.dwSecSize / 100;
						ZeroMemory(&p2, m_desc.dwBlockSize - dwTempSize);
						m_end_f = true;
					}
				} else {
					ReadOggData(p, m_desc.dwBufferSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek / m_desc.dwPcm);
					m_desc.dwFileSeek += m_desc.dwBufferSize;
				}
				m_pBuffer->Unlock(p, dwBufSize, NULL, NULL);
			}
		}
	}
	m_pBuffer->Play(NULL, NULL, DSBPLAY_LOOPING);
	m_playing = true;
}

bool CMusic::IsPlaying() {
	return m_playing;
}

void CMusic::Stop() {
	m_pBuffer->Stop();
	m_playing = false;
}

void CMusic::Update() {
	if(!m_playing) {
		return;
	}
	DWORD dwPlayCursor;
	m_pBuffer->GetCurrentPosition(&dwPlayCursor, NULL);
	if(m_end_f) {
		if(dwPlayCursor - m_desc.dwWriteCursor > 0 && dwPlayCursor - m_desc.dwWriteCursor < m_desc.dwBlockSize) {
			Stop();
		}
		return;
	}
	if((dwPlayCursor > m_desc.dwWriteCursor + m_desc.dwBlockSize) || (dwPlayCursor < m_desc.dwWriteCursor && m_desc.dwWriteCursor >= m_desc.dwBlockSize*3)) {
		void *p = 0;
		DWORD dwBufSize = 0;
		if(m_desc.format == WAVE) {
			if(SUCCEEDED(m_pBuffer->Lock(m_desc.dwWriteCursor, m_desc.dwBlockSize, &p, &dwBufSize, NULL, NULL, NULL))) {
				if(m_desc.dwFileSeek+m_desc.dwBlockSize > m_desc.end * m_desc.dwSecSize / 100) {
					if(m_looping) {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadWaveData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek);
						m_desc.dwFileSeek = m_desc.start * m_desc.dwSecSize / 100;
						void *p2 = (void*)((char*)p + dwTempSize);
						ReadWaveData(p2, m_desc.dwBlockSize - dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek);
						m_desc.dwFileSeek += m_desc.dwBlockSize - dwTempSize;
					} else {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadWaveData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek / m_desc.dwPcm);
						void *p2 = (void*)((char*)p + dwTempSize);
						m_desc.dwFileSeek = m_desc.end * m_desc.dwSecSize / 100;
						ZeroMemory(&p2, m_desc.dwBlockSize - dwTempSize);
						m_end_f = true;
					}
				} else {
					ReadWaveData(p, m_desc.dwBlockSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek);
					m_desc.dwFileSeek += m_desc.dwBlockSize;
				}
				m_pBuffer->Unlock(p, dwBufSize, NULL, NULL);
				m_desc.dwWriteCursor = (m_desc.dwWriteCursor + m_desc.dwBlockSize) % m_desc.dwBufferSize;
			}
		} else if(m_desc.format == OGG) {
			HRESULT hr;
			if(SUCCEEDED(hr = m_pBuffer->Lock(m_desc.dwWriteCursor, m_desc.dwBlockSize, &p, &dwBufSize, NULL, NULL, NULL))) {
				if(m_desc.dwFileSeek + m_desc.dwBlockSize > m_desc.end * m_desc.dwSecSize / 100) {
					if(m_looping) {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadOggData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek/m_desc.dwPcm);
						m_desc.dwFileSeek = m_desc.start * m_desc.dwSecSize / 100;
						void *p2 = (void*)((char*)p + dwTempSize);
						ReadOggData(p2, m_desc.dwBlockSize - dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek / m_desc.dwPcm);
						m_desc.dwFileSeek += m_desc.dwBlockSize - dwTempSize;
					} else {
						DWORD dwTempSize = m_desc.end * m_desc.dwSecSize / 100 - m_desc.dwFileSeek;
						ReadWaveData(p, dwTempSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek / m_desc.dwPcm);
						void *p2 = (void*)((char*)p + dwTempSize);
						m_desc.dwFileSeek = m_desc.end * m_desc.dwSecSize / 100;
						ZeroMemory(p2, m_desc.dwBlockSize - dwTempSize);
						m_end_f = true;
					}
				} else {
					ReadOggData(p, m_desc.dwBlockSize, m_desc.filename, m_desc.dwHeadSize, m_desc.dwFileSeek/m_desc.dwPcm);
					m_desc.dwFileSeek += m_desc.dwBlockSize;
				}
				m_pBuffer->Unlock(p, dwBufSize, NULL, NULL);
				m_desc.dwWriteCursor = (m_desc.dwWriteCursor + m_desc.dwBlockSize) % m_desc.dwBufferSize;
			}
		}
	}
}

void CMusic::SetVolume(int per) {
	m_pBuffer->SetVolume(10000 * per / 100);
}

void CMusic::SetPan(int per) {
	m_pBuffer->SetPan(per * 100);
}

void CMusic::SetFrequency(int per) {
	DWORD freq;
	m_pBuffer->SetFrequency(DSBFREQUENCY_ORIGINAL);
	m_pBuffer->GetFrequency(&freq);
	m_pBuffer->SetFrequency(freq * per / 100);
}