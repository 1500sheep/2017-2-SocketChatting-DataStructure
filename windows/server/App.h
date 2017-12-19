#pragma once

#ifndef __APP_CONF__
#define __APP_CONF__
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include "ChattingServer.h"
#include "User.h"
#include "CThread.h"
#include "ChatException.h"
#include <Windows.h>
#include "LinkedList.h"
#include "BinarySearchTree.h"
#include "Graph.h"
using namespace std;


class App {
private:
	ChattingServer chattingserver;
	static const int MAXUSER; // 10명
	ifstream m_InFile;
	ofstream m_Outfile;
public:
	App();

	void start();
	
	void printNewUser(const User *) const;
	
	void printExceedUser(const User *) const;

	//process단위의 UserList
	static HANDLE hMutex;

	//멤버를 BST로 표현!
	static BinarySearchTree<string> nickList;

	//친구 관계를 그래프로 표현
	static Graph<string>friendList;


	//Map을 통해 방 이름과 해 당 room에 user List를 넣을 수 있다
	static map<string, vector<User*> > room_map;

	static map<string, vector<string>> chatList;

	static map<string, vector<string>> chat_room;

	static vector<User*> allUser;

	void ReadDataFromFile();
	void WriteDataFromFile();
};

#endif