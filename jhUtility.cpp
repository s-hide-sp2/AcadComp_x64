#include "StdAfx.h"
//#include "StdArx.h"
#include "resource.h"
#include "jhTypeDef.h"
#include "jhUtility.h"
#include "jhRegistry.h"
#include "jhOption.h"
#include "jhDlgMenu.h"


//	�����v�f���͂ދ�`�̍����E�E����W���v�Z����
bool jhUtility::GetMinMaxPoints( 
	AcGePoint3d& pntMin,				//(o)�������W
	AcGePoint3d& pntMax,				//(o)�E����W
	const AcDbVoidPtrArray& entArray	//(i)�G���e�B�e�B�z��
	)
{
	bool bResult = false;
	double dMinX = DBL_MAX;
	double dMinY = DBL_MAX;
	double dMinZ = DBL_MAX;
	double dMaxX = -DBL_MAX;
	double dMaxY = -DBL_MAX;
	double dMaxZ = -DBL_MAX;

	for( int n = 0; n < entArray.length(); n++ ){
		AcDbEntity*	pEnt = AcDbEntity::cast((AcRxObject*)entArray[n]);
		AcDbExtents extents;
		AcGePoint3d pntMax, pntMin;

		if( pEnt == NULL )
			continue;

		pEnt->getGeomExtents(extents);

		pntMin = extents.minPoint();
		pntMax = extents.maxPoint();
		
		dMinX = __min(pntMin.x, dMinX);
		dMinY = __min(pntMin.y, dMinY);
		dMinZ = __min(pntMin.z, dMinZ);

		dMaxX = __max(pntMax.x, dMaxX);
		dMaxY = __max(pntMax.y, dMaxY);
		dMaxZ = __max(pntMax.z, dMaxZ);
		bResult = true;
	}
	
	if( bResult ){
		pntMin.set( dMinX, dMinY, dMinZ );
		pntMax.set( dMaxX, dMaxY, dMaxZ );
	}

	return ( bResult );
}

void jhUtility::UpdateView()
{
	actrTransactionManager->flushGraphics();
	acedUpdateDisplay();
}

//	�}�ʂ��J��
Jhc::Result jhUtility::OpenDwg( 
	LPCTSTR lpszDwgPath //(i)�}�ʃt�@�C���p�X
	)
{
	Jhc::Result result = Jhc::rOk;
	DWORD dwAttributes = ::GetFileAttributes( lpszDwgPath );

	if( dwAttributes == ACC_FILE_NOT_EXIST || dwAttributes == FILE_ATTRIBUTE_DIRECTORY )
		result = Jhc::rInvalidFilePath;
	else{
/*		AcApDocument* pTargetDoc = NULL;		
		AcApDocumentIterator* pItr = acDocManager->newAcApDocumentIterator();
	
		if( pItr == NULL )
			return ( Jhc::rOutOfMemory );

		//	�����̐}�ʂ����ɃI�[�v���ς݂��m�F����
		for( ; !pItr->done() && pTargetDoc == NULL; pItr->step() ){
			AcApDocument* pDoc = pItr->document();

			if( !_tcsicmp( pDoc->fileName(), lpszDwgPath ) )
				pTargetDoc = pDoc;
		}

		delete pItr;

		//	�����̐}�ʂ��I�[�v�����ĂȂ��ꍇ�͐}�ʂ��J��
		//	�J���Ă���ꍇ�ɂ͂��̐}�ʂ��J�����g�}�ʂɂ���
		if( pTargetDoc == NULL )
			openSyncDocHelper( (void*)lpszDwgPath );
		else
			acDocManager->setCurDocument( pTargetDoc, AcAp::kNone, true );
			*/
		result = SetCurDocument( lpszDwgPath );
		if( result == Jhc::rCancel ){
			result = Jhc::rOk;
			openSyncDocHelper( (void*)lpszDwgPath );
		}
	}

	return ( result );
}

