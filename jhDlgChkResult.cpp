// jhDlgChkResult.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "resource.h"
#include "jhTypeDef.h"
#include "jhDlgChkResult.h"
#include "jhUtility.h"
#include "jhOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
//--2008/1/30 Unicode �Ή�
//static char THIS_FILE[] = __FILE__;
static TCHAR THIS_FILE[] = __FILE__;
#endif

#define JHC_COL_NO				(0)
#define JHC_COL_FILE_PATH		(1)
#define JHC_COL_CHECK_ITEM		(2)
#define JHC_COL_CHECK_RESULT	(3)
#define JHC_COL_CHECK_CONTENTS	(4)
#define JHC_COL_FIGURE_HANDLE	(5)
#define JHC_COL_ZOOM_WAY		(6)
#define JHC_NUM_OF_COL_CHK_RESULT JHC_COL_ZOOM_WAY+1

jhDlgChkResult jhDlgChkResult::m_dlg;

/////////////////////////////////////////////////////////////////////////////
// jhDlgChkResult �_�C�A���O


jhDlgChkResult::jhDlgChkResult(CWnd* pParent /*=NULL*/)
	: CAcUiDialog(jhDlgChkResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(jhDlgChkResult)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
	SetShow( FALSE );
	memset( &m_rectWnd, 0, sizeof(m_rectWnd) );
	memset( &m_szFilePathInChkCurDwg, 0, sizeof(m_szFilePathInChkCurDwg) );

	m_vecColWidth.resize(JHC_NUM_OF_COL_CHK_RESULT);
	m_vecColWidth[JHC_COL_NO] = 30;
	m_vecColWidth[JHC_COL_FILE_PATH] = 135;
	m_vecColWidth[JHC_COL_CHECK_ITEM] = 135;
	m_vecColWidth[JHC_COL_CHECK_RESULT] = 40;
	m_vecColWidth[JHC_COL_CHECK_CONTENTS] = 260;
#ifdef _DEBUG
	m_vecColWidth[JHC_COL_FIGURE_HANDLE] = 70;
	m_vecColWidth[JHC_COL_ZOOM_WAY] = 10;
#else
	m_vecColWidth[JHC_COL_FIGURE_HANDLE] = 0;
	m_vecColWidth[JHC_COL_ZOOM_WAY] = 0;
#endif

}

void jhDlgChkResult::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(jhDlgChkResult)
	DDX_Control(pDX, IDC_LSV_CHK_RESULT, m_lvChkResult);
	//}}AFX_DATA_MAP
}

jhDlgChkResult& jhDlgChkResult::Instance()
{
	return ( m_dlg );
}

//	������
BOOL jhDlgChkResult::Initialize()
{
	BOOL bResult = TRUE;

	bResult = m_lvChkResult.Initialize();

	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_NO, _T("No"), GetColWidth(JHC_COL_NO) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_FILE_PATH, _T("�t�@�C����"), GetColWidth(JHC_COL_FILE_PATH) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_CHECK_ITEM, _T("�`�F�b�N����"), GetColWidth(JHC_COL_CHECK_ITEM) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_CHECK_RESULT, _T("����"), GetColWidth(JHC_COL_CHECK_RESULT) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_CHECK_CONTENTS, _T("���e"), GetColWidth(JHC_COL_CHECK_CONTENTS) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_FIGURE_HANDLE, _T("�}�`�n���h��"), GetColWidth(JHC_COL_FIGURE_HANDLE) );
	if( bResult )
		bResult = m_lvChkResult.AddColumn( JHC_COL_ZOOM_WAY, _T("�Y�[�����@"), GetColWidth(JHC_COL_ZOOM_WAY) );

	return ( bResult );
}

