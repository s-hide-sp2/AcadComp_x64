#include "stdafx.h"
#include "zfcUtility.h"
#include "zfcLogger.h"

zfcUtility::zfcUtility(void)
{
}


zfcUtility::~zfcUtility(void)
{
}

//	図面を読み込む
Acad::ErrorStatus zfcUtility::readDwg( AcDbDatabase*& pDb, const CString& strPath )
{
	Acad::ErrorStatus es = Acad::eOk;
	
	try{
		pDb = new AcDbDatabase(false);

		es = pDb->readDwgFile( strPath );
	}
	catch( std::bad_alloc& ){
		es = Acad::eOutOfMemory;
	}

	return es;
}

//	全要素のIDを取得する
Acad::ErrorStatus zfcUtility::getAllObjectId( acd::objectIdContainer& conObjectId, AcDbDatabase* pDb )
{
	Acad::ErrorStatus es = Acad::eOk;

	try{
		zfc::entityContainer conEntity;
		int nIndex = 0;

		es = zfcUtility::getAllEntity( conEntity, pDb, AcDb::kForRead );
		if( Acad::eOk == es ){
			zfc::for_each( conEntity, [&](zfc::entityContainer::const_reference p){
				const auto& pEnt = p.second;

				conObjectId[nIndex++] = pEnt->objectId();
			} );
		}
	}
	catch( std::bad_alloc& ){
		es = Acad::eOutOfMemory;
	}
		
	return es;
}

//	図面内の全要素を取得する
Acad::ErrorStatus zfcUtility::getAllEntity( zfc::entityContainer& conEntity, AcDbDatabase* pDb, AcDb::OpenMode mode )
{
	Acad::ErrorStatus es = Acad::eOk;
	AcDbBlockTable* pBlkTable = nullptr;
	AcDbBlockTableIterator* pBlkItr = nullptr;

	es = pDb->getBlockTable(pBlkTable, mode);

	if( Acad::eOk == es )
		es = pBlkTable->newIterator(pBlkItr);

	for( pBlkItr->start(); Acad::eOk == es && !pBlkItr->done(); pBlkItr->step() ){
		AcDbBlockTableRecord* pBlkRec = nullptr;

		es = pBlkItr->getRecord(pBlkRec, AcDb::kForRead);
		if( Acad::eOk != es || !pBlkRec->isLayout() ){
			if( nullptr != pBlkRec )
				pBlkRec->close();
		}
		else{
			es = getAllEntity( conEntity, pBlkRec, mode );
			pBlkRec->close();
		}
	}

	if( nullptr != pBlkTable )
		pBlkTable->close();

	delete pBlkItr;

	return es;
}

//	ブロックテーブルレコード内の全要素を取得する
Acad::ErrorStatus zfcUtility::getAllEntity( zfc::entityContainer& conEntity, AcDbBlockTableRecord* pBlkRec, AcDb::OpenMode mode )
{
	Acad::ErrorStatus es = Acad::eOk;
	AcDbBlockTableRecordIterator* pBlkRecItr = nullptr;
	bool bFirstVp = true;
	AcDbHandle handle;
	TCHAR szHandle[AC_LEN_HANDLE];

	es = pBlkRec->newIterator( pBlkRecItr );

	for( pBlkRecItr->start(); Acad::eOk == es && !pBlkRecItr->done(); pBlkRecItr->step() ){
		AcDbEntity* pEnt = nullptr;

		es = pBlkRecItr->getEntity(pEnt, mode);

		// ペーパー空間の最初のビューポートは除く??? todo 必要性の確認
		if( pEnt->isA()->isEqualTo(AcDbViewport::desc()) == Adesk::kTrue && bFirstVp ){
			bFirstVp = false;
		}
		else {
			pEnt->getAcDbHandle( handle );		
			handle.getIntoAsciiBuffer(szHandle);
			conEntity.insert( zfc::entityContainer::value_type(szHandle, pEnt) );

			if( pEnt->isA()->isEqualTo(AcDbBlockReference::desc() ) )
				es = getAllAttribute( conEntity, AcDbBlockReference::cast(pEnt), mode );
		}
	}

	delete pBlkRecItr;

	return es;
}

//	ブロック参照内の全属性を取得する
Acad::ErrorStatus zfcUtility::getAllAttribute( zfc::entityContainer& conEntity, AcDbBlockReference* pBlockRef, AcDb::OpenMode mode )
{
	Acad::ErrorStatus es = Acad::eOk;
	AcDbObjectIterator* pItrAttr = pBlockRef->attributeIterator();
	
	if( nullptr == pItrAttr )
		return Acad::eOutOfMemory;

	AcDbHandle handle;
	TCHAR szHandle[AC_LEN_HANDLE];

	for( pItrAttr->start(); Acad::eOk == es && !pItrAttr->done(); pItrAttr->step() ){
		AcDbEntity* pEnt = nullptr;
		AcDbHandle handle;
		auto id = pItrAttr->objectId();

		assert( !id.isNull() );
	
		es = acdbOpenAcDbEntity(pEnt, id, mode);
		assert( Acad::eOk == es );

		if( Acad::eOk == es ){
			pEnt->getAcDbHandle(handle);
			handle.getIntoAsciiBuffer(szHandle);
			conEntity.insert( zfc::entityContainer::value_type(szHandle, pEnt) );
		}
	}

	delete pItrAttr;

	return es;
}