//	�J�����g�h�L�������g�Z�b�g
Jhc::Result jhUtility::SetCurDocument( 
	LPCTSTR lpszDwgPath //(i)�t�@�C���p�X
	)
{
	Jhc::Result result = Jhc::rCancel;
	AcApDocument* pTargetDoc = NULL;		
	AcApDocumentIterator* pItr = acDocManager->newAcApDocumentIterator();

	if( pItr == NULL )
		return ( Jhc::rOutOfMemory );

	//	�����̐}�ʂ����o
	for( ; !pItr->done() && pTargetDoc == NULL; pItr->step() ){
		AcApDocument* pDoc = pItr->document();

		if( !_tcsicmp( pDoc->fileName(), lpszDwgPath ) )
			pTargetDoc = pDoc;
	}
	delete pItr;

	//	�����̐}�ʂ����o������J�����g�}�ʂɂ���
	if( pTargetDoc != NULL ){
		Acad::ErrorStatus es;

		if( curDoc() == pTargetDoc )
			return ( Jhc::rOk );		

		es = acDocManager->setCurDocument( pTargetDoc, AcAp::kNone, true );
		if( es == Acad::eOk )
			result = Jhc::rOk;
		else
			result = Jhc::rAcadError;
	}
	
	return ( result );
}

void jhUtility::openSyncDocHelper( void *pData )
{
	if( acDocManager->isApplicationContext() ){
		//--2008/1/30 Unicode �Ή�
		//acDocManager->appContextOpenDocument( (const char *)pData );
		acDocManager->appContextOpenDocument( (LPCTSTR)pData );
	}
}

//	�R�}���h�Ăяo���R�}���h���s����
bool jhUtility::DoesExecuteCommandCall( 
	const Jhc::CheckItem& checkItem //(i)�`�F�b�N����
	)
{
	bool bResult = false;

	if( DoesCheckByDwg(checkItem) || checkItem.bCheckFileNum )
		bResult = true;

	return ( bResult );
}

//	�}�ʖ��`�F�b�N����
bool jhUtility::DoesCheckByDwg( 
	const Jhc::CheckItem& checkItem //(i)�`�F�b�N����
	)
{
	bool bResult = false;
	
	if(		checkItem.bCheckFileName	//	�t�@�C�����`�F�b�N
		||	checkItem.bCheckClosed		//	���`�F�b�N
		||	checkItem.bCheckLayer		//	���C���`�F�b�N
		||	checkItem.bCheckFrame		//	�}�g�`�F�b�N
		||	checkItem.bCheckRoadArea	//	���H���`�F�b�N
		||	checkItem.bCheckPlane		//	���ʐ��`�`�F�b�N			
		||	checkItem.bCheckXRefPath	//	�p�X�`�F�b�N
		||	checkItem.bCheckDwgVer	)	//	Dwg�o�[�W����
	bResult = true;

	return ( bResult );
}

//	�g�[�N�����擾����
void jhUtility::GetTokens( 
	//--2008/1/30 Unicode �Ή�
	//std::vector<std::string>& vecToken,	//(o)�g�[�N��
	std::vector<atstring>& vecToken,	//(o)�g�[�N��
	CString& str,					//(i/o)�g�[�N���f�[�^
	TCHAR chSep						//(i)��؂蕶��
	)
{
	CString strToken;

	for( int n = 0; !str.IsEmpty(); n++ ){
		strToken = StrTok( str, chSep );
		vecToken.push_back( (LPCTSTR)strToken );
	}
}

//	�g�[�N�����擾����
CString jhUtility::StrTok( 
	CString& str,	//(i/o)�g�[�N���f�[�^
	TCHAR chSep		//(i)��؂蕶��	
	)
{
	CString strToken;
	int nIndex = str.Find( chSep );
	
	if( nIndex == -1 ){
		strToken = str;
		str.Empty();
	}
	else{
		strToken = str.Left(nIndex);
		str = str.Mid( nIndex+1 );
	}

	return ( strToken );
}

//	�p�X����t�H���_�����擾����
void jhUtility::GetFolderName(
	LPTSTR lpszFolder,	//(o)�t�H���_���i�啶���j
	LPCTSTR lpszPath	//(i)�p�X
	)
{
	TCHAR szDrive[_MAX_DRIVE];
    TCHAR szDir[_MAX_DIR];

	_tsplitpath( lpszPath, szDrive, szDir, NULL, NULL );
	_tmakepath( lpszFolder, szDrive, szDir, NULL, NULL );
	//	�啶���ɕϊ�
	_tcsupr( lpszFolder );
}

