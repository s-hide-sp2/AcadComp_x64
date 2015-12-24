// ACSort.cpp: ACSort �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACSort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACSort::ACSort()
{
	m_iAsc = 1;
}

ACSort::~ACSort()
{

}

void ACSort::QSortRecur(CMap<int, int, CString, CString> &strmapSrc, 
	CMap<int, int, int, int> &imapRet, int iLow, int iHigh)
{    	
	//if (iLow < 0 || iHigh < 0)
	//	return;

    if(iLow >= iHigh) 
		return;

	// 2�����v�f���Ȃ��A���ꂪ���ɂȂ��Ă���Ƃ��͏I������B
	/*
	if (iHigh - iLow == 1) {
		if (m_iAsc * ACCMap::GetKeyVal(strmapSrc, iLow).CompareNoCase(
				ACCMap::GetKeyVal(strmapSrc, iHigh)) <= 0) {
			return;
		}
	}
	*/
	
	int iSep = (int)((iLow + iHigh + 1) / 2);
	CString strSep = "";
	strSep = ACCMap::GetKeyVal(strmapSrc, iSep);	

	int i = iLow; 
	int j = iHigh;

    // ��������������܂ŌJ��Ԃ��܂�
    while(1){
		// ���v�f�ȏ�̃f�[�^��T���܂�
		while(i <= iHigh) {						
			if (m_iAsc * ACCMap::GetKeyVal(strmapSrc, i).CompareNoCase(strSep) >= 0) 
				break;			
			i++;
		}

	    // ���v�f�����̃f�[�^��T���܂�
		while(j >= iLow) {						
			if (m_iAsc * ACCMap::GetKeyVal(strmapSrc, j).CompareNoCase(strSep) <= 0) 				
				break;									
			j--;
		}

		if (i >= j) 
		  break;

		if (ACCMap::GetKeyVal(strmapSrc, j).CompareNoCase(ACCMap::GetKeyVal(strmapSrc, i)) != 0) {
			CString strTmp; 
			int iTmp;	 		
			
			strTmp = ACCMap::GetKeyVal(strmapSrc, i);
			strmapSrc[i] = ACCMap::GetKeyVal(strmapSrc, j);
			strmapSrc[j] = strTmp;		
			
			iTmp = ACCMap::GetKeyVal(imapRet, i);
			imapRet[i] = ACCMap::GetKeyVal(imapRet, j);
			imapRet[j] = iTmp;		
		}
		i++; 
		j--;
	}
    
	QSortRecur(strmapSrc, imapRet, iLow, i - 1);
	QSortRecur(strmapSrc, imapRet, i,    iHigh);
}


// ������̃\�[�g
void ACSort::QSort(CMap<int, int, CString, CString> &strmapSrc, CMap<int, int, int, int> &imapRet)
{	
	
	imapRet.InitHashTable((int)((double)strmapSrc.GetCount() * 1.2));
	for (int i = 0; i < strmapSrc.GetCount(); i++) {
		imapRet[i] = i;
	}

	QSortRecur(strmapSrc, imapRet, 0, strmapSrc.GetCount() - 1);	
}

// �����A�~�����w�肷��Btrue: ����, false: �~��
void ACSort::SetAsc(bool bAsc)
{
	if (bAsc)
		m_iAsc = 1;
	else
		m_iAsc = -1;
}


