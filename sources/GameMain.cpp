#include "GameMain.h"
#include "DIManager.h"
#include "HightScore.h"

GameMain::GameMain() {
	pShoot = new Shoot();
	pPlayer = new Player();
	pPlayer->SetShoot(pShoot);
	pPhoton = new Photon();
	pGate = new Gate();
	pGate->SetPhoton(pPhoton);

	m_pSprBack = new CSprite();
	m_pSprBack->Init("Data/back.png");

	m_pSprBack2 = new CSprite();
	m_pSprBack2->Init("Data/back2.png");

	m_pSprFrame = new CSprite();
	m_pSprFrame->Init("Data/frame.png");

	m_pSprTime = new CSprite();
	m_pSprTime->Init("Data/time.png");

	m_pSprNumber = new CSprite();
	m_pSprNumber->Init("Data/number.png");

	m_pSprGameOver = new CSprite();
	m_pSprGameOver->Init("Data/gameover.png");

	m_pSprClear = new CSprite();
	m_pSprClear->Init("Data/clear.png");

	m_pSeGameOver = new CSound();
	m_pSeGameOver->Init("Data/gameover.wav");

	m_pSeClear = new CSound();
	m_pSeClear->Init("Data/clear.wav");

	m_pBgmMain = new CMusic();
	m_pBgmMain->Init("Data/n1.ogg");
	m_pBgmMain->SetVolume(-10);

	Init();
}

void GameMain::Init() {
	time = 0;
	level = 99;
	score = 0;
	level_time = 0;
	pPlayer->Init();
	pPhoton->Init();
	pGate->Init();
	pShoot->Init();
	next_scene = -1;
	srand(timeGetTime());
	wait = 0;
	gameover = false;
	complete = false;
	back_t = 0;
	gate_t = 0;
}

GameMain::~GameMain() {
	delete pPlayer;
	delete pPhoton;
	delete pGate;

	delete m_pSprBack;
	delete m_pSprFrame;
	delete m_pSprNumber;
	delete m_pSprTime;
	delete m_pSprGameOver;
	delete m_pSprClear;

	delete m_pSeGameOver;
	delete m_pSeClear;

	delete m_pBgmMain;
}

void GameMain::Update() {

	if(wait) {
		wait--;
		return;
	}

	if(gameover) {
		if(g_pDIManager->IsKeyDownNow(0)) {
			if(g_highscore < score) {
				g_highscore = score;
			}
			next_scene = 0;
		}
		return;
	}

	if(complete) {
		if(g_pDIManager->IsKeyDownNow(0)) {
			if(g_highscore < score) {
				g_highscore = score;
			}
			next_scene = 0;
		}
		return;
	}

	if(!m_pBgmMain->IsPlaying())
		m_pBgmMain->Play(true, true, 0);
	m_pBgmMain->Update();

	// �V���[�g�̈ړ�
	pShoot->Move();

	// �v���C���[�̈ړ�
	pPlayer->Move();

	// ���q�̈ړ�
	pPhoton->Move();

	// �Q�[�g�̈ړ�
	pGate->Move();

	// �Q�[�g�̔���
	gate_t++;
	if((gate_t % 60 == 0) && level > 0) {
		if(pGate->CreateNewGate(rand()%8, rand()%300+160, rand()%200+80, 100 - level)) {
			level -= 1;
		}
	}

	// ���q�ƃv���C���[�̓����蔻��
	int i = pPhoton->HitPlayerAndCristal(pPlayer);
	if(i > 0) {
		// �Q�[���I�|�o�[
		m_pBgmMain->Stop();
		m_pSeGameOver->Play(false, true);
		wait = 250;
		gameover = true;
		return;
	}

	// �Q�[�g�ƃv���C���[�̓����蔻��
	i = pGate->HitShoot(pShoot);
	if(i>0) {
		score += 10 * i * (time/60) * (100-level);
		time /= 2;
	}

	// �X�R�A�̃J���X�g
	if(score >= 99999999) {
		score = 99999999;
	}

	// �w�i�̃X�N���[��
	back_t += 2;
	if(back_t > 400) {
		back_t -= 400;
	}

	// �^�C���̏���
	time += 1;
	if(time > 600) time = 600;

	if(0) {
		// �Q�[���I�|�o�[
		m_pBgmMain->Stop();
		m_pSeGameOver->Play(false, true);
		wait = 180;
		gameover = true;
		return;
	}

	if(level == 0 && pGate->IsZero()) {
		level = 0;
		// �Q�[���N���A
		m_pBgmMain->Stop();
		m_pSeClear->Play(false, true);
		wait = 180;
		complete = true;
		return;
	}
}

