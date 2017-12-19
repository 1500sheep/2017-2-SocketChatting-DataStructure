
#include "ChattingClient.h"
#include "App.h"

const int ChattingClient::MAXSTRLEN = 256;

ChattingClient::ChattingClient(const char *ip, int port) {
	c_com = 0;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->client_socket == INVALID_SOCKET){
		throw ChatException(1000);
		WSACleanup();
	}

	memset(&this->server_address, 0, sizeof(this->server_address));
	this->server_address.sin_addr.S_un.S_addr = inet_addr(ip);
	this->server_address.sin_port = htons(port);
	this->server_address.sin_family = AF_INET;
}

ChattingClient::~ChattingClient() {
	closesocket(this->client_socket);
	delete this->st;
	delete this->rt;

	WSACleanup();
}

void ChattingClient::connectServer() {
	cout << "¯— ¯— ž± ·Œ Á¢¼Ó ½ÃµµÁß..." << endl;
	if (connect(this->client_socket, (SOCKADDR*)&this->server_address, sizeof(this->server_address)) == SOCKET_ERROR){
		cout << "¯— ¯— ž± ·Œ Á¢¼Ó ½ÇÆÐ" << endl;
		throw ChatException(1001);
	}
	cout << "¯— ¯— ž± ·Œ¿¡ Á¢¼ÓÇß½À´Ï´Ù." << endl;
}

//Thread´Â runÀ» ÅëÇØ¼­ °è¼Ó µ¿ÀÛÇÑ´Ù
int ChattingClient::run() {
	this->st = new SendThread(this->client_socket);
	this->rt = new RecvThread(this->client_socket);
	connectServer();
	
	st->start();
	rt->start();
	st->join();
	rt->join();
	return st->getExitCode();
}