//	���X�g�r���[�Ƀf�[�^���Z�b�g
Jhc::Result jhDlgChkResult::SetDataToListView()
{
	Jhc::Result result = Jhc::rOk;
	TCHAR szFilePath[MAX_PATH];

	if( !m_lvChkResult.DeleteAllItems() )
		return ( Jhc::rFail );

	//	�}�ʓ��`�F�b�N�̏ꍇ�`�F�b�N���̐}�ʃt�@�C���p�X���Z�b�g
	if( jhOption::Instance().GetCheckCurDwg() )
		SetFilePathInChkCurDwg( curDoc()->fileName() );

	m_lvChkResult.ShowWindow( SW_HIDE );

	//	�}�ʓ��`�F�b�N�p�b�r�u
	if( jhOption::Instance().GetCheckCurDwg() )
		result = jhUtility::GetFilePath( szFilePath, JHC_CHECK_IN_DWG_RESULT_FILE_NAME );
	else
		result = jhUtility::GetFilePath( szFilePath, JHC_CHECK_FOR_DISP_RESULT_FILE_NAME );
	
	if( result == Jhc::rOk ){
		CStdioFile cf( szFilePath, CFile::modeRead | CFile::typeText );
		CString str;

		for( int nRow = 0; cf.ReadString( str ); nRow++ ){
			//--2008/1/30 Unicode �Ή�
			//std::vector<std::string> vecToken;
			std::vector<atstring> vecToken;
			TCHAR szBuff[64];
			
			jhUtility::GetTokens( vecToken, str, _T(',') );

			//	No �Z�b�g
			m_lvChkResult.SetCellText( 0, nRow, _itot(nRow+1, szBuff, 10) );

			//	CSV���e�Z�b�g
			for( int nCol = 0; nCol < vecToken.size(); nCol++ )
				m_lvChkResult.SetCellText( nCol+1, nRow, vecToken[nCol].c_str() );
		}
	}

	m_lvChkResult.ShowWindow( SW_SHOWNA );

	return ( result );
}

//	�_�C�A���O��\������
BOOL jhDlgChkResult::Show()
{
	BOOL bResult = TRUE;
	
	if( !GetShow() ){	
		bResult = Create( IDD_DLG_CHK_RESULT, acedGetAcadFrame());
		if( bResult )
			ShowWindow( SW_SHOWNORMAL );

		SetShow( bResult );
		if( bResult ){
			int width = m_rectWnd.right - m_rectWnd.left;
			int hight = m_rectWnd.bottom - m_rectWnd.top;
		
			if( 0 < width && 0 < hight )
				MoveWindow( m_rectWnd.left, m_rectWnd.top, width, hight );
		}
	}
	else
		bResult = SetDataToListView();

	return ( bResult );
}

//	�}�`�n���h�����L�[�ɃG���e�B�e�C���I�[�v��
Jhc::Result jhDlgChkResult::OpenEntity( 
	AcDbVoidPtrArray& entArray, //(o)�G���e�B�e�B
	LPCTSTR lpszHandle			//(i)�}�`�n���h��
	) const
{
	Jhc::Result result = Jhc::rOk;
	//--2008/1/30 Unicode �Ή�
	//std::vector<std::string> vecHandle;
	std::vector<atstring> vecHandle;
	CString strHandle( lpszHandle );
	
	//	�}�`�n���h�������񂩂�}�`�n���h���擾
	jhUtility::GetTokens( vecHandle, strHandle, JHC_LOG_COD_TO_COD_SEP );
	
	//	�}�`�n���h���ɑΉ�����G���e�B�e�B���I�[�v��
	for( int n = 0; n < vecHandle.size() && result == Jhc::rOk; n++ ){
		AcDbObjectId objId;
		AcDbHandle handle( vecHandle[n].c_str() );
		AcDbEntity* pEnt = NULL;
		Acad::ErrorStatus es;

		//	�}�`�n���h���ɑ΂���I�u�W�F�N�gID�擾
		es = acdbCurDwg()->getAcDbObjectId(objId, Adesk::kFalse, handle);
		//	���ɍ폜����Ă���v�f��}�ʂ������Ă���\��������̂ŃG���[�͖���
		if( es == Acad::eOk && objId.isValid() )
			es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
		if( pEnt != NULL )
			entArray.append( pEnt );
	}
	
	return ( result );
}

