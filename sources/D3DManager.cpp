#include "D3DManager.h"

//=============================================================
// ウィンドウサイズの設定
//=============================================================
void CD3DManager::SetWindowSize(int iWidth, int iHeight) {
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}

//=============================================================
// ウィンドウかフルスクリーンか
//=============================================================
void CD3DManager::SetWindowFlag(BOOL bWindowed) {
	m_bWindowed = bWindowed;
}

//=============================================================
// ウィンドウの幅の取得
//=============================================================
int CD3DManager::GetWindowWidth() {
	return m_iWidth;
}

//=============================================================
// ウィンドウの高さの取得
//=============================================================
int CD3DManager::GetWindowHeight() {
	return m_iHeight;
}

//=====================================================================
//=====================================================================
// Direct3D9バージョンのマネージャー
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_9

//=============================================================
// コンストラクタ
//=============================================================
CD3DManager::CD3DManager() {
	m_bWindowed = TRUE;
	m_iWidth = 0;
	m_iHeight = 0;
	m_pD3D = NULL;
	m_pDevice = NULL;
	m_pSprite = NULL;
}

//=============================================================
// デストラクタ
//=============================================================
CD3DManager::~CD3DManager() {
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pD3D);
}

//=============================================================
// 初期化
//=============================================================
BOOL CD3DManager::Init(HWND hWnd) {
	// Direct3Dの作成
	if(!(m_pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
		MessageBox(NULL, "Direct3Dの生成に失敗", NULL, MB_OK);
		return FALSE;
	}

	// ディスプレイモードの取得
	D3DDISPLAYMODE d3ddm;
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		MessageBox(NULL, "DisplayModeの取得に失敗", NULL, MB_OK);
		return FALSE;
	}

	// デバイスプレゼントパラメータ
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferWidth = m_iWidth;
	d3dpp.BackBufferHeight = m_iHeight;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = m_bWindowed;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// デバイスの作成
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &m_pDevice))) {
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))) {
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))) {
				if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice))) {
					MessageBox(NULL, "Deviceの生成に失敗", NULL, MB_OK);
					return FALSE;
				}
			}
		}
	}

	// スプライトの生成
	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite))) {
		MessageBox(NULL, "Spriteの生成に失敗", NULL, MB_OK);
		return FALSE;
	}

	//アルファブレンドの設定
    m_pDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	return TRUE;
}

//=============================================================
// レンダーターゲットのクリア
//=============================================================
void CD3DManager::Clear() {
	// レンダーターゲットのクリア
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	// シーンの開始
	m_pDevice->BeginScene();
}

//=============================================================
// 画面の更新
//=============================================================
void CD3DManager::Present() {
	// シーンの終了
	m_pDevice->EndScene();
	// スワップチェーンへの通達
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

#else
//=====================================================================
//=====================================================================
// Direct3D10バージョンのマネージャー
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_10

//=============================================================
// コンストラクタ
//=============================================================
CD3DManager::CD3DManager() {
	m_bWindowed = TRUE;
	m_iWidth = 0;
	m_iHeight = 0;
	m_pSwapChain = NULL;
	m_pDevice = NULL;
	m_pRenderTargetView = NULL;
	m_pDepthStencilView = NULL;
	m_pSprite = NULL;
	m_pBlendState = NULL;
}

//=============================================================
// デストラクタ
//=============================================================
CD3DManager::~CD3DManager() {
	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pSwapChain);
}