//Data¹Þ±â
void SendRecvInterface::recvData(SOCKET socket, char *buf){
	Data data;
	string temp_str;
	memset(&data,0, sizeof(Data));
	if (recv(socket, (char*)&data, sizeof(Data), 0) <= 0){
		throw ChatException(1100);
	}
	//Ä£±¸ ¸ñ·Ï ³Ö±â!
	if (data.type == LIST_FRIEND) {
		App::temp_friendList.clear();
		temp_str = data.data;
		char sep = ' ';
		istringstream iss(temp_str);
		while (getline(iss, temp_str, sep)) {
			App::temp_friendList.push_back(temp_str);
		}
	}
	temp_str = "";
	
	if (data.type == FIRST_ON_CHAT) {
		temp_str = data.data;
		char sep = '\n';
		istringstream iss(temp_str);
		while (getline(iss, temp_str, sep)) {
			App::chat_msg.push_back(temp_str);
		}
	}
	switch (data.type)
	{
	case LOGIN_SUCCESS:
		App::current_data=data;
		break;
	case LOGIN_FAIL:
		cout << data.data << endl;
		break;
	case SIGN_UP_SUCCESS:
		cout << data.data << endl;
		break;
	case SIGN_UP_FAIL:
		cout << data.data << endl;
		break;
	case ADD_FRIEND_FAIL:
		cout << data.data << endl;
	case LIST_FRIEND:
		cout << "\t***************Ä£±¸ ¸ñ·Ï***************" << endl;
		cout <<"\t" <<data.data << endl;
		break;
	case RECOMMANDATION_FRIEND:
		cout << endl;
		cout <<"***************ÃßÃµ Ä£±¸***************"<<endl;
		cout << data.data << endl;
		break;
	case LIST_CHAT:
		cout << endl;
		cout << "\t***************Ã¤ÆÃ¹æ LIST***************" << endl;
		cout << "\t" << data.data << endl;
		break;
	case CHECK_CHAT:
		cout << endl;
		temp_str = data.data;
		cout << "\t***************Ã¤ÆÃ¹æ Á¤º¸***************" << endl;
		cout << "\tÀÎ¿ø : ";
		cout<<temp_str.substr(temp_str.find_first_of(" "));
		break;
	case FIRST_ON_CHAT:
		cout << endl;
		App::current_data.status = ON_CHAT;
		App::current_room = data.data;
		//ÃÖÃÊ Ã¹¹øÂ° µ¥ÀÌÅÍ°¡ ¹æÀÌ¸§ÀÓ
		App::current_room = App::current_room.substr(0, App::current_room.find("\n"));
		cout << endl << endl;
		cout << "\t***************Ã¤ÆÃ¹æ ½ÃÀÛ***************" << endl;
		cout << "Ã¤ÆÃ ¹æ ÀÌ¸§ : <"<< App::current_room <<">"<< endl;
		cout << "\t" << data.data << endl;
		
		Sleep(500);
		break;
	case ON_CHAT:
		cout << endl;
		cout << data.data << endl;
	default:
		break;
	}
	
}
//Data º¸³»±â ¸Å°³º¯¼ö command·Î ¾î¶² µ¿ÀÛÀ» ÇÒ Áö Á¤ÇÑ´Ù!
void SendRecvInterface::sendData(SOCKET socket){
	string menu_on;
	string menu_off;
	Sleep(100);
	cout << endl << endl;
	menu_off.append("\t---- - ¯—\t¯—\t ž±\t ·Œ---- -\n");
	menu_off.append("\t   1 : ·Î±×ÀÎ\n");
	menu_off.append("\t   2 : È¸¿ø °¡ÀÔ\n");
	menu_off.append("\t   0 : Á¾·á\n");
	menu_off.append("\n\tÀÔ·ÂÇÏ¼¼¿ä--> ");

	menu_on.append("\t---- - ¯—\t¯—\t ž±\t ·Œ---- -\n");
	menu_on.append("\t   1 : Ä£±¸ Ãß°¡\n");
	menu_on.append("\t   2 : Ä£±¸ ¸ñ·Ï\n");
	menu_on.append("\t   3 : Ä£±¸ °Ë»ö\n");
	menu_on.append("\t   4 : ÃßÃµ Ä£±¸\n");
	menu_on.append("\t   5 : Ã¤ÆÃ ¸ñ·Ï\n");
	menu_on.append("\t   6 : Ã¤ÆÃ¹æ ¸¸µé±â\n");
	menu_on.append("\t   7 : Ã¤ÆÃ¿¡ Ä£±¸ ÃÊ´ë\n");
	menu_on.append("\t   8 : Ã¤ÆÃ¹æ Á¤º¸\n");
	menu_on.append("\t   9 : Ã¤ÆÃ¹æ ÀÔÀå\n");
	menu_on.append("\t   10 : ³ª¿¡°Ô Ã¤ÆÃ\n");
	menu_on.append("\t   0 : ·Î±×¾Æ¿ô\n");
	menu_on.append("\n\tÀÔ·ÂÇÏ¼¼¿ä--> ");
	
	//·Î±×ÀÎ ¾È ÇßÀ» ¶§!
	int command;
	if (App::current_data.status == LOGIN_OFF) {
		cout << menu_off;
		cin >> command;
		cin.clear();
		cin.ignore();
		//¸Å°³º¯¼ö command·Î ¾î¶² µ¥ÀÌÅÍ º¸³»ÁÙ Áö ±¸ºÐÇØÁØ´Ù!
		switch (command)
		{
			//·Î±×ÀÎ ÇÒ ¶§!
		case 1:
			sendSIGN_IN(socket);
			break;
		case 2:
			sendSIGN_UP(socket);
			break;
		case 0:
			exit(1);
			return;
		default:
			cout << "\t\tÀß¸ø ÀÔ·ÂÇÏ¼Ì½À´Ï´Ù" << endl;
			break;
		}
	}
	else if (App::current_data.status==ON_CHAT) {
		cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tÃ¤ÆÃ¹æ¿¡¼­ ³ª°¡½Ã°Ú½À´Ï±î? [Y : 1 OR N : 0] ";
		cin >> command;
		cin.clear();
		cin.ignore();
		switch (command)
		{
		case 1:
			App::current_data.status = LOGIN_ON;
			App::chat_msg.clear();
			sendOUT_CHAT(socket);
			break;
		case 0:
			sendMESSAGE(socket);
			break;
		default:
			break;
		}
	}
	//·Î±×ÀÎ ÇÑ ÈÄ¿¡!
	else {
		cout << menu_on;
		cin >> command;
		cin.clear();
		cin.ignore();
		switch (command)
		{
		//Ä£±¸ Ãß°¡
		case 1:
			sendADD_FRIEND(socket);
			break;
		//Ä£±¸ ¸ñ·Ï
		case 2:
			sendLIST_FRIEND(socket);
			break;
		//Ä£±¸ °Ë»ö
		case 3:
			retrieveFriend();
			break;
		//Ä£±¸ ÃßÃµ
		case 4:
			sendRecommand_FRIEND(socket);
			break;
		//Ã¤ÆÃ ¸ñ·Ï
		case 5:
			sendLIST_CHAT(socket);
			break;
		//Ã¤ÆÃ¹æ ¸¸µé±â
		case 6:
			sendADD_CHAT(socket);
			break;
		//Ã¤ÆÃ¿¡ Ä£±¸ ÃÊ´ëÇÏ±â 
		case 7:
			sendINVITE_CHAT(socket);
			break;
		//Ã¤ÆÃ¹æ Á¤º¸
		case 8:
			sendINFO_CHAT(socket);
			break;
		//Ã¤ÆÃ¹æ ÀÔÀå
		case 9:
			sendIN_CHAT(socket);
			break;
		//³ª¿¡°Ô Ã¤ÆÃ!
		case 10:
			sendMESSAGE_ME(socket);
			break;
		case 0:
			App::current_data.status = LOGIN_OFF;
			App::temp_friendList.clear();//ÀÓ½Ã Ä£±¸ ¸®½ºÆ® »èÁ¦
			cout << "\t\t·Î±×¾Æ¿ô ÇÏ¿´½À´Ï´Ù" << endl;
			break;
		default:
			break;
		}
	
		
	
	}
}
void SendRecvInterface::sendRecommand_FRIEND(SOCKET socket) {
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

								   //LOGIN ½ÅÈ£ º¸³½´Ù!
	data.type = RECOMMANDATION_FRIEND;

	//nick¿¡ ³ÖÀ» ÀÌ¸§
	strcpy(data.nick, App::current_data.nick);

	strcpy(data.data, "");

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}
//·Î±×ÀÎ º¸³»´Â ÇÔ¼ö
void SendRecvInterface::sendSIGN_IN(SOCKET socket) {
	//·Î±×ÀÎ µ¥ÀÌÅÍ¸¦ º¸³½´Ù!
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	//LOGIN ½ÅÈ£ º¸³½´Ù!
	data.type = SIGN_IN;
	
	//nick¿¡ ³ÖÀ» ÀÌ¸§
	char suBuffer[30];
	cout << "È¸¿ø ÀÌ¸§À» ÀÔ·ÂÇÏ¼¼¿ä : ";
	cin.getline(suBuffer, 30);

	strcpy(data.nick, suBuffer);

	strcpy(data.data, "");

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}
//·Î±×ÀÎ ÀÀ´ä ¹Þ´Â ÇÔ¼ö
void SendRecvInterface::recvSIGN_IN(SOCKET socket) {

}

