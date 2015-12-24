// ResultRecordEntity.h: ResultRecordEntity �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTRECORDENTITY_H__F7E18BA1_E290_4BBC_AD62_44653E559272__INCLUDED_)
#define AFX_RESULTRECORDENTITY_H__F7E18BA1_E290_4BBC_AD62_44653E559272__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ResultRecordEntity  
{
public:
	ResultRecordEntity();
	virtual ~ResultRecordEntity();
	
	// ������^�̃t�B�[���h�z��̃C���f�b�N�X
	enum E_FIELD_STRING {
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
	};

	// �{���������_�^�̃t�B�[���h�z��̃C���f�b�N�X
	enum E_FIELD_DOUBLE {
		LINETYPESCALE1,
		LINETYPESCALE2
	};

	// BOOL�^�̃t�B�[���h�z��̃C���f�b�N�X
	enum E_FIELD_BOOL {
		DIFFGEOM,
		DIFFCOLOR,
		DIFFLAYER,
		DIFFLINETYPE,
		DIFFPLOTSTYLENAME,
		DIFFLINETYPESCALE,
		DIFFLINEWEIGHT
	};

public:
	bool GetValue(E_FIELD_BOOL eFieldIdx);
	double GetValue(E_FIELD_DOUBLE eFieldIdx);
	LPCTSTR GetValue(E_FIELD_STRING eFieldIdx);
	// 2�̐}�`���ʂ̃n���h��(�ǂ��炩�Е��̏ꍇ������B)
	AcDbHandle m_hdlEnts;
	// ���ꂼ��̃I�u�W�F�N�gID
	AcDbObjectId m_idObj1;
	AcDbObjectId m_idObj2;	
	
	// �e�t�B�[���h�ɒl���Z�b�g����B
	void SetData(
		AcDbHandle hdlEnts, AcDbObjectId idObj1, AcDbObjectId idObj2, bool bDiffGeom, 
		CString strColor1, CString strColor2, CString strLayer1, CString strLayer2,
		CString strLinetype1, CString strLinetype2, CString strPlotstyle1, CString strPlotstyle2,
		CString strLineWeight1, CString strLineWeight2, double dLineTypeScale1, double dLineTypeScale2);

	
	/*
	// �F(���O�̏ꍇ�ƃC���f�b�N�X�̏ꍇ������B)
	CString m_strColor1;
	CString m_strColor2;
	// ���C���[
	CString m_strLayer1;
	CString m_strLayer2;
	// ����
	CString m_strLinetype1;
	CString m_strLinetype2;
	// ����X�^�C��
	CString m_strPlotstyleName1;
	CString m_strPlotstyleName2;
	// ���̑���(Bylayer�Ȃǂ̏ꍇ������B)
	CString m_strLineWeight1;	
	CString m_strLineWeight2;	
	// ����ړx
	double  m_dLinetypeScale1;
	double  m_dLinetypeScale2;
	// �W�I���g���ɈႢ������ꍇtrue
	bool    m_bDiffGeom; 
    // �F�ɈႢ������ꍇtrue
	bool    m_bDiffColor;
	// ���C���[�ɈႢ������ꍇtrue
	bool    m_bDiffLayer;
	// ����ɈႢ������ꍇtrue
	bool    m_bDiffLinetype;
	// ����X�^�C���ɈႢ������ꍇtrue
	bool	m_bDiffPlotstyleName;
	// ����ړx�ɈႢ������ꍇtrue
	bool	m_bLinetypeScale;
	// ���̑����ɈႢ������ꍇtrue
	bool	m_bLineWeight;
	*/
private:
	// ���ꂼ��̌^�̃t�B�[���h�l��ێ�����B	
	CString m_strAttr[10];
	double  m_dAttr[2];
	bool    m_bAttr[7];
};

#endif // !defined(AFX_RESULTRECORDENTITY_H__F7E18BA1_E290_4BBC_AD62_44653E559272__INCLUDED_)
