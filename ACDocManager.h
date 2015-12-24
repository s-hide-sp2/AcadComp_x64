// ACDocManager.h: ACDocManager �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACDOCMANAGER_H__7641E627_F0E1_4B31_B44E_CF2702644CB8__INCLUDED_)
#define AFX_ACDOCMANAGER_H__7641E627_F0E1_4B31_B44E_CF2702644CB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// [ADD]
#define ACDM_EXTENTS_MIN  0.5
#define ACDM_EXTENTS_RATE 3
#define ACDM_INFINITE_ENT_ZOOM_RATE 400
#define ACDM_COLOR_EQUAL      8 
#define ACDM_COLOR_NOT_EQUAL 20
#define ACDM_COLOR_DELETE   142
#define ACDM_COLOR_NEW        3
#define ACDM_COLOR_LAYER      7 
#define ACDM_COLOR_BYBLOCK    0
#define ACDM_FILE_NAME_BIGFONT _T("BIGFONT.SHX") //2006.07.20
#define	ACDM_RESULTAPPNAME	_T("ACADCOMP_RESULTAPP")
#define	ACDM_RESULTXDATA	_T("ACADCOMP_RESULTDRAWING")

// �ύX�O�����񃌃C���[
#define ACDM_PREVMODIFY_COLOR	26
#define ACDM_PREVMODIFY_LAYER	_T("ZZ_�ύX�O������")

#include "InfoDwgEntity.h"
#include "ResultCompEntity.h"
//#include "AcConvertColor.h"
#include "ACConvertName.h"
#include "ACCompEntitiesGeom.h"
#include "ACCMap.h"
#include "ACSort.h"
#include "ACEntityInfo.h"
#include "ACRetType.h"
#include "ACResultLayers.h"
#include "ACCompRect.h"


class ACDocManager  
{
public:
	ACDocManager();
	virtual ~ACDocManager();		

	// ���݊J���Ă���S�}�ʂ̃p�X�̔z��𓾂�B
	void GetDocPaths(CStringArray &saryRet, AcDbObjectIdArray &idaryBlkTbl);

	// �p�X���w�肵�āA�}�ʃI�u�W�F�N�g�𓾂�B�Y������}�ʂ��Ȃ��Ƃ���false��Ԃ��B
	bool GetDoc(LPCTSTR lpctDwgPath, AcApDocument *pDoc);
	
	// �w�肵���}�ʂ��r���A���ʂ𓾂�B
	bool CompDwgs(
		CMap<int, int, AcDbObjectId, AcDbObjectId> &idmap1, 
		CMap<int, int, AcDbObjectId, AcDbObjectId> &idmap2,
		ResultCompEntity &rce);

	// �w�肵���}�ʂ̑S�Ă̐}�`�v�f��ID�𓾂�B
	bool GetDwgAllEntityIds(AcApDocument *pDoc1, CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRet);
	
	// �w�肵���p�X�̐}�ʂ̑S�Ă̐}�`�v�f��ID�𓾂�B
	bool GetDwgAllEntityIds(const AcDbObjectId &idBlkTbl, CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRet);

	// �w�肵��ID�̐}�`���n�C���C�g����B�w�肳�ꂽ�}�`���Ȃ��ꍇ���l������B
	// �Ώۂ͈�̐}�ʂ̂݁H ��Ԃ��Ⴄ�Ƃ��͖���
	bool HighlightEntities(const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt, bool bOn);
	
	// �w�肵��ID�̐}�`�͈̔͂𓾂�B
	bool GetEntityExtents(const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt, AcGePoint2d &pt2LB, AcGePoint2d &pt2RT);	

	// �w�肵���͈͂��g������B
	void Extents(AcGePoint2d &pt2LB, AcGePoint2d &pt2RT);

	// �w�肳�ꂽ�I�u�W�F�N�gID�̔z����I�u�W�F�N�g�n���h�����Ƀ\�[�g����B
	void SortEntities(CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapSort);

