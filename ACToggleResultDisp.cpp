#include "stdafx.h"
#include "actoggleresultdisp.h"
#include "ACDocManager.h"

ACToggleResultDisp::ACToggleResultDisp(void)
{
}

ACToggleResultDisp::~ACToggleResultDisp(void)
{
}

bool ACToggleResultDisp::IsResultDrawing()
{
	bool	bRet = false;

	// �J�����g�}�ʂ��擾
	AcDbDatabase	*pDb = acdbHostApplicationServices()->workingDatabase();

	// �u���b�N�e�[�u�����擾
	AcDbBlockTable	*pBT;
	if(pDb->getBlockTable(pBT,AcDb::kForRead) != Acad::eOk){
		return false;
	}

	// ���f����Ԃ̃u���b�N�e�[�u�����R�[�h���擾
	AcDbBlockTableRecord	*pBTR;
	if(pBT->getAt(ACDB_MODEL_SPACE,pBTR,AcDb::kForWrite) != Acad::eOk){
		pBT->close();
		return false;
	}
	pBT->close();

	/* �g���`�o�o�������݂��Ȃ��� */
	struct resbuf *pRb, *pRbTop;
	pRbTop = pRb = pBTR->xData(ACDM_RESULTAPPNAME);
	if( pRb == NULL ) {
		pBTR->close();
		return false;
	}

	pRb = pRb->rbnext;

	if( pRb->restype == AcDb::kDxfXdAsciiString ) {
		if( !_tcscmp( pRb->resval.rstring, ACDM_RESULTXDATA ) ) {
			bRet = true;
		}
	}

	pBTR->close();
	ads_relrb( pRbTop );

	return bRet;
}

bool ACToggleResultDisp::ToggleDisp( Adesk::UInt16 uiColIdx )
{
	// �J�����g�}�ʂ��擾
	AcDbDatabase	*pDb = acdbHostApplicationServices()->workingDatabase();

	// �u���b�N�e�[�u�����R�[�h���擾
    AcDbBlockTableRecord	*pBTR;
	acdbOpenObject(pBTR, pDb->currentSpaceId(), AcDb::kForWrite);

	// �u���b�N�e�[�u�����R�[�h�̃C�e���^���擾
	AcDbBlockTableRecordIterator	*pIter;
	pBTR->newIterator(pIter);
	pBTR->close();

	AcDbEntity		*pEnt;
	for(;!pIter->done();pIter->step()){
		// �J���Ȃ������肢��Ȃ����i�������玟��
		if( pIter->getEntity( pEnt, AcDb::kForWrite ) != Acad::eOk ) continue;

		if( pEnt->colorIndex() == uiColIdx ) {
			if( pEnt->visibility() == AcDb::kVisible ) {
				pEnt->setVisibility( AcDb::kInvisible );
			} else {
				pEnt->setVisibility( AcDb::kVisible );
			}
		}

		pEnt->close();
	}
	delete pIter;

	return true;
}
