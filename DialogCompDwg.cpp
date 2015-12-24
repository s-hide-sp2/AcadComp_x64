// DialogCompDwg.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "StdAfx.h"
#include "resource.h"
#include "DialogCompDwg.h"

//[ADD]
#include "ResultRecord.h"
#include "LVSortInfo.h"
#include "ACDocManager.h"
#include "ACUIManager.h"
#include "DialogSetting.h"
//#include "acad.h"
#include "ACFinally.h"
#include "ACErrorInfo.h"
#include <io.h>
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �ÓI�����o�̏�����
DialogCompDwg *DialogCompDwg::m_pDlgCompDwg = NULL;

//bool DialogCompDwg::ms_bUserZoom = false;
bool DialogCompDwg::m_bFocus = true; 
//DialogCompDwg *DialogCompDwg::ms_pthis = NULL;
//CMap<int, int, AcDbObjectId, AcDbObjectId> DialogCompDwg::ms_idmapHighlight;
//ACCommandReactor *DialogCompDwg::ms_pCmdReac = NULL;
//AcApDocument *DialogCompDwg::ms_pDocUserZoom = NULL;
/////////////////////////////////////////////////////////////////////////////
// DialogCompDwg �_�C�A���O


DialogCompDwg::DialogCompDwg(CWnd* pParent /*=NULL*/)
	: CDialog(DialogCompDwg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DialogCompDwg)
	//}}AFX_DATA_INIT

	//m_bFocus = true;
	
}


