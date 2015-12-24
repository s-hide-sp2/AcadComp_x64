// ACDocManager.cpp: ACDocManager �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACDocManager.h"
#include "ACProfileManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ACADTLB

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACDocManager::ACDocManager()
{
	m_pDocCur = NULL;
	m_bRangeCompMode = false;
	m_vec3Gap.set(0, 0, 0);
}

ACDocManager::~ACDocManager()
{

}

// ���݊J���Ă���S�}�ʂ̃p�X�̔z��𓾂�B
void ACDocManager::GetDocPaths(CStringArray &saryRet, AcDbObjectIdArray &idaryBlkTbl)
{	
	AcApDocManager *pDocMng = acDocManager;
	AcApDocumentIterator *pDocItr = pDocMng->newAcApDocumentIterator(); 	
	int iDocCount = pDocMng->documentCount();
	int i = 0;

	saryRet.SetSize(iDocCount);
	
	if (!idaryBlkTbl.isEmpty())
		idaryBlkTbl.removeSubArray(0, idaryBlkTbl.logicalLength());
	idaryBlkTbl.setLogicalLength(iDocCount);	
	
	// ���݊J����Ă���h�L�������g�𑖍�����B
	for (; !pDocItr->done(); pDocItr->step()) {
		AcApDocument *pDoc = pDocItr->document();
		
		saryRet[i] = pDoc->docTitle();
		//saryRet[i] = pDoc->fileName();
		
		idaryBlkTbl[i] = pDoc->database()->blockTableId();

		i++;
	}
	delete pDocItr;
}


// �p�X���w�肵�āA�}�ʃI�u�W�F�N�g�𓾂�B
bool ACDocManager::GetDoc(LPCTSTR lpctDwgPath, AcApDocument *pDocRet)
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	AcApDocManager *pDocMng = acDocManager;
	AcApDocumentIterator *pDocItr = pDocMng->newAcApDocumentIterator(); 		
	bool bRes = false;

	// ���݊J����Ă���h�L�������g�𑖍�����B
	for (; !pDocItr->done(); pDocItr->step()) {
		AcApDocument *pDoc = pDocItr->document();
		CString strDwgPath = pDoc->fileName();

		//acutPrintf("\ndoc: %s", pDoc->fileName());

		if (strDwgPath.CompareNoCase(lpctDwgPath) == 0) {
			pDocRet = (AcApDocument *)pDoc->contextPtr();
			bRes = true;
			break;
		}
	}
	delete pDocItr;	
	return bRes;
}

// �w�肵���}�ʂ��r���A���ʂ𓾂�B
bool ACDocManager::CompDwgs(
	CMap<int, int, AcDbObjectId, AcDbObjectId> &idmap1, 
	CMap<int, int, AcDbObjectId, AcDbObjectId> &idmap2,
	ResultCompEntity &rce)
{
	Acad::ErrorStatus es;
	int i = 0, j = 0, iIdx = 0;
	int iMax1 = idmap1.GetCount(), iMax2 = idmap2.GetCount();
	
	while (1) {
		AcDbObjectId idEnt1, idEnt2;	
		AcDbEntity *pEnt1 = NULL, *pEnt2 = NULL;
		
		if (i < iMax1) {
			idmap1.Lookup(i, idEnt1);
			es = acdbOpenObject(pEnt1, idEnt1, AcDb::kForRead);
//			es = acdbOpenObject(pEnt1, idEnt1, AcDb::kForWrite);
			if (es != Acad::eOk) {
				i++;
				continue;
			}
		}
		
		if (j < iMax2) {
			idmap2.Lookup(j, idEnt2);
			es = acdbOpenObject(pEnt2, idEnt2, AcDb::kForRead);
//			es = acdbOpenObject(pEnt2, idEnt2, AcDb::kForWrite);
			if (es != Acad::eOk) {
				if (pEnt1 != NULL)
					pEnt1->close();
				j++;
				continue;
			}
		}				

		// �ǂݍ��݂ŃI�[�v����Ԃ̃G���e�B�e�B���r����B
		if (pEnt1 == NULL) {				
			CompEntities(NULL, pEnt2, iIdx, rce);
			//iIdx++;
			j++;
		}
		else if (pEnt2 == NULL) {
			CompEntities(pEnt1, NULL, iIdx, rce);
			//iIdx++;
			i++;
		}
		else {
			AcDbHandle hdlEnt1, hdlEnt2;
						
			pEnt1->getAcDbHandle(hdlEnt1);
			pEnt2->getAcDbHandle(hdlEnt2);

			TCHAR cBuff1[AC_LEN_HANDLE], cBuff2[AC_LEN_HANDLE];	//2006.07.20						
			pEnt1->getAcDbHandle(hdlEnt1);
			hdlEnt1.getIntoAsciiBuffer(cBuff1);
			
			pEnt2->getAcDbHandle(hdlEnt2);
			hdlEnt2.getIntoAsciiBuffer(cBuff2);
			
			CString strHdl1(cBuff1), strHdl2(cBuff2);
			int iRes;

			iRes = strHdl1.CompareNoCase(strHdl2); 
			if (iRes == 0) {
				CompEntities(pEnt1, pEnt2, iIdx, rce);
				//iIdx++;
				i++;
				j++;
			}
			else if (iRes < 0) {
				CompEntities(pEnt1, NULL, iIdx, rce);										
				//iIdx++;
				i++;
			}
			else {
				CompEntities(NULL, pEnt2, iIdx, rce);										
				//iIdx++;
				j++;
			}			
		}

		if (pEnt1 != NULL)
			pEnt1->close();

		if (pEnt2 != NULL)
			pEnt2->close();			

		if (i >= iMax1 && j >= iMax2)
			break;
	}

	return true;
}

// �ǂݍ��݂ŃI�[�v����Ԃ̃G���e�B�e�B���r���A���ʂ�Ԃ��B
// �ǂ��炩�̃G���e�B�e�B���Ȃ��ꍇ��NULL���w�肷��B
void ACDocManager::CompEntities(AcDbEntity *pEnt1, AcDbEntity *pEnt2, int &iIdx, ResultCompEntity &rce)
{
	if (pEnt1 != NULL && pEnt2 == NULL) {		
		RCESetValue1(iIdx, rce, pEnt1);
		RCESetValue2(iIdx, rce, pEnt2);
		rce.SetResult(iIdx, ResultCompEntity::ONLY1, ResultCompEntity::ONLY1);
		iIdx++;
	}
	else if (pEnt1 == NULL && pEnt2 != NULL) {
		RCESetValue1(iIdx, rce, pEnt1);
		RCESetValue2(iIdx, rce, pEnt2);		
		rce.SetResult(iIdx, ResultCompEntity::ONLY2, ResultCompEntity::ONLY2);
		iIdx++;
	}		
	else if (pEnt1 != NULL && pEnt2 != NULL) {		
		ACCompEntitiesGeom compEntGeom;
		ResultCompEntity::RCE_TYPE_RESULT rceResGeom, rceResAll;
		CString strVal1, strVal2;

		// [ADD:2005/06/14] �͈͔�r���[�h�̂Ƃ��́A��r��_�̂�����Z�b�g����B
		if (m_bRangeCompMode) {
			compEntGeom.SetBasePointGap(m_vec3Gap);
		}

		bool bEqualGeom = compEntGeom.CompareEntityGeoms(pEnt1, pEnt2, m_bRangeCompMode);		
		bool bEqualGeneralAttr = compEntGeom.IsEqualGeneralAttrs(pEnt1, pEnt2);		
		
		// ���ʂ������ł������ꍇ�͏������܂Ȃ��B
		if (bEqualGeom)
			rceResGeom = ResultCompEntity::EQUAL;
		else
			rceResGeom = ResultCompEntity::NOTEQUAL;

		if (!bEqualGeneralAttr || !bEqualGeom) {
			rceResAll = ResultCompEntity::NOTEQUAL;

			RCESetValue1(iIdx, rce, pEnt1);
			RCESetValue2(iIdx, rce, pEnt2);
			rce.SetResult(iIdx, rceResAll, rceResGeom);
			iIdx++;
		}
		

	}	
	
}

// ###�悭�m�F
// �w�肵���p�X�̐}�ʂ̑S�Ă̐}�`�v�f��ID�𓾂�B
bool ACDocManager::GetDwgAllEntityIds(const AcDbObjectId &idBlkTbl, CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRet)
{
	AcApDocManager *pDocMng = acDocManager;	
	Acad::ErrorStatus es;	
	int iIdx = -1;
	bool bRes = false;
	//AcDbDatabase *pDb = NULL; 

	AcApDocumentIterator *pDocItr = pDocMng->newAcApDocumentIterator(); 			

	// ���݊J����Ă���h�L�������g�𑖍�����B
	for (; !pDocItr->done() && !bRes; pDocItr->step()) {
		AcApDocument *pDoc = pDocItr->document();
		//CString strDwgPath = pDoc->fileName();				
		AcDbDatabase *pDb = NULL; 

		pDb = pDoc->database();
		if (pDb == NULL)
			break; 
				
		if (pDb->blockTableId() == idBlkTbl) {
		//if (strDwgPath.CompareNoCase(lpctDwgPath) == 0) {							
			
			AcDbBlockTable *pBlkTable;
			es = pDb->getBlockTable(pBlkTable, AcDb::kForRead);
			if (es != Acad::eOk) 
				break;
			
			// �R�}���h�g�p���ł���ꍇ�͏I������B
			if (IsCommandRunning(pDb))
				break;
					
			// ���f����Ԃɐ؂�ւ����Ȃ������Ƃ��͏I������B
			if (!ChangeToModelSpace(pDb))
				break;

			bRes = true;

			AcDbBlockTableIterator *pBlkItr;					
			
			pBlkTable->newIterator(pBlkItr); 
			for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
				AcDbBlockTableRecord *pBlkRec;				

				es = pBlkItr->getRecord(pBlkRec, AcDb::kForRead);						
				if (es != Acad::eOk)
					continue;

				if (pBlkRec->isLayout()) {			
					AcDbBlockTableRecordIterator *pBlkRecItr;
					bool bFirstVp = true;

					pBlkRec->newIterator(pBlkRecItr);
					for (pBlkRecItr->start(); !pBlkRecItr->done(); pBlkRecItr->step()) {												
						AcDbEntity *pEnt;

						es = pBlkRecItr->getEntity(pEnt, AcDb::kForRead);
						if (es == Acad::eOk) {														
							
							// �y�[�p�[��Ԃ̍ŏ��̃r���[�|�[�g�͏���???
							if (pEnt->isA()->isEqualTo(AcDbViewport::desc()) == Adesk::kTrue && bFirstVp) {
								bFirstVp = false;
							}
							else {	
								iIdx++;	
								idmapRet[iIdx] = pEnt->objectId();								
							}
							
							// �u���b�N�Q�Ƃ̏ꍇ�́A�T�u�G���e�B�e�B(Attribute)�����ׂ�B
							AcDbBlockReference *pBlockRef = AcDbBlockReference::cast(pEnt);														
							if (pBlockRef != NULL) {
								AcDbObjectIterator *pItrAttr = pBlockRef->attributeIterator();
								AcDbObjectId idAttr;

								for (pItrAttr->start(); !pItrAttr->done(); pItrAttr->step()) {
									AcDbObjectId idAttr;

									idAttr = pItrAttr->objectId();
									if (idAttr != AcDbObjectId::kNull) {
										iIdx++;	
										idmapRet[iIdx] = idAttr;	
									}
								}							
								delete pItrAttr;
							}

							pEnt->close();
						}						
					}
					delete pBlkRecItr;							
				}
				pBlkRec->close();
			}				

			delete pBlkItr;			
			pBlkTable->close();		
			//return true;
		}
	}
	delete pDocItr;
	
	return bRes;
}


// �w�肵���}�ʂ̑S�Ă̐}�`�v�f��ID�𓾂�B
bool ACDocManager::GetDwgAllEntityIds(AcApDocument *pDoc, CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRet)
{
	AcApDocManager *pDocMng = acDocManager;	
	Acad::ErrorStatus es;	
	int iIdx = -1;

	// �w�肳�ꂽ�h�L�������g��T���B					
	AcDbDatabase *pDb = NULL;
	pDb = pDoc->database();
	if (pDb == NULL)
		return false;

	AcDbBlockTable *pBlkTable;
	es = pDb->getBlockTable(pBlkTable, AcDb::kForRead);
	if (es != Acad::eOk) 
		return false;

	AcDbBlockTableIterator *pBlkItr;					
	
	pBlkTable->newIterator(pBlkItr); 
	for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
		AcDbBlockTableRecord *pBlkRec;				

		es = pBlkItr->getRecord(pBlkRec, AcDb::kForRead);						
		if (es != Acad::eOk)
			continue;

		if (pBlkRec->isLayout()) {			
			AcDbBlockTableRecordIterator *pBlkRecItr;

			pBlkRec->newIterator(pBlkRecItr);
			for (pBlkRecItr->start(); !pBlkRecItr->done(); pBlkRecItr->step()) {												
				AcDbObjectId idEnt;
										
				es = pBlkRecItr->getEntityId(idEnt);
				if (es == Acad::eOk) {
					iIdx++;	
					idmapRet[iIdx] = idEnt;							
				}													
			}
			delete pBlkRecItr;							
		}
		pBlkRec->close();
	}				

	delete pBlkItr;			
	pBlkTable->close();										

	return true;
}

