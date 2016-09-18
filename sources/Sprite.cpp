#include "Sprite.h"

//=====================================================================
//=====================================================================
// Direct3D9バージョンのスプライト
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_9

//=============================================================
// コンストラクタ
//=============================================================
CSprite::CSprite() {
	m_pTexture = NULL;
	m_vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecScale = D3DXVECTOR2(1.0f, 1.0f);
	m_fRot = 0.0f;
	m_uv.top = m_uv.left = m_uv.bottom = m_uv.right = 0;
	m_cPos = CP_TOP | CP_LEFT;
	m_vecOrigin = D3DXVECTOR2(0.0f, 0.0f);
	m_color = D3DCOLOR_ARGB(255, 255, 255, 255);
	UpdateMatrix();
}

//=============================================================
// デストラクタ
//=============================================================
CSprite::~CSprite() {
	SAFE_RELEASE(m_pTexture);
}

//=============================================================
// 初期化
//=============================================================
BOOL CSprite::Init(LPCSTR filename) {
	SAFE_RELEASE(m_pTexture);

	if(FAILED(D3DXCreateTextureFromFileEx(g_pD3DManager->GetDevice(),
		filename, 0, 0, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR, D3DCOLOR_RGBA(0, 255, 255, 255),
		NULL, NULL, &m_pTexture))) {
			return FALSE;
	}

	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc(0, &desc);
	m_iTexWidth = desc.Width;
	m_iTexHeight = desc.Height;

	SetCenterPosition(m_cPos);

	return TRUE;
}

//=============================================================
// 行列の更新
//=============================================================
void CSprite::UpdateMatrix() {
	D3DXMATRIX matTemp;

	// 原点を重ね合わせる平行移動
	D3DXMatrixTranslation(&m_matWorld, -m_vecOrigin.x, -m_vecOrigin.y, 0.0f);

	// 拡大行列と合成
	if(m_vecScale.x != 1.0f || m_vecScale.y != 1.0f){
		D3DXMatrixScaling(&matTemp, m_vecScale.x, m_vecScale.y, 1.0f);
		D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matTemp);
	}

	// 回転行列との合成
	if(m_fRot != 0.0f){
		D3DXMatrixRotationZ(&matTemp, m_fRot);
		D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matTemp);
	}

	// 平行移動行列との合成
	if(m_vecPos.x != 0.0f || m_vecPos.y != 0.0f || m_vecPos.z != 0.0f) {
		D3DXMatrixTranslation(&matTemp, m_vecPos.x, m_vecPos.y, m_vecPos.z);
		D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matTemp);
	}
}

//=============================================================
// 描画処理
//=============================================================
HRESULT CSprite::Draw() {
	HRESULT hr;

	UpdateMatrix();
	
	// スプライトの開始
	g_pD3DManager->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	// スプライトへの行列の設定
	g_pD3DManager->GetSprite()->SetTransform(&m_matWorld);
	
	// 描画
	if(m_uv.bottom || m_uv.left || m_uv.right || m_uv.top) {
		hr = g_pD3DManager->GetSprite()->Draw(m_pTexture, &m_uv, NULL, NULL, m_color);
	} else {
		hr = g_pD3DManager->GetSprite()->Draw(m_pTexture, NULL, NULL, NULL, m_color);
	}
	// スプライトの終了
	g_pD3DManager->GetSprite()->End();
	
	return hr;
}

//=============================================================
// 位置のセット
//=============================================================
void CSprite::SetPos(float fX, float fY) {
	m_vecPos.x = fX;
	m_vecPos.y = fY;
}

//=============================================================
// 拡大縮小のセット
//=============================================================
void CSprite::SetScale(float fX, float fY) {
	m_vecScale.x = fX;
	m_vecScale.y = fY;
}

//=============================================================
// 回転のセット
//=============================================================
void CSprite::SetRot(float fRot) {
	m_fRot = fRot;
}

