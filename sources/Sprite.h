#ifndef SPRITE_H
#define SPRITE_H

#include "D3DManager.h"

//��_
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
// �X�v���C�g�C���^�t�F�[�X�B
// DirectX�̃o�[�W�����Ɋ֌W�Ȃ������Œ�`���ꂽ�֐��œ����܂��B
//=============================================================
struct ISprite {
	virtual BOOL Init(LPCSTR filename)=0;								// ������
	virtual void SetPos(float fX, float fY)=0;							// �ʒu�̃Z�b�g
	virtual void SetScale(float fX, float fY)=0;						// �傫���̃Z�b�g
	virtual void SetRot(float fRot)=0;									// ��]�̃Z�b�g
	virtual void SetCenterPosition(BYTE cPos)=0;						// ���S�_�̃Z�b�g
	virtual void SetUV(float fU0, float fV0, float fU1, float fV1)=0;	// UV���W�̃Z�b�g
	virtual void SetColor(BYTE a, BYTE r, BYTE g, BYTE b)=0;			// �F�̃Z�b�g
	virtual HRESULT Draw()=0;											// �`��
};


//=====================================================================
//=====================================================================
// Direct3D9�o�[�W�����̃X�v���C�g
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_9

class CSprite : public ISprite {
	LPDIRECT3DTEXTURE9 m_pTexture;		// �X�v���C�g�p�e�N�X�`��
	int m_iTexWidth, m_iTexHeight;		// �e�N�X�`���T�C�Y
	D3DXVECTOR3 m_vecPos;				// �X�v���C�g�\���ʒu
	D3DXVECTOR2 m_vecScale;				// �g��k��
	float m_fRot;						// ��]�p�x(���W�A���p)
	RECT m_uv;							// �e�N�X�`���؂����`
	BYTE m_cPos;						// ��_�̈ʒuID
	D3DXVECTOR2 m_vecOrigin;			// ���_
	D3DCOLOR m_color;					// �\���J���[

	D3DXMATRIX m_matWorld;				// ���[���h�ϊ��s��
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
// Direct3D10�o�[�W�����̃X�v���C�g
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_10

class CSprite : public ISprite {
	D3DX10_SPRITE m_sprite;				// �X�v���C�g
	int m_iTexWidth, m_iTexHeight;		// �e�N�X�`���T�C�Y
	D3DXVECTOR3 m_vecPos;				// �X�v���C�g�\���ʒu
	D3DXVECTOR2 m_vecScale;				// �g��k��
	float m_fRot;						// ��]�p�x(���W�A���p)
	BYTE m_cPos;						// ��_�̈ʒuID
	D3DXVECTOR2 m_vecOrigin;			// ���_

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
// Direct3D11�o�[�W�����̃X�v���C�g
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_11

class CSprite : public ISprite {
	ID3D11ShaderResourceView *m_pTexture;		// �e�N�X�`���p�e�N�X�`��
	int m_iTexWidth, m_iTexHeight;				// �e�N�X�`���̃T�C�Y
	D3DXVECTOR3 m_vecPos;						// �ʒu
	D3DXVECTOR2 m_vecScale;						// �g��k��
	float m_fRot;								// ��]�p�x
	BYTE m_cPos;								// ���S�_
	D3DXVECTOR2 m_vecOrigin;					// ���_�x�N�g��
	struct SVSCB {
		D3DXMATRIX m_matWorld;					// ���[���h�s��
		D3DXVECTOR4 m_vecUV;					// uv�}�b�v
	} m_vs_constant;
	D3DXCOLOR m_vecColor;						// �F

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