	// �I�[�g���[�V�������g���ăY�[�������s����B
	bool AutomationZoom(const AcGePoint2d &pt2LB, const AcGePoint2d &pt2RT);	

	// �A�N�e�B�u�Ȑ}�ʂɑ΂��A�I�[�g���[�V�����ŃR�}���h�𑗂�B
	bool AutomationCommand(LPCTSTR strCmd);	

	// �w�肵��ID�̃u���b�N�e�[�u�������h�L�������g(�f�[�^�x�[�X)���R�}���h�g�p���ł��邩�ǂ������ׂ�B
	ACRetType::ACRETTYPE CheckDwg(const AcDbObjectId &idBlkTbl);

	// �J�����g�}�ʂ�ێ�����B
	void SetCurDoc();

	// ���̃J�����g�}�ʂ��A�N�e�B�u�ɂ���B
	void ReturnCurDoc();
	
	// �V�����}�ʂ��쐬���A�A�N�e�B�u�ɂ���B
	bool CreateNewDwg();

	// ���݃A�N�e�B�u�Ȑ}�ʂɔ�r���ʂ�F�ŕ\�������}�`��`���B		
	// idNewDwgBlockTable: �V�}�ʂ̃u���b�N�e�[�u����ID
	// idOldDwgBlockTable: ���}�ʂ̃u���b�N�e�[�u����ID
	// idMapNew: 
	bool DrawResultDwg(AcDbObjectId idNewDwgBlockTable, AcDbObjectId idOldDwgBlockTable, 
		ResultCompEntity &rce, CMap<int, int, AcDbObjectId, AcDbObjectId> &idMapNew);

	// SetCurDoc�ŕێ������}�ʂ����b�N��������B
	void UnlockCurDoc();
	
	// �w�肵��DB�ŁA���C�A�E�g�ɂȂ��Ă��Ȃ��u���b�N�e�[�u�����R�[�h�����C�A�E�g������B
	bool CreateLayout(AcDbDatabase *pDb);
	
private:
	// �ǂݍ��݂ŃI�[�v����Ԃ̃G���e�B�e�B���r���A���ʂ�Ԃ��B
	// �ǂ��炩�̃G���e�B�e�B���Ȃ��ꍇ�͕Е���NULL�ɂ��ď������ށB
	void CompEntities(AcDbEntity *pEnt1, AcDbEntity *pEnt2, int &iIdx, ResultCompEntity &rce);
	
	// �w�肳�ꂽ�G���e�B�e�B�̃W�I���g���̈Ⴂ�𒲂ׂ�B
	//bool CompareEntitiesGeom(AcDbEntity *pEnt1, AcDbEntity *pEnt2);	

	// �I�u�W�F�N�gID�̔z����A�I�u�W�F�N�g�n���h���̔z��𓾂�B
	void GetEntityHandles(
		CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapSort,
		CMap<int, int, CString, CString> &strmapRet);	

	// ���ʏ��N���X�ɏ�����������
	void RCESetValue1(int iIdx, ResultCompEntity &rce, AcDbEntity *pEnt);

	// ���ʏ��N���X�ɏ�����������
	void RCESetValue2(int iIdx, ResultCompEntity &rce, AcDbEntity *pEnt);

	// �r���[�I�u�W�F�N�g�𓾂�B
	bool GetGsView(AcGsView*& pView, int& iVportNum);
	
	// CMap��ID�ŁAidOwner���I�[�i�[�ł�����̂�idaryRet�ɃZ�b�g����B
	void IdFilterByOwner(CMap<int, int, AcDbObjectId, AcDbObjectId> &idMap, 
		const AcDbObjectId &idOwner, AcDbObjectIdArray &idaryRet);
	
	// pDb�̃u���b�N�e�[�u�����R�[�h��ID�̔z��𓾂�B
	bool GetBlockTableRecordIdArray(AcDbDatabase *pDb, AcDbObjectIdArray &idaryBlkRec);