//	�|�����C��������
bool jhUtility::IsIn( 
	const AcDbPolyline* pPoly,	//(i)�|�����C��
	const AcGePoint3d& pnt		//(i)������W
	)
{
	bool bResult = IsClosed( pPoly );
	
	if( bResult ){
		AcGePoint3dArray vertex;
		int n = 0;
		
		//	�|�����C�����_�擾
		vertex.setLogicalLength( pPoly->numVerts() );
		//--2008/1/30 Unicode �Ή�
		//for( int n = 0; n < pPoly->numVerts(); n++ )
		for( n = 0; n < pPoly->numVerts(); n++ )
			pPoly->getPointAt( n, vertex[n] );
		
		double dGTol = AcGeContext::gTol.equalPoint();

		AcGeContext::gTol.setEqualPoint( JHC_TOL );
		
		//	������W-�|�����C�����_�Ԓ����ƃ|�����C���Ƃ̌�_�v�Z���s��
		//	��_���|�����C�����_�ƈ�v���Ȃ���΁A�|�����C���O�Ɣ��肷��
		for( n = 0; n < vertex.length() && bResult; n++ ){
			AcDbLine line( pnt, vertex[n] );
			AcGePoint3dArray intersects;

			line.intersectWith( pPoly, AcDb::kOnBothOperands, intersects );

			//	�|�����C�����_��v����
			if( intersects.length() == 1 )
				bResult = vertex.contains( intersects[0] );
			else if( 1 < intersects.length() )
				bResult = false;
		}

		AcGeContext::gTol.setEqualPoint( dGTol );
	}

	return ( bResult );
}

//	��`�T�C�Y������Ԃ�
CString jhUtility::GetRectString( 
	double dW,	//(i)��
	double dH	//(i)����
	)
{
	CString str;
	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
	wostringstream ostr;
#else
	ostrstream ostr;
#endif

	ostr.setf( ios::fixed, ios::floatfield );	
	ostr.precision( JHC_LOG_PRECISION );

	//	�[�_���W������擾		
	ostr << _T("(W:") << dW << _T(" H:") << dH << _T(')') << ends;

	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
	str = ostr.str().c_str();
#else
	str = ostr.str();
	ostr.rdbuf()->freeze(0);
#endif

	return ( str );
}

//	��`�T�C�Y���v�Z����
Jhc::Result jhUtility::CalcRectSize( 
	double& dW,					//(o)��
	double& dH,					//(o)����
	const AcDbPolyline* pPoly	//(i)��`
	) 
{
	Jhc::Result result = Jhc::rOk;
	Acad::ErrorStatus es = Acad::eOk;
	AcGePoint2dArray vertex;

	//	�|�����C���̂R���_���擾
	vertex.setLogicalLength(3);
	for( int n = 0; n < vertex.length() && es == Acad::eOk; n++ )
		es = pPoly->getPointAt( n, vertex[n] );

	//	���_�ԋ������v�Z����
	if( es == Acad::eOk ){
		double dDist[2];

		dDist[0] = vertex[0].distanceTo(vertex[1]);
		dDist[1] = vertex[1].distanceTo(vertex[2]);
		
		dW = __max( dDist[0], dDist[1] );
		dH = __min( dDist[0], dDist[1] );
	}
	else
		result = Jhc::rFail;

	return ( result );
}

//	�t�@�C���p�X���擾����
Jhc::Result jhUtility::GetFilePath(
	LPTSTR lpszFilePath,	//(o)�t�@�C���p�X
	LPCTSTR lpszFileName	//(i)�t�@�C����
	)
{
	Jhc::Result result = Jhc::rOk;
	jhRegistry reg;
	TCHAR szInstPath[MAX_PATH];

	//--2008/1/30 Unicode �Ή�
	//result = reg.ReadData( szInstPath, JHC_REG_DATA_INSTALL_PATH, sizeof(szInstPath) );
	result = reg.ReadData( szInstPath, JHC_REG_DATA_INSTALL_PATH, sizeof(szInstPath)/sizeof(TCHAR) );
	if( result == Jhc::rOk )
		_tmakepath( lpszFilePath, NULL, szInstPath, lpszFileName, NULL );

	return ( result );
}

//	�ݒ�t�@�C���p�X���擾����
Jhc::Result jhUtility::GetIniFilePath(
	LPTSTR lpszFilePath		//(o)�t�@�C���p�X
	)
{
	return ( GetFilePath( lpszFilePath, JHC_INI_FILE_NAME ) );
}

