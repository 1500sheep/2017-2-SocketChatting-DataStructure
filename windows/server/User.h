#pragma once

#ifndef __USER_CONF__
#define __USER_CONF__

#include <iostream>
#include <vector>
#include <sstream>
#include <WinSock2.h>
#include "ChatException.h"
#include "CThread.h"
#include <fstream>
#include "LinkedList.h"

using namespace std;

class RecvThread;
class User;

enum {
	LOGIN_ON,
	LOGIN_OFF,

	LOGOUT,

	SIGN_UP,
	SIGN_IN,

	ADD_FRIEND,
	DELETE_FRIEND,
	LIST_FRIEND,
	RECOMMANDATION_FRIEND,

	JOIN_ROOM,
	DELETE_ROOM,

	LIST_CHAT,
	ADD_CHAT,
	INVITE_CHAT,
	CHECK_CHAT,
	ADD_MESSAGE,
	IN_CHAT,
	FIRST_ON_CHAT,
	ON_CHAT,
	OUT_CHAT,
	INVITE_CHAT_FAIL,

	LOGIN_SUCCESS,
	LOGIN_FAIL,

	SIGN_UP_SUCCESS,
	SIGN_UP_FAIL,

	ADD_FRIEND_SUCCESS,
	ADD_FRIEND_FAIL
};

typedef struct _Data{
	int status;
	int type;
	char nick[30];
	char data[256];
	//������ ���� �̿��ؼ� �������̷� ����ü �ѱ��
} Data;



class User : public CThread {
private:
	SOCKET client_socket;
	SOCKADDR_IN client_address;
	static const int MAXSTRLEN;

	Data current_data;
	string nick;
	string room;

public:
	
	User(SOCKET cs, SOCKADDR_IN ca);
	User(const User &user) {}
	void operator=(const User &user) {}
	~User();

	char* getIP() const;
	int getPort() const;
	SOCKET getSocket() const;
	void closeSession();
	void setNick(string n) { nick = n; }
	void setRoom(string r) { room = r; }

	string getNick() const { return nick; }
	string getRoom()const { return room; }

	string recvNick();
	string recvRoom();
	//�α��� ���н� 
	void sendLoginFail();

	//�����͸� ��� �������� ������
	void sendDataAll(const Data &data);

	//ä�� ������ ������
	void sendChat(Data temp_data,const string str);

	//���� ä�� �濡 �ִ� ����鿡�� ä�� ������
	void sendOneChat(Data temp_data,string str);

	//������ �ޱ�!
	void recvData(Data &data);

	//������ ������!
	void sendData(SOCKET socket, const Data &data);

	DWORD run(void);

	void printLeaveUser(const User &user) const;

};

#endif