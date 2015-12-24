// ACCompRect.cpp: ACCompRect �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACCompRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACCompRect::ACCompRect()
{

}

ACCompRect::~ACCompRect()
{

}

bool ACCompRect::DrawCompRect()
{
	ads_point apt1, apt2;	

	acedInitGet(RSG_DASH, NULL);
	//2006.07.20
	if (acedGetPoint(NULL, _T("\n��r�͈͂��w��F1�_�ځF"), apt1) == RTCAN) {
		acutPrintf(_T("\n�I���Ɏ��s���܂����B"));	
		return false;		
	}			

	acedInitGet(RSG_DASH, NULL);
	if (acedGetCorner(apt1, _T("\n��r�͈͂��w��F2�_�ځF"), apt2) == RTCAN) {
		acutPrintf(_T("\n�I���Ɏ��s���܂����B"));	
		return false;		
	}			
		
	// �����ƉE��̓_�����߂�B
	AcGePoint2d ptLB, ptRT;

	if (apt1[0] <= apt2[0]) {
		ptLB[0] = apt1[0];
		ptRT[0] = apt2[0];		
	}
	else {
		ptLB[0] = apt2[0];
		ptRT[0] = apt1[0];		
	}

	if (apt1[1] <= apt2[1]) {
		ptLB[1] = apt1[1];
		ptRT[1] = apt2[1];		
	}
	else {
		ptLB[1] = apt2[1];
		ptRT[1] = apt1[1];		
	}
	
	// �V�K��`�|�����C���̍쐬
	AcGePoint3dArray ptary;

	ptary.setLogicalLength(4);
	ptary.setAt(0, AcGePoint3d(ptLB[0], ptRT[1], 0));
	ptary.setAt(1, AcGePoint3d(ptLB[0], ptLB[1], 0));
	ptary.setAt(2, AcGePoint3d(ptRT[0], ptLB[1], 0));
	ptary.setAt(3, AcGePoint3d(ptRT[0], ptRT[1], 0));

	AcDb2dPolyline *pPLine = new AcDb2dPolyline(
		AcDb::Poly2dType::k2dSimplePoly,
		ptary,
		0,
		Adesk::kTrue);

	// �g���f�[�^�̕t�^
	struct resbuf *pRb, *pTemp; 

	acdbRegApp(APPNAME);
	pRb = acutNewRb(AcDb::kDxfRegAppName);
	// �ŏ���resbuf�̃|�C���^��ێ�����B
	pTemp = pRb;
//	pTemp->resval.rstring = (TCHAR *) malloc(_tcslen(APPNAME) + 1);//2006.07.20
	pTemp->resval.rstring = new TCHAR[_tcslen(APPNAME) + 1];//2006.07.20
	_tcscpy(pTemp->resval.rstring, APPNAME);

	pTemp->rbnext = acutNewRb(AcDb::kDxfXdAsciiString);
	pTemp = pTemp->rbnext;
//	pTemp->resval.rstring = (TCHAR *) malloc(_tcslen(OBJNAME) + 1);//2006.07.20
	pTemp->resval.rstring = new TCHAR[_tcslen(OBJNAME) + 1];//2006.07.20
	_tcscpy(pTemp->resval.rstring, OBJNAME);
	pTemp->rbnext = NULL;

	pPLine->setXData(pRb);
	acutRelRb(pRb);

	ACDatabaseUtility acUtil;
	AcDbObjectId      idPLine;
	
	// �|�����C�����u���b�N�e�[�u���ɓo�^
	acUtil.AppendEntity(pPLine, idPLine);
	
	return true;
}	

void ACCompRect::Test01()
{
	struct resbuf eb1, eb2, eb3;
	TCHAR sbuf1[10], sbuf2[10];//2006.07.20
	
	/*
	eb1.restype = -3; // �g���f�[�^
	eb1.rbnext = &eb2;
	eb2.restype = 1001;
	strcpy(sbuf1, APPNAME);
	eb2.resval.rstring = sbuf1;
	eb2.rbnext = NULL;	
	*/

	/*
	eb1.restype = 0; // Entity�^�C�v
	strcpy(sbuf1, "CIRCLE");
	eb1.resval.rstring = sbuf1;
	eb1.rbnext = NULL;
	*/


	eb1.restype = 0; // Entity Type
	_tcscpy(sbuf1, _T("POLYLINE"));//2006.07.20
	eb1.resval.rstring = sbuf1;
	eb1.rbnext = &eb2;
	eb2.restype = -3; // �g���f�[�^
	eb2.rbnext = &eb3;
	eb3.restype = 1001;
	_tcscpy(sbuf2, APPNAME);//2006.07.20
	eb3.resval.rstring = sbuf2;
	eb3.rbnext = NULL;	

	ads_name sset;
	acedSSGet(_T("X"), NULL, NULL, &eb1, sset);//2006.07.20
	
	long l, lLen = 0;		
	acedSSLength(sset, &lLen);

	for (l = 0; l < lLen; l++) {
		ads_name nameEnt;
		AcDbObjectId idEnt;

		acedSSName(sset, l, nameEnt);
		acdbGetObjectId(idEnt, nameEnt);
		
		AcDbEntity *pEnt;

		if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
			pEnt->highlight();
			pEnt->close();
		}
	}
	acedSSFree(sset);
}

