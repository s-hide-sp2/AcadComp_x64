// ListGrid.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "StdAfx.h"
#include "resource.h"
#include "ListGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ListGrid

IMPLEMENT_DYNCREATE(ListGrid, CListView)

ListGrid::ListGrid()
{
	// �e��̏����A�~����ێ�����Btrue: �����Afalse: �~��
	for (int i = 0; i < LG_NUM_FIELD; i++) 
		m_bOrderAsc[i] = true;
	
	CWinApp *pApp = AfxGetApp();		

	m_imglstIcon.Create( 16, 16, ILC_COLOR4 | ILC_MASK , 5, 1);
		
	int iRet;	
	iRet = m_imglstIcon.Add(pApp->LoadIcon(IDI_RES_EQUAL));				
	iRet = m_imglstIcon.Add(pApp->LoadIcon(IDI_RES_NOT_EQUAL));
	iRet = m_imglstIcon.Add(pApp->LoadIcon(IDI_RES_ONLY1));	
	iRet = m_imglstIcon.Add(pApp->LoadIcon(IDI_RES_ONLY2));			

	// ��̌^���w�肷��B�iCSV�o�͎��Ɏg�p������j
	// 0:���l/1:������
	m_iColType[(int)RESULTALL]		= 0;
	m_iColType[(int)RESULTGEOM]		= 0;
	m_iColType[(int)TYPE1]			= 1;
	m_iColType[(int)TYPE2]			= 1;
	m_iColType[(int)COLOR1]			= 1;
	m_iColType[(int)COLOR2]			= 1;
	m_iColType[(int)LAYER1]			= 1;
	m_iColType[(int)LAYER2]			= 1;
	m_iColType[(int)LINETYPE1]		= 1;
	m_iColType[(int)LINETYPE2]	    = 1;
	m_iColType[(int)LINETYPESCALE1] = 1;
	m_iColType[(int)LINETYPESCALE2] = 1;
	m_iColType[(int)PLOTSTYLE1]		= 1;
	m_iColType[(int)PLOTSTYLE2]		= 1;
	m_iColType[(int)LINEWEIGHT1]	= 1;
	m_iColType[(int)LINEWEIGHT2]	= 1;
	m_iColType[(int)HYPERLINK1]		= 1;
	m_iColType[(int)HYPERLINK2]		= 1;
	m_iColType[(int)KEY]			= 0;
	
	// �񖼂̐ݒ�	
	m_strColName[(int)RESULTALL]	  = _T("����");
	m_strColName[(int)RESULTGEOM]	  = _T("�޵���");
	m_strColName[(int)TYPE1]		  = _T("����1");
	m_strColName[(int)TYPE2]		  = _T("����2");
	m_strColName[(int)COLOR1]		  = _T("�F1");
	m_strColName[(int)COLOR2]		  = _T("�F2");
	m_strColName[(int)LAYER1]		  = _T("ڲ԰1");
	m_strColName[(int)LAYER2]		  = _T("ڲ԰2");
	m_strColName[(int)LINETYPE1]	  = _T("����1");
	m_strColName[(int)LINETYPE2]	  = _T("����2");
	m_strColName[(int)LINETYPESCALE1] = _T("����ړx1");
	m_strColName[(int)LINETYPESCALE2] = _T("����ړx2");
	m_strColName[(int)PLOTSTYLE1]	  = _T("�������1");
	m_strColName[(int)PLOTSTYLE2]	  = _T("�������2");
	m_strColName[(int)LINEWEIGHT1]	  = _T("���̑���1");
	m_strColName[(int)LINEWEIGHT2]	  = _T("���̑���2");
	m_strColName[(int)HYPERLINK1]	  = _T("ʲ�߰�ݸ1");
	m_strColName[(int)HYPERLINK2]	  = _T("ʲ�߰�ݸ2");
	m_strColName[(int)KEY]			  = _T("KEY");

	m_iColSize[(int)RESULTALL]	=	40;
	m_iColSize[(int)RESULTGEOM]	=	50;
	m_iColSize[(int)TYPE1]	=	100;
	m_iColSize[(int)TYPE2]	=	100;
	m_iColSize[(int)COLOR1]	=	60;
	m_iColSize[(int)COLOR2]	=	60;
	m_iColSize[(int)LAYER1]	=	60;
	m_iColSize[(int)LAYER2]	=	60;
	m_iColSize[(int)LINETYPE1]	=	60;
	m_iColSize[(int)LINETYPE2]	=	60;
	m_iColSize[(int)LINETYPESCALE1]	=	90;
	m_iColSize[(int)LINETYPESCALE2]	=	90;
	m_iColSize[(int)PLOTSTYLE1]	=	75;
	m_iColSize[(int)PLOTSTYLE2]	=	75;
	m_iColSize[(int)LINEWEIGHT1]	=	70;
	m_iColSize[(int)LINEWEIGHT2]	=	70;
	m_iColSize[(int)HYPERLINK1]	=	75;
	m_iColSize[(int)HYPERLINK2]	=	75;
	m_iColSize[(int)KEY]	=	0;
	

}

