// ACConvertName.cpp: ACConvertName �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ACConvertName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

ACConvertName::ACConvertName()
{

}

ACConvertName::~ACConvertName()
{

}

CString ACConvertName::GetColorStr(int iColIdx)
{
	//2006.07.20
	CString strColor[7] = {
		_T("Red"), 
		_T("Yellow"), 
		_T("Green"),
		_T("Cyan"),
		_T("Blue"),
		_T("Magenta"),
		_T("White")};

	if (0 < iColIdx && iColIdx <= 7)
		return strColor[iColIdx - 1];
	else if (iColIdx == 256) {
		return CString(_T("ByLayer"));
	}
	else {
		CString strTmp;
		strTmp.Format(_T("%d"), iColIdx);
		return strTmp;			
	}			
	
	/*
	switch (iColIdx) {		
		case 1:
			return "Red";			
		case 2:
			return "Yellow";
		case 3:
			return "Green";
		case 4:
			return "Cyan";
		case 5:
			return "Blue";	
		case 6:
			return "Magenta";
		case 7:
			return "White";  // or Black
		default:
			strTmp.Format("%d", iColIdx);
			return strTmp;			
	}
	*/
}

CString ACConvertName::GetLineWeightStr(AcDb::LineWeight lw)
{	
	CString strTmp;
	
	//2006.07.20
	strTmp.Format(_T("%d"), lw);
	return strTmp;
}