void DialogCompDwg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DialogCompDwg)
	DDX_Control(pDX, IDC_CHK_RECTCOMP, m_chkRectComp);
	DDX_Control(pDX, IDC_STAT_CSV, m_statCsv);
	DDX_Control(pDX, IDC_STAT_COMP, m_statComp);
	DDX_Control(pDX, IDC_BTN_CSV, m_btnCsv);
	DDX_Control(pDX, IDC_CHK_RESDWG, m_chkResDwg);
	DDX_Control(pDX, IDC_CHK_EXCEL, m_chkExcel);
	DDX_Control(pDX, IDC_STAT_2, m_stat2);
	DDX_Control(pDX, IDC_STAT_1, m_stat1);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SEL2, m_btnSel2);
	DDX_Control(pDX, IDC_BTN_SEL, m_btnSel1);
	DDX_Control(pDX, IDC_BTN_REF2, m_btnRef2);
	DDX_Control(pDX, IDC_BTN_REF1, m_btnRef1);
	DDX_Control(pDX, IDC_BTN_COMP, m_btnComp);
	DDX_Control(pDX, IDC_CBO_DWG_LIST2, m_cboList2);
	DDX_Control(pDX, IDC_CBO_DWG_LIST1, m_cboList1);
	DDX_Control(pDX, IDC_LIST_GRID, m_lstGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DialogCompDwg, CDialog)
	//{{AFX_MSG_MAP(DialogCompDwg)		
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_GRID, OnColumnclickListGrid)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, onAcadKeepFocus)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST_GRID, OnItemclickListGrid)	
	ON_BN_CLICKED(IDC_BTN_COMP, OnBtnComp)
	ON_CBN_DROPDOWN(IDC_CBO_DWG_LIST1, OnDropdownCboDwgList1)
	ON_CBN_DROPDOWN(IDC_CBO_DWG_LIST2, OnDropdownCboDwgList2)
	ON_BN_CLICKED(IDC_BTN_SEL, OnBtnSel)
	ON_BN_CLICKED(IDC_BTN_SEL2, OnBtnSel2)
	ON_BN_CLICKED(IDC_BTN_REF1, OnBtnRef1)
	ON_BN_CLICKED(IDC_BTN_REF2, OnBtnRef2)
	ON_COMMAND(ID_MNU_SETTING, OnMnuSetting)				
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_CSV, OnBtnCsv)			
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DialogCompDwg ���b�Z�[�W �n���h��

BOOL DialogCompDwg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	InitCommonControls();

	m_pMenu = new CMenu();
	m_pMenu->LoadMenu(IDR_MNU_COMP);	 
	this->SetMenu(m_pMenu);
	
	// �R���{�{�b�N�X�ɐ}�ʖ��̍��ڂ�������B
	SetComboItem(m_cboList1, m_idaryBlkTbl1);
	SetComboItem(m_cboList2, m_idaryBlkTbl2);
	
	// �R���{2�͈Ⴄ�}�ʂ�I������B
	if (m_cboList2.GetCount() > 1)
		m_cboList2.SetCurSel(1);
	
	// �񍀖ڂ�ݒ肷��B
	m_lstGrid.InitGridCompEntity();
	
	// �t�H�[�J�X�𓾂�B
	m_bFocus = true;

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void DialogCompDwg::OnColumnclickListGrid(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	
	// �\�[�g���\�b�h�ɓn���������쐬
    LVSortInfo sortInfo;    
    sortInfo.m_iIdx = pNMListView->iSubItem;
	sortInfo.m_bAsc = m_lstGrid.m_bOrderAsc[sortInfo.m_iIdx];
	sortInfo.m_pListGrid = &m_lstGrid;
	
	// lParam��U��Ȃ���
	m_lstGrid.SetlParam();

	m_lstGrid.SortItems( SortDataString, (DWORD)&sortInfo);

	m_lstGrid.m_bOrderAsc[sortInfo.m_iIdx] = 
		!m_lstGrid.m_bOrderAsc[sortInfo.m_iIdx];
    // �\�[�g���s��

	//if (sortInfo.m_iIdx == 0) {
	//	m_lstGrid.SortItems( SortDataString, (DWORD)&sortInfo);
	//}

	//else if (sortInfo.m_iIdx == 1) {
	//	m_lstGrid.SortItems( SortDataDouble, (DWORD)&sortInfo);
	//}	

	*pResult = 0;
}

// �_�C�A���O�̃t�H�[�J�X�̐���
//afx_msg LONG DialogCompDwg::onAcadKeepFocus(UINT ui, LONG l)
//{
//	// TRUE: ���[�h���X�t�H�[���Ƀt�H�[�J�X����������B
//	// FALSE: Acad�Ƀt�H�[�J�X��߂��B
//	return m_bFocus;
//}

afx_msg LRESULT DialogCompDwg::onAcadKeepFocus(WPARAM ui, LPARAM l)
{
	// TRUE: ���[�h���X�t�H�[���Ƀt�H�[�J�X����������B
	// FALSE: Acad�Ƀt�H�[�J�X��߂��B
	return m_bFocus;
}

void DialogCompDwg::OnItemclickListGrid(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	*pResult = 0;
}


int CALLBACK DialogCompDwg::SortDataString(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// �P�ԖڂƂQ�Ԗڂɂ́A
    // ��r����f�[�^�̃A�h���X�������Ă���B
    // InsertItem���s�����ۂ�LPARAM�ł���
	
    // lParam1, lParam2�̓O���b�h�A�C�e����
	// �C���f�b�N�X��A�C�e���̃\�[�X�f�[�^�̃|�C���^�����Z�b�g���Ă����B

	// Modeless�Ȃ̂ŁA���̂�������ACAD�̃��C���E�B���h�E���Ƃ��Ă��܂��B
	//DialogCompDwg* pDlg = (DialogCompDwg*)AfxGetMainWnd();

	// �R�Ԗڂɂ́A
    // SortItems�̍Ō�̈����̃A�h���X�������Ă���B
    LVSortInfo *pData = (LVSortInfo *)lParamSort;

	ListGrid *plstGrid = pData->m_pListGrid;
	

	CString str1 = plstGrid->GetItemText(lParam1, pData->m_iIdx);
	CString str2 = plstGrid->GetItemText(lParam2, pData->m_iIdx);

    int nSort = str1.CompareNoCase((LPCTSTR)str2);	

    // ����
    if( pData->m_bAsc)
        return nSort;
	// �~��
	else    
		return nSort * -1;

}

// XXX
int CALLBACK DialogCompDwg::SortDataDouble(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// �P�ԖڂƂQ�Ԗڂɂ́A
    // ��r����f�[�^�̃A�h���X�������Ă���B
    // InsertItem���s�����ۂ�LPARAM�ł���
    double *pdData1 = (double *)lParam1;
    double *pdData2 = (double *)lParam2;

    // �R�Ԗڂɂ́A
    // SortItems�̍Ō�̈����̃A�h���X�������Ă���B
    LVSortInfo *pData = (LVSortInfo *)lParamSort;

    int nSort = (*pdData1 > *pdData2);

    // ����
    if( pData->m_bAsc == TRUE)
    {
        return nSort * -1;
    }

    // �~��
    return nSort;
}

// �R���{�{�b�N�X�ɍ��ڂ�ǉ�����B
void DialogCompDwg::SetComboItem(CComboBox &cboList, AcDbObjectIdArray &idaryBlkTbl)
{		
	// ���ݑI�����Ă��鍀�ڂ̃e�L�X�g��ێ�����B
	CString strCurText;
	if( cboList.GetCurSel() != -1 ) {
		cboList.GetLBText(cboList.GetCurSel(), strCurText);
	}

	// ���e�����Z�b�g����B
	cboList.ResetContent();	
	
	// ���݊J���Ă���}�ʂ̃p�X���X�g�𓾂āA�Z�b�g����B
	ACDocManager docMng;	
	CStringArray saryRet;	
	CUIntArray   uiarySortedIdx;
	
	docMng.GetDocPaths(saryRet, idaryBlkTbl);		
		
	uiarySortedIdx.SetSize(saryRet.GetSize());
	for (int i = 0; i < saryRet.GetSize(); i++) {
		int iRet = cboList.AddString(saryRet[i]);
		if (iRet != CB_ERR)
			uiarySortedIdx.InsertAt(iRet, i);
			
	}
	
	for (int j = 0; j < saryRet.GetSize(); j++) 
		cboList.SetItemData(j, uiarySortedIdx[j]);

	// �J���Ă���}�ʂ��Ȃ��ꍇ�͏I������B
	if (saryRet.GetSize() == 0)
		return;

	// ���Z�b�g�O�ɑI�����Ă������ڂ̃C���f�b�N�X���Z�b�g����B
	// �I�����Ă������ڂ����݂��Ă��Ȃ��ꍇ�͍ŏ��̍��ڂ��Z�b�g����B
	int iSelIdx;
	iSelIdx = cboList.FindString(-1, strCurText);
	
	if (iSelIdx == CB_ERR) 
		iSelIdx = 0;

	cboList.SetCurSel(iSelIdx);	
}

// ��r���s
void DialogCompDwg::OnBtnComp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	ACDocManager docMng;		
	//CAcModuleResourceOverride resOverride;
	try {	
		
		// �J�����g�}�ʂ�ێ�����B
		docMng.SetCurDoc();

		this->BeginWaitCursor();
		// ��r�{�^���̓R�}���h���s���̂ݎg�p�s�Ƃ���B����ȊO�́A���ʂ����݂��A���������ꍇ�̂ݎg�p�ɂ���B
		m_btnComp.EnableWindow(FALSE);		
		m_btnSel1.EnableWindow(FALSE);
		m_btnSel2.EnableWindow(FALSE);
		m_btnCsv.EnableWindow(FALSE);

		// �O���b�h�s���폜����B	
		m_lstGrid.DeleteAllItems();		

		// �I�����Ă���}�ʂ̃`�F�b�N���s���B
		// ��肪�������ꍇ�́A�x�����X���[����B		
		int iCurIdx1, iCurIdx2;		

		if ((iCurIdx1 = m_cboList1.GetCurSel()) == CB_ERR)
			throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��1��I�����Ă��������B")));

		if ((iCurIdx2 = m_cboList2.GetCurSel()) == CB_ERR)
			throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��2��I�����Ă��������B")));
		
		// �}�ʂ̃u���b�N�e�[�u��ID���r���āA�����}�ʂ��ǂ������f����B
		AcDbObjectId idBlkTbl1, idBlkTbl2;
		idBlkTbl1 = m_idaryBlkTbl1[m_cboList1.GetItemData(iCurIdx1)];
		idBlkTbl2 = m_idaryBlkTbl2[m_cboList2.GetItemData(iCurIdx2)];				

		if (!idBlkTbl1.isValid())
			throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��1�͖����ł��B")));
		if (!idBlkTbl2.isValid())
			throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��2�͖����ł��B")));

		if (idBlkTbl1 == idBlkTbl2) 
			throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�Ⴄ�}�ʂ�I�����Ă��������B")));
				
		// �}�ʂ��L���ł��邩�A�R�}���h�g�p���łȂ����̃`�F�b�N
		ACRetType::ACRETTYPE rType;				
		
		if ((rType = docMng.CheckDwg(idBlkTbl1)) != ACRetType::rOk) {	
			ACRetType::SetI1(1);
			throw (new ACErrorInfo(ACErrorInfo::ALERT, ACRetType::GetMsg(rType)));
		}		
		
		if ((rType = docMng.CheckDwg(idBlkTbl2)) != ACRetType::rOk) {	
			ACRetType::SetI1(2);
			throw (new ACErrorInfo(ACErrorInfo::ALERT, ACRetType::GetMsg(rType)));
		}						

		// ��r�͈͂ɂ���S�}�`�̃I�u�W�F�N�gID�𓾂�B
		// �͈͔�r�̎w�肪�����ꍇ�͐}�ʂ̑S�͈͂���擾����B
		CMap<int, int, AcDbObjectId, AcDbObjectId> idmapRet1, idmapRet2;

		// �͈͔�r�̎w�肪����ꍇ
		if (m_chkRectComp.GetCheck() == 1) {
			AcDbObjectId idCompRect1, idCompRect2;
			long lRectCount = 0;
			
			if (!docMng.ChangeToModelSpace(idBlkTbl1.database()))
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��1�̃��f����Ԃւ̐؂�ւ��Ɏ��s���܂����B")));

			if (!docMng.ChangeToModelSpace(idBlkTbl2.database()))
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��2�̃��f����Ԃւ̐؂�ւ��Ɏ��s���܂����B")));

			// ��r�͈͂̃`�F�b�N
			lRectCount = docMng.GetCompRectId(idBlkTbl1, idCompRect1);
			if (lRectCount == 0)
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��1�ɔ�r�͈͂�ݒ肵�Ă��������B")));
			else if (lRectCount > 1)
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��1�̔�r�͈͂���������܂��B")));
			
			lRectCount = 0;			
			lRectCount = docMng.GetCompRectId(idBlkTbl2, idCompRect2);
			if (lRectCount == 0)
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��2�ɔ�r�͈͂�ݒ肵�Ă��������B")));
			else if (lRectCount > 1)
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��2�̔�r�͈͂���������܂��B")));
			
			// 2�̔�r�͈͂̃T�C�Y�`�F�b�N
			AcDbExtents ext1, ext2;
			docMng.GetGeomExtents(idCompRect1, ext1);			
			docMng.GetGeomExtents(idCompRect2, ext2);
			
			ACCompBase acComp;
			
			// AcGeVector3d�^�̔�r
			if (!acComp.IsEqual( 
			  ext1.maxPoint() - ext1.minPoint(), 
			  ext2.maxPoint() - ext2.minPoint()))				
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("��r�͈͂̃T�C�Y���قȂ�܂��B")));
			
			// �͈͔�r���[�h�̃Z�b�g
			docMng.SetRangeCompMode(ext2.minPoint() - ext1.minPoint());

			// ��r�͈͂ɂ���G���e�B�e�B�̃I�u�W�F�N�gID�̔z����擾����B
			docMng.GetRangeEntityIds(idCompRect1, idmapRet1);
			docMng.GetRangeEntityIds(idCompRect2, idmapRet2);

			////########SSS
			//CMap<int, int, AcDbObjectId, AcDbObjectId> idmapRet1B, idmapRet2B;
			//if (!docMng.GetDwgAllEntityIds(idBlkTbl1, idmapRet1)) 
			//	throw (new ACErrorInfo(ACErrorInfo::ALERT, "�}��1�͖����ł��B"));							
			//if (!docMng.GetDwgAllEntityIds(idBlkTbl2, idmapRet2)) 
			//	throw (new ACErrorInfo(ACErrorInfo::ALERT, "�}��2�͖����ł��B"));									
			////########EEE

		}
		// �͈͔�r�̎w�肪�����ꍇ�͐}�ʑS�̂���G���e�B�e�B���擾����B
		else {
			if (!docMng.GetDwgAllEntityIds(idBlkTbl1, idmapRet1)) 
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��1�͖����ł��B")));					

			if (!docMng.GetDwgAllEntityIds(idBlkTbl2, idmapRet2)) 
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��2�͖����ł��B")));									
		}

		//####
		//for (int k = 0; k < 1000; k++) {
		// �I�u�W�F�N�g�n���h���Ń\�[�g����B
		docMng.SortEntities(idmapRet1);
		docMng.SortEntities(idmapRet2);
		
		// �S�Ă̗v�f���폜��A�T�C�Y��ݒ肷��B
		m_rce.RemoveAll();
		m_rce.SetHashTableSize(idmapRet1.GetCount() + idmapRet2.GetCount());  
			
		// ��r���s
		docMng.CompDwgs(idmapRet1, idmapRet2, m_rce);		
		//####
		//}
				
		// �Ⴂ���Ȃ��ꍇ
		if (m_rce.GetCount() == 0) 			
			throw (new ACErrorInfo(ACErrorInfo::INFO, _T("�����}�ʂł��B")));
			//throw (new ACErrorInfo(ACErrorInfo::ALERT, ""));
		
		//////////////////////////////
		// ---- ���ʂ�\������B---- //
		//////////////////////////////
		m_lstGrid.ShowResultCompEntity(m_rce);		

		// �����}�ʍ쐬�`�F�b�N���t���Ă���Ƃ��͍����}�ʂ��쐬����B
		if (m_chkResDwg.GetCheck() == 1) {		
			bool bRes;
			docMng.CreateNewDwg();
			docMng.SetCurDoc();
			bRes = docMng.DrawResultDwg(idBlkTbl2, idBlkTbl1, m_rce, idmapRet2);			
			docMng.UnlockCurDoc();
			docMng.AutomationCommand(_T("zoom e ")); //2006.07.20
			docMng.AutomationCommand(_T("regen "));  //2006.07.20

			if (!bRes)
				throw (new ACErrorInfo(ACErrorInfo::ERR, _T("�����}�ʂ̕`��Ɏ��s���܂����B")));			
			
		}		

		docMng.ReturnCurDoc();
		this->EndWaitCursor();
		m_btnComp.EnableWindow(TRUE);		
		m_btnSel1.EnableWindow(TRUE);
		m_btnSel2.EnableWindow(TRUE);
		m_btnCsv.EnableWindow(TRUE);
		//__except (GetExceptionInformation()) {
	}	
	catch (ACErrorInfo *pEI) {
		CString strCaption;		
						
		this->EndWaitCursor();			
		m_btnComp.EnableWindow(TRUE);				

		this->GetWindowText(strCaption);
		MessageBox(pEI->m_strMsg, strCaption, pEI->GetMBType(pEI->m_eiType));
		//ShowAlert(pEI->m_strMsg);		
		delete pEI;		
	}
	catch (CException *pExc) {
		TCHAR cBuff[256];//2006.07.20
						
		this->EndWaitCursor();
		m_btnComp.EnableWindow(TRUE);				

		pExc->GetErrorMessage(cBuff, 256);
		ShowError(cBuff);
		delete pExc;
	}

