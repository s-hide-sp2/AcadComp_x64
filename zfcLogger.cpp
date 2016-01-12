#include "stdafx.h"
#include "zfcLogger.h"


zfcLogger::zfcLogger(void)
	: m_bIsOpen(false)
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
	if( isOpen() )
		return true;

	if( m_of.Open( strPath, CFile::modeCreate | CFile::modeWrite ) ){
		setIsOpen(true);
	}

	return isOpen();
}

//	���O�t�@�C���N���[�Y
void zfcLogger::close()
{
	if( isOpen() ){
		m_of.Close();
		setIsOpen(false);
	}
}

//	���O�o��
bool zfcLogger::write( const CString& strLog, bool bCrLf )
{
	bool bResult = true;

	assert( isOpen() );

	try{
		m_of.WriteString( strLog );
		if( bCrLf )
			m_of.WriteString( _T("\r\n") );
	}
	catch(...){
		bResult = false;
	}

	return bResult;
}