//=============================================================
// 描画色のセット
//=============================================================
void CSprite::SetColor(BYTE a, BYTE r, BYTE g, BYTE b) {
	m_color = D3DCOLOR_ARGB(a, r, g, b);
}

//=============================================================
// 中心点のセット
//=============================================================
void CSprite::SetCenterPosition(BYTE cPos) {
	m_cPos = cPos;

	float swidth;
	float sheight;

	// 描画範囲を計算
	if(m_uv.bottom || m_uv.left || m_uv.right || m_uv.top) {
		swidth = m_uv.right - m_uv.left;
		sheight = m_uv.bottom - m_uv.top;
	} else {
		swidth = (float)m_iTexWidth;
		sheight = (float)m_iTexHeight;
	}

	// 原点の位置を計算
	if(m_cPos & CP_LEFT) {
		m_vecOrigin.x = 0.0f;
	} else if(m_cPos & CP_RIGHT) {
		m_vecOrigin.x = (float)swidth;
	} else {
		m_vecOrigin.x = (float)swidth / 2.0f;
	}

	if(m_cPos & CP_TOP) {
		m_vecOrigin.y = 0.0f;
	} else if(m_cPos & CP_BOTTOM) {
		m_vecOrigin.y = (float)sheight;
	} else {
		m_vecOrigin.y = (float)sheight / 2.0f;
	}
}

//=============================================================
// UV座標のセット
//=============================================================
void CSprite::SetUV(float fU0, float fV0, float fU1, float fV1) {
	m_uv.top = fU0;
	m_uv.left = fV0;
	m_uv.bottom = fU1;
	m_uv.right = fV1;

	SetCenterPosition(m_cPos);
}

#else
//=====================================================================
//=====================================================================
// Direct3D10バージョンのスプライト
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_10

