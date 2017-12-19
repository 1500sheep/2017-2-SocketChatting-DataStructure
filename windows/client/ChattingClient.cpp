
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
	cout << "�� �� �� �� ���� �õ���..." << endl;
	if (connect(this->client_socket, (SOCKADDR*)&this->server_address, sizeof(this->server_address)) == SOCKET_ERROR){
		cout << "�� �� �� �� ���� ����" << endl;
		throw ChatException(1001);
	}
	cout << "�� �� �� ���� �����߽��ϴ�." << endl;
}

//Thread�� run�� ���ؼ� ��� �����Ѵ�
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


//Data�ޱ�
void SendRecvInterface::recvData(SOCKET socket, char *buf){
	Data data;
	string temp_str;
	memset(&data,0, sizeof(Data));
	if (recv(socket, (char*)&data, sizeof(Data), 0) <= 0){
		throw ChatException(1100);
	}
	//ģ�� ��� �ֱ�!
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
		cout << "\t***************ģ�� ���***************" << endl;
		cout <<"\t" <<data.data << endl;
		break;
	case RECOMMANDATION_FRIEND:
		cout << endl;
		cout <<"***************��õ ģ��***************"<<endl;
		cout << data.data << endl;
		break;
	case LIST_CHAT:
		cout << endl;
		cout << "\t***************ä�ù� LIST***************" << endl;
		cout << "\t" << data.data << endl;
		break;
	case CHECK_CHAT:
		cout << endl;
		temp_str = data.data;
		cout << "\t***************ä�ù� ����***************" << endl;
		cout << "\t�ο� : ";
		cout<<temp_str.substr(temp_str.find_first_of(" "));
		break;
	case FIRST_ON_CHAT:
		cout << endl;
		App::current_data.status = ON_CHAT;
		App::current_room = data.data;
		//���� ù��° �����Ͱ� ���̸���
		App::current_room = App::current_room.substr(0, App::current_room.find("\n"));
		cout << endl << endl;
		cout << "\t***************ä�ù� ����***************" << endl;
		cout << "ä�� �� �̸� : <"<< App::current_room <<">"<< endl;
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
//Data ������ �Ű����� command�� � ������ �� �� ���Ѵ�!
void SendRecvInterface::sendData(SOCKET socket){
	string menu_on;
	string menu_off;
	Sleep(100);
	cout << endl << endl;
	menu_off.append("\t---- - ��\t��\t ��\t ��---- -\n");
	menu_off.append("\t   1 : �α���\n");
	menu_off.append("\t   2 : ȸ�� ����\n");
	menu_off.append("\t   0 : ����\n");
	menu_off.append("\n\t�Է��ϼ���--> ");

	menu_on.append("\t---- - ��\t��\t ��\t ��---- -\n");
	menu_on.append("\t   1 : ģ�� �߰�\n");
	menu_on.append("\t   2 : ģ�� ���\n");
	menu_on.append("\t   3 : ģ�� �˻�\n");
	menu_on.append("\t   4 : ��õ ģ��\n");
	menu_on.append("\t   5 : ä�� ���\n");
	menu_on.append("\t   6 : ä�ù� �����\n");
	menu_on.append("\t   7 : ä�ÿ� ģ�� �ʴ�\n");
	menu_on.append("\t   8 : ä�ù� ����\n");
	menu_on.append("\t   9 : ä�ù� ����\n");
	menu_on.append("\t   10 : ������ ä��\n");
	menu_on.append("\t   0 : �α׾ƿ�\n");
	menu_on.append("\n\t�Է��ϼ���--> ");
	
	//�α��� �� ���� ��!
	int command;
	if (App::current_data.status == LOGIN_OFF) {
		cout << menu_off;
		cin >> command;
		cin.clear();
		cin.ignore();
		//�Ű����� command�� � ������ ������ �� �������ش�!
		switch (command)
		{
			//�α��� �� ��!
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
			cout << "\t\t�߸� �Է��ϼ̽��ϴ�" << endl;
			break;
		}
	}
	else if (App::current_data.status==ON_CHAT) {
		cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tä�ù濡�� �����ðڽ��ϱ�? [Y : 1 OR N : 0] ";
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
	//�α��� �� �Ŀ�!
	else {
		cout << menu_on;
		cin >> command;
		cin.clear();
		cin.ignore();
		switch (command)
		{
		//ģ�� �߰�
		case 1:
			sendADD_FRIEND(socket);
			break;
		//ģ�� ���
		case 2:
			sendLIST_FRIEND(socket);
			break;
		//ģ�� �˻�
		case 3:
			retrieveFriend();
			break;
		//ģ�� ��õ
		case 4:
			sendRecommand_FRIEND(socket);
			break;
		//ä�� ���
		case 5:
			sendLIST_CHAT(socket);
			break;
		//ä�ù� �����
		case 6:
			sendADD_CHAT(socket);
			break;
		//ä�ÿ� ģ�� �ʴ��ϱ� 
		case 7:
			sendINVITE_CHAT(socket);
			break;
		//ä�ù� ����
		case 8:
			sendINFO_CHAT(socket);
			break;
		//ä�ù� ����
		case 9:
			sendIN_CHAT(socket);
			break;
		//������ ä��!
		case 10:
			sendMESSAGE_ME(socket);
			break;
		case 0:
			App::current_data.status = LOGIN_OFF;
			App::temp_friendList.clear();//�ӽ� ģ�� ����Ʈ ����
			cout << "\t\t�α׾ƿ� �Ͽ����ϴ�" << endl;
			break;
		default:
			break;
		}
	
		
	
	}
}
void SendRecvInterface::sendRecommand_FRIEND(SOCKET socket) {
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

								   //LOGIN ��ȣ ������!
	data.type = RECOMMANDATION_FRIEND;

	//nick�� ���� �̸�
	strcpy(data.nick, App::current_data.nick);

	strcpy(data.data, "");

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}
//�α��� ������ �Լ�
void SendRecvInterface::sendSIGN_IN(SOCKET socket) {
	//�α��� �����͸� ������!
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	//LOGIN ��ȣ ������!
	data.type = SIGN_IN;
	
	//nick�� ���� �̸�
	char suBuffer[30];
	cout << "ȸ�� �̸��� �Է��ϼ��� : ";
	cin.getline(suBuffer, 30);

	strcpy(data.nick, suBuffer);

	strcpy(data.data, "");

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}
//�α��� ���� �޴� �Լ�
void SendRecvInterface::recvSIGN_IN(SOCKET socket) {

}

//ȸ������ ������ �Լ�
void SendRecvInterface::sendSIGN_UP(SOCKET socket) {
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = SIGN_UP;

	//nick�� ���� �̸�
	char suBuffer[30];
	cout << "ȸ�� �̸��� �Է��ϼ��� : ";
	cin.getline(suBuffer, 30);

	strcpy(data.nick, suBuffer);

	//���� ȸ������ �ϸ� �ڱ� �̸��� ������ �� �̸� �����.
	strcpy(data.data, data.nick);

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
	
}


//ȸ������ ���� �޴� �Լ�
void SendRecvInterface::recvSIGN_UP(SOCKET socket) {

}

void SendRecvInterface::sendLIST_FRIEND(SOCKET socket) {
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = LIST_FRIEND;

	strcpy(data.nick, App::current_data.nick);

	//���� ȸ������ �ϸ� �ڱ� �̸��� ������ �� �̸� �����.
	strcpy(data.data,"");

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}



void SendRecvInterface::sendADD_FRIEND(SOCKET socket)
{
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = ADD_FRIEND;

	//nick�� ���� �̸�
	char suBuffer[30];
	cout << "ģ�� �̸��� �Է��ϼ��� : ";
	cin.getline(suBuffer, 30);
	

	strcpy(data.nick, App::current_data.nick);

	//���� ȸ������ �ϸ� �ڱ� �̸��� ������ �� �̸� �����.
	strcpy(data.data,suBuffer);

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}
void SendRecvInterface::recvADD_FRIEND(SOCKET socket)
{

}
void SendRecvInterface::sendLIST_CHAT(SOCKET socket)
{
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = LIST_CHAT;

	strcpy(data.nick, App::current_data.nick);

	strcpy(data.data, "");

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendADD_CHAT(SOCKET socket)
{
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = ADD_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string add_chat;
	
	cout << "ä�ù���� ���ּ��� : ";
	getline(cin, add_chat);
	
	strcpy(data.data, add_chat.c_str());

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendINVITE_CHAT(SOCKET socket) {
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = INVITE_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string add_chat;
	string add_friend;
	cout << "ä�ù���� ���ּ��� : ";
	getline(cin,add_chat);

	cout << "�ʴ��� ģ���� ���ּ��� : ";
	getline(cin,add_friend);
	add_chat.append(" "+add_friend);
	strcpy(data.data, add_chat.c_str());

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendINFO_CHAT(SOCKET socket)
{
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = CHECK_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string check_chat;
	cout << "ä�ù���� ���ּ��� : ";
	getline(cin,check_chat);

	strcpy(data.data, check_chat.c_str());

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendIN_CHAT(SOCKET socket) {
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = IN_CHAT;

	strcpy(data.nick, App::current_data.nick);

	string in_chat;
	
	cout << "���� �� ä�ù���� ���ּ��� : ";
	getline(cin,in_chat);

	strcpy(data.data, in_chat.c_str());

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}
void SendRecvInterface::sendOUT_CHAT(SOCKET socket) {
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = OUT_CHAT;

	strcpy(data.nick, App::current_data.nick);
	strcpy(data.data, "ä�ù� �����ϴ�");

	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendMESSAGE(SOCKET socket) {
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

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
	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}

void SendRecvInterface::sendMESSAGE_ME(SOCKET socket) {
	//������ ���� ������ 
	Data data;
	memset(&data, 0, sizeof(Data));//data ���� default?!

	data.type = IN_CHAT;

	strcpy(data.nick, App::current_data.nick);

	strcpy(data.data, App::current_data.nick);

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	WaitForSingleObject(App::hMutex, INFINITE);

	if (send(socket, (const char*)&data, sizeof(Data), 0) <= 0) {
		ReleaseMutex(App::hMutex);
		throw ChatException(1100);
	}

	//����ȭ �κ��� ���� �ʿ��� �κ�, �ڼ��Ѱ� ��
	ReleaseMutex(App::hMutex);
}

//ģ�� �˻�
void SendRecvInterface::retrieveFriend() {
	int count = 0;
	string str;
	cout << "\t�˻��� ģ�� �̸��� �ۼ��ϼ��� : ";
	getline(cin, str);
	cout << "\t******�˻� ģ��******" << endl;
	cout << "\t";
	for (int i = 0; i < App::temp_friendList.size(); i++) {
		if (App::temp_friendList[i].find(str) <100) {
			cout << App::temp_friendList[i] << " ";
			count++;
		}
	}
		cout << endl;
		cout << "\t�� �˻� ģ�� : " << count << "�� �Դϴ�." << endl;
		cout << endl;
}

//ģ�� ����
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



//�� �𸣴� �Լ���1
void SendRecvInterface::gotoxy(int x, int y) {
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
//�� �𸣴� �Լ���2
SendThread::SendThread(SOCKET cs) : client_socket(cs) {

}
//�� �𸣴� �Լ���3
bool SendThread::exitUser(const char *buf) {
	if (stricmp(buf, UserCommand::EXIT) == 0){
		return true;
	}
	return false;
}
//�� �𸣴� �Լ���4
void SendThread::printcin(const char*) {
	gotoxy(1, 1);
}

//�� �𸣴� �Լ���5
RecvThread::RecvThread(SOCKET cs) : client_socket(cs) {

}



