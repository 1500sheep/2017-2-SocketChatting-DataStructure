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
	//포인터 변수 이용해서 가변길이로 구조체 넘기기
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
	//로그인 실패시 
	void sendLoginFail();

	//데이터를 모든 유저에게 보내기
	void sendDataAll(const Data &data);

	//채팅 데이터 보내기
	void sendChat(Data temp_data,const string str);

	//동일 채팅 방에 있는 사람들에게 채팅 보내기
	void sendOneChat(Data temp_data,string str);

	//데이터 받기!
	void recvData(Data &data);

	//데이터 보내기!
	void sendData(SOCKET socket, const Data &data);

	DWORD run(void);

	void printLeaveUser(const User &user) const;

};

#endif