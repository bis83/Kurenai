#include "GameTitle.h"
#include "DIManager.h"
#include "HightScore.h"

GameTitle::GameTitle() {
	m_pSprTitleBack = new CSprite();
	m_pSprTitleBack->Init("Data/title_back.png");

	m_pSprTitleLogo = new CSprite();
	m_pSprTitleLogo->Init("Data/title.png");
	m_pSprTitleLogo->SetPos(0, 60);

	m_pSprNumber = new CSprite();
	m_pSprNumber->Init("Data/number.png");

	m_pSeStart = new CSound();
	m_pSeStart->Init("Data/gamestart.wav");

	Init();
}

GameTitle::~GameTitle() {
	delete m_pSprTitleBack;
	delete m_pSprTitleLogo;

	delete m_pSeStart;
}

void GameTitle::Init() {
	next_scene = -1;
	t = 0;
}

void GameTitle::Update() {
	t++;
	if(t >= 240) {
		t -= 240;
	}
	// 決定ボタンが押されたら
	if(g_pDIManager->IsKeyDownNow(0)) {
		m_pSeStart->Play(false, true);
		// ゲームを開始
		next_scene = 1;
	}
}

void GameTitle::Draw() {
	m_pSprTitleBack->Draw();
	m_pSprTitleLogo->SetPos(0, 60 + 10*sin( 2*3.14 * (t/240.0f) ));
	m_pSprTitleLogo->Draw();

	int keta = 10000000;
	int j = g_highscore;
	m_pSprNumber->SetScale(1.0f, 1.0f);
	for(int i=0; i<8; ++i) {
		int k = j / keta;
		m_pSprNumber->SetPos(260+24*i, 420);
		m_pSprNumber->SetUV(k*24, 0, k*24+24, 24);
		m_pSprNumber->Draw();
		j %= keta;
		keta /= 10;
	}
}

int GameTitle::IsNextScene() {
	return next_scene;
}