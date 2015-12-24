// ACDbSysVar.cpp: ACDbSysVar �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACDbSysVar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACDbSysVar::ACDbSysVar()
{
	m_pDb = NULL;
}

ACDbSysVar::~ACDbSysVar()
{

}

// �V�X�e���ϐ����擾�^�ݒ肷��f�[�^�x�[�X���w�肷��B
void ACDbSysVar::SetDatabase(AcDbDatabase *pDb) 
{
	m_pDb = pDb;
}

// limmax�𓾂�B
bool ACDbSysVar::Getlimmax(AcGePoint2d &pt2Ret)
{
	if (m_pDb == NULL)
		return false;	

	pt2Ret = m_pDb->limmax();

	return true;
}

// limmin�𓾂�B
bool ACDbSysVar::Getlimmin(AcGePoint2d &pt2Ret)
{
	if (m_pDb == NULL)
		return false;	

	pt2Ret = m_pDb->limmin();

	return true;
}