//	__finally {
	
}

//void DialogCompDwg::CheckDwgs()
//{}

void DialogCompDwg::OnDropdownCboDwgList1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetComboItem(m_cboList1, m_idaryBlkTbl1);		
}

void DialogCompDwg::OnDropdownCboDwgList2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	SetComboItem(m_cboList2, m_idaryBlkTbl2);			
}

void DialogCompDwg::CreateInstance()
{
	CAcModuleResourceOverride resOverride;	

	/*
	if (m_pDlgCompDwg == NULL) {
		//delete m_pDlgCompDwg;		
		m_pDlgCompDwg = new DialogCompDwg();			
	}
	*/

	if (m_pDlgCompDwg == NULL || m_pDlgCompDwg->m_hWnd == NULL) {
		delete m_pDlgCompDwg;		
		m_pDlgCompDwg = new DialogCompDwg();
//		m_pDlgCompDwg = new DialogCompDwg;
		m_pDlgCompDwg->Create(IDD_DLG_COMP);
	}
		
	//if (m_pDlgCompDwg->IsIconic() == TRUE)
	m_pDlgCompDwg->ShowWindow(SW_SHOWNORMAL);			
}

void DialogCompDwg::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	//CDialog::OnCancel();
	m_bFocus = false;
	
	m_rce.RemoveAll();	
	//ms_idmapHighlight.RemoveAll();
	//ms_idmapHighlight.InitHashTable(0);
	m_pMenu->DestroyMenu();
	m_pDlgCompDwg->DestroyWindow();	
}

void DialogCompDwg::DeleteInstance()
{
	if (m_pDlgCompDwg != NULL) 
		delete m_pDlgCompDwg;
}

// �O���b�h�őI�������}�`���n�C���C�g����B
void DialogCompDwg::OnBtnSel() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnBtnSel0(ResultCompEntity::FIRST);	
}

// �O���b�h�őI�������}�`���n�C���C�g����B
void DialogCompDwg::OnBtnSel2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnBtnSel0(ResultCompEntity::SECOND);
}

