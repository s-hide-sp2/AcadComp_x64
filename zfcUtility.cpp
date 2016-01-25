#include "stdafx.h"
#include "zfcUtility.h"
#include "zfcLogger.h"

zfcUtility::zfcUtility(void)
{
}


zfcUtility::~zfcUtility(void)
{
}

//	�}�ʂ�ǂݍ���
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

//	�S�v�f��ID���擾����
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

//	�}�ʓ��̑S�v�f���擾����
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

//	�u���b�N�e�[�u�����R�[�h���̑S�v�f���擾����
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

		// �y�[�p�[��Ԃ̍ŏ��̃r���[�|�[�g�͏���??? todo �K�v���̊m�F
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

//	�u���b�N�Q�Ɠ��̑S�������擾����
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

//	���O�t�@�C������Ԃ�
CString zfcUtility::logFileName()
{
	static CString LOG_FILE_NAME( _T("CompareResult.log") );

	return LOG_FILE_NAME;
}

//	�t�@�C���p�X��Ԃ�
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

//	���O�o�͂���
void zfcUtility::writeLog1( int nResourceId, const CString& strFmt1 )
{
	CString str;

	AfxFormatString1( str, nResourceId, strFmt1 );

	zfcLogger::instance().write( str );
}

//	���O�o�͂���
void zfcUtility::writeLog2( int nResourceId, const CString& strFmt1, const CString& strFmt2 )
{
	CString str;

	AfxFormatString2( str, nResourceId, strFmt1, strFmt2 );

	zfcLogger::instance().write( str );
}

//	�t�@�C���������O�o�͂���
void zfcUtility::writeFileName( const zfc::pathContainer& conPath )
{
	zfc::for_each( conPath, [](zfc::pathContainer::const_reference pair){ 
		CString str;

		str.Format( _T("%s\r\n"), pair.first + _T(".dwg") );
		zfcLogger::instance().write( str ); } );
}

//	�t�H���_�p�X�����O�o�͏o�͂���
void zfcUtility::writeFolderPath( const zfc::pathContainer& conFolderPath )
{
	zfc::for_each( conFolderPath, [](zfc::pathContainer::const_reference pair){ 
		CString str;

		str.Format( _T("%s\r\n"), pair.first );
		zfcLogger::instance().write( str ); 
	} );
}

//	�t�@�C���p�X����t�@�C������Ԃ�
CString zfcUtility::fileName( const CString& strPath )
{
	TCHAR szPath[_MAX_PATH];
	TCHAR szName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];

	_tsplitpath( strPath, NULL, NULL, szName, szExt );
	_tmakepath( szPath, NULL, NULL, szName, szExt ); 
	
	return szPath;
}

//	�t�@�C��������t�@�C���^�C�g��(�g���q������)��Ԃ�
CString zfcUtility::fileTitle( const CString& strPath )
{
	TCHAR szPath[_MAX_PATH];
	TCHAR szName[_MAX_FNAME];
	
	_tsplitpath( strPath, NULL, NULL, szName, NULL );
	_tmakepath( szPath, NULL, NULL, szName, NULL ); 
	
	return szPath;
}

//	���΃p�X��Ԃ�
CString zfcUtility::relativePath( const CString& strFilePath, const CString& strFolderPath )
{
	CString strResult(strFilePath);
	auto temp = strFilePath.Left( strFolderPath.GetLength() );

	if( !strFolderPath.CompareNoCase(temp) ){
		strResult = strFilePath.Mid( strFolderPath.GetLength()+1 );
		
		if( strResult.Right(1) == _T("\\") )
			strResult = strResult.Left(strResult.GetLength()-1);
	}

	return strResult;
}

//	�t�H���_���쐬����
BOOL zfcUtility::createFolders(const CString& strFolder)
{
	BOOL bResult = TRUE;
	CFileFind ff;

	if( !ff.FindFile(strFolder) ){
		auto result = ::SHCreateDirectoryEx( NULL, strFolder, NULL );
	
		if( ERROR_SUCCESS != result )
			bResult = FALSE;
	}

	return bResult;
}

//	�����v�f���͂ދ�`�̍����E�E����W���v�Z����
Acad::ErrorStatus zfcUtility::getMinMaxPoints( AcGePoint3d& pntMin, AcGePoint3d& pntMax, const zfc::entityContainer& conEntity )
{
	Acad::ErrorStatus es = Acad::eOk;
	double dMinX = DBL_MAX;
	double dMinY = DBL_MAX;
	double dMinZ = DBL_MAX;
	double dMaxX = -DBL_MAX;
	double dMaxY = -DBL_MAX;
	double dMaxZ = -DBL_MAX;

	zfc::for_each_if( conEntity, [&](zfc::entityContainer::const_reference pair)->bool{
		const auto& pEnt = pair.second;
		AcDbExtents extents;
		AcGePoint3d pntMax, pntMin;

		es = pEnt->getGeomExtents(extents);
		if( Acad::eOk != es )
			return true;

		pntMin = extents.minPoint();
		pntMax = extents.maxPoint();
		
		dMinX = __min(pntMin.x, dMinX);
		dMinY = __min(pntMin.y, dMinY);
		dMinZ = __min(pntMin.z, dMinZ);

		dMaxX = __max(pntMax.x, dMaxX);
		dMaxY = __max(pntMax.y, dMaxY);
		dMaxZ = __max(pntMax.z, dMaxZ);

		return false;
	} );
	
	if( Acad::eOk == es ){
		pntMin.set( dMinX, dMinY, dMinZ );
		pntMax.set( dMaxX, dMaxY, dMaxZ );
	}

	return es;
}

//	�Y�[������
Acad::ErrorStatus zfcUtility::zoom( const AcGePoint2d& center, double w, double h, AcDbDatabase* pDb )
{
	Acad::ErrorStatus es = Acad::eOk;
	AcDbViewportTable* pVpT = nullptr;
	AcDbViewportTableRecord* pActVp = nullptr;
	
	es = pDb->getViewportTable(pVpT, AcDb::kForRead);
	if( Acad::eOk == es ){
		es = pVpT->getAt(_T("*Active"), pActVp, AcDb::kForWrite );
		pVpT->close();
	}

	if( Acad::eOk == es ){
		pActVp->setCenterPoint( center );
		pActVp->setWidth( w );
		pActVp->setHeight( h );
		pActVp->close();
	}

	return es;
}
