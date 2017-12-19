#pragma once

#ifndef __EXCEPTION_CONF__
#define __EXCEPTION_CONF__

#include <iostream>

using std::cerr;
using std::endl;

class ChatException {
private:
	int code;

public:
	ChatException(int code) : code(code) {};

	int getCode() const { return code; }
	void printError() {
		switch (code){
		case 1000:
			cerr << "���� ���� ���� (�����ڵ�:" << code << ")" << endl;
			break;
		case 1001:
			cerr << "���� ���ε� ���� (�����ڵ�:" << code << ")" << endl;
			break;
		case 1002:
			cerr << "���� ������ ���� (�����ڵ�:" << code << ")" << endl;
			break;
		case 1100:
			cerr << "���� ���� ���� (�����ڵ�:" << code << ")" << endl;
			break;
		case 1200:
			cerr << "�α��� ���� (�����ڵ�:" << code << ")" << endl;
			break;
		case 1201:
			cerr << "������ ���� (�����ڵ�:" << code << ")" << endl;
			break;
		}
	}


};

#endif