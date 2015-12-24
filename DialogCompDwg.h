#if !defined(AFX_DIALOGCOMPDWG_H__B75418AF_4A76_4D62_B9E0_4C0ADFF01290__INCLUDED_)
#define AFX_DIALOGCOMPDWG_H__B75418AF_4A76_4D62_B9E0_4C0ADFF01290__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogCompDwg.h : �w�b�_�[ �t�@�C��
//

//[ADD]???
#include "resource.h"
#include "ListGrid.h"
#include "ACCommandReactor.h"
#include "ACRetType.h"

//#define DCD_DUMMY_COMMAND "11ss\n"
#define DCD_DUMMY_COMMAND _T("\033\033")
#define DCD_MAX_OUTPUT    2000
#define DCD_INVALID       -1

/////////////////////////////////////////////////////////////////////////////
// DialogCompDwg �_�C�A���O



class DialogCompDwg : public CDialog
{
// �R���X�g���N�V����
public:
	static void CreateInstance();
	static void DeleteInstance();

	// ���[�U�[�̃Y�[���������I�������Ƃ��Ƀ��A�N�^����Ăяo�����B
	static void HighlightEnd();

	DialogCompDwg(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^	

// �_�C�A���O �f�[�^
	//{{AFX_DATA(DialogCompDwg)
	enum { IDD = IDD_DLG_COMP };
	CButton	m_chkRectComp;
	CButton	m_statCsv;
	CButton	m_statComp;
	CButton	m_btnCsv;
	CButton	m_chkResDwg;
	CButton	m_chkExcel;
	CButton	m_stat2;
	CButton	m_stat1;
	CStatic	m_lbl1;
	CStatic	m_lbl2;
	CButton	m_btnCancel;
	CButton	m_btnSel2;
	CButton	m_btnSel1;
	CButton	m_btnRef2;
	CButton	m_btnRef1;
	CButton	m_btnComp;
	CComboBox	m_cboList2;
	CComboBox	m_cboList1;
	ListGrid	m_lstGrid;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(DialogCompDwg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(DialogCompDwg)	
	virtual BOOL OnInitDialog();
	afx_msg void OnColumnclickListGrid(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg LONG onAcadKeepFocus(UINT, LONG);
	afx_msg LRESULT onAcadKeepFocus(WPARAM, LPARAM);
	afx_msg void OnItemclickListGrid(NMHDR* pNMHDR, LRESULT* pResult);	
	afx_msg void OnBtnComp();
	afx_msg void OnDropdownCboDwgList1();
	afx_msg void OnDropdownCboDwgList2();
	virtual void OnCancel();
	afx_msg void OnBtnSel();
	afx_msg void OnBtnSel2();
	afx_msg void OnBtnRef1();
	afx_msg void OnBtnRef2();
	afx_msg void OnMnuSetting();		
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnBtnCsv();		
	afx_msg void OnBtnTest1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// ��r���ʂ�CSV���w�肵���p�X�ɏo�͂���B
	void WriteCSV(LPCTSTR strCSVPath);

	// ���[�h���X�t�H�[���̃C���X�^���X��ێ�����B
	static DialogCompDwg *m_pDlgCompDwg;

	// �R���{�{�b�N�X�ɍ��ڂ�ǉ�����B
	void SetComboItem(CComboBox &cboList, AcDbObjectIdArray &idaryBlkTbl);	

	// �O���b�h�őI�𒆂̍s�ɑΉ�����}�`���n�C���C�g����B
	//  rceDwgType: FIRST: dwg1�̐}�`�ASECOND: dwg2�̐}�`
	bool SelEntities(ResultCompEntity::RCE_FIELD_OBJID rceDwgType); 

	// �t�@�C���I���_�C�A���O�{�b�N�X��\������B���[�U�[���I�������p�X��Ԃ��B
	bool SelectFileDialog(LPCTSTR strTitle, LPCTSTR strDefaultPath, CString &strRetPath); 

	// ���O��t���ĕۑ��_�C�A���O�{�b�N�X��\������B���[�U�[���I�������p�X��Ԃ��B
	bool SaveFileDialog(LPCTSTR strTitle, LPCTSTR strDefaultPath, LPCTSTR strDefaultExt, CString &strRetPath);	
	
	void OnBtnSel0(ResultCompEntity::RCE_FIELD_OBJID);
	
	// �I���s�̃L�[�𓾂�B
	bool GetSelectedKey(CMap<int, int, int, int> &imapRetKey);

	static int CALLBACK SortDataString(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	static int CALLBACK SortDataDouble(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	//bool m_bAsc;
	
	// �E�B���h�E���t�H�[�J�X��ێ��������邩�ۂ�
	static bool m_bFocus; 

	// ��r���ʂ̃f�[�^�Z�b�g
	ResultCompEntity m_rce;
	
	// �t�@�C���I���_�C�A���O���J���B�A���[�U�[�ɐ}�ʂ�I�������A���̐}�ʂ��J��
	bool SelectDwgOpen(CComboBox &cboList, AcDbObjectIdArray &idaryBlkTbl);

	// Acad�h�L�������g�E�B���h�E�����E�ɕ������ĕ\������B
	// iShowDocNum: �\������h�L�������g�̔ԍ� 1 or 2
	void SetDocumentSize(int iShowDocNum);
	
	// �x�����b�Z�[�W�̕\��
	void ShowAlert(LPCTSTR lpszMsg); 

	// �G���[���b�Z�[�W�̕\��
	void ShowError(LPCTSTR lpszMsg); 	

	//// �I�[�g���[�V�������g���ăY�[�������s����B
	//bool AutomationZoom(const AcGePoint2d &pt2LB, const AcGePoint2d &pt2RT);	

	CMenu *m_pMenu;

	// �}�`�n�C���C�g���m�F�ŁA
	// ���[�U�[�̃Y�[���R�}���h�̏I����m�邽�߁A���A�N�^���g���B
	// ���A�N�^����{�N���X�̃n�C���C�g�I���������Ăяo���Ƃ��ɁA�ȉ��̃X�^�e�B�b�N�����o���g���B
	//static bool ms_bUserZoom;
	//static CMap<int, int, AcDbObjectId, AcDbObjectId> ms_idmapHighlight;
	//static DialogCompDwg *ms_pthis;
	//static ACCommandReactor *ms_pCmdReac;
	//static AcApDocument *ms_pDocUserZoom;

	CMap<int, int, int, int> m_imapTest;
	void TestMakeMap(CMap<int, int, int, int> &imapRet) ;
	int m_iPreWidth, m_iPreHeitht;

	// �e�h�L�������g�����ʂ��邽�߁A�f�[�^�x�[�X�̃u���b�N�e�[�u����ID����g���B
	AcDbObjectIdArray m_idaryBlkTbl1;
	AcDbObjectIdArray m_idaryBlkTbl2;
	//CMap<int, int, AcDbObjectId, AcDbObjectId> m_mappBlkTblId;

	void SetControlPosition(
		CWnd &wndControl, 
		int iRightDist  = DCD_INVALID,  // �E�B���h�E�̉E����̋��� �R���g���[���̕����l������B�w�肵�Ȃ��ꍇ�́ADCD_INVALID��n���B
		int iLeftDist   = DCD_INVALID,  // �E�B���h�E�̍�����̋��� �w�肵�Ȃ��ꍇ�́ADCD_INVALID��n���B
		int iTopDist    = DCD_INVALID,  // �E�B���h�E�̏ォ��̋��� �w�肵�Ȃ��ꍇ�́ADCD_INVALID��n���B
		int iBottomDist = DCD_INVALID,  // �E�B���h�E�̉�����̋��� �R���g���[���̍������l������B�w�肵�Ȃ��ꍇ�́ADCD_INVALID��n���B
		int iWidthNew   = DCD_INVALID,  // �R���g���[���̕� �w�肵�Ȃ��ꍇ�́ADCD_INVALID��n���B
		int iHeightNew  = DCD_INVALID   // �R���g���[���̍��� �w�肵�Ȃ��ꍇ�́ADCD_INVALID��n���B
	);
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGCOMPDWG_H__B75418AF_4A76_4D62_B9E0_4C0ADFF01290__INCLUDED_)