//	�`�F�b�N���ʃt�@�C���p�X���擾����
Jhc::Result jhUtility::GetCheckResultFilePath(
	LPTSTR lpszFilePath		//(o)�t�@�C���p�X
	)
{
	Jhc::Result result = Jhc::rOk;

	result = GetFilePath( lpszFilePath, JHC_CHECK_RESULT_FILE_NAME );

/*	if( jhDlgMenu::Instance().GetCheckCurDwg() )
		lpszFilePath[0] = _T('\0');
	else
		result = GetFilePath( lpszFilePath, JHC_CHECK_RESULT_FILE_NAME );
*/
	return ( result );
}

//	�`�F�b�N���ʃt�@�C���p�X���擾����(AutoCad�\���p)
//##ModelId=4316551103DD
Jhc::Result jhUtility::GetCheckResultFilePath2(LPTSTR lpszFilePath)
{
	Jhc::Result result = Jhc::rOk;

	if( jhOption::Instance().GetCheckCurDwg() )
		result = GetFilePath( lpszFilePath, JHC_CHECK_IN_DWG_RESULT_FILE_NAME );
	else
		result = GetFilePath( lpszFilePath, JHC_CHECK_FOR_DISP_RESULT_FILE_NAME );

	return ( result );
}

//	�`�F�b�N�T�����v�Z����
UINT jhUtility::CalcCheckSum( 
	LPCTSTR lpszBuff,	//(i)������o�b�t�@
	UINT size			//(i)�o�b�t�@�T�C�Y
	)
{
	UINT unResult = 0;

	for( UINT n = 0; n < size; n++ )
		unResult += lpszBuff[n] * 10;

	return ( unResult );
}

//	�G���[���b�Z�[�W��Ԃ�
CString jhUtility::GetErrorMessage( 
	Jhc::Result result //(i)���s����
	)
{
	CString strMessage;
	UINT resourceId = 0;
	CAcModuleResourceOverride resOverride;
	
	switch( result ){
	case Jhc::rOk:
		resourceId = IDS_OK;	break;
	case Jhc::rFail:
		resourceId = IDS_FAIL;	break;
	case Jhc::rAcadError:
		resourceId = IDS_ACAD_ERROR;	break;
	case Jhc::rOutOfMemory:
		resourceId = IDS_OUT_OF_MEMORY;	break;
	case Jhc::rFailToWriteCheckLayer:
		resourceId = IDS_FAIL_TO_WRITE_LOG;	break;
	case Jhc::rFailToAccessRegistry:
		resourceId = IDS_FAIL_TO_ACCESS_REGISTRY;	break;
	case Jhc::rFailToWriteIni:
		resourceId = IDS_FAIL_TO_WRITE_INI;	break;
	case Jhc::rFailToReadIni:				
		resourceId = IDS_FAIL_TO_READ_INI;	break;
	case Jhc::rIncorrectIni:				
		resourceId = IDS_INCORRECT_INI;		break;
	case Jhc::rExecelError:				
		resourceId = IDS_EXECEL_ERROR;	break;
	case Jhc::rOverUseSpan:				
		resourceId = IDS_OVER_USE_SPAN;	break;
	case Jhc::rOpenedCheckResultExcelFile:				
		resourceId = IDS_OPENED_CHECK_RESULT_EXCEL_FILE;	break;
	case Jhc::rInvalidFilePath:
		resourceId = IDS_INVALID_FILE_PATH;	break;
	};
	
	strMessage.LoadString( resourceId );

	return ( strMessage );
}