// �w�肵��ID�̐}�`���n�C���C�g����B�w�肳�ꂽ�}�`���Ȃ��ꍇ���l������B
// �Ώۂ͈�̐}�ʂ̂݁H ��Ԃ��Ⴄ�Ƃ��͖���
bool ACDocManager::HighlightEntities(const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt, bool bOn)
{
	Acad::ErrorStatus es;
	int iCount  = idmapEnt.GetCount();
	bool bExistValidEnt = false;
	bool bChangeSpace   = false;	

	for (int i = 0; i < iCount; i++) {
		AcDbObjectId idEnt;
		AcDbEntity   *pEnt;

		idmapEnt.Lookup(i, idEnt);

		es = acdbOpenObject(pEnt, idEnt, AcDb::kForRead);
		if (es == Acad::eOk) {			
			//// �ŏ��̐}�`�̐}�ʂ��A�N�e�B�u�ɂ���B	
			//if (!bExistValidEnt) {				
			bExistValidEnt = true;			
			//	AcApDocument *pDoc = acDocManager->document(pEnt->database());				
			//	acDocManager->lockDocument(pDoc);
			//	acDocManager->activateDocument(pDoc);
			//	acDocManager->unlockDocument(pDoc);
			//}
			
			//if (!bChangeSpace) {
			//	bChangeSpace = true;
			//	// �G���e�B�e�B���������Ԃ֐؂�ւ���B
			//	ChangeSpace(pEnt);
			//}
			
			if (bOn) {	 
				//pEnt->draw();
				pEnt->highlight(); //kNullSubent, true);
			}
			else
				pEnt->unhighlight();
			
			pEnt->close();
		}					
	}

	return bExistValidEnt;
}

// �w�肳�ꂽ�I�u�W�F�N�gID�̔z����I�u�W�F�N�g�n���h�����Ƀ\�[�g����B
void ACDocManager::GetEntityHandles(
CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapSort,
CMap<int, int, CString, CString> &strmapRet)
{
	Acad::ErrorStatus es;	
	int iAdd = 0;

	strmapRet.InitHashTable(idmapSort.GetCount());
	
	for (int i = 0; i < idmapSort.GetCount(); i++) {
		AcDbEntity *pEnt;		
		AcDbObjectId idEnt;

		idmapSort.Lookup(i, idEnt);
		es = acdbOpenObject(pEnt, idEnt, AcDb::kForRead);
		if (es == Acad::eOk) {
			AcDbHandle hdlEnt;
			TCHAR cBuff[AC_LEN_HANDLE];	//2006.07.20					

			pEnt->getAcDbHandle(hdlEnt);
			hdlEnt.getIntoAsciiBuffer(cBuff);

			CString strHdl(cBuff);

			strmapRet[iAdd] = strHdl;
			iAdd++;
			
			pEnt->close();
		}
	}
}


// �w�肳�ꂽ�I�u�W�F�N�gID�̔z����I�u�W�F�N�g�n���h�����Ƀ\�[�g����B
void ACDocManager::SortEntities(CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapSort)
{
	CMap<int, int, AcDbObjectId, AcDbObjectId> idmapTmp;
	CMap<int, int, CString, CString> strmapHdl;
	CMap<int, int, int, int>		 imapIdx;	
	int i;

	idmapTmp.InitHashTable(idmapSort.GetCount());
	
	// �I�u�W�F�N�gID�̔z��̓��e���R�s�[����B
	for (i = 0; i < idmapSort.GetCount(); i++) {
		AcDbObjectId idEnt = AcDbObjectId::kNull;

		idmapSort.Lookup(i, idEnt);
		idmapTmp[i] = idEnt;
	}

	// �I�u�W�F�N�gID�̗���A�I�u�W�F�N�g�n���h���̗�𓾂�B
	GetEntityHandles(idmapTmp, strmapHdl);
	
	// �n���h���̔z����\�[�g���A�\�[�g��̃C���f�b�N�X��𓾂�B
	ACSort sort;
	sort.QSort(strmapHdl, imapIdx);
	
	for (i = 0; i < imapIdx.GetCount(); i++) {
		AcDbObjectId idTmp;

		idmapTmp.Lookup(ACCMap::GetKeyVal(imapIdx, i), idTmp);
		idmapSort[i] = idTmp;			
	}
}

void ACDocManager::RCESetValue1(int iIdx, ResultCompEntity &rce, AcDbEntity *pEnt) {		
	if (pEnt != NULL) {
		TCHAR *pcLayer = NULL;//2006.07.20
		TCHAR *pcLinetype = NULL;//2006.07.20
		TCHAR *pcPlotStyleName = NULL;//2006.07.20

		pcLayer		    = pEnt->layer(),
		pcLinetype      = pEnt->linetype(),
		pcPlotStyleName = pEnt->plotStyleName(),
				
		rce.SetValue1(
			iIdx, pEnt->objectId(), 
			ACConvertName::GetEntityTypeStr(pEnt), // pEnt->isA()->name(), 
			ACConvertName::GetColorStr(pEnt->colorIndex()), 
			pcLayer,
			pcLinetype,
			pcPlotStyleName,
			ACConvertName::GetLineWeightStr(pEnt->lineWeight()),			
			ACEntityInfo::GetHyperLink(pEnt), 
			pEnt->linetypeScale());		

		acutDelString(pcLayer);
		acutDelString(pcLinetype);
		acutDelString(pcPlotStyleName);
	}
	else {
		rce.SetValue1(
			iIdx, AcDbObjectId::kNull, 
			_T(""), 
			_T(""), 
			_T(""),
			_T(""),
			_T(""),
			_T(""),
			_T(""),
			-1);
	}
}

void ACDocManager::RCESetValue2(int iIdx, ResultCompEntity &rce, AcDbEntity *pEnt) {

	if (pEnt != NULL) {		
		TCHAR *pcLayer = NULL;//2006.07.20
		TCHAR *pcLinetype = NULL;//2006.07.20
		TCHAR *pcPlotStyleName = NULL;//2006.07.20

		pcLayer		    = pEnt->layer(),
		pcLinetype      = pEnt->linetype(),
		pcPlotStyleName = pEnt->plotStyleName(),
			
		rce.SetValue2(
			iIdx, pEnt->objectId(), 
			ACConvertName::GetEntityTypeStr(pEnt), //pEnt->isA()->name(), 
			ACConvertName::GetColorStr(pEnt->colorIndex()), 
			pcLayer,
			pcLinetype,
			pcPlotStyleName,
			ACConvertName::GetLineWeightStr(pEnt->lineWeight()),
			ACEntityInfo::GetHyperLink(pEnt), 
			pEnt->linetypeScale());

		acutDelString(pcLayer);
		acutDelString(pcLinetype);
		acutDelString(pcPlotStyleName);
	}
	else {
		rce.SetValue2(
			iIdx, AcDbObjectId::kNull, 
			"", 
			"", 
			"",
			"",
			"",
			"",
			"",
			-1);
	}
}

// �w�肵��ID�̐}�`�͈̔͂𓾂�B
bool ACDocManager::GetEntityExtents(const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt, 
AcGePoint2d &pt2RetLB, AcGePoint2d &pt2RetRT)
{
	//Acad::ErrorStatus es;
	AcGePoint3d pt3Min, pt3Max;
	int iCount   = idmapEnt.GetCount();		
	bool bGetExt = false;

	for (int i = 0; i < iCount; i++) {
		AcDbObjectId idEnt;
		//AcDbEntity  *pEnt;
		AcDbExtents  extEnt;

		idmapEnt.Lookup(i, idEnt);
		

		if (!GetGeomExtents(idEnt, extEnt))
			continue;
		
		if (!bGetExt) {
			bGetExt = true;				
			pt3Min = extEnt.minPoint();
			pt3Max = extEnt.maxPoint();										
		}
		else {										
			for (int j = 0; j < 3; j++) {
				if (extEnt.minPoint()[j] < pt3Min[j])
					pt3Min[j] = extEnt.minPoint()[j];

				if (extEnt.maxPoint()[j] > pt3Max[j])
					pt3Max[j] = extEnt.maxPoint()[j];
			}										
		}		
		
		/*
		es = acdbOpenObject(pEnt, idEnt, AcDb::kForRead);
		if (es == Acad::eOk) {
			AcDbExtents extEnt;
			
			es = pEnt->getGeomExtents(extEnt);
			if (es == Acad::eOk) {
				if (!bGetExt) {
					bGetExt = true;				
					pt3Min = extEnt.minPoint();
					pt3Max = extEnt.maxPoint();										
				}
				else {										
					for (int j = 0; j < 3; j++) {
						if (extEnt.minPoint()[j] < pt3Min[j])
							pt3Min[j] = extEnt.minPoint()[j];

						if (extEnt.maxPoint()[j] > pt3Max[j])
							pt3Max[j] = extEnt.maxPoint()[j];
					}										
				}												
			}			
			pEnt->close();
		}					
		*/
	}

	//extRet.set(pt3Min, pt3Max);
	pt2RetLB.set(pt3Min[0], pt3Min[1]);
	pt2RetRT.set(pt3Max[0], pt3Max[1]);

	return bGetExt;		
}	


bool ACDocManager::GetGsView(AcGsView*& pView, int& iVportNum)
{
	/*
    struct resbuf rb;
    int rt = acedGetVar(_T("CVPORT"), &rb);
    if(rt != RTNORM)
    {
        //acutPrintf(_T("\nError acquiring sysvar \"CVPORT\" value."));
        return false;
    }	
    iVportNum = rb.resval.rint;
	*/

    pView = acgsGetGsView(iVportNum, true);
    if(!pView)
    {
        //acutPrintf("\nError obtaining AcGsView object.");
        return false;
    }
    return true;
}

// �w�肵���͈͂��g������B
// LB��RT�̒l���t�]���Ă���ꍇ�͏C������B
void ACDocManager::Extents(AcGePoint2d &pt2LB, AcGePoint2d &pt2RT)
{
	double dTmp;
	double dExtent;

	// Left Bottom��Right Top���t�]���Ă���Ƃ��͏I������B
	if (pt2RT[0] - pt2LB[0] < 0) {
		dTmp = pt2LB[0];
		pt2LB[0] = pt2RT[0];
		pt2RT[0] = dTmp;
	}

	if (pt2RT[1] - pt2LB[1] < 0) {
		dTmp = pt2LB[1];
		pt2LB[1] = pt2RT[1];
		pt2RT[1] = dTmp;
	}

	// �͈͂��g������BX
	dTmp = pt2RT[0] - pt2LB[0];

	// LB��RT���قƂ�Ǔ����_�̏ꍇ�͍ŏ��͈͂�ݒ肷��
	//if (pt2RT[0] - pt2LB[0] < ACDM_EXTENTS_MIN) 
	//	dExtent = ACDM_EXTENTS_MIN;
	//else
	dExtent = dTmp * 0.5 * ACDM_EXTENTS_RATE;
	if (dExtent == 0)
		dExtent = ACDM_EXTENTS_MIN;


	pt2RT[0] = pt2LB[0] + dTmp / 2 + dExtent;
	pt2LB[0] = pt2LB[0] + dTmp / 2 - dExtent;
	


	// �͈͂��g������BY
	dTmp = pt2RT[1] - pt2LB[1];

	// LB��RT���قƂ�Ǔ����_�̏ꍇ�͍ŏ��͈͂�ݒ肷��
	//if (pt2RT[1] - pt2LB[1] < ACDM_EXTENTS_MIN) 
	//	dExtent = ACDM_EXTENTS_MIN;
	//else
	dExtent = dTmp * 0.5 * ACDM_EXTENTS_RATE;
	if (dExtent == 0)
		dExtent = ACDM_EXTENTS_MIN;

	pt2RT[1] = pt2LB[1] + dTmp / 2 + dExtent;
	pt2LB[1] = pt2LB[1] + dTmp / 2 - dExtent;

}

// �n���ꂽ�}�`ID��̓��A������Ԃɂ���}�`��ID���Ԃ��B
bool ACDocManager::GetSameSpaceEntities(const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt,
CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRetEnt)
{
	AcDbObjectId idSpaceCommon = AcDbObjectId::kNull;
	Acad::ErrorStatus es;
	int iCount = idmapEnt.GetCount(), iAddCount = 0;;
	bool bExistValidEnt = false;	
	
	idmapRetEnt.InitHashTable(iCount);

	for (int i = 0; i < iCount; i++) {
		AcDbObjectId idEnt;
		AcDbEntity   *pEnt;

		idmapEnt.Lookup(i, idEnt);
		
		// ID�������}�`�̃f�[�^�x�[�X�����p�ł��邩�ǂ����𒲂ׂ�B
		if (!idEnt.isValid())
			continue;

		es = acdbOpenObject(pEnt, idEnt, AcDb::kForRead);
		if (es == Acad::eOk) {				
			// �ŏ��̐}�`�Ɠ�����Ԃɑ�����}�`ID�̂ݒǉ�����B	
			AcDbObjectId idSpace;
			bool bAdd = false;

			if (GetSpaceId(pEnt, idSpace)) {
				if (idSpaceCommon == AcDbObjectId::kNull) {
					bExistValidEnt = true;
					idSpaceCommon = idSpace;					
					bAdd = true;
				}					
				else if (idSpaceCommon == idSpace) {
					bAdd = true;
				}

				if (bAdd) {
					idmapRetEnt[iAddCount]	= idEnt; 
					iAddCount++;
				}
			}						
			
			pEnt->close();
		}		
	}
		
	if (idSpaceCommon != AcDbObjectId::kNull)
		return true;
	else
		return false;	
}