void jhDlgChkResult::OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	Jhc::Result result = Jhc::rOk;
	HD_NOTIFY* phdn = (HD_NOTIFY*)pNMHDR;
	int nRow = phdn->iItem;
	CString strFilePath = m_lvChkResult.GetItemText( nRow, JHC_COL_FILE_PATH );
	CString strCheckResult = m_lvChkResult.GetItemText( nRow, JHC_COL_CHECK_RESULT );
	
	//	�`�F�b�N�n�j�̏ꍇ�͉������Ȃ�
	if( strCheckResult == _T("OK") )
		return;

	//	�}�ʓ��`�F�b�N���̓`�F�b�N�����}�ʂ��J�����g�}�ʂɐݒ肷��
	if( jhOption::Instance().GetCheckCurDwg() ){
		if( _tcsicmp( curDoc()->fileName(), GetFilePathInChkCurDwg() ) )
			result = jhUtility::SetCurDocument( GetFilePathInChkCurDwg() );
	}
	//	�`�F�b�N���ʕ\���̏ꍇ�͐}�ʃI�[�v��
	else
		result = jhUtility::OpenDwg( strFilePath );

	if( result == Jhc::rOk ){
		CString strItem = m_lvChkResult.GetItemText( nRow, JHC_COL_CHECK_ITEM );

		if(		strItem != _T("�t�@�C�����`�F�b�N") 
			&&	strItem != _T("���C���`�F�b�N") ){
			CString strZoomWay = m_lvChkResult.GetItemText( nRow, JHC_COL_ZOOM_WAY );
			CString strFigureHandle = m_lvChkResult.GetItemText( nRow, JHC_COL_FIGURE_HANDLE );
			Jhc::ZoomWay zoomWay = (Jhc::ZoomWay)_ttoi(strZoomWay);
			AcGePoint2dArray points;
			
			//	�G���e�B�e�B�̒��S�ɃY�[����������
			if( zoomWay == Jhc::zEntCenter ){
				AcDbVoidPtrArray entArray;
				AcGePoint3d pntMin, pntMax, pntCenter;

				//	�v�f�I�[�v��
				result = OpenEntity( entArray, strFigureHandle );
				if( result == Jhc::rOk ){	
					//	�S�v�f�̋�`�T�C�Y�擾
					jhUtility::GetMinMaxPoints( pntMin, pntMax, entArray );
					//	�G���e�B�e�B�����
					for( int n = 0;  n < entArray.length(); n++ )
						AcDbEntity::cast((AcRxObject*)entArray[n])->close();

					pntCenter = pntMin + 0.5*( pntMax - pntMin );
					points.append( AcGePoint2d( pntCenter.x, pntCenter.y ) );
					Zoom( points, (pntMax.x - pntMin.x)+20.0, (pntMax.y - pntMin.y)+20.0 );
					Highlight( strFigureHandle );
				}
			}
			//	���W�Y�[��
			else{
				CString strContents = m_lvChkResult.GetItemText( nRow, JHC_COL_CHECK_CONTENTS );
				
				//	���e�񂩂���W�擾
				GetPointsFromContents( points, strContents );
				Zoom( points, 30.0, 30.0 );
				Highlight( strFigureHandle );
			}		
		}
	}
	else if( result != Jhc::rCancel )
		AfxMessageBox( jhUtility::GetErrorMessage(result) );
}

//	�n�C���C�g����������
Jhc::Result jhDlgChkResult::Unhighlight()
{
	Jhc::Result result = Jhc::rOk;

	//	�}�ʂ�����Ă���v�f�̃n�C���C�g�͎��s����׃G���[�͖���
	for( int n = 0; n < m_highlightIdArray.length(); n++ ){
		result = Unhighlight( m_highlightIdArray[n] );
		assert( result == Jhc::rOk );
	}
	
	m_highlightIdArray.setLogicalLength(0);

	return ( result );
}

//	�v�f���n�C���C�g����
Jhc::Result jhDlgChkResult::Highlight( 
	CString& strHandles //(i)�}�`�n���h��������
	)
{
	Jhc::Result result = Jhc::rOk;
	//--2008/1/30 Unicode �Ή�
/*	std::vector<std::string> vecHandle;
	std::vector<std::string>::const_iterator it;*/
	std::vector<atstring> vecHandle;
	std::vector<atstring>::const_iterator it;

	//	�O��n�C���C�g�����v�f���n�C���C�g�\���ɂ���
	Unhighlight();

	//	�g�[�N�����擾����
	jhUtility::GetTokens( vecHandle, strHandles, JHC_LOG_COD_TO_COD_SEP );

	for( it = vecHandle.begin(); it != vecHandle.end() && result == Jhc::rOk; it++ )
		result = Highlight( it->c_str() );
			
	//	�r���[�̕\����Ԃ��X�V
	jhUtility::UpdateView();

	return ( result );
}

