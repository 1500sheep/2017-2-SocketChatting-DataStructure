#pragma once

#ifndef __APP_CONF__
#define __APP_CONF__

#include <iostream>
#include "ChattingClient.h"
#include <Windows.h>
#include <vector>
#include <map>
#include "QueueType.h"

using namespace std;

class App {
private:
	char serverip[16];
	int serverport;
	ChattingClient *chattingclient;
	
public:
	App();
	~App();
	static Data current_data;
	static string current_room;
	void start();
	void printStart();
	void printExit();
	void printError();

	
	static HANDLE hMutex;

	//Ŭ���̾�Ʈ �ӽ� ä�� ����Ʈ
	static vector<string> chat_msg;

	//Ŭ���̾�Ʈ �ӽ� ģ�� ����Ʈ
	static vector<string> temp_friendList;

};

#endif