//	Ini�t�@�C�����`�F�b�N����
Jhc::Result jhUtility::CheckIniFile()
{
	Jhc::Result result = Jhc::rOk;
	TCHAR szPath[MAX_PATH];

	result = GetIniFilePath(szPath);
	if( result == Jhc::rOk ){
		//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
		wifstream ifs;
#else
		ifstream ifs;
#endif
		
		ifs.open( szPath, ios::in );
				
		if( ifs.is_open() ){
			TCHAR szCheckSum[64];
	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
			wostringstream ostr;
#else
			ostrstream ostr;
#endif
			UINT unCalcCheckSum;
			CString str;
			long lCheckSum;
			//--2008/1/30 Unicode �Ή�
			//char* pStop = NULL;
			LPTSTR pStop = NULL;

			//	�`�F�b�N�T���ǂݍ���
			ifs.getline( szCheckSum, sizeof(szCheckSum) );
			
			//	Ini���e�ǂݍ���
			ifs >> ostr.rdbuf();
			ostr << ends;
	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
			str = ostr.str().c_str();
#else
			str = ostr.str();
			ostr.rdbuf()->freeze(0);
#endif
			
			//	16�i�@���@10�i�ϊ�
			lCheckSum = _tcstoul( szCheckSum, &pStop, 16 );
			
			//	Ini���e�̃`�F�b�N�T���v�Z
			unCalcCheckSum = CalcCheckSum( str, str.GetLength() );
			
		//	ostr.rdbuf()->freeze( 0 );
			
			//	Ini�ɏ������܂�Ă���`�F�b�N�T����Ini���e�Ōv�Z�����l����v���邩�𔻒�
			if( lCheckSum != unCalcCheckSum )
				result = Jhc::rIncorrectIni;

			ifs.close();
		}
		else
			result = Jhc::rFailToReadIni;
	}

	return ( result );
}

//	Ini����f�[�^���擾����
Jhc::Result jhUtility::GetIniData( 
	LPTSTR lpszValue,		//(o)�l
	DWORD dwSize ,			//(i)�o�b�t�@�T�C�Y
	LPCTSTR lpszSecName,	//(i)�Z�N�V������
	LPCTSTR lpszKey			//(i)�L�[
	)
{
	Jhc::Result result = Jhc::rOk;
	TCHAR szIniPath[MAX_PATH];
	
	result = GetIniFilePath( szIniPath );
	if( result == Jhc::rOk ){
		if( ::GetPrivateProfileString(lpszSecName, lpszKey, _T(""), lpszValue, dwSize, szIniPath ) == 0 )
			result = Jhc::rFailToReadIni;		
	}

	return ( result );
}

//	�|�����C������
bool jhUtility::IsClosed( const AcDbPolyline* pPoly )
{
	bool bResult = false;

	if( pPoly->isClosed() )
		bResult = true;
	else{
		AcGePoint3d pntS, pntE;

		pPoly->getStartPoint( pntS );
		pPoly->getEndPoint( pntE );

		if( pntS.isEqualTo( pntE, jhOption::Instance().GetTol() ) )
			bResult = true;
	}

	return ( bResult );
}

//	�_����
bool jhUtility::FindEqualPoint( 
	const AcGePoint3d& pntTarget,	//(i)���o�_
	const AcGePoint3dArray points	//(i)�_��
	)
{
	bool bFind = false;
	
	for( int n = 0; n < points.length() && !bFind; n++ ){
		if( pntTarget.isEqualTo( points[n], jhOption::Instance().GetTol() ) )
			bFind = true;
	}

	return ( bFind );
}

//	�ŏ������v�Z
double jhUtility::CalcMinDistPoint( 
	AcGePoint3d& pntCalc,			//(o)�Z�o���W
	const AcGePoint3d& pnt,			//(i)��_
	const AcGePoint3dArray& points	//(i)�_��
	)
{
	double dMin = DBL_MAX;

	for( int n = 0; n < points.length(); n++ ){
		double dDist = pnt.distanceTo( points[n] );

		if( LargerThan( dMin, dDist ) ){
			dMin = dDist;
			pntCalc = points[n];
		}
	}

	return ( dMin );
}

//	��[�_���ݔ���
bool jhUtility::ExistNotEdgePoint( 
	const AcDbCurve* pCurve,			//(i)�v�f
	const AcGePoint3dArray& points		//(i)�_��
	)
{
	bool bExist = false;
	AcGePoint3d pntS, pntE;
	
	pCurve->getStartPoint( pntS );
	pCurve->getEndPoint( pntE );

	for( int n = 0; n < points.length() && !bExist; n++ ){
		AcGePoint3d intersect = points[n];

		if(		!pntS.isEqualTo( intersect, jhOption::Instance().GetTol() ) 
			&&	!pntE.isEqualTo( intersect, jhOption::Instance().GetTol() ) ){
#ifdef _DEBUG
			acutPrintf(_T("\n pntS(%.4lf,%.4lf)"), pntS.x, pntS.y);
			acutPrintf(_T("\n pntE(%.4lf,%.4lf)"), pntE.x, pntE.y);
			acutPrintf(_T("\n intersect(%.4lf,%.4lf)"), intersect.x, intersect.y);
#endif
			bExist = true;
		}
	}
	
	return ( bExist );
}

