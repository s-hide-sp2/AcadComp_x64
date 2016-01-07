#include "stdafx.h"
#include "resource.h"
#include "zfcUtility.h"
#include "zfcComparetor.h"
#include "ACDocManager.h"
#include "ResultCompEntity.h"
#include "zfcLogger.h"

zfcComparetor::zfcComparetor(void)
{
	init(_T(""));
}


zfcComparetor::~zfcComparetor(void)
{
}

//	�V���O���g���I�u�W�F�N�g��Ԃ�
zfcComparetor& zfcComparetor::instance()
{
	static zfcComparetor sigleton;

	return sigleton;
}

//	����������
void zfcComparetor::init(const CString& strFolderOutput)
{
	m_nCntCorrespond = 0;
	m_nCntDiscord = 0;
	m_nCntWarning = 0;
	m_nCntError = 0;
	m_strFolderOutput = strFolderOutput;
}

//	�}�ʔ�r���s��
bool zfcComparetor::execute( const CString& strPathOldDwg, const CString& strPathNewDwg )
{
	bool bResult = false;
	AcDbDatabase* pDbOld = nullptr;
	AcDbDatabase* pDbNew = nullptr;
	acd::objectIdContainer conObjectIdOld;
	acd::objectIdContainer conObjectIdNew;
	ResultCompEntity resultCompEntity;
	ACDocManager docManager;

	//	�t�@�C�����(�X�V�����E�T�C�Y)�Ŕ�r
	if( !compareFileStatus(strPathOldDwg, strPathNewDwg) )
		return true;

	//	�V�E���}�ʓǂݍ���
	bResult = readDwg( pDbOld, strPathOldDwg );
	if( bResult )
		bResult = readDwg( pDbNew, strPathNewDwg );

	//	�V�E���}�ʂ̑S�G���e�B�e�B��ID�擾
	if( bResult )
		bResult = getAllObjectId( conObjectIdOld, pDbOld );
	if( bResult )
		bResult = getAllObjectId( conObjectIdNew, pDbNew );
	
	//	��r���s
	if( bResult ){
		resultCompEntity.SetHashTableSize( conObjectIdOld.GetCount() + conObjectIdNew.GetCount() );
		bResult = docManager.CompDwgs( conObjectIdOld, conObjectIdNew, resultCompEntity );
		
		if( bResult ){
			if( resultCompEntity.GetCount() == 0 ){
				incrementCorrespond();
				zfcUtility::writeLog2( IDS_CORRESPOND, zfcUtility::fileName(strPathOldDwg), zfcUtility::fileName(strPathNewDwg) );
			}
			else{
				incrementDiscord();
				zfcUtility::writeLog2( IDS_DISCORD, zfcUtility::fileName(strPathOldDwg), zfcUtility::fileName(strPathNewDwg) );
			}
		}
		else{
			zfcUtility::writeLog2( IDS_FAIL_TO_COMPARE_DWG, zfcUtility::fileName(strPathOldDwg), zfcUtility::fileName(strPathNewDwg) );
		}
	}
	
	//	�����}�ʍ쐬
	if( bResult && 0 < resultCompEntity.GetCount() ){
		bResult = makeCompoundDwg(pDbNew->blockTableId(), pDbOld->blockTableId(), resultCompEntity, conObjectIdNew, zfcUtility::fileName(strPathNewDwg) );

		if( !bResult ){
			zfcUtility::writeLog2( IDS_FAIL_TO_COMPOUND_DWG, zfcUtility::fileName(strPathOldDwg), zfcUtility::fileName(strPathNewDwg) );
		}
	}

	if( !bResult )
		incrementError();

	delete pDbOld;
	delete pDbNew;

	return bResult;
}

//	�t�@�C�����(�X�V�����E�T�C�Y)�Ŕ�r
bool zfcComparetor::compareFileStatus( const CString& strPathOldDwg, const CString strPathNewDwg )
{
	bool bContinue = true;
	CFileStatus fileStatusOld;
	CFileStatus fileStatusNew;

	VERIFY( CFile::GetStatus( strPathOldDwg, fileStatusOld ) );
	VERIFY( CFile::GetStatus( strPathNewDwg, fileStatusNew ) );

	if( fileStatusOld.m_mtime == fileStatusNew.m_mtime && fileStatusOld.m_size == fileStatusNew.m_size ){
		incrementCorrespond();
		zfcUtility::writeLog2( IDS_CORRESPOND, zfcUtility::fileName(strPathOldDwg), zfcUtility::fileName(strPathNewDwg) );
		bContinue = false;
	}
	else if( fileStatusNew.m_mtime < fileStatusOld.m_mtime ){
		incrementWarning();
		zfcUtility::writeLog2( IDS_OLD_DWG_TIMESTAMP_IS_NEW, zfcUtility::fileName(strPathOldDwg), zfcUtility::fileName(strPathNewDwg) );
		bContinue = false;
	}

	return bContinue;
}

//	�}�ʓǂݍ���
bool zfcComparetor::readDwg( AcDbDatabase*& pDb, const CString& strPath ) const
{
	bool bResult = true;
	auto es = zfcUtility::readDwg( pDb, strPath );

	if( Acad::eOk != es ){
		zfcUtility::writeLog1( IDS_FAIL_TO_READ_DWG, strPath );
		bResult = false;
	}
	
	return bResult;
}

//	�}�ʓ��v�f�̃I�u�W�F�N�gID�擾
bool zfcComparetor::getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb ) const
{
	bool bResult = true;
	auto es = zfcUtility::getAllObjectId( conObjectId, pDb );

	if( Acad::eOk != es ){
		const ACHAR* pFileName = nullptr;
		
		if( Acad::eOk == pDb->getFilename(pFileName) )
			zfcUtility::writeLog1( IDS_FAIL_TO_GET_ID, pFileName );
		bResult = false;
	}
	
	return bResult;
}

//	�����}�ʍ쐬
bool zfcComparetor::makeCompoundDwg(const AcDbObjectId& blockIdNew, const AcDbObjectId& blockIdOld, ResultCompEntity& resultCompEntity, acd::objectIdContainer& conObjectIdNew, const CString& strFileName ) const
{
	bool bResult = true;
	
	try{
		AcDbDatabase* pDb = new AcDbDatabase(false, true);
		ACDocManager docManager;

		bResult = docManager.DrawResultDwg(blockIdNew, blockIdOld, resultCompEntity, conObjectIdNew, pDb);

		if( bResult ){
			auto filePath = zfcUtility::filePath( folderOutput(), strFileName );
			auto es = pDb->saveAs( filePath );

			if( Acad::eOk != es )
				bResult = false;
		}

		delete pDb;
	}
	catch( std::bad_alloc& ){
		assert( false );
		bResult = false;
	}
	catch(...){
		assert( false );
		bResult = false;
	}

	return bResult;
}