ListGrid::~ListGrid()
{
	//this->~ListGrid();
	m_imglstIcon.DeleteImageList();
}


BEGIN_MESSAGE_MAP(ListGrid, CListCtrl)
	//{{AFX_MSG_MAP(ListGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ListGrid �`��

void ListGrid::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: ���̈ʒu�ɕ`��p�̃R�[�h��ǉ����Ă�������
}

/////////////////////////////////////////////////////////////////////////////
// ListGrid �f�f

#ifdef _DEBUG
void ListGrid::AssertValid() const
{
	CListCtrl::AssertValid();
}

void ListGrid::Dump(CDumpContext& dc) const
{
	CListCtrl::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ListGrid ���b�Z�[�W �n���h��

void ListGrid::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{		
	//CListCtrl::DrawItem(lpDrawItemStruct);	
	
	m_nHighlight = 2;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	int nItem = lpDrawItemStruct->itemID;
	CImageList* pImageList;

	// Save dc state
	int nSavedDC = pDC->SaveDC();

	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);
	
	// Should the item be highlighted	
	//BOOL bHighlight =((lvi.state & LVIS_DROPHILITED) || 
	//				((lvi.state & LVIS_SELECTED) && 
	//				((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS))));
	
	
	// �ォ��t�H�[�J�X�̏�����������B
	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED) || 
					((lvi.state & LVIS_SELECTED) && 
					((1) || (GetStyle() & LVS_SHOWSELALWAYS))));


	// Get rectangles for drawing
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	CRect rcCol( rcBounds ); 


	CString sLabel = GetItemText( nItem, 0 );
	
	// ��w�b�_�̋��E���_�u���N���b�N�����Ƃ��A�I�t�Z�b�g������ƕ�����̕��ɂȂ�Ȃ��B
	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character

	int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;
	//int offset = 0;
	
	CRect rcHighlight;
	CRect rcWnd;
	int nExt;
	switch( 2 )
	{
	case 0: 
		nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
		rcHighlight = rcLabel;
		if( rcLabel.left + nExt < rcLabel.right )
			rcHighlight.right = rcLabel.left + nExt;
		break;
	case 1:
		rcHighlight = rcBounds;
		rcHighlight.left = rcLabel.left;
		break;
	case 2:
		GetClientRect(&rcWnd);
		rcHighlight = rcBounds;
		rcHighlight.left = 0;
		rcHighlight.right = rcWnd.right;
		break;
	default:
		rcHighlight = rcLabel;
	}
	
	// [ADD] ���ʂ��A�F��ݒ肷��B
	// �� �F��lResAll���C���f�b�N�X�Ƃ����z�񂩂瓾��悤�ɂ���B
	sLabel = GetItemText(nItem, 0);
	int lResAll = _ttol(sLabel); //2006.07.20
	COLORREF lColor;
	
	// �������Ȃ�(��)
	if (lResAll == 2) 		
		lColor = RGB(252, 128, 92);
	// �}��1�̂ݑ��݂���B(�D�F)
	else if (lResAll == 3) 
		lColor = RGB(100, 150, 200);		
	// �}��2�̂ݑ��݂���B(��)
	else if (lResAll == 4)
		lColor = RGB(140, 252, 140);		
	else
		lColor = ::GetSysColor(COLOR_WINDOW);


	// Draw the background color
	if( bHighlight )
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
	}
	else 
		//pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_WINDOW)));
		pDC->FillRect(rcHighlight, &CBrush(lColor));
	
	// Set clip region
	rcCol.right = rcCol.left + GetColumnWidth(0);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcCol);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();
	
	// Draw normal and overlay icon
	pImageList = GetImageList(LVSIL_SMALL);
	if (pImageList)
	{
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		pImageList->Draw(pDC, lvi.iImage, 
			CPoint(rcIcon.left + (int)((rcCol.right - rcCol.left) / 2) - 10, rcIcon.top),
			(bHighlight?ILD_BLEND25:0) | ILD_TRANSPARENT | nOvlImageMask );

		//ILD_BLEND50

		//pImageList->Draw(pDC, lvi.iImage, 
		//	CPoint(rcIcon.left + 30, rcIcon.top),
		//	(bHighlight?ILD_BLEND50:0) | ILD_TRANSPARENT | nOvlImageMask );
	}
			
	// Draw item label - Column 0
	rcLabel.left += offset/2;
	rcLabel.right -= offset;
	

	// #### �y����ݒ肷�邱��
	/*
	CPen *ppenOld = pDC->GetCurrentPen();
	CPen penNew;
	int iPosGrid;
	penNew.CreatePen(PS_SOLID, 2, RGB(20, 20, 20));
	ppenOld = pDC->SelectObject(&penNew);		
	//pDC->MoveTo(rcBounds.left, rcBounds.top);
	iPosGrid = rcBounds.left;
	pDC->MoveTo(rcBounds.left, rcBounds.bottom);
	pDC->LineTo(iPosGrid, rcBounds.bottom);
	penNew.DeleteObject();	
	pDC->SelectObject(ppenOld);
	*/

	// #### �y����ݒ肷�邱��
	//CBrush *pBruOld = pDC->GetCurrentBrush();
	//CBrush bruNew; 	
	//bruNew.CreateSolidBrush(RGB(0, 0, 0));	
	//pDC->FrameRect(rcBounds, &bruNew);	
	//bruNew.DeleteObject();
	//pDC->SelectObject(pBruOld);


	// �޵��ر���	
	//rcCol.left = rcCol.right;
		
	RECT rcIco2 = {rcCol.left, rcCol.top, rcCol.right, rcCol.bottom};
	rcIco2.left = rcIco2.right;
	rcIco2.right += GetColumnWidth(1);	

	CRgn rgn2;
	rgn2.CreateRectRgnIndirect(&rcIco2);
	pDC->SelectClipRgn(&rgn2);
	rgn2.DeleteObject();

	sLabel = GetItemText(nItem, 1);
	int lResGeom = _ttol(sLabel); //2006.07.20

	lResGeom = (lResGeom - 1) % 4;

	if (pImageList)
	{
		UINT nOvlImageMask=lvi.state & LVIS_OVERLAYMASK;
		pImageList->Draw(pDC, lResGeom, 
			CPoint(rcIco2.left + (int)((rcIco2.right - rcIco2.left) / 2) - 7, rcIco2.top),
			(bHighlight?ILD_BLEND50:0) | ILD_TRANSPARENT | nOvlImageMask );		
	}
	
	// Draw labels for remaining columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right : rcBounds.right;
	rgn.CreateRectRgnIndirect(&rcBounds);
	pDC->SelectClipRgn(&rgn);
	
	

	for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
	{
		rcCol.left = rcCol.right;
		rcCol.right += lvc.cx;
				
		// Draw the background if needed
		if( m_nHighlight == HIGHLIGHT_NORMAL )
			pDC->FillRect(rcCol, &CBrush(RGB(30, 30, 30)));
			//pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));
			//pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));

		sLabel = GetItemText(nItem, nColumn);
		if (sLabel.GetLength() == 0)
			continue;

		// Get the text justification
		UINT nJustify = DT_LEFT;
		//if (nColumn == LINEWEIGHT1 || nColumn == LINEWEIGHT2)
		//	nJustify = DT_RIGHT;		
		switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_RIGHT:
			nJustify = DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nJustify = DT_CENTER;
			break;
		default:
			break;
		}
		
		rcLabel = rcCol;
		rcLabel.left += offset;
		rcLabel.right -= offset;
		
		if (nColumn == LINETYPESCALE1 || nColumn == LINETYPESCALE2) {
			nJustify = DT_RIGHT;		
		}
			

		if (nColumn != 1)
			pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
				DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

		
		/*
		//####		
		CPen *ppenOld2 = pDC->GetCurrentPen();
		CPen penNew2;
		penNew2.CreatePen(PS_SOLID, 1, RGB(20, 20, 20));
		ppenOld2 = pDC->SelectObject(&penNew2);			
		pDC->MoveTo(iPosGrid, rcBounds.top);
		//pDC->LineTo(rcBounds2.left, rcBounds2.bottom);
		pDC->LineTo(iPosGrid, rcBounds.bottom);
		iPosGrid += lvc.cx;
		penNew2.DeleteObject();	
		pDC->SelectObject(ppenOld2);
		*/

	}
	
	// �O���b�h���̕`��	
	//POINT ptOrg;
	//GetOrigin(&ptOrg);
	int iPosGrid = rcBounds.left;

	CPen *ppenOld = pDC->GetCurrentPen();
	CPen penNew;
	penNew.CreatePen(PS_SOLID, 1, RGB(208, 208, 208));
	ppenOld = pDC->SelectObject(&penNew);
	
	for(int i = 0; GetColumn(i, &lvc); i++) {											
		if (iPosGrid >= 0) {						
			// �c�̐�
			pDC->MoveTo(iPosGrid, rcBounds.top);
			pDC->LineTo(iPosGrid, rcBounds.bottom - 1);			
		}
		else
			pDC->MoveTo(iPosGrid, rcBounds.bottom - 1);
		// ���̐�
		pDC->LineTo(iPosGrid + lvc.cx, rcBounds.bottom - 1);

		// ��̐�
		//pDC->MoveTo(iPosGrid, rcBounds.top);
		//pDC->LineTo(iPosGrid + lvc.cx, rcBounds.top);

		iPosGrid += lvc.cx;	
	}
	
	penNew.DeleteObject();	
	pDC->SelectObject(ppenOld);
	

	
	// Restore dc
	pDC->RestoreDC( nSavedDC );
}