void DialogCompDwg::OnBtnSel0(ResultCompEntity::RCE_FIELD_OBJID rceObjType)
{
	// �I���s���Ȃ��ꍇ�͏I������B
	UINT uiSelCount = m_lstGrid.GetSelectedCount();
	if (uiSelCount == 0) {
		ShowAlert(_T("�n�C���C�g����}�`�̍s��I�����Ă��������B"));
		return;
	}
	
	// �t�H�[�J�X��Acad�ɓn��
	m_bFocus = false;		
	CWnd *pWndAcad = AcUiMainWindow();	
	// �E�B���h�E���\���ɂ���B
	this->SetWindowPos(pWndAcad, 
		0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);	
	
	// ���A�N�^�����݂��Ȃ��ꍇ�͍쐬����B
	//	if (ms_pCmdReac == NULL) {
	//			ms_pCmdReac = new ACCommandReactor();
	//			//ACCommandReactor *pCmdReac= new ACCommandReactor();
	//			acedEditor->addReactor(ms_pCmdReac);
	//#ifdef _DEBUG
	//			acutPrintf("\nadd command reactor");
	//#endif
	//		}

	try {
		SelEntities(rceObjType);

		// �t�H�[�J�X���E�B���h�E���ێ�����
		m_bFocus = true;
		// �E�B���h�E���ĕ\������B		
		this->SetWindowPos(NULL, 0, 0, 100, 100, SWP_SHOWWINDOW |SWP_NOMOVE | SWP_NOSIZE);		
	}
	catch (ACErrorInfo *pErrInfo) {
		m_bFocus = true;
		this->SetWindowPos(NULL, 0, 0, 100, 100, SWP_SHOWWINDOW |SWP_NOMOVE | SWP_NOSIZE);
		ShowAlert(pErrInfo->m_strMsg);
		delete pErrInfo;
	}
}

// �I���ł��Ȃ��ꍇ���l����B
// �I���s�ɗL���Ȑ}�`���Ȃ��Ƃ���false��Ԃ��B
bool DialogCompDwg::SelEntities(ResultCompEntity::RCE_FIELD_OBJID rceDwgType) 
{	
	// �I���s�̃L�[�𓾂�B
	CMap<int, int, int, int> imapKey;
	GetSelectedKey(imapKey);

	if (imapKey.GetCount() == 0)
		throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�n�C���C�g����}�`�̍s��I�����Ă��������B")));		

	ACDocManager acDocMng;
	CMap<int, int, AcDbObjectId, AcDbObjectId> idmapTmp1, idmapHighlight1;
	
	// �I���s�ɑΉ�����AcDbObjectId��𓾂�B	
	m_rce.GetSelId(imapKey, rceDwgType, idmapTmp1);

	// ������Ԃɑ�����}�`�݂̂𓾂�B
	if (!acDocMng.GetSameSpaceEntities(idmapTmp1, idmapHighlight1))
		throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�I���ł���}�`�͂���܂���ł����B")));		
		
	ACUIManager acUIMng;		
	AcGePoint2d pt2LB, pt2RT;
	
	// �I�������}�`�͈̔͂��g�������͈͂𓾂�B
	acDocMng.GetEntityExtents(idmapHighlight1, pt2LB, pt2RT);	
	acDocMng.Extents(pt2LB, pt2RT);			

	// ID��̍ŏ��̐}�`�̐}�ʂ��A�N�e�B�u�ɂ��A�������Ԃɐ؂�ւ���B
	if (!acDocMng.ChangeToFirstEntitySpace(idmapHighlight1))
		throw (new ACErrorInfo(ACErrorInfo::ALERT, 
			_T("�Y�[���Ɏ��s���܂����B\n�R�}���h���s���̉\��������܂��B")));				
	
	// �g�������I��}�`�͈͂ŃY�[��			
	acDocMng.AutomationZoom(pt2LB, pt2RT);	

	// �I�������}�`���n�C���C�g����B
	if (!acDocMng.HighlightEntities(idmapHighlight1, true))	
		throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�n�C���C�g�Ɏ��s���܂����B")));				
	
	// ���[�U�[���L�[�������܂ő҂�
	acUIMng.ReqSomeKey(_T("\nEnter�܂���Esc�ŏI��: "));

	// �I�������}�`���A���n�C���C�g����B
	acDocMng.HighlightEntities(idmapHighlight1, false);	

	return true;
}

// �I���s�̃L�[�𓾂�B
bool DialogCompDwg::GetSelectedKey(CMap<int, int, int, int> &imapRetKey)
{
	int iItemIdx = -1;
	int iFlags   = LVNI_ALL | LVNI_SELECTED;
	int i = -1; 	

	imapRetKey.InitHashTable((int)((double)m_lstGrid.GetSelectedCount() * 1.2));
	
	// �I�����Ă���s�̃L�[(���ʃZ�b�g�̃C���f�b�N�X)�𓾂�B
	while ((iItemIdx = m_lstGrid.GetNextItem(iItemIdx, iFlags)) != -1) {
		LVITEM lvItem;
		TCHAR cBuff[20];//2006.07.20
		int iAdd;

		lvItem.mask = LVIF_TEXT; // | LVIF_IMAGE | LVIF_STATE;
		lvItem.iItem = iItemIdx;		
		lvItem.pszText = cBuff;
		lvItem.cchTextMax = sizeof(cBuff) / sizeof(TCHAR); //2006.07.20
		//lvItem.stateMask = 0xFFFF;		
		lvItem.iSubItem = ListGrid::KEY;
		m_lstGrid.GetItem(&lvItem);
						
		iAdd = _ttoi(cBuff);//2006.07.20
		i++;	
		imapRetKey[i] = iAdd;
	}	

	if (i == -1)
		return false;
	else
		return true;
}

void DialogCompDwg::OnBtnRef1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	if (!SelectDwgOpen(m_cboList1, m_idaryBlkTbl1))
		return;

	SetDocumentSize(1);

	// ADD 2004/08/30 E�Y�[���R�}���h�𑗂�B
	ACDocManager docMng;
	docMng.AutomationCommand(_T("zoom e ")); //2006.07.20
}

void DialogCompDwg::OnBtnRef2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������	
	if (!SelectDwgOpen(m_cboList2, m_idaryBlkTbl2))
		return;
	
	SetDocumentSize(2);

	// ADD 2004/08/30 E�Y�[���R�}���h�𑗂�B
	ACDocManager docMng;
	docMng.AutomationCommand(_T("zoom e ")); //2006.07.20
}

