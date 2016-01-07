#pragma once

#include "zfcTypeDef.h"

/**
 * ���O�o�̓N���X
 * 
 */
class zfcLogger
{
public:
	//	�V���O���g���I�u�W�F�N�g��Ԃ�
	static zfcLogger& instance();

	//	���O�t�@�C���I�[�v��
	bool open( const CString& strPath );

	//	���O�t�@�C���N���[�Y
	void close();

	//	���O�o��
	bool write( const CString& strLog, bool bCrLf = false );

	bool isOpen() const;

protected:

	zfcLogger(void);
	virtual ~zfcLogger(void);

private:

	CStdioFile m_of;

	bool m_bIsOpen;
};

inline bool zfcLogger::isOpen() const
{
	return m_bIsOpen;
}

