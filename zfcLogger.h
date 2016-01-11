#pragma once

#include "zfcTypeDef.h"

/**
 * ログ出力クラス
 * 
 */
class zfcLogger
{
public:
	//	シングルトンオブジェクトを返す
	static zfcLogger& instance();

	//	ログファイルオープン
	bool open( const CString& strPath );

	//	ログファイルクローズ
	void close();

	//	ログ出力
	bool write( const CString& strLog, bool bCrLf = false );

	bool isOpen() const;

protected:

	zfcLogger(void);
	virtual ~zfcLogger(void);

	void setIsOpen(bool value );

private:

	CStdioFile m_of;

	bool m_bIsOpen;
};

inline bool zfcLogger::isOpen() const
{
	return m_bIsOpen;
}

inline void zfcLogger::setIsOpen(bool value)
{
	m_bIsOpen = value;
}
