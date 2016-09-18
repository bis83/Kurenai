#include "GameMain.h"

#define TAMA_SIZE 16

Photon::Photon() {
	m_pSprTama = new CSprite();
	m_pSprTama->Init("Data/tama.png");
	m_pSprTama->SetCenterPosition(CP_MIDDLECENTER);

	m_pSeTamakie = new CSound();
	m_pSeTamakie->Init("Data/tamakie.wav");

	Init();
}

Photon::~Photon() {
	delete m_pSprTama;

	delete m_pSeTamakie;
}

void Photon::Init() {
	for(int i=0; i<MAX_PHOTON; ++i) {
		m_photonlist[i].active = false;
	}
}

void Photon::Draw() {
	for(int i=0; i<MAX_PHOTON; ++i) {
		if(m_photonlist[i].active) {
			m_pSprTama->SetPos(m_photonlist[i].pos_x, m_photonlist[i].pos_y);
			m_pSprTama->SetUV( (m_photonlist[i].type % 4)*TAMA_SIZE, (m_photonlist[i].type / 4)*TAMA_SIZE,
				(m_photonlist[i].type % 4)*TAMA_SIZE+TAMA_SIZE, (m_photonlist[i].type / 4)*TAMA_SIZE+TAMA_SIZE );
			m_pSprTama->SetRot(atan2(-m_photonlist[i].v_y, m_photonlist[i].v_x));
			m_pSprTama->Draw();
		}
	}
}

int Photon::HitPlayerAndCristal(Player *pPlayer) {
	int ret = 0;
	for(int i=0; i<MAX_PHOTON; ++i) {
		if(m_photonlist[i].active) {
			if(pPlayer->HitPlayer(m_photonlist[i].pos_x, m_photonlist[i].pos_y, 4.0f, m_photonlist[i].type%2)) {
				m_pSeTamakie->Play(false, true);
				ret++;
			}
		}
	}
	return ret;
}

void Photon::CreateNewPhoton(int type, float pos_x, float pos_y, float v_x, float v_y) {
	for(int i=0; i<MAX_PHOTON; ++i) {
		if(!m_photonlist[i].active) {
			m_photonlist[i].active = true;
			m_photonlist[i].type = type;
			m_photonlist[i].pos_x = pos_x;
			m_photonlist[i].pos_y = pos_y;
			m_photonlist[i].v_x = v_x;
			m_photonlist[i].v_y = v_y;
			break;
		}
	}
}

void Photon::Move() {
	for(int i=0; i<MAX_PHOTON; ++i) {
		if(m_photonlist[i].active) {
			m_photonlist[i].pos_x += m_photonlist[i].v_x;
			m_photonlist[i].pos_y += m_photonlist[i].v_y;

			// ‰æ–ÊŠO”»’è
			if( (m_photonlist[i].pos_x < 120.0f-16.0f) || (m_photonlist[i].pos_x > 520.0f+16.0f) ||
				(m_photonlist[i].pos_y < 40.0f-16.0f) || (m_photonlist[i].pos_y > 440.0f+16.0f) ) {
					m_photonlist[i].active = false;
			}
		}
	}
}
