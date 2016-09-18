#include "GameMain.h"

Gate::Gate() {
	m_pSprGate = new CSprite();
	m_pSprGate->Init("Data/gate.png");
	m_pSprGate->SetCenterPosition(CP_MIDDLECENTER);

	m_pSeGatekie = new CSound();
	m_pSeGatekie->Init("Data/gatekie.wav");

	m_pSeGateAppear = new CSound();
	m_pSeGateAppear->Init("Data/gateappear.wav");

	m_pSeTamahassya = new CSound();
	m_pSeTamahassya->Init("Data/tamahassya.wav");
}

void Gate::Init() {
	for(int i=0; i<MAX_GATE; ++i) {
		m_gatelist[i].active = false;
	}
	nActiveGate = 0;
	zero_flag = true;
}

Gate::~Gate() {
	delete m_pSprGate;

	delete m_pSeGatekie;
	delete m_pSeGateAppear;
	delete m_pSeTamahassya;
}

void Gate::Move() {
	for(int i=0; i<MAX_GATE; ++i) {
		if(m_gatelist[i].active) {
			m_gatelist[i].time++;
			if(m_gatelist[i].time <= 0) {
				continue;
			}
			if(m_gatelist[i].time == 1) {
				zero_flag = false;
				m_pSeGateAppear->Play(false, true);
			}

			m_gatelist[i].pos_x += m_gatelist[i].v_x;
			m_gatelist[i].pos_y += m_gatelist[i].v_y;

			// 壁との反射
			if(m_gatelist[i].pos_x < 110 || m_gatelist[i].pos_x > 520 - 64) {
				m_gatelist[i].v_x = -m_gatelist[i].v_x;
			}
			if(m_gatelist[i].pos_y < 30 || m_gatelist[i].pos_y > 440 - 64) {
				m_gatelist[i].v_y = -m_gatelist[i].v_y;
			}

			// 壁の制限
			if(m_gatelist[i].pos_x < 110) {
				m_gatelist[i].pos_x = 110.0f;
			}
			if(m_gatelist[i].pos_x > 520 - 64) {
				m_gatelist[i].pos_x = 520.0f - 64.0f;
			}
			if(m_gatelist[i].pos_y < 30) {
				m_gatelist[i].pos_y = 30.0f;
			}
			if(m_gatelist[i].pos_y > 440 - 64) {
				m_gatelist[i].pos_y = 440.0f - 64.0f;
			}

			switch(m_gatelist[i].type) {
				case 0:
					if(m_gatelist[i].time % (60 - m_gatelist[i].level%8*3) == 0) {
						m_pSeTamahassya->Play(false, true);
						for(int j=0; j< (8 + m_gatelist[i].level/10); j++) {
							m_pPhoton->CreateNewPhoton(0, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
								2.0f * cos(2.0f * D3DX_PI * j / (8 + m_gatelist[i].level/10)),
								2.0f * sin(2.0f * D3DX_PI * j / (8 + m_gatelist[i].level/10))
								);
						}
					}
					break;
				case 1:
					if(m_gatelist[i].time % (60 - m_gatelist[i].level%10*3) == 0) {
						m_pSeTamahassya->Play(false, true);
						for(int j=0, rnd = rand()%360; j< (8 + m_gatelist[i].level/10); j++) {
							m_pPhoton->CreateNewPhoton(1, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
								2.0f * cos(D3DX_PI * j / (8 + m_gatelist[i].level/10) + 2 * D3DX_PI * rnd / 360),
								2.0f * sin(D3DX_PI * j / (8 + m_gatelist[i].level/10) + 2 * D3DX_PI * rnd / 360)
								);
						}
					}
					break;
				case 2:
					if(m_gatelist[i].time % (12 - m_gatelist[i].level % 10 / 3) == 0) {
						m_pSeTamahassya->Play(false, true);
						m_pPhoton->CreateNewPhoton(2, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
							2.0f * cos(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10)),
							2.0f * sin(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10))
							);
					}
					break;
				case 3:
					if(m_gatelist[i].time % (30 - m_gatelist[i].level%10*2) == 0) {
						m_pSeTamahassya->Play(false, true);
						for(int j=0, rnd=rand()%360; j< (4 + m_gatelist[i].level/10); j++) {
							rnd = rand() % 360;
							m_pPhoton->CreateNewPhoton(3, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
								2.0f * cos(2 * D3DX_PI * rnd / 360),
								2.0f * sin(2 * D3DX_PI * rnd / 360)
								);
						}
					}
					break;
				case 4:
					if(m_gatelist[i].time % (60 - m_gatelist[i].level%8*3) == 0) {
						m_pSeTamahassya->Play(false, true);
						for(int j=0; j< (4 + m_gatelist[i].level/10); j++) {
							m_pPhoton->CreateNewPhoton(4, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
								1.5f * cos(2.0f * D3DX_PI * j / (4 + m_gatelist[i].level/10)),
								1.5f * sin(2.0f * D3DX_PI * j / (4 + m_gatelist[i].level/10))
								);
						}
					}
					if((m_gatelist[i].time + 30) % (60 - m_gatelist[i].level%8*3) == 0) {
						m_pSeTamahassya->Play(false, true);
						for(int j=0; j< (4 + m_gatelist[i].level/10); j++) {
							m_pPhoton->CreateNewPhoton(4, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
								1.5f * cos(2.0f * D3DX_PI * (2*j+1) / ((4 + m_gatelist[i].level/10) * 2) ),
								1.5f * sin(2.0f * D3DX_PI * (2*j+1) / ((4 + m_gatelist[i].level/10) * 2) )
								);
						}
					}
					break;
				case 5:
					if(m_gatelist[i].time % (60 - m_gatelist[i].level%10*3) == 0) {
						m_pSeTamahassya->Play(false, true);
						for(int j=0, rnd = rand()%360; j< (4 + m_gatelist[i].level/10); j++) {
							m_pPhoton->CreateNewPhoton(5, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
								1.0f * cos(D3DX_PI / 2 * j / (4 + m_gatelist[i].level/10) + 2 * D3DX_PI * rnd / 360),
								1.0f * sin(D3DX_PI / 2 * j / (4 + m_gatelist[i].level/10) + 2 * D3DX_PI * rnd / 360)
								);
							m_pPhoton->CreateNewPhoton(5, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
								2.0f * cos(D3DX_PI / 2 * j / (4 + m_gatelist[i].level/10) + 2 * D3DX_PI * rnd / 360),
								2.0f * sin(D3DX_PI / 2 * j / (4 + m_gatelist[i].level/10) + 2 * D3DX_PI * rnd / 360)
								);
						}
					}
					break;
				case 6:
					if(m_gatelist[i].time % (24 - m_gatelist[i].level % 10 / 3) == 0) {
						m_pSeTamahassya->Play(false, true);
						m_pPhoton->CreateNewPhoton(6, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
							2.0f * cos(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10)),
							2.0f * sin(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10))
							);
						m_pPhoton->CreateNewPhoton(6, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
							-2.0f * cos(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10)),
							-2.0f * sin(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10))
							);
					}
					break;
				case 7:
					if(m_gatelist[i].time % (16 - m_gatelist[i].level % 10 / 3) == 0) {
						m_pSeTamahassya->Play(false, true);
						m_pPhoton->CreateNewPhoton(7, m_gatelist[i].pos_x, m_gatelist[i].pos_y,
							0.5f * (rand()%5 + 2 ) * cos(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10)),
							0.5f * (rand()%5 + 2) * sin(2 * D3DX_PI * (m_gatelist[i].time % (60 + m_gatelist[i].level/10*10)) / (60 + m_gatelist[i].level/10*10))
							);
					}
					break;
			}
		}
	}
}

