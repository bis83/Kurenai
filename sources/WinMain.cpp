
#include <windows.h>

#include <string.h>
#include <stdio.h>

#include "D3DManager.h"
#include "Sprite.h"

#include "DIManager.h"
#include "DSManager.h"

#include "GameTitle.h"
#include "GameMain.h"

//=============================================================
// ウィンドウプロシージャ
//=============================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

//=============================================================
// エントリーポイント(メイン関数)
//=============================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpCmdLine, int nShowCmd) {

	//=========================================================
	// 定数宣言
	//=========================================================
	LPCSTR wndClsName = "Sample2010";	// ウィンドウクラス名
	LPCSTR wndText = "Kurenai";	// ウィンドウバーに表示されるテキスト
	const int iClientWidth = 640;		// ウィンドウの横サイズ
	const int iClientHeight = 480;		// ウィンドウの縦サイズ
	const BOOL bWindowed = TRUE;		// TRUE = ウィンドウモード , FALSE = フルスクリーン

	// ウィンドウハンドル
	HWND hWnd;

	//=========================================================
	// ウィンドウクラスの定義
	//=========================================================
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpszClassName = wndClsName;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//=========================================================
	// ウィンドウクラスの登録
	//=========================================================
	if(RegisterClassEx(&wcex) == NULL) {
		return 0;
	}

	//=========================================================
	// ウィンドウの生成
	//=========================================================
	int iWindowWidth = iClientWidth + GetSystemMetrics( SM_CXFRAME );
	int iWindowHeight = iClientHeight + GetSystemMetrics( SM_CYFRAME ) + GetSystemMetrics( SM_CYCAPTION );
	hWnd = CreateWindow(
		wndClsName,
		wndText,
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		iWindowWidth,
		iWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	ShowWindow(hWnd, nShowCmd);

	//=========================================================
	// Managerの初期化
	//=========================================================
	g_pD3DManager = new CD3DManager();
	g_pD3DManager->SetWindowSize(iClientWidth, iClientHeight);
	g_pD3DManager->SetWindowFlag(bWindowed);
	if(!g_pD3DManager->Init(hWnd)) {
		return 0;
	}

	g_pDIManager = new CDIManager();
	if(!g_pDIManager->Init(hWnd)) {
		return 0;
	}

	g_pDSManager = new CDSManager();
	if(!g_pDSManager->Init(hWnd)) {
		return 0;
	}

	// ゲームデータの初期化

	GameTitle *pTitle = new GameTitle();
	GameMain *pMain = new GameMain();
	int scene = 0;

	// FPS関連のリソース

	DWORD nowTime = timeGetTime();
	DWORD beforeTime = nowTime;
	DWORD fps = 60;
	DWORD frame = 0;
	DWORD idealTime = 1000 / fps;
	char text[128];

	//=========================================================
	// メインループ
	//=========================================================
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// フレーム処理
			frame++;
			nowTime = timeGetTime();
			if( (nowTime - beforeTime) < frame * idealTime ) {
				Sleep( frame*idealTime - (nowTime - beforeTime) );
			}
			if( (nowTime - beforeTime) > 1000) {
				sprintf(text, "Kurenai fps=%d", frame);
				SetWindowText(hWnd, text);
				frame = 0;
				beforeTime = nowTime;
			}

			// シーンの更新
			g_pDIManager->Update();
			switch(scene) {
				case 0:
					pTitle->Update();
					break;
				case 1:
					pMain->Update();
					break;
			}

			// シーンの描画
			g_pD3DManager->Clear();
			switch(scene) {
				case 0:
					pTitle->Draw();
					break;
				case 1:
					pMain->Draw();
					break;
			}
			g_pD3DManager->Present();

			// シーンの切り替え
			switch(scene) {
				case 0:
					if(pTitle->IsNextScene() != -1) {
						scene = pTitle->IsNextScene();
						pMain->Init();
					}
					break;
				case 1:
					if(pMain->IsNextScene() != -1) {
						scene = pMain->IsNextScene();
						pTitle->Init();
					}
					break;
			}
		}
	}

	//=========================================================
	// 解放
	//=========================================================

	delete pTitle;

	delete g_pD3DManager;
	delete g_pDIManager;
	delete g_pDSManager;

	return 0;
}