// �w�肵���G���e�B�e�B���������Ԃ�ID��Ԃ��B
// �I�[�i�[����ԂłȂ��Ƃ��́A���̃I�[�i�[��H���Ă����B
// ���s�����ꍇ��false��Ԃ��B
bool ACDocManager::GetSpaceId(const AcDbEntity *pEnt, AcDbObjectId &idRetSpace)
{
	AcDbObjectId idTmp; 
	bool bLayout = false;

	idTmp = pEnt->blockId();
	do {
		AcDbBlockTableRecord *pBlkTbl;		
		if (acdbOpenObject(pBlkTbl, idTmp, AcDb::kForRead) == Acad::eOk) {
			bLayout = pBlkTbl->isLayout();			
			pBlkTbl->close();
		}		
		else
			return false;

		if (!bLayout) {
			AcDbEntity *pEnt;
			if (acdbOpenObject(pEnt, idTmp, AcDb::kForRead) == Acad::eOk) {
				idTmp = pEnt->blockId();
				pEnt->close();
			}
			else
				return false;		
		}
	}
	while(!bLayout); 
		
	idRetSpace = idTmp;

	return true;
}

// �w�肵���f�[�^�x�[�X���A�w�肵����Ԃ֐؂�ւ���B
bool ACDocManager::ChangeToSpace(AcDbDatabase *pDb, const AcDbObjectId &idSpace)
{
	//AcDbDatabase *pDb;
	//AcDbObjectId idSpace;		
	//AcDbEntity   *pEnt = NULL;	
	//bool bGetSpaceId;				
	
	// �R�}���h���s���̏ꍇ�͏I������B
	if (!acDocManager->document(pDb)->isQuiescent())
		return false;			
	
	// �Ώۂ�DB���J�����g�ɂ��Ȃ��ƁA�ȉ��́ApDb->currentSpaceId()�A
	// pLayoutMng->setCurrentLayoutId(idLayout); �������������Ȃ��B
	acDocManager->setCurDocument(acDocManager->document(pDb), AcAp::kNone, true);

	// �G���e�B�e�B�̑������Ԃ��A���݂̋�Ԃł���Ƃ��͏I������B
	if (idSpace == pDb->currentSpaceId())
		return true;	
	
	AcDbBlockTableRecord *pBlkRec;

	if (acdbOpenObject(pBlkRec, idSpace, AcDb::kForRead) != Acad::eOk)
		return false;

	// �}�`���������Ԃ֐؂�ւ���B
	AcDbObjectId idLayout = pBlkRec->getLayoutId();		
	Acad::ErrorStatus es;

	pBlkRec->close();
	
	AcApLayoutManager *pLayoutMng = (AcApLayoutManager *)acdbHostApplicationServices()->layoutManager();					
	AcApDocument *pDoc = acDocManager->document(pDb); //pEnt->database());

	acDocManager->lockDocument(pDoc);
	es = pLayoutMng->setCurrentLayoutId(idLayout);							
	acDocManager->unlockDocument(pDoc);

	if (es == Acad::eOk)
		return true;
	else
		return false;
}

/*
// �w�肵���G���e�B�e�B���������Ԃ֐؂�ւ���B
bool ACDocManager::ChangeSpace(const AcDbObjectId &idEnt)
{
	AcDbDatabase *pDb;
	AcDbEntity   *pEnt = NULL;
	AcDbObjectId idSpace;		
	bool bGetSpaceId;
	
	if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) != Acad::eOk)
		return false;

	pDb = pEnt->database();	
	bGetSpaceId = GetSpaceId(pEnt, idSpace);
	pEnt->close();
	
	if (!bGetSpaceId)
		return false;

	// �R�}���h���s���̏ꍇ�͏I������B
	if (!acDocManager->document(pDb)->isQuiescent())
		return false;			

	// �G���e�B�e�B�̑������Ԃ��A���݂̋�Ԃł���Ƃ��͏I������B
	if (idSpace == pDb->currentSpaceId())
		return true;	
	
	AcDbBlockTableRecord *pBlkRec;

	if (acdbOpenObject(pBlkRec, idSpace, AcDb::kForRead) != Acad::eOk)
		return false;

	// �}�`���������Ԃ֐؂�ւ���B
	AcDbObjectId idLayout = pBlkRec->getLayoutId();		
	Acad::ErrorStatus es;

	pBlkRec->close();
	
	AcApLayoutManager *pLayoutMng = (AcApLayoutManager *)acdbHostApplicationServices()->layoutManager();					
	AcApDocument *pDoc = acDocManager->document(pDb); //pEnt->database());

	acDocManager->lockDocument(pDoc);
	es = pLayoutMng->setCurrentLayoutId(idLayout);							
	acDocManager->unlockDocument(pDoc);

	if (es == Acad::eOk)
		return true;
	else
		return false;
}


*/


// �w�肵��ID��̍ŏ��̃G���e�B�e�B�̂����Ԃ֐؂�ւ���B���̐}�ʂ��A�N�e�B�u�ɂ���B
// �L���Ȑ؂�ւ����Ȃ������ꍇ�ƁA
bool ACDocManager::ChangeToFirstEntitySpace(const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt)
{
	Acad::ErrorStatus es;
	int iCount  = idmapEnt.GetCount();
	bool bActivatedDoc = false;
	bool bChangedSpace = false;	

	for (int i = 0; i < iCount && (!(bActivatedDoc && bChangedSpace)); i++) {
		AcDbObjectId idEnt;
		AcDbEntity   *pEnt;

		idmapEnt.Lookup(i, idEnt);

		es = acdbOpenObject(pEnt, idEnt, AcDb::kForRead);
		if (es == Acad::eOk) {
			// �ŏ��̐}�`�̐}�ʂ��A�N�e�B�u�ɂ���B	
			if (!bActivatedDoc) {												
				AcApDocument *pDoc = acDocManager->document(pEnt->database());				
				//if (acDocManager->setCurDocument(pDoc, AcAp::kNone, true) == Acad::eOk)							     
				acDocManager->setCurDocument(pDoc, AcAp::kNone, true);
				bActivatedDoc = true;
				
				//AcApDocument *pDoc = acDocManager->document(pEnt->database());				
				//acDocManager->lockDocument(pDoc);
				//if (acDocManager->activateDocument(pDoc) == Acad::eOk)
				//	bActivatedDoc = true;
				//acDocManager->unlockDocument(pDoc);
			}

			pEnt->close();						
		}		

		if (!bChangedSpace) {				
			// �G���e�B�e�B���������Ԃ֐؂�ւ���B
			bChangedSpace = ChangeToEntitySpace(idEnt);
		}			
	}

	return (bActivatedDoc && bChangedSpace);
}

// ID�Ŏw�肵���G���e�B�e�B�͈̔͂𓾂�B
// Xline��Ray�͂��ꂼ��̈ʒu�ƕ�����蓾��B
bool ACDocManager::GetGeomExtents(AcDbObjectId idEnt, AcDbExtents &extRet) {
	AcDbExtents extEnt;
	AcDbEntity *pEnt;
	Acad::ErrorStatus es = Acad::eInvalidInput;		
	
	if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) != Acad::eOk)
		return false;
	
	if (pEnt->isA()->isEqualTo(AcDbXline::desc()) == Adesk::kTrue) {
		AcDbXline *pXline = AcDbXline::cast(pEnt);	
		
		if (pXline != NULL) {
			extEnt.set(pXline->basePoint() - pXline->unitDir() * ACDM_INFINITE_ENT_ZOOM_RATE, 
					   pXline->basePoint() + pXline->unitDir() * ACDM_INFINITE_ENT_ZOOM_RATE); 
			es = Acad::eOk;
		}
		
	}
	else if (pEnt->isA()->isEqualTo(AcDbRay::desc()) == Adesk::kTrue) {
		AcDbRay *pRay = AcDbRay::cast(pEnt);	
		
		if (pRay != NULL) {
			extEnt.set(pRay->basePoint() - pRay->unitDir() * ACDM_INFINITE_ENT_ZOOM_RATE, 
					   pRay->basePoint() + pRay->unitDir() * ACDM_INFINITE_ENT_ZOOM_RATE); 
			es = Acad::eOk;
		}
	}
	else {	
		es = pEnt->getGeomExtents(extEnt);
	}

	pEnt->close();

	if (es == Acad::eOk) {
		extRet = extEnt;	
		return true;
	}
	else
		return false;
		
	
}

// �I�[�g���[�V�������g���ăY�[�������s����B
bool ACDocManager::AutomationCommand(LPCTSTR strCmd)	
{
#ifdef ACADTLB
	try
    {
		IDispatch* pDisp = acedGetAcadWinApp()->GetIDispatch(FALSE);
        AutoCAD::IAcadApplicationPtr pApp;
		_com_util::CheckError(pDisp->QueryInterface(__uuidof(AutoCAD::IAcadApplication),(void**)&pApp));
        AutoCAD::IAcadDocumentPtr pDoc;
        _com_util::CheckError(pApp->get_ActiveDocument(&pDoc));

		//CString strCmd;

		//strCmd.Format("zoom %.4f,%.4f %.4f,%.4f ", pt2LBUcs[0], pt2LBUcs[1], pt2RTUcs[0], pt2RTUcs[1]);
		// �I�[�g���[�V�����ɃR�}���h�𑗂�B
		_com_util::CheckError(pDoc->SendCommand(_bstr_t(strCmd)));		
		return true;
    }
    catch(const _com_error e)
    {
        if (e.ErrorMessage()!=NULL)
#ifdef UNICODE
            wprintf(_T("Error: %s"),e.ErrorMessage());
#else
            printf("Error: %s",e.ErrorMessage());
#endif
		return false;
    }
#else
	return true;
#endif		
}


// �I�[�g���[�V�������g���ăY�[�������s����B
bool ACDocManager::AutomationZoom(const AcGePoint2d &pt2LB, const AcGePoint2d &pt2RT)	
{
#ifdef ACADTLB
	AcGePoint2d pt2LBUcs, pt2RTUcs;

	WcsToUcs2d(pt2LB, pt2LBUcs);
	WcsToUcs2d(pt2RT, pt2RTUcs);


	try
    {
		IDispatch* pDisp = acedGetAcadWinApp()->GetIDispatch(FALSE);
        AutoCAD::IAcadApplicationPtr pApp;
		_com_util::CheckError(pDisp->QueryInterface(__uuidof(AutoCAD::IAcadApplication),(void**)&pApp));
        AutoCAD::IAcadDocumentPtr pDoc;
        _com_util::CheckError(pApp->get_ActiveDocument(&pDoc));

		CString strCmd;

		strCmd.Format(_T("zoom %.4f,%.4f %.4f,%.4f "), pt2LBUcs[0], pt2LBUcs[1], pt2RTUcs[0], pt2RTUcs[1]);
		// Zoom�͈�
		_com_util::CheckError(pDoc->SendCommand(_bstr_t((LPCTSTR)strCmd)));		
		
		//ms_bUserZoom = true;

		// ���A���^�C��Zoom
		//_com_util::CheckError(pDoc->SendCommand(_bstr_t("zoom  ")));

		return true;
    }
    catch(const _com_error e)
    {
        if (e.ErrorMessage()!=NULL)
#ifdef UNICODE
            wprintf(_T("Error: %s"),e.ErrorMessage());
#else
            printf("Error: %s",e.ErrorMessage());
#endif
		return false;
    }
#else
	return true;
#endif
}

bool ACDocManager::WcsToUcs2d(const AcGePoint2d &pt2Wcs, AcGePoint2d &pt2RetUcs)
{
	ads_point aptWcs, aptUcs;
	bool      bRes;

	aptWcs[0] = pt2Wcs[0];
	aptWcs[1] = pt2Wcs[1];
	bRes = acdbWcs2Ucs(aptWcs, aptUcs, false);
	
	pt2RetUcs.set(aptUcs[0], aptUcs[1]);

	return bRes;
}

// �w�肵���f�[�^�x�[�X���A�w�肵���G���e�B�e�B�̑������Ԃ֐؂�ւ���B
bool ACDocManager::ChangeToEntitySpace(const AcDbObjectId &idEnt) 
{
	AcDbDatabase *pDb;
	AcDbEntity   *pEnt = NULL;
	AcDbObjectId idSpace;		
	bool bGetSpaceId;
	
	if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) != Acad::eOk)
		return false;

	pDb = pEnt->database();	
	bGetSpaceId = GetSpaceId(pEnt, idSpace);
	pEnt->close();			
	
	return ChangeToSpace(pDb, idSpace);
}

// �w�肵���f�[�^�x�[�X���A���f����Ԃ֐؂�ւ���B
bool ACDocManager::ChangeToModelSpace(AcDbDatabase *pDb)
{
	AcDbObjectId idModelSpace;

	// �R�}���h���s���̏ꍇ�͏I������B
	//if (!acDocManager->document(pDb)->isQuiescent())
	//	return false;			

	// ���f����Ԃ�ID�𓾂�B
	if (!GetModelSpaceId(pDb, idModelSpace))
		return false;	
	
	return ChangeToSpace(pDb, idModelSpace);	
}

// �w�肵���f�[�^�x�[�X�̃��f����ԃu���b�N��ID�𓾂�B
bool ACDocManager::GetModelSpaceId(AcDbDatabase *pDb, AcDbObjectId &idRet) 
{
	AcDbBlockTable       *pBlkTbl;
    //AcDbBlockTableRecord *pBlkRec;
	bool bRes = true;	

    if (pDb->getSymbolTable(pBlkTbl, AcDb::kForRead) != Acad::eOk)
		return false;

    if (pBlkTbl->getAt(ACDB_MODEL_SPACE, idRet, AcDb::kForRead) != Acad::eOk)
		bRes = false;
    pBlkTbl->close();
	
	if (!bRes)
		return false;

	//idRet = pBlkRec->objectId();
	//pBlkRec->close();

	return true;
}