void ListGrid::InitGridCompEntity()
{
	// ���ʂ�\������B
	DWORD dwStyle = GetExtendedStyle();
	
	//####
	//dwStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_EX_FULLROWSELECT; 
	SetExtendedStyle( dwStyle ); 
	
	//####
	SetBkColor(RGB(120, 120, 120));


	int iCol = 0;

	while (DeleteColumn(0) != 0);
	DeleteAllItems();	
		
	LVCOLUMN lvcol;
	lvcol.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;	
	lvcol.fmt = LVCFMT_LEFT;		
	
	for (int i = 0; i < LG_NUM_FIELD; i++)
	{
		lvcol.pszText = (LPTSTR)(LPCTSTR)m_strColName[i];
		lvcol.cx = m_iColSize[i];		
		InsertColumn(i, &lvcol);	
	}

	/*
	lvcol.pszText = "����";
	lvcol.cx = 40;		
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "�޵���";
	lvcol.cx = 50;
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "����1";
	lvcol.cx = 100;
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "����2";
	lvcol.cx = 100;
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "�F1";
	lvcol.cx = 60;
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "�F2";
	lvcol.cx = 60;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "ڲ԰1";
	lvcol.cx = 60;
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "ڲ԰2";
	lvcol.cx = 60;
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "����1";
	lvcol.cx = 60;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "����2";
	lvcol.cx = 60;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "����ړx1";
	lvcol.cx = 90;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "����ړx2";
	lvcol.cx = 90;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "�������1";
	lvcol.cx = 75;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "�������2";
	lvcol.cx = 75;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "���̑���1";
	lvcol.cx = 70;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "���̑���2";
	lvcol.cx = 70;	
	InsertColumn(iCol++, &lvcol);	


	lvcol.pszText = "ʲ�߰�ݸ1";
	lvcol.cx = 75;	
	InsertColumn(iCol++, &lvcol);

	lvcol.pszText = "ʲ�߰�ݸ2";
	lvcol.cx = 75;	
	InsertColumn(iCol++, &lvcol);

	// �}�b�v�̃L�[��ێ�����B��\������
	lvcol.pszText = "KEY";
	lvcol.cx = 0;	
	InsertColumn(iCol++, &lvcol);		
	*/
}

