#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"

CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}



HRESULT CSingleTexture::Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey , const int & iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어오는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"D3DXGetImageInfoFromFile Failed");
		return E_FAIL;
	}

	// D3DPOOL_DEFAULT : 가장 적합한 메모리에 보관
	// D3DPOOL_MANAGED : dx에 의해 자원을 관리하며 그래픽 메모리를 사용하지만 램에 백업
	// D3DPOOL_SYSTEMMEM : 램에 저장
	// D3DPOOL_SCRATCH : 시스템 메모리를 사용하지만 dx 장치 접근 금지

	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0,	// D3DUSAGE_RENDERTARGET : 렌더타겟 용 텍스처		
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,  // 이미지 필터링 방식, 이미지 확대, 축소 픽셀에 대한 처리를 묻는 옵션
		D3DX_DEFAULT,  // 밉맵 이미지 필터링 방식
		0,				// 제거할 색상, 0을 넣으면 컬러키 무효화
		nullptr,		// 이미지 정보 기록(m_pTexInfo->tImgInfo 값을 위에서 이미 채웠음)
		nullptr,		// 팔레트 이미지 처리
		&(m_pTexInfo->pTexture))))
	{
		Safe_Delete(m_pTexInfo);
		AfxMessageBox(L"Single Texture Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CSingleTexture::Release(void)
{
	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
