#pragma once


#pragma region HEEJUNE

#define		VECTOR		D3DXVECTOR3

#pragma endregion


#pragma region CHAN

//#define		WINCX					1400
//#define		WINCY					1200

#define		WINCX				1200
#define		WINCY				800

#define		UNIT_CX					100
#define		UNIT_CY					100

#define		UNIT_MAX_HP				300.f
#define		UNIT_MAX_AD				100.f
#define		UNIT_MAX_SPEED			500.f

#define		TILE_PATH_RENDER_CLOSE	2
#define		TILE_PATH_RENDER_BEST	3
#define		TILE_PATH_RENDER_OPEN	4

#define		UNIT_DEFALUT_SPEED		300.f

#pragma endregion

#define		SCROLL_SPEED			500.f

#define		CONSOLE_OUT				false

#define		KEYBOARD_SCROLL true

#define		TAB_HEIGHT	30
#define		TAB_WIDTH	10


#define		TOOL_CX		1000


#define		OBJ_NOEVENT		0
#define		OBJ_DEAD		1

#define		VK_MAX			0xff

#define		TILECX			160
#define		TILECY			80

#define		TILEX			20
#define		TILEY			30

#define		MIN_STR			64
#define		MAX_STR			256

#define		RUBY			0x01		// -> 0001
#define		DIAMOND			0x02		// -> 0010
#define		SAPPHIRE		0x04		// -> 0100

//타일옵션
#define		NONETILE		0x00
#define		BLOCKTILE		0x01
#define		DAMAGETILE		0x02





#define ERR_MSG(message)								\
::MessageBox(nullptr, message, L"error", MB_OK)

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static void		  Destroy_Instance();				\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
void ClassName::Destroy_Instance()						\
{														\
	if(nullptr != m_pInstance)							\
	{													\
		delete m_pInstance;								\
		m_pInstance = nullptr;							\
	}													\
}


extern HWND		g_hWnd;

// 선언과 동시에 초기화 불가능, 먼저 선언된 전역변수와 자료형, 변수명까지 일치하는 상태에서만 사용 가능