// ���ʃZ�b�g���󂯎��A���ʂ�\������B
void ListGrid::ShowResultCompEntity(ResultCompEntity &rce)
{
	LVITEM item; 	
	item.mask = LVIF_TEXT; // | LVIF_PARAM;
	item.iItem = 0;
	item.iSubItem = 0;			
	
	int iCount = rce.GetStringMap(ResultCompEntity::ENTTYPE1).GetCount();
	int iRes;
	
	SetItemCount(iCount);
	//SetItemCountEx // OwnerDraw�̂Ƃ��͂�����?
	
	// �A�C�R�����\�[�X���Z�b�g
	SetImageList(&m_imglstIcon, LVSIL_SMALL);

	for (int i = 0; i < iCount; i++) {
		ResultCompEntity::RCE_TYPE_RESULT rceRes = ResultCompEntity::INVALID;	
		CString strText = _T("");
		double dVal = 0.0;
		int    iSub = 0;
		
		item.iItem = i;
		
		// LVIF_PARAM�w��͈�s�ɂ�����H
		item.iSubItem = iSub;				
		//item.mask = LVIF_TEXT | LVIF_PARAM;
		item.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
		rce.GetResultMap(ResultCompEntity::COMPALL).Lookup(i, rceRes);			
		strText.Format(_T("%d"), rceRes); //2006.07.20
		item.pszText = (LPTSTR)(LPCTSTR)strText;	
		item.iImage = (((int)rceRes - 1) % 4);
		item.lParam  = i;
		iRes = InsertItem(&item);					
				
		iSub++;
		item.iSubItem = iSub;				
		item.mask = LVIF_TEXT;
		rce.GetResultMap(ResultCompEntity::COMPGEOM).Lookup(i, rceRes);
		strText.Format(_T("%d"), rceRes); //2006.07.20
		item.pszText = (LPTSTR)(LPCTSTR)strText;		
		iRes = SetItem(&item);		
				
		iSub++;
		item.iSubItem = iSub;						
		rce.GetStringMap(ResultCompEntity::ENTTYPE1).Lookup(i, strText);		
		item.pszText = (LPTSTR)(LPCTSTR)strText;		
		iRes = SetItem(&item);				
		 
		iSub++;
		item.iSubItem = iSub;			
		rce.GetStringMap(ResultCompEntity::ENTTYPE2).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);			

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::COLOR1).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);			
		
		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::COLOR2).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::LAYER1).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);			
		
		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::LAYER2).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::LINETYPE1).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::LINETYPE2).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetDoubleMap(ResultCompEntity::LINETYPESCALE1).Lookup(i, dVal);
		strText.Format(_T("%.4f"), dVal); //2006.07.20
		//strText.Format("%f", dVal);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetDoubleMap(ResultCompEntity::LINETYPESCALE2).Lookup(i, dVal);
		strText.Format(_T("%.4f"), dVal); //2006.07.20
		//strText.Format("%f", dVal);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::PLOTSTYLE1).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::PLOTSTYLE2).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::LINEWEIGHT1).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::LINEWEIGHT2).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);		

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::HYPERLINK1).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);

		iSub++;
		item.iSubItem = iSub;				
		rce.GetStringMap(ResultCompEntity::HYPERLINK2).Lookup(i, strText);
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);		


		// ��\���Ń}�b�v�̃L�[��ێ�����B
	    // �\�[�g�i���X�g�R���g���[���̋@�\�ɔC���Ă���j�����ꍇ�ɁA���̃L�[�𓾂邽�߂ɕK�v
		iSub++;
		item.iSubItem = iSub;						
		strText.Format(_T("%d"), i); //2006.07.20	
		item.pszText = (LPTSTR)(LPCTSTR)strText;
		iRes = SetItem(&item);		
		
		
		// �C���[�W
		//iSub++;		
		//item.iSubItem = iSub;						
		//item.mask = LVIF_IMAGE;  					
		//int iImgIdx = (int)rceRes;
		//item.iImage = iImgIdx;							
		//iRes = SetItem(&item);		
	}		

	int iItemCount = GetItemCount();
}

