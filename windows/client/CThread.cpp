
#include "CThread.h"

DWORD WINAPI CThread::StaticThreadStart( LPVOID lpParam ) {
	CThread* pThread = (CThread*)lpParam;
	return pThread->run();
}
//Thread�� ����� ���� HANDLE�� �Լ�, C/C++���̺귯���� ���ؼ� _beginthreadex�� �� ����?!
//�̰� ������ ���� �Լ��ε� ����԰� ���� _beginthreadex���� �ȸ���
unsigned int WINAPI CThread::ThreadFunction(LPVOID lpParam) {
	CThread* pThread = (CThread*)lpParam;
	return pThread->run();
}

bool CThread::start() {
	if( hThread ) {
		if( WaitForSingleObject( hThread, 0 ) == WAIT_TIMEOUT ) {
			return false;
		}
		CloseHandle( hThread );
	}
	//Thread�� ����� ���� HANDLE�� �Լ�, C/C++���̺귯���� ���ؼ� _beginthreadex�� �� ����?!
	/*
	hThread = (HANDLE)_beginthreadex(
	NULL, 
	0, 
	ThreadFunction,
	this, 
	0, 
	(unsigned*)&ThreadID
	);
	*/
	
	hThread = CreateThread(
		NULL,			
		0,				
		(LPTHREAD_START_ROUTINE)CThread::StaticThreadStart,
		this,
		0,
		&ThreadID
		);
	
	if( hThread != NULL ) return true;

	return false;
}

void CThread::join() {
	::WaitForSingleObject( hThread, INFINITE );
}

bool CThread::isRunning() {
	if( hThread ){
		DWORD dwExitCode = 0;
		::GetExitCodeThread( hThread, &dwExitCode );
		if( dwExitCode == STILL_ACTIVE ) return true;
	}
	return false;
}

DWORD CThread::getExitCode() {
	DWORD dwExitCode = 0;
	::GetExitCodeThread( hThread, &dwExitCode );
	return dwExitCode;
}