
#include "App.h"

HANDLE App::hMutex = CreateMutex(NULL, FALSE, NULL);
Data App::current_data;
string App::current_room;
vector<string> App::chat_msg;
vector<string> App::temp_friendList;

App::App() : serverport(0) {
	memset(serverip, 0, sizeof(serverip));
	App::current_data.status = LOGIN_OFF;
}

App::~App() {
	
	delete chattingclient;
}

void App::start() {
	int result = 0;

	printStart();
	chattingclient = new ChattingClient(this->serverip, this->serverport);

	try {
		result = chattingclient->run();

	} catch (ChatException e) {
		e.printError();
		return;
	}

	if (result == 0){
		printExit();
	} else {
		printError();
	}
}

void App::printStart() {
	char buf[16];
	int port;

	cout << "============< ¯— ¯— ž± ·Œ ÇÁ·Î±×·¥ >============" << endl;
	strcpy(buf, "127.0.0.1");
	port = 3490;

	sprintf(this->serverip, "%s", buf);
	this->serverport = port;
}

void App::printExit() {
	cout << "¼­¹öÀÇ ¿¬°áÀÌ Á¾·áµÆ½À´Ï´Ù." << endl;
}
void App::printError() {
	cout << "¼­¹öÀÇ ¿¬°áÀÌ ²÷°å½À´Ï´Ù." << endl;
}


//Send Member to Server
int sendMembertoServer()
{
	return 0;
}