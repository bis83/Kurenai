#include "DIManager.h"

#include <vector>
using namespace std;

CDIManager *g_pDIManager = NULL;

BOOL CALLBACK EnumJoypadsGuid(const DIDEVICEINSTANCE *pdid, VOID *p) {
	vector<GUID> *vec = (vector<GUID>*)p;
	vec->push_back(pdid->guidInstance);
	return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumJoypadsProperty(const DIDEVICEOBJECTINSTANCE *pdid, VOID *p) {
	LPDIRECTINPUTDEVICE8 pDevice = (LPDIRECTINPUTDEVICE8)p;
	if (pdid->dwType & DIDFT_AXIS) {
		DIPROPRANGE dip;
		dip.diph.dwSize = sizeof(DIPROPRANGE);
		dip.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dip.diph.dwHow = DIPH_BYID;
		dip.diph.dwObj = pdid->dwType;
		dip.lMin= -1000;
		dip.lMax= 1000;
		pDevice->SetProperty(DIPROP_RANGE, &dip.diph);
	}
	return DIENUM_CONTINUE;
}

CDIManager::CDIManager() {
	m_pDInput = NULL;
	m_pKeyboard = NULL;
	m_ppJoypads = NULL;
	m_numJoypads = 0;

	for(int i=0; i<4; i++) {
		m_key_state_now[i] = false;
		m_key_state_prev[i] = false;
	}
	m_analog[0] = m_analog[1] = 0;

	// 初期オプション
	m_key_option[0] = DIK_Z;
	m_key_option[1] = DIK_X;
	m_key_option[2] = DIK_C;
	m_key_option[3] = DIK_V;
	m_button_option[0] = 0;
	m_button_option[1] = 1;
	m_button_option[2] = 2;
	m_button_option[3] = 3;
}

CDIManager::~CDIManager() {
	for(UINT i=0; i<m_numJoypads; i++)
		m_ppJoypads[i]->Release();
	delete[] m_ppJoypads;
	m_pKeyboard->Release();
	m_pDInput->Release();
}

BOOL CDIManager::Init(HWND hWnd) {
	if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDInput, NULL))) {
		MessageBox(NULL, "DirectInputの生成に失敗", NULL, MB_OK);
		return FALSE;
	}

	if(FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL))) {
		MessageBox(NULL, "デバイスの生成に失敗", NULL, MB_OK);
		return FALSE;
	}
	if(FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
		MessageBox(NULL, "ダータフォーマットのセットに失敗", NULL, MB_OK);
		return FALSE;
	}
	if(FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) {
		MessageBox(NULL, "協調レベルのセットに失敗", NULL, MB_OK);
		return FALSE;
	}
	if(FAILED(m_pKeyboard->Acquire())) {
		MessageBox(NULL, "Acquireに失敗", NULL, MB_OK);
		return FALSE;
	}

	vector<GUID> vGuid;
	if(FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoypadsGuid, &vGuid, DIEDFL_ATTACHEDONLY))) {
		MessageBox(NULL, "デバイスの列挙に失敗", NULL, MB_OK);
 		return FALSE;
	}
	m_numJoypads = vGuid.size();
	m_ppJoypads = new LPDIRECTINPUTDEVICE8[m_numJoypads];
	for(UINT i=0; i<m_numJoypads; i++) {
		if(FAILED(m_pDInput->CreateDevice(vGuid[i], &m_ppJoypads[i], NULL))) {
			MessageBox(NULL, "デバイスの生成に失敗", NULL, MB_OK);
			return FALSE;
		}
	}
	for(UINT i=0; i<m_numJoypads; i++) {
		if(FAILED(m_ppJoypads[i]->SetDataFormat(&c_dfDIJoystick2))) {
			return FALSE;
		}
		if(FAILED(m_ppJoypads[i]->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) {
			return FALSE;
		}
		if(FAILED(m_ppJoypads[i]->EnumObjects(EnumJoypadsProperty, m_ppJoypads[i], DIDFT_ALL))) {
			return FALSE;
		}
	}

	return TRUE;
}

void CDIManager::Update() {
	int x, y;
	bool key_f[4];
	x = y = 0;
	for(int i=0; i<4; i++) {
		key_f[i] = false;
	}
	
	m_pKeyboard->Acquire();
	BYTE dik[256];
	ZeroMemory(&dik, sizeof(BYTE)*256);
	m_pKeyboard->GetDeviceState(sizeof(dik), &dik);
	if(dik[DIK_UP] & 0x80) y = -1000;
	if(dik[DIK_DOWN] & 0x80) y = 1000;
	if(dik[DIK_LEFT] & 0x80) x = -1000;
	if(dik[DIK_RIGHT] & 0x80) x = 1000;
	for(int i=0; i<4; i++) {
		if(dik[m_key_option[i]]) {
			key_f[i] = true;
		}
	}

	for(UINT i=0; i<m_numJoypads; i++) {
		m_ppJoypads[i]->Acquire();
		DIJOYSTATE2 js;
		ZeroMemory(&js, sizeof(js));
		m_ppJoypads[i]->GetDeviceState(sizeof(js), &js);
		if(abs(js.lX) > abs(x)) x = js.lX;
		if(abs(js.lY) > abs(y)) y = js.lY;
		for(int i=0; i<4; i++) {
			if(js.rgbButtons[m_button_option[i]]) {
				key_f[i] = true;
			}
		}
	}

	m_analog[0] = x;
	m_analog[1] = y;
	for(int i=0; i<4; i++) {
		m_key_state_prev[i] = m_key_state_now[i];
		m_key_state_now[i] = key_f[i];
	}
}

bool CDIManager::IsKeyDown(int key) {
	return m_key_state_now[key];
}

bool CDIManager::IsKeyDownNow(int key) {
	return m_key_state_now[key] && !m_key_state_prev[key];
}

bool CDIManager::IsKeyUpNow(int key) {
	return !m_key_state_now[key] && m_key_state_prev[key];
}

int CDIManager::GetAnalog(int idx) {
	return m_analog[idx];
}