//	ログファイル名を返す
CString zfcUtility::logFileName()
{
	static CString LOG_FILE_NAME( _T("CompareResult.log") );

	return LOG_FILE_NAME;
}

//	ファイルパスを返す
CString zfcUtility::filePath( const CString& strFolderPath, const CString& strFileName, const CString& strExt )
{
	TCHAR szPath[MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	CString strDir;

	_tsplitpath( strFolderPath, szDrive, szDir, szFname, NULL );
	strDir.Format( _T("%s%s"), szDir, szFname );
	_tmakepath( szPath, szDrive, strDir, strFileName, strExt );

	return szPath;
}

//	ログ出力する
void zfcUtility::writeLog1( int nResourceId, const CString& strFmt1 )
{
	CString str;

	AfxFormatString1( str, nResourceId, strFmt1 );

	zfcLogger::instance().write( str );
}

//	ログ出力する
void zfcUtility::writeLog2( int nResourceId, const CString& strFmt1, const CString& strFmt2 )
{
	CString str;

	AfxFormatString2( str, nResourceId, strFmt1, strFmt2 );

	zfcLogger::instance().write( str );
}

//	ファイル名をログ出力する
void zfcUtility::writeFileName( const zfc::pathContainer& conPath )
{
	zfc::for_each( conPath, [](zfc::pathContainer::const_reference pair){ 
		CString str;

		str.Format( _T("%s\r\n"), pair.first + _T(".dwg") );
		zfcLogger::instance().write( str ); } );
}

//	ファイルパスからファイル名を返す
CString zfcUtility::fileName( const CString& strPath )
{
	TCHAR szPath[_MAX_PATH];
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];

	_tsplitpath( strPath, NULL, NULL, szName, szExt );
	_tmakepath( szPath, NULL, NULL, szName, szExt ); 
	
	return szPath;
}

//	ファイル名からファイルタイトル(拡張子を除く)を返す
CString zfcUtility::fileTitle( const CString& strPath )
{
	TCHAR szPath[_MAX_PATH];
	TCHAR szName[_MAX_FNAME];
	
	_tsplitpath( strPath, NULL, NULL, szName, NULL );
	_tmakepath( szPath, NULL, NULL, szName, NULL ); 
	
	return szPath;
}

//	アプリケーション名を返す
CString& zfcUtility::appName()
{
	static CString strAppName(_T("ZFC_APPLICATION"));

	return strAppName;
}

//	拡張データ用図面名を返す
CString& zfcUtility::drawingNameForXData()
{
	static CString strDrawingName(_T("ZFC_DRAWING"));

	return strDrawingName;
}

//	図面に対して拡張データをセットする
Acad::ErrorStatus zfcUtility::setAppXData( AcDbDatabase* pDb )
{
	Acad::ErrorStatus es = Acad::eOk;
	AcDbBlockTable* pBlkTbl = nullptr;
	AcDbObjectId idModelSpace;
	AcDbObject* pModelSpace = nullptr;
	struct resbuf* pRb = nullptr;
	struct resbuf* pRbData = nullptr;
	bool bSetXData = false;

	//	モデル空間のID取得
	es = pDb->getSymbolTable(pBlkTbl, AcDb::kForRead);

    if( Acad::eOk == es ){
		es = pBlkTbl->getAt(ACDB_MODEL_SPACE, idModelSpace, AcDb::kForRead);
		pBlkTbl->close();
	} 

	if( Acad::eOk == es )
		es = acdbOpenObject(pModelSpace, idMoidModelSpacedel, AcDb::kForWrite);
	
	if( Acad::eOk == es )
		pRb = pModelSpace->xData( zfcUtility::appName() );

	//	設定済みの場合は何もしない
	if( nullptr != pRb ){
		ads_relrb(pRb);
		pRb = nullptr;
	}
	else{
		pRb = ads_newrb(AcDb::kDxfRegAppName);

		if( nullptr == pRb ){
			es = Acad::rOutOfMemory;
		}
		else{
			bSetXData = true;
		}
	}

	//	拡張データをセット
	if( bSetXData ){
		try{
			pRb->resval.rstring = new TCHAR[zfcUtility::appName().GetLength() + 1];
			_tcscpy( pRb->resval.rstring, zfcUtility::appName() );

			pRbData = ads_newrb(AcDb::kDxfXdAsciiString);
			pRbData->resval.rstring = new TCHAR[zfcUtility::drawingNameForXData().GetLength() + 1];
			_tcscpy(pRbData->resval.rstring, zfcUtility::drawingNameForXData());
			pRb->rbnext = pRbData;

			es = pModelSpace->setXData(pRb);
			ads_relrb(pRb);
		}
		catch(...){
			assert( false );
			if( nullptr != pRb )
				ads_relrb(pRb);
			if( nullptr != pRbData )
				ads_relrb(pRbData);

			es = Acad::rOutOfMemory;
		}
	}
		
	if( nullptr != pModelSpace )
		pModelSpace->close();

	return es;
}
