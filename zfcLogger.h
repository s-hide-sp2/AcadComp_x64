#pragma once

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
	bool close();

	//	���O�o��
	bool write( LPCTSTR lpszLog, ...);

protected:

	zfcLogger(void);
	virtual ~zfcLogger(void);

private:
	FILE* m_fp;
};