void ACCompRect::Test02()
{
	ads_name nameEnt;
	ads_point apt;

	//2006.07.20
	if (acedEntSel(_T("\n�}�`��I��"), nameEnt, apt) != RTNORM) {
		acutPrintf(_T("\n�I���Ɏ��s���܂����B"));	
		return;
	}

	AcDbObjectId idEnt;
	AcDbEntity *pEnt;

	acdbGetObjectId(idEnt, nameEnt);
	
	if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
		resbuf *pRb = NULL;

		//pRb = pEnt->xData(APPNAME);
		pRb = pEnt->xData(NULL);

		while (pRb != NULL) {
			if (pRb->resval.rstring != NULL) {
				//2006.07.20
				acutPrintf(_T("\nXData: %s"), pRb->resval.rstring);	
			}
			pRb = pRb->rbnext;
		}
		pEnt->close();
	}
	

}

void ACCompRect::DeleteCompRect()
{
	struct resbuf eb1, eb2, eb3;
	TCHAR sbuf1[10], sbuf2[10];//2006.07.20

	eb1.restype = 0; // Entity Type
	_tcscpy(sbuf1, _T("POLYLINE"));
	eb1.resval.rstring = sbuf1;
	eb1.rbnext = &eb2;
	eb2.restype = -3; // �g���f�[�^
	eb2.rbnext = &eb3;
	eb3.restype = 1001;
	_tcscpy(sbuf2, APPNAME);//2006.07.20
	eb3.resval.rstring = sbuf2;
	eb3.rbnext = NULL;	

	ads_name sset;
	acedSSGet(_T("X"), NULL, NULL, &eb1, sset);//2006.07.20
	
	long l, lLen = 0;		
	acedSSLength(sset, &lLen);

	for (l = 0; l < lLen; l++) {
		ads_name nameEnt;
		AcDbObjectId idEnt;

		acedSSName(sset, l, nameEnt);
		acdbGetObjectId(idEnt, nameEnt);
		
		AcDbEntity *pEnt;

		if (acdbOpenObject(pEnt, idEnt, AcDb::kForWrite) == Acad::eOk) {
			pEnt->erase();
			pEnt->close();
		}
	}
	acedSSFree(sset);

	if (lLen == 0)
		acutPrintf(_T("\n�폜�����r�͈͂͂���܂���ł����B"));//2006.07.20
	else
		acutPrintf(_T("\n�S�Ă̔�r�͈͂��폜���܂����B"));//2006.07.20

}

long ACCompRect::GetCompRectId(AcDbObjectId &idCompRect)
{
	struct resbuf eb1, eb2, eb3;
	TCHAR sbuf1[10], sbuf2[10];//2006.07.20

	eb1.restype = 0; // Entity Type
	_tcscpy(sbuf1, _T("POLYLINE"));//2006.07.20
	eb1.resval.rstring = sbuf1;
	eb1.rbnext = &eb2;
	eb2.restype = -3; // �g���f�[�^
	eb2.rbnext = &eb3;
	eb3.restype = 1001;
	_tcscpy(sbuf2, APPNAME);//2006.07.20
	eb3.resval.rstring = sbuf2;
	eb3.rbnext = NULL;	

	ads_name sset;
	acedSSGet(_T("X"), NULL, NULL, &eb1, sset);//2006.07.20
	
	long l, lLen = 0;		
	acedSSLength(sset, &lLen);

	idCompRect = NULL;
	for (l = 0; l < lLen; l++) {
		ads_name nameEnt;
		// AcDbObjectId idEnt;

		acedSSName(sset, l, nameEnt);
		if (idCompRect == NULL) {
			acdbGetObjectId(idCompRect, nameEnt);
		}
	}
	acedSSFree(sset);
	return lLen;
}

void ACCompRect::ShowName()
{
	ads_point apt1, apt2;	

	acedInitGet(RSG_DASH, NULL);
	//2006.07.20
	if (acedGetPoint(NULL, _T("\n�I��͈͂��w��F1�_�ځF"), apt1) == RTCAN) {
		acutPrintf(_T("\n�I���Ɏ��s���܂����B"));	
		return;		
	}			

	acedInitGet(RSG_DASH, NULL);
	//2006.07.20
	if (acedGetCorner(apt1, _T("\n�I��͈͂��w��F2�_�ځF"), apt2) == RTCAN) {
		acutPrintf(_T("\n�I���Ɏ��s���܂����B"));	
		return;		
	}				

	/*
	// �����ƉE��̓_�����߂�B
	AcGePoint2d ptLB, ptRT;

	if (apt1[0] <= apt2[0]) {
		ptLB[0] = apt1[0];
		ptRT[0] = apt2[0];		
	}
	else {
		ptLB[0] = apt2[0];
		ptRT[0] = apt1[0];		
	}

	if (apt1[1] <= apt2[1]) {
		ptLB[1] = apt1[1];
		ptRT[1] = apt2[1];		
	}
	else {
		ptLB[1] = apt2[1];
		ptRT[1] = apt1[1];		
	}
	*/

	ads_name sset;
	acedSSGet(_T("C"), apt1, apt2, NULL, sset);//2006.07.20

	long l, lLen = 0;		
	acedSSLength(sset, &lLen);

	if (lLen > 0)
		acutPrintf(_T("\n--show name start--"));//2006.07.20			

	for (l = 0; l < lLen; l++) {
		ads_name nameEnt;
		AcDbObjectId idEnt;

		acedSSName(sset, l, nameEnt);
		acdbGetObjectId(idEnt, nameEnt);
		
		AcDbEntity *pEnt;

		if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
			acutPrintf(_T("\nname=%s"), pEnt->isA()->name());//2006.07.20			
			pEnt->close();
		}
	}	

	acedSSFree(sset);

	if (lLen > 0)
		acutPrintf(_T("\n--show name end--"));//2006.07.20			
}