// �}�`�^�C�v�̈�ʖ��𓾂�B
CString ACConvertName::GetEntityTypeStr(AcDbEntity *pEnt)
{
	// AcDbCurve����h�������}�`
	if (pEnt->isKindOf(AcDbCurve::desc()))	{
		// ���C���̔�r
		if      (pEnt->isA()->isEqualTo(AcDbLine::desc()) == Adesk::kTrue)
			return CString(_T("����"));
		// 2d�|�����C���̔�r
		else if (pEnt->isA()->isEqualTo(AcDb2dPolyline::desc()) == Adesk::kTrue)			
			return CString(_T("2D ���ײ�"));
		// 3d�|�����C���̔�r
		else if (pEnt->isA()->isEqualTo(AcDb3dPolyline::desc()) == Adesk::kTrue)
			return CString(_T("3D ���ײ�"));
		// �~�ʂ̔�r
		else if (pEnt->isA()->isEqualTo(AcDbArc::desc()) == Adesk::kTrue)
			return CString(_T("�~��"));
		// �~�̔�r
		else if (pEnt->isA()->isEqualTo(AcDbCircle::desc()) == Adesk::kTrue)
			return CString(_T("�~"));
		// �ȉ~�̔�r
		else if (pEnt->isA()->isEqualTo(AcDbEllipse::desc()) == Adesk::kTrue)
			return CString(_T("�ȉ~"));
		// ���o�����̔�r
		else if (pEnt->isA()->isEqualTo(AcDbLeader::desc()) == Adesk::kTrue)
			return CString(_T("���o��"));
		// �|�����C���̔�r
		else if (pEnt->isA()->isEqualTo(AcDbPolyline::desc()) == Adesk::kTrue)
			return CString(_T("���ײ�"));
		// Ray�̔�r
		else if (pEnt->isA()->isEqualTo(AcDbRay::desc()) == Adesk::kTrue)
			return CString(_T("���ː�"));
		// Spline�̔�r
		else if (pEnt->isA()->isEqualTo(AcDbSpline::desc()) == Adesk::kTrue)
			return CString(_T("���ײ�"));
		// Xline�̔�r
		else if (pEnt->isA()->isEqualTo(AcDbXline::desc()) == Adesk::kTrue)
			return CString(_T("�\�z��"));
		// ����ȊO�@�J�X�^���I�u�W�F�N�g
		//else
		//	return CString(true);
	}
	// ���@���h���N���X	
	else if (pEnt->isKindOf(AcDbDimension::desc())) {
		//return CString(CompareDimensionGeoms(pEnt, pEnt2)); //####
		//return CString(false); //###
		// 2LineAngularDimension
		if (pEnt->isA()->isEqualTo(AcDb2LineAngularDimension::desc()) == Adesk::kTrue)
			return CString(_T("�p�x���@"));
		// 3PointAngularDimension
		else if (pEnt->isA()->isEqualTo(AcDb3PointAngularDimension::desc()) == Adesk::kTrue)
			return CString(_T("3�_�p�x���@"));
		// AlignedDimension
		else if (pEnt->isA()->isEqualTo(AcDbAlignedDimension::desc()) == Adesk::kTrue)
			return CString(_T("���s���@"));	
		// DiametricDimension
		else if (pEnt->isA()->isEqualTo(AcDbDiametricDimension::desc()) == Adesk::kTrue)
			return CString(_T("���a���@"));
		// OrdinateDimension
		else if (pEnt->isA()->isEqualTo(AcDbOrdinateDimension::desc()) == Adesk::kTrue)
			return CString(_T("���W���@"));
		// RadialDimension
		else if (pEnt->isA()->isEqualTo(AcDbRadialDimension::desc()) == Adesk::kTrue)
			return CString(_T("���a���@"));
		// RotatedDimension
		else if (pEnt->isA()->isEqualTo(AcDbRotatedDimension::desc()) == Adesk::kTrue)		
			return CString(_T("�������@"));
		// ���̑� ���@���h���N���X�H
		//else
		//	return CString(true);
	}
	// �u���b�N�Q��	
	else if (pEnt->isKindOf(AcDbBlockReference::desc())) {
		// �u���b�N�Q��	
		if (pEnt->isA()->isEqualTo(AcDbBlockReference::desc()) == Adesk::kTrue)
			return CString(_T("��ۯ��Q��"));
		// MInsertBlock?
		else if (pEnt->isA()->isEqualTo(AcDbMInsertBlock::desc()) == Adesk::kTrue)
			return CString(pEnt->isA()->name()); //CompareMInsertBlockGeoms(pEnt, pEnt2);
		// ���̑��u���b�N�Q�Ɣh���N���X�H
		//else
		//	return CString(true);
	}
	// Face // 3d�̓T�|�[�g���Ȃ��B
	else if (pEnt->isA()->isEqualTo(AcDbFace::desc()) == Adesk::kTrue) {
		return CString(_T("3D ��"));
	}
	// FaceRecord  // 3d�̓T�|�[�g���Ȃ��B
	else if (pEnt->isA()->isEqualTo(AcDbFaceRecord::desc()) == Adesk::kTrue) {
		return CString(pEnt->isA()->name());
	}
	// Fcf: �􉽌���
	else if (pEnt->isA()->isEqualTo(AcDbFcf::desc()) == Adesk::kTrue) {
		return CString(_T("�􉽌���"));
	}
	// Frame�h���N���X // // �T�|�[�g���Ȃ��B
	else if (pEnt->isKindOf(AcDbFrame::desc())) {
		// ??? Frame�͒��ۃN���X��? �� ���ۃN���X
		//if (pEnt->isA()->isEqualTo(AcDbFrame::desc()) == Adesk::kTrue) 
		//	return CString(CompareFrameGeoms(pEnt, pEnt2));		
		
		// OleFrame
		if (pEnt->isA()->isEqualTo(AcDbOleFrame::desc()) == Adesk::kTrue) 
			return CString(pEnt->isA()->name());
		// Ole2Frame
		else if (pEnt->isA()->isEqualTo(AcDbOle2Frame::desc()) == Adesk::kTrue) 
			return CString(_T("OLE"));
		// ���̑� Frame�h���N���X?
		//else
		//	return CString(false);
		
	}
	// �n�b�`���O
	else if (pEnt->isA()->isEqualTo(AcDbHatch::desc()) == Adesk::kTrue) {
		return CString(_T("ʯ�ݸ�"));
	}
	// Image�h���N���X
	else if (pEnt->isKindOf(AcDbImage::desc())) {
		// ??? Frame�͒��ۃN���X��? �� ���ۃN���X
		//if (pEnt->isA()->isEqualTo(AcDbImage::desc()) == Adesk::kTrue) 
		//	return CString(CompareImageGeoms(pEnt, pEnt2));

		// RasterImage
		if (pEnt->isA()->isEqualTo(AcDbRasterImage::desc()) == Adesk::kTrue) 
			return CString(_T("׽�� �Ұ��"));
		// ���̑� Image�h���N���X?
		//else
		//	return CString(false);
	}
	// Mline
	else if (pEnt->isA()->isEqualTo(AcDbMline::desc()) == Adesk::kTrue) {
		return CString(_T("���ײ�"));
	}	
	// MText
	else if (pEnt->isA()->isEqualTo(AcDbMText::desc()) == Adesk::kTrue) {
		return CString(_T("��� ÷��"));
	}
	// Point
	else if (pEnt->isA()->isEqualTo(AcDbPoint::desc()) == Adesk::kTrue) {
		return CString(_T("�_"));
	}
	// PolyFaceMesh // 3d�̓T�|�[�g���Ȃ��B
	else if (pEnt->isA()->isEqualTo(AcDbPolyFaceMesh::desc()) == Adesk::kTrue) {		
		return CString(pEnt->isA()->name());
	}
	// PolygonMesh
	else if (pEnt->isA()->isEqualTo(AcDbPolygonMesh::desc()) == Adesk::kTrue) {
		return CString(_T("��غ�� ү��"));
	}
	// ProxyEntity
	else if (pEnt->isA()->isEqualTo(AcDbProxyEntity::desc()) == Adesk::kTrue) {
		return CString(pEnt->isA()->name());
	}
	// Shape
	else if (pEnt->isA()->isEqualTo(AcDbShape::desc()) == Adesk::kTrue) {
		return CString(_T("�����"));
	}
	// Solid
	else if (pEnt->isA()->isEqualTo(AcDbSolid::desc()) == Adesk::kTrue) {
		return CString(_T("�د��"));
	}
	// Text�h���N���X
	else if (pEnt->isKindOf(AcDbText::desc())) {
		// �e�L�X�g
		if (pEnt->isA()->isEqualTo(AcDbText::desc()) == Adesk::kTrue) 
			return CString(_T("÷��"));
		// ����
		else if (pEnt->isA()->isEqualTo(AcDbAttribute::desc()) == Adesk::kTrue) 
			return CString(_T("����"));
		// ������`
		else if (pEnt->isA()->isEqualTo(AcDbAttributeDefinition::desc()) == Adesk::kTrue) 
			return CString(_T("����(��`)"));
		// ���̑� Text�h���N���X?
		//else
		//	return CString(false);
	}
	// Trace
	else if (pEnt->isA()->isEqualTo(AcDbTrace::desc()) == Adesk::kTrue) {
		return CString(_T("����"));
	}
	// Viewport
	else if (pEnt->isA()->isEqualTo(AcDbViewport::desc()) == Adesk::kTrue) {
		return CString(_T("�ޭ��߰�"));
	}
	// 3dSolid  // 3d�̓T�|�[�g���Ȃ�
	else if (pEnt->isA()->isEqualTo(AcDb3dSolid::desc()) == Adesk::kTrue) {
		return CString(_T("3D �د��"));
	}
	// Body // 3d�̓T�|�[�g���Ȃ�
	else if (pEnt->isA()->isEqualTo(AcDbBody::desc()) == Adesk::kTrue) {
		return CString(pEnt->isA()->name());
	}
	// Region
	else if (pEnt->isA()->isEqualTo(AcDbRegion::desc()) == Adesk::kTrue) {
		return CString(_T("ذ�ޮ�"));
	}
	// AcDbVertex �h���N���X
	else if (pEnt->isKindOf(AcDbVertex::desc())) {
		// AcDbVertex
		if (pEnt->isA()->isEqualTo(AcDbVertex::desc()) == Adesk::kTrue) 
			return CString(pEnt->isA()->name());
		// AcDb2dVertex
		else if (pEnt->isA()->isEqualTo(AcDb2dVertex::desc()) == Adesk::kTrue) 
			return CString(pEnt->isA()->name());
		// AcDb3dPolylineVertex
		else if (pEnt->isA()->isEqualTo(AcDb3dPolylineVertex::desc()) == Adesk::kTrue) 
			return CString(pEnt->isA()->name());
		// AcDbPolyFaceMeshVertex
		else if (pEnt->isA()->isEqualTo(AcDbPolyFaceMeshVertex::desc()) == Adesk::kTrue) 
			return CString(pEnt->isA()->name());
		// AcDbPolygonMeshVertex
		else if (pEnt->isA()->isEqualTo(AcDbPolygonMeshVertex::desc()) == Adesk::kTrue) 
			return CString(pEnt->isA()->name());
		//else 
		//	return CString(true);
	}
	// ���̑��}�`�@�J�X�^���I�u�W�F�N�g
	//else {
	//	return CString(true);
	//}
	
	return CString(pEnt->isA()->name());

}
