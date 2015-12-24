// ACProfileManager.cpp: ACProfileManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACProfileManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString ACProfileManager::m_strUserProfilePath = _T("");
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACProfileManager::ACProfileManager()
{
	InitUserPath();
}

ACProfileManager::~ACProfileManager()
{

}

void ACProfileManager::InitUserPath()
{
	// �ʃ��[�U�[�t�@�C���̃p�X
	TCHAR szPath[256];    
	HWND hWnd = NULL;
	CString strPath;
	HRESULT hRes;
	CFile   fileInfo;
	CFileStatus fStatus;	
	
	if (!m_strUserProfilePath.IsEmpty())
		return;

	// ���̂�ERROR_INVALID_FUNCTION�̃G���[���������邪�A�p�X�͎���̂Ŗ�������B
	hRes = SHGetSpecialFolderPath(hWnd, szPath, CSIDL_APPDATA, TRUE); 		
	if (hRes == NOERROR || hRes == ERROR_INVALID_FUNCTION) {
		m_strUserProfilePath = szPath;
// [Taka 2009/06] ARX2010�Ή�  ��1�s�R�����g�A�E�g ��1�s�ǉ�
// SoftAlive��ύX
//		m_strUserProfilePath += _T("\\SoftAlive");
		m_strUserProfilePath += _T("\\OtsukaACADAddinsCOMP");
		if (CFile::GetStatus(m_strUserProfilePath, fStatus) == FALSE) {
			if (_tmkdir(m_strUserProfilePath) != 0) { //2006.07.20
				m_strUserProfilePath = _T("");
				return;
			}
		}

			
		m_strUserProfilePath += _T("\\ACUserAppInfo.ini");			
	}	
		
}

// ���[�U�[�ʃt�@�C���Ɏw�肵��������������
// strSec: �Z�N�V����, strKey: �L�[
bool ACProfileManager::WriteProfileString(const CString strSec, const CString strKey, const CString strWrite)
{	
	if (WritePrivateProfileString(strSec, strKey, strWrite, m_strUserProfilePath) == TRUE) 
		return true;
	else
		return false;
}

// ���[�U�[�ʃt�@�C���������ǂݍ���
// strSec: �Z�N�V����, strKey: �L�[
bool ACProfileManager::ReadProfileString(const CString strSec, const CString strKey, CString& strRet)
{	
	TCHAR cRet[AC_REG_BUFF];//2006.07.20
		
	if (GetPrivateProfileString(strSec, strKey, _T(""), cRet, AC_REG_BUFF, m_strUserProfilePath) == 0) {
		strRet = _T("");
		return false;
	}
	else {
		strRet = cRet;
		return true;
	}
	
}

// ���e�덷����������
void ACProfileManager::SetUserTol(double dTol)
{
	CString strWrite;

	strWrite = ACString::NumberToString(dTol);
	WriteProfileString(ACP_SEC_COMPDWG, _T("TOL"), strWrite);  //2006.07.20
}

// �I���f�B���N�g���p�X����������
void ACProfileManager::SetUserOpenDirPath(LPCTSTR lpctPath)
{	
	WriteProfileString(ACP_SEC_COMPDWG, _T("DIRPATH"), lpctPath); //2006.07.20	
}

// CSV�̕ۑ���f�B���N�g����ۑ�����B
void ACProfileManager::SetUserSaveCSVPath(LPCTSTR lpctPath)
{
	WriteProfileString(ACP_SEC_COMPDWG, _T("CSVDIRPATH"), lpctPath);	//2006.07.20
}


// ���e�덷�𓾂�
double ACProfileManager::GetUserTol()
{
	CString strRet;
	double dTol;

	ReadProfileString(ACP_SEC_COMPDWG, _T("TOL"), strRet);//2006.07.20
	if (ACString::StringToNumber(strRet, dTol) == false)
		return 0.0001;
	else
		return dTol;	
}

// �ύX�O��������ύX��֐��������Œ዗��(�������������̐��l)
double ACProfileManager::GetUserModTextArrowMinLength()
{
	CString strRet;
	double dLen;

	ReadProfileString(ACP_SEC_COMPDWG, _T("MODTEXTARROWMINLENGTH"), strRet);
	if (ACString::StringToNumber(strRet, dLen) == false)
		return 3;
	else
		return dLen;	
}

// �I���f�B���N�g���p�X��Ԃ��B
CString ACProfileManager::GetUserOpenDirPath()
{
	CString strRet;

	ReadProfileString(ACP_SEC_COMPDWG, _T("DIRPATH"), strRet);//2006.07.20
	return strRet;
}

// CSV�̕ۑ���f�B���N�g���p�X��Ԃ��B
CString ACProfileManager::GetUserSaveCSVPath()
{
	CString strRet;

	ReadProfileString(ACP_SEC_COMPDWG, _T("CSVDIRPATH"), strRet);//2006.07.20
	return strRet;
}