// ���[�U�[�ɐ}�ʂ�I�������A���̐}�ʂ��J���B
bool DialogCompDwg::SelectDwgOpen(CComboBox &cboList, AcDbObjectIdArray &idaryBlkTbl)
{
	ACProfileManager proMng;
	CString strRetPath, strDefDir;	
	
	// �v���t�@�C������I�[�v���f�B���N�g���p�X�𓾂�B
	// ���̃p�X���L���łȂ��Ƃ��́A�󕶎����Z�b�g����B
	strDefDir = proMng.GetUserOpenDirPath();
	if (_taccess(strDefDir, 0) == -1)  //2006.07.20
		strDefDir = "";
	
	// �}�ʃt�@�C���I���_�C�A���O��\������B
	if (!SelectFileDialog(_T("�}��̧�ق�I��"), strDefDir, strRetPath)) {		
		// �L�����Z�����ꂽ�ꍇ�́A�A�N�e�B�u�ł������}�ʂ��Ȃ����R�}���h�g�p���ɂȂ邽�߁A
		// �Ӗ��̂Ȃ��R�}���h�𑗂�B
		acDocManager->sendStringToExecute(acDocManager->mdiActiveDocument(), DCD_DUMMY_COMMAND, false, true, false);
		//acDocManager->unlockDocument(acDocManager->mdiActiveDocument());	
		return false;					
	}
	
	this->EnableWindow(FALSE);

	AcApDocument *pDocBefore, *pDocAfter;
	pDocBefore = acDocManager->mdiActiveDocument();	
	// �}�ʂ��J�� �w���v�ɂȂ��֐�(�ȑO�̃o�[�W�����H)
	acDocManager->appContextOpenDocument(strRetPath);	

	pDocAfter = acDocManager->mdiActiveDocument();
	
	// ���ɊJ���Ă����}�ʂ��R�}���h�I����Ԃɂ��邽�߁A�Ӗ��̂Ȃ��R�}���h�𑗂�B

	////// acDocManager->activateDocument(pDocBefore);	
	
	//AcDbDatabase *pDbBefore = pDocBefore->database();
	//acdbSendInitialDwgFileOpenComplete(pDbBefore);
	//acdbHostApplicationServices()->
	
	//acDocManager->sendStringToExecute(pDocBefore, "1sa\t", true, false);

	// Taka 2009/08/03    AutoCAD�N������́ADrawing1 �����Ȃ���Ԃōŏ��̔�r�p�̐}�ʂ��J����
	//                    �ǂ������킯���A��Ŏ擾���� pDocBefore ���A�N�Z�X�s�\�ɂȂ�(Drawing1��������?)�̂ŁA
	//	                  �����J���Ă���Ƃ�����������ʂ�悤�� if �u���b�N�ɓ��ꂽ
	if ( acDocManager->documentCount() != 1){
		acDocManager->sendStringToExecute(pDocBefore, DCD_DUMMY_COMMAND, false, true, false);
	}

	// �V�����J�����}�ʂ��A�N�e�B�u�ɂ���B
	acDocManager->activateDocument(pDocAfter);
	
	// �R���{�{�b�N�X�ɑI�������p�X��\������B
	SetComboItem(cboList, idaryBlkTbl);
	//cboList.SelectString(-1, pDocAfter->fileName());
	cboList.SelectString(-1, pDocAfter->docTitle());
	
	
	// �I���p�X���v���t�@�C���ɃZ�b�g����B
	strDefDir = ACString::GetSegmentR(strRetPath, _T('\\'), false, false);
	strDefDir = strDefDir + _T('\\');
	proMng.SetUserOpenDirPath(strDefDir);
	
	this->EnableWindow(TRUE);
	
	this->SetFocus();

	return true;
}

// �t�@�C���I���_�C�A���O�{�b�N�X��\������B���[�U�[���I�������p�X��Ԃ��B
bool DialogCompDwg::SelectFileDialog(LPCTSTR strTitle, LPCTSTR strDefaultPath, CString &strRetPath) 
{	
	struct resbuf* rbuf;
	bool   bRes = true;
	int    iFlags;

	rbuf = acutNewRb(RTSTR);		
	iFlags = 8 | 16 | 32 | 256 | 512;

	// �_�C�A���O��\������B�L�����Z�����ꂽ�Ƃ���RTERROR���Ԃ�B
	//2006.07.20
	if (acedGetFileNavDialog(strTitle, strDefaultPath, _T("dwg"), _T("ACSELECTDLG"), iFlags, &rbuf) == RTERROR)
		bRes = false;				

	if (bRes)
		strRetPath = rbuf->resval.rstring;	

	acutRelRb(rbuf);

	return bRes;		
}

// ���O��t���ĕۑ��_�C�A���O�{�b�N�X��\������B���[�U�[���I�������p�X��Ԃ��B
bool DialogCompDwg::SaveFileDialog(LPCTSTR strTitle, LPCTSTR strDefaultPath, LPCTSTR strDefaultExt, CString &strRetPath)
{
	struct resbuf* rbuf;
	bool   bRes = true;
	int    iFlags;

	rbuf = acutNewRb(RTSTR);		
	iFlags = 1 | 8 | 16 | 64 | 128 | 256 | 512;  //8 | 16 | 32 | 256 | 512;

	// �_�C�A���O��\������B�L�����Z�����ꂽ�Ƃ���RTERROR���Ԃ�B	
	//2006.07.20
	if (acedGetFileNavDialog(strTitle, strDefaultPath, strDefaultExt, _T("ACSAVEDLG"), iFlags, &rbuf) == RTERROR)
		bRes = false;						

	if (bRes)
		strRetPath = rbuf->resval.rstring;	

	acutRelRb(rbuf);

	return bRes;		
} 

void DialogCompDwg::OnMnuSetting() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	DialogSetting dlgSet;

	dlgSet.DoModal();
}

// Acad�h�L�������g�E�B���h�E�����E�ɕ������ĕ\������B
// iShowDocNum: �\������h�L�������g�̔ԍ� 1 or 2
void DialogCompDwg::SetDocumentSize(int iShowDocNum)
{
	CMDIFrameWnd *pWndFrm = acedGetAcadFrame();	
	CMDIChildWnd *pWndChild;	
	
	pWndChild = pWndFrm->MDIGetActive();	

	
	// �N���C�A���g�̈���擾	
	//RECT rectCli;
	//pWndFrm->GetClientRect(&rectCli);		

	// �R�}���h���C���E�B���h�E�̍�����������Ȃ����߁A��x�E�B���h�E���ő剻���A
	// ���̃T�C�Y�𓾂āA���E�ɓ������������v�Z����B	
	RECT rectChild;	
	pWndChild->ShowWindow(SW_SHOWMAXIMIZED);		
	pWndChild->GetWindowRect(&rectChild);

	pWndChild->ShowWindow(SW_SHOWNORMAL);

	// �ő剻��������-�E�B���h�E�L���v�V�����̍����@���h�L�������g�E�B���h�E�̍����Ƃ���B
	// ���E�̕��ɂ��Ă��X�N���[���o�[���l������B
	//2006.07.20
	pWndChild->SetWindowPos(&wndTopMost, 
		(int)((iShowDocNum - 1) * ceil((double)((rectChild.right - rectChild.left) / 2 - 6))), 
		0, 
		(int)(ceil((double)((rectChild.right - rectChild.left) / 2 - 6))), 
		rectChild.bottom - rectChild.top - 30, 
		SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);	
}


// �n�C���C�g�̏I�������@�R�}���h���A�N�^����Ă΂��B
void DialogCompDwg::HighlightEnd()
{
	//if (!ms_bUserZoom)
	//	return;
	
	// �I���@�\�����s�����}�ʂłȂ��Ƃ��͏I�����������Ȃ��B
	//if (ms_pDocUserZoom != acDocManager->mdiActiveDocument())
	//	return;

	ACDocManager acDocMng;
	// �I�������}�`���n�C���C�g����B
	//acDocMng.HighlightEntities(ms_idmapHighlight, false);	
	
	// �A�N�e�B�u�Ȑ}�ʂ����b�N����B
	//acDocManager->unlockDocument(acDocManager->mdiActiveDocument());

	// �t�H�[�J�X���E�B���h�E���ێ�����
	m_bFocus = true;
	// �E�B���h�E���ĕ\������B
	//if (ms_pthis != NULL)
	//	ms_pthis->SetWindowPos(NULL, 0, 0, 100, 100, SWP_SHOWWINDOW |SWP_NOMOVE | SWP_NOSIZE);
	//m_lstGrid.Invalidate();		
	//ms_bUserZoom = false;
}

