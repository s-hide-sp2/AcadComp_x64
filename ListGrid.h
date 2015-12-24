#if !defined(AFX_LISTGRID_H__F5BCA644_A498_40E1_B161_575D9D9DD87D__INCLUDED_)
#define AFX_LISTGRID_H__F5BCA644_A498_40E1_B161_575D9D9DD87D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListGrid.h : �w�b�_�[ �t�@�C��
//

#include <afxcview.h>

// [ADD] 
#include "ResultCompEntity.h"

/////////////////////////////////////////////////////////////////////////////
// ListGrid �r���[

#define LG_NUM_FIELD 19


// �G���e�B�e�B��r�ɓ��������O���b�h�N���X
class ListGrid : public CListCtrl
{
protected:
public:
	ListGrid();           // ���I�����Ɏg�p�����v���e�N�g �R���X�g���N�^
	DECLARE_DYNCREATE(ListGrid)

// �A�g���r���[�g
public:
	
// �I�y���[�V����
public:	

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(ListGrid)
	protected:
	virtual void OnDraw(CDC* pDC);      // ���̃r���[��`�悷�邽�߂ɃI�[�o�[���C�h���܂����B
	//}}AFX_VIRTUAL
	
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	

// �C���v�������e�[�V����
protected:
public:
	virtual ~ListGrid();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(ListGrid)
	
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//void GetClientRect( LPRECT lpRect ) const;

	int GetColCount();
	CString GetColumnName(int iIdx);
	enum LG_FIELD_IDX {
		RESULTALL,
		RESULTGEOM,
		TYPE1,
		TYPE2,
		COLOR1,
		COLOR2,
		LAYER1,
		LAYER2,
		LINETYPE1,
		LINETYPE2,
		LINETYPESCALE1,
		LINETYPESCALE2,
		PLOTSTYLE1,
		PLOTSTYLE2,
		LINEWEIGHT1,
		LINEWEIGHT2,
		HYPERLINK1,
		HYPERLINK2,
		KEY
	};		
	
	// ��̍��ڂ�ݒ肷��B
	void InitGridCompEntity();

	// ���ʂ�\������B
	void ShowResultCompEntity(ResultCompEntity &rce);
	
	// lParam���ăZ�b�g����B
	void SetlParam();

	enum EHighlight {HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW};

	// �e��̏����A�~����ێ�����Btrue: �����Afalse: �~��
	bool m_bOrderAsc[LG_NUM_FIELD];

	int GetColType(int iIdx);
protected:
	int  m_nHighlight;		// Indicate type of selection highlighting

private:
	// �w�肵�����(�����Ă���͈͂�)�S�s�̓��e��\������̂ɕK�v�ȕ����擾����B
	int GetMaxViewColumnWidth(int iCol) const; 
	int m_iColType[LG_NUM_FIELD];
	int m_iColSize[LG_NUM_FIELD];
	CString m_strColName[LG_NUM_FIELD];
	
	CImageList m_imglstIcon;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_LISTGRID_H__F5BCA644_A498_40E1_B161_575D9D9DD87D__INCLUDED_)
