#pragma once

//	���W�X�g������N���X
class TCS_EXPORT_CLASS tcRegistry{
public:
	//	�R���X�g���N�^
	tcRegistry();

	//	�f�X�g���N�^
	virtual ~tcRegistry();

	//	���W�X�g���f�[�^���擾����
	virtual Tcs::Result ReadData( LPTSTR lpszValue, LPCTSTR lpszData, DWORD dwSize ) const;

	//	���W�X�g���f�[�^����������
	virtual Tcs::Result WriteData( LPCTSTR lpszData, LPCTSTR lpszKey ) const;


	//////////////////////////////////////////////
	//
	//	Get Set ����


	//	���W�X�g���L�[
	HKEY GetHKey() const;
	void SetHKey(HKEY value);
	
	//	���W�X�g���L�[
	LPCTSTR GetKey() const;
	void SetKey(LPCTSTR lpszKey);

protected:


private:

	HKEY m_hKey;

	//	�L�[
	TCHAR m_szKey[512];
};

//	���W�X�g���L�[
inline HKEY tcRegistry::GetHKey() const
{
	return ( m_hKey );
}

inline void tcRegistry::SetHKey(HKEY value)
{
	m_hKey = value;
}


//	���W�X�g���L�[
inline LPCTSTR tcRegistry::GetKey() const
{
	return ( m_szKey );
}

inline void tcRegistry::SetKey(LPCTSTR lpszKey)
{
	memset( m_szKey, 0, sizeof(m_szKey) );
	//2006.07.01
	_tcsncpy( m_szKey, lpszKey, sizeof(m_szKey)/sizeof(TCHAR)-1 );
}