void DialogCompDwg::TestMakeMap(CMap<int, int, int, int> &imapRet) 
{	
	int i;

	imapRet.InitHashTable(10000);				

	for (i = 0; i < 9999; i++) 
		imapRet[i] = i;	
}

void DialogCompDwg::ShowAlert(LPCTSTR lpszMsg) 
{
	CString strCaption;

	this->GetWindowText(strCaption);
	MessageBox(lpszMsg, strCaption, MB_OK | MB_ICONWARNING); 	
}

void DialogCompDwg::ShowError(LPCTSTR lpszMsg) 
{
	CString strCaption;

	this->GetWindowText(strCaption);
	MessageBox(lpszMsg, strCaption, MB_OK | MB_ICONERROR); //| MB_ICONHAND); 	
}

void DialogCompDwg::SetControlPosition(
CWnd &cwndControl, int iRightDist, int iLeftDist, int iTopDist, int iBottomDist, int iWidthNew, int iHeightNew)
{
	RECT recWindow, recControl;	
	int iLeft, iTop, iWidth, iHeight;

	this->GetClientRect(&recWindow);	
	cwndControl.GetClientRect(&recControl);

	if (iWidthNew != DCD_INVALID)
		iWidth = iWidthNew;
	else
		iWidth = recControl.left - recControl.right;

	if (iHeightNew != DCD_INVALID)
		iHeight = iHeightNew;
	else
		iHeight = recControl.bottom - recControl.top;

	if (iRightDist != DCD_INVALID) 
		iLeft = recWindow.right - iWidth - iRightDist;
	else if (iLeftDist != DCD_INVALID)
		iLeft = recWindow.left - iLeftDist;
	else
		iLeft = recControl.left;
		
	if (iTopDist != DCD_INVALID) 
		iTop = recWindow.top - iTopDist;
	else if (iBottomDist != DCD_INVALID)
		iTop = recWindow.bottom - iHeight - iBottomDist;
	else
		iTop = recControl.top;
	
	cwndControl.MoveWindow(iLeft, iTop, iWidth, iHeight);

}

void DialogCompDwg::OnSize(UINT nType, int cx, int cy) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������

	RECT rec;
	const int MARGIN_X          = 14;
	const int MARGIN_TOP        = 14;
	const int MARGIN_BOTTOM     = 14;
	const int WIDTH_BUTTON_L    = 100;
	const int WIDTH_BUTTON_S    = 56;
	const int WIDTH_COMBO       = 150;
	const int WIDTH_LABEL       = 34;
	const int WIDTH_CHK         = 100;
	const int HEIGHT_BUTTON     = 22;
	const int HEIGHT_COMBO      = 28;
	const int HEIGHT_MENU       = 32;
	const int HEIGHT_DROP_LIST  = 100;	
	const int HEIGHT_FRAME_TEXT = 14;	
	const int HEIGHT_CHK        = 18;		
	const int PITCH_X           = 12;
	const int PITCH_Y           = 6;
	const int REV_FRAME_Y       = 4;	
	
	int iWidthCombo;
	
	CDialog::OnSize(nType, cx, cy);

	this->GetClientRect(&rec);	
	iWidthCombo = (rec.right - rec.left - 2 * MARGIN_X - 2 * WIDTH_BUTTON_S - 7 * PITCH_X) / 2;		

	m_lstGrid.MoveWindow(
		rec.left + MARGIN_X, 
		rec.top + HEIGHT_FRAME_TEXT + HEIGHT_COMBO + 3 * PITCH_Y, 
		rec.right - rec.left - 2 * MARGIN_X, 
		rec.bottom - rec.top - HEIGHT_COMBO - HEIGHT_COMBO - MARGIN_BOTTOM - HEIGHT_BUTTON - 2 * PITCH_Y 
		  - 4 * PITCH_Y - 2 * HEIGHT_CHK- HEIGHT_FRAME_TEXT / 2);				
	
	m_btnCancel.MoveWindow(
		rec.right - WIDTH_BUTTON_L - MARGIN_X, 
		rec.bottom - MARGIN_BOTTOM - 2 * HEIGHT_CHK - 3 * PITCH_Y - HEIGHT_BUTTON,
		WIDTH_BUTTON_L,  	
		HEIGHT_BUTTON);

	m_btnComp.MoveWindow(
		rec.left + MARGIN_X + PITCH_X, 
		rec.bottom - MARGIN_BOTTOM - 3 * PITCH_Y - 2 * HEIGHT_CHK - HEIGHT_BUTTON,
		WIDTH_BUTTON_L,  	
		HEIGHT_BUTTON);

	m_btnSel1.MoveWindow(
		rec.left + WIDTH_BUTTON_L + MARGIN_X + PITCH_X + 2 * PITCH_X, 
		rec.bottom - MARGIN_BOTTOM - 2 * HEIGHT_CHK - 3 * PITCH_Y - HEIGHT_BUTTON,
		WIDTH_BUTTON_L,  	
		HEIGHT_BUTTON);

	m_btnSel2.MoveWindow(
		rec.left + 2 * WIDTH_BUTTON_L + MARGIN_X + 2 * PITCH_X + 2 * PITCH_X,
		rec.bottom - MARGIN_BOTTOM - 2 * HEIGHT_CHK - 3 * PITCH_Y - HEIGHT_BUTTON,
		WIDTH_BUTTON_L,  	
		HEIGHT_BUTTON);	
		
	m_statComp.MoveWindow(
		rec.left + MARGIN_X, 
		rec.bottom - MARGIN_BOTTOM - 2 * HEIGHT_CHK - HEIGHT_BUTTON - 4 * PITCH_Y - HEIGHT_FRAME_TEXT,
		WIDTH_BUTTON_L + 2 * PITCH_X,  	
		HEIGHT_BUTTON + 2 * HEIGHT_CHK + 4 * PITCH_Y + HEIGHT_FRAME_TEXT + REV_FRAME_Y);

	m_statCsv.MoveWindow(
		WIDTH_BUTTON_L + 6 * PITCH_X + 2 * WIDTH_BUTTON_L, 
		rec.bottom - MARGIN_BOTTOM - 2 * HEIGHT_CHK - HEIGHT_BUTTON - 4 * PITCH_Y - HEIGHT_FRAME_TEXT,
		WIDTH_BUTTON_L + 2 * PITCH_X,  	
		HEIGHT_BUTTON + HEIGHT_CHK + 3 * PITCH_Y + HEIGHT_FRAME_TEXT + REV_FRAME_Y);

	m_chkResDwg.MoveWindow(
		rec.left + MARGIN_X + PITCH_X, 
		rec.bottom - MARGIN_BOTTOM - PITCH_Y - HEIGHT_CHK,
		WIDTH_CHK,  	
		HEIGHT_CHK);

	m_chkRectComp.MoveWindow(
		rec.left + MARGIN_X + PITCH_X, 
		rec.bottom - MARGIN_BOTTOM - 2 * PITCH_Y - 2 * HEIGHT_CHK,
		WIDTH_CHK,  	
		HEIGHT_CHK);

	m_chkExcel.MoveWindow(
		WIDTH_BUTTON_L + 7 * PITCH_X + 2 * WIDTH_BUTTON_L, 
		rec.bottom - MARGIN_BOTTOM - 2 * PITCH_Y - 2 * HEIGHT_CHK,
		WIDTH_CHK,  	
		HEIGHT_CHK);

	m_btnCsv.MoveWindow(
		WIDTH_BUTTON_L + 7 * PITCH_X + 2 * WIDTH_BUTTON_L, 
		rec.bottom - MARGIN_BOTTOM - 2 * HEIGHT_CHK - 3 * PITCH_Y - HEIGHT_BUTTON,
		WIDTH_BUTTON_L,  	
		HEIGHT_BUTTON);

	/////////


	m_btnRef1.MoveWindow(
		rec.left + MARGIN_X + iWidthCombo + 2 * PITCH_X, 
		rec.top + HEIGHT_FRAME_TEXT + PITCH_Y,
		WIDTH_BUTTON_S,  	
		HEIGHT_BUTTON);

	m_btnRef2.MoveWindow(
		rec.right - WIDTH_BUTTON_S - MARGIN_X - 2 * PITCH_Y, 
		rec.top + HEIGHT_FRAME_TEXT + PITCH_Y,
		WIDTH_BUTTON_S,  	
		HEIGHT_BUTTON);		
	
	m_stat1.MoveWindow(
		rec.left + MARGIN_X, 
		rec.top,
		3 * PITCH_X + iWidthCombo + WIDTH_BUTTON_S,  	
		HEIGHT_COMBO + 2 * PITCH_Y + HEIGHT_FRAME_TEXT);

	m_stat2.MoveWindow(
		rec.right - MARGIN_X - WIDTH_BUTTON_S - iWidthCombo - 3 * PITCH_X, 
		rec.top,
		3 * PITCH_X + iWidthCombo + WIDTH_BUTTON_S,  	
		HEIGHT_COMBO + 2 * PITCH_Y + HEIGHT_FRAME_TEXT);		

	m_cboList1.MoveWindow(
		rec.left + MARGIN_X + PITCH_X, 
		rec.top + HEIGHT_FRAME_TEXT + PITCH_Y,
		iWidthCombo,  	
		HEIGHT_COMBO + HEIGHT_DROP_LIST);

	m_cboList2.MoveWindow(
		rec.right - MARGIN_X - WIDTH_BUTTON_S - iWidthCombo - 2 * PITCH_X, 
		rec.top + HEIGHT_FRAME_TEXT + PITCH_Y,
		iWidthCombo,  	
		HEIGHT_COMBO + HEIGHT_DROP_LIST);

		
	m_btnCancel.RedrawWindow();

	m_statCsv.RedrawWindow();

	m_btnComp.RedrawWindow();
	m_btnSel1.RedrawWindow();
	m_btnSel2.RedrawWindow();
	m_btnRef1.RedrawWindow();	
	m_btnRef2.RedrawWindow();
	m_stat1.RedrawWindow();
	m_stat2.RedrawWindow();
	m_cboList1.RedrawWindow();
	m_cboList2.RedrawWindow();
		
	m_statComp.RedrawWindow();	
	
	m_chkResDwg.RedrawWindow();
	m_chkExcel.RedrawWindow();
	m_btnCsv.RedrawWindow();

	m_lstGrid.RedrawWindow();

	this->RedrawWindow();			
}


