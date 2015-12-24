// ResultCompEntity.h: ResultCompEntity �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTCOMPENTITY_H__FC350BC8_BA87_4C87_9963_D72E831942C2__INCLUDED_)
#define AFX_RESULTCOMPENTITY_H__FC350BC8_BA87_4C87_9963_D72E831942C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RCE_NUM_FIELD_STRING 14
#define RCE_NUM_FIELD_DOUBLE  2
#define RCE_NUM_FIELD_BOOL    7
#define RCE_NUM_FIELD_RESULT  2

class ResultCompEntity  
{
public:
	ResultCompEntity();
	virtual ~ResultCompEntity();

	// ������^�̃t�B�[���h�z��̃C���f�b�N�X
	enum RCE_FIELD_STRING {
		ENTTYPE1,
		ENTTYPE2,
		COLOR1,
		COLOR2,
		LAYER1,
		LAYER2,
		LINETYPE1,
		LINETYPE2,
		PLOTSTYLE1,
		PLOTSTYLE2,
		LINEWEIGHT1,
		LINEWEIGHT2,
		HYPERLINK1,
		HYPERLINK2
	};

	// DOUBLE�^�̃t�B�[���h�z��̃C���f�b�N�X
	enum RCE_FIELD_DOUBLE {
		LINETYPESCALE1,
		LINETYPESCALE2
	};

	// BOOL�^�̃t�B�[���h�z��̃C���f�b�N�X
	enum RCE_FIELD_BOOL {
		DIFFALL,	
		//DIFFGEOM,
		DIFFCOLOR,
		DIFFLAYER,
		DIFFLINETYPE,
		DIFFPLOTSTYLENAME,
		DIFFLINETYPESCALE,
		DIFFLINEWEIGHT
	};
	
	// RESULT�^�̃t�B�[���h�z��̃C���f�b�N�X
	enum RCE_FIELD_RESULT {
		COMPALL,
		COMPGEOM
	};	

	// AcDbObjectId�^�̃t�B�[���h�z��̃C���f�b�N�X
	enum RCE_FIELD_OBJID {
		FIRST,
		SECOND
	};

	// ��r���ʃ^�C�v
	enum RCE_TYPE_RESULT {
		INVALID,		// ����	
		EQUAL,			// ������
		NOTEQUAL,		// �قȂ��Ă���
		ONLY1,			// �}�`1�̂ݑ��݂��Ă���
		ONLY2			// �}�`2�̂ݑ��݂��Ă���
	};	
	
	
		

	// �z��̃T�C�Y���Z�b�g����B
	void SetSize(int iSize);
	
	/*
	void SetValue(
		int iKey, 
		AcDbHandle hdlEnt, AcDbObjectId idEnt1, AcDbObjectId idEnt2, 
		CString strEnttype1, CString strEnttype2,
		CString strColor1, CString strColor2,
		CString strLayer1, CString strLayer2, 
		CString strLinetype1, CString strLinetype2,
		CString strPlotStyle1, CString strPlotStyle2,
		CString strLineWeight1, CString strLineWeight2,
		CString strHyperLink1, CString strHyperLink2,
		double dLinetypeScale1, double dLinetypeScale2,
		bool bDiffAll, bool bDiffGeom, 
		bool bDiffColor, bool bDiffLayer, 
		bool bDiffLinetype, bool bDiffPlotstylename, 
		bool bDiffLinetypeScale, bool bDiffLineWeight); 
	*/

	void SetValue1(
		int iKey, 
		AcDbObjectId idEnt1, 
		CString strEnttype1, 
		CString strColor1, 
		CString strLayer1,  
		CString strLinetype1, 
		CString strPlotStyle1, 
		CString strLineWeight1, 
		CString strHyperLink1,
		double dLinetypeScale1); 

	void SetValue2(
		int iKey, 
		AcDbObjectId idEnt2, 
		CString strEnttype2, 
		CString strColor2, 
		CString strLayer2,  
		CString strLinetype2, 
		CString strPlotStyle2, 
		CString strLineWeight2,
		CString strHyperLink2,
		double dLinetypeScale2); 

	void SetHandle(int iKey, AcDbHandle hdlEnt);
	
	void SetDiff(
		int iKey, 		
		bool bDiffAll, bool bDiffGeom, 
		bool bDiffColor, bool bDiffLayer, 
		bool bDiffLinetype, bool bDiffPlotstylename, 
		bool bDiffLinetypeScale, bool bDiffLineWeight); 
	
	// ��r���ʂ̃t�B�[���h�ɐݒ肷��B
	void SetResult(
		int iKey,
		RCE_TYPE_RESULT resAll, RCE_TYPE_RESULT resGeom);
	
	// �n�b�V���e�[�u���̃T�C�Y���w�肷��B���ۂɃZ�b�g����v�f���ł���K�v�͂Ȃ��B
	// iSize�͑f�����悢�炵��
	void SetHashTableSize(int iSize);
	
	// �S�Ă̗v�f���폜����B
	void RemoveAll();
	
	// ���ʃZ�b�g�̒�����A�w�肵���C���f�b�N�X��ObjectID��Ԃ��B
	void GetSelId(
		const CMap<int, int, int, int> &imapSel, ResultCompEntity::RCE_FIELD_OBJID rceObjId,
		CMap<int, int, AcDbObjectId, AcDbObjectId> &idmapRet);
	
	// ���ʃZ�b�g�̃J�E���g��Ԃ��B
	int GetCount();

	// ������^�̃}�b�v��Ԃ��B
	CMap<int, int, CString, CString> &GetStringMap(RCE_FIELD_STRING eFieldIdx);

	// double�^�̃}�b�v��Ԃ��B
	CMap<int, int, double, double>   &GetDoubleMap(RCE_FIELD_DOUBLE eFieldIdx);	

	// RCE_TYPE_RESULT�^�̃}�b�v��Ԃ��B
	CMap<int, int, RCE_TYPE_RESULT, RCE_TYPE_RESULT> &GetResultMap(RCE_FIELD_RESULT eFieldIdx);
	
	// AcDbObjectId�^�̃}�b�v��Ԃ��B
	CMap<int, int, AcDbObjectId, AcDbObjectId> &GetObjectIdMap(RCE_FIELD_OBJID eFieldIdx);

private:
	static int m_iSizeStr;
	static int m_iSizeDouble;
	static int m_iSizeBool;
	
	// ���ʃZ�b�g
	CMap<int, int, AcDbObjectId, AcDbObjectId> m_idmapEnt[2];
	CMap<int, int, AcDbHandle, AcDbHandle>     m_hdlmapEnt;

	CMap<int, int, CString, CString> m_smapAttr[RCE_NUM_FIELD_STRING];
	CMap<int, int, double, double>   m_dmapAttr[RCE_NUM_FIELD_DOUBLE];	
	CMap<int, int, RCE_TYPE_RESULT, RCE_TYPE_RESULT> m_resmapAttr[RCE_NUM_FIELD_RESULT];		
};

#endif // !defined(AFX_RESULTCOMPENTITY_H__FC350BC8_BA87_4C87_9963_D72E831942C2__INCLUDED_)
