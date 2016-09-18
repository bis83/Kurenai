#ifndef SPRITE_H
#define SPRITE_H

#include "D3DManager.h"

//基点
#define	CP_MIDDLECENTER 0
#define	CP_LEFT 1
#define	CP_CENTER 2
#define	CP_RIGHT 4
#define	CP_TOP 8
#define	CP_MIDDLE 16
#define	CP_BOTTOM 32

//=============================================================
// ISprite
//-------------------------------------------------------------
// スプライトインタフェース。
// DirectXのバージョンに関係なくここで定義された関数で動きます。
//=============================================================
struct ISprite {
	virtual BOOL Init(LPCSTR filename)=0;								// 初期化
	virtual void SetPos(float fX, float fY)=0;							// 位置のセット
	virtual void SetScale(float fX, float fY)=0;						// 大きさのセット
	virtual void SetRot(float fRot)=0;									// 回転のセット
	virtual void SetCenterPosition(BYTE cPos)=0;						// 中心点のセット
	virtual void SetUV(float fU0, float fV0, float fU1, float fV1)=0;	// UV座標のセット
	virtual void SetColor(BYTE a, BYTE r, BYTE g, BYTE b)=0;			// 色のセット
	virtual HRESULT Draw()=0;											// 描画
};


//=====================================================================
//=====================================================================
// Direct3D9バージョンのスプライト
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_9

class CSprite : public ISprite {
	LPDIRECT3DTEXTURE9 m_pTexture;		// スプライト用テクスチャ
	int m_iTexWidth, m_iTexHeight;		// テクスチャサイズ
	D3DXVECTOR3 m_vecPos;				// スプライト表示位置
	D3DXVECTOR2 m_vecScale;				// 拡大縮小
	float m_fRot;						// 回転角度(ラジアン角)
	RECT m_uv;							// テクスチャ切り取り矩形
	BYTE m_cPos;						// 基点の位置ID
	D3DXVECTOR2 m_vecOrigin;			// 原点
	D3DCOLOR m_color;					// 表示カラー

	D3DXMATRIX m_matWorld;				// ワールド変換行列
	void UpdateMatrix();
public:
	CSprite();
	~CSprite();

	BOOL Init(LPCSTR filename);
	void SetPos(float fX, float fY);
	void SetScale(float fX, float fY);
	void SetRot(float fRot);
	void SetCenterPosition(BYTE cPos);
	void SetUV(float fU0, float fV0, float fU1, float fV1);
	void SetColor(BYTE a, BYTE r, BYTE g, BYTE b);
	HRESULT Draw();
};

#else

//=====================================================================
//=====================================================================
// Direct3D10バージョンのスプライト
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_10

class CSprite : public ISprite {
	D3DX10_SPRITE m_sprite;				// スプライト
	int m_iTexWidth, m_iTexHeight;		// テクスチャサイズ
	D3DXVECTOR3 m_vecPos;				// スプライト表示位置
	D3DXVECTOR2 m_vecScale;				// 拡大縮小
	float m_fRot;						// 回転角度(ラジアン角)
	BYTE m_cPos;						// 基点の位置ID
	D3DXVECTOR2 m_vecOrigin;			// 原点

	void UpdateMatrix();
public:
	CSprite();
	~CSprite();

	BOOL Init(LPCSTR filename);
	void SetPos(float fX, float fY);
	void SetScale(float fX, float fY);
	void SetRot(float fRot);
	void SetCenterPosition(BYTE cPos);
	void SetUV(float fU0, float fV0, float fU1, float fV1);
	void SetColor(BYTE a, BYTE r, BYTE g, BYTE b);
	HRESULT Draw();
};

#else

//=====================================================================
//=====================================================================
// Direct3D11バージョンのスプライト
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_11

class CSprite : public ISprite {
	ID3D11ShaderResourceView *m_pTexture;		// テクスチャ用テクスチャ
	int m_iTexWidth, m_iTexHeight;				// テクスチャのサイズ
	D3DXVECTOR3 m_vecPos;						// 位置
	D3DXVECTOR2 m_vecScale;						// 拡大縮小
	float m_fRot;								// 回転角度
	BYTE m_cPos;								// 中心点
	D3DXVECTOR2 m_vecOrigin;					// 原点ベクトル
	struct SVSCB {
		D3DXMATRIX m_matWorld;					// ワールド行列
		D3DXVECTOR4 m_vecUV;					// uvマップ
	} m_vs_constant;
	D3DXCOLOR m_vecColor;						// 色

	void UpdateMatrix();
public:
	CSprite();
	~CSprite();

	BOOL Init(LPCSTR filename);
	void SetPos(float fX, float fY);
	void SetScale(float fX, float fY);
	void SetRot(float fRot);
	void SetCenterPosition(BYTE cPos);
	void SetUV(float fU0, float fV0, float fU1, float fV1);
	void SetColor(BYTE a, BYTE r, BYTE g, BYTE b);
	HRESULT Draw();
};

#endif

#endif

#endif

#endif