// �w�肵���f�[�^�x�[�X�Ō��݃R�}���h���g�p���ł��邩�ǂ����𒲂ׂ�B
bool ACDocManager::IsCommandRunning(AcDbDatabase *pDb) 
{	
	return (!acDocManager->document(pDb)->isQuiescent());
}

// �w�肵��ID�̃u���b�N�e�[�u�������h�L�������g(�f�[�^�x�[�X)���R�}���h�g�p���ł��邩�ǂ������ׂ�B
ACRetType::ACRETTYPE ACDocManager::CheckDwg(const AcDbObjectId &idBlkTbl)
{		
	AcApDocumentIterator *pDocItr = acDocManager->newAcApDocumentIterator(); 	
	ACRetType::ACRETTYPE acRet = ACRetType::rInvalidDwg;
	int iDocCount = acDocManager->documentCount();
	int i = 0;
	
	// �}�ʂ������Ă���ꍇ�́A�u���b�N�e�[�u��ID�������ɂȂ�B
	if (!idBlkTbl.isValid())
		return ACRetType::rInvalidDwg;		
	
	// ���݊J����Ă���h�L�������g�𑖍�����B
	for (; !pDocItr->done(); pDocItr->step()) {
		AcApDocument *pDoc = pDocItr->document();

		if (pDoc->database()->blockTableId() != idBlkTbl)
			continue;
				
		//CString strTestName = pDoc->docTitle();		
		//acutPrintf("\ntype: %s", strTestName);

		// ���ꂪ���݂���ƁA�Ăяo�����Ń��b�Z�[�W�{�b�N�Xor throw���g���ꍇ�ɗ�����炵���B
		//MessageBox(NULL, strTestName, "test", MB_OK);

		acRet = ACRetType::rOk;

		// �R�}���h���g�p���ł��邩�ǂ������ׂ�B
		if (!pDoc->isQuiescent()) 
			acRet = ACRetType::rCommandRunning;
			
		break;
	}
	delete pDocItr;

	return acRet;
}

// �J�����g�}�ʂ�ێ�����B
void ACDocManager::SetCurDoc()
{
	m_pDocCur = acDocManager->mdiActiveDocument();

	//CString strDocName = m_pDocCur->fileName(); //### 
}

// ���̃J�����g�}�ʂ��A�N�e�B�u�ɂ���B
void ACDocManager::ReturnCurDoc()
{
	//CString strDocName1 = m_pDocCur->fileName(); //### 
	if (m_pDocCur != NULL)
		acDocManager->activateDocument(m_pDocCur);

	//CString strDocName = m_pDocCur->fileName(); //### 
	m_pDocCur = NULL;
}

// �V�����}�ʂ��쐬���A�A�N�e�B�u�ɂ���B
bool ACDocManager::CreateNewDwg()
{	
	AcApDocument *pDocNew;	
	AcApDocumentIterator *pDocItr;

	// �V�����}�ʂ��J��
	acDocManager->appContextNewDocument(_T(""));	//2006.07.20		

	// ���b�N���|����B
	pDocItr = acDocManager->newAcApDocumentIterator();
	for (; !pDocItr->done(); pDocItr->step()) {				
		pDocNew = pDocItr->document();
	}
	delete pDocItr;

	//pDocNew = acDocManager->mdiActiveDocument();	
	acDocManager->lockDocument(pDocNew);
	acDocManager->activateDocument(pDocNew);

	return true;
}

// CreateNewDwg���ɍs�����}�ʂ̃��b�N����������B
void ACDocManager::UnlockCurDoc()
{
	acDocManager->unlockDocument(m_pDocCur);
}


// ���݃A�N�e�B�u�Ȑ}�ʂɔ�r���ʂ�F�ŕ\�������}�`��`���B
bool ACDocManager::DrawResultDwg(AcDbObjectId idNewDwgBlockTable, AcDbObjectId idOldDwgBlockTable, 
	ResultCompEntity &rce, CMap<int, int, AcDbObjectId, AcDbObjectId> &idMapNew)
{							
	//return false; //####

	// �A�N�e�B�u�Ȑ}�ʂ𓾂�B
	AcApDocument *pDocAct = acDocManager->mdiActiveDocument(); 	
	
	// �V�}�ʂ̃��f����Ԃ�ID�𓾂�B
	AcDbObjectId idModelNew;

	//[DEL: 2005.06.15]
	//CMap<int, int, AcDbObjectId, AcDbObjectId> idMapNew; #####
	
	GetModelSpaceId(idNewDwgBlockTable.database(), idModelNew); 			
	
	//[DEL: 2005.06.15]
	// �}�ʂ̑S�Ă̐}�`��ID�𓾂�B
	//GetDwgAllEntityIds(idNewDwgBlockTable, idMapNew);	##### 

	acDocManager->activateDocument(pDocAct);
	
	//SetMlineStyle(pDocAct->database());	

	// �擾�����}�`ID�̂����A���f����Ԃɑ��݂�����̂�z��ɂ���B
	AcDbObjectIdArray idaryModelNew;
	int iAddIdx = -1;		
		
	IdFilterByOwner(idMapNew, idModelNew, idaryModelNew);
	
	AcDbObjectId   idModelAct;	
	GetModelSpaceId(pDocAct->database(), idModelAct); 		

	// �����}�ʂ̖ڈ�pXDATA�����f���X�y�[�X�֐ݒ肷��
	if( !SetResultXData( idModelAct ) ) {
		return false;
	}

	// ���݃A�N�e�B�u�Ȑ}�ʂɐV�}�ʂ̑S�}�`��}������B
	// �u���b�N�̑�����ύX���ꂽ�P�[�X�ŁA���}�ʂ����ɂ���ƁA�����̕ύX���\���ł��Ȃ��Ƃ�����肪����̂ŁA
	// �V�}�ʂ����ɂ��邱�Ƃɂ����B
	AcDbIdMapping idMapModelNew;	

	if (pDocAct->database()->wblockCloneObjects(
		idaryModelNew, idModelAct, idMapModelNew, AcDb::kDrcIgnore) != Acad::eOk)
		return false;					
	
	// �R�s�[�����O���Q�Ƃ��o�C���h����B
	BindXRef(idMapModelNew);


	// ���ʃZ�b�g����A�폜�A�ύX�A�ǉ����ꂽ���̂��������o���B
	CMap<int, int, AcDbObjectId, AcDbObjectId> idmap1;
	CMap<int, int, AcDbObjectId, AcDbObjectId> idmap2;
	CMap<int, int, ResultCompEntity::RCE_TYPE_RESULT, ResultCompEntity::RCE_TYPE_RESULT> resmap;
	int iAddMapIdx = -1;
	idmap1.InitHashTable(rce.GetCount());
	idmap2.InitHashTable(rce.GetCount());
	resmap.InitHashTable(rce.GetCount());
	for (int j =0 ; j < rce.GetCount(); j++) {
		ResultCompEntity::RCE_TYPE_RESULT rceres;

		rce.GetResultMap(ResultCompEntity::COMPALL).Lookup(j, rceres);
		if (rceres != ResultCompEntity::EQUAL && rceres != ResultCompEntity::INVALID) {
			AcDbObjectId id1, id2;
			
			rce.GetObjectIdMap(ResultCompEntity::FIRST).Lookup(j, id1);
			rce.GetObjectIdMap(ResultCompEntity::SECOND).Lookup(j, id2);
			
			iAddMapIdx++;
			idmap1.SetAt(iAddMapIdx, id1);
			idmap2.SetAt(iAddMapIdx, id2);
			resmap.SetAt(iAddMapIdx, rceres);			
		}
	}

	//return true; //####


	// ���}�ʂ̃��f����Ԃ�ID�𓾂�B
	AcDbObjectId idModelOld;
	GetModelSpaceId(idOldDwgBlockTable.database(), idModelOld); 

	// ���}�ʂŕύX���ꂽ�}�`�̓��A���f����Ԃɑ�������̂������A�N�e�B�u�Ȑ}�ʂɃR�s�[����B
	AcDbObjectIdArray idaryModelOld;
	IdFilterByOwner(idmap1, idModelOld, idaryModelOld);
	
	AcDbIdMapping idMapModelOld;
	if (pDocAct->database()->wblockCloneObjects(
		idaryModelOld, idModelAct, idMapModelOld, AcDb::kDrcIgnore) != Acad::eOk)
		return false;
	
	


	// [ADD:2005.06.14] �͈͔�r���[�h�̂Ƃ��́A���}�ʂ̐}�`���A��r�͈͂̊�_�̂���̕������ړ�����B
	if (m_bRangeCompMode) 
		MoveGap(idMapModelOld);	


	

	// �R�s�[�����O���Q�Ƃ��o�C���h����B
	BindXRef(idMapModelOld);

	// �S�Ẵ��C���[�̐F�𔒂ɂ���B
	AllLayerDisable(pDocAct->database(), true, true, false);
	
	// 2005.03.15 �V�K�A�C���A�폜�}�`�p�̃��C���[���쐬����B
	//ACResultLayers resLayers;
	//CreateResultLayers(pDocAct->database(), resLayers);	
	


	// �N���[�������}�`��S�Ĕ�������B	
	ClonedColorWhite(idMapModelNew);
	ClonedColorWhite(idMapModelOld);		

	// �S�Ă̐��@�X�^�C���̐F��BYBLOCK�ɂ���B
	SetDimStyle(pDocAct->database(), ACDM_COLOR_BYBLOCK);
	// �S�Ẵe�L�X�g�X�^�C���̃r�b�O�t�H���g�t�@�C����ݒ肷��B
	SetAllTextStyle(pDocAct->database());		

	ACProfileManager	profile;
	double dArrowMinLen =  profile.GetUserModTextArrowMinLength();

	// �R�s�[����ID�̃y�A����Â��ׂ�B
	//   �ύX���Ȃ��}�`: ��:1,2
	//   �폜���ꂽ�}�`: �Â���:1
	//   �ύX�O�̐}�`  : �폜����B:1
	//   �ύX��̐}�`  : ��:2
	//   �ǉ����ꂽ�}�`: ��:2
	for (int k = 0; k < idmap1.GetCount(); k++)	{
		ResultCompEntity::RCE_TYPE_RESULT rceTmp;
		AcDbObjectId idEnt1, idEnt2;

		resmap.Lookup(k, rceTmp);

		// ��r���ʂ��ύX�̏ꍇ�A���}�ʂ̐}�`�ɑΉ�����N���[�����폜���A�V�}�ʂ̐}�`�ɑΉ�����N���[���̐F��Ԃɂ���B
		if (rceTmp == ResultCompEntity::NOTEQUAL) {
			BOOL	bIsText = FALSE;
			AcDbObjectId idRet1;

			AcGePoint3d	pntBeforeModText;

			idmap1.Lookup(k, idEnt1);

			if (idEnt1.isValid() && SearchClonedPairId(idMapModelOld, idEnt1, idRet1)) {
				AcDbEntity *pEnt;	
				if (acdbOpenObject(pEnt, idRet1, AcDb::kForWrite) == Acad::eOk) {
					// �n�b�`���O�͊֘A�t����؂��Ă���폜����B�i���̂悤�ɂ��Ȃ��ƁA�}�ʍ쐬��ɗ����邱�Ƃ�����j
					if (pEnt->isA()->isEqualTo(AcDbHatch::desc()) == Adesk::kTrue) {
						AcDbHatch *pHatch;
						pHatch = AcDbHatch::cast(pEnt);
						pHatch->setAssociative(false);						
					}

					// �e�L�X�g�̏ꍇ�̓��C���[��ς���
					// �e�L�X�g�ȊO�͍폜����
					if( pEnt->isA()->isEqualTo(AcDbText::desc()) == Adesk::kTrue ||
						pEnt->isA()->isEqualTo(AcDbMText::desc()) == Adesk::kTrue ) {
						CreateBeforeModifyLayers(pDocAct->database());
						pEnt->setLayer(ACDM_PREVMODIFY_LAYER);
						pEnt->setColorIndex( ACDM_PREVMODIFY_COLOR );

						if( pEnt->isA()->isEqualTo(AcDbText::desc()) == Adesk::kTrue ) {
							pntBeforeModText = AcDbText::cast(pEnt)->position();
						} else {
							pntBeforeModText = AcDbMText::cast(pEnt)->location();
						}

						bIsText = TRUE;
					} else {
						pEnt->erase();	
					}

					pEnt->close();
				}			
			}

			AcDbObjectId idRet2;

			idmap2.Lookup(k, idEnt2);

			if (idEnt2.isValid() && SearchClonedPairId(idMapModelNew, idEnt2, idRet2)) {
				SetEntityColor(idRet2, ACDM_COLOR_NOT_EQUAL);							
			}

			// �e�L�X�g�̏ꍇ�A�������֐�������
			if( bIsText ) {
				AcDbBlockTableRecordPointer	pTbl( pDocAct->database()->currentSpaceId(), AcDb::kForWrite );
				AcDbEntityPointer	pEnt( idEnt2, AcDb::kForRead );
				AcGePoint3d	pntAfterModText;
				double	dHeight;

				if( pEnt->isA()->isEqualTo(AcDbText::desc()) == Adesk::kTrue ) {
					pntAfterModText = AcDbText::cast(pEnt)->position();
					dHeight = AcDbText::cast(pEnt)->height();
				} else {
					pntAfterModText = AcDbMText::cast(pEnt)->location();
					dHeight = AcDbMText::cast(pEnt)->height();
				}

				if( (dHeight * dArrowMinLen) <= pntBeforeModText.distanceTo(pntAfterModText) ) {
					AcDbObjectPointer<AcDbLeader>	pLeader;
					pLeader.create();
					pLeader->setDatabaseDefaults( pDocAct->database() );
					pLeader->appendVertex( pntAfterModText );
					pLeader->appendVertex( pntBeforeModText );
					pLeader->setColorIndex( ACDM_PREVMODIFY_COLOR );
					pLeader->setLayer( ACDM_PREVMODIFY_LAYER );
					pTbl->appendAcDbEntity( pLeader );
				}
			}
		}
		// ���}�ʂɂ������݂��Ȃ��}�`�͐F�ɂ���B
		else if (rceTmp == ResultCompEntity::ONLY1) {
			AcDbObjectId idRet;

			idmap1.Lookup(k, idEnt1);

			if (idEnt1.isValid() && SearchClonedPairId(idMapModelOld, idEnt1, idRet)) 
				SetEntityColor(idRet, ACDM_COLOR_DELETE);						
		}
		// �V�}�ʂɂ������݂��Ȃ��}�`�͗ΐF�ɂ���B
		else if (rceTmp == ResultCompEntity::ONLY2) {
			AcDbObjectId idRet;

			idmap2.Lookup(k, idEnt2);

			if (idEnt2.isValid() && SearchClonedPairId(idMapModelNew, idEnt2, idRet)) 
				SetEntityColor(idRet, ACDM_COLOR_NEW);
		}

	}

	//acDocManager->activateDocument(pDocAct); //####
	return true;
}


