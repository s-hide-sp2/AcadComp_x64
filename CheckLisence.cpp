// CheckLisence.cpp: CCheckLisence �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CheckLisence.h"
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CCheckLisence::CCheckLisence()
{

}

CCheckLisence::~CCheckLisence()
{

}

BOOL CCheckLisence::CheckUsableTerm(CString strAppName, CString strAcadVer)
{
	CString			strRegPath;
	CString			strCRC;
	CString			strBuf;
	CString			strVersion=_T(""), strOverride=_T(""), strTarFolder=_T(""),
					strInstallDate=_T(""), strLastAccess=_T(""),
					strUsableTerm=_T(""),  strCheckPrgName=_T(""),  strPrgCRC=_T("");
	unsigned short	crc;
	long			lRet;
	HKEY			hKey; 
	CString			strCheckFileName;
	BOOL			bRet = FALSE;

	//���W�X�g���p�X���擾����
	if (GetRegBase(strAcadVer, strRegPath)==FALSE){
//::MessageBox( adsw_acadMainWnd(), _T("strRegPath���擾�ł��܂���B"), _T("�G���["), MB_ICONINFORMATION );
		return FALSE;
	}
	strRegPath += "_"; 
	strRegPath += strAppName;
	lRet = RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKey);
	if (lRet == ERROR_SUCCESS){

		lRet = RegQueryValueAcad(hKey, ITM_VERSION,		  	strVersion);
		lRet = RegQueryValueAcad(hKey, ITM_OVERRIDE,	  	strOverride);
		lRet = RegQueryValueAcad(hKey, ITM_INSTALLFOLDER, 	strTarFolder);
		lRet = RegQueryValueAcad(hKey, ITM_INSTALLDATE,   	strInstallDate);
		lRet = RegQueryValueAcad(hKey, ITM_LASTACCESS,    	strLastAccess);
		lRet = RegQueryValueAcad(hKey, ITM_USABLETERM,  	strUsableTerm);
		lRet = RegQueryValueAcad(hKey, ITM_CHECKPRG,  		strCheckPrgName);
		lRet = RegQueryValueAcad(hKey, ITM_REVISION,  		strPrgCRC);
		lRet = RegQueryValueAcad(hKey, ITM_REVISIONSUB, 	strCRC);
		RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS || strCRC == _T("")){
//::MessageBox( adsw_acadMainWnd(), _T("���W�X�g�����擾�ł��܂���B"), _T("�G���["), MB_ICONINFORMATION );
			return FALSE;
		}
		strBuf = strVersion + strOverride + strTarFolder + strInstallDate + strLastAccess +
				 strUsableTerm + strCheckPrgName + strPrgCRC;
		GetCRCVal(strBuf, crc);
		strBuf.Format (_T("%d"), crc);
		if (strBuf != strCRC){
//::MessageBox( adsw_acadMainWnd(), _T("CRC����v���܂���B"), _T("�G���["), MB_ICONINFORMATION );
			return FALSE;
		}
		if (strCheckPrgName != _T("")){
			strCheckFileName = strTarFolder + _T("\\") + strCheckPrgName;
			if (GetCRCValFile(strCheckFileName, crc)){
				strBuf.Format(_T("%d"), crc);
				if (strBuf != strPrgCRC){
//::MessageBox( adsw_acadMainWnd(), _T("�`�F�b�N�v���O����CRC����v���܂���B"), _T("�G���["), MB_ICONINFORMATION );
					return FALSE;
				}
			}
			else {
//::MessageBox( adsw_acadMainWnd(), _T("�`�F�b�N�v���O����CRC���擾�ł��܂���B"), _T("�G���["), MB_ICONINFORMATION );
				return FALSE;
			}
		}
		bRet = FALSE;
		CTime timeCur = CTime::GetCurrentTime();	
		CTime timeInst( _ttoi(strInstallDate.Left(4)), _ttoi(strInstallDate.Mid(4,2)), _ttoi(strInstallDate.Mid(6,2)), 0,0,0  );
		CTime timeAccess( _ttoi(strLastAccess.Left(4)), _ttoi(strLastAccess.Mid(4,2)), _ttoi(strLastAccess.Mid(6,2)), 0,0,0  );
		//	�C���X�g�[�����t <= �X�V���t <= ���ݓ��t �`�F�b�N
		if( timeInst <= timeAccess && timeAccess <= timeCur ){
			CTimeSpan span = timeCur - timeInst;
			LONG lUseSpan = span.GetDays();
			
			//	�g�p���ԃ`�F�b�N
			if( _ttol(strUsableTerm) <= lUseSpan ){
//::MessageBox( adsw_acadMainWnd(), _T("���Ԃ��߂��Ă��܂�1�B"), _T("�G���["), MB_ICONINFORMATION );
				bRet = FALSE;
			}
			//�@�ŏI�A�N�Z�X�����X�V����
			else {
				long restDays = _ttol(strUsableTerm) - lUseSpan;
				CString strMsg;
				strMsg.Format (_T("�g�p�\�����F%d��  "),restDays);
				::MessageBox( adsw_acadMainWnd(), strMsg, _T("���m�点"), MB_ICONINFORMATION );
				bRet = TRUE;
			}
		}
		else {
//::MessageBox( adsw_acadMainWnd(), _T("���Ԃ��߂��Ă��܂�2�B"), _T("�G���["), MB_ICONINFORMATION );
			bRet = FALSE;
		}
		if (timeAccess < timeCur ){
			TCHAR timeBuf[9];
			GetDayString(timeBuf, timeCur);
			strLastAccess = timeBuf;
			strBuf = strVersion + strOverride + strTarFolder + strInstallDate + strLastAccess +
					 strUsableTerm + strCheckPrgName + strPrgCRC;
			GetCRCVal(strBuf, crc);
			strBuf.Format (_T("%d"), crc);
			lRet = RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_WRITE | KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hKey);
			if (lRet == ERROR_SUCCESS){
				lRet = RegSetValueAcad(hKey, ITM_LASTACCESS,   strLastAccess);
				lRet = RegSetValueAcad(hKey, ITM_REVISIONSUB,  strBuf);
				RegCloseKey(hKey);
			}
		}
		return bRet;
	}
	else {
//::MessageBox( adsw_acadMainWnd(), _T("�J�����g���[�U���J���܂���B"), _T("�G���["), MB_ICONINFORMATION );
		return FALSE;
	}
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//�A�v���P�[�V�����̃��W�X�g���t�H���_���擾����
// strCadVer    : AutoCAD�̃o�[�W���� 
// strRegPath	: �A�v���P�[�V�������Ǘ����郌�W�X�g���p�X�i�Ԃ��j
// �߂�l		: ���s�����ꍇ�̓G���[�R�[�h
BOOL CCheckLisence::GetRegBase(CString strCadVer, CString & strRegPath)
{
	strRegPath = _T("");
	if (strCadVer == VER_14)
		strRegPath = CAD_14REG;
	else if (strCadVer == VER_2000)
		strRegPath = CAD_2000REG;
	else if (strCadVer == VER_2004)
		strRegPath = CAD_2004REG;
	else if (strCadVer == VER_2005)
		strRegPath = CAD_2005REG;
	else if (strCadVer == VER_2006)
		strRegPath = CAD_2006REG;
	//2006.07.02
	else if (strCadVer == VER_2007)
		strRegPath = CAD_2007REG;
	//2007.05.08
	else if (strCadVer == VER_2008)
		strRegPath = CAD_2008REG;
	//2008.04.14
	else if (strCadVer == VER_2009)
		strRegPath = CAD_2009REG;
	else return FALSE;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// ���W�X�g������l���擾����
// hKey		�F���W�X�g���L�[
// strItem	�F���ږ�
// strStr	�F�l���擾����
// �߂�l	�F0=����  ���̑�=�G���[�R�[�h
long CCheckLisence::RegQueryValueAcad(HKEY hKey, CString strItem, CString &strStr)
{
	long lRet;
	DWORD dwDataSize, dwDataType;
	static TCHAR szStr[BUF_MAX + 1];

	dwDataSize = BUF_MAX;
	szStr[0]=0;
	lRet = RegQueryValueEx(hKey, strItem, NULL, &dwDataType, (LPBYTE)szStr, &dwDataSize);
	strStr = szStr;
	return lRet;
}


//	���t������(YYYYMMDD)���擾����
void CCheckLisence::GetDayString( 
	TCHAR * lpszString,	//(o)���t������
	const CTime& time	//(i)����
	)
{
	_stprintf( lpszString, _T("%04d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
}

/////////////////////////////////////////////////////////////////////////////
// ���W�X�g���ɒl���Z�b�g����
// hKey		�F���W�X�g���L�[
// strItem	�F���ږ�
// strStr	�F�l
// �߂�l	�F0=����  ���̑�=�G���[�R�[�h
long CCheckLisence::RegSetValueAcad(HKEY hKey, CString strItem,  CString strStr)
{
	long lRet;
	static TCHAR szStr[MAX_PATH * 4 + 1];

	if (strStr.GetLength () <= MAX_PATH*4){
		_tcscpy(szStr, strStr);
		lRet = RegSetValueEx(hKey, strItem,  NULL, REG_SZ, (LPBYTE)szStr, _tcslen(szStr)*sizeof(TCHAR));
		return lRet;
	}
	return ERROR_OUTOFMEMORY;
}



//////////////////////////////////////////////////////////////////////
// CRC���擾����
// strBuf	: CRC�𐶐�����o�b�t�@
// crc      : CRC�̒l
// �߂�l	: Err_Nothing=����	���̑�=���s
BOOL CCheckLisence::GetCRCVal(CString strBuf, unsigned short & crc)
{
	int l,i,j; 
	TCHAR ch;
	unsigned long	sum;
	unsigned long	polynomial = 	0x01102100;	//CCITT-16	

	sum = 0;

	l =  strBuf.GetLength();
	for (i=0; i<l; i++){
		ch = strBuf[i];
		sum |= (unsigned long)ch;
		for( j=0; j< 8; j++)
		{
			sum <<= 1;
			if( sum & 0x01000000 )
			sum ^= polynomial; 	
		}
	}
	for( i=0; i< 2; i++)	// last 2 byte zero
	{	
		sum |= 0x00;
		for( j=0; j< 8; j++)
		{
			sum <<= 1;
			if( sum & 0x01000000 )
			sum ^= polynomial; 	
		}
	}

	crc =(unsigned short) (( sum>>8 ) & 0x0000FFFF);

	return TRUE;

}

//////////////////////////////////////////////////////////////////////
// CRC���擾����
// strItem	: INI�t�@�C����
// crc      : CRC�̒l
// �߂�l	: Err_Nothing=����	���̑�=���s
BOOL CCheckLisence::GetCRCValFile(CString strInpFile, unsigned short & crc)
{
	int fh;
	int l,i,j; 
	char ch;
	unsigned long	sum;
	unsigned long	polynomial = 	0x01102100;	//CCITT-16	
	CFile cf;

	sum = 0;

	fh = _topen(strInpFile, O_RDONLY|O_BINARY);
	if (fh > 0){
		l =  _filelength(fh);
		for (i=0; i<l; i++){
			_read(fh, &ch, 1);
			sum |= (unsigned long)ch;
			for( j=0; j< 8; j++)
			{
				sum <<= 1;
				if( sum & 0x01000000 )
				sum ^= polynomial; 	
			}
		}
		for( i=0; i< 2; i++)	// last 2 byte zero
		{	
			sum |= 0x00;
			for( j=0; j< 8; j++)
			{
				sum <<= 1;
				if( sum & 0x01000000 )
				sum ^= polynomial; 	
			}
		}

		crc =(unsigned short) (( sum>>8 ) & 0x0000FFFF);
		_close(fh);
	}
	else {
		return FALSE;
	}
	

	return TRUE;

}
