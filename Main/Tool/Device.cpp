#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice()
	: m_pSDK(nullptr), m_pDevice(nullptr), m_pSprite(nullptr), m_pLine(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::Initialize(void)
{
	// 1번
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 2번
	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 정보(지원 수준)를 얻어오는 함수
	// D3DADAPTER_DEFAULT : 정보를 얻으려는 기본 그래픽 카드를 의미
	
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed");
		return E_FAIL;
	}

	DWORD	vp = 0;	// 버텍스 프로세싱 = 정점 변환 + 조명 연산

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	Set_Parameters(d3dpp);

	// 3번
	// CreateDevice : 장치를 제어할 객체 생성
	// CreateDevice(어떤 그래픽 카드를 제어할 것인가, 어떤 정보로 장치에 접근할 것인가, 장치를 사용할 윈도우, 동작 방식, 사용 환경, 장치 제어 객체 생성)
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		AfxMessageBox(L"Create Sprite Failed");
		return E_FAIL;
	}

	D3DXFONT_DESCW		tFontInfo;
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW));

	tFontInfo.Height = 20;
	tFontInfo.Width = 10;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"궁서");
	
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		AfxMessageBox(L"Create Font Failed");
		return E_FAIL;
	}

	D3DXCreateLine(m_pDevice, &m_pLine);

	return S_OK;
}

void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0,			// Rect 배열의 원소 개수
					nullptr,	// Rect를 저장한 배열의 첫 주소, nullptr인경우 전체 영역을 비운다
					D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, // 스텐실,깊이, 타겟 버퍼 모두 비우겠다는 의미
					D3DCOLOR_ARGB(255, 125, 125, 125), // 후면 버퍼를 비우고 채울 색상
					1.f, // 깊이 버퍼 초기화 값
					0); // 스텐실 버퍼 초기화 값

	// 여기서부터 후면 버퍼에 그리기 시작
	m_pDevice->BeginScene();

	// 2D 이미지를 그릴 수 있도록 장치를 준비(렌더링 옵션)
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CDevice::Render_End(HWND hWnd)
{
	m_pSprite->End();
	m_pDevice->EndScene();

	m_pDevice->Present(NULL, NULL, hWnd, NULL);
	// 1, 2번 : 스왑 체인이 옵션이 D3DSWAPEFFECT_COPY일 때만 동작, 둘 다 렉트의 주소를 받음 
	// 3 : 출력 대산 윈도우 핸들 
	// 4번 : D3DSWAPEFFECT_COPY일 때만 동작

}

void CDevice::Release(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
	Safe_Release(m_pSDK);
}

void CDevice::Set_Parameters(D3DPRESENT_PARAMETERS & d3dpp)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD : 스왑체인 방식
	// D3DSWAPEFFECT_FLIP    : 버퍼 하나를 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY    : 더블 버퍼링과 유사한 복사 방식
	
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	
	d3dpp.hDeviceWindow = g_hWnd;
	
	// 창 모드로 실행할 것인가 전체화면 모드로 실행할 것인가
	d3dpp.Windowed = TRUE;	// 창 모드 실행
		
	// DX가 자동적으로 깊이 / 스텐실 버퍼를 만들고 관리하길 원하는 세팅
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체화면 모드일 경우만 사용하는 멤버 변수 옵션

	// 전체화면 모드 시에 현재 모니터 재생률을 토대로 갱신을 수행
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// D3DPRESENT_INTERVAL_IMMEDIATE : 재생율과 시연의 간격을 고려하지 말고 그 즉시 시연
	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 DX가 알아서 결정, 보통 모니터 재생율을 따라감

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
