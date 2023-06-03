#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR		szRelativePath[MAX_PATH] = L""; // 상대 경로를 저장할 문자배열
	TCHAR		szCurDirPath[MAX_PATH] = L""; // 현재 절대 경로를 저장할 문자배열

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// szCurDirPath 에서 strFullPath.GetString()으로 가는 상대 경로를 구해서 szRelativePath 저장해주는 함수(단, 같은 드라이브 내에서만 변경 가능)
	
	PathRelativePathTo(szRelativePath, 
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY, 
		strFullPath.GetString(), 
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

HRESULT CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathList)
{
	CFileFind	Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	if (!bContinue)
		return E_FAIL;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathList);
		}
		else
		{
			if (Find.IsSystem())
				continue;

			IMGPATH*	pImgPath = new IMGPATH;
			TCHAR		szPathBuf[MAX_STR] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szPathBuf);

			pImgPath->iCount = CFileInfo::DirFileCnt(szPathBuf);

			wstring		wstrTextureName = Find.GetFileTitle().GetString();
			
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			PathCombine(szPathBuf, szPathBuf, wstrTextureName.c_str());

			pImgPath->wstrPath = ConvertRelativePath(szPathBuf);

			PathRemoveFileSpec(szPathBuf);

			pImgPath->wstrStateKey = PathFindFileName(szPathBuf);

			PathRemoveFileSpec(szPathBuf);

			pImgPath->wstrObjKey = PathFindFileName(szPathBuf);

			rPathList.push_back(pImgPath);
			bContinue = FALSE;
		}
	}
	return S_OK;
}

int CFileInfo::DirFileCnt(const wstring & wstrPath)
{
	CFileFind			Find;

	wstring		wstrFilePath = wstrPath + L"\\*.*";

	BOOL		bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCnt = 0;

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCnt;

	}

	return iFileCnt;
}