/*
// ���݃A�N�e�B�u�Ȑ}�ʂɔ�r���ʂ�F�ŕ\�������}�`��`���B
bool ACDocManager::DrawResultDwg(AcDbObjectId idNewDwgBlockTable, AcDbObjectId idOldDwgBlockTable, 
	ResultCompEntity &rce)
{	
	// �V���}�ʂ̃��C�A�E�gID�̔z��𓾂�B
	// ���ꂼ��̔z����r���A�����̃��C�A�E�g�̃n���h���𑫂����킹��ID�̔z����쐬����B
	AcDbObjectIdArray idaryOldLayout, idaryNewLayout, idaryAppend;

	GetLayoutBlkRecObjectIdArray(idOldDwgBlockTable.database(), idaryOldLayout);
	GetLayoutBlkRecObjectIdArray(idNewDwgBlockTable.database(), idaryNewLayout);
	
	for (int i = 0; i < idaryOldLayout.logicalLength(); i++) {
		bool bHandleExist = false;
		for (int j = 0; j < idaryNewLayout.logicalLength(); j++) {
			if (idaryOldLayout[i].handle() == idaryNewLayout[j].handle()) {
				bHandleExist = true;
				break;
			}						
		}
		if (!bHandleExist)
			idaryAppend.append(idaryOldLayout[i]);
	}

	idaryNewLayout.append(idaryAppend);
	

	// �A�N�e�B�u�Ȑ}�ʂ𓾂�B
	AcApDocument *pDocAct = acDocManager->mdiActiveDocument(); 	

	// �A�N�e�B�u�Ȑ}�ʂɁA��̑���œ������C�A�E�g��̂����A�n���h�������݂��Ȃ����̂��쐬����B
	// ���C�A�E�gID��ƁA�V�K�}�ʂ�ID�̃}�b�v���쐬����B
	CArray<AcDbIdPair, AcDbIdPair> idparyLayout;
	CreateLayout(pDocAct->database(), idaryNewLayout, idparyLayout);
		

	return false; //####	








	
	
	// �V�}�ʂ̃��f����Ԃ�ID�𓾂�B
	AcDbObjectId idModelNew;
	CMap<int, int, AcDbObjectId, AcDbObjectId> idMapNew;
	
	GetModelSpaceId(idNewDwgBlockTable.database(), idModelNew); 			
	
	// �}�ʂ̑S�Ă̐}�`��ID�𓾂�B
	GetDwgAllEntityIds(idNewDwgBlockTable, idMapNew);	

	acDocManager->activateDocument(pDocAct);

	// �擾�����}�`ID���A���f����Ԃƃy�[�p�[��Ԃɕ����A���ꂼ��z��ɂ���B
	AcDbObjectIdArray idaryModelNew;
	int iAddIdx = -1;		
		
	IdFilterByOwner(idMapNew, idModelNew, idaryModelNew);
	
	AcDbObjectId   idModelAct;	
	GetModelSpaceId(pDocAct->database(), idModelAct); 		

	// ���݃A�N�e�B�u�Ȑ}�ʂɐV�}�ʂ̑S�}�`��}������B
	// �u���b�N�̑�����ύX���ꂽ�P�[�X�ŁA���}�ʂ����ɂ���ƁA�����̕ύX���\���ł��Ȃ��Ƃ�����肪����̂ŁA
	// �V�}�ʂ����ɂ��邱�Ƃɂ����B
	AcDbIdMapping idMapModelNew;	

	if (pDocAct->database()->wblockCloneObjects(
		idaryModelNew, idModelAct, idMapModelNew, AcDb::kDrcIgnore) != Acad::eOk)
		return false;					
	
	// ���ʃZ�b�g����A�폜�A�ύX�A�ǉ����ꂽ���̂��������o���B
	CMap<int, int, AcDbObjectId, AcDbObjectId> idmap1;
	CMap<int, int, AcDbObjectId, AcDbObjectId> idmap2;
	CMap<int, int, ResultCompEntity::RCE_TYPE_RESULT, ResultCompEntity::RCE_TYPE_RESULT> resmap;
	int iAddMapIdx = -1;
	idmap1.InitHashTable(rce.GetCount());
	idmap2.InitHashTable(rce.GetCount());
	resmap.InitHashTable(rce.GetCount());
	for (int j =0 ; j < rce.GetCount(); j++) {
		ResultCompEntity::RCE_TYPE_RESULT rceres;

		rce.GetResultMap(ResultCompEntity::COMPALL).Lookup(j, rceres);
		if (rceres != ResultCompEntity::EQUAL && rceres != ResultCompEntity::INVALID) {
			AcDbObjectId id1, id2;
			
			rce.GetObjectIdMap(ResultCompEntity::FIRST).Lookup(j, id1);
			rce.GetObjectIdMap(ResultCompEntity::SECOND).Lookup(j, id2);
			
			iAddMapIdx++;
			idmap1.SetAt(iAddMapIdx, id1);
			idmap2.SetAt(iAddMapIdx, id2);
			resmap.SetAt(iAddMapIdx, rceres);			
		}
	}

	// ���}�ʂ̃��f����Ԃ�ID�𓾂�B
	AcDbObjectId idModelOld;
	GetModelSpaceId(idOldDwgBlockTable.database(), idModelOld); 

	AcDbObjectIdArray idaryModelOld;
	IdFilterByOwner(idmap1, idModelOld, idaryModelOld);
	
	AcDbIdMapping idMapModelOld;
	if (pDocAct->database()->wblockCloneObjects(
		idaryModelOld, idModelAct, idMapModelOld, AcDb::kDrcIgnore) != Acad::eOk)
		return false;	
	
	// �S�Ẵ��C���[�̐F�𔒂ɂ���B
	AllLayerDisable(pDocAct->database(), true, true, false);
			
	// �R�s�[����ID�̃y�A����Â��ׂ�B
	//   �ύX���Ȃ��}�`: ��:1,2
	//   �폜���ꂽ�}�`: �Â���:1
	//   �ύX�O�̐}�`  : �폜����B:1
	//   �ύX��̐}�`  : ��:2
	//   �ǉ����ꂽ�}�`: ��:2
	ClonedColorWhite(idMapModelNew);
	ClonedColorWhite(idMapModelOld);	
	
	for (int k = 0; k < idmap1.GetCount(); k++)	{
		ResultCompEntity::RCE_TYPE_RESULT rceTmp;
		AcDbObjectId idEnt1, idEnt2;

		resmap.Lookup(k, rceTmp);

		// ��r���ʂ��ύX�̏ꍇ�A���}�ʂ̐}�`�ɑΉ�����N���[�����폜���A�V�}�ʂ̐}�`�ɑΉ�����N���[���̐F��Ԃɂ���B
		if (rceTmp == ResultCompEntity::NOTEQUAL) {
			AcDbObjectId idRet1;

			idmap1.Lookup(k, idEnt1);			

			if (idEnt1.isValid() && SearchClonedPairId(idMapModelOld, idEnt1, idRet1)) {
				AcDbEntity *pEnt;	
				if (acdbOpenObject(pEnt, idRet1, AcDb::kForWrite) == Acad::eOk) {
					pEnt->erase();	
					pEnt->close();
				}			
			}

			AcDbObjectId idRet2;

			idmap2.Lookup(k, idEnt2);

			if (idEnt2.isValid() && SearchClonedPairId(idMapModelNew, idEnt2, idRet2)) 
				SetEntityColor(idRet2, ACDM_COLOR_NOT_EQUAL);							
		}
		else if (rceTmp == ResultCompEntity::ONLY1) {
			AcDbObjectId idRet;

			idmap1.Lookup(k, idEnt1);

			if (idEnt1.isValid() && SearchClonedPairId(idMapModelOld, idEnt1, idRet)) 
				SetEntityColor(idRet, ACDM_COLOR_DELETE);						
		}
		else if (rceTmp == ResultCompEntity::ONLY2) {
			AcDbObjectId idRet;

			idmap2.Lookup(k, idEnt2);

			if (idEnt2.isValid() && SearchClonedPairId(idMapModelNew, idEnt2, idRet)) 
				SetEntityColor(idRet, ACDM_COLOR_NEW);
		}

	}
	return true;
}
*/

// pDb�̃u���b�N�e�[�u�����R�[�h��ID�̔z��𓾂�B
bool ACDocManager::GetBlockTableRecordIdArray(AcDbDatabase *pDb, AcDbObjectIdArray &idaryBlkRec)
{
	AcDbBlockTable *pBlkTbl;
	
	if (pDb->getSymbolTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) 
		return false;
	
	AcDbBlockTableIterator *pBlkItr;		

	pBlkTbl->newIterator(pBlkItr);
	pBlkTbl->close();
	
	int iBlkRecCount = 0, i = -1;	

	for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
		iBlkRecCount++;
	}
	
	idaryBlkRec.setLogicalLength(iBlkRecCount);
	for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
		AcDbBlockTableRecord *pBlkRec;	
		if (pBlkItr->getRecord(pBlkRec, AcDb::kForRead) == Acad::eOk) {
			i++;
			idaryBlkRec.setAt(i, pBlkRec->objectId());	
			//const char *pcName;
			//pBlkRec->getName(pcName);	
			//OutputDebugString(pcName);
			//OutputDebugString("\n");
			pBlkRec->close();
		}		
	}
	delete pBlkItr;			

	return true;
}

// idMap�̐}�`�̂����A�I�[�i�[��idOwner�ɓ��������݂̂̂�z��iidaryRet�j�ɂ��ĕԂ��B
void ACDocManager::IdFilterByOwner(CMap<int, int, AcDbObjectId, AcDbObjectId> &idMap, 
const AcDbObjectId &idOwner, AcDbObjectIdArray &idaryRet)
{	
	int iAddIdx = -1;	

	idaryRet.setLogicalLength(idMap.GetCount());
	for (int i = 0; i < idMap.GetCount(); i++) {			
		AcDbEntity   *pEnt;	
		AcDbObjectId idObj;		

		idMap.Lookup(i, idObj);
		
		if (!idObj.isValid()) 
			continue;
		
		if (acdbOpenObject(pEnt, idObj, AcDb::kForRead) == Acad::eOk) {
			///////////S
			//AcDbBlockTableRecord *pBlkRec;
			//bool bOwnerIsLayout = false;
			//if (acdbOpenObject(pBlkRec, pEnt->ownerId(), AcDb::kForRead) == Acad::eOk) {
			//	if (pBlkRec->isLayout())
			//		bOwnerIsLayout = true;
			//	pBlkRec->close();
			//}			
			///////////E

			if (pEnt->ownerId() == idOwner) {   // || bOwnerIsLayout /**/
				iAddIdx++;
				idaryRet.setAt(iAddIdx, idObj);
			}

			pEnt->close();
		}
	}
	idaryRet.setLogicalLength(iAddIdx + 1);		
}

// �w�肵��DB�̃y�[�p�[��ԃ^�u��S�č폜����BXXXXXXXX
bool ACDocManager::DeleteAllTabs(AcDbDatabase *pDb)
{
	AcDbBlockTable *pBlkTbl;

	if (pDb->getSymbolTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) 
		return false;
	
	AcDbBlockTableIterator *pBlkItr;		

	pBlkTbl->newIterator(pBlkItr);
	pBlkTbl->close();
	
	AcDbLayoutManager *pLayoutMng = acdbHostApplicationServices()->layoutManager();							

	// �u���b�N�e�[�u���̑S���R�[�h�𒲂ׁA�֘A�t����ꂽ���C�A�E�g���폜����B
	for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
		AcDbBlockTableRecord *pBlkRec;

		if (pBlkItr->getRecord(pBlkRec, AcDb::kForRead) != Acad::eOk) 
			continue;
		
		AcDbObjectId idLayout;		
		AcDbLayout   *pLayout;

		idLayout = pBlkRec->getLayoutId();
		pBlkRec->close();

		if (acdbOpenObject(pLayout, idLayout, AcDb::kForRead) != Acad::eOk)	
			continue;
		
		const TCHAR *pcName;//2006.07.20

		pLayout->getLayoutName(pcName);
		pLayoutMng->deleteLayout(pcName);
		pLayout->close();		
	}	
	delete pBlkItr;	
		
	return true;
}

