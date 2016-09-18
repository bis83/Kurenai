
#ifndef D3DMANAGER_H
#define D3DMANAGER_H

//=============================================================
// Direct3DFeatureLevel
//
// DIRECT3D_FEATURE_LEVEL_9
//		Direct3D9�o�[�W�����B�Ώ�OS - XP���炢
//		�ΏۃO���t�B�b�N�{�[�h - ��������
//
// DIRECT3D_FEATURE_LEVEL_10
//		Direct3D10�o�[�W�����B�Ώ�OS - Vista�ȏ�
//		�ΏۃO���t�B�b�N�{�[�h - �ŋ߂�
//
// DIRECT3D_FEATURE_LEVEL_11
//		Direct3D11�o�[�W�����B�Ώ�OS - 7�ȏ�
//		�ΏۃO���t�B�b�N�{�[�h - ����
//
//=============================================================

#define DIRECT3D_FEATURE_LEVEL_11

//=============================================================
//=============================================================

#define SAFE_RELEASE(p) { if(p!=NULL) { p->Release(); p = NULL; } }

//=====================================================================
//=====================================================================
// Direct3D9�o�[�W�����̃}�l�[�W���[
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_9

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

class CD3DManager {
public:
	CD3DManager();
	~CD3DManager();

	BOOL Init(HWND hWnd);							// ������
	void SetWindowSize(int iWidth, int iHeight);	// �E�B���h�E�T�C�Y�̃Z�b�g
	void SetWindowFlag(BOOL bWindowed);				// �E�B���h�E���t���X�N���[����
	int GetWindowWidth();							// �E�B���h�E�̕��̎擾
	int GetWindowHeight();							// �E�B���h�E�̍����̎擾

	void Clear();		// �����_�[�^�[�Q�b�g�̃N���A
	void Present();		// ��ʂ̍X�V

	LPDIRECT3D9 GetDirect3D() { return m_pD3D; }
	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice; }

	LPD3DXSPRITE GetSprite() { return m_pSprite; }
private:
	BOOL m_bWindowed;
	int m_iWidth, m_iHeight;

	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pDevice;

	LPD3DXSPRITE m_pSprite;
};

#else

//=====================================================================
//=====================================================================
// Direct3D10�o�[�W�����̃}�l�[�W���[
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_10

#include <d3d10.h>
#include <d3dx10.h>

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")

class CD3DManager {
public:
	CD3DManager();
	~CD3DManager();

	BOOL Init(HWND hWnd);							// ������
	void SetWindowSize(int iWidth, int iHeight);	// �E�B���h�E�T�C�Y�̐ݒ�
	void SetWindowFlag(BOOL bWindowed);				// �E�B���h�E���t���X�N���[����
	int GetWindowWidth();							// �E�B���h�E�̕��̎擾
	int GetWindowHeight();							// �E�B���h�E�̍����̎擾

	void Clear();		// �����_�[�^�[�Q�b�g�̃N���A
	void Present();		// ��ʂ̍X�V

	IDXGISwapChain *GetSwapChain() { return m_pSwapChain; }
	ID3D10Device *GetDevice() { return m_pDevice; }
	ID3D10RenderTargetView *GetRenderTargetView() { return m_pRenderTargetView; }
	ID3D10DepthStencilView *GetDepthStencilView() { return m_pDepthStencilView; }

	ID3DX10Sprite *GetSprite() { return m_pSprite; }
private:
	BOOL m_bWindowed;
	int m_iWidth, m_iHeight;

	IDXGISwapChain *m_pSwapChain;
	ID3D10Device *m_pDevice;
	
	ID3D10RenderTargetView *m_pRenderTargetView;
	ID3D10DepthStencilView *m_pDepthStencilView;

	ID3DX10Sprite *m_pSprite;

	ID3D10BlendState *m_pBlendState;
};

#else

//=====================================================================
//=====================================================================
// Direct3D11�o�[�W�����̃}�l�[�W���[
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_11

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

class CD3DManager {
public:
	CD3DManager();
	~CD3DManager();

	BOOL Init(HWND hWnd);							// ������
	void SetWindowSize(int iWidth, int iHeight);	// �E�B���h�E�T�C�Y�̐ݒ�
	void SetWindowFlag(BOOL bWindowed);				// �E�B���h�E���t���X�N���[����
	int GetWindowWidth();							// �E�B���h�E�̕��̎擾
	int GetWindowHeight();							// �E�B���h�E�̍����̎擾

	void Clear();		// �����_�[�^�[�Q�b�g�̃N���A
	void Present();		// ��ʂ̍X�V

	D3D_FEATURE_LEVEL GetFeatureLevel() { return m_featureLevelSupported; }
	IDXGISwapChain *GetSwapChain() { return m_pSwapChain; }
	ID3D11Device *GetDevice() { return m_pDevice; }
	ID3D11DeviceContext *GetContext() { return m_pContext; }
	ID3D11RenderTargetView *GetRenderTargetView() { return m_pRenderTargetView; }
	ID3D11DepthStencilView *GetDepthStencilView() { return m_pDepthStencilView; }
	ID3D11BlendState *GetBlendState() { return m_pBlendState; }
	ID3D11VertexShader *GetSpriteVS() { return m_pSpriteVS; }
	ID3D11Buffer *GetSpriteVSConstant() { return m_pSpriteVSConstant; }
	ID3D11PixelShader *GetSpritePS() { return m_pSpritePS; }
	ID3D11Buffer *GetSpritePSConstant() { return m_pSpritePSConstant; }
	ID3D11InputLayout *GetSpriteInputLayout() { return m_pSpriteInputLayout; }
	ID3D11Buffer *GetSpriteInputVertex() { return m_pSpriteInputVertex; }
private:
	BOOL m_bWindowed;
	int m_iWidth, m_iHeight;

	D3D_FEATURE_LEVEL m_featureLevelSupported;
	IDXGISwapChain *m_pSwapChain;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pContext;

	ID3D11RenderTargetView *m_pRenderTargetView;
	ID3D11DepthStencilView *m_pDepthStencilView;

	ID3D11BlendState *m_pBlendState;

	// �X�v���C�g�@�\�p�e��V�F�[�_�[�y�ѓ��͏W���C���^�t�F�[�X
	ID3D11InputLayout *m_pSpriteInputLayout;
	ID3D11Buffer *m_pSpriteInputVertex;
	ID3D11VertexShader *m_pSpriteVS;
	ID3D11Buffer *m_pSpriteVSConstant;
	ID3D11PixelShader *m_pSpritePS;
	ID3D11Buffer *m_pSpritePSConstant;
};

#endif

#endif

#endif

// �O���[�o���̃}�l�[�W���[
extern CD3DManager *g_pD3DManager;

#endif