//=============================================================
// 初期化
//=============================================================
BOOL CD3DManager::Init(HWND hWnd) {

	// スワップチェーンデスク
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_iWidth;
	sd.BufferDesc.Height = m_iHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = m_bWindowed;

	// スワップチェーンとデバイスの生成
	if(FAILED(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_REFERENCE, NULL, 0, D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice))) {
		return FALSE;
	}

	// レンダーターゲットの設定
	ID3D10Texture2D *pBackBuffer;
	if(FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer))) {
		return FALSE;
	}
	if(FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView))) {
		pBackBuffer->Release();
		return FALSE;
	}
	pBackBuffer->Release();

	// 深度ステンシルターゲットの設定
	ID3D10Texture2D *pDepthBuffer;
	D3D10_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Format = DXGI_FORMAT_D16_UNORM;
	td.Usage = D3D10_USAGE_DEFAULT;
	td.Width = m_iWidth;
	td.Height = m_iHeight;
	td.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	td.ArraySize = 1;
	td.MipLevels =1;
	td.SampleDesc.Count =1;
	td.SampleDesc.Quality = 0;
	if(FAILED(m_pDevice->CreateTexture2D(&td, NULL, &pDepthBuffer))) {
		return FALSE;
	}
	if(FAILED(m_pDevice->CreateDepthStencilView(pDepthBuffer, NULL, &m_pDepthStencilView))) {
		pDepthBuffer->Release();
		return FALSE;
	}

	// レンダーターゲットと深度ステンシルのセット
	m_pDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// ビューポートの設定
	D3D10_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = m_iWidth;
	vp.Height = m_iHeight;
	vp.TopLeftX = vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pDevice->RSSetViewports(1, &vp);

	// スプライトの生成
	if(FAILED(D3DX10CreateSprite(m_pDevice, 0, &m_pSprite))) {
		MessageBox(NULL, "Spriteの生成が失敗", NULL, MB_OK);
		return FALSE;
	}
	// スプライトへの正射影行列の設定
	D3DXMATRIX matP;
	D3DXMatrixOrthoLH(&matP, (float)m_iWidth/(float)m_iHeight, 1.0f, 0.0f, 1.0f);
	m_pSprite->SetProjectionTransform(&matP);

	// アルファブレンドステートの設定
	D3D10_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BlendEnable[0] = TRUE;
	bd.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	bd.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	bd.BlendOp = D3D10_BLEND_OP_ADD;
	bd.SrcBlendAlpha = D3D10_BLEND_ONE;
	bd.DestBlendAlpha = D3D10_BLEND_ONE;
	bd.BlendOpAlpha = D3D10_BLEND_OP_MAX;
	bd.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	if(FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendState))) {
		MessageBox(NULL, "ブレンドステートの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	m_pDevice->OMSetBlendState(m_pBlendState, NULL, 0xffffffff);

	return TRUE;
}

//=============================================================
// レンダーターゲットのクリア
//=============================================================
void CD3DManager::Clear() {
	// レンダーターゲットおよび深度ステンシルのクリア
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pDevice->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}

//=============================================================
// 画面の更新
//=============================================================
void CD3DManager::Present() {
	// スワップチェーンへの通達
	m_pSwapChain->Present(0, 0);
}

#else
//=====================================================================
//=====================================================================
// Direct3D11バージョンのマネージャー
//=====================================================================
//=====================================================================
#ifdef DIRECT3D_FEATURE_LEVEL_11

//=============================================================
// コンストラクタ
//=============================================================
CD3DManager::CD3DManager() {
	m_bWindowed = TRUE;
	m_iWidth = m_iHeight = 0;
	m_pSwapChain = NULL;
	m_pDevice = NULL;
	m_pContext = NULL;
	m_pRenderTargetView = NULL;
	m_pDepthStencilView = NULL;
}

//=============================================================
// デストラクタ
//=============================================================
CD3DManager::~CD3DManager() {
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pSwapChain);
}

