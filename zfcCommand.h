#pragma once

/**
 * �}�ʔ�r�R�}���h���N���X
 * 
 */
class zfcCommand
{
public:
	zfcCommand(void);
	virtual ~zfcCommand(void);
	
	//	�R�}���h�����s����
	static bool execute( zfcCommand& cmd );

protected:

	//	�R�}���h�����s����
	virtual bool execute() = 0;
};

