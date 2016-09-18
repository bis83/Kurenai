#include "GameMain.h"

#include "DIManager.h"

Player::Player() {
	m_pSprPlayer = new CSprite();
	m_pSprPlayer->Init("Data/player.png");
	Init();
}

void Player::Init() {
	player_pos_x = 320 - 32;
	player_pos_y = 360 - 32;
	player_color = 0;
}

Player::~Player() {
	delete m_pSprPlayer;

}

void Player::Move() {

	// プレイヤーの移動
	if(abs(g_pDIManager->GetAnalog(0)) > 500) {
		player_pos_x += 2.0f * (g_pDIManager->GetAnalog(0) > 0 ? 1 : -1) / (abs(g_pDIManager->GetAnalog(1)) > 500 ?  sqrt(2.0f) : 1.0f);
	}
	if(abs(g_pDIManager->GetAnalog(1)) > 500) {
		player_pos_y += 2.0f * (g_pDIManager->GetAnalog(1) > 0 ? 1 : -1) / (abs(g_pDIManager->GetAnalog(0)) > 500 ?  sqrt(2.0f) : 1.0f);
	}

	// 壁の制限
	if(player_pos_x < 110) {
		player_pos_x = 110.0f;
	}
	if(player_pos_x > 530 - 64) {
		player_pos_x = 530.0f - 64.0f;
	}
	if(player_pos_y < 30) {
		player_pos_y = 30.0f;
	}
	if(player_pos_y > 450 - 64) {
		player_pos_y = 450.0f - 64.0f;
	}

	// ショット
	if(g_pDIManager->IsKeyDownNow(0)) {
		pShoot->CreateNewShoot(player_pos_x+32.0f, player_pos_y-0.0f);
	}
}

void Player::SetShoot(Shoot *p) {
	pShoot = p;
}

void Player::Draw() {
	m_pSprPlayer->SetPos(player_pos_x, player_pos_y);
	m_pSprPlayer->Draw();
}

bool Player::HitPlayer(float pos_x, float pos_y, float r, int color) {
	return ( pow( player_pos_x + 32.0f - pos_x, 2 ) + pow( player_pos_y + 32.0f - pos_y, 2 ) < pow( r + 2.0f , 2 ) );
}

bool Player::HitCristal(float pos_x, float pos_y, float r, int color) {
	return false;
}
