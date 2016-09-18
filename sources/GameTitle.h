#pragma once

#include "Sprite.h"
#include "DSManager.h"

// ゲームタイトル
class GameTitle {
	ISprite *m_pSprTitleBack;
	ISprite *m_pSprTitleLogo;

	ISprite *m_pSprNumber;

	CSound *m_pSeStart;

	int next_scene;
	int t;
public:
	GameTitle();
	~GameTitle();

	void Init();

	void Update();

	void Draw();

	int IsNextScene();
};
