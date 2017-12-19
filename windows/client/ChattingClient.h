#pragma once

#ifndef __CHATTINGCLIENT_CONF__
#define __CHATTINGCLIENT_CONF__

#include <iostream>
#include <cstring>
#include <cstdio>
#include <WinSock2.h>
#include "CThread.h"
#include "ChatException.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>


class ChattingClient;
class SendThread;
class RecvThread;
class SendRecvInterface;


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

namespace UserCommand {
	const char * const EXIT = "/exit";
};

typedef struct _DATA{
	int status;
	int type;
	char nick[30];
	char data[256];
} Data;


class ChattingClient {
private:
	SendThread *st;
	RecvThread *rt;
	SOCKET client_socket;
	SOCKADDR_IN server_address;
	int c_com = 0;
	void connectServer();

public:
	ChattingClient(const char *ip, int port);
	~ChattingClient();
	
	int run();

	static const int MAXSTRLEN;
};


class SendRecvInterface : public CThread {
public:
	virtual DWORD run(void)=0;

	//Data ������ �޴� �Լ�
	void sendData(SOCKET socket);
	void recvData(SOCKET socket, char *buf);

	//SIGN_IN ������ �޴� �Լ�
	void sendSIGN_IN(SOCKET socket);
	void recvSIGN_IN(SOCKET socket);

	//SIGN_UP ������ �޴� �Լ�
	void sendSIGN_UP(SOCKET socket);
	void recvSIGN_UP(SOCKET socket);

	//ADD_FRIEND ������ �޴� �Լ�
	void sendADD_FRIEND(SOCKET socket);
	void recvADD_FRIEND(SOCKET socket);

	void sendLIST_FRIEND(SOCKET socket);

	//��õ ģ�� ������ �Լ�
	void sendRecommand_FRIEND(SOCKET socket);

	//ä�� ����Ʈ ������!
	void sendLIST_CHAT(SOCKET socket); 

	//ä�� �߰��ϱ�!
	void sendADD_CHAT(SOCKET socket);

	//ä�ÿ� ģ�� �߰��ϱ�
	void sendINVITE_CHAT(SOCKET socket);

	//ä�ù� ���� ��������
	void sendINFO_CHAT(SOCKET socket);
	
	//ä�ù濡 �����ϱ�
	void sendIN_CHAT(SOCKET socket);
	
	//ä�ù濡 �޼��� ������
	void sendMESSAGE(SOCKET socket);

	//������ �޼��� ������
	void sendMESSAGE_ME(SOCKET socket);

	void sendOUT_CHAT(SOCKET socket);
	
	//ģ�� �˻�
	void retrieveFriend();

	//ģ�� ����
	void deleteFriend();

	void gotoxy(int x, int y);
};


class SendThread : public SendRecvInterface {
private:
	SOCKET client_socket;

public:
	SendThread(SOCKET cs);
	virtual DWORD run(void);

	bool exitUser(const char *buf);
	void printcin(const char*);
};

class RecvThread : public SendRecvInterface {
private:
	SOCKET client_socket;

public:
	RecvThread(SOCKET cs);
	virtual DWORD run(void);
	void printcout(const char*);
};

#endif