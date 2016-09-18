#pragma once

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class CDIManager {
	LPDIRECTINPUT8 m_pDInput;
	LPDIRECTINPUTDEVICE8 m_pKeyboard;
	LPDIRECTINPUTDEVICE8 *m_ppJoypads;
	UINT m_numJoypads;

	BYTE m_key_option[4];
	BYTE m_button_option[4];

	bool m_key_state_now[4];
	bool m_key_state_prev[4];
	int m_analog[2];
public:
	CDIManager();
	~CDIManager();

	BOOL Init(HWND hWnd);

	void Update();
	bool IsKeyDown(int key);
	bool IsKeyDownNow(int key);
	bool IsKeyUpNow(int key);
	int GetAnalog(int idx);
};

extern CDIManager *g_pDIManager;