void GameMain::Draw() {
	// �w�i�̕`��
	BYTE c = 255 - (BYTE)(25 + 25 * sin( 2*3.14f * (back_t/400.0f) ));
	m_pSprBack->SetColor(196, c, c, c);
	m_pSprBack->SetPos(120, 40);
	m_pSprBack->Draw();

	m_pSprBack2->SetColor(128, 255, 255, 255);
	m_pSprBack2->SetPos(120, 40+back_t);
	m_pSprBack2->Draw();
	m_pSprBack2->SetPos(120, -360+back_t);
	m_pSprBack2->Draw();

	// �Q�[�g�̕`��
	pGate->Draw();

	pShoot->Draw();

	// �v���C���[�̕`��
	pPlayer->Draw();

	// ���q�̕`��
	pPhoton->Draw();

	// �g�̕`��
	m_pSprFrame->SetPos(0, 0);
	m_pSprFrame->Draw();

	// �X�R�A�⃌�x���A�^�C���̕`��
	m_pSprTime->SetPos(120, 450);
	m_pSprTime->SetColor(196, 36, 36, 36);
	m_pSprTime->SetUV(0, 0, 400, 20);
	m_pSprTime->Draw();

	m_pSprTime->SetColor(196, 255, 255, 255);
	m_pSprTime->SetUV(0, 0, 400 * time / (60.0f * 10), 20.0f);
	m_pSprTime->Draw();

	// �X�R�A
	int keta = 10000000;
	int j = score;
	m_pSprNumber->SetScale(1.0f, 1.0f);
	for(int i=0; i<8; ++i) {
		int k = j / keta;
		m_pSprNumber->SetPos(260+24*i, 8);
		m_pSprNumber->SetUV(k*24, 0, k*24+24, 24);
		m_pSprNumber->Draw();
		j %= keta;
		keta /= 10;
	}

	keta = 10;
	j = level;
	m_pSprNumber->SetScale(1.5f, 1.5f);
	for(int i=0; i<2; ++i) {
		int k = j / keta;
		m_pSprNumber->SetPos(20+36*i, 120);
		m_pSprNumber->SetUV(k*24, 0, k*24+24, 24);
		m_pSprNumber->Draw();
		j %= keta;
		keta /= 10;
	}

	keta =  100;
	j = time / 60;
	m_pSprNumber->SetScale(1.0f, 1.0f);
	for(int i=0; i<3; ++i) {
		int k = j / keta;
		m_pSprNumber->SetPos(80+24*i, 440);
		m_pSprNumber->SetUV(k*24, 0, k*24+24, 24);
		m_pSprNumber->Draw();
		j %= keta;
		keta /= 10;
	}

	// �Q�[���I�[�o�[��N���A��ʂ̕`��
	if(gameover) {
		m_pSprGameOver->SetColor(255-wait, 255, 255, 255);
		m_pSprGameOver->Draw();

		keta = 10000000;
		j = score;
		m_pSprNumber->SetScale(2.0f, 2.0f);
		for(int i=0; i<8; ++i) {
			int k = j / keta;
			m_pSprNumber->SetPos(120+48*i, 360);
			m_pSprNumber->SetUV(k*24, 0, k*24+24, 24);
			m_pSprNumber->Draw();
			j %= keta;
			keta /= 10;
		}
	}
	if(complete) {
		m_pSprClear->SetColor(255-wait, 255, 255, 255);
		m_pSprClear->Draw();

		keta = 10000000;
		j = score;
		m_pSprNumber->SetScale(2.0f, 2.0f);
		for(int i=0; i<8; ++i) {
			int k = j / keta;
			m_pSprNumber->SetPos(120+48*i, 360);
			m_pSprNumber->SetUV(k*24, 0, k*24+24, 24);
			m_pSprNumber->Draw();
			j %= keta;
			keta /= 10;
		}
	}
}

int GameMain::IsNextScene() {
	return next_scene;
}