// lParam���ăZ�b�g����B
void ListGrid::SetlParam()
{
	LVITEM item; 									
	int    iCount = GetItemCount();	
	int    iRes;

	for (int i = 0; i < iCount; i++) {				
		item.iItem = i;				
		item.iSubItem = 0;								
		item.mask = LVIF_PARAM;		
		item.lParam  = i;
		iRes = SetItem(&item);					
	}
}



BOOL ListGrid::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

	switch( ((NMHDR*)lParam)->code ) {
//		case HDN_BEGINTRACK:
//			*pResult = TRUE;
//			return TRUE;										

		case HDN_DIVIDERDBLCLICKW:  // -325
			int iCol = pHDN->iItem;						
		
			SetColumnWidth(iCol, GetMaxViewColumnWidth(iCol));
			return TRUE;		
	}
			
	//OutputDebugString(strOut);

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

// �w�肵�����(�����Ă���͈͂�)�S�s�̓��e��\������̂ɕK�v�ȕ����擾����B
int ListGrid::GetMaxViewColumnWidth(int iCol) const 
{	
	CHeaderCtrl *pHdCtrl = ((CListCtrl *)this)->GetHeaderCtrl();
	int iTopIdx       = GetTopIndex();
	int iCountPerPage = GetCountPerPage();
	int iMaxWidth = 1;						

	// �w�b�_�̃L���v�V�����̕��𓾂�B
	HD_ITEM hdItem;

	TCHAR cBuf[256];            
    hdItem.mask = HDI_TEXT | HDI_FORMAT;
    hdItem.pszText = cBuf;
    hdItem.cchTextMax = 255;
	pHdCtrl->GetItem(iCol, &hdItem);
	
	iMaxWidth = GetStringWidth(CString(cBuf));
	iMaxWidth += GetStringWidth(_T("    "));//2006.07.20

	// �w�b�_�̃L���v�V�����ƕ\���s�̕�
	for (int i = 0; i < iCountPerPage; i++) {
		CString strItem = GetItemText(i + iTopIdx, iCol);
		int iWidth = GetStringWidth(strItem);
		
		// �]������������B
		iWidth += GetStringWidth(_T("    "));//2006.07.20

		if (iWidth > iMaxWidth)
			iMaxWidth = iWidth;		
	}	
	
	return iMaxWidth;
}


