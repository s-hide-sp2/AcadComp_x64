// ACRetType.cpp: ACRetType �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACRetType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int ACRetType::m_i1 = -1;
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACRetType::ACRetType()
{

}

ACRetType::~ACRetType()
{

}

CString ACRetType::GetMsg(ACRETTYPE rType)
{
	CString strMsg;

	switch (rType) {
		case rOk:
			return CString(_T("����I��")); //2006.07.20		
		case rCommandRunning:
			strMsg.Format(_T("�}��%d�̓R�}���h���s���ł��B"), m_i1);//2006.07.20
			return strMsg;
		case rInvalidDwg:
			strMsg.Format(_T("�}��%d�͖����ł��B"), m_i1);//2006.07.20
			return strMsg;
		default:
			strMsg.Empty();
			return strMsg;
	}		
}
	
void ACRetType::SetI1(const int &i)
{
	m_i1 = i;
}


	