//È¸¿ø°¡ÀÔ º¸³»´Â ÇÔ¼ö
void SendRecvInterface::sendSIGN_UP(SOCKET socket) {
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = SIGN_UP;

	//nick¿¡ ³ÖÀ» ÀÌ¸§
	char suBuffer[30];
	cout << "È¸¿ø ÀÌ¸§À» ÀÔ·ÂÇÏ¼¼¿ä : ";
	cin.getline(suBuffer, 30);

	strcpy(data.nick, suBuffer);

	//ÃÖÃÊ È¸¿ø°¡ÀÔ ÇÏ¸é ÀÚ±â ÀÌ¸§°ú µ¿ÀÏÇÑ ¹æ ÀÌ¸§ »ý±ä´Ù.
	strcpy(data.data, data.nick);

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
	
}


//È¸¿ø°¡ÀÔ ÀÀ´ä ¹Þ´Â ÇÔ¼ö
void SendRecvInterface::recvSIGN_UP(SOCKET socket) {

}

void SendRecvInterface::sendLIST_FRIEND(SOCKET socket) {
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = LIST_FRIEND;

	strcpy(data.nick, App::current_data.nick);

	//ÃÖÃÊ È¸¿ø°¡ÀÔ ÇÏ¸é ÀÚ±â ÀÌ¸§°ú µ¿ÀÏÇÑ ¹æ ÀÌ¸§ »ý±ä´Ù.
	strcpy(data.data,"");

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}



