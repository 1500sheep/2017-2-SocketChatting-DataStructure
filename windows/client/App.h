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

	//클라이언트 임시 채팅 리스트
	static vector<string> chat_msg;

	//클라이언트 임시 친구 리스트
	static vector<string> temp_friendList;

};

#endif