	// �w�肳�ꂽID�}�b�v�̃N���[�����ꂽ���̐}�`�̐F�𔒂ɂ���B
	bool ClonedColorWhite(const AcDbIdMapping &idMap);

	// �w�肳�ꂽID�}�b�v���ŁAidSearch�ƈ�v����KEY��T���A���̃y�A��ObjectId��Ԃ��B
	bool SearchClonedPairId(const AcDbIdMapping &idMap, const AcDbObjectId &idSearch, AcDbObjectId &idRet);

	// �w�肵��DB�̑S�Ẵ��C���[�̐F�𔒂ɂ���B
	bool AllLayerDisable(AcDbDatabase *pDb, bool bUnLock, bool bNoFreeze, bool bAllVisible);

	// �w�肳�ꂽID�̐}�`�̐F���w�肳�ꂽ�F�ɕύX����B�u���b�N�}�`�̏ꍇ�́A�\���v�f�̐F�������ɂ���B
	bool SetEntityColor(AcDbObjectId idEnt, Adesk::UInt16 uiColIdx);

	// �w�肳�ꂽDB�̃��C�A�E�g��Ԃ�ID��𓾂�B
	void GetLayoutBlkRecObjectIdArray(AcDbDatabase *pDb, AcDbObjectIdArray &idaryRetLayout);

	// �A�N�e�B�u�Ȑ}�ʂɁA��̑���œ������C�A�E�g��̂����A�n���h�������݂��Ȃ����̂��쐬����B
	// ���C�A�E�gID��ƁA�V�K�}�ʂ�ID�̃}�b�v���쐬����B		
	//bool CreateLayout(AcDbDatabase *pDb, const AcDbObjectIdArray &idaryRefLayout, 
	//	CArray<LayoutBlkRecMap3, LayoutBlkRecMap3> &idaryRetMapLayout);
		
	bool SetMlineStyle(AcDbDatabase *pDb, UINT uiColIdx);

	bool SetDimStyle(AcDbDatabase *pDb, UINT uiColIdx);

	// ���@�X�^�C���̐F��ݒ肷��B
	bool SetAllTextStyle(AcDbDatabase *pDb);
	
	// �R�s�[�����O���Q�Ƃ��o�C���h����B
	void BindXRef(const AcDbIdMapping &idMap);
	
	// �ύX�����A�ǉ��A�C���A�폜�̌��ʂ�F�ŕ\�����郌�C���[���쐬����B
	bool CreateResultLayers(AcDbDatabase *pDb, ACResultLayers &resLayers);

	// �ύX�O�����񃌃C���[���쐬����B
	bool CreateBeforeModifyLayers(AcDbDatabase *pDb);

	// �w�肵�����O��t�������C���[��Ԃ��B���C���[�����ɑ��݂���ꍇ�͎w�肵�����O�̌�ɔԍ���t����B
	//bool CreateNameLayer(const AcDbDatabase *pDb, const ACCreateLayerInfo &lyrInfo, AcDbObjectId &idRetLayer);

	// �w�肳�ꂽ�I�u�W�F�N�gID�������o�����̂��̂ł��邩���ׂ�B
	bool IsAcDbLeaderId(AcDbObjectId idEnt);

public:	
	// �u���b�N�e�[�u����ID��idBlkTbl�ł���}�ʂ́AextRange�͈̔͂ɂ���G���e�B�e�B���擾���A�I�u�W�F�N�gID��MAP�𓾂�B
	bool GetRangeEntityIds(const AcDbObjectId &idCompRect, CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRet);
	// idBlkTbl�Ŏw�肵���}�ʂ��ACOMPRECT�R�}���h�ō쐬������r�͈͂̋�`��ID�𓾂�B�߂�l�Ŕ�r�͈͂̋�`�̌��𓾂�B
	long GetCompRectId(const AcDbObjectId &idBlkTbl, AcDbObjectId &idRetCompRect);
	// �n���ꂽ�}�`ID��̓��A������Ԃɂ���}�`��ID���Ԃ��B
	bool GetSameSpaceEntities(
		const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt,
		CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRetEnt);

