#include "stdafx.h"
#include "zfcLogger.h"


zfcLogger::zfcLogger(void)
	: m_fp(nullptr)
{
}


zfcLogger::~zfcLogger(void)
{
	close();
}

//	�V���O���g���I�u�W�F�N�g��Ԃ�
zfcLogger& zfcLogger::instance()
{
	static zfcLogger singleton;

	return singleton;
}

//	���O�t�@�C���I�[�v��
bool zfcLogger::open( const CString& strPath )
{
	bool bResult = true;
	
	if( nullptr != m_fp )
		m_fp = _tfopen( strPath, _T("a") );	

	if( nullptr == m_fp )
		bResult = false;

	return bResult;
}

//	���O�t�@�C���N���[�Y
bool zfcLogger::close()
{
	bool bResult = true;

	if( nullptr != m_fp ){
		if( fclose(m_fp) != 0 )
			bResult = false;
		
		m_fp = nullptr;
	}

	return bResult;
}

//	���O�o��
bool zfcLogger::write( 
	LPCTSTR lpszLog,		//(i)�o�͓��e
	...	)					//(i)�t�H�[�}�b�g����
{
	bool bResult = true;
	va_list	ap;
	TCHAR* lpszBuff = nullptr;
	int nLen = 0;

	va_start(ap, lpszLog);
	nLen = _vsctprintf( lpszLog, ap ) + 1;

	try{
		lpszBuff = new TCHAR[nLen];

#ifdef _UNICODE
		_vstprintf( lpszBuff, nLen, lpszLog, ap );
#else
		_vstprintf( lpszBuff, lpszLog, ap );
#endif
		_ftprintf( m_fp, lpszBuff );
		va_end(ap);
		delete [] lpszBuff;
	}
	catch( ... ){
		bResult = false;
	}

	return bResult;
}
