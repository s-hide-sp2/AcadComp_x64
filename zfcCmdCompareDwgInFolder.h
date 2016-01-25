#pragma once
#include "zfccommand.h"

class zfcCmdCompareDwgInFolder : public zfcCommand
{
public:
	zfcCmdCompareDwgInFolder(void);
	virtual ~zfcCmdCompareDwgInFolder(void);

	//	�}�ʔ�r�����s����
	virtual bool execute();

	/////////////////////////////////////////////////
	//
	//	Get/Set

	void setFolderOldDwg(const CString& value );
	const CString& folderOldDwg() const;

	void setFolderNewDwg( const CString& value );
	const CString& folderNewDwg() const;

	void setfolderOutput(const CString& value);
	const CString& folderOutput() const;

	BOOL doesCompareSubFolder() const;
	void setCompareSubFolder(BOOL value);

protected:
	//	�e��t�H���_��I������
	bool selectFolder();

	//	�t�H���_���̐}�ʃt�@�C�����擾
	void getDwgInFolder( zfc::pathContainer& conPath, const CString& strFolder, const CString& strParentFolder ) const;

	//	�t�@�C���p�X�����o
	bool findPath( CString& strTitle, CString& strPath, const CString& strFind, const zfc::pathContainer& conPath ) const;

	//	�}�ʂ��r����
	void compare( zfc::pathContainer::const_reference pairNew, const zfc::pathContainer& conPathOld );

	//	�����ςݐ}�ʂ�ǉ�
	void addProcessed( const CString& strTitle, const CString& strPath );
	
	//	�������}�ʂ�ǉ�
	void addUnProcessed( const CString& strTitle, const CString& strPath );

	//	�����ς݃t�H���_��ǉ�
	void addProcessedFolder( const CString& strKey, const CString& strValue );

	//	�������t�H���_��ǉ�
	void addUnProcessedFolder( const CString& strKey, const CString& strValue );

	// ���}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
	void writeLogOnlyExistInOldDwgFolder(zfc::pathContainer& conPathOld) const;

	// �V�}�ʃt�H���_�ɂ����Ȃ��t�@�C���������O�o��
	void writeLogOnlyExistInNewDwgFolder() const;

	// ��r���ʂ����O�o�͂���
	void writeLogResult() const;

	// ��r�����s����
	void compare(const CString& strFolderOldDwg, const CString& strFolderNewDwg);

	// �T�u�t�H���_�̔�r�����s����
	void compareSubFolder();

	//	�T�u�t�H���_���擾
	void getSubFolder( zfc::pathContainer& conPath, const CString& strFolder, const CString& strParentFolder ) const;

	//	�T�u�t�H���_���r����
	void compareSubFolder( zfc::pathContainer::const_reference pairFolderNew, const zfc::pathContainer& conFolderOld );

	// ���}�ʃt�H���_�ɂ����Ȃ��T�u�t�H���_�������O�o��
	void writeLogOnlyExistSubFolderInOld(zfc::pathContainer& conSubFolderOld) const;

	// �V�}�ʃt�H���_�ɂ����Ȃ��T�u�t�H���_�������O�o��
	void writeLogOnlyExistSubFolderInNew() const;

private:
	//	���}�ʊi�[�t�H���_
	static CString m_strFolderOldDwg;

	//	�V�}�ʊi�[�t�H���_
	static CString m_strFolderNewDwg;

	//	�o�̓t�H���_
	static CString m_strFolderOutput;

	// �T�u�t�H���_��r�t���O
	static BOOL m_bCompareSubFolder;

	//	�����σt�@�C��
	zfc::pathContainer m_conProcessed;

	//	�������t�@�C��
	zfc::pathContainer m_conUnProcessed;

	//	�����σt�H���_(key=���΃t�H���_�p�X, value=�t�H���_�p�X)
	zfc::pathContainer m_conProcessedFolder;

	//	�������t�H���_(key=���΃t�H���_�p�X, value=�t�H���_�p�X)
	zfc::pathContainer m_conUnProcessedFolder;

	//	���}�ʃt�@�C���p�X
	zfc::pathContainer m_conPathOld;
	
	//	�V�}�ʃt�@�C���p�X
	//zfc::pathContainer m_conPathNew;
};

inline void zfcCmdCompareDwgInFolder::setFolderOldDwg(const CString& value )
{
	m_strFolderOldDwg = value;
}

inline const CString& zfcCmdCompareDwgInFolder::folderOldDwg() const
{
	return m_strFolderOldDwg;
}

inline void zfcCmdCompareDwgInFolder::setFolderNewDwg( const CString& value )
{
	m_strFolderNewDwg = value;
}

inline const CString& zfcCmdCompareDwgInFolder::folderNewDwg() const
{
	return m_strFolderNewDwg;
}

inline void zfcCmdCompareDwgInFolder::setfolderOutput(const CString& value)
{
	m_strFolderOutput = value;
}

inline const CString& zfcCmdCompareDwgInFolder::folderOutput() const
{
	return m_strFolderOutput;
}

inline void zfcCmdCompareDwgInFolder::addProcessed( const CString& strTitle, const CString& strPath )
{
	auto itp = m_conProcessed.insert( zfc::pathContainer::value_type( strTitle, strPath ) );
	assert( itp.second );
}

inline void zfcCmdCompareDwgInFolder::addUnProcessed( const CString& strTitle, const CString& strPath )
{
	auto itp = m_conUnProcessed.insert( zfc::pathContainer::value_type( strTitle, strPath ) );
	assert( itp.second );
}

//	�����ς݃t�H���_��ǉ�
inline void zfcCmdCompareDwgInFolder::addProcessedFolder( const CString& strKey, const CString& strValue )
{
	auto itp = m_conProcessedFolder.insert( zfc::pathContainer::value_type( strKey, strValue ) );
	assert( itp.second );
}

//	�������t�H���_��ǉ�
inline void zfcCmdCompareDwgInFolder::addUnProcessedFolder( const CString& strKey, const CString& strValue )
{
	auto itp = m_conUnProcessedFolder.insert( zfc::pathContainer::value_type( strKey, strValue ) );
	assert( itp.second );
}

inline BOOL zfcCmdCompareDwgInFolder::doesCompareSubFolder() const
{
	return m_bCompareSubFolder;
}

inline void zfcCmdCompareDwgInFolder::setCompareSubFolder(BOOL value)
{
	m_bCompareSubFolder = value;
}


