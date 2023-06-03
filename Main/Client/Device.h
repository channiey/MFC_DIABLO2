#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

	// 1. 장치를 조사할 객체 생성
	// 2. 장치 조사(지원 수준 파악)
	// 3. 장치를 제어할 객체 생성(디바이스 객체)

public:
	LPDIRECT3DDEVICE9		Get_Device(void) { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT				Get_Font(void) { return m_pFont; }

public:
	HRESULT		Initialize(void);

	void		Render_Begin(void);
	void		Render_End(HWND hWnd = nullptr);

	void		Release(void);

private:
	void		Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9			m_pSDK;		// 장치를 조사할 객체 생성
	LPDIRECT3DDEVICE9	m_pDevice;	// 장치를 제어할 객체 생성(디바이스 객체), 그래픽 장치를 제어하는 객체 포인터
	LPD3DXSPRITE		m_pSprite;	// dx 상에서 2D 이미지를 출력해주는 객체
	LPD3DXFONT			m_pFont;	

};

