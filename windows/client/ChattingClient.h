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

	//Data 보내고 받는 함수
	void sendData(SOCKET socket);
	void recvData(SOCKET socket, char *buf);

	//SIGN_IN 보내고 받는 함수
	void sendSIGN_IN(SOCKET socket);
	void recvSIGN_IN(SOCKET socket);

	//SIGN_UP 보내고 받는 함수
	void sendSIGN_UP(SOCKET socket);
	void recvSIGN_UP(SOCKET socket);

	//ADD_FRIEND 보내고 받는 함수
	void sendADD_FRIEND(SOCKET socket);
	void recvADD_FRIEND(SOCKET socket);

	void sendLIST_FRIEND(SOCKET socket);

	//추천 친구 보내는 함수
	void sendRecommand_FRIEND(SOCKET socket);

	//채팅 리스트 보내기!
	void sendLIST_CHAT(SOCKET socket); 

	//채팅 추가하기!
	void sendADD_CHAT(SOCKET socket);

	//채팅에 친구 추가하기
	void sendINVITE_CHAT(SOCKET socket);

	//채팅방 정보 가져오기
	void sendINFO_CHAT(SOCKET socket);
	
	//채팅방에 입장하기
	void sendIN_CHAT(SOCKET socket);
	
	//채팅방에 메세지 보내기
	void sendMESSAGE(SOCKET socket);

	//나에게 메세지 보내기
	void sendMESSAGE_ME(SOCKET socket);

	void sendOUT_CHAT(SOCKET socket);
	
	//친구 검색
	void retrieveFriend();

	//친구 삭제
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