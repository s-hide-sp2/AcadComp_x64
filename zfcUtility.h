#pragma once

#include "zfcTypeDef.h"

/**
 * ���[�e�B���e�B�N���X
 * 
 */
class zfcUtility
{
public:

	//	�}�ʂ�ǂݍ���
	static Acad::ErrorStatus readDwg( AcDbDatabase*& pDb, const CString& strPath );

	//	�}�ʓ��S�v�f��ID���擾����
	static Acad::ErrorStatus getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb );

	//	�}�ʓ��̑S�v�f���擾����
	static Acad::ErrorStatus getAllEntity( zfc::entityContainer& conEntity, AcDbDatabase* pDb, AcDb::OpenMode mode );

	//	�u���b�N�e�[�u�����R�[�h���̑S�v�f���擾����
	static Acad::ErrorStatus getAllEntity( zfc::entityContainer& conEntity, AcDbBlockTableRecord* pBlkRec, AcDb::OpenMode mode );

	//	�u���b�N�Q�Ɠ��̑S�������擾����
	static Acad::ErrorStatus getAllAttribute( zfc::entityContainer& conEntity, AcDbBlockReference* pBlockRef, AcDb::OpenMode mode );

	//	���O�t�@�C������Ԃ�
	static CString logFileName();

	//	�t�@�C���p�X��Ԃ�
	static CString filePath( const CString& strFolderPath, const CString& strFileName, const CString& strExt = _T("") );

	//	���O�o�͂���
	static void writeLog1( int nResourceId, const CString& strFmt1 );

	//	���O�o�͂���
	static void writeLog2( int nResourceId, const CString& strFmt1, const CString& strFmt2 );

	//	�t�@�C���������O�o�͏o�͂���
	static void writeFileName( const zfc::pathContainer& conPath );

	//	�t�@�C���p�X����t�@�C������Ԃ�
	static CString fileName( const CString& strPath );

	//	�t�@�C���p�X����t�@�C���^�C�g��(�g���q������)��Ԃ�
	static CString fileTitle( const CString& strPath );

	//	�A�v���P�[�V��������Ԃ�
	static CString& appName();

	//	�g���f�[�^�p�}�ʖ���Ԃ�
	static CString& drawingNameForXData();

	//	�}�ʂɑ΂��Ċg���f�[�^���Z�b�g����
	static Acad::ErrorStatus setAppXData( AcDbDatabase* pDb );

protected:

	zfcUtility(void);
	virtual ~zfcUtility(void);

};