//=============================================================
// 初期化
//=============================================================
BOOL CD3DManager::Init(HWND hWnd) {
	// スワップチェーンデスク
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_iWidth;
	sd.BufferDesc.Height = m_iHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = m_bWindowed;

	// リクエストするFeatureLevelの設定
	D3D_FEATURE_LEVEL featureLevelRequested[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
	UINT numRequested = 3;

	// スワップチェーンとデバイスの生成
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,  featureLevelRequested, numRequested,
		D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &m_featureLevelSupported, &m_pContext))) {
			if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, 0,  featureLevelRequested, numRequested,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &m_featureLevelSupported, &m_pContext))) {
					MessageBox(NULL, "デバイスの生成に失敗", NULL, MB_OK);
					return FALSE;
			}
	}

	// レンダーターゲットの設定
	ID3D11Texture2D *pBackBuffer;
	if(FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) {
		return FALSE;
	}
	if(FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView))) {
		MessageBox(NULL, "レンダーターゲットビューの生成に失敗", NULL, MB_OK);
		pBackBuffer->Release();
		return FALSE;
	}
	pBackBuffer->Release();

	// 深度ステンシルターゲットの設定
	/*ID3D11Texture2D *pDepthBuffer;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Format = DXGI_FORMAT_D16_UNORM;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.Width = m_iWidth;
	td.Height = m_iHeight;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.ArraySize = 1;
	td.MipLevels =1;
	td.SampleDesc.Count =1;
	td.SampleDesc.Quality = 0;
	if(FAILED(m_pDevice->CreateTexture2D(&td, NULL, &pDepthBuffer))) {
		MessageBox(NULL, "深度ステンシルテクスチャの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	if(FAILED(m_pDevice->CreateDepthStencilView(pDepthBuffer, NULL, &m_pDepthStencilView))) {
		MessageBox(NULL, "深度ステンシルビューの生成に失敗", NULL, MB_OK);
		pDepthBuffer->Release();
		return FALSE;
	}*/

	// レンダーターゲットおよび深度ステンシルのセット
	//m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	m_pContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = (float)m_iWidth;
	vp.Height = (float)m_iHeight;
	vp.TopLeftX = vp.TopLeftY = 0.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pContext->RSSetViewports(1, &vp);

	// ブレンドステートの設定
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if(FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendState))) {
		MessageBox(NULL, "ブレンドステートの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	m_pContext->OMSetBlendState(m_pBlendState, NULL, 0xffffffff);

	// 頂点シェーダーの設定
	ID3D10Blob *pShaderByteCode;
	ID3D10Blob *pShaderDebugCode;
	if(FAILED(D3DX11CompileFromFile("Data/VSSprite11.vsh", NULL, NULL, "main", "vs_4_0",
		NULL, NULL, NULL, &pShaderByteCode, &pShaderDebugCode, NULL))) {
		MessageBox(NULL, "VertexShaderのコンパイルに失敗", NULL, MB_OK);
		MessageBox(NULL, (char *)pShaderDebugCode->GetBufferPointer(), NULL, MB_OK);
		SAFE_RELEASE(pShaderDebugCode);
		return FALSE;
	}
	if(FAILED(m_pDevice->CreateVertexShader(pShaderByteCode->GetBufferPointer(), pShaderByteCode->GetBufferSize(), NULL, &m_pSpriteVS))) {
		MessageBox(NULL, "VertexShaderInterfaceの生成に失敗", NULL, MB_OK);
		SAFE_RELEASE(pShaderByteCode);
		SAFE_RELEASE(pShaderDebugCode);
		return FALSE;
	}
	SAFE_RELEASE(pShaderDebugCode);
	m_pContext->VSSetShader(m_pSpriteVS, NULL, 0);

	// 入力レイアウトの設定
	D3D11_INPUT_ELEMENT_DESC inDesc[] = {
		{"IA_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"IA_INDEX", 0, DXGI_FORMAT_R32_UINT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	if(FAILED(m_pDevice->CreateInputLayout(inDesc, 2,
		pShaderByteCode->GetBufferPointer(), pShaderByteCode->GetBufferSize(), &m_pSpriteInputLayout))) {
		MessageBox(NULL, "InputLayoutの生成に失敗", NULL, MB_OK);
		SAFE_RELEASE(pShaderByteCode);
		return FALSE;
	}
	SAFE_RELEASE(pShaderByteCode);
	m_pContext->IASetInputLayout(m_pSpriteInputLayout);

	// ジオメトリシェーダーの設定
	m_pContext->GSSetShader(NULL, NULL, 0);

	// テッセレーションの設定
	m_pContext->HSSetShader(NULL, NULL, 0);
	m_pContext->DSSetShader(NULL, NULL, 0);

	// ピクセルシェーダーの設定
	if(FAILED(D3DX11CompileFromFile("Data/PSSprite11.psh", NULL, NULL, "main", "ps_4_0",
		NULL, NULL, NULL, &pShaderByteCode, &pShaderDebugCode, NULL))) {
		MessageBox(NULL, "PixelShaderのコンパイルに失敗", NULL, MB_OK);
		MessageBox(NULL, (char *)pShaderDebugCode->GetBufferPointer(), NULL, MB_OK);
		SAFE_RELEASE(pShaderDebugCode);
		return FALSE;
	}
	if(FAILED(m_pDevice->CreatePixelShader(pShaderByteCode->GetBufferPointer(), pShaderByteCode->GetBufferSize(), NULL, &m_pSpritePS))) {
		MessageBox(NULL, "PixelShaderInterfaceの生成に失敗", NULL, MB_OK);
		SAFE_RELEASE(pShaderByteCode);
		SAFE_RELEASE(pShaderDebugCode);
		return FALSE;
	}
	SAFE_RELEASE(pShaderByteCode);
	SAFE_RELEASE(pShaderDebugCode);
	m_pContext->PSSetShader(m_pSpritePS, NULL, 0);

	// 頂点バッファの設定
	D3D11_BUFFER_DESC buDesc;
	buDesc.ByteWidth = (sizeof(D3DXVECTOR3)+sizeof(UINT))*4;
	buDesc.Usage = D3D11_USAGE_DEFAULT;
	buDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buDesc.CPUAccessFlags = 0;
	buDesc.MiscFlags = 0;
	buDesc.StructureByteStride = 0;

	struct MyVertex {
		D3DXVECTOR3 pos;
		UINT idx;
	} vsource[] = {
		{D3DXVECTOR3(-0.5f, -0.5f, 0.0f), 0},
		{D3DXVECTOR3(-0.5f,  0.5f, 0.0f), 1},
		{D3DXVECTOR3( 0.5f, -0.5f, 0.0f), 2},
		{D3DXVECTOR3( 0.5f,  0.5f, 0.0f), 3}
	};

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vsource;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	if(FAILED(m_pDevice->CreateBuffer(&buDesc, &initData, &m_pSpriteInputVertex))) {
		MessageBox(NULL, "頂点バッファの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	UINT stride = sizeof(MyVertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0, 1, &m_pSpriteInputVertex, &stride, &offset);

	// トーポロジーの設定
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 定数バッファの作成
	D3D11_BUFFER_DESC vscb;
	vscb.ByteWidth = sizeof(D3DXMATRIX)+sizeof(D3DXVECTOR4);
	vscb.Usage = D3D11_USAGE_DYNAMIC;
	vscb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vscb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vscb.MiscFlags = 0;
	vscb.StructureByteStride = 0;

	if(FAILED(m_pDevice->CreateBuffer(&vscb, NULL, &m_pSpriteVSConstant))) {
		MessageBox(NULL, "頂点シェーダ定数バッファの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	m_pContext->VSSetConstantBuffers(0, 1, &m_pSpriteVSConstant);

	D3D11_BUFFER_DESC pscb;
	pscb.ByteWidth = sizeof(D3DXCOLOR);
	pscb.Usage = D3D11_USAGE_DYNAMIC;
	pscb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	pscb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pscb.MiscFlags = 0;
	pscb.StructureByteStride = 0;

	if(FAILED(m_pDevice->CreateBuffer(&pscb, NULL, &m_pSpritePSConstant))) {
		MessageBox(NULL, "ピクセルシェーダ定数バッファの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	m_pContext->PSSetConstantBuffers(0, 1, &m_pSpritePSConstant);

	return TRUE;
}

//=============================================================
// レンダーターゲットのクリア
//=============================================================
void CD3DManager::Clear() {
	// レンダーターゲットのクリア
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_pContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//=============================================================
// 画面の更新
//=============================================================
void CD3DManager::Present() {
	// スワップチェーンへの通達
	m_pSwapChain->Present(0, 0);
}

#endif

#endif

#endif

CD3DManager *g_pD3DManager = NULL;