//	�v�f���n�C���C�g����
Jhc::Result jhDlgChkResult::Highlight( 
	LPCTSTR lpszHandle	//(i)�}�`�n���h��
	)
{
	Jhc::Result result = Jhc::rOk;
	AcDbHandle handle( lpszHandle );
	AcDbObjectId objId;
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	bool bLockDoc = false;
	
	//	�}�`�n���h���ɑ΂���I�u�W�F�N�gID�擾
	es = acdbCurDwg()->getAcDbObjectId(objId, Adesk::kFalse, handle);
	if( es != Acad::eOk )
		return ( Jhc::rCancel );
	
	//	�h�L�������g�����b�N���I�u�W�F�N�g�̍X�V�����𓾂�
	es = acDocManager->lockDocument(curDoc(), AcAp::kXWrite, NULL, NULL, false);

	if( es == Acad::eOk ){
		es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
		bLockDoc = true;
	}

	//	�v�f���n�C���C�g
	if( es == Acad::eOk ){
		AcDbLayerTableRecord* pLayer = NULL;
		
		//	�v�f�̃��C������\���Ȃ�\����Ԃɂ���
		es = acdbOpenAcDbObject( (AcDbObject*&)pLayer, pEnt->layerId(), AcDb::kForRead );
		
		if( es == Acad::eOk ){
			if( pLayer->isOff() ){
				es = pLayer->upgradeOpen();
				if( es == Acad::eOk )
					pLayer->setIsOff( false );
			}
			pLayer->close();
		}
		
		pEnt->highlight();
		pEnt->close();
	}
	
	if( es != Acad::eOk )
		result = Jhc::rAcadError;
	
	//	�n�C���C�g�v�f�h�c�ǉ�
	if( result == Jhc::rOk )
		m_highlightIdArray.append( objId );

	//	�r���[�̕\����Ԃ��X�V
//	jhUtility::UpdateView();

	//	�h�L�������g���b�N����
	if( bLockDoc ){
		es = acDocManager->unlockDocument(curDoc());
		assert( es == Acad::eOk );
	}

	return ( result );
}

//	�n�C���C�g����������
Jhc::Result jhDlgChkResult::Unhighlight( 
	const AcDbObjectId& objId //(i)�I�u�W�F�N�gID
	) const
{
	Jhc::Result result = Jhc::rOk;
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;

	if( objId.isNull() || !objId.isValid() )
		return ( Jhc::rOk );

	es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
	if( es == Acad::eOk ){
		pEnt->unhighlight();
		pEnt->close();
	}
	else
		result = Jhc::rAcadError;

	return ( result );
}

//	�`�F�b�N���e������W���擾����
Jhc::Result jhDlgChkResult::GetEntityGeom( 
	AcGePoint2d& pntCenter, 
	double& dW, 
	double& dH, 
	LPCTSTR lpszHandle 
	) const
{
	Jhc::Result result = Jhc::rOk;
	AcDbHandle handle( lpszHandle );
	AcDbObjectId objId;
	Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	
	//	�}�`�n���h���ɑ΂���I�u�W�F�N�gID�擾
	es = acdbCurDwg()->getAcDbObjectId(objId, Adesk::kFalse, handle);
	if( es != Acad::eOk )
		return ( Jhc::rCancel );
	
	es = acdbOpenAcDbEntity( pEnt, objId, AcDb::kForRead );
	if( es == Acad::eOk ){
		AcDbExtents extents;
		AcGePoint3d pntMin, pntMax, pntCenter3d;
		
		pEnt->getGeomExtents(extents);
		pEnt->close();

		pntMin = extents.minPoint();
		pntMax = extents.maxPoint();
		
		pntCenter3d = pntMin + 0.5*(pntMax-pntMin);
		pntCenter.set( pntCenter3d.x, pntCenter3d.y );
		dW = pntMax.x - pntMin.x;
		dH = pntMax.y - pntMin.y;
	}

	return ( result );
}