void SendRecvInterface::sendADD_FRIEND(SOCKET socket)
{
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = ADD_FRIEND;

	//nick¿¡ ³ÖÀ» ÀÌ¸§
	char suBuffer[30];
	cout << "Ä£±¸ ÀÌ¸§À» ÀÔ·ÂÇÏ¼¼¿ä : ";
	cin.getline(suBuffer, 30);
	

	strcpy(data.nick, App::current_data.nick);

	//ÃÖÃÊ È¸¿ø°¡ÀÔ ÇÏ¸é ÀÚ±â ÀÌ¸§°ú µ¿ÀÏÇÑ ¹æ ÀÌ¸§ »ý±ä´Ù.
	strcpy(data.data,suBuffer);

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}
void SendRecvInterface::recvADD_FRIEND(SOCKET socket)
{

}
void SendRecvInterface::sendLIST_CHAT(SOCKET socket)
{
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = LIST_CHAT;

	strcpy(data.nick, App::current_data.nick);

	strcpy(data.data, "");

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendADD_CHAT(SOCKET socket)
{
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = ADD_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string add_chat;
	
	cout << "Ã¤ÆÃ¹æ¸íÀ» ½áÁÖ¼¼¿ä : ";
	getline(cin, add_chat);
	
	strcpy(data.data, add_chat.c_str());

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendINVITE_CHAT(SOCKET socket) {
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = INVITE_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string add_chat;
	string add_friend;
	cout << "Ã¤ÆÃ¹æ¸íÀ» ½áÁÖ¼¼¿ä : ";
	getline(cin,add_chat);

	cout << "ÃÊ´ëÇÒ Ä£±¸¸¦ ½áÁÖ¼¼¿ä : ";
	getline(cin,add_friend);
	add_chat.append(" "+add_friend);
	strcpy(data.data, add_chat.c_str());

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendINFO_CHAT(SOCKET socket)
{
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = CHECK_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string check_chat;
	cout << "Ã¤ÆÃ¹æ¸íÀ» ½áÁÖ¼¼¿ä : ";
	getline(cin,check_chat);

	strcpy(data.data, check_chat.c_str());

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendIN_CHAT(SOCKET socket) {
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = IN_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string in_chat;
	
	cout << "ÀÔÀå ÇÒ Ã¤ÆÃ¹æ¸íÀ» ½áÁÖ¼¼¿ä : ";
	getline(cin,in_chat);

	strcpy(data.data, in_chat.c_str());

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}
void SendRecvInterface::sendOUT_CHAT(SOCKET socket) {
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = OUT_CHAT;

	strcpy(data.nick, App::current_data.nick);
	strcpy(data.data, "Ã¤ÆÃ¹æ ³ª°©´Ï´Ù");

	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendMESSAGE(SOCKET socket) {
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = ON_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string add_chat;
	cout << ": ";
	getline(cin,add_chat);
	string chat = App::current_data.nick;
	chat.insert(0,"(");
	chat.append(")");
	chat.append(" : " + add_chat);
	strcpy(data.data, chat.c_str());
	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendMESSAGE_ME(SOCKET socket) {
	//º¸³»±â À§ÇÑ µ¥ÀÌÅÍ 
	Data data;
	memset(&data, 0, sizeof(Data));//data º¯¼ö default?!

	data.type = IN_CHAT;

	strcpy(data.nick, App::current_data.nick);

	strcpy(data.data, App::current_data.nick);

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//µ¿±âÈ­ ºÎºÐÀ» À§ÇØ ÇÊ¿äÇÑ ºÎºÐ, ÀÚ¼¼ÇÑ°Ç ¸ð¸§
	ReleaseMutex(App::hMutex);
}

//Ä£±¸ °Ë»ö
void SendRecvInterface::retrieveFriend() {
	int count = 0;
	string str;
	cout << "\t°Ë»öÇÒ Ä£±¸ ÀÌ¸§À» ÀÛ¼ºÇÏ¼¼¿ä : ";
	getline(cin, str);
	cout << "\t******°Ë»ö Ä£±¸******" << endl;
	cout << "\t";
	for (int i = 0; i < App::temp_friendList.size(); i++) {
		if (App::temp_friendList[i].find(str) <100) {
			cout << App::temp_friendList[i] << " ";
			count++;
		}
	}
		cout << endl;
		cout << "\tÃÑ °Ë»ö Ä£±¸ : " << count << "¸í ÀÔ´Ï´Ù." << endl;
		cout << endl;
}

//Ä£±¸ »èÁ¦
void SendRecvInterface::deleteFriend() {

}

DWORD SendThread::run(void) {
	int result = -1;
	int command;
	char buf[ChattingClient::MAXSTRLEN];
	
	while (true){
		try {			
				sendData(this->client_socket);

		} catch (ChatException e){
			closesocket(this->client_socket);
			break;
		}
	}
	return result;
}

DWORD RecvThread::run(void) {
	int commandR = 0;
	char buf[ChattingClient::MAXSTRLEN];
	while (true) {
		
		try {
			recvData(this->client_socket, buf);

		}
		catch (ChatException e) {
			closesocket(this->client_socket);
			break;
		}
	}
	return 0;
}



//Àß ¸ð¸£´Â ÇÔ¼öµé1
void SendRecvInterface::gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
//Àß ¸ð¸£´Â ÇÔ¼öµé2
SendThread::SendThread(SOCKET cs) : client_socket(cs) {

}
//Àß ¸ð¸£´Â ÇÔ¼öµé3
bool SendThread::exitUser(const char *buf) {
	if (stricmp(buf, UserCommand::EXIT) == 0){
		return true;
	}
	return false;
}
//Àß ¸ð¸£´Â ÇÔ¼öµé4
void SendThread::printcin(const char*) {
	gotoxy(1, 1);
}

//Àß ¸ð¸£´Â ÇÔ¼öµé5
RecvThread::RecvThread(SOCKET cs) : client_socket(cs) {

}