//	���W��`������
bool jhUtility::IsIn( 
	const AcGePoint3d& pnt,		//(i)������W
	const AcGePoint3d& pntMin,	//(i)��`�������W
	const AcGePoint3d& pntMax	//(i)��`�E����W
	)
{
	bool bResult = true;

	if(		Larger( pnt.x, pntMax.x ) || Larger( pnt.y, pntMax.y ) 
		||	Smaller( pnt.x, pntMin.x ) || Smaller( pnt.y, pntMin.y ) )
		bResult = false;

	return ( bResult );
}

//	���W������Ԃ�
CString jhUtility::GetCodString( 
	const AcGePoint3d& pnt	//(i)���W
	)
{
	CString strCod;
	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
	wostringstream ostr;
#else
	ostrstream ostr;
#endif

	ostr.setf( ios::fixed, ios::floatfield );	
	ostr.precision( JHC_LOG_PRECISION );

	//	�[�_���W������擾		
	ostr << _T('(') << pnt.x  << JHC_LOG_COD_SEP << pnt.y << _T(')') << ends;

	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
	strCod = ostr.str().c_str();
#else
	strCod = ostr.str();
	ostr.rdbuf()->freeze(0);
#endif

	return ( strCod );
}

//	���W������Ԃ�
CString jhUtility::GetCodString2( 
	const AcGePoint3d& pnt	//(i)���W
	)
{
	CString strCod;
	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
	wostringstream ostr;
#else
	ostrstream ostr;
#endif

	ostr.setf( ios::fixed, ios::floatfield );	
	ostr.precision( JHC_LOG_PRECISION );

	//	�[�_���W������擾		
	ostr << pnt.x  << JHC_LOG_COD_SEP << pnt.y << ends;

	//--2008/1/30 Unicode �Ή�
#ifdef _UNICODE
	strCod = ostr.str().c_str();
#else
	strCod = ostr.str();
	ostr.rdbuf()->freeze(0);
#endif

	return ( strCod );
}

//	�A�v���P�[�V�����L�[��Ԃ�
CString jhUtility::GetAppKey()
{
	resbuf rb;
	CString strAppKey;

	acedGetVar( _T("ACADVER"), &rb );

	if( rb.resval.rstring != NULL ){
		//--2008/1/30 Unicode �Ή�
		//double dVer = atof(rb.resval.rstring);
		double dVer = _tstof(rb.resval.rstring);
		atmap<double,atstring> mapVer;
		atmap<double,atstring>::const_iterator it;

		free(rb.resval.rstring);

		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2000, JHC_APP_KEY_2000) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2000I, JHC_APP_KEY_2000I) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2002, JHC_APP_KEY_2002) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2004, JHC_APP_KEY_2004) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2005, JHC_APP_KEY_2005) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2006, JHC_APP_KEY_2006) );
		//*Acad�o�[�W�����A�b�v�Ή�
		//-- 2008/1/31 H.Suzuki after update begin
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2007, JHC_APP_KEY_2007) );
		mapVer.insert( atpair<double,atstring>(JHC_ACAD_SYSVAR_VER2008, JHC_APP_KEY_2008) );
		//-- 2008/1/31 H.Suzuki after update end

		it = mapVer.find( dVer );
		if( it != mapVer.end() )
			strAppKey = it->second.c_str();
	}
	
	return ( strAppKey );
}

//	���o�t�@�C���p�X���擾����
Jhc::Result jhUtility::GetFindFilePath( 
	LPTSTR lpszFilePath //(o)Ini�t�@�C���p�X
	)
{
	Jhc::Result result = Jhc::rOk;
	jhRegistry reg;
	TCHAR szInstPath[MAX_PATH];

	reg.SetKey( ACC_REG_KEY );

	//	���W�X�g���f�[�^���擾����
	//--2008/1/30 Unicode �Ή�
	//result = reg.ReadData( szInstPath, ACC_REG_DATA_INSTALL_PATH, sizeof(szInstPath) );
	result = reg.ReadData( szInstPath, ACC_REG_DATA_INSTALL_PATH, sizeof(szInstPath)/sizeof(TCHAR) );
	if( result == Jhc::rOk )
		_tmakepath( lpszFilePath, NULL, szInstPath, ACC_FIND_FILE_NAME, NULL );

	return ( result );
}
