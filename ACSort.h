// ACSort.h: ACSort �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACSORT_H__6592ED89_84C9_43AC_A40F_17A5D7802723__INCLUDED_)
#define AFX_ACSORT_H__6592ED89_84C9_43AC_A40F_17A5D7802723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ACCMap.h"

class ACSort  
{
public:
	ACSort();
	virtual ~ACSort();
	
	// ������̃\�[�g
	void QSort(
		CMap<int, int, CString, CString> &strmapSrc, 
		CMap<int, int, int, int> &lmapRet);
	
	// �����A�~�����w�肷��Btrue: ����, false: �~��
	void SetAsc(bool bAsc);

private:
	int m_iAsc;   // 1: �����A -1: �~��
	
	// �N�C�b�N�\�[�g�i�ċA�p�j
	void QSortRecur(
		CMap<int, int, CString, CString> &strmapSrc, 
		CMap<int, int, int, int> &imapRet, int iLow, int iHigh);	
	
};	

#endif // !defined(AFX_ACSORT_H__6592ED89_84C9_43AC_A40F_17A5D7802723__INCLUDED_)
