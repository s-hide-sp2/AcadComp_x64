// ACCommandReactor.cpp: ACCommandReactor �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACCommandReactor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "DialogCompDwg.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACCommandReactor::ACCommandReactor()
{

}

ACCommandReactor::~ACCommandReactor()
{

}

void ACCommandReactor::commandCancelled(const TCHAR * cmdStr)//2006.07.20
{
	if (_tcsicmp(cmdStr, _T("zoom")) == 0)//2006.07.20
		DialogCompDwg::HighlightEnd();
	acutPrintf(_T("\ncommandCancelledEnded %s"), cmdStr);//2006.07.20
	return;
}

// �I���O�Ƀg�����U�N�V����������������B
void ACCommandReactor::commandEnded(const TCHAR * cmdStr)//2006.07.20
{
	if (_tcsicmp(cmdStr, _T("zoom")) == 0)//2006.07.20
		DialogCompDwg::HighlightEnd();
	acutPrintf(_T("\ncommandEnded %s"), cmdStr);//2006.07.20
	return;
}

void ACCommandReactor::commandFailed(const TCHAR * cmdStr)//2006.07.20
{
	if (_tcsicmp(cmdStr, _T("zoom")) == 0)//2006.07.20
		DialogCompDwg::HighlightEnd();
	acutPrintf(_T("\ncommandFailed %s"), cmdStr);//2006.07.20
	return;
}
