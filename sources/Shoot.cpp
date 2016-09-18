#include "GameMain.h"

Shoot::Shoot() {
	m_pSprShoot = new CSprite();
	m_pSprShoot->Init("Data/shoot.png");
	m_pSprShoot->SetCenterPosition(CP_MIDDLECENTER);

	Init();
}

Shoot::~Shoot() {
	delete m_pSprShoot;
}

void Shoot::Init() {
	for(int i=0; i<MAX_SHOOT; ++i) {
		m_shootlist[i].active = false;
	}
}

void Shoot::Draw() {
	for(int i=0; i<MAX_SHOOT; ++i) {
		if(m_shootlist[i].active) {
			m_pSprShoot->SetPos(m_shootlist[i].x, m_shootlist[i].y);
			m_pSprShoot->Draw();
		}
	}
}

void Shoot::CreateNewShoot(float pos_x, float pos_y) {
	for(int i=0; i<MAX_SHOOT; ++i) {
		if(!m_shootlist[i].active) {
			m_shootlist[i].active = true;
			m_shootlist[i].x = pos_x;
			m_shootlist[i].y = pos_y;
			break;
		}
	}
}

void Shoot::Move() {
	for(int i=0; i<MAX_SHOOT; ++i) {
		if(m_shootlist[i].active) {
			m_shootlist[i].y -= 12.0f;

			// ‰æ–ÊŠO”»’è
			if( (m_shootlist[i].x < 120.0f-16.0f) || (m_shootlist[i].x > 520.0f+32.0f) ||
				(m_shootlist[i].y < 40.0f-16.0f) || (m_shootlist[i].y > 440.0f+32.0f) ) {
					m_shootlist[i].active = false;
			}
		}
	}
}

bool Shoot::HitShoot(float pos_x, float pos_y, float r) {
	for(int i=0; i<MAX_SHOOT; ++i) {
		if(m_shootlist[i].active) {
			float x = m_shootlist[i].x;
			float y = m_shootlist[i].y;
			if(pow( x - pos_x, 2 ) + pow( y - pos_y, 2 ) < pow( r + 16.0f , 2 ) ) {
				m_shootlist[i].active = false;
				return true;
			}
		}
	}
	return false;
}
