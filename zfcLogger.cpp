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

//	シングルトンオブジェクトを返す
zfcLogger& zfcLogger::instance()
{
	static zfcLogger singleton;

	return singleton;
}

//	ログファイルオープン
bool zfcLogger::open( const CString& strPath )
{
	if( isOpen() )
		return true;

	if( m_of.Open( strPath, CFile::modeCreate | CFile::modeWrite ) ){
		setIsOpen(true);
	}

	return isOpen();
}

//	ログファイルクローズ
void zfcLogger::close()
{
	if( isOpen() ){
		m_of.Close();
		setIsOpen(false);
	}
}

//	ログ出力
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
