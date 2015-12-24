#include "stdafx.h"
#include "resource.h"
#include "zfcCommand.h"

zfcCommand::zfcCommand(void)
{
}

zfcCommand::~zfcCommand(void)
{
}

//	�R�}���h�����s����
bool zfcCommand::execute( zfcCommand& cmd )
{
	bool bResult = false;

	try{
		bResult = cmd.execute();
	}
	catch(...){
		assert( false );
		AfxMessageBox( IDS_UNKNOWN_ERROR, MB_OK | MB_ICONEXCLAMATION );
	};

	return bResult;
}

