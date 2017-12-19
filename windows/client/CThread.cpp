
#include "CThread.h"

DWORD WINAPI CThread::StaticThreadStart( LPVOID lpParam ) {
	CThread* pThread = (CThread*)lpParam;
	return pThread->run();
}
//Thread를 만들기 위한 HANDLE의 함수, C/C++라이브러리를 위해선 _beginthreadex가 더 낫다?!
//이거 때문에 만든 함수인데 못써먹고 있음 _beginthreadex에서 안먹힘
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
	//Thread를 만들기 위한 HANDLE의 함수, C/C++라이브러리를 위해선 _beginthreadex가 더 낫다?!
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