void Gate::SetPhoton(Photon *pPhoton) {
	m_pPhoton = pPhoton;
}

void Gate::Draw() {
	for(int i=0; i<MAX_GATE; ++i) {
		if(!m_gatelist[i].active)
			continue;
		if(m_gatelist[i].time <= 0) {
			m_pSprGate->SetColor(128, 255, 255, 255);
		} else {
			m_pSprGate->SetColor(255, 255, 255, 255);
		}
		//m_pSprGate->SetRot(2 * D3DX_PI * m_gatelist[i].time / 120);
		m_pSprGate->SetPos(m_gatelist[i].pos_x, m_gatelist[i].pos_y);
		m_pSprGate->Draw();
	}
}

int Gate::HitShoot(Shoot *pShoot) {
	int ret = 0;
	for(int i=0; i<MAX_GATE; ++i) {
		if(!m_gatelist[i].active) {
			continue;
		}
		if(m_gatelist[i].time <= 0) {
			continue;
		}
		if(pShoot->HitShoot(m_gatelist[i].pos_x, m_gatelist[i].pos_y, 32.0f)) {
			// ゲートの削除
				m_pSeGatekie->Play(false, true);
				m_gatelist[i].active = false;
				nActiveGate--;
				ret++;
		}
	}
	return ret;
}

bool Gate::CreateNewGate(int type, float pos_x, float pos_y, int level) {
	// レベルによるゲート最大数制限
	if(nActiveGate >= level/10+1) {
		return false;
	}
	for(int i=0; i<MAX_GATE; ++i) {
		if(!m_gatelist[i].active) {
			m_gatelist[i].active = true;
			m_gatelist[i].level = level;
			m_gatelist[i].pos_x = pos_x;
			m_gatelist[i].pos_y = pos_y;
			m_gatelist[i].type = type;
			m_gatelist[i].time = -60;
			int rnd = rand() % 360;
			m_gatelist[i].v_x = cos(2 * D3DX_PI * rnd / 360);
			m_gatelist[i].v_y = sin(2 * D3DX_PI * rnd / 360);
			nActiveGate++;
			break;
		}
	}
	return true;
}

bool Gate::GateZero() {
	if(zero_flag == false) {
		for(int i=0; i<MAX_GATE; ++i) {
			if(m_gatelist[i].active == true && m_gatelist[i].time > 0) {
				return false;
			}
		}
		zero_flag = true;
		return true;
	}
	return false;
}

bool Gate::IsZero() {
	return nActiveGate <= 0;
}
