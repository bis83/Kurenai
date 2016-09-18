#pragma once

#include "windows.h"
#include <vorbis/vorbisfile.h>

#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")

BOOL IsWaveFile(LPSTR filename) {
	HMMIO hMmio = NULL;
	hMmio = mmioOpen(filename, NULL, MMIO_ALLOCBUF | MMIO_READ);

	MMCKINFO riffckInfo;

	if(MMSYSERR_NOERROR != mmioDescend(hMmio, &riffckInfo, NULL, 0)) {
		return FALSE;
	}
	if((riffckInfo.ckid != mmioFOURCC('R','I','F','F'))||(riffckInfo.fccType != mmioFOURCC('W','A','V','E'))) {
		return FALSE;
	}
	mmioClose(hMmio, NULL);
	return TRUE;
}

BOOL ReadWaveFormatFromWave(LPSTR filename, WAVEFORMATEX *pWavFmt, DWORD *pWavSize, DWORD *pDataOffset) {
	HMMIO hMmio = NULL;
	hMmio = mmioOpen(filename, NULL, MMIO_ALLOCBUF | MMIO_READ);

	MMCKINFO ckInfo;
	MMCKINFO riffckInfo;

	ckInfo.ckid = mmioFOURCC('f','m','t',' ');
	if(MMSYSERR_NOERROR != mmioDescend(hMmio, &riffckInfo, NULL, NULL)) {
		return FALSE;
	}
	if(MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK)) {
		return FALSE;
	}
	if(mmioRead(hMmio, (HPSTR)pWavFmt, sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX)) {
		return FALSE;
	}
	if(pWavFmt->wFormatTag != WAVE_FORMAT_PCM) {
		return FALSE;
	}
	if(MMSYSERR_NOERROR != mmioAscend(hMmio, &ckInfo, 0)) {
		return FALSE;
	}
	ckInfo.ckid = mmioFOURCC('d','a','t','a');
	if(MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK)) {
		return FALSE;
	}
	*pWavSize = ckInfo.cksize;
	*pDataOffset = ckInfo.dwDataOffset;
	mmioClose(hMmio, NULL);
	return TRUE;
}

BOOL ReadWaveData(void *pBuf, UINT readSize, LPSTR filename, UINT head_size, UINT seek) {
	FILE *fp = NULL;
	if((fp = fopen(filename, "rb")) < 0) {
		return FALSE;
	}
	fseek(fp, head_size+seek, SEEK_SET);
	fread(pBuf, 1, readSize, fp);
	fclose(fp);
	return TRUE;
}

BOOL IsOggFile(LPSTR filename) {
	BOOL ret = FALSE;
	FILE *fp = NULL;
	if((fp = fopen(filename, "rb")) > 0) {
		OggVorbis_File vf;
		if(ov_open(fp, &vf, NULL, 0) == 0) {
			ret = TRUE;
			ov_clear(&vf);
		}
		fclose(fp);
	}
	return ret;
}

BOOL ReadWaveFormatFromOgg(LPSTR filename, WAVEFORMATEX *pWavFmt, DWORD *pWavSize, DWORD *pDataOffset) {
	BOOL ret = FALSE;
	FILE *fp = NULL;
	if((fp=fopen(filename, "rb")) > 0) {
		OggVorbis_File vf;
		if(ov_open(fp, &vf, NULL, 0) == 0) {
			vorbis_info *vi;
			vi = ov_info(&vf, -1);
			pWavFmt->cbSize = sizeof(WAVEFORMATEX);
			pWavFmt->nChannels = vi->channels;
			pWavFmt->nSamplesPerSec = vi->rate;
			pWavFmt->wBitsPerSample = 16;
			pWavFmt->nBlockAlign = pWavFmt->nChannels * pWavFmt->wBitsPerSample / 8;
			pWavFmt->nAvgBytesPerSec = pWavFmt->nBlockAlign * pWavFmt->nSamplesPerSec;
			pWavFmt->wFormatTag = WAVE_FORMAT_PCM;
			*pWavSize = (DWORD)(max(ov_pcm_total(&vf, -1), 0)) * pWavFmt->nBlockAlign;
			*pDataOffset = 0;
			ov_clear(&vf);
			ret = TRUE;
		}
		fclose(fp);
	}
	return ret;
}

BOOL ReadOggData(void *pBuf, UINT readSize, LPSTR filename, UINT head_size, UINT seek) {
	BOOL ret = FALSE;
	FILE *fp = NULL;
	if((fp = fopen(filename, "rb")) > 0) {
		OggVorbis_File vf;
		if(ov_open(fp, &vf, NULL, 0) == 0) {
			int read = 0;
			ov_pcm_seek(&vf, seek);
			do {
				read = ov_read(&vf, (char*)pBuf, readSize, 0, 2, 1, NULL);
				pBuf = (void*)((char*)pBuf + read);
				readSize -= read;
			} while(read > 0);
			ov_clear(&vf);
			ret = TRUE;
		}
		fclose(fp);
	}
	return ret;
}