// ���[�U�[�ɕۑ�����w�肳���A��r���ʂ�CSV���o�͂���B
// @T: CSV�����ɑ��݂��Ă���ꍇ; ���݂��AExcel�ɊJ����Ă���ꍇ�͂ǂ��Ȃ邩�H
// @T: ��r���ʂ��Ȃ��Ƃ��Ɍx��MSG���\������邩�H
// @T: �_�C�A���O�ŃL�����Z�����ꂽ�Ƃ��A�E�F�C�g�J�[�\�������ɖ߂邱�ƁB
void DialogCompDwg::OnBtnCsv() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	ACProfileManager proMng;
	CString strRetPath, strDefDir;	
	
	try {
		this->BeginWaitCursor();
		
		// ��r���ʂ����݂��邩�ǂ����̃`�F�b�N
		if (m_lstGrid.GetItemCount() == 0)
			throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("��r���ʂ�����܂���B")));

		// �v���t�@�C������I�[�v���f�B���N�g���p�X�𓾂�B
		// ���̃p�X���L���łȂ��Ƃ��́A�󕶎����Z�b�g����B
		strDefDir = proMng.GetUserSaveCSVPath();
		if (_taccess(strDefDir, 0) == -1) //2006.07.20
			strDefDir = "";
		
		// �}�ʃt�@�C���I���_�C�A���O��\������B
		if (!SaveFileDialog(_T("���O��t���ĕۑ�"), strDefDir, _T("csv"), strRetPath)) 
			throw (new ACErrorInfo(ACErrorInfo::NONE, ""));
		
		// ���[�U�[���w�肵���t�@�C���ɔ�r���ʂ��������ށB	
		WriteCSV(strRetPath);

		// �I���p�X���v���t�@�C���ɃZ�b�g����B
		//2006.07.20
		strDefDir = ACString::GetSegmentR(strRetPath, _T('\\'), false, false);
		strDefDir = strDefDir + _T('\\');
		proMng.SetUserSaveCSVPath(strDefDir);							
		
		// �`�F�b�N������Ă���Ƃ��̂�Excel���N������B
		if (m_chkExcel.GetCheck() == 1) {
			//2006.07.20
			CString strBootPath = _T("\"") + strRetPath + _T("\"");
			ShellExecute(this->m_hWnd, _T("open"), _T("EXCEL"), strBootPath, NULL, SW_SHOW);
		}

		throw (new ACErrorInfo(ACErrorInfo::NONE, ""));		
	}
	catch (ACErrorInfo *pEI) {
		CString strCaption;		
		
		this->EndWaitCursor();					
		this->GetWindowText(strCaption);
		pEI->ShowMsgDlg(this->m_hWnd, strCaption);
		
		// �_�C�A���O�\����ɃA�N�e�B�u�Ȑ}�ʂ��R�}���h�I����Ԃɂ��邽�߁A���Ӗ��ȃR�}���h�𑗂�B
		AcApDocument *pDocBefore = acDocManager->mdiActiveDocument();
		acDocManager->sendStringToExecute(pDocBefore, DCD_DUMMY_COMMAND, false, true, false);
		this->SetFocus();

		delete pEI;		
	}
	catch (CException *pExc)
	{
		TCHAR cBuff[256];//2006.07.20
		
		this->EndWaitCursor();	

		pExc->GetErrorMessage(cBuff, 256);
		ShowError(cBuff);		

		// �_�C�A���O�\����ɃA�N�e�B�u�Ȑ}�ʂ��R�}���h�I����Ԃɂ��邽�߁A���Ӗ��ȃR�}���h�𑗂�B
		AcApDocument *pDocBefore = acDocManager->mdiActiveDocument();
		acDocManager->sendStringToExecute(pDocBefore, DCD_DUMMY_COMMAND, false, true, false);
		this->SetFocus();

		pExc->Delete();
	}
}



