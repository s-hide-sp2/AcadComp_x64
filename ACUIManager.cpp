// ACUIManager.cpp: ACUIManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACUIManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACUIManager::ACUIManager()
{

}

ACUIManager::~ACUIManager()
{

}

// �����L�[�������ꂽ���ɏI������B
void ACUIManager::ReqSomeKey(LPCTSTR lpctMsg) 
{
	ads_point aptSel;
	//int iRes;
	
	//acutPrintf("\n'zoom\n"); //###


	//iRes = 
	acedGetPoint(NULL, lpctMsg, aptSel);

	acutPrintf(_T("\n�����: "));//2006.07.20
}
