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
	static const int MAXUSER; // 10��
	ifstream m_InFile;
	ofstream m_Outfile;
public:
	App();

	void start();
	
	void printNewUser(const User *) const;
	
	void printExceedUser(const User *) const;

	//process������ UserList
	static HANDLE hMutex;

	//����� BST�� ǥ��!
	static BinarySearchTree<string> nickList;

	//ģ�� ���踦 �׷����� ǥ��
	static Graph<string>friendList;


	//Map�� ���� �� �̸��� �� �� room�� user List�� ���� �� �ִ�
	static map<string, vector<User*> > room_map;

	static map<string, vector<string>> chatList;

	static map<string, vector<string>> chat_room;

	static vector<User*> allUser;

	void ReadDataFromFile();
	void WriteDataFromFile();
};

#endif