void DialogCompDwg::WriteCSV(LPCTSTR strCSVPath)
{
	CStdioFile cFile;
	CFileException cfExp;	
	int iItemIdx = -1;
	int iFlags   = LVNI_ALL;	
	
	if (!cFile.Open(strCSVPath, CFile::modeWrite | CFile::modeCreate, &cfExp)) {
		CString strErrMsg;
		strErrMsg.Format(_T("�t�@�C���I�[�v���G���[: %d"), cfExp.m_cause);//2006.07.20
		throw (new ACErrorInfo(ACErrorInfo::ERR, strErrMsg));
	}
	
	// �^�C�g���s�̏o��
	CString strTitle;
	for (int i = 0; i < (int)ListGrid::KEY; i++) {   // �n�C�p�[�����N�܂ŏo�͂����B�L�[�͏o�͂���Ȃ��B
		if (i > 0)
		strTitle = strTitle + _T(",");//2006.07.20
		strTitle = strTitle + _T("\"") + m_lstGrid.GetColumnName(i) + _T("\"");//2006.07.20
	}
	strTitle = strTitle + _T("\n"); //2006.07.20
	cFile.WriteString(strTitle);

	// ��r���ʂ̏o��
	while ((iItemIdx = m_lstGrid.GetNextItem(iItemIdx, iFlags)) != -1) {
		CString strLineOutput;	
		for (int j = 0; j < (int)ListGrid::KEY; j++) {
			LVITEM lvItem;
			TCHAR cBuff[DCD_MAX_OUTPUT];//2006.07.20

			lvItem.mask = LVIF_TEXT;
			lvItem.iItem = iItemIdx;		
			lvItem.pszText = cBuff;
			lvItem.cchTextMax = sizeof(cBuff)/sizeof(TCHAR);//2006.07.20			
			lvItem.iSubItem = j;
			m_lstGrid.GetItem(&lvItem);
			
			if (j > 0)
				strLineOutput = strLineOutput + _T(",");

			// ���l�̂Ƃ�
			if (m_lstGrid.GetColType(j) == 0)
				strLineOutput = strLineOutput + cBuff;
			// ������̂Ƃ�
			else
				strLineOutput = strLineOutput + _T("\"") + cBuff + _T("\"");			
			//i++;			
		}		
		strLineOutput += _T("\n");
		cFile.WriteString(strLineOutput);
	}	
	cFile.Close();
}

void DialogCompDwg::OnBtnTest1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	// �R���{�{�b�N�X�őI�����Ă��鍀�ڂ̃C���f�b�N�X�𓾂�B
	int iCurIdx1, iCurIdx2;		

	if ((iCurIdx1 = m_cboList1.GetCurSel()) == CB_ERR)
		throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��1��I�����Ă��������B")));

	if ((iCurIdx2 = m_cboList2.GetCurSel()) == CB_ERR)
		throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("�}��2��I�����Ă��������B")));

	
	
	try {
		// ���ݑI�����Ă���}�ʂ�BlockTable��ID�𓾂�B
		AcDbObjectId idBlkTbl1, idBlkTbl2;

		idBlkTbl1 = m_idaryBlkTbl1[m_cboList1.GetItemData(iCurIdx1)];
		idBlkTbl2 = m_idaryBlkTbl2[m_cboList2.GetItemData(iCurIdx2)];				

		
		ACRetType::ACRETTYPE rType;
		ACDocManager docMng;

		// �}�ʏ�Ԃ̃`�F�b�N
		if ((rType = docMng.CheckDwg(idBlkTbl1)) != ACRetType::rOk) {	
			ACRetType::SetI1(1);
			throw (new ACErrorInfo(ACErrorInfo::ALERT, ACRetType::GetMsg(rType)));
		}		
		
		if ((rType = docMng.CheckDwg(idBlkTbl2)) != ACRetType::rOk) {	
			ACRetType::SetI1(2);
			throw (new ACErrorInfo(ACErrorInfo::ALERT, ACRetType::GetMsg(rType)));
		}						

		// �E�B���h�E���ŏ�������B
		m_bFocus = false;		
		CWnd *pWndAcad = AcUiMainWindow();	
		// �E�B���h�E���\���ɂ���B
		this->SetWindowPos(pWndAcad, 
			0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);	

		// doc1���A�N�e�B�u�ɂ��A2�_�|�C���g��I������B
		AcApDocument *pDoc;
		pDoc = acDocManager->document(idBlkTbl1.database());


		acDocManager->setCurDocument(pDoc, AcAp::kNone, true);	

		ads_point apt1, apt2, aptBase;
		
		int iRes;

		//acedInitGet(RSG_DASH || RSG_OTHER, NULL);

		acedInitGet(RSG_DASH, NULL);
		while(1) {
			iRes = acedGetPoint(NULL, _T("\n�}��1�̔�r�͈͂��w��F1�_�ځF"), apt1);//2006.07.20
			if (iRes == RTNORM) {
				break;
			}
			else if (iRes = RTCAN) {
				throw (new ACErrorInfo(ACErrorInfo::ALERT, _T("\n�I���Ɏ��s���܂����B")));//2006.07.20
			}
		}
		

		//acedInitGet(RSG_DASH || RSG_OTHER, NULL);
		acedInitGet(RSG_DASH, NULL);

		//iRes = acedGetCorner(apt1, "\n�}��1�̔�r�͈͂��w��F2�_�ځF", apt2);
		
		if (acedGetCorner(apt1, _T("\n�}��1�̔�r�͈͂��w��F2�_�ځF"), apt2) == RTCAN) {//2006.07.20
			throw (new ACErrorInfo(ACErrorInfo::ALERT, 
				_T("\n�I���Ɏ��s���܂����B")));//2006.07.20				
		}			
		
		ads_name sset1;
		acedSSGet(_T("W"), apt1, apt2, NULL, sset1); //2006.07.20
		long l, lLen = 0;		
		acedSSLength(sset1, &lLen);

		for (l = 0; l < lLen; l++) {
			ads_name nameEnt;
			AcDbObjectId idEnt;

			acedSSName(sset1, l, nameEnt);
			acdbGetObjectId(idEnt, nameEnt);
			
			AcDbEntity *pEnt;

			if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
	
				pEnt->highlight();
				pEnt->close();
			}


		}


		// doc2���A�N�e�B�u�ɂ��A1�_�|�C���g��I������B	
		pDoc = acDocManager->document(idBlkTbl2.database());
			
		acDocManager->setCurDocument(pDoc, AcAp::kNone, true);	
		if (acedGetPoint(NULL, _T("\n�}��2�̔�r��_���w��F"), aptBase) == RTCAN) {//2006.07.20
			throw (new ACErrorInfo(ACErrorInfo::ALERT, 
				_T("\n�I���Ɏ��s���܂����B")));//2006.07.20			
		}			
		

		//< S
		/*
		acedSSGet("W", aptBase, apt2, NULL, sset1); 		
		lLen = 0;
		acedSSLength(sset1, &lLen);

		for (l = 0; l < lLen; l++) {
			ads_name nameEnt;
			AcDbObjectId idEnt;

			acedSSName(sset1, l, nameEnt);
			acdbGetObjectId(idEnt, nameEnt);
			
			AcDbEntity *pEnt;

			if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
	
				pEnt->highlight();
				pEnt->close();
			}
		}
		*/
		//> E



		// �E�B���h�E���ĕ\������B
		this->SetWindowPos(NULL, 0, 0, 100, 100, SWP_SHOWWINDOW |SWP_NOMOVE | SWP_NOSIZE);		
	}	
	catch (ACErrorInfo *pErrInfo) {
		m_bFocus = true;
		// �E�B���h�E���ĕ\������B
		this->SetWindowPos(NULL, 0, 0, 100, 100, SWP_SHOWWINDOW |SWP_NOMOVE | SWP_NOSIZE);
		ShowAlert(pErrInfo->m_strMsg);
		delete pErrInfo;
	}
}