/*
int ListGrid::GetColumnCount() const {
	// get the header control
	CHeaderCtrl* pHeader = GetHeaderCtrl(); //(CHeaderCtrl*)GetDlgItem(0);
	// return the number of items in it - ie. the number of columns
	return pHeader->GetItemCount();
}

void ListGrid::AutoSizeColumns(int col) {
	// Call this after your list control is filled
	SetRedraw(false);
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? GetColumnCount()-1 : col;

	for (col = mincol; col <= maxcol; col++) {
		SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = GetColumnWidth(col);
		SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = GetColumnWidth(col);
		int wc = MAX(MINCOLWIDTH, MAX(wc1,wc2));
		SetColumnWidth(col, wc);
	}
	// RecalcHeaderTips(); *** uncomment this if you use my header tips method

	SetRedraw(true);
	// Invalidate(); *** uncomment this if you don't use my SetRedraw function
}
*/

// ��̃^�C�v��Ԃ��B0:���l/1:������
int ListGrid::GetColType(int iIdx)
{
	return m_iColType[iIdx];
}

CString ListGrid::GetColumnName(int iIdx)
{
	return m_strColName[iIdx];
}

int ListGrid::GetColCount()
{
	return LG_NUM_FIELD;
}

/*
void ListGrid::GetClientRect( LPRECT lpRect ) const
{
	GetClientRect(lpRect);	
}
*/
 