//=============================================================
// コンストラクタ
//=============================================================
CSprite::CSprite() {
	m_sprite.pTexture = NULL;
	m_sprite.TextureIndex = 0;
	D3DXMatrixIdentity(&m_sprite.matWorld);
	m_sprite.TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	m_sprite.TexSize = D3DXVECTOR2(1.0f, 1.0f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	m_sprite.ColorModulate = color;

	m_vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecScale = D3DXVECTOR2(1.0f, 1.0f);
	m_fRot = 0.0f;
	m_cPos = CP_TOP | CP_LEFT;

	SetCenterPosition(m_cPos);
}

//=============================================================
// デストラクタ
//=============================================================
CSprite::~CSprite() {
	SAFE_RELEASE(m_sprite.pTexture);
}

//=============================================================
// 初期化
//=============================================================
BOOL CSprite::Init(LPCSTR filename) {
	// テクスチャ情報の取得
	D3DX10_IMAGE_INFO ii;
	if(FAILED(D3DX10GetImageInfoFromFile(filename, NULL, &ii, NULL))) {
		MessageBox(NULL, "ImageInfoの取得に失敗", NULL, MB_OK);
		return FALSE;
	}
	m_iTexWidth = ii.Width;
	m_iTexHeight = ii.Height;
	m_sprite.TexCoord.x = 0.0f;
	m_sprite.TexCoord.y = 0.0f;
	m_sprite.TexSize.x = 1.0f;
	m_sprite.TexSize.y = 1.0f;

	// テクスチャの生成
	ID3D10Texture2D *pTextureSource;
	if(FAILED(D3DX10CreateTextureFromFile(g_pD3DManager->GetDevice(), filename, NULL, NULL, (ID3D10Resource**)&pTextureSource, NULL))) {
		MessageBox(NULL, "Textureの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	if(FAILED(g_pD3DManager->GetDevice()->CreateShaderResourceView(pTextureSource, NULL, &m_sprite.pTexture))) {
		MessageBox(NULL, "ShaderResourceViewの生成に失敗", NULL, MB_OK);
		pTextureSource->Release();
		return FALSE;
	}

	SetCenterPosition(m_cPos);

	return TRUE;
}

//=============================================================
// 描画
//=============================================================
HRESULT CSprite::Draw() {
	UpdateMatrix();
	g_pD3DManager->GetSprite()->Begin(NULL);
	g_pD3DManager->GetSprite()->DrawSpritesImmediate(&m_sprite, 1, 0, 0);
	g_pD3DManager->GetSprite()->End();
	return S_OK;
}

//=============================================================
// 位置のセット
//=============================================================
void CSprite::SetPos(float fX, float fY) {
	m_vecPos.x = fX;
	m_vecPos.y = fY;
}

//=============================================================
// 回転のセット
//=============================================================
void CSprite::SetRot(float fRot) {
	m_fRot = fRot;
}

//=============================================================
// 拡大縮小のセット
//=============================================================
void CSprite::SetScale(float fX, float fY) {
	m_vecScale.x = fX;
	m_vecScale.y = fY;
}

//=============================================================
// UV座標のセット
//=============================================================
void CSprite::SetUV(float fU0, float fV0, float fU1, float fV1) {
	m_sprite.TexCoord.x = fU0/m_iTexWidth;
	m_sprite.TexCoord.y = fV0/m_iTexHeight;
	m_sprite.TexSize.x = fU1/m_iTexWidth - m_sprite.TexCoord.x;
	m_sprite.TexSize.y = fV1/m_iTexHeight - m_sprite.TexCoord.y;

	SetCenterPosition(m_cPos);
}

//=============================================================
// 中心点のセット
//=============================================================
void CSprite::SetCenterPosition(BYTE cPos) {
	m_cPos = cPos;

	// 原点の位置を計算
	if(m_cPos & CP_LEFT) {
		m_vecOrigin.x = 0.5f;
	} else if(m_cPos & CP_RIGHT) {
		m_vecOrigin.x = -0.5f;
	} else {
		m_vecOrigin.x = 0.0f;
	}

	if(m_cPos & CP_TOP) {
		m_vecOrigin.y = -0.5f;
	} else if(m_cPos & CP_BOTTOM) {
		m_vecOrigin.y = 0.5f;
	} else {
		m_vecOrigin.y = 0.0f;
	}
}

//=============================================================
// 描画色のセット
//=============================================================
void CSprite::SetColor(BYTE a, BYTE r, BYTE g, BYTE b) {
	D3DXCOLOR color;
	color.a = 1.0f * a / 255;
	color.r = 1.0f * r / 255;
	color.g = 1.0f * g / 255;
	color.b = 1.0f * b / 255;
	m_sprite.ColorModulate = color;
}

//=============================================================
// 行列の更新
//=============================================================
void CSprite::UpdateMatrix() {
	D3DXMATRIX matTemp;

	// 原点を重ね合わせる平行移動
	D3DXMatrixTranslation(&m_sprite.matWorld, m_vecOrigin.x, m_vecOrigin.y, 0.0f);

	// 拡大行列と合成
	D3DXMatrixScaling(&matTemp,
		m_vecScale.x*((float)m_iTexWidth/(float)g_pD3DManager->GetWindowHeight())*(m_sprite.TexSize.x),
		m_vecScale.y*((float)m_iTexHeight/(float)g_pD3DManager->GetWindowHeight())*(m_sprite.TexSize.y),
		1.0f);
	D3DXMatrixMultiply(&m_sprite.matWorld, &m_sprite.matWorld, &matTemp);

	// 回転行列との合成
	if(m_fRot != 0.0f){
		D3DXMatrixRotationZ(&matTemp, -m_fRot);
		D3DXMatrixMultiply(&m_sprite.matWorld, &m_sprite.matWorld, &matTemp);
	}

	// 平行移動行列との合成
	D3DXMatrixTranslation(&matTemp,
		(m_vecPos.x/g_pD3DManager->GetWindowWidth()-0.5f)*((float)g_pD3DManager->GetWindowWidth()/(float)g_pD3DManager->GetWindowHeight()),
		-m_vecPos.y/g_pD3DManager->GetWindowHeight()+0.5f,
		m_vecPos.z);
	D3DXMatrixMultiply(&m_sprite.matWorld, &m_sprite.matWorld, &matTemp);
}

#else
//=====================================================================
//=====================================================================
// Direct3D11バージョンのスプライト
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_11

//=============================================================
// コンストラクタ
//=============================================================
CSprite::CSprite() {
	m_pTexture = NULL;
	D3DXMatrixIdentity(&m_vs_constant.m_matWorld);
	m_vs_constant.m_vecUV = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	m_vecColor = color;

	m_vecPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecScale = D3DXVECTOR2(1.0f, 1.0f);
	m_fRot = 0.0f;
	m_cPos = CP_TOP | CP_LEFT;

	SetCenterPosition(m_cPos);
}

//=============================================================
// デストラクタ
//=============================================================
CSprite::~CSprite() {
	SAFE_RELEASE(m_pTexture);
}

//=============================================================
// 初期化
//=============================================================
BOOL CSprite::Init(LPCSTR filename) {
	// テクスチャ情報の取得
	D3DX11_IMAGE_INFO ii;
	if(FAILED(D3DX11GetImageInfoFromFile(filename, NULL, &ii, NULL))) {
		MessageBox(NULL, "ImageInfoの取得に失敗", NULL, MB_OK);
		return FALSE;
	}
	m_iTexWidth = ii.Width;
	m_iTexHeight = ii.Height;

	// テクスチャの生成
	ID3D11Texture2D *pTextureSource;
	if(FAILED(D3DX11CreateTextureFromFile(g_pD3DManager->GetDevice(), filename, NULL, NULL, (ID3D11Resource**)&pTextureSource, NULL))) {
		MessageBox(NULL, "Textureの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	if(FAILED(g_pD3DManager->GetDevice()->CreateShaderResourceView(pTextureSource, NULL, &m_pTexture))) {
		MessageBox(NULL, "ShaderResourceViewの生成に失敗", NULL, MB_OK);
		pTextureSource->Release();
		return FALSE;
	}

	SetCenterPosition(m_cPos);
	return TRUE;
}

//=============================================================
// 位置のセット
//=============================================================
void CSprite::SetPos(float fX, float fY) {
	m_vecPos.x = fX;
	m_vecPos.y = fY;
}

//=============================================================
// 拡大縮小のセット
//=============================================================
void CSprite::SetScale(float fX, float fY) {
	m_vecScale.x = fX;
	m_vecScale.y = fY;
}

//=============================================================
// 回転のセット
//=============================================================
void CSprite::SetRot(float fRot) {
	m_fRot = fRot;
}

//=============================================================
// UV座標のセット
//=============================================================
void CSprite::SetUV(float fU0, float fV0, float fU1, float fV1) {
	m_vs_constant.m_vecUV.x = fU0/m_iTexWidth;
	m_vs_constant.m_vecUV.y = fV0/m_iTexHeight;
	m_vs_constant.m_vecUV.z = fU1/m_iTexWidth;
	m_vs_constant.m_vecUV.w = fV1/m_iTexHeight;
	
	SetCenterPosition(m_cPos);
}

//=============================================================
// 中心点のセット
//=============================================================
void CSprite::SetCenterPosition(BYTE cPos) {
	m_cPos = cPos;

	// 原点の位置を計算
	if(m_cPos & CP_LEFT) {
		m_vecOrigin.x = 0.5f;
	} else if(m_cPos & CP_RIGHT) {
		m_vecOrigin.x = -0.5f;
	} else {
		m_vecOrigin.x = 0.0f;
	}

	if(m_cPos & CP_TOP) {
		m_vecOrigin.y = -0.5f;
	} else if(m_cPos & CP_BOTTOM) {
		m_vecOrigin.y = 0.5f;
	} else {
		m_vecOrigin.y = 0.0f;
	}
}

//=============================================================
// 描画色のセット
//=============================================================
void CSprite::SetColor(BYTE a, BYTE r, BYTE g, BYTE b) {
	D3DXCOLOR color;
	color.a = 1.0f * a / 255;
	color.r = 1.0f * r / 255;
	color.g = 1.0f * g / 255;
	color.b = 1.0f * b / 255;
	m_vecColor = color;
}

//=============================================================
// 行列の更新
//=============================================================
void CSprite::UpdateMatrix() {
	D3DXMATRIX matTemp;

	D3DXMatrixIdentity(&m_vs_constant.m_matWorld);
	
	// 原点を重ね合わせる平行移動
	D3DXMatrixTranslation(&matTemp, m_vecOrigin.x, m_vecOrigin.y, 0.0f);
	D3DXMatrixMultiply(&m_vs_constant.m_matWorld, &m_vs_constant.m_matWorld, &matTemp);
	
	// 拡大行列と合成
	D3DXMatrixScaling(&matTemp,
		m_vecScale.x*((float)m_iTexWidth/(float)g_pD3DManager->GetWindowHeight())*(m_vs_constant.m_vecUV.z-m_vs_constant.m_vecUV.x),
		m_vecScale.y*((float)m_iTexHeight/(float)g_pD3DManager->GetWindowHeight())*(m_vs_constant.m_vecUV.w-m_vs_constant.m_vecUV.y),
		1.0f);
	D3DXMatrixMultiply(&m_vs_constant.m_matWorld, &m_vs_constant.m_matWorld, &matTemp);

	// 回転行列との合成
	if(m_fRot != 0.0f){
		D3DXMatrixRotationZ(&matTemp, m_fRot);
		D3DXMatrixMultiply(&m_vs_constant.m_matWorld, &m_vs_constant.m_matWorld, &matTemp);
	}

	// 平行移動行列との合成
	D3DXMatrixTranslation(&matTemp,
		(m_vecPos.x/g_pD3DManager->GetWindowWidth()-0.5f)*((float)g_pD3DManager->GetWindowWidth()/(float)g_pD3DManager->GetWindowHeight()),
		-m_vecPos.y/g_pD3DManager->GetWindowHeight()+0.5f,
		m_vecPos.z);
	D3DXMatrixMultiply(&m_vs_constant.m_matWorld, &m_vs_constant.m_matWorld, &matTemp);

	// 正射影行列との合成
	D3DXMatrixOrthoLH(&matTemp, (float)g_pD3DManager->GetWindowWidth()/(float)g_pD3DManager->GetWindowHeight(), 1.0f, 0.0f, 1.0f);
	D3DXMatrixMultiply(&m_vs_constant.m_matWorld, &m_vs_constant.m_matWorld, &matTemp);
}

//=============================================================
// 描画処理
//=============================================================
HRESULT CSprite::Draw() {
	UpdateMatrix();

	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE sub = {0};

	if(SUCCEEDED(hr = g_pD3DManager->GetContext()->Map(g_pD3DManager->GetSpriteVSConstant(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub))) {
		memcpy(sub.pData, &m_vs_constant, sizeof(SVSCB));
		g_pD3DManager->GetContext()->Unmap(g_pD3DManager->GetSpriteVSConstant(), 0);
	}

	if(SUCCEEDED(hr = g_pD3DManager->GetContext()->Map(g_pD3DManager->GetSpritePSConstant(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &sub))) {
		memcpy(sub.pData, &m_vecColor, sizeof(D3DXCOLOR));
		g_pD3DManager->GetContext()->Unmap(g_pD3DManager->GetSpritePSConstant(), 0);
	}

	g_pD3DManager->GetContext()->PSSetShaderResources(0, 1, &m_pTexture);

	g_pD3DManager->GetContext()->Draw(4, 0);
	return S_OK;
}

#endif

#endif

#endif
