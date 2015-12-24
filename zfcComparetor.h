#pragma once

#include "zfcTypeDef.h"
#include "ResultCompEntity.h"

/**
 * �}�ʔ�r�N���X
 * 
 */
class zfcComparetor
{
public:

	//	�V���O���g���I�u�W�F�N�g��Ԃ�
	static zfcComparetor& instance();

	//	����������
	void init();

	//	�}�ʔ�r���s��
	bool execute( const CString& strPathOldDwg, const CString strPathNewDwg );

	//	��v�}�ʐ�
	int cntCorrespond() const;

	//	�s��v�}�ʐ�
	int cntDiscord() const;

	//	�x����
	int cntWarning() const;

	//	�G���[��
	int cntError() const;

protected:

	zfcComparetor(void);
	virtual ~zfcComparetor(void);

	//	�t�@�C�����(�X�V�����E�T�C�Y)�Ŕ�r
	bool compareFileStatus( const CString& strPathOldDwg, const CString strPathNewDwg );

	//	�}�ʓǂݍ���
	bool readDwg( AcDbDatabase*& pDb, const CString& strPath ) const;

	//	�}�ʓ��v�f�̃I�u�W�F�N�gID�擾
	bool getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb ) const;

	//	��v�}�ʃC���N�������g
	void incrementCorrespond();

	//	�s��v�}�ʃC���N�������g
	void incrementDiscord();

	//	�x�����C���N�������g
	void incrementWarning();

	//	�G���[���C���N�������g
	void incrementError();

private:
	//	��v�}�ʐ�
	int m_nCntCorrespond;

	//	�s��v�}�ʐ�
	int m_nCntDiscord;

	//	�x����
	int m_nCntWarning;

	//	�G���[��
	int m_nCntError;
};

//	��v�}�ʐ�
inline int zfcComparetor::cntCorrespond() const
{
	return m_nCntCorrespond;
}

//	�s��v�}�ʐ�
inline int zfcComparetor::cntDiscord() const
{
	return m_nCntDiscord;
}

//	�x����
inline int zfcComparetor::cntWarning() const
{
	return m_nCntWarning;
}

//	�G���[��
inline int zfcComparetor::cntError() const
{
	return m_nCntError;
}

//	��v�}�ʃC���N�������g
inline void zfcComparetor::incrementCorrespond()
{
	m_nCntCorrespond++;
}

//	�s��v�}�ʃC���N�������g
inline void zfcComparetor::incrementDiscord()
{
	m_nCntDiscord++;
}

//	�x�����C���N�������g
inline void zfcComparetor::incrementWarning()
{
	m_nCntWarning++;
}

//	�G���[���C���N�������g
inline void zfcComparetor::incrementError()
{
	m_nCntError++;
}