	// �w�肵��ID��̍ŏ��̃G���e�B�e�B�̂����Ԃ֐؂�ւ���B���̐}�ʂ��A�N�e�B�u�ɂ���B
	// �L���Ȑ}�`���Ȃ������ꍇ�ƁA�}�ʂ��A�N�e�B�u�ɂ���̂Ɏ��s�����ꍇ�ƁA�؂�ւ����Ȃ������ꍇ��false
	bool ChangeToFirstEntitySpace(const CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapEnt);

private:
	// �O���Q�Ƃ�}�ʊԂŃR�s�[����B	
	bool CopyXRefBlock(AcDbDatabase *pDbSrc, AcDbDatabase *pDbDst);

	// �O���Q�Ƃ�}�ʊԂŃR�s�[����B
	bool ACDocManager::CopyXRefBlock(AcDbDatabase *pDbSrc, AcDbIdMapping &idMap, AcDbDatabase *pDbDst);

	// �w�肵���G���e�B�e�B���������Ԃ�ID��Ԃ��B
	// �I�[�i�[����ԂłȂ��Ƃ��́A���̃I�[�i�[��H���Ă����B
	// ���s�����ꍇ��false��Ԃ��B
	bool GetSpaceId(const AcDbEntity *pEnt, AcDbObjectId &idRetSpace);

public:	
	void SetRangeCompMode(const AcGeVector3d &vec3Gap);
	// ID�Ŏw�肵���G���e�B�e�B�͈̔͂𓾂�B
	// Xline��Ray�͂��ꂼ��̈ʒu�ƕ�����蓾��B
	bool GetGeomExtents(AcDbObjectId idEnt, AcDbExtents &extRet);

	// �w�肵���f�[�^�x�[�X���A���f����Ԃ֐؂�ւ���B
	bool ChangeToModelSpace(AcDbDatabase *pDb);

private:
	// Wcs���W����Ucs���W�֕ϊ�����B
	bool WcsToUcs2d(const AcGePoint2d &pt2Wcs, AcGePoint2d &pt2RetUcs);

	

	// �w�肵���G���e�B�e�B���������Ԃ֐؂�ւ���B	
	bool ChangeToEntitySpace(const AcDbObjectId &idEnt);	

	// �w�肵���f�[�^�x�[�X���A�w�肵����Ԃ֐؂�ւ���B
	bool ChangeToSpace(AcDbDatabase *pDb, const AcDbObjectId &idSpace);

	// �w�肵���f�[�^�x�[�X�̃��f����ԃu���b�N��ID�𓾂�B
	bool GetModelSpaceId(AcDbDatabase *pDb, AcDbObjectId &idRet); 

	// �w�肵���f�[�^�x�[�X�Ō��݃R�}���h���g�p���ł��邩�ǂ����𒲂ׂ�B
	bool IsCommandRunning(AcDbDatabase *pDb); 
	
	// �w�肵��DB�̃y�[�p�[��ԃ^�u��S�č폜����B
	bool DeleteAllTabs(AcDbDatabase *pDb);

	// �J�����g�}�ʂ�ێ�����B
	AcApDocument *m_pDocCur;	

	// �͈͔�r���[�h�̂Ƃ��́A���}�ʂ̐}�`���A��r�͈͂̊�_�̂���̕������ړ�����B
	bool MoveGap(AcDbIdMapping &idMap);

	// �͈͔�r���[�h
	bool m_bRangeCompMode; 
	// �}��1�̔�r��_����}��2�̔�r�͈͂ւ̃x�N�g���B�͈͔�r���[�h���̂ݎw�肷��B
	AcGeVector3d m_vec3Gap; 
public:
	bool SetResultXData(AcDbObjectId idModel);
};

#endif // !defined(AFX_ACDOCMANAGER_H__7641E627_F0E1_4B31_B44E_CF2702644CB8__INCLUDED_)