BOOL jhDlgChkResult::GetPointsFromContents(
	AcGePoint2dArray& points,	//(o)���W
	CString& strContents		//(i/o)���e
	) const
{
	BOOL bResult = TRUE;
	
	if( strContents.Find(JHC_LOG_COD_SEP) == -1 )
		bResult = FALSE;
	else{
		//std::vector<std::string> vecTokens;
		std::vector<atstring> vecTokens;

		jhUtility::GetTokens( vecTokens, strContents, JHC_LOG_COD_TO_COD_SEP );
		
		for( int n = 0; n < vecTokens.size(); n++ ){
			CString str( vecTokens[n].c_str() );
			double dX, dY;
			
			//--2008/1/30 Unicode �Ή�
		/*	dX = atof( jhUtility::StrTok( str, JHC_LOG_COD_SEP ) );
			dY = atof( jhUtility::StrTok( str, JHC_LOG_COD_TO_COD_SEP ) );*/
			dX = _tstof( jhUtility::StrTok( str, JHC_LOG_COD_SEP ) );
			dY = _tstof( jhUtility::StrTok( str, JHC_LOG_COD_TO_COD_SEP ) );

			points.append( AcGePoint2d(dX, dY) );
		}
	}

	return ( bResult );
}

//	�Y�[������
void jhDlgChkResult::Zoom( 
	const AcGePoint2dArray& points, //(i)�Y�[�����W
	double dW, 
	double dH
	)
{	
    AcDbViewTableRecord view;
	AcGePoint2d pntCenter;

	if( points.isEmpty() )
		return;

	//	�Y�[�����W�̒��S�����߂�
	for( int n = 0; n < points.length(); n++ )
		 pntCenter += points[n].asVector();
	
	pntCenter /= (double)points.length();

	//	�Y�[�����s�Ȃ�
    view.setCenterPoint( pntCenter );
    view.setHeight( dW );
    view.setWidth( dH );
	acedSetCurrentView( &view, NULL );
}

//	�񕝂�Ԃ�
long jhDlgChkResult::GetColWidth( 
	long lCol	//(i)��
	) const
{
	long lWidth = 0;
	
	if( lCol < m_vecColWidth.size() )
		lWidth = m_vecColWidth[lCol];

	if( jhOption::Instance().GetCheckCurDwg() ){
		if( lCol == JHC_COL_NO || lCol == JHC_COL_FILE_PATH )
			lWidth = 0;
	}

	return ( lWidth );
}

//	�I������
void jhDlgChkResult::EndProc()
{
	GetWindowRect(&m_rectWnd);
	for( int n = 0; n < JHC_NUM_OF_COL_CHK_RESULT; n++ ){
		//	�}�ʓ��`�F�b�N����NO�E�t�@�C���p�X�@�̗񕝂͕ێ����Ȃ�
		if(		jhOption::Instance().GetCheckCurDwg() 
			&&  (n == JHC_COL_NO || n == JHC_COL_FILE_PATH) )
			continue;
		
		m_vecColWidth[n] = m_lvChkResult.GetColumnWidth( n );
	}
	
	if( DestroyWindow() )
		SetShow(FALSE);

	//	�O��n�C���C�g�����v�f���n�C���C�g�\���ɂ���
	Unhighlight();
	
	//	�r���[�̕\����Ԃ��X�V
	jhUtility::UpdateView();
}

BEGIN_MESSAGE_MAP(jhDlgChkResult, CAcUiDialog)
	//{{AFX_MSG_MAP(jhDlgChkResult)
	ON_MESSAGE( WM_ACAD_KEEPFOCUS, onAcadKeepFocus )
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// jhDlgChkResult ���b�Z�[�W �n���h��

LONG jhDlgChkResult::onAcadKeepFocus( UINT, LONG )
{
    return TRUE;
}

void jhDlgChkResult::OnClose() 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	EndProc();

	CAcUiDialog::OnClose();
}

void jhDlgChkResult::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	EndProc();

	CAcUiDialog::OnCancel();
}

BOOL jhDlgChkResult::OnInitDialog() 
{
	CAcUiDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	Initialize();

	SetDataToListView();
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void jhDlgChkResult::OnSize(UINT nType, int cx, int cy) 
{
	CAcUiDialog::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	WINDOWPLACEMENT place;
		
	if( m_lvChkResult.GetWindowPlacement( &place ) ){
		int width, hight;
	
		width = cx - 2 * place.rcNormalPosition.left;
		hight = cy - 2 * place.rcNormalPosition.top;
		if( width <= 0 )
			width = 1;
		if( hight <= 0 )
			hight = 1;

		m_lvChkResult.MoveWindow(	place.rcNormalPosition.left, 
									place.rcNormalPosition.top, 
									width, 
									hight );
	}	
}