bool ACDocManager::CreateLayout(AcDbDatabase *pDb)
{
	AcDbBlockTable *pBlkTbl;

	if (pDb->getSymbolTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) 
		return false;
	
	AcDbBlockTableIterator *pBlkItr;		

	pBlkTbl->newIterator(pBlkItr);
	pBlkTbl->close();
	
	AcDbLayoutManager *pLayoutMng = acdbHostApplicationServices()->layoutManager();							

	// �u���b�N�e�[�u���̑S���R�[�h�𒲂ׁA�֘A�t����ꂽ���C�A�E�g���폜����B
	for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
		AcDbBlockTableRecord *pBlkRec;

		if (pBlkItr->getRecord(pBlkRec, AcDb::kForRead) != Acad::eOk) 
			continue;
		
		AcDbObjectId idLayout, idBlkRec;				

		idLayout = pBlkRec->getLayoutId();
		idBlkRec = pBlkRec->objectId();
		pBlkRec->close();

		if (idLayout.isValid())
			continue;
		
		AcDbLayout   *pLayout = new AcDbLayout();
		
		pLayout->setBlockTableRecordId(idBlkRec);

		pLayoutMng->cloneLayout(pLayout, _T(""));
		
			
		
		//const char *pcName;
		//pLayout->getLayoutName(pcName);
		//pLayoutMng->deleteLayout(pcName);
		//pLayout->close();		
	}	
	delete pBlkItr;	
		
	return true;
}

bool ACDocManager::ClonedColorWhite(const AcDbIdMapping &idMap)
{
	AcDbIdMappingIter idmapItr(idMap);
	for (idmapItr.start(); !idmapItr.done(); idmapItr.next()) {
		AcDbIdPair idPair;

		idmapItr.getMap(idPair);
		if (!idPair.isCloned())
			continue;
	
		SetEntityColor(idPair.value(), ACDM_COLOR_EQUAL);
		
		/*
		AcDbEntity *pEnt;
		if (acdbOpenObject(pEnt, idPair.value(), AcDb::kForWrite) != Acad::eOk)
			continue;

		pEnt->setColorIndex(ACDM_COLOR_EQUAL, false);
		pEnt->close();	
		*/
	}		

	return true;
}

bool ACDocManager::SearchClonedPairId(const AcDbIdMapping &idMap, const AcDbObjectId &idSearch, AcDbObjectId &idRet)
{
	AcDbIdMappingIter idmapItr(idMap);	

	for (idmapItr.start(); !idmapItr.done(); idmapItr.next()) {
		AcDbIdPair idPair;
		
		idmapItr.getMap(idPair);
		if (!idPair.isCloned())
			continue;

		if (idPair.key() == idSearch) {
			idRet = idPair.value();
			return true;
		}
	}
	return false;
}

// �w�肵��DB�̑S�Ẵ��C���[�̐F�𔒂ɂ���B
bool ACDocManager::AllLayerDisable(AcDbDatabase *pDb, bool bAllUnLock, bool bAllNoFreeze, bool bAllVisible)
{
	AcDbLayerTable *pLayerTbl;

	if (pDb->getLayerTable(pLayerTbl, AcDb::kForRead) != Acad::eOk)
		return false;

	AcDbLayerTableIterator *pLayerItr;

	pLayerTbl->newIterator(pLayerItr);	
	pLayerTbl->close();

	// �S�Ẵ��C���[���R�[�h�ɑ΂��A�F�A���C���[�̃��b�N�����A�t���[�Y�����A����ݒ肷��B
	for (pLayerItr->start(); !pLayerItr->done(); pLayerItr->step()) {
		AcDbLayerTableRecord *pLayerRec;

		if (pLayerItr->getRecord(pLayerRec, AcDb::kForWrite) != Acad::eOk)
			continue;
		
		if (bAllUnLock)
			pLayerRec->setIsLocked(false);

		if (bAllNoFreeze)
			pLayerRec->setIsFrozen(false);
		
		if (bAllVisible)
			pLayerRec->setIsOff(false);

		AcCmColor col;		
		col.setColorIndex(ACDM_COLOR_LAYER);
		pLayerRec->setColor(col);

		pLayerRec->close();		
	}
	delete pLayerItr;

	return true;
}


// �w�肳�ꂽID�̐}�`�̐F���w�肳�ꂽ�F�ɕύX����B�u���b�N�}�`�A���@�̏ꍇ�́A�\���v�f�̐F�������ɂ���B
bool ACDocManager::SetEntityColor(AcDbObjectId idEnt, Adesk::UInt16 uiColIdx)
{	
	AcDbEntity *pEnt;	


	if (acdbOpenObject(pEnt, idEnt, AcDb::kForWrite) != Acad::eOk) 
		return false;
	
	pEnt->setColorIndex(uiColIdx, false);	
	
	//CString strName = pEnt->isA()->name();  //####	

	AcDbObjectId idBlkRec = AcDbObjectId::kNull;
	// �u���b�N�Q�Ƃ̏ꍇ�͍\���v�f�̐F���ύX����B	
	if (pEnt->isKindOf(AcDbBlockReference::desc())) {					
		AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);		
		idBlkRec = pBlkRef->blockTableRecord();		
	}
	// ���@�̏ꍇ�͍\���v�f�̐F���ύX����B
	else if (pEnt->isKindOf(AcDbDimension::desc())) {
		AcDbDimension *pDim = AcDbDimension::cast(pEnt);		
		idBlkRec = pDim->dimBlockId();		
	}
	// ���@�̏ꍇ�͍\���v�f�̐F���ύX����B
	else if (pEnt->isKindOf(AcDbLeader::desc())) {
		AcDbLeader *pLeader = AcDbLeader::cast(pEnt);
//		pLeader->setDimstyleData( pLeader->dimensionStyle() );
		AcDbDimStyleTableRecord	*pRecord = new AcDbDimStyleTableRecord();
		if( pLeader->getDimstyleData( pRecord ) == Acad::eOk ) {
			pLeader->setDimstyleData( pRecord );
			pRecord->erase();
			pRecord->close();
			delete pRecord;
		}

		AcCmColor col;		
		col.setColorIndex(uiColIdx);
		pLeader->setDimclrd( col );
	}
	//else if (pEnt->isA()->isEqualTo(AcDbMline::desc()) == Adesk::kTrue) {
	//	AcDbMline *pMline = AcDbMline::cast(pEnt);
	//	idBlkRec = pMline->blockId();
	//}

	//idBlkRec = AcDbObjectId::kNull; //####

	if (idBlkRec != AcDbObjectId::kNull) {		
		AcDbBlockTableRecord *pBlkRec;	

		if (acdbOpenObject(pBlkRec, idBlkRec, AcDb::kForRead) == Acad::eOk) {
			AcDbBlockTableRecordIterator *pBlkItr;	

			pBlkRec->newIterator(pBlkItr);
			pBlkRec->close();

			for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
				AcDbObjectId idEntElm;

				pBlkItr->getEntityId(idEntElm);
				SetEntityColor(idEntElm, ACDM_COLOR_BYBLOCK);
			}			
			delete pBlkItr;
		}
	}
			
	pEnt->close();
	return true;
}

// �w�肵��DB�̃��C�A�E�g�̃u���b�N�e�[�u�����R�[�h��ID��𓾂�B
void ACDocManager::GetLayoutBlkRecObjectIdArray(AcDbDatabase *pDb, AcDbObjectIdArray &idaryRetLayout)
{
	AcDbBlockTable *pBlkTbl;

	if (pDb->getSymbolTable(pBlkTbl, AcDb::kForRead) != Acad::eOk) 
		return;
	
	AcDbBlockTableIterator *pBlkItr;		
	int iLayerCount = 0, iAddIdx = -1;

	pBlkTbl->newIterator(pBlkItr);
	pBlkTbl->close();
	
	AcDbLayoutManager *pLayoutMng = acdbHostApplicationServices()->layoutManager();							

	// �u���b�N�e�[�u���̑S���R�[�h�𒲂ׁA���C�A�E�g�̃J�E���g�𓾂�B
	for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
		AcDbBlockTableRecord *pBlkRec;

		if (pBlkItr->getRecord(pBlkRec, AcDb::kForRead) != Acad::eOk) 
			continue;
		
		if (pBlkRec->isLayout())
			iLayerCount++;
		
		pBlkRec->close();	
	}	
	
	idaryRetLayout.setLogicalLength(iLayerCount);
	// �u���b�N�e�[�u���̑S���R�[�h�𒲂ׁA�֘A�t����ꂽ���C�A�E�g���폜����B
	for (pBlkItr->start(); !pBlkItr->done(); pBlkItr->step()) {
		AcDbBlockTableRecord *pBlkRec;

		if (pBlkItr->getRecord(pBlkRec, AcDb::kForRead) != Acad::eOk) 
			continue;
		
		if (pBlkRec->isLayout()) {
			iAddIdx++;
			//idaryRetLayout.setAt(iAddIdx, pBlkRec->getLayoutId());
			idaryRetLayout.setAt(iAddIdx, pBlkRec->objectId());
		}
					
		pBlkRec->close();	
	}	

	delete pBlkItr;	
		
	//return true;
}

/*
bool ACDocManager::CreateLayoutMap(
	const AcDbObjectIdArray &idaryLayoutBlkRecOldDwg,
	const AcDbObjectIdArray &idaryLayoutBlkRecOldDwg,								   
	CArray<LayoutBlkRecMap3, LayoutBlkRecMap3> &lomaparyRetMap)
{
	/////
	return true;
}
*/

/*
bool ACDocManager::CreateLayout(AcDbDatabase *pDb, const AcDbObjectIdArray &idaryRefLayoutBlkRec, 
	CArray<LayoutBlkRecMap3, LayoutBlkRecMap3> &lomaparyRetMap)
{
	AcDbObjectIdArray idaryLayoutBlkRec;
	
	GetLayoutBlkRecObjectIdArray(pDb, idaryLayoutBlkRec);
	
	///////////////////////////
	
	CStringArray straryRefName;
	
	// �Q�l�̃��C�A�E�g�ɑΉ�����u���b�N���R�[�h���̗�𓾂�B
	straryRefName.SetSize(idaryRefLayoutBlkRec.logicalLength());
	for (int i = 0; i < idaryRefLayoutBlkRec.logicalLength(); i++) {
		AcDbBlockTableRecord *pBlkRec;	
		if (acdbOpenObject(pBlkRec, idaryLayoutBlkRec[i], AcDb::kForRead) == Acad::eOk) {	
			CString    strName;
			const char *pcName;

			pBlkRec->getName(pcName);
			strName = pcName;
			straryRefName.SetAt(i, strName);
			pBlkRec->close();
		}
		else {
			straryRefName.SetAt(i, "");
		}
	}

	// ���C�A�E�g��Ԃ̃u���b�N���R�[�hID�̃y�A�̔z�������������B
	lomaparyRetMap.SetSize(idaryRefLayoutBlkRec.logicalLength());
	for (int l = 0; l < idaryRefLayoutBlkRec.logicalLength(); l++) {
		AcDbIdPair idPair(idaryRefLayoutBlkRec[i], AcDbObjectId::kNull, false); 

		lomaparyRetMap.SetAt(l, idPair);
	}

	for (int j = 0; j < idaryLayoutBlkRec.logicalLength(); j++) {
		AcDbBlockTableRecord *pBlkRec;	
		CString strName = "";
		if (acdbOpenObject(pBlkRec, idaryLayoutBlkRec[j], AcDb::kForRead) == Acad::eOk) {
			const char *pcName;		
			pBlkRec->getName(pcName);
			strName = pcName;
			pBlkRec->close();
		}
		
		if (strName != "") {
			for (int k = 0; k < straryRefName.GetSize(); k++) {
				if (straryRefName[k].CollateNoCase(strName) == 0) {
					//AcDbIdPair idPair(idaryRefLayoutBlkRec[k], 
					break;
				}
			}
		}
	}



	
	idaryRetMapLayout.setLogicalLength(idaryRefLayout.logicalLength);
	for (int i = 0; i < idaryRefLayout.logicalLength(); i++) {
		bool bExist = false;
		for (int j = 0; j < idaryLayout.logicalLength(); j++) {
			// �Q�ƃ��C�A�E�g��̃n���h�����w��DB�ɂ����݂����ꍇ�ɂ́A���̊֌W���L�^����B
			if (idaryRefLayout[i].handle() == idaryActLayout[j].hendle()) {
				AcDbIdPair idPair(idaryRefLayout[i], idaryActLayout[i], false);
				
				idaryRetMapLayout.setAt(i, idPair);
				bExist = true;
				break;
			}
		}

		
	}
	

	return false;
}
*/

bool ACDocManager::SetMlineStyle(AcDbDatabase *pDb, UINT uiColIdx)
{
	AcDbMlineStyle *pmlStl;

	if (acdbOpenObject(pmlStl, pDb->mLStyleDictionaryId(), AcDb::kForWrite) != Acad::eOk) 
		return false;
	
		
	//pmlStl->

	pmlStl->close();
	

	return true;	
}

