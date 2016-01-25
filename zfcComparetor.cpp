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
				zfcUtility::writeLog2( IDS_CORRESPOND, zfcUtility::relativePath(strPathOldDwg, folderOldDwg()), zfcUtility::relativePath(strPathNewDwg, folderNewDwg()) );
			}
			else{
				incrementDiscord();
				zfcUtility::writeLog2( IDS_DISCORD, zfcUtility::relativePath(strPathOldDwg, folderOldDwg()), zfcUtility::relativePath(strPathNewDwg, folderNewDwg()) );
			}
		}
		else{
			zfcUtility::writeLog2( IDS_FAIL_TO_COMPARE_DWG, zfcUtility::relativePath(strPathOldDwg, folderOldDwg()), zfcUtility::relativePath(strPathNewDwg, folderNewDwg()) );
		}
	}
	
	//	�����}�ʍ쐬
	if( bResult && 0 < resultCompEntity.GetCount() ){
		CString strCompoundDwgFileName = compoundFileName( zfcUtility::fileTitle(strPathOldDwg), zfcUtility::fileTitle(strPathNewDwg) );
		bResult = makeCompoundDwg(pDbNew->blockTableId(), pDbOld->blockTableId(), resultCompEntity, conObjectIdNew, strCompoundDwgFileName );

		if( !bResult ){
			zfcUtility::writeLog2( IDS_FAIL_TO_COMPOUND_DWG, zfcUtility::relativePath(strPathOldDwg, folderOldDwg()), strCompoundDwgFileName );
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
		zfcUtility::writeLog2( IDS_CORRESPOND, zfcUtility::relativePath(strPathOldDwg, folderOldDwg()), zfcUtility::relativePath(strPathNewDwg, folderNewDwg()) );
		bContinue = false;
	}
	else if( fileStatusNew.m_mtime < fileStatusOld.m_mtime ){
		incrementWarning();
		zfcUtility::writeLog2( IDS_OLD_DWG_TIMESTAMP_IS_NEW, zfcUtility::relativePath(strPathOldDwg, folderOldDwg()), zfcUtility::relativePath(strPathNewDwg, folderNewDwg()) );
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
		AcDbDatabase* pDb = new AcDbDatabase(true, true);
		ACDocManager docManager;

		bResult = docManager.DrawResultDwg(blockIdNew, blockIdOld, resultCompEntity, conObjectIdNew, pDb);

		if( bResult ){
			zfc::entityContainer conEntity;
			auto es = zfcUtility::getAllEntity( conEntity, pDb, AcDb::kForRead );
			AcGePoint3d pntMin, pntMax;

			if( Acad::eOk == es )
				es = zfcUtility::getMinMaxPoints( pntMin, pntMax, conEntity );
			if( Acad::eOk == es ){
				auto pntCenter3d = pntMin + 0.5*( pntMax - pntMin );
				AcGePoint2d pntCenter2d( pntCenter3d.x, pntCenter3d.y );
				es = zfcUtility::zoom( pntCenter2d, (pntMax.x - pntMin.x), (pntMax.y - pntMin.y), pDb );
			}
		}

		if( bResult ){
			VERIFY( zfcUtility::createFolders(folderOutput()) );
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

//	�����}�ʃt�@�C������Ԃ�
CString zfcComparetor::compoundFileName( const CString& strOldDwgFileTitle, const CString& strNewDwgFileTitle ) const
{
	CString strCompoundDwgFileName;

	if( !strOldDwgFileTitle.CompareNoCase(strNewDwgFileTitle) ){
		strCompoundDwgFileName.Format( _T("%s.dwg"), strNewDwgFileTitle );
	}
	else{
		strCompoundDwgFileName.Format( _T("%s-%s.dwg"), strOldDwgFileTitle, strNewDwgFileTitle.Right(1) );
	}

	return strCompoundDwgFileName;
}

