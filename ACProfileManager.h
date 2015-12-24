// ACProfileManager.h: ACProfileManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACPROFILEMANAGER_H__90551992_6BB4_4027_B574_C9D7B20DC68E__INCLUDED_)
#define AFX_ACPROFILEMANAGER_H__90551992_6BB4_4027_B574_C9D7B20DC68E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AC_REG_BUFF 1024
#define ACP_SEC_COMPDWG "COMPDWG"

#include <direct.h>
#include "ACString.h"

class ACProfileManager  
{
public:
	ACProfileManager();
	virtual ~ACProfileManager();


	// ���e�덷����������
	void SetUserTol(double dTol);

	// �I���f�B���N�g���p�X����������
	void SetUserOpenDirPath(LPCTSTR lpctPath);

	// CSV�̕ۑ���f�B���N�g����ۑ�����B
	void SetUserSaveCSVPath(LPCTSTR lpctPath);


	// ���e�덷�𓾂�
	double GetUserTol();

	// �ύX�O��������ύX��֐��������Œ዗��(�������������̐��l)
	double GetUserModTextArrowMinLength();

	// �I���f�B���N�g���p�X��Ԃ��B
	CString GetUserOpenDirPath();
	
	// CSV�̕ۑ���f�B���N�g���p�X��Ԃ��B
	CString GetUserSaveCSVPath();

protected:
	// ���[�U�[�ʃt�@�C���Ɏw�肵��������������
	// strSec: �Z�N�V����, strKey: �L�[
	bool WriteProfileString(const CString strSec, const CString strKey, const CString strWrite);

	// ���[�U�[�ʃt�@�C���������ǂݍ���
	// strSec: �Z�N�V����, strKey: �L�[
	bool ReadProfileString(const CString strSec, const CString strKey, CString& strRet);

private:
	// ���O�C�����[�U�[�̃v���t�@�C���p�X�𓾂āA�����o�ϐ��ɃZ�b�g����B
	void InitUserPath();

	static CString m_strUserProfilePath;	
	
	
};

#endif // !defined(AFX_ACPROFILEMANAGER_H__90551992_6BB4_4027_B574_C9D7B20DC68E__INCLUDED_)