// ���@�X�^�C���̐F��ݒ肷��B
bool ACDocManager::SetDimStyle(AcDbDatabase *pDb, UINT uiColIdx)
{
	AcDbDimStyleTable *pDimStlTbl;
	
	if (pDb->getDimStyleTable(pDimStlTbl, AcDb::kForRead) != Acad::eOk)
		return false;
	
	AcDbDimStyleTableIterator *pDimStlItr;
	pDimStlTbl->newIterator(pDimStlItr);
	pDimStlTbl->close();
	
	for (pDimStlItr->start(); !pDimStlItr->done(); pDimStlItr->step()) {
		AcDbDimStyleTableRecord *pDimStlRec;

		if (pDimStlItr->getRecord(pDimStlRec, AcDb::kForWrite) == Acad::eOk) {
			AcCmColor col;

			col.setColorIndex(uiColIdx);
	
			pDimStlRec->setDimclrd(col);
			pDimStlRec->setDimclre(col);
			pDimStlRec->setDimclrt(col);
			
			pDimStlRec->close();
		}
	}
	delete pDimStlItr;

	return true;
}

// ���@�X�^�C���̐F��ݒ肷��B
bool ACDocManager::SetAllTextStyle(AcDbDatabase *pDb)
{
	AcDbTextStyleTable *pTxtStlTbl;
	
	if (pDb->getTextStyleTable(pTxtStlTbl, AcDb::kForRead) != Acad::eOk)
		return false;
	
	AcDbTextStyleTableIterator *pTxtStlItr;
	pTxtStlTbl->newIterator(pTxtStlItr);
	pTxtStlTbl->close();
	
	for (pTxtStlItr->start(); !pTxtStlItr->done(); pTxtStlItr->step()) {
		AcDbTextStyleTableRecord *pTxtStlRec;

		if (pTxtStlItr->getRecord(pTxtStlRec, AcDb::kForWrite) == Acad::eOk) {
			pTxtStlRec->setBigFontFileName(ACDM_FILE_NAME_BIGFONT);			
			
			pTxtStlRec->close();
		}
	}
	delete pTxtStlItr;

	return true;
}

// �O���Q�Ƃ�}�ʊԂŃR�s�[����B
bool ACDocManager::CopyXRefBlock(AcDbDatabase *pDbSrc, AcDbDatabase *pDbDst)
{
	AcDbBlockTable *pBlkTbl;
	AcDbBlockTableIterator *pBlkTblItr;
	bool bRes = true;

	if (pDbSrc->getBlockTable(pBlkTbl, AcDb::kForRead) != Acad::eOk)
		return false;

	pBlkTbl->newIterator(pBlkTblItr);
	pBlkTbl->close();
	
	for (pBlkTblItr->start(); !pBlkTblItr->done(); pBlkTblItr->step()) {	
		AcDbBlockTableRecord *pBlkRec;

		

		if (pBlkTblItr->getRecord(pBlkRec, AcDb::kForRead) == Acad::eOk) {
			AcDbObjectIdArray idaryEnt;	
			AcDbIdMapping	  idMapDst;	
			long lCount = 0;
			long lIdx   = 0;


			// �O���Q�ƃu���b�N�̂Ƃ�	
			if (pBlkRec->xrefDatabase(true) != NULL) {
				AcDbBlockTableRecordIterator *pBlkRecItr;
				

				TCHAR *pcName;//2006.07.20
				pBlkRec->xrefDatabase(true)->getFilename((const ACHAR *&)pcName);
				acutPrintf(_T("\nXRef: %s"), pcName);
				

				pBlkRec->newIterator(pBlkRecItr); 
				
				// �J�E���g���Ƃ�B
				for (pBlkRecItr->start(); !pBlkRecItr->done(); pBlkRecItr->step()) {
					AcDbEntity *pEnt;
					
					if (pBlkRecItr->getEntity(pEnt, AcDb::kForRead) == Acad::eOk) {

						pEnt->database()->getFilename((const ACHAR *&)pcName);
						acutPrintf(_T("\nElm : %s"), pcName);

						lCount++;					
						pEnt->close();
					}
				}
				
				idaryEnt.setLogicalLength(lCount);

				// �v�f�̃I�u�W�F�N�gID��z��ɂ���B
				for (pBlkRecItr->start(); !pBlkRecItr->done(); pBlkRecItr->step()) {					
					AcDbEntity *pEnt;
					
					if (pBlkRecItr->getEntity(pEnt, AcDb::kForRead) == Acad::eOk) {
						idaryEnt.setAt(lIdx, pEnt->objectId());						
						lIdx++;
						pEnt->close();
					}		
					
				}
				
				//Acad::ErrorStatus es;
				
				// �R�s�[�̎��s
				//if ((es = pDbDst->wblockCloneObjects(
				//	idaryEnt, pBlkRec->objectId(), idMapDst, AcDb::kDrcIgnore)) != Acad::eOk) {
			//		bRes = false;					
			//	}

				delete pBlkRecItr;				
			}			
			pBlkRec->close();


			Acad::ErrorStatus es;
			// �R�s�[�̎��s
			if (lCount > 0) {
				if ((es = pDbDst->wblockCloneObjects(
					idaryEnt, pBlkRec->objectId(), idMapDst, AcDb::kDrcIgnore)) != Acad::eOk) {
					bRes = false;					
					break;
				}
			}

			//if (!bRes)
			//	break;
		}		
	}
	
	delete pBlkTblItr;
	return bRes;
}

// �O���Q�Ƃ�}�ʊԂŃR�s�[����B
bool ACDocManager::CopyXRefBlock(AcDbDatabase *pDbSrc, AcDbIdMapping &idMap, AcDbDatabase *pDbDst)
{
	AcDbIdMappingIter idMapItr(idMap);
	bool bRes = true;


	for (idMapItr.start(); !idMapItr.done(); idMapItr.next()) {
		AcDbIdPair idPair;

		idMapItr.getMap(idPair);
		if (!idPair.isCloned())
			continue;
	
		//idPair
		
		AcDbBlockTableRecord *pBlkRec;

		if (acdbOpenObject(pBlkRec, idPair.key(), AcDb::kForRead) == Acad::eOk) {
			AcDbObjectIdArray idaryEnt;	
		   	AcDbIdMapping   idMapDst;
			long lCount = 0;

			// �O���Q�ƃu���b�N�̂Ƃ�	
			if (pBlkRec->xrefDatabase(true) != NULL) {
				AcDbBlockTableRecordIterator *pBlkRecItr;
				long lIdx = 0;

				//char *pcName;
				//pBlkRec->xrefDatabase(true)->getFilename(pcName);
				//acutPrintf("\nXRef: %s", pcName);
				
				pBlkRec->newIterator(pBlkRecItr); 
				
				// �J�E���g���Ƃ�B
				for (pBlkRecItr->start(); !pBlkRecItr->done(); pBlkRecItr->step()) {
					AcDbEntity *pEnt;
					
					if (pBlkRecItr->getEntity(pEnt, AcDb::kForRead) == Acad::eOk) {

						//pEnt->database()->getFilename(pcName);
						//acutPrintf("\nElm : %s", pcName);

						lCount++;					
						pEnt->close();
					}
				}
				
				idaryEnt.setLogicalLength(lCount);

				// �v�f�̃I�u�W�F�N�gID��z��ɂ���B
				for (pBlkRecItr->start(); !pBlkRecItr->done(); pBlkRecItr->step()) {					
					AcDbEntity *pEnt;
					
					if (pBlkRecItr->getEntity(pEnt, AcDb::kForRead) == Acad::eOk) {
						idaryEnt.setAt(lIdx++, pEnt->objectId());						
						//lIdx++;
						pEnt->close();
					}		
					
				}
				
				//Acad::ErrorStatus es;
				
				// �R�s�[�̎��s
				//if ((es = pDbDst->wblockCloneObjects(
				//	idaryEnt, pBlkRec->objectId(), idMapDst, AcDb::kDrcIgnore)) != Acad::eOk) {
			//		bRes = false;					
			//	}

				delete pBlkRecItr;				
			}			

			//pBlkRec->setIsUnloaded(false);
			pBlkRec->close();
			
			// �R�s�[��̊O���Q�ƃ��R�[�h���J���A�A�����[�h����Ă��邩�ǂ������ׂ�B
			if (acdbOpenObject(pBlkRec, idPair.value(), AcDb::kForWrite) == Acad::eOk) {
				pBlkRec->setIsUnloaded(false);
				pBlkRec->setPathName(NULL);
				pBlkRec->close();
			}

			Acad::ErrorStatus es;
			// �R�s�[�̎��s
			if (lCount > 0) {
				if ((es = pDbDst->wblockCloneObjects(
					idaryEnt, idPair.value(), idMapDst, AcDb::kDrcIgnore)) != Acad::eOk) {
					bRes = false;					
					break;
				}
			}

			pBlkRec->close();
		}
	
	}		
	

	return true;
}

// �R�s�[�����O���Q�Ƃ��o�C���h����B
void ACDocManager::BindXRef(const AcDbIdMapping &idMap)
{
	AcDbIdMappingIter idMapItr(idMap);	

	for (idMapItr.start(); !idMapItr.done(); idMapItr.next()) {
		AcDbIdPair idPair;

		idMapItr.getMap(idPair);
		if (!idPair.isCloned())
			continue;
		
		AcDbBlockTableRecord *pBlkRec;	
		
		
		if (acdbOpenObject(pBlkRec, idPair.key(), AcDb::kForRead) == Acad::eOk) {			
			if (pBlkRec->xrefDatabase() != NULL) {
				Acad::ErrorStatus es;	
				TCHAR *pcName;//2006.07.20
				
				// �ŏ��Ƀ����[�h�����A��������΃o�C���h����B�����[�h�Ɏ��s�����ꍇ�i�t�@�C���������ꍇ�Ȃǁj
				// �́A�f�^�b�`���A�x����\������B
				// �O���Q�Ƃ̖��O�͌��̐}�ʂ��擾����B
				//acedXrefReload�̓A�N�e�B�u�Ȑ}��(�����}��)�ɑ΂��ēK�p�����B
				pBlkRec->getName(pcName);
				if ((es = acedXrefReload(pcName)) == Acad::eOk) {
					es = acedXrefBind(pcName);
				}
				else {
					es = acedXrefDetach(pcName);
					acutPrintf(_T("\n���̊O���Q�Ƃ͌�����܂���B: %s"), pcName);//2006.07.20
				}
				
				acutDelString(pcName);
			}
			
			pBlkRec->close();
		}
	}
}

// �ύX�����A�ǉ��A�C���A�폜�̌��ʂ�F�ŕ\�����郌�C���[���쐬����B
bool ACDocManager::CreateResultLayers(AcDbDatabase *pDb, ACResultLayers &resLayers)
{
	AcDbLayerTable       *pLayerTbl;
	AcDbLayerTableRecord *pLayerRec; 	
	AcCmColor		  col;
	CString			  strName;	
	
	if (pDb->getLayerTable(pLayerTbl, AcDb::kForWrite) != Acad::eOk)
		return false;

	strName.Format(_T("�ύX����")); 
	col.setColorIndex(ACDM_COLOR_EQUAL);
	pLayerRec = new AcDbLayerTableRecord();
	pLayerRec->setName(strName);
	pLayerRec->setIsFrozen(0);
	pLayerRec->setIsOff(0);
	pLayerRec->setVPDFLT(0);
	pLayerRec->setIsLocked(0);	
	pLayerRec->setColor(col);
	pLayerTbl->add(resLayers.m_idNotModify, pLayerRec);
	pLayerRec->close();

	
	strName.Format(_T("�ύX�L��"));//2006.07.20 
	col.setColorIndex(ACDM_COLOR_NOT_EQUAL);
	pLayerRec = new AcDbLayerTableRecord();
	pLayerRec->setName(strName);
	pLayerRec->setIsFrozen(0);
	pLayerRec->setIsOff(0);
	pLayerRec->setVPDFLT(0);
	pLayerRec->setIsLocked(0);	
	pLayerRec->setColor(col);
	pLayerTbl->add(resLayers.m_idModify, pLayerRec);
	pLayerRec->close();

	strName.Format(_T("�ǉ�"));//2006.07.20 
	col.setColorIndex(ACDM_COLOR_NEW);
	pLayerRec = new AcDbLayerTableRecord();
	pLayerRec->setName(strName);
	pLayerRec->setIsFrozen(0);
	pLayerRec->setIsOff(0);
	pLayerRec->setVPDFLT(0);
	pLayerRec->setIsLocked(0);	
	pLayerRec->setColor(col);
	pLayerTbl->add(resLayers.m_idAdd, pLayerRec);
	pLayerRec->close();

	strName.Format(_T("�폜"));//2006.07.20 
	col.setColorIndex(ACDM_COLOR_DELETE);
	pLayerRec = new AcDbLayerTableRecord();
	pLayerRec->setName(strName);
	pLayerRec->setIsFrozen(0);
	pLayerRec->setIsOff(0);
	pLayerRec->setVPDFLT(0);
	pLayerRec->setIsLocked(0);	
	pLayerRec->setColor(col);
	pLayerTbl->add(resLayers.m_idDelete, pLayerRec);
	pLayerRec->close();
	

	pLayerTbl->close();
	
	return true;
}

// �ύX�O�����񃌃C���[���쐬����B
bool ACDocManager::CreateBeforeModifyLayers(AcDbDatabase *pDb)
{
	AcDbSymbolTablePointer<AcDbLayerTable>	pLayerTbl( pDb->layerTableId(), AcDb::kForWrite );
	if( pLayerTbl.openStatus() != Acad::eOk ) {
		return false;
	}

	if( pLayerTbl->has( ACDM_PREVMODIFY_LAYER ) ) {
		return true;
	}

	AcDbSymbolTableRecordPointer<AcDbLayerTableRecord>	pLayerRec;

	pLayerRec.create();
	AcCmColor	col;
	col.setColorIndex( ACDM_PREVMODIFY_COLOR );
	pLayerRec->setName(ACDM_PREVMODIFY_LAYER);
	pLayerRec->setColor( col );
	pLayerTbl->add(pLayerRec);
	pLayerRec->close();

	return true;
}

