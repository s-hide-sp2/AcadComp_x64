#include "StdAfx.h"
#include "tcTypeDef.h"
#include "tcRegistry.h"
	

//	�R���X�g���N�^
tcRegistry::tcRegistry()
{
	SetHKey( HKEY_CLASSES_ROOT );
	SetKey( TCS_REG_KEY_INSTALL_INFO );
}

//	�f�X�g���N�^
tcRegistry::~tcRegistry()
{
}

//	���W�X�g���f�[�^��ǂݍ���
Tcs::Result tcRegistry::ReadData( 
	LPTSTR lpszValue,	//(o)�l
	LPCTSTR lpszData,	//(i)�f�[�^��
	DWORD dwSize		//(i)�o�b�t�@�T�C�Y
	) const
{
	Tcs::Result result = Tcs::rOk;
	HKEY hKey;
	long lResult = ::RegOpenKeyEx( GetHKey(), GetKey(), 0, KEY_EXECUTE, &hKey );
	
	if( lResult == ERROR_SUCCESS ){
		DWORD dwSizeTemp = dwSize;

		lResult = ::RegQueryValueEx( hKey, lpszData, NULL, NULL, (LPBYTE)lpszValue, &dwSizeTemp );
		::RegCloseKey( hKey );
	}
	
	if( lResult != ERROR_SUCCESS )
		result = Tcs::rFailToAccessRegistry;
	
	return ( result );	
}

//	���W�X�g���f�[�^����������
Tcs::Result tcRegistry::WriteData( 
	LPCTSTR lpszValue,	//(i)�l
	LPCTSTR lpszData	//(i)�f�[�^��
	) const
{
	Tcs::Result result = Tcs::rOk;
	HKEY hKey;
	DWORD dwDisposition;
	long lResult = ::RegCreateKeyEx( GetHKey(), GetKey(), 0, _T(""), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition );

	if( lResult == ERROR_SUCCESS ){
		
		lResult = ::RegSetValueEx( hKey, lpszData, 0, REG_SZ, (CONST LPBYTE)lpszValue, (_tcslen(lpszValue)+1)*sizeof(TCHAR) );
		::RegCloseKey( hKey );
	}
	
	if( lResult != ERROR_SUCCESS )
		result = Tcs::rFailToAccessRegistry;

	return ( result );
}