/*
// �w�肵�����O��t�������C���[��Ԃ��B���C���[�����ɑ��݂���ꍇ�͖��O�̌��ɔԍ�������B
bool ACDocManager::CreateNameLayer(const AcDbDatabase *pDb, const ACCreateLayerInfo &lyrInfo, AcDbObjectId &idRetLayer)
{
	AcDbLayerTable       *pLayerTbl;
	AcDbLayerTableRecord *pLayerRec;
	Acad::ErrorStatus    es;
	CString			     strName;
	long				 lNumber = 1;

	if (pDb->getLayerTable(pLayerTbl, AcDb::kForWrite) != Acad::eOk)
		return false;

	pLayerRec = new AcDbLayerTableRecord();
	strName.Format("0"); 
	es = pLayerRec->setName(strName);
	es = pLayerTbl->add(pLayerRec);

	return true;
}
*/

long ACDocManager::GetCompRectId(const AcDbObjectId &idBlkTbl, AcDbObjectId &idRetCompRect)
{
	// �u���b�N�e�[�u����ID��idBlkTbl�ł���}�ʂ��A�N�e�B�u�ɂ���B
	AcApDocument *pDoc;
	
	pDoc = acDocManager->document(idBlkTbl.database());
	acDocManager->setCurDocument(pDoc, AcAp::kNone, true);

	// �A�N�e�B�u�Ȑ}�ʂ����r�͈͂̋�`�̌��Ɛ擪�̃I�u�W�F�N�gID����B
	ACCompRect acRect;
	return acRect.GetCompRectId(idRetCompRect);
}

bool ACDocManager::GetRangeEntityIds(const AcDbObjectId &idCompRect, CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRet)
{
	// �u���b�N�e�[�u����ID��idBlkTbl�ł���}�ʂ��A�N�e�B�u�ɂ���B
	AcApDocument *pDoc;

	pDoc = acDocManager->document(idCompRect.database());
	acDocManager->setCurDocument(pDoc, AcAp::kNone, true);
	
	acDocManager->lockDocument(pDoc);

	// ��r�͈͂̋�`���擾����B
	AcDbExtents extRange;

	GetGeomExtents(idCompRect, extRange);

	// �w�肵���͈͂ŃY�[������B
	double dZoomExtX, dZoomExtY;  // �Y�[���̂��߂̊g��
	
	dZoomExtX = (extRange.maxPoint()[X] - extRange.minPoint()[X]) / 1000;
	dZoomExtY = (extRange.maxPoint()[Y] - extRange.minPoint()[Y]) / 1000;

	AcGePoint2d ptLB(extRange.minPoint()[X] - dZoomExtX, extRange.minPoint()[Y] - dZoomExtY);
	AcGePoint2d ptRT(extRange.maxPoint()[X] + dZoomExtX, extRange.maxPoint()[Y] + dZoomExtY);

	AutomationZoom(ptLB, ptRT); 
	//AutomationCommand("REGEN "); 

	// �I��͈͂̐}�`���擾����B
	ads_point aptLB, aptRT;
	ads_name  sset;

	aptLB[X] = extRange.minPoint()[X];
	aptLB[Y] = extRange.minPoint()[Y];
	aptRT[X] = extRange.maxPoint()[X];
	aptRT[Y] = extRange.maxPoint()[Y];

	//acedSSGet("B", aptLB, aptRT, NULL, sset);
	acedSSGet(_T("W"), aptLB, aptRT, NULL, sset);//2006.07.20
	
	long l, lLen = 0, lMapKey = 0;		
	acedSSLength(sset, &lLen);
	
	

	// �͈͂ɂ���}�`�̃J�E���g�����B�u���b�N�Q�Ƃ̏ꍇ�́A�����̃J�E���g��������B
	for (l = 0; l < lLen; l++) {
		ads_name nameEnt;
		AcDbObjectId idEnt;

		acedSSName(sset, l, nameEnt);
		acdbGetObjectId(idEnt, nameEnt);
		
		// ��r�͈͂̋�`�̃I�u�W�F�N�gID�ȊO�̂Ƃ��́A�}�b�v�ɒǉ�����B
		if (idEnt != idCompRect) 
			lMapKey++;

		AcDbEntity *pEnt;

		if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
			// �u���b�N�Q�Ƃ̏ꍇ�́A�����̃J�E���g�����B
			AcDbBlockReference *pBlockRef = AcDbBlockReference::cast(pEnt);
			if (pBlockRef != NULL) {
				AcDbObjectIterator *pItrAttr = pBlockRef->attributeIterator();
				AcDbObjectId idAttr;

				for (pItrAttr->start(); !pItrAttr->done(); pItrAttr->step()) {
					AcDbObjectId idAttr;

					idAttr = pItrAttr->objectId();
					if (idAttr != AcDbObjectId::kNull) 						
						lMapKey++;
				}							
				delete pItrAttr;
			}
			pEnt->close();
		}
	}

	idmapRet.InitHashTable(lMapKey);
	lMapKey = 0;

	// �͈͂ɂ���}�`��ID���}�b�v�ɉ�����B�u���b�N�Q�Ƃ̏ꍇ�́A������ID��������B
	for (l = 0; l < lLen; l++) {
		ads_name nameEnt;
		AcDbObjectId idEnt;

		acedSSName(sset, l, nameEnt);
		acdbGetObjectId(idEnt, nameEnt);
		
		// ��r�͈͂̋�`�̃I�u�W�F�N�gID�ȊO�̂Ƃ��́A�}�b�v�ɒǉ�����B
		if (idEnt != idCompRect) 
			idmapRet[lMapKey++] = idEnt;

		AcDbEntity *pEnt;

		if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
			// �u���b�N�Q�Ƃ̏ꍇ�́A������ID��������B
			AcDbBlockReference *pBlockRef = AcDbBlockReference::cast(pEnt);
			if (pBlockRef != NULL) {
				AcDbObjectIterator *pItrAttr = pBlockRef->attributeIterator();
				AcDbObjectId idAttr;

				for (pItrAttr->start(); !pItrAttr->done(); pItrAttr->step()) {
					AcDbObjectId idAttr;

					idAttr = pItrAttr->objectId();
					if (idAttr != AcDbObjectId::kNull) {						
						idmapRet[lMapKey++] = idAttr;	
					}
				}							
				delete pItrAttr;
			}
			pEnt->close();
		}
	}



	acedSSFree(sset);
	
	AutomationCommand(_T("zoom p "));//2006.07.20

	acDocManager->unlockDocument(pDoc);
	return true;
}

void ACDocManager::SetRangeCompMode(const AcGeVector3d &vec3Gap)
{
	m_bRangeCompMode = true;
	m_vec3Gap.set(vec3Gap.x, vec3Gap.y, vec3Gap.z);
}


// �͈͔�r���[�h�̂Ƃ��́A���}�ʂ̐}�`���A��r�͈͂̊�_�̂���̕������ړ�����B
bool ACDocManager::MoveGap(AcDbIdMapping &idMap)
{
	// �A�N�e�B�u�Ȑ}�ʂ̃��f����Ԃ̃u���b�N�e�[�u��ID�𓾂�B
	AcDbBlockTable *pBlkTbl;
	
	acdbHostApplicationServices()->workingDatabase()->
		getSymbolTable(pBlkTbl, AcDb::kForRead);

	AcDbObjectId idModelSpace;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, idModelSpace);
	pBlkTbl->close();

	// �f�B�[�v�N���[�������}�`�̃I�u�W�F�N�gID�̃}�b�v�𓾂�B
	AcDbIdMappingIter idMapItr(idMap);	

	// �ړ��̃}�g���b�N�X�𓾂�B
	AcGeMatrix3d matMove; 

	matMove = AcGeMatrix3d::translation(m_vec3Gap);
	
	for (idMapItr.start(); !idMapItr.done(); idMapItr.next()) {
		AcDbIdPair idPair;

		idMapItr.getMap(idPair);
		if (!idPair.isCloned())
			continue;
		
		AcDbEntity *pEnt;	
		Acad::ErrorStatus es;

		if ((es = acdbOpenObject(pEnt, idPair.value(), AcDb::kForWrite)) == Acad::eOk) {			
			// �O���[�v���}�`�̏ꍇ�́A���A�N�^�[�ɂ��A���̈ړ��ƃX�P�[���̕ϊ���
			// ��d�ɂ������Ă��܂����Ƃ�����̂ŁA���A�N�^�������B
			// �����o�����̏ꍇ�́A���߃I�u�W�F�N�g�Ƀ��A�N�^���Z�b�g����Ă��邪�A
			// �ړ���A�X�P�[����������悤�ȃ��A�N�^�ł͂Ȃ��̂Ń��A�N�^�������Ȃ��B
			AcDbVoidPtrArray  *pReactors;	
			void			  *pSomething;
			AcDbObjectId      idPersReac;
			Acad::ErrorStatus es;
			int				  iLenReac;						
			
			pReactors = pEnt->reactors();							
			if (pReactors != NULL) {
				int iIdxReac = 0;
				iLenReac = pReactors->logicalLength();
				for (int l = 0; l < iLenReac; l++) {					
					pSomething = (void *)pReactors->at(iIdxReac);	
					if (acdbIsPersistentReactor(pSomething)) {
						idPersReac = acdbPersistentReactorObjectId(pSomething);

						es = Acad::eInvalidInput;
						// ���߃I�u�W�F�N�g�̃��A�N�^��ID�́A���o������\���B
						if (!IsAcDbLeaderId(idPersReac))
							es = pEnt->removePersistentReactor(idPersReac);
						
						// ���A�N�^�̎�菜���Ɏ��s�����Ƃ���A�����o�����̃��A�N�^�ł������ꍇ�́A
						// ���̃��A�N�^�Ɉړ�����B
						if (es != Acad::eOk)
							iIdxReac++;						
					}
					// �i�����A�N�^�łȂ��ꍇ�͎��̃��A�N�^�Ɉړ�����B
					else
						iIdxReac++;			
				}
			}
			

			// �I�[�i�[�����f����Ԃł���ꍇ�݈̂ړ������s����B
			if (pEnt->ownerId() == idModelSpace)
				pEnt->transformBy(matMove);	
			pEnt->close();
		}
	}	
	
	return true;
}

// �w�肳�ꂽ�I�u�W�F�N�gID�������o�����̂��̂ł��邩���ׂ�B
bool ACDocManager::IsAcDbLeaderId(AcDbObjectId idEnt)
{
	AcDbEntity *pEnt;
	bool bRes = false;

	if (acdbOpenObject(pEnt, idEnt, AcDb::kForRead) == Acad::eOk) {
		if (pEnt->isA()->isEqualTo(AcDbLeader::desc()) == Adesk::kTrue)	
			bRes = true;
		pEnt->close();
	}
	
	return bRes;	
}

bool ACDocManager::SetResultXData(AcDbObjectId idModel)
{
	// ���f����Ԃ̃I�u�W�F�N�g���J��
	AcDbObject* pObj;
	Acad::ErrorStatus retStat;
	if ((retStat = acdbOpenObject(pObj, idModel, AcDb::kForWrite)) != Acad::eOk) {
		ads_printf(_T("\nacdbOpenEntity failed: retStat:%d"), retStat);
		return false;
	}

	struct resbuf *pRb, *pTemp;
	pRb = pObj->xData(ACDM_RESULTAPPNAME);
	pTemp = pRb;

	/* �g���f�[�^�̎�ނ��قȂ�ƍ���̂ŁA���ɂ���g���f�[�^�͉������B	*/
    if (pRb != NULL) {
		ads_relrb(pRb);
		pRb = NULL;
	}

	/* �A�v���P�[�V�������̓o�^�ƃZ�b�g */
	ads_regapp(ACDM_RESULTAPPNAME);
	pRb = ads_newrb(AcDb::kDxfRegAppName);
	if (pRb == NULL) {
		pObj->close();
		return false;
	}
	pTemp = pRb;
//	pTemp->resval.rstring = (TCHAR*)malloc(_tcslen(ACDM_RESULTAPPNAME) + 1);
	pTemp->resval.rstring = new TCHAR[_tcslen(ACDM_RESULTAPPNAME) + 1];
	_tcscpy(pTemp->resval.rstring, ACDM_RESULTAPPNAME);

	// �����}��XDATA��ݒ�
	pTemp->rbnext = ads_newrb(AcDb::kDxfXdAsciiString);
	if (pTemp->rbnext == NULL) {
		pObj->close();
		ads_relrb(pRb);
		return false;
	}
	pTemp = pTemp->rbnext;
//	pTemp->resval.rstring = (TCHAR*)malloc(_tcslen(ACDM_RESULTXDATA) + 1);
	pTemp->resval.rstring = new TCHAR[_tcslen(ACDM_RESULTXDATA) + 1];
	if (pTemp->resval.rstring == NULL) {
		pObj->close();
		ads_relrb(pRb);
		return false;
	}
	_tcscpy(pTemp->resval.rstring, ACDM_RESULTXDATA);

	pTemp->rbnext = NULL;

	if (Acad::eOk != pObj->setXData(pRb)) {
		ads_printf(_T("\n setXData ERR"));
	}
	pObj->close();
	ads_relrb